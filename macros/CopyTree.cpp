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







// /opt/sbg/cms/ui3_data1/dapparu/HSCP/Production/prodMay2021_CMSSW_10_6_2/HSCPgluinoonlyneutral_M-2600_TuneCP5_13TeV-pythia8/MC17_Gluino2600_onlyneutral/210518_143843/0000


void CopyTree::CopyWithSelec(string mode){

	//faire l'association, deux traces associées à ces muons et ensuite faire la requête sur la qualité
	TString cuts = "";
	//TString cuts = "nmuons>=2 && muon_pt[0] >= 20 && muon_pt[1] >= 20 && (track_charge[0]*track_charge[1] == -1) && track_qual[0] >= 3 && track_qual[1] >= 3 && muon_isMediumMuon[0]"; //(track_charge[0]*track_charge[1]) == -1 && muon_isMediumMuon[1] && track_qual[0] >= 2 && track_qual[1] >= 2
	//&& ndedxhits >= 5 && muon_isTrackerMuon[0] && muon_isTrackerMuon[1]
	// && muon_pt[0] >= 10 && muon_pt[1] >= 10 && (track_charge[0]*track_charge[1] == -1) && track_qual[0] >= 2 && track_qual[1] >= 2 && muon_isMediumMuon[0]"
	// /opt/sbg/cms/ui3_data1/dapparu/HSCP/Production/prodMay2021_CMSSW_10_6_2/HSCPppstau_M-494_TuneZ2star_13TeV-pythia6/MC17_Stau494/210510_161454/0000/
	string path("/opt/sbg/cms/ui3_data1/dapparu/HSCP/Production/prodMay2021_CMSSW_10_6_2/HSCPstop_M-2400_TuneCP5_13TeV-pythia8/MC17_Stop2400/210517_144941/0000/");
	string ext(".root");
	
	Long64_t sumentries=0,smallsumentries=0;
	if(mode=="norm"){
		DIR *dir;
		struct dirent *ent;
		if ((dir = opendir (path.c_str())) != NULL) {
  			while ((ent = readdir (dir)) != NULL) {
				NameFiles.push_back(ent->d_name);
  			}
	
  		closedir (dir);
		}

		else{
 			cout << "couldn't open directory" << endl;
		}
		
		NameFiles.erase( NameFiles.begin(), NameFiles.size() > 2 ?  NameFiles.begin() + 2 : NameFiles.end() );
		


		files.resize(NameFiles.size());
		fs.resize(NameFiles.size());
		small.resize(NameFiles.size());
		ntuple.resize(NameFiles.size());
		int nbsubf;
		string namsmall = "namesmall";
		cout << "How many files do you want to merge ? " <<endl;
		cin >> nbsubf ;
		int x;
		float f = ( NameFiles.size() / nbsubf );
		int sizeofsub = (int)f;

		if(NameFiles.size() < nbsubf){
			cout << "There are " << NameFiles.size() << " files, we will merge them all" << endl;


			for(int j = 0 ; j < NameFiles.size() ; j++){
				
				
				string s = to_string(j);
				string transfer =  namsmall + s + ext;
				namesmall.push_back(transfer);
				string transfer2 = path + NameFiles[j];
				pathfile.push_back(transfer2);
				files[j] = new TFile(pathfile[j].c_str());
				ntuple[j] = (TTree*) files[j]->Get("stage/ttree");
				Long64_t nentries = ntuple[j]->GetEntriesFast();
				sumentries+=nentries;

				fs[j] = new TFile(namesmall[j].c_str(),"RECREATE");
				fs[j]->cd();
				fs[j]->mkdir("stage");
				fs[j]->cd("stage");
				small[j] = ntuple[j]->CopyTree(cuts);

				Long64_t smallnentries = small[j]->GetEntriesFast();
				smallsumentries+=smallnentries;
				
				small[j]->Write();
				fs[j]->Close();

			
				cout << " Copied file " << NameFiles[j] << " in place " << j << " with name " << namesmall[j].c_str() << endl;


			}

		}

		else {
			cout << "There are " << f << " subgroups, and " <<  NameFiles.size() - (f*nbsubf) << " files remaining " << endl;

			cout << "Which subgroup of " <<  nbsubf << "  files do you want to study ?" << endl;
			cin >> x ;


			cout << "Copying from file " << NameFiles[(x-1)*nbsubf] << " to file " << NameFiles[(x*nbsubf)-1] << endl;
			if(x > f){

			}
			else{
				for(int j = (x-1)*nbsubf; j < x*nbsubf ; j++){
					string s = to_string(j);
					string transfer =  namsmall + s + ext;
					namesmall.push_back(transfer);
					string transfer2 = path + NameFiles[j];
					pathfile.push_back(transfer2);
					int filenumber = j%nbsubf;
					cout << filenumber << endl;
					files[j] = new TFile(pathfile[filenumber].c_str());
					ntuple[j] = (TTree*) files[j]->Get("stage/ttree");
					Long64_t nentries = ntuple[j]->GetEntriesFast();
					sumentries+=nentries;

					fs[j] = new TFile(namesmall[filenumber].c_str(),"RECREATE");
					fs[j]->cd();
					fs[j]->mkdir("stage");
					fs[j]->cd("stage");
					small[j] = ntuple[j]->CopyTree(cuts);

					Long64_t smallnentries = small[j]->GetEntriesFast();
					smallsumentries+=smallnentries;
				
					small[j]->Write();
					fs[j]->Close();

			
					cout << " Copied file " << NameFiles[j] << " in place " << filenumber << " with name " << namesmall[filenumber].c_str() << endl;
				}

			}



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
		smallaod->Write();	
		faod->Close();
		

	}
	
}


int main(){

	CopyTree c;
	c.CopyWithSelec("norm");

}


