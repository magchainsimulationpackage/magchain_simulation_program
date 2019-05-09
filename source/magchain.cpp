/***************************************************************************
 *
 *                       (C) Copyright 2011
 *                Universitat Autonoma de Barcelona (UAB)
 *  Agencia Estatal Consejo Superior de Investigaciones Cientificas (CSIC)
 *                       All Rights Reserved
 *
 ***************************************************************************/

/*
 *  magchain.cpp
 *  MagChain
 *  Version: 24/11/2011
 * 
 */

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <cstdlib>
#include <time.h>
#include <math.h>
#include <iomanip>
#include <vector>
#include "aggregate.h"

using namespace std;
#define pi 3.14159265

#define _DIR_NPARTICLES "nparticles"
#define _DIR_SIMBOX "sbox"
#define _DIR_TIMESTEP "timestep"
#define _DIR_ATTRADIUS "attradius"
#define _DIR_RESTART_DATA "read_restart"
#define _DIR_EQUILIBRATION "equil"
#define _DIR_RUN "run"

#define _EVERY_STATS "stats_every"
#define _EVERY_TRAJ "traj_every"
#define _EVERY_RESTART "restart_every"
#define _EVERY_HIST "hist_every"

#define _FILE_STATS "stats.txt"
#define _FILE_LOG "logfile.txt"
#define _FILE_TRAJ "trajectoryCG.xyz"
#define _FILE_HISTOGRAM "histogram.txt"


void minim(double& dx, double& dy, double& dz);
void minimz(double& dz);
void cpc(double& x, double& y, double& z);
double ran2(long *idum);
double rangauss(double x, double y);


float D_ER_par(float l, float w);
float D_ER_per(float l, float w);


void brownian(CAggregate &aggregate);
void collision(CAggregate* aggregate1, CAggregate* aggregate1old, CAggregate* agregat2, bool& flagcol, double& dx, double& dy, double& dz, double& gruix);
void collision0(CAggregate* aggregate1, CAggregate* aggregate1old, CAggregate* agregat2);

void aggregation(vector <CAggregate> &aggregate, CAggregate* agregatnold, int n, int m, double& dx, double& dy, double& dz, double& gruix);

bool init_system(ifstream& infile, vector <CAggregate> &a, vector <CBead> &b);
void init_config_random(int nparts, vector <CAggregate> &aggregate, vector <CBead> &bead, double boxx, double boxy, double boxz);
bool read_attradius(string filename, int nd);
bool read_restart(string filename, vector <CAggregate> &a, vector <CBead> &b, vector <double> &attr);
void write_restart(vector <CAggregate> &a, int ts);
void write_xyz_cg(vector <CAggregate> &aggregate, int ts, ostream &out, ostream &log);

///// Global VARIABLES Declarations /////
string notice;
string version = " MagChain 24Nov2011";
string web = " web: www.icmab.es/softmattertheory/";
string agree = " Please include this reference in published work using MagChain:";
string ref = " arXiv:1111.5784 [cond-mat.soft]";

int nparts;
double boxx, boxy, boxz, dt;
vector <double> attradius;
long int vseed2, vseed3, vseed4;

int every_stats;
int every_traj;
int every_restart;
int every_hist;
unsigned long maxbin_hist;

double limit_equil, limit_run;

vector <double> histogram;

ofstream logfile;
ofstream statsfile;
ofstream trajfile;
ofstream histogramfile;

// CHRONOMETER
time_t start, end;
double elapsed_time;


