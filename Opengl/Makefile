main: main.h main.cpp
	g++ main.cpp -o main -lGL -lglut -lGLU -lpulse-simple -lpulse -lfftw3

get_sinks: get_sinks.cpp pulse_interface.cpp
	g++ get_sinks.cpp pulse_interface.cpp -o get_sinks -lpulse

clean: 
	rm main get_sinks
