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
#include <TGraph.h>

using namespace std;

class DrawHist
{
 public :
   
   DrawHist();
   ~DrawHist();

   
   virtual void	    FitSignalBg();


 private :

	TFile *myFile; 
	TFile *myFile2;
	TFile *myFileEff;


	TFile* OutputHisto;
	TFile* OutputHisto2;
	

	TString outputfilename;
	TString outputfilename2;

	

	vector<int> ValBins;
	vector <TEfficiency*> EffList2;
	vector <TEfficiency*> EffListvsObs;
	
	vector <TH1D*> Efficiencies;
	vector <TH2F*> hr;
	vector <TGraphErrors*> Efficiencies2;
	vector <TGraph*> Efficiencies3;
};



#endif 
