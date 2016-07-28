#include <slm/slmath.h>
#include <stdio.h>
#include <vector>
#include <new>

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

struct A
{
	int			a;
	slm::vec4	x;
};

// __rdtsc() in <intrin.h>
// timeGetTime() in <windows.h>/<mmsystem.h>
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <mmsystem.h>
#include <intrin.h>
#pragma comment( lib, "winmm" )

// MSVC doesnt have stdint.h
#ifdef _MSC_VER
typedef unsigned __int64 uint64_t;
#else
#include <stdint.h>
#endif

/** Clock cycles / sec */
static double s_freqInv = 1.0;

/** Returns CPU time stamp. */
static uint64_t now()
{
	return __rdtsc();
}

/** Returns time difference between two last tick() calls. */
static double tick()
{
	static uint64_t t0 = 0;
	if ( !t0 )
		t0 = now();
	uint64_t t1 = now();
	uint64_t dt = t1 - t0;
	t0 = t1;
	return double(dt) * s_freqInv;
}

/** Returns millions of ops / sec */
double opsM( double t, uint64_t n )
{
	assert( t > FLT_MIN );
	return double(n)/double(t)/1e6;
}

/** Helper determinant calculation for non-slm libs */
template <class T> float det4( const T& m )
{
	return
		m(0,3) * m(1,2) * m(2,1) * m(3,0) - m(0,2) * m(1,3) * m(2,1) * m(3,0) - m(0,3) * m(1,1) * m(2,2) * m(3,0)+m(0,1) * m(1,3) * m(2,2) * m(3,0) +
		m(0,2) * m(1,1) * m(2,3) * m(3,0) - m(0,1) * m(1,2) * m(2,3) * m(3,0) - m(0,3) * m(1,2) * m(2,0) * m(3,1)+m(0,2) * m(1,3) * m(2,0) * m(3,1) +
		m(0,3) * m(1,0) * m(2,2) * m(3,1) - m(0,0) * m(1,3) * m(2,2) * m(3,1) - m(0,2) * m(1,0) * m(2,3) * m(3,1)+m(0,0) * m(1,2) * m(2,3) * m(3,1) +
		m(0,3) * m(1,1) * m(2,0) * m(3,2) - m(0,1) * m(1,3) * m(2,0) * m(3,2) - m(0,3) * m(1,0) * m(2,1) * m(3,2)+m(0,0) * m(1,3) * m(2,1) * m(3,2) +
		m(0,1) * m(1,0) * m(2,3) * m(3,2) - m(0,0) * m(1,1) * m(2,3) * m(3,2) - m(0,2) * m(1,1) * m(2,0) * m(3,3)+m(0,1) * m(1,2) * m(2,0) * m(3,3) +
		m(0,2) * m(1,0) * m(2,1) * m(3,3) - m(0,0) * m(1,2) * m(2,1) * m(3,3) - m(0,1) * m(1,0) * m(2,2) * m(3,3)+m(0,0) * m(1,1) * m(2,2) * m(3,3);
}

SLMATH_USING()

// optional Eigen benchmarking
//#define TEST_EIGEN
#ifdef TEST_EIGEN
#define EIGEN_SSE2_ON_MSVC_2008_OR_LATER
#include <Eigen/Core>

using namespace Eigen;

inline Matrix4f toEigen( const slm::mat4& m ) {Matrix4f b1; for ( int i = 0 ; i < 4 ; ++i ) for ( int j = 0 ; j < 4 ; ++j ) b1(i,j) = m[i][j];return b1;}
#endif

// optional XNA Math benchmarking
#ifdef _M_X64
#define TEST_XNA
#endif
#ifdef TEST_XNA
#include <xnamath.h>

inline XMMATRIX toXNA( const slm::mat4& m ) {XMMATRIX b1; for ( int i = 0 ; i < 4 ; ++i ) for ( int j = 0 ; j < 4 ; ++j ) b1(i,j) = m[i][j];return b1;}
inline XMVECTOR toXNA( const slm::vec4& v ) {return *(XMVECTOR*)&v;}
#endif


// vector += scalar * vector
void test1()
{
#ifdef _DEBUG
	const size_t n_reps = 1000;
#else
	const size_t n_reps = 10000;
#endif

	printf( "--------------------------------------------------------------\n" );
	printf( "vector += scalar * vector\n--------------------------------------------------------------\n" );
	for ( int n = 0 ; n < 3 ; ++n )
	{
#ifdef TEST_XNA
		{
			XMVECTOR y0 = toXNA( vec4(0) );
			const XMVECTOR x0 = toXNA( vec4(1.0012345f) );
			const float alpha1 = 1.000001f;
			tick();
			for ( size_t i = 0 ; i < n_reps ; ++i )
			{
				y0 += alpha1 * x0;
			}
			double time1 = tick();
			XMVECTORF32 res = *(XMVECTORF32*)&XMVector4Length(y0);
			printf( "  ops (XNAMath) = %.1f (checksum=%g)\n", opsM(time1,n_reps), res.f[0] );
		}
#endif

		{
			vec4 y0 = vec4(0);
			const vec4 x0 = vec4(1.0012345f);
			const float alpha0 = 1.000001f;
			tick();
			for ( size_t i = 0 ; i < n_reps ; ++i )
			{
				y0 += alpha0 * x0;
			}
			double time0 = tick();
			printf( "  ops (slm) = %.1fM (checksum=%g)\n", opsM(time0,n_reps), length(y0) );
		}

#ifdef TEST_EIGEN
		{
			Vector4f y1; y1[0] = 0.f; y1[1] = 0.f; y1[2] = 0.f; y1[3] = 0.f;
			Vector4f x1; x1[0] = 1.0012345f; x1[1] = 1.0012345f; x1[2] = 1.0012345f; x1[3] = 1.0012345f;
			const float alpha1 = 1.000001f;
			tick();
			for ( size_t i = 0 ; i < n_reps ; ++i )
			{
				y1 += alpha1 * x1;
			}
			double time1 = tick();
			printf( "  ops (Eigen) = %.1f (checksum=%g)\n", opsM(time1,n_reps), sqrtf(y1.dot(y1)) );
		}
#endif
	}
}

