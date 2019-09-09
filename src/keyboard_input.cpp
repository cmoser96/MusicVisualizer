#include "../include/keyboard_input.h"


void processNormalKeys(unsigned char key, int x, int y){
    switch(key){
        case 27:
            glutReshapeWindow(1800,950);
            break;
        case 'f':
            glutFullScreenToggle();
            break;
        case 's':
            break;
        case 'x':
            glutLeaveMainLoop();
            break;
        default:
            if(key >= '0' && key <='9'){

            }
    }
}

void processSpecialKeys(int key, int x, int y){
    //TODO
}
