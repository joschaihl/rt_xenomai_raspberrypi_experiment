#ifndef ARGUMENTPARSER_TEST_H_
#define ARGUMENTPARSER_TEST_H_

#include <cxxtest/TestSuite.h>

#include "ArgumentParser.h"
/**
 *
 */
class ArgumentParser_test : public CxxTest::TestSuite
{
public:

	ArgumentParser aParser;

	/**
	 * getopt() und getopt_long() haben ein undefiniertes Verhalten,
	 * wenn sich die Variablen argc und argv ändern sollten
	 * und dabei getopt() mehrfach aufgerufen wird.
	 * Das wurde durch Debugging auf diesem Debian System
	 * festgestellt durch die CxxTest-Suite herausgefunden.
	 * Normalerweise ändern sich diese Variablen nicht, da
	 * sie einmal über die Main-Funktion übergeben werden.
	 * In der Praxis sollte dies kein Problem darstellen,
	 * da sich da argc und argv praktisch nicht ändern kann.
	 * Wegen diesem Problem können hier nicht viele
	 * Parameter-Varianten getestet werden.
	 */
	void testArgumentParser()
	{
		char *argv[] = {"read", "-p", "666", "-m", "990", NULL};
		ArgumentParser p;
		/*char *dummy_args[] = {"read_shm", "l", NULL};
		TS_ASSERT(p.parse(1, dummy_args));
		TS_ASSERT_EQUALS(p.getReaderState(), READ_LAST_WRITTEN_DATA);
		TS_ASSERT_EQUALS(p.getMaxLength(), 0);
		TS_ASSERT_EQUALS(p.getPage(), 0);*/

		int i;
		TS_ASSERT(aParser.parse(5, argv));

		TS_ASSERT_EQUALS(aParser.getReaderState(), READ_SPECIFIC_PAGE);
		TS_ASSERT_EQUALS(aParser.getPage(), 666);
		TS_ASSERT_EQUALS(aParser.getMaxLength(), 990);


	}
};

#endif /*ARGUMENTPARSER_TEST_H_*/
