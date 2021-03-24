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
#include <algorithm>
#include <vector>


using namespace std;





void AnaEff::Loop()
{

	Long64_t nentries = fChain->GetEntriesFast();
	Long64_t nbytes = 0, nb = 0, nbi = 0;

	// Initialization/reading the number of triggers (ntrigger), prescale triggers etc..

	Long64_t initializing = LoadTree(0); 
	if (initializing < 0) cout << "Aborted"<< endl;
	nbi = fChain->GetEntry(initializing);   nbytes += nbi;
	cout << " ntrigger  : " << ntrigger << endl;


	ifstream ifile("data/triggerNamesSingleMuon.txt"); // ifstream ifile("data/triggerNames.txt")
	vector<string> triggerNames;

	string tmp;
	while(getline(ifile,tmp)){
   		triggerNames.push_back(tmp);
	}

	

	cout<<"size of triggerNames : "<< triggerNames.size() <<endl;
	ifile.close();
	
	vector<string> str;
	string interfstr;
	

	ifstream inttrigs("data/inttrigs.txt"); // change in file inttrigs the list of triggers that you want to study
	while(getline(inttrigs,tmp)){
   		str.push_back(tmp);
	}
	inttrigs.close();
	vector<string> listofprescaledtriggers;

	/* SUBLIST
	
	for(int i=0;i<str.size();i++){
		if(prescaleTrigger[i]==1){
			strprescaled.push_back(str[i]);
		}
	}

	*/

	for(int i=0;i<triggerNames.size();i++){ // ntrigger
			//cout << "Prescale # " << i << " : " << prescaleTrigger[i] << endl;
			//cout << "Passtrigger # " << i << " : " << passTrigger[i] << endl;
			if(prescaleTrigger[i]==1){
				listofprescaledtriggers.push_back(triggerNames[i]);
			}

		}


	//trigEff_presel.Load(triggerNames,str,1,"entered",""); 
	trigEff_selection_obs.Load(triggerNames,str,1,"entered","MET","eff_MET.root");

	str.clear();
	
	//nentries=30;
	for (Long64_t jentry=0; jentry<nentries;jentry++) { //All entries
		Long64_t ientry = LoadTree(jentry);
		if(jentry!=0 && jentry%5000==0) cout << "Still here " << endl;
		if (ientry < 0) break;
        	nb = fChain->GetEntry(jentry);   nbytes += nb;	// 
		
		vector<Bool_t> vtrigger; //Convert array into vector
		vector<float> TrackPT,MuonPT,METPT;

		float HighestPT,HighestMuonPT,HighestMET;

		if(nhscp !=0){
			for(int ihs=0; ihs<nhscp;ihs++){
				TrackPT.push_back(track_pt[hscp_track_idx[ihs]]);
				//METPT.push_back(pfmet_pt[hscp_track_idx[ihs]]);
				//MuonPT.push_back(muon_pt[hscp_track_idx[ihs]]);
				cout << "PT : " << TrackPT[ihs] << endl;
			}
			sort(TrackPT.rbegin(), TrackPT.rend());
			//sort(METPT.rbegin(), METPT.rend());
			//sort(MuonPT.rbegin(), MuonPT.rend());	
			
			HighestPT = TrackPT[0];
			//HighestMET=METPT[0];
			//HighestMuonPT=MuonPT[0];
			//METPT.clear();
			//MuonPT.clear();
			TrackPT.clear();
		}
		else{
			
			HighestMuonPT = muon_pt[0];
			HighestMET = pfmet_pt[0];
			
			HighestPT = 0.0001;

		}
		//cout << "Highest PT after reverse: " << HighestPT << endl;
		
		 //Convert array into vector
		for(int i=0;i<ntrigger;i++){
			vtrigger.push_back(passTrigger[i]);
			//prescaletrigger.push_back(prescaleTrigger[i]);
		}
		
		//trigEff_presel.Fill(vtrigger); 

		trigEff_selection_obs.Fill(vtrigger,HighestMET);
		
		//trigEff_selection_obs.Fill(vtrigger,HighestMuonPT);
		
		//trigEff_presel.Fill(vtrigger,HighestPT);
	}
	cout << "Before compute " << endl;
	//trigEff_presel.Compute();

	trigEff_selection_obs.Compute();
	
	cout << "Just ended " << endl;

	triggerNames.clear();
	listofprescaledtriggers.clear();

}



int main(){

	AnaEff ana;
	ana.Loop();
	
}


