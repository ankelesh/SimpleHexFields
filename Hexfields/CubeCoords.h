#pragma once
#include "stdafx.h"
#include <list>
#include <vector>
#include <exception>
#include <sstream>
#include "debug.h"

class bad_coordinates_exception : std::exception
{
	const char * what() const
	{ return "Bad given coordinates in CubeCoords constructor"; };
};



enum CubeDirections { Righttop, Right, Rigthlow, Leftlow, Left, Lefttop };
static const int directions[6][3]{
	{1,-1,0}, {1,0,-1},
	{0,1,-1}, {-1,1,0},
	{-1,0,1},{0,-1,1}
};
static const int diagonals[6][3]{
	{2, -1, -1}, {1, 1, -2},
	{-1, 2, -1}, {-2, 1, 1}, 
	{-1, -1, 2}, {1, -2, 1}
};

template <class Number>
class CubeCoords
{
private:
	Number x;
	Number y;
	Number z;
public:
	CubeCoords(const Number & x, const Number & y, const Number & z);
	CubeCoords(const CubeCoords<Number> & second);
	template <class From>
	explicit CubeCoords(const CubeCoords<From> & second);
	CubeCoords<Number> operator+(const CubeCoords<Number> & second) const;
	CubeCoords<Number> operator-(const CubeCoords<Number> & second) const;
	bool operator==(const CubeCoords<Number> & second) const;
	bool operator<(const CubeCoords<Number> & second) const;
	CubeCoords<Number> & operator+=(const CubeCoords<Number> & second);
	CubeCoords<Number> & operator-=(const CubeCoords < Number> & second);
	CubeCoords<Number> operator+() const;
	CubeCoords<Number> operator-() const;
	CubeCoords<Number> & operator=(const CubeCoords<Number> & second);
	Number distanceTo(const CubeCoords<Number> & second);
	std::vector<CubeCoords<Number>> createLine(const CubeCoords<Number> & to);
	std::vector<CubeCoords<Number>>	fromCenterInRange(const int & range);
	friend CubeCoords<Number> cubeLinearInterpolation(
		CubeCoords<Number> & start, CubeCoords<Number> & finish, double t)
	{
		return CubeCoords<Number>
			(
				start.x + (finish.x - start.x) * t,
				start.y + (finish.y - start.y) * t,
				start.z + (finish.z - start.z) * t
				);
	};
	CubeCoords<Number> rounded();
	void roundThis();
	CubeCoords<Number> neighbor(const CubeDirections & dir);
	CubeCoords<Number> diagonal(const CubeDirections & dir);
	std::string toStr();
	Number getX() const { return x; }
	Number getY() const { return y; }
	Number getZ() const { return z; }
};
typedef CubeCoords<int> ICube;
typedef CubeCoords<double> FloatCube;




template <class Number>
Number round(Number & num)
{
	return (num > (abs(num + 0.5))) ? ceil(num) : floor(num);
}
template<class Number>
CubeCoords<Number>::CubeCoords(const Number & cx, const Number & cy, const Number & cz)
	:	x(cx), y(cy), z(cz)
{
	if ((int)(x + y + z)) throw bad_coordinates_exception();
}
template<class Number>
CubeCoords<Number>::CubeCoords(const CubeCoords<Number> & second)
	: x(second.x), y(second.y), z(second.z) { };
