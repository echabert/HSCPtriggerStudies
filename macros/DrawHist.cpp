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

	string Purity = "Purity";
	string DataType = "Stau";
	string ExtTxt = ".txt";
	string Date = "1105_";
	string SubNum = "all";
	
	string Path = "/home/raph/CMS/HSCPtriggerStudies/data/MergedMET/11.05/";
	string OutPutName = Path + Purity + DataType + Date + SubNum + ExtTxt;


	InfosPurity.open (OutPutName.c_str());

	TString filepath = "/home/raph/CMS/HSCPtriggerStudies/data/MergedMET/11.05/MU1105_all.root";  ///home/raph/CMS/HSCPtriggerStudies/data/MergedMET/Cuts3/64-00.root
	
	
	myFile = new TFile(filepath);
	TH1D* HIST_MASSES = (TH1D*)gROOT->FindObject("MASS");
	HIST_MASSES->SetTitle("Invariant mass of candidates");
	//HIST_MASSES->SetMarkerColor(1);


	outputfilename="/home/raph/CMS/HSCPtriggerStudies/data/MergedMET/11.05/HIST_MU1105.root";

	

	OutputHisto = new TFile(outputfilename,"RECREATE");

	
	TH1D* HIST_FITBG = (TH1D*) HIST_MASSES->Clone();


	TH1D* HIST_FITSIG = (TH1D*) HIST_MASSES->Clone();
	
	TH1D* HIST_FIT = (TH1D*) HIST_MASSES->Clone();

	TH1D* HIST_FITSUM = (TH1D*) HIST_MASSES->Clone();

	//HIST_FITBG->SetMarkerColor(kBlack);
	//HIST_FITBG->SetMarkerColor(kBlack);
	//HIST_FITSIG->SetMarkerColor(kBlack);


	HIST_FITBG->SetName("FitBackground");
	
	HIST_FITSIG->SetName("FitSignal");

	HIST_FIT->SetName("General fit");

	HIST_FITSUM->SetName("Fit with sum");

	HIST_FITSUM->GetXaxis()->SetTitle("Mass [GeV]");
	HIST_FITSUM->GetYaxis()->SetTitle(" # candidates");
	
	HIST_FITBG->GetXaxis()->SetTitle("Mass [GeV]");
	HIST_FITBG->GetYaxis()->SetTitle(" # candidates");
	

	HIST_FITSIG->GetXaxis()->SetTitle("Mass [GeV]");
	HIST_FITSIG->GetYaxis()->SetTitle(" # candidates");
	

	HIST_FIT->GetXaxis()->SetTitle("Mass [GeV]");
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
		//cout << randombg << endl;
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

	int sizeparamgaus = 3,sizeparampol=2;
	double m[sizeparamgaus],m2[sizeparampol];
	

	TF1 *g2 = new TF1("g2", "pol1", 15,140);
	g2->SetLineColor(6);
	g2->SetLineStyle(1);
	g2->SetLineWidth(2);


	TF1 *g1 = new TF1("g1","gaus", 75,105);
	g1->SetLineColor(kRed);
	g1->SetLineStyle(1);
	g1->SetLineWidth(3);

	
	HIST_FITBG->Fit(g2,"R");
	HIST_FITSIG->Fit(g1,"R");


	HIST_FIT->Fit(g2,"R");
	HIST_FIT->Fit(g1,"R+");


	TF1 *g = (TF1*)HIST_FIT->GetListOfFunctions()->FindObject(g2);
	double IntegralBg = g2->Integral(86,96);
	TF1 *s = (TF1*)HIST_FIT->GetListOfFunctions()->FindObject(g1);
	double IntegralGauss = g1->Integral(86,96);
	
	for (int i =0 ; i < sizeparamgaus ; i++){
		m[i] = g1->GetParameter(i);
		cout << m[i] << endl;
	}
	for (int i =0 ; i < sizeparampol ; i++){
		m2[i] = g2->GetParameter(i);
		cout << m2[i] << endl;
	}
	
	

	TF1 *fitboth = new TF1("fits","gaus(0) +pol1(3)", 15,140);
	fitboth->SetLineColor(2);
	fitboth->SetParameters(m[0],m[1],m[2],m2[0],m2[1]);
	HIST_FITSUM->Fit(fitboth,"R+");

	auto leg1 =new TLegend(0.13,0.45,0.48,0.65);
	
	leg1->SetHeader("Background and signal fits");
	leg1->AddEntry(fitboth,"Sum of background and signal in the region of interest","l");
	//leg1->AddEntry(g1,"Signal : gaussian","l");
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
	


	/*auto c2 = new TCanvas("c2","Fit for background",1300,700);
	auto leg2 = new TLegend(0.13,0.45,0.48,0.65);
	leg1->AddEntry(g2,"Background : polynom of degree 1","l");
	leg2->SetTextFont(42);
	leg2->SetTextSize(0.03);
	leg2->SetBorderSize(0);
	leg2->SetFillStyle(0);
	c2->cd();
	
	leg2->Draw();
	TLegendEntry *header2 = (TLegendEntry*)leg2->GetListOfPrimitives()->First();
	header2->SetTextColor(1);
	header2->SetTextSize(.04);*/


	cout << "Integral of the Signal for mass between [86-96] GeV: " << IntegralGauss << endl;
	cout << "Integral of the background for mass between [86-96] GeV = 0 : " << IntegralBg << endl;
	cout << "Ratio signal/total ¦ bins in 40-50 -> 0 = " << (IntegralGauss *1.0 / (IntegralGauss+IntegralBg))*100 << " %" << endl;
	
	cout << "There was " << nbentriesZ << " Z pair" << endl;



	InfosPurity << "Integral of the Signal for mass between [86-96] : " << IntegralGauss << " , Integral of the background for mass between [86-96] : " << IntegralBg << endl ;

	InfosPurity << "Ratio signal/total ¦ bins in 40-50 -> 0 = " << (IntegralGauss *1.0 / (IntegralGauss+IntegralBg))*100 << " %" << endl;
	
	InfosPurity << "There was " << nbentriesZ << " Z pair" << endl;

	InfosPurity.close();

	OutputHisto->cd();

	c1->Write();
	HIST_FITSUM->Write();
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


