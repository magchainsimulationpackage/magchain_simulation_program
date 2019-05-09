/*
 *  attradius_lin.cpp
 *  Attraction radius application
 *  Author: Jordi Andreu Segura
 *  e-mail: jandreu@icmab.es
 *  Version: 05/07/2012
 *  Last Modification: 31/7/2012. We print the gamma value for the edge colloidal particle only for the largest aggregate.
 *  Description: Solve the equation which gives the distance between the center of the first colloid in a single chain aggregate and the
 *  point laying in the aggregate axis for E=-1kBT, for a given external applied magnetic field and different number of colloids in
 *  the aggregate. In this case, we need also to provide the magnetic response of the particles.
 *  Generates the attraction radius data file for MagChain code.
 *
 */


#include <iostream>
#include <cstdlib>
#include <iomanip>
#include <cstdio>
#include <cmath>
#include <fstream>
#include <vector>
#include <algorithm>

using namespace std;

// Physical Constants
double kb = 1.3806503e-23;
double pi = 4.0*atan(1);
double mu0 = 4.0*pi*1.0e-7;



///// Dipole Class /////
class CDipole{
    
public:
    
    int id;             // identifier
    double diameter;    // particles diameter
    double xpos;        // position along chain axis
    double msat;        // magnetic dipole at satration
    double meff;        // magnetic dipole
    double lparam;      // fitting parameter corresponding to the langevin function
    double xi;          // magnetic susceptibility
    
    CDipole(int i, double d, double x, double xx, double m, double b){
        
        id = i;
        diameter = d;
        xpos = x;
        msat = m;
        meff = 0.0;
        lparam = b;
        xi = xx;
        
    }
    
    // MAGNETIC RESPONSE 1: Returns the magnetic dipole according to a Langevin function in [J/T] or [A/m2]
    // bfield is given in Tesla
    long double LangevinF( double bfield ){
        
        long double x = (lparam*bfield);        
        if ( x == 0) {return 0;}
        else { return msat*((exp(x)+exp(-x))/(exp(x)-exp(-x)) - 1.0/x); }

    }
    
    // MAGNETIC RESPONSE 2: Returns the magnetic dipole according to a linear function in [J/T] or [A/m2]
    // bfield is given in Tesla
    long double LinearF(long double bfield){
        
        double volume = 4.0*atan(1)*diameter*diameter*diameter/6.0;
//        cout << this->diameter << endl;
        return xi*bfield*volume/mu0;
    }
    
    // Prints information about dipole properties
    void Print(){
        
        cout << setw(4) << id << setw(12) << diameter << setw(12) << xpos << setw(12) << xi << setw(12) << meff << setw(12) << msat << setw(4) << lparam << setw(12) << endl;
        
    }
    
    // Writes the magnetic response of the particles in a file
    void WriteLangevinResponse(double bmin, double bmax, double db){
        
        ofstream outfile;
        string filename("response.txt");
        outfile.open(filename.c_str());
        
        CDipole dipole(0,diameter,0,xi,msat,lparam);
        
        outfile << "#B\tm[J/T]\t[A/m]\n";
        
        double volume = 2.0*atan(1)*diameter*diameter*diameter/3.0;
        
        for (int i=0; i<ceil((bmax-bmin)/db); i++){
            //Langevin response
//P1            outfile << bmin + i*db << "\t" << dipole.LangevinF(bmin + i*db) << "\t" << dipole.LangevinF(bmin + i*db)/volume << endl;
            //Linear response
            outfile << bmin + i*db << "\t" << dipole.LinearF(bmin + i*db) << "\t" << dipole.LinearF(bmin + i*db)/volume << endl;
            
        }
        
        outfile.close();
        
    }
    
};


// Dipole Properties (Maria's sample)
//double musat = 3.0e-19;
//double lparam = 68.0;
//double diameter = 12.0e-9;

// Dipole Properties (Granada)
double diameter = 170.0e-9;
double musat = 1.066e-16;
double lparam = 39.433;
//double xi = 0.6;

// Dipole Properties (Promislow)
//double diameter = 600.0e-9;
//double musat = 0.0;
//double lparam = 0.0;
//double xi = 0.9;

// Dipole Properties (Invented, targeting gamma=100)
//double musat = 4.55e-17;
//double lparam = 10.0;
//double diameter = 100.0e-9;

