CC      = aarch64-buildroot-linux-gnu-gcc
CFLAGS  = -Wall -O2 -Iinclude
SRCS    = src/main.c src/fb.c src/draw.c src/face.c
TARGET  = face

all: $(TARGET)

$(TARGET): $(SRCS)
	$(CC) $(CFLAGS) -o $@ $(SRCS) -lm

clean:
	rm -f $(TARGET)