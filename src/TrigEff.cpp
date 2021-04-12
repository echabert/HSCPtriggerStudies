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


using namespace std;



TrigEff::TrigEff(){
	EFF_TRIG=0;
	EFF_DISTRIB=0;
	CORR=0;
	OutputHisto=0;
	FITBG2=0;
	MASS=0;

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
	cout << "Name is : " << NameObs << endl;
	

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
		cout << "selection size : " << TriggerNames.size() << endl;
		for(int j=0; j < TriggerNames.size(); j++){ //selected trigger names
			if(NameVar=="PT"){
				EffvsObs[j] = new TEfficiency("Eff","Efficiency;PT;#epsilon",50,0,2000); 
			}
			if(NameVar=="MET"){
				EffvsObs[j] = new TEfficiency("Eff","Efficiency;MET;#epsilon",50,0,2000);
			}
			EffvsObs[j]->SetName(TriggerNames[j].c_str());

			//EffvsObs[j]->Draw("AP");
			//gPad->Update();
			//EffvsObs[j]->GetPaintedGraph()->GetXaxis()->SetTitle(NameObs.c_str());
			}
	}
	
	EFF_TRIG = new TH1D("EFF_TRIG", "EFF", 100,0,1); 
	EFF_DISTRIB = new TH1D("Efficiency distribution for int trigs", "eff for triggers", TriggerNames.size(),0,TriggerNames.size());
	CORR = new TH2D("Correlation", "Correlation plot",  TriggerNames.size() , 0 , TriggerNames.size() , TriggerNames.size(), 0 , TriggerNames.size()); 
	MASS = new TH1D("MASS" , " Masses invariante des muons" , 60 , 0 , 120);

	MASS->Sumw2();
	EFF_TRIG->Sumw2();
	EFF_DISTRIB->Sumw2();
	CORR->Sumw2();
	
	if(ErrorType == 1 ){
	}
	
}




//***************************************************************************************************************************
void TrigEff::Load(const vector<string> &triggerNames,const vector<string> &SelectedTriggerNames,int ErrorType, string Selection,string NameVar,string FileName){ 
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

	
	NumCorr.resize(ListTriggers.size(), vector<double>(ListTriggers.size(), 0.0)); 
	DenomCorr.resize(ListTriggers.size(), vector<double>(ListTriggers.size(), 0.0));
	Correlation.resize(ListTriggers.size(), vector<double>(ListTriggers.size(), 0.0)); 
	
	NumEfficiency.resize(ListTriggers.size(), 0.0);
	DenomEfficiency.resize(ListTriggers.size(), 0.0);
	Efficiency.resize(ListTriggers.size(), 0.0);
	
	EffErr.resize(ListTriggers.size(), 0.0);
	EffvsObs.resize(ListTriggers.size());

	

	this->TriggerNames = triggerNames;
	

	
	TString outputfilename=FileName.c_str();

	OutputHisto = new TFile(outputfilename,"RECREATE");
	

	//SelectedTriggerNames=str;
	//Selection=triggerNames;
	//************* Init of histograms ****************

	
	if(NameVar!=""){
		for(int j=0; j < ListTriggers.size(); j++){
			if(NameVar=="PT"){
				EffvsObs[j] = new TEfficiency("Eff","Efficiency;PT;#epsilon",50,0,2000); // changer le titre MET/PT !!
			}
			if(NameVar=="MET"){
				EffvsObs[j] = new TEfficiency("Eff","Efficiency;MET;#epsilon",50,0,2000);
			}
			EffvsObs[j]->SetName(SelectedTriggerNames[j].c_str());

			//EffvsObs[j]->Draw("AP");
			//gPad->Update();
			//EffvsObs[j]->GetPaintedGraph()->GetXaxis()->SetTitle(NameObs.c_str());
			}
	}
	
	EFF_TRIG = new TH1D("EFF_TRIG", "EFF", 100,0,1); 
	EFF_DISTRIB = new TH1D("Efficiency distribution for int trigs", "eff for triggers", ListTriggers.size(),0,ListTriggers.size());
	CORR = new TH2D("Correlation", "Correlation plot",  ListTriggers.size() , 0 , ListTriggers.size() , ListTriggers.size(), 0 , ListTriggers.size());

	MASS = new TH1D("MASS" , " Z Mass from Z->mu mu decay" , 80 , 0 , 160);
	MASS->GetXaxis()->SetTitle("M [GeV]");
	MASS->GetYaxis()->SetTitle(" # candidates");
	
	MASS->Sumw2();
	EFF_TRIG->Sumw2();
	EFF_DISTRIB->Sumw2();
	CORR->Sumw2();
	
	if(ErrorType == 1 ){
	}
	
}



