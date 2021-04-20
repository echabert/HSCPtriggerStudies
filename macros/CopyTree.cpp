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

void CopyTree::CopyWithSelec(){
	string namesmall = "small1064.root";
	string namesmall2 = "small1062.root";
	string namesmall3 = "small1059.root";
	string namesmall4 = "small1058.root";
	string namesmall5 = "small1057.root";



	file = new TFile("/opt/sbg/cms/ui3_data1/dapparu/HSCP/Production/prodApril2021_CMSSW_10_6_2/MET/0001/nt_data_aod_1-1064.root");
	TTree *ntuple = (TTree*) file->Get("stage/ttree");

	f2 = new TFile(namesmall.c_str(),"recreate");

	TTree *small = ntuple->CopyTree("muon_pt[0]","nmuons>2 && muon_pt[0] > 10 && muon_pt[1] > 10 && ");


	file2 = new TFile("/opt/sbg/cms/ui3_data1/dapparu/HSCP/Production/prodApril2021_CMSSW_10_6_2/MET/0001/nt_data_aod_1-1062.root");
	TTree *ntuple2 = (TTree*) file2->Get("stage/ttree");

	f22 = new TFile(namesmall2.c_str(),"recreate");

	TTree *small2 = ntuple2->CopyTree("muon_pt[0]","nmuons>2 && muon_pt[0] > 10 && muon_pt[1] > 10 && ");



	file3 = new TFile("/opt/sbg/cms/ui3_data1/dapparu/HSCP/Production/prodApril2021_CMSSW_10_6_2/MET/0001/nt_data_aod_1-1059.root");
	TTree *ntuple3 = (TTree*) file3->Get("stage/ttree");

	f23 = new TFile(namesmall3.c_str(),"recreate");

	TTree *small3 = ntuple3->CopyTree("muon_pt[0]","nmuons>2 && muon_pt[0] > 10 && muon_pt[1] > 10 && ");




	file4 = new TFile("/opt/sbg/cms/ui3_data1/dapparu/HSCP/Production/prodApril2021_CMSSW_10_6_2/MET/0001/nt_data_aod_1-1058.root");
	TTree *ntuple4 = (TTree*) file4->Get("stage/ttree");

	f24 = new TFile(namesmall4.c_str(),"recreate");

	TTree *small4 = ntuple4->CopyTree("muon_pt[0]","nmuons>2 && muon_pt[0] > 10 && muon_pt[1] > 10 && ");


	
}


int main(){

	CopyTree c;
	c.CopyWithSelec();

}


