# Compiler settings
CC = x86_64-w64-mingw32-gcc
# Compilation flags
CFLAGS = -mwindows

# Target name
TARGET = win_msg.exe
# Source file
SRC = win_msg.c

# Default rule
all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(SRC) -o $(TARGET) $(CFLAGS)

# Clean rule to remove the executable
clean:
	rm -f $(TARGET)

# Run rule using wine
run: $(TARGET)
	wine $(TARGET)
