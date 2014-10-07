// ****************************************************************************************
//
// Fubi Math Functions
// ---------------------------------------------------------
// Copyright (C) 2010-2013 Felix Kistler 
// 
// This software is distributed under the terms of the Eclipse Public License v1.0.
// A copy of the license may be obtained at: http://www.eclipse.org/org/documents/epl-v10.html
// 
// ****************************************************************************************
#pragma once

/** \file FubiConfig.h 
 * \brief a header file containing defines for configuring the Fubi build
*/ 

#include "FubiUtils.h"
#include <cmath>

namespace Fubi
{
	/** \addtogroup FUBIMATH FUBI Math library
	* Math structures and functions such as vectors, matrices, ...
	* 
	* @{
	*/

	// The following is a modified version of the utMath.h within the Horde3D Engine by Nicolas Schulz (http://www.horde3d.org). 
	// Horde3D namespace and some unnescessary functions are removed.
	// Some special vector functions are added

	// *************************************************************************************************
	//
	// Horde3D
	//   Next-Generation Graphics Engine
	// --------------------------------------
	// Copyright (C) 2006-2011 Nicolas Schulz
	//
	// This software is distributed under the terms of the Eclipse Public License v1.0.
	// A copy of the license may be obtained at: http://www.eclipse.org/legal/epl-v10.html
	//
	// *************************************************************************************************

	// -------------------------------------------------------------------------------------------------
	//
	// Math library
	//
	// Conventions:
	//
	// - Coordinate system is right-handed with positive y as up axis
	// - All rotation angles are counter-clockwise when looking from the positive end of the rotation
	//	 axis towards the origin
	// - An unrotated view vector points along the negative z-axis
	//
	// -------------------------------------------------------------------------------------------------


	/**
	* \namespace Math
	*
	* \brief Contains math constants, e.g. min and max values for different data types, Pi, Epsilon, NaN, and the NO_INIT hint
	*
	*/
	namespace Math
	{
		const unsigned int MaxUInt32 = 0xFFFFFFFF;
		const unsigned short MaxUShort16 = 0xFFFF;
		const int MinInt32 = 0x80000000;
		const int MaxInt32 = 0x7FFFFFFF;
		const float MaxFloat = 3.402823466e+38F;
		const float MinPosFloat = 1.175494351e-38F;

		const float Pi = 3.141592654f;
		const float TwoPi = 6.283185307f;
		const float PiHalf = 1.570796327f;

		const float Epsilon = 0.000001f;
		const float ZeroEpsilon = 32.0f * MinPosFloat;  // Very small epsilon for checking against 0.0f
#ifdef __GNUC__
		const float NaN = __builtin_nanf("");
#else
		const float NaN = *(float *)&MaxUInt32;
#endif

		enum NoInitHint
		{
			NO_INIT
		};
	};


	// -------------------------------------------------------------------------------------------------
	// General
	// -------------------------------------------------------------------------------------------------

	/**
	* \brief Converts degrees to radians
	*/
	static inline float degToRad( float f ) 
	{
		return f * 0.017453293f;
	}

	/**
	* \brief Converts radians to degrees
	*/
	static inline float radToDeg( float f ) 
	{
		return f * 57.29577951f;
	}

	/**
	* \brief Restricts a value to a certain minimum and maximum
	*/
	static inline float clampf( float f, float min, float max )
	{
		if( f < min ) f = min;
		else if( f > max ) f = max;

		return f;
	}

	/**
	* \brief Returns the minimum
	*/
	static inline float minf( float a, float b )
	{
		return a < b ? a : b;
	}

	/**
	* \brief Returns the maximum
	*/
	static inline float maxf( float a, float b )
	{
		return a > b ? a : b;
	}

	/**
	* \brief Returns a if test >= 0, else b
	*/
	static inline float fsel( float test, float a, float b )
	{
		// Branchless selection
		return test >= 0 ? a : b;
	}


	// -------------------------------------------------------------------------------------------------
	// Conversion
	// -------------------------------------------------------------------------------------------------

	/**
	* \brief Float to int conversion using truncation
	*/
	static inline int ftoi_t( double val )
	{

		return (int)val;
	}

	/**
	* \brief Fast round (banker's round) using Sree Kotay's method
	* This function is much faster than a naive cast from float to int
	*/
	static inline int ftoi_r( double val )
	{
		union
		{
			double dval;
			int ival[2];
		} u;

		u.dval = val + 6755399441055744.0;  // Magic number: 2^52 * 1.5;
		return u.ival[0];         // Needs to be [1] for big-endian
	}


	/**
	* \brief Vector class with three components
	*/
	class Vec3f
	{
	public:
		float x, y, z;


		// ------------
		// Constructors
		// ------------
		Vec3f() : x( 0.0f ), y( 0.0f ), z( 0.0f )
		{ 
		}

		explicit Vec3f( Math::NoInitHint )
		{
			// Constructor without default initialization
		}

		Vec3f( const float x, const float y, const float z ) : x( x ), y( y ), z( z ) 
		{
		}

		Vec3f( const Vec3f &v ) : x( v.x ), y( v.y ), z( v.z )
		{
		}

		// ------
		// Access
		// ------
		float &operator[]( unsigned int index )
		{
			return *(&x + index);
		}

		// -----------
		// Comparisons
		// -----------
		bool operator==( const Vec3f &v ) const
		{
			return (x > v.x - Math::Epsilon && x < v.x + Math::Epsilon && 
				y > v.y - Math::Epsilon && y < v.y + Math::Epsilon &&
				z > v.z - Math::Epsilon && z < v.z + Math::Epsilon);
		}

		bool operator!=( const Vec3f &v ) const
		{
			return (x < v.x - Math::Epsilon || x > v.x + Math::Epsilon || 
				y < v.y - Math::Epsilon || y > v.y + Math::Epsilon ||
				z < v.z - Math::Epsilon || z > v.z + Math::Epsilon);
		}

		// ---------------------
		// Arithmetic operations
		// ---------------------
		Vec3f operator-() const
		{
			return Vec3f( -x, -y, -z );
		}

		Vec3f operator+( const Vec3f &v ) const
		{
			return Vec3f( x + v.x, y + v.y, z + v.z );
		}

		Vec3f &operator+=( const Vec3f &v )
		{
			return *this = *this + v;
		}

		Vec3f operator-( const Vec3f &v ) const 
		{
			return Vec3f( x - v.x, y - v.y, z - v.z );
		}

		Vec3f &operator-=( const Vec3f &v )
		{
			return *this = *this - v;
		}

		Vec3f operator*( const float f ) const
		{
			return Vec3f( x * f, y * f, z * f );
		}

		Vec3f &operator*=( const float f )
		{
			return *this = *this * f;
		}

		Vec3f operator/( const float f ) const
		{
			return Vec3f( x / f, y / f, z / f );
		}

		Vec3f &operator/=( const float f )
		{
			return *this = *this / f;
		}

		Vec3f operator*(const Vec3f &v) const
		{
			return Vec3f( x * v.x, y * v.y, z * v.z);
		}

		Vec3f &operator*=(const Vec3f &v)
		{
			return *this = *this * v;
		}

		Vec3f operator/(const Vec3f &v) const
		{
			return Vec3f( x / v.x, y / v.y, z / v.z);
		}

		Vec3f &operator/=(const Vec3f &v)
		{
			return *this = *this / v;
		}

		// ----------------
		// Special products
		// ----------------
		float dot( const Vec3f &v ) const
		{
			return x * v.x + y * v.y + z * v.z;
		}

		Vec3f cross( const Vec3f &v ) const
		{
			return Vec3f( y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x );
		}

		// ----------------
		// Other operations
		// ----------------
		float length() const 
		{
			return sqrtf( x * x + y * y + z * z );
		}

		Vec3f normalized() const
		{
			float invLen = 1.0f / length();
			return Vec3f( x * invLen, y * invLen, z * invLen );
		}

		void normalize()
		{
			float invLen = 1.0f / length();
			x *= invLen;
			y *= invLen;
			z *= invLen;
		}

		/*void fromRotation( float angleX, float angleY )
		{
		x = cosf( angleX ) * sinf( angleY ); 
		y = -sinf( angleX );
		z = cosf( angleX ) * cosf( angleY );
		}*/

		Vec3f toRotation() const
		{
			// Assumes that the unrotated view vector is (0, 0, -1)
			Vec3f v;

			if( y != 0 ) v.x = atan2f( y, sqrtf( x*x + z*z ) );
			if( x != 0 || z != 0 ) v.y = atan2f( -x, -z );

			return v;
		}

		Vec3f lerp( const Vec3f &v, float f ) const
		{
			return Vec3f( x + (v.x - x) * f, y + (v.y - y) * f, z + (v.z - z) * f ); 
		}
	};

   /**
	* \brief Converts radians to degrees for a Vec3f
	*/
	static inline void radToDeg( Vec3f& v ) 
	{
		v *= 57.29577951f;
	}
	/**
	* \brief Converts degrees to radians for a Vec3f
	*/
	static inline void degToRad( Vec3f& v ) 
	{
		v *= 0.017453293f;
	}

	/**
	* \brief Vector class with four components
	*/
	class Vec4f
	{
	public:

		float x, y, z, w;


		Vec4f() : x( 0 ), y( 0 ), z( 0 ), w( 0 )
		{
		}

		explicit Vec4f( const float x, const float y, const float z, const float w ) :
		x( x ), y( y ), z( z ), w( w )
		{
		}

		explicit Vec4f( Vec3f v ) : x( v.x ), y( v.y ), z( v.z ), w( 1.0f )
		{
		}

		Vec4f operator+( const Vec4f &v ) const
		{
			return Vec4f( x + v.x, y + v.y, z + v.z, w + v.w );
		}

