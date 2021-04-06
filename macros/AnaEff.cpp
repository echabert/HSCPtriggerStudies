#define efficiency_cxx
#include "AnaEff.h"


#include <iostream>
#include <string>
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
#include <vector>


using namespace std;


void AnaEff::Loop()
{

	Long64_t nentries = fChain->GetEntriesFast();
	Long64_t nbytes = 0, nb = 0, nbi = 0;

	// Initialization/reading the number of triggers (ntrigger), prescale triggers etc..

	Long64_t initializing = LoadTree(0); 
	if (initializing < 0) cout << "Aborted"<< endl;
	nbi = fChain->GetEntry(initializing);   nbytes += nbi;
	cout << "ntrigger  : " << ntrigger << endl;
	

	ifstream ifile("CompleteList.txt"); 
	vector<string> triggerNames;
	vector<string> SubListMET;
	
	string s2 = "PFMET";

	string tmp;
	while(getline(ifile,tmp)){
   		triggerNames.push_back(tmp);
		if(strstr(tmp.c_str(),s2.c_str())){
			SubListMET.push_back(tmp);
		}
		
	}

	

	cout<<"size of triggerNames : "<< triggerNames.size() <<endl;
	ifile.close();
	



	vector<string> str;
	string interfstr;
	

	ifstream inttrigs("PrescaledSubList.txt"); // FIle with triggers that you want to study (prescale == 1)
	while(getline(inttrigs,tmp)){
   		str.push_back(tmp);
	}
	inttrigs.close();
	vector<string> listofprescaledtriggers;


	
	//trigEff_selection_obs.LoadNoMap(str,1,"PT","test_PT_nomap.root");
	//trigEff_presel.LoadNoMap(str,1,"MET","test_MET_nomap.root");

	

	trigEff_selection_obs.Load(triggerNames,str,1,"entered","PT","test_PT.root");

	

	trigEff_presel.Load(triggerNames,SubListMET,1,"entered","MET","test_MET.root"); 

	//trigEff_selection_obs.CreateHisto("s", str);
	str.clear();
	int counter=0,passedevent=0;
	int indexcandidate;
	//nentries=30;
	for (Long64_t jentry=0; jentry<nentries;jentry++) { //All entries
		Long64_t ientry = LoadTree(jentry);
		if(jentry!=0 && jentry%5000==0) cout << "Still here " << endl;
		if (ientry < 0) break;
        	nb = fChain->GetEntry(jentry);   nbytes += nb;	// 



		//for(int ihs=0; ihs<nhscp;ihs++){
			counter+=1;
		
			vector<Bool_t> vtrigger; //Convert array into vector
			vector<float> TrackPT,MuonPT,METPT;

			float HighestPT,HighestMuonPT,HighestMET;
			bool selections;
		
		

		/*if(nhscp !=0){
			for(int ihs=0; ihs<nhscp;ihs++){
				TrackPT.push_back(track_pt[hscp_track_idx[ihs]]);
				METPT.push_back(pfmet_pt[hscp_track_idx[ihs]]);
				MuonPT.push_back(muon_pt[hscp_track_idx[ihs]]);
			}

			sort(TrackPT.rbegin(), TrackPT.rend());
			sort(METPT.rbegin(), METPT.rend());
			sort(MuonPT.rbegin(), MuonPT.rend());	
			
			HighestPT = TrackPT[0];
			HighestMET=METPT[0];
			HighestMuonPT=MuonPT[0];

			METPT.clear();
			MuonPT.clear();
			TrackPT.clear();
		}
		else{
			
			HighestMuonPT = muon_pt[0];
			HighestMET = pfmet_pt[0];
			HighestPT = 0.0001;

		}*/
			indexcandidate=Selection();
			if(indexcandidate != 64){
				HighestPT = track_pt[indexcandidate];
				HighestMET = pfmet_pt[indexcandidate];
		
				for(int i=0;i<ntrigger;i++){
					vtrigger.push_back(passTrigger[i]);
				}

				passedevent+=1;
				trigEff_selection_obs.Fill(vtrigger,HighestPT);
				trigEff_presel.Fill(vtrigger,HighestMET);

				//trigEff_selection_obs.FillNoMap(vtrigger,HighestPT);
				//trigEff_presel.FillNoMap(vtrigger,HighestMET);			
				
			}
		
	
			/*HighestPT = track_pt[ihs];
			HighestMET = pfmet_pt[ihs];

			for(int i=0;i<ntrigger;i++){
				vtrigger.push_back(passTrigger[i]);
			}
		
			//selections=Selection(ihs);

			if(selections){
				passedevent+=1;
				trigEff_selection_obs.Fill(vtrigger,HighestPT); // HighestMET
				//trigEff_presel.Fill(vtrigger,HighestPT);
			}*/
		//}	
		
	}
	cout << "Before compute " << endl;

	double ratio = passedevent*1.0/counter;
	cout << " Number of candidates that passed the selection : " << passedevent << " , total number : " << counter << endl;
	cout << " Ratio passed/total : " << ratio*100 << " %" << endl;
	trigEff_selection_obs.Compute("test_TriggersOfInterest_PT_withmap.txt");

	trigEff_presel.Compute("test_TriggersOfInterest_MET_withmap.txt");
	//trigEff_presel.Compute();

	
	cout << "Just ended " << endl;

	triggerNames.clear();
	listofprescaledtriggers.clear();
	
	trigEff_selection_obs.WritePlots("");
	trigEff_presel.WritePlots("");
	//trigEff_presel.WritePlots("PT");
}

