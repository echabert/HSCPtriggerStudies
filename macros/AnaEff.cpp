#define efficiency_cxx
#include "AnaEff.h"


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
	//cout << "ntrigger is "<< ntrigger << endl;

	
	// Choosing one trigger manually to estimate the efficiency 
	

	/*trigEff_presel.Print_num_corr();
	trigEff_presel.Print_denom_corr();*/ // verification of vectors (all 0s)


	
	for (Long64_t jentry=0; jentry<nentries;jentry++) { // looping on all entries
		Long64_t ientry = LoadTree(jentry);
		if (ientry < 0) break;
        	nb = fChain->GetEntry(jentry);   nbytes += nb;
		//cout << "Entry " << jentry << ", PT : " << track_pt[hscp_track_idx[0]] << endl;
		
//convert array into vector
		vector<Bool_t> vtrigger;
		
		for(int i=0;i<ntrigger;i++){
			vtrigger.push_back(passTrigger[i]);
		}	
		
	        trigEff_presel.Fill(vtrigger); //Fill info about the trigger
		//trigEff_presel.get_corr();

	}
	
	//trigEff_presel.get_num_corr();
	//trigEff_presel.get_denom_corr();
	
	//trigEff_presel.Compute_corr();
	//trigEff_presel.Print_corr();
	

	trigEff_presel.Compute_eff();
	//trigEff_presel.Print_denom_eff();
	trigEff_presel.Fill_error();
	//trigEff_presel.Print_eff();
	trigEff_presel.Print_spec_eff(trigEff_presel.column);
	trigEff_presel.Print_denom_eff();




cout << " JUST END" << endl; 

//Freeing all calloc/malloc //355 355 : 100% / 661 235 : 33.3 %

}

 //ajouter une fonction qui retourne ntrigger? Ou comment peut-on appeler ntrigger d'ici ?
int main(){

	AnaEff ana;
	ana.Loop();
	
}
