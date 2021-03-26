#include <vector>
#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include <iostream>
#include <fstream>
#include <map>
#include <iterator>

#include "FindListTriggers.h" 

using namespace std; 



void ListNameTriggers::FindAllNames(){
	
	ofstream CompleteList;
	CompleteList.open ("CompleteList.txt");

	ofstream PrescaledSubList;
	PrescaledSubList.open ("PrescaledSubList.txt");


	int testcount=0;
	vector<string> TrigNames;

	Long64_t nentries = fChain->GetEntriesFast();
	Long64_t nbytes = 0, nb = 0, nbi = 0;
	Long64_t initializing = LoadTree(0); 

	cout << "nentries : " << nentries << endl;

	if (initializing < 0) cout << "Aborted"<< endl;

	nbi = fChain->GetEntry(initializing);   nbytes += nbi;
	
	cout << " ntrigger  : " << ntrigger << endl;
	
	for(int i=0; i< ntrigger; i++){
		cout << "TriggerName " << i << " : " << triggerName->at(i) << endl;
		TrigNames.push_back(triggerName->at(i));
		if(prescaleTrigger[i]){
			MapOfTriggerNames[triggerName->at(i)] = make_pair(true,true);
		}
		else{
			MapOfTriggerNames[triggerName->at(i)] = make_pair(false,true);
		}
	}

	TrigNames.clear();
	
	for (Long64_t jentry=0; jentry<nentries;jentry++) { //All entries
		Long64_t ientry = LoadTree(jentry);
		if(jentry!=0 && jentry%5000==0) cout << "Still here " << endl;
		if (ientry < 0) break;
        	nb = fChain->GetEntry(jentry);   nbytes += nb;	// 
		testcount+=1;
		

		for(int i=0; i< ntrigger; i++){
		//if(prescaleTrigger[i] == 1){
			//cout << " Prescale " << i << " : " << prescaleTrigger[i] << endl;
			TrigNames.push_back(triggerName->at(i));	
		//}
		}		



		//if(TrigNames.size() < MapOfTriggerNames.size()){


		//}

		for(int i=0; i< ntrigger; i++){
			auto it = MapOfTriggerNames.find(TrigNames[i]);
			if(it != MapOfTriggerNames.end()){
				//cout << "Found " << TrigNames[i] << endl;
				
			}
			else{
				if(prescaleTrigger[i]){
					cout << "Added one vector to the map " << endl;
					MapOfTriggerNames.insert(pair<string,pair<bool, bool> > (TrigNames[i],pair<bool, bool>(true,false)));
				}
				else{
					MapOfTriggerNames.insert(pair<string,pair<bool, bool> > (TrigNames[i],pair<bool, bool>(false,false)));

				}
			}

		}
		TrigNames.clear();
	}

	for(auto itr = MapOfTriggerNames.begin(); itr != MapOfTriggerNames.end(); itr++){
		CompleteList << itr->first << " " << itr->second.first << " " << itr->second.second << endl;
		if(itr->second.first){
			PrescaledSubList << itr->first << " " << itr->second.first << " " << itr->second.second << endl;
		}

		
	}
	CompleteList.close();
}



int main(){

	ListNameTriggers c;
	c.FindAllNames();

}
