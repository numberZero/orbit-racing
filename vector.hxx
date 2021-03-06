#pragma once
#include <cmath>
#include <initializer_list>
#include <stdexcept>
#include <type_traits>

template <typename Scalar, int Length>
struct Vector
{
	static_assert(Length >= 1, "Invalid vector length");
	static_assert(std::is_floating_point<Scalar>::value, "Vector must have floating-point elements");
	constexpr static int const n = Length;
	constexpr static int const N = Length;

	Vector() = default;
	Vector(Vector const &) = default;
	Vector &operator= (Vector const &) = default;

	template <typename Scalar2>
	explicit Vector(Vector<Scalar2, Length> const &b)
	{
		for (int k = 0; k < n; k++)
			data[k] = Scalar(b[k]);
	}

	Vector(std::initializer_list<Scalar> list)
	{
		if (list.size() != Length)
			throw std::invalid_argument("Invalid vector initializer list length");
		int k = 0;
		for (Scalar x: list)
			data[k++] = x;
	}

	operator double * ()
	{
		return data;
	}

	operator double const * () const
	{
		return data;
	}

	void setZero()
	{
		std::fill(data, data + N, Scalar(0));
	}

	Scalar dot(Vector const &b) const
	{
		return *this * b;
	}

	Scalar norm() const
	{
		return std::sqrt(squaredNorm());
	}

	Scalar squaredNorm() const
	{
		return *this * *this;
	}

	Vector normed() const
	{
		return *this / norm();
	}

	Scalar &operator[] (int index)
	{
		if(index < 0 || index >= N)
			throw std::out_of_range("Vector index out of range");
		return data[index];
	}

	Scalar const &operator[] (int index) const
	{
		if(index < 0 || index >= N)
			throw std::out_of_range("Vector index out of range");
		return data[index];
	}

	Vector &operator+= (Vector const &b)
	{
		for(int i = 0; i != N; ++i)
			data[i] += b.data[i];
		return *this;
	}

	Vector &operator-= (Vector const &b)
	{
		for(int i = 0; i != N; ++i)
			data[i] -= b.data[i];
		return *this;
	}

	Vector &operator*= (Scalar c)
	{
		for(int i = 0; i != N; ++i)
			data[i] *= c;
		return *this;
	}

	Vector &operator/= (Scalar c)
	{
		return *this *= 1 / c;
	}

	union
	{
		Scalar item[N];
		Scalar data[N];
	};
};

template <typename T, int n>
inline Vector<T, n> operator- (Vector<T, n> const &a)
{
	return T(-1) * a;
}

template <typename T, int n>
inline Vector<T, n> operator+ (Vector<T, n> const &a, Vector<T, n> const &b)
{
	return Vector<T, n>(a) += b;
}

template <typename T, int n>
inline Vector<T, n> operator- (Vector<T, n> const &a, Vector<T, n> const &b)
{
	return Vector<T, n>(a) -= b;
}

template <typename T, int n>
inline Vector<T, n> operator* (Vector<T, n> const &a, T c)
{
	return Vector<T, n>(a) *= c;
}

template <typename T, int n>
inline Vector<T, n> operator* (T c, Vector<T, n> const &a)
{
	return Vector<T, n>(a) *= c;
}

template <typename T, int n>
inline Vector<T, n> operator/ (Vector<T, n> const &a, T c)
{
	return Vector<T, n>(a) /= c;
}

template <typename T, int n>
inline T operator* (Vector<T, n> const &a, Vector<T, n> const &b)
{
	T x = 0;
	for(int i = 0; i != n; ++i)
		x += a[i] * b[i];
	return x;
}

template <typename T>
inline Vector<T, 2> directionVector(T angle, T length = T(1))
{
	return {
		length * std::cos(angle),
		length * std::sin(angle)
	};
}

template <typename T>
inline Vector<T, 2> ortho(Vector<T, 2> v)
{
	return {
		-v[1],
		v[0]
	};
}

template <typename T>
inline T cross(Vector<T, 2> u, Vector<T, 2> v)
{
	return u[0] * v[1] - u[1] * v[0];
}

template <typename T, int n>
inline T cosphi(Vector<T, n> u, Vector<T, n> v)
{
	return (u * v) / std::sqrt(u.squaredNorm() * v.squaredNorm());
}

template <typename T>
inline T sinphi(Vector<T, 2> u, Vector<T, 2> v)
{
	return cross(u, v) / std::sqrt(u.squaredNorm() * v.squaredNorm());
}

template <typename T, int n>
inline T angle(Vector<T, n> u, Vector<T, n> v)
{
	return std::acos(cosphi(u, v));
}

template <typename T>
inline T sangle(Vector<T, 2> u, Vector<T, 2> v)
{
	return std::asin(sinphi(u, v));
}
