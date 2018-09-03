COMPILER=g++
RUNSTRING=./${TARGET}
OBJECTS=main.o draw.o shaderprogram.o
LIBS= -lpthread -lsndfile `sdl2-config --libs` -lGL
CXXFLAGS=-g -std=c++11 -Ofast `sdl2-config --cflags`

TARGET=project

all: ${TARGET}


${TARGET}: ${OBJECTS} #cleancpp
	${COMPILER} ${FLAGS} -o ${TARGET} ${OBJECTS} ${LIBS}


#För att kunna köra filen direkt
run: ${TARGET}
	${RUNSTRING}

clean:
	rm *.o
	rm .depend

rebuild: clean ${TARGET}
