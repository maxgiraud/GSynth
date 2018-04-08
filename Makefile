CC=g++
CFLAGS=
LDFLAGS=-lrt -lm -lasound -ljack -pthread -lncurses
EXEC=gMix
SRC= $(wildcard src/*.cpp)
OBJ= $(SRC:.cpp=.o)

all: $(EXEC)
	./$(EXEC)

$(EXEC): $(OBJ)
	$(CC) -o $@ $^ lib/libportaudio.a $(LDFLAGS)

%.o: %.cpp
	$(CC) -o $@ -c $< $(CFLAGS)

main.o: WorkSpace.h

WorkSpace.o: Buffer.h WavIO.h SoundIO.h Display.h Tools.h

Buffer.o: Wave.h

.PHONY: release clean rebuild mrproper

release: clean $(EXEC)
	clear
	./$(EXEC) 2> /dev/null

clean:
	rm -rfv src/*.o

rebuild: mrproper all

mrproper: clean
	rm -rfv $(EXEC)
