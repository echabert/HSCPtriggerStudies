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

	
	
	selection.clear();
	selectedtriggernames.clear();

	efflist.clear();
	
	triggernames.clear();
	triggerpass.clear();

	correlation.clear();
	num_corr.clear();
	denom_corr.clear();

	corr_err.clear();

	efficiency.clear();
	
	num_efficiency.clear();
	denom_efficiency.clear();
	eff_err.clear();
	


	/*if(!EFF_TRIG){
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
	}*/
}


/*void TrigEff::Init(int ntrigger,bool *passtrig){
	for(int i=0;i<ntrigger; i++){ 
		triggerpass.push_back(passtrig[i]);
		cout << "bool[" << i << "] = "  << triggerpass[i] << endl;
	}

}*/

void TrigEff::Load(vector<string> triggerNames,vector<string> selection,int error_type){ 
	
	num_corr.resize(triggerNames.size(), vector<double>(triggerNames.size(), 0.0)); // mettre map.size() dans les vecteurs
	denom_corr.resize(triggerNames.size(), vector<double>(triggerNames.size(), 0.0));
	correlation.resize(triggerNames.size(), vector<double>(triggerNames.size(), 0.0)); // taille après avoir récupérer la liste des trigg of interest
	num_efficiency.resize(triggerNames.size(), 0.0);
	denom_efficiency.resize(triggerNames.size(), 0.0);
	efficiency.resize(triggerNames.size(), 0.0);
	
	eff_err.resize(triggerNames.size(), 0.0);

	this->triggernames = triggerNames;

	TString outputfilename="testnewdata.root";
	OutputHisto= new TFile(outputfilename,"RECREATE");

	//************* Init of histograms ****************


	/*EFF_TRIG = new TH1D("EFF_TRIG", "EFF", 100,0,1); 
	EFF_DISTRIB = new TH1D("Efficiency distribution for int trigs", "eff for triggers", 100,0,100);

	TH2D* CORR = new TH2D("Correlation", " Correlation plot",  triggernames.size() , 0 , triggernames.size() , triggernames.size(), 0 , triggernames.size()); 
	
	EFF_TRIG->Sumw2();
	EFF_DISTRIB->Sumw2();
	CORR->Sumw2();*/
	
	if(error_type == 1 ){
		//cout << " We will use the most general error estimator " << endl;
	}
	
	int count=0;

	for(unsigned int curline=0; curline < triggerNames.size();curline++){
		for(unsigned int j=0; j < selection.size();j++){
			if(triggerNames[curline] == selection[j]){
				//cout << "Found  " << selection[j] << " in line  " << curline+1 << endl;
				count+=1;
				currentlines.push_back(curline);
			}
		}
	}
	if(selection.size()==1){
		if(count!= selection.size()) cout <<  "Trigger was not found  " << endl;
	}
	else if(count!= selection.size()) cout <<  "Triggers were not found  " << endl;
}




/*void TrigEff::Fill(const vector<bool> &passtrig, string obs, double weight){ // Fill(map<int,int> ListTriggers
	bool trig1,trig2;
	for(int i=0;i< passtrig.size();i++){
		trig1 = passtrig.at(i);
		denom_efficiency[i]+=1;
		if (trig1){
			num_efficiency[i]+=1;
		}
	
		for(int j=0;j< passtrig.size();j++){
			trig2 = passtrig.at(j);
			if(trig1 || trig2){
				denom_corr[i][j]+=1;
			}
			if(trig1 && trig2){
				num_corr[i][j]+=1;
			}
		}
	}
}*/

//****************************************************************************




void TrigEff::Fill(map<int,int> ListTriggers, string obs, double weight){ 
	bool trig1,trig2,transf=0;
	auto iter = ListTriggers.begin();
   	while (iter != ListTriggers.end()) {
		//cout << "[" << iter->first << "," << iter->second <<"]" << endl;
		if(iter->second==1){
			transf=true;
		}
		else{
			transf=false;
		}
		//cout << "bool is " << transf << endl;
		trig1 = transf;
		denom_efficiency[iter->first]+=1;
		//cout << "Trigger :" << iter->first << " num : " << num_efficiency[iter->first] <<" , denom : " << denom_efficiency[iter->first] << endl;
		if (trig1){
			num_efficiency[iter->first]+=1;
		}
		++iter;
    	}
		
		/*trig1 = ListTriggers[i].second;
		denom_efficiency[ListTriggers[i].first]+=1;
		if (trig1){
			num_efficiency[ListTriggers[i].first]+=1;
		}*/
	
		/*for(int j=0;j< passtrig.size();j++){
			trig2 = passtrig.at(j);
			if(trig1 || trig2){
				denom_corr[i][j]+=1;
			}
			if(trig1 && trig2){
				num_corr[i][j]+=1;
			}
		}*/
	
}


//***************************************************************************





void TrigEff::PrescaleSelection(vector<string> selection){
	//ofstream prescaledtriggers;
	//prescaledtriggers.open ("PrescaledTriggers.txt");
	
	for(int j = 0; j <selection.size(); j++){
		//if(prescaleTrigger[j]==1){
			
			for ( int i =0; i < triggernames.size(); i++){
			//
			//ListTriggers[j] = 
			//selection[n]
			//ici map int int, find le nom du trigger triggernames.find()
			//prescaledtriggers << j << " " << trigEff_presel.selectedtriggernames[j] << endl;
			}
		//}
	}
	//prescaledtriggers.close();

}




