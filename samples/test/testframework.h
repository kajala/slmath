/*
 * Simple testing framework.
 *
 * Usage:
 * 
 * 1) Write test function (using TEST macro) which contains all actual tests. For example:
 *
 * bool myTests( char* testid )<br>
 * { <br>
 *      vec2 a(1,2); <br>
 *      vec2 b(3,4); <br>
 *      TEST( length(a+b-vec2(4,6)) < 1e-20f ) <br>
 * <br>
 *      vec2 a(1,2); <br>
 *      vec2 b(3,4); <br>
 *      TEST( length(dot(a,b)-11.0f) < 1e-20f ) <br>
 * } <br>
 *
 * 2) Then call (for example from your main() function):
 *
 * executeTest( "myTests", myTests );
 *
 * 3) See stdout if you got any errors. (you can also override this behavior
¨*   by providing custom string output function if you like)
 */
#ifndef SLMATH_TESTFRAMEWORK_H
#define SLMATH_TESTFRAMEWORK_H

#ifdef _MSC_VER
#pragma warning( disable : 4996 )
#endif

#include <slm/slmath_pp.h>

/**
 * Pointer to the user-defined test function.
 * @param testid [out] Received unique identifier information of the failed test. Must have 256 characterbuffer space.
 * @return false if the test failed.
 */
typedef bool (*TestFunc_t)( char* testid );

/**
 * Zero-terminated string output function with user-defined calling context.
 * @param context User-defined context. Can be 0.
 * @param sz Zero-terminated string to output.
 */
typedef void (*TestOutputFunc_t)( void* context, const char* sz );

/**
 * Options used to control test execution and output printing.
 */
enum TestFlags
{
	/** Only output if errors. */
	TEST_QUIET		= 0,
	/** Output ok status as well besides errors. */
	TEST_VERBOSE	= 1,
};

/**
 * Executes test function and reports any found errors.
 * Also catches all thrown exceptions and reports error.
 * The test function has following format:
 *
 * bool testfunc( char* testid );
 *
 * @param libname The library/component name to be tested.
 * @param testfunc The test function.
 * @param flags Optional. Testing options. See TestFlags. Default is errors only (TEST_QUIET).
 * @param output Optional. Testing result string output. Default output function is 0 (stdout).
 * @param context Optional. Testing result string output calling context. Default is 0.
 * @return false if the test failed.
 */
bool executeTests( const char* libname, TestFunc_t testfunc,
	int flags=TEST_QUIET, TestOutputFunc_t output=0, void* context=0 );

/*
 * Formats test identifier message.
 * Internal method to the testframework.
 * @param testid [out] Receives test identifier generated from the parameters.
 * @param file Test source file name.
 * @param line Test source file line number.
 * @param line Test source file time stamp.
 * @param expr Test expression.
 */
void formatTestId( char* testid, const char* file, int line, const char* timestamp, const char* expr );

/*
 * Breaks execution into debugger if available.
 */
void debugBreak();

/**
 * Tests an assumption. Returns from the calling function if the test fails.
 * Stores test identification info before executing the test.
 *
 * Assumes that variable testid is defined as char[512].
 *
 * @param A The test to be executed, e.g. a==b.
 * @return false if test failed, continues execution otherwise.
 */
#define TEST(A) \
{ \
	formatTestId( testid, __FILE__, __LINE__, __TIMESTAMP__, #A ); \
	if ( !(A) ) \
	{ \
		debugBreak(); \
		return false; \
	} \
}

#endif

// This file is part of 'slmath' C++ library. Copyright (C) 2009 Jani Kajala (kajala@gmail.com). See http://sourceforge.net/projects/slmath/
