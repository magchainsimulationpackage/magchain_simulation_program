import numpy as np
import matplotlib.pyplot as plt

k_B = 1.38064e-23


#Diffusion coefficients

def D_perpendicular(l, w):

	if l == 1.0:

		D = 1

	elif l == 2:

		D = 0.5 + (1.0/(8.0))*(np.log(6.0/w) + 0.5);

	elif l > 2.0 :

		D = (3.0/(4*l))*(np.log(2.0*l/w) + 0.5);
	
	else:
		#Error
		pass
	
	return D

def D_parallel(l, w):

	if l == 1.0:

		D = 1

	elif l == 2:
		
		D = 0.5 + (1.0/(4.0))*(np.log(6.0/w) - 0.5);

	elif l > 2.0 :

		D = (3.0/(2*l))*(np.log(2.0*l/w) - 0.5);
	
	else:
		#Error
		pass
	
	return D


#Magnetic energy for chains

def U_adim(r, d, theta): #In units of U*
	return -( d / r)**3 * (1 - 1.5 * np.sin(theta)**2)


#Unit conversion

class ConvertUnits(object):

	def __init__(self, d, T, eta):
		
		self.d = d
		self.T = T
		self.eta = eta

		self.D = k_B * self.T / (3 * np.pi * self.eta * self.d)
		self.tau = self.d**2 / self.D

	def real_time(self, t):

		return t * self.tau

	def real_length(self, r):
		return r * self.d

	def sim_time(self, t):

		return t / self.tau

	def sim_length(self, r):
		return r / self.d

	def diffusion_coefficient(self, D):
		return D * self.d**2 / self.tau


#Plots

