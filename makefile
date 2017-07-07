P=anonymize
CFLAGS=`pkg-config --cflags --libs libxml-2.0` -g -Wall -O3 -std=gnu99
LDLIBS=`pkg-config --libs libxml-2.0` -larchive
objects=keyval.o dict.o comments.o zip.o xmlbuff.o

$(P): $(objects)

clean:
	rm *.o
