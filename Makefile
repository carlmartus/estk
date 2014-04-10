CC=clang
LDFLAGS=-lSDL -lGL -lGLEW -lm -lSDL_image
EMCC=/opt/emscripten/emcc
DEMOS=$(wildcard demo_*.c)
PROGS=$(DEMOS:%.c=%)
HTMLS=$(DEMOS:demo_%.c=emscripten_%.html)
TKS=estk.c estk.h

all: $(PROGS) $(HTMLS)

%: %.c $(TKS)
	$(CC) -O2 -Wall -g -o $@ $< estk.c $(LDFLAGS)

emscripten_%.html: demo_%.c demores $(TKS)
	$(EMCC) -Wall -o $@ $< estk.c --preload-file demores

clean:
	$(RM) emscripten_*.* $(PROGS)

re: clean all

