#include "inc/TrigEff.h"

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
#include <iostream>


using namespace std;



TrigEff::TrigEff(){
	EFF_TRIG=0;
	EFF_DISTRIB=0;
	CORR=0;
	OutputHisto=0;
	
}



TrigEff::~TrigEff(){ 

	
	
	SelectedTriggerNames.clear();
	//Selection.clear();

	EffList.clear();
	
	TriggerNames.clear();
	TriggerPass.clear();

	Correlation.clear();
	NumCorr.clear();
	DenomCorr.clear();

	CorrErr.clear();

	Efficiency.clear();
	
	NumEfficiency.clear();
	DenomEfficiency.clear();
	EffErr.clear();
	


	if(!EFF_TRIG){
		delete EFF_TRIG;
	}
	if(!OutputHisto){
		delete OutputHisto;
	}
	if(!CORR){
		delete CORR;
	}
	if(!EFF_DISTRIB){ 
		delete EFF_DISTRIB;
	}
}




//*************************************************************
//Dans Load : Selection va être le nom de la variable qu'on veut étudier. Dans Fill on va mettre la valeur

void TrigEff::Load(const vector<string> &triggerNames,const vector<string> &SelectedTriggerNames,int ErrorType, string Selection,string NameVar){ 
	
	NameObs=NameVar;
	cout << "Name is : " << NameObs << endl;
	if(Selection=="entered"){
		for(int j = 0; j < SelectedTriggerNames.size(); j++){
			//if(prescaleTrigger[j]==1){
				auto it = find(triggerNames.begin(), triggerNames.end(), SelectedTriggerNames[j]);
				auto index = distance(triggerNames.begin(), it);
				ListTriggers[j] = (index);
				cout << "[" << j<< "," << index+1 << "]" << endl; 
			//}
		}
	}
/*		 Corrélation, fichier .txt commence ligne 1 et passtrig[] commence à 0	*/
	else if(Selection=="sel1"){
		for(int j = 0; j < 10; j++){
			ListTriggers[j] = (j);
		}
	}
	else if(Selection=="sel2"){
		for(int j = 0; j < 20; j++){
			ListTriggers[j] = (j);
			cout << "pos in map " << j << " pos of trigger " << j << endl; 
		}
	}
	else if(Selection=="sel3"){
		for(int j = 0; j < 30; j++){
			ListTriggers[j] = (j);
			cout << "pos in map " << j << " pos of trigger " << j << endl; 
		}
	}
	else if(Selection=="sel4"){
		for(int j = 0; j < 40; j++){
			ListTriggers[j] = (j);
			cout << "pos in map " << j << " pos of trigger " << j << endl; 
		}
	}

	else if(Selection=="all"){
		for(int j = 0; j < triggerNames.size(); j++){
			ListTriggers[j] = (j);
		}
	}


	NumCorr.resize(ListTriggers.size(), vector<double>(ListTriggers.size(), 0.0)); 
	DenomCorr.resize(ListTriggers.size(), vector<double>(ListTriggers.size(), 0.0));
	Correlation.resize(ListTriggers.size(), vector<double>(ListTriggers.size(), 0.0)); 
	
	NumEfficiency.resize(ListTriggers.size(), 0.0);
	DenomEfficiency.resize(ListTriggers.size(), 0.0);
	Efficiency.resize(ListTriggers.size(), 0.0);
	
	EffErr.resize(ListTriggers.size(), 0.0);

	this->TriggerNames = triggerNames;

	TString outputfilename="testnewdata.root";
	OutputHisto= new TFile(outputfilename,"RECREATE");


	//SelectedTriggerNames=str;
	//Selection=triggerNames;
	//************* Init of histograms ****************

	
	EffvsObs.resize(ListTriggers.size());
	
	if(NameVar!=""){
	
		for(int j=0; j < ListTriggers.size(); j++){

			EffvsObs[j] = new TEfficiency("Eff","Efficiency;x;#epsilon",200,0,2000);
			//Une fois qu'on a la liste de Tefficiency, on doit avoir la var entrée par l'utilisateur : (track_pt/.../...)
			}
		//Ensuite il faut les remplir, retour a Anaeff.cpp : 
	}

		

	EFF_TRIG = new TH1D("EFF_TRIG", "EFF", 100,0,1); 
	EFF_DISTRIB = new TH1D("Efficiency distribution for int trigs", "eff for triggers", ListTriggers.size(),0,ListTriggers.size());

	CORR = new TH2D("Correlation", " Correlation plot",  ListTriggers.size() , 0 , ListTriggers.size() , ListTriggers.size(), 0 , ListTriggers.size()); 
	
	EFF_TRIG->Sumw2();
	EFF_DISTRIB->Sumw2();
	CORR->Sumw2();
	
	if(ErrorType == 1 ){
		//cout << " We will use the most general error estimator " << endl;
	}
	
}
//*************************************************************

/*
Differentes méthodes fill (en fonction du string donné, est-ce que je peux pas simplement remplir la map dans des if/else if ?




*/

//*************************************************************





void TrigEff::Fill(const vector<bool> &passtrig, float Obs, double weight){  
	bool trig1,trig2;
	for(auto iter = ListTriggers.begin(); iter != ListTriggers.end() ;iter++){
		trig1 = passtrig[iter->second];
		DenomEfficiency[iter->first]+=1;
		if (trig1){
			NumEfficiency[iter->first]+=1;
		}
		for(auto jter = ListTriggers.begin(); jter != ListTriggers.end();jter++){
			trig2 = passtrig[jter->second];
			if(trig1 || trig2){
				DenomCorr[iter->first][jter->first]+=1;
			}
			if(trig1 && trig2){
				NumCorr[iter->first][jter->first]+=1;
			}
		}
	}
	


	
}


