/*
 * ArgumentParser.h
 *
 *  Created on: 05.02.2013
 *      Author: ihl
 */

#ifndef ARGUMENTPARSER_H_
#define ARGUMENTPARSER_H_

#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>

enum ReaderState {
		READER_STATE_UNDEFINED = 0,
	    READ_LAST_WRITTEN_DATA = 1,
	    READ_SPECIFIC_PAGE = 2
};

class ArgumentParser
{
public:
	ArgumentParser();
	void printHelp(FILE* stream);
	bool parse(int argc, char **argv);
	unsigned long long getMaxLength();
	unsigned long long getPage();
	ReaderState getReaderState();
	virtual ~ArgumentParser();
private:
	ReaderState readerState;
	unsigned long long page_index;
	unsigned long long maxlength;
	const char* program_name;
};

#endif /* ARGUMENTPARSER_H_ */
