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
	
	/*for(int i=0; i < ListTriggers.size() ; i++){
		EffvsObs[i]=0;
	}*/
}

CopyTree::~CopyTree(){ 

	files.clear();
	fs.clear();
	NameFiles.clear();
	pathfile.clear();
	namesmall.clear();
}












void CopyTree::CopyWithSelec(string mode){

	//faire l'association, deux traces associées à ces muons et ensuite faire la requête sur la qualité 
	TString cuts = "nmuons>=2 && muon_pt[0] >= 20 && muon_pt[1] >= 20 && (track_charge[0]*track_charge[1] == -1) && track_qual[0] >= 3 && track_qual[1] >= 3 && muon_isMediumMuon[0]"; //(track_charge[0]*track_charge[1]) == -1 && muon_isMediumMuon[1] && track_qual[0] >= 2 && track_qual[1] >= 2
	//&& ndedxhits >= 5 && muon_isTrackerMuon[0] && muon_isTrackerMuon[1]
	// && muon_pt[0] >= 10 && muon_pt[1] >= 10 && (track_charge[0]*track_charge[1] == -1) && track_qual[0] >= 2 && track_qual[1] >= 2 && muon_isMediumMuon[0]"

	string path("/opt/sbg/cms/ui3_data1/dapparu/HSCP/Production/prodApril2021_CMSSW_10_6_2/MET/29Apr/");
	string ext(".root");
	
	Long64_t sumentries=0,smallsumentries=0;
	if(mode=="norm"){
		DIR *dir;
		struct dirent *ent;
		if ((dir = opendir ("/opt/sbg/cms/ui3_data1/dapparu/HSCP/Production/prodApril2021_CMSSW_10_6_2/MET/29Apr/")) != NULL) {
  			while ((ent = readdir (dir)) != NULL) {
				NameFiles.push_back(ent->d_name);
  			}
		cout << NameFiles.size() << endl;
  		closedir (dir);
		} 
		else {
 		cout << "couldn't open directory" << endl;
		}

		files.resize(NameFiles.size()-2);
		fs.resize(NameFiles.size()-2);
		small.resize(NameFiles.size()-2);
		ntuple.resize(NameFiles.size()-2);
		for(int i = 2; i < NameFiles.size() ; i++){
			
			string namsmall = "namesmall";
			int intransf = i-2;
			string s = to_string(intransf);
			string transfer = namsmall + s + ext;
			namesmall.push_back(transfer);
			string transfer2 = path + NameFiles[i];
			pathfile.push_back(transfer2);
			files[intransf] = new TFile(pathfile[intransf].c_str());
			ntuple[intransf] = (TTree*) files[intransf]->Get("stage/ttree");

			Long64_t nentries = ntuple[intransf]->GetEntriesFast();
			sumentries+=nentries;

			fs[intransf] = new TFile(namesmall[intransf].c_str(),"RECREATE");
			fs[intransf]->cd();
			fs[intransf]->mkdir("stage");
			fs[intransf]->cd("stage");
			small[intransf] = ntuple[intransf]->CopyTree(cuts);

			Long64_t smallnentries = small[intransf]->GetEntriesFast();
			smallsumentries+=smallnentries;
				
			small[intransf]->Write();
			fs[intransf]->Close();

			cout << " Copied file " << intransf << " with name " << namesmall[intransf].c_str() << endl;
		}
	
		cout <<"There was initially " << sumentries << " entries, reduced to " << smallsumentries << " , we took only "<< (smallsumentries*1.0/sumentries) * 100 << " %" << endl;

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
	c.CopyWithSelec("norm");

}