int AnaEff::Selection(){

	//Selectionné si au moins 1 par evenemtn est selectionné (dans fonction selection : trouver l'indice du sélectioné)

	
	for(int ihs=0; ihs<nhscp;ihs++){
	
	
	
		int index;
		bool selec=1;

		if( track_eta[hscp_track_idx[ihs]] >= 2.1 || track_eta[hscp_track_idx[ihs]] <= -2.1 ){
			selec = 0;
			//return false;
		}

		if( track_npixhits[hscp_track_idx[ihs]] <= 1 ){ //?
			selec = 0;
		}
	
		if( track_nhits[hscp_track_idx[ihs]] <= 7 ){
			selec = 0;
		}

		if( track_validfraction[hscp_track_idx[ihs]] <= 0.8 ){
			selec = 0;
		}

		if( ndedxhits <= 5 ){
			selec = 0;
		}

		if( track_pt[hscp_track_idx[ihs]] <= 55 ){
			selec = 0;
		}

		if( track_dxy[hscp_track_idx[ihs]] >=0.5 ){
			selec = 0;
		}
		
		if( track_dz[hscp_track_idx[ihs]] >=0.5 ){
			selec = 0;
		}
	
		if( track_pterr[hscp_track_idx[ihs]]/track_pt[hscp_track_idx[ihs]] >= 1 ){ 
			selec = 0;
		}

		if( track_qual[hscp_track_idx[ihs]] < 2 ){//?
			selec = 0;
		}

		if(hscp_iso2_tk[ihs] >= 50){
			selec = 0;
		}
		
		//if ( muon_comb_inversebeta[hscp_track_idx[i]] < 1   ) { // no branch yet
	 	//	selec = 0;
	//	}

		if(selec){
			return ihs;
		}
		else{
			return 64;
		}
		//ecal + hcal/p

		//Rajouter les critères pour un muon seulement + condition timing
		//vérifier que c'est un muon, et ensuite regarder inversemuonbeta
	 
		
	}

	/*
	bool selec=1;

		if( track_eta[hscp_track_idx[i]] >= 2.1 || track_eta[hscp_track_idx[i]] <= -2.1 ){
			selec = 0;
			//return false;
		}

		if( track_npixhits[hscp_track_idx[i]] <= 1 ){ //?
			selec = 0;
		}
	
		if( track_nhits[hscp_track_idx[i]] <= 7 ){
			selec = 0;
		}

		if( track_validfraction[hscp_track_idx[i]] <= 0.8 ){
			selec = 0;
		}

		if( ndedxhits <= 5 ){
			selec = 0;
		}

		if( track_pt[hscp_track_idx[i]] <= 55 ){
			selec = 0;
		}

		if( track_dxy[hscp_track_idx[i]] >=0.5 ){
			selec = 0;
		}
		
		if( track_dz[hscp_track_idx[i]] >=0.5 ){
			selec = 0;
		}
	
		if( track_pterr[hscp_track_idx[i]]/track_pt[hscp_track_idx[i]] >= 1 ){ 
			selec = 0;
		}

		if( track_qual[hscp_track_idx[i]] < 2 ){//?
			selec = 0;
		}

		if(hscp_iso2_tk[i] >= 50){
			selec = 0;
		}

		//ecal + hcal/p

	
		//vérifier que c'est un muon, et ensuite regarder inversemuonbeta
	 
		return selec;
	}
*/
}

int main(){

	AnaEff ana;
	ana.Loop();
	
}