		Vec4f operator*( const float f ) const
		{
			return Vec4f( x * f, y * f, z * f, w * f );
		}
	};


	/**
	* \brief Quaternion class (for representing orientations)
	*/
	class Quaternion
	{
	public:	

		float x, y, z, w;

		// ------------
		// Constructors
		// ------------
		Quaternion() : x( 0.0f ), y( 0.0f ), z( 0.0f ), w( 0.0f ) 
		{ 
		}

		explicit Quaternion( const float x, const float y, const float z, const float w ) :
		x( x ), y( y ), z( z ), w( w )
		{
		}

		Quaternion( const float eulerX, const float eulerY, const float eulerZ )
		{
			Quaternion roll( sinf( eulerX * 0.5f ), 0, 0, cosf( eulerX * 0.5f ) );
			Quaternion pitch( 0, sinf( eulerY * 0.5f ), 0, cosf( eulerY * 0.5f ) );
			Quaternion yaw( 0, 0, sinf( eulerZ * 0.5f ), cosf( eulerZ * 0.5f ) );

			// Order: y * x * z
			*this = pitch * roll * yaw;
		}

		// ---------------------
		// Arithmetic operations
		// ---------------------
		Quaternion operator*( const Quaternion &q ) const
		{
			return Quaternion(
				y * q.z - z * q.y + q.x * w + x * q.w,
				z * q.x - x * q.z + q.y * w + y * q.w,
				x * q.y - y * q.x + q.z * w + z * q.w,
				w * q.w - (x * q.x + y * q.y + z * q.z) );
		}

		Quaternion &operator*=( const Quaternion &q )
		{
			return *this = *this * q;
		}

		// ----------------
		// Other operations
		// ----------------

		Quaternion slerp( const Quaternion &q, const float t ) const
		{
			// Spherical linear interpolation between two quaternions
			// Note: SLERP is not commutative

			Quaternion q1( q );

			// Calculate cosine
			float cosTheta = x * q.x + y * q.y + z * q.z + w * q.w;

			// Use the shortest path
			if( cosTheta < 0 )
			{
				cosTheta = -cosTheta; 
				q1.x = -q.x; q1.y = -q.y;
				q1.z = -q.z; q1.w = -q.w;
			}

			// Initialize with linear interpolation
			float scale0 = 1 - t, scale1 = t;

			// Use spherical interpolation only if the quaternions are not very close
			if( (1 - cosTheta) > 0.001f )
			{
				// SLERP
				float theta = acosf( cosTheta );
				float sinTheta = sinf( theta );
				scale0 = sinf( (1 - t) * theta ) / sinTheta;
				scale1 = sinf( t * theta ) / sinTheta;
			} 

			// Calculate final quaternion
			return Quaternion( x * scale0 + q1.x * scale1, y * scale0 + q1.y * scale1,
				z * scale0 + q1.z * scale1, w * scale0 + q1.w * scale1 );
		}

		Quaternion nlerp( const Quaternion &q, const float t ) const
		{
			// Normalized linear quaternion interpolation
			// Note: NLERP is faster than SLERP and commutative but does not yield constant velocity

			Quaternion qt;
			float cosTheta = x * q.x + y * q.y + z * q.z + w * q.w;

			// Use the shortest path and interpolate linearly
			if( cosTheta < 0 )
				qt = Quaternion( x + (-q.x - x) * t, y + (-q.y - y) * t,
				z + (-q.z - z) * t, w + (-q.w - w) * t );
			else
				qt = Quaternion( x + (q.x - x) * t, y + (q.y - y) * t,
				z + (q.z - z) * t, w + (q.w - w) * t );

			// Return normalized quaternion
			float invLen = 1.0f / sqrtf( qt.x * qt.x + qt.y * qt.y + qt.z * qt.z + qt.w * qt.w );
			return Quaternion( qt.x * invLen, qt.y * invLen, qt.z * invLen, qt.w * invLen );
		}

		Quaternion inverted() const
		{
			float len = x * x + y * y + z * z + w * w;
			if( len > 0 )
			{
				float invLen = 1.0f / len;
				return Quaternion( -x * invLen, -y * invLen, -z * invLen, w * invLen );
			}
			else return Quaternion();
		}
	};


	/**
	* \brief Matrix class with 3x3 layout (rotation matrices)
	*/
	class Matrix3f
	{
	public:
		union
		{
			float c[3][3];
			float x[9];
		};

		static Matrix3f RotMat( float x, float y, float z )
		{
			// Rotation order: YXZ [* Vector]
			return Matrix3f( Quaternion( x, y, z ) );
		}
		static Matrix3f RotMat( const Vec3f rot )
		{
			// Rotation order: YXZ [* Vector]
			return Matrix3f( Quaternion( rot.x, rot.y, rot.z ) );
		}
		// ------------
		// Constructors
		// ------------
		Matrix3f()
		{
			c[0][0] = 1; c[1][0] = 0; c[2][0] = 0;
			c[0][1] = 0; c[1][1] = 1; c[2][1] = 0;
			c[0][2] = 0; c[1][2] = 0; c[2][2] = 1;
		}

		explicit Matrix3f( Math::NoInitHint )
		{
			// Constructor without default initialization
		}

		Matrix3f( const float *floatArray9 )
		{
			for( unsigned int i = 0; i < 3; ++i )
			{
				for( unsigned int j = 0; j < 3; ++j )
				{
					c[i][j] = floatArray9[i * 3 + j];
				}
			}
		}

		Matrix3f( const Quaternion &q )
		{
			// Calculate coefficients
			float x2 = q.x + q.x, y2 = q.y + q.y, z2 = q.z + q.z;
			float xx = q.x * x2,  xy = q.x * y2,  xz = q.x * z2;
			float yy = q.y * y2,  yz = q.y * z2,  zz = q.z * z2;
			float wx = q.w * x2,  wy = q.w * y2,  wz = q.w * z2;

			c[0][0] = 1 - (yy + zz);  c[1][0] = xy - wz;		c[2][0] = xz + wy;
			c[0][1] = xy + wz;        c[1][1] = 1 - (xx + zz);	c[2][1] = yz - wx;
			c[0][2] = xz - wy;        c[1][2] = yz + wx;		c[2][2] = 1 - (xx + yy);
		}

		// ----------
		// Matrix sum
		// ----------
		Matrix3f operator+( const Matrix3f &m ) const 
		{
			Matrix3f mf( Math::NO_INIT );

			mf.x[0] = x[0] + m.x[0];
			mf.x[1] = x[1] + m.x[1];
			mf.x[2] = x[2] + m.x[2];
			mf.x[3] = x[3] + m.x[3];
			mf.x[4] = x[4] + m.x[4];
			mf.x[5] = x[5] + m.x[5];
			mf.x[6] = x[6] + m.x[6];
			mf.x[7] = x[7] + m.x[7];
			mf.x[8] = x[8] + m.x[8];

			return mf;
		}

		Matrix3f &operator+=( const Matrix3f &m )
		{
			return *this = *this + m;
		}

		// ---------------------
		// Matrix multiplication
		// ---------------------
		Matrix3f operator*( const Matrix3f &m ) const 
		{
			Matrix3f mf( Math::NO_INIT );

			mf.x[0] = x[0] * m.x[0] + x[3] * m.x[1] + x[6] * m.x[2];
			mf.x[1] = x[1] * m.x[0] + x[4] * m.x[1] + x[7] * m.x[2];
			mf.x[2] = x[2] * m.x[0] + x[5] * m.x[1] + x[8] * m.x[2];

			mf.x[3] = x[0] * m.x[3] + x[3] * m.x[4] + x[6] * m.x[5];
			mf.x[4] = x[1] * m.x[3] + x[4] * m.x[4] + x[7] * m.x[5];
			mf.x[5] = x[2] * m.x[3] + x[5] * m.x[4] + x[8] * m.x[5];

			mf.x[6] = x[0] * m.x[6] + x[3] * m.x[7] + x[6] * m.x[8];
			mf.x[7] = x[1] * m.x[6] + x[4] * m.x[7] + x[7] * m.x[8];
			mf.x[8] = x[2] * m.x[6] + x[5] * m.x[7] + x[8] * m.x[8];

			return mf;
		}

		Matrix3f operator*( const float f ) const
		{
			Matrix3f m( *this );

			m.x[0]  *= f; m.x[1]  *= f; m.x[2]  *= f; m.x[3]  *= f;
			m.x[4]  *= f; m.x[5]  *= f; m.x[6]  *= f; m.x[7]  *= f;
			m.x[8]  *= f; 

			return m;
		}

		// ----------------------------
		// Vector-Matrix multiplication
		// ----------------------------
		Vec3f operator*( const Vec3f &v ) const
		{
			return Vec3f( v.x * c[0][0] + v.y * c[1][0] + v.z * c[2][0],
				v.x * c[0][1] + v.y * c[1][1] + v.z * c[2][1],
				v.x * c[0][2] + v.y * c[1][2] + v.z * c[2][2] );
		}


		// ---------------
		// Transformations
		// ---------------
		void rotate( const float x, const float y, const float z )
		{
			*this = RotMat( x, y, z ) * *this;
		}

		// ---------------
		// Other
		// ---------------

		Matrix3f transposed() const
		{
			Matrix3f m( *this );

			for( unsigned int y = 0; y < 3; ++y )
			{
				for( unsigned int x = y + 1; x < 3; ++x ) 
				{
					float tmp = m.c[x][y];
					m.c[x][y] = m.c[y][x];
					m.c[y][x] = tmp;
				}
			}

			return m;
		}

		float determinant() const
		{
			return 
				c[0][0]*c[1][1]*c[2][2] + c[0][1]*c[1][2]*c[2][0]+ c[0][2]*c[1][0]*c[2][1]
			- c[0][2]*c[1][1]*c[2][0] - c[0][1]*c[1][0]*c[2][2] - c[0][0]*c[1][2]*c[2][1];
		}

