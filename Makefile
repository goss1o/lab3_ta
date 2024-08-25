
SRC = $(wildcard ast/*.cpp translator/*.cpp)
OBJ = $(patsubst %.cpp, %.o, $(SRC))

CC = g++ -std=c++20 -g -Wno-register

TARGET = lab

all: $(OBJ)
	bison -d lab.y -b lab
	flex lab.l
	$(CC) $(OBJ) lex.yy.c lab.tab.c -o $(TARGET)
	rm -f $(OBJ)

%.o: %.cpp
	$(CC) -c $< -o $@

clean:
	rm -f lab.tab.* lex.yy.* *.output $(OBJ) $(TARGET)