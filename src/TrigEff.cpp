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
#include <TFrame.h>

using namespace std;



TrigEff::TrigEff(){
	EFF_TRIG=0;
	EFF_DISTRIB=0;
	CORR=0;
	OutputHisto=0;
	FITBG2=0;
	MASS=0;
	MASSW=0;
	SOLOM=0;
	ORTRIGGER=0;
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
	if(!ORTRIGGER){
		delete ORTRIGGER;
	}
	if(!EFF_DISTRIB){ 
		delete EFF_DISTRIB;
	}
	if(!MASS){
		delete MASS;
	}
	if(!MASSW){
		delete MASSW;
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
			cout << "[" << j<< "," << index << "]" << endl; 
			
		}
	
	NumCorr.resize(triggerNames.size(), vector<double>(triggerNames.size(), 0.0)); 
	DenomCorr.resize(triggerNames.size(), vector<double>(triggerNames.size(), 0.0));
	Correlation.resize(triggerNames.size(), vector<double>(triggerNames.size(), 0.0)); 
	
	LogicalOr.resize(triggerNames.size(), vector<double>(triggerNames.size(), 0.0));
	NumLogicalOr.resize(triggerNames.size(), vector<double>(triggerNames.size(), 0.0));
	DenomLogicalOr.resize(triggerNames.size(), vector<double>(triggerNames.size(), 0.0));

	ErrorLogicalOr.resize(triggerNames.size(), vector<double>(triggerNames.size(), 0.0));
	
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
		//cout << "selection size : " << TestNoMap.size() << endl;
		for(int j=0; j < triggerNames.size(); j++){
			
			if(NameVar=="PT"){
				EffvsObs[j] = new TEfficiency("Eff","Efficiency;PT;#epsilon",50,0,2000); 
			}
			else if(NameVar=="MET"){
				EffvsObs[j] = new TEfficiency("Eff","Efficiency;MET;#epsilon",50,0,2000);
			}
			else if(NameVar=="MU"){
				EffvsObs[j] = new TEfficiency("Eff","Efficiency;MUPT;#epsilon",50,0,2000);
			}
			else if(NameVar=="Stau494"){
				EffvsObs[j] = new TEfficiency("Eff","Efficiency;Stau494PT;#epsilon",50,0,2000);
			}
			else if(NameVar=="Stau1599"){
				EffvsObs[j] = new TEfficiency("Eff","Efficiency;Stau1599PT;#epsilon",50,0,2000);
			}
			else if(NameVar=="SingleMuon"){
				EffvsObs[j] = new TEfficiency("Eff","Efficiency;SingleMuonPT;#epsilon",50,0,2000);
			}
			else if(NameVar=="Gluino1600"){
				EffvsObs[j] = new TEfficiency("Eff","Efficiency;Gluino1600P/M;#epsilon",50,0,2000);
			}
			else if(NameVar=="Gluino1800"){
				EffvsObs[j] = new TEfficiency("Eff","Efficiency;Gluino1800P/M;#epsilon",50,0,2000);
			}
			else if(NameVar=="Gluino2000"){
				EffvsObs[j] = new TEfficiency("Eff","Efficiency;Gluino2000P/M;#epsilon",50,0,2000);
			}
			else if(NameVar=="Gluino2200"){
				EffvsObs[j] = new TEfficiency("Eff","Efficiency;Gluino2200P/M;#epsilon",50,0,2000);
			}
			else if(NameVar=="Gluino2400"){
				EffvsObs[j] = new TEfficiency("Eff","Efficiency;Gluino2400P/M;#epsilon",50,0,2000);
			}
			else if(NameVar=="Gluino2600"){
				EffvsObs[j] = new TEfficiency("Eff","Efficiency;Gluino2600P/M;#epsilon",50,0,2000);
			}
			else if(NameVar=="Gluino2000test"){
				EffvsObs[j] = new TEfficiency("Eff","Efficiency;Gluino2000PT;#epsilon",50,0,2000);
			}
			else if(NameVar=="Stop1600"){
				EffvsObs[j] = new TEfficiency("Eff","Efficiency;Stop1600PT;#epsilon",50,0,2000);
			}
			else if(NameVar=="Stop2000"){
				EffvsObs[j] = new TEfficiency("Eff","Efficiency;Stop2000PT;#epsilon",50,0,2000);
			}
			else if(NameVar=="Gluino1600test2"){
				EffvsObs[j] = new TEfficiency("Eff","Efficiency;Gluino1600test2PT;#epsilon",50,0,2000);
			}
			else if(NameVar=="Gluino1800av"){
				EffvsObs[j] = new TEfficiency("Eff","Efficiency;Gluino1600test2PT;#epsilon",50,0,2000);
			}
			
			


			else{
				EffvsObs[j] = new TEfficiency("Eff","Efficiency;TestPT;#epsilon",50,0,2000);

			}

			EffvsObs[j]->SetName(triggerNames[j].c_str());


		}	




		
			
			//EffvsObs[j]->Draw("AP");
			//gPad->Update();
			//EffvsObs[j]->GetPaintedGraph()->GetXaxis()->SetTitle(NameObs.c_str());
			
	}
	cout << " TriggerNames size " << TriggerNames.size() << endl;

	EFF_TRIG = new TH1D("EFF_TRIG", "EFF", 100,0,1); 
	EFF_DISTRIB = new TH1D("Efficiency distribution for int trigs", "eff for triggers", TriggerNames.size(),0,TriggerNames.size());
	CORR = new TH2D("Correlation", "Correlation plot",  TriggerNames.size() , 0 , TriggerNames.size() , TriggerNames.size(), 0 , TriggerNames.size()); 
	ORTRIGGER = new TH2D("LogicalOr", "LogicalOr", TriggerNames.size() , 0 , TriggerNames.size() , TriggerNames.size(), 0 , TriggerNames.size());


	MASSW = new TH1D("MASSW" , " Masses invariante des muons enrichi en W" , nbins , 0 , massmax);

	MASS = new TH1D("MASS" , " Masses invariante des muons" , nbins , 0 , massmax);

	MASS->GetXaxis()->SetTitle("Mass [GeV]");
	MASS->GetYaxis()->SetTitle(" # candidates");

	MASSW->GetXaxis()->SetTitle("Mass [GeV]");
	MASSW->GetYaxis()->SetTitle(" # candidates");



	cout << "end of load" << endl;
	MASS->Sumw2();
	MASSW->Sumw2();
	EFF_TRIG->Sumw2();
	EFF_DISTRIB->Sumw2();
	CORR->Sumw2();
	ORTRIGGER->Sumw2();
	if(ErrorType == 1 ){
	}
	
}




