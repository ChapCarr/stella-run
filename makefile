CC = gcc
SRC = main.c
OUT = run

CFLAGS = -Wall -Wextra
RAYLIB_PATH = $(shell brew --prefix raylib)

INCLUDES = -I$(RAYLIB_PATH)/include
LIBS = -L$(RAYLIB_PATH)/lib -lraylib \
       -framework Cocoa -framework IOKit -framework CoreVideo -framework OpenGL

all: $(OUT)

$(OUT): $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o $(OUT) $(INCLUDES) $(LIBS)

clean:
	rm -f $(OUT)
