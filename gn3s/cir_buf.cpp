#include "cir_buf.h"


using namespace std;
Circ_buf::Circ_buf(long long llength, int type)
{
    b_size = llength;
    b_type = type;
    cout<<"llength "<<llength<<endl;
}



void Circ_buf::buff_delete(void)
{
    // fclose(pfile);
//    if (b_type ==1&&b_file==0)
//        delete[] pb_buf;
    if (b_type ==2&&b_file==0)
        delete[] pb_buf_s;
}


unsigned long Circ_buf::buff_size()
{
    return(b_size);
}
bool Circ_buf::buff_add(long length,char *new_data,int mode)
//Add data to the buffer
//TODO make generic
{

    if_parse(new_data,length,mode);
    if (b_file ==0)
    {

        if ((unsigned long long)((b_pos +length)%b_size) >=(unsigned long long)length)
        {
            memcpy(pb_buf+(b_pos)%b_size, new_data,(int)(length) );
            //       std::cout<<"wrote "<<length <<"bytes to ram"<<endl;
        }
        else
        {
            memcpy(pb_buf+(b_pos)%b_size, new_data,(length -(b_pos+length)%b_size));
            memcpy(pb_buf,new_data+(length -(b_pos+length)%b_size),(b_pos+length)%b_size);
        }
    }
    else if (b_file ==1)
    {
        if ((unsigned long long)((b_pos +length)%b_size) >=(unsigned long long)length)
        {
            fwrite(new_data, sizeof(char),(length),pfile);
        }
        else
        {
            fwrite(new_data, sizeof(char),((length -(b_pos+length)%b_size)),pfile);
            rewind(pfile); //rewinds the file
            fwrite(new_data+((length-(b_pos+length)%b_size)),sizeof(char),(((b_pos+length)%b_size)),pfile);
        }
    }
    b_pos= b_pos +length;
    if (b_pos > (11*b_size))
        b_pos = b_pos -10*b_size; //prevents b_pos to get an overflow
    return(true);
}

bool Circ_buf::buff_add(long length_s,short *new_data_s,int mode)
//Add data to the buffer
//TODO make generic
{
    int i =0;
    while ((i) < length_s)
    {
        *(pb_buf_s+(b_pos_s%b_size)) =new_data_s[i];
        b_pos_s++;
        i++;
    };
    if ((b_pos_s >= b_size) && ( b_type == 2))
        return(false);
    if (b_pos_s > (11*b_size))
        b_pos_s = b_pos_s -10*b_size;


    return(true);
}


bool Circ_buf::buff_get(char *new_loc)  // Memcpy the data to a new location
{
    if (b_file ==0)
    {
        memcpy(new_loc, (pb_buf+(b_pos%b_size)),(b_size-(b_pos%b_size))*sizeof(char));
        memcpy(new_loc+(b_size-(b_pos%b_size)), pb_buf, ((b_pos-1)%b_size)*sizeof(char));
    }
    else if (b_file==1)
    {
        b_size=b_size>>8;
        if ((b_pos-b_size)<1)
        fseek(pfile,(b_pos%b_size)>>8,0);
        fread(new_loc,sizeof(char),((b_size-(b_pos%b_size))>>8),pfile);
        rewind(pfile);
        fread(new_loc+((b_size-(b_pos%b_size))>>8),sizeof(char),(((b_pos-1)%b_size)>>8),pfile);

    }
    return(true);
}

bool Circ_buf::buff_get(char *new_loc, unsigned long long ul_get,unsigned long long ul_get_from )
{
    if (ul_get>b_size)
        std::cout<<"you are to greedy, buffer not large enough"<<endl;

    if (b_file ==0)
    {
        if (((b_pos+ul_get+ul_get_from)%b_size) >= (ul_get+ul_get_from ))
        {
            memcpy(new_loc, (pb_buf+((b_pos+ul_get_from)%b_size)),(ul_get)*sizeof(char));
        }
        else
        {
            memcpy(new_loc, (pb_buf+((b_pos+ul_get_from)%b_size)),(ul_get-((ul_get+ul_get_from+b_pos)%b_size))*sizeof(char));
            memcpy(new_loc+(b_size-(b_pos%b_size+ul_get_from)), pb_buf, (((ul_get+ul_get_from+b_pos-1)%b_size))*sizeof(char));
        }
    }
    else if (b_file==1)
    {
        if ((b_pos-b_size)<1)
            std::cout<<"file not full"<<endl;

        if (ul_get>b_size)
            std::cout<<"you are to greedy, buffer not large enough"<<endl;

        if (((b_pos+ul_get+ul_get_from)%b_size) > (ul_get+ul_get_from ))
        {
            fseek(pfile,(((b_pos+ul_get_from)%b_size)>>8),0);
            fread(new_loc,sizeof(char),ul_get,pfile);

        }
        else
        {
            fseek(pfile,(((b_pos+ul_get_from)%b_size)>>8),0);
            fread(new_loc,sizeof(char),(ul_get-((ul_get+ul_get_from+b_pos)%b_size)),pfile);
            rewind(pfile);
            fread(new_loc+(b_size-((b_pos+ul_get_from)%b_size)),sizeof(char),(((ul_get+ul_get_from+b_pos-1)%b_size)),pfile);

        }

    }
    return(true);
}


bool Circ_buf::buff_get(short *new_loc_s)  // Memcpy the data to a new location
{
    memcpy(new_loc_s,(pb_buf_s+(b_pos_s%b_size)),(b_size-(b_pos_s%b_size))*sizeof(short));
    memcpy(new_loc_s+(b_size-(b_pos_s%b_size)), pb_buf_s, ((b_pos_s)%b_size)*sizeof(short));
    return(true);
}

