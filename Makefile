CC=gcc
OBJ=
FLAGS=-g -DUSE_XATTR=1 -D_FILE_OFFSET_BITS=64 -g -O2 -DPACKAGE_NAME=\"\" -DPACKAGE_TARNAME=\"\" -DPACKAGE_VERSION=\"\" -DPACKAGE_STRING=\"\" -DPACKAGE_BUGREPORT=\"\" -DPACKAGE_URL=\"\" -DHAVE_LIBUSEFUL=1 -DHAVE_LIBSSL=1 -DHAVE_LIBCRYPTO=1 -DSTDC_HEADERS=1 -D_FILE_OFFSET_BITS=64
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
	$(MAKE) -C libUseful-4 clean
	-rm -f *.o term_ctrl
	-rm config.log config.status 
	-rm -r autom4te.cache config.cache

test:
	echo "no tests"
