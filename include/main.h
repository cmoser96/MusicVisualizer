#ifndef MUSICVISUALIZER_MAIN_H
#define MUSICVISUALIZER_MAIN_H

#include <pulse/pulseaudio.h>
#include <pulse/simple.h>

#define BUCKETS 10
#define SAMPLE_RATE 192000
#define FRAME_RATE 30
#define BUFFER_SIZE SAMPLE_RATE/FRAME_RATE
#define DOUBLE_BUFFER 2*BUFFER_SIZE

//Pulse
pa_simple *s;

//FFT
const int nc = (BUFFER_SIZE/2)+1;
double *in;
fftw_complex *out;
fftw_plan p;
uint8_t buf[BUFFER_SIZE];
uint8_t dub_buf[DOUBLE_BUFFER];
const int LOWER = 1;
const int UPPER = 1315;
float correct[UPPER];

//Functions
void mainLoop(int val);
void render();
void bucket();

#endif // MUSICVISUALIZER_MAIN_H