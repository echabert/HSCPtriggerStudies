#include <vector>
#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include <TTree.h>
#include <iostream>
#include <fstream>
#include <map>
#include <iterator>
#include <algorithm>
#include "CopyTree.h"


using namespace std; 

CopyTree::CopyTree(){
	file = 0;
	f2 = 0;
	file2 = 0;
	f22 = 0;
	file3 = 0;
	f23 = 0;
	file4 = 0;
	f24 = 0;
	/*for(int i=0; i < ListTriggers.size() ; i++){
		EffvsObs[i]=0;
	}*/
}

CopyTree::~CopyTree(){ 
	if(!file){
		delete file;
	}
	if(!f2){
		delete f2;
	}
	if(!file2){
		delete file2;
	}
	if(!f22){
		delete f22;
	}
	if(!file3){
		delete file3;
	}
	if(!f23){
		delete f23;
	}
	if(!file4){
		delete file4;
	}
	if(!f24){
		delete f24;
	}
}

void CopyTree::CopyWithSelec(string mode){


	TString cuts = "nmuons>2 && muon_pt[0] > 10 && muon_pt[1] > 10 && muon_eta[0] < 2.1 && muon_eta[0] > -2.1 && ndedxhits >= 5 && muon_isTrackerMuon[0] && muon_isTrackerMuon[1]";


	if(mode == "norm"){
		string namesmall = "small1057.root";
		string namesmall2 = "small1056.root";
		string namesmall3 = "small1055.root";
		string namesmall4 = "small1054.root";
		

		TString pathfile = "/opt/sbg/cms/ui3_data1/dapparu/HSCP/Production/prodApril2021_CMSSW_10_6_2/MET/0001/nt_data_aod_1-1057.root";
		TString pathfile2 = "/opt/sbg/cms/ui3_data1/dapparu/HSCP/Production/prodApril2021_CMSSW_10_6_2/MET/0001/nt_data_aod_1-1056.root";
		TString pathfile3 = "/opt/sbg/cms/ui3_data1/dapparu/HSCP/Production/prodApril2021_CMSSW_10_6_2/MET/0001/nt_data_aod_1-1055.root";
		TString pathfile4 = "/opt/sbg/cms/ui3_data1/dapparu/HSCP/Production/prodApril2021_CMSSW_10_6_2/MET/0001/nt_data_aod_1-1054.root";
		


		file = new TFile(pathfile);
		TTree *ntuple = (TTree*) file->Get("stage/ttree");
		f2 = new TFile(namesmall.c_str(),"RECREATE");
		f2->cd();
		f2->mkdir("stage");
		f2->cd("stage");
		TTree *small = ntuple->CopyTree(cuts);
		small->Write();
		f2->Close();

		cout << " Copied file 1" << endl;


		file2 = new TFile(pathfile2);
		TTree *ntuple2 = (TTree*) file2->Get("stage/ttree");
		f22 = new TFile(namesmall2.c_str(),"RECREATE");
		f22->cd();
		f22->mkdir("stage");
		f22->cd("stage");
		TTree *small2 = ntuple2->CopyTree(cuts);
		small2->Write();
		f22->Close();

		cout << " Copied file 2" << endl;


		file3 = new TFile(pathfile3);
		TTree *ntuple3 = (TTree*) file3->Get("stage/ttree");
		f23 = new TFile(namesmall3.c_str(),"RECREATE");
		f23->cd();
		f23->mkdir("stage");
		f23->cd("stage");
		TTree *small3 = ntuple3->CopyTree(cuts);
		small3->Write();
		f23->Close();

		cout << " Copied file 3" << endl;
	
		
		file4 = new TFile(pathfile4);
		TTree *ntuple4 = (TTree*) file4->Get("stage/ttree");
		f24 = new TFile(namesmall4.c_str(),"RECREATE");
		f24->cd();
		f24->mkdir("stage");
		f24->cd("stage");
		TTree *small4 = ntuple4->CopyTree(cuts);
		small4->Write();
		f24->Close();

		cout << " Copied file 4" << endl;
	

	}
	else if(mode == "aod"){
		
		TString filepathaod="/home/raph/CMS/prodMarch2021_CMSSW_10_6_2/nt_data_aod.root";
		string namesmallaod = "smallaod.root";
		fileaod = new TFile(filepathaod);
		TTree *ntupleaod = (TTree*) fileaod->Get("stage/ttree");

		faod = new TFile(namesmallaod.c_str(),"RECREATE");
		faod->cd();
		faod->mkdir("stage");
		faod->cd("stage");
		
		TTree *smallaod = ntupleaod->CopyTree(cuts);

		cout << " Copied tree with selection" << endl;
	
		//faod->cd();
		//ntupleaod->Write();
		//faod->Close();
		//faod->cd();
		smallaod->Write();	
		faod->Close();
		

	}
	
}


int main(){

	CopyTree c;
	c.CopyWithSelec("norm");

}


