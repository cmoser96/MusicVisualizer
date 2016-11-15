#ifndef MAIN_H
#define MAIN_H
#include <fftw3.h>
#include "UdpServer.h"
#define BUFFER_SIZE 44100/240
#define excess 128

//udp server variables
char server[] = "0.0.0.0";
int port = 8000;
UdpServer *client;
char redbluegreen[4] = {char(0), char(0), char(0), char(0)};
char leave[4] = {char(0), char(0), char(0), char(0)};

//pulse variables
pa_simple *s;
pa_sample_spec ss;

//flag
volatile sig_atomic_t flag = 0;

//fft variables
int N = BUFFER_SIZE;
fftw_complex *in, *out;
fftw_plan p;
uint8_t buf[BUFFER_SIZE];

//functions
void stop(int sig);
void leaving();
void vu_loop();
void fourier_loop();
void vu(int level);

#endif