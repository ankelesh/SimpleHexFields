#pragma once
#include "stdafx.h"
#include <list>
#include <vector>
#include <exception>
#include <sstream>
#include "debug.h"




namespace HexCoords {

	static const int hash_multiplier_x = 100000000;
	static const int hash_multiplier_y = 10000;
	static const int hash_multiplier_z = 1;

	class bad_coordinates_exception : std::exception
	{
		const char * what() const
		{
			return "Bad given coordinates in CubeCoords constructor";
		};
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
		CubeCoords() { x = 0; y = 0; z = 0; };
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
		Number distanceTo(const CubeCoords<Number> & second) const;
		std::vector<CubeCoords<Number>> createLine(const CubeCoords<Number> & to);
		std::vector<CubeCoords<Number>>	fromCenterInRange(const int & range);
		friend CubeCoords<Number> cubeLinearInterpolation(
			CubeCoords<Number> & start, CubeCoords<Number> & finish, double t)
		{
			try {
				return CubeCoords<Number>
					(
						start.x + (finish.x - start.x) * t,
						start.y + (finish.y - start.y) * t,
						start.z + (finish.z - start.z) * t
						);
			}
			catch (bad_coordinates_exception & exc)
			{
				CubeCoords<Number> temp;
				temp.roundAndConstruct(
					start.x + (finish.x - start.x) * t,
					start.y + (finish.y - start.y) * t,
					start.z + (finish.z - start.z) * t);
				return temp;
			}
		};
		CubeCoords<Number> rounded();
		void roundAndConstruct(Number a, Number b, Number c);
		void roundThis();
		CubeCoords<Number> neighbor(const CubeDirections & dir) const;
		CubeCoords<Number> diagonal(const CubeDirections & dir) const;
		std::pair<int, int> toPixel(const int & size, const bool & pointy);
		void fromPixel(const int & pixel_x, const int & pixel_y,
			const int & size, const int & coords_offset_x, const int & coords_offset_y, const bool pointy);
		std::string toStr() const;
		long long int hash() const;
		Number getX() const { return x; }
		Number getY() const { return y; }
		Number getZ() const { return z; }
	};
	typedef CubeCoords<int> ICube;
	typedef CubeCoords<double> FloatCube;



