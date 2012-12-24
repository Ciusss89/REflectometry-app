CC=gcc -pipe -g -Wall -O2
PREFIX=/usr/local/
VIEWER_LIBS= -L/usr/X11/lib -lGL -lGLU -lglut -lfftw3


USER_PATH=/usr/local/bin

#all: 	fx2_programmer fx2_viewer
all: 	fx2_programmer
install: install_step 

fx2_programmer: fx2_programmer.c
	$(CC) fx2_programmer.c -lusb -o fx2_programmer

fx2_viewer: fx2.c fx2.h viewer.c 
	$(CC) fx2.c viewer.c $(VIEWER_LIBS) -lusb -o fx2_viewer
	

install_step: 
	install -s -m 0755 ./fx2_programmer ${PREFIX}/bin/
#	install -s -m 0755 ./fx2_viewer ${PREFIX}/bin/
	rm fx2_programmer
#	rm fx2_viewer

