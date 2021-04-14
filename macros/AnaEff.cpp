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
#include <TLorentzVector.h>
#include <TMath.h>


using namespace std;

const double massZ = 91.1876;
const double uncertaintyZ = 0.0021;

const double massMu = 0.1056583745;
const double uncertaintyMu = 0.0000000024;

void AnaEff::Loop()
{

	Long64_t nentries = fChain->GetEntriesFast();
	Long64_t nbytes = 0, nb = 0, nbi = 0;

	// Initialization

	Long64_t initializing = LoadTree(0); 
	if (initializing < 0) cout << "Aborted"<< endl;
	nbi = fChain->GetEntry(initializing);   nbytes += nbi;
	cout << "ntrigger  : " << ntrigger << endl;
	

	ifstream ifile("CompleteList.txt"); 
	vector<string> triggerNames;
	vector<string> SubListMET;
	vector<string> SubListPT;


	string s2 = "PFMET";
	string s3 = "TESTPT";

	

	string tmp;
	while(getline(ifile,tmp)){
   		triggerNames.push_back(tmp);
		if(strstr(tmp.c_str(),s2.c_str())){
			SubListMET.push_back(tmp);
		}
		if(strstr(tmp.c_str(),s3.c_str())){
			SubListPT.push_back(tmp);
		}
		
	}
	
	
	ifile.close();

	vector<string> str;
	string interfstr;
	
	ifstream inttrigs("PrescaledSubList.txt"); 
	while(getline(inttrigs,tmp)){
   		str.push_back(tmp);
	}
	inttrigs.close();

	//cout << "avant loadnomap" << endl;
	trigEff_selection_obs.LoadNoMap(triggerNames,str,1,"PT","SingleMuonZ.root"); 
	//trigEff_presel.LoadNoMap(triggerNames,SubListMET,1,"MET","test_MET_nomap.root");

	

	//trigEff_selection_obs.Load(triggerNames,str,1,"entered","PT","test_PT.root");
	//trigEff_presel.Load(triggerNames,SubListMET,1,"entered","MET","test_MET.root"); 

	
	str.clear();
	SubListMET.clear();
	SubListPT.clear();

	int counter=0,passedevent=0,nbofpairs=0;
	int indexcandidate;
	//nentries=30;
	for (Long64_t jentry=0; jentry<nentries;jentry++) { //All entries
		Long64_t ientry = LoadTree(jentry);
		if(jentry!=0 && jentry%5000==0) cout << "Still here " << endl;
		if (ientry < 0) break;
        	nb = fChain->GetEntry(jentry);   nbytes += nb;	// 
		//cout << "prescale 0 different of 1 in entry : " << jentry <<endl;
		
		
		double InvMass = MuonsInvariantMass();
		//double IsoInvMass = MuonInvariantMass();
		if(InvMass!=1){
			//cout << InvMass << endl;
			nbofpairs+=1;
			trigEff_selection_obs.FillMass(InvMass,1);
		}
		//if(IsoInvMass != 1){
		//	trigEff_selection_obs.FillMass(IsoInvMass,2);
		//}
		counter+=nhscp;
		
		vector<Bool_t> vtrigger; //Convert array into vector
		//vector<float> TrackPT,MuonPT,METPT;
		float HighestPT,HighestMuonPT,HighestMET;
		//cout << nhscp << endl;
		indexcandidate=Selection();
		if(indexcandidate != 64){
			HighestPT = track_pt[indexcandidate];
			HighestMET = pfmet_pt[indexcandidate];
			for(int i=0;i<ntrigger;i++){
				vtrigger.push_back(passTrigger[i]);
			}
			passedevent+=1;
			//trigEff_selection_obs.Fill(vtrigger,HighestPT);
			//trigEff_presel.Fill(vtrigger,HighestMET);
			
			trigEff_selection_obs.FillNoMap(vtrigger,HighestPT,1);
			//trigEff_presel.FillNoMap(vtrigger,HighestMET);					
		}	
	}
	

	cout << "Number of pairs found " << nbofpairs << "\n" << endl;
	double ratio = passedevent*1.0/counter;
	cout << "Number of candidates that passed the selection : " << passedevent << " , total number : " << counter << "\n" << endl;
	cout << "Ratio passed/total : " << ratio*100 << " %" << "\n" << endl;
	
	trigEff_selection_obs.Compute("test_TriggersOfInterest_PT_nomap.txt");
	//trigEff_presel.Compute("test_TriggersOfInterest_MET_withmap.txt");
	
	triggerNames.clear();
	
	trigEff_selection_obs.WritePlots("");
	//trigEff_presel.WritePlots("");

}

