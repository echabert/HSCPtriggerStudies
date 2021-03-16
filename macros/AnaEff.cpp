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
	

	//liste des prescale, if psrescale == 1 : print dans un vecteur /fichier texte le trigger associé et la ligne eff.push_back(trigger) + le nombre total de triggers qui nous interesse

	ofstream prescaledtriggers;
	prescaledtriggers.open ("PrescaledTriggers.txt");
	vector<pair<int, string> > selecttriger;

	
	for(int j = 0; j <ntrigger; j++){
		if(prescaleTrigger[j]==1){
			selecttriger.push_back(make_pair(j,trigEff_presel.selectedtriggernames[j]));
			prescaledtriggers << j << " " << trigEff_presel.selectedtriggernames[j] << endl;
		}
	}
	prescaledtriggers.close();



	//cout << "Prescale 12 : " << prescaleTrigger[12] << endl;
	cout << "ntrigger before loop entries : " << ntrigger << endl;
	cout << "nentries : " << nentries << endl;
	nentries=1000;
	for (Long64_t jentry=0; jentry<nentries;jentry++) { //All entries
		Long64_t ientry = LoadTree(jentry);
		if(jentry%5000==0) cout << "Still here " << endl;
		if (ientry < 0) break;
        	nb = fChain->GetEntry(jentry);   nbytes += nb;	// 
		vector<Bool_t> vtrigger; //Convert array into vector
		for(int i=0;i<ntrigger;i++){
			vtrigger.push_back(passTrigger[i]); //Fill vtrigger with bool values
		}
		trigEff_presel.Fill(vtrigger); 
	}
	trigEff_presel.Compute();

}



int main(){

	AnaEff ana;
	ana.Loop();
	
}
