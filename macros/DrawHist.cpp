#include <TH2.h>
#include <TROOT.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <TFile.h>
#include <TF1.h>
#include <iostream>
#include <fstream>
#include "DrawHist.h"


using namespace std; 

DrawHist::DrawHist(){
	OutputHisto = 0;
	
	myFile=0;
	
	/*for(int i=0; i < ListTriggers.size() ; i++){
		EffvsObs[i]=0;
	}*/
}

DrawHist::~DrawHist(){ 
	if(!OutputHisto){
		delete OutputHisto;
	}
	if(!myFile){
		delete myFile;
	}
	
}


void DrawHist::FitSignalBg(){
	ofstream InfosPurity;
	InfosPurity.open ("PurityallMET.txt");

	
	myFile = new TFile("64-00.root");
	TH1D* HIST_MASSES = (TH1D*)gROOT->FindObject("MASS");
	

	outputfilename="FIT_ALL.root";

	OutputHisto = new TFile(outputfilename,"RECREATE");

	
	TH1D* HIST_FITBG = (TH1D*) HIST_MASSES->Clone();
	TH1D* HIST_FITSIG = (TH1D*) HIST_MASSES->Clone();

	
	HIST_FITSIG->SetName("FitSignal");
	HIST_FITBG->SetName("FitBackground");
	
	HIST_FITBG->GetXaxis()->SetTitle("M [GeV]");
	HIST_FITBG->GetYaxis()->SetTitle(" # candidates");
	

	HIST_FITSIG->GetXaxis()->SetTitle("M [GeV]");
	HIST_FITSIG->GetYaxis()->SetTitle(" # candidates");
	
	int nbinxfit = HIST_FITSIG->GetNbinsX();
	int nbinyfit = HIST_FITSIG->GetNbinsY();
	int tab[nbinxfit];
	cout << "nb of bins : " <<  nbinxfit << endl;
	int nbentriesZ=0;

	for(int x = 0 ; x < nbinxfit ; x++){
		tab[x] = HIST_FITBG->GetBinContent(x);
		//cout << "bin " << x << " = " << tab[x] << endl;
	}
	for(int x = 0 ; x < 40 ; x++){
		HIST_FITSIG->SetBinContent(x,0);
	}
	for(int x = 50; x < 70 ; x++){
		HIST_FITSIG->SetBinContent(x,0);
	}
	
	for(int x = 40 ; x < 50 ; x++){
		//compter ici le signal
		nbentriesZ+=HIST_FITSIG->GetBinContent(x);
		HIST_FITBG->SetBinContent(x,0);

	}
	for(int x = 0 ; x < 10 ; x++){
		HIST_FITBG->SetBinContent(x,0);
	}
	
	
	HIST_FITSIG->Fit("gaus");
	TF1 *s = (TF1*)HIST_FITSIG->GetListOfFunctions()->FindObject("gaus");
	double m = s->GetParameter(0);
	double IntegralGauss = s->Integral(86,96);


	HIST_FITBG->Fit("pol0");
	//TF1 *g = new TF1("title","aa",0,160);
	TF1 *g = (TF1*)HIST_FITBG->GetListOfFunctions()->FindObject("pol0");
	
	double c = g->GetParameter(0);
	//cout << c << endl;
	
	double IntegralBg = g->Integral(86,96);
	
	cout << "This is the integral of the Signal in bins [40-50]: " << IntegralGauss << endl;
	cout << "This is the integral of the background with bins [40-50] = 0 : " << IntegralBg << endl;
	cout << "Ratio signal/total ¦ bins in 40-50 -> 0 = " << (IntegralGauss *1.0 / (IntegralGauss+IntegralBg))*100 << " %" << endl;
	
	cout << "There was " << nbentriesZ << " Z pair" << endl;



	InfosPurity << "Integral of the Signal in bins [40-50] : " << IntegralGauss << " , Integral of the background in bins [40-50] : " << IntegralBg << endl ;

	InfosPurity << "Ratio signal/total ¦ bins in 40-50 -> 0 = " << (IntegralGauss *1.0 / (IntegralGauss+IntegralBg))*100 << " %" << endl;
	
	InfosPurity << "There was " << nbentriesZ << " Z pair" << endl;

	InfosPurity.close();
	OutputHisto->cd();
	
	HIST_FITBG->Write();
	HIST_FITSIG->Write();
	HIST_MASSES->Write();

	OutputHisto->Close();

}




int main(){

	DrawHist h;
	h.FitSignalBg();

}