int AnaEff::Selection(){
	int index=64;
	for(int ihs=0; ihs<nhscp;ihs++){
		//cout << ihs  << endl;
		//ecal + hcal/p
		//vérifier que c'est un muon, et ensuite regarder inversemuonbeta

		if( track_eta[hscp_track_idx[ihs]] >= 2.1 || track_eta[hscp_track_idx[ihs]] <= -2.1 ){
			return 64;
		}
		if( track_npixhits[hscp_track_idx[ihs]] <= 1 ){ //?
			return 64;
		}
		if( track_nhits[hscp_track_idx[ihs]] <= 7 ){
			return 64;
		}
		if( track_validfraction[hscp_track_idx[ihs]] <= 0.8 ){
			return 64;
		}
		if( ndedxhits <= 5 ){
			return 64;
		}
		if( track_pt[hscp_track_idx[ihs]] <= 55 ){
			return 64;
		}
		if( track_dxy[hscp_track_idx[ihs]] >=0.5 ){
			return 64;
		}
		if( track_dz[hscp_track_idx[ihs]] >=0.5 ){
			return 64;
		}
		if( track_pterr[hscp_track_idx[ihs]]/track_pt[hscp_track_idx[ihs]] >= 1 ){ 
			return 64;
		}
		if( track_qual[hscp_track_idx[ihs]] < 2 ){//?
			return 64;
		}
		if(hscp_iso2_tk[ihs] >= 50){
			return 64;
		}
		return ihs; // pb ici, return que 0
		
	}
	return 64;
}

double AnaEff::MuonInvariantMass(){
	double InvariantMass,cpt,cphi,ceta;
	TLorentzVector mu;
	vector<int> candidates;
	for(int ihs=0; ihs<nhscp;ihs++){
		//if(muon isolé)
		if(track_eta[hscp_track_idx[ihs]] >= 2.1 || track_eta[hscp_track_idx[ihs]] <= -2.1){
			return 1;
		}
		
		if(pfmet_pt[hscp_track_idx[ihs]] >= 5000 ){
			return 1;
		}
	
		if( track_pt[hscp_track_idx[ihs]] >= 5000 ){
			return 1;
		}
		if( track_dxy[hscp_track_idx[ihs]] >=0.5 ){
			return 1;
		}
		
		if( track_dz[hscp_track_idx[ihs]] >=0.5 ){
			return 1;
		}
		if(muon_isTrackerMuon[hscp_track_idx[ihs]]){
			candidates.push_back(ihs);
			//cout << ihs << endl;
		}
	}
		//
		//if ismuon
	if(candidates.size() == 1 ){
		cpt = muon_pt[0];
		ceta = muon_eta[0];
		cphi = muon_phi[0];
		mu.SetPtEtaPhiM(cpt,ceta,cphi,massMu);
		double armass = mu.M();
		return armass;
	}

	candidates.clear();
	return 1;
}

