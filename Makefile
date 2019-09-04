main.run: main.h main.cpp
	g++ main.cpp -o main.run -lGL -lglut -lGLU -lpulse-simple -lpulse -lfftw3

clean: 
	rm main.run
