#pragma once
#include "stdafx.h"
#include <fstream>
#include <qstring.h>

	class Debug
	{
	private:
		std::ofstream fout;
	public:
		Debug();
		Debug(const char *);
		void in(const char *);
		void in(const char, const char);
		void in(const std::string &);
		Debug & operator<<(std::string & st) { this->in(st); return *this; };
		Debug & operator<<(QString & st);
		Debug & operator<<(const char * chs);
		Debug & operator<<(const int &);
		Debug & operator<<(const bool &);
		Debug & operator<<(const double &);
		~Debug();
	};
	static Debug debug;


