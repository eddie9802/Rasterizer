OBJS = sdl.cpp

CC = g++

OBJ_NAME = ./out/program

LINKERFLAGS = -lSDL2 `sdl-config --libs` -lSDL2_image

COMPILERFLAGS = -std=c++17 -g



all: $(OBJS)
	$(CC) $(OBJS) $(COMPILERFLAGS) $(LINKERFLAGS) -o $(OBJ_NAME)

clean:
	rm ./out/*.o