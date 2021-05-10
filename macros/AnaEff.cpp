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
	cout << "Number of triggers for this file  : " << ntrigger << endl;
	cout << " Number of events for this file : " << nentries << endl;

	ifstream ifile("CompleteList.txt"); 
	vector<string> triggerNames;
	vector<string> SubListMET;
	vector<string> SubListPT;
	

	string s2 = "mu";
	//string s4 = "Mu";
	//string s5 = "Muon";
	string s3 = "TESTPT";
	string s6 = "testmu";
	
	
	string tmp;
	while(getline(ifile,tmp)){
   		triggerNames.push_back(tmp);
		if(strstr(tmp.c_str(),s2.c_str())){
			SubListMET.push_back(tmp);
		}
		/*if(strstr(tmp.c_str(),s4.c_str())){
			SubListMET.push_back(tmp);
		}
		if(strstr(tmp.c_str(),s5.c_str())){
			SubListMET.push_back(tmp);
		}*/

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
	
	string subnum = "all"; //to_string(2);
	string extroot = ".root";
	string exttxt = ".txt";
	string date = "0505_";
	
	
	
	string TransferTxt="AllInfos";
	string TransferEff = "Eff";
	string TransferZ = "EntriesFromZ";
	string TransferDistrib = "DistribZpeak";
	string DataType = "MU";

		
	string StudyData = DataType + date;
	string StudyTxt = TransferTxt + DataType + date;
	string StudyEff= TransferEff + DataType + date;
	string StudyZ= TransferZ + DataType + date;
	string StudyDistribZ = TransferDistrib + DataType + date;

	


	string NameOfFile = StudyData + subnum + extroot;

	string NameOfEff = StudyEff + subnum + exttxt;
	string NameOfTxt = StudyTxt + subnum + exttxt;
	string EntriesFromZ = StudyZ + subnum + exttxt;
	string distribvarZ = StudyDistribZ + subnum + extroot;
	
	

	MUONPT_DISTRIB = new TH1D("MuonPT close to Z", "muon_pt close to z peak", 50,0,100);
	ISOR03_DISTRIB = new TH1D("ISOR03 close to Z", "ISOR03 close to z peak", 50,0,100);
	trigEff_selection_obs.LoadNoMap(triggerNames,SubListMET,1,DataType,NameOfFile); 
	//trigEff_presel.LoadNoMap(triggerNames,SubListMET,1,"MET","test_MET_nomap.root");
	//a
	
	str.clear();
	SubListMET.clear();
	SubListPT.clear();

	int counter=0,passedevent=0,nbofpairs=0,nbofpairsZ=0,nbmuons=0;
	int indexcandidate;
	double InvMass;
	//nentries=30;
	ofstream InfosZ;
	InfosZ.open (EntriesFromZ);

	cout << "Before loop nentries" << endl;
	for (Long64_t jentry=0; jentry<nentries;jentry++) { //All entries
		Long64_t ientry = LoadTree(jentry);
		if(jentry!=0 && jentry%5000==0) cout << "Still here " << endl;
		if (ientry < 0) break;
        	nb = fChain->GetEntry(jentry);   nbytes += nb;	// 
		
		InvMass = MuonsInvariantMass();

		//double IsoInvMass = MuonInvariantMass();
		if(InvMass!=1){
			if(InvMass < massZ + 10 && InvMass > massZ - 10){ // 10 
				InfosZ << "Z found entry " << jentry << " muons " << muon1 << " and " << muon2 << endl;
				nbofpairsZ+=1;
			}
			//cout << InvMass << endl;
			nbofpairs+=1;
			if(InvMass > 15){
				trigEff_selection_obs.FillMass(InvMass,1);
			}
		}
		nbmuons+=nmuons;
		counter+=1;
		
		vector<Bool_t> vtrigger; //Convert array into vector
		vector<int> position;
		vector< pair<int, bool > > PosPass;
		float HighestPT,HighestMuonPT,HighestMET;
		indexcandidate=Selection();
		if(indexcandidate != 64){
			//cout << indexcandidate << endl;
			HighestPT = track_pt[indexcandidate];
			HighestMET = pfmet_pt[indexcandidate];
			for(int i=0;i<ntrigger;i++){
				vtrigger.push_back(passTrigger[i]);
			}
			cout << " size of triggerName : " << ntrigger << endl;
			for(int p = 0; p < ntrigger; p++){
				auto iter = std::find(triggerNames.begin(), triggerNames.end(), triggerName->at(p));
				if(iter == triggerNames.end()){

					cout << " one trigger not found in CompleteList" << endl;
				}
				else{
					auto pos = std::distance(triggerNames.begin(), iter);
					//position.push_back(pos);
					PosPass.push_back(make_pair(pos,vtrigger[p]));
					//cout << "found trigger " << p << " ( " << triggerName->at(p) << " ) " << " in position" << pos << " inside CompleteList" << endl;
				}
				
				
			}
			
			passedevent+=1;
			//cout << " new candidate --------------" << endl;
			cout << vtrigger.size() << endl;
			

			trigEff_selection_obs.FillNoMap2(PosPass,HighestPT,1);



			//trigEff_selection_obs.FillNoMap(vtrigger,HighestPT,1);
			


			//trigEff_presel.FillNoMap(vtrigger,HighestMET);					
		}	
	}
	
	InfosZ.close();
	ofstream InfosData;
	InfosData.open (NameOfTxt);

	InfosData << "Number of muons pairs found " << nbofpairs << "\n" << endl;

	cout << "Number of pairs found " << nbofpairs << "\n" << endl;


	double ratio = passedevent*1.0/counter;

	cout << " Number of candidates that passed the selection : " << passedevent << " , total number : " << counter << "\n" << endl;

	InfosData << "Number of candidates that passed the selection : " << passedevent << " , total number : " << nentries << "\n" << endl;

	cout << " Ratio passed/total : " << ratio*100 << " %" << "\n" << endl;
	
	InfosData << "Ratio passed/total : " << ratio*100 << " %" << "\n" << endl;

	cout << " # muons as a pair (Z)/ total # of muons : " << nbofpairsZ << " / " << nbmuons << " , Ratio :" << (nbofpairsZ*2.0/nbmuons)*100 << " %" << endl << endl << " Total number of pairs = " << nbofpairs  <<" Ratio pair Z / total pairs:" << (nbofpairsZ*1.0/nbofpairs)*100 << " %" << endl;



	InfosData << "# muons as a pair (Z)/ total # of muons : " << nbofpairsZ << " / " << nbmuons << " , Ratio :" << (nbofpairsZ*2.0/nbmuons)*100 << " %" << endl << endl <<  "Ratio pair Z / total pairs:" << (nbofpairsZ*1.0/nbofpairs)*100 << " %" << endl;


	InfosData.close();
	trigEff_selection_obs.Compute(NameOfEff);
	//trigEff_presel.Compute("test_TriggersOfInterest_MET_withmap.txt");
	
	triggerNames.clear();
	
	trigEff_selection_obs.WritePlots("",NameOfFile);

	

	
	distrib = new TFile(distribvarZ.c_str(),"RECREATE");
	
	distrib->cd();
	MUONPT_DISTRIB->Write();
	ISOR03_DISTRIB->Write();
	distrib->Close();
	//trigEff_presel.WritePlots("");

}

