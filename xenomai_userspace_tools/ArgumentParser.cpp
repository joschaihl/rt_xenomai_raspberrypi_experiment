/*
 * ArgumentParser.cpp
 *
 *  Created on: 05.02.2013
 *      Author: ihl
 */

#include "ArgumentParser.h"
#include <cxxtest/TestSuite.h>


const char* short_options = "hp:lm:";
/* An array describing valid long options.  */
const struct option long_options[] = {
	{ "help", no_argument, NULL, 'h' },
	{ "page", required_argument, NULL, 'p' },
	{ "last", no_argument, NULL, 'l' },
	{ "maxlength", required_argument, NULL, 'm' },
	{ NULL, 0, NULL, 0 }
};

ArgumentParser::ArgumentParser() :
	readerState(READ_LAST_WRITTEN_DATA),
	page_index(0), maxlength(0), program_name("undefined")
{

}

/* Prints usage information for this program to STREAM (typically
 stdout or stderr).  */
void ArgumentParser::printHelp(FILE* stream)
{
	fprintf(stream, "Usage:  %s [-h] [[-p PAGE] | [-l]] [-m MAXLENGTH]\n",
			program_name);
	fprintf(stream,
			"  -h  --help              Display this usage information.\n",
			"  -p  --page PAGE         set page from ringbuffer memory to view (from 0 to x)\n",
			"  -l  --last              view the last written data of the ringbuffer (this is default if -p or -l is not set)\n",
			"  -m  --maxlength LENGTH  set maximum datasets in page to view (default 100)\n\n");
}

bool ArgumentParser::parse(int argc, char **argv)
{
	int next_option;
	/* Remember the name of the program, to incorporate in messages.
	 The name is stored in argv[0].  */
	this->readerState = READER_STATE_UNDEFINED;
	this->maxlength = 100ULL;
	this->page_index = 0;
	opterr = 0;
	if(argc >= 1)
	{
		program_name = argv[0];
		do
		{
			//next_option = getopt(argc, argv, "hp:lm:");
			next_option = getopt_long(argc, argv, short_options, long_options,
					NULL );
			switch (next_option)
			{
			case 'h': /* -h or --help */
				/* User has requested usage information.  Print it to standard
				 output, and exit with exit code zero (normal termination).  */
				printHelp(stdout);
				return true;
				break;
			case 'p':
				if(readerState == READER_STATE_UNDEFINED)
				{
					this->readerState = READ_SPECIFIC_PAGE;
					page_index = strtoull(optarg, NULL, 0);
				}
				else
				{
					printHelp(stderr);
					return false;
				}
				break;
			case 'l':
				if(readerState == READER_STATE_UNDEFINED)
				{
					this->readerState = READ_LAST_WRITTEN_DATA;
				}
				else
				{
					printHelp(stderr);
					return false;
				}
				break;
			case 'm':
				maxlength = strtoull(optarg, NULL, 0);
				break;
			case '?': /* The user specified an invalid option.  */
				/* Print usage information to standard error, and exit with exit
				 code one (indicating abnormal termination).  */
				printHelp(stderr);
				break;
			case -1: /* Done with options.  */
				break;
			default: /* Something else: unexpected.  */
				return false;
				break;
			}
		} while (next_option != -1);
	}
	if(readerState == READER_STATE_UNDEFINED)
	{
		this->readerState = READ_LAST_WRITTEN_DATA;
	}
	return true;
}

unsigned long long ArgumentParser::getMaxLength()
{
	return maxlength;
}
unsigned long long ArgumentParser::getPage()
{
	return page_index;
}

ReaderState ArgumentParser::getReaderState()
{
	return this->readerState;
}

ArgumentParser::~ArgumentParser()
{

}