// ------------------------------------------------
//
//	MAIN
//
// ------------------------------------------------
int main(int argc, char * const argv[]){

    notice += "*******************************************************************\n";
    notice += version + "\n";
    notice += web + "\n";
    notice += agree + "\n";
    notice += ref + "\n";
    notice += "*******************************************************************\n";
	
    cout << notice << endl;
	
	string name;
	name = _FILE_STATS; statsfile.open(name.c_str());
	name = _FILE_TRAJ; trajfile.open(name.c_str());
	name = _FILE_LOG; logfile.open(name.c_str());
	name = _FILE_HISTOGRAM; histogramfile.open(name.c_str());

	vector<CAggregate> aggregate;
	vector<CBead> bead;
				
	char *infilename = argv[1];

	ifstream infile;

	if (argc != 1){
	
		infile.open(infilename);
		if (!infile.is_open()){ cerr << "error: unable to open infile " << argv[1] << ".\n" << endl; exit(0); }
		if (!init_system(infile,aggregate,bead)){ cerr << "error: invalid list of directives in input file." << endl; exit(0); }
	
	} else { cerr << "error: invalid number of arguments. An input file is required." << endl; exit(0); }

	infile.close();

	// DECLARING AND INITIALIZING LOCAL (MAIN) VARIABLES
	double smean = 0.0;
	double smean1 = 0.0;
	double smean2 = 0.0;
	unsigned int nt = 0; 
	unsigned int ncount = 0;
	bool flagcol;
	double dx, dy, dz, gruix;

	vseed2 = -84567;
	vseed3 = -95465;
	vseed4 = -35489;

	// ------------------------------------------------
	//
	//	END OF INITIALIZATION
	//
	// ------------------------------------------------
	cout << "\nStarting calculation..." << '\n';

	// ------------------------------------------------
	//
	//	EQUILIBRATION STAGE
	//
	// ------------------------------------------------
	time(&start);
	cout << "\nEquilibration for " << limit_equil << " timesteps..." << endl;
	logfile << "\nEquilibration for " << limit_equil << " timesteps..." << endl;

	// Writing the initial position in the trajectory file
	write_xyz_cg(aggregate, nt, trajfile, logfile);

	// Writing restart file
	write_restart(aggregate, nt);


	while(nt < limit_equil){

		//	IMPLEMENTATION OF BROWNIAN DYNAMICS:
		for(int n=0; n<aggregate.size(); n++){
			CAggregate agregatold;
			agregatold = aggregate[n];
			brownian(aggregate[n]);
			ncount = 0;

            for(int m=0; m<aggregate.size(); m++){

                if(m!=n){
                    dx = aggregate[m].xcm - aggregate[n].xcm;
                    dy = aggregate[m].ycm - aggregate[n].ycm;
                    dz = aggregate[m].zcm - aggregate[n].zcm;
                    minim(dx,dy,dz);
                    gruix = (aggregate[m].diameter + aggregate[n].diameter)/2.0;

                    if (dx*dx + dy*dy < gruix*gruix){
                        collision(&aggregate[n], &agregatold, &aggregate[m], flagcol, dx, dy, dz, gruix);
                    }
                }
            }

		}

		nt = nt + 1;

		// WRITES TRAJECTORY FILE:
        if (every_traj!=0){
            if (nt % every_traj == 0) { write_xyz_cg(aggregate, nt, trajfile, logfile);	}
        }
		
        // WRITES RESTART FILE
        if (every_restart!=0){
            if (nt % every_restart == 0) { write_restart(aggregate, nt);}
        }
        
	} // END EQUILIBRATION LOOP

	// CHRONOMETER
	time(&end);
	elapsed_time = difftime (end,start);
	cout << "\nElapsed Equilibration time: " << elapsed_time << " seconds" << endl;
	logfile << "\nElapsed Equilibration time: " << elapsed_time << " seconds" << endl;
	//////////////

	// ------------------------------------------------
	//
	//	PRODUCTION STAGE
	//
	// ------------------------------------------------
	time(&start);
	cout << "\nProduction for " << limit_run << " timesteps..." << endl;
	logfile << "\nProduction for " << limit_run << " timesteps..." << endl;
	nt = 0;

    
	while((nt < limit_run)){

		//	IMPLEMENTATION OF BROWNIAN DYNAMICS:
		for(int n=0; n<aggregate.size(); n++){
			CAggregate agregatold;
			agregatold = aggregate[n];

			brownian(aggregate[n]);
			ncount = 0;

            for(int m=0; m<aggregate.size(); m++){

                if(m!=n){
                    dx = aggregate[m].xcm - aggregate[n].xcm;
                    dy = aggregate[m].ycm - aggregate[n].ycm;
                    dz = aggregate[m].zcm - aggregate[n].zcm;
                    minim(dx,dy,dz);
                    gruix = (aggregate[m].diameter + aggregate[n].diameter)/2.0;

                    if (dx*dx + dy*dy < gruix*gruix){
                        collision(&aggregate[n], &agregatold, &aggregate[m], flagcol, dx, dy, dz, gruix);
                    }
                }
            }


			for(int m=0; m<aggregate.size(); m++){
				if(m != n){
					dx = aggregate[m].xcm - aggregate[n].xcm;
					dy = aggregate[m].ycm - aggregate[n].ycm;
					dz = aggregate[m].zcm - aggregate[n].zcm;
					minim(dx,dy,dz);
					gruix = (aggregate[m].diameter + aggregate[n].diameter)/2.0;

				   if(dx*dx + dy*dy < (*aggregate[n].attradius)*(*aggregate[n].attradius))

					{
					aggregation(aggregate, &agregatold, n, m, dx, dy, dz, gruix);
				   }
				}
			}
		}

	// MOVE TIMESTEP COUNTERS
		nt = nt + 1;

	// ------------------------------------------------
	//
	//	CALCULATION OF RELEVANT MAGNITUDES AND WRITING TO OUTPUT FILES 
	//
	// ------------------------------------------------
		
	// CALCULATES THE STATISTICS
	if (nt % every_stats == 0){

		//	CALCULATION OF <S>:
		smean1 = 0.0;
		smean2 = 0.0;
		
		for(int n=0; n < aggregate.size(); n++){ smean1 = smean1 + aggregate[n].size; }
		for(int n=0; n < aggregate.size(); n++){ smean2 = smean2 + aggregate[n].size*aggregate[n].size; }

		smean = smean2/smean1;

		//	CALCULATION OF <N>:
		smean1 = smean1/aggregate.size();

		// CALCULATES THE ELAPSED TIME
		time(&end);
		elapsed_time = difftime (end,start);
		//////////////


		cout.setf(ios::fixed,ios::floatfield);
		cout << "timestep:" << setprecision(0) << nt << "; A:" << aggregate.size() << "; N:" << setprecision(5) << smean1 << "; S:"  << smean << "; ET:" << elapsed_time << ";" << endl;
		logfile.setf(ios::fixed,ios::floatfield);
		logfile << "timestep:" << setprecision(0) << nt << "; A:" << aggregate.size() << "; N:" << setprecision(5) << smean1 << "; S:"  << smean << "; ET:" << elapsed_time << ";" << endl;
		
		// WRITES TO STATS FILE:
		
		statsfile.setf(ios::fixed,ios::floatfield);
		statsfile << setprecision(0) << nt << " " << aggregate.size() << " " << setprecision(5) << smean1 << " "  << smean << " " << elapsed_time << endl;

	}

        
    // CALCULATES THE HISTOGRAM
    if (every_hist!=0){
        if(nt % every_hist == 0){
            
            int badcount = 0;
            
            for (int i=0; i<aggregate.size(); i++){
                
                if (aggregate[i].length < histogram.size()){ histogram[int(aggregate[i].length -1.0)] += 1.0; }
                else { logfile << "warning: histrogram size too small!" << endl; badcount++; }
                                
            }
            
            histogramfile << nt  << " " << aggregate.size() << " ";	
            for (int i=0; i<histogram.size(); i++){
                
                histogramfile << " " << histogram[i];
                histogram[i] = 0.0;
                
            }
            
            histogramfile << endl;
            
            if (badcount!=0) { logfile << "warning: truncated histogram at timestep" << nt << ". Number of aggregates skipped: " << badcount << endl; }
	
        }
    }

        
		
	// WRITES TRAJECTORY FILE:
    if (every_traj!=0){
        if (nt % every_traj == 0) { 

            write_xyz_cg(aggregate, nt, trajfile, logfile);

        }
    }
        
	// WRITES RESTART FILES
        if (every_restart!=0){    
            if (nt % every_restart == 0) { write_restart(aggregate, nt);}
        }
        
	}// END PRODUCTION LOOP

    write_restart(aggregate, nt);//last configuration

	// CHRONOMETER: Final Elapsed Time
	time(&end);
	elapsed_time = difftime (end,start);
	cout << "\nElapsed Production time: " << elapsed_time << " seconds" << endl;
	logfile << "\nElapsed Production time: " << elapsed_time << " seconds" << endl;
	//////////////


	logfile.close();
	statsfile.close();
	trajfile.close();
	histogramfile.close();
	return 0;

}

