#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "dialogshowattradius.h"

#include <QString>
#include <QFile>
#include <QDebug>
#include <iostream>
#include <QTextStream>
#include <QMessageBox>
#include <QPushButton>
#include <QCloseEvent>
#include <QDir>
#include <QFileDialog>
#include <QButtonGroup>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->checkBoxRestartFile,SIGNAL(toggled(bool)),ui->labelRestartFileWarning ,SLOT(setDisabled(bool)));
    connect(ui->radioButtonFromExternal,SIGNAL(toggled(bool)),ui->labelAttradiusFileWarning ,SLOT(setDisabled(bool)));

    ui->labelRestartFileWarning->setText("<font color=red>empty<\font>");
    ui->labelAttradiusFileWarning->setText("<font color=red>empty<\font>");

}

MainWindow::~MainWindow()
{

    delete ui;

}


// public functions

void MainWindow::readInputFile()
{

    QString filename = QFileDialog::getOpenFileName(this, tr("Open file"), "", tr("Files (*.*)"));
    QFile file(filename);

    if(!file.exists()){

        QString error_msg = filename + " does not exist or operation cancelled by user.";
        qDebug() << error_msg;
        QMessageBox::critical(this,"Error loading input file",error_msg);

    }else{

        qDebug() << filename << " found! Loading...";

        QString line;
        QStringList fields;

        if (file.open(QIODevice::ReadOnly | QIODevice::Text)){

            QTextStream stream(&file);

            while (!stream.atEnd()){

                line = stream.readLine();
                fields = line.split(" ");
                searchToken(fields); // Pass fields to a check function

            }

            file.close();

        }

    }

}

bool MainWindow::readAttradiusFile()
{
    bool succeed = false;

    QString filename = QFileDialog::getOpenFileName(this, tr("Open file"), "", tr("Files (*.*)"));
    QFile file(filename);
    QFileInfo fi(file);

    if(!file.exists()){

        QString error_msg = filename + " does not exist or operation cancelled by user.";
        qDebug() << error_msg;
        QMessageBox::critical(this,"Error loading attraction radius file",error_msg);

    }else{

        qDebug() << filename << " found! Loading...";

        QString line;
        QStringList fields;

        if (file.open(QIODevice::ReadOnly | QIODevice::Text)){

            QTextStream stream(&file);

            attradius_file = filename; //global file name with path
            ui->labelAttradiusFileWarning->setText(fi.fileName());//without path
            attr_values_from_external = "";

            while (!stream.atEnd()){

                line = stream.readLine();
                attr_values_from_external+= line + "\n";
                fields += line.split(" ");

            }

            QString size = QString::number(fields.size()-1);
            ui->lineEditAttradiusExternalSize->setText(size);

            file.close();
            succeed = true;

        }

    }

    return succeed;

}

bool MainWindow::readRestartFile(){

    bool succeed = false;

    QString filename = QFileDialog::getOpenFileName(this, tr("Open file"), "", tr("Files (*.*)"));
    QFile file(filename);
    QFileInfo fi(file);

    if(!file.exists()){

        QString error_msg = filename + " does not exist or operation cancelled by user.";
        qDebug() << error_msg;
        QMessageBox::critical(this,"Error loading restart file",error_msg);

    }else{

        qDebug() << filename << " found! Loading...";

        QString line;
        QStringList fields;

        if (file.open(QIODevice::ReadOnly | QIODevice::Text)){

            QTextStream stream(&file);

            restart_file = filename;//global filename with path
            ui->labelRestartFileWarning ->setText(fi.fileName());//without path

            int count = 0;
            bool attr = false;

            attr_values_from_restart = "";

            int max=0;

            while (!stream.atEnd()){
                line = stream.readLine();
                fields = line.split(" ");
                count++;
                // Only certain lines contain relevant info: we use a counter

                // restarting from timestep
                if (count == 2){ }

                // particles/aggregates
                if (count == 5){

                    ui->lineEditParticles->setText(fields[0]);

                }

                // simulation box
                if (count == 8){

                    ui->lineEditXBox->setText(fields[0]);
                    ui->lineEditYBox->setText(fields[1]);
                    ui->lineEditZBox->setText(fields[2]);

                }

                // timestep
                if (count == 11){

                    ui->lineEditTimestep->setText(fields[0]);
                }

                // attradius
                if (count == 13 && fields[0]!="ATTRACTION RADIUS"){ attr=true; }
                if (count == 14 && attr){ max=fields[0].toDouble(); }
                if (count == 15 && attr){

                    for (int i=0; i<max; i++){

                        attr_values_from_restart += QString::number(i+1) + "\t" + fields[i] + "\n";

                    }
                }

                ui->lineEditAttradiusRestartSize->setText(QString::number(max));

            }
            // end oppening the file

            file.close();

            succeed = true;

        }
    }

    return succeed;

}

