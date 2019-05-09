/*
 *  attradius.cpp
 *  Attraction radius application (MagChain package)
 *  Author: Jordi Andreu Segura
 *  e-mail: jandreu@icmab.es
 *  Version: 16/11/2011
 *  Last Modification: 16/11/2011
 *  Description: Solve the equation which gives the distance between the center of the first colloid in a single chain aggregate and the
 *  point laying in the aggregate axis for E=-1kBT, for a given Gamma and different number of colloids in the aggregate.
 *  Generates the attraction radius data file for MagChain code.
 *
 */


#include<iostream>
#include<cstdlib>
#include<iomanip>
#include<cstdio>
#include<cmath>
#include<fstream>

using namespace std;

double minsize=1;
double maxsize=25;
double g=1.0;
double tol = 1e-3;
double dx = 0.01;
double utarget = -1.0;

ofstream logfile,radiusfile;

///// Function Prototypes /////
double EvalEnergy(double m, double g, double x);
void PrintLog(ostream &out);
double randdouble();

/////////////// MAIN FUNCTION //////////
int main(int argc, char * const argv[]){

	cout << endl;
	cout << "*************************************************" << endl;
	cout << " Calculates the attraction radius for aggregates" << endl;
	cout << " from size 1 to maxsize." << endl;
	cout << "*************************************************" << endl;
	cout <<endl;

	cout << "Enter the maximum number of particles in the CG Object (maxsize):" << endl;
	cin >> maxsize;
	cout << "Enter the magnetic coupling parameter (Gamma): " << endl;
	cin >> g;
	cout << "Tolerance: " << endl;
	cin >> tol;

    string filename;
    filename = "attradius.dat";    
	radiusfile.open(filename.c_str());
	radiusfile << "#size\tattradius(gamma="  << g << ")" << endl;
	logfile.open("log.txt");

	double u = 0.0;
	double xmin = 1.0;
	double xmax = 50.0;

	if (EvalEnergy(maxsize, g, xmax) < utarget) { cout << "increase xmax! = " << xmax << endl; exit(0);}
	if (EvalEnergy(minsize, g, xmin) > utarget) { cout << "decrease xmin! = " << xmin << endl; exit(0);}
		
	for (int i=minsize; i<=maxsize; i++){

		radiusfile << i ;

		double x = xmin + (xmax - xmin)*randdouble();
			
		u = EvalEnergy(i,g,x);
		  
		while (abs(u - utarget) > tol) {
		
			if ((u - utarget < 0)) { x = x + (xmax - x)*randdouble(); }
			else { x = x - (x - xmin)*randdouble(); }				

			u = EvalEnergy(i,g,x);
			
		}		
		
		radiusfile << "\t" << (x-0.5)/2.0 << endl;
		cout << "size " << i << ", gamma " << g << ": distance " << x << " (energy: " << u << ")" << ": radius " << (x-0.5)/2.0 << endl;

	}

	PrintLog(logfile);
	PrintLog(cout);
	radiusfile.close();
	logfile.close();

    return 0;
}


// Calculates the energy of two aligned dipoles with vector distance components x,y
double EvalEnergy(double m, double g, double x){

	double energy = 0.0;
	double d = 0.0;
	
	for (int i=1; i<=m; i++){
		
		d = x + (i-1);
		d = pow(d,3);
		energy = energy + 1/d;

	}

	energy = -g*energy;
	return energy;

}

void PrintLog(ostream &out){

	out << "***** attradius Summary *****" << endl;
	out << "Aggregate size range: [" << minsize << "," << maxsize << "]" << endl;
	out << "Magnetic coupling parameter (Gamma): " << g << endl;
	out << "Tolerance: " << tol << endl;
	out << "*****************************" << endl;

}

// Random number [0,1)
double randdouble()
{
    return rand()/(double(RAND_MAX)+1);
} 
