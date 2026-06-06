TARGET = build/cnsynth

SRCS = $(wildcard src/*.c)
OBJS = $(SRCS:src/%.c=build/obj/%.o)

$(CC) = gcc

COMPILE_FLAGS = -Iinclude -lm -lportaudio
LINKER_FLAGS = 

$(TARGET): $(OBJS)
	$(CC) -o $@ $^ $(COMPILE_FLAGS)

build/obj/%.o: src/%.c
	mkdir -p build/obj/
	$(CC) -c -o $@ $^ $(COMPILE_FLAGS)

clean:
	rm -rf build/

run: $(TARGET)
	./$(TARGET)
