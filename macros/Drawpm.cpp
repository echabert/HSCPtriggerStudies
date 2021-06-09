#include <TH2.h>
#include <TROOT.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <TFile.h>
#include <TF1.h>
#include "Drawpm.h"
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
#include <TMultiGraph.h>
#include <TLegendEntry.h>
#include <TFrame.h>
#include <TPaveText.h>
using namespace std; 

Drawpm::Drawpm(){
	myFile = 0;
	OutputHisto = 0;
	/*for(int i=0; i < ListTriggers.size() ; i++){
		EffvsObs[i]=0;
	}*/
}

Drawpm::~Drawpm(){ 
	if(!OutputHisto){
		delete OutputHisto;
	}
	if(!myFile){
		delete myFile;
	}
	
}


void Drawpm::FitSignalPM(){
	string filepath = "/home/raph/CMS/HSCPtriggerStudies/data/MergedMET/RENDU_5/Gluino/PreselAndSel/Selec/Loose/";
	string pointofmass,DataType="Gluino",Date = "1105", All = "all", ExtRoot = ".root",Distribz = "DistribZpeak";
	int minm=1600,maxm=2400,nbmbin=200;
	int nbdiffpt = ((maxm- minm)/nbmbin) +1;
	AllFiles.resize(nbdiffpt);
	TempTr.resize(nbdiffpt);
	MyMyF.resize(nbdiffpt);
	MyTf1.resize(nbdiffpt);
	TString outputfilename="/home/raph/CMS/HSCPtriggerStudies/data/MergedMET/RENDU_5/Gluino/PreselAndSel/Selec/Loose/Eff/Povermwithmass.root";
	
	OutputHisto = new TFile(outputfilename,"RECREATE");
	
	int nbcount = 0;
	for(int i = minm; i <= maxm ; i+=nbmbin){
		
		pointofmass = to_string(i);
		
		
		string PathPom = filepath + Distribz  + DataType + pointofmass + Date + All + ExtRoot;
		string Title = "Ratio p/m distribution " + DataType + pointofmass;
		
		cout << PathPom << endl;

		
		MyMyF[nbcount] = new TFile(PathPom.c_str());

		if (!MyMyF[nbcount] || !MyMyF[nbcount]->IsOpen()) {
			cout << "There was a problem opening the input file!" << PathPom << endl;
		}
		
		else{
			TempTr[nbcount] = (TH1D*)gROOT->FindObject("DISTRIB_POVERM");
			TempTr[nbcount]->SetTitle(Title.c_str());
			cout << "all good" << endl;
		}

	nbcount+=1;
	}


	cout << "after loop" << endl;
	
	auto c1 = new TCanvas("c1","Ratio p/m",1300,700);
	TLegend* leg1 = new TLegend(0.7, 0.8, .5, .6);

	c1->SetTitle("Ratio impulsion / Mass");
	TH1D *hr = new TH1D("hr","P/m for different masses",100,0,3);
	hr->SetXTitle("P/M");
	hr->SetYTitle("# Gluino");
	hr->SetStats(kFALSE);
	hr->Draw();
	c1->GetFrame()->SetBorderSize(12);
	cout << "before new TF1 " << endl;
	for(int j = 0; j < nbdiffpt ; j++){
		string nametf1 = "g" + to_string(j);
		MyTf1[j] = new TF1(nametf1.c_str(), "landau", 0, 3);
		cout << "after new tf1" << endl;
		//MyTf1[]->SetName(
		if(j==0){
			MyTf1[j]->SetLineColor(9);
			MyTf1[j]->SetLineStyle(1);
			MyTf1[j]->SetLineWidth(2);
			//MyTf1[j]->Draw();
			TempTr[j]->Fit(MyTf1[j]);
			TempTr[j]->Draw("SAME");
			c1->Modified();
			c1->Update();
		}
		if(j==1){
			MyTf1[j]->SetLineColor(8);
			MyTf1[j]->SetLineStyle(1);
			MyTf1[j]->SetLineWidth(2);
			//MyTf1[j]->Draw();
			TempTr[j]->Fit(MyTf1[j]);
			TempTr[j]->Draw("SAME");		
			c1->Modified();
			c1->Update();
		}
		if(j==2){
			MyTf1[j]->SetLineColor(7);
			MyTf1[j]->SetLineStyle(1);
			MyTf1[j]->SetLineWidth(2);
			//MyTf1[j]->Draw();
			//c1->Modified();
			//c1->Update();
		}
		if(j==3){
			MyTf1[j]->SetLineColor(6);
			MyTf1[j]->SetLineStyle(1);
			MyTf1[j]->SetLineWidth(2);
			//MyTf1[j]->Draw();
			TempTr[j]->Fit(MyTf1[j]);
			TempTr[j]->Draw("SAME");
			c1->Modified();
			c1->Update();
		}
		if(j==4){
			MyTf1[j]->SetLineColor(5);
			MyTf1[j]->SetLineStyle(1);
			MyTf1[j]->SetLineWidth(2);
			//MyTf1[j]->Draw();
			TempTr[j]->Fit(MyTf1[j]);
			TempTr[j]->Draw("SAME");
			c1->Modified();
			c1->Update();
		}
		if(j==5){
			MyTf1[j]->SetLineColor(4);
			MyTf1[j]->SetLineStyle(1);
			MyTf1[j]->SetLineWidth(2);
			TempTr[j]->Fit(MyTf1[j]);
			TempTr[j]->Draw("SAME");
			//MyTf1[j]->Draw();
			c1->Modified();
			c1->Update();
		}
		if(j==6){
			MyTf1[j]->SetLineColor(3);
			MyTf1[j]->SetLineStyle(1);
			MyTf1[j]->SetLineWidth(2);
			//MyTf1[j]->Draw();
			TempTr[j]->Fit(MyTf1[j]);
			TempTr[j]->Draw("SAME");
			c1->Modified();
			c1->Update();
		}

		
		//hr->Fit(MyTf1[0],"R");

		//TempTr[j]->Fit(MyTf1[j],"SAME+");
		
		//c1->Modified();
		//c1->Update();

	
	}

	


	
	cout << "before write " << endl;
	
	OutputHisto->cd();
	c1->Write();
	OutputHisto->Close();
}




int main(){

	Drawpm h;
	h.FitSignalPM();

}


