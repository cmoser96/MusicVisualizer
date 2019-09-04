#ifndef MUSICVISUALIZER_MAIN_H
#define MUSICVISUALIZER_MAIN_H

#include <pulse/pulseaudio.h>
#include <pulse/simple.h>

#define BUCKETS 10
#define SAMPLE_RATE 192000
#define FRAME_RATE 30
#define BUFFER_SIZE SAMPLE_RATE/FRAME_RATE
#define DOUBLE_BUFFER 2*BUFFER_SIZE

//Opengl
const float r = .5f;
float avg;

//Pulse
pa_simple *s;
pa_sample_spec ss;

//FFT
const int N = BUFFER_SIZE;
const int nc = (N/2)+1;
double *in;
fftw_complex *out;
fftw_plan p;
uint8_t buf[BUFFER_SIZE];
const int LOWER = 0;
const int UPPER = 1400;

//Functions
void mainLoop(int val);
void renderBouncy();
void renderWaveform();
void renderFFT();
void bucket();

#endif // MUSICVISUALIZER_MAIN_H