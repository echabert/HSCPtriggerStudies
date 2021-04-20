//  /opt/sbg/cms/ui3_data1/dapparu/HSCP/Production/prodMarch2021_CMSSW_10_6_2/Glu1600/nt_mc_aod_1.root
//////////////////////////////////////////////////////////

#ifndef EFFICIENCY_H
#define EFFICIENCY_H


#include <vector>
#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include <iostream>
#include <fstream>
#include <map>
#include <iterator>
#include <TTree.h>
//include our classes
#include "inc/TrigEff.h"










using namespace std;


class AnaEff
{
public :
  
   //--------------------------------------
   //Data members
   //--------------------------------------

   //User variables
   TrigEff   trigEff_presel;

   TrigEff   trigEff_selection_obs;


	//_varname

   //List of variables with ROOT dependencies
   
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain
   Int_t           fCurrent; //!current Tree number in a TChain


   TString	FilePath="/home/raph/CMS/HSCPtriggerStudies/smallaod.root";
  // Declaration of leaf types
   Int_t	runNumber;
   UInt_t	event;
   Int_t	npv;
   Int_t	ngoodpv;
   Int_t	ntrigger;
   Int_t	nhscp;
   Int_t	ndedxhits;

   Float_t	pfmet_pt[32]; //test

   Float_t	prescaleTrigger[1000];
   Bool_t	passTrigger[1000];
  // vector<string>*	triggerName;
   
   Float_t	track_pt[33]; //[ntracks] augmenter la taille pour pas de overflow, it was 33
   Float_t      track_p[33];   
   Float_t	track_pterr[33];
   Int_t	hscp_track_idx[9];  //[nhscp] it was 9
 

   Float_t	track_eta[33];

   Int_t	track_npixhits[33];
   Int_t	track_nvalidhits[33];
   Float_t	track_validfraction[33];
  
   Int_t	track_nhits[33];
   Float_t	track_dz[33];
   Float_t	track_dxy[33];
   Int_t	track_qual[33];
   Float_t	hscp_iso2_tk[9];
   Bool_t	muon_isTrackerMuon[32];
   Float_t      muon_comb_inversebeta[32];
   Float_t      track_ih_ampl[33];
   Float_t      muon_phi[32];
   Float_t      muon_eta[32];
   Float_t      muon_pt[32];
   Int_t	nmuons;



    // List of branches
   TBranch        *b_runNumber;   //!
   TBranch        *b_event;   //!
   TBranch        *b_npv;   //!
   TBranch        *b_ngoodpv;   //!
   TBranch        *b_ntrigger; //!
   TBranch        *b_prescaleTrigger;
   //TBranch	  *b_triggerName;
   TBranch        *b_passTrigger; //!
   TBranch        *b_track_pt;   //!
   TBranch        *b_track_pterr; //!
   TBranch        *b_hscp_track_idx;   //!
   TBranch        *b_nhscp;  //!

   TBranch        *b_track_nhits;
   TBranch        *b_pfmet_pt; // !
   TBranch        *b_track_eta; //!
   TBranch        *b_track_npixhits; //!
   TBranch        *b_track_nvalidhits;
   TBranch        *b_track_validfraction;
   TBranch        *b_ndedxhits;
   TBranch        *b_track_dz;
   TBranch        *b_track_dxy;
   TBranch        *b_track_qual;
   TBranch        *b_hscp_iso2_tk;
   TBranch        *b_muon_isTrackerMuon;
   TBranch        *b_muon_comb_inversebeta;
   TBranch        *b_track_p;
   TBranch        *b_track_ih_ampl;
   TBranch        *b_muon_eta;   //!
   TBranch        *b_muon_phi;
   TBranch        *b_muon_pt;
   TBranch        *b_nmuons;

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
   virtual int      Selection();
   virtual double   MuonsInvariantMass();
   virtual double   IsolateMuons(const vector<bool> &passtrig);
   virtual int      fact(int n);
};


#endif
///opt/sbg/cms/ui3_data1/dapparu/HSCP/Production/prodMarch2021_CMSSW_10_6_2/SingleMuon/2017B/nt_data_aod-2.root
// /opt/sbg/cms/ui3_data1/dapparu/HSCP/Production/prodMarch2021_CMSSW_10_6_2/HSCPgluino_M-1600_TuneCP5_13TeV-pythia8/MC17_Gluino1600_runv3/210324_135858/0000/nt_mc_aod_10.root
#ifdef efficiency_cxx

