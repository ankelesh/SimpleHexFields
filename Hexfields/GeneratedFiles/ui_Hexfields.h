/********************************************************************************
** Form generated from reading UI file 'Hexfields.ui'
**
** Created by: Qt User Interface Compiler version 5.11.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_HEXFIELDS_H
#define UI_HEXFIELDS_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_HexfieldsClass
{
public:

    void setupUi(QWidget *HexfieldsClass)
    {
        if (HexfieldsClass->objectName().isEmpty())
            HexfieldsClass->setObjectName(QStringLiteral("HexfieldsClass"));
        HexfieldsClass->resize(600, 400);

        retranslateUi(HexfieldsClass);

        QMetaObject::connectSlotsByName(HexfieldsClass);
    } // setupUi

    void retranslateUi(QWidget *HexfieldsClass)
    {
        HexfieldsClass->setWindowTitle(QApplication::translate("HexfieldsClass", "Hexfields", nullptr));
    } // retranslateUi

};

namespace Ui {
    class HexfieldsClass: public Ui_HexfieldsClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_HEXFIELDS_H
