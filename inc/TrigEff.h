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
   

   void	Load(vector<string> TriggerNames,vector<string> SelectedTriggerNames,int ErrorType=1, string Selection="entered");

   //void	Fill(const vector<bool> &triggerpass, string obs ="", double weight = 1); 

  // void	Fill(map<int,int> ListTriggers, string obs ="", double weight = 1); 

   void Fill(const vector<bool> &passtrig, string obs="", double weight = 1);

   void Compute();
     
   void SortEffVec();



 private :
	
//*************************************METHODS********************************************
   

   // ****************** Correlation between triggers ******************
   void ComputeCorr(); //Computes the 2D array of correlations

   void PrintCorr(); // Shows the 2D array of correlations
   
   void PrintNumCorr(); // Shows the numerator (# candidates that passed either one of the other trigger)

   void PrintDenomCorr(); // Show the denominator (# candidates that passed both triggers)

   void Get2DPlot(); // 
   




   // ************** Efficiency of triggers ***********************

   void ComputeEff(); // Computes the efficiency for every given trigger 
   
   void PrintEff(); //Show all efficiencies 

   void PrintNumEff(); // Show the numerator (# candidates that passed the trigger)

   void PrintDenomEff(); // Show the denominator (# candidates)
	
   void ComputeError(); // computes the error on the efficiencies




   // ******************* Plotting ********************************

   void GetPlot(string SelectedTriggerNames); // format latex 

   void SavePlots(); // en png/hist ou whatever

   void SaveIntTrigs(); // Saves the list of interesting triggers (efficiency > threshold)



// ******************************MEMBERS**************************




   TFile* OutputHisto;

   TH1D* EFF_TRIG;
   TH1D* EFF_DISTRIB;

   TH2D* CORR; 



   int ErrorType; // Int that corresponds to a way of calculating the error
   
   string Selection; // Takes only triggers with prescale == 1;
   
   string obs; // observable 
  
   
   map<int,int> ListTriggers; // map linking position of given trigger in the .txt file and an index


   vector<string> SelectedTriggerNames; // Input by the user for specific triggers efficiencies 

   vector< pair<double, pair<double,string> > > EffList; 

   vector<string> TriggerNames;

   vector<bool> TriggerPass; 
   

   vector<vector<double> > Correlation; 

   vector<vector<double> > NumCorr;
   vector<vector<double> > DenomCorr;

   vector<vector<double> > CorrErr;


   vector<double> Efficiency;

   vector<double> NumEfficiency;
   vector<double> DenomEfficiency;

   vector<double> EffErr;
};

#endif
