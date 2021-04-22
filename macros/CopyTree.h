#ifndef COPYTREE_H
#define COPYTREE_H

#include <vector>
#include <iostream>
#include <string>
#include <TH2.h>
#include <TH1.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <TPad.h>
#include <TFile.h>
#include <map>
#include <TEfficiency.h>

using namespace std;

class CopyTree
{
 public :
   
   CopyTree();
   ~CopyTree();

   
   virtual void	    CopyWithSelec(string mode);

 private : 
	
	TFile* file;
	TFile* f2;

	TFile* file2;
	TFile* f22;

	TFile* file3;
	TFile* f23;

	TFile* file4;
	TFile* f24;

	TFile* file5;
	TFile* f25;

	TFile* file6;
	TFile* f26;

	TFile* file7;
	TFile* f27;

	TFile* file8;
	TFile* f28;

	TFile* file9;
	TFile* f29;

	TFile* file10;
	TFile* f210;

	TFile* file11;
	TFile* f211;

	TFile* file12;
	TFile* f212;

	TFile* fileaod;
	TFile* faod;

	vector<TFile*> files;
	vector<TFile*> fs;
	
	vector<TTree*> small;
	vector<TTree*> ntuple;

	vector<string> NameFiles;
	vector<string> namesmall;
	vector<string> pathfile;


	string namesmall[100]; //number of files in dylans given directory
};



#endif 
