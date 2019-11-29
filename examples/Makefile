

CC=gcc


x64 := 0
ifeq ($(x64),1)
MDIR=x64
BITMODE=-m64
else
MDIR=x32
BITMODE=-m32
endif

# Graphite loop optimizations
#GLOP = -ftree-vectorize -floop-interchange -floop-strip-mine -floop-block
GLOP =

#CFLAGS = $(BITMODE) -I../libmylcd/include -I../src/ -Wall -march=i686 -mtune=i686 -O2 -DSTBI_FAILURE_USERMSG -D_WIN32 -D_WIN32_WINNT=0x0501 -DWINVER=0x0501 -D__WIN32__=1 -std=gnu99 -funroll-loops -finline-functions -fomit-frame-pointer -pipe -s -DUSE_MMX -DHAVE_MMX -DHAVE_MMX1 -DUSE_MMX1 -mmmx -msse -mfpmath=sse,387 -fgcse-las -fgcse-sm -fgcse-lm -fmodulo-sched-allow-regmoves -fmodulo-sched -ftree-vectorizer-verbose=0 $(GLOP)
CFLAGS = $(BITMODE) -static-libgcc -I../libmylcd/include -I../src/ -O2 -D_WIN32_WINNT=0x0601 -DWINVER=0x0601 -D__WIN32__=1 -std=gnu99  -Wall -march=i686 -mtune=i686
LIBS = $(BITMODE) -static-libgcc -L"../libmylcd/libs/$(MDIR)/" -L"lib" -lm -lmylcddll -lole32 


EXAMPLES=vtest.exe


all : $(EXAMPLES)


# fileal.o 

vtest.exe: vtest.o ../src/primitives.o ../src/vfont.o ../libmylcd/fileal.o fonts.o
	$(CC) -static-libgcc -o $@ $^ $(LIBS)
	strip.exe $@

%.o: %.c $(DEPS) 
	$(CC) -c -o $@ $< $(CFLAGS)

clean :
	rm -f *.exe *.o *.bak


### PHONY define
.PHONY: all all-before all-after clean clean-custom



