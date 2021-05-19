#ifndef LISTNAMETRIG_H
#define LISTNAMETRIG_H

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

using namespace std;

class ListNameTriggers
{
 public :


   TTree          *fChain;   //!pointer to the analyzed TTree or TChain
   Int_t           fCurrent; //!current Tree number in a TChain

   //----------------------------------------
   //----------------METHODS ----------------
   //----------------------------------------
   
   ListNameTriggers(TTree *tree=0); 
   ~ListNameTriggers();

 
  /* Int_t    GetEntry(Long64_t entry);
   Long64_t LoadTree(Long64_t entry);
   void Init(TTree *tree);
   void Loop();
   Bool_t Notify();
   void Show(Long64_t entry = -1);*/


   virtual Int_t    GetEntry(Long64_t entry);
   virtual Long64_t LoadTree(Long64_t entry);
   virtual void     Init(TTree *tree);
   virtual Bool_t   Notify();
   virtual void     Show(Long64_t entry = -1);
   
   void FindAllNames();
   int IsInList(string name);

   // Declaration of leaf types

   Int_t	ntrigger;
   Int_t	nhscp;

   Float_t	prescaleTrigger[1000];
   Bool_t	passTrigger[1000];
   Int_t	nmuons;
   Float_t      muon_phi[32];
   Float_t      muon_eta[32];
   Float_t      muon_pt[32];
   vector<string>* triggerName;

   
    // List of branches

   TBranch        *b_ntrigger; //!
   TBranch        *b_nhscp;  //!
   TBranch        *b_passTrigger; //!
   TBranch        *b_prescaleTrigger;
   TBranch	  *b_triggerName;
   TBranch        *b_nmuons;
   TBranch        *b_muon_eta;   //!
   TBranch        *b_muon_phi;
   TBranch        *b_muon_pt;

 private : 


   map<string, pair<bool,bool> > MapOfTriggerNames;

   vector< pair<string, pair<bool,bool> > > InfoTriggers;
   
};



ListNameTriggers::ListNameTriggers(TTree *tree) : fChain(0) //constructeur
{
	triggerName = 0;
// if parameter tree is not specified (or zero), connect the file
// used to generate this class and read the Tree.
	if (tree == 0) {
		TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject("/opt/sbg/cms/safe1/cms/rhaeberl/CMSSW_10_6_2/src/HSCPtriggerStudies/all_stau1599.root");
		if (!f || !f->IsOpen()) {
			f = new TFile("/opt/sbg/cms/safe1/cms/rhaeberl/CMSSW_10_6_2/src/HSCPtriggerStudies/all_stau1599.root");
		}
		TDirectory * dir = (TDirectory*)f->Get("/opt/sbg/cms/safe1/cms/rhaeberl/CMSSW_10_6_2/src/HSCPtriggerStudies/all_stau1599.root:/stage");
		dir->GetObject("ttree",tree);

	}
   
   Init(tree);

}


ListNameTriggers::~ListNameTriggers() //deconstructeur
{
   if (!fChain) return;
   delete fChain->GetCurrentFile();
   
   //delete[] passTrigger;
}

Int_t ListNameTriggers::GetEntry(Long64_t entry)
{
// Read contents of entry.
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}

Long64_t ListNameTriggers::LoadTree(Long64_t entry)
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

void ListNameTriggers::Init(TTree *tree)
{

   if (!tree) return;
   fChain = tree;
   fCurrent = -1;
   fChain->SetMakeClass(1);

   fChain->SetBranchAddress("nmuons", &nmuons, &b_nmuons);
   fChain->SetBranchAddress("ntrigger", &ntrigger, &b_ntrigger);
   fChain->SetBranchAddress("prescaleTrigger", prescaleTrigger, &b_prescaleTrigger);
   fChain->SetBranchAddress("nameTrigger", &triggerName, &b_triggerName);
   fChain->SetBranchAddress("passTrigger", passTrigger, &b_passTrigger); // & devant PT 1
   fChain->SetBranchAddress("nhscp", &nhscp, &b_nhscp);
   fChain->SetBranchAddress("muon_phi", muon_phi, &b_muon_phi);
   fChain->SetBranchAddress("muon_eta", muon_eta, &b_muon_eta);
   fChain->SetBranchAddress("muon_pt", muon_pt, &b_muon_pt);

 //  fChain->SetBranchAddress("hscp_muon_idx", hscp_muon_idx, &b_hscp_muon_idx); 
   Notify();
}

Bool_t ListNameTriggers::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

void ListNameTriggers::Show(Long64_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry
   if (!fChain) return;
   fChain->Show(entry);
}

#endif 