		Matrix3f inverted() const
		{
			float d = determinant();
			if( d == 0 )
				return Matrix3f();
			d = 1.0f / d;

			return this->transposed() * d;
		}

		Vec3f getRot(bool inDegree = true) const
		{
			Vec3f rot;

			rot.x = asinf(-x[7]);

			// Special case: Cos[x] == 0 (when Sin[x] is +/-1)
			float f = abs(x[7]);
			if (f > 0.999f && f < 1.001f)
			{
				// Pin arbitrarily one of y or z to zero
				// Mathematical equivalent of gimbal lock
				rot.y = 0;

				// Now: Cos[x] = 0, Sin[x] = +/-1, Cos[y] = 1, Sin[y] = 0
				// => m[0][0] = Cos[z] and m[1][0] = Sin[z]
				rot.z = atan2f(-x[3], x[0]);
			}
			// Standard case
			else
			{
				rot.y = atan2f(x[6], x[8]);
				rot.z = atan2f(x[1], x[4]);
			}

			if (inDegree)
				radToDeg(rot);

			return rot;
		}

		Vec4f getCol( unsigned int col ) const
		{
			return Vec4f( x[col * 4 + 0], x[col * 4 + 1], x[col * 4 + 2], x[col * 4 + 3] );
		}

		Vec4f getRow( unsigned int row ) const
		{
			return Vec4f( x[row + 0], x[row + 4], x[row + 8], x[row + 12] );
		}
	};

	/**
	* \brief Matrix class with 4x4 layout (transformation matrices)
	*/
	class Matrix4f
	{
	public:

		union
		{
			float c[4][4];	// Column major order for OpenGL: c[column][row]
			float x[16];
		};

		// --------------
		// Static methods
		// --------------
		static Matrix4f TransMat( float x, float y, float z )
		{
			Matrix4f m;

			m.c[3][0] = x;
			m.c[3][1] = y;
			m.c[3][2] = z;

			return m;
		}

		static Matrix4f ScaleMat( float x, float y, float z )
		{
			Matrix4f m;

			m.c[0][0] = x;
			m.c[1][1] = y;
			m.c[2][2] = z;

			return m;
		}

		static Matrix4f RotMat( float x, float y, float z )
		{
			// Rotation order: YXZ [* Vector]
			return Matrix4f( Quaternion( x, y, z ) );
		}

		static Matrix4f RotMat( Vec3f axis, float angle )
		{
			axis = axis * sinf( angle * 0.5f );
			return Matrix4f( Quaternion( axis.x, axis.y, axis.z, cosf( angle * 0.5f ) ) );
		}

		static Matrix4f PerspectiveMat( float l, float r, float b, float t, float n, float f )
		{
			Matrix4f m;

			m.x[0] = 2 * n / (r - l);
			m.x[5] = 2 * n / (t - b);
			m.x[8] = (r + l) / (r - l);
			m.x[9] = (t + b) / (t - b);
			m.x[10] = -(f + n) / (f - n);
			m.x[11] = -1;
			m.x[14] = -2 * f * n / (f - n);
			m.x[15] = 0;

			return m;
		}

		static Matrix4f OrthoMat( float l, float r, float b, float t, float n, float f )
		{
			Matrix4f m;

			m.x[0] = 2 / (r - l);
			m.x[5] = 2 / (t - b);
			m.x[10] = -2 / (f - n);
			m.x[12] = -(r + l) / (r - l);
			m.x[13] = -(t + b) / (t - b);
			m.x[14] = -(f + n) / (f - n);

			return m;
		}

		static void fastMult43( Matrix4f &dst, const Matrix4f &m1, const Matrix4f &m2 )
		{
			// Note: dst may not be the same as m1 or m2

			float *dstx = dst.x;
			const float *m1x = m1.x;
			const float *m2x = m2.x;

			dstx[0] = m1x[0] * m2x[0] + m1x[4] * m2x[1] + m1x[8] * m2x[2];
			dstx[1] = m1x[1] * m2x[0] + m1x[5] * m2x[1] + m1x[9] * m2x[2];
			dstx[2] = m1x[2] * m2x[0] + m1x[6] * m2x[1] + m1x[10] * m2x[2];
			dstx[3] = 0.0f;

			dstx[4] = m1x[0] * m2x[4] + m1x[4] * m2x[5] + m1x[8] * m2x[6];
			dstx[5] = m1x[1] * m2x[4] + m1x[5] * m2x[5] + m1x[9] * m2x[6];
			dstx[6] = m1x[2] * m2x[4] + m1x[6] * m2x[5] + m1x[10] * m2x[6];
			dstx[7] = 0.0f;

			dstx[8] = m1x[0] * m2x[8] + m1x[4] * m2x[9] + m1x[8] * m2x[10];
			dstx[9] = m1x[1] * m2x[8] + m1x[5] * m2x[9] + m1x[9] * m2x[10];
			dstx[10] = m1x[2] * m2x[8] + m1x[6] * m2x[9] + m1x[10] * m2x[10];
			dstx[11] = 0.0f;

			dstx[12] = m1x[0] * m2x[12] + m1x[4] * m2x[13] + m1x[8] * m2x[14] + m1x[12] * m2x[15];
			dstx[13] = m1x[1] * m2x[12] + m1x[5] * m2x[13] + m1x[9] * m2x[14] + m1x[13] * m2x[15];
			dstx[14] = m1x[2] * m2x[12] + m1x[6] * m2x[13] + m1x[10] * m2x[14] + m1x[14] * m2x[15];
			dstx[15] = 1.0f;
		}

		// ------------
		// Constructors
		// ------------
		Matrix4f()
		{
			c[0][0] = 1; c[1][0] = 0; c[2][0] = 0; c[3][0] = 0;
			c[0][1] = 0; c[1][1] = 1; c[2][1] = 0; c[3][1] = 0;
			c[0][2] = 0; c[1][2] = 0; c[2][2] = 1; c[3][2] = 0;
			c[0][3] = 0; c[1][3] = 0; c[2][3] = 0; c[3][3] = 1;
		}

		explicit Matrix4f( Math::NoInitHint )
		{
			// Constructor without default initialization
		}

		Matrix4f( const float *floatArray16 )
		{
			for( unsigned int i = 0; i < 4; ++i )
			{
				for( unsigned int j = 0; j < 4; ++j )
				{
					c[i][j] = floatArray16[i * 4 + j];
				}
			}
		}

		Matrix4f( const Quaternion &q )
		{
			// Calculate coefficients
			float x2 = q.x + q.x, y2 = q.y + q.y, z2 = q.z + q.z;
			float xx = q.x * x2,  xy = q.x * y2,  xz = q.x * z2;
			float yy = q.y * y2,  yz = q.y * z2,  zz = q.z * z2;
			float wx = q.w * x2,  wy = q.w * y2,  wz = q.w * z2;

			c[0][0] = 1 - (yy + zz);  c[1][0] = xy - wz;	
			c[2][0] = xz + wy;        c[3][0] = 0;
			c[0][1] = xy + wz;        c[1][1] = 1 - (xx + zz);
			c[2][1] = yz - wx;        c[3][1] = 0;
			c[0][2] = xz - wy;        c[1][2] = yz + wx;
			c[2][2] = 1 - (xx + yy);  c[3][2] = 0;
			c[0][3] = 0;              c[1][3] = 0;
			c[2][3] = 0;              c[3][3] = 1;
		}

		Matrix4f( const Matrix3f& m3)
		{
			c[0][0] = m3.c[0][0]; c[1][0] = m3.c[1][0]; c[2][0] = m3.c[2][0]; c[3][0] = 0;
			c[0][1] = m3.c[0][1]; c[1][1] = m3.c[1][1]; c[2][1] = m3.c[2][1]; c[3][1] = 0;
			c[0][2] = m3.c[0][2]; c[1][2] = m3.c[1][2]; c[2][2] = m3.c[2][0]; c[3][2] = 0;
			c[0][3] = 0;		  c[1][3] = 0;			c[2][3] = 0;		  c[3][3] = 1;
		}

		// ----------
		// Matrix sum
		// ----------
		Matrix4f operator+( const Matrix4f &m ) const 
		{
			Matrix4f mf( Math::NO_INIT );

			mf.x[0] = x[0] + m.x[0];
			mf.x[1] = x[1] + m.x[1];
			mf.x[2] = x[2] + m.x[2];
			mf.x[3] = x[3] + m.x[3];
			mf.x[4] = x[4] + m.x[4];
			mf.x[5] = x[5] + m.x[5];
			mf.x[6] = x[6] + m.x[6];
			mf.x[7] = x[7] + m.x[7];
			mf.x[8] = x[8] + m.x[8];
			mf.x[9] = x[9] + m.x[9];
			mf.x[10] = x[10] + m.x[10];
			mf.x[11] = x[11] + m.x[11];
			mf.x[12] = x[12] + m.x[12];
			mf.x[13] = x[13] + m.x[13];
			mf.x[14] = x[14] + m.x[14];
			mf.x[15] = x[15] + m.x[15];

			return mf;
		}

		Matrix4f &operator+=( const Matrix4f &m )
		{
			return *this = *this + m;
		}

