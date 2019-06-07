#!/usr/bin/env python3

import sys, re
import PyQt5
from PyQt5.QtWidgets import *
from PyQt5 import uic
from PyQt5.QtCore import pyqtSlot, QDate, Qt
from PyQt5.QtGui import QIcon, QPixmap, QFont, QImage

import os
import shutil

dir_principal = os.getcwd()

data_folder = dir_principal + '/Data'
output_folder = dir_principal + '/Outputs'

if not os.path.exists(output_folder): os.mkdir(output_folder)

class Window(QMainWindow): 
    def __init__(self):
        QMainWindow.__init__(self)

        os.chdir(data_folder)
        uic.loadUi('MagChainSimulator.ui', self)

        #self.showMaximized()

        self.manage_files = ManageFiles()

        #Load files
        self.load_attradius.clicked.connect(self.obrir_load_attradius)

        self.load_input.clicked.connect(self.obrir_load_input)

        self.load_magchain.clicked.connect(self.obrir_load_magchain)

        #Run
        self.run.clicked.connect(self.run_simulation)


    #Load files
    def obrir_load_attradius(self):
        self.manage_files.openFileNameDialog(self.attradius)

    def obrir_load_input(self):
        self.manage_files.openFileNameDialog(self.input)

    def obrir_load_magchain(self):
        self.manage_files.openFileNameDialog(self.magchain)

    def get_name(self, filename):
        dirs = []
        this = ''

        for item in filename:

            if item != '/':
                this = this + str(item)
                
            elif item == '/':
                dirs.append(this)
                this = ''

        dirs.append(this)

        return dirs[-1]
    
    def copy_files(self):
        #Copy the files into the data folder

        attradius_file = self.attradius.text()
        input_file = self.input.text()
        magchain_file = self.magchain.text()

        all_correct = False
        total_filename = ''

        if attradius_file == '':
            QMessageBox.warning(self, 'Warning!', 'attradius file not loaded!')


        elif input_file == '':
            QMessageBox.warning(self, 'Warning!', 'Input file not loaded!')

        elif magchain_file == '':
            QMessageBox.warning(self, 'Warning!', 'MagChain executable not loaded!')

        else:

            attradius_name = self.get_name(attradius_file)
            input_name = self.get_name(input_file)
            magchain_name = self.get_name(magchain_file)

            shutil.copy(attradius_file, output_folder + '/' + attradius_name)
            shutil.copy(input_file, output_folder + '/' + input_name)
            shutil.copy(magchain_file, output_folder + '/' + magchain_name)

            magchain_filename = output_folder + '/' + magchain_name
            input_filename = output_folder + '/' + input_name

            total_filename = magchain_filename + ' ' + input_filename

            all_correct = True

        return total_filename, all_correct

    def run_simulation(self):

        total_filename, all_correct = self.copy_files()

        if all_correct:

            os.chdir(output_folder)

            os.system(total_filename)

    #close event
    def closeEvent(self, event):
        result = QMessageBox.question(self, 'Leaving...','Do you want to exit?', QMessageBox.Yes | QMessageBox.No)
        if result == QMessageBox.Yes:

            event.accept()  
        else:
            event.ignore()


class ManageFiles(QFileDialog):
    def __init__(self):
        QFileDialog.__init__(self)

        self.title = 'Save files'
        self.left = 10
        self.top = 10
        self.width = 640
        self.height = 400 

        self.initUI()

    def initUI(self):
        self.setWindowTitle(self.title)
        self.setGeometry(self.left, self.top, self.width, self.height)

    def saveFileDialog(self, name):
        options = QFileDialog.Options()
        options |= QFileDialog.DontUseNativeDialog

        fileName, _ = QFileDialog.getSaveFileName(self, 'Save files') 

        if fileName:
            os.chdir(data_folder)
            if os.path.exists('%s.png' % name): copyfile('%s.png' % name, fileName + '.png')
            else: QMessageBox.warning(self, 'Warning!', 'The plot doesn\'t exist!') 

    def openFileNameDialog(self, name):
        options = QFileDialog.Options()
        options |= QFileDialog.DontUseNativeDialog

        fileName, _ = QFileDialog.getOpenFileName(self,"QFileDialog.getOpenFileName()", "","All Files (*);;Python Files (*.py)", options=options)

        if fileName:
            name.setText(fileName)

    def openFolderDialog(self, name):
        options = QFileDialog.Options()
        options |= QFileDialog.DontUseNativeDialog

        fileName = str(QFileDialog.getExistingDirectory(self, 'Select folder', options=options))

        if fileName:
            name.setText(fileName)

        
app = QApplication(sys.argv)
_window=Window()
_window.show()
app.exec_()

