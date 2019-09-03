all: main.run pulse_interface_testing.run

main.run: main.h main.cpp pulse_interface.cpp
	g++ main.cpp pulse_interface.cpp -o main.run -lGL -lglut -lGLU -lpulse-simple -lpulse -lfftw3

pulse_interface_testing.run: pulse_interface_testing.cpp pulse_interface.cpp
	g++ pulse_interface_testing.cpp pulse_interface.cpp -o pulse_interface_testing.run -lpulse

clean: 
	rm main.run pulse_interface_testing.run