		// ---------------------
		// Matrix multiplication
		// ---------------------
		Matrix4f operator*( const Matrix4f &m ) const 
		{
			Matrix4f mf( Math::NO_INIT );

			mf.x[0] = x[0] * m.x[0] + x[4] * m.x[1] + x[8] * m.x[2] + x[12] * m.x[3];
			mf.x[1] = x[1] * m.x[0] + x[5] * m.x[1] + x[9] * m.x[2] + x[13] * m.x[3];
			mf.x[2] = x[2] * m.x[0] + x[6] * m.x[1] + x[10] * m.x[2] + x[14] * m.x[3];
			mf.x[3] = x[3] * m.x[0] + x[7] * m.x[1] + x[11] * m.x[2] + x[15] * m.x[3];

			mf.x[4] = x[0] * m.x[4] + x[4] * m.x[5] + x[8] * m.x[6] + x[12] * m.x[7];
			mf.x[5] = x[1] * m.x[4] + x[5] * m.x[5] + x[9] * m.x[6] + x[13] * m.x[7];
			mf.x[6] = x[2] * m.x[4] + x[6] * m.x[5] + x[10] * m.x[6] + x[14] * m.x[7];
			mf.x[7] = x[3] * m.x[4] + x[7] * m.x[5] + x[11] * m.x[6] + x[15] * m.x[7];

			mf.x[8] = x[0] * m.x[8] + x[4] * m.x[9] + x[8] * m.x[10] + x[12] * m.x[11];
			mf.x[9] = x[1] * m.x[8] + x[5] * m.x[9] + x[9] * m.x[10] + x[13] * m.x[11];
			mf.x[10] = x[2] * m.x[8] + x[6] * m.x[9] + x[10] * m.x[10] + x[14] * m.x[11];
			mf.x[11] = x[3] * m.x[8] + x[7] * m.x[9] + x[11] * m.x[10] + x[15] * m.x[11];

			mf.x[12] = x[0] * m.x[12] + x[4] * m.x[13] + x[8] * m.x[14] + x[12] * m.x[15];
			mf.x[13] = x[1] * m.x[12] + x[5] * m.x[13] + x[9] * m.x[14] + x[13] * m.x[15];
			mf.x[14] = x[2] * m.x[12] + x[6] * m.x[13] + x[10] * m.x[14] + x[14] * m.x[15];
			mf.x[15] = x[3] * m.x[12] + x[7] * m.x[13] + x[11] * m.x[14] + x[15] * m.x[15];

			return mf;
		}

		Matrix4f operator*( const float f ) const
		{
			Matrix4f m( *this );

			m.x[0]  *= f; m.x[1]  *= f; m.x[2]  *= f; m.x[3]  *= f;
			m.x[4]  *= f; m.x[5]  *= f; m.x[6]  *= f; m.x[7]  *= f;
			m.x[8]  *= f; m.x[9]  *= f; m.x[10] *= f; m.x[11] *= f;
			m.x[12] *= f; m.x[13] *= f; m.x[14] *= f; m.x[15] *= f;

			return m;
		}

		// ----------------------------
		// Vector-Matrix multiplication
		// ----------------------------
		Vec3f operator*( const Vec3f &v ) const
		{
			return Vec3f( v.x * c[0][0] + v.y * c[1][0] + v.z * c[2][0] + c[3][0],
				v.x * c[0][1] + v.y * c[1][1] + v.z * c[2][1] + c[3][1],
				v.x * c[0][2] + v.y * c[1][2] + v.z * c[2][2] + c[3][2] );
		}

		Vec4f operator*( const Vec4f &v ) const
		{
			return Vec4f( v.x * c[0][0] + v.y * c[1][0] + v.z * c[2][0] + v.w * c[3][0],
				v.x * c[0][1] + v.y * c[1][1] + v.z * c[2][1] + v.w * c[3][1],
				v.x * c[0][2] + v.y * c[1][2] + v.z * c[2][2] + v.w * c[3][2],
				v.x * c[0][3] + v.y * c[1][3] + v.z * c[2][3] + v.w * c[3][3] );
		}

		Vec3f mult33Vec( const Vec3f &v ) const
		{
			return Vec3f( v.x * c[0][0] + v.y * c[1][0] + v.z * c[2][0],
				v.x * c[0][1] + v.y * c[1][1] + v.z * c[2][1],
				v.x * c[0][2] + v.y * c[1][2] + v.z * c[2][2] );
		}

		// ---------------
		// Transformations
		// ---------------
		void translate( const float x, const float y, const float z )
		{
			*this = TransMat( x, y, z ) * *this;
		}

		void scale( const float x, const float y, const float z )
		{
			*this = ScaleMat( x, y, z ) * *this;
		}

		void rotate( const float x, const float y, const float z )
		{
			*this = RotMat( x, y, z ) * *this;
		}

		// ---------------
		// Other
		// ---------------

		Matrix4f transposed() const
		{
			Matrix4f m( *this );

			for( unsigned int y = 0; y < 4; ++y )
			{
				for( unsigned int x = y + 1; x < 4; ++x ) 
				{
					float tmp = m.c[x][y];
					m.c[x][y] = m.c[y][x];
					m.c[y][x] = tmp;
				}
			}

			return m;
		}

		float determinant() const
		{
			return 
				c[0][3]*c[1][2]*c[2][1]*c[3][0] - c[0][2]*c[1][3]*c[2][1]*c[3][0] - c[0][3]*c[1][1]*c[2][2]*c[3][0] + c[0][1]*c[1][3]*c[2][2]*c[3][0] +
				c[0][2]*c[1][1]*c[2][3]*c[3][0] - c[0][1]*c[1][2]*c[2][3]*c[3][0] - c[0][3]*c[1][2]*c[2][0]*c[3][1] + c[0][2]*c[1][3]*c[2][0]*c[3][1] +
				c[0][3]*c[1][0]*c[2][2]*c[3][1] - c[0][0]*c[1][3]*c[2][2]*c[3][1] - c[0][2]*c[1][0]*c[2][3]*c[3][1] + c[0][0]*c[1][2]*c[2][3]*c[3][1] +
				c[0][3]*c[1][1]*c[2][0]*c[3][2] - c[0][1]*c[1][3]*c[2][0]*c[3][2] - c[0][3]*c[1][0]*c[2][1]*c[3][2] + c[0][0]*c[1][3]*c[2][1]*c[3][2] +
				c[0][1]*c[1][0]*c[2][3]*c[3][2] - c[0][0]*c[1][1]*c[2][3]*c[3][2] - c[0][2]*c[1][1]*c[2][0]*c[3][3] + c[0][1]*c[1][2]*c[2][0]*c[3][3] +
				c[0][2]*c[1][0]*c[2][1]*c[3][3] - c[0][0]*c[1][2]*c[2][1]*c[3][3] - c[0][1]*c[1][0]*c[2][2]*c[3][3] + c[0][0]*c[1][1]*c[2][2]*c[3][3];
		}

		Matrix4f inverted() const
		{
			Matrix4f m( Math::NO_INIT );

			float d = determinant();
			if( d == 0 ) return m;
			d = 1.0f / d;

			m.c[0][0] = d * (c[1][2]*c[2][3]*c[3][1] - c[1][3]*c[2][2]*c[3][1] + c[1][3]*c[2][1]*c[3][2] - c[1][1]*c[2][3]*c[3][2] - c[1][2]*c[2][1]*c[3][3] + c[1][1]*c[2][2]*c[3][3]);
			m.c[0][1] = d * (c[0][3]*c[2][2]*c[3][1] - c[0][2]*c[2][3]*c[3][1] - c[0][3]*c[2][1]*c[3][2] + c[0][1]*c[2][3]*c[3][2] + c[0][2]*c[2][1]*c[3][3] - c[0][1]*c[2][2]*c[3][3]);
			m.c[0][2] = d * (c[0][2]*c[1][3]*c[3][1] - c[0][3]*c[1][2]*c[3][1] + c[0][3]*c[1][1]*c[3][2] - c[0][1]*c[1][3]*c[3][2] - c[0][2]*c[1][1]*c[3][3] + c[0][1]*c[1][2]*c[3][3]);
			m.c[0][3] = d * (c[0][3]*c[1][2]*c[2][1] - c[0][2]*c[1][3]*c[2][1] - c[0][3]*c[1][1]*c[2][2] + c[0][1]*c[1][3]*c[2][2] + c[0][2]*c[1][1]*c[2][3] - c[0][1]*c[1][2]*c[2][3]);
			m.c[1][0] = d * (c[1][3]*c[2][2]*c[3][0] - c[1][2]*c[2][3]*c[3][0] - c[1][3]*c[2][0]*c[3][2] + c[1][0]*c[2][3]*c[3][2] + c[1][2]*c[2][0]*c[3][3] - c[1][0]*c[2][2]*c[3][3]);
			m.c[1][1] = d * (c[0][2]*c[2][3]*c[3][0] - c[0][3]*c[2][2]*c[3][0] + c[0][3]*c[2][0]*c[3][2] - c[0][0]*c[2][3]*c[3][2] - c[0][2]*c[2][0]*c[3][3] + c[0][0]*c[2][2]*c[3][3]);
			m.c[1][2] = d * (c[0][3]*c[1][2]*c[3][0] - c[0][2]*c[1][3]*c[3][0] - c[0][3]*c[1][0]*c[3][2] + c[0][0]*c[1][3]*c[3][2] + c[0][2]*c[1][0]*c[3][3] - c[0][0]*c[1][2]*c[3][3]);
			m.c[1][3] = d * (c[0][2]*c[1][3]*c[2][0] - c[0][3]*c[1][2]*c[2][0] + c[0][3]*c[1][0]*c[2][2] - c[0][0]*c[1][3]*c[2][2] - c[0][2]*c[1][0]*c[2][3] + c[0][0]*c[1][2]*c[2][3]);
			m.c[2][0] = d * (c[1][1]*c[2][3]*c[3][0] - c[1][3]*c[2][1]*c[3][0] + c[1][3]*c[2][0]*c[3][1] - c[1][0]*c[2][3]*c[3][1] - c[1][1]*c[2][0]*c[3][3] + c[1][0]*c[2][1]*c[3][3]);
			m.c[2][1] = d * (c[0][3]*c[2][1]*c[3][0] - c[0][1]*c[2][3]*c[3][0] - c[0][3]*c[2][0]*c[3][1] + c[0][0]*c[2][3]*c[3][1] + c[0][1]*c[2][0]*c[3][3] - c[0][0]*c[2][1]*c[3][3]);
			m.c[2][2] = d * (c[0][1]*c[1][3]*c[3][0] - c[0][3]*c[1][1]*c[3][0] + c[0][3]*c[1][0]*c[3][1] - c[0][0]*c[1][3]*c[3][1] - c[0][1]*c[1][0]*c[3][3] + c[0][0]*c[1][1]*c[3][3]);
			m.c[2][3] = d * (c[0][3]*c[1][1]*c[2][0] - c[0][1]*c[1][3]*c[2][0] - c[0][3]*c[1][0]*c[2][1] + c[0][0]*c[1][3]*c[2][1] + c[0][1]*c[1][0]*c[2][3] - c[0][0]*c[1][1]*c[2][3]);
			m.c[3][0] = d * (c[1][2]*c[2][1]*c[3][0] - c[1][1]*c[2][2]*c[3][0] - c[1][2]*c[2][0]*c[3][1] + c[1][0]*c[2][2]*c[3][1] + c[1][1]*c[2][0]*c[3][2] - c[1][0]*c[2][1]*c[3][2]);
			m.c[3][1] = d * (c[0][1]*c[2][2]*c[3][0] - c[0][2]*c[2][1]*c[3][0] + c[0][2]*c[2][0]*c[3][1] - c[0][0]*c[2][2]*c[3][1] - c[0][1]*c[2][0]*c[3][2] + c[0][0]*c[2][1]*c[3][2]);
			m.c[3][2] = d * (c[0][2]*c[1][1]*c[3][0] - c[0][1]*c[1][2]*c[3][0] - c[0][2]*c[1][0]*c[3][1] + c[0][0]*c[1][2]*c[3][1] + c[0][1]*c[1][0]*c[3][2] - c[0][0]*c[1][1]*c[3][2]);
			m.c[3][3] = d * (c[0][1]*c[1][2]*c[2][0] - c[0][2]*c[1][1]*c[2][0] + c[0][2]*c[1][0]*c[2][1] - c[0][0]*c[1][2]*c[2][1] - c[0][1]*c[1][0]*c[2][2] + c[0][0]*c[1][1]*c[2][2]);

			return m;
		}