// External Properties
double temp = 298;
// External field in Tesla
double extBfield = 0.03;//Granada
//double extBfield = 0.00241;//Promislow lambda=8.6
//double extBfield = 0.00359;//Promislow lambda=19
//double extBfield = 0.00480;//Promislow lambda=34
//double extBfield = 0.00637;//Promislow lambda=60
//double extBfield = 0.00781;//Promislow lambda=90

// Convergence criterias
double mtol = 1e-5;
double utol = 1e-4;
double u = 0.0;
double utarget = -1.0;

// Chain size range
double minsize=1;
double maxsize=100;

// Distance range
double xmin = 1.0*diameter;
double xmax = 50.0*diameter;
double dx = 0.01*diameter;

// Other global variables for log
double maxgamma = 0.0;
double mingamma = 1.0e10;
double edgegamma = 0.0;

// Output files
ofstream logfile,radiusfile;

///// Function Prototypes /////
double effBfield(vector <CDipole*> dipole, CDipole* auxd);
double EvalEnergy(vector <CDipole*> dipole, CDipole* auxd);
double randdouble();
void PrintLog(ostream &out);
double CalculateGammaStatistics(vector <CDipole*> const d, double &mingamma, double &maxgamma, double &edgegamma);

/////////////// MAIN FUNCTION //////////
int main(int argc, char * const argv[]){
    
	cout << endl;
	cout << "*************************************************" << endl;
	cout << " Calculates the attraction radius for aggregates" << endl;
	cout << " from size 1 to maxsize." << endl;
	cout << "*************************************************" << endl;

/*
	cout << "Enter the maximum number of particles in the CG Object (maxsize):" << endl;
	cin >> maxsize;
    cout << "Enter system temperature [K]: " << endl;
    cin >> temp;
    cout << "Enter the external magnetic field [T]: " << endl;
	cin >> extBfield;
    cout << "Enter the magnetic dipole [J/T] at saturation:" << endl;
    cin >> musat;
    cout << "Enter particle diameter [m]:" << endl;
    cin >> diameter;
    cout << "Enter the Langevin fitting parameter [1/T]:" <<endl;
    cin >> lparam;
	cout << "Energy tolerance: " << endl;
	cin >> utol;
    cout << "Magnetization tolerance: " << endl;
    cin >> mtol;

*/    
    string filename;
    filename = "attradius.dat";    
	radiusfile.open(filename.c_str());
	radiusfile << "#size\tattradius(extB = "  << extBfield << ")" << endl;
	logfile.open("log.txt");

    
    // Writes particles response to auxiliar file (check magnetic response)
    CDipole d(0,diameter,0,0,musat,lparam);
//    CDipole d(0,diameter,0,xi,musat,lparam);
    d.WriteLangevinResponse(-extBfield, extBfield, (2.0*extBfield/100.0));

    // Declares a chain of dipolar particles
    vector <CDipole*> dipole;
    
    // For each chain with different length
    for (int n=minsize; n<=maxsize; n++){

        cout << "***************************************\n";
        cout << "Chain with size " << n << ":" << endl;
        cout << "***************************************\n";
        // 1) Initializes temporary variables for each change
        //    Creates a dipolar chain along a z axis with minsize dipolar particles

        
        for (int k=0; k<n; k++){
                                            // id, diameter, pos, suscept, musat, lparam
//            CDipole* auxdipole = new CDipole(k,diameter,0-k*diameter,xi,musat,lparam);
            CDipole* auxdipole = new CDipole(k,diameter,0-k*diameter,0,musat,lparam);
            dipole.push_back(auxdipole);
            
        }
        
        for (int k=0; k<dipole.size(); k++){

            dipole[k]->Print();
            
        }

        vector <double> oldmeff(dipole.size(),0.0);         // old dipole magnetization
        vector <double> oldBeff(dipole.size(),extBfield);   // old magnetic field experienced by a dipole
        vector <double> newmeff(dipole.size(),0.0);         // new dipole magnetization (after update the total magnetic field applied)
        vector <double> newBeff(dipole.size(),0.0);         // New magnetic field experienced by a dipole
        
                        
        // 2) Initializes the dipoles: calculates the effective magnetization according to the applied field for each dipole
        //    in the chain. Stores the old magnetization values.
        cout << "Effective magnetization due to ext B:" << endl;
        for (int i=0; i<dipole.size(); i++){
         
            dipole[i]->meff = dipole[i]->LangevinF(extBfield);
//Promislow            dipole[i]->meff = dipole[i]->LinearF(extBfield);
            oldmeff[i] = dipole[i]->meff;
            dipole[i]->Print();
            
        }


        // 3) Calculates the effective magnetic field experienced by each dipole in a chain due to the external field
        //    and the local field created by the other dipoles in the chain.
        double maxdeltamueff = 2*mtol;    // Initialize 2*maximum error accepted in magnetization (convergence criteria)
        int iterm = 0;

        while (maxdeltamueff > mtol){
        
            for (int i=0; i<dipole.size(); i++){

                newBeff[i] = extBfield + effBfield(dipole, dipole[i]);
                
            }

            // 4) Calculates the magnetization of each dipole according to the effective magnetic field
            for (int i=0; i<dipole.size(); i++){    
                
                   newmeff[i] = dipole[i]->LangevinF(newBeff[i]);
 //Promislow                    newmeff[i] = dipole[i]->LinearF(newBeff[i]);            
            }

            
            // 5) Applies convergence criteria:
            //    Check change on magnetization between new and old for all dipoles
            
            vector <double> deltamueff(dipole.size(),0.0);
            
            for (int i=0; i<dipole.size(); i++){    
                        
                deltamueff[i] = abs((newmeff[i] - oldmeff[i])/oldmeff[i]);      
                
            }
            
            //returns largest element: error in effective magnetization
            maxdeltamueff = *max_element(deltamueff.begin(),deltamueff.end());
                
            oldBeff = newBeff;      // replaces old field by new ones
            oldmeff = newmeff;      // replaces old magnetization by new ones

            // Stores new effective magnetizations for each dipole
            for (int i=0; i<dipole.size(); i++){    
                    
                dipole[i]->meff = newmeff[i];
                    
            }

            iterm++;
            
        }
        
        // End convergence criteria. Here, each dipole contains its effective magnetization.
        
        // 6) Check the update on the effective magnetic field for all dipoles in a chain
        cout << "Effective magnetization due to ext B and local dipolar field (converged after " << iterm << " iterations):" << endl;
        CalculateGammaStatistics(dipole, mingamma, maxgamma, edgegamma);
        for (int i=0; i<dipole.size(); i++){
            
            dipole[i]->Print();
            
        }
        
        
        cout << "\n*** Calculating the position according to energy criteria ***" << endl;
        // 7) Calculates the energy at a certain point in space aligned with the chain.
        //    The calculation is iterated untill the energy diference between the calculated
        //    value and the target value (kT in our model) is less than certain value.
        //    The function EvalEnergy2 should return the energy at distance x due to a dipolar chain
        //    dipole
        
        
        // Attention, the test dipole will have a magnetization which depends on the external B field 
        // plus the local field created by the dipole.
        
		double x = xmin + (xmax - xmin)*randdouble();

        // Creates a test dipole at position x. Its id is chainlength + 1
//        CDipole testdipole((int)dipole.size(),diameter,x,xi,musat,lparam);
        CDipole testdipole((int)dipole.size(),diameter,x,0,musat,lparam);

        // Calculates the effective magnetization according to the external magnetic field + local field
        testdipole.meff = testdipole.LangevinF(extBfield + effBfield(dipole,&testdipole));
        cout << "test dipole: ";
        testdipole.Print();

        u = EvalEnergy(dipole,&testdipole);
        
        int iteru = 0;

		while (abs(u - utarget) > utol) {
            
			if ((u - utarget < 0)) { x = x + (xmax - x)*randdouble(); }
			else { x = x - (x - xmin)*randdouble(); }				
            
            // Update position and magnetization
            testdipole.xpos = x;
            testdipole.meff = testdipole.LangevinF(extBfield + effBfield(dipole,&testdipole));
//Promislow            testdipole.meff = testdipole.LinearF(extBfield + effBfield(dipole,&testdipole));
            
            u = EvalEnergy(dipole,&testdipole);
//            cout << u  << "\t" << x << endl;
            iteru++;
		}		
		
        x = x/diameter;
        
        cout << "position matching energy criteria (after " << iteru << " iterations): " << u << ", position: " << x << endl;
        
//		radiusfile << n << "\t" << (x-0.5)/2.0 << "\t" << CalculateGammaStatistics(dipole, mingamma, maxgamma) << endl;        
		radiusfile << n << "\t" << (x-0.5)/2.0 << endl;        
        
        // Freeing dynamic memory for dipolar chain
        for (int i=0; i<dipole.size(); i++){ delete dipole[i]; }; dipole.clear();
        
        // clear auxiliar vectors
        oldBeff.clear();
        oldmeff.clear();
        newBeff.clear();
        newmeff.clear();
        
	}
    
	PrintLog(logfile);
	PrintLog(cout);
	radiusfile.close();
	logfile.close();
    
    cout << "done!" << endl;

    return 0;

}



