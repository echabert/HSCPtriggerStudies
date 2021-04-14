#include <vector>
#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include <iostream>
#include <fstream>
#include <map>
#include <iterator>
#include <algorithm>
#include <ranges>
#include "FindListTriggers.h"


using namespace std; 

int ListNameTriggers::IsInList(string name){
	int ct=0;
	for(int i = 0 ; i < InfoTriggers.size() ; i++){
		if(InfoTriggers[i].first == name){
			return ct;
		}
		ct+=1;
	}
	return 9999;
}


void ListNameTriggers::FindAllNames(){
	
	ofstream CompleteList;
	CompleteList.open ("CompleteList.txt"); //change name

	ofstream PrescaledSubList;
	PrescaledSubList.open ("PrescaledSubList.txt");

	ofstream PrescaledTurnedBad;
	PrescaledTurnedBad.open ("PrescaledTurnedBad.txt");

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
			InfoTriggers.push_back(make_pair(triggerName->at(i), make_pair(true,true)));

		//	MapOfTriggerNames[triggerName->at(i)] = make_pair(true,true);
		}
		else{
			InfoTriggers.push_back(make_pair(triggerName->at(i), make_pair(false,true)));

		//	MapOfTriggerNames[triggerName->at(i)] = make_pair(false,true);
			PrescaledTurnedBad << "Prescale " << i << " is not 1" << endl;
		}


	}
	
	PrescaledTurnedBad << "----------------------------------------------------------------\n ----------------------------------------------------------------\n " << endl;
	
	for (Long64_t jentry=0; jentry<nentries;jentry++) { //All entries
		Long64_t ientry = LoadTree(jentry);
		if(jentry!=0 && jentry%5000==0) cout << "Still here " << endl;
		if (ientry < 0) break;
        	nb = fChain->GetEntry(jentry);   nbytes += nb;	// 
		testcount+=1;
		 

		
		//if(TrigNames.size() < MapOfTriggerNames.size()){

		/*for( auto it= MapOfTriggerNames.begin(); it != MapOfTriggerNames.end(); it++){
			string transf = it->first;
			//cout << transf << endl;  
			auto itf = std::find( triggerName->begin() , triggerName->end(), transf);
			//auto itf = find (triggerName->begin() , triggerName->end(), transf);
			if(itf != triggerName->end()){
			
			}
			else{
				
				PrescaledTurnedBad << "Prescale " << it->first << " was not in event " << jentry << endl;

			}
		

					

		}*/
	//	PrescaledTurnedBad <<"------------------------------------------------" << endl;
		//}

		for(int i=0; i< ntrigger; i++){
			//auto it = find(InfoTriggers.begin(), InfoTriggers.end(),triggerName->at(i));

			//auto it = std::ranges::find(InfoTriggers, triggerName->at(i), &std::pair<std::string, std::pair<bool,bool> >::first);
			int a = IsInList(triggerName->at(i));
			if(a!=9999){
				if(prescaleTrigger[a]!=1){
					if(InfoTriggers[a].second.first == true && InfoTriggers[a].second.second == true){
						PrescaledTurnedBad << "Entry " << jentry << " made prescale " << i << " false" << endl;
					}
					InfoTriggers[a] = make_pair(triggerName->at(a) , make_pair(false,true));
				}
			}
			else{
				if(prescaleTrigger[i]==1){
					cout << "Added one vector to the map " << endl;
					InfoTriggers.push_back(make_pair(triggerName->at(i),make_pair(true,false)));
				}
				else{
					InfoTriggers.push_back(make_pair(triggerName->at(i),make_pair(false,false)));
				}
			}
			//auto it = std::find_if(InfoTriggers.begin(), InfoTriggers.end(), IsInList(triggerName->at(i)));

			//
			/*if(it != InfoTriggers.end()){
				if(prescaleTrigger[i]!=1){
					if(InfoTriggers[i].second.first == true && InfoTriggers[i].second.second == true){
						PrescaledTurnedBad << "Entry " << jentry << " made prescale " << i << " false" << endl;
					}
					InfoTriggers[i] = make_pair(triggerName->at(i) , make_pair(false,true));
				}
			}
			else{
				if(prescaleTrigger[i]==1){
					cout << "Added one vector to the map " << endl;
					InfoTriggers.push_back(make_pair(triggerName->at(i),make_pair(true,false)));

					
				}
				else{
					InfoTriggers.push_back(make_pair(triggerName->at(i),make_pair(false,false)));
				}
			}*/


/*
			auto it = MapOfTriggerNames.find(triggerName->at(i));
			if(it != MapOfTriggerNames.end()){
				//cout << "Found " << TrigNames[i] << endl;
				if(prescaleTrigger[i]!=1){
					if(MapOfTriggerNames[triggerName->at(i)] == pair<bool, bool>(true,true)){
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
			}*/

		}
		
	}
	for ( auto it = 0 ; it != InfoTriggers.size() ; it++){
		CompleteList << InfoTriggers[it].first << endl;
		if(InfoTriggers[it].second.first){
			PrescaledSubList << InfoTriggers[it].first << endl; //<< " " << itr->second.first << " " << itr->second.second
		}
	}

	/*for(auto itr = MapOfTriggerNames.begin(); itr != MapOfTriggerNames.end(); itr++){
		CompleteList << itr->first << endl; //<< " " << itr->second.first << " " << itr->second.second
		if(itr->second.first){
			PrescaledSubList << itr->first << endl; //<< " " << itr->second.first << " " << itr->second.second
		}	
	}*/

	CompleteList.close();
	PrescaledSubList.close();
	PrescaledTurnedBad.close();
}



int main(){

	ListNameTriggers c;
	c.FindAllNames();

}
