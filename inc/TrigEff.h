#ifndef TRIGGEFF_H
#define TRIGGEFF_H

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

   //void	Fill(const vector<bool> &triggerpass, string obs ="", double weight = 1); 

   void	Fill(map<int,int> ListTriggers, string obs ="", double weight = 1); 

   void Compute();
     
   void SortEffVec();


   
   vector<string> selection; // Input by the user for specific triggers efficiencies 
   vector<string> selectedtriggernames; // Takes only triggers with prescale == 1;
   map<int,int> ListTriggers;


 private :
	
//*************************************METHODS********************************************

   void PrescaleSelection(vector<string> selection);

   // ****************** Correlation between triggers ******************
   void ComputeCorr(); //Computes the 2D array of correlations

   void PrintCorr(); // Shows the 2D array of correlations
   
   void PrintNumCorr(); // Shows the numerator (# candidates that passed either one of the other trigger)

   void PrintDenomCorr(); // Show the denominator (# candidates that passed both triggers)

   void Get2DPlot(); // 
   




   // ************** Efficiency of triggers ***********************

   void ComputeEff(); // Computes the efficiency for every given trigger 
   
   void PrintEff(); //Show all efficiencies 

   void PrintSpecEff(vector<int> curline); //Show the efficiencies of entered triggers 

   void PrintNumEff(); // Show the numerator (# candidates that passed the trigger)

   void PrintDenomEff(); // Show the denominator (# candidates)
	
   void ComputeError(); // computes the error on the efficiencies




   // ******************* Plotting ********************************

   void GetPlot(string selection); // format latex 

   void SavePlots(); // en png/hist ou whatever

   void SaveIntTrigs(); // Saves the list of interesting triggers (efficiency > threshold)



// ******************************MEMBERS**************************


   int error_type; // Int that corresponds to a way of calculating the error



   

   TFile* OutputHisto;
   TH1D* EFF_TRIG;
   TH1D* EFF_DISTRIB;

   TH2D* CORR; 

   vector<int> currentlines; // 
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
