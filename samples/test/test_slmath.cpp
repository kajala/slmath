#include "test_slmath.h"
#include "testframework.h"
#include <slm/slmath.h>
#include <stdio.h>
#include <string.h>

SLMATH_USING()

static float err( const mat4& a, const mat4& b )
{
	mat4 c = a - b;
	vec4 maxdiff(0.f);
	for ( int i = 0 ; i < 4 ; ++i )
		maxdiff = max( maxdiff, abs(c[i]) );
	return dot(maxdiff,vec4(1.f));
}

static bool test_vec2( char* testid )
{
	vec2 a,b;

	a = vec2(1,2);
	b = vec2(3,4);
	TEST( length(a+b-vec2(4.f,6.f)) < 1e-20f )

	a = vec2(1,2);
	b = vec2(3,4);
	TEST( length(dot(a,b)-11.0f) < 1e-20f )

	TEST( vec2(1) == vec2(1,1) );
	TEST( vec2(1) != vec2(1,0) );

	a = vec2(1,2);
	b = vec2(3,4);
	a += b;
	TEST( distance(a,vec2(4,6)) < 1e-20f );

	a -= a;
	TEST( distance(a,vec2(0,0)) < 1e-20f );

	a = vec2(1,2);
	a *= 3.f;
	TEST( distance(a,vec2(3,6)) < 1e-20f );
	return true;
}

static bool test_vec3( char* testid )
{
	vec3 a,b;

	a = vec3(1,2,4);
	b = vec3(3,4,5);
	TEST( length(a+b-vec3(4.f,6.f,9.f)) < 1e-20f )

	a = vec3(1,2,5);
	b = vec3(3,4,6);
	TEST( length(dot(a,b)-41.0f) < 1e-20f )

	TEST( vec3(1) == vec3(1,1,1) );
	TEST( vec3(1) != vec3(1,0,0) );

	a = vec3(1,2,3);
	b = vec3(3,4,5);
	a += b;
	TEST( distance(a,vec3(4,6,8)) < 1e-20f );

	a -= a;
	TEST( distance(a,vec3(0,0,0)) < 1e-20f );

	a = vec3(1,2,3);
	a *= 3.f;
	TEST( distance(a,vec3(3,6,9)) < 1e-20f );
	return true;
}

static bool test_mat4( char* testid )
{
	// set device transformations
	mat4 proj = perspectiveFovRH( radians(60.f), 640.f/480.f, 0.01f, 10000.f );
	mat4 view = lookAtRH( vec3(0,0,200), vec3(0,0,0), vec3(0,1,0) );
	mat4 world(1);

	// test: transform a point with world-view-projection transform
	vec4 v0 = vec4( -43.9114f, -39.4834f, 0.f, 1.f );
	mat4 wvp = proj * (view * world);
	vec4 v1 = wvp * v0;
	v1 *= 1.f/v1.w;
	vec4 v2 = proj * view * world * v0;
	v2 = v2 * 1.f/v2.w;
	TEST( distance(v1,v2) < 1e-6f );
	TEST( length(v1-v2) < 1e-6f );
	vec4 d = vec4(-0.285213f,-0.341936f,0.999951f,1.f) - v1;
	float dlen = length(d);
	TEST( dlen < 1e-5f );

	// fromToRotation
	v0 = vec4(1,0,0,0);
	v1 = vec4(0,1,0,0);
	vec4 v1b = fromToRotation( v0.xyz(), v1.xyz() ) * v0;
	TEST( distance(v1,v1b) < 1e-5f );
	v0 = vec4(1,0,0,0);
	v1 = vec4(0,0,1,0);
	v1b = fromToRotation( v0.xyz(), v1.xyz() ) * v0;
	TEST( distance(v1,v1b) < 1e-5f );

	// determinant
	random randval( 1234 );
	mat4 det_m0;
	for ( size_t i = 0 ; i < 16 ; ++i )
		det_m0[i/4][i&3] = randomFloat(randval);
	assert( fabsf(0.021621689f-det(det_m0)) < 1e-12f );

	// inverse
	const mat4 m = translation( vec3(1.f,2.f,3.f) ) * fromToRotation( normalize(vec3(1.f,2.f,3.f)), normalize(vec3(4.f,1.f,3.f)) );
	const mat4 im = inverse(m);

	// test: angle-axis
	mat4 r1( radians(80.f), vec3(1,0,0) );
	mat4 r1b = rotationX( radians(80.f) );
	mat4 r2( radians(80.f), vec3(0,1,0) );
	mat4 r2b = rotationY( radians(80.f) );
	mat4 r3( radians(80.f), vec3(0,0,1) );
	mat4 r3b = rotationZ( radians(80.f) );
	TEST( err(r1,r1b) < 1e-3f );
	TEST( err(r2,r2b) < 1e-3f );
	TEST( err(r3,r3b) < 1e-3f );

	return true;
}

static bool test_rand( char* testid )
{
	size_t slots[101];
	memset( slots, 0, sizeof(slots) );
	random seed = 1;
	const size_t n = 100000;
	for ( size_t i = 0 ; i < n ; ++i )
	{
		float x = seed.randomFloat();
		size_t s = size_t(x*100.f);
		slots[s] += 1;
	}
	//printf( "Distribution (n=%d):\n", n );
	const size_t avg = (n/100);
	for ( size_t i = 0 ; i < 100 ; ++i )
	{
		size_t dist = ::abs( int(slots[i]-avg) );
		//printf( "[%d] = %d, dist(avg)=%d\n", i, slots[i], dist );
		TEST( dist < avg/10 );
	}
	return true;
}

