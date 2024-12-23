CC = gcc
CFLAGS = -Isrc/Cpu -Isrc/Mem -Wall -Wextra -g
LDFLAGS =

# List of source files
sources = src/main.c src/Cpu/Instructions.c src/Cpu/Cpu.c src/Mem/Memory.c
# Create object files list in bin directory
objects = $(sources:src/%.c=bin/%.o)

# Final executable
EXEC = emulator

all: $(EXEC)

$(EXEC): $(objects)
	@mkdir -p bin 
	$(CC) $(objects) -o $(EXEC) $(LDFLAGS)

# Rule to create object files in the bin directory
bin/%.o: src/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

# Clean up the bin directory and object files
clean:
	rm -rf bin $(EXEC)

