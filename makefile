CFLAGS=`pkg-config --cflags --libs libxml-2.0` -g -Wall -Wextra -O3 -std=c99
LDLIBS=`pkg-config --libs libxml-2.0` -larchive
objects=keyval.o dict.o comments.o zip.o xmlbuff.o
mingwCFLAGS=`x86_64-w64-mingw32-pkg-config --cflags --libs libxml-2.0` -g -Wall -Wextra -O3 -std=c99
mingwLDLIBS=`x86_64-w64-mingw32-pkg-config --libs libxml-2.0` -larchive
mingw=x86_64-w64-mingw32-gcc
MACCFLAGS=`pkg-config --cflags --libs libxml-2.0 libarchive` -g -Wall -Wextra -O3 -std=c99
MACLDLIBS=`pkg-config --libs libxml-2.0 libarchive`

anonymize: $(objects)

bin/anonymize.exe:
	$(mingw) $(mingwCFLAGS) -c keyval.c $(mingwLDLIBS)
	$(mingw) $(mingwCFLAGS) -c dict.c $(mingwLDLIBS)
	$(mingw) $(mingwCFLAGS) -c comments.c $(mingwLDLIBS)
	$(mingw) $(mingwCFLAGS) -c zip.c $(mingwLDLIBS)
	$(mingw) $(mingwCFLAGS) -c xmlbuff.c $(mingwLDLIBS)
	$(mingw) $(mingwCFLAGS) anonymize.c $(objects) $(mingwLDLIBS) -o $@

mac/anonymize:
	gcc $(MACCFLAGS) -c keyval.c $(MACLDLIBS)
	gcc $(MACCFLAGS) -c dict.c $(MACLDLIBS)
	gcc $(MACCFLAGS) -c comments.c $(MACLDLIBS)
	gcc $(MACCFLAGS) -c zip.c $(MACLDLIBS)
	gcc $(MACCFLAGS) -c xmlbuff.c $(MACLDLIBS)
	gcc $(MACCFLAGS) anonymize.c $(objects) $(MACLDLIBS) -o $@

.PHONY: clean
clean:
	rm *.o

win: bin/anonymize.exe

mac: mac/anonymize

default: anonymize