static bool test_quat( char* testid )
{
	// angle-axis <-> mat4 test
	// quat mul vs mat4 mul test
	float rad = radians(80.f);
	quat a( rad, vec3(1,0,0) );
	quat b( rad, vec3(0,1,0) );
	quat c = a*b;
	mat4 a2( rad, vec3(1,0,0) );
	mat4 b2( rad, vec3(0,1,0) );
	mat4 c2 = a2*b2;
	mat4 c2b( c );
	float e = err( c2, c2b );
	TEST( e < 1e-5f );

	// test quat -> angle-axis
	vec3 axis[2];
	float angle[2];
	toAngleAxis(a,&angle[0],&axis[0]);
	toAngleAxis(b,&angle[1],&axis[1]);
	TEST( fabsf(angle[0]-radians(80.f)) < 1e-3f );
	TEST( fabsf(angle[1]-radians(80.f)) < 1e-3f );
	TEST( distance(axis[0],vec3(1,0,0)) < 1e-3f );
	TEST( distance(axis[1],vec3(0,1,0)) < 1e-3f );
	return true;
}

bool test_gaussian( char* testid )
{
	const float OK_KERNEL_2D[7*7] = 
	{
		0.00000f, 0.00002f, 0.00019f, 0.00039f, 0.00019f, 0.00002f, 0.00000f, 
		0.00002f, 0.00079f, 0.00656f, 0.01330f, 0.00656f, 0.00079f, 0.00002f, 
		0.00019f, 0.00656f, 0.05472f, 0.11098f, 0.05472f, 0.00656f, 0.00019f, 
		0.00039f, 0.01330f, 0.11098f, 0.22508f, 0.11098f, 0.01330f, 0.00039f, 
		0.00019f, 0.00656f, 0.05472f, 0.11098f, 0.05472f, 0.00656f, 0.00019f, 
		0.00002f, 0.00079f, 0.00656f, 0.01330f, 0.00656f, 0.00079f, 0.00002f, 
		0.00000f, 0.00002f, 0.00019f, 0.00039f, 0.00019f, 0.00002f, 0.00000f, 
	};

	const float OK_KERNEL_1D[7] = {0.00082f, 0.02804f, 0.23393f, 0.47443f, 0.23393f, 0.02804f, 0.00082f, };

	const int r = 3;
	const int s = (2*r+1);
	float kernel[s*s];
	getGaussianBlurKernel2D( s, 0.84089642f, &kernel[0] );
	for ( size_t i = 0 ; i < size_t(s*s) ; ++i )
		TEST( fabsf(OK_KERNEL_2D[i]-kernel[i]) < 1e-5f );

	getGaussianBlurKernel1D( s, 0.84089642f, &kernel[0] );
	for ( size_t i = 0 ; i < size_t(s) ; ++i )
		TEST( fabsf(OK_KERNEL_1D[i]-kernel[i]) < 1e-5f );

	// generate test kernels
	/*printf( "\nfloat OK_KERNEL_2D[%d*%d] = {", s, s );
	for ( int j = 0 ; j < s ; ++j )
	{
		for ( int i = 0 ; i < s ; ++i )
			printf( "%.5ff, ", kernel[j*s+i] );
		printf( "\n" );
	}
	printf( "};\n" );

	getGaussianBlurKernel1D( s, 0.84089642f, &kernel[0], s );
	printf( "\nfloat OK_KERNEL_1D[%d] = {", s );
	for ( int j = 0 ; j < s ; ++j )
	{
		printf( "%.5ff, ", kernel[j] );
	}
	printf( "};\n" );*/

	return true;
}

bool test_rotations( char* testid )
{
	float a = radians(45.f);
	vec4 v = vec4(10,10,0,0);
	vec3 r1 = rotateZ( v.xyz(), a );
	vec3 r2 = (rotationZ(a) * v).xyz();
	TEST( length(r1-r2) < 1e-5f );
	r1 = rotateX( v.xyz(), a );
	r2 = (rotationX(a) * v).xyz();
	TEST( length(r1-r2) < 1e-5f );
	r1 = rotateY( v.xyz(), a );
	r2 = (rotationY(a) * v).xyz();
	TEST( length(r1-r2) < 1e-5f );
	return true;
}

bool test_vector_sse( char* testid )
{
	vector_simd<vec4> buf;
	TEST( buf.size() == 0 );
	TEST( buf.empty() );
	buf.push_back( vec4(1,2,3,4) );
	TEST( buf.front() == buf.back() );
	TEST( buf[0] == buf.back() );
	TEST( buf[0] == *buf.begin() );
	buf.resize( 2 );
	TEST( buf.size() == 2 );
	buf[1] = vec4(5,6,7,8);
	buf.pop_back();
	TEST( buf.size() == 1 );
	return true;
}

bool test_slmath( char* testid )
{
	TEST( test_vec2(testid) );
	TEST( test_vec3(testid) );
	TEST( test_mat4(testid) );
	TEST( test_rand(testid) );
	TEST( test_quat(testid) );
	TEST( test_gaussian(testid) );
	TEST( test_rotations(testid) );
	TEST( test_vector_sse(testid) );
	return true;
}

// This file is part of 'slmath' C++ library. Copyright (C) 2009 Jani Kajala (kajala@gmail.com). See http://sourceforge.net/projects/slmath/
