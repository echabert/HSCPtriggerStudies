#include "inc/TrigEff.h"

#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <stdio.h>
#include <array>
#include <cstdlib>
#include <iomanip>
#include <ctime>
#include <cmath>
#include <fstream>
#include <algorithm>


using namespace std;



TrigEff::TrigEff(int size){
	int ntrig;
	
	if(size>0){
		ntrig = size;
		data = new float[ntrig];
	}
	else{
		ntrig = 0;
		data = 0;
	}
	//cout << "n = " << ntrig << endl;
}



TrigEff::~TrigEff(){ 
	//cout << "data : " << data << endl;
	if(data!=0)
		delete[] data; // supprimer tous les malloc/calloc

}


void TrigEff::Init(int ntrigger,bool *passtrig){
	for(int i=0;i<ntrigger; i++){ 
		triggerpass.push_back(passtrig[i]);
		cout << "bool[" << i << "] = "  << triggerpass[i] << endl;
	}

}

void TrigEff::Load(vector<string> triggerNames,string selection,int error_type){ 
	
	num_corr.resize(triggerNames.size(), vector<double>(triggerNames.size(), 0.0));
	denom_corr.resize(triggerNames.size(), vector<double>(triggerNames.size(), 0.0));
	correlation.resize(triggerNames.size(), vector<double>(triggerNames.size(), 0.0));
	
	num_efficiency.resize(triggerNames.size(), 0.0);
	denom_efficiency.resize(triggerNames.size(), 0.0);
	efficiency.resize(triggerNames.size(), 0.0);
	
	eff_err.resize(triggerNames.size(), 0.0);

	if(error_type == 1 ){
		//cout << " We will use the most general error estimator " << endl;
	}

	this->triggernames = triggerNames;

	//find(triggerNames.begin(), triggerNames.end(), selection) != triggerNames.end();
	//triggerNames.find(selection);
	for(unsigned int curline=0; curline < triggerNames.size();curline++){
		
		if(triggerNames[curline] == selection){
			cout << "Found trigger :" << selection << " in line  " << curline << endl;
			column = curline;
			break;
		}

		
		else if(triggerNames[curline]!=selection && curline==triggerNames.size()-1) {
			cout<<"That name is not an element in this vector"<<'\n';
        	}
	}
	

}




void TrigEff::Fill(vector<bool> passtrig, double obs,double weight){
	bool trig1,trig2,trig3;
	for(int i=0;i< passtrig.size();i++){
		trig1 = passtrig.at(i);
		denom_efficiency[i]+=1;
		if (trig1){
			num_efficiency[i]+=1;
		}
	
		for(int j=0;j< passtrig.size();j++){
			trig2 = passtrig.at(j);
			if(trig1 || trig2){
				denom_corr[i][j]+=1;
			}
			if(trig1 && trig2){
				num_corr[i][j]+=1;
			}
		}
	}
}

void TrigEff::Compute_corr(){

	for(int i=0;i< correlation.size();i++){
		for(int j=0;j< correlation[i].size();j++){
			if(denom_corr[i][j]==0){
				correlation[i][j]=0;
			}
			else{	
			correlation[i][j] = ((num_corr[i][j]*1.0) / denom_corr[i][j]);
			}
		}
	}
}

void TrigEff::Print_corr(){

	for ( int i = 0; i < correlation.size(); i++ ){
   		for ( int j = 0; j < correlation[i].size(); j++ ){
      			cout << correlation[i][j] * 100  << "% ";
   		}
   	cout << endl;
	}
}

void TrigEff::Print_num_corr(){
	
	for ( int i = 0; i < num_corr.size(); i++ ){
   		for ( int j = 0; j < num_corr[i].size(); j++ ){
      			cout << num_corr[i][j] << ' ';
   		}
   	cout << endl;
	}	
}

void TrigEff::Print_denom_corr(){

	for ( int i = 0; i < denom_corr.size(); i++ ){
   		for ( int j = 0; j < denom_corr[i].size(); j++ ){
      			cout << denom_corr[i][j] << ' ';
   		}
   	cout << endl;
	}
}



void TrigEff::Compute_eff()
{
	for(int i=0;i< efficiency.size();i++){
		if(denom_efficiency[i]==0){
			efficiency[i]=0;
		}
		else{	
			efficiency[i] = ((num_efficiency[i]*1.0) / denom_efficiency[i]*1.0);
		}
		
	}
}

void TrigEff::Print_eff(){

	for(int i=0;i< efficiency.size();i++){
		cout << efficiency[i] *100 << "% " << "denom : " << denom_efficiency[i] <<" error : " << eff_err[i] << endl;
	}
}

void TrigEff::Print_spec_eff(int curline){
		cout << "efficiency of this trigger : " << efficiency[curline] *100 << "% " << " error associated : " << eff_err[curline] << endl;
	
}



void TrigEff::Print_num_eff(){
	for ( int i = 0; i < num_efficiency.size(); i++ ){
      		cout << num_efficiency[i] << endl ;
	}	
}


void TrigEff::Print_denom_eff(){
	for ( int i = 0; i < denom_efficiency.size(); i++ ){
      		cout << denom_efficiency[i] << endl ;
	}
}


void TrigEff::Fill_error(){

	for(int i=0;i< eff_err.size();i++){
		eff_err[i]=sqrt((1.0*efficiency[i]*(1.0-efficiency[i]))/1.0*denom_efficiency[i]);	
	}
}



