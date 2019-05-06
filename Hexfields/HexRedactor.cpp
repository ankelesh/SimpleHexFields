#include "stdafx.h"
#include "HexRedactor.h"


HexRedactor::HexRedactor(std::string & fname, QSize widgetsz, hexagonDrawOptions lo, QWidget * parent)
	:	mainLayout(new QHBoxLayout(this)), rightPanel(new QVBoxLayout(this)),
	hexfield(new WeightedMapWidget(fname, 50, widgetsz,true, this)),Info(new QLabel(this)),
	weightSetter(new QSpinBox(this)),filename(new QLineEdit(this)),
	colors(new QCheckBox("Colors", this)),saveButton(new QPushButton("Save", this))
{
	this->setLayout(mainLayout);
	mainLayout->addWidget(hexfield);
	mainLayout->addLayout(rightPanel);
	rightPanel->addWidget(Info);
	rightPanel->addWidget(weightSetter);
	rightPanel->addWidget(colors);
	colors->setChecked(true);
	rightPanel->addWidget(saveButton);
	rightPanel->addWidget(filename);
	rightPanel->addStretch(1);
	hexfield->setSizePolicy(QSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding));
	auto min = QSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
	Info->setSizePolicy(min);
	saveButton->setSizePolicy(min);
	filename->setSizePolicy(min);
	filename->setText(QString::fromStdString(fname));
	Info->setText("Right click to add or delete hex, left click to change weight");
	weightSetter->setMinimum(1);
	weightSetter->setMaximum(100);
	hexfield->setTestWidgetView();
	for (int i = 0; i < colorranges::intervalQuantity; ++i)
	{
		hexfield->addColorInterval(*(colorranges::normalLayout[i]));
	}
	hexfield->setupColors();
	QObject::connect(hexfield, &WeightedMapWidget::clicked, this, &HexRedactor::gotClick);
	QObject::connect(saveButton, &QPushButton::clicked, this, &HexRedactor::savetime);
	QObject::connect(colors, &QCheckBox::stateChanged, this, &HexRedactor::colorsstate);
}
void HexRedactor::savetime()
{
	QString name = filename->text();
	if (name.endsWith(".txt"))
	{
		std::ofstream fout(name.toStdString());
		fout << hexfield->getMap().serialize();
		fout.close();
	}
}
void HexRedactor::gotClick(bool left, int x, int y)
{
	QPoint offset = hexfield->getOffset();
	ICube temp;
	temp.fromPixel(x, y, 50, offset.x(), offset.y(), true);
	if (left)
	{
		if (hexfield->getMap().contains(temp))
		{
			hexfield->getMap().setWeightForOne(temp, weightSetter->value());
			hexfield->getMap()[temp].set_text(
				QString::fromStdString(temp.toStr()) + QString("w=") + QString::number(weightSetter->value()));
			if (colors->isChecked()) {
				hexfield->setupColorForOne(temp);
			}
		}
	}
	else
	{
		if (hexfield->getMap().contains(temp))
		{
			hexfield->getMap().remove(temp);
		}
		else
		{
			hexfield->getMap().insert(temp, weightSetter->value());
			hexfield->getMap()[temp].set_text(
				QString::fromStdString(temp.toStr()) + QString("w=") + QString::number(weightSetter->value()));
			if (colors->isChecked()) {
				hexfield->setupColorForOne(temp);
			}
		}
	}
	hexfield->update();
}
void HexRedactor::colorsstate(bool ch)
{
	hexfield->setColors(ch);
	hexfield->dropField();
	hexfield->update();
}