// ------------------------------------------------------------------
//
//	EXTERNAL FUNCTIONS
//
// ------------------------------------------------------------------


bool init_system(ifstream& infile, vector <CAggregate> &a, vector <CBead> &b)
// System initialization: read input script and initializes all system variables.
{

	bool f_restart = false;
	
	string line;
	stringstream ss;
	vector <string> parameter;
	string par;
	string restart_file;

	cout << "\nInitializing system...\n";

// INITIALIZE DEFAULT VARIABLES

	every_traj = 0;
	every_stats = 1;
	every_restart = 0;	// ONLY FIRST AND LAST CONFIGURATIONS
	every_hist = 0;
	maxbin_hist = 300;

	limit_equil = 0.0;
	limit_run = 0.0;
	
   	bool f_attradius = false;
    
    cout << "Setting default attradius to 0." << endl;
    
    attradius.push_back(0.0); f_attradius = true;


	while (getline(infile,line)){	

		stringstream ss(line);
		string directive;
		ss >> directive;

		// Discard empty lines and lines starting with # character
		if (directive.substr(0,1).compare("#") != 0 && directive.substr(0,1).compare(" ") != 0){			

			ss >> par;
			
			while (!par.empty()){

				parameter.push_back(par);
				par.clear();
				ss >> par;

			}

// execute directive RESTART_DATA
			if (directive.compare(_DIR_RESTART_DATA) == 0){
			
				if ( parameter.size() == 1 ) {
	
					restart_file = parameter[0]; 
					read_restart(restart_file, a, b,attradius); f_restart = true;
			
				} else { cerr << "error: invalid _DIR_RESTART_DATA!" << endl; exit(0);}
			}

		}
	
	parameter.clear();

	}

// rewind infile
	infile.clear();
	infile.seekg(0,ios::beg); 


// Overwritable variables

	vector <double> old_attradius;

// Control variables
	bool f_nparticles = false;
	bool f_simbox = false;
	bool f_timestep = false;


	bool f_every_stats = false;
	bool f_every_traj = false;
	bool f_every_restart = false;
	bool f_every_hist = false;
	
	bool f_limit_equil = false;
	bool f_limit_run = false;
    
//	cout << "reading directives...\n" << endl;
	
	while (getline(infile,line)){	

		stringstream ss(line);      
		string directive;
		ss >> directive;

		// Discard empty lines and lines starting with # character
		if (directive.substr(0,1).compare("#") != 0 && directive.substr(0,1).compare(" ") != 0){			

			ss >> par;
			
			while (!par.empty()){

				parameter.push_back(par);
				par.clear();
				ss >> par;

			}

// execute directive nparticles
			if (directive.compare(_DIR_NPARTICLES) == 0 && !f_restart){
			
				if ( parameter.size() == 1 ) {
	
					nparts = atoi(parameter[0].c_str()); f_nparticles = true;
						
				} else { cerr << "error: invalid _DIR_NPARTICLES!" << endl; exit(0);}
			}
			
// execute directive simulation box
			if (directive.compare(_DIR_SIMBOX) == 0 && !f_simbox){ 

				if ( parameter.size() == 3 ) {
	
					boxx = atof(parameter[0].c_str());
					boxy = atof(parameter[1].c_str());
					boxz = atof(parameter[2].c_str());
					f_simbox = true;
					
				} else { cerr << "error: invalid _DIR_SIMBOX!" << endl; exit(0);}
			}
			
// execute directive timestep
			if (directive.compare(_DIR_TIMESTEP) == 0){


                
                    if ( parameter.size() == 1 ) {
                        
                        dt = atof(parameter[0].c_str()); f_timestep = true;

                        if (f_restart){ cout << "Overwritting timestep to " << dt << endl; }
                        
                    } else { cerr << "error: invalid _DIR_TIMESTEP!" << endl; exit(0);}
            }

// execute directive attradius
			if (directive.compare(_DIR_ATTRADIUS) == 0){
				
				attradius.clear();
				
				if (parameter.size() == 2) {

					string filename = parameter[0].c_str();
					int n_attradius = atoi(parameter[1].c_str());

                    if (f_restart){ cout << "Overwritting attraction radius values from " << filename << " (" << n_attradius << " values)..." << endl; }
					f_attradius = read_attradius(filename, n_attradius);
				
				} else { cerr << "error: invalid _DIR_ATTR_RADIUS!" << endl; exit(0);}
				
				// Update attraction radius for each aggregate!!
				for (int i=0; i<a.size(); i++){
				
					a[i].UpdateAttradius(attradius);
				
				}
			}

// execute directive stats_every
			if (directive.compare(_EVERY_STATS) == 0){

				if ( parameter.size() == 1 ) {
	
					every_stats = atoi(parameter[0].c_str()); f_every_stats = true;
			
				} else { cerr << "error: invalid _EVERY_STATS!" << endl; exit(0);}
			}

// execute directive traj_every
			if (directive.compare(_EVERY_TRAJ) == 0){

				if ( parameter.size() == 1 ) {
	
					every_traj = atoi(parameter[0].c_str()); f_every_traj = true;
			
				} else { cerr << "error: invalid _EVERY_TRAJ!" << endl; exit(0);}
			}

// execute directive restart_every
			if (directive.compare(_EVERY_RESTART) == 0){

				if ( parameter.size() == 1 ) {
	
					every_restart = atoi(parameter[0].c_str()); f_every_restart = true;
			
				} else { cerr << "error: invalid _EVERY_RESTART!" << endl; exit(0);}
			}

            // execute directive hist_every
			if (directive.compare(_EVERY_HIST) == 0){            
                if ( parameter.size() == 1 ) {	every_hist = atoi(parameter[0].c_str()); f_every_hist = true;}
                else if ( parameter.size() == 2 ) { every_hist = atoi(parameter[0].c_str());  maxbin_hist = atoi(parameter[1].c_str()); f_every_hist = true;}
                else { cerr << "error: invalid _EVERY_HIST!" << endl; exit(0);}

                for (int i=0; i<maxbin_hist; i++){ histogram.push_back(0); }
            } 

            
// execute directive equilibration
			if (directive.compare(_DIR_EQUILIBRATION) == 0){

				if ( parameter.size() == 1 ) {
	
					limit_equil = atof(parameter[0].c_str()); f_limit_equil = true;
			
				} else { cerr << "error: invalid _DIR_EQUILIBRATION!" << endl; exit(0);}
			}

// execute directive run
			if (directive.compare(_DIR_RUN) == 0){

				if ( parameter.size() == 1 ) {
	
                    limit_run = atof(parameter[0].c_str()); f_limit_run = true;
			
				} else { cerr << "error: invalid _DIR_RUN!" << endl; exit(0);}
			}

        }
	// clear stream variable
	parameter.clear();

	}

// ONLY EQUILIBRATION OR PRODUCTION RUN

	if (f_limit_equil) {limit_run = 0;}
	if (f_limit_run) {limit_equil = 0;}

	
// CHECKS INITIALIZATION
    
    if (!f_restart){
        
        if (!f_nparticles){ cerr << "error: invalid number of particles!" << endl; exit(0);}
        if (!f_simbox){ cerr << "error: invalid simulation box!" << endl; exit(0);}
        if (!f_timestep){ cerr << "error: invalid timestep!" << endl; exit(0);}        

        // INITIALIZES AGGREGATES/BEAD AT RANDOM POSITIONS
        init_config_random(nparts,a, b, boxx, boxy, boxz);
        
    }
    
    
    bool init = false;

    // New Equilibration
	init = (f_nparticles && f_simbox && f_timestep && f_limit_equil && !f_restart);
    // New Production
    init = init || (f_nparticles && f_simbox && f_timestep && f_limit_run && f_attradius && !f_restart);
    // Restart equilibration
    init = init || (f_restart && f_limit_equil);
    // Restart production
    init = init || (f_restart && f_attradius && f_limit_run);

    //IF INITIALIZATION FAILS
	if (!init){ a.clear(); b.clear(); attradius.clear(); cout << "error: Bad system initialization!" << endl; exit(0); } else { 

    if (f_limit_run && attradius[0] == 0.0){cout << "error: Null attraction radius during production run" << endl; exit(0);}
        
        // ...ELSE...
        // INITIALIZES OUTPUTFILE HEADERS
        statsfile << "#" << version << endl;
        statsfile << "#timestep" << "\tA" << "\t<N>" << "\t<S>" << "\tET [s]" << endl;
        logfile << notice << endl;
        
        histogramfile << "#" << version << endl;
        histogramfile << "#timestep n-Chains";
        for (int i=0; i<histogram.size(); i++){ histogramfile << " size" << i+1 ; }
        histogramfile << endl;

        //counts the total number of particles
        unsigned long tnp = 0;
        for (int i=0; i<a.size(); i++){
         
            tnp += a[i].size;
            
        }    
        
        
		// WRITES SUMMARY TO LOGFILE:
		
		if (f_restart) { logfile << "restart data file used: " << restart_file << '\n';  }
		
		logfile << "initial number of particles: " << tnp << '\n';
		logfile << "initial number of objects: " << nparts << '\n';
		logfile << "timestep: " << dt << '\n';
		logfile << "simulation box: " << boxx << " " << boxy << " " << boxz << '\n';


		double volume = boxx*boxy*boxz;
		double fparts = double(tnp)/volume;

		logfile << "\nvolume (length unit: diameter colloids) = " << volume << '\n';
		logfile << "number density (Num parts./Volume) = " << fparts << '\n';
		logfile << "volume fraction = " << (4.0/3.0)*pi*(0.5*0.5*0.5)*tnp/volume << '\n' << endl;

		if (old_attradius != attradius && f_restart && f_attradius){
		
			logfile << "new attraction radius values found:\n";
			logfile << "old values (from restart file):\n";
			for (int i=0; i<old_attradius.size(); i++){	logfile << "(" <<  i+1 << ") " << old_attradius[i] << "\n";	}
			logfile << "\nnew values:\n";
			for (int i=0; i<attradius.size(); i++){	logfile << "(" <<  i+1 << ") " << attradius[i] << "\n";	}	
		
		} else {
		
			logfile << "attraction radius values:\n";
			for (int i=0; i<attradius.size(); i++){	logfile << "(" <<  i+1 << ") " << attradius[i] << "\n";	}	
		
		}
		
		logfile << '\n' << endl;
		logfile << "histogram size " << maxbin_hist << endl;
		logfile << "save stats every " << every_stats << " timestep(s)\n";
		logfile << "save trajectory every " << every_traj << " timestep(s)\n";
		logfile << "save restart file every " << every_restart << " timestep(s)\n";
		logfile << "save histogram every " << every_hist << " timestep(s)\n";
		
		logfile << endl;		
		if (f_limit_equil) { logfile << "equilibration limit: " << limit_equil << '\n'; }
		if (f_limit_run) { logfile << "production limit: " << limit_run << '\n'; }
						
		logfile << "\n##### end initialization #####" << endl;

	}
	
    cout << "Initialization finished.\n";
    
	return init;

}

