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
#include <TMath.h>
#include <TLorentzVector.h>
#include <Math/Vector4D.h>
#include <Math/Vector4Dfwd.h>
//#include <LorentzVector.h>
//#include <LorentzVector.h>
//#include <Math/Vector4D.h>
#include <Math/GenVector/LorentzVector.h>
#include <Math/GenVector/PxPyPzM4D.h>

//#include <Math/GenVector/LorentzVector.h>

//#include <Vector4Dfwd.h>
//#include <Math/Vector4D.h> 

using namespace std;

const double massZ = 91.1876;
const double uncertaintyZ = 0.0021;

const double massMu = 0.1056583745;
const double uncertaintyMu = 0.0000000024;

const double massW = 80.379;
const double uncertaintyW = 0.012;
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

	
	string NameList = "CompleteList";
	string PrescaledList = "PrescaledList";
	string ListAll = "ListOfAllTriggersEff";
	string SubNum = "all"; //to_string(2);
	string ExtRoot = ".root";
	string ExtTxt = ".txt";
	string Date = "1105";
	

	string TransferTxt="AllInfos";
	string TransferEff = "Eff";
	string TransferZ = "EntriesFromZ";
	string TransferW = "EntriesFromW";


	string TransferDistribZ = "DistribZpeak";
	string TransferDistribW = "DistribWpeak";
	string DataType = "Stop1600";

	string ListAllTriggers = ListAll + DataType + ExtTxt;
	//string NameCompleteList = "CompleteListTest.txt";

	string NameListEff = TransferEff + DataType + ExtTxt;
	string NameCompleteList = NameList + DataType + ExtTxt; // + DataType for others
	string NameCompleteListTest = "ListeInteretTriggers";
	string NameListForType = NameCompleteListTest + DataType + ExtTxt;
	string NameCompletePrescaledList = PrescaledList + DataType + ExtTxt;
	string EffTriggers = TransferEff + DataType + SubNum + ExtTxt;


	string s2 = "mu";
	string s4 = "Mu";
	string s5 = "Muon";

	string s3 = "TESTPT";
	string s6 = "testmu";
	string s7 = "MET";
	
	ifstream ifile(NameCompleteListTest.c_str()); 
	vector<string> triggerNames;
	vector<string> SubListMET;
	vector<string> SubListPT;
	string tmp;

	while(getline(ifile,tmp)){
   		triggerNames.push_back(tmp);
		if(strstr(tmp.c_str(),s4.c_str()) || strstr(tmp.c_str(),s2.c_str()) || strstr(tmp.c_str(),s5.c_str()) ||  strstr(tmp.c_str(),s7.c_str())){
			SubListMET.push_back(tmp);
		}
		if(strstr(tmp.c_str(),s6.c_str())){
			SubListPT.push_back(tmp);
		}
		
	}
	cout << triggerNames.size() << endl;

	ofstream TrigPrescaledList;
	TrigPrescaledList.open(NameCompletePrescaledList);

	
	/*for(int i = 0; i < triggerNames.size() ; i++){

		//cout << triggerNames[i] << endl;
		if(passTrigger[i] != 1 ){
			TrigPrescaledList << triggerNames[i] << " has prescale = " << passTrigger[i] << endl;
		}
		else{
			TrigPrescaledList << triggerNames[i] << " has prescale 1" << endl;

		}

	}*/

	TrigPrescaledList.close();
	ifile.close();

	vector<string> str;
	string interfstr;
	
	ifstream inttrigs("PrescaledSubList.txt"); 
	while(getline(inttrigs,tmp)){
   		str.push_back(tmp);
	}
	inttrigs.close();
		
	string StudyData = DataType + Date;
	string StudyTxt = TransferTxt + DataType + Date;
	string StudyEff= TransferEff + DataType + Date;
	string StudyZ= TransferZ + DataType + Date;
	string StudyW = TransferW + DataType + Date;
	string StudyDistribZ = TransferDistribZ + DataType + Date;

	string NameOfFile = StudyData + SubNum + ExtRoot;

	string NameOfEff = StudyEff + SubNum + ExtTxt;
	string NameOfTxt = StudyTxt + SubNum + ExtTxt;
	string EntriesFromZ = StudyZ + SubNum + ExtTxt;
	string EntriesFromW = StudyW + SubNum + ExtTxt;
	string distribvarZ = StudyDistribZ + SubNum + ExtRoot;

	/*DISTRIB_PT = new TH1D("DISTRIB_PT", "( PT )", 620,0,1550);
	DISTRIB_ETA = new TH1D("DISTRIB_ETA", "( ETA )", 400,-8,8);
	DISTRIB_IH = new TH1D("DISTRIB_IH", "( IH )", 400,0,8);
	DISTRIB_P = new TH1D("DISTRIB_P", "( P )", 1240,0,3100);
	DISTRIB_IAS = new TH1D("DISTRIB_IAS", "( IAS )",400,0,1.2);
	DISTRIB_IH_IAS = new TH2D("DISTRIB_IH_IAS", "IH ( IAS ) ", 100 , 0 , 1.2 , 100, 0 , 8 );
	DISTRIB_PT_P = new TH2D("DISTRIB_PT_P", "PT ( P ) ", 620 , 0 , 1550 , 1240, 0 , 3100 );

	DISTRIB_PT->Sumw2();
	DISTRIB_IAS->Sumw2();
	DISTRIB_ETA->Sumw2();
	DISTRIB_IH->Sumw2();
	DISTRIB_P->Sumw2();
	DISTRIB_IH_IAS->Sumw2();
	DISTRIB_PT_P->Sumw2();*/

	MUONPT_DISTRIB = new TH1D("MuonPT close to Z", "muon_pt close to z peak", 50,0,100);
	ISOR03_DISTRIB = new TH1D("ISOR03 close to Z", "ISOR03 close to z peak", 50,0,100);
	trigEff_selection_obs.LoadNoMap(triggerNames,SubListMET,1,DataType,NameOfFile); 
	//trigEff_presel.LoadNoMap(triggerNames,SubListMET,1,DataType2,NameOfFile);
	//a
	
	str.clear();
	SubListMET.clear();
	SubListPT.clear();

	int counter=0,passedevent=0,nbofpairs=0,nbofpairsZ=0,nbofWpairs=0,nbofmuonsW=0,nbmuons=0,nbwrong=0;
	int indexcandidate;
	double InvMass;
	double MissingW;
	//nentries=30;
	ofstream InfosZ;
	InfosZ.open (EntriesFromZ);

	ofstream InfosW;
	InfosW.open (EntriesFromW);


	cout << "Before loop nentries" << endl;
	for (Long64_t jentry=0; jentry<nentries;jentry++) { //All entries
		Long64_t ientry = LoadTree(jentry);
		if(jentry!=0 && jentry%1000==0) cout << "+1k" << " => " << jentry << " , "<<(jentry*1.0/nentries)*100 << " %" << endl;
		if (ientry < 0) break;
        	nb = fChain->GetEntry(jentry);   nbytes += nb;	// 
		
		InvMass = MuonsInvariantMass();

		MissingW = MuonsMissingET();


		/*for ( int jtrack = 0 ; jtrack < ntracks ; jtrack++){
			DISTRIB_PT->Fill(track_pt[jtrack]);
			DISTRIB_ETA->Fill(track_eta[jtrack]);
			DISTRIB_IH->Fill(track_ih_ampl[jtrack]);
			DISTRIB_P->Fill(track_p[jtrack]);
			DISTRIB_IAS->Fill(track_ias_ampl[jtrack]);
			DISTRIB_IH_IAS->Fill(track_ias_ampl[jtrack],track_ih_ampl[jtrack]);
			DISTRIB_PT_P->Fill(track_p[jtrack],track_pt[jtrack]);
		}*/

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

		if(MissingW!=1){
			if(MissingW < massW + 10 && MissingW > massW - 10){ // 10 
				InfosW << "W found entry " << jentry << " muon " << muonW << endl;
				nbofmuonsW+=1;
			}
			//cout << InvMass << endl;
			nbofWpairs+=1;
			if(MissingW > 15){
				trigEff_selection_obs.FillMass(MissingW,2);
			}
		}


		nbmuons+=nmuons;
		counter+=1;
		vector<Bool_t> vtrigger; //Convert array into vector
		vector<int> position;
		vector< pair<int, bool > > PosPass;
		float HighestPT,HighestMuonPT,HighestMET;
		int trignull=0;
		indexcandidate=Selection();
		//cout << " -------- NEW ENTRY -------- " << endl;
		if(indexcandidate != 64){
			//cout << indexcandidate << endl;
			HighestPT = track_pt[indexcandidate];
			HighestMET = pfmet_pt[indexcandidate];
			for(int i=0;i<ntrigger;i++){
				vtrigger.push_back(passTrigger[i]);
				if(vtrigger[i] == 0){
					trignull+=1;
				}
			}
			if(trignull==ntrigger){
				nbwrong+=1;
			}
			for(int p = 0; p < ntrigger; p++){
				auto iter = std::find(triggerNames.begin(), triggerNames.end(), triggerName->at(p));
				if(iter == triggerNames.end()){
					//cout << " one trigger not found in CompleteList" << endl;
				}
				else{
					auto pos = std::distance(triggerNames.begin(), iter);
					PosPass.push_back(make_pair(pos,vtrigger[p])); // [pos] ?
					//cout << "found trigger " << p << " ( " << triggerName->at(p) << " ) " << " in position" << pos << " inside CompleteList" << endl;
				}
				
			}
			
			passedevent+=1;
			trigEff_selection_obs.FillNoMap2(PosPass,HighestPT,1);
			//cout << "After fill" << endl;
			//trigEff_selection_obs.FillNoMap(vtrigger,HighestPT,1);
			//trigEff_presel.FillNoMap(vtrigger,HighestMET);					
		}
	}
	cout << "After loop nentries" << endl;
	InfosZ.close();
	InfosW.close();
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


	cout << " # of times all triggers = 0 : " << nbwrong << endl;
	InfosData << "# muons as a pair (Z)/ total # of muons : " << nbofpairsZ << " / " << nbmuons << " , Ratio :" << (nbofpairsZ*2.0/nbmuons)*100 << " %" << endl << endl <<  "Ratio pair Z / total pairs:" << (nbofpairsZ*1.0/nbofpairs)*100 << " %" << endl;


	InfosData.close();
	trigEff_selection_obs.Compute(NameOfEff,NameListEff,ListAllTriggers,EffTriggers);

	//trigEff_presel.Compute("test_TriggersOfInterest_MET_withmap.txt");
	
	triggerNames.clear();
	
	trigEff_selection_obs.WritePlots("",NameOfFile);

	distrib = new TFile(distribvarZ.c_str(),"RECREATE");
	
	distrib->cd();
	MUONPT_DISTRIB->Write();
	ISOR03_DISTRIB->Write();
	/*DISTRIB_PT->Write();
	DISTRIB_IH->Write();
	DISTRIB_IAS->Write();
	DISTRIB_ETA->Write();
	DISTRIB_P->Write();
	DISTRIB_IH_IAS->Write();
	DISTRIB_PT_P->Write();*/
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