template<class Number> template < class From>
CubeCoords<Number>::CubeCoords(const CubeCoords<From> & second)
{
	x = (Number)second.getX();
	y = (Number)second.getY(); 
	z = (Number)second.getZ();
}
template<class Number>
CubeCoords<Number> CubeCoords<Number>::operator+(const CubeCoords<Number> & second) const
{
	return CubeCoords<Number>(second.x + x, second.y + y, second.z + z);
}
template<class Number>
CubeCoords<Number> CubeCoords<Number>::operator-(const CubeCoords<Number> & second) const
{
	return CubeCoords<Number>(second.x - x, second.y - y, second.z - z);
};
template<class Number>
bool CubeCoords<Number>::operator==(const CubeCoords<Number> & second) const
{
	return (second.x == x && second.y == y && second.z == z);
};
template<class Number>
bool CubeCoords<Number>::operator<(const CubeCoords<Number> & second) const
{
	CubeCoords<Number> zero(0, 0, 0);
	return (distanceTo(zero) < second.distanceTo(zero));
}
template <class Number>
CubeCoords<Number> & CubeCoords<Number>::operator+=(const CubeCoords<Number> & second)
{
	x += second.x;
	y += second.y;
	z += second.z;
	return *this;
};
template <class Number>
CubeCoords<Number> & CubeCoords<Number>::operator-=(const CubeCoords < Number> & second)
{
	x -= second.x;
	y -= second.y;
	z -= second.z;
	return *this;
};
template<class Number>
CubeCoords<Number> CubeCoords<Number>::operator+() const
{
	return CubeCoords<Number>(*this);
};
template<class Number>
CubeCoords<Number> CubeCoords<Number>::operator-() const
{
	return CubeCoords<Number>(-x, -y, -z);
};
template <class Number>
CubeCoords<Number> & CubeCoords<Number>::operator=(const CubeCoords<Number> & second)
{
	x = second.x; y = second.y; z = second.z;
	return *this;
};
template<class Number>
Number CubeCoords<Number>::distanceTo(const CubeCoords<Number> & second)
{
	return (abs(x - second.x) + abs(y - second.y) + abs(z - second.z)) / 2;
};
template<class Number>
std::vector<CubeCoords<Number>> CubeCoords<Number>::createLine(const CubeCoords<Number> & to)
{
	std::vector<CubeCoords<Number>> temp;
	temp.reserve((int)distanceTo(to));
	Number distance = distanceTo(to);
	auto start = *this + CubeCoords<Number>(0.0000001, 0.000001, 0.000001);
	auto finish = to + CubeCoords<Number>(0.000001, 0.0000001, 0.000001);
	int end = distance + 1;
	for (int i = 0; i < end; ++i)
	{
		temp.push_back(cubeLinearInterpolation(start, finish, (1 / (float)distance * i)).rounded());
	}
	return temp;
};
template <> 
std::vector<CubeCoords<int>> CubeCoords<int>::createLine(const CubeCoords<int> & to)
{
	std::vector<CubeCoords<int>> temp;
	int dist = distanceTo(to);
	temp.reserve(dist);
	FloatCube start = FloatCube(*this) + FloatCube(0.0000001, 0.000001, 0.000001);
	FloatCube finish = FloatCube(to) + FloatCube(0.0000001, 0.000001, 0.000001);	
	int end = dist + 1;
	for (int i = 0; i < end; ++i)
	{
		temp.push_back((ICube)(cubeLinearInterpolation(start, finish, (1 / (float) dist * i))).rounded());
	}
	return temp;
}
template<class Number>
std::vector<CubeCoords<Number>>	CubeCoords<Number>::fromCenterInRange(const int & range)
{
	std::vector<CubeCoords<Number>> temp;
	int end = range + 1;
	for (int i = -range; i < end; ++i)
	{
		int a = -range;
		int b = -i - range;
		int fr = (a > b) ? a : b;
		a = range; b = -i + range;
		int to = (a < b) ? a : b;
		++to;
		for (int j = fr; j < to; ++j)
		{
			a = -i - j;
			temp.push_back(*this + CubeCoords<Number>(i, j, a));
		}
	}
	return temp;
};
template<class Number>
std::string CubeCoords<Number>::toStr()
{
	std::ostringstream sout;
	sout << ' ' << x << ' ' << y << ' ' << z << ' ';
	return sout.str();
};
template <class Number>
void CubeCoords<Number>::roundThis()

{
	Number rx = round(x);
	Number ry = round(y);
	Number rz = round(z);
	Number x_diff = abs(rx - x);
	Number y_diff = abs(ry - y);
	Number z_diff = abs(rz - z);
	if (x_diff > y_diff && y_diff > z_diff)
	{	rx = -ry - rz;   }
	else if (y_diff > z_diff)
	{	ry = -rx - rz;   }
	else
	{	rz = -rx - ry;	}
	x = rx; y = ry; z = rz;
}
template <class Number>
CubeCoords<Number> CubeCoords<Number>::rounded()
{
	CubeCoords<Number> temp(*this);
	temp.roundThis();
	return temp;
}
template<class Number>
CubeCoords<Number> CubeCoords<Number>::neighbor(const CubeDirections & dir)
{
	return *this + CubeCoords<Number>(directions[dir][0], directions[dir][1], directions[dir][2]);
}
template<class Number>
CubeCoords<Number> CubeCoords<Number>::diagonal(const CubeDirections & dir)
{
	return *this + CubeCoords<Number>(diagonals[dir][0], diagonals[dir][1], diagonals[dir][2]);
}