/*
Circular buffer:
*/
#ifndef CIR_BUF_H
#define CIR_BUF_H



#include <stdio.h>
#include <string.h>
#include <vector>
#include <math.h>
#include "filesaver.h"
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <fcntl.h>

class Circ_buf
{
private:
    char *pb_buf;
    char *pb_start;
    unsigned long long b_pos;
    unsigned long long b_sta; // where is zero?
    unsigned long long b_size;
    FILE *pfile;
    FileSaver fs;
    char *str_filename;

    short *pb_buf_s;
    short *pb_start_s;
    unsigned long long b_pos_s;
    unsigned long long b_sta_s; // where is zero?

    //unsigned long b_size_s;
    int b_dec;  //Should the data be decimated?
    int b_type; //What type is the data in the buffer?
    int b_save; //2 ==men 1==circ_buff
    int buf_id; //What is the id of the buffer? (to make it easier to have multiple
    int b_pac;  //Should the data be packed?
    int b_file;
    int b_mode; //is it real or IQ data?
public:
    Circ_buf(long long llength, int type);
    Circ_buf(void);
    void buff_delete(void);
    bool buff_get(char *new_loc); // Memcpy the data to a new location
    bool buff_get(char *new_loc, unsigned long long ul_get,unsigned long long ul_get_from);
    bool buff_get(short *new_loc);
    bool buff_get_dec(char *new_loc);

    bool buff_add(long length,char *new_data, int mode); //add data to the buffer
    bool buff_add(long length,short *new_data,int mode); //add data to the buffer
    unsigned long buff_size(void);
    int buff_print(void);
    void set_buf_id(int i, int fi);
    void set_b_dec(int i);
    void set_b_save(int i);
    int if_parse(char *buf, unsigned int N, int e_bits);
    void set_b_file(int i);
    void set_b_mode(int i);
    void set_b_filename(char *str);
    void save_b_toFile(char *str);

};

#endif
