#pragma once

#include <sstream>
#define CHECK_VALID(_v) 0
#define PI2 3.14159265358979323846f
#define Assert(_exp) ((void)0)


class Vector
{
public:
    Vector(void)
    {
        Invalidate();
    }
    Vector(float X, float Y, float Z)
    {
        x = X;
        y = Y;
        z = Z;
    }
    Vector(const float* clr)
    {
        x = clr[0];
        y = clr[1];
        z = clr[2];
    }

    void Init(float ix = 0.0f, float iy = 0.0f, float iz = 0.0f)
    {
        x = ix; y = iy; z = iz;
    }

    __inline void Mul(float scalar)
	{
		x *= scalar;
		y *= scalar;
		z *= scalar;
	}

    float Normalize() const
    {
        Vector res = *this;
        float l = res.Length();
        if (l != 0.0f)
        {
            res /= l;
        }
        else
        {
            res.x = res.y = res.z = 0.0f;
        }
        return l;
    }
   


	__inline void MulAdd(const Vector& a, const Vector& b, float scalar) {
		x = a.x + b.x * scalar;
		y = a.y + b.y * scalar;
		z = a.z + b.z * scalar;
	}
    bool IsValid() const
    {
        return std::isfinite(x) && std::isfinite(y) && std::isfinite(z);
    }

    bool IsZero(float tolerance = 0.01f) const
    {
        return (x > -tolerance && x < tolerance &&
            y > -tolerance && y < tolerance &&
            z > -tolerance && z < tolerance);
    }


    void Invalidate()
    {
        x = y = z = std::numeric_limits<float>::infinity();
    }

    float& operator[](int i)
    {
        return ((float*)this)[i];
    }
    float operator[](int i) const
    {
        return ((float*)this)[i];
    }

    void Zero()
    {
        x = y = z = 0.0f;
    }

    bool operator==(const Vector& src) const
    {
        return (src.x == x) && (src.y == y) && (src.z == z);
    }
    bool operator!=(const Vector& src) const
    {
        return (src.x != x) || (src.y != y) || (src.z != z);
    }

    Vector& operator+=(const Vector& v)
    {
        x += v.x; y += v.y; z += v.z;
        return *this;
    }
    Vector& operator-=(const Vector& v)
    {
        x -= v.x; y -= v.y; z -= v.z;
        return *this;
    }


    Vector& operator*=(float fl)
    {
        x *= fl;
        y *= fl;
        z *= fl;
        return *this;
    }
    Vector& operator*=(const Vector& v)
    {
        x *= v.x;
        y *= v.y;
        z *= v.z;
        return *this;
    }
    Vector& operator/=(const Vector& v)
    {
        x /= v.x;
        y /= v.y;
        z /= v.z;
        return *this;
    }
    Vector& operator+=(float fl)
    {
        x += fl;
        y += fl;
        z += fl;
        return *this;
    }
    Vector& operator/=(float fl)
    {
        x /= fl;
        y /= fl;
        z /= fl;
        return *this;
    }
    Vector& operator-=(float fl)
    {
        x -= fl;
        y -= fl;
        z -= fl;
        return *this;
    }


    void NormalizeInPlace()
    {
        *this = Normalized();
    }
    Vector Normalized() const
    {
        Vector res = *this;
        float l = res.Length();
        if(l != 0.0f) {
            res /= l;
        } else {
            res.x = res.y = res.z = 0.0f;
        }
        return res;
    }

    float DistTo(const Vector &vOther) const
    {
        Vector delta;

        delta.x = x - vOther.x;
        delta.y = y - vOther.y;
        delta.z = z - vOther.z;

        return delta.Length();
    }

	float Dist(Vector other) {
		return (float)sqrt(pow(x - other.x, 2) + pow(y - other.y, 2) + pow(z - other.z, 2));

	}

