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
#include <TGraphErrors.h>
#include <TMultiGraph.h>
#include <TLegendEntry.h>
#include <TFrame.h>
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
	string DataType = "Gluino";

	string ExtTxt = ".txt";
	
	string SubNum = "all";
	string EffList = "Eff";
	string Mass = "1600";
	string Date = "1105all";
	string pointofmass;
	string ExtRoot = ".root";
	string All = "all";
	string ErrorEff = "Error";
	string Path = "/home/raph/CMS/HSCPtriggerStudies/data/MergedMET/RENDU_5/" + DataType + "/Eff/";
	
	
	string OutPutName = Path + Purity + DataType + Date + SubNum + ExtTxt;


	InfosPurity.open (OutPutName.c_str());


	string s2 = "mu";
	string s4 = "Mu";
	string s5 = "Muon";
	string s7 = "MET";

	string NameList = "CompleteList";

	string NameListInterest = "ListeInteretTriggers";
	string CompleteNameListInterest = NameListInterest + DataType + Mass + ExtTxt;
//	string NameCompleteList = "CompleteListTest.txt";

	string NameCompleteListSingleMuon = NameList + DataType + ExtTxt;
	string NameCompleteList = NameList + DataType + Mass + ExtTxt;
	string PathEffFile = "/home/raph/CMS/HSCPtriggerStudies/data/MergedMET/RENDU_5/" + DataType + "/Eff/" + CompleteNameListInterest;
	

	vector<string> AlltriggerNames;
	vector<string> SubListMET;
	vector<string> SubListPT;
	
	
	
	cout << PathEffFile << endl;
	ifstream EfficiencyFile(PathEffFile.c_str());
	
	if(!EfficiencyFile){
		cout  << " cannot open file"  << endl;
	}

	string tmpp;

	while(std::getline(EfficiencyFile,tmpp)){
		AlltriggerNames.push_back(tmpp);
		cout  << tmpp  << endl;
	}

	EfficiencyFile.close();
	cout << "size : " << AlltriggerNames.size() << endl;


	outputfilename2=Path + "HIST_" + EffList + DataType + ExtRoot;

	
	OutputHisto2 = new TFile(outputfilename2,"RECREATE");

	TCanvas *c111 = new TCanvas("c111","c111",200,10,700,500);
	//c111->SetFillColor(42);
	c111->SetTitle("Efficiencies depending on mass");

	TH2F *hr2 = new TH2F("hr2","Efficiency of triggers for gluino",800,1400,2600,100,0,1);
	hr2->SetXTitle("HSCP Mass [GeV]");
	hr2->SetYTitle("#epsilon");
	hr2->SetStats(kFALSE);
	hr2->Draw();

	c111->GetFrame()->SetBorderSize(12);



	hr.resize(AlltriggerNames.size());
	/*for(int l = 0; l < AlltriggerNames.size(); l++){
		hr[l] = new TH2F;
		hr[l]->SetTitle(AlltriggerNames[l].c_str());
		hr[l]->SetXTitle("Mass [GeV]");
		hr[l]->SetYTitle("Efficiency (%)");
		hr[l]->SetName(AlltriggerNames[l].c_str());
		hr[l]->Draw();
	}	*/
	



	int kmin = 1600,kmax=2600,kincre = 200;
		
	int n1 = ((kmax-kmin)/kincre)+1 ;
	double x0[n1],y0[n1],x1[n1],y1[n1],x2[n1],y2[n1],x3[n1],y3[n1],x4[n1],y4[n1],x5[n1],y5[n1],x6[n1],y6[n1];
	double Errx0[n1] = {0},Erry0[n1] = {0},Errx1[n1] = {0},Erry1[n1]= {0},Errx2[n1]= {0},Erry2[n1]= {0},Errx3[n1]= {0},Erry3[n1]= {0},Errx4[n1]= {0},Erry4[n1]= {0},Errx5[n1]= {0},Erry5[n1]= {0},Errx6[n1]= {0},Erry6[n1]= {0};

	Efficiencies2.resize(AlltriggerNames.size());
	int actualbin = 0;
	int actualbinerror = 0;
	for(int k = kmin; k <= kmax ; k+=kincre){
		vector<double> EffNotOrdered;
		vector<double> ErrorEffNotOrdered;
		cout << " Number of mass points : " << n1 << endl;


		pointofmass = to_string(k);
		string DataPom = DataType + pointofmass + Date + ExtRoot;
		string PathPom = Path + DataPom;
		
		string FromList = Path + EffList + DataType + pointofmass + All + ExtTxt;

		cout << FromList << endl;


		string FromListError = Path + ErrorEff + EffList + DataType + pointofmass + All + ExtTxt;
		



		ifstream afile(FromListError.c_str(), std::ios::in);





		if (!afile.is_open()) {
			cout << "There was a problem opening the error input file!" << FromListError << endl;
	   	}
		else{
			double num2;
			while (afile >> num2) {
				//cout << num << endl;
        			ErrorEffNotOrdered.push_back(num2);
			}
			afile.close();
			cout << "Error Eff size : " << ErrorEffNotOrdered.size() << endl;

			
			for(int l = 0; l < AlltriggerNames.size(); l++){
				cout << actualbinerror << endl;
				if(l==0){
					
					Erry0[actualbinerror] = (ErrorEffNotOrdered[l]*1.0/100);

					cout << "Err x0 and y0 = " << Errx0[actualbinerror] <<  " " << Erry0[actualbinerror] << endl;
				}
				if(l==1){
					
					Erry1[actualbinerror] = (ErrorEffNotOrdered[l]*1.0/100);
					cout << "Err x1 and y1 = " << Errx1[actualbinerror] << " " << Erry1[actualbinerror] << endl;
				}

				if(l==2){
					
					Erry2[actualbinerror] = (ErrorEffNotOrdered[l]*1.0/100);
					cout << "Err x2 and y2 = " << Errx2[actualbinerror] <<" " <<  Erry2[actualbinerror] << endl;
				}

				if(l==3){
					
					
					Erry3[actualbinerror] = (ErrorEffNotOrdered[l]*1.0/100);
					cout << "Err x3 and y3 = " << Errx3[actualbinerror] << " " << Erry3[actualbinerror] << endl;

				}

				if(l==4){
					
					Erry4[actualbinerror] = (ErrorEffNotOrdered[l]*1.0/100);
					cout << "Err x4 and y4 = " << Errx4[actualbinerror] << " " << Erry4[actualbinerror] << endl;

				}
				if(l==5){	
					
					Erry5[actualbinerror] = (ErrorEffNotOrdered[l]*1.0/100);
					cout << "Err x5 and y5 = " << Errx5[actualbinerror] << " " << Erry5[actualbinerror] << endl;
				}
				if(l==6){	
					
					Erry6[actualbinerror] = (ErrorEffNotOrdered[l]*1.0/100);
					cout << "Err x6 and y6 = " << Errx6[actualbinerror] << " " << Erry6[actualbinerror] << endl;
				}
				
			}
			actualbinerror+=1;

		}




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
			ifile.close();
			cout << " Eff size : " << EffNotOrdered.size() << endl;

			
			for(int l = 0; l < AlltriggerNames.size(); l++){
				cout << actualbin << endl;
				if(l==0){
					x0[actualbin] = k;
					y0[actualbin] = (EffNotOrdered[l]*1.0/100);

					cout << "x0 and y0 = " << x0[actualbin] <<  " " << y0[actualbin] << endl;
				}
				if(l==1){
					x1[actualbin] = k;
					y1[actualbin] = (EffNotOrdered[l]*1.0/100);
					cout << "x1 and y1 = " << x1[actualbin] << " " << y1[actualbin] << endl;
				}

				if(l==2){
					x2[actualbin] = k;
					y2[actualbin] = (EffNotOrdered[l]*1.0/100);
					cout << "x2 and y2 = " << x2[actualbin] <<" " <<  y2[actualbin] << endl;
				}

				if(l==3){
					
					x3[actualbin] = k;
					y3[actualbin] = (EffNotOrdered[l]*1.0/100);
					cout << "x3 and y3 = " << x3[actualbin] << " " << y3[actualbin] << endl;

				}

				if(l==4){
					x4[actualbin] = k;
					y4[actualbin] = (EffNotOrdered[l]*1.0/100);
					cout << "x4 and y4 = " << x4[actualbin] << " " << y4[actualbin] << endl;

				}
				if(l==5){	
					x5[actualbin] = k;
					y5[actualbin] = (EffNotOrdered[l]*1.0/100);
					cout << "x5 and y5 = " << x5[actualbin] << " " << y5[actualbin] << endl;
				}
				if(l==6){	
					x6[actualbin] = k;
					y6[actualbin] = (EffNotOrdered[l]*1.0/100);
					cout << "x6 and y6 = " << x6[actualbin] << " " << y6[actualbin] << endl;
				}
				
			}
			actualbin+=1;
			
		}
		EffNotOrdered.clear();
	}

			TLegend* leg9 = new TLegend(0.9,0.85,0.5,0.6);
			
			OutputHisto2->cd();
			c111->cd();
			cout << "AlltriggerNames.size() = " << AlltriggerNames.size() << endl;
			for(int l = 0; l < AlltriggerNames.size(); l++){
				if(l==0){
					Efficiencies2[l] = new TGraphErrors(actualbin, x0,y0,Errx0,Erry0);
					
					Efficiencies2[l]->SetMarkerColor(kRed);
					Efficiencies2[l]->SetMarkerStyle(23);
					Efficiencies2[l]->SetMarkerSize(1);
					Efficiencies2[l]->Draw("P");
					leg9->AddEntry(Efficiencies2[l], "HLT_Mu50_v11", "p");
					c111->Modified();
					c111->Update();
				}
				if(l==1){
					Efficiencies2[l] = new TGraphErrors(actualbin, x1,y1,Errx1,Erry1);
					Efficiencies2[l]->SetMarkerColor(kBlue);
					Efficiencies2[l]->SetMarkerStyle(24);
					Efficiencies2[l]->SetMarkerSize(1);
					Efficiencies2[l]->Draw("P");
					leg9->AddEntry(Efficiencies2[l], "HLT_IsoMu20_v12", "p");
					c111->Modified();
					c111->Update();
				}
				if(l==2){
					Efficiencies2[l] = new TGraphErrors(actualbin, x2,y2,Errx2,Erry2);
					Efficiencies2[l]->SetMarkerColor(kOrange);

					Efficiencies2[l]->SetLineColor(kOrange);

					Efficiencies2[l]->SetMarkerStyle(22);
					Efficiencies2[l]->SetMarkerSize(1);
					Efficiencies2[l]->Draw("P");
					leg9->AddEntry(Efficiencies2[l], "HLT_PFHT1050_v14", "p");
					c111->Modified();
					c111->Update();
				}
				if(l==3){
					Efficiencies2[l] = new TGraphErrors(actualbin,x3,y3,Errx3,Erry3);
					Efficiencies2[l]->SetMarkerColor(kBlack);
					Efficiencies2[l]->SetMarkerStyle(20);
					Efficiencies2[l]->SetMarkerSize(1);
					Efficiencies2[l]->Draw("P");
					leg9->AddEntry(Efficiencies2[l], "HLT_PFHT500_PFMET100_PFMHT100_IDTight_v8", "p");
					c111->Modified();
					c111->Update();
				}	
				if(l==4){
					Efficiencies2[l] = new TGraphErrors(actualbin, x4,y4,Errx4,Erry4);
					Efficiencies2[l]->SetMarkerColor(kBlack);
					Efficiencies2[l]->SetMarkerStyle(21);
					Efficiencies2[l]->SetMarkerSize(1);
					Efficiencies2[l]->Draw("P");
					leg9->AddEntry(Efficiencies2[l], "HLT_PFMET120_PFMHT120_IDTight_v16", "p");
					c111->Modified();
					c111->Update();
				}
				if(l==5){
					Efficiencies2[l] = new TGraphErrors(actualbin, x5,y5,Errx5,Erry5);
					Efficiencies2[l]->SetMarkerColor(kMagenta);
					Efficiencies2[l]->SetMarkerStyle(33);
					Efficiencies2[l]->SetMarkerSize(1);
					Efficiencies2[l]->Draw("P");
					leg9->AddEntry(Efficiencies2[l], "HLT_CaloMET70_HBHECleaned_v3", "p");
					c111->Modified();
					c111->Update();
				}
				if(l==6){
					cout << "We are in case l == 6" << endl;
					Efficiencies2[l] = new TGraphErrors(actualbin, x6,y6,Errx6,Erry6);
					Efficiencies2[l]->SetMarkerColor(kBlack);
					Efficiencies2[l]->SetMarkerStyle(22);
					Efficiencies2[l]->SetMarkerSize(1);
					Efficiencies2[l]->Draw("P");
					leg9->AddEntry(Efficiencies2[l], "HLT_MonoCentralPFJet80_PFMETNoMu120_PFMHTNoMu120_IDTight_v16", "p");
					c111->Modified();
					c111->Update();
				}


			}
		x1[n1] = {0};
		y1[n1] = {0};

		x2[n1] = {0};
		y2[n1] = {0};
		
		x3[n1] = {0};
		y3[n1] = {0};

		x4[n1] = {0};
		y4[n1] = {0};

		x5[n1] = {0};
		y5[n1] = {0};

		x6[n1] = {0};
		y6[n1] = {0};



		leg9->Draw();
			
			
		
		/*x0.clear();
		y0.clear();
		x1.clear();
		y1.clear();
		x2.clear();
		y2.clear();
		x3.clear();
		y3.clear();
		x4.clear();
		y4.clear();
		x5.clear();
		y5.clear();*/
	
	


	c111->Write();
	
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


