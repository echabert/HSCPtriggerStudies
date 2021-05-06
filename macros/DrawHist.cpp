#include <TH2.h>
#include <TROOT.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <TFile.h>
#include <TF1.h>
#include <iostream>
#include <fstream>
#include <ctime>
#include "DrawHist.h"
#include <TCanvas.h>
#include <TLegend.h>
#include <TGraph.h>
#include <TMultiGraph.h>
#include <TLegendEntry.h>
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
	srand(time(NULL));
	ofstream InfosPurity;
	InfosPurity.open ("/home/raph/CMS/HSCPtriggerStudies/data/MergedMET/30Apr_All/PurityMET.txt");

	TString filepath = "/home/raph/CMS/HSCPtriggerStudies/data/MergedMET/30Apr_All/MET0505_all.root";  ///home/raph/CMS/HSCPtriggerStudies/data/MergedMET/Cuts3/64-00.root
	myFile = new TFile(filepath);
	TH1D* HIST_MASSES = (TH1D*)gROOT->FindObject("MASS");
	HIST_MASSES->SetTitle("Invariant mass of candidates");
	//HIST_MASSES->SetMarkerColor(1);


	outputfilename="/home/raph/CMS/HSCPtriggerStudies/data/MergedMET/30Apr_All/HIST_MET30Apr.root";

	OutputHisto = new TFile(outputfilename,"RECREATE");

	
	TH1D* HIST_FITBG = (TH1D*) HIST_MASSES->Clone();


	TH1D* HIST_FITSIG = (TH1D*) HIST_MASSES->Clone();
	
	TH1D* HIST_FIT = (TH1D*) HIST_MASSES->Clone();

	//HIST_FITBG->SetMarkerColor(kBlack);
	//HIST_FITBG->SetMarkerColor(kBlack);
	//HIST_FITSIG->SetMarkerColor(kBlack);


	HIST_FITBG->SetName("FitBackground");
	
	HIST_FITSIG->SetName("FitSignal");

	HIST_FIT->SetName("General fit");
	
	HIST_FITBG->GetXaxis()->SetTitle("M [GeV]");
	HIST_FITBG->GetYaxis()->SetTitle(" # candidates");
	

	HIST_FITSIG->GetXaxis()->SetTitle("M [GeV]");
	HIST_FITSIG->GetYaxis()->SetTitle(" # candidates");
	

	HIST_FIT->GetXaxis()->SetTitle("M [GeV]");
	HIST_FIT->GetYaxis()->SetTitle(" # candidates");
	
	int nbinxfit = HIST_FITSIG->GetNbinsX();
	int nbinyfit = HIST_FITSIG->GetNbinsY();

	int tab[nbinxfit];
//	double x[nbinxfit],y[nbinyfit],x_2[nbinxfit], y_2[nbinyfit];
	cout << "nb of bins : " <<  nbinxfit << endl;
	int nbentriesZ=0;

	for(int x = 0 ; x < nbinxfit ; x++){
		if(x < 41 || x > 50 ){
			ValBins.push_back(HIST_FITBG->GetBinContent(x));
		}
		tab[x] = HIST_FITBG->GetBinContent(x);
		//cout << "bin " << x << " = " << tab[x] << endl;
	}
	
	sort(ValBins.begin(),ValBins.end());
	int valsize = ValBins.size();
	for(int x = 0 ; x < 40 ; x++){
		HIST_FITSIG->SetBinContent(x,0);
	}
	for(int x = 50; x < 70 ; x++){
		HIST_FITSIG->SetBinContent(x,0);
	}

	for(int x = 40 ; x < 50 ; x++){
		int randombg = rand()%(ValBins[valsize-1]-ValBins[0]+1)+ValBins[0];
		cout << randombg << endl;
		nbentriesZ+=HIST_FITSIG->GetBinContent(x);
		//HIST_FITBG->SetBinContent(x,0);
		HIST_FITBG->SetBinContent(x,randombg);
		

	}
	for(int x = 0 ; x < 10 ; x++){
		HIST_FITBG->SetBinContent(x,0);
	}
	
	

	auto c1 = new TCanvas("c1","Fits for background and signal",1300,700);
	auto pad = new TPad("pad","",0,0,1,1);
	pad->SetGrid();
	pad->Draw();
	pad->cd();



	HIST_FITSIG->Fit("gaus");
	
	TF1 *s = (TF1*)HIST_FITSIG->GetListOfFunctions()->FindObject("gaus");
	
	//double m = s->GetParameter(0);
	double IntegralGauss = s->Integral(86,96);


	HIST_FITBG->Fit("pol0");
	TF1 *g = (TF1*)HIST_FITBG->GetListOfFunctions()->FindObject("pol0");


	//double c = g->GetParameter(0);

	double IntegralBg = g->Integral(86,96);
	

	TF1 *g2 = new TF1("g2", "pol0", 15,140);
	g2->SetLineColor(6);
	g2->SetLineStyle(1);
	g2->SetLineWidth(2);


	TF1 *g1 = new TF1("g1","gaus", 75,105);
	g1->SetLineColor(kRed);
	g1->SetLineStyle(1);
	g1->SetLineWidth(3);
	
	HIST_FIT->Fit(g2,"R");
	HIST_FIT->Fit(g1,"R+");

	auto leg1 =new TLegend(0.13,0.45,0.48,0.65);


	


	leg1->SetHeader("Background and signal fits");
	leg1->AddEntry(g2,"Background : constant","l");
	leg1->AddEntry(g1,"Signal : gaussian","l");
	//leg1->SetLegendFont(42);
	leg1->SetTextFont(42);
	leg1->SetTextSize(0.03);
	leg1->SetBorderSize(0);
	leg1->SetFillStyle(0);
	leg1->Draw();
	TLegendEntry *header = (TLegendEntry*)leg1->GetListOfPrimitives()->First();
	//header->SetTextAlign(13);
	header->SetTextColor(1);
	header->SetTextSize(.04);
	
	


	cout << "This is the integral of the Signal for mass between [86-96]: " << IntegralGauss << endl;
	cout << "This is the integral of the background for mass between [86-96] = 0 : " << IntegralBg << endl;
	cout << "Ratio signal/total ¦ bins in 40-50 -> 0 = " << (IntegralGauss *1.0 / (IntegralGauss+IntegralBg))*100 << " %" << endl;
	
	cout << "There was " << nbentriesZ << " Z pair" << endl;



	InfosPurity << "Integral of the Signal for mass between [86-96] : " << IntegralGauss << " , Integral of the background for mass between [86-96] : " << IntegralBg << endl ;

	InfosPurity << "Ratio signal/total ¦ bins in 40-50 -> 0 = " << (IntegralGauss *1.0 / (IntegralGauss+IntegralBg))*100 << " %" << endl;
	
	InfosPurity << "There was " << nbentriesZ << " Z pair" << endl;

	InfosPurity.close();
	OutputHisto->cd();
	c1->Write();
	
	
	HIST_FIT->Write();
	HIST_FITBG->Write();
	HIST_FITSIG->Write();
	HIST_MASSES->Write();

	OutputHisto->Close();

}




int main(){

	DrawHist h;
	h.FitSignalBg();

}


