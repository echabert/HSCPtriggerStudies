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
	
	cout << " # of triggers : " << ntrigger << endl;

	ifstream ifile("data/triggerNames.txt");
	vector<string> triggerNames;

	string tmp;
	while(getline(ifile,tmp)){
   		triggerNames.push_back(tmp);
	}

	

	cout<<"#triggers: "<< triggerNames.size() <<endl;
	ifile.close();
	
	vector<string> str;
	string interfstr;
	

	ifstream inttrigs("data/inttrigs.txt"); // for choosen ones : data/inttrigs.txt
	while(getline(inttrigs,tmp)){
   		str.push_back(tmp);
	}
	inttrigs.close();

	/*int studytrig;
	
	cout << "How many triggers do you want to study? "  << endl;
	cin >> studytrig;

	if(studytrig==1) cout <<"Name the trigger : " << endl;

	else cout <<"Name the triggers : " << endl;

	for (int i = 0; i< studytrig;i++){
   		cin >> interfstr;
		str.push_back(interfstr);
	}*/


	
	
	vector<string> listofprescaledtriggers;

	for(int i=0;i<ntrigger;i++){
			
			//cout << "Prescale # " << i << " : " << prescaleTrigger[i] << endl;
			//cout << "Passtrigger # " << i << " : " << passTrigger[i] << endl;
			if(prescaleTrigger[i]==1){
				listofprescaledtriggers.push_back(triggerNames[i]);
			}

		}

	trigEff_presel.Load(triggerNames,listofprescaledtriggers,1,"entered"); // str instead of listofprescaledtriggers
	str.clear();
	
//Reconnu comme muon, puis check muon_pt 
	//nentries=30;
	for (Long64_t jentry=0; jentry<nentries;jentry++) { //All entries
		Long64_t ientry = LoadTree(jentry);
		if(jentry!=0 && jentry%5000==0) cout << "Still here " << endl;
		if (ientry < 0) break;
        	nb = fChain->GetEntry(jentry);   nbytes += nb;	// 
		
		vector<Bool_t> vtrigger; //Convert array into vector
		vector<float> TrackPT;

		float HighestPT,HighestMuonPT;

		

		if(nhscp !=0){
			for(int ihs=0; ihs<nhscp;ihs++){
				TrackPT.push_back(track_pt[hscp_track_idx[ihs]]);
			}

			sort(TrackPT.rbegin(), TrackPT.rend());
			HighestPT = TrackPT[0];

			TrackPT.clear();
		}
		else{
			HighestPT = 0.0001;

		}
		//cout << "Highest PT after reverse: " << HighestPT << endl;
		
		 //Convert array into vector
		for(int i=0;i<ntrigger;i++){
			vtrigger.push_back(passTrigger[i]); //Fill vtrigger with bool values
			//prescaletrigger.push_back(prescaleTrigger[i]);
		}
		
		trigEff_presel.Fill(vtrigger,HighestPT); 
		
	}
	cout << "Before compute " << endl;
	trigEff_presel.Compute();
	
	cout << "Just ended " << endl;
	triggerNames.clear();
	listofprescaledtriggers.clear();

}



int main(){

	AnaEff ana;
	ana.Loop();
	
}


