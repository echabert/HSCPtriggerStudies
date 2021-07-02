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
#include <Math/GenVector/LorentzVector.h>
#include <Math/GenVector/PxPyPzM4D.h>


using namespace std;

const double massZ = 91.1876;
const double uncertaintyZ = 0.0021;

const double massMu = 0.1056583745;
const double uncertaintyMu = 0.0000000024;

const double massW = 80.379;
const double uncertaintyW = 0.012;

const float TheorMass = 1800;

void AnaEff::Loop()
{
	

	Long64_t nentries = fChain->GetEntriesFast();
	Long64_t nbytes = 0, nb = 0, nbi = 0;
	Long64_t initializing = LoadTree(0); 
	if (initializing < 0) cout << "Aborted"<< endl;
	nbi = fChain->GetEntry(initializing);   nbytes += nbi;
	cout << "Number of triggers for this file  : " << ntrigger << endl;
	cout << " Number of events for this file : " << nentries << endl;

	
	string NameList = "CompleteList",PrescaledList = "PrescaledList",ListAll = "ListOfAllTriggersEff";
	
	string SubNum = "all",ExtRoot = ".root",ExtTxt = ".txt",Date="1105", Or = "LogicalOr"; //to_string(2);
	
	string TransferTxt="AllInfos",TransferEff = "Eff",TransferZ = "EntriesFromZ",TransferW = "EntriesFromW";
	
	string ErrorEffTransfer = "Error";
	

	string TransferDistribZ = "DistribZpeak";
	string TransferDistribW = "DistribWpeak";
	string DataType = "Gluino1800";
	
	string NameCompleteListTest = "ListeInteretTriggers";
	string ListAllTriggers = ListAll + DataType + ExtTxt;
	//string NameCompleteList = "CompleteListTest.txt";

	string NameListEff = TransferEff + DataType + ExtTxt;
	string ErrorEffTriggers = ErrorEffTransfer + TransferEff + DataType + SubNum + ExtTxt;

	string NameCompleteList = NameList + DataType + ExtTxt; // + DataType for others
	
	string NameListForType = NameCompleteListTest + DataType + ExtTxt;
	string NameCompletePrescaledList = PrescaledList + DataType + ExtTxt;
	string EffTriggers = TransferEff + DataType + SubNum + ExtTxt;
	string OrAllTriggers = Or + SubNum + DataType + ExtTxt;

	string s1 = "mu",s2="Mu",s3 = "TESTPT", s4 = "testmu", s5 = "Muon", s7 = "MET", s8 = "HT";
	
	
	
	ifstream ifile(NameListForType.c_str()); 
	vector<string> triggerNames;
	vector<string> SubListMET;
	vector<string> SubListPT;
	string tmp;

	while(getline(ifile,tmp)){
   		triggerNames.push_back(tmp);
		if(strstr(tmp.c_str(),s1.c_str()) || strstr(tmp.c_str(),s2.c_str()) || strstr(tmp.c_str(),s5.c_str()) ||  strstr(tmp.c_str(),s7.c_str()) ||  strstr(tmp.c_str(),s8.c_str())){
			SubListMET.push_back(tmp);
		}
		if(strstr(tmp.c_str(),s4.c_str())){
			SubListPT.push_back(tmp);
		}
		
	}
	cout << triggerNames.size() << endl;

	ifile.close();

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

	DISTRIB_PT = new TH1D("DISTRIB_PT", "( PT )", 110,0,2550);
	DISTRIB_PT->GetXaxis()->SetTitle("PT [GeV/c*c]");
	DISTRIB_PT->GetYaxis()->SetTitle("# HSCP");
 
	DISTRIB_P = new TH1D("DISTRIB_P", "( P )", 110,0,2550);
	DISTRIB_P->GetXaxis()->SetTitle("Reco pf_MET[GeV/c*c]");
	DISTRIB_P->GetYaxis()->SetTitle("# HSCP");

	DISTRIB_METNOSEL = new TH1D("DISTRIB_METNOSEL", "( MET )", 100,0,4000);
	DISTRIB_METNOSEL->GetXaxis()->SetTitle("MET [GeV]");
	DISTRIB_METNOSEL->GetYaxis()->SetTitle("# HSCP");


	DISTRIB_METPRESEL = new TH1D("DISTRIB_METPRESEL", "( MET )", 100,0,4000);
	DISTRIB_METPRESEL->GetXaxis()->SetTitle("MET [GeV]");
	DISTRIB_METPRESEL->GetYaxis()->SetTitle("# HSCP");

	DISTRIB_METSEL = new TH1D("DISTRIB_METSEL", "( MET )", 100,0,4000);
	DISTRIB_METSEL->GetXaxis()->SetTitle("MET [GeV]");
	DISTRIB_METSEL->GetYaxis()->SetTitle("# HSCP");

	DISTRIB_PT1_PT2 = new TH2D("DISTRIB_PT1_PT2", "PT1_PT2 ", 300 , 0 , 2000 , 300, 0 , 2000 );
	DISTRIB_PT1_PT2->GetXaxis()->SetTitle("PT candidate 1");
	DISTRIB_PT1_PT2->GetYaxis()->SetTitle("PT candidate 2");


	DISTRIB_IAS = new TH1D("DISTRIB_IAS", "( IAS )",80,0,1.2);
	DISTRIB_IAS->GetXaxis()->SetTitle("Ias");
	DISTRIB_IAS->GetYaxis()->SetTitle("# HSCP");

	DISTRIB_IASCHN = new TH1D("DISTRIB_IASCHN", "( IAS ) CHN",80,0,1.2);
	DISTRIB_IASCHN->GetXaxis()->SetTitle("Ias");
	DISTRIB_IASCHN->GetYaxis()->SetTitle("# HSCP");
	
	DISTRIB_IASCHCH = new TH1D("DISTRIB_IASCHCH", "( IAS )CHCH",80,0,1.2);
	DISTRIB_IASCHCH->GetXaxis()->SetTitle("Ias");
	DISTRIB_IASCHCH->GetYaxis()->SetTitle("# HSCP");

	DISTRIB_IASDCH = new TH1D("DISTRIB_IASDCH", "( IAS )DCH",80,0,1.2);
	DISTRIB_IASDCH->GetXaxis()->SetTitle("Ias");
	DISTRIB_IASDCH->GetYaxis()->SetTitle("# HSCP");


	DISTRIB_IH = new TH1D ("DISTRIB_IH", " IH ", 100, 0 , 15);
	DISTRIB_IH->GetXaxis()->SetTitle("Ih");
	DISTRIB_IH->GetYaxis()->SetTitle("# HSCP");

	DISTRIB_IHCHN = new TH1D ("DISTRIB_IHCHN", " IH CHN ", 100, 0 , 15);
	DISTRIB_IHCHN->GetXaxis()->SetTitle("Ih");
	DISTRIB_IHCHN->GetYaxis()->SetTitle("# HSCP");

	DISTRIB_IHCHCH = new TH1D ("DISTRIB_IHCHCH", " IH CHCH", 100, 0 , 30);
	DISTRIB_IHCHCH->GetXaxis()->SetTitle("Ih");
	DISTRIB_IHCHCH->GetYaxis()->SetTitle("# HSCP");

	DISTRIB_IHDCH = new TH1D ("DISTRIB_IHDCH", " IH DCH", 100, 0 , 30);
	DISTRIB_IHDCH->GetXaxis()->SetTitle("Ih");
	DISTRIB_IHDCH->GetYaxis()->SetTitle("# HSCP");

	DISTRIB_POVERM = new TH1D ("DISTRIB_POVERM", "( P/ M )", 80,0,2.5);
	DISTRIB_POVERM->GetXaxis()->SetTitle("p/m = βγ");
	DISTRIB_POVERM->GetYaxis()->SetTitle("# HSCP");


	DISTRIB_POVERMASSO1 = new TH1D ("DISTRIB_POVERMASSO1", "( P/ M )", 80,0,2.5);
	DISTRIB_POVERMASSO1->GetXaxis()->SetTitle("p/m = βγ");
	DISTRIB_POVERMASSO1->GetYaxis()->SetTitle("# HSCP");

	DISTRIB_POVERMASSO2 = new TH1D ("DISTRIB_POVERMASS02", "( P/ M )", 80,0,2.5);
	DISTRIB_POVERMASSO2->GetXaxis()->SetTitle("p/m = βγ");
	DISTRIB_POVERMASSO2->GetYaxis()->SetTitle("# HSCP");

	DISTRIB_P1MP2 = new TH1D ("DISTRIB_P1MP2", "P1MP2", 100, -5, 5);
	DISTRIB_P1MP2->GetXaxis()->SetTitle("2*(p1 - p2) / (p1 + p2)");
	DISTRIB_P1MP2->GetYaxis()->SetTitle("# HSCP");

	DISTRIB_P1MP2CHN = new TH1D ("DISTRIB_P1MP2CHN", "P1MP2CHN", 100, -5, 5);
	DISTRIB_P1MP2CHN->GetXaxis()->SetTitle("2*(p1 - p2) / (p1 + p2)");
	DISTRIB_P1MP2CHN->GetYaxis()->SetTitle("# HSCP");

	DISTRIB_P1MP2CHCH = new TH1D ("DISTRIB_P1MP2CHCH", "P1MP2CHCH", 100, -5, 5);
	DISTRIB_P1MP2CHCH->GetXaxis()->SetTitle("2*(p1 - p2) / (p1 + p2)");
	DISTRIB_P1MP2CHCH->GetYaxis()->SetTitle("# HSCP");


	DISTRIB_MET = new TH1D ("DISTRIB_MET", " ( MET ) " , 100,0,500);
	DISTRIB_MET->GetXaxis()->SetTitle("MET (GeV)");
	DISTRIB_MET->GetYaxis()->SetTitle("# HSCP");


	DISTRIB_MET_CHCH = new TH1D ("DISTRIB_MET_CHCH", " ( MET CHCH) " , 100,0,4000);
	DISTRIB_MET_CHCH->GetXaxis()->SetTitle("MET (GeV)");
	DISTRIB_MET_CHCH->GetYaxis()->SetTitle("# HSCP");

	DISTRIB_MET_CHN = new TH1D ("DISTRIB_MET_CHN", " ( MET CHN) " , 100,0,4000);
	DISTRIB_MET_CHN->GetXaxis()->SetTitle("MET (GeV)");
	DISTRIB_MET_CHN->GetYaxis()->SetTitle("# HSCP");

	//DISTRIB_IH_IAS = new TH2D("DISTRIB_IH_IAS", "IH ( IAS ) ", 100 , 0 , 1.2 , 100, 0 , 8 );

	DISTRIB_MET_pt = new TH2D("DISTRIB_MET_pt", "Met vs pt", 600, 0, 4000, 600, 0, 4000);
	DISTRIB_MET_pt->GetYaxis()->SetTitle("Reco MET [GeV]");
	DISTRIB_MET_pt->GetXaxis()->SetTitle("Pt [GeV]");

	DISTRIB_MET_eta = new TH2D("DISTRIB_MET_eta", "Met vs eta", 100, -2.1, 2.1, 600, 0, 4000);
	DISTRIB_MET_eta->GetYaxis()->SetTitle("Reco MET [GeV]");
	DISTRIB_MET_eta->GetXaxis()->SetTitle("Eta");

	DISTRIB_MET_iso = new TH2D("DISTRIB_MET_iso", "Met vs iso", 100, 0, 8, 600, 0, 4000);
	DISTRIB_MET_iso->GetYaxis()->SetTitle("Reco MET [GeV]");
	DISTRIB_MET_iso->GetXaxis()->SetTitle("iso");

	DISTRIB_P1_P2 = new TH2D("DISTRIB_P1_P2", "P1_P2", 600 , 0 , 4000 , 600, 0 , 4000 );
	DISTRIB_P1_P2->GetXaxis()->SetTitle("P candidate 1");
	DISTRIB_P1_P2->GetYaxis()->SetTitle("P candidate 2");


	DISTRIB_P1_P2_CHN = new TH2D("DISTRIB_P1_P2_CHN", "P1_P2CHN", 600 , 0 , 4000 , 600, 0 , 4000 );
	DISTRIB_P1_P2_CHN->GetXaxis()->SetTitle("P candidate 1");
	DISTRIB_P1_P2_CHN->GetYaxis()->SetTitle("P candidate 2");

	DISTRIB_P1_P2_CHCH = new TH2D("DISTRIB_P1_P2_CHCH", "P1_P2CHCH", 600 , 0 , 4000 , 600, 0 , 4000 );
	DISTRIB_P1_P2_CHCH->GetXaxis()->SetTitle("P candidate 1");
	DISTRIB_P1_P2_CHCH->GetYaxis()->SetTitle("P candidate 2");


	DISTRIB_PT->Sumw2();
	DISTRIB_IAS->Sumw2();
	DISTRIB_POVERM->Sumw2();

	DISTRIB_POVERMASSO1->Sumw2();
	DISTRIB_POVERMASSO2->Sumw2();

	DISTRIB_METNOSEL->Sumw2();
	DISTRIB_METPRESEL->Sumw2();
	DISTRIB_METSEL->Sumw2();
	DISTRIB_PT1_PT2->Sumw2();

	DISTRIB_MET_CHN->Sumw2();
	DISTRIB_MET_CHCH->Sumw2();

	DISTRIB_MET->Sumw2();
	DISTRIB_MET_pt->Sumw2();
	DISTRIB_MET_eta->Sumw2();
	DISTRIB_MET_iso->Sumw2();
	DISTRIB_P->Sumw2();

	DISTRIB_IHCHN->Sumw2();
	DISTRIB_IHDCH->Sumw2();
	DISTRIB_IHCHCH->Sumw2();

	DISTRIB_IASCHN->Sumw2();
	DISTRIB_IASDCH->Sumw2();
	DISTRIB_IASCHCH->Sumw2();
	
	
	DISTRIB_P1_P2_CHCH->Sumw2();
	DISTRIB_P1_P2_CHN->Sumw2();
	DISTRIB_P1_P2->Sumw2();

	MUONPT_DISTRIB = new TH1D("MuonPT close to Z", "muon_pt close to z peak", 50,0,100);
	ISOR03_DISTRIB = new TH1D("ISOR03 close to Z", "ISOR03 close to z peak", 50,0,100);

	trigEff_selection_obs.LoadNoMap(triggerNames,triggerNames,1,DataType,NameOfFile); 
	
	SubListMET.clear();
	SubListPT.clear();

	int counter=0,passedevent=0,nbofpairs=0,nbofpairsZ=0,nbofWpairs=0,nbofmuonsW=0,nbmuons=0,nbwrong=0;
	int indexcandidate,indexcandidatesel;
	double InvMass;
	double MissingW;
	ofstream InfosZ;
	InfosZ.open (EntriesFromZ);

	ofstream InfosW;
	InfosW.open (EntriesFromW);


	cout << "Before loop nentries" << endl;
	for (Long64_t jentry=0; jentry<nentries;jentry++) { //All entries
		Long64_t ientry = LoadTree(jentry);
		if(jentry!=0 && jentry%1000==0) cout << "+1k" << " => " << jentry << " , "<<(jentry*1.0/nentries)*100 << " %" << endl;
		if (ientry < 0) break;
        	nb = fChain->GetEntry(jentry);   nbytes += nb;	
	
		InvMass = MuonsInvariantMass();
		MissingW = MuonsMissingET();
		DISTRIB_METNOSEL->Fill(pfmet_pt[0]);

		if(InvMass!=1){
			if(InvMass < massZ + 10 && InvMass > massZ - 10){ 
				InfosZ << "Z found entry " << jentry << " muons " << muon1 << " and " << muon2 << endl;
				nbofpairsZ+=1;
			}
			
			nbofpairs+=1;
			if(InvMass > 15){
				trigEff_selection_obs.FillMass(InvMass,1);
			}
		}

		if(MissingW!=1){
			if(MissingW < massW + 10 && MissingW > massW - 10){ 
				InfosW << "W found entry " << jentry << " muon " << muonW << endl;
				nbofmuonsW+=1;
			}
			nbofWpairs+=1;
			if(MissingW > 15){
				trigEff_selection_obs.FillMass(MissingW,2);
			}
		}

		nbmuons+=nmuons;
		counter+=1;
		vector<Bool_t> vtrigger; 
		vector<int> position;
		vector< pair<int, bool > > PosPass;
		float HighestPT,HighestMuonPT,HighestMET,POVERMBG, HighestP;
		int trignull=0;
		indexcandidate=Preselection();
		
		//cout << " -------- NEW ENTRY -------- " << endl;
		if(indexcandidate != 64){

			DISTRIB_METPRESEL->Fill(pfmet_pt[0]);
			indexcandidatesel = Selection(indexcandidate);
			if(indexcandidatesel != 64){
				

				DISTRIB_MET_iso->Fill(hscp_iso2_tk[indexcandidatesel],pfmet_pt[0]);
				DISTRIB_MET_eta->Fill(track_eta[hscp_track_idx[indexcandidatesel]],pfmet_pt[0]);
				DISTRIB_MET_pt->Fill(track_pt[hscp_track_idx[indexcandidatesel]],pfmet_pt[0]);

				DISTRIB_METSEL->Fill(pfmet_pt[0]);

 				AssoGenId(indexcandidatesel);
		
				DISTRIB_PT->Fill(track_pt[hscp_track_idx[indexcandidatesel]]);
				DISTRIB_IAS->Fill(track_ias_ampl[hscp_track_idx[indexcandidatesel]]);
			
				HighestPT = track_pt[hscp_track_idx[indexcandidatesel]];
				HighestMET = pfmet_pt[0];

					

				//cout << " MET : " << HighestMET << endl;

				HighestP = track_p[hscp_track_idx[indexcandidatesel]];
				POVERMBG = (track_p[hscp_track_idx[indexcandidatesel]] *1.0/ TheorMass);
				DISTRIB_P->Fill(HighestP);
				DISTRIB_POVERM->Fill(POVERMBG);
				if(HighestMET > 5 ){
					DISTRIB_MET->Fill(HighestMET);
				}

				for(int i=0;i<ntrigger;i++){
					vtrigger.push_back(passTrigger[i]);
					if(vtrigger[i] == 0){
						trignull+=1;
					}
				}
				if(trignull==ntrigger){
					nbwrong+=1;
				}
				bool ismissing = false;
				for(int p = 0; p < ntrigger; p++){
					auto iter = std::find(triggerNames.begin(), triggerNames.end(), triggerName->at(p));
					if(iter == triggerNames.end()){
				
					}
					else{
						ismissing = true;
						auto pos = std::distance(triggerNames.begin(), iter);
						PosPass.push_back(make_pair(pos,vtrigger[p]));	
					}
				}
				if (ismissing == false){
					cout << " at least one trigger not found in list ?" << endl;
				}
				passedevent+=1;
				trigEff_selection_obs.FillNoMap2(PosPass,HighestMET,1);
			}					
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

	cout << "--------------------- Gluinos ---------------- "  << endl;
	cout << " There was " << nbtot << " events, " << nbchn << " charged + neutral and" << nbchch << " charged + charged" << endl;
	cout << " Charged-Charged : " << nbchch << " / " << nbtot << " = " << nbchch*1.0/nbtot <<  endl;
	cout << " Neutral-Charged : " << nbchn << " / " << nbtot << " = "  <<  nbchn*1.0/nbtot << endl;
	cout << " Neutral-X : " << nbnn << " / " << nbtot << " = "  <<  nbnn*1.0/nbtot << endl;
	cout << "Double charged R-hadrons :  " << nbtch << " / " << nbtot << " = " << nbtch*1.0/nbtot << endl;

	InfosData.close();
	trigEff_selection_obs.Compute(NameOfEff,NameListEff,ListAllTriggers,EffTriggers, ErrorEffTriggers, OrAllTriggers);

	//trigEff_presel.Compute("test_TriggersOfInterest_MET_withmap.txt");
	cout << "After compute" << endl;
	triggerNames.clear();
	
	trigEff_selection_obs.WritePlots("",NameOfFile);
	cout << "After WritePlots" << endl;
	distrib = new TFile(distribvarZ.c_str(),"RECREATE");
	
	distrib->cd();
	MUONPT_DISTRIB->Write();
	ISOR03_DISTRIB->Write();
	DISTRIB_P->Write();
	DISTRIB_MET->Write();
	DISTRIB_PT->Write();

	DISTRIB_IH->Write();
	DISTRIB_IHCHN->Write();
	DISTRIB_IHCHCH->Write();
	DISTRIB_IHDCH->Write();

	DISTRIB_IAS->Write();
	DISTRIB_IASCHN->Write();
	DISTRIB_IASCHCH->Write();
	DISTRIB_IASDCH->Write();


	DISTRIB_P1MP2->Write();
	DISTRIB_P1MP2CHN->Write();
	DISTRIB_P1MP2CHCH->Write();

	DISTRIB_MET_iso->Write();
	DISTRIB_MET_eta->Write();
	DISTRIB_MET_pt->Write();

	DISTRIB_POVERM->Write();
	DISTRIB_PT1_PT2->Write();
	DISTRIB_METNOSEL->Write();
	DISTRIB_METPRESEL->Write();
	DISTRIB_METSEL->Write();
	DISTRIB_MET_CHCH->Write();
	DISTRIB_MET_CHN->Write();

	DISTRIB_POVERMASSO1->Write();
	DISTRIB_POVERMASSO2->Write();

	DISTRIB_P1_P2->Write();
	DISTRIB_P1_P2_CHN->Write();
	DISTRIB_P1_P2_CHCH->Write();
	
	distrib->Close();
	cout << "Program terminated without any logic call out of bound" << endl;
	

}

int AnaEff::Preselection(){
	int index=64,count2=0;
	vector<int> positions;
	vector< pair<float, int > > Muonpt,HSCPpt,HSCPponm;
	
	bool yon=true;
	for(int ihs=0; ihs<nhscp;ihs++){
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
		if( track_pt[hscp_track_idx[ihs]] <= 55 ){ //55
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
		if(track_ias_ampl[hscp_track_idx[ihs]] < 0.8){ 
			yon = false;
		}
		
		if(yon){
			positions.push_back(ihs); 
			HSCPpt.push_back(make_pair(track_pt[hscp_track_idx[ihs]],ihs));
			Muonpt.push_back(make_pair(muon_pt[ihs],ihs));
		}
		
	}

	if(positions.size() != 0){
		int siz=Muonpt.size(),sizH = HSCPpt.size();

		sort(Muonpt.begin(),Muonpt.end());
		sort(HSCPpt.begin(),HSCPpt.end());
		return HSCPpt[sizH-1].second;
		//return Muonpt[siz-1].second;
	}
	else{
		return 64;
	}

}

int AnaEff::Selection(int indexcandidate){
	bool yoy = false;
	
	if(track_ias_ampl[hscp_track_idx[indexcandidate]] > 0.2){ 
			yoy = true;
			return indexcandidate;
		}

	else{
		return 64;
	}
	
}

int AnaEff::fact(int n){
     return (n==0) || (n==1) ? 1 : n* fact(n-1);
}




double AnaEff::MuonsMissingET(){
	vector< pair<float, int > > muonPT,muonPHI,muonETA,muonP;
	double MissingET,mu_phi,mu_eta,mu_pt,mu_px,mu_py,mu_pz,mu_p;
	
	TLorentzVector muon,sum,transf;
	
	vector<double> missingET;

	if(nmuons<1){
		return 1;

	}
	else {
		for(int i = 0; i < nmuons ; i++){
			if(muon_pt[i] > 10){
				muonPT.push_back(make_pair(muon_pt[i],i));
				muonETA.push_back(make_pair(muon_eta[i],i));
				muonPHI.push_back(make_pair(muon_phi[i],i));
				muonP.push_back(make_pair(muon_p[i],i));
			}
			
		}
		
		if(muonPT.size() < 2){
			return 1;
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
			
			muon.SetPxPyPzE(mu_px,mu_py,0,Energy);
			muonW = index; 
			double invmassw = muon.M();
			return invmassw;
		}
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
			double armass = sum.M();	
			return armass;
		}
		else{
			return 1;
		}
	}

	else if(nmuons > 2){
		nbcomb = (fact(nmuons) / (fact(2) * fact(nmuons-2)) );
		for(int i = 0; i < nmuons ; i++){
			if(muon_pt[i] > 10){
				muonPT.push_back(make_pair(muon_pt[i],i));
				muonETA.push_back(make_pair(muon_eta[i],i));
				muonPHI.push_back(make_pair(muon_phi[i],i));
			}
			
		}

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
			if((abs(invmass[u]-massZ)) < tram){
				pom=u;
				tram=abs(invmass[u]-massZ);
			}
		}
	
		muon1 = binom[pom].first;
		muon2 = binom[pom].second;

		if(invmass[pom] < massZ + 10 && invmass[pom] > massZ -10){
			MUONPT_DISTRIB->Fill(muon_pt[binom[pom].first]);
			ISOR03_DISTRIB->Fill(muon_isoR03_sumChargedHadronPt[binom[pom].first]);	
		}
		binom.clear();
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



void AnaEff::AssoGenId(int indexcandidate){

	vector<int> indexpdgch{1009213, 1009323, 1092214, 1091114, 1093114, 1093224, 1093314, 1093334, 1000612, 1000632, 1000652, 1006211, 1006213, 1006313, 1006321, 1006323 };
	vector<int> indexpdgn{1000622, 1093324, 1092114, 1000993, 1009113, 1009223, 1009313, 1009333, 1093214, 1000642, 1006113, 1006311, 1006313};
	vector<int> indexpdgch2{1006223, 1092224};

	vector<int> candidatesrh,candidatesneutral,candidatesdoublech;
	int nbmothgen=0;
	int nbnn=0;
	double p1=0,p2=0,eta1=0,eta2=0,pt1=0,pt2=0;
	float poverm1,poverm2;
	//cout << " NEW EVENT ------"<<endl;
	for(int i=0; i < ngenpart ; i++){
		
		//cout << "gen : " << gen_pdg[i] << " , gen_moth : " << gen_moth_pdg[i] << " , status : " << gen_status[i] << " , p = pt * cosh(eta) : " << gen_pt[i] * cosh(gen_eta[i]) << endl;
		if(gen_moth_pdg[i] == 1000021){
			nbmothgen+=1;
		}

		for(int k = 0; k < indexpdgch.size() ; k++){
			if(abs(gen_pdg[i]) == indexpdgch[k]){
				if(gen_status[i] == 1){
					nbch+=1;
					candidatesrh.push_back(i);
				}
			}
		}
		
		for(int j=0; j < indexpdgn.size(); j++){
			if(abs(gen_pdg[i]) == indexpdgn[j]){
				if(gen_status[i] == 1){
				 nbn+=1;
				 candidatesneutral.push_back(i);
				}
			}
		}
		
		
		for(int j=0; j < indexpdgch2.size(); j++){
			if(abs(gen_pdg[i]) == indexpdgch2[j]){
				if(gen_status[i] == 1){
					candidatesdoublech.push_back(i);
					cout << "gen : " << gen_pdg[i] << " , gen_moth : " << gen_moth_pdg[i] << " , status : " << gen_status[i] << " , p = pt * cosh(eta) : " << gen_pt[i] * cosh(gen_eta[i]) << endl;
				}
			}
		}
	}

	
	bool alo = false,alo2=false;


	nbtot+=1;
	//cout << "nb neutral : " << candidatesneutral.size() << " , nb charged : " << candidatesrh.size() << " ,nb tot = " << candidatesneutral.size() + candidatesrh.size() <<endl;
	if(candidatesdoublech.size() >= 1 ){
		nbtch+=1;
		double deltatranfrdch = deltaR2(track_eta[hscp_track_idx[indexcandidate]], track_phi[hscp_track_idx[indexcandidate]], gen_eta[candidatesdoublech[candidatesdoublech.size()-1]], gen_phi[candidatesdoublech[candidatesdoublech.size()-1]]);
		double finaldeltadch = deltaR(deltatranfrdch);	

		if(finaldeltadch < 0.3 ){
	
			DISTRIB_IHDCH->Fill(track_ih_ampl[hscp_track_idx[indexcandidate]]);
			DISTRIB_IASDCH->Fill(track_ias_ampl[hscp_track_idx[indexcandidate]]);
			
		}

		


		if(candidatesrh.size() >= 1){
			//cout << " There is " << candidatesdoublech.size() << " double charged and " << candidatesrh.size() <<" single charged " << endl;
		}
		else if(candidatesneutral.size() >= 1){
			//cout << " There is 1 double charged and 1 neutral" << endl;

		}

		else{

			//cout << " There is 1 double charged and nothing else" << endl;
		}

	}



	if( candidatesrh.size() == 0 && candidatesneutral.size() >= 1 ){
		nbnn+=1;

	}

	if( candidatesrh.size() >= 1 && candidatesneutral.size() >= 1 ){
		nbchn+=1;
		//cout << " charged + neutral " << endl;
		double p1 = gen_pt[candidatesrh[candidatesrh.size()-1]] * cosh(gen_eta[candidatesrh[candidatesrh.size()-1]]);
		double p2 = gen_pt[candidatesneutral[candidatesneutral.size()-1]] * cosh(gen_eta[candidatesneutral[candidatesneutral.size()-1]]);
		DISTRIB_MET_CHN->Fill(pfmet_pt[0]);
		//cout << " p1 = " << p1 << " , p2 = " << p2 << endl;
		double deltatranfr1chn = deltaR2(track_eta[hscp_track_idx[indexcandidate]], track_phi[hscp_track_idx[indexcandidate]], gen_eta[candidatesrh[candidatesrh.size()-1]], gen_phi[candidatesrh[candidatesrh.size()-1]]);
		double finaldeltachn1 = deltaR(deltatranfr1chn);

		double deltatranfr2chn = deltaR2(track_eta[hscp_track_idx[indexcandidate]], track_phi[hscp_track_idx[indexcandidate]], gen_eta[candidatesneutral[candidatesneutral.size()-1]], gen_phi[candidatesneutral[candidatesneutral.size()-1]]);
		double finaldeltachn2 = deltaR(deltatranfr2chn);
		
		DISTRIB_P1MP2CHN->Fill((2*(p1-p2))/(p1+p2));

		if(finaldeltachn1 < 0.3 || finaldeltachn2 < 0.3){
			alo=true;
			//if(finaldelta1chn < finaldelta2chn ){
				DISTRIB_IHCHN->Fill(track_ih_ampl[hscp_track_idx[indexcandidate]]);
				DISTRIB_IASCHN->Fill(track_ias_ampl[hscp_track_idx[indexcandidate]]);
				DISTRIB_P1_P2_CHN->Fill(p1,p2);
			//}
			//else{
				
			
			//}

		}
		/*else if ( finaldelta1chn < 0.3 && finaldelta2chn > 0.3){
			
		}
		else if( finaldelta1chn > 0.3 && finaldelta2chn < 0.3){
			
		}
		else if( finaldelta1chn > 0.3 && finaldelta2chn > 0.3){
			
		}*/


		DISTRIB_PT1_PT2->Fill(gen_pt[candidatesrh[candidatesrh.size()-1]],gen_pt[candidatesrh[candidatesneutral.size()-1]]);
		
		

	}


	else if(candidatesrh.size() >= 2 && candidatesneutral.size() == 0){
		//cout << "charged + charged " << endl;
		double p1chch = (gen_pt[candidatesrh[candidatesrh.size()-1]] * cosh(gen_eta[candidatesrh[candidatesrh.size()-1]]));
		double p2chch = (gen_pt[candidatesrh[candidatesrh.size()-2]] * cosh(gen_eta[candidatesrh[candidatesrh.size()-2]]));
		DISTRIB_MET_CHCH->Fill(pfmet_pt[0]);
		DISTRIB_P1MP2CHCH->Fill((2*(p1chch-p2chch))/(p1chch+p2chch));

		DISTRIB_PT1_PT2->Fill(gen_pt[candidatesrh[candidatesrh.size()-1]],gen_pt[candidatesrh[candidatesrh.size()-2]]);
		DISTRIB_P1_P2_CHCH->Fill(p1chch,p2chch);
		
		
		nbchch+=1;
		double deltatranfr1 = deltaR2(track_eta[hscp_track_idx[indexcandidate]], track_phi[hscp_track_idx[indexcandidate]], gen_eta[candidatesrh[candidatesrh.size()-1]], gen_phi[candidatesrh[candidatesrh.size()-1]]);
		double finaldelta1 = deltaR(deltatranfr1);

		double deltatranfr2 = deltaR2(track_eta[hscp_track_idx[indexcandidate]], track_phi[hscp_track_idx[indexcandidate]], gen_eta[candidatesrh[candidatesrh.size()-2]], gen_phi[candidatesrh[candidatesrh.size()-2]]);
		double finaldelta2 = deltaR(deltatranfr2);			

		poverm1 = ((gen_pt[candidatesrh[candidatesrh.size()-1]] * cosh(gen_eta[candidatesrh[candidatesrh.size()-1]]))/TheorMass);
		poverm2 = ((gen_pt[candidatesrh[candidatesrh.size()-2]] * cosh(gen_eta[candidatesrh[candidatesrh.size()-2]]))/TheorMass);

		if(finaldelta1 < 0.3 || finaldelta2 < 0.3){
			alo=true;
			DISTRIB_IHCHCH->Fill(track_ih_ampl[hscp_track_idx[indexcandidate]]);
			DISTRIB_IASCHCH->Fill(track_ias_ampl[hscp_track_idx[indexcandidate]]);
			DISTRIB_P1_P2_CHCH->Fill(p1chch,p2chch);


			/*if(finaldelta1 < finaldelta2 ){
				Psurm1 = poverm1;
			}
			else{
				Psurm1 = poverm2;
			}*/

		}
		/*else if ( finaldelta1 < 0.3 && finaldelta2 > 0.3){
			Psurm1 = poverm1;
		}
		else if( finaldelta1 > 0.3 && finaldelta2 < 0.3){
			Psurm1 = poverm2;
		}
		else if( finaldelta1 > 0.3 && finaldelta2 > 0.3){
			Psurm1 = 0;
		}*/

	}
	
	if(alo==false && alo2 == false){
		//cout << "no track matched any gluino" << endl;
	}
	candidatesrh.clear();
	candidatesneutral.clear();
	candidatesdoublech.clear();
}
int main(){

	AnaEff ana;
	ana.Loop();
	
}