void init_config_random(int nparts, vector <CAggregate> &aggregate, vector <CBead> &bead, double boxx, double boxy, double boxz)
// Generates an initial configuration with random positions.
{

	long int seed1 = -54;
	long int seed2 = -543;
	long int seed3 = -5432;
	double x,y,z;
	bool overlap = false;

	if (aggregate.size() != 0 || bead.size() != 0) { cerr << "error: non empty aggregate/bead array(s)!" << endl; exit(0); }
	if (attradius.size() == 0) { cerr << "error: attradius must be initialized!" << endl; exit(0); }
	if (boxx == 0 || boxy == 0 || boxz == 0) { cerr << "error: invalid simulation box!" << endl; exit(0); }
	aggregate.reserve(nparts);
	bead.reserve(nparts);
	
	// Spherical particles of diameter = 1.
	cout << "Initializing random positions for " << nparts << " particles in a " << boxx << "x" << boxy << "x" << boxz << " simulations box...";  
	
	for (int n=0; n<nparts; n++){

		do {

			overlap = false;
			x = ran2(&seed1)*boxx;
			y = ran2(&seed2)*boxy;
			z = ran2(&seed3)*boxz;

			for (int i=0; i<aggregate.size(); i++){

				double dx = aggregate[i].xcm - x;
				double dy = aggregate[i].ycm - y;
				double dz = aggregate[i].zcm - z;
				minim(dx,dy,dz);
				if ((dx*dx + dy*dy + dz*dz) <= 2.0) {overlap = true;}
			
			}
		// refuse coordinates which overlap with particles already in the simulation box.			
		} while (overlap);

		// Once we have found coordinates which do not overlap...
		CBead b(n,0.0,0.0,0.0);
		bead.push_back(b);
		
		CAggregate a(n, x, y, z, 1.0, 1.0, 1.0, attradius[0], &bead.back());
		aggregate.push_back(a);

	}

	cout << "done!" << endl;

}