void TrigEff::ComputeCorr(){

	for(int i=0;i< correlation.size();i++){
		for(int j=0;j< correlation[i].size();j++){
			if(denom_corr[i][j]==0){
				correlation[i][j]=0;
			}
			else{	
			correlation[i][j] = ((num_corr[i][j]*1.0) / denom_corr[i][j]);
			}
			
			//CORR->Fill( // Fill avec 
		}
	}
}

void TrigEff::PrintCorr(){

	for ( int i = 0; i < correlation.size(); i++ ){
   		for ( int j = 0; j < correlation[i].size(); j++ ){
      			cout << correlation[i][j] * 100  << "% ";
   		}
   	cout << endl;
	}
}

void TrigEff::PrintNumCorr(){
	
	for ( int i = 0; i < num_corr.size(); i++ ){
   		for ( int j = 0; j < num_corr[i].size(); j++ ){
      			cout << num_corr[i][j] << ' ';
   		}
   	cout << endl;
	}	
}

void TrigEff::PrintDenomCorr(){

	for ( int i = 0; i < denom_corr.size(); i++ ){
   		for ( int j = 0; j < denom_corr[i].size(); j++ ){
      			cout << denom_corr[i][j] << ' ';
   		}
   	cout << endl;
	}
}



void TrigEff::ComputeEff()
{
	for(int i=0;i< efficiency.size();i++){
		if(denom_efficiency[i]==0){
			efficiency[i]=0;
			//EFF_TRIG->Fill(efficiency[i]);
		}
		else{	
			efficiency[i] = ((num_efficiency[i]*1.0) / denom_efficiency[i]*1.0);
			//EFF_TRIG->Fill(efficiency[i]);
		}
		
	}
	//OutputHisto->cd();
	//EFF_TRIG->Write();
}



void TrigEff::PrintEff(){

	for(int i=0;i< efficiency.size();i++){
		cout << efficiency[i] *100 << "% " << "denom : " << denom_efficiency[i] <<" error : " << eff_err[i] << endl;
	}
}



void TrigEff::SortEffVec(){
	for (int i = 0; i < efficiency.size(); i++) { 
        	efflist.push_back(make_pair(efficiency[i], make_pair(eff_err[i],triggernames[i])));
	}
	sort(efflist.begin(),efflist.end());
	cout << "Efficiency " << "\t" << "Error" << "\t" << "Trigger name" << endl; 
    	for (int i = 0; i < efficiency.size(); i++) { 
        	cout << setprecision (8) << efflist[i].first << "\t\t" << efflist[i].second.first << "\t" << efflist[i].second.second << endl; 
		}
    
}

void TrigEff::SaveIntTrigs(){
	int j=0;
	ofstream TriggersOfInterest;
	TriggersOfInterest.open ("TriggersOfInterest.txt");
	cout << "After open .txt" << endl;
	
	if (TriggersOfInterest.good()){
		
		for (int i = 0; i < efficiency.size(); i++) { 
			if(efflist[i].first >= 0.5 ){
			TriggersOfInterest << efflist[i].first << " " << efflist[i].second.first << " " << efflist[i].second.second << "\n";
			double effem=efflist[i].first;
			//EFF_DISTRIB->SetBinContent(j,effem);
			j++;
			}
    		}

	TriggersOfInterest.close();
	//EFF_DISTRIB->Write();
	//OutputHisto->Close();
	}

	else{
		cout << "File .txt was not opened, aborting" << endl;
	}
}

void TrigEff::PrintSpecEff(vector<int> currentlines){
		for(int i=0;i<currentlines.size();i++){
			cout << "Trigger # "<< currentlines[i]+1 << " has e = " << efficiency[currentlines[i]] *100 << "% " << "+/- " << eff_err[currentlines[i]]*100 << "% " << endl;
		}
}


/*void TrigEff::PrintSpecEff(vector<int> currentlines){
		for(int i=0;i<currentlines.size();i++){
			cout << "Trigger # "<< ListTriggers[i]+1 << " has e = " << efficiency[currentlines[i]] *100 << "% " << "+/- " << eff_err[currentlines[i]]*100 << "% " << endl;
		}
}*/







void TrigEff::PrintNumEff(){
	for ( int i = 0; i < num_efficiency.size(); i++ ){
      		cout << num_efficiency[i] << endl ;
	}	
}


void TrigEff::PrintDenomEff(){
	for ( int i = 0; i < denom_efficiency.size(); i++ ){
      		cout << num_efficiency[i] << " " << denom_efficiency[i] << " " <<eff_err[i] << endl ;
	}
}


void TrigEff::ComputeError(){

	for(int i=0;i< eff_err.size();i++){
		//cout << num_efficiency[i] << " " << denom_efficiency[i] << endl ;
		if(efficiency[i]==0){
			eff_err[i]=0;
		}
		else{
			eff_err[i]=sqrt((efficiency[i]*(1.0-efficiency[i]))/denom_efficiency[i]);
		}
	}
}

void TrigEff::GetPlot(string selection){
	//utiliser cette fonction pour plot l'efficacité en fonction de la masse : entrer le trigger (nom), il obtient son efficacité et à chaque fois il la met dans un TGRAPH avec la masse ( calculée à base de Ih et des deux coefficients dans la publication 1 envoyée), on peut donc avoir l'efficacité en fonction de la masse 

	
	//cout << "Its efficiency is : " << efficiency[currentlines] *100 << "% " << " +/- [error]: " << eff_err[currentlines] << endl;
	

}

void TrigEff::Compute(){
	ComputeEff();
	ComputeError();

	//PrintNumEff();
	//PrintDenomEff();
	//SortEffVec();
	PrintEff();
	PrintSpecEff(currentlines);
	//SaveIntTrigs();
	//ComputeCorr();
	//PrintCorr();
	
}