	template<class Number>
	CubeCoords<Number>::CubeCoords(const Number & cx, const Number & cz, const Number & cy)
		: x(cx), y(cy), z(cz)
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
		return CubeCoords<Number>(second.x + x, second.z + z, second.y + y);
	}
	template<class Number>
	CubeCoords<Number> CubeCoords<Number>::operator-(const CubeCoords<Number> & second) const
	{
		return CubeCoords<Number>(second.x - x, second.z - z, second.y - y);
	};
	template<class Number>
	bool CubeCoords<Number>::operator==(const CubeCoords<Number> & second) const
	{
		return (second.x == x && second.y == y && second.z == z);
	};
	template<class Number>
	bool CubeCoords<Number>::operator<(const CubeCoords<Number> & second) const
	{
		return hash() < second.hash();
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
		return CubeCoords<Number>(-x, -z, -y);
	};
	template <class Number>
	CubeCoords<Number> & CubeCoords<Number>::operator=(const CubeCoords<Number> & second)
	{
		x = second.x; y = second.y; z = second.z;
		return *this;
	};
	template<class Number>
	Number CubeCoords<Number>::distanceTo(const CubeCoords<Number> & second) const
	{
		return (abs(x - second.x) + abs(y - second.y) + abs(z - second.z)) / 2;
	};
	template<class Number>
	std::vector<CubeCoords<Number>> CubeCoords<Number>::createLine(const CubeCoords<Number> & to)
	{
		std::vector<CubeCoords<int>> temp;
		int dist = distanceTo(to);
		temp.reserve(dist);
		FloatCube start = FloatCube(*this) + FloatCube(0.0000001, 0.000001, 0.000001);
		FloatCube finish = FloatCube(to) + FloatCube(0.0000001, 0.000001, 0.000001);
		int end = dist + 1;
		for (int i = 0; i < end; ++i)
		{
			temp.push_back((CubeCoords<Number>)(cubeLinearInterpolation(start, finish, (1 / (float)dist * i))).rounded());
		}
		return temp;
	};
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
	std::string CubeCoords<Number>::toStr() const
	{
		std::ostringstream sout;
		sout << ' ' << x << ' ' << z << ' ' << y << ' ';
		return sout.str();
	};
	template <class Number>
	void CubeCoords<Number>::roundThis()

	{
		Number rx = std::roundf(x);
		Number ry = std::roundf(y);
		Number rz = std::roundf(z);
		Number x_diff = abs(rx - x);
		Number y_diff = abs(ry - y);
		Number z_diff = abs(rz - z);
		if (x_diff > y_diff && y_diff > z_diff)
		{
			rx = -ry - rz;
		}
		else if (y_diff > z_diff)
		{
			ry = -rx - rz;
		}
		else
		{
			rz = -rx - ry;
		}
		x = rx; y = ry; z = rz;
	}
	template<class Number>
	void CubeCoords<Number>::roundAndConstruct(Number a, Number b, Number c)
	{
		Number rx = std::roundf(a);
		Number ry = std::roundf(b);
		Number rz = std::roundf(c);
		Number x_diff = abs(rx - a);
		Number y_diff = abs(ry - b);
		Number z_diff = abs(rz - c);
		if (x_diff > y_diff && y_diff > z_diff)
		{
			rx = -ry - rz;
		}
		else if (y_diff > z_diff)
		{
			ry = -rx - rz;
		}
		else
		{
			rz = -rx - ry;
		}
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
	CubeCoords<Number> CubeCoords<Number>::neighbor(const CubeDirections & dir) const
	{
		return *this + CubeCoords<Number>(directions[dir][0], directions[dir][1], directions[dir][2]);
	}
	template<class Number>
	CubeCoords<Number> CubeCoords<Number>::diagonal(const CubeDirections & dir) const
	{
		return *this + CubeCoords<Number>(diagonals[dir][0], diagonals[dir][1], diagonals[dir][2]);
	}
	template<class Number>
	long long int CubeCoords<Number>::hash() const
	{
		long long int hashed = z * hash_multiplier_z;
		hashed += y * hash_multiplier_y;
		hashed += x * hash_multiplier_x;
		return hashed;
	}
	static void testICube()
	{
		ICube fcube(3, 1, -4);
		ICube scube(1, 0, -1);
		debug << "First test: summ " << (fcube + scube).toStr()
			<< "\nSecond test: minus " << (fcube - scube).toStr()
			<< "\nThird test: distance " << fcube.distanceTo(scube)
			<< "\nForth test: create line -> begins at " << fcube.toStr();
		for each(auto a in fcube.createLine(scube))
		{
			debug << "\n" << a.toStr();
		}
		debug << "\nends at-> " << scube.toStr();
		debug << "\nTest 5: make range from -> " << scube.toStr() << " at 2\n";
		for  each (auto a in scube.fromCenterInRange(2))
		{
			debug << a.toStr() << "\n";
		}
		debug << "\nTest 6: make diagonals from-> " << scube.toStr() << "\n";
		for (int i = 0; i <= Lefttop; ++i)
		{
			debug << scube.diagonal((CubeDirections)i).toStr() << "\n";
		}
	}
	template <class Number>
	std::pair<int, int> CubeCoords<Number>::toPixel(const int & size,const bool & pointy)
	{
		int axial_x = (int)x, axial_y = (int)z;
		double pixel_x = (pointy) ? (sqrt(3) * axial_x + sqrt(3) / 2 * axial_y) * size
			: (3.0/2.0 * axial_x) * size;
		double pixel_y = (pointy) ? (3.0 / 2.0 * axial_y) * size
			: (sqrt(3) / 2 * axial_x + sqrt(3) * axial_y) * size;
		return std::pair<int, int>({ pixel_x, pixel_y });
	}
	template <class Number>
	void CubeCoords<Number>::fromPixel(const int & pxl_x, const int & pxl_y,
		const int & size, const int & coords_offset_x, const int & coords_offset_y, const bool pointy)
	{
		double pixel_x = pxl_x - coords_offset_x;
		double pixel_y = pxl_y - coords_offset_y;
		if (pointy)
		{
			double axial_q = ((sqrt(3) / 3 * pixel_x) - (1.0 / 3.0 * pixel_y)) /(float) size;
			double axial_r = (2.0 / 3.0 * pixel_y) / (float)size;
			FloatCube tempcube(axial_q, axial_r, -axial_q - axial_r);
			tempcube.roundThis();
			x = tempcube.getX(); y = tempcube.getY(); z = tempcube.getZ();
		}
		else
		{
			double axial_q = (2.0 / 3.0 * pixel_x) / size;
			double axial_r = ((-1.0 / 3.0 * pixel_x) + (sqrt(3) / 3.0 * pixel_y)) / size;
			FloatCube tempcube(axial_q, axial_r, -axial_q - axial_r);
			tempcube.roundThis();
			x = tempcube.getX(); y = tempcube.getY(); z = tempcube.getZ();
		}
	}
}