bool read_attradius(string filename, int nd)
// Reads attraction radius values for chain-like aggregates from a data file. The file have to be column-formatted.
// First column: number of beads in a chain aggregate. Second column: radius of the attraction zone in particle diameters.
{


	bool succed = true;
	string line;
	stringstream ss;
	vector <string> parameter;
	string par;

	int nc;		// number of colloids in aggregate
	double radius;	// radius

	cout << "\nreading attraction radius' file..." << endl;
	
	ifstream attrfile;
	attrfile.open(filename.c_str());
	
	if (!attrfile.is_open()){ cerr << "error: unable to open " << filename << " !"; exit(0); } else {
	
		getline(attrfile,line);		//skips first line
	
		while (getline(attrfile,line) && attradius.size() < nd){	

			stringstream ss(line);		//reads first line to a stream
			string token;				//string to parse ss stream

			ss >> token;
			nc = atoi(token.c_str());
			ss >> token;		
			radius = atof(token.c_str());
			attradius.push_back(radius);		//GLOBAL variable
		}
	}

	attrfile.close();
	if (attradius.size() < nd) {cerr << "error: insufficient number of attraction radius in " << filename << " !"; exit(0); }

	cout << attradius.size() << " values readed from " << filename << endl;
	
	for (int i=0; i<attradius.size(); i++){
	
		cout << "(" <<  i+1 << ") " << attradius[i] << "\n";
	
	}

	cout << nd << " values will be used." << endl;

	return succed;

}

bool read_restart(string filename, vector <CAggregate> &a, vector <CBead> &b, vector <double> &attr)
// Reads restart file.
{

	string line;
	string token;
	stringstream ss;
	bool succed = false;
	int maxatoken = 10;
	int maxbtoken = 4;
	ifstream infile;
	infile.open(filename.c_str());
	
	if (!infile.is_open()){ cerr << "error: unable to open restart file " << filename; succed = false; exit(0); } else {
		
		succed = true;
		cout << "Reading restart file " << filename;
		a.clear();
		b.clear();
		a.reserve(nparts);
		b.reserve(nparts);	
		
		//TITLE
		getline(infile,line);
		getline(infile,line);
		getline(infile,line);

		//NUMBER OF PARTICLES
		getline(infile,line);
		getline(infile,line);
		ss << line; ss >> token;
		nparts = atoi(token.c_str());
		ss.clear();
		getline(infile,line);

		//SIMULATION BOX
		getline(infile,line);
		getline(infile,line);
		ss << line;
		ss >> token;
		boxx = atof(token.c_str());token.clear();
		ss >> token;
		boxy = atof(token.c_str());token.clear();
		ss >> token;
		boxz = atof(token.c_str());token.clear();
		ss.clear();
		getline(infile,line);
		
		//TIMESTEP
		getline(infile,line);
		getline(infile,line);
		ss << line; ss >> token;
		dt = atof(token.c_str());
		ss.clear();
		getline(infile,line);

		//ATTRACTION RADIUS		
		getline(infile,line);
		getline(infile,line);
		ss << line; ss >> token;
		int nattr = atoi(token.c_str());
		ss.clear();

		attr.clear();
		attr.reserve(nattr);
		
		getline(infile,line);
		ss << line;
		
		for (int i=0; i<nattr; i++){
		
			ss >> token;
			attr.push_back(atof(token.c_str()));
		
		}
		ss.clear();	
		getline(infile,line);
		
		//AGGREGATES/BEADS
		getline(infile,line);		
		if (a.size() != 0 || b.size() != 0) { cerr << "error: non empty aggregate/bead array(s)!" << endl; exit(0); }
		a.reserve(nparts);
		b.reserve(nparts);
		
		while (getline(infile,line)){	

			ss << line;
			vector <string> apar;
			string token;

			// Reads an aggregate
			for (int i=0; i<maxatoken; i++){
			
				ss >> token;
				apar.push_back(token);

			}

			// THAT MUST CHECK consistency in attradius corresponds with vector value!! TODO
			int l = int(ceil(atof(apar[5].c_str())));
			
			// Add aggregate in vector container
			//	format in restart: (int)id, (int)#beads, (double)xcm, (double)ycm, (double)zcm, (double)size, (double)length, (double)diameter, (double) attradius
			CAggregate auxa = CAggregate(atoi(apar[0].c_str()), atof(apar[2].c_str()), atof(apar[3].c_str()), atof(apar[4].c_str()), atof(apar[5].c_str()), atof(apar[6].c_str()), atof(apar[7].c_str()), attr[l],NULL);
			a.push_back(auxa);
			ss.clear();

			for (int i=0; i<atoi(apar[1].c_str()); i++){

				getline(infile,line);
				ss << line;
				vector <string> bpar;

					// Reads each bead for this aggregate
					for (int j=0; j<maxbtoken; j++){
					
						ss >> token;
						bpar.push_back(token);

					}
					
				CBead auxb(atoi(bpar[0].c_str()), atof(bpar[1].c_str()), atof(bpar[2].c_str()), atof(bpar[3].c_str()));
				b.push_back(auxb);
				a[int(a.size()-1)].AddBead(&b[int(b.size()-1)]);
				bpar.clear();
				ss.clear();
			
			}
			
			apar.clear();

		}

		infile.close();

	}


	cout << " done!" << endl;
	return succed;

}

