#include "filesaver.h"
using namespace std;

FileSaver::FileSaver() {}

FileSaver::FileSaver(char *fname)
{
    name = fname;
//    char LUT_2bit[4] = {1,-1,3,-3};
//    char LUT_I_1bit[16] = {1,-1,0,0,3,-3,0,0,0,0,0,0,0,0,0,0};
//    char LUT_Q_1bit[16] = {0,0,1,-1,0,0,0,0,3,-3,0,0,0,0,0,0};

}

int FileSaver::open_file(char* name)
//
{
    ui_buffer_len =0;
    ui_dec =1;
    ui_pack=1;
    pfile = fopen(name,"wb");
    i_overflow = 0;

    if (pfile != NULL)
        return(0);
    else
        printf("could not open file %s \n",name);
    return -1;
}

int FileSaver::open_file_r(char* name)
{
    ui_buffer_len =0;
    ui_dec =1;
    ui_pack=1;
    pfile = fopen(name,"rb");
    i_overflow = 0;

    if (pfile != NULL)
        return(0);
    else
        printf("could not open file %s \n",name);
    return -1;
}


int FileSaver::close_file(void)
{
    if (ui_buffer_len >0)
    {
        std::cout<<" wrote "<<ui_buffer_pos<<" bytes to the file" <<std::endl;

        fwrite(cp_buffer,sizeof(char),ui_buffer_pos,pfile);

    }

    //cout<<"closes file"<<endl;
    int i =fclose(pfile);
    if (i!=0)
        //printf("success !\n");
    //else
        printf("fail %d !\n", i);
    return(0);
}


void FileSaver::set_decimate(unsigned int i)
{
    ui_dec = i;
}

int FileSaver::save_data( char *str, unsigned int nbytes, int i_mode)
{
    if_parse(str,nbytes,0,i_mode);
    return(0);
}

int FileSaver::save_data(short *shp, unsigned int nbytes)
{
    if_parse(shp,nbytes,0);
    return(0);
}



/*int FileSaver::save_data( char *stri, unsigned int nbytes, int i_mode)
{
//TODO make this function generic
    return(fwrite(stri,sizeof(char),nbytes,pfile));
}

int FileSaver::save_data( short *stri, unsigned int nbytes, int i_mode)
{
    fwrite(stri,sizeof(short),nbytes,pfile);
    fflush(pfile);
    std::cout<<"wrote "<<nbytes<<" bytes"<<endl;
    return(1);
}
*/

int FileSaver::read_data(char *cp_str, int i_nbytes)
{
//TODO make this function generic
    int i=fread(cp_str,sizeof(char),i_nbytes,pfile);
    return(i);
}

int FileSaver::unpack_data(char *cp_instr, char *cp_outstr, int i_bytes)
{
    int i;
    char LUT_2bit[4] = {1,-1,3,-3};
    for (i = 0; i<i_bytes; i++)
    {
        cp_outstr[i*4] = LUT_2bit[((cp_instr[i])& 0x03)];
        cp_outstr[i*4+1] = LUT_2bit[((cp_instr[i]>>2)& 0x03)];
        cp_outstr[i*4+2] = LUT_2bit[((cp_instr[i]>>4)& 0x03)];
        cp_outstr[i*4+3] = LUT_2bit[((cp_instr[i]>>6)& 0x03)];
    }
    return 0;
}

void FileSaver::set_fid(int i)
{
    fid=i;
}

