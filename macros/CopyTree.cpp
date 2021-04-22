#define BOOST_FILESYSTEM_VERSION 3
#define BOOST_FILESYSTEM_NO_DEPRECATED
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
#include <filesystem>
#include <dirent.h>
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
	file5 = 0;
	f25 = 0;
	file6 = 0;
	f26 = 0;
	file7 = 0;
	f27 = 0;
	file8 = 0;
	f28 = 0;
	file9 = 0;
	f29 = 0;
	file10 = 0;
	f210 = 0;
	file11 = 0;
	f211 = 0;
	file12 = 0;
	f212 = 0;
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
	if(!file5){
		delete file5;
	}
	if(!f25){
		delete f25;
	}
	if(!file6){
		delete file6;
	}
	if(!f26){
		delete f26;
	}
	if(!file7){
		delete file7;
	}
	if(!f27){
		delete f27;
	}
	if(!file8){
		delete file8;
	}
	if(!f28){
		delete f28;
	}
	if(!file9){
		delete file9;
	}
	if(!f29){
		delete f29;
	}
	if(!file10){
		delete file10;
	}
	if(!f210){
		delete f210;
	}
	if(!file11){
		delete file11;
	}
	if(!f211){
		delete f211;
	}
	if(!file12){
		delete file12;
	}
	if(!f212){
		delete f212;
	}
}












