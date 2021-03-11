#define efficiency_cxx
#include "AnaEff.h"
//#include "Computing.h"

#include <iostream>
#include <string>
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


void AnaEff::compute_spectwo_efficiency(int *m_countorspectwotrig,int *m_countandspectwotrig,double *m_efficiencyspectwotrig,int a,int b) //computes the efficiency between two specific triggers
{
	double m = (*(m_countandspectwotrig + a*ntrigger +b));
	double k = (*(m_countorspectwotrig + a*ntrigger +b));
	*(m_efficiencyspectwotrig + a*ntrigger +b)=m/k; // triggers "counts" (ratio trig1 OR trig2 / trig1 AND trig2
	cout << "Entre " << a << " et " << b << ": " << *(m_efficiencyspectwotrig + a*ntrigger +b) *100 << "% ";
}


void AnaEff::number_spectwo(bool passTrigger[],int *m_countorspectwotrig,int *m_countandspectwotrig,int a,int b) //computes the number of candidates passing both and/or one trigger
{
    bool trig1= passTrigger[a],trig2= passTrigger[b];
    if(trig1 || trig2){
        *(m_countorspectwotrig + a*ntrigger +b)+=1;
    }
    if(trig1 && trig2){
        *(m_countandspectwotrig + a*ntrigger +b)+=1;
    }
}


void AnaEff::compute_efficiency(int ntrigger,int *m_countortrig,int *m_countbothtrig,double *m_computed_eff)
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


void AnaEff::number_candidates(bool passTrigger[],int ntrigger,int *m_countortrig,int *m_countbothtrig) //Calculating the 2D array as the matrix of efficiency (1 with 1, 1 with 2 etc...)
{
	bool trig1,trig2;
	for(int i=0;i<ntrigger;i++){
		trig1 = passTrigger[i];

		for(int j=0;j<ntrigger;j++){
			trig2 = passTrigger[j];
			if(trig1 || trig2){
				*(m_countortrig + i*ntrigger +j)+=1;
			}
			if(trig1 && trig2){
				*(m_countbothtrig + i*ntrigger +j)+=1;
			}

		}
	}
}

void AnaEff::Loop()
{
	/*TString outputfilename="results.root";
	TFile* OutputHisto = new TFile(outputfilename,"RECREATE");*/

	//cout << "OutputHisto right after init : " << OutputHisto << endl;
	Long64_t nentries = fChain->GetEntriesFast();
	Long64_t nbytes = 0, nb = 0, nbi = 0;

	
	// Initialization/reading the number of triggers (ntrigger)

	Long64_t initializing = LoadTree(0); 
	if (initializing < 0) cout << "Aborted"<< endl;
	nbi = fChain->GetEntry(initializing);   nbytes += nbi;
	int nbtrig=ntrigger; 
	cout << "ntrigger is "<< ntrigger << endl;
	
 	// recup dans un vecteur : lire et re enregistrer
	

	// Dynamically allowed memory for 2D arrays (size : ntrigger*ntrigger)
	

	/*int *m_countortrig = (int *)calloc(nbtrig * nbtrig , sizeof(int));
	int *m_countbothtrig = (int *)calloc(nbtrig * nbtrig , sizeof(int));

	double *m_efficiencyspectwotrig = (double *)calloc(nbtrig * nbtrig , sizeof(double));

	int *m_countorspectwotrig = (int *)calloc(nbtrig * nbtrig , sizeof(int));
	int *m_countandspectwotrig = (int *)calloc(nbtrig * nbtrig , sizeof(int));

	double *m_computed_eff = (double *)calloc(nbtrig * nbtrig , sizeof(double));
	*/

	int *m_countortrig = new int[nbtrig*nbtrig];

	cout << "countortrig right after init : " <<m_countortrig << endl; 

	int *m_countbothtrig = new int[nbtrig*nbtrig];

	double *m_efficiencyspectwotrig = new double[nbtrig*nbtrig];



	int *m_countorspectwotrig = new int[nbtrig*nbtrig];
	int *m_countandspectwotrig = new int[nbtrig*nbtrig];

	double *m_computed_eff = new double[nbtrig*nbtrig];




	// Choosing two triggers manually to estimate the efficiency of trig2
	
	int a,b;
	cout << "Choose two specific triggers to compute the efficiency" << endl;
	cin >> a >> b; // Specific number of triggers to estimate the efficiency, the list of triggers is available and the corresponding is line i = passTrigger[i] in file triggerNames
	

	for (Long64_t jentry=0; jentry<nentries;jentry++) { // looping on all entries
		Long64_t ientry = LoadTree(jentry);
		if (ientry < 0) break;
        	nb = fChain->GetEntry(jentry);   nbytes += nb;
		cout << track_pt[hscp_track_idx[0]] << endl; // test PT
		cout << "Entry number " << jentry << endl;
		
		//convert array into vector
		vector<Bool_t> vtrigger (passTrigger, passTrigger+sizeof(passTrigger)/sizeof(Bool_t));
		//Fill info about the trigger
	        trigEff_presel.Fill(vtrigger);
		cout<<vtrigger.size()<<endl;

		//number_candidates(passTrigger,ntrigger,m_countortrig,m_countbothtrig);
		//number_spectwo(passTrigger,m_countorspectwotrig,m_countandspectwotrig,a,b); //355 355 : 100% / 661 235 : 33.3 %
	}
	
	//compute_efficiency(ntrigger,m_countortrig,m_countbothtrig,m_computed_eff);
	//compute_spectwo_efficiency(m_countorspectwotrig,m_countandspectwotrig,m_efficiencyspectwotrig,a,b);



cout << " JUST END" << endl; 
cout << "countortrig right after JUST END: " <<m_countortrig << endl;
//Freeing all calloc/malloc
/*if(m_countortrig!=0){
	free(m_countortrig);
	m_countortrig=NULL;
}
if(m_countbothtrig!=0){
	free(m_countbothtrig);
	m_countbothtrig=NULL;
}
if(m_efficiencyspectwotrig!=0){
	free(m_efficiencyspectwotrig);
	m_efficiencyspectwotrig=NULL;
}
if(m_countorspectwotrig!=0){
	free(m_countorspectwotrig);
	m_countorspectwotrig=NULL;
}
if(m_countandspectwotrig!=0){
	free(m_countandspectwotrig);
	m_countandspectwotrig=NULL;
}
if(m_computed_eff!=0){
	free(m_computed_eff);
	m_computed_eff=NULL;
}*/

/*if(OutputHisto!=0){
	delete[] OutputHisto;
}*/
//delete[] OutputHisto;
delete[] m_countortrig;
cout << "countortrig right after delete: " <<m_countortrig << endl; 
delete[] m_countbothtrig;
delete[] m_efficiencyspectwotrig;
delete[] m_countorspectwotrig;
delete[] m_countandspectwotrig;
delete[] m_computed_eff;

/*
free(m_countortrig);
m_countortrig=NULL;

free(m_countbothtrig);
m_countbothtrig=NULL;

free(m_efficiencyspectwotrig);
m_efficiencyspectwotrig=NULL;

free(m_countorspectwotrig);
m_countorspectwotrig=NULL;

free(m_countandspectwotrig);
m_countandspectwotrig=NULL;

free(m_computed_eff);
m_computed_eff=NULL;
*/
}

 //ajouter une fonction qui retourne ntrigger? Ou comment peut-on appeler ntrigger d'ici ?
int main(){

	AnaEff ana;
	ana.Loop();
	
}
