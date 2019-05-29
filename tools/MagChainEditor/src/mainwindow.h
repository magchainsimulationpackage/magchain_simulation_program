#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "dialogshowattradius.h"


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:

    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void readInputFile();

    bool readAttradiusFile();

    bool readRestartFile();

    int form_integrity();

    void searchToken(QStringList stl);

private slots:

    void on_pushButtonOk_clicked();

    void on_pushButtonCancel_clicked();


    void on_pushButtonImportInput_clicked();

    void on_checkBoxRestartFile_toggled(bool checked);

    void on_radioButtonFromExternal_toggled(bool checked);

    void on_pushButtonShowValues_clicked();

    void on_radioButtonProd_toggled(bool checked);

    void on_checkBoxAttradiusFile_toggled(bool checked);

    void on_pushButtonClearAll_clicked();

signals:

    void sendValues(QString *str);

private:
    Ui::MainWindow *ui;
    DialogShowAttradius *attradiusDialog;

    QString attr_values_from_restart;
    QString attr_values_from_external;
    QString *attr_values;

    QString attradius_file;
    QString restart_file;

};

#endif // MAINWINDOW_H