AnaEff::AnaEff(TTree *tree) : fChain(0) //constructeur
{
	cout << "hi" << endl;
	//triggerName = 0;
// if parameter tree is not specified (or zero), connect the file
// used to generate this class and read the Tree.
	if (tree == 0) {
		TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject("/opt/sbg/cms/safe1/cms/rhaeberl/CMSSW_10_6_2/src/HSCPtriggerStudies/first5.root"); // /home/raph/CMS/nt_data_aod.root / /home/raph/CMS/prodMarch2021_CMSSW_10_6_2/nt_mc_aod_1.root
///opt/sbg/cms/ui3_data1/dapparu/HSCP/Production/prodMarch2021_CMSSW_10_6_2/HSCPgluino_M-1600_TuneCP5_13TeV-pythia8/MC17_Gluino1600_runv3/210324_135858/0000
		
		//pas dans stage ?
///home/raph/CMS/prodMarch2021_CMSSW_10_6_2/SingleMuon/run2017D_march21/210316_163645/0000/nt_mc_aod_106.root
		if (!f || !f->IsOpen()) {
			f = new TFile("/opt/sbg/cms/safe1/cms/rhaeberl/CMSSW_10_6_2/src/HSCPtriggerStudies/first5.root"); // /home/raph/CMS/nt_data_aod.root / /home/raph/CMS/prodMarch2021_CMSSW_10_6_2/nt_mc_aod_1.root
		}
		
	cout << "hi2" << endl;
		TDirectory * dir = (TDirectory*)f->Get("/opt/sbg/cms/safe1/cms/rhaeberl/CMSSW_10_6_2/src/HSCPtriggerStudies/first5.root:/stage"); //  // /home/raph/CMS/prodMarch2021_CMSSW_10_6_2/SingleMuon/run2017D_march21/210316_163645/0000/nt_mc_aod_237.root
		dir->GetObject("ttree",tree);
		

	cout << "hi2" << endl;

  	 }
  
   //passTrigger = new bool[ntrigger];
   // /opt/sbg/cms/ui3_data1/dapparu/HSCP/Production/prodMarch2021_CMSSW_10_6_2/SingleMuon/2017B/nt_data_aod-2.root
   Init(tree);

}
////home/raph/CMS/prodMarch2021_CMSSW_10_6_2/smallaod.root
// /opt/sbg/cms/ui3_data1/dapparu/HSCP/Production/prodApril2021_CMSSW_10_6_2/MET/0001/nt_data_aod_1-1059.root
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
   fChain->SetBranchAddress("prescaleTrigger", prescaleTrigger, &b_prescaleTrigger);

   //fChain->SetBranchAddress("nameTrigger", &triggerName, &b_triggerName);
   fChain->SetBranchAddress("nmuons", &nmuons, &b_nmuons);
   fChain->SetBranchAddress("passTrigger", passTrigger, &b_passTrigger); // & devant PT 1
   fChain->SetBranchAddress("runNumber", &runNumber, &b_runNumber);
   fChain->SetBranchAddress("event", &event, &b_event);
   fChain->SetBranchAddress("npv", &npv, &b_npv);
   fChain->SetBranchAddress("ngoodpv", &ngoodpv, &b_ngoodpv);
   fChain->SetBranchAddress("track_pt", track_pt, &b_track_pt);
   fChain->SetBranchAddress("track_pterr", track_pterr, &b_track_pterr);
   fChain->SetBranchAddress("hscp_track_idx", hscp_track_idx, &b_hscp_track_idx);
   fChain->SetBranchAddress("nhscp", &nhscp, &b_nhscp);

   fChain->SetBranchAddress("pfmet_pt", pfmet_pt, &b_pfmet_pt);
   fChain->SetBranchAddress("track_eta", track_eta, &b_track_eta);
   fChain->SetBranchAddress("track_npixhits", track_npixhits, &b_track_npixhits);
   fChain->SetBranchAddress("track_nvalidhits", track_nvalidhits, &b_track_nvalidhits);
   fChain->SetBranchAddress("track_validfraction", track_validfraction, &b_track_validfraction);
   fChain->SetBranchAddress("ndedxhits", &ndedxhits, &b_ndedxhits);
   fChain->SetBranchAddress("track_dxy", track_dxy, &b_track_dxy);
   fChain->SetBranchAddress("track_qual", track_qual, &b_track_qual);
   fChain->SetBranchAddress("track_nhits", track_nhits, &b_track_nhits);
   fChain->SetBranchAddress("ndedxhits", &ndedxhits, &b_ndedxhits);
   fChain->SetBranchAddress("track_dz", track_dz, &b_track_dz);
   fChain->SetBranchAddress("hscp_iso2_tk", hscp_iso2_tk, &b_hscp_iso2_tk);
   fChain->SetBranchAddress("muon_isTrackerMuon", muon_isTrackerMuon, &b_muon_isTrackerMuon);
   fChain->SetBranchAddress("muon_comb_inversebeta", muon_comb_inversebeta, &b_muon_comb_inversebeta);
   fChain->SetBranchAddress("track_p", track_p, &b_track_p);
   fChain->SetBranchAddress("track_ih_ampl", track_ih_ampl, &b_track_ih_ampl);
   fChain->SetBranchAddress("muon_phi", muon_phi, &b_muon_phi);
   fChain->SetBranchAddress("muon_eta", muon_eta, &b_muon_eta);
   fChain->SetBranchAddress("muon_pt", muon_pt, &b_muon_pt);

 //  fChain->SetBranchAddress("hscp_muon_idx", hscp_muon_idx, &b_hscp_muon_idx); 
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
