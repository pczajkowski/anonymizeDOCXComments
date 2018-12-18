#include "zip.h"
#include "stopif.h"

int processComments(struct archive *archiveOut, XMLBuff *comments) {
	if (!anonymizeComments(comments)) return 0;

	struct archive_entry *newEntry = archive_entry_new();
	archive_entry_set_pathname(newEntry, comments->name);
	archive_entry_set_size(newEntry, comments->size);
	archive_entry_set_filetype(newEntry, AE_IFREG);
	archive_entry_set_perm(newEntry, 0664);

	Stopif(archive_write_header(archiveOut, newEntry) != ARCHIVE_OK, return 0, "Can't write entry header (comments)!\n");
	Stopif(archive_write_data(archiveOut, comments->data, comments->size) != comments->size, return 0, "Can't write data (comments)!\n");
	archive_entry_free(newEntry);
	return 1;
}

int rewriteZIP(struct archive *archiveIn, struct archive *archiveOut) {
	const char *commentsFile = "word/comments.xml";
	struct archive_entry *entryIn;

	while (archive_read_next_header(archiveIn, &entryIn) == ARCHIVE_OK) {
		const char* path = archive_entry_pathname(entryIn);
		int64_t size = archive_entry_size(entryIn);
		char buf[size];
		Stopif(archive_read_data(archiveIn, buf, size) != size, return 0, "Archive entry has no size (%s)!\n", path);

		if (strcmp(commentsFile, path) == 0){
			XMLBuff *comments = XMLBuffNew();
			Stopif(comments == NULL, return 0, "Couldn't obtain comments!\n");
			*comments = (XMLBuff){.data=buf, .size=size, .name=path};

			if (!processComments(archiveOut, comments)) return 0;
			XMLBuffFree(comments);
		} else {
			Stopif(archive_write_header(archiveOut, entryIn) != ARCHIVE_OK, return 0, "Can't write entry header!\n");
			Stopif(archive_write_data(archiveOut, buf, size) != size, return 0, "Can't write data!\n");
		}
	}
	return 1;
}

int processDOCX(const char *infile, const char *outfile) {
	struct archive *archiveIn;
	struct archive *archiveOut;

	archiveIn = archive_read_new();
	archive_read_support_format_zip(archiveIn);
	Stopif(archive_read_open_filename(archiveIn, infile, 10240) != ARCHIVE_OK, return 0, "Can't read file %s!\n", infile);

	archiveOut = archive_write_new();
	archive_write_set_format_zip(archiveOut);

	Stopif(archive_write_open_filename(archiveOut, outfile) != ARCHIVE_OK, return 0, "Can't create new archive %s!\n", outfile);

	if (!rewriteZIP(archiveIn, archiveOut)) return 0;

	Stopif(archive_read_free(archiveIn) != ARCHIVE_OK, return 0, "Can't free %s!\n", infile);
	Stopif(archive_write_free(archiveOut) != ARCHIVE_OK, return 0, "Can't free %s!\n", outfile);
	return 1;
}

int process(const char *infile, char *outfile) {
	if (!outfile || deanonymize) {
		strcat(binnFile, infile);
	} else {
		strcat(binnFile, outfile);
	}

	strcat(binnFile, ".bin");

	if (!outfile || strcmp(infile, outfile) == 0){
		const char *outfile = "tmpFile.docx";
		processDOCX(infile, outfile);
		remove(infile);
		rename(outfile, infile);
	} else {
		processDOCX(infile, outfile);
	}

	return 1;
}
