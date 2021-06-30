#include <TH2.h>
#include <TROOT.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <TFile.h>
#include <TF1.h>
#include "DrawMet.h"
#include <iostream>
#include <iomanip>
#include <fstream>
#include <ctime>
#include "DrawHist.h"
#include <TCanvas.h>
#include <vector>
#include <TLegend.h>
#include <TGraph.h>
#include <TGraphErrors.h>
#include <random>
#include <ctime>
#include <TMultiGraph.h>
#include <TLegendEntry.h>
#include <TFrame.h>
#include <TPaveText.h>
#include <TPaveStats.h>
using namespace std; 

DrawMet::DrawMet(){
	myFileMet = 0;
	OutputHistoMet = 0;
	MyMyFMet=0;
	TempTrMet=0;
	TempTr2Met=0;
	TempTr3Met=0;
	EffMetPre=0;
	EffMetSel=0;
	/*for(int i=0; i < ListTriggers.size() ; i++){
		EffvsObs[i]=0;
	}*/
}

DrawMet::~DrawMet(){ 
	if(!OutputHistoMet){
		delete OutputHistoMet;
	}
	if(!myFileMet){
		delete myFileMet;
	}
	if(!MyMyFMet){
		delete MyMyFMet;
	}
	if(!TempTrMet){
		delete TempTrMet;
	}
	if(!TempTr2Met){
		delete TempTr2Met;
	}
	if(!TempTr3Met){
		delete TempTr3Met;
	}
	if(!EffMetPre){
		delete EffMetPre;
	}
	if(!EffMetSel){
		delete EffMetSel;
	}
}


void DrawMet::FitSignalMet(){

	double effprevsno[400]= {0}, effselvsno[400]= {0};
	double xeffprevsno[400] = {0}, xeffselvsno[400] = {0};

	string filepathmet = "/home/raph/CMS/HSCPtriggerStudies/data/MergedMET/RENDU_5/Gluino/New/Sel_Ias0.2/";
	//string filepathmetpresel = "/home/raph/CMS/HSCPtriggerStudies/data/MergedMET/RENDU_5/Gluino/New/;

	string pointofmass,pointofmasssel,DataType="Gluino",Date = "1105", All = "all", ExtRoot = ".root",Distribz = "DistribZpeak";
	
	string PathPomMet = filepathmet + Distribz  + DataType + "1800" + Date + All + ExtRoot;

	TString outputfilenamemet="/home/raph/CMS/HSCPtriggerStudies/data/MergedMET/RENDU_5/Gluino/New/Sel_Ias0.2/MetRatio.root";

	OutputHistoMet = new TFile(outputfilenamemet,"RECREATE");



	MyMyFMet = new TFile(PathPomMet.c_str());

	TempTrMet = (TH1D*)gROOT->FindObject("DISTRIB_METNOSEL");
	TempTr2Met = (TH1D*)gROOT->FindObject("DISTRIB_METPRESEL");
	TempTr3Met = (TH1D*)gROOT->FindObject("DISTRIB_METSEL");

	EffMetPre = new TH1D("EffMetPre", "( Eff Presel) ", 100,0,4000);

	EffMetPre->GetXaxis()->SetTitle("Reco MET [GeV]");
	EffMetPre->GetYaxis()->SetTitle(" Efficiency");

	EffMetSel = new TH1D("EffMetSel", "( Eff Sel) ", 100,0,4000);
	EffMetSel->GetXaxis()->SetTitle("Reco MET [GeV]");
	EffMetSel->GetYaxis()->SetTitle(" Efficiency");


	for(int i = 0; i < 400 ; i++){
		xeffprevsno[i] = i*10;
		double s = TempTrMet->GetBinContent(i);
		cout << " no sel bin " << i << " : " << s << endl;
		double t = TempTr2Met->GetBinContent(i);
		cout << " presel bin " << i << " : " << t << endl;
		double k = TempTr3Met->GetBinContent(i);
		if(s!=0){
			effprevsno[i] = t*1.0/s;
		}
		
		cout << "presel vs nosel " << effprevsno[i] << endl;
		if(s!=0){
			effselvsno[i] = k*1.0/s;
		}
			
		cout << "sel vs no sel " << effselvsno[i] << endl;

		
		EffMetPre->SetBinContent(i,effprevsno[i]);
		EffMetSel->SetBinContent(i,effselvsno[i]);
	}

	auto c = new TCanvas("c","Efficiency MET vs selections",1300,700);
	c->SetTitle("Efficiency MET vs selections");
	TLegend* leg = new TLegend(0.7, 0.8, .5, .6);

	TMultiGraph *mg = new TMultiGraph();



	TestMet = new TGraphErrors(400, xeffprevsno,effprevsno);
	TestMet->SetLineColor(8);
	TestMet->SetLineStyle(1);
	TestMet->SetLineWidth(1);
	//Test[0]->Fit(MyTf1[0],"q");
	TestMet->SetMarkerColor(8);
   	TestMet->SetMarkerStyle(49);
	TestMet->SetMarkerSize(1);
	
	leg->AddEntry(TestMet,"# presel / # no sel, mass = 1800 GeV/c^{2}");
	mg->Add(TestMet,"p");
	c->Modified();
	c->Update();

	Test2Met = new TGraphErrors(400, xeffprevsno, effselvsno);
	Test2Met->SetLineColor(7);
	Test2Met->SetLineStyle(1);
	Test2Met->SetLineWidth(1);
	//Test[0]->Fit(MyTf1[0],"q");
	Test2Met->SetMarkerColor(7);
   	Test2Met->SetMarkerStyle(49);
	Test2Met->SetMarkerSize(1);
	
	leg->AddEntry(Test2Met,"# sel / # no sel, mass = 1800 GeV/c^{2}");
	mg->Add(Test2Met,"p");
	c->Modified();
	c->Update();


	mg->Draw("a");
	c->Update();
	c->Modified();
	mg->GetXaxis()->SetTitle("Reco MET [GeV]");
	mg->GetYaxis()->SetTitle("Efficiency");
	//mg2->GetYaxis()->SetRange(0,1.1);
	mg->GetHistogram()->SetTitle("Efficiency of various selections");
	
	c->cd();
	c->GetFrame()->SetBorderSize(12);
	leg->SetBorderSize(0);
	leg->Draw();
	
	OutputHistoMet->cd();
	c->Write();
	EffMetPre->Write();
	EffMetSel->Write();
	OutputHistoMet->Close();

}




int main(){

	DrawMet u;
	u.FitSignalMet();

}
