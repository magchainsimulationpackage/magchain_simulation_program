/***************************************************************************
 *
 *                       (C) Copyright 2011
 *                Universitat Autonoma de Barcelona (UAB)
 *  Agencia Estatal Consejo Superior de Investigaciones Cientificas (CSIC)
 *                       All Rights Reserved
 *
 ***************************************************************************/

/*
 *  aggregate.h
 *  MagChain
 *  Version: 16/11/2011
 * 
 */

#include <vector>

using namespace std;


class CBead{

public:

	int id;
	double x,y,z;

	CBead(){}

	CBead(int ii, double xx, double yy, double zz ){
	
		id = ii;
		x = xx;
		y = yy;
		z = zz;

	}
	
	~CBead(){}

};

class CAggregate{

public:

	int name; 
	double size;
	double length;
	double diameter;
	double xcm, ycm,zcm;
	vector <CBead*> bead;
	double *attradius;


	CAggregate(){}

	CAggregate(int i, double x, double y,double z, double s, double l, double d, double &attr, CBead* a){

		name = i;
		xcm = x;
		ycm = y;
		zcm = z;
		size = s;
		length = l;
		diameter = d;
		attradius = &attr;
		
		bead.reserve(20);
		
		if (a != NULL){
			bead.push_back(a);
		}

	}

	~CAggregate(){}


// ------ Member Functions -------------
	void UpdateAttradius(vector <double> &vad);
	void AddBead(CBead*);
	void Write2xyzCG(ostream &out);
	void Write2restart(ostream &out);	
};