void write_restart(vector <CAggregate> &a, int ts)
// Writes restart file.
{

	stringstream ssname;
	string name;
	ofstream outfile;
	bool succed = false;
	
	ssname << "restart." << ts ;
	name = ssname.str();
	outfile.open(name.c_str());
	
	if (!outfile.is_open()) { cerr << "warning: unable to open restart file"; succed = false;} else {
	
		succed = true;
		cout << "** writing restart file " << name ;

		outfile << "RESTART FILE" << endl;
		outfile << "Timestep: " << ts << endl;
		outfile << endl;

		outfile << "NUMBER OF PARTICLES" << endl;
		outfile << a.size() << endl;
		outfile << endl;

		outfile << "SIMULATION BOX" << endl;
		outfile << boxx << " " << boxy << " " << boxz << endl;
		outfile << endl;
		
		outfile << "TIMESTEP" << endl;
		outfile << dt << endl;
		outfile << endl;
		
		outfile << "ATTRACTION RADIUS" << endl;
		outfile << attradius.size() << endl;
		for (int i=0; i<attradius.size(); i++){ outfile << attradius[i] << " "; }
		outfile << endl;
		outfile << endl;
		
		outfile << "AGGREGATES/BEADS" << endl;
		for (int i=0; i<a.size(); i++){
		
			a[i].Write2restart(outfile);
		
		}
		
		outfile.close();
	}
	
	cout << " done!" << endl;

}

void write_xyz_cg(vector <CAggregate> &aggregate, int ts, ostream &out, ostream &log)
// Writes system configuration in a xyz format (Coarse Grain representation)
{

//	cout << "** writing trajectory (xyz-CG) timestep " << ts << "...";

	out << aggregate.size() << endl;					// number of aggregates
	out << boxx << " " << boxy << " " << boxz << endl;	//simulation box coordinates

	for(int n=0; n < aggregate.size(); n++){ aggregate[n].Write2xyzCG(out); }

//	cout << " done!" << endl;
		
}

void brownian(CAggregate &aggregate)
// Applies brownian dynamics and external force to each aggregate in the system.
{

//	Implements brownian dynamics algorithm to model 
//	the movement of the center of mass of the aggregates.

// Elongated ROD diffusion model

	aggregate.xcm = aggregate.xcm + pow(2.0*dt*D_ER_per(aggregate.length, aggregate.diameter), 0.5)*rangauss(ran2(&vseed2),ran2(&vseed2));
	aggregate.ycm = aggregate.ycm + pow(2.0*dt*D_ER_per(aggregate.length, aggregate.diameter), 0.5)*rangauss(ran2(&vseed3),ran2(&vseed3));
	aggregate.zcm = aggregate.zcm + pow(2.0*dt*D_ER_par(aggregate.length, aggregate.diameter), 0.5)*rangauss(ran2(&vseed4),ran2(&vseed4));
	
	// Applying periodical boundary conditions to the new coordinates:
	cpc(aggregate.xcm, aggregate.ycm, aggregate.zcm);
	
}

void collision0(CAggregate* aggregate1, CAggregate* aggregate1old, CAggregate* agregat2)
// Re-positioning a particle if it overlaps with another particle. The particle stops in contact according to the real treajectory.
{
double cosA, sinA,sinC,cosC,ltrav, dx10, dy10, dz10, dx20, dy20, dz20, gruix, dist1, dist2;

	// We suppose that an aggregate which collides with another one stops when they are in contact. 
	// To know the length traveled, ltrav, by the 2D-particle (in the XY plane) until contact we "solve" the triangle 
	// formed by the initial and final positions of the moving aggregate and the position of the 
	// aggregate with which it collides. For that we use the law of sines and that the angles of a triangle add to 180º.

		// Calculation of the sides of the triangle
		gruix = (aggregate1->diameter + agregat2->diameter)/2.0;
		dx10 = agregat2->xcm - aggregate1old->xcm;
		dy10 = agregat2->ycm - aggregate1old->ycm;
		dz10 = agregat2->zcm - aggregate1old->zcm;
		dx20 = aggregate1->xcm - aggregate1old->xcm;
		dy20 = aggregate1->ycm - aggregate1old->ycm;
		dz20 = aggregate1->zcm - aggregate1old->zcm;
		minim(dx10,dy10, dz10);
		minim(dx20,dy20, dz20);
		
		dist1 = pow(dx10*dx10+dy10*dy10,0.5);
		dist2 = pow(dx20*dx20+dy20*dy20,0.5);
		if((dx20 == 0.0)&&(dy20 == 0.0)){
			cosA = 0.0;
		}
		else{
			cosA = fabs((dx10*dx20 + dy10*dy20)/(dist1*dist2));
		}
		sinA = pow(1.0-cosA*cosA,0.5);
		sinC = dist1*sinA/gruix;
		cosC = -pow(1.0-sinC*sinC,0.5);
		ltrav = gruix*cosC + cosA*dist1;

		if((ltrav < 0.0001)&&(ltrav>0.0)){ltrav = 0.0;}

		if((dx20==0.0)&&(dy20==0.0)){
			aggregate1->xcm = aggregate1old->xcm;
			aggregate1->ycm = aggregate1old->ycm;
			aggregate1->zcm = aggregate1old->zcm;
		}
		else if((dist1 < gruix)||(ltrav < 0)){
			aggregate1-> xcm = aggregate1old->xcm - 1.001*(dx10/dist1)*(gruix -dist1);
			aggregate1-> ycm = aggregate1old->ycm - 1.001*(dy10/dist1)*(gruix -dist1);
			aggregate1->zcm = aggregate1old->zcm;

		}
		else{
		// New coordinates of the moving aggregate after the collision
			aggregate1->xcm = aggregate1old->xcm + 0.999*(dx20/dist2)*ltrav;;
			aggregate1->ycm = aggregate1old->ycm + 0.999*(dy20/dist2)*ltrav;
			aggregate1->zcm = aggregate1old->zcm + 0.999*(dz20/dist2)*ltrav;
			cpc(aggregate1->xcm, aggregate1->ycm, aggregate1->zcm);
		}

}

