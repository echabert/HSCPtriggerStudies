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
#include <TPad.h>
#include <TCanvas.h>
#include <TROOT.h> 
#include <TTree.h>
#include <TFile.h>
#include <TEntryList.h>

using namespace std;



TrigEff::TrigEff(){
	EFF_TRIG=0;
	EFF_DISTRIB=0;
	CORR=0;
	OutputHisto=0;
	FITBG2=0;
	MASS=0;
	SOLOM=0;
	/*for(int i=0; i < ListTriggers.size() ; i++){
		EffvsObs[i]=0;
	}*/
}



TrigEff::~TrigEff(){ 

	
	
	SelectedTriggerNames.clear();
	
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
	
	/*for(int i=0; i < ListTriggers.size() ; i++){
		if(!EffvsObs[i]){
			delete EffvsObs[i];
		}
	}*/

	EffvsObs.clear(); 

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
	if(!MASS){
		delete MASS;
	}
	if(!FITBG2){
		delete FITBG2;
	}
	if(!SOLOM){
		delete SOLOM;
	}
	
}



/*void TrigEff::CreateHisto(string NameVar,const vector<string> &SelectedTriggerNames){

	TString outputfilename="Efficiency_results.root";

	OutputHisto = new TFile(outputfilename,"RECREATE");

	if(NameVar!=""){
		for(int j=0; j < ListTriggers.size(); j++){

			EffvsObs[j] = new TEfficiency("Eff","Efficiency;MET;#epsilon",200,0,2000);
			EffvsObs[j]->SetName(SelectedTriggerNames[j].c_str());
			
//EffvsObs[j]->Draw();
			//gPad->Update();
			//EffvsObs[j]->GetPaintedGraph()->GetXaxis()->SetTitle(NameObs.c_str());

		
			}

		
	}
	
}*/


//*************************************************************
//Dans Load : Selection va être le nom de la variable qu'on veut étudier. Dans Fill on va mettre la valeur


void TrigEff::LoadNoMap(const vector<string> &triggerNames, const vector<string> &SelectedTriggerNames,int ErrorType, string NameVar,string FileName){ 
	NameObs=NameVar;
	int nbins=70;
	int massmax=140;
	cout << "Name is : " << NameObs << endl;
	
	for(int j = 0; j < SelectedTriggerNames.size(); j++){
			auto it = find(triggerNames.begin(), triggerNames.end(), SelectedTriggerNames[j]);
			auto index = distance(triggerNames.begin(), it);
			TestNoMap.push_back(make_pair(j,index));
			cout << "[" << j<< "," << index+1 << "]" << endl; 
			
		}
	
	NumCorr.resize(triggerNames.size(), vector<double>(triggerNames.size(), 0.0)); 
	DenomCorr.resize(triggerNames.size(), vector<double>(triggerNames.size(), 0.0));
	Correlation.resize(triggerNames.size(), vector<double>(triggerNames.size(), 0.0)); 
	
	NumEfficiency.resize(triggerNames.size(), 0.0);
	DenomEfficiency.resize(triggerNames.size(), 0.0);
	Efficiency.resize(triggerNames.size(), 0.0);
	
	EffErr.resize(triggerNames.size(), 0.0);
	EffvsObs.resize(triggerNames.size());

	this->TriggerNames = triggerNames;
	
	TString outputfilename=FileName.c_str();

	OutputHisto = new TFile(outputfilename,"RECREATE");
	

	//SelectedTriggerNames=str;
	//Selection=triggerNames;
	//************* Init of histograms ****************

	
	if(NameVar!=""){
		cout << "selection size : " << TestNoMap.size() << endl;
		for(int j=0; j < TestNoMap.size(); j++){ //selected trigger names
			if(NameVar=="PT"){
				EffvsObs[j] = new TEfficiency("Eff","Efficiency;PT;#epsilon",50,0,2000); 
			}
			if(NameVar=="MET"){
				EffvsObs[j] = new TEfficiency("Eff","Efficiency;MET;#epsilon",50,0,2000);
			}
			if(NameVar=="MU"){
				EffvsObs[j] = new TEfficiency("Eff","Efficiency;MUPT;#epsilon",50,0,2000);
			}
			EffvsObs[j]->SetName(TriggerNames[TestNoMap[j].second].c_str());

			//EffvsObs[j]->Draw("AP");
			//gPad->Update();
			//EffvsObs[j]->GetPaintedGraph()->GetXaxis()->SetTitle(NameObs.c_str());
			}
	}
	
	EFF_TRIG = new TH1D("EFF_TRIG", "EFF", 100,0,1); 
	EFF_DISTRIB = new TH1D("Efficiency distribution for int trigs", "eff for triggers", TriggerNames.size(),0,TriggerNames.size());
	CORR = new TH2D("Correlation", "Correlation plot",  TriggerNames.size() , 0 , TriggerNames.size() , TriggerNames.size(), 0 , TriggerNames.size()); 
	
	MASS = new TH1D("MASS" , " Masses invariante des muons" , nbins , 0 , massmax);
	MASS->GetXaxis()->SetTitle("Mass [GeV]");
	MASS->GetYaxis()->SetTitle(" # candidates");

	cout << "end of load" << endl;
	MASS->Sumw2();
	EFF_TRIG->Sumw2();
	EFF_DISTRIB->Sumw2();
	CORR->Sumw2();
	
	if(ErrorType == 1 ){
	}
	
}




