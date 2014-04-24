LDFLAGS=-lSDL -lGL -lGLEW -lm -lSDL_image -lSDL_mixer
EMCC=/opt/emscripten/emcc
DDIR=demos
DEMOS=$(wildcard $(DDIR)/*.c)
PROGS=$(DEMOS:$(DDIR)/%.c=demo_%)
HTMLS=$(DEMOS:$(DDIR)/%.c=emscripten_%.html)
TKS=estk.c estk.h

.SECONDARY:

all: $(PROGS) $(HTMLS)

demo_%: $(DDIR)/%.c $(TKS)
	$(CC) -O2 -Wall -g -o $@ $< estk.c $(LDFLAGS) -I"."

emscripten_%.html: $(DDIR)/%.c demores $(TKS)
	$(EMCC) -Wall -o $@ $< estk.c -I"." --preload-file demores

%/.d:
	mkdir $(@:%/.d=%)
	touch $@

clean:
	$(RM) $(PROGS) emscripten_*.*

re: clean all