		void decompose( Vec3f &trans, Vec3f &rot, Vec3f &scale ) const
		{
			// Getting translation is trivial
			trans = Vec3f( c[3][0], c[3][1], c[3][2] );

			// Scale is length of columns
			scale.x = sqrtf( c[0][0] * c[0][0] + c[0][1] * c[0][1] + c[0][2] * c[0][2] );
			scale.y = sqrtf( c[1][0] * c[1][0] + c[1][1] * c[1][1] + c[1][2] * c[1][2] );
			scale.z = sqrtf( c[2][0] * c[2][0] + c[2][1] * c[2][1] + c[2][2] * c[2][2] );

			if( scale.x == 0 || scale.y == 0 || scale.z == 0 ) return;

			// Detect negative scale with determinant and flip one arbitrary axis
			if( determinant() < 0 ) scale.x = -scale.x;

			// Combined rotation matrix YXZ
			//
			// Cos[y]*Cos[z]+Sin[x]*Sin[y]*Sin[z]   Cos[z]*Sin[x]*Sin[y]-Cos[y]*Sin[z]  Cos[x]*Sin[y]	
			// Cos[x]*Sin[z]                        Cos[x]*Cos[z]                       -Sin[x]
			// -Cos[z]*Sin[y]+Cos[y]*Sin[x]*Sin[z]  Cos[y]*Cos[z]*Sin[x]+Sin[y]*Sin[z]  Cos[x]*Cos[y]

			rot.x = asinf( -c[2][1] / scale.z );

			// Special case: Cos[x] == 0 (when Sin[x] is +/-1)
			float f = fabsf( c[2][1] / scale.z );
			if( f > 0.999f && f < 1.001f )
			{
				// Pin arbitrarily one of y or z to zero
				// Mathematical equivalent of gimbal lock
				rot.y = 0;

				// Now: Cos[x] = 0, Sin[x] = +/-1, Cos[y] = 1, Sin[y] = 0
				// => m[0][0] = Cos[z] and m[1][0] = Sin[z]
				rot.z = atan2f( -c[1][0] / scale.y, c[0][0] / scale.x );
			}
			// Standard case
			else
			{
				rot.y = atan2f( c[2][0] / scale.z, c[2][2] / scale.z );
				rot.z = atan2f( c[0][1] / scale.x, c[1][1] / scale.y );
			}
		}

		Vec4f getCol( unsigned int col ) const
		{
			return Vec4f( x[col * 4 + 0], x[col * 4 + 1], x[col * 4 + 2], x[col * 4 + 3] );
		}

		Vec4f getRow( unsigned int row ) const
		{
			return Vec4f( x[row + 0], x[row + 4], x[row + 8], x[row + 12] );
		}

		Vec3f getTrans() const
		{
			return Vec3f( c[3][0], c[3][1], c[3][2] );
		}

		Vec3f getScale() const
		{
			Vec3f scale;
			// Scale is length of columns
			scale.x = sqrtf( c[0][0] * c[0][0] + c[0][1] * c[0][1] + c[0][2] * c[0][2] );
			scale.y = sqrtf( c[1][0] * c[1][0] + c[1][1] * c[1][1] + c[1][2] * c[1][2] );
			scale.z = sqrtf( c[2][0] * c[2][0] + c[2][1] * c[2][1] + c[2][2] * c[2][2] );
			return scale;
		}
	};


	/**
	* \brief Plane class (e.g. for representing the floor plane)
	*/
	class Plane
	{
	public:
		Vec3f normal; 
		float dist;

		// ------------
		// Constructors
		// ------------
		Plane() 
		{ 
			normal.x = 0; normal.y = 0; normal.z = 0; dist = 0; 
		};

		explicit Plane( const float a, const float b, const float c, const float d )
		{
			normal = Vec3f( a, b, c );
			float invLen = 1.0f / normal.length();
			normal *= invLen;	// Normalize
			dist = d * invLen;
		}

		Plane( const Vec3f &v0, const Vec3f &v1, const Vec3f &v2 )
		{
			normal = v1 - v0;
			normal = normal.cross( v2 - v0 );
			normal.normalize();
			dist = -normal.dot( v0 );
		}

		// ----------------
		// Other operations
		// ----------------
		float distToPoint( const Vec3f &v ) const
		{
			return normal.dot( v ) + dist;
		}
	};


	/**
	* \brief Test whether a ray intersects with a triangle
	*/
	static inline bool rayTriangleIntersection( const Vec3f &rayOrig, const Vec3f &rayDir, 
		const Vec3f &vert0, const Vec3f &vert1, const Vec3f &vert2,
		Vec3f &intsPoint )
	{
		// Idea: Tomas Moeller and Ben Trumbore
		// in Fast, Minimum Storage Ray/Triangle Intersection 

		// Find vectors for two edges sharing vert0
		Vec3f edge1 = vert1 - vert0;
		Vec3f edge2 = vert2 - vert0;

		// Begin calculating determinant - also used to calculate U parameter
		Vec3f pvec = rayDir.cross( edge2 );

		// If determinant is near zero, ray lies in plane of triangle
		float det = edge1.dot( pvec );


		// *** Culling branch ***
		/*if( det < Math::Epsilon )return false;

		// Calculate distance from vert0 to ray origin
		Vec3f tvec = rayOrig - vert0;

		// Calculate U parameter and test bounds
		float u = tvec.dot( pvec );
		if (u < 0 || u > det ) return false;

		// Prepare to test V parameter
		Vec3f qvec = tvec.cross( edge1 );

		// Calculate V parameter and test bounds
		float v = rayDir.dot( qvec );
		if (v < 0 || u + v > det ) return false;

		// Calculate t, scale parameters, ray intersects triangle
		float t = edge2.dot( qvec ) / det;*/


		// *** Non-culling branch ***
		if( det > -Math::Epsilon && det < Math::Epsilon ) return 0;
		float inv_det = 1.0f / det;

		// Calculate distance from vert0 to ray origin
		Vec3f tvec = rayOrig - vert0;

		// Calculate U parameter and test bounds
		float u = tvec.dot( pvec ) * inv_det;
		if( u < 0.0f || u > 1.0f ) return 0;

		// Prepare to test V parameter
		Vec3f qvec = tvec.cross( edge1 );

		// Calculate V parameter and test bounds
		float v = rayDir.dot( qvec ) * inv_det;
		if( v < 0.0f || u + v > 1.0f ) return 0;

		// Calculate t, ray intersects triangle
		float t = edge2.dot( qvec ) * inv_det;


		// Calculate intersection point and test ray length and direction
		intsPoint = rayOrig + rayDir * t;
		Vec3f vec = intsPoint - rayOrig;
		if( vec.dot( rayDir ) < 0 || vec.length() > rayDir.length() ) return false;

		return true;
	}