//***************************************************************************************************************************


void TrigEff::FillNoMap(vector<bool> &passtrig, float Obs, double weight,string mode){  //const vector<bool> 
	if(mode== "all"){ 
		bool trig1,trig2;
		for(int i = 0; i < TestNoMap.size()  ;i++){
			trig1 = passtrig.at(TestNoMap[i].second);
			DenomEfficiency[TestNoMap[i].second]+=1;
			if(trig1){
				NumEfficiency[TestNoMap[i].second]+=1;
			}

			for(int j = 0; j < TestNoMap.size()  ;j++){
				trig2 = passtrig.at(TestNoMap[j].second);
				if(trig1 || trig2){
					DenomCorr[TestNoMap[i].second][TestNoMap[j].second]+=1;
				}
				if(trig1 && trig2){
					NumCorr[TestNoMap[i].second][TestNoMap[j].second]+=1;
				}
	
			}
		}

		if(Obs!=0.0){
			for(int i = 0 ; i < TestNoMap.size(); i++){
				//cout << "filled passtrig :" << i << "with value " << passtrig[i] << "and obs = " << Obs << endl;
				EffvsObs[i]->TEfficiency::Fill(passtrig[TestNoMap[i].second],Obs);
			}
		}

	}
	if(mode == "muon"){
		bool trigmu1,trigmu2;
		//for(int i = 0; i < TestNoMap.size()  ;i++){
		
		//trigmu1 = passtrig.at(TestNoMap[i].second);

		//}
	}
}


void TrigEff::StudyTrigvsMass(double mass){

}

void TrigEff::ComputeCorr(){
	for(int i=0;i< Correlation.size();i++){
		for(int j=0;j< Correlation[i].size();j++){
			if(DenomCorr[i][j]==0){
				Correlation[i][j]=0;
			}
			else{	
			Correlation[i][j] = ((NumCorr[i][j]*1.0) / DenomCorr[i][j]);
			}

		}
	}
	
	
}