int MainWindow::form_integrity(){

    int mode = 0;

    bool mandatory = false;

    bool equilibration_new = false;
    bool equilibration_restart = false;
    bool production_new = false;
    bool production_restart = false;
    bool production_restart_with_external = false;

    //mandatory arguments:

    mandatory = (!ui->lineEditParticles->text().isEmpty() && \
                !ui->lineEditXBox->text().isEmpty() &&\
                !ui->lineEditYBox->text().isEmpty() &&\
                !ui->lineEditZBox->text().isEmpty() &&\
                !ui->lineEditTimestep->text().isEmpty() &&\
                !ui->lineEditRun->text().isEmpty());

    equilibration_new = (ui->radioButtonEquil->isChecked() &&\
                         !ui->checkBoxRestartFile->isChecked());

    equilibration_restart = (ui->radioButtonEquil->isChecked() &&\
                             ui->checkBoxRestartFile->isChecked());

    production_new = (ui->radioButtonProd->isChecked() &&\
                      !ui->checkBoxRestartFile->isChecked() &&\
                      !ui->lineEditAttradiusExternalSize->text().isEmpty());

    production_restart = (ui->radioButtonProd->isChecked() &&\
                          ui->checkBoxRestartFile->isChecked() &&\
                          !ui->lineEditAttradiusRestartSize->text().isEmpty());

    production_restart_with_external = (ui->radioButtonProd->isChecked() &&\
                          ui->checkBoxRestartFile->isChecked() &&\
                          !ui->lineEditAttradiusExternalSize->text().isEmpty());

    if (mandatory && equilibration_new) {mode = 1;}
    else if (mandatory && equilibration_restart) { mode = 2;}
    else if (mandatory && production_new) { mode = 3;}
    else if (mandatory && production_restart) { mode = 4;}
    else if (mandatory && production_restart_with_external) { mode = 5;}

    return mode;
}

void MainWindow::searchToken(QStringList stl){

    int size = stl.size();

    //qDebug() << stl << endl;

    if (stl[0] == "nparticles"){

        ui->lineEditParticles->setEnabled(true);
        ui->lineEditParticles->setText(stl[1]);

    }

    if (stl[0] == "sbox"){

        ui->lineEditXBox->setEnabled(true);
        ui->lineEditYBox->setEnabled(true);
        ui->lineEditZBox->setEnabled(true);

        ui->lineEditXBox->setText(stl[1]);
        ui->lineEditYBox->setText(stl[2]);
        ui->lineEditZBox->setText(stl[3]);

    }

    if (stl[0] == "timestep"){ ui->lineEditTimestep->setText(stl[1]);}

    if (stl[0] == "stats_every"){ ui->lineEditStats->setText(stl[1]);}

    if (stl[0] == "traj_every"){ ui->lineEditTrajectory->setText(stl[1]);}

    if (stl[0] == "restart_every"){ ui->lineEditRestart->setText(stl[1]);}

    if (stl[0] == "hist_every"){

        ui->lineEditHistogram->setText(stl[1]);
        if (size==3){ ui->lineEditHistogramSize->setText(stl[2]); }

    }

    if (stl[0] == "equil"){

        ui->lineEditRun->setText(stl[1]);
        ui->radioButtonEquil->setChecked(true);
    }

    if (stl[0] == "run"){

        ui->lineEditRun->setText(stl[1]);
        ui->radioButtonProd->setChecked(true);

    }
}

