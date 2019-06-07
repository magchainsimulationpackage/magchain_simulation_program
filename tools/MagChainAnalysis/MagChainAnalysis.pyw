#!/usr/bin/env python3

import sys, re
import PyQt5
from PyQt5.QtWidgets import *
from PyQt5 import uic
from PyQt5.QtCore import pyqtSlot, QDate, Qt
from PyQt5.QtGui import QIcon, QPixmap, QFont, QImage

import numpy as np
import matplotlib.pyplot as plt

import time
import os

from Data.Library import *


current_time = lambda : time.time()

dir_principal = os.getcwd()

data_folder = dir_principal + '/Data' #.ui files and temporal files
file_folder = dir_principal + '/Outfiles' #Output file folder

#if not os.path.exists(data_folder): os.mkdir(data_folder)
if not os.path.exists(file_folder): os.mkdir(file_folder)

class Window(QMainWindow): 
	def __init__(self):
		QMainWindow.__init__(self)
		os.chdir(data_folder)
		uic.loadUi('MagChainAnalysis.ui', self)
		os.chdir(dir_principal)

		self.showMaximized()

		self.manage_files = ManageFiles()

		#Load files
		self.load_statistics.clicked.connect(self.obrir_load_files_statistics)
		self.load_histogram.clicked.connect(self.obrir_load_files_histogram)

		#Make plots
		self.plot.clicked.connect(self.make_plots)
		self.plot_hist.clicked.connect(self.make_hist)
		self.plot_specific.clicked.connect(self.make_specific_plots)

		#Convert units
		self.convert.clicked.connect(self.obrir_convert_units)

		#Other
		self.default_fig_settings.clicked.connect(self.obrir_default_figure_settings)

	#Load files
	def obrir_load_files_statistics(self):
		self.manage_files.openFileNameDialog(self.loaded_statistics)

		filename = self.loaded_statistics.text()

		f = open(filename, 'r')

		frames = len(f.readlines()) - 2

		f.close()

		self.stats_frames.setText(str(frames))

	def obrir_load_files_histogram(self):
		self.manage_files.openFileNameDialog(self.loaded_histogram)

		filename = self.loaded_histogram.text()

		f = open(filename, 'r')

		frames = len(f.readlines()) - 2

		f.close()

		self.hist_frames.setText(str(frames))

	def obrir_convert_units(self):

		d = self.d.value() * 1e-9
		T = self.T.value()

		try:
			eta = float(self.eta.text())

			convert = ConvertUnits(d, T, eta)

			self.tau.setText(str(convert.tau))
			self.D1.setText(str(convert.D))

		except:
			QMessageBox.warning(self, 'Warning!', 'Invalid syntax for eta value!')


	#Read data
	def read_stats(self, filename):

		if filename == '':
			QMessageBox.warning(self, 'Warning!', 'You must load the statistics file first!')

		else:

			file = open(filename, 'r')

			time = []
			agg = []
			avg_length = []
			avg_s = []
			cpu_time = []

			for line in file:
				cols = line.split()

				#Use try in order to not append the comments or other imprevists
				try:
					time.append(float(cols[0]))
					agg.append(float(cols[1]))
					avg_length.append(float(cols[2]))
					avg_s.append(float(cols[3]))
					cpu_time.append(float(cols[4]))

				except:
					pass

			return np.array(time), np.array(agg), np.array(avg_length), np.array(avg_s), np.array(cpu_time)

	def read_hist(self, filename, d, T, eta):

		'''
			Returns the data to plot the histogram of the last time step
		'''

		if filename == '':
			QMessageBox.warning(self, 'Warning!', 'You must load the histogram file first!')

		else:

			file = open(filename, 'r')

			lines = file.readlines()

			#take the data for a single frame
			if self.choose_time.isChecked() and self.adimensionalised.isChecked(): #t in simulation units
				t = self.time.value() #input should be in simulation units

				data = lines[t + 2].split()

			elif self.choose_time.isChecked() and not self.adimensionalised.isChecked(): #t in real units
				t = self.time.value() #input should be in real units, so we must convert to simulation units

				convert = ConvertUnits(d, T, eta)

				t = convert.sim_time(t)

				data = lines[int(t) + 2].split()

			else:

				data = lines[-1].split()

			data = [float(item) for item in data[2:]]

			sizes = []

			cols = lines[1].split()

			for item in cols:

				try:
					sizes.append(float(item[4:]))

				except:
					pass

			return sizes, data

	def data_avg_D(self, filename):

		'''
			Returns the data to plot the average diffusion coefficient in time
		'''

		if filename == '':
			QMessageBox.warning(self, 'Warning!', 'You must load the histogram file first!')

		else:

			file = open(filename, 'r')

			#Get sizes (2nd line)
			lines = file.readlines()

			cols = lines[1].split()

			sizes = [float(item[4:]) for item in cols[2:]]

			#Avoid first two lines (cutre)
			file.seek(0)
			next(file) 
			next(file)

			time = []
			D_parallel_avg = []
			D_perpendicular_avg = []
			D_parallel_avg_2 = []
			D_perpendicular_avg_2 = []

			os.chdir(file_folder)

			outfile = open('D_average.dat', 'w')
			outfile.write('#Time\tD parallel avg\tD perpendicular avg\tD parallel avg 2\tD perpendicular avg 2\n')

			for line in file:

				cols = line.split()

				time.append(float(cols[0]))
				time_i = float(cols[0])

				avg_dpar = 0
				avg_dperp = 0

				avg2_dperp = 0
				avg2_dpar = 0

				totalagg = 0
				totalagg2 = 0

				for i in range(len(sizes)):

					N_aggregates_s = float(cols[i + 2])

					avg_dpar = avg_dpar + N_aggregates_s * D_parallel(sizes[i] + 1, 1);

					avg_dperp = avg_dperp + N_aggregates_s * D_perpendicular(sizes[i] + 1, 1);

					avg2_dpar = avg2_dpar + N_aggregates_s * (i+1)*(i+1) * D_parallel(sizes[i] + 1, 1);

					avg2_dperp = avg2_dperp + N_aggregates_s * (i+1)*(i+1) * D_perpendicular(sizes[i] + 1, 1);

					totalagg = totalagg + N_aggregates_s;

					totalagg2 = totalagg2 + N_aggregates_s * (sizes[i] + 1) * (sizes[i] + 1)


				D_parallel_avg.append(avg_dpar / totalagg)
				D_perpendicular_avg.append(avg_dperp / totalagg)
				D_parallel_avg_2.append(avg2_dpar / totalagg2)
				D_perpendicular_avg_2.append(avg2_dperp / totalagg2)

				outfile.write(str(time_i) + '\t' + str(avg_dpar / totalagg) + '\t' + str(avg_dperp / totalagg) + '\t' + str(avg2_dpar / totalagg2) + '\t' + str(avg2_dperp / totalagg2) + '\n')
			
			outfile.close()

			os.chdir(data_folder) #Save temporal npy files

			np.save('time.npy', time)
			np.save('D_parallel_avg.npy', D_parallel_avg)
			np.save('D_perpendicular_avg.npy', D_perpendicular_avg)
			np.save('D_parallel_avg_2.npy', D_parallel_avg_2)
			np.save('D_perpendicular_avg_2.npy', D_perpendicular_avg_2)


	def magnetic_energy(self, filename):

		'''
			Returns the data to plot the average diffusion coefficient in time
		'''

		if filename == '':
			QMessageBox.warning(self, 'Warning!', 'You must load the histogram file first!')

		else:

			file = open(filename, 'r')

			#Get sizes (2nd line)
			lines = file.readlines()

			cols = lines[1].split()

			sizes = [float(item[4:]) for item in cols[2:]]

			#Calculate number of particles and store in N variable
			N = 0
			k = 0

			Energies = [] #List of energy of every type of aggregate

			for line in lines[2 : 3]:
				
				cols = line.split()

				for N_agg in cols[2:]:

					particles = int(sizes[k])
					N += int(N_agg) * particles

					E = 0 #Energy of the aggregate of size sizes[i]
					d = 1

					for i in range(particles - 1):
						for j in range(particles - i - 1):

							r = j*d + d
							theta = 0

							E += U_adim(r, d, theta)

					Energies.append(E)

					k += 1


			#Calculate total energy
			E_frame = []
			time = []

			os.chdir(file_folder)

			f = open('Magnetic_energy_per_colloid.dat', 'w')
			f.write('#Time\tE/N\n')

			for line in lines[2:]:
				
				cols = line.split() #cols[0]=time, cols[1]=Nagg, cols[i>1]=aggregates of size sizes[i-2] 

				time.append(float(cols[0]))

				count = 0
				E_N_i = 0

				for N_agg in cols[2:]: #Naggregates
					
					particles = int(sizes[count])

					E_N_i += (Energies[count] * float(N_agg)) #Energy in this frame due different aggregates

					count += 1

				E_frame.append(E_N_i / N)

				f.write(cols[0] + '\t' + str(E_N_i / N) + '\n')
			
			f.close()

			os.chdir(data_folder) #Save .npy temporal files

			np.save('time.npy', time)
			np.save('Energy.npy', E_frame)

			return np.array(time), np.array(E_frame)


	#Make plots
	def make_plots(self):

		#System parameters
		T = self.T.value()
		eta = self.eta.text()
		d = self.d.value() * 1e-9 #convert to meters

		#Analysis parameters
		filename = self.loaded_statistics.text()
		filename_hist = self.loaded_histogram.text()

		#Plot settings
		title = self.title.text()
		xlabel = self.xlabel.text()
		ylabel = self.ylabel.text()
		data_labels = self.data_labels.text().split()
		avg_labels = self.avg_labels.text().split()
		grid = self.grid.isChecked()

		data_linestyles = self.data_ls.text().split()
		avg_linestyles = self.avg_ls.text().split()
		data_colors = self.data_colors.text().split()
		avg_colors = self.avg_colors.text().split()
		data_markers=self.data_markers.text().split()

		plot_avg = self.plot_avg.isChecked()
		default = self.default_2.isChecked()
		legend = self.legend.isChecked()
		inverse_xlim = self.inverse_xlim.isChecked()
		adimensionalised = self.adimensionalised.isChecked()

		init_frame = self.init_frame.value()
		final_frame = self.final_frame.value()
		dt = self.dt.value()

		equil = 0 #Change it!

		xlog_scale = self.xlog_scale.isChecked()
		ylog_scale = self.ylog_scale.isChecked()

		#Customize figure
		right = self.right.value()
		left = self.left.value()
		top = self.top.value()
		bottom = self.bottom.value()
		wspace = self.wspace.value()
		hspace = self.hspace.value()

		width = self.width.value()
		height = self.height.value()

		background = self.background.text()
		borders = self.borders.text()

		bool_eta = True

		if adimensionalised == False:

			try:
				eta = float(self.eta.text())

			except:
				QMessageBox.warning(self, 'Warning!', 'Invalid syntax for vicosity imput!')

				bool_eta = False


		if bool_eta == True:

			try:

				time_dat, agg_dat, avg_length_dat, avg_s_dat, cpu_time_dat = self.read_stats(filename)

				#Clear arrays
				if final_frame == -1:
					final_frame = len(time_dat)


				time = []
				agg = []
				avg_length = []
				avg_s = []
				cpu_time = []


				for i in range(init_frame, final_frame, dt):

					time.append(time_dat[i])
					agg.append(agg_dat[i])
					avg_length.append(avg_length_dat[i])
					avg_s.append(avg_s_dat[i])
					cpu_time.append(cpu_time_dat[i])

				time = np.array(time)
				agg = np.array(agg)
				avg_length = np.array(avg_length)
				avg_s = np.array(avg_s)
				cpu_time = np.array(cpu_time)


				if adimensionalised == False and self.units_type_time.currentText() != 'Simulation units': #Convert time units

					factor = 1

					if self.units_type_time.currentText() == 'Seconds':
						pass

					elif self.units_type_time.currentText() == 'Minutes':
						factor = 1 / 60

					elif self.units_type_time.currentText() == 'Hours':
						factor = 1/ 3600

					convert = ConvertUnits(d, T, eta)

					time = convert.real_time(time) * factor

				if adimensionalised == False and self.units_type_length.currentText() != 'Simulation units':

					factor = 1

					if self.units_type_length.currentText() == 'Meters':
						pass

					elif self.units_type_length.currentText() == 'Milimeters':
						factor = 1e3

					elif self.units_type_length.currentText() == 'Micrometers':
						factor = 1e6

					elif self.units_type_length.currentText() == 'Nanometers':
						factor = 1e9

					convert = ConvertUnits(d, T, eta)

					avg_length = convert.real_length(avg_length) * factor
					avg_s = convert.real_length(avg_s) * factor
					

				if self.plot_type.currentText() == 'Number of aggregates':

					make_plot = MakePlot(title, xlabel, ylabel, data_labels, avg_labels, grid, data_linestyles, avg_linestyles, data_colors, avg_colors, data_markers, plot_avg, default, legend, inverse_xlim, xlog_scale, ylog_scale, equil, right, left, top, bottom, wspace, hspace, width, height, background, borders)
					plt = make_plot.plot(time, agg)

					plt.show()

				elif self.plot_type.currentText() == 'Average aggregate lenght':

					make_plot = MakePlot(title, xlabel, ylabel, data_labels, avg_labels, grid, data_linestyles, avg_linestyles, data_colors, avg_colors, data_markers, plot_avg, default, legend, inverse_xlim, xlog_scale, ylog_scale, equil, right, left, top, bottom, wspace, hspace, width, height, background, borders)
					plt = make_plot.plot(time, avg_length)

					plt.show()

				elif self.plot_type.currentText() == 'Avgerage s':

					make_plot = MakePlot(title, xlabel, ylabel, data_labels, avg_labels, grid, data_linestyles, avg_linestyles, data_colors, avg_colors, data_markers, plot_avg, default, legend, inverse_xlim, xlog_scale, ylog_scale, equil, right, left, top, bottom, wspace, hspace, width, height, background, borders)
					plt = make_plot.plot(time, avg_s)

					plt.show()

				elif self.plot_type.currentText() == 'CPU time':

					make_plot = MakePlot(title, xlabel, ylabel, data_labels, avg_labels, grid, data_linestyles, avg_linestyles, data_colors, avg_colors, data_markers, plot_avg, default, legend, inverse_xlim, xlog_scale, ylog_scale, equil, right, left, top, bottom, wspace, hspace, width, height, background, borders)
					plt = make_plot.plot(time, cpu_time)

					plt.show()

			except:
				pass

	def make_hist(self):

		#System parameters
		T = self.T.value()
		eta = self.eta.text()
		d = self.d.text()

		#Analysis parameters
		filename = self.loaded_statistics.text()
		filename_hist = self.loaded_histogram.text()

		#Plot settings
		title = self.title.text()
		xlabel = self.xlabel.text()
		ylabel = self.ylabel.text()
		data_labels = self.data_labels.text().split()
		avg_labels = self.avg_labels.text().split()
		grid = self.grid.isChecked()

		data_linestyles = self.data_ls.text().split()
		avg_linestyles = self.avg_ls.text().split()
		data_colors = self.data_colors.text().split()
		avg_colors = self.avg_colors.text().split()
		data_markers=self.data_markers.text().split()

		plot_avg = self.plot_avg.isChecked()
		default = self.default_2.isChecked()
		legend = self.legend.isChecked()
		inverse_xlim = self.inverse_xlim.isChecked()
		adimensionalised = self.adimensionalised.isChecked()

		equil = 0 #Change it!
		N_bars = self.N_bars.value()
		all_bars = self.all_bars.isChecked()
		all_ticks = self.all_ticks.isChecked()

		xlog_scale = self.xlog_scale.isChecked()
		ylog_scale = self.ylog_scale.isChecked()

		#Customize figure
		right = self.right.value()
		left = self.left.value()
		top = self.top.value()
		bottom = self.bottom.value()
		wspace = self.wspace.value()
		hspace = self.hspace.value()

		width = self.width.value()
		height = self.height.value()

		background = self.background.text()
		borders = self.borders.text()

		bool_eta = True
		bool_d = True

		d = 0
		T = 0
		eta = 0

		if adimensionalised == False:

			try:
				eta = float(self.eta.text())

			except:
				QMessageBox.warning(self, 'Warning!', 'Invalid syntax for vicosity imput!')

				bool_eta = False

			try:
				d = float(self.eta.text())

			except:
				QMessageBox.warning(self, 'Warning!', 'Invalid syntax for diameter imput!')

				bool_d = False

		if bool_d == True and bool_eta == True:


			if self.hist_type.currentText() == 'Average aggregate length':

				try:
					x, y = self.read_hist(filename_hist, d, T, eta)

					if not all_bars:
						x = np.linspace(1, N_bars, N_bars)
						y = y[0 : N_bars]

					make_plot = MakePlot(title, xlabel, ylabel, data_labels, avg_labels, grid, data_linestyles, avg_linestyles, data_colors, avg_colors, data_markers, plot_avg, default, legend, inverse_xlim, xlog_scale, ylog_scale, equil, right, left, top, bottom, wspace, hspace, width, height, background, borders)
					plt = make_plot.plot_histogram(x, y)

					if all_ticks:
						ticks = [str(int(item)) for item in x]

						plt.xticks(x, ticks)

					plt.show()

				except:
					pass

	def make_specific_plots(self):

		#System parameters
		T = self.T.value()
		eta = self.eta.text()
		d = self.d.value() * 1e-9 #convert to meters
		gamma = self.gamma.value()

		#Analysis parameters
		filename = self.loaded_statistics.text()
		filename_hist = self.loaded_histogram.text()

		#Plot settings
		title = self.title.text()
		xlabel = self.xlabel.text()
		ylabel = self.ylabel.text()
		data_labels = self.data_labels.text().split()
		avg_labels = self.avg_labels.text().split()
		grid = self.grid.isChecked()

		data_linestyles = self.data_ls.text().split()
		avg_linestyles = self.avg_ls.text().split()
		data_colors = self.data_colors.text().split()
		avg_colors = self.avg_colors.text().split()
		data_markers=self.data_markers.text().split()

		plot_avg = self.plot_avg.isChecked()
		default = self.default_2.isChecked()
		legend = self.legend.isChecked()
		inverse_xlim = self.inverse_xlim.isChecked()
		adimensionalised = self.adimensionalised.isChecked()

		init_frame = self.init_frame.value()
		final_frame = self.final_frame.value()
		dt = self.dt.value()

		equil = 0 #Change it!

		xlog_scale = self.xlog_scale.isChecked()
		ylog_scale = self.ylog_scale.isChecked()

		#Customize figure
		right = self.right.value()
		left = self.left.value()
		top = self.top.value()
		bottom = self.bottom.value()
		wspace = self.wspace.value()
		hspace = self.hspace.value()

		width = self.width.value()
		height = self.height.value()

		background = self.background.text()
		borders = self.borders.text()

		bool_eta = True

		if adimensionalised == False:

			try:
				eta = float(self.eta.text())

			except:
				QMessageBox.warning(self, 'Warning!', 'Invalid syntax for vicosity imput!')

				bool_eta = False


		if bool_eta == True:


			if self.specific_type.currentText() == 'Perpendicular diffusion coefficient' or self.specific_type.currentText() == 'Parallel diffusion coefficient':

				os.chdir(data_folder)

				if not os.path.exists('D_parallel_avg.npy'):

					self.data_avg_D(filename_hist)

				time_dat = np.load('time.npy')
				D_parallel_avg_dat = np.load('D_parallel_avg.npy')
				D_perpendicular_avg_dat = np.load('D_perpendicular_avg.npy')
				D_parallel_avg_2_dat = np.load('D_parallel_avg_2.npy')
				D_perpendicular_avg_2_dat = np.load('D_perpendicular_avg_2.npy')

				#Clear arrays
				if final_frame == -1:
					final_frame = len(time_dat)


				time = []
				D_parallel_avg = []
				D_perpendicular_avg = []
				D_parallel_avg_2 = []
				D_perpendicular_avg_2 = []


				for i in range(init_frame, final_frame, dt):

					time.append(time_dat[i])
					D_parallel_avg.append(D_parallel_avg_dat[i])
					D_perpendicular_avg.append(D_perpendicular_avg_dat[i])
					D_parallel_avg_2.append(D_parallel_avg_2_dat[i])
					D_perpendicular_avg_2.append(D_perpendicular_avg_2_dat[i])

				time = np.array(time)
				D_parallel_avg = np.array(D_parallel_avg)
				D_perpendicular_avg = np.array(D_perpendicular_avg)
				D_parallel_avg_2 = np.array(D_parallel_avg_2)
				D_perpendicular_avg_2 = np.array(D_perpendicular_avg_2)

			elif self.specific_type.currentText() == 'Magnetic energy per colloid':

				if not os.path.exists('Energy.npy'):

					self.magnetic_energy(filename_hist)

				time_dat = np.load('time.npy')
				E_N_dat = np.load('Energy.npy')

				#Clear arrays
				if final_frame == -1:
					final_frame = len(time_dat)


				time = []
				E_N = []

				for i in range(init_frame, final_frame, dt):

					time.append(time_dat[i])
					E_N.append(E_N_dat[i])

				time = np.array(time)
				E_N = np.array(E_N)
				
				if adimensionalised == False and self.energy_units.currentText() == 'U* units':
					pass

				elif adimensionalised == False and self.energy_units.currentText() == 'KB*T units':

					if gamma == 0:
						QMessageBox.warning(self, 'Warning!', 'Gamma shouldn\'t be 0! Unit conversion cancelled.')

					else:
						E_N *= gamma


			factor_time = 1
			factor_length = 1


			if adimensionalised == False and self.units_type_time_specific.currentText() != 'Simulation units': #Convert time units

				if self.units_type_time_specific.currentText() == 'Seconds':
					pass

				elif self.units_type_time_specific.currentText() == 'Minutes':
					factor_time = 1 / 60

				elif self.units_type_time_specific.currentText() == 'Hours':
					factor_time = 1/ 3600

				convert = ConvertUnits(d, T, eta)

				time = convert.real_time(time) * factor_time

			if adimensionalised == False and self.units_type_length_specific.currentText() != 'Simulation units':

				if self.units_type_length_specific.currentText() == 'Meters':
					pass

				elif self.units_type_length_specific.currentText() == 'Milimeters':
					factor_length = 1e3

				elif self.units_type_length_specific.currentText() == 'Micrometers':
					factor_length = 1e6

				elif self.units_type_length_specific.currentText() == 'Nanometers':
					factor_length = 1e9 #Convert length units

			if self.specific_type.currentText() == 'Perpendicular diffusion coefficient' or self.specific_type.currentText() == 'Parallel diffusion coefficient':

				if adimensionalised == False and (self.units_type_time_specific.currentText() != 'Simulation units' or self.units_type_length_specific.currentText() != 'Simulation units'):
					convert = ConvertUnits(d, T, eta)

					D_parallel_avg = convert.diffusion_coefficient(D_parallel_avg) * factor_length / factor_time
					D_perpendicular_avg = convert.diffusion_coefficient(D_perpendicular_avg) * factor_length / factor_time

			#Plots
			if self.specific_type.currentText() == 'Perpendicular diffusion coefficient':

				make_plot = MakePlot(title, xlabel, ylabel, data_labels, avg_labels, grid, data_linestyles, avg_linestyles, data_colors, avg_colors, data_markers, plot_avg, default, legend, inverse_xlim, xlog_scale, ylog_scale, equil, right, left, top, bottom, wspace, hspace, width, height, background, borders)
				plt = make_plot.plot(time, D_perpendicular_avg)

				plt.show()

			elif self.specific_type.currentText() == 'Parallel diffusion coefficient':

				make_plot = MakePlot(title, xlabel, ylabel, data_labels, avg_labels, grid, data_linestyles, avg_linestyles, data_colors, avg_colors, data_markers, plot_avg, default, legend, inverse_xlim, xlog_scale, ylog_scale, equil, right, left, top, bottom, wspace, hspace, width, height, background, borders)
				plt = make_plot.plot(time, D_parallel_avg)

				plt.show()

			elif self.specific_type.currentText() == 'Magnetic energy per colloid':
				
				make_plot = MakePlot(title, xlabel, ylabel, data_labels, avg_labels, grid, data_linestyles, avg_linestyles, data_colors, avg_colors, data_markers, plot_avg, default, legend, inverse_xlim, xlog_scale, ylog_scale, equil, right, left, top, bottom, wspace, hspace, width, height, background, borders)
				plt = make_plot.plot(time, E_N)

				plt.show()

	#Other
	def obrir_default_figure_settings(self):

		self.right.setValue(0.9)
		self.left.setValue(0.12)
		self.top.setValue(0.88)
		self.bottom.setValue(0.11)
		self.wspace.setValue(0.2)
		self.hspace.setValue(0.2)

		self.width.setValue(6.4)
		self.height.setValue(4.8)

		self.background.setText('w')
		self.borders.setText('w')

			
	#close event
	def closeEvent(self, event):
		result = QMessageBox.question(self, 'Leaving...','Do you want to exit?', QMessageBox.Yes | QMessageBox.No)
		if result == QMessageBox.Yes:

			os.chdir(data_folder)

			if os.path.exists('Energy.npy'): os.remove('Energy.npy')
			if os.path.exists('D_parallel_avg.npy'): os.remove('D_parallel_avg.npy')
			if os.path.exists('D_perpendicular_avg.npy'): os.remove('D_perpendicular_avg.npy')
			if os.path.exists('D_parallel_avg_2.npy'): os.remove('D_parallel_avg_2.npy')
			if os.path.exists('D_perpendicular_avg_2.npy'): os.remove('D_perpendicular_avg_2.npy')
			if os.path.exists('time.npy'): os.remove('time.npy')

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

		
app = QApplication(sys.argv)
_window=Window()
_window.show()
app.exec_()