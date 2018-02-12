#ifndef SLMATH_CONFIGURE_H
#define SLMATH_CONFIGURE_H

/** Enable SIMD extensions (if supported by this platform) */
#if defined(_M_X64) || (_M_IX86_FP == 2)
#define SLMATH_SIMD
#endif

/** Enable namespace support, everything placed inside 'slm' namespace */
#define SLMATH_NAMESPACE

/** Disable pragma messages */
#define SLMATH_NO_PRAGMA_MESSAGES

/** Enable vec-op asserts on _DEBUG build */
//#define SLMATH_VEC_ASSERTS

#endif // SLMATH_CONFIGURE_H

// This file is part of 'slm' C++ library. Copyright (C) 2009-2018 Jani Kajala (kajala@gmail.com). Licensed under BSD/MIT license
