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

/***
 * Reader State
 */
enum ReaderState {
		READER_STATE_UNDEFINED = 0,   //!< READER_STATE_UNDEFINED
	    READ_LAST_WRITTEN_DATA = 1,//!< READ_LAST_WRITTEN_DATA
	    READ_SPECIFIC_PAGE = 2     //!< READ_SPECIFIC_PAGE
};

/**
 *
 */
class ArgumentParser
{
public:
	ArgumentParser();
	/**
	 *
	 * @param stream
	 */
	void printHelp(FILE* stream);
	/**
	 *
	 * @param argc
	 * @param argv
	 * @return
	 */
	bool parse(int argc, char **argv);
	/**
	 * @return
	 */
	unsigned long long getMaxLength();
	/**
	 * @return
	 */
	unsigned long long getPage();
	/**
	 *
	 * @return
	 */
	ReaderState getReaderState();
	/**
	 *
	 */
	virtual ~ArgumentParser();
private:
	ReaderState readerState;
	unsigned long long page_index;
	unsigned long long maxlength;
	const char* program_name;
};

#endif /* ARGUMENTPARSER_H_ */