// private slots

void MainWindow::on_pushButtonOk_clicked()
{

    int mode = form_integrity();

    if (mode != 0){

        //Save InFile Dialog
        QString outfilename = QFileDialog::getSaveFileName(this, tr("Save file"), "", tr("Files (*.*)"));
        QFile outfile(outfilename);
        QFileInfo fi(outfile);

        qDebug() << "exporting input file...";

        outfile.open(QIODevice::WriteOnly | QIODevice::Text);
        QTextStream stream(&outfile);

        stream << "#Template generated by MagChainEdit v0.1\n" << endl;

        if(mode == 2 || mode == 4){

            stream << "\nrestart_data\t" << ui->labelRestartFileWarning->text() << endl;

        } else{

        stream << "nparticles\t" << ui->lineEditParticles->text() << endl;
        stream << "sbox\t" << ui->lineEditXBox->text() << " " << ui->lineEditYBox->text() << " " << ui->lineEditZBox->text() << endl;

        }

        stream << "timestep\t" << ui->lineEditTimestep->text() << "\n";

        if (mode == 3 || mode == 4 || mode == 5){

            if (ui->radioButtonFromExternal->isChecked()){

                stream << "\nattradius\t" << ui->labelAttradiusFileWarning->text() << " " << ui->lineEditAttradiusExternalSize->text() << "\n";

            }

        }

        stream << "stats_every\t" << ui->lineEditStats->text() << endl;
        stream << "traj_every\t" << ui->lineEditTrajectory->text() << endl;
        stream << "restart_every\t" << ui->lineEditRestart->text() << endl;
        stream << "hist_every\t" << ui->lineEditHistogram->text() << endl;

        if (mode == 1 || mode == 2){

            stream << "\nequil\t" << ui->lineEditRun->text() << endl;

        } else if (mode == 3 || mode == 4) {

            stream << "\nrun\t" << ui->lineEditRun->text() << endl;
        }

        outfile.flush();
        outfile.close();


        if (ui->radioButtonAll->isChecked()){

        // export Attradius
            if (ui->checkBoxAttradiusFile->isChecked() && !ui->labelAttradiusFileWarning->text().isEmpty()){

                // copy attradius file to new location
                qDebug() << "exporting attradius file...";
                QFile::copy(attradius_file, fi.absolutePath() + "/" + ui->labelAttradiusFileWarning->text());
            }

        // export restart
            if (ui->checkBoxRestartFile->isChecked() && !ui->labelRestartFileWarning->text().isEmpty()){

                // copy attradius file to new location
                qDebug() << "exporting restart file...";
                QFile::copy(restart_file, fi.absolutePath() + "/" + ui->labelRestartFileWarning->text());

            }

        }

    } else {

        QMessageBox::critical(this,"Erro","Invalid operation");

    }



}

void MainWindow::on_pushButtonCancel_clicked()
{
    this->close();
}


void MainWindow::on_pushButtonImportInput_clicked()
{
    readInputFile();
}

