main: main.h main.cpp pulse_interface.cpp
	g++ main.cpp pulse_interface.cpp -o main -lGL -lglut -lGLU -lpulse-simple -lpulse -lfftw3

pulse_interface_testing: pulse_interface_testing.cpp pulse_interface.cpp
	g++ pulse_interface_testing.cpp pulse_interface.cpp -o pulse_interface_testing -lpulse

clean: 
	rm main pulse_interface_testing