double AnaEff::MuonsMissingET(){
	vector< pair<float, int > > muonPT,muonPHI,muonETA,muonP;
	double MissingET,mu_phi,mu_eta,mu_pt,mu_px,mu_py,mu_pz,mu_p;
	
	TLorentzVector muon,sum,transf;
	
	vector<double> missingET;
	// indications eric : pz = 0, pas d'infos sur pz,px,py ? muon_p / muon_pt peut donner px ?
	
	// recuperer px py pz de ce vecteur
	// 
	//dylan : mu.SetPtEtaPhiM(muon_pt,muon_eta,muon_phi,massMu);
	//trouver la MET si il y en a dans une trace opposée pi/2
	//sum both to get mass,  W probably (10% mu+nu in W) 
	
	//
	if(nmuons>1){
		for(int i = 0; i < nmuons ; i++){
			if(muon_pt[i] > 10){
				muonPT.push_back(make_pair(muon_pt[i],i));
				muonETA.push_back(make_pair(muon_eta[i],i));
				muonPHI.push_back(make_pair(muon_phi[i],i));
				muonP.push_back(make_pair(muon_p[i],i));
			}
			
		}

	}
	sort(muonPT.begin(),muonPT.end());
	int index = muonPT[muonPT.size()-1].second;
	
	for(int k = 0; k < muonPT.size(); k++){
		if(index == muonETA[k].second){
			mu_eta = muonETA[k].first;
		}

		if(index == muonPHI[k].second){
			mu_phi = muonPHI[k].first;
		}
		if(index == muonP[k].second){
			mu_p = muonP[k].first;
		}

	}
	mu_pt = muonPT[index].first;
	
	if(muonPT.size() < 2){
		return 1;
	}
	else{
		transf.SetPtEtaPhiM(mu_pt,mu_eta,mu_phi,massMu);
		mu_px = transf.Px();
		mu_py = transf.Py();
		mu_pz = transf.Pz();
		double Energy = sqrt((massMu*massMu)+mu_p*mu_p);
		//cout << " energy = " << Energy << endl;
		muon.SetPxPyPzE(mu_px,mu_py,0,Energy);
		muonW = index; 
		//test3.SetCoordinates(mu_px,mu_py,0,0.105);
		//get E from m p 
		
		//test3.SetM(0.105);
		

		//cout << " [px,py,pz,M] = " << "{" << muon.Px() << "," << muon.Py() << "," << muon.Pz() << "," << muon.M() << "]" << endl;
		double invmassw = muon.M();
		//cout << "InvMass transverse = " << invmassw << endl;
		return invmassw;
	}
	return 1;
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


