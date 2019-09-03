main.run: main.h main.cpp keyboard_input.cpp
	g++ main.cpp keyboard_input.cpp -o main.run -lGL -lglut -lGLU -lpulse-simple -lpulse -lfftw3

clean: 
	rm main.run
