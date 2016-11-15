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

int main()
{
    // Initializeing variables
    signal(SIGINT, stop);

    // Initializing UDP server
    client = new UdpServer(server, port);
    client->setUp();

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
        client->sendMessage(leave);
        client->closeClient();
        pa_simple_free(s);
    }
}

void fourier_loop(){
    while(true){
        pa_simple_read(s, buf, BUFFER_SIZE, NULL);

        in = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * N);
        out = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * N);
        p = fftw_plan_dft_1d(N, in, out, FFTW_FORWARD, FFTW_ESTIMATE);

        fftw_execute(p);
        fftw_destroy_plan(p);
        
        if(flag){
            fftw_free(in);
            fftw_free(out);
            break;
        }
    }
}

void vu_loop(){
    int avg;
    int absolute;

    while(true){
        pa_simple_read(s, buf, BUFFER_SIZE, NULL);
        avg = 0;
        for(int i = 0; i < BUFFER_SIZE; i ++){
            absolute = abs(int(buf[i] - excess));
            avg = avg + absolute;
        }
        avg = avg/BUFFER_SIZE;
        printf("\n");
        printf("%d", absolute);

        vu(avg);
        if(flag){
            break;
        }
    }
}

void vu(int level){
    /*
    this function splits up the value and sends it to the server
    */
    if(level<64){
        redbluegreen[1] = char(level*4+3);
        redbluegreen[0] = char(255);
    }
    else{
        redbluegreen[1] = char(255);;
        redbluegreen[0] = char(abs(255 - level*2));
    }
    client->sendMessage(redbluegreen);
}