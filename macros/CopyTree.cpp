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


	TString cuts = "nmuons>2 && muon_pt[0] > 10 && muon_pt[1] > 10 && muon_eta[0] < 2.1 && muon_eta[0] > -2.1  && muon_isTrackerMuon[0] && muon_isTrackerMuon[1]";
	//&& ndedxhits >= 5
	
	//Verifier les coupures ici, comparées à celles d'avant 
	//Si les grandeurs sont décorélées, on s'en fout 
	// P-e la combinaison des coupures ->  
	// Contraintes supplémentaires qui arrivent ici 

	if(mode == "norm"){
		string namesmall = "small1064.root";
		string namesmall2 = "small1062.root";
		string namesmall3 = "small1059.root";
		string namesmall4 = "small1058.root";
		string namesmall5 = "small1057.root";
		string namesmall6 = "small1056.root";
		string namesmall7 = "small1055.root";
		string namesmall8 = "small1054.root";
		string namesmall9 = "small1053.root";
		string namesmall10 = "small1051.root";
		string namesmall11 = "small1050.root";
		string namesmall12 = "small1048.root";
		

		TString pathfile = "/opt/sbg/cms/ui3_data1/dapparu/HSCP/Production/prodApril2021_CMSSW_10_6_2/MET/0001/nt_data_aod_1-1064.root";
		TString pathfile2 = "/opt/sbg/cms/ui3_data1/dapparu/HSCP/Production/prodApril2021_CMSSW_10_6_2/MET/0001/nt_data_aod_1-1062.root";
		TString pathfile3 = "/opt/sbg/cms/ui3_data1/dapparu/HSCP/Production/prodApril2021_CMSSW_10_6_2/MET/0001/nt_data_aod_1-1059.root";
		TString pathfile4 = "/opt/sbg/cms/ui3_data1/dapparu/HSCP/Production/prodApril2021_CMSSW_10_6_2/MET/0001/nt_data_aod_1-1058.root";
		TString pathfile5 = "/opt/sbg/cms/ui3_data1/dapparu/HSCP/Production/prodApril2021_CMSSW_10_6_2/MET/0001/nt_data_aod_1-1057.root";
		TString pathfile6 = "/opt/sbg/cms/ui3_data1/dapparu/HSCP/Production/prodApril2021_CMSSW_10_6_2/MET/0001/nt_data_aod_1-1056.root";
		TString pathfile7 = "/opt/sbg/cms/ui3_data1/dapparu/HSCP/Production/prodApril2021_CMSSW_10_6_2/MET/0001/nt_data_aod_1-1055.root";
		TString pathfile8 = "/opt/sbg/cms/ui3_data1/dapparu/HSCP/Production/prodApril2021_CMSSW_10_6_2/MET/0001/nt_data_aod_1-1054.root";
		TString pathfile9 = "/opt/sbg/cms/ui3_data1/dapparu/HSCP/Production/prodApril2021_CMSSW_10_6_2/MET/0001/nt_data_aod_1-1053.root";
		TString pathfile10 = "/opt/sbg/cms/ui3_data1/dapparu/HSCP/Production/prodApril2021_CMSSW_10_6_2/MET/0001/nt_data_aod_1-1051.root";
		TString pathfile11 = "/opt/sbg/cms/ui3_data1/dapparu/HSCP/Production/prodApril2021_CMSSW_10_6_2/MET/0001/nt_data_aod_1-1050.root";
		TString pathfile12 = "/opt/sbg/cms/ui3_data1/dapparu/HSCP/Production/prodApril2021_CMSSW_10_6_2/MET/0001/nt_data_aod_1-1048.root";


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
	
		file5 = new TFile(pathfile5);
		TTree *ntuple5 = (TTree*) file5->Get("stage/ttree");
		f25 = new TFile(namesmall5.c_str(),"RECREATE");
		f25->cd();
		f25->mkdir("stage");
		f25->cd("stage");
		TTree *small5 = ntuple5->CopyTree(cuts);
		small5->Write();
		f25->Close();

		cout << " Copied file 5" << endl;

		file6 = new TFile(pathfile6);
		TTree *ntuple6 = (TTree*) file6->Get("stage/ttree");
		f26 = new TFile(namesmall6.c_str(),"RECREATE");
		f26->cd();
		f26->mkdir("stage");
		f26->cd("stage");
		TTree *small6 = ntuple6->CopyTree(cuts);
		small6->Write();
		f26->Close();

		cout << " Copied file 6" << endl;

		file7 = new TFile(pathfile7);
		TTree *ntuple7 = (TTree*) file7->Get("stage/ttree");
		f27 = new TFile(namesmall7.c_str(),"RECREATE");
		f27->cd();
		f27->mkdir("stage");
		f27->cd("stage");
		TTree *small7 = ntuple7->CopyTree(cuts);
		small7->Write();
		f27->Close();

		cout << " Copied file 7" << endl;

		file8 = new TFile(pathfile8);
		TTree *ntuple8 = (TTree*) file8->Get("stage/ttree");
		f28 = new TFile(namesmall8.c_str(),"RECREATE");
		f28->cd();
		f28->mkdir("stage");
		f28->cd("stage");
		TTree *small8 = ntuple8->CopyTree(cuts);
		small8->Write();
		f28->Close();

		cout << " Copied file 8" << endl;

		file9 = new TFile(pathfile9);
		TTree *ntuple9 = (TTree*) file9->Get("stage/ttree");
		f29 = new TFile(namesmall9.c_str(),"RECREATE");
		f29->cd();
		f29->mkdir("stage");
		f29->cd("stage");
		TTree *small9 = ntuple9->CopyTree(cuts);
		small9->Write();
		f29->Close();

		cout << " Copied file 9" << endl;

		file10 = new TFile(pathfile10);
		TTree *ntuple10 = (TTree*) file10->Get("stage/ttree");
		f210 = new TFile(namesmall8.c_str(),"RECREATE");
		f210->cd();
		f210->mkdir("stage");
		f210->cd("stage");
		TTree *small10 = ntuple10->CopyTree(cuts);
		small10->Write();
		f210->Close();

		cout << " Copied file 10" << endl;

		file11 = new TFile(pathfile11);
		TTree *ntuple11 = (TTree*) file11->Get("stage/ttree");
		f211 = new TFile(namesmall8.c_str(),"RECREATE");
		f211->cd();
		f211->mkdir("stage");
		f211->cd("stage");
		TTree *small11 = ntuple11->CopyTree(cuts);
		small11->Write();
		f211->Close();

		cout << " Copied file 11" << endl;

		file12 = new TFile(pathfile12);
		TTree *ntuple12 = (TTree*) file12->Get("stage/ttree");
		f212 = new TFile(namesmall12.c_str(),"RECREATE");
		f212->cd();
		f212->mkdir("stage");
		f212->cd("stage");
		TTree *small12 = ntuple12->CopyTree(cuts);
		small12->Write();
		f212->Close();

		cout << " Copied file 12" << endl;
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


