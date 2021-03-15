// found on file: /opt/sbg/cms/ui3_data1/dapparu/HSCP/Production/prodMarch2021_CMSSW_10_6_2/Glu1600_5ev/ntupleRaphael_MC16_AOD_Gluino1600_5ev.root
//////////////////////////////////////////////////////////

#ifndef EFFICIENCY_H
#define EFFICIENCY_H


#include <vector>
#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include <iostream>
#include <fstream>
//include our classes
#include "inc/TrigEff.h"









using namespace std;

//rajouter la classe pour dire "je remplis etc... , tu me donnes ce qui passe ou non et je fais avec tout ça
// pas de dependence entre ce qui calcule les efficacités et comment tu donnes les variables
class AnaEff
{
public :
  
   //--------------------------------------
   //Data members
   //--------------------------------------

   //User variables
   TrigEff   trigEff_presel;


   //List of variables with ROOT dependancies
   
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain
   Int_t           fCurrent; //!current Tree number in a TChain



  // Declaration of leaf types
   Int_t           runNumber;
   UInt_t          event;
   Int_t           npv;
   Int_t           ngoodpv;
   Int_t           ntrigger;

   //Bool_t *passTrigger;
   
   Bool_t	passTrigger[1000];
   //Bool_t          passTrigger[665];
   Float_t         track_pt[33];   //[ntracks] augmenter la taille pour pas de overflow, it was 33
   Int_t           hscp_track_idx[9];   //[nhscp] it was 9

    // List of branches
   TBranch        *b_runNumber;   //!
   TBranch        *b_event;   //!
   TBranch        *b_npv;   //!
   TBranch        *b_ngoodpv;   //!
   TBranch        *b_ntrigger; //!
   TBranch        *b_passTrigger; //!
   TBranch        *b_track_pt;   //!
   TBranch        *b_hscp_track_idx;   //!

   //--------------------------------------
   // Methods
   //--------------------------------------

   AnaEff(TTree *tree=0);
   virtual ~AnaEff();
   virtual Int_t    GetEntry(Long64_t entry);
   virtual Long64_t LoadTree(Long64_t entry);
   virtual void     Init(TTree *tree);
   virtual void     Loop();
   virtual Bool_t   Notify();
   virtual void     Show(Long64_t entry = -1);


};


#endif

#ifdef efficiency_cxx

AnaEff::AnaEff(TTree *tree) : fChain(0) //constructeur
{
// if parameter tree is not specified (or zero), connect the file
// used to generate this class and read the Tree.
   if (tree == 0) {
      TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject("/home/raph/CMS/TEST/ntupleRaphael_MC16_AOD_Gluino1600_5ev.root");
      if (!f || !f->IsOpen()) {
	f = new TFile("/home/raph/CMS/TEST/ntupleRaphael_MC16_AOD_Gluino1600_5ev.root");
      }
      TDirectory * dir = (TDirectory*)f->Get("/home/raph/CMS/TEST/ntupleRaphael_MC16_AOD_Gluino1600_5ev.root:/stage");
      dir->GetObject("ttree",tree);

   }
   //passTrigger = new bool[ntrigger];
   
   Init(tree);


   //read trigger list from the file


   ifstream ifile("data/triggerNames.txt");
   vector<string> triggerNames;
   string tmp;
   while(getline(ifile,tmp)){
	   //cout<<tmp<<endl;
   	triggerNames.push_back(tmp);
   }
   cout<<"#triggers: "<< triggerNames.size() <<endl;
   vector<string> str;
   string interfstr;
   int studytrig;
   cout << "How many triggers do you want to study? "  << endl;
   cin >> studytrig;
   cout <<"Name the triggers : " << endl;
   for (int i = 0; i< studytrig;i++){
   	cin >> interfstr;
	str.push_back(interfstr);
   }
   trigEff_presel.selection=str;
   trigEff_presel.Load(triggerNames,str);
}


AnaEff::~AnaEff() //deconstructeur
{
   if (!fChain) return;
   delete fChain->GetCurrentFile();
   //delete[] passTrigger;
}


Int_t AnaEff::GetEntry(Long64_t entry)
{
// Read contents of entry.
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}

Long64_t AnaEff::LoadTree(Long64_t entry)
{
// Set the environment to read one entry
   if (!fChain) return -5;
   Long64_t centry = fChain->LoadTree(entry);
   if (centry < 0) return centry;
   if (fChain->GetTreeNumber() != fCurrent) {
      fCurrent = fChain->GetTreeNumber();
      Notify();
   }
   return centry;
}

void AnaEff::Init(TTree *tree)
{

   if (!tree) return;
   fChain = tree;
   fCurrent = -1;
   fChain->SetMakeClass(1);


   fChain->SetBranchAddress("ntrigger", &ntrigger, &b_ntrigger);
   fChain->SetBranchAddress("passTrigger", passTrigger, &b_passTrigger); // & devant PT 1
   fChain->SetBranchAddress("runNumber", &runNumber, &b_runNumber);
   fChain->SetBranchAddress("event", &event, &b_event);
   fChain->SetBranchAddress("npv", &npv, &b_npv);
   fChain->SetBranchAddress("ngoodpv", &ngoodpv, &b_ngoodpv);
   fChain->SetBranchAddress("track_pt", track_pt, &b_track_pt);
   fChain->SetBranchAddress("hscp_track_idx", hscp_track_idx, &b_hscp_track_idx);
   Notify();
}

Bool_t AnaEff::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

void AnaEff::Show(Long64_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry
   if (!fChain) return;
   fChain->Show(entry);
}

#endif
