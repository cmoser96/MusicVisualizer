#include "keyboard_input.h"
#include <iostream>


void processNormalKeys(unsigned char key, int x, int y){
	switch(key){
		case 27:
			glutLeaveMainLoop();
			break;
		case 'f':
			glutFullScreenToggle();
			break;
		case 's':
			break;
	}
}

void processSpecialKeys(int key, int x, int y){

}