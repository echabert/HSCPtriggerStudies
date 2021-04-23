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
	
	

	TFile* fileaod;
	TFile* faod;

	vector <TFile*> files;
	vector <TFile*> fs;
	
	vector <TTree*> small;
	vector <TTree*> ntuple;

	vector<string> NameFiles;
	vector<string> namesmall;
	vector<string> pathfile;


	
};



#endif 
