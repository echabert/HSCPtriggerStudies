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


   void	Load(vector<string> triggernames,string selection="",int error_type=1);

   

   
   
   void	Fill(vector<bool> triggerpass, double obs = 0, double weight = 1); // fill avec les valeurs de booléens  renvoie true et false si difference de taille 

   // *********Correlations methods***********************
// private : 


  // void Search_trig(vector<string> triggerNames, string selection ="" ); // triggerNames

   void Compute_corr(); //return nothing, just computes the 2D array of correlations

   void Print_corr();
   
   void Print_num_corr();

   void Print_denom_corr();

   void Get2DPlot(); // 
   
   // ************** Efficiency for a given trigger **************



   void Compute_eff(); // int a : string selection
   
   void Print_eff(); //specific trigger efficiency au lieu de int a : string selection

   void Print_spec_eff(int curline);

   void Print_num_eff();

   void Print_denom_eff();
	
   void Fill_error();

   double get_all_eff(); //all trigger efficiencies


   // ******************* 

   void GetPlot(string selection); // format latex 


   void SavePlots(); // en png/hist ou whatever


   int column;
private:
  // int ntrig=665;
	
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

   vector<vector<double> > corr_err;

   //vector<int> numerator;
   //vector<int> denominator;

   vector<double> efficiency;
   vector<double> num_efficiency;
   vector<double> denom_efficiency;
   vector<double> eff_err;
};
  









#endif