class MakePlot(object):
	def __init__(self, title, xlabel, ylabel, data_labels, avg_labels, grid, data_linestyles, avg_linestyles, data_colors, avg_colors, data_markers, plot_avg, default, legend, inverse_xlim, xlog_scale, ylog_scale, equil, right, left, top, bottom, wspace, hspace, width, height, background, borders):

		self.title = title
		self.xlabel = xlabel
		self.ylabel = ylabel
		self.data_labels = data_labels
		self.avg_labels = avg_labels
		self.grid = grid

		self.data_linestyles = data_linestyles
		self.avg_linestyles = avg_linestyles
		self.data_colors = data_colors
		self.avg_colors = avg_colors
		self.data_markers = data_markers

		self.plot_avg = plot_avg
		self.default = default
		self.legend = legend
		self.inverse_xlim = inverse_xlim

		self.equil = equil

		self.xlog_scale = xlog_scale
		self.ylog_scale = ylog_scale

		#Customize figure
		self.right = right
		self.left = left
		self.top = top
		self.bottom = bottom
		self.wspace = wspace
		self.hspace = hspace

		self.x = width
		self.y = height

		self.background = background
		self.borders = borders

	#Plot analysed data
	def plot(self, x_array, y_array):


		if self.default == False:
			plt.figure(figsize=(self.x, self.y), facecolor=self.background, edgecolor=self.borders)

		if len(y_array.shape) == 1 :

			avg = np.mean(y_array[self.equil :])

			if self.default == True:
				plt.plot(x_array, y_array)

				if self.plot_avg == True:
					plt.plot(x_array, np.linspace(avg, avg, len(x_array)))

			else:
				plt.plot(x_array, y_array, ls=self.data_linestyles[0], marker=self.data_markers[0] , color=self.data_colors[0], label=self.data_labels[0])

				if self.plot_avg == True:
					plt.plot(x_array, np.linspace(avg, avg, len(x_array)), ls=self.avg_linestyles[0], color=self.avg_colors[0], label=self.avg_labels[0] + ' ' + str(round(avg, 3)))

		else:

			if len(self.data_labels) == len(y_array) and len(self.data_colors) == len(y_array) and len(self.data_linestyles) == len(y_array):
				for i in range(len(y_array)):

					avg = np.mean(y_array[i][self.equil :])

					if self.default == True:

						plt.plot(x_array, y_array[i])

						if self.plot_avg == True:
							plt.plot(x_array, np.linspace(avg, avg, len(x_array)))

					else:
						plt.plot(x_array, y_array[i], ls=self.data_linestyles[i], marker=self.data_markers[i], color=self.data_colors[i], label=self.data_labels[i])

						if self.plot_avg == True:
							plt.plot(x_array, np.linspace(avg, avg, len(x_array)), ls='--', color='k', label='Average' + ' ' + str(round(avg, 3)))	

			else:			

				for i in range(len(y_array)):

					avg = np.mean(y_array[i][self.equil :])

					if self.default == True:

						plt.plot(x_array, y_array[i])

						if self.plot_avg == True:
							plt.plot(x_array, np.linspace(avg, avg, len(x_array)))

					else:
						plt.plot(x_array, y_array[i], ls=self.data_linestyles[0], marker=self.data_markers[0], color=self.data_colors[0], label=self.data_labels[0])

						if self.plot_avg == True:
							plt.plot(x_array, np.linspace(avg, avg, len(x_array)), ls='--', color='k', label='Average' + ' ' + str(round(avg, 3)))
							

		plt.title(self.title)
		plt.xlabel(self.xlabel)
		plt.ylabel(self.ylabel)

		if self.inverse_xlim == True:
			plt.xlim(x_array[0], x_array[-1])


		if self.default == False:
			plt.subplots_adjust(right=self.right, left=self.left, top=self.top, bottom=self.bottom, wspace=self.wspace, hspace=self.hspace)
			self.legend == False

		if self.legend == True:
			plt.legend()

		if self.xlog_scale == True:
			plt.xscale("log")

		if self.ylog_scale == True:
			plt.yscale("log")

		plt.grid(self.grid)

		return plt

	def plot_comparison(self, x_array, y_array):

		if self.default == False:
			plt.figure(figsize=(self.x, self.y), facecolor=self.background, edgecolor=self.borders)

		for i in range(2):

			avg = np.mean(y_array[i][self.equil :])

			if self.default == True:
				plt.plot(x_array[i], y_array[i])

				if self.plot_avg == True:
					plt.plot(x_array[i], np.linspace(avg, avg, len(x_array[i])))

			else:
				plt.plot(x_array[i], y_array[i], ls=self.data_linestyles[i], marker=self.data_markers[i] , color=self.data_colors[i], label=self.data_labels[i])

				if self.plot_avg == True:
					plt.plot(x_array[i], np.linspace(avg, avg, len(x_array[i])), ls=self.avg_linestyles[i], color=self.avg_colors[i], label=self.avg_labels[i] + ' ' + str(round(avg, 3)))


			plt.title(self.title)
			plt.xlabel(self.xlabel)
			plt.ylabel(self.ylabel)

			if self.inverse_xlim == True:
				plt.xlim(x_array[-1], x_array[0])

			if self.default == False:
				plt.subplots_adjust(right=self.right, left=self.left, top=self.top, bottom=self.bottom, wspace=self.wspace, hspace=self.hspace)
				self.legend == False

			if self.legend == True:
				plt.legend()

			plt.grid(self.grid)

		return plt

	def plot_histogram(self, x_array, y_array):

		if self.default == False:
			plt.figure(figsize=(self.x, self.y), facecolor=self.background, edgecolor=self.borders)

		if self.default == True:
			plt.bar(x_array, y_array)

		else:
			plt.bar(x_array, y_array, color=self.data_colors[0], label=self.data_labels[0])


		plt.title(self.title)
		plt.xlabel(self.xlabel)
		plt.ylabel(self.ylabel)

		if self.inverse_xlim == True:
				plt.xlim(x_array[-1], x_array[0])

		if self.default == False:
			plt.subplots_adjust(right=self.right, left=self.left, top=self.top, bottom=self.bottom, wspace=self.wspace, hspace=self.hspace)
			self.legend == False

		if self.legend == True:
			plt.legend()

			plt.grid(self.grid)

		return plt
		


if __name__ == '__main__':
	
	convert = ConvertUnits(100e-9, 300, 1e-3)

	print(convert.real_time(1))