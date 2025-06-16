init: include/calam.cpp
	g++ include/calam.cpp -c -o calam.o -I/usr/include/SDL2 -D_REENTRANT -lSDL2 -Wall -Wextra

example: init calam.o example/example.cpp
	g++ example/example.cpp calam.o -o main -I/usr/include/SDL2 -D_REENTRANT -lSDL2 -Wall -Wextra

map: init example/map_example.cpp calam.o
	g++ example/map_example.cpp calam.o -o map -I/usr/include/SDL2 -D_REENTRANT -lSDL2 -Wall -Wextra

clean:
	rm main calam.o map output.ppm
