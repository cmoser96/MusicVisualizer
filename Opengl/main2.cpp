#include <iostream>
#include <GL/freeglut.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <pulse/simple.h>
#include "main.h"
#define SAMPLE_RATE 192000
#define FRAME_RATE 30
#define BUFFER_SIZE SAMPLE_RATE/FRAME_RATE
#define DOUBLE_BUFFER 2*BUFFER_SIZE

int main(int argc, char* args[])
{
    ss.format = PA_SAMPLE_U8;
    ss.channels = 1;
    ss.rate = SAMPLE_RATE;
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

    glutInit(&argc, args);
    glutInitContextVersion(3,0);
    glutInitDisplayMode(GLUT_DOUBLE);
    glutInitWindowSize(1800,950);
    glutCreateWindow("Visualizer");

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glClearColor(0.f, 0.f, 0.f, 1.f);
    glutTimerFunc(1000/60, mainLoop, 0);

    glutMainLoop(); 
    return 0;
}

void mainLoop(int val){
    render();
    glutTimerFunc(1000/1800, mainLoop, val);
}

void render(){
    glClear(GL_COLOR_BUFFER_BIT);

    glBegin (GL_LINES);
    glColor3f(1.0f, 0.6f, 0.0f);
    glLineWidth(2.f);
    uint8_t buf[BUFFER_SIZE];
    pa_simple_read(s, buf, sizeof(buf), NULL);
    for(int i = 0; i < sizeof(buf); i ++){
        glVertex2f(-1.f + 2*float(i)/sizeof(buf), float(abs(int(buf[i] - 128)))/128);
    }
    glEnd();

    glutSwapBuffers();
}