// Calculates the energy of a dipole m at position x (aligned with a dipolar chain)
// due to the local field produced by the chain. Returns the energy in KbT!!
double EvalEnergy(vector <CDipole*> dipole, CDipole* auxd){
    
    return -auxd->meff*effBfield(dipole,auxd)/(kb*temp);
    
}


// Calculates the effective magnetic field experimented by a dipole due to the other dipoles
// stored in the CDipole vector
double effBfield(vector <CDipole*> dipole, CDipole* auxd){

    double btot = 0;    // Total B field created by dipoles in a chain at position of dipole i
    double d=0;         //distance between aligned dipoles

    for (int k=0; k<dipole.size(); k++){
        
        if (dipole[k]->id!=auxd->id){   // Discards it self-contribution in case the aux dipole is already in the chain

            d = abs(auxd->xpos-dipole[k]->xpos);
            btot = btot + mu0*2.0*dipole[k]->meff/(4.0*pi*d*d*d);
            
        }

    }

    return btot;
    
}

// Writes log summary to a file or screen
void PrintLog(ostream &out){
    
	out << "***** attradius Summary *****" << endl;
	out << "Aggregate size range: [" << minsize << "," << maxsize << "]" << endl;
    out << "Particle diameter [m]: " << diameter << endl;
//    out << "Magnetic susceptibility: " << xi << endl;
    out << "Magnetic dipole at saturation [J/T]: " << musat << endl;
    out << "Langevin fitting parameter [1/T]: " << lparam << endl;
    out << "External magnetic field [T]: " << extBfield << endl;
    out << "Temperature [K]: " << temp << endl;
	out << "Magnetic coupling parameter (Gamma) at saturation: " << mu0*musat*musat/(2.0*pi*diameter*diameter*diameter*kb*temp) << endl;
    out << "Minimum average Gamma: " << mingamma << endl;
    out << "Maximum average Gamma: " << maxgamma << endl;
    out << "Maximum Gamma at the edge: " << edgegamma << endl;
	out << "Energy tolerance: " << utol << endl;
	out << "Magnetization tolerance: " << mtol << endl;
	out << "*****************************" << endl;
    
}