    float DistToSqr(const Vector &vOther) const
    {
        Vector delta;

        delta.x = x - vOther.x;
        delta.y = y - vOther.y;
        delta.z = z - vOther.z;

        return delta.LengthSqr();
    }
    float Dot(const Vector& vOther) const
    {
        return (x*vOther.x + y*vOther.y + z*vOther.z);
    }


    void VectorCrossProduct(const Vector &a, const Vector &b, Vector &result)
    {
        result.x = a.y*b.z - a.z*b.y;
        result.y = a.z*b.x - a.x*b.z;
        result.z = a.x*b.y - a.y*b.x;


    }
    Vector Cross(const Vector &vOther)
    {
        Vector res;
        VectorCrossProduct(*this, vOther, res);
        return res;
    }



    float Length() const
    {
        return sqrt(x*x + y*y + z*z);
    }
    float LengthSqr(void) const
    {
        return (x*x + y*y + z*z);
    }
    float Length2D() const
    {
        return sqrt(x*x + y*y);
    }

    Vector& operator=(const Vector &vOther)
    = default;

    Vector operator-(void) const
    {
        return Vector(-x, -y, -z);
    }
    Vector operator+(const Vector& v) const
    {
        return Vector(x + v.x, y + v.y, z + v.z);
    }

    Vector Vector::operator+(float fl) const
    {
        return Vector(x + fl, y + fl, z + fl);
    }

    Vector operator-(const Vector& v) const
    {
        return Vector(x - v.x, y - v.y, z - v.z);
    }

    Vector Vector::operator-(float fl) const
    {
        return Vector(x - fl, y - fl, z - fl);
    }


    Vector operator*(float fl) const
    {
        return Vector(x * fl, y * fl, z * fl);
    }
    Vector operator*(const Vector& v) const
    {
        return Vector(x * v.x, y * v.y, z * v.z);
    }
    Vector operator/(float fl) const
    {
        return Vector(x / fl, y / fl, z / fl);
    }
    Vector operator/(const Vector& v) const
    {
        return Vector(x / v.x, y / v.y, z / v.z);
    }

    float x, y, z;
};

inline Vector operator*(float lhs, const Vector& rhs)
{
    return rhs * lhs;
}
inline Vector operator/(float lhs, const Vector& rhs)
{
    return rhs / lhs;
}

inline Vector angle_vector2(Vector meme)
{
    auto sy = sin(meme.y / 180.f * static_cast<float>(PI2));
    auto cy = cos(meme.y / 180.f * static_cast<float>(PI2));

    auto sp = sin(meme.x / 180.f * static_cast<float>(PI2));
    auto cp = cos(meme.x / 180.f* static_cast<float>(PI2));

    return Vector(cp*cy, cp*sy, -sp);
}


class __declspec(align(16)) VectorAligned : public Vector
{
public:
    inline VectorAligned(void) {};
    inline VectorAligned(float X, float Y, float Z)
    {
        Init(X, Y, Z);
    }

public:
    explicit VectorAligned(const Vector &vOther)
    {
        Init(vOther.x, vOther.y, vOther.z);
    }

    VectorAligned& operator=(const Vector &vOther)
    {
        Init(vOther.x, vOther.y, vOther.z);
        return *this;
    }

    VectorAligned& operator=(const VectorAligned &vOther)
    {
        Init(vOther.x, vOther.y, vOther.z);
        return *this;
    }

    float w;
};

inline void VectorMA(const Vector &start, float scale, const Vector &direction, Vector &dest)
{
    CHECK_VALID(start);
    CHECK_VALID(direction);

    dest.x = start.x + scale * direction.x;
    dest.y = start.y + scale * direction.y;
    dest.z = start.z + scale * direction.z;
}

inline void VectorMultiply(const Vector &a, float b, Vector &c)
{
    CHECK_VALID(a);
    Assert(IsFinite(b));
    c.x = a.x * b;
    c.y = a.y * b;
    c.z = a.z * b;
}

