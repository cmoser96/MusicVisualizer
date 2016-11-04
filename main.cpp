/* Carl Moser and Ian Paul
*  Fall 2016 Music Visualizer
*/

//includes for printing/closing properly
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <signal.h>

//include for pulse
#include <pulse/simple.h>

//include for my own header file
#include "main.h"
#include "UdpServer.h"

//include fftw to do the fourier transform
#include <fftw3.h>

//defining variables
#define BUFFER_SIZE 44100/240 //This smaller buffer works better, effectively 240 updates/s
#define excess 128

//setting up global variables

//variable for closing things before quiting
volatile sig_atomic_t flag = 0;

//variables for Pulseaudio
pa_simple *s;
pa_sample_spec ss;

char server[] = "0.0.0.0";
int port = 8000;
UdpServer client(server, port);

//variables for the lights
char redbluegreen[3] = {char(0), char(0), char(0)};
char leave[3] = {char(0), char(0), char(0)};

int main()
{
    // Initializeing variables
    signal(SIGINT, stop);

    // Setting up Pulse
    ss.format = PA_SAMPLE_U8;
    ss.channels = 1;
    ss.rate = 44100;
    s = pa_simple_new(NULL,
                    "Peak",
                    PA_STREAM_RECORD,
                    "alsa_output.pci-0000_00_1b.0.analog-stereo.monitor",
                    "Recording",
                    &ss,
                    NULL,
                    NULL,
                    NULL
                    );

    // Function calls
    //fourier_loop();
    vu_loop();
    leaving();
    return 0;
}

void stop(int sig){
    //this function changes flag if ctrl+c is pressed
    flag = 1;
}

void leaving(){
    //this function closes all of the open connections
    if(s){
        client.sendMessage(leave);
        client.closeClient();
        pa_simple_free(s);
        std::cout << "disconnected" << std::endl;
    }
}

void vu_loop(){
    int avg;
    int absolute;

    client.setUp();

    while(true){
        uint8_t buf[BUFFER_SIZE];
        pa_simple_read(s, buf, sizeof(buf), NULL);
        avg = 0;
        for(int i = 0; i < sizeof(buf); i ++){
            absolute = abs(int(buf[i] - excess));
            //avg = avg+pow(absolute, 2);
            avg = avg + absolute;
        }
        //avg = sqrt(avg/sizeof(buf));
        avg = avg/sizeof(buf);
        printf("\n");
        printf("%d", absolute);

        //for more of a VU, send avg, for more rapid changes, send intermediate
        vu(avg);
        if(flag){
            break;
        }
    }
}

void print_loop(){
    int absolute;
    while(true){
        uint8_t buf[BUFFER_SIZE];
        pa_simple_read(s, buf, sizeof(buf), NULL);
        for(int i = 0; i < sizeof(buf); i ++){
            absolute = abs(int(buf[i] - excess));
            for(int d = 0; d < absolute; d++){
                printf(">");
            }
            printf("\n");
        }
        if(flag){
            break;
        }
    }
}

void fourier_loop(){
    while(true){
        uint8_t buf[BUFFER_SIZE];
        pa_simple_read(s, buf, sizeof(buf), NULL);
        int N = sizeof(buf);
        fftw_complex *in, *out;
        fftw_plan p;

        in = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * N);
        out = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * N);
        p = fftw_plan_dft_1d(N, in, out, FFTW_FORWARD, FFTW_ESTIMATE);

        fftw_execute(p); /* repeat as needed */

        fftw_destroy_plan(p);
        fftw_free(in);
        fftw_free(out);

        if(flag){
            break;
        }
    }
}

void vu(int level){
    /*this function splits up the value and sends it to the server

    RGB green = (0,255,0)
    RGB yellow = (255,255,0)
    RGB red = (255,0,0)
    */
    if(level<64){
        redbluegreen[1] = char(level*4+3);
        redbluegreen[0] = char(255);
    }
    else{
        redbluegreen[1] = char(255);;
        redbluegreen[0] = char(abs(255 - level*2));
    }
    client.sendMessage(redbluegreen);
}