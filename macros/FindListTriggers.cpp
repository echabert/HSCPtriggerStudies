#include <vector>
#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include <iostream>
#include <fstream>
#include <map>
#include <iterator>
#include <algorithm>
#include "FindListTriggers.h"


using namespace std; 


int ListNameTriggers::ListNameTriggersReverse(string name){
	int cs = 0;
	
	for (int i = 0 ; i < triggerName->size() ; i++){
		if(triggerName->at(i) == name){
			return cs;

		}
		cs+=1;

	}
	return 9999;

}

int ListNameTriggers::IsInList(string name){
	int ct=0;
	cout << InfoTriggers.size() << endl;
	for(int i = 0 ; i < InfoTriggers.size() ; i++){
		//cout << i << endl;
		if(InfoTriggers[i].first == name){
			return ct;
		}
		ct+=1;
	}
	return 9999;
}


void ListNameTriggers::FindAllNames(){
	
	ofstream CompleteList;
	string DataType = "Gluino18002023";
	string NameList = "CompleteList";
	string ExtTxt = ".txt";
	string PresList = "PrescaledSubList";
	string NamePrescaledList = PresList + DataType + ExtTxt;
	string NameCompleteList = NameList + DataType + ExtTxt;

	CompleteList.open (NameCompleteList.c_str()); //change name


	ofstream PrescaledSubList;
	PrescaledSubList.open (NamePrescaledList.c_str());

	ofstream PrescaledTurnedBad;
	PrescaledTurnedBad.open ("PrescaledTurnedBad.txt");

	int testcount=0;
	
	Long64_t nentries = fChain->GetEntriesFast();
	Long64_t nbytes = 0, nb = 0, nbi = 0;
	Long64_t initializing = LoadTree(0); 

	cout << "Number of entries : " << nentries << endl;
	
	if (initializing < 0) cout << "Aborted"<< endl;

	nbi = fChain->GetEntry(initializing);   nbytes += nbi;
	
	cout << " ntrigger  : " << ntrigger << endl;
	


	for(int i=0; i< ntrigger; i++){
		cout << "TriggerName " << i << " : " << triggerName->at(i) << endl;
		
		//cout << prescaleTrigger[i] << endl;
		
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
	cout << "before loop"<<nentries<<endl;
	
	for (Long64_t jentry=0; jentry<nentries;jentry++) { //All entries
		Long64_t ientry = LoadTree(jentry);
		if(jentry!=0 && jentry%1000==0) cout << "+1k" << " => " << jentry << " , "<<(jentry*1.0/nentries)*100 << " %" << endl;
		if (ientry < 0) break;
        	nb = fChain->GetEntry(jentry);   nbytes += nb;	// 
		testcount+=1;
		cout << "in loop" << ntrigger << endl;
		
		//cout << "----------------------------- new event -----------------------------" << endl;
		//if(ntrigger > InfoTriggers.size()){
			//cout << " --------NEW EVENT ------- "<< endl;
			for(int i=0; i< ntrigger; i++){
				cout << i <<"is ok"<< triggerName->at(i) <<  endl;
				int b = IsInList(triggerName->at(i));
				//cout <<"after IsInList"<<endl;
				if(b!=9999){
					if(prescaleTrigger[i]==1){
						//cout << triggerName->at(i) << " has prescale 1"  << endl;
					}
					else{
						InfoTriggers[b] = make_pair(triggerName->at(i) , make_pair(false,true));
						//cout << triggerName->at(i) << " has prescale " << prescaleTrigger[i] << endl;
					}

				}

				else if(b==9999){
					//if(prescaleTrigger[i]==1){
					cout << "Added one vector to the list that wasnt there before, prescale false" << endl;
					InfoTriggers.push_back(make_pair(triggerName->at(i),make_pair(false,false)));
				
				}

			}
	//cout <<"end loop ntrigger"<<endl;
	}

	for ( int it = 0 ; it < InfoTriggers.size() ; it++){
		CompleteList << InfoTriggers[it].first << " " << endl;
		if(InfoTriggers[it].second.first){
			PrescaledSubList << InfoTriggers[it].first << endl; //<< " " << itr->second.first << " " << itr->second.second
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


