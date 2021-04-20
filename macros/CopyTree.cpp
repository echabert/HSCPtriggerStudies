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




void CopyTree::CopyWithSelec(){
	string namesmall = "small1064.root";

	TFile *file = new TFile("/opt/sbg/cms/ui3_data1/dapparu/HSCP/Production/prodApril2021_CMSSW_10_6_2/MET/0001/nt_data_aod_1-1064.root");
	TTree *ntuple = (TTree*) file->Get("stage/ttree");

	TFile *f2 = new TFile(namesmall.c_str(),"recreate");

	TTree *small = ntuple->CopyTree("muon_pt[0]","nmuons>2 && muon_pt[0] > 10 && muon_pt[1] > 10");

}


int main(){

	CopyTree c;
	c.CopyWithSelec();

}


