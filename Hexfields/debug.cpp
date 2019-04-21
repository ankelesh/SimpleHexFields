#include "stdafx.h"
#include "debug.h"



	Debug::Debug()
	{
		fout.open("log.txt");
	}
	Debug::Debug(const char * filename)
	{
		fout.open(filename);
	}

	void Debug::in(const char * st)
	{
		fout << st;
		fout.flush();
	}
	void Debug::in(const char ch, const char bord = ' ')
	{
		fout << bord << ch << bord;
		fout.flush();
	}
	void Debug::in(const std::string & s)
	{
		fout << s.c_str() << ' ';
		fout.flush();
	}
	Debug & Debug::operator<<(QString & st)
	{
		this->in(st.toStdString());
		return *this;
	}
	Debug & Debug::operator<<(const char * chs)
	{
		fout << chs ;
		fout.flush();
		return *this;
	}
	Debug & Debug::operator<<(const int & num)
	{
		fout << ' ' << num << ' ';
		fout.flush();
		return *this;
	}
	Debug & Debug::operator<<(const bool & val)
	{
		fout << ' ' << ((val) ? "true" : "false") << ' ';
		fout.flush();
		return *this;
	}
	Debug & Debug::operator<<(const double & val)
	{
		fout << ' ' << val << ' ';
		fout.flush();
		return *this;
	}
	Debug::~Debug()
	{
		fout.close();
	}