int Circ_buf::buff_print(void)
{
    unsigned int k;
    for (k=0;k<Circ_buf::b_size;k++)
    {
        cout<<"content of the buf at pos" <<k<< "is " << *(pb_start+k)<<endl;
    }
    return 0;

}
void Circ_buf::set_buf_id(int i, int fi )
{
    b_file = fi;
    buf_id =i;
    b_mode=32; //Default mode

    if (b_type ==1 &&b_file==0)
    {
        pb_buf =new char[b_size];
        pb_start = pb_buf;
        b_sta=0;
        b_pos =0;
    }
    else if (b_type ==1 &&b_file ==1)
    {
        set_b_file(fi);
        b_pos = 0;
        b_sta = 0;
        pb_start = pb_buf;
        pfile = tmpfile();

        if (pfile == NULL)
	{
            printf("could not open file\n");
	}
        else
	{
            std::cout<<"opened cb file"<<endl;
	}
    }
    if (b_type ==2)
    {
        b_pos =0;
        pb_buf_s = new short[b_size];
        for (unsigned int k=0;k<b_size;k++)
        {
            pb_buf_s[k]=0;
        }
        pb_start_s = pb_buf_s;
        b_sta_s=0;
        b_pos_s =0;
        b_pos =0;
    }
    b_file= 0;
}


void Circ_buf::set_b_dec(int i)
{
    b_dec = i;
}
void Circ_buf::set_b_save(int i)
{
    b_save = i;
}

int Circ_buf::if_parse(char *buf, unsigned int N, int i_mode)
{
//TODO implement IQ parsing
	unsigned int i = 0;
    char *LUT;
    char MASK;
    int i_length;
    //  char LUT_I_1bit[16] = {1,-1,0,0,3,-3,0,0,0,0,0,0,0,0,0,0};
    // char LUT_Q_1bit[16] = {0,0,1,-1,0,0,0,0,3,-3,0,0,0,0,0,0};
    // 2bit SGN/MAG
    char LUT_2bit[4] = {1,-1,3,-3};
    char MASK_2bit = 0x03;
    i_mode= i_mode % 100;

    if ((i_mode == 31)||(i_mode == 32)||(i_mode == 37)||(i_mode == 38))
    {
        LUT = LUT_2bit;
        MASK = MASK_2bit;
        i_length = N;
    }
    else
    {
	    //other modes then real

    }

    // Store IF data as 8bit signed values
    if (!((b_dec==1)||(b_dec==2)||(b_dec==4)))
        fprintf(stderr,"wrong b_dec value! is %d but must be 1,2 or 4",b_dec);

    for (i=0; i<N; i++)
    {
        if (b_dec ==1)
        {
            // Use LUT to rearrange and interpret bits (D0-D3)
            buf[i] = LUT[buf[i] & MASK];
        }
        else if (b_dec==2)
        {
            buf[i]=LUT[buf[i] & MASK];
            buf[i] =(buf[i]<<4)|LUT[buf[i+1] & MASK];
            i++;
        }
        else if (b_dec==4)
        {
            buf[i]=LUT[buf[i] & MASK];
            buf[i] =(buf[i]<<2)|LUT[buf[i+1] & MASK];
            i++;
            buf[i] =(buf[i]<<2)|LUT[buf[i+1] & MASK];
            i++;
            buf[i] =(buf[i]<<2)|LUT[buf[i+1] & MASK];
            i++;
        }
    }
    return i_length;
}
void Circ_buf::set_b_file(int i)
{
    b_file =i;
    if (b_type ==1)
    {
        pfile = tmpfile();//fopen("temp_if.b01","wb");
        if (pfile == NULL)
	{
            printf("could not open file\n");
	}
        else
	{
            std::cout<<"opened cb file"<<endl;
	}
    }
    else if (b_type ==2)
    {
        pfile = fopen("temp_agc.b01","wrb");
        if (pfile == NULL)
	{
	     printf("could not open file\n");
	}
    }
    else
    {
        std::cerr<<"ERROR!! no file type specified" <<endl;
    }
}


void Circ_buf::set_b_mode(int i)
{
    b_mode = i;
}

void Circ_buf::set_b_filename(char *str)
{
    str_filename = str;
    fs = FileSaver(str);
}

void Circ_buf::save_b_toFile(char *str)
{
    FILE *fp;
    int i_length = 1000000;
    unsigned long long ul_fp=0;

    fp = fopen(str,"wb");
    ul_fp = 0;
    if (b_size < i_length)
        i_length = b_size;

    char *c_fp = new char[i_length];

    //  std::cout <<"save to b_file "<< b_size<< " "<< i_length << " " << ul_fp << " " << endl;
    while ((ul_fp) < b_size)
    {
        buff_get(c_fp,i_length,ul_fp);
        //     std::cout<<"save to b_file "<<i_length<< " "<<ul_fp<<" "<<std::endl;
        fwrite(c_fp,sizeof(char),i_length,fp);
        ul_fp= ul_fp+i_length;

    }
    ul_fp-=i_length;
    std::cout << " save b 2file "<< ul_fp<< " "<<  (b_size-ul_fp)<<endl;

    buff_get(c_fp,(b_size-ul_fp),ul_fp);
    fwrite(c_fp,sizeof(char),(b_size-ul_fp),fp);
    fclose(fp);
    delete[] c_fp;
    std::cout<<"save bfile DONE"<<endl;


}

