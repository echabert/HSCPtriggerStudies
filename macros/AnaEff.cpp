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

	// Initialization/reading the number of triggers (ntrigger)

	Long64_t initializing = LoadTree(0); 
	if (initializing < 0) cout << "Aborted"<< endl;
	nbi = fChain->GetEntry(initializing);   nbytes += nbi;
	
	ifstream ifile("data/triggerNames.txt");
	vector<string> triggerNames;
	string tmp;
	while(getline(ifile,tmp)){
   		triggerNames.push_back(tmp);
	}
	cout<<"#triggers: "<< triggerNames.size() <<endl;
	vector<string> str;
	string interfstr;
	int studytrig;
	cout << "How many triggers do you want to study? "  << endl;
	cin >> studytrig;
	if(studytrig==1) cout <<"Name the trigger : " << endl;
	else cout <<"Name the triggers : " << endl;
	for (int i = 0; i< studytrig;i++){
   		cin >> interfstr;
		str.push_back(interfstr);
	}


	trigEff_presel.selection=str;
	trigEff_presel.selectedtriggernames=triggerNames;
	trigEff_presel.Load(triggerNames,str);




	ofstream prescaledtriggers;
	prescaledtriggers.open ("PrescaledTriggers.txt");
	
	
	//nentries=100;
	for (Long64_t jentry=0; jentry<nentries;jentry++) { //All entries
		Long64_t ientry = LoadTree(jentry);
		if(jentry!=0 && jentry%5000==0) cout << "Still here " << endl;
		if (ientry < 0) break;
        	nb = fChain->GetEntry(jentry);   nbytes += nb;	// 
		vector<Bool_t> vtrigger; //Convert array into vector
		for(int i=0;i<ntrigger;i++){
			vtrigger.push_back(passTrigger[i]); //Fill vtrigger with bool values
		}

		for(int j = 0; j <trigEff_presel.selection.size(); j++){
			//if(prescaleTrigger[j]==1){
			
				auto it = find(triggerNames.begin(), triggerNames.end(), trigEff_presel.selection[j]);
				auto index = distance(triggerNames.begin(), it);
				trigEff_presel.ListTriggers[index] = (passTrigger[index]);
			//}
		}
		trigEff_presel.Fill(trigEff_presel.ListTriggers); 
		//trigEff_presel.Fill(vtrigger); 
	}

	prescaledtriggers.close();
	trigEff_presel.Compute();
	
	cout << "Just ended " << endl;
	
}



int main(){

	AnaEff ana;
	
	ana.Loop();
	
}
