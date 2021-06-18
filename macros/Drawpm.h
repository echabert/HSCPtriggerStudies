#ifndef COPYPM_H
#define COPYPM_H

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
#include <TGraphErrors.h>
using namespace std;

class Drawpm
{
 public :
   
   Drawpm();
   ~Drawpm();

   
   virtual void	    FitSignalPM();


 private :
   TFile *myFile; 
   TFile* OutputHisto;

   vector <TFile*> AllFiles;

   vector <TH1D*> TempTr;
   vector <TH1D*> TempTr2;
   vector <TH1D*> TempTr3;

   vector <TFile*> MyMyF;
   vector <TFile*> MyMyF2;
   vector <TFile*> MyMyF3;


   vector <TF1*> MyTf1;
   vector <TF1*> MyTf2;

   vector <TGraphErrors*> Test;
   vector <TGraphErrors*> Test2;
   vector <TGraphErrors*> Test3;
   TF1* Total;
};



#endif 
