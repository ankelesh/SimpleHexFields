#pragma once

#include <QtWidgets/QWidget>
#include "ui_Hexfields.h"

class Hexfields : public QWidget
{
	Q_OBJECT

public:
	Hexfields(QWidget *parent = Q_NULLPTR);

private:
	Ui::HexfieldsClass ui;
};