void collision(CAggregate* aggregate1, CAggregate* aggregate1old, CAggregate* agregat2, bool& flagcol, double& dx, double& dy, double& dz, double& gruix)
// Evaluates if exists a collision between aggregates.
{

double distx, disty, dz1, dz1old, dz1b, deltaz, dx20, dy20, dz20, dz2;
//	We suppose that an aggregate which collides with another one stops when they are in contact. 
//	To know the length traveled, ltrav, by the 2D-particle (in the XY plane) until contact we "solve" the triangle 
//	formed by the initial and final positions of the moving aggregate and the position of the 
//	aggregate with which it collides. For that we use the law of sines and that the angles of a triangle add to 180º.


	// Calculation of different distances between the two aggregates to establish the condition of collision

	dx20 = aggregate1->xcm - aggregate1old->xcm;
	dy20 = aggregate1->ycm - aggregate1old->ycm;
	dz20 = aggregate1->zcm - aggregate1old->zcm;
	dz2 = agregat2->zcm - aggregate1old->zcm;

	minim(dx20,dy20, dz20);
	minimz(dz2);

	dz1 = (dz + agregat2->length/2.0 + aggregate1->length/2.0);  
	dz1old = (dz2 + agregat2->length/2.0 + aggregate1old->length/2.0);  
	dz1b= (-dz + aggregate1->length/2.0 + agregat2->length/2.0);  

	distx = dx*dx;
	disty = dy*dy;

//	VERTICAL COLLISION:
//	Condition for collision: if the distance in the XY plane between the two aggregates is shorter than the  
//	sum of their radiuses, there is overlap in the vertical z-direction and there was not overlap in the previous 
//	timestep.
//

	if((dz1>0.0)&&(dz1 < aggregate1->length+agregat2->length)&&((dz1old<0.0)||(dz1old > aggregate1->length+agregat2->length))){

		flagcol = true;
		deltaz = min(fabs(dz1old), fabs(dz1old -(aggregate1->length+agregat2->length)));
		aggregate1->xcm = aggregate1old->xcm + (dx20/fabs(dz20))*deltaz;
		aggregate1->ycm = aggregate1old->ycm + (dy20/fabs(dz20))*deltaz;
		if(dz20 < 0.0){
			aggregate1->zcm = aggregate1old->zcm - deltaz+0.001;
		}
		else if(dz20 > 0.0){
			aggregate1->zcm = aggregate1old->zcm + deltaz-0.001;
		}
	}


 		else if ((dz1>0.0)&&(dz1 < aggregate1->length+agregat2->length))

			{
				flagcol = true;
				// Call to the function which implements the collision on the moving aggregate
				collision0(aggregate1, aggregate1old, agregat2);

			}
} 

void aggregation(vector <CAggregate> &aggregate, CAggregate* agregatnold, int n, int m, double& dx, double& dy, double& dz, double& gruix)
// Evaluates if an aggregation event has occured.
{

	double distx, disty, dz1, dz1b;
	double zmcmold, zncmold;

	dz1b = (dz + aggregate[m].length/2.0 + aggregate[n].length/2.0);
	dz1= (-dz + aggregate[n].length/2.0 + aggregate[m].length/2.0);

	distx = dx*dx;
	disty = dy*dy;

	double distz2botn = (dz1b+*aggregate[n].attradius)*(dz1b+*aggregate[n].attradius);
	double distz2topn = (dz1+*aggregate[n].attradius)*(dz1+*aggregate[n].attradius);
	double attradiusn2 = (*aggregate[n].attradius)*(*aggregate[n].attradius);


		//	TIP TO TIP AGGREGATION
	// Condition for aggregation: the top (bottom) of the second aggregate is within a sphere of diameter "attr_diameter" 
	// tangent to the the bottom (top) of the first aggregate

	if( ((distx + disty + distz2topn < attradiusn2)&&(dz1<0.0))  || ((distx + disty + distz2botn < attradiusn2 )&&(dz1b<0.0)) )  {

	
		// If there is tip-to-tip aggregation between two aggregates, one of them becomes an aggregate located at the CM 
		// with an extension which is the sum of the extensions of the two original aggregates. The second one (the one that moved)
		// is eliminated.
		
		zmcmold = aggregate[m].zcm;
		zncmold = aggregate[n].zcm;
		
		// Calculating the center of mass of the new aggregate
		aggregate[m].xcm = (aggregate[m].size*(aggregate[n].xcm + dx) + aggregate[n].size*aggregate[n].xcm)/(aggregate[m].size+aggregate[n].size);
		aggregate[m].ycm = (aggregate[m].size*(aggregate[n].ycm + dy) + aggregate[n].size*aggregate[n].ycm)/(aggregate[m].size+aggregate[n].size);
		aggregate[m].zcm = (aggregate[m].size*(aggregate[n].zcm + dz) + aggregate[n].size*aggregate[n].zcm)/(aggregate[m].size+aggregate[n].size);
		cpc(aggregate[m].xcm, aggregate[m].ycm, aggregate[m].zcm);

		// Updating the size, length and diameter of the new aggregate (and its attradius)
		aggregate[m].size = aggregate[n].size + aggregate[m].size;
		aggregate[m].length = aggregate[n].length + aggregate[m].length;
		aggregate[m].diameter = pow(aggregate[m].size/aggregate[m].length,0.5);
		aggregate[m].UpdateAttradius(attradius);

		aggregate.erase (aggregate.begin()+n);

	} 

}

