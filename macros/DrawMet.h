#ifndef DRAWMET_H
#define DRAWMET_H

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

class DrawMet
{
 public :
   
   DrawMet();
   ~DrawMet();

   
   virtual void	    FitSignalMet();


 private :
   TFile *myFileMet; 
   TFile* OutputHistoMet;

   TFile* MyMyFMet;


   vector <TFile*> AllFilesMet;

   TH1D* TempTrMet;
   TH1D* TempTr2Met;
   TH1D* TempTr3Met;

   TH1D* EffMetPre;
   TH1D* EffMetSel;


   vector <TFile*> MyMyF2Met;
   vector <TFile*> MyMyF3Met;


   vector <TF1*> MyTf1Met;
   vector <TF1*> MyTf2Met;

   TGraphErrors* TestMet;
   TGraphErrors* Test2Met;
   TGraphErrors* Test3Met;


   TF1* TotalMet;
};



#endif 
