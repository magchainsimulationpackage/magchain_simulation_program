/***************************************************************************
 *
 *                       (C) Copyright 2011
 *                Universitat Autonoma de Barcelona (UAB)
 *  Agencia Estatal Consejo Superior de Investigaciones Cientificas (CSIC)
 *                       All Rights Reserved
 *
 ***************************************************************************/

/*
 *  aggregate.cpp
 *  MagChain
 *  Version: 16/11/2011
 * 
 */

#include <math.h>
#include <iomanip>
#include<iostream>

#include "aggregate.h"

void CAggregate::UpdateAttradius(vector <double> &var){

	if (this->length <= var.size()){

		this->attradius = &var[int(ceil(this->length-1.5))];

	} else {
	
		this->attradius = &var.back();
	
	}

}

void CAggregate::AddBead(CBead* a){
	bead.push_back(a);
}

void CAggregate :: Write2xyzCG(ostream &out){

		out << name << " " << xcm << " "  << ycm << " " << zcm <<  " " << length << " " << diameter <<'\n';

}

void CAggregate :: Write2restart(ostream &out){

	out << fixed << setprecision(6);

	out << name << " " << this->bead.size() << " " << xcm << " " << ycm << " " << zcm << " " << size << " " << length << " " << diameter << " " << (*attradius) << endl; 

	for(int k=0; k<this->bead.size(); k++){

		out << this->bead[k]->id << " " << fixed << setprecision(6) << (this->bead[k]->x) << " " << fixed << setprecision(6) << (this->bead[k]->y) << " " << fixed << setprecision(6) << (this->bead[k]->z) <<'\n';

	}

}
