# Makefile pour le projet Game Of Life en 3D
CXX = g++
CFLAGS = -std=c++17 -Wall -Wextra -Werror -Wpedantic -Wconversion
LDFLAGS =
EXEC = main
SRC = $(wildcard *.cpp)
OBJ = $(SRC:.cpp=.o)

ifeq ($(OS), Windows_NT)
	DEL = del /q /s
else
	DEL = rm -rf
endif

ifeq ($(OS), Windows_NT)
	DEL_EXEC = $(DEL) $(EXEC).exe
else
	DEL_EXEC = $(DEL) $(EXEC)
endif


all : $(EXEC)

$(EXEC) : $(OBJ)
	$(CXX) -o $(EXEC) $(OBJ) $(LDFLAGS)

%.o : %.c
	$(CXX) -o $@ -c $< $(CFLAGS)

clean :
	$(DEL) *.o

mrproper : clean
	$(DEL_EXEC)