bool FileSaver::if_parse(char *buf, unsigned int N, char e_bits, int i_mode)
{
    unsigned int i = 0;
    // char *LUT;
    //char MASK;
    // 2bit SGN/MAG
    char LUT_1bit[2] = {1,-1};
    char LUT_2bit[4] = {1,-1,3,-3};
    char LUT_I_4bit[16] = {1,-1,0,0,3,-3,0,0,0,0,0,0,0,0,0,0};
    char LUT_Q_4bit[16] = {1,0,-1,0,0,0,0,0,3,0,-3,0,0,0,0,0};
    char LUT_I_2bit[4] = {1,-1,0,0};
    char LUT_Q_2bit[4] = {1,0,-1,0};
    char MASK_1bit = 0x01;
    char MASK_2bit = 0x03;

    char MASK_2bit_I = 0x01;
    char MASK_2bit_Q = 0x02;
    char MASK_4bit_I = 0x05;
    char MASK_4bit_Q = 0x0A;
    int i_overf_tmp = 0;
    int i_idx;
    int i_dtype = 0;
    char *iq_buf;
    //ui_pack =1;




    //LUT = LUT_2bit;
    //MASK = MASK_2bit;


    // dtermine  mode
    if (((i_mode% 100) ==31)|| ((i_mode% 100) ==37))
    {
        i_dtype =100;
    }
    if (((i_mode% 100) ==32)|| ((i_mode% 100) ==38))
    {
        i_dtype =2;
        //buf= (char*)malloc(2*N*sizeof(char));
    }

    if (((i_mode% 100) ==33)|| ((i_mode% 100) ==35)|| ((i_mode% 100) ==39)|| ((i_mode% 100) ==41))
    {
        i_dtype =3;
        iq_buf= (char*)malloc(2*N*sizeof(char));
    }
    if (((i_mode% 100) ==34)|| ((i_mode% 100) ==36)|| ((i_mode% 100) ==40)|| ((i_mode% 100) ==42))
    {
        i_dtype =4;
        iq_buf= (char*)malloc(2*N*sizeof(char));

    }





    i_idx=0;
    i = 0;


    while ((ui_dec*i+i_overflow) < N)
    {
        if(i_dtype ==4)
        {
            iq_buf[i_idx] = LUT_I_4bit[buf[i*ui_dec+i_overflow] & MASK_4bit_I];
            i_idx++;
            iq_buf[i_idx] = LUT_Q_4bit[buf[i*ui_dec+i_overflow] & MASK_4bit_Q];
            i++;
            i_idx++;
        }
        else if(i_dtype==3)
        {
            iq_buf[i_idx] = LUT_I_2bit[buf[i*ui_dec+i_overflow] & MASK_2bit_I];
            i_idx++;

            iq_buf[i_idx]= LUT_Q_2bit[buf[i*ui_dec+i_overflow] & MASK_2bit_Q];
            i_idx++;
            i++;
        }
        else if(i_dtype==2)
        {
            buf[i_idx] = LUT_2bit[buf[i*ui_dec+i_overflow] & MASK_2bit];
            i_idx++;
            i++;
        }
        else if(i_dtype==100)
        {
            // Use LUT to rearrange and interpret bits (D0-D3)
            if (ui_pack ==1)
            {
                buf[i_idx] = LUT_1bit[buf[i*ui_dec+i_overflow] & MASK_1bit];
                i_idx++;
                i++;
            }
            else if (ui_pack==2)
            {
                buf[i_idx]=((buf[i] & MASK_2bit)) |( (buf[i+1] & MASK_2bit)<<4);
                i_overf_tmp = i_overf_tmp-2*ui_dec;
                i_idx++;
                i +=2;
            }
            else if (ui_pack==4)
            {
                buf[i_idx] = buf[i*ui_dec+i_overflow] & MASK_2bit;
                i++;
                buf[i_idx] =buf[i_idx] | (buf[i*ui_dec+i_overflow] & MASK_2bit)<<2;
                i++;
                buf[i_idx] = buf[i_idx] | (buf[i*ui_dec+i_overflow] & MASK_2bit)<<4;
                i++;
                buf[i_idx] = buf[i_idx] | (buf[i*ui_dec+i_overflow] & MASK_2bit)<<6;
                i++;
                i_idx++;
            }
        }
    }

    i_overflow  = (ui_dec*i+i_overflow)-N;

    if(i_dtype<=2)
    {
        fwrite(buf,sizeof(char),i_idx,pfile);

    }
    else
    {
        fwrite(iq_buf,sizeof(char),i_idx,pfile);
    }


    return false;


}

bool FileSaver::if_parse(short *shp, unsigned int N, char e_bits)
{
    unsigned int i;
//   int i_overf_tmp = 0;
    int i_idx;
    i_idx=0;
    i = 0;

    while ((ui_dec*(i)+i_overflow) <N)
    {
        shp[i_idx] = shp[i*ui_dec+i_overflow] ;
        i_idx++;
        i++;
    }

    i_overflow  = (ui_dec*i+i_overflow)-N;
    fwrite(shp,sizeof(short),i_idx,pfile);
    //printf("AGC %f (v)\n",3.3*(shp[0]&0xfff)/4096);
    //free(iq_data);

    return false;
}


bool FileSaver::agc_parse(char *buf, short *agc, unsigned int agc_size,
                          unsigned int *count, bool *RFI)
{

    unsigned int i = 0;
    unsigned int j = 0;
    unsigned int e_headers = 0;
    short buf16;
    short  header2;//header,
    short  data2;//data,


// Parse AGC data
    if (agc_size>0)
    {
        for (i=0; i<agc_size; i+=4)
        {
            buf16 = buf[i+1] << 8;
            buf16 |= (buf[i] & 0xff);
            buf16 = buf[i+3] << 8;
            buf16 |= (buf[i+2] & 0xff);
            header2 = buf16 & 0xf000;
// Break the loop if only zeroes are read
            if ( data2>0)
            {
                if (header2 & bmAGC)
                {
// Save AGC data separately
                    agc[j] = data2;
// Decode ADC value
                   // agc_f = data2 * 3.3 / 4095;
                    //printf("agc_f = %f\n",agc_f);
// Clear channel bits
                    header2 = header2 & 0x3000;
                    j++;
                }

            }
        }
        *count = j;
    }
    if (e_headers > j/2)
        return true;
    else
        return false;
}

void FileSaver::FS_rewind(void)
{
    rewind(pfile);
}

void FileSaver::set_pack(unsigned int i)
{
    ui_pack =i;
}

void FileSaver::init_buffer(void)
{
    ui_buffer_len= BUFFER_LEN;
    cp_buffer = new char[ui_buffer_len];
    ui_buffer_pos=1;
}
