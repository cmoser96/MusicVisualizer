all: main.run

main.run: main.cpp pulse_interface.cpp
	g++ main.cpp pulse_interface.cpp -o main.run -lGL -lglut -lGLU -lpulse-simple -lpulse -lfftw3

clean: 
	rm main.run
