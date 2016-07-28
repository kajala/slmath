#ifndef SLMATH_TESTS_H
#define SLMATH_TESTS_H

#include <slm/slmath_pp.h>

/**
 * The slmath library regression/unit testing.
 * @param testid [out] Receives identifier of the failed test. Buffer size must be 512 characters.
 * @return true if success, false otherwise.
 * @ingroup slmath
 */
bool	test_slmath( char* testid );

#endif

// This file is part of 'slmath' C++ library. Copyright (C) 2009 Jani Kajala (kajala@gmail.com). See http://sourceforge.net/projects/slmath/
