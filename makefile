CFLAGS=`xml2-config --cflags` -g -Wall -Wextra -O3 -std=gnu99
LDLIBS=`xml2-config --libs` -larchive -lbinn
objects=comments.o zip.o xmlbuff.o
mingwCFLAGS=`x86_64-w64-mingw32-pkg-config --cflags --libs libxml-2.0` -g -Wall -Wextra -O3 -std=gnu99
mingwLDLIBS=`x86_64-w64-mingw32-pkg-config --libs libxml-2.0` -larchive -lbinn-1.0
mingw=x86_64-w64-mingw32-gcc
MACCFLAGS=`xml2-config --cflags` `pkg-config --cflags --libs libarchive` -g -Wall -Wextra -O3 -std=gnu99
MACLDLIBS=`xml2-config --libs` -lbinn

anonymize: $(objects)

bin/anonymize.exe:
	$(mingw) $(mingwCFLAGS) -c comments.c $(mingwLDLIBS)
	$(mingw) $(mingwCFLAGS) -c zip.c $(mingwLDLIBS)
	$(mingw) $(mingwCFLAGS) -c xmlbuff.c $(mingwLDLIBS)
	$(mingw) $(mingwCFLAGS) anonymize.c $(objects) $(mingwLDLIBS) -o $@

mac/anonymize:
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