	/**
	* \brief Test whether a ray intersects with an AABB bounding box
	*/
	static inline bool rayAABBIntersection( const Vec3f &rayOrig, const Vec3f &rayDir, 
		const Vec3f &mins, const Vec3f &maxs )
	{
		// SLAB based optimized ray/AABB intersection routine
		// Idea taken from http://ompf.org/ray/

		float l1 = (mins.x - rayOrig.x) / rayDir.x;
		float l2 = (maxs.x - rayOrig.x) / rayDir.x;
		float lmin = minf( l1, l2 );
		float lmax = maxf( l1, l2 );

		l1 = (mins.y - rayOrig.y) / rayDir.y;
		l2 = (maxs.y - rayOrig.y) / rayDir.y;
		lmin = maxf( minf( l1, l2 ), lmin );
		lmax = minf( maxf( l1, l2 ), lmax );

		l1 = (mins.z - rayOrig.z) / rayDir.z;
		l2 = (maxs.z - rayOrig.z) / rayDir.z;
		lmin = maxf( minf( l1, l2 ), lmin );
		lmax = minf( maxf( l1, l2 ), lmax );

		if( (lmax >= 0.0f) & (lmax >= lmin) )
		{
			// Consider length
			const Vec3f rayDest = rayOrig + rayDir;
			Vec3f rayMins( minf( rayDest.x, rayOrig.x), minf( rayDest.y, rayOrig.y ), minf( rayDest.z, rayOrig.z ) );
			Vec3f rayMaxs( maxf( rayDest.x, rayOrig.x), maxf( rayDest.y, rayOrig.y ), maxf( rayDest.z, rayOrig.z ) );
			return 
				(rayMins.x < maxs.x) && (rayMaxs.x > mins.x) &&
				(rayMins.y < maxs.y) && (rayMaxs.y > mins.y) &&
				(rayMins.z < maxs.z) && (rayMaxs.z > mins.z);
		}
		else
			return false;
	}


	/**
	* \brief Calculate the nearest distance of a point to a AABB bounding box
	*/
	static inline float nearestDistToAABB( const Vec3f &pos, const Vec3f &mins, const Vec3f &maxs )
	{
		const Vec3f center = (mins + maxs) * 0.5f;
		const Vec3f extent = (maxs - mins) * 0.5f;

		Vec3f nearestVec;
		nearestVec.x = maxf( 0, fabsf( pos.x - center.x ) - extent.x );
		nearestVec.y = maxf( 0, fabsf( pos.y - center.y ) - extent.y );
		nearestVec.z = maxf( 0, fabsf( pos.z - center.z ) - extent.z );

		return nearestVec.length();
	}

	/**
	* \brief Calculate the distance of a point to a ray or line
	*/
	static inline float distancePointToRay( const Vec3f& point, const Vec3f& rayOrigin, const Vec3f& rayDir, bool withinLineSegmentOnly = false)
	{
		float rayLength = rayDir.length();

		if (rayLength == 0)
			return -1.f;

		float u = (point - rayOrigin).dot(rayDir) / ( rayLength * rayLength );

		if( withinLineSegmentOnly && (u < 0.0f || u > 1.0f))
			return -1.f;   // closest point does not fall within the line segment

		Vec3f intersect;
		intersect.x = rayOrigin.x + u * rayDir.x;
		intersect.y = rayOrigin.y + u * rayDir.y;
		intersect.z = rayOrigin.z + u * rayDir.z;

		return (point - intersect).length();
	}

	/**
	* \brief SkeletonJointPosition contains the position (Vec3f) and tracking confidence for a skeleton joint
	*/
	struct SkeletonJointPosition
	{
		SkeletonJointPosition() : m_confidence(0) {}
		SkeletonJointPosition(Fubi::Math::NoInitHint noInitHint) : m_position(noInitHint) {}
		SkeletonJointPosition(const Vec3f& position, float confidence)
			: m_position(position), m_confidence(confidence) {}
		SkeletonJointPosition(float x, float y, float z, float confidence)
			: m_position(x, y, z), m_confidence(confidence)	{}
		float m_confidence;
		Vec3f m_position;
	};

	/**
	* \brief SkeletonJointOrientation contains the orientation (Matrix3f) and tracking confidence for a skeleton joint
	*/
	struct SkeletonJointOrientation
	{
		SkeletonJointOrientation() : m_confidence(0), m_orientation() {}
		SkeletonJointOrientation(Fubi::Math::NoInitHint noInitHint) : m_orientation(noInitHint) {}
		SkeletonJointOrientation(const Matrix3f& rotMat, float confidence)
			: m_orientation(rotMat), m_confidence(confidence)	{}
		SkeletonJointOrientation(float* array9, float confidence)
			: m_orientation(array9), m_confidence(confidence)	{}
		SkeletonJointOrientation(const Quaternion& quaternion, float confidence)
			: m_orientation(quaternion), m_confidence(confidence)	{}
		float m_confidence;
		Matrix3f m_orientation;
	};

	/**
	* \brief SkeletonJointAcceleration contains the acceleration and tracking confidence for a skeleton joint
	*/
	struct SkeletonJointAcceleration
	{
		SkeletonJointAcceleration() : m_confidence(0) {}
		SkeletonJointAcceleration(Fubi::Math::NoInitHint noInitHint) : m_acceleration(noInitHint) {}
		SkeletonJointAcceleration(const Vec3f& acceleration, float confidence)
			: m_acceleration(acceleration), m_confidence(confidence) {}
		SkeletonJointAcceleration(float x, float y, float z, float confidence)
			: m_acceleration(x, y, z), m_confidence(confidence)	{}
		float m_confidence;
		Vec3f m_acceleration;
	};

	/**
	* \brief BodyMeasurementDistance contains the distance and tracking confidence for a body measurement
	*/
	struct BodyMeasurementDistance
	{
		BodyMeasurementDistance() : m_confidence(0), m_dist(0) {}
		BodyMeasurementDistance(Fubi::Math::NoInitHint noInit) {}
		BodyMeasurementDistance(float distance, float confidence) : m_confidence(confidence), m_dist(distance) {}
		float m_dist;
		float m_confidence;
	};

	/**
	* \brief Normalizes a rotation vector for -180 to 180 range 
	*/
	static void normalizeRotationVec(Vec3f& rotVec)
	{
		// Ensure upper bound of 180
		while(rotVec.x > 180.000001f)
			rotVec.x -= 360.0f;
		while(rotVec.y > 180.000001f)
			rotVec.y -= 360.0f;
		while(rotVec.z > 180.000001f)
			rotVec.z -= 360.0f;

		//Ensure lower bound of -180
		while(rotVec.x < -180.000001f)
			rotVec.x += 360.0f;
		while(rotVec.y < -180.000001f)
			rotVec.y += 360.0f;
		while(rotVec.z < -180.000001f)
			rotVec.z += 360.0f;
	}

	/**
	* \brief Calculate alpha value for the 1€ filter
	*/
	static float oneEuroAlpha(float timeStep, float cutOffFrequency)
	{
		if (cutOffFrequency > Math::Epsilon && timeStep > Math::Epsilon)
			return 1.0f / (1.0f + (1.0f / (Math::TwoPi * cutOffFrequency * timeStep)));
		return 0;
	}

	/**
	* \brief Vec3f containing only min float values
	*/
	static const Vec3f DefaultMinVec = Vec3f(-Math::MaxFloat, -Math::MaxFloat, -Math::MaxFloat);
	/**
	* \brief Vec3f containing only max float values
	*/
	static const Vec3f DefaultMaxVec = Vec3f(Math::MaxFloat, Math::MaxFloat, Math::MaxFloat);
	/**
	* \brief Vec3f containing only 0s
	*/
	static const Vec3f NullVec = Vec3f(0,0,0);

	/**
	* \brief Calculate global position out of local position and global parent position
	*/
	static inline void calculateGlobalPosition(const Vec3f& localPos, const Vec3f& absParentPos, const Matrix3f& absParentRot, Vec3f& dstPos)
	{
		// Combine transformation as rotatation
		Matrix4f parentTrans(absParentRot);
		// + translation
		parentTrans.x[12] = absParentPos.x;
		parentTrans.x[13] = absParentPos.y;
		parentTrans.x[14] = absParentPos.z;
		// Add local translation
		dstPos = parentTrans * localPos;
	}
	/**
	* \brief Calculate local positions out of global position and parent transformation
	*/
	static inline void calculateLocalPosition(const Vec3f& absPos, const Vec3f& absParentPos, const Matrix3f& absParentRot, Vec3f& dstPos)
	{
		// Combine transformation as rotatation
		Matrix4f parentTrans(absParentRot);
		// + translation
		parentTrans.x[12] = absParentPos.x;
		parentTrans.x[13] = absParentPos.y;
		parentTrans.x[14] = absParentPos.z;
		// Remove translation and rotation of the parent by applying the inverted transformation
		dstPos = parentTrans.inverted() * absPos;
	}

	/**
	* \brief Calculate local positions out of global position and parent transformation
	*/
	static inline void calculateLocalPosition(const SkeletonJointPosition& absPos, const SkeletonJointPosition& absParentPos, const SkeletonJointOrientation& absParentRot, SkeletonJointPosition& dstPos)
	{
		// First calculate the position
		calculateLocalPosition(absPos.m_position, absParentPos.m_position, absParentRot.m_orientation, dstPos.m_position);
		// Confidence is the minima of all used transformations
		dstPos.m_confidence = minf(absPos.m_confidence, minf(absParentPos.m_confidence, absParentRot.m_confidence));
	}

	/**
	* \brief Calculate body measurement (distance between two joints with filtering applied)
	*/
	static inline void calculateBodyMeasurement(const SkeletonJointPosition& joint1, const SkeletonJointPosition& joint2, BodyMeasurementDistance& dstMeasurement, float filterFactor = 1.0f)
	{
		float currentConfidence = minf(joint1.m_confidence, joint2.m_confidence);
		if (currentConfidence > dstMeasurement.m_confidence + 0.1f) // current confidence much more accurate than the last measure
			filterFactor = 1.0f;	// ..so forget about the last one

		float reverseFac = 1.0f - filterFactor;
		dstMeasurement.m_confidence = (reverseFac * dstMeasurement.m_confidence) + (filterFactor * currentConfidence);
		dstMeasurement.m_dist = (reverseFac * dstMeasurement.m_dist) + (filterFactor * (joint1.m_position-joint2.m_position).length());
	}

