#include "inc/TrigEff.h"

#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <stdio.h>
#include <array>
#include <cstdlib>
#include <iomanip>
#include <ctime>
#include <cmath>
#include <fstream>
#include <algorithm>
#include <iostream>
#include <fstream>

using namespace std;



TrigEff::TrigEff(){
	//EFF_TRIG=NULL;
}



TrigEff::~TrigEff(){ 

	num_corr.clear();
	denom_corr.clear();

	correlation.clear();

	num_efficiency.clear();
	denom_efficiency.clear();

	efficiency.clear();

	eff_err.clear();

	triggernames.clear();
	
}


/*void TrigEff::Init(int ntrigger,bool *passtrig){
	for(int i=0;i<ntrigger; i++){ 
		triggerpass.push_back(passtrig[i]);
		cout << "bool[" << i << "] = "  << triggerpass[i] << endl;
	}

}*/

void TrigEff::Load(vector<string> triggerNames,vector<string> selection,int error_type){ 
	
	num_corr.resize(triggerNames.size(), vector<double>(triggerNames.size(), 0.0));
	denom_corr.resize(triggerNames.size(), vector<double>(triggerNames.size(), 0.0));
	correlation.resize(triggerNames.size(), vector<double>(triggerNames.size(), 0.0));
	
	num_efficiency.resize(triggerNames.size(), 0.0);
	denom_efficiency.resize(triggerNames.size(), 0.0);
	efficiency.resize(triggerNames.size(), 0.0);
	
	eff_err.resize(triggerNames.size(), 0.0);


	/*EFF_TRIG = new TH1D("EFF_TRIG", "EFF", 100,0,1);
	EFF_TRIG->Sumw2();*/

	if(error_type == 1 ){
		//cout << " We will use the most general error estimator " << endl;
	}

	this->triggernames = triggerNames;

	//find(triggerNames.begin(), triggerNames.end(), selection) != triggerNames.end();
	//triggerNames.find(selection);

	// if selection is not given, then 
	
	/*for(unsigned int j=0; j < selection.size();j++){
		if (find(triggerNames.begin(),triggerNames.end(), selection[j]) != triggerNames.end())
			cout << "Found  " << selection[j] << endl;
			
	}*/

	int count=0;

	for(unsigned int curline=0; curline < triggerNames.size();curline++){
		for(unsigned int j=0; j < selection.size();j++){
			if(triggerNames[curline] == selection[j]){
				cout << "Found  " << selection[j] << " in line  " << curline+1 << endl;
				count+=1;
				column.push_back(curline);
			}	
		}
	}

	if(count!= selection.size()) cout <<  "Not all triggers were found  " << endl;
	

}




void TrigEff::Fill(vector<bool> passtrig, double obs, double weight){
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

	//EFF_TRIG->Write();
}



void TrigEff::PrintEff(){

	for(int i=0;i< efficiency.size();i++){
		cout << efficiency[i] *100 << "% " << "denom : " << denom_efficiency[i] <<" error : " << eff_err[i] << endl;
	}
}



void TrigEff::SortEffVec(){
	for (int i = 0; i < efficiency.size(); ++i) { 
        	efflist.push_back(make_pair(efficiency[i], make_pair(eff_err[i],triggernames[i])));
	}
	sort(efflist.begin(),efflist.end());
	cout << "Efficiency " << "\t" << "Error" << "\t" << "Trigger name" << endl; 
    	for (int i = 0; i < efficiency.size(); i++) { 
        	cout << efflist[i].first << "\t" << efflist[i].second.first << "\t" << efflist[i].second.second << endl; 
		}
    
}

void TrigEff::SaveIntTrigs(){
	ofstream TriggersOfInterest;
	TriggersOfInterest.open ("TriggersOfInterest.txt");
	for (int i = 0; i < efficiency.size(); i++) { 
		if(efflist[i].first >= 0.6 ){ 
		TriggersOfInterest << efflist[i].first << " " << efflist[i].second.first << " " << efflist[i].second.second << "\n";
		}
    }
	TriggersOfInterest.close();

}

void TrigEff::PrintSpecEff(vector<int> column){
		for(int i=0;i<column.size();i++){
			cout << "Trigger # "<< column[i]+1 << " has e = " << efficiency[column[i]] *100 << "% " << "+/- " << eff_err[column[i]]*100 << "% " << endl;
		}
}



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
		eff_err[i]=sqrt((efficiency[i]*(1.0-efficiency[i]))/denom_efficiency[i]);
		//cout << "error:" << eff_err[i] << endl;
	}
}

void TrigEff::GetPlot(string selection){
	//utiliser cette fonction pour plot l'efficacité en fonction de la masse : entrer le trigger (nom), il obtient son efficacité et à chaque fois il la met dans un TGRAPH avec la masse ( calculée à base de Ih et des deux coefficients dans la publication 1 envoyée), on peut donc avoir l'efficacité en fonction de la masse 

	
	//cout << "Its efficiency is : " << efficiency[column] *100 << "% " << " +/- [error]: " << eff_err[column] << endl;
	

}

void TrigEff::Compute(){
	ComputeEff();
	ComputeError();
	
	SortEffVec();
	//PrintEff();
	
	PrintSpecEff(column);
	SaveIntTrigs();
	
	ComputeCorr();
	//PrintCorr();
	
}

