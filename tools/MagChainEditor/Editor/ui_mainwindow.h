/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created: Fri Jul 19 14:59:10 2013
**      by: Qt User Interface Compiler version 4.8.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCheckBox>
#include <QtGui/QFrame>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QMainWindow>
#include <QtGui/QMenuBar>
#include <QtGui/QPushButton>
#include <QtGui/QRadioButton>
#include <QtGui/QStatusBar>
#include <QtGui/QToolBar>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralWidget;
    QPushButton *pushButtonCancel;
    QRadioButton *radioButtonEquil;
    QLineEdit *lineEditTimestep;
    QLabel *labelRestart;
    QLineEdit *lineEditRestart;
    QLineEdit *lineEditRun;
    QLineEdit *lineEditYBox;
    QLineEdit *lineEditHistogram;
    QLabel *labelHistogram;
    QLabel *labelTrajectory;
    QPushButton *pushButtonOk;
    QLineEdit *lineEditStats;
    QLineEdit *lineEditXBox;
    QLabel *labelStats;
    QPushButton *pushButtonImportInput;
    QLineEdit *lineEditHistogramSize;
    QCheckBox *checkBoxRestartFile;
    QLabel *labelTimestep;
    QRadioButton *radioButtonProd;
    QLabel *labelRun;
    QLineEdit *lineEditZBox;
    QLineEdit *lineEditTrajectory;
    QLabel *labelRestartFileWarning;
    QLabel *labelAttradiusFileWarning;
    QRadioButton *radioButtonFromRestart;
    QRadioButton *radioButtonFromExternal;
    QCheckBox *checkBoxAttradiusFile;
    QPushButton *pushButtonClearAll;
    QLabel *labelXBox;
    QLabel *labelYBox;
    QLabel *label;
    QLabel *labelHistogramSize;
    QFrame *line;
    QLineEdit *lineEditParticles;
    QLabel *labelParticles;
    QPushButton *pushButtonShowValues;
    QLineEdit *lineEditAttradiusRestartSize;
    QLineEdit *lineEditAttradiusExternalSize;
    QRadioButton *radioButtonExportInfile;
    QRadioButton *radioButtonAll;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;
    QButtonGroup *buttonGroupAttradius;
    QButtonGroup *buttonGroupExport;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->setEnabled(true);
        //MainWindow->resize(627, 507); 
	MainWindow->resize(700, 507); //Changed by Alex Giménez
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(10);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(MainWindow->sizePolicy().hasHeightForWidth());
        MainWindow->setSizePolicy(sizePolicy);
        //MainWindow->setMinimumSize(QSize(627, 507));
        //MainWindow->setMaximumSize(QSize(627, 507));
        MainWindow->setAnimated(true);
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        pushButtonCancel = new QPushButton(centralWidget);
        pushButtonCancel->setObjectName(QString::fromUtf8("pushButtonCancel"));
        pushButtonCancel->setGeometry(QRect(390, 420, 114, 32));
        radioButtonEquil = new QRadioButton(centralWidget);
        radioButtonEquil->setObjectName(QString::fromUtf8("radioButtonEquil"));
        radioButtonEquil->setGeometry(QRect(55, 200, 106, 20));
        radioButtonEquil->setChecked(false);
        lineEditTimestep = new QLineEdit(centralWidget);
        lineEditTimestep->setObjectName(QString::fromUtf8("lineEditTimestep"));
        lineEditTimestep->setGeometry(QRect(160, 130, 101, 21));
        labelRestart = new QLabel(centralWidget);
        labelRestart->setObjectName(QString::fromUtf8("labelRestart"));
        labelRestart->setGeometry(QRect(389, 70, 101, 16));
        labelRestart->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        lineEditRestart = new QLineEdit(centralWidget);
        lineEditRestart->setObjectName(QString::fromUtf8("lineEditRestart"));
        lineEditRestart->setGeometry(QRect(509, 70, 91, 21));
        lineEditRun = new QLineEdit(centralWidget);
        lineEditRun->setObjectName(QString::fromUtf8("lineEditRun"));
        lineEditRun->setGeometry(QRect(159, 160, 101, 21));
        lineEditYBox = new QLineEdit(centralWidget);
        lineEditYBox->setObjectName(QString::fromUtf8("lineEditYBox"));
        lineEditYBox->setGeometry(QRect(160, 70, 101, 21));
        lineEditHistogram = new QLineEdit(centralWidget);
        lineEditHistogram->setObjectName(QString::fromUtf8("lineEditHistogram"));
        lineEditHistogram->setGeometry(QRect(509, 100, 91, 21));
        labelHistogram = new QLabel(centralWidget);
        labelHistogram->setObjectName(QString::fromUtf8("labelHistogram"));
        labelHistogram->setGeometry(QRect(369, 100, 121, 16));
        labelHistogram->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        labelTrajectory = new QLabel(centralWidget);
        labelTrajectory->setObjectName(QString::fromUtf8("labelTrajectory"));
        labelTrajectory->setGeometry(QRect(369, 40, 121, 16));
        labelTrajectory->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        pushButtonOk = new QPushButton(centralWidget);
        pushButtonOk->setObjectName(QString::fromUtf8("pushButtonOk"));
        pushButtonOk->setGeometry(QRect(500, 420, 114, 32));
        lineEditStats = new QLineEdit(centralWidget);
        lineEditStats->setObjectName(QString::fromUtf8("lineEditStats"));
        lineEditStats->setGeometry(QRect(509, 10, 91, 21));
        lineEditXBox = new QLineEdit(centralWidget);
        lineEditXBox->setObjectName(QString::fromUtf8("lineEditXBox"));
        lineEditXBox->setGeometry(QRect(160, 40, 101, 21));
        labelStats = new QLabel(centralWidget);
        labelStats->setObjectName(QString::fromUtf8("labelStats"));
        labelStats->setGeometry(QRect(359, 10, 131, 16));
        labelStats->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        pushButtonImportInput = new QPushButton(centralWidget);
        pushButtonImportInput->setObjectName(QString::fromUtf8("pushButtonImportInput"));
        pushButtonImportInput->setGeometry(QRect(460, 170, 151, 32));
        lineEditHistogramSize = new QLineEdit(centralWidget);
        lineEditHistogramSize->setObjectName(QString::fromUtf8("lineEditHistogramSize"));
        lineEditHistogramSize->setGeometry(QRect(509, 130, 91, 21));
        checkBoxRestartFile = new QCheckBox(centralWidget);
        checkBoxRestartFile->setObjectName(QString::fromUtf8("checkBoxRestartFile"));
        checkBoxRestartFile->setGeometry(QRect(50, 240, 241, 20));
        labelTimestep = new QLabel(centralWidget);
        labelTimestep->setObjectName(QString::fromUtf8("labelTimestep"));
        labelTimestep->setGeometry(QRect(70, 130, 81, 20));
        labelTimestep->setLayoutDirection(Qt::LeftToRight);
        labelTimestep->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        radioButtonProd = new QRadioButton(centralWidget);
        radioButtonProd->setObjectName(QString::fromUtf8("radioButtonProd"));
        radioButtonProd->setGeometry(QRect(180, 200, 102, 20));
        labelRun = new QLabel(centralWidget);
        labelRun->setObjectName(QString::fromUtf8("labelRun"));
        labelRun->setGeometry(QRect(29, 160, 123, 16));
        labelRun->setLayoutDirection(Qt::LeftToRight);
        labelRun->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        lineEditZBox = new QLineEdit(centralWidget);
        lineEditZBox->setObjectName(QString::fromUtf8("lineEditZBox"));
        lineEditZBox->setGeometry(QRect(160, 100, 101, 21));
        lineEditTrajectory = new QLineEdit(centralWidget);
        lineEditTrajectory->setObjectName(QString::fromUtf8("lineEditTrajectory"));
        lineEditTrajectory->setGeometry(QRect(509, 40, 91, 21));
        labelRestartFileWarning = new QLabel(centralWidget);
        labelRestartFileWarning->setObjectName(QString::fromUtf8("labelRestartFileWarning"));
        labelRestartFileWarning->setGeometry(QRect(50, 260, 561, 16));
        labelRestartFileWarning->setMinimumSize(QSize(250, 0));
        QFont font;
        font.setPointSize(9);
        labelRestartFileWarning->setFont(font);
        labelAttradiusFileWarning = new QLabel(centralWidget);
        labelAttradiusFileWarning->setObjectName(QString::fromUtf8("labelAttradiusFileWarning"));
        labelAttradiusFileWarning->setEnabled(false);
        labelAttradiusFileWarning->setGeometry(QRect(50, 310, 561, 16));
        labelAttradiusFileWarning->setMinimumSize(QSize(250, 0));
        labelAttradiusFileWarning->setFont(font);
        radioButtonFromRestart = new QRadioButton(centralWidget);
        buttonGroupAttradius = new QButtonGroup(MainWindow);
        buttonGroupAttradius->setObjectName(QString::fromUtf8("buttonGroupAttradius"));
        buttonGroupAttradius->addButton(radioButtonFromRestart);
        radioButtonFromRestart->setObjectName(QString::fromUtf8("radioButtonFromRestart"));
        radioButtonFromRestart->setEnabled(false);
        radioButtonFromRestart->setGeometry(QRect(30, 350, 261, 20));
        radioButtonFromExternal = new QRadioButton(centralWidget);
        buttonGroupAttradius->addButton(radioButtonFromExternal);
        radioButtonFromExternal->setObjectName(QString::fromUtf8("radioButtonFromExternal"));
        radioButtonFromExternal->setEnabled(false);
        radioButtonFromExternal->setGeometry(QRect(30, 380, 271, 20));
        radioButtonFromExternal->setChecked(true);
        checkBoxAttradiusFile = new QCheckBox(centralWidget);
        checkBoxAttradiusFile->setObjectName(QString::fromUtf8("checkBoxAttradiusFile"));
        checkBoxAttradiusFile->setEnabled(false);
        checkBoxAttradiusFile->setGeometry(QRect(50, 290, 231, 20));
        pushButtonClearAll = new QPushButton(centralWidget);
        pushButtonClearAll->setObjectName(QString::fromUtf8("pushButtonClearAll"));
        pushButtonClearAll->setGeometry(QRect(350, 170, 114, 32));
        labelXBox = new QLabel(centralWidget);
        labelXBox->setObjectName(QString::fromUtf8("labelXBox"));
        labelXBox->setGeometry(QRect(90, 40, 61, 20));
        labelXBox->setLayoutDirection(Qt::LeftToRight);
        labelXBox->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        labelYBox = new QLabel(centralWidget);
        labelYBox->setObjectName(QString::fromUtf8("labelYBox"));
        labelYBox->setGeometry(QRect(90, 70, 61, 20));
        labelYBox->setLayoutDirection(Qt::LeftToRight);
        labelYBox->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        label = new QLabel(centralWidget);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(100, 100, 51, 20));
        label->setLayoutDirection(Qt::LeftToRight);
        label->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        labelHistogramSize = new QLabel(centralWidget);
        labelHistogramSize->setObjectName(QString::fromUtf8("labelHistogramSize"));
        labelHistogramSize->setGeometry(QRect(350, 130, 141, 20));
        labelHistogramSize->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        line = new QFrame(centralWidget);
        line->setObjectName(QString::fromUtf8("line"));
        line->setGeometry(QRect(310, 10, 20, 171));
        line->setFrameShape(QFrame::VLine);
        line->setFrameShadow(QFrame::Sunken);
        lineEditParticles = new QLineEdit(centralWidget);
        lineEditParticles->setObjectName(QString::fromUtf8("lineEditParticles"));
        lineEditParticles->setGeometry(QRect(160, 10, 101, 21));
        labelParticles = new QLabel(centralWidget);
        labelParticles->setObjectName(QString::fromUtf8("labelParticles"));
        labelParticles->setGeometry(QRect(17, 10, 134, 21));
        labelParticles->setLayoutDirection(Qt::LeftToRight);
        labelParticles->setScaledContents(false);
        labelParticles->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        pushButtonShowValues = new QPushButton(centralWidget);
        pushButtonShowValues->setObjectName(QString::fromUtf8("pushButtonShowValues"));
        pushButtonShowValues->setEnabled(false);
        pushButtonShowValues->setGeometry(QRect(70, 410, 119, 32));
        lineEditAttradiusRestartSize = new QLineEdit(centralWidget);
        lineEditAttradiusRestartSize->setObjectName(QString::fromUtf8("lineEditAttradiusRestartSize"));
        lineEditAttradiusRestartSize->setEnabled(false);
        lineEditAttradiusRestartSize->setGeometry(QRect(300, 350, 61, 21));
        lineEditAttradiusExternalSize = new QLineEdit(centralWidget);
        lineEditAttradiusExternalSize->setObjectName(QString::fromUtf8("lineEditAttradiusExternalSize"));
        lineEditAttradiusExternalSize->setEnabled(false);
        lineEditAttradiusExternalSize->setGeometry(QRect(300, 380, 61, 21));
        radioButtonExportInfile = new QRadioButton(centralWidget);
        buttonGroupExport = new QButtonGroup(MainWindow);
        buttonGroupExport->setObjectName(QString::fromUtf8("buttonGroupExport"));
        buttonGroupExport->addButton(radioButtonExportInfile);
        radioButtonExportInfile->setObjectName(QString::fromUtf8("radioButtonExportInfile"));
        radioButtonExportInfile->setGeometry(QRect(430, 350, 171, 20));
        radioButtonExportInfile->setChecked(true);
        radioButtonAll = new QRadioButton(centralWidget);
        buttonGroupExport->addButton(radioButtonAll);
        radioButtonAll->setObjectName(QString::fromUtf8("radioButtonAll"));
        radioButtonAll->setGeometry(QRect(430, 380, 141, 20));
        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 627, 22));
        MainWindow->setMenuBar(menuBar);
        mainToolBar = new QToolBar(MainWindow);
        mainToolBar->setObjectName(QString::fromUtf8("mainToolBar"));
        MainWindow->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        MainWindow->setStatusBar(statusBar);
        QWidget::setTabOrder(lineEditParticles, lineEditXBox);
        QWidget::setTabOrder(lineEditXBox, lineEditYBox);
        QWidget::setTabOrder(lineEditYBox, lineEditZBox);
        QWidget::setTabOrder(lineEditZBox, lineEditTimestep);
        QWidget::setTabOrder(lineEditTimestep, lineEditRun);
        QWidget::setTabOrder(lineEditRun, lineEditStats);
        QWidget::setTabOrder(lineEditStats, lineEditTrajectory);
        QWidget::setTabOrder(lineEditTrajectory, lineEditRestart);
        QWidget::setTabOrder(lineEditRestart, lineEditHistogram);
        QWidget::setTabOrder(lineEditHistogram, lineEditHistogramSize);
        QWidget::setTabOrder(lineEditHistogramSize, radioButtonEquil);
        QWidget::setTabOrder(radioButtonEquil, radioButtonProd);
        QWidget::setTabOrder(radioButtonProd, pushButtonClearAll);
        QWidget::setTabOrder(pushButtonClearAll, pushButtonImportInput);
        QWidget::setTabOrder(pushButtonImportInput, checkBoxRestartFile);
        QWidget::setTabOrder(checkBoxRestartFile, checkBoxAttradiusFile);
        QWidget::setTabOrder(checkBoxAttradiusFile, radioButtonFromRestart);
        QWidget::setTabOrder(radioButtonFromRestart, radioButtonFromExternal);
        QWidget::setTabOrder(radioButtonFromExternal, lineEditAttradiusRestartSize);
        QWidget::setTabOrder(lineEditAttradiusRestartSize, pushButtonShowValues);
        QWidget::setTabOrder(pushButtonShowValues, pushButtonCancel);
        QWidget::setTabOrder(pushButtonCancel, pushButtonOk);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        //MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MagChain Input Script Editor ver 0.1", 0, QApplication::UnicodeUTF8));
	MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MagChain Input Script Editor ver 0.2", 0, QApplication::UnicodeUTF8)); //Changes by Àlex Giménez (only design changes)
        pushButtonCancel->setText(QApplication::translate("MainWindow", "Cancel", 0, QApplication::UnicodeUTF8));
        radioButtonEquil->setText(QApplication::translate("MainWindow", "Equilibration", 0, QApplication::UnicodeUTF8));
        labelRestart->setText(QApplication::translate("MainWindow", "Restart every", 0, QApplication::UnicodeUTF8));
        labelHistogram->setText(QApplication::translate("MainWindow", "Histogram every", 0, QApplication::UnicodeUTF8));
        labelTrajectory->setText(QApplication::translate("MainWindow", "Trajectory every", 0, QApplication::UnicodeUTF8));
        pushButtonOk->setText(QApplication::translate("MainWindow", "Ok", 0, QApplication::UnicodeUTF8));
        labelStats->setText(QApplication::translate("MainWindow", "Statistics every", 0, QApplication::UnicodeUTF8));
        pushButtonImportInput->setText(QApplication::translate("MainWindow", "Import Input File", 0, QApplication::UnicodeUTF8));
        checkBoxRestartFile->setText(QApplication::translate("MainWindow", "Restart simulation from restart file", 0, QApplication::UnicodeUTF8));
        labelTimestep->setText(QApplication::translate("MainWindow", "Timestep *", 0, QApplication::UnicodeUTF8));
        radioButtonProd->setText(QApplication::translate("MainWindow", "Production", 0, QApplication::UnicodeUTF8));
        labelRun->setText(QApplication::translate("MainWindow", "N steps *", 0, QApplication::UnicodeUTF8));
        labelRestartFileWarning->setText(QApplication::translate("MainWindow", "Empty", 0, QApplication::UnicodeUTF8));
        labelAttradiusFileWarning->setText(QApplication::translate("MainWindow", "Empty", 0, QApplication::UnicodeUTF8));
        radioButtonFromRestart->setText(QApplication::translate("MainWindow", "Attraction radius from restart file", 0, QApplication::UnicodeUTF8));
        radioButtonFromExternal->setText(QApplication::translate("MainWindow", "Attraction radius from external file", 0, QApplication::UnicodeUTF8));
        checkBoxAttradiusFile->setText(QApplication::translate("MainWindow", "Load external attradius file", 0, QApplication::UnicodeUTF8));
        pushButtonClearAll->setText(QApplication::translate("MainWindow", "Clear All", 0, QApplication::UnicodeUTF8));
        labelXBox->setText(QApplication::translate("MainWindow", "X box *", 0, QApplication::UnicodeUTF8));
        labelYBox->setText(QApplication::translate("MainWindow", "Y box *", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("MainWindow", "Z box *", 0, QApplication::UnicodeUTF8));
        labelHistogramSize->setText(QApplication::translate("MainWindow", "Histogram bin size", 0, QApplication::UnicodeUTF8));
        labelParticles->setText(QApplication::translate("MainWindow", "N particles *", 0, QApplication::UnicodeUTF8));
        pushButtonShowValues->setText(QApplication::translate("MainWindow", "Show values", 0, QApplication::UnicodeUTF8));
        radioButtonExportInfile->setText(QApplication::translate("MainWindow", "Export input script", 0, QApplication::UnicodeUTF8));
        radioButtonAll->setText(QApplication::translate("MainWindow", "Export All Project", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