void TrigEff::FillNoMap(const vector<bool> &passtrig, float Obs, double weight){  
	bool trig1,trig2;
	for(int i = 0; i < passtrig.size()  ;i++){
		trig1 = passtrig.at(i);
		DenomEfficiency[i]+=1;
		if(trig1){
			NumEfficiency[i]+=1;
		}
	
		for(int j = 0; j < passtrig.size()  ;j++){
			trig2 = passtrig.at(j);
			if(trig1 || trig2){
				DenomCorr[i][j]+=1;
			}
			if(trig1 && trig2){
				NumCorr[i][j]+=1;
			}
	
		}
	}
	
	if(Obs!=0.0){
		for(int i = 0 ; i < TriggerNames.size(); i++){
			//cout << "filled passtrig :" << i << "with value " << passtrig[i] << "and obs = " << Obs << endl;
			EffvsObs[i]->TEfficiency::Fill(passtrig[i],Obs);
		}

	}
}







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
	
	if(Obs!=0.0){
		for(auto ster = ListTriggers.begin() ; ster != ListTriggers.end(); ster++){
			EffvsObs[ster->first]->TEfficiency::Fill(passtrig[ster->second],Obs);
		}
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
		if(DenomEfficiency[i]==0){
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






void TrigEff::SortEffVec(){
	
	for (int i = 0; i < Efficiency.size(); i++) { 
        	EffList.push_back(make_pair(Efficiency[i], make_pair(EffErr[i],TriggerNames[i]))); // ListTriggers[i] if we work with a map
	}
	
	sort(EffList.begin(),EffList.end());
	cout << "Efficiency " << "\t\t" << "Error" << "\t\t\t" << "Trigger name" << endl; 
    	for (int i = 0; i < Efficiency.size(); i++) { 
        	cout << setprecision (8) << (EffList[i].first)*100 << "\t\t" << setprecision (8) << (EffList[i].second.first)*100 << "\t\t" << EffList[i].second.second << endl; 
		}
    
}



void TrigEff::SaveIntTrigs(string NameOutputFile){
	int j=0;
	ofstream TriggersOfInterest;
	TriggersOfInterest.open (NameOutputFile.c_str());
	if (TriggersOfInterest.good()){
		for (int i = 0; i < Efficiency.size(); i++){ 
			//if(EffList[i].first >= 0.5 ){
			TriggersOfInterest << EffList[i].first*100 << " " << EffList[i].second.first*100 << " " << EffList[i].second.second << "\n";
			double effem=EffList[i].first;
			j++;
			//}
    		}

	TriggersOfInterest.close();
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
		//cout << NumEfficiency[i] << " " << DenomEfficiency[i] << endl ;
		if(Efficiency[i]==0){
			EffErr[i]=0;
		}
		else{
			EffErr[i]=sqrt((Efficiency[i]*(1.0-Efficiency[i]))/DenomEfficiency[i]);
		}
	}
}

void TrigEff::WritePlots(string NameVar){ //TFile* OutputHisto

	OutputHisto->cd();
	//gDirectory->mkdir("MET");
	//OutputHisto->mkdir("MET");
	//OutputHisto->cd("MET");

	
	/*OutputHisto->mkdir(NameVar.c_str());
	OutputHisto->cd(NameVar.c_str());*/
	
	
	for(int i=0;i < TriggerNames.size();i++){
		EffvsObs[i]->Write();
	}

	//OutputHisto->cd();
	
	/*OutputHisto->mkdir("Correlations");
	OutputHisto->cd("Correlations");*/
	

	for(int i=0;i < Correlation.size();i++){
		for(int j=0;j< Correlation[i].size();j++){
			CORR->SetBinContent((i+1),(j+1),(Correlation[i][j]*100));
		}
	}
	//CORR->SetDirectory("Correlations");
	CORR->Write();
	MASS->Write();
	//OutputHisto->cd();
	//cout << "right before closing  outputhisto" << endl;
	OutputHisto->Close();
	//cout << "right after closing  outputhisto" << endl;
}

void TrigEff::FillMass(double INVMASS){
	MASS->Fill(INVMASS);
}


void TrigEff::FitSignal(){
	FITSIG = (TH1D*) MASS->Clone();
	FITBG = (TH1D*) MASS->Clone();
	FITBG2 = new TH1D("FitBackground2" , " Z Mass from Z->mu mu decay" , 80 , 0 , 160);
	
	//FITBG2->Sumw2();

	FITSIG->SetName("FitSignal");
	FITBG->SetName("FitBackground");

	
	int nbinxfit = FITSIG->GetNbinsX();
	int nbinyfit = FITSIG->GetNbinsY();
	int tab[nbinxfit];
	
	for(int x = 0 ; x < nbinxfit ; x++){
		tab[x] = FITBG->GetBinContent(x);
		//cout << "bin " << x << " = " << tab[x] << endl;
	}


	for(int x = 0 ; x < 40 ; x++){
		FITSIG->SetBinContent(x,0);
		FITBG2->SetBinContent(x,tab[x]);

	}
	for(int x = 50; x < 80 ; x++){
		FITSIG->SetBinContent(x,0);
		FITBG2->SetBinContent(x,tab[x]);
	}
	
	for(int x = 40 ; x < 50 ; x++){
		FITBG->SetBinContent(x,0);
	}
	
	FITSIG->Fit("gaus"); 
	double IntegralGauss = FITSIG->Integral(40,50, "width");
	
	//myfunc = FITSIG->GetFunction("gaus");
	//myfunc->Draw();

	//cout << "This is the integral of the signal : " << IntegralGauss << endl;
	//FITSIG->GetFunction("gaus")->SetLineColor(kRed);
	//FITSIG->GetFunction("gaus")->SetLineStyle(1);
	//FITSIG->GetFunction("gaus")->SetLineWidth(2);


	FITBG->Fit("expo");
	FITBG2->Fit("expo");
	double IntegralBg = FITBG->Integral(40,50, "width");
	double IntegralBg2 = FITBG2->Integral(40,50, "width");
	cout << "This is the integral of the background with bins [40-50] = 0 : " << IntegralBg << " and without the bins : " << IntegralBg2 << endl;
	cout << "Ratio signal/total with bins in 40-50 are 0 = " << (IntegralGauss *1.0 / (IntegralGauss+IntegralBg))*100 << " %" << endl;
	cout << "Ratio signal/total without bins = " << (IntegralGauss *1.0 / (IntegralGauss+IntegralBg2))*100 << " %" << endl;
	//FITBG->GetFunction("expo")->SetLineColor(kBlue);
	//FITBG->GetFunction("expo")->SetLineStyle(1);
	//FITBG->GetFunction("expo")->SetLineWidth(2);


	FITBG2->Write();
	FITBG->Write();
	FITSIG->Write();
}





void TrigEff::Compute(string NameOutputFile){
	
	ComputeEff();
	
	ComputeError();
	
	//PrintNumEff();
	//PrintDenomEff();
	SortEffVec();
	
	//PrintEff();
	SaveIntTrigs(NameOutputFile.c_str());


	ComputeCorr();
	//PrintDenomCorr();
	//PrintCorr();
	FitSignal();
}
