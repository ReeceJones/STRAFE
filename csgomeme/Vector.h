#pragma once
#define CHECK_VALID( _v ) 0
#define Assert( _exp ) ((void)0)
typedef unsigned __int32		uint32;
#include <cmath>
inline float BitsToFloat(uint32 i)
{
	union Convertor_t
	{
		float f;
		unsigned long ul;
	}tmp;
	tmp.ul = i;
	return tmp.f;
}

#define FLOAT32_NAN_BITS     (uint32)0x7fc00000	
#define FLOAT32_NAN          BitsToFloat( FLOAT32_NAN_BITS )

#define VEC_T_NAN FLOAT32_NAN

#define DECL_ALIGN(x)			__declspec( align( x ) )

#define ALIGN4 DECL_ALIGN(4)
#define ALIGN8 DECL_ALIGN(8)
#define ALIGN16 DECL_ALIGN(16)
#define ALIGN32 DECL_ALIGN(32)
#define ALIGN128 DECL_ALIGN(128)

class QAngleByValue;
class Vector
{
public:
	float x, y, z;
	Vector(void)
	{

	}
	Vector(float X, float Y, float Z)
	{
		x = X; y = Y; z = Z;
		CHECK_VALID(*this);
	}
	void Init(float ix = 0.0f, float iy = 0.0f, float iz = 0.0f)
	{
		x = ix; y = iy; z = iz;
		CHECK_VALID(*this);
	}
	bool IsValid() const;
	float operator[](int i) const
	{
		Assert((i >= 0) && (i < 3));
		return ((float*)this)[i];
	}
	float& operator[](int i)
	{
		Assert((i >= 0) && (i < 3));
		return ((float*)this)[i];
	}
	void Zero()
	{
		x = y = z = 0.0f;
	}
	bool operator==(const Vector& v) const
	{
		CHECK_VALID(src);
		CHECK_VALID(*this);
		return (v.x == x) && (v.y == y) && (v.z == z);
	}
	bool operator!=(const Vector& v) const
	{
		CHECK_VALID(src);
		CHECK_VALID(*this);
		return (v.x != x) || (v.y != y) || (v.z != z);
	}
	Vector&	operator+=(const Vector &v)
	{
		CHECK_VALID(*this);
		CHECK_VALID(v);
		x += v.x; y += v.y; z += v.z;
		return *this;
	}
	Vector&	operator-=(const Vector &v)
	{
		CHECK_VALID(*this);
		CHECK_VALID(v);
		x -= v.x; y -= v.y; z -= v.z;
		return *this;
	}
	Vector&	operator*=(const Vector &v)
	{
		CHECK_VALID(v);
		x *= v.x;
		y *= v.y;
		z *= v.z;
		CHECK_VALID(*this);
		return *this;
	}
	Vector&	operator*=(float s)
	{
		x *= s;
		y *= s;
		z *= s;
		CHECK_VALID(*this);
		return *this;
	}
	Vector&	operator/=(const Vector &v)
	{
		CHECK_VALID(v);
		Assert(v.x != 0.0f && v.y != 0.0f && v.z != 0.0f);
		x /= v.x;
		y /= v.y;
		z /= v.z;
		CHECK_VALID(*this);
		return *this;
	}
	Vector&	operator/=(float s)
	{
		Assert(s != 0.0f);
		float oofl = 1.0f / s;
		x *= oofl;
		y *= oofl;
		z *= oofl;
		CHECK_VALID(*this);
		return *this;
	}
	Vector&	operator+=(float fl)
	{
		x += fl;
		y += fl;
		z += fl;
		CHECK_VALID(*this);
		return *this;
	}
	Vector&	operator-=(float fl)
	{
		x -= fl;
		y -= fl;
		z -= fl;
		CHECK_VALID(*this);
		return *this;
	}
	float	Length() const
	{
		CHECK_VALID(*this);

		float root = 0.0f;

		float sqsr = x*x + y*y + z*z;

		__asm sqrtss xmm0, sqsr
		__asm movss root, xmm0

		return root;
	}
	float LengthSqr(void) const
	{
		CHECK_VALID(*this);
		return (this->x*this->x + this->y*this->y + this->z*this->z);
	}
	bool IsZero(float tolerance = 0.01f) const
	{
		return (x > -tolerance && x < tolerance && y > -tolerance && y < tolerance && z > -tolerance && z < tolerance);
	}
	float	NormalizeInPlace()
	{
		Vector& v = *this;

		float iradius = 1.f / (this->Length() + 1.192092896e-07F);
		v.x *= iradius;
		v.y *= iradius;
		v.z *= iradius;
		return iradius;
	}
	Vector	Normalize()
	{
		Vector vector;
		float length = this->Length();

		if (length != 0) {
			vector.x = x / length;
			vector.y = y / length;
			vector.z = z / length;
		}
		else
			vector.x = vector.y = 0.0f; vector.z = 1.0f;

		return vector;
	}
	float	DistToSqr(const Vector &vOther) const
	{
		Vector delta;

		delta.x = x - vOther.x;
		delta.y = y - vOther.y;
		delta.z = z - vOther.z;

		return delta.LengthSqr();
	}
	float	Dist(const Vector &vOther) const
	{
		Vector delta;

		delta.x = x - vOther.x;
		delta.y = y - vOther.y;
		delta.z = z - vOther.z;

		return delta.Length();
	}
	float	Dot(const Vector& vOther) const
	{
		const Vector& a = *this;

		return(a.x*vOther.x + a.y*vOther.y + a.z*vOther.z);
	}
	float	Dot(const float* fOther) const
	{
		const Vector& a = *this;

		return(a.x*fOther[0] + a.y*fOther[1] + a.z*fOther[2]);
	}
	float	Length2D(void) const
	{
		CHECK_VALID(*this);

		float root = 0.0f;

		float sqst = x*x + y*y;

		__asm
		{
			sqrtss xmm0, sqst
			movss root, xmm0
		}

		return root;
	}
	float	Length2DSqr(void) const
	{
		return (x*x + y*y);
	}
	Vector& operator=(const Vector &vOther)
	{
		CHECK_VALID(vOther);
		x = vOther.x; y = vOther.y; z = vOther.z;
		return *this;
	}
	Vector	operator-(const Vector& v) const
	{
		Vector res;
		res.x = x - v.x;
		res.y = y - v.y;
		res.z = z - v.z;
		return res;
	}
	Vector	operator+(const Vector& v) const
	{
		Vector res;
		res.x = x + v.x;
		res.y = y + v.y;
		res.z = z + v.z;
		return res;
	}
	Vector	operator*(const Vector& v) const
	{
		Vector res;
		res.x = x * v.x;
		res.y = y * v.y;
		res.z = z * v.z;
		return res;
	}
	Vector	operator/(const Vector& v) const
	{
		Vector res;
		res.x = x / v.x;
		res.y = y / v.y;
		res.z = z / v.z;
		return res;
	}
	Vector	operator*(float fl) const
	{
		Vector res;
		res.x = x * fl;
		res.y = y * fl;
		res.z = z * fl;
		return res;
	}
	Vector	operator/(float fl) const
	{
		Vector res;
		res.x = x / fl;
		res.y = y / fl;
		res.z = z / fl;
		return res;
	}
	float* Base()
	{
		return (float*)this;
	}
	float const* Base() const
	{
		return (float const*)this;
	}
};
class QAngle
{
public:
	float x, y, z;

