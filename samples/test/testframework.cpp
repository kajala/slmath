#include "testframework.h"

#include <stdio.h>
#include <string.h>
#ifdef WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif

#if defined(_DEBUG) && defined(_MSC_VER) 
#include <crtdbg.h>
#endif

/** 
 * Maximum length of the test ID string. 
 * Note that the constant is internal to the implementation (this file),
 * so it does not harm even if we hardcode the constant like this.
 */
static const int TESTID_MAX_LEN = 512;

void formatTestId( char* testid, const char* file, int line, const char* timestamp, const char* expr )
{
	// limit filename to 128+-some characters
	char fname[256];
	if ( strlen(file) > 128 )
	{
		strncpy_s( fname, sizeof(fname), file, 16 );
		strcat_s( fname, sizeof(fname), file+strlen(file)-128 );
	}
	else
	{
		strcpy_s( fname, sizeof(fname), file );
	}

	// limit expression to 128+-some characters
	char e[256];
	if ( strlen(expr) > 128 )
	{
		strncpy_s( e, sizeof(e), expr, 128 );
		strcat_s( e, sizeof(e), "..." );
	}
	else
	{
		strcpy_s( e, sizeof(e), expr );
	}

    sprintf( testid, "TEST(%s): %s(%d) version %s", e, fname, line, timestamp );
}

void debugBreak()
{
#if defined(_DEBUG) && defined(_MSC_VER) 
	_CrtDbgBreak();
#endif
}

static void defaultOutput( void* /*context*/, const char* sz )
{
	printf( "%s", sz );
#ifdef WIN32
	OutputDebugStringA( sz );
#endif
}

bool executeTests( const char* libname, TestFunc_t testfunc, 
	int flags, TestOutputFunc_t output, void* context )
{
	if ( 0 == output )
		output = defaultOutput;

	const bool verbose = 0 != (TEST_VERBOSE & flags);
	char testid[TESTID_MAX_LEN] = "";

	try
	{
		if ( verbose )
		{
			output( context, "Testing " );
			output( context, libname );
			output( context, " ... " );
		}

		bool ok = testfunc( testid );
		if ( ok )
		{
			if ( verbose ) 
				output( context, "OK!\n" );
		}
		else
		{
			output( context, "\n" );
			output( context, "FAILED " );
			output( context, testid );
			output( context, "\n" );
		}

		return ok;
	}
	catch ( ... )
	{
		output( context, "\n" );
		output( context, "EXCEPTION IN " );
		output( context, libname );
		output( context, " " );
		output( context, testid );
		output( context, "\n" );
		return false;
	}
}

// This file is part of 'slmath' C++ library. Copyright (C) 2009 Jani Kajala (kajala@gmail.com). See http://sourceforge.net/projects/slmath/
