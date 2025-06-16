init: ../windowing/calam.cpp
	g++ ../windowing/calam.cpp -c -o calam.o -I/usr/include/SDL2 -D_REENTRANT -lSDL2 -Wall -Wextra

render: calam.o gridRenderer.cpp
	g++ gridRenderer.cpp calam.o -o map -I/usr/include/SDL2 -D_REENTRANT -lSDL2 -Wall -Wextra

pierrgen: pierrGen.c
	gcc pierrGen.c -o pierrG

pierrDec: PierrDecoder.c
	gcc PierrDecoder.c -o pierr

clean:
	rm ../windowing/map ../windowing/calam.o pierrG pierr
