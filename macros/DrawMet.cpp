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
	METCHCH=0;
	METCHN=0;
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
	if(!METCHN){
		delete METCHN;
	}
	if(!METCHCH){
		delete METCHCH;
	}



}


void DrawMet::FitSignalMet(){

	double effprevsno[400]= {0}, effselvsno[400]= {0};
	double xeffprevsno[400] = {0}, xeffselvsno[400] = {0};

	string filepathmet = "/home/raph/CMS/HSCPtriggerStudies/data/MergedMET/RENDU_5/Gluino/Ias0.20207/";
	//string filepathmetpresel = "/home/raph/CMS/HSCPtriggerStudies/data/MergedMET/RENDU_5/Gluino/New/;

	string pointofmass,pointofmasssel,DataType="Gluino",Date = "1105", All = "all", ExtRoot = ".root",Distribz = "DistribZpeak";
	
	string PathPomMet = filepathmet + Distribz  + DataType + "1800" + Date + All + ExtRoot;

	TString outputfilenamemet="/home/raph/CMS/HSCPtriggerStudies/data/MergedMET/RENDU_5/Gluino/Ias0.20207/MetRatio.root";

	OutputHistoMet = new TFile(outputfilenamemet,"RECREATE");



	MyMyFMet = new TFile(PathPomMet.c_str());

	TempTrMet = (TH1D*)gROOT->FindObject("DISTRIB_METNOSEL");
	TempTr2Met = (TH1D*)gROOT->FindObject("DISTRIB_METPRESEL");
	TempTr3Met = (TH1D*)gROOT->FindObject("DISTRIB_METSEL");
	METCHCH = (TH1D*)gROOT->FindObject("DISTRIB_MET_CHCH");
	METCHN = (TH1D*)gROOT->FindObject("DISTRIB_MET_CHN");
	
	double x0chch[100]={0}, y0chch[100]={0}, x0chn[100]={0}, y0chn[100]={0};

	EffMetPre = new TH1D("EffMetPre", "( Eff Presel) ", 100,0,4000);

	EffMetPre->GetXaxis()->SetTitle("Reco MET [GeV]");
	EffMetPre->GetYaxis()->SetTitle(" Efficiency");

	EffMetSel = new TH1D("EffMetSel", "( Eff Sel) ", 100,0,4000);
	EffMetSel->GetXaxis()->SetTitle("Reco MET [GeV]");
	EffMetSel->GetYaxis()->SetTitle(" Efficiency");


	for(int i = 0; i < 100 ; i++){
		xeffprevsno[i] = i*40;
		double s = TempTrMet->GetBinContent(i);
		//cout << " no sel bin " << i << " : " << s << endl;
		double t = TempTr2Met->GetBinContent(i);
		//cout << " presel bin " << i << " : " << t << endl;
		double k = TempTr3Met->GetBinContent(i);
		if(s!=0){
			effprevsno[i] = t*1.0/s;
			effselvsno[i] = k*1.0/s;
		}
		else if(s == 0 && i != 0){
			effprevsno[i] = effprevsno[i-1];
			effselvsno[i] = effselvsno[i-1];
		}

		//cout << "presel vs nosel " << effprevsno[i] << endl;	
		//cout << "sel vs no sel " << effselvsno[i] << endl;

		
		EffMetPre->SetBinContent(i,effprevsno[i]);
		EffMetSel->SetBinContent(i,effselvsno[i]);
	}
	

	Double_t scalemetchn = (1.0/METCHN->Integral());
	Double_t scalemetchch = (1.0/METCHCH->Integral());
	cout << "scale ch n : " << scalemetchn << " and ch ch : " << scalemetchch << endl;
	METCHN->Scale(scalemetchn);
	METCHCH->Scale(scalemetchch);

	int s=0;
	for(int i=0; i<100;i++){
		s=i*40;
		y0chch[i] = METCHCH->GetBinContent(i);

		x0chch[i] = s;

		y0chn[i] = METCHN->GetBinContent(i);

		x0chn[i] = s;
	
	}
	auto c2 = new TCanvas("c2","Distrib MET different scenarios",1300,700);
	c2->SetTitle("Efficiency MET vs selections");
	TLegend* leg2 = new TLegend(0.7, 0.8, .5, .6);

	TMultiGraph *mg2 = new TMultiGraph();
	

	MET_chch = new TGraphErrors(100, x0chch, y0chch);
	MET_chch->SetLineColor(8);
	MET_chch->SetLineStyle(1);
	MET_chch->SetLineWidth(1);
	//Test[0]->Fit(MyTf1[0],"q");
	MET_chch->SetMarkerColor(8);
   	MET_chch->SetMarkerStyle(49);
	MET_chch->SetMarkerSize(2);
	
	leg2->AddEntry(MET_chch,"charged-charged Reco pf MET");
	mg2->Add(MET_chch,"p");
	c2->Modified();
	c2->Update();

	MET_chn = new TGraphErrors(100, x0chn, y0chn);
	MET_chn->SetLineColor(7);
	MET_chn->SetLineStyle(1);
	MET_chn->SetLineWidth(1);
	//Test[0]->Fit(MyTf1[0],"q");
	MET_chn->SetMarkerColor(7);
   	MET_chn->SetMarkerStyle(49);
	MET_chn->SetMarkerSize(2);
	
	leg2->AddEntry(MET_chn,"charged-neutral Reco pf MET");
	mg2->Add(MET_chn,"p");
	c2->Modified();
	c2->Update();


	mg2->Draw("a");
	c2->Update();
	c2->Modified();
	mg2->GetXaxis()->SetTitle("Reco MET [GeV]");
	mg2->GetYaxis()->SetTitle("Efficiency");
	//mg2->GetYaxis()->SetRange(0,1.1);
	mg2->GetHistogram()->SetTitle("Efficiency of various selections");
	
	c2->cd();
	c2->GetFrame()->SetBorderSize(12);
	leg2->SetBorderSize(0);
	leg2->Draw("p");


	Double_t scalenosel = (1.0/TempTrMet->Integral());
	Double_t scalepresel = (1.0/TempTr2Met->Integral());
	Double_t scalesel = (1.0/TempTr3Met->Integral());

	TempTrMet->Scale(scalenosel);
	TempTr2Met->Scale(scalepresel);
	TempTr3Met->Scale(scalesel);

	double intnosel = TempTrMet->Integral(3,99);
	double intpresel = TempTr2Met->Integral(3,99);
	double intsel = TempTr3Met->Integral(3,99);

	cout << " Integrals between 120 - 4000 GeV : " << endl; 
	cout << "Integral nosel : " << intnosel << " , integral presel : " << intpresel << " , integral sel : " << intsel << endl;

	double intnosel2 = TempTrMet->Integral(3,15);
	double intpresel2 = TempTr2Met->Integral(3,15);
	double intsel2 = TempTr3Met->Integral(3,15);

	cout << " Integrals between 120 - 600 GeV : " << endl; 
	cout << "Integral nosel : " << intnosel2 << " , integral presel : " << intpresel2 << " , integral sel : " << intsel2 << endl;


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
	TestMet->SetMarkerSize(2);
	
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
	Test2Met->SetMarkerSize(2);
	
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
	leg->Draw("p");
	
	OutputHistoMet->cd();
	c->Write();
	c2->Write();
	EffMetPre->Write();
	EffMetSel->Write();
	OutputHistoMet->Close();

}




int main(){

	DrawMet u;
	u.FitSignalMet();

}
