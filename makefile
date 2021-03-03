CFLAGS=`xml2-config --cflags` -I/usr/local/opt/libarchive/include -g -Wall -Wextra -Wshadow -O3 -std=c99
LDLIBS=`xml2-config --libs` -larchive -lbinn
objects=comments.o zip.o xmlbuff.o

anonymize: $(objects)

.PHONY: clean
clean:
	rm *.o

test: anonymize
	@./anonymize testFiles/input.docx testFiles/output.docx
	@cmp testFiles/input.docx testFiles/output.docx -s ; if [ $$? -eq 0 ] ; then echo "FAILURE: Files should differ after anonymizing!"; exit 1; fi
	@./anonymize testFiles/output.docx -d
	@cmp testFiles/input.docx testFiles/output.docx -s ; if [ $$? -eq 0 ] ; then echo "OK"; else echo "FAILURE: Files should be the same after de-anonymizing!"; fi
	@rm testFiles/output.docx

default: anonymize
