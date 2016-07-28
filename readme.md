# slmath ver. 2.6.0

Mathematics C++ library with GLSL shading language style classes. 
The library supports various linear algebra classes like 
vec2, vec3, mat4, etc. OpenGL Shading Language specification 
is used as reference for the functionality.

The code is very portable, besides Visual Studio 2008/2010 people 
have used the lib on iPhone, Linux, N-Gage and Symbian S60.
The code is licensed under BSD/MIT, so as free/liberal as it gets.

The library also contains a simple testing framework 
(see samples/test), which is meant for unit/regression testing 
of the slmath library, but it can be used for testing other stuff as well.


## Documentation

"documentation\slmath.chm" contains the HTML Help reference manual.

See also OpenGL Shading Language specification ("GLSLangSpec.Full.1.20.8") 
to see how the library should behave. :)


## Compatibility

Only basic C/C++ is required, so should be relatively easy to compile this on any platform.

Known to work on:

* VS2012
* VS2010
* VS2008
* VS2003 (thanks to Peter Ingebretson <peter at ingebretson.net>)
* iOS
* Android (NDK)
* Maemo/MeeGo (Nokia N900)
* Symbian/S60
* Samsung Bada
* N-Gage

So it should be easy to port to almost any platform. It can be compiled even
on a platform which doesn't support namespaces (e.g. BREW).


## Install/usage 

There is no external dependencies so as long as the headers are found and cpps compiled, everything should be fine.
Personally, I prefer using SLMATH environment variable pointing to the SLMATH directory, so I can just use 
that as reference in Visual Studio project settings $(SLMATH)\include.

Then you just add the slmath project file to your Solution, e.g. from "c:\slmath\build\vs2008\slmath\slmath.vcproj"

After that you can use the library, for example, by including in your code include <slm/vec2.h>


## Using SIMD on 32-bit builds (x86)

32-bit C-runtime libs of Visual Studio don't guarantee memory alignment on 16-byte boundary in heap alloocation,
so you need to align it yourself and use following as replacements for new/deletes:

void* __cdecl operator new( size_t count )
{
    return _aligned_malloc( count, 16 );
}

void* __cdecl operator new[]( size_t count )
{
    return _aligned_malloc( count, 16 );
}

void __cdecl operator delete( void* p )
{
    return _aligned_free( p );
}

void __cdecl operator delete[]( void* p )
{
    return _aligned_free( p );
}


## Asserts in Vector Ops (_DEBUG)

Vector operations have a lot of assert checks. 
Those slow down _DEBUG build but are usually very useful to catch programming errors.
However, if you want to disable those you can comment out line from slmath_configure:

        //#define SLMATH_VEC_ASSERTS

You can also selectively uncomment per compilation unit using following trick:

        #include <slm/slmath_configure.h>
        #undef SLMATH_VEC_ASSERTS
        #include <slm/vec4.h>
        // ...


## About some design decisions (free functions vs member functions)

The choice between member function vs free function depends on case.
There are different things to consider:

Compatibility/similarity with GLSL has always been one of the project goals.
So if GLSL specs have a function done in one way, this lib most likely follows it.
That explains a large number of free functions.

