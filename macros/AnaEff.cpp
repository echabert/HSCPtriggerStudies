#define efficiency_cxx
#include "AnaEff.h"


#include <iostream>
#include <string>
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <TPad.h>
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
	

	Long64_t nentries = fChain->GetEntriesFast();
	Long64_t nbytes = 0, nb = 0, nbi = 0;

	// Initialization/reading the number of triggers (ntrigger)

	Long64_t initializing = LoadTree(0); 
	if (initializing < 0) cout << "Aborted"<< endl;
	nbi = fChain->GetEntry(initializing);   nbytes += nbi;
	int nbtrig=ntrigger; 

	//cout << "ntrigger : " << ntrigger << endl;
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
	        trigEff_presel.Fill(vtrigger); //Fill vtrigger with the bool values
	}
	
	
	//OutputHisto->cd();
	trigEff_presel.Compute();

//cout << " JUST END" << endl; 
}



int main(){

	AnaEff ana;
	ana.Loop();
	
}

 //355 355 : 100% / 661 235 : 33.3 %
