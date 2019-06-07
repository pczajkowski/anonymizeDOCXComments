CFLAGS=`xml2-config --cflags` -g -Wall -Wextra -Wshadow -O3 -std=c99
LDLIBS=`xml2-config --libs` -larchive -lbinn
objects=comments.o zip.o xmlbuff.o
mingwCFLAGS=`x86_64-w64-mingw32-pkg-config --cflags --libs libxml-2.0` -g -Wall -Wextra -Wshadow -O3 -std=c99
mingwLDLIBS=`x86_64-w64-mingw32-pkg-config --libs libxml-2.0` -larchive -lbinn-1.0
mingw=x86_64-w64-mingw32-gcc

anonymize: $(objects)

bin/anonymize.exe:
	$(mingw) $(mingwCFLAGS) -c comments.c $(mingwLDLIBS)
	$(mingw) $(mingwCFLAGS) -c zip.c $(mingwLDLIBS)
	$(mingw) $(mingwCFLAGS) -c xmlbuff.c $(mingwLDLIBS)
	$(mingw) $(mingwCFLAGS) anonymize.c $(objects) $(mingwLDLIBS) -o $@

.PHONY: clean
clean:
	rm *.o

win: bin/anonymize.exe

default: anonymize