void TrigEff::StudyTrigvsMass(double mass){
	



}



void TrigEff::ComputeCorr(){
	int binx;
	for(int i=0;i< Correlation.size();i++){
		for(int j=0;j< Correlation[i].size();j++){
			if(DenomCorr[i][j]==0){
				Correlation[i][j]=0;
			}
			else{	
			Correlation[i][j] = ((NumCorr[i][j]*1.0) / DenomCorr[i][j]);
			}
			
			CORR->SetBinContent((i+1),(j+1),(Correlation[i][j]*100)); // Fill avec 
			//binx=CORR->GetXAxis;
			//transformer i et j en string ?
			//BinLabel en 2D ? SetBinLabel(binx, triggername[indice]) Puis il faut une boucle pour écrire le label I J a chaque fois ? 
		}
	}
	CORR->Write();
	OutputHisto->Close();
}

void TrigEff::PrintCorr(){
	cout << endl;
	for ( int i = 0; i < Correlation.size(); i++ ){
   		for ( int j = 0; j < Correlation[i].size(); j++ ){
      			cout << Correlation[i][j] * 100  << "% ";
   		}
   	cout << endl;
	}
	cout << endl;
}

void TrigEff::PrintNumCorr(){
	
	for ( int i = 0; i < NumCorr.size(); i++ ){
   		for ( int j = 0; j < NumCorr[i].size(); j++ ){
      			cout << NumCorr[i][j] << ' ';
   		}
   	cout << endl;
	}	
}

void TrigEff::PrintDenomCorr(){

	for ( int i = 0; i < DenomCorr.size(); i++ ){
   		for ( int j = 0; j < DenomCorr[i].size(); j++ ){
      			cout << "num:" << NumCorr[i][j] << "denom: " << DenomCorr[i][j] << ' ';
   		}
   	cout << endl;
	}
}



void TrigEff::ComputeEff()
{
	for(int i=0;i< Efficiency.size();i++){
		if(DenomEfficiency[i]==0){
			Efficiency[i]=0;
			//EFF_TRIG->Fill(Efficiency[i]);
		}
		else{	
			Efficiency[i] = ((NumEfficiency[i]*1.0) / DenomEfficiency[i]*1.0);
			//EFF_TRIG->Fill(Efficiency[i]);
		}
		
	}
	OutputHisto->cd();
	//EFF_TRIG->Write();
}



void TrigEff::PrintEff(){

	for(int i=0;i< Efficiency.size();i++){
		cout << Efficiency[i] *100 << "% " << "denom : " << DenomEfficiency[i] <<" error : " << EffErr[i] << endl;
	}
}



void TrigEff::SortEffVec(){
	
	for (int i = 0; i < Efficiency.size(); i++) { 
        	EffList.push_back(make_pair(Efficiency[i], make_pair(EffErr[i],TriggerNames[ListTriggers[i]])));
	}

	sort(EffList.begin(),EffList.end());
	cout << "Efficiency " << "\t" << "Error" << "\t" << "Trigger name" << endl; 
    	for (int i = 0; i < Efficiency.size(); i++) { 
        	cout << setprecision (8) << (EffList[i].first)*100 << "\t" << EffList[i].second.first << "\t" << EffList[i].second.second << endl; 
		}
    
}

void TrigEff::SaveIntTrigs(){
	int j=0;
	ofstream TriggersOfInterest;
	TriggersOfInterest.open ("TriggersOfInterest.txt");
	if (TriggersOfInterest.good()){
		for (int i = 0; i < Efficiency.size(); i++){ 
			//if(EffList[i].first >= 0.5 ){
			TriggersOfInterest << EffList[i].first << " " << EffList[i].second.first << " " << EffList[i].second.second << "\n";
			double effem=EffList[i].first;
			EFF_DISTRIB->SetBinContent(j,effem);
			j++;
			//}
    		}

	TriggersOfInterest.close();
	EFF_DISTRIB->Write();
	}

	else{
		cout << "File .txt was not opened, aborting" << endl;
	}
	
}







void TrigEff::PrintNumEff(){
	for ( int i = 0; i < NumEfficiency.size(); i++ ){
      		cout << NumEfficiency[i] << endl ;
	}	
}


void TrigEff::PrintDenomEff(){
	for ( int i = 0; i < DenomEfficiency.size(); i++ ){
      		cout << NumEfficiency[i] << " " << DenomEfficiency[i] << " " <<EffErr[i] << endl ;
	}
}


void TrigEff::ComputeError(){

	for(int i=0;i< EffErr.size();i++){
		cout << NumEfficiency[i] << " " << DenomEfficiency[i] << endl ;
		if(Efficiency[i]==0){
			EffErr[i]=0;
		}
		else{
			EffErr[i]=sqrt((Efficiency[i]*(1.0-Efficiency[i]))/DenomEfficiency[i]);
		}
	}
}

void TrigEff::GetPlot(string SelectedTriggerNames){
	//utiliser cette fonction pour plot l'efficacité en fonction de la masse : entrer le trigger (nom), il obtient son efficacité et à chaque fois il la met dans un TGRAPH avec la masse ( calculée à base de Ih et des deux coefficients dans la publication 1 envoyée), on peut donc avoir l'efficacité en fonction de la masse 

	
	

}

void TrigEff::Compute(){
	ComputeEff();
	ComputeError();
	
	//PrintNumEff();
	//PrintDenomEff();
	SortEffVec();
	//PrintEff();
	SaveIntTrigs();

	ComputeCorr();
	//PrintDenomCorr();
	PrintCorr();
	
}

