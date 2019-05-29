/********************************************************************************
** Form generated from reading UI file 'dialogshowattradius.ui'
**
** Created: Thu May 23 08:57:48 2019
**      by: Qt User Interface Compiler version 4.8.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DIALOGSHOWATTRADIUS_H
#define UI_DIALOGSHOWATTRADIUS_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QHeaderView>
#include <QtGui/QPushButton>
#include <QtGui/QTextEdit>

QT_BEGIN_NAMESPACE

class Ui_DialogShowAttradius
{
public:
    QPushButton *pushButtonClose;
    QTextEdit *textEditAttradius;

    void setupUi(QDialog *DialogShowAttradius)
    {
        if (DialogShowAttradius->objectName().isEmpty())
            DialogShowAttradius->setObjectName(QString::fromUtf8("DialogShowAttradius"));
        DialogShowAttradius->resize(313, 472);
        pushButtonClose = new QPushButton(DialogShowAttradius);
        pushButtonClose->setObjectName(QString::fromUtf8("pushButtonClose"));
        pushButtonClose->setGeometry(QRect(190, 430, 114, 32));
        textEditAttradius = new QTextEdit(DialogShowAttradius);
        textEditAttradius->setObjectName(QString::fromUtf8("textEditAttradius"));
        textEditAttradius->setGeometry(QRect(10, 10, 291, 411));

        retranslateUi(DialogShowAttradius);

        QMetaObject::connectSlotsByName(DialogShowAttradius);
    } // setupUi

    void retranslateUi(QDialog *DialogShowAttradius)
    {
        DialogShowAttradius->setWindowTitle(QApplication::translate("DialogShowAttradius", "Dialog", 0, QApplication::UnicodeUTF8));
        pushButtonClose->setText(QApplication::translate("DialogShowAttradius", "close", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class DialogShowAttradius: public Ui_DialogShowAttradius {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DIALOGSHOWATTRADIUS_H
