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



	
	

	/*for(int i=0;i<ntrigger;i++) cout << "triggerName: " << triggerName->at(i) << endl;

	//Initialiser avec le premier evenement

	for(int i=0; i< ntrigger; i++){
		TrigNames.push_back(triggerName->at(i));

	}
	//Lors de la lecture des evenements 
	bool result = std::equal(TrigNames.begin(), TrigNames.end(), triggerName.begin());

	if(result){
		cout << "No trigger names added" << endl;
	}
	else{
		[c,ia,ib] = interesect(TrigNames,triggerName); // c : les elements pareil, ia : positions des TrigNames, ib : position des triggerName
	
		auto itPos = TrigNames.begin() + 

	}*/




void ListNameTriggers::FindAllNames(){
	
	int testcount=0;
	vector<string> TrigNames;

	Long64_t nentries = fChain->GetEntriesFast();
	Long64_t nbytes = 0, nb = 0, nbi = 0;
	
	// Initialization/reading the number of triggers (ntrigger), prescale triggers etc..

	Long64_t initializing = LoadTree(0); 

	cout << "nentries : " << nentries << endl;

	if (initializing < 0) cout << "Aborted"<< endl;

	nbi = fChain->GetEntry(initializing);   nbytes += nbi;
	
	cout << " ntrigger  : " << ntrigger << endl;
	
	for(int i=0; i< ntrigger; i++){
		//if(prescaleTrigger[i] == 1){
			cout << "TriggerName " << i << " : " << triggerName->at(i) << endl;
			TrigNames.push_back(triggerName->at(i));
			//MapOfTriggerNames.insert(triggerName->at(i),
			MapOfTriggerNames[triggerName->at(i)] = make_pair(true,true);
			
		//}
	}
	TrigNames.clear();
	/*for( auto it = MapOfTriggerNames.begin(); it != MapOfTriggerNames.end(); it++){
		cout << "["<< it->first << "," << it->second.first << "-" << it->second.second << "]"<< endl;

	
	}*/
	for (Long64_t jentry=0; jentry<nentries;jentry++) { //All entries
		Long64_t ientry = LoadTree(jentry);
		if(jentry!=0 && jentry%5000==0) cout << "Still here " << endl;
		if (ientry < 0) break;
        	nb = fChain->GetEntry(jentry);   nbytes += nb;	// 
		testcount+=1;
		
		//push_back dans
		cout << ntrigger << endl;
		for(int i=0; i< ntrigger; i++){
		//if(prescaleTrigger[i] == 1){
			TrigNames.push_back(triggerName->at(i));
			//MapOfTriggerNames.insert(triggerName->at(i),	
		//}
		}		



		cout << testcount << " triggerName size : " << TrigNames.size() << ", map size : "  << MapOfTriggerNames.size() << endl;
		TrigNames.clear();
		//for(int j=0; j < ntrigger ; j++){
			//for(auto itr = MapOfTriggerNames.begin(); itr != MapOfTriggerNames.end(); itr++){
				//auto it = find(triggerName->begin(), triggerName->end(), itr->first);
				
				//if(triggerName->size() >= MapOfTriggerNames.size()) 
				//cout << " Il y a des triggers en plus pour cet evenement ! " << endl;
				/*if(it!=triggerName->end()){
					
					cout << "Name " << itr->first << " found at position : " << it - triggerName->begin() << endl;
					
					
				}*/
				/*else{
					MapOfTriggerNames.insert(

				
				}*/
				//Si pas trouvé, il return last ? 	
				
				
			//}
			
			

		//}
	


	}
}



int main(){

	ListNameTriggers c;
	c.FindAllNames();

}