float D_ER_per(float l, float w)
// Diffusion coefficient for an ELONGATED ROD (prolate spheroiod limit) in direction PERPENDICULAR to its symmetry axis.
{
float D;
if(l == 1.0){
	D = 1.0;
}
else if(l == 2.0){
	D = 0.5 + (1.0/(8.0))*(log(6.0/w) + 0.5);
}
else if(l > 2.0){
	D = (3.0/(4*l))*(log(2.0*l/w) + 0.5);
}
else{
	D = -1.0;
	cout << "Error: el cluster seleccionat té l= " << l << " i w= " << w << endl;
}

return (D);

}

float D_ER_par(float l, float w)
// Diffusion coefficient for an ELONGATED ROD (prolate spheroiod limit) in direction PARALLEL to its symmetry axis.
{
float D;
if(l == 1.0){
	D = 1.0;
}
else if(l == 2){
	D = 0.5 + (1.0/(4.0))*(log(6.0/w) - 0.5);
}
else if(l > 2.0){
	D = (3.0/(2*l))*(log(2.0*l/w) - 0.5);
}
else{
	D = -1.0;
	cout << "Error: el cluster seleccionat té l= " << l << " i w= " << w << endl;
}

return (D);

}


void minim(double& dx, double& dy, double& dz)
{
	if (dx > boxx/2.0){
		dx = dx - boxx;
	}
	else if (dx < -boxx/2.0){
		dx = dx + boxx;
	}
	if (dy > boxy/2.0){
		dy = dy - boxy;
	}
	else if (dy < -boxy/2.0){
		dy = dy + boxy;
	}
	if (dz > boxz/2.0){
		dz = dz - boxz;
	}
	else if (dz < -boxz/2.0){
		dz = dz + boxz;
	}

}

void minimz(double& dz)
{
	if (dz > boxz/2.0){
		dz = dz - boxz;
	}
	else if (dz < -boxz/2.0){
		dz = dz + boxz;
	}

}

void cpc(double& x, double& y, double& z)
{
	if (x > boxx){
		x = x - boxx;
	}
	else if (x < 0.0){
		x = x + boxx;
	}
	if (y > boxy){
		y = y - boxy;
	}
	else if (y < 0.0){
		y = y + boxy;
	}
	if (z > boxz){
		z = z - boxz;
	}
	else if (z < 0.0){
		z = z + boxz;
	}

}

double ran2(long *idum)
/*Long period (> 2 × 1018) random number generator of L’Ecuyer with Bays-Durham shuffle
and added safeguards. Returns a uniform random deviate between 0.0 and 1.0 (exclusive of
the endpoint values). Call with *idum a negative integer to initialize; thereafter, do not alter
*idum between successive deviates in a sequence. RNMX should approximate the largest doubleing
value that is less than 1.*/
{
#define IM1 2147483563
#define IM2 2147483399
#define AM (1.0/IM1)
#define IMM1 (IM1-1)
#define IA1 40014
#define IA2 40692
#define IQ1 53668
#define IQ2 52774
#define IR1 12211
#define IR2 3791
#define NTAB 32
#define NDIV (1+IMM1/NTAB)
#define EPS 1.2e-7
#define RNMX (1.0-EPS)
int j;
long k;
static long idum2=123456789;
static long iy=0;
static long iv[NTAB];
double temp;
if (*idum <= 0) { //Initialize.
if (-(*idum) < 1) *idum=1; //Be sure to prevent *idum = 0.
else *idum = -(*idum);
idum2=(*idum);
for (j=NTAB+7;j>=0;j--) { //Load the shuffle table (after 8 warm-ups).
k=(*idum)/IQ1;
*idum=IA1*(*idum-k*IQ1)-k*IR1;
if (*idum < 0) *idum += IM1;
if (j < NTAB) iv[j] = *idum;
}
iy=iv[0];
}
k=(*idum)/IQ1; // Start here when not initializing.
*idum=IA1*(*idum-k*IQ1)-k*IR1; // Compute *idum=(IA1**idum) % IM1 without
if (*idum < 0) *idum += IM1;// overflows by Schrage’s method.
k=idum2/IQ2;
idum2=IA2*(idum2-k*IQ2)-k*IR2;// Compute idum2=(IA2**idum) % IM2 likewise.
if (idum2 < 0) idum2 += IM2;
j=(int)iy/NDIV; // Will be in the range 0..NTAB-1.
iy=iv[j]-idum2; // Here *idum is shuffled, *idum and idum2 are
iv[j] = *idum;// combined to generate output.
if (iy < 1) iy += IMM1;
if ((temp=AM*iy) > RNMX) return RNMX; // Because users don’t expect endpoint values.
else return temp;
} 

double rangauss(double x, double y)
/* Obtains a Gaussian-distributed random number from two given uniformly distributed random numbers "x" and "y" */
{
	double rgauss;
	rgauss = pow(-2.0*log(x), 0.5)*sin(2.0*pi*y);
	return rgauss;
}
