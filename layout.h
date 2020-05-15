/********************************************************************************
** Form generated from reading UI file 'LaunchBoxRIsfOs.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef LAUNCHBOXRISFOS_H
#define LAUNCHBOXRISFOS_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QPushButton>

QT_BEGIN_NAMESPACE

class Ui_Aplication
{
public:
    QPushButton *pushButton;
    QPushButton *pushButton_2;
    QPushButton *pushButton_3;

    void setupUi(QDialog *Aplication)
    {
        if (Aplication->objectName().isEmpty())
            Aplication->setObjectName(QString::fromUtf8("Aplication"));
        Aplication->resize(1038, 596);
        Aplication->setStyleSheet(QString::fromUtf8("QWidget#Aplication {\n"
"background-color: rgb(50,50,50);\n"
"}\n"
"\n"
"\n"
"  \n"
"QPushButton {\n"
"background-color: rgb(0,0,0,0);\n"
"color:  rgb(150,150,150);\n"
"}\n"
"QPushButton:hover {\n"
"color:  rgb(230,230,230);\n"
"}"));
        pushButton = new QPushButton(Aplication);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));
        pushButton->setGeometry(QRect(340, 40, 186, 41));
        QFont font;
        font.setFamily(QString::fromUtf8("Lato"));
        font.setPointSize(16);
        font.setBold(true);
        font.setUnderline(false);
        font.setWeight(75);
        font.setStrikeOut(false);
        pushButton->setFont(font);
        pushButton->setStyleSheet(QString::fromUtf8(""));
        pushButton_2 = new QPushButton(Aplication);
        pushButton_2->setObjectName(QString::fromUtf8("pushButton_2"));
        pushButton_2->setGeometry(QRect(40, 480, 186, 41));
        pushButton_2->setFont(font);
        pushButton_2->setStyleSheet(QString::fromUtf8(""));
        pushButton_3 = new QPushButton(Aplication);
        pushButton_3->setObjectName(QString::fromUtf8("pushButton_3"));
        pushButton_3->setGeometry(QRect(40, 40, 186, 41));
        pushButton_3->setFont(font);
        pushButton_3->setStyleSheet(QString::fromUtf8(""));

        retranslateUi(Aplication);

        QMetaObject::connectSlotsByName(Aplication);
    } // setupUi

    void retranslateUi(QDialog *Aplication)
    {
        Aplication->setWindowTitle(QCoreApplication::translate("Aplication", "Dialog", nullptr));
        pushButton->setText(QCoreApplication::translate("Aplication", "COs", nullptr));
        pushButton_2->setText(QCoreApplication::translate("Aplication", "Graj", nullptr));
        pushButton_3->setText(QCoreApplication::translate("Aplication", "Zaloguj", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Aplication: public Ui_Aplication {};
} // namespace Ui

QT_END_NAMESPACE

#endif // LAUNCHBOXRISFOS_H
