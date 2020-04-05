CFLAGS=`xml2-config --cflags` -g -Wall -Wextra -Wshadow -O3 -std=c99
LDLIBS=`xml2-config --libs` -larchive -lbinn
objects=comments.o zip.o xmlbuff.o

anonymize: $(objects)

.PHONY: clean
clean:
	rm *.o

default: anonymize
