#include <stdio.h>
#include <archive.h>
#include <archive_entry.h>
#include <string.h>
#include "comments.h"
#include "stopif.h"

static int processComments(struct archive *archiveOut, char buf[], size_t size, const char* path) {
	XMLBuff *comments = XMLBuffNew();
	*comments = (XMLBuff){.data=buf, .size=size, .name=path};
	anonymizeComments(comments);

	struct archive_entry *newEntry = archive_entry_new();
	archive_entry_set_pathname(newEntry, path);
	archive_entry_set_size(newEntry, comments->size);
	archive_entry_set_filetype(newEntry, AE_IFREG);
	archive_entry_set_perm(newEntry, 0664);

	Stopif(archive_write_header(archiveOut, newEntry) != ARCHIVE_OK, return -2, "Can't write entry header (comments)!\n");
	Stopif(archive_write_data(archiveOut, comments->data, comments->size) != comments->size, return -3, "Can't write data (comments)!\n");
	archive_entry_free(newEntry);
	XMLBuffFree(comments);
	return 1;
}

static int rewriteZIP(struct archive *archiveIn, struct archive *archiveOut) {
	const char *commentsFile = "word/comments.xml";
	struct archive_entry *entryIn;

	while (archive_read_next_header(archiveIn, &entryIn) == ARCHIVE_OK) {
		const char* path = archive_entry_pathname(entryIn);
		size_t size = archive_entry_size(entryIn);
		char buf[size];
		Stopif(archive_read_data(archiveIn, buf, size) != size, return -2, "Archive entry has no size (%s)!\n", path);

		if (strcmp(commentsFile, path) == 0){
			Stopif(!processComments(archiveOut, buf, size, path), return -1, "Can't process comments!\n");
		} else {
			Stopif(archive_write_header(archiveOut, entryIn) != ARCHIVE_OK, return -2, "Can't write entry header!\n");
			Stopif(archive_write_data(archiveOut, buf, size) != size, return -3, "Can't write data!\n");
		}
	}
	return 1;
}

static int processDOCX(char const *infile, char const *outfile) {
	struct archive *archiveIn;
	struct archive *archiveOut;

	archiveIn = archive_read_new();
	archive_read_support_format_zip(archiveIn);
	Stopif(archive_read_open_filename(archiveIn, infile, 10240), return -1, "Can't read file %s!\n", infile);

	archiveOut = archive_write_new();
	archive_write_set_format_zip(archiveOut);

	Stopif(archive_write_open_filename(archiveOut, outfile) != ARCHIVE_OK, return -1, "Can't create new archive %s!\n", outfile);

	Stopif(!rewriteZIP(archiveIn, archiveOut), return -1, "Problems rewriting zip!\n");
	Stopif(archive_read_free(archiveIn) != ARCHIVE_OK, return -1, "Can't free %s!\n", infile);
	Stopif(archive_write_free(archiveOut) != ARCHIVE_OK, return -1, "Can't free %s!\n", outfile);
	return 1;
}

int process(char const *infile, char *outfile) {
	if (!outfile || strcmp(infile, outfile) == 0){
		const char *outfile = "tmpFile.docx";
		processDOCX(infile, outfile);
		rename(outfile, infile);
	} else {
		processDOCX(infile, outfile);
	}
	return 1;
}