//***************************************************************************************************************************


void TrigEff::FillNoMap(vector<bool> &passtrig, float Obs, double weight,string mode){  //const vector<bool> 
	if(mode== "all"){ 
		bool trig1,trig2;
		for(int i = 0; i < TestNoMap.size()  ;i++){
			
			if(TestNoMap[i].second >= passtrig.size()){
				break;
			}
			
			trig1 = passtrig.at(TestNoMap[i].second);
			DenomEfficiency[TestNoMap[i].second]+=1;
			
			if(trig1){
				NumEfficiency[TestNoMap[i].second]+=1;
			}

			for(int j = 0; j < TestNoMap.size()  ;j++){

				if(TestNoMap[j].second >= passtrig.size()){
					break;
				}

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
	else if(mode == "muon"){
		bool trigmu1,trigmu2;
		//for(int i = 0; i < TestNoMap.size()  ;i++){
		
		//trigmu1 = passtrig.at(TestNoMap[i].second);

		//}
	}
}

//***************************************************************************************************************************


void TrigEff::FillNoMap2(vector< pair<int, bool > > PosPass, float Obs, double weight,string mode){  //const vector<bool> 
	if(mode== "all"){ 
		bool trig1,trig2;
		
		//for(int j=0; j< TestNoMap.size(); j++){
		
			for(int i = 0; i < PosPass.size(); i++){

				//if(PosPass[i].first == TestNoMap[j].second){
					DenomEfficiency[PosPass[i].first] +=1;
					trig1 = PosPass[i].second;
					if(trig1){
						NumEfficiency[PosPass[i].first] += 1;
						
					}
					if(Obs!=0.0){
						EffvsObs[i]->TEfficiency::Fill(PosPass[i].second ,Obs);
					}
						//break;
				
				//}
				
				for(int k = 0; k<PosPass.size(); k++){
					//if(PosPass[k].first == TestNoMap[j].second){
						trig2 = PosPass[k].second;
						DenomLogicalOr[PosPass[i].first][PosPass[k].first]+=1;
						if(trig1 || trig2){
							NumLogicalOr[PosPass[i].first][PosPass[k].first]+=1;
							DenomCorr[PosPass[i].first][PosPass[k].first]+=1;
						}
						if(trig1 && trig2){
							NumCorr[PosPass[i].first][PosPass[k].first]+=1;
						}
					//}

					
				}

			}
	
		//}
	
	}
	else if(mode == "muon"){
		bool trigmu1,trigmu2;
		//for(int i = 0; i < TestNoMap.size()  ;i++){
		
		//trigmu1 = passtrig.at(TestNoMap[i].second);

		//}
	}
}






//***************************************************************************************************************************
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


void TrigEff::ComputeLogicalOr(){
	for(int i=0;i< LogicalOr.size();i++){
		for(int j=0;j< LogicalOr[i].size();j++){
			if(DenomLogicalOr[i][j]==0){
				LogicalOr[i][j]=0;
			}
			else{	
			LogicalOr[i][j] = ((NumLogicalOr[i][j]*1.0) / DenomLogicalOr[i][j]);
			}
		}
	}
	
	
}



void TrigEff::PrintLogicalOr(){
	cout << endl;
	for ( int i = 0; i < LogicalOr.size(); i++ ){
   		for ( int j = 0; j < LogicalOr[i].size(); j++ ){
      			cout << setprecision(5) << " ¦¦ of [" << i+1 << "," << j+1 << "] : " << NumLogicalOr[i][j] << " / " << DenomLogicalOr[i][j] << " = " << LogicalOr[i][j] * 100  << "% ";
   		}
   	cout << endl;
	}
	cout << endl;
}



void TrigEff::PrintCorr(){
	cout << endl;
	for ( int i = 0; i < Correlation.size(); i++ ){
   		for ( int j = 0; j < Correlation[i].size(); j++ ){
      			cout << setprecision(5) << "[" << i+1 << "," << j+1 << "] : " << NumCorr[i][j] << " / " << DenomCorr[i][j] << " = " << Correlation[i][j] * 100  << "% ";
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
      			cout << NumCorr[i][j] << " / " << DenomCorr[i][j] << "  ";
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


void TrigEff::SaveIntTrigs(string NameOutputFile, string NameListEff, string ListAllTriggers, string EffTriggers, string ErrorEffTriggers){ // triggersofinterest = EffGluino16001105all.txt
	
	ofstream TriggersOfInterest;
	ofstream AllTriggers;
	ofstream EffOnly;
	ofstream EffAll;
	ofstream EffOnlyError;

	
	TriggersOfInterest.open (NameOutputFile.c_str());
	AllTriggers.open (ListAllTriggers.c_str());
	EffOnly.open (NameListEff.c_str());
	EffAll.open (EffTriggers.c_str());
	EffOnlyError.open (ErrorEffTriggers.c_str());





	for (int i = 0; i < Efficiency.size(); i++){
		EffList.push_back(make_pair(make_pair(Efficiency[i],i), make_pair(EffErr[i],TriggerNames[i])));
    	}

	for (int i = 0; i < Efficiency.size(); i++){ 
		EffAll << EffList[i].first.first*100 << endl; //TestNoMap[i].second
		EffOnlyError << EffList[i].second.first*100 << endl;
	}


	sort(EffList.begin(),EffList.end());

	for (int i = 0; i < Efficiency.size(); i++){ 
		AllTriggers << EffList[i].first.first*100 << " " << EffList[i].second.first*100 << " " << EffList[i].second.second << "\n"; //TestNoMap[i].second
		
	}

	AllTriggers.close();
	
	for (int i = 0; i < TestNoMap.size(); i++){ 	
		TransferVec.push_back(make_pair(make_pair(Efficiency[TestNoMap[i].second],TestNoMap[i].second), make_pair(EffErr[TestNoMap[i].second],TriggerNames[TestNoMap[i].second])));
		cout << setprecision (8) << (TransferVec[i].first.first)*100 << "\t\t" << setprecision (8) << (TransferVec[i].second.first)*100 << "\t\t" << TransferVec[i].second.second << endl; 

		AllCondensed.push_back(make_pair( make_pair(Efficiency[TestNoMap[i].second] ,make_pair(TestNoMap[i].second, make_pair( NumEfficiency[TestNoMap[i].second], DenomEfficiency[TestNoMap[i].second])) ), make_pair(EffErr[TestNoMap[i].second],TriggerNames[TestNoMap[i].second])));	

		EffOnly << (AllCondensed[i].first.first) * 100 << " " << (AllCondensed[i].second.second) << endl;

		
		//TriggersOfInterest << (TransferVec[i].first.first)*100 << " " << (TransferVec[i].second.first)*100 << " " << TransferVec[i].second.second << "\n";
    	}

	//sort(TransferVec.begin(), TransferVec.end());
	sort(AllCondensed.begin(), AllCondensed.end());
	for(int i = 0; i < TransferVec.size(); i++){ 

		TriggersOfInterest << i+1 << " " <<(AllCondensed[i].first.first) * 100 << " +/- " << (AllCondensed[i].second.first) *100  << "  " << (AllCondensed[i].first.second.second.first) << " / " << (AllCondensed[i].first.second.second.second) << " " << (AllCondensed[i].second.second) << endl;

		
		//TriggersOfInterest << (TransferVec[i].first.first)*100 << " +/-" << (TransferVec[i].second.first)*100  << "  " << TransferVec[i].second.second << "\n";
	}

	TransferVec.clear();
	TriggersOfInterest.close();
	EffOnly.close();
	EffOnlyError.close();
	EffAll.close();
}

void TrigEff::PrintNumEff(){
	for ( int i = 0; i < NumEfficiency.size(); i++ ){
      		cout << NumEfficiency[i] << " / " << DenomEfficiency[i] << endl ;
	}	
}


void TrigEff::PrintDenomEff(){
	for ( int i = 0; i < DenomEfficiency.size(); i++ ){
      		cout << i+1 << " : " << NumEfficiency[i] << " / " << DenomEfficiency[i] << " +/- " << EffErr[i] << endl ;
	}
}



void TrigEff::ComputeErrorLogicalOr(){
	for ( int i = 0; i < LogicalOr.size(); i++ ){
   		for ( int j = 0; j < LogicalOr[i].size(); j++ ){
      			if(LogicalOr[i][j] == 0){
				ErrorLogicalOr[i][j] = 0;
			}
			else{
				ErrorLogicalOr[i][j] = sqrt((LogicalOr[i][j] * (1.0-LogicalOr[i][j]))/DenomLogicalOr[i][j]);
			
			}
   		}
   	cout << endl;
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

	const char *trigger[7] = {"HLT_Mu50_v11","HLT_IsoMu20_v12", "HLT_PFHT1050_v14", "HLT_PFHT500_PFMET100_PFMHT100_IDTight_v8", "HLT_PFMET120_PFMHT120_IDTight_v16", "HLT_CaloMET70_HBHECleaned_v3", "HLT_MonoCentralPFJet80_PFMETNoMu120_PFMHTNoMu120_IDTight_v16"};

	for(int i=0;i < TestNoMap.size();i++){
		EffvsObs[i]->Write();
	}

	for(int i=0;i < Correlation.size();i++){
		for(int j=0;j< Correlation[i].size();j++){
			CORR->SetBinContent((i+1),(j+1),(Correlation[i][j]*100));
		}
	}


	CORR->Write();

	TCanvas *c21 = new TCanvas("c21","c21",200,10,700,500);
	//c111->SetFillColor(42);
	c21->SetTitle("Logical OR of triggers");


	ORTRIGGER->SetXTitle("HSCP Mass [GeV]");
	ORTRIGGER->SetYTitle("#epsilon");
	ORTRIGGER->SetStats(kFALSE);
	ORTRIGGER->Draw();


	c21->GetFrame()->SetBorderSize(12);
	

	for(int i=0;i < LogicalOr.size();i++){
		for(int j=0;j< LogicalOr[i].size();j++){
			ORTRIGGER->SetBinContent((i+1),(j+1),(LogicalOr[i][j]*100));
		}
	}


	for(int j = 1; j <= 7 ;j++ ){
		ORTRIGGER->GetXaxis()->SetBinLabel(j, trigger[j-1]);
		ORTRIGGER->GetYaxis()->SetBinLabel(j, trigger[j-1]);
	}

	ORTRIGGER->Draw("TEXT");
	c21->Modified();
	c21->Update();
	//CORR->SetDirectory("Correlations");
	
	ORTRIGGER->Write();
	MASS->Write();
	MASSW->Write();
	OutputHisto->Close();


}

void TrigEff::FillMass(double INVMASS,int choice){
	if(choice==1){
		MASS->Fill(INVMASS);
	}
	if(choice==2){
		MASSW->Fill(INVMASS);
	}	
}

void TrigEff::Compute(string NameOutputFile,string NameListEff, string ListAllTriggers, string EffTriggers, string ErrorEffTriggers){
	
	ComputeEff();
	ComputeError();
	
	//PrintNumEff();
	PrintDenomEff();
	//PrintEff();

	SaveIntTrigs(NameOutputFile,NameListEff,ListAllTriggers,EffTriggers, ErrorEffTriggers);

	ComputeCorr();

	//PrintDenomCorr();
	PrintCorr();
	ComputeLogicalOr();
	PrintLogicalOr();

}

