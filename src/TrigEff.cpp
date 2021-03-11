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


using namespace std;

void TrigEff::Initvectors(int ntrigger){

	num_corr.resize(ntrigger, vector<double>(ntrigger, 0.0));
	denom_corr.resize(ntrigger, vector<double>(ntrigger, 0.0));
	correlation.resize(ntrigger, vector<double>(ntrigger, 0.0));

}




void TrigEff::Init(int ntrigger,bool *passtrig){
	for(int i=0;i<ntrigger; i++){ 
		triggerpass.push_back(passtrig[i]);
		cout << "bool[" << i << "] = "  << triggerpass[i] << endl;
	}

}

/*
void TrigEff::Init(int ntrigger,vector<bool> passtrig){
	for(int i=0;i<ntrigger; i++){ 
		triggerpass.push_back(passtrig[i]);
		cout << "bool[" << i << "] = "  << triggerpass[i] << endl;
	}

}

*/

void TrigEff::noc_corr(){
/*	
	bool trig1,trig2;
	for(int i=0;i<ntrigger;i++){
		trig1 = triggerpass[i];

		for(int j=0;j<ntrigger;j++){
			trig2 = triggerpass[j];
			if(trig1 || trig2){

				numerator[i].push_back(0); // not good syntax, mais vecteur de vecteurs 
				numerator[i][j]+=1;
			}
			if(trig1 && trig2){
				denominator[i].push_back(0);
				denominator[i][j]+=1;
			}

		}
	}

*/
}




void TrigEff::noc_eff()
{
}



void TrigEff::compute_eff(int a)
{
}


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
	cout << "n = " << ntrig << endl;
}



TrigEff::~TrigEff(){ 
	//cout << "data : " << data << endl;
	if(data!=0)
		delete[] data; // supprimer tous les malloc/calloc

}



void TrigEff::Load(vector<string> triggerNames,string selection,int error_type){ 
	this->triggernames = triggerNames;
}



void TrigEff::Fill(vector<bool> passtrig, double obs,double weight){
	//obtient les vecteurs de passtrig
	bool trig1,trig2;
	//num_corr.resize(passtrig.size(), vector<double>(passtrig.size()));
	//cout << "size columns : " << num_corr[].size() << " rows : " << num_corr.size() << endl;
	//cout << "passtrig.size : " << passtrig.size() << endl;
	for(int i=0;i< passtrig.size();i++){
		trig1 = passtrig.at(i);
		//if(i> 660) cout << "i : " << i << endl;
		for(int j=0;j< passtrig.size();j++){
			//cout << "j : " << j << endl;
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


void TrigEff::compute_corr(){

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
	



void TrigEff::get_num_corr(){
	
	for ( int i = 0; i < num_corr.size(); i++ ){
   		for ( int j = 0; j < num_corr[i].size(); j++ ){
      			cout << num_corr[i][j] << ' ';
   		}
   	cout << endl;
	}
	
}

void TrigEff::get_denom_corr(){

	for ( int i = 0; i < denom_corr.size(); i++ ){
   		for ( int j = 0; j < denom_corr[i].size(); j++ ){
      			cout << denom_corr[i][j] << ' ';
   		}
   	cout << endl;
	}
}

void TrigEff::get_corr(){

	for ( int i = 0; i < correlation.size(); i++ ){
   		for ( int j = 0; j < correlation[i].size(); j++ ){
      			cout << correlation[i][j] * 100  << "% ";
   		}
   	cout << endl;
	}
}







/*
int dataArray[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
unsigned dataArraySize = sizeof(dataArray) / sizeof(int);

// Method 1: Copy the array to the vector using back_inserter.
{
    copy(&dataArray[0], &dataArray[dataArraySize], back_inserter(dataVec));
}
*/

/*
void TrigEff::compute_spectwo_efficiency(int ntrigger,int *m_countorspectwotrig,int *m_countandspectwotrig,double *m_efficiencyspectwotrig,int a,int b) //computes the efficiency between two specific triggers
{
	double m = (*(m_countandspectwotrig + a*ntrigger +b));
	double k = (*(m_countorspectwotrig + a*ntrigger +b));
	*(m_efficiencyspectwotrig + a*ntrigger +b)=m/k; // triggers "counts" (ratio trig1 OR trig2 / trig1 AND trig2
	cout << "Entre " << a << " et " << b << ": " << *(m_efficiencyspectwotrig + a*ntrigger +b) *100 << "% ";
}

void TrigEff::compute_efficiency(int ntrigger,int *m_countortrig,int *m_countbothtrig,double *m_computed_eff)
{
    for(int i=0;i<ntrigger;i++){
		for(int j=0;j<ntrigger;j++){
			if((*(m_countortrig + i*ntrigger +j))==0){
				*(m_computed_eff + i*ntrigger +j)=0;
			}
			else{
				*(m_computed_eff + i*ntrigger +j)=((*(m_countbothtrig + i*ntrigger +j)*1.0)/(*(m_countortrig + i*ntrigger +j)));
			}
			cout << *(m_computed_eff + i*ntrigger +j) *100 << "% ";
		}
	cout << "\n";
	}
}
*/ //liste des fonctions dans computing.cpp