void MainWindow::on_checkBoxRestartFile_toggled(bool checked)
{
    if (checked) {

        bool restart = readRestartFile();

        ui->checkBoxRestartFile->setChecked(restart);

        ui->lineEditParticles->setDisabled(restart);
        ui->lineEditXBox->setDisabled(restart);
        ui->lineEditYBox->setDisabled(restart);
        ui->lineEditZBox->setDisabled(restart);

        ui->radioButtonFromRestart->setChecked(restart);

        if (ui->radioButtonProd->isChecked()){

            ui->checkBoxAttradiusFile->setEnabled(restart);
            ui->radioButtonFromRestart->setEnabled(restart);

            ui->pushButtonShowValues->setEnabled(restart);
            ui->lineEditAttradiusRestartSize->setEnabled(restart);
        }

    } else {

        ui->lineEditParticles->setEnabled(true);
        ui->lineEditXBox->setEnabled(true);
        ui->lineEditYBox->setEnabled(true);
        ui->lineEditZBox->setEnabled(true);

        ui->radioButtonFromRestart->setEnabled(false);
        ui->radioButtonFromRestart->setChecked(false);
        ui->lineEditAttradiusRestartSize->setEnabled(false);

    }

}

void MainWindow::on_radioButtonFromExternal_toggled(bool checked)
{

       ui->checkBoxAttradiusFile->setEnabled(checked);

}

void MainWindow::on_pushButtonShowValues_clicked()
{

    if (ui->radioButtonFromExternal->isChecked()) { attr_values = &attr_values_from_external;}
    if (ui->radioButtonFromRestart->isChecked()) { attr_values = &attr_values_from_restart;}

    attradiusDialog = new DialogShowAttradius(this);
    connect(this,SIGNAL(sendValues(QString*)), attradiusDialog,SLOT(slotShowAttradius(QString*)));
    emit sendValues(attr_values);
    attradiusDialog->show();

}

void MainWindow::on_radioButtonProd_toggled(bool checked)
{

    if (ui->checkBoxRestartFile->isChecked()){

        ui->radioButtonFromRestart->setEnabled(checked);
        ui->radioButtonFromRestart->setChecked(checked);
        ui->lineEditAttradiusRestartSize->setEnabled(checked);

        ui->pushButtonShowValues->setEnabled(checked);

    } else {

        ui->radioButtonFromRestart->setEnabled(false);
        ui->lineEditAttradiusRestartSize->setEnabled(false);

    }

    if (ui->checkBoxAttradiusFile->isChecked()){

        ui->radioButtonFromExternal->setEnabled(checked);
        ui->radioButtonFromExternal->setChecked(checked);
        ui->lineEditAttradiusExternalSize->setEnabled(checked);

        ui->pushButtonShowValues->setEnabled(checked);

    } else {

        ui->radioButtonFromExternal->setEnabled(false);
        ui->lineEditAttradiusExternalSize->setEnabled(false);

    }

    ui->checkBoxAttradiusFile->setEnabled(checked);
    ui->labelAttradiusFileWarning->setEnabled(checked);

}


void MainWindow::on_checkBoxAttradiusFile_toggled(bool checked)
{
    if (checked) {

        bool restart = readAttradiusFile();

        ui->checkBoxAttradiusFile->setChecked(restart);

        ui->radioButtonFromExternal->setEnabled(restart);
        ui->radioButtonFromExternal->setChecked(restart);
        ui->lineEditAttradiusExternalSize->setEnabled(restart);
        ui->pushButtonShowValues->setEnabled(restart);

    } else {

        ui->radioButtonFromExternal->setEnabled(false);
        ui->lineEditAttradiusExternalSize->setEnabled(false);
        ui->pushButtonShowValues->setEnabled(false);

    }
}

void MainWindow::on_pushButtonClearAll_clicked()
{

    ui->lineEditParticles->clear();
    ui->lineEditXBox->clear();
    ui->lineEditYBox->clear();
    ui->lineEditZBox->clear();
    ui->lineEditTimestep->clear();
    ui->lineEditStats->clear();
    ui->lineEditRestart->clear();
    ui->lineEditTrajectory->clear();
    ui->lineEditHistogram->clear();
    ui->lineEditHistogramSize->clear();
    ui->lineEditRun->clear();

}
