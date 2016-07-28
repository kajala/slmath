/*
 * Run unit/regression tests of slmath using the simple testing framework
 * provided in the library.
 */
#include "test_slmath.h"
#include "testframework.h"
#include <slm/slmath.h>
#include <stdio.h>
#include <string.h>

SLMATH_USING()


/** 
 * Returns true if specified option is in command line.
 * @param opt Option name. For example "-v".
 * @param argc Command line argument count.
 * @param argv Command line arguments array.
 */
static bool hasOption( const char* opt, int argc, char* argv[] )
{
	for ( int i = 1 ; i < argc ; ++i )
		if ( !strcmp(argv[i],opt) )
			return true;
	return false;
}

int main( int argc, char* argv[] )
{
	int testflags = hasOption("-q",argc,argv) ? TEST_QUIET : TEST_VERBOSE;

	executeTests( "slmath", test_slmath, testflags );

	return 0;
}