Enabling const-usage. For example, if normalize() would be a modifier member function
use like this:
v.normalize()
then you cannot use const expressions like this:
const vec3 v = normalize( vec3(1,2,3) );
(actually, there is both now, but that's just an example)

Minimizing sources of errors. If one form reduces errors and other form
increases them, the error-reducing form is used.


## Changes

v2.6.0 (2016-07-29):
* Cleaned up svn files
* Removed build files
* Moved sources to Github

v2.5.0 (2013-09-02):
* Visual Studio 2012 project files included
* Mersenne twister / gcc compiling warning fixes (thanks to Loren Hoffman!)
* Added scaling(vec3) (thanks to Wouter van Oortmersssen for improvement suggestions!)

v2.4.1 (2011-11-28):
* New VS2008 project files

v2.4.0 (2011-11-28):
* Added vector_simd for 32-bit SSE builds -- thanks to Wouter van Oortmerssen for the code!
* setsize -> resize, drop -> pop_back (as in std::vector), and added support for resizing the buffer bigger
* Added const-methods and fixed couple of const-correctness issues
* Added assert() about malloc alignment assumption
* Added some comments to vector_simd
* Added a few unit tests for vector_simd (not in release package but in svn)

v2.3.0 (2011-11-23):
* Thanks Wouter van Oortmerssen for feedback and suggestions!
* Added component wise scalar -= and += vec2, vec3 and vec4
* In _DEBUG uninitialized vectors are set to NaN
* Added in-place vec2, vec3, vec4 and quat normalize() (as method to avoid confusion to free function)
* Fixed 1D Gaussian blur kernel
* Added VS2010 test project and rotation tests (only in svn version, not release)
* Added vec3::rotateX, vec3::rotateY, vec3::rotateZ and vec2::rotate

v2.2.1 (2011-10-30):
* fixed bug in float_util.inl clamp (thanks to Brad Johnston!)

v2.2.0 (2011-09-26):
* doxygen doc update
* new VS2010 project file which outputs to solution directory (more convenient when using VS IDE)
* added SLMATH_AUTOLINK directive and disabled autolink by default
* quat (in)equality

v2.1.1 (2011-04-04):
* added <stddef.h> include to make sure size_t is defined

v2.1.0 (2011-04-01):
* much faster _DEBUG build
* SSE2 needs to be enabled from project settings to be enabled for 32-bit builds (too confusing if forced since requires care!)
* non-SIMD optimizations

v2.0.1 (2011-03-24):
* much faster _DEBUG build
* vec-op asserts optional (comment out SLMATH_VEC_ASSERTS from slmath_configure.h before compiling)
* optimized version of mat4*mat4 for non-SIMD platforms

v2.0.0 (2011-03-19):
* simplified naming conventions: slmath -> slm
* include e.g. <slm/vec3.h>
* namespace is slm
* immigration guide to 2.0 included

v1.8.5 (2011-03-14):
* cleaned up install instructions
* removed unnecessary non-SIMD implementations (since macros handle this already)
* inlined check(vector) check
  (thanks Wouter van Oortmerssen for the feedback!)

v1.8.4 (2011-03-09):
* improved SIMD support on 32bit builds 
  (enabled by default; prints warning about alignment)

v1.8.3 (2011-03-09):
* 4-vectors and matrices check 16-byte alignment in SIMD + _DEBUG build 

v1.8.2 (2011-03-02):
* optimizations and internal refactoring

v1.8.1 (2011-02-10):
* perspectiveFovLH fixed, [2][2] was + instead of - (thanks to Brad Johnston for finding the bug)
* added Visual Studio 2010 project files
* SIMD transpose(mat4)
* SWIG-related code removed

v1.8.0 (2010-10-21):
* VS2003 support (Thanks to Peter Ingebretson <peter at ingebretson.net>!)
* vcproj: simplified lib dir structure (lib/vs2008/x64/slmath-Debug.lib, etc.)
* random: better seed
* Moved testing framework and tests from core to separate project (samples/test)
* Moved from SourceForge to Google Code

v1.7.9 (2010-09-12):
* frameFromNormal: vec3(1,1,1) caused the function to fail (thanks Loren Hoffman for finding the bug!)
* intersectLineTri: intersection was incorrectly returned when s>1 (thanks Fernando Resco for finding the bug!)
* more vec scripting compatibility functions (add,mul,neg)

v1.7.8 (2009-09-12):
* removed overloaded scalar abs to avoid stdlib compatibility problems

v1.7.7 (2009-09-11):
* removed template based function declarations to improve SWIG compatibility
* added set members to vector classes (for SWIG scripting)
* added mat4 get/set methods (for SWIG scripting)

v1.7.6 (2009-09-04):
* Added automatic linking of correct lib (Visual Studio only)
* Test framework: Cleaned up namespace-macro usage, renamed tests -> test_slmath
* Test framework: Automatic formatting file & expression size limits
* New lib output paths: vs2008\$(PlatformName)\$(ConfigurationName)\slmath.lib
* Added SWIG interfaces and optional Lua support (see slmath/slmath_lua.h).
   Uncomment SLMATH_LUA_SUPPORT from slmath/slmath_configure.h to enable. 
   See samples/lua_test.
* VS2003 compatibility fix included (in formatTestId)

v1.7.5 (2009-05-09):
* SIMD implementation of mat4 and vec4 ops
* Random number seed/function separation cleanup, renamed Random -> random
* Removed <slm/random_util.h>, use <slm/random.h> instead
* Random number generation inlines
* Documentation cleanup

v1.7.4 (2009-05-08):
* Unified abs, max, min behavior
* Added #undef min, max, #define NOMINMAX (to avoid problems with <windows.h>)

v1.7.3 (2009-05-08):
* Added x64 builds
* Added mat4 outerProduct(vec4,vec4)

v1.7.2 (2009-05-07):
* Unified build naming conventions to avoid file conflicts
* SIMD macros to separate header, <slm/simd.h>

v1.7.1 (2009-05-04):
* Reference documentation cleanup

v1.7 (2009-05-04):
* Added SSE2/SIMD support for Visual Studio 2005+
* SIMD implementation of various mat4 and vec4 functions
* Added speed_test sample

v1.6 (2009-04-23):
* Mersenne Twister based high-quality random number generation
* added Gaussian blur kernel 1D and 2D functions (see slmath/float_util.h)

v1.5.1 (2009-02-25):
* fixed vector clamp

v1.5 (2009-02-25):
* added quat class (quaternion) and mat4(quat)
* added mat4(angle,axis)
* added intersect_util: intersectRayBox, intersectRayTriangle
* added vec3: facenormal_cw and facenormal_ccw
* added abs, min, max ('v' as vector, to avoid possible name clashes with platform C-macros)
* added mat4_util: orthoRH, orthoLH
* merged mat4_util.h -> mat4.h since the division between the two was not very intuitive
* removed assert(check(*this)) from [] op of vectors since init-assign is valid op
* size_t usage for better 64-bit compatibility

v1.4 (2009-01-16):
* x64 Release build
* added random_util: randomFloat, randomFloatSigned, randomPointOnSphere, randomPointInSphere
* random_util: added randomSamplePointsOnHemisphere, randomSamplePointsInHemisphere
* mat4_util: added fromToRotation, frameFromNormal
* vec4_util: merged

v1.3 (2009-01-08):
* Removed assumption about affine transformation in inverse(mat4)
* added mat4 compare ops
* added vec4_util

v1.2 (2008-12-04):
* vec2,3,4: added scalar division
* vec2,3,4: added saturate, clamp
* mat4: added row vector matrix multiply, cleaned up documentation.
* mat4_util: added translation(vec3)
* mat4_util: added cubeMapViewRH and cubeMapProjectionRH

v1.1 (2008-11-21):
* vec2,3,4: added constructors using lower dimension vectors
* mat4_util: added perspectiveRH, rotationX, rotationY, rotationZ, targetAtRH, lookAtRH
* vec2,3,4: added GLSL style component wise multply and division operators
* tests: added test_mat4

v1.0 (2008-06-03):
* Initial public release


## Contact

Jani Kajala
kajala (at) gmail.com


## License (MIT/BSD Compatible License)

Copyright (C) 2008-2010 Jani Kajala

Permission is hereby granted, free of charge, to any person
obtaining a copy of this software and associated documentation 
files (the "Software"), to deal in the Software without restriction, 
including without limitation the rights to use, copy, modify, merge, 
publish, distribute, sublicense, and/or sell copies of the Software, 
and to permit persons to whom the Software is furnished to do so, 
subject to the following conditions:

The above copyright notice and this permission notice shall be included 
in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, 
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF 
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. 
IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, 
DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR 
OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR 
THE USE OR OTHER DEALINGS IN THE SOFTWARE.

