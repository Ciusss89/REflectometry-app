#ifndef MAIN_H
#include <ostream>
#include <sstream>
#include <iostream>
//#include "cir_buf.h"
#include "filesaver.h"
#include "fx2.h"
//#include "if_collect.h"
#include <vector>
#include <stdio.h>
#include <string>
#include <iostream>
//#include <pthread.h>
#include <getopt.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


//#include <netdb.h>
#include <sys/types.h>
//#include <netinet/in.h>
//#include <sys/socket.h>
//#include <arpa/inet.h>
#include <sys/time.h>

//#include "linux.h"   // #include <windows.h>

#define PORT 1234 // the port client will be connecting to
#define DATA_PORT 1235 //The port where the client will send the data
//#define DEST_IP "127.0.0.1" //Localhost IP
//#define DEST_IP "128.138.94.237"
#define MAXDATASIZE 64000 // max number of bytes we can get at once

#define CIRCULAR_BUFFER 0x02
#define NETWORK 0x04
#define FILEBASED_BUFFER 0x08



int main(int argc ,char *argv[]);
int main_loop();
void help_lv(void);
void msg(void);
int chk_uid(void);
void get_if_data(Fx2_dev fx2, FileSaver fs);
void get_agc_data(Fx2_dev fx2, FileSaver fs);
#endif

/*
Notes to save

\e[37;44m'"\033[1mContact List\033[0m

*/
