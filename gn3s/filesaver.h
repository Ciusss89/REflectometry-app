#ifndef FILESAVER_H_INCLUDED
#define FILESAVER_H_INCLUDED
#include <stdio.h>
#include <iostream>
#include <fcntl.h>
#include <fstream>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>


#define bmANT_SHORT	0x8000
#define bmANT_NC	0x4000
#define bmANT_OK	0x0000

//#define BUFFER_LEN  104
#define BUFFER_LEN  100*16384
#define bm4BIT_MODE 	0x2000
#define bmIF_MODE 	0x1000
#define bmAGC		0x8000
#define bmANT_DET	0x4000

using namespace std;
class FileSaver
{
private:
    char *data;
    char *iq_data;
    unsigned int ui_dec;
    char *name;
    int fid;
    int i_overflow;
    unsigned int ui_pack;
    char*  cp_buffer;
    unsigned int ui_buffer_len;
    unsigned int ui_buffer_pos;
    char LUT_2bit[4] ;
    char LUT_I_1bit[16];
    char LUT_Q_1bit[16];
    FILE *pfile;


public:
    FileSaver(void);
    FileSaver(char *fname);
    int open_file(char *name);
    int open_file_r(char *name);
    int close_file(void);
    void set_decimate(unsigned int dec);
    void set_fid(int i);
    void init_buffer(void);
    //int save_data(char *str, int nbytes, int mode);
    //int save_data(short *shp, int nbytes, int mode);
    int save_data(char *str, unsigned int nbytes, int  i_mode);
    int save_data(short *stri, unsigned int nbytes);
    int read_data(char *cp_str, int i_nbytes);
    int unpack_data(char *cp_instr, char *cp_outstr, int i_bytes);
    bool if_parse(char *buf, unsigned int N, char e_bits,int mode );
    bool if_parse(short *shp, unsigned int N, char e_bits);
    bool agc_parse(char *buf, short *agc, unsigned int agc_size, unsigned int *count, bool *RFI);
    void FS_rewind(void);
    void set_pack(unsigned int i);

};


#endif // FILESAVER_H_INCLUDED