// matrix * matrix * matrix
void test2()
{
#ifdef _DEBUG
	const size_t n_reps = 1000;
#else
	const size_t n_reps = 10000;
#endif

	printf( "--------------------------------------------------------------\n" );
	printf( "matrix * matrix * matrix\n-------------------------------------------------\n" );
	for ( int n = 0 ; n < 3 ; ++n )
	{
		const mat4 a0 = rotationX( 1.0012345f ) * rotationY( 1.0012345f );

#ifdef TEST_XNA
		{
			XMMATRIX a0_ = toXNA(a0);
			XMMATRIX a1 = a0_;
			XMMATRIX b1 = a0_;
			tick();
			for ( size_t i = 0 ; i < n_reps ; ++i )
			{
				b1 = a1 * b1 * a1;
				//b1 = XMMatrixTranspose(b1);
				if ( ((i+1) & 255) == 0 )
					b1 = a0_;
			}
			double time1 = tick();
			printf( "  ops (XNAMath) = %.1f (checksum=%g)\n", opsM(time1,n_reps), det4(b1) );
		}
#endif

		{
			mat4 b0 = a0;
			tick();
			for ( size_t i = 0 ; i < n_reps ; ++i )
			{
				b0 = a0 * b0 * a0;
				//b0 = transpose(b0);
				if ( ((i+1) & 255) == 0 )
					b0 = a0;
			}
			double time0 = tick();
			printf( "  ops (slm) = %.1fM (checksum=%g)\n", opsM(time0,n_reps), det(b0) );
		}

#ifdef TEST_EIGEN
		{
			Matrix4f a1 = toEigen(a0);
			Matrix4f b1 = toEigen(a0);
			tick();
			for ( size_t i = 0 ; i < n_reps ; ++i )
			{
				b1 = (a1 * b1 * a1).transpose();
				if ( ((i+1) & 255) == 0 )
					b1 = toEigen(a0);
			}
			float time1 = tick();
			printf( "  ops (Eigen) = %.1f (checksum=%g)\n", opsM(time1,n_reps), det4(b1) );
		}
#endif
	}
}

// inverse(matrix)
void test3()
{
#ifdef _DEBUG
	const size_t n_reps = 1000;
#else
	const size_t n_reps = 10000;
#endif

	printf( "--------------------------------------------------------------\n" );
	printf( "inverse(matrix)\n--------------------------------------------------------------\n" );
	for ( int n = 0 ; n < 3 ; ++n )
	{
		const mat4 a0 = rotationX( 1.0012345f ) * rotationY( 1.0012345f );
		float sumx;

#ifdef TEST_XNA
		{
			XMMATRIX a0_ = toXNA(a0);
			XMMATRIX a1 = a0_;
			XMMATRIX b1 = a0_;
			sumx = 0.f;
			tick();
			for ( size_t i = 0 ; i < n_reps ; ++i )
			{
				XMVECTOR det;
				b1 = XMMatrixInverse(&det,a0_);
				sumx += b1.m[0][0];
			}
			double time1 = tick();
			printf( "  ops (XNAMath) = %.1f (checksum=%g)\n", opsM(time1,n_reps), sumx );
		}
#endif

		{
			mat4 b0 = a0;
			sumx = 0.f;
			tick();
			for ( size_t i = 0 ; i < n_reps ; ++i )
			{
				b0 = inverse(a0);
				sumx += b0[0][0];
			}
			double time0 = tick();
			printf( "  ops (slm) = %.1fM (checksum=%g)\n", opsM(time0,n_reps), sumx );
		}
	}
}

void testMemAlign()
{
	// memory alignment test
	std::vector<char*> allocs;
	std::vector<A*> allocs2;
	const size_t repcount = 100000;
	for ( size_t i = 0 ; i < repcount ; ++i )
	{
		if ( !(i&1) )
		{
			char* mem = new char[ 100 + (rand() % 100 + 1) ];
			allocs.push_back(mem);
		}
		else
		{
			A* obj = new A();
			allocs2.push_back(obj);
			obj->x = vec4(1.f,2.f,3.f,4.f);
			obj->x *= 2.f;
			obj = obj;
		}

		if ( allocs.size() > 100 || i+1 == repcount )
		{
			while ( allocs.size() > 0 )
			{
				delete[] allocs.back();
				allocs.pop_back();
			}
			while ( allocs2.size() > 0 )
			{
				delete allocs2.back();
				allocs2.pop_back();
			}
		}
	}
}

int main( int argc, char* argv[] )
{
	if ( !isValidCPU() )
	{
		printf( "slm was compiled for CPU features not supported by this platform\n" );
		return 1;
	}

	testMemAlign();

	// init s_freqInv
	uint64_t freq0 = now();
	DWORD t0 = timeGetTime();
	double secs = 5.0;
	while ( timeGetTime()-t0 < DWORD(secs*1000.0) )
	{
	}
	s_freqInv = secs / (now()-freq0);

	test1();
	test2();
	//test3();
	return 0;
}