	QAngle(void)
	{
#ifdef _DEBUG
#ifdef VECTOR_PARANOIA
		x = y = z = float_NAN;
#endif
#endif
	}
	QAngle(float X, float Y, float Z)
	{
		x = X; y = Y; z = Z;
		CHECK_VALID(*this);
	}

	operator QAngleByValue &() { return *((QAngleByValue *)(this)); }
	operator const QAngleByValue &() const { return *((const QAngleByValue *)(this)); }

	void Init(float ix = 0.0f, float iy = 0.0f, float iz = 0.0f)
	{
		x = ix; y = iy; z = iz;
		CHECK_VALID(*this);
	}
	void Random(float minVal, float maxVal)
	{
		x = minVal + ((float)rand() / RAND_MAX) * (maxVal - minVal);
		y = minVal + ((float)rand() / RAND_MAX) * (maxVal - minVal);
		z = minVal + ((float)rand() / RAND_MAX) * (maxVal - minVal);
		CHECK_VALID(*this);
	}

	bool IsValid() const;
	void Invalidate();

	bool IsZero()
	{
		CHECK_VALID(*this);
		if (this->x == 0.f && this->y == 0.f && this->z == 0.f)
			return true;

		return false;
	}

	float operator[](int i) const
	{
		Assert((i >= 0) && (i < 3));
		return ((float*)this)[i];
	}
	float& operator[](int i)
	{
		Assert((i >= 0) && (i < 3));
		return ((float*)this)[i];
	}

	float* Base()
	{
		return (float*)this;
	}
	float const* Base() const
	{
		return (float const*)this;
	}

	bool operator==(const QAngle& v) const
	{
		CHECK_VALID(src);
		CHECK_VALID(*this);
		return (v.x == x) && (v.y == y) && (v.z == z);
	}
	bool operator!=(const QAngle& v) const
	{
		CHECK_VALID(src);
		CHECK_VALID(*this);
		return (v.x != x) || (v.y != y) || (v.z != z);
	}

