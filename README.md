# Anonymize DOCX Comments

While doing review in Word documents translators/reviewers often use tracked changes and comments to exchange feedback on translations. Usually these people are from different organizations and shouldn't know about each other. Hence the need to anonymize comments and this is what this tool will do for you.

It'll go through comments in "word/comments.xml" and change each author's name to Author*number*, where number starts from 1. It'll keep track of authors so "John Smith" will always be "Author1" for instance. After it's done it'll print list of authors and their new names.

Usage:

*./anonymize test.docx* - test.docx will be replaced with anonymized version.

*./anonymize test.docx test2.docx* - anonymized version will be saved as test2.docx leaving original test.docx intact.

Running it on provided *test.docx* should produce:

	"King, Stephen" is now "Author1"
	"Kowalski, Jan" is now "Author2"
	"Piotr Fronczewski" is now "Author3"

You'll need libarchive and libxml2 to compile it. It was created as learning project while I was exploring C, so use it freely, but at your own risk. Output was tested with Word 2013 and Libre Office Writer.

To relieve your pain I've added compiled Windows 64 bit binary in the bin folder. It includes all necessary MingW dlls. Enjoy!
