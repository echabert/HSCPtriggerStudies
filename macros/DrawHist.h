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

class DrawHist
{
 public :
   
   DrawHist();
   ~DrawHist();

   
   virtual void	    FitSignalBg();


 private :

	TFile *myFile; 
	TFile* OutputHisto;

	TString outputfilename;
	vector<int> ValBins;
};



#endif 
