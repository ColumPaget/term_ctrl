CC=gcc
OBJ=
FLAGS=-g -DUSE_XATTR=1 -D_FILE_OFFSET_BITS=64 $(CFLAGS)
BUNDLED_LIBS=
LIBS=-lcrypto -lssl -lUseful 
prefix=/usr/local
exec_prefix=${prefix}
bindir=${exec_prefix}/bin
sysconfdir=${prefix}/etc

all: $(OBJ) main.c $(BUNDLED_LIBS)
	$(CC) $(FLAGS) -o term_ctrl main.c $(OBJ) $(LIBS) $(BUNDLED_LIBS) 

libUseful-4/libUseful.a: 
	$(MAKE) -C libUseful-4

install:
	mkdir -p $(DESTDIR)$(bindir)

clean:
	rm -f *.o */*.o */*.so */*.a

test:
	echo "no tests"
