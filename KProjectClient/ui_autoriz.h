/********************************************************************************
** Form generated from reading UI file 'autoriz.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_AUTORIZ_H
#define UI_AUTORIZ_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>

QT_BEGIN_NAMESPACE

class Ui_Autoriz
{
public:
    QLabel *label_2;
    QLineEdit *lineEdit;
    QLineEdit *lineEdit_2;
    QLabel *label;
    QPushButton *pushButton_2;
    QPushButton *pushButton;

    void setupUi(QDialog *Autoriz)
    {
        if (Autoriz->objectName().isEmpty())
            Autoriz->setObjectName(QString::fromUtf8("Autoriz"));
        Autoriz->resize(516, 309);
        Autoriz->setStyleSheet(QString::fromUtf8("\n"
"background-color:rgb(251, 237, 255);"));
        label_2 = new QLabel(Autoriz);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(10, 270, 441, 21));
        lineEdit = new QLineEdit(Autoriz);
        lineEdit->setObjectName(QString::fromUtf8("lineEdit"));
        lineEdit->setGeometry(QRect(60, 110, 181, 31));
        lineEdit_2 = new QLineEdit(Autoriz);
        lineEdit_2->setObjectName(QString::fromUtf8("lineEdit_2"));
        lineEdit_2->setGeometry(QRect(260, 110, 181, 31));
        label = new QLabel(Autoriz);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(60, 50, 141, 51));
        QFont font;
        font.setPointSize(13);
        label->setFont(font);
        pushButton_2 = new QPushButton(Autoriz);
        pushButton_2->setObjectName(QString::fromUtf8("pushButton_2"));
        pushButton_2->setGeometry(QRect(160, 210, 181, 31));
        pushButton = new QPushButton(Autoriz);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));
        pushButton->setGeometry(QRect(160, 170, 181, 31));
        QWidget::setTabOrder(lineEdit, lineEdit_2);
        QWidget::setTabOrder(lineEdit_2, pushButton);
        QWidget::setTabOrder(pushButton, pushButton_2);

        retranslateUi(Autoriz);

        QMetaObject::connectSlotsByName(Autoriz);
    } // setupUi

    void retranslateUi(QDialog *Autoriz)
    {
        Autoriz->setWindowTitle(QCoreApplication::translate("Autoriz", "Dialog", nullptr));
        label_2->setText(QString());
        lineEdit->setText(QString());
        lineEdit->setPlaceholderText(QCoreApplication::translate("Autoriz", "\320\222\320\262\320\265\320\264\320\270\321\202\320\265 \320\273\320\276\320\263\320\270\320\275", nullptr));
        lineEdit_2->setPlaceholderText(QCoreApplication::translate("Autoriz", "\320\222\320\262\320\265\320\264\320\270\321\202\320\265 \320\277\320\260\321\200\320\276\320\273\321\214", nullptr));
        label->setText(QCoreApplication::translate("Autoriz", "\320\220\320\262\321\202\320\276\321\200\320\270\320\267\320\260\321\206\320\270\321\217", nullptr));
        pushButton_2->setText(QCoreApplication::translate("Autoriz", "\320\222\321\213\320\271\321\202\320\270", nullptr));
        pushButton->setText(QCoreApplication::translate("Autoriz", "\320\222\320\276\320\271\321\202\320\270", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Autoriz: public Ui_Autoriz {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_AUTORIZ_H