	/**
	* \brief Calculate local orientation out of global orientaion and parent orientation
	*/
	static inline void calculateLocalRotMat(const float* absRotMat, const float* absParentMat, float* dstMat)
	{
		Fubi::Matrix3f matLocal = Fubi::Matrix3f(absRotMat) * Fubi::Matrix3f(absParentMat).inverted();
		for (int i = 0; i< 9; i++)
		{
			dstMat[i] = matLocal.x[i];
		}
	}

	/**
	* \brief Calculate local orientation out of global orientaion and parent orientation
	*/
	static inline void calculateLocalRotation(const SkeletonJointOrientation& absRot, const SkeletonJointOrientation& absParentRot, SkeletonJointOrientation& dstRot)
	{
		calculateLocalRotMat(absRot.m_orientation.x, absParentRot.m_orientation.x, dstRot.m_orientation.x);
		dstRot.m_confidence = minf(absRot.m_confidence, absParentRot.m_confidence);
	}

	/**
	* \brief Copy the three orientation columns to the right position in the orientation matrix
	*/
	static void orientationVectorsToRotMat(SkeletonJointOrientation &jointOrientation, const Vec3f& xCol,const Vec3f& yCol, const Vec3f& zCol)
	{
		jointOrientation.m_orientation.x[0] = xCol.x;
		jointOrientation.m_orientation.x[3] = xCol.y;
		jointOrientation.m_orientation.x[6] = xCol.z;

		jointOrientation.m_orientation.x[1] = yCol.x;
		jointOrientation.m_orientation.x[4] = yCol.y;
		jointOrientation.m_orientation.x[7] = yCol.z;

		jointOrientation.m_orientation.x[2] = zCol.x;
		jointOrientation.m_orientation.x[5] = zCol.y;
		jointOrientation.m_orientation.x[8] = zCol.z;
	}

	/**
	* \brief Calculate local transformations out of the global ones
	*/
	static void calculateLocalTransformations(const SkeletonJointPosition* globalPos, const SkeletonJointOrientation* globalOrients,
		SkeletonJointPosition* localPos, SkeletonJointOrientation* localOrients)
	{
		// Calculate new relative orientations
		// Torso is the root, so the local orientation is the same as the global one
		localOrients[SkeletonJoint::TORSO] = globalOrients[SkeletonJoint::TORSO];
		// Neck
		calculateLocalRotation(globalOrients[SkeletonJoint::NECK], 
			globalOrients[SkeletonJoint::TORSO], localOrients[SkeletonJoint::NECK]);
		// Head
		calculateLocalRotation(globalOrients[SkeletonJoint::HEAD], 
			globalOrients[SkeletonJoint::NECK], localOrients[SkeletonJoint::HEAD]);
		// Nose
		calculateLocalRotation(globalOrients[SkeletonJoint::FACE_NOSE], 
			globalOrients[SkeletonJoint::HEAD], localOrients[SkeletonJoint::FACE_NOSE]);
		// Chin
		calculateLocalRotation(globalOrients[SkeletonJoint::FACE_CHIN], 
			globalOrients[SkeletonJoint::HEAD], localOrients[SkeletonJoint::FACE_CHIN]);
		// Forehead
		calculateLocalRotation(globalOrients[SkeletonJoint::FACE_FOREHEAD], 
			globalOrients[SkeletonJoint::HEAD], localOrients[SkeletonJoint::FACE_FOREHEAD]);
		// Left ear
		calculateLocalRotation(globalOrients[SkeletonJoint::FACE_LEFT_EAR], 
			globalOrients[SkeletonJoint::HEAD], localOrients[SkeletonJoint::FACE_LEFT_EAR]);
		// Right ear
		calculateLocalRotation(globalOrients[SkeletonJoint::FACE_RIGHT_EAR], 
			globalOrients[SkeletonJoint::HEAD], localOrients[SkeletonJoint::FACE_RIGHT_EAR]);
		// Left shoulder
		calculateLocalRotation(globalOrients[SkeletonJoint::LEFT_SHOULDER], 
			globalOrients[SkeletonJoint::HEAD], localOrients[SkeletonJoint::LEFT_SHOULDER]);
		// Left elbow
		calculateLocalRotation(globalOrients[SkeletonJoint::LEFT_ELBOW], 
			globalOrients[SkeletonJoint::LEFT_SHOULDER], localOrients[SkeletonJoint::LEFT_ELBOW]);
		// Left wrist
		calculateLocalRotation(globalOrients[SkeletonJoint::LEFT_WRIST], 
			globalOrients[SkeletonJoint::LEFT_ELBOW], localOrients[SkeletonJoint::LEFT_WRIST]);
		// Left hand
		calculateLocalRotation(globalOrients[SkeletonJoint::LEFT_HAND], 
			globalOrients[SkeletonJoint::LEFT_WRIST], localOrients[SkeletonJoint::LEFT_HAND]);
		// Right shoulder
		calculateLocalRotation(globalOrients[SkeletonJoint::RIGHT_SHOULDER], 
			globalOrients[SkeletonJoint::NECK], localOrients[SkeletonJoint::RIGHT_SHOULDER]);
		// Right elbow
		calculateLocalRotation(globalOrients[SkeletonJoint::RIGHT_ELBOW], 
			globalOrients[SkeletonJoint::RIGHT_SHOULDER], localOrients[SkeletonJoint::RIGHT_ELBOW]);
		// Right wrist
		calculateLocalRotation(globalOrients[SkeletonJoint::RIGHT_WRIST], 
			globalOrients[SkeletonJoint::RIGHT_ELBOW], localOrients[SkeletonJoint::RIGHT_WRIST]);
		// Right hand
		calculateLocalRotation(globalOrients[SkeletonJoint::RIGHT_HAND], 
			globalOrients[SkeletonJoint::RIGHT_WRIST], localOrients[SkeletonJoint::RIGHT_HAND]);
		// Waist
		calculateLocalRotation(globalOrients[SkeletonJoint::WAIST], 
			globalOrients[SkeletonJoint::TORSO], localOrients[SkeletonJoint::WAIST]);
		// Left hip
		calculateLocalRotation(globalOrients[SkeletonJoint::LEFT_HIP], 
			globalOrients[SkeletonJoint::WAIST], localOrients[SkeletonJoint::LEFT_HIP]);
		// Left knee
		calculateLocalRotation(globalOrients[SkeletonJoint::LEFT_KNEE], 
			globalOrients[SkeletonJoint::LEFT_HIP], localOrients[SkeletonJoint::LEFT_KNEE]);
		// Left ankle
		calculateLocalRotation(globalOrients[SkeletonJoint::LEFT_ANKLE], 
			globalOrients[SkeletonJoint::LEFT_KNEE], localOrients[SkeletonJoint::LEFT_ANKLE]);
		// Left foot
		calculateLocalRotation(globalOrients[SkeletonJoint::LEFT_FOOT], 
			globalOrients[SkeletonJoint::LEFT_ANKLE], localOrients[SkeletonJoint::LEFT_FOOT]);
		// Right hip
		calculateLocalRotation(globalOrients[SkeletonJoint::RIGHT_HIP], 
			globalOrients[SkeletonJoint::WAIST], localOrients[SkeletonJoint::RIGHT_HIP]);
		// Right knee
		calculateLocalRotation(globalOrients[SkeletonJoint::RIGHT_KNEE], 
			globalOrients[SkeletonJoint::RIGHT_HIP], localOrients[SkeletonJoint::RIGHT_KNEE]);
		// Right ankle
		calculateLocalRotation(globalOrients[SkeletonJoint::RIGHT_ANKLE], 
			globalOrients[SkeletonJoint::RIGHT_KNEE], localOrients[SkeletonJoint::RIGHT_ANKLE]);
		// Right foot
		calculateLocalRotation(globalOrients[SkeletonJoint::RIGHT_FOOT], 
			globalOrients[SkeletonJoint::RIGHT_ANKLE], localOrients[SkeletonJoint::RIGHT_FOOT]);

		// Calculate local positions (removing the torso transformation=loc+rot from the position data)
		// Torso is the root, so the local pos is the same as the global one
		localPos[SkeletonJoint::TORSO] = globalPos[SkeletonJoint::TORSO];
		const SkeletonJointPosition& torsoPos = globalPos[SkeletonJoint::TORSO];
		const SkeletonJointOrientation& torsoRot = globalOrients[SkeletonJoint::TORSO];
		for (unsigned int i = 0; i < SkeletonJoint::NUM_JOINTS; ++i)
		{
			if (i != SkeletonJoint::TORSO)
				calculateLocalPosition(globalPos[i], torsoPos, torsoRot, localPos[i]);
		}
	}

	/**
	* \brief Calculate local hand transformations out of the global ones
	*/
	static void calculateLocalHandTransformations(const SkeletonJointPosition* globalPos, const SkeletonJointOrientation* globalOrients,
		SkeletonJointPosition* localPos, SkeletonJointOrientation* localOrients)
	{
		// Calculate new relative orientations
		// Palm is the root, so the local orientation is the same as the global one
		localOrients[SkeletonHandJoint::PALM] = globalOrients[SkeletonHandJoint::PALM];
		localPos[SkeletonHandJoint::PALM] = globalPos[SkeletonHandJoint::PALM];
		// Fingers
		for (int i = SkeletonHandJoint::FINGER_ONE; i < SkeletonHandJoint::NUM_JOINTS; ++i)
		{
			calculateLocalRotation(globalOrients[i], 
				globalOrients[SkeletonHandJoint::PALM], localOrients[i]);
			calculateLocalPosition(globalPos[i], globalPos[SkeletonHandJoint::PALM], globalOrients[SkeletonHandJoint::PALM], localPos[i]);
		}
	}

