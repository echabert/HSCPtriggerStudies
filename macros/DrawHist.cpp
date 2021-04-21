#include <TH2.h>
#include <TROOT.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <TFile.h>
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
	
	myFile = new TFile("all.root");
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
		if(x == 45 || x == 46){
			nbentriesZ+=HIST_FITSIG->GetBinContent(x);
		}
		HIST_FITBG->SetBinContent(x,0);
	}
	
	HIST_FITSIG->Fit("gaus");
	double IntegralGauss = HIST_FITSIG->Integral(40,50, "width");


	HIST_FITBG->Fit("expo");
	
	
	double IntegralBg = HIST_FITBG->Integral(40,50, "width");
	
	cout << "This is the integral of the background with bins [40-50] = 0 : " << IntegralBg << endl;
	cout << "Ratio signal/total ¦ bins in 40-50 -> 0 = " << (IntegralGauss *1.0 / (IntegralGauss+IntegralBg))*100 << " %" << endl;
	
	cout << "There was " << nbentriesZ << " Z pair" << endl;
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


