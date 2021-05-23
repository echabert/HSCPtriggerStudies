#include <TH2.h>
#include <TROOT.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <TFile.h>
#include <TF1.h>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <ctime>
#include "DrawHist.h"
#include <TCanvas.h>
#include <vector>
#include <TLegend.h>
#include <TGraph.h>
#include <TMultiGraph.h>
#include <TLegendEntry.h>
using namespace std; 

DrawHist::DrawHist(){
	OutputHisto = 0;
	OutputHisto2 = 0;
	myFile=0;
	
	/*for(int i=0; i < ListTriggers.size() ; i++){
		EffvsObs[i]=0;
	}*/
}

DrawHist::~DrawHist(){ 
	if(!OutputHisto){
		delete OutputHisto;
	}
	if(!OutputHisto2){
		delete OutputHisto2;
	}
	if(!myFile){
		delete myFile;
	}
	
}


void DrawHist::FitSignalBg(){
	srand(time(NULL));
	ofstream InfosPurity;

	string Purity = "Purity";
	string DataType = "SingleMuon";

	string ExtTxt = ".txt";
	
	string SubNum = "all";
	string EffList = "Eff";
	string Mass = "1600";
	string Date = "1105all";
	string pointofmass;
	string ExtRoot = ".root";
	string All = "all";
	
	string Path = "/home/raph/CMS/HSCPtriggerStudies/data/MergedMET/RENDU_5/" + DataType + "/";
	
	
	string OutPutName = Path + Purity + DataType + Date + SubNum + ExtTxt;


	InfosPurity.open (OutPutName.c_str());


	string s2 = "mu";
	string s4 = "Mu";
	string s5 = "Muon";
	string s7 = "MET";

	string NameList = "CompleteList";
	
//	string NameCompleteList = "CompleteListTest.txt";

	string NameCompleteListSingleMuon = NameList + DataType + ExtTxt;
	string NameCompleteList = NameList + DataType + Mass + ExtTxt;
	string PathEffFile = "/home/raph/CMS/HSCPtriggerStudies/data/MergedMET/RENDU_5/" + DataType + "/" + NameCompleteListSingleMuon;
	

	vector<string> AlltriggerNames;
	vector<string> SubListMET;
	vector<string> SubListPT;
	
	
	
	cout << NameCompleteList << endl;
	ifstream EfficiencyFile(PathEffFile.c_str());
	
	if(!EfficiencyFile)
		cout  << " cannot open file"  << endl;
	string tmpp;

	while(std::getline(EfficiencyFile,tmpp)){
		AlltriggerNames.push_back(tmpp);
	}
	EfficiencyFile.close();
	cout << "size : " <<AlltriggerNames.size() << endl;
	

	//HIST_MASSES->SetMarkerColor(1);

	
	//Test efficiency fct mass

	outputfilename2="/home/raph/CMS/HSCPtriggerStudies/data/MergedMET/RENDU_5/Gluino/HIST_EffGluino.root";
	
	OutputHisto2 = new TFile(outputfilename2,"RECREATE");
	
	//auto mg = new TMultiGraph();

	Efficiencies2.resize(AlltriggerNames.size());

	for(int l = 0; l < AlltriggerNames.size(); l++){
		Efficiencies2[l] = new TGraph();
		Efficiencies2[l]->SetName(AlltriggerNames[l].c_str());


	}

	Efficiencies.resize(AlltriggerNames.size());
	
	Efficiencies3.resize(6);
	int BinCt = 0;
	for(int k = 1600; k <= 2600 ; k+=200){
		vector<double> EffNotOrdered;
		//EffNotOrdered.resize(AlltriggerNames.size());
		
		pointofmass = to_string(k);
		string grname = "gr" + pointofmass ;
		
		
		string DataPom = DataType + pointofmass + Date + ExtRoot;
		string PathPom = Path + DataPom;
		
		string FromList = Path + EffList + DataType + pointofmass + All + ExtTxt;
		cout << FromList << endl;
		ifstream ifile(FromList.c_str(), std::ios::in);
		
		
		if (!ifile.is_open()) {
			cout << "There was a problem opening the input file!" << FromList << endl;
	   	}
		else{

			double num;
			while (ifile >> num) {
				//cout << num << endl;
        			EffNotOrdered.push_back(num);
			}
	
			cout << " Eff size : " << EffNotOrdered.size() << endl;
			//myFileEff = new TFile(PathPom.c_str());
			int counter=1;
	
			


			for(int l = 0; l < AlltriggerNames.size(); l++){
				//Efficiencies[l] = new TH1D(AlltriggerNames[l].c_str(), AlltriggerNames[l].c_str(), 1200,1400,2600);
				//cout << "eff : " <<EffNotOrdered[l] << endl;
				//Efficiencies[l]->SetBinContent(BinCt,EffNotOrdered[l]);
				cout << counter << " ," << k << " ," << EffNotOrdered[l] <<  endl;
				Efficiencies2[l]->SetPoint(counter,k,EffNotOrdered[l]);
				counter+=1;
			}
			
			EffNotOrdered.clear();
			BinCt +=200;
		}
	
	}
	OutputHisto2->cd();
	for(int l = 0; l < Efficiencies2.size(); l++){
		Efficiencies2[l]->Write();
	}
	
	
	OutputHisto2->Close();
	TString filepath = "/home/raph/CMS/HSCPtriggerStudies/data/MergedMET/RENDU_5/SingleMuon/SingleMuon1105all.root";  ///home/raph/CMS/HSCPtriggerStudies/data/MergedMET/Cuts3/64-00.root
	
	
	myFile = new TFile(filepath);
	TH1D* HIST_MASSES = (TH1D*)gROOT->FindObject("MASS");
	HIST_MASSES->SetTitle("Invariant mass of candidates");







	outputfilename="/home/raph/CMS/HSCPtriggerStudies/data/MergedMET/RENDU_5/SingleMuon/HIST_SingleMuon.root";

	

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
	HIST_FITSUM->SetMarkerStyle(8);


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
	for(int x = 60; x < 70 ; x++){
		HIST_FITSIG->SetBinContent(x,0);
	}

	for(int x = 40 ; x < 50 ; x++){
		int randombg = rand()%(ValBins[valsize-1]-ValBins[0]+1)+ValBins[0];
		//cout << randombg << endl;
		nbentriesZ+=HIST_FITSIG->GetBinContent(x);
		HIST_FITBG->SetBinContent(x,0);
		//HIST_FITBG->SetBinContent(x,randombg);
		

	}
	for(int x = 0 ; x < 10 ; x++){
		HIST_FITBG->SetBinContent(x,0);
	}
	
	

	auto c1 = new TCanvas("c1","Fits for background and signal",1300,700);
	auto pad = new TPad("pad","",0,0,1,1);
	//pad->SetGrid();
	pad->Draw();
	pad->cd();

	int sizeparamgaus = 3,sizeparampol=2;
	double m[sizeparamgaus],m2[sizeparampol];
	

	TF1 *g2 = new TF1("g2", "pol1", 15,140);
	g2->SetLineColor(6);
	g2->SetLineStyle(1);
	g2->SetLineWidth(2);


	TF1 *g1 = new TF1("g1","gaus", 50,130);
	g1->SetLineColor(kRed);
	g1->SetLineStyle(1);
	g1->SetLineWidth(3);

	
	HIST_FITBG->Fit(g2,"R");
	HIST_FITSIG->Fit(g1,"R");


	HIST_FIT->Fit(g2,"R");
	HIST_FIT->Fit(g1,"R+");


	/*TF1 *g = (TF1*)HIST_FIT->GetListOfFunctions()->FindObject(g2);
	double IntegralBg = g2->Integral(86,96);
	TF1 *s = (TF1*)HIST_FIT->GetListOfFunctions()->FindObject(g1);
	double IntegralGauss = g1->Integral(86,96);*/
	
	for (int i =0 ; i < sizeparamgaus ; i++){
		m[i] = g1->GetParameter(i);
		cout << m[i] << endl;
	}
	for (int i =0 ; i < sizeparampol ; i++){
		m2[i] = g2->GetParameter(i);
		cout << m2[i] << endl;
	}
	
	

	TF1 *fitboth = new TF1("fits","gaus(0) +pol1(3)", 20,140);
	fitboth->SetLineColor(2);
	fitboth->SetLineWidth(1);
	fitboth->SetParameters(m[0],m[1],m[2],m2[0],m2[1]);
	HIST_FITSUM->Fit(fitboth,"R+");

	auto leg1 =new TLegend(0.13,0.45,0.48,0.65);
	
	leg1->SetHeader("Background and signal fits");
	leg1->AddEntry(fitboth,"Fit background + signal","l");
	//leg1->AddEntry(g1,"Signal : gaussian","l");
	//leg1->SetLegendFont(42);
	leg1->SetTextFont(42);
	leg1->SetTextSize(0.04);
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
	

	//------------------------------------------------------------------plot efficiency vs mass----------------------------------------






	//get the Tefficiency we want (string, all names = name of trigger)
	//TEfficiency hltmu_45->GetEfficiency(int bin), for a given bin.
	// If we have 4 different mass points, only 4 bins to consider ?

	//Get in those bins, then TGRaph, new TEfficiency -> SetEff in bin x with value obtained ?

	//
	






	/*cout << "Integral of the Signal for mass between [86-96] GeV: " << IntegralGauss << endl;
	cout << "Integral of the background for mass between [86-96] GeV = 0 : " << IntegralBg << endl;
	cout << "Ratio signal/total ¦ bins in 40-50 -> 0 = " << (IntegralGauss *1.0 / (IntegralGauss+IntegralBg))*100 << " %" << endl;
	
	cout << "There was " << nbentriesZ << " Z pair" << endl;



	InfosPurity << "Integral of the Signal for mass between [86-96] : " << IntegralGauss << " , Integral of the background for mass between [86-96] : " << IntegralBg << endl ;

	InfosPurity << "Ratio signal/total ¦ bins in 40-50 -> 0 = " << (IntegralGauss *1.0 / (IntegralGauss+IntegralBg))*100 << " %" << endl;
	
	InfosPurity << "There was " << nbentriesZ << " Z pair" << endl;*/

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


