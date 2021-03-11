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


void TrigEff::compute_corr()
{
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
	for(int i=0;i<passtrig.size();i++){
		for(int j=0;j<passtrig.size();j++){
			num_corr[i].push_back(0);
		}
	denom_corr[i].push_back(0);
	}
	


	for(int i=0;i<passtrig.size();i++){
		trig1 = passtrig[i];

		for(int j=0;j<passtrig.size();j++){
			trig2 = passtrig[j];
			if(trig1 || trig2){
				 // not good syntax, mais vecteur de vecteurs 
				num_corr[i][j]+=1;
			}
			if(trig1 && trig2){
				denom_corr[i][j]+=1;
			}

		}
	}


	
	
}


void TrigEff::get_corr(){
	for(int i=0;i<num_corr.size();i++){
		for(int j=0;j<denom_corr.size();j++){
		cout << "numerateur : " << num_corr[i][j] << endl;
		}
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