double CalculateGammaStatistics(vector <CDipole*> const d, double &mingamma, double &maxgamma, double &edgegamma){
 
    vector <double> value(d.size()-1,0.0);
    
    for (int i=1; i<d.size(); i++){
        
        value[i-1] = mu0*d[i-1]->meff*d[i]->meff/(2.0*pi*d[i]->diameter*d[i]->diameter*d[i]->diameter*kb*temp);
        
    }
    
    
    double ave = 0.0;
    double max = 0.0;
    double min = 0.0;

    if (d.size()!=1){
    
    max = *max_element(value.begin(),value.end());
    min = *min_element(value.begin(),value.end());
    edgegamma = value[0];
    
    for (int i=0; i<value.size(); i++){
        
        ave += value[i];
    }
    
    ave = ave/value.size();
        
    } else {
        
        ave = min = max = edgegamma = mu0*d[0]->meff*d[0]->meff/(2.0*pi*d[0]->diameter*d[0]->diameter*d[0]->diameter*kb*temp);
    }
    
    if (mingamma > ave){mingamma = ave;}
    if (maxgamma < ave){maxgamma = ave;}
    
    cout << min << "\t" << max << "\t" << ave << endl;
    
    return ave;
    
}

// Random number [0,1)
double randdouble()
{
    return rand()/(double(RAND_MAX)+1);
}