void CopyTree::CopyWithSelec(string mode){

	//faire l'association, deux traces associées à ces muons et ensuite faire la requête sur la qualité 
	TString cuts = "nmuons>=2 && muon_pt[0] >= 10 && muon_pt[1] >= 10 && (track_charge[0]*track_charge[1] == -1) && track_qual[0] >= 2 && track_qual[1] >= 2 && muon_isMediumMuon[0]"; //(track_charge[0]*track_charge[1]) == -1 && muon_isMediumMuon[1] && track_qual[0] >= 2 && track_qual[1] >= 2
	//&& ndedxhits >= 5 && muon_isTrackerMuon[0] && muon_isTrackerMuon[1]
	
	//Verifier les coupures ici, comparées à celles d'avant 
	//Si les grandeurs sont décorélées, on s'en fout 
	// P-e la combinaison des coupures ->  
	// Contraintes supplémentaires qui arrivent ici 
	std::string path("/opt/sbg/cms/ui3_data1/dapparu/HSCP/Production/prodApril2021_CMSSW_10_6_2/MET/0001/");
	std::string ext(".root");
	
	DIR *dir;
	struct dirent *ent;
	if ((dir = opendir ("/opt/sbg/cms/ui3_data1/dapparu/HSCP/Production/prodApril2021_CMSSW_10_6_2/MET/0001/")) != NULL) {
  		/* print all the files and directories within directory */
  		while ((ent = readdir (dir)) != NULL) {
			if(ent->d_name != "." && ent->d_name != ".."){
				NameFiles.push_back(ent->d_name);
			}
  		}
  	closedir (dir);
	} else {
  	/* could not open directory */
 	cout << "couldn't open directory" << endl;
	}

	for(int i = 0; i < NameFiles.size() ; i++){
		cout << NameFiles[i] << endl;
		string namsmall = "namesmall";
		string s = to_string(i);
		namesmall[i] = namsmall + s;
		
		pathfile[i] = path + NameFiles[i];

		files[i] = new TFile(pathfile[i]);
		ntuple[i] = (TTree*) files[i]->Get("stage/ttree");
		fs[i] = new TFile(namesmall[i].c_str(),"RECREATE");
		fs[i]->cd();
		fs[i]->mkdir("stage");
		fs[i]->cd("stage");
		small[i] = ntuple[i]->CopyTree(cuts);
		small[i]->Write();
		fs[i]->Close();

		cout << " Copied file " << i << endl;
	}
	
	if(mode == "first"){

		cout << " Working on files [64-48]" << endl;
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
		f210 = new TFile(namesmall10.c_str(),"RECREATE");
		f210->cd();
		f210->mkdir("stage");
		f210->cd("stage");
		TTree *small10 = ntuple10->CopyTree(cuts);
		small10->Write();
		f210->Close();

		cout << " Copied file 10" << endl;

		file11 = new TFile(pathfile11);
		TTree *ntuple11 = (TTree*) file11->Get("stage/ttree");
		f211 = new TFile(namesmall11.c_str(),"RECREATE");
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
	else if(mode == "second"){

		cout << " Working on files [38-20]" << endl;

		string namesmall = "small1038.root";
		string namesmall2 = "small1037.root";
		string namesmall3 = "small1034.root";
		string namesmall4 = "small1033.root";
		string namesmall5 = "small1032.root";
		string namesmall6 = "small1031.root";
		string namesmall7 = "small1030.root";
		string namesmall8 = "small1028.root";
		string namesmall9 = "small1025.root";
		string namesmall10 = "small1024.root";
		string namesmall11 = "small1021.root";
		string namesmall12 = "small1020.root";
		

		TString pathfile = "/opt/sbg/cms/ui3_data1/dapparu/HSCP/Production/prodApril2021_CMSSW_10_6_2/MET/0001/nt_data_aod_1-1038.root";
		TString pathfile2 = "/opt/sbg/cms/ui3_data1/dapparu/HSCP/Production/prodApril2021_CMSSW_10_6_2/MET/0001/nt_data_aod_1-1037.root";
		TString pathfile3 = "/opt/sbg/cms/ui3_data1/dapparu/HSCP/Production/prodApril2021_CMSSW_10_6_2/MET/0001/nt_data_aod_1-1034.root";
		TString pathfile4 = "/opt/sbg/cms/ui3_data1/dapparu/HSCP/Production/prodApril2021_CMSSW_10_6_2/MET/0001/nt_data_aod_1-1033.root";
		TString pathfile5 = "/opt/sbg/cms/ui3_data1/dapparu/HSCP/Production/prodApril2021_CMSSW_10_6_2/MET/0001/nt_data_aod_1-1032.root";
		TString pathfile6 = "/opt/sbg/cms/ui3_data1/dapparu/HSCP/Production/prodApril2021_CMSSW_10_6_2/MET/0001/nt_data_aod_1-1031.root";
		TString pathfile7 = "/opt/sbg/cms/ui3_data1/dapparu/HSCP/Production/prodApril2021_CMSSW_10_6_2/MET/0001/nt_data_aod_1-1030.root";
		TString pathfile8 = "/opt/sbg/cms/ui3_data1/dapparu/HSCP/Production/prodApril2021_CMSSW_10_6_2/MET/0001/nt_data_aod_1-1028.root";
		TString pathfile9 = "/opt/sbg/cms/ui3_data1/dapparu/HSCP/Production/prodApril2021_CMSSW_10_6_2/MET/0001/nt_data_aod_1-1025.root";
		TString pathfile10 = "/opt/sbg/cms/ui3_data1/dapparu/HSCP/Production/prodApril2021_CMSSW_10_6_2/MET/0001/nt_data_aod_1-1024.root";
		TString pathfile11 = "/opt/sbg/cms/ui3_data1/dapparu/HSCP/Production/prodApril2021_CMSSW_10_6_2/MET/0001/nt_data_aod_1-1021.root";
		TString pathfile12 = "/opt/sbg/cms/ui3_data1/dapparu/HSCP/Production/prodApril2021_CMSSW_10_6_2/MET/0001/nt_data_aod_1-1020.root";
	
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
		f210 = new TFile(namesmall10.c_str(),"RECREATE");
		f210->cd();
		f210->mkdir("stage");
		f210->cd("stage");
		TTree *small10 = ntuple10->CopyTree(cuts);
		small10->Write();
		f210->Close();

		cout << " Copied file 10" << endl;

		file11 = new TFile(pathfile11);
		TTree *ntuple11 = (TTree*) file11->Get("stage/ttree");
		f211 = new TFile(namesmall11.c_str(),"RECREATE");
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
	else if(mode == "third"){

		cout << " Working on files [19-00]" << endl;

		string namesmall = "small1019.root";
		string namesmall2 = "small1016.root";
		string namesmall3 = "small1015.root";
		string namesmall4 = "small1014.root";
		string namesmall5 = "small1013.root";
		string namesmall6 = "small1008.root";
		string namesmall7 = "small1006.root";
		string namesmall8 = "small1005.root";
		string namesmall9 = "small1003.root";
		string namesmall10 = "small1002.root";
		string namesmall11 = "small1000.root";
		
		TString pathfile = "/opt/sbg/cms/ui3_data1/dapparu/HSCP/Production/prodApril2021_CMSSW_10_6_2/MET/0001/nt_data_aod_1-1019.root";
		TString pathfile2 = "/opt/sbg/cms/ui3_data1/dapparu/HSCP/Production/prodApril2021_CMSSW_10_6_2/MET/0001/nt_data_aod_1-1016.root";
		TString pathfile3 = "/opt/sbg/cms/ui3_data1/dapparu/HSCP/Production/prodApril2021_CMSSW_10_6_2/MET/0001/nt_data_aod_1-1015.root";
		TString pathfile4 = "/opt/sbg/cms/ui3_data1/dapparu/HSCP/Production/prodApril2021_CMSSW_10_6_2/MET/0001/nt_data_aod_1-1014.root";
		TString pathfile5 = "/opt/sbg/cms/ui3_data1/dapparu/HSCP/Production/prodApril2021_CMSSW_10_6_2/MET/0001/nt_data_aod_1-1013.root";
		TString pathfile6 = "/opt/sbg/cms/ui3_data1/dapparu/HSCP/Production/prodApril2021_CMSSW_10_6_2/MET/0001/nt_data_aod_1-1008.root";
		TString pathfile7 = "/opt/sbg/cms/ui3_data1/dapparu/HSCP/Production/prodApril2021_CMSSW_10_6_2/MET/0001/nt_data_aod_1-1006.root";
		TString pathfile8 = "/opt/sbg/cms/ui3_data1/dapparu/HSCP/Production/prodApril2021_CMSSW_10_6_2/MET/0001/nt_data_aod_1-1005.root";
		TString pathfile9 = "/opt/sbg/cms/ui3_data1/dapparu/HSCP/Production/prodApril2021_CMSSW_10_6_2/MET/0001/nt_data_aod_1-1003.root";
		TString pathfile10 = "/opt/sbg/cms/ui3_data1/dapparu/HSCP/Production/prodApril2021_CMSSW_10_6_2/MET/0001/nt_data_aod_1-1002.root";
		TString pathfile11 = "/opt/sbg/cms/ui3_data1/dapparu/HSCP/Production/prodApril2021_CMSSW_10_6_2/MET/0001/nt_data_aod_1-1000.root";
		
	
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
		f210 = new TFile(namesmall10.c_str(),"RECREATE");
		f210->cd();
		f210->mkdir("stage");
		f210->cd("stage");
		TTree *small10 = ntuple10->CopyTree(cuts);
		small10->Write();
		f210->Close();

		cout << " Copied file 10" << endl;

		file11 = new TFile(pathfile11);
		TTree *ntuple11 = (TTree*) file11->Get("stage/ttree");
		f211 = new TFile(namesmall11.c_str(),"RECREATE");
		f211->cd();
		f211->mkdir("stage");
		f211->cd("stage");
		TTree *small11 = ntuple11->CopyTree(cuts);
		small11->Write();
		f211->Close();

		cout << " Copied file 11" << endl;

	}
	else if(mode == "aod"){
		cout << " Copying on singlemuon data" << endl;
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
	c.CopyWithSelec("dadk");

}