double AnaEff::MuonsInvariantMass(){
	double InvariantMass,c1pt,c2pt,c1phi,c2phi,c1eta,c2eta;
	TLorentzVector mu1,mu2,mu3,mu4,sum;
	vector<int> candidates,order;
	for(int ihs=0; ihs<nhscp;ihs++){
		//if(muon isolé)
		if(track_eta[hscp_track_idx[ihs]] >= 2.1 || track_eta[hscp_track_idx[ihs]] <= -2.1){
			return 1;
		}
		
		if(pfmet_pt[hscp_track_idx[ihs]] >= 5000 ){
			return 1;
		}
	
		if( track_pt[hscp_track_idx[ihs]] >= 5000 ){
			return 1;
		}
		if( track_dxy[hscp_track_idx[ihs]] >=0.5 ){
			return 1;
		}
		
		if( track_dz[hscp_track_idx[ihs]] >=0.5 ){
			return 1;
		}
	
		if(muon_isTrackerMuon[hscp_track_idx[ihs]]){
			candidates.push_back(ihs);
			//cout << ihs << endl;
		}
			
	}
	if(candidates.size() == 2){
		//cout << "Picking from 2 candidates" << endl;
		c1phi = muon_phi[candidates[0]];
		c2phi = muon_phi[candidates[1]];

		c1eta = muon_eta[candidates[0]];
		c2eta = muon_eta[candidates[1]];

		c1pt = muon_pt[candidates[0]];
		c2pt = muon_pt[candidates[1]];
				
		mu1.SetPtEtaPhiM(c1pt,c1eta,c1phi,massMu);
		mu2.SetPtEtaPhiM(c2pt,c2eta,c2phi,massMu);

		sum = mu1 + mu2;
		double angle = mu1.Angle(mu2.Vect());
		//cout << " Angle between the two muons :" << angle << endl; 
		//cout << sum[0] << ", " << sum[1] << ", " << sum[2] << endl;
		double armass = sum.M();
		//cout << "invariant mass : " << armass << endl;	
		return armass;

		
	}
	else if(candidates.size() == 3){
		//cout << "Picking from 3 candidates" << endl;
		if(muon_pt[candidates[0]] > muon_pt[candidates[1]] && muon_pt[candidates[0]] > muon_pt[candidates[2]]){
			order.push_back(0);
			if(muon_pt[candidates[1]] > muon_pt[candidates[2]]){
				order.push_back(1);
				//order.push_back(2);
			}
			else{
				order.push_back(2);
				//order.push_back(1);
			}	
		}
		else if (muon_pt[candidates[1]] > muon_pt[candidates[0]] && muon_pt[candidates[1]] > muon_pt[candidates[2]] ){
			order.push_back(1);
			if(muon_pt[candidates[0]] > muon_pt[candidates[2]]){
				order.push_back(0);
				//order.push_back(2);
			}
			else{
				order.push_back(2);
				//order.push_back(0);
			}

		}
		else if (muon_pt[candidates[2]] > muon_pt[candidates[1]] && muon_pt[candidates[2]] > muon_pt[candidates[0]]) {
			order.push_back(2);
			if(muon_pt[candidates[1]] > muon_pt[candidates[0]]){
				order.push_back(1);
				//order.push_back(0);
			}
			else{
				order.push_back(0);
				//order.push_back(1);
			}
		}
		//m plus proche du Z
		//norme du vec p 
	
		c1pt = muon_pt[order[0]];
		c2pt = muon_pt[order[1]];
		
		c1eta = muon_eta[order[0]];
		c2eta = muon_eta[order[1]];

		c1phi = muon_phi[order[0]];
		c2phi = muon_phi[order[1]];
		
		mu1.SetPtEtaPhiM(c1pt,c1eta,c1phi,massMu);
		mu2.SetPtEtaPhiM(c2pt,c2eta,c2phi,massMu);
		double angle = mu1.Angle(mu2.Vect());
		//cout << " Angle between the two muons :" << angle << endl; 
		sum = mu1 + mu2;
		double armass = sum.M();
		//cout << "invariant mass of candidates: " << order[0] << " and " << order[1] << " = " << armass << endl;
		order.clear();
		return armass;
	}

	else if(candidates.size() == 4){
		//cout << " 4 candidates, picking " << endl;
		mu1.SetPtEtaPhiM(muon_pt[0],muon_eta[0],muon_phi[0],massMu);
		mu2.SetPtEtaPhiM(muon_pt[1],muon_eta[1],muon_phi[1],massMu);
		mu3.SetPtEtaPhiM(muon_pt[2],muon_eta[2],muon_phi[2],massMu);
		mu4.SetPtEtaPhiM(muon_pt[3],muon_eta[3],muon_phi[3],massMu);
		
		vector<double> InvMass;
		TLorentzVector six[6];
		six[0] = mu1 + mu2;
		six[1] = mu1 + mu3;
		six[2] = mu1 + mu4;
		six[3] = mu2 + mu3;
		six[4] = mu2 + mu4;
		six[5] = mu3 + mu4;

		for(int i = 0; i < 6 ; i++ ){
			InvMass.push_back(six[i].M());
		}

		sort(InvMass.begin(), InvMass.end());
		//cout << "Highest Invariant mass 1-2 : " << InvMass[0] << " , 1-3 : " << InvMass[1] << " , 1-4 : " << InvMass[2] << " , 2-3 : " << InvMass[3] << " , 2-4 : " << InvMass[4] << " , 3-4 : " << InvMass[5] << endl;
		return InvMass[5];
	}
	else if(candidates.size() == 5){
		cout << "5 candidates, too much bg" << endl;
	}
	candidates.clear();
	return 1;
	//trigEff_selection_obs.MASS->Write();
	
}



int main(){

	AnaEff ana;
	ana.Loop();
	
}


