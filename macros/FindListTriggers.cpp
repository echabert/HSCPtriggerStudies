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

	ofstream PrescaledTurnedBad;
	PrescaledSubList.open ("PrescaledTurnedBad.txt");

	int testcount=0;
	

	
	
	Long64_t nentries = fChain->GetEntriesFast();
	Long64_t nbytes = 0, nb = 0, nbi = 0;
	Long64_t initializing = LoadTree(0); 

	cout << "nentries : " << nentries << endl;
	
	if (initializing < 0) cout << "Aborted"<< endl;

	nbi = fChain->GetEntry(initializing);   nbytes += nbi;
	
	cout << " ntrigger  : " << ntrigger << endl;
	


	for(int i=0; i< ntrigger; i++){
		cout << "TriggerName " << i << " : " << triggerName->at(i) << endl;
		

		
		if(prescaleTrigger[i]==1){
			
			MapOfTriggerNames[triggerName->at(i)] = make_pair(true,true);
		}
		else{
			
			MapOfTriggerNames[triggerName->at(i)] = make_pair(false,true);
			PrescaledTurnedBad << "Prescale " << i << " is not 1" << endl;
		}

	}
	
	PrescaledTurnedBad << "----------------------------------------------------------------\n ----------------------------------------------------------------\n " << i << " is not 1" << endl;
	
	for (Long64_t jentry=0; jentry<nentries;jentry++) { //All entries
		Long64_t ientry = LoadTree(jentry);
		if(jentry!=0 && jentry%5000==0) cout << "Still here " << endl;
		if (ientry < 0) break;
        	nb = fChain->GetEntry(jentry);   nbytes += nb;	// 
		testcount+=1;
		

		
		//if(TrigNames.size() < MapOfTriggerNames.size()){

		for( auto it= MapOfTriggerNames.begin(); it != MapOfTriggerNames.end(); it++){
			auto itf = find (triggerName.begin() , triggerName.end(), it->first);
			if(itf != triggerName.end()){
			
			}
			else{
				
				PrescaledTurnedBad << "Prescale " << it->first << " was not in event " << jentry << endl;

			}


					

		}

		//}

		for(int i=0; i< ntrigger; i++){
			auto it = MapOfTriggerNames.find(triggerName->at(i));
			if(it != MapOfTriggerNames.end()){
				//cout << "Found " << TrigNames[i] << endl;
				if(prescaleTrigger[i]!=1){
					if(MapOfTriggerNames[triggerName->at(i)] == (true, true)){
						PrescaledTurnedBad << "Entry " << jentry << " made prescale " << i << " false" << endl;
					}
					MapOfTriggerNames[triggerName->at(i)] = make_pair<bool, bool>(false,true);
					
					
				}

				
			}
			else{
				if(prescaleTrigger[i]==1){
					cout << "Added one vector to the map " << endl;
					MapOfTriggerNames.insert(pair<string,pair<bool, bool> > (triggerName->at(i),pair<bool, bool>(true,false)));
				}
				else{
					MapOfTriggerNames.insert(pair<string,pair<bool, bool> > (triggerName->at(i),pair<bool, bool>(false,false)));

				}
			}

		}
		TrigNames.clear();
	}

	for(auto itr = MapOfTriggerNames.begin(); itr != MapOfTriggerNames.end(); itr++){
		CompleteList << itr->first << endl; //<< " " << itr->second.first << " " << itr->second.second
		if(itr->second.first){
			PrescaledSubList << itr->first << endl; //<< " " << itr->second.first << " " << itr->second.second
		}

		
	}
	CompleteList.close();
	PrescaledSubList.close();
	PrescaledTurnedBad.close();
}



int main(){

	ListNameTriggers c;
	c.FindAllNames();

}
