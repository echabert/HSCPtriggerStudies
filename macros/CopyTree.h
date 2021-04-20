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


   //----------------------------------------
   //----------------METHODS ----------------
   //----------------------------------------
   
   CopyTree();
   ~CopyTree();

   
   virtual void	    CopyWithSelec();

   // Declaration of leaf types


 private : 
	
	TFile* file;
	TFile* f2;

	TFile* file2;
	TFile* f22;

	TFile* file3;
	TFile* f23;

	TFile* file4;
	TFile* f24;
};



#endif 
