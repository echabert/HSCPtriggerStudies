#ifndef TRIGGEFF_H
#define TRIGGEFF_H

#include <vector>
#include <iostream>
#include <string>
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <TPad.h>

/*
How to compile this file : if using root : .L AnaEff.cpp 







*/

using namespace std;



class TrigEff 
{
public:


   TrigEff(); 
   ~TrigEff();
   

   void	Load(vector<string> triggernames,vector<string> selection,int error_type=1);

   void	Fill(vector<bool> triggerpass, double obs = 0, double weight = 1); // fill avec les valeurs de booléens  renvoie true et false si difference de taille 

   void Compute();
     
   void SortEffVec();


   vector<int> column;
   vector<string> selection;

 private :
	// *********Correlations methods***********************


   void ComputeCorr(); //return nothing, just computes the 2D array of correlations

   void PrintCorr();
   
   void PrintNumCorr();

   void PrintDenomCorr();

   void Get2DPlot(); // 
   
   // ************** Efficiency for a given trigger **************



   void ComputeEff(); // int a : string selection
   
   void PrintEff(); //Show all efficiencies 

   void PrintSpecEff(vector<int>curline); // 

   void PrintNumEff();

   void PrintDenomEff();
	
   void ComputeError(); // computes the error on the efficiencies

   double GetAllEff(); //all trigger efficiencies


   // *******************  PLOTTING ********************************

   void GetPlot(string selection); // format latex 

   void SavePlots(); // en png/hist ou whatever

   int error_type;

   TH1D* EFF_TRIG;

   //TH2F : correlation , 
   //TH2* correlation = new TH2F("correlation matrix", "hist of trigger correlations", ntrigger, 0, ntrigger, ntrigger,0,ntrigger);
   
 //  TH1* efficiency = new TH1F("efficiency of triggers","hist of efficiencies", 100,0,1);


   

   vector< pair<double, pair<double,string> > > efflist; 

   string obs; // observable 


   vector<string> triggernames;

   vector<bool> triggerpass; 
   

   vector<vector<double> > correlation; 

   vector<vector<double> > num_corr;
   vector<vector<double> > denom_corr;

   vector<vector<double> > corr_err;


   vector<double> efficiency;

   vector<double> num_efficiency;
   vector<double> denom_efficiency;

   vector<double> eff_err;
};

#endif