int AnaEff::Selection(){
	int index=64,count2=0;
	vector<int> positions;
	vector< pair<float, int > > Muonpt; 
	bool yon=true;
	for(int ihs=0; ihs<nhscp;ihs++){
		//cout << ihs  << endl;
		//ecal + hcal/p
		//vérifier que c'est un muon, et ensuite regarder inversemuonbeta
		yon=true;
		if( track_eta[hscp_track_idx[ihs]] >= 2.1 || track_eta[hscp_track_idx[ihs]] <= -2.1 ){
			yon=false;
		}
		if( track_npixhits[hscp_track_idx[ihs]] <= 1 ){ //?
			yon=false;
		}
		if( track_nhits[hscp_track_idx[ihs]] <= 4 ){ //7
			yon=false;
		}
		if( track_validfraction[hscp_track_idx[ihs]] <= 0.8 ){
			yon=false;
		}
		if( ndedxhits <= 5 ){
			yon=false;
		}
		if( track_pt[hscp_track_idx[ihs]] <= 50 ){ //55
			yon=false;
		}
		if( track_dxy[hscp_track_idx[ihs]] >=0.5 ){
			yon=false;
		}
		if( track_dz[hscp_track_idx[ihs]] >=0.5 ){
			yon=false;
		}
		if( track_pterr[hscp_track_idx[ihs]]/track_pt[hscp_track_idx[ihs]] >= 1 ){ 
			yon=false;
		}
		if( track_qual[hscp_track_idx[ihs]] < 2 ){//?
			yon=false;
		}
		if(hscp_iso2_tk[ihs] >= 100){ //50
			yon=false;
		}

		/*if (muon_isMediumMuon[ihs]){
			cout << " muon index : " << hscp_muon_idx[ihs] << " , with track index : " << hscp_track_idx[ihs] << endl;

		}*/

		if(yon){
			positions.push_back(ihs); 
			Muonpt.push_back(make_pair(muon_pt[ihs],ihs));
		}
		 // pb ici, return que 0
		
	}

	
	//cout << "nhscp:" << nhscp <<endl;
	
	if(positions.size() != 0){
		int siz=Muonpt.size();
		sort(Muonpt.begin(),Muonpt.end());
		
		return Muonpt[siz-1].second;
	}
	else{
		return 64;
	}

}

int AnaEff::fact(int n){
     return (n==0) || (n==1) ? 1 : n* fact(n-1);
}




double AnaEff::deltaR2(float track_eta,float track_phi, float muon_eta, float muon_phi){
	float dp = std::abs(track_phi - muon_phi);
	if (dp > M_PI){
		dp -= 2.0 * M_PI;
	}
	return (track_eta - muon_eta)*(track_eta - muon_eta) + dp * dp;

}