	/**
	* \brief Calculate orientation out of a position vector
	*/
	static void orientVectorsFromVecX(const Vec3f& x, SkeletonJointOrientation &jointOrientation)
	{		
		// Columnn vectors (initialized with 0,0,0)
		Vec3f xCol;
		Vec3f yCol;
		Vec3f zCol;

		// x vector is used directly
		xCol = x.normalized();

		// y vector is set to be orthogonal to x, and pointing in parallel to the y-axis if the x vector is pointing in parallel to the x axis
		if (xCol.y != 0 || xCol.x != 0)
		{
			yCol.x = -xCol.y;
			yCol.y = xCol.x;
			yCol.z = 0;
			yCol.normalize();
		}
		else
		{
			yCol.y = 1.0f;
		}

		// z vector can now be calculated as the cross product of the others
		zCol = xCol.cross(yCol);

		// Now copy the values into matrix
		orientationVectorsToRotMat(jointOrientation, xCol, yCol, zCol);
	}

	/**
	* \brief Calculate orientation out of a position vector
	*/
	static void orientVectorsFromVecY(const Vec3f& v1, SkeletonJointOrientation &jointOrientation)
	{		
		// Columnn vectors (initialized with 0,0,0)
		Vec3f xCol;
		Vec3f yCol;
		Vec3f zCol;


		// y vector is used directly
		yCol = v1.normalized();

		// x vector is set to be orthogonal to y, and pointing in parallel to the x-axis if the y vector is pointing in parallel to the y axis
		if (yCol.x != 0 || yCol.y != 0)
		{
			xCol.x = yCol.y;
			xCol.y = -yCol.x;
			xCol.z = 0.0f;
			xCol.normalize();
		}
		else
		{
			xCol.x = 1.0f;
		}

		// z vector can now be calculated as the cross product of the others
		zCol = xCol.cross(yCol);

		// Now copy the values into matrix
		orientationVectorsToRotMat(jointOrientation, xCol, yCol, zCol);
	}

	/**
	* \brief Calculate orientation out of position vectors
	*/
	static void orientVectorsFromVecYX(const Vec3f& yUnnormalized, const Vec3f& xUnnormalized, SkeletonJointOrientation &jointOrientation)
	{		
		// Columnn vectors (initialized with 0,0,0)
		Vec3f xCol;
		Vec3f yCol;
		Vec3f zCol;

		// y vector is used directly
		yCol = yUnnormalized.normalized();
		// z vector is calculated as the cross product of x and y
		zCol = xUnnormalized.normalized().cross(yCol).normalized();
		// x vector is again calculated as the cross product of y and z (may be different from given x vector)
		xCol = yCol.cross(zCol);

		//copy values into matrix
		orientationVectorsToRotMat(jointOrientation, xCol, yCol, zCol);
	}

	/**
	* \brief Calculate orientation out of position vectors
	*/
	static void jointOrientationFromPositionsYX(const SkeletonJointPosition& yStart, const SkeletonJointPosition& yEnd,
		const SkeletonJointPosition& xStart, const SkeletonJointPosition& xEnd, SkeletonJointOrientation &jointOrientation)
	{
		float xConf = (xStart.m_confidence + xEnd.m_confidence) / 2.0f;
		float yConf = (yStart.m_confidence + yEnd.m_confidence) / 2.0f;
		jointOrientation.m_confidence = (xConf + yConf) / 2.0f;

		if (xConf > 0 && yConf > 0)
		{
			Vec3f vx = xEnd.m_position-xStart.m_position;
			Vec3f vy = yEnd.m_position-yStart.m_position;
			orientVectorsFromVecYX(vy, vx, jointOrientation);
		}
		else if (xConf > 0)
		{
			Vec3f vx = xEnd.m_position-xStart.m_position;
			orientVectorsFromVecX(vx, jointOrientation);
		}
		else if (yConf > 0)
		{
			Vec3f vy = yEnd.m_position-yStart.m_position;
			orientVectorsFromVecY(vy, jointOrientation);
		}
	}

	/**
	* \brief Calculate orientation out of position vectors
	*/
	static void jointOrientationFromPositionY(const SkeletonJointPosition& yStart, const SkeletonJointPosition& yEnd, SkeletonJointOrientation &jointOrientation)
	{
		jointOrientation.m_confidence = (yStart.m_confidence + yEnd.m_confidence) / 2.0f;

		if (jointOrientation.m_confidence > 0)
		{
			Vec3f vy = yEnd.m_position - yStart.m_position;
			orientVectorsFromVecY(vy, jointOrientation);
		}
	}

	/**
	* \brief Calculate orientation out of a position vector
	*/
	static void jointOrientationFromPositionX(const SkeletonJointPosition& xStart, const SkeletonJointPosition& xEnd,
		SkeletonJointOrientation &jointOrientation)
	{
		jointOrientation.m_confidence = (xStart.m_confidence + xEnd.m_confidence) / 2.0f;
		if (jointOrientation.m_confidence > 0)
		{
			Vec3f vx = xEnd.m_position-xStart.m_position;
			orientVectorsFromVecX(vx, jointOrientation);
		}
	}

	/**
	* \brief Approximate global orientations out of the global positions
	*/
	static void approximateGlobalOrientations(const SkeletonJointPosition* globalPos, SkeletonJointOrientation* globalOrients)
	{
		Vec3f vx;
		Vec3f vy;
		Vec3f temp;

		// Torso uses left-to-right-shoulder for x and torso-to-neck for y
		jointOrientationFromPositionsYX(globalPos[SkeletonJoint::TORSO], globalPos[SkeletonJoint::NECK], 
			globalPos[SkeletonJoint::LEFT_SHOULDER], globalPos[SkeletonJoint::RIGHT_SHOULDER],
			globalOrients[SkeletonJoint::TORSO]);

		// Waist uses the same as torso
		globalOrients[SkeletonJoint::WAIST] = globalOrients[SkeletonJoint::TORSO];

		// Neck uses left-to-right-shoulder for x and neck-to-head for y
		jointOrientationFromPositionsYX(globalPos[SkeletonJoint::NECK], globalPos[SkeletonJoint::HEAD],
			globalPos[SkeletonJoint::LEFT_SHOULDER], globalPos[SkeletonJoint::RIGHT_SHOULDER],
			globalOrients[SkeletonJoint::NECK]);

		// Head uses same as neck
		globalOrients[SkeletonJoint::HEAD] = globalOrients[SkeletonJoint::NECK]; 

		// Shoulder uses elbow-to-shoulder for x
		jointOrientationFromPositionX(globalPos[SkeletonJoint::LEFT_ELBOW], globalPos[SkeletonJoint::LEFT_SHOULDER],
			globalOrients[SkeletonJoint::LEFT_SHOULDER]);

		// Elbow uses hand-to-elbow for x
		jointOrientationFromPositionX(globalPos[SkeletonJoint::LEFT_HAND], globalPos[SkeletonJoint::LEFT_ELBOW],
			globalOrients[SkeletonJoint::LEFT_ELBOW]);

		// Hand/wrist uses the same as elbow
		globalOrients[SkeletonJoint::LEFT_WRIST] = globalOrients[SkeletonJoint::LEFT_HAND] = globalOrients[SkeletonJoint::LEFT_ELBOW];

		// Hip uses knee-to-hip for y and left-to-right-hip for x
		jointOrientationFromPositionsYX(globalPos[SkeletonJoint::LEFT_KNEE], globalPos[SkeletonJoint::LEFT_HIP],
			globalPos[SkeletonJoint::LEFT_HIP], globalPos[SkeletonJoint::RIGHT_HIP],
			globalOrients[SkeletonJoint::LEFT_HIP]);

		// Knee users foot-to-knee for y 	
		jointOrientationFromPositionY(globalPos[SkeletonJoint::LEFT_FOOT], globalPos[SkeletonJoint::LEFT_KNEE],
			globalOrients[SkeletonJoint::LEFT_KNEE]);

		// Foot/Ankle uses the same as knee
		globalOrients[SkeletonJoint::LEFT_ANKLE] = globalOrients[SkeletonJoint::LEFT_FOOT] = globalOrients[SkeletonJoint::LEFT_KNEE];


		// Shoulder uses shoulder-to-elbow for x
		jointOrientationFromPositionX(globalPos[SkeletonJoint::RIGHT_SHOULDER], globalPos[SkeletonJoint::RIGHT_ELBOW],
			globalOrients[SkeletonJoint::RIGHT_SHOULDER]);

		// Elbow uses elbow-to-hand for x
		jointOrientationFromPositionX(globalPos[SkeletonJoint::RIGHT_ELBOW], globalPos[SkeletonJoint::RIGHT_HAND],
			globalOrients[SkeletonJoint::RIGHT_ELBOW]);

		// Hand/wrist uses the same as elbow
		globalOrients[SkeletonJoint::RIGHT_WRIST] = globalOrients[SkeletonJoint::RIGHT_HAND] = globalOrients[SkeletonJoint::RIGHT_ELBOW];

		// Hip uses knee-to-hip for y and left-to-right-hip for x
		jointOrientationFromPositionsYX(globalPos[SkeletonJoint::RIGHT_KNEE], globalPos[SkeletonJoint::RIGHT_HIP],
			globalPos[SkeletonJoint::LEFT_HIP], globalPos[SkeletonJoint::RIGHT_HIP],
			globalOrients[SkeletonJoint::RIGHT_HIP]);

		// Knee users foot-to-knee for y 		
		jointOrientationFromPositionY(globalPos[SkeletonJoint::RIGHT_FOOT], globalPos[SkeletonJoint::RIGHT_KNEE],
			globalOrients[SkeletonJoint::RIGHT_KNEE]);

		// Foot/ankle uses the same as knee
		globalOrients[SkeletonJoint::RIGHT_ANKLE] = globalOrients[SkeletonJoint::RIGHT_FOOT] = globalOrients[SkeletonJoint::RIGHT_KNEE];
	}

	/*! @}*/
};
