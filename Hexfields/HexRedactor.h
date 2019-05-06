#pragma once
#include <HexfieldWidget.h>


class HexRedactor	: public QWidget	
{
	Q_OBJECT
private:
	QHBoxLayout * mainLayout;
	QVBoxLayout * rightPanel;
	WeightedMapWidget * hexfield;
	QLabel * Info;
	QSpinBox * weightSetter;
	QLineEdit * filename;
	QCheckBox * colors;
	QPushButton * saveButton;
public:
	HexRedactor(std::string & fname, QSize widgetsz, hexagonDrawOptions lo, QWidget * parent = Q_NULLPTR);
public slots:
	void savetime();
	void gotClick(bool left, int x, int y);
	void colorsstate(bool ch);
};