void TrigEff::PrintCorr(){
	cout << endl;
	for ( int i = 0; i < Correlation.size(); i++ ){
   		for ( int j = 0; j < Correlation[i].size(); j++ ){
      			cout << "[" << i << "," << j << "] : " <<Correlation[i][j] * 100  << "% ";
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
		if(DenomEfficiency[i]==0 && NumEfficiency[i] == 0){
			Efficiency[i]=0;
		}
		else if(DenomEfficiency[i]==0){
			Efficiency[i]=0;
		}
		else{	
			Efficiency[i] = ((NumEfficiency[i]*1.0) / DenomEfficiency[i]*1.0);
		}
		
	}
	
}

void TrigEff::PrintEff(){

	for(int i=0;i< Efficiency.size();i++){
		cout << Efficiency[i] *100 << "% " << "denom : " << DenomEfficiency[i] <<" error : " << EffErr[i] << endl;
	}
}


void TrigEff::SaveIntTrigs(string NameOutputFile){
	
	ofstream TriggersOfInterest;
	ofstream AllTriggers;
	
	TriggersOfInterest.open (NameOutputFile.c_str());
	AllTriggers.open ("ListOfAllTriggersEff.txt");
	
	for (int i = 0; i < Efficiency.size(); i++){
		EffList.push_back(make_pair(make_pair(Efficiency[i],i), make_pair(EffErr[i],TriggerNames[i])));
		//if(EffList[i].first >= 0.5 ){
		
		//}
    	}

	sort(EffList.begin(),EffList.end());

	for (int i = 0; i < Efficiency.size(); i++){
		//cout << setprecision (8) << (EffList[i].first.first)*100 << "\t\t" << setprecision (8) << (EffList[i].second.first)*100 << "\t\t" << EffList[i].second.second << endl; 
		AllTriggers << EffList[i].first.first*100 << " " << EffList[i].second.first*100 << " " << EffList[i].second.second << "\n"; //TestNoMap[i].second
	}

	AllTriggers.close();

	for (int i = 0; i < TestNoMap.size(); i++){ 	
		TransferVec.push_back(make_pair(make_pair(Efficiency[TestNoMap[i].second],TestNoMap[i].second), make_pair(EffErr[TestNoMap[i].second],TriggerNames[TestNoMap[i].second])));
		cout << setprecision (8) << (TransferVec[i].first.first)*100 << "\t\t" << setprecision (8) << (TransferVec[i].second.first)*100 << "\t\t" << TransferVec[i].second.second << endl; 		
		TriggersOfInterest << (TransferVec[i].first.first)*100 << " " << (TransferVec[i].second.first)*100 << " " << TransferVec[i].second.second << "\n";
    	}

	//sort(TransferVec.begin(), TransferVec.end());
		
	/*for(int i = 0; i < TransferVec.size(); i++){ 
		TriggersOfInterest <<  TransferVec[i].first.first*100 << " " << TransferVec[i].second.first*100 << " " << TransferVec[i].second.second << "\n"; //TestNoMap[i].second
	}*/

	TransferVec.clear();
	TriggersOfInterest.close();

	
}

void TrigEff::PrintNumEff(){
	for ( int i = 0; i < NumEfficiency.size(); i++ ){
      		cout << NumEfficiency[i] << " / " << DenomEfficiency[i] << endl ;
	}	
}


void TrigEff::PrintDenomEff(){
	for ( int i = 0; i < DenomEfficiency.size(); i++ ){
      		cout << i << " : " << NumEfficiency[i] << " / " << DenomEfficiency[i] << " +/- " << EffErr[i] << endl ;
	}
}

void TrigEff::ComputeError(){
	for(int i=0;i< EffErr.size();i++){
		if(Efficiency[i]==0){
			EffErr[i]=0;
		}
		else{
			EffErr[i]=sqrt((Efficiency[i]*(1.0-Efficiency[i]))/DenomEfficiency[i]);
		}
	}
}

void TrigEff::WritePlots(string NameVar,string NameOfFile){ //TFile* OutputHisto
	OutputHisto->cd();
	//gDirectory->mkdir("MET");
	//OutputHisto->mkdir("MET");
	//OutputHisto->cd("MET");
	//OutputHisto->mkdir(NameVar.c_str());
	//OutputHisto->cd(NameVar.c_str());

	for(int i=0;i < TestNoMap.size();i++){
		EffvsObs[i]->Write();
	}

	for(int i=0;i < Correlation.size();i++){
		for(int j=0;j< Correlation[i].size();j++){
			CORR->SetBinContent((i),(j),(Correlation[i][j]*100));
		}
	}
	//CORR->SetDirectory("Correlations");
	CORR->Write();
	MASS->Write();
	OutputHisto->Close();


}

void TrigEff::FillMass(double INVMASS,int choice){
	if(choice==1){
		MASS->Fill(INVMASS);
	}
}

void TrigEff::Compute(string NameOutputFile){
	
	ComputeEff();
	ComputeError();
	
	//PrintNumEff();
	PrintDenomEff();
	//PrintEff();

	SaveIntTrigs(NameOutputFile);

	ComputeCorr();

	//PrintDenomCorr();
	//PrintCorr();

}