	QAngle& operator+=(const QAngle &v)
	{
		CHECK_VALID(*this);
		CHECK_VALID(v);
		x += v.x; y += v.y; z += v.z;
		return *this;
	}
	QAngle& operator-=(const QAngle &v)
	{
		CHECK_VALID(*this);
		CHECK_VALID(v);
		x -= v.x; y -= v.y; z -= v.z;
		return *this;
	}
	QAngle& operator*=(float s)
	{
		x *= s;
		y *= s;
		z *= s;
		CHECK_VALID(*this);
		return *this;
	}
	QAngle& operator/=(float s)
	{
		Assert(s != 0.0f);
		float oofl = 1.0f / s;
		x *= oofl;
		y *= oofl;
		z *= oofl;
		CHECK_VALID(*this);
		return *this;
	}

	float   Length() const
	{
		CHECK_VALID(*this);
		return (float)sqrt(LengthSqr());
	}
	float   LengthSqr() const
	{
		CHECK_VALID(*this);
		return x * x + y * y;
	}

	QAngle& operator=(const QAngle& src)
	{
		CHECK_VALID(vOther);
		x = src.x; y = src.y; z = src.z;
		return *this;
	}

	QAngle  operator-(void) const
	{
		return QAngle(-x, -y, -z);
	}

	QAngle  operator+(const QAngle& v) const
	{
		QAngle res;
		res.x = x + v.x;
		res.y = y + v.y;
		res.z = z + v.z;
		return res;
	}
	QAngle  operator-(const QAngle& v) const
	{
		QAngle res;
		res.x = x - v.x;
		res.y = y - v.y;
		res.z = z - v.z;
		return res;
	}
	QAngle  operator*(float fl) const
	{
		QAngle res;
		res.x = x * fl;
		res.y = y * fl;
		res.z = z * fl;
		return res;
	}
	QAngle  operator/(float fl) const
	{
		QAngle res;
		res.x = x / fl;
		res.y = y / fl;
		res.z = z / fl;
		return res;
	}
	QAngle Normalize()
	{
		if (this->y > 180 || this->y < -180)
		{
			auto AngleY = this->y / 360;
			if (AngleY < 0)
				AngleY = -AngleY;

			AngleY = round(AngleY);

			if (this->y < 0)
				this->y += 360 * AngleY;
			else
				this->y -= 360 * AngleY;
		}

		if (this->x > 89 || this->x < -89)
		{
			auto AngleX = this->x / 360;
			if (AngleX < 0)
				AngleX = -AngleX;

			AngleX = round(AngleX);

			if (this->x < 0)
				this->x += 360 * AngleX;
			else
				this->x -= 360 * AngleX;
		}

		this->z = 0;

		return *this;
	}

	QAngle Clamp()
	{
		if (this->y > 180.f || this->y < -180.f)
		{
			if (this->y > 180)
				this->y = 180.f;
			else
				this->y = -180.f;
		}
		if (this->x > 89.f || this->x < -89.f)
		{
			if (this->x > 89.f)
				this->x = 89.f;
			else
				this->x = -89.f;
		}
		this->z = 0.f;
		return  *this;
	}

	QAngle ClampInPlace()
	{
		return (*this = QAngle((x > 89.f ? 89.f : x < -89.f ? -89.f : x), (y > 180.f ? 180.f : y < -180.f ? -180.f : y), 0));
	}

	QAngle Mod(float N)
	{
		CHECK_VALID(*this);
		this->x = fmod(x, N);
		this->y = fmod(y, N);
		this->z = fmod(z, N);

		return *this;
	}
};
class Quaternion				// same data-layout as engine's vec4_t,
{								//		which is a vec_t[4]
public:
	inline Quaternion(void) 
	{

	}
	inline Quaternion(float ix, float iy, float iz, float iw) : x(ix), y(iy), z(iz), w(iw) { }

	inline void Init(float ix = 0.0f, float iy = 0.0f, float iz = 0.0f, float iw = 0.0f) { x = ix; y = iy; z = iz; w = iw; }

	bool IsValid() const;
	void Invalidate();

	bool operator==(const Quaternion &src) const;
	bool operator!=(const Quaternion &src) const;

	float* Base() { return (float*)this; }
	const float* Base() const { return (float*)this; }

	// array access...
	float operator[](int i) const;
	float& operator[](int i);

	float x, y, z, w;
};
class RadianEuler
{
public:
	inline RadianEuler(void) { }
	inline RadianEuler(float X, float Y, float Z) { x = X; y = Y; z = Z; }
	inline RadianEuler(Quaternion const &q);	// evil auto type promotion!!!
	inline RadianEuler(QAngle const &angles);	// evil auto type promotion!!!

												// Initialization
	inline void Init(float ix = 0.0f, float iy = 0.0f, float iz = 0.0f) { x = ix; y = iy; z = iz; }

	//	conversion to qangle
	QAngle ToQAngle(void) const;
	bool IsValid() const;
	void Invalidate();

	// array access...
	float operator[](int i) const;
	float& operator[](int i);

	float x, y, z;
};