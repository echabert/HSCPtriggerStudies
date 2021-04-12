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
#include <TEfficiency.h>

/*
How to compile this file : if using root : .L AnaEff.cpp 







*/

using namespace std; // ici std::vector 



class TrigEff 
{
public:


   TrigEff(); 
   ~TrigEff();
   

   void LoadNoMap(const vector<string> &triggerNames,const vector<string> &SelectedTriggerNames,int ErrorType=1, string NameVar="a",string FileName="testnewdata.root");


   void	Load(const vector<string> &TriggerNames,const vector<string> &SelectedTriggerNames,int ErrorType=1, string Selection="entered",string NameVar="a",string FileName="testnewdata.root");



   void FillNoMap(const vector<bool> &passtrig, float Obs = 0.0, double weight = 1);

   void Fill(const vector<bool> &passtrig, float Obs= 0.0, double weight = 1); // 

   void Compute(string NameOutputFile);
   
   //void CreateHisto(string NameVar="random", const vector<string> &SelectedTriggerNames);

   void WritePlots(string NameVar="a"); // format latex TFile* OutputHisto

   void FillMass(double INVMASS);

   



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

   void SortEffVec();


   // ******************* Plotting ********************************

   void SavePlots(); // en png/hist ou whatever

   void SaveIntTrigs(string NameOutputFile); // Saves the list of interesting triggers (efficiency > threshold)

   void StudyTrigvsMass(double mass); 


//Mettre la masse en input, dans le anaEff.ccp : get la masse dans la double boucle for, et appeler cette fct avec la masse? 

// ******************************FITS**************************



   void FitSignal();

// ******************************MEMBERS**************************




   TFile* OutputHisto;

   TH1D* EFF_TRIG;
   TH1D* EFF_DISTRIB;
   TH1D* MASS;

   TH1D* FITSIG;
   TH1D* FITBG;
   TH1D* FITBG2;


   TH2D* CORR; 

   

   int ErrorType; // Int that corresponds to a way of calculating the error
   
   string Selection; // The name of the variable we want to study
   
   string NameObs;
   string FileName;

   string NameOutputFile;
   float Obs; // observable 
  
   
   map<int,int> ListTriggers; // map linking position of given trigger in the .txt file and an index


   vector<string> SelectedTriggerNames; // Input by the user for specific triggers efficiencies 

   vector< pair<double, pair<double,string> > > EffList; 

   vector<string> TriggerNames;


   vector <TEfficiency*> EffvsObs;
   vector <TEfficiency*> EffvsObsTwo;


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