double AnaEff::deltaR(double delta) {
	return std::sqrt(delta);
}





double AnaEff::MuonsInvariantMass(){
	double InvariantMass,c1pt,c2pt,c1phi,c2phi,c1eta,c2eta;
	TLorentzVector mu1,mu2,sum;
	double tram;
	vector<TLorentzVector> mus;
	vector<double> invmass;
	bool yon=true,diff=true;
	vector< pair<float, int > > muonPT,muonPHI,muonETA;
	vector< pair<int, int > > binom;
	int nbcomb,pom=0,newcomb;
	
	if(nmuons < 2){
		return 1;
	}
	else if (nmuons == 2){
		if(muon_pt[0] >= 10 && muon_pt[1] >= 10){
			//cout << "2 muons " << endl; 
			c1phi = muon_phi[0];
			c2phi = muon_phi[1];

			c1eta = muon_eta[0];
			c2eta = muon_eta[1];

			c1pt = muon_pt[0];
			c2pt = muon_pt[1];
		
			mu1.SetPtEtaPhiM(c1pt,c1eta,c1phi,massMu);
			mu2.SetPtEtaPhiM(c2pt,c2eta,c2phi,massMu);

			sum = mu1 + mu2;
			//double angle = mu1.Angle(mu2.Vect());
			//cout << " Angle between the two muons :" << angle << endl; 
			//cout << sum[0] << ", " << sum[1] << ", " << sum[2] << endl;
			double armass = sum.M();

			//cout << "invariant mass : " << armass << endl;	
			return armass;
		}
		else{
			return 1;
		}
	}

	else if(nmuons > 2){
		nbcomb = (fact(nmuons) / (fact(2) * fact(nmuons-2)) );

	
	
		//&& muon_pt[j] >= 10 && muon_pt[k] >= 10
		//cout << nmuons << " muons " << endl; 
		for(int i = 0; i < nmuons ; i++){
			if(muon_pt[i] > 10){
				muonPT.push_back(make_pair(muon_pt[i],i));
				muonETA.push_back(make_pair(muon_eta[i],i));
				muonPHI.push_back(make_pair(muon_phi[i],i));
			}
			
		}
		//cout << "On a sample of " << nmuons << " muons, only " << muonPT.size() << " were picked" << endl;
		//newcomb = (fact(muonPT.size()) / (fact(2) * fact(muonPT.size()-2)));
		//cout << "That leaves us with " << newcomb << " possible pairs" << endl;

		if(muonPT.size() < 2){
			return 1;
		}
		
		mus.resize(muonPT.size());
		for(int j = 0 ; j < muonPT.size() ; j++){
			mus[j].SetPtEtaPhiM(muonPT[j].first,muonETA[j].first,muonPHI[j].first,massMu);
			for(int k = 0; k < muonPT.size() ; k++){
				if(k!=j){
					diff=true;
					for(int s = 0; s < binom.size() ; s++){
						if(binom[s].first == k && binom[s].second == j){
							diff=false;
						}
					}
					if(diff){
						mus[k].SetPtEtaPhiM(muonPT[k].first,muonETA[k].first,muonPHI[k].first,massMu);
						binom.push_back(make_pair(j,k));
						invmass.push_back((mus[j]+mus[k]).M());
					}
				}
			}
			
		}
		
		mus.clear();
		muonPT.clear();
		muonETA.clear();
		muonPHI.clear();
		
		
		
		tram = abs(invmass[0]-massZ);
		for (int u = 0; u < invmass.size() ; u++){
			//cout << pom << endl;
			//cout << "--------------" << invmass[u] << endl;
			if((abs(invmass[u]-massZ)) < tram){
				pom=u;
				tram=abs(invmass[u]-massZ);
				//cout << pom << " ," << invmass[pom] << endl;
				//cout << "mass-mZ : " << tram << endl;
			}
		}
	
		muon1 = binom[pom].first;
		muon2 = binom[pom].second;
		//cout << "muon " << binom[pom].first << "[pt] :" << muon_pt[binom[pom].first] << " , muon " << binom[pom].second << "[pt] :"  << muon_pt[binom[pom].second] << endl;
		if(invmass[pom] < massZ + 10 && invmass[pom] > massZ -10){
			MUONPT_DISTRIB->Fill(muon_pt[binom[pom].first]);
			ISOR03_DISTRIB->Fill(muon_isoR03_sumChargedHadronPt[binom[pom].first]);	
		}
		binom.clear();
	
		//cout << invmass[pom] << endl;
		double armass = invmass[pom];	
		invmass.clear();
		return armass;
		}

	mus.clear();
	muonPT.clear();
	muonETA.clear();
	muonPHI.clear();
	binom.clear();
	invmass.clear();
	return 1;
}



double AnaEff::IsolateMuons(const vector<bool> &passtrig){
	vector<int> candidates;
	int nbpairs=0;
	double proba1,proba2;
	vector<bool> ootm1,ootm2,combinedootm;
	vector<double> inversebeta;	
	return 1;	
}






int main(){

	AnaEff ana;
	ana.Loop();
	
}


