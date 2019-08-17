#include "keyboard_input.h"
#include <iostream>


void processNormalKeys(unsigned char key, int x, int y){
	switch(key){
		case 27:
			exit(0);
			break;
		case 'f':
			break;
		case 's':
			break;
	}
}

void processSpecialKeys(int key, int x, int y){

}
