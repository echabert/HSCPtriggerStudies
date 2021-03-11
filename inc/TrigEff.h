#ifndef TRIGGEFF_H
#define TRIGGEFF_H

#include <vector>
#include <iostream>
#include <string>
#include <TH2.h>


using namespace std;



class TrigEff // 
{
public:


   TrigEff(int size = 0); // supprimer le int size
   ~TrigEff();
   

   void Init(int ntrigger,bool *passtrig); // Recupère le tableau de booleens et le met dans un vecteur


   void	Load(vector<string> triggernames,string selection,int error_type);

   void	Fill(vector<bool> triggerpass, double obs, double weight = 1); // fill avec les valeurs de booléens 

   
   // *********Correlations methods***********************
// private : 

   void noc_corr();

   void compute_corr(); //return nothing, just computes the 2D array of correlations

   double get_corr();

   void Get2DPlot(); // 
   
   // ************** Efficiency for a given trigger **************


   void noc_eff();

   void compute_eff(int a); // int a : string selection
   
   double get_eff(int a); //specific trigger efficiency au lieu de int a : string selection

   double get_all_eff(); //all trigger efficiencies


   // ******************* 

   void GetPlot(string selection); // 


   void SavePlots(); // en png/hist ou whatever


private:

   //mettre ici compute_eff, compute correlation
   int error_type;
   
   float * data;


   //TH2F : correlation , 
   //TH2* correlation = new TH2F("correlation matrix", "hist of trigger correlations", ntrigger, 0, ntrigger, ntrigger,0,ntrigger);
   
 //  TH1* efficiency = new TH1F("efficiency of triggers","hist of efficiencies", 100,0,1);

   string obs; // observable 
   string selection;


   vector<string> triggernames;


   vector<bool> triggerpass; //creer une paire std::pair<int,string> ou un vecteur de paires : vector<std::pair<int,string>>
   
   vector<vector<double> > correlation;

   vector<vector<double> > num_corr;
   vector<vector<double> > denom_corr;

   vector<vector<double> > f_err;

   //vector<int> numerator;
   //vector<int> denominator;

   vector<double> efficiency;
   vector<double> num_efficiency;
   vector<double> denom_efficiency;
};
  









#endif
