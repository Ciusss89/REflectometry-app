 /** Gn3s V3 linux plataform, linux porting by Giuseppe Tipaldi.
     La revisione besente è basata sul codice sorgente rilasciato dagli sviluppatori.
     Questa revisione lavora solo con la versione harware 2 del frontend, pid 0b39
  */

 #include "main.h"

 #define ColMod 32                  //	Data collection mode, default value is 32

 using namespace std;

 // Entry of main function
 int main(int argc, char *argv[]) {
    char revision[]="r3.6\0";
 	bool b_overrun = false; 		//	buffer overrun flag
 	bool s40_limit = false;         //  Max time limit flag
 	char cp_filename[64];			//	data file name
 	char user[30];

 	unsigned char uc_flags[5];

 	/*  Control Variable */
 	int i_col_time = 0;

 	int i_ch = -1;					//	Command parameter buffer;
 	int i_decimate = 1;

 	/*  State variable */
 	unsigned int ui_pack = 1;		        //	Pack mode
 	unsigned int ui_time_to_collect = 2;    //	Data collection time
 	unsigned int ul_num_bytes_written = 0;

 	bool b_run = true;				//	Main loop control variable
 	unsigned int k = 0;				//	Main loop numbers

 	float ul_kmax;					//  Size of data buffered
 	timeval tv_max, tv_now, tv_start;

    FILE *fp=popen( "whoami", "r" );
    fscanf(fp,"%s",user); fclose(fp);
 	strcpy(cp_filename," ");

 	/*	Timer	*/
 	string  str;
 	/*	Device */
 	Fx2_dev	fx2_d = Fx2_dev(0);
    /************************* check sudo privileges**************************/
    printf("* Gn3s V3 linux platform REVISION %s:\n*",revision);
    if((strcmp(user,"root"))!=0){
        printf(" This applications need root privileges, Bye-bye. \n");
        exit(EXIT_FAILURE);
    }
    /************************* Look for a device*****************************/
    fx2_d = Fx2_dev(0);		//	Looking for device with PID 0b39
 	if (fx2_d.usb_fx2_find() != 0){
 		s40_limit = false;
 	}else{
 		fx2_d = Fx2_dev(1);	//	Looking for device with PID 0x0b3a
 		if (fx2_d.usb_fx2_find() != 0){
 			s40_limit = true;
 		}else{
            printf("\t no GNSS frontend found\n");
 			///return(0);
 			fx2_d.close();
 			}
 	}
    /************************ Display the help information according to PID of module **************/
    while ( (i_ch = getopt (argc,argv,"ho:s:")) != EOF ){
 			switch(i_ch){
 				case 'h':	// help
 					help_lv();
 					return 0;
 				case 's':	// size(sec)
 					sscanf(optarg, "%u", &ui_time_to_collect);
 					if(ui_time_to_collect>40 && s40_limit==true){
                        printf(" Second collect value must be lower than 40sec!");
                        help_lv();
                        return 1;
 					}
 					break;
 				case 'o':	// filename
 					strcpy(cp_filename, optarg);
	                break;
	             default:
	             	break;
 			}
    }
 	/***********************  Create init file *******************************************/
 	if (strlen(cp_filename)<2)
        strcpy(cp_filename, "test.bin");

    FileSaver fs = FileSaver(cp_filename);
    fs.open_file(cp_filename);
    fs.set_fid(1);
    fs.set_decimate(i_decimate);
    fs.set_pack(ui_pack);
    /*****************************INIT USB **********************************/
    fx2_d.usb_fx2_init();
    fx2_d.usrp_xfer (VRQ_AGC,0);
    fx2_d.usrp_xfer (VRQ_CMODE,132 );
    fx2_d.usrp_xfer (VRQ_XFER, 0 );

    fx2_d.usrp_xfer (VRQ_XFER, 1);
    fx2_d.usrp_xfer2(VRQ_FLAGS, 0, uc_flags, 5);
    fx2_d.usrp_xfer (VRQ_XFER, 0 );
    /******************************INIT Buffers ************************/
    short *shp_agc_data;
    char  *ch_if_data;


    fs.set_decimate(i_decimate);
    // Allocate buffer
    ch_if_data = new char[BUFSIZE];
    shp_agc_data = new short[100];

    // Initiate buffer
    for(int t = 0; t< 100; t++)
    	shp_agc_data[t] = 0;
    for(int t = 0; t<BUFSIZE; t++)
    	ch_if_data[t] = 0;
    // Set up the Timer

    tv_max.tv_sec = i_col_time;
    gettimeofday(&tv_start,NULL);


    fx2_d.usrp_xfer(VRQ_CMODE, ColMod);
    fx2_d.usrp_xfer(VRQ_XFER, 1);
    gettimeofday(&tv_max,NULL);

    gettimeofday(&tv_now,NULL);

    int j = 0;		// Buffer data collected in each loop
    int currK = 0;
    float percentage = 0.0;
    while(b_run){
    	// If is 100%
    	if ((tv_now.tv_sec+tv_now.tv_usec/1e6) - (tv_start.tv_sec+tv_start.tv_usec/1e6) >= ui_time_to_collect){
    		printf("%3.0f%% data collected",100.0);
    		b_run = false;
    	}
    	// Print initial info about size and time
    	if(k==0){
    		printf("\n");
    		ul_kmax = 16.368 * ui_time_to_collect/i_decimate;
            printf("Attemping to collect %dsecs/%.1fMB of data\n",ui_time_to_collect,ul_kmax);
            printf("%3.0f%% date collected\n",1.0);
    	}

    	k++;
    	//	Print progress information
    	if ((k%250)==0){
                if ((k%1000) == 0){
                    percentage = k/10/ui_time_to_collect;
                    (percentage>1)?printf("%3.0f%% date collected\n",percentage):printf("%3.0f%% date collected",1.0);
                }
    	}
    	//	Check buffer overrun
    	fx2_d.check_rx_overrun (&b_overrun );
    	if ( b_overrun==true ){
    		gettimeofday(&tv_now,NULL);
    		printf("Buffer Overrun!\n");
    		delete []ch_if_data;
            delete []shp_agc_data;

            fs.close_file();
            currK = int(k/1000);break;
    		printf("%dsec/%dMB data collected\n",currK,(int)(ul_num_bytes_written/1e6));

            fx2_d.close();
            fx2_d.~Fx2_dev();
            return(EXIT_FAILURE);
    	}

        //	Read data
        j = fx2_d.read_IF(ch_if_data);
        if ((j>0)){
            ul_num_bytes_written +=j;		//	Number of data collection update
            fs.save_data(ch_if_data,j,ColMod);	//	Write data buffered to disc
        }
        //	Get current time
        gettimeofday(&tv_now,NULL);
    }
    gettimeofday(&tv_now,NULL);
    printf("\n");
    // To modify
    std::cout<<"Total time = "<< (float)((tv_now.tv_sec+tv_now.tv_usec/1e6) - (tv_start.tv_sec+tv_start.tv_usec/1e6))+(tv_now.tv_usec-tv_start.tv_usec)/1e6<<"s"<<endl;
    std::cout<<"Data size = "<<ul_num_bytes_written/1e6<<"MB"<<std::endl;
    std::cout<<"Average speed = "<< (ul_num_bytes_written/((float)(tv_now.tv_sec - tv_start.tv_sec+(tv_now.tv_usec-tv_start.tv_usec)/1e6)))/1e6<<"MB/s"<<endl;
    gettimeofday(&tv_max,NULL);

    delete []shp_agc_data;
    delete []ch_if_data;

    fs.close_file();

    fx2_d.~Fx2_dev();
    fx2_d.close();
    fx2_d.~Fx2_dev();
    printf("success!\n");

    return 0;
 }

 /* Help screen */
 void help_lv(void){
    printf ( "\n");
    printf ( "*******************************************************************************\n" );
    printf ( "* -o filename  : output file name                                             *\n" );
    printf ( "* -s filesize  : Number of sec to collect                                     *\n" );
    printf ( "* -h           : help screen                                                  *\n" );
    printf ( "* Usage example :                                                             *\n" );
    printf ( "*   sudo ./gn3s -s 450 -o data.bin                                            *\n" );
    printf ( "* NOTE: for Gn3s hardware revision 2, pid (0b39), max sec to cllect is 40.    *\n" );
    printf ( "*******************************************************************************\n" );
    printf ( "\n" );
}
