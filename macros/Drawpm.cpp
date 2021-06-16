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
#include <TPaveStats.h>
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
	string filepath = "/home/raph/CMS/HSCPtriggerStudies/data/MergedMET/RENDU_5/Gluino/Recons/Eff/";
	string pointofmass,DataType="Gluino",Date = "1105", All = "all", ExtRoot = ".root",Distribz = "DistribZpeak";
	int minm=1800,maxm=2400,nbmbin=200, nbbing = 80;
	double x0[nbbing] = {0} ,y0[nbbing]= {0},x1[nbbing]= {0},y1[nbbing]= {0},x2[nbbing]= {0},y2[nbbing]= {0},x3[nbbing]= {0},y3[nbbing]= {0},x4[nbbing]= {0},y4[nbbing]= {0},x5[nbbing]= {0},y5[nbbing]= {0};


	int nbdiffpt = ((maxm- minm)/nbmbin) +1;
	cout << nbdiffpt << endl;
	AllFiles.resize(nbdiffpt);
	TempTr.resize(nbdiffpt);
	MyMyF.resize(nbdiffpt);
	MyTf1.resize(nbdiffpt);
	Test.resize(nbdiffpt);
	TString outputfilename="/home/raph/CMS/HSCPtriggerStudies/data/MergedMET/RENDU_5/Gluino/Recons/Eff/Povermwithmass.root";
	
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
			cout << nbcount << endl;
			TempTr[nbcount] = (TH1D*)gROOT->FindObject("DISTRIB_POVERMASSO1");
			TempTr[nbcount]->SetTitle(Title.c_str());
			cout << "all good" << endl;
		
		}

	nbcount+=1;
	}
	cout << "before for bins" << endl;
	for ( int i = 0; i< nbbing ; i++){
			cout << "in loop nb : " << i << endl;
			int s = TempTr[0]->GetBinContent(i);
			//cout << s << endl;
			x0[i] = i*0.03125;
			y0[i] = s;
		
			int s1 = TempTr[1]->GetBinContent(i);
			//cout << s1 << endl;
			x1[i] = i*0.03125;
			y1[i] = s1;

			int s2 = TempTr[2]->GetBinContent(i);
			//cout << s2 << endl;
			x2[i] = i*0.03125;
			y2[i] = s2;

			int s3 = TempTr[3]->GetBinContent(i);
			//cout << s3 << endl;
			x3[i] = i*0.03125;
			y3[i] = s3;

			/*int s4 = TempTr[4]->GetBinContent(i);
			//cout << s4 << endl;
			x4[i] = i*0.03125;
			y4[i] = s4;

			int s5 = TempTr[5]->GetBinContent(i);
			//cout << s5 << endl;
			x5[i] = i*0.03125;
			y5[i] = s5;*/
			
	}
	

	auto c1 = new TCanvas("c1","Ratio p/m",1300,700);
	c1->SetTitle("Ratio impulsion / Mass");
	TLegend* leg1 = new TLegend(0.7, 0.8, .5, .6);
	for(int j = 0; j < nbdiffpt ; j++){
		string nametf1 = "g" + to_string(j);
		MyTf1[j] = new TF1(nametf1.c_str(), "landau", 0, 2.5);
		if(j==0){
			MyTf1[j]->SetLineColor(9);
		}
		if(j==1){
			MyTf1[j]->SetLineColor(8);
		}
		if(j==2){
			MyTf1[j]->SetLineColor(7);
		}
		if(j==3){
			MyTf1[j]->SetLineColor(6);
		}
		if(j==4){
			MyTf1[j]->SetLineColor(5);
		}
		if(j==5){
			MyTf1[j]->SetLineColor(2);
		}








	}
	TMultiGraph *mg = new TMultiGraph();


	Test[0] = new TGraphErrors(80, x0,y0,0,0);
	
	Test[0]->SetLineColor(9);
	Test[0]->SetLineStyle(1);
	Test[0]->SetLineWidth(2);
	//Test[0]->Fit(MyTf1[0],"q");
	Test[0]->SetMarkerColor(9);
   	Test[0]->SetMarkerStyle(20);
	Test[0]->SetMarkerSize(0);

	leg1->AddEntry(Test[0],"mass = 1600 GeV/c^{2}");
	mg->Add(Test[0]);
	c1->Modified();
	c1->Update();

	Test[1] = new TGraphErrors(80, x1,y1,0,0);

	Test[1]->SetLineColor(8);
	Test[1]->SetLineStyle(1);
	Test[1]->SetLineWidth(2);
	//Test[1]->Fit(MyTf1[1],"q");
	Test[1]->SetMarkerColor(8);
   	Test[1]->SetMarkerStyle(20);
	Test[1]->SetMarkerSize(0);
   	
	leg1->AddEntry(Test[1],"mass = 1800 GeV/c^{2}");
	mg->Add(Test[1]);
	c1->Modified();
	c1->Update();
	
	Test[2] = new TGraphErrors(80, x2,y2,0,0);
	
	Test[2]->SetLineColor(7);
	Test[2]->SetLineStyle(1);
	Test[2]->SetLineWidth(2);
	//Test[2]->Fit(MyTf1[2],"q");
	Test[2]->SetMarkerColor(7);
   	Test[2]->SetMarkerStyle(20);
	Test[2]->SetMarkerSize(0);
	
	leg1->AddEntry(Test[2],"mass = 2000 GeV/c^{2}");
	mg->Add(Test[2]);
	c1->Modified();
	c1->Update();

	Test[3] = new TGraphErrors(80, x3,y3,0,0);

	Test[3]->SetLineColor(6);
	Test[3]->SetLineStyle(1);
	Test[3]->SetLineWidth(2);
	//Test[3]->Fit(MyTf1[3],"q");
	Test[3]->SetMarkerColor(6);
   	Test[3]->SetMarkerStyle(20);
	Test[3]->SetMarkerSize(0);
	
	leg1->AddEntry(Test[3],"mass = 2200 GeV/c^{2}");
	mg->Add(Test[3]);
	c1->Modified();
	c1->Update();
	
	Test[4] = new TGraphErrors(80, x4,y4,0,0);

	Test[4]->SetLineColor(5);
	Test[4]->SetLineStyle(1);
	Test[4]->SetLineWidth(2);
	//Test[4]->Fit(MyTf1[4],"q");
	Test[4]->SetMarkerColor(5);
   	Test[4]->SetMarkerStyle(20);
	Test[4]->SetMarkerSize(0);
	
	leg1->AddEntry(Test[4],"mass = 2400 GeV/c^{2}");
	mg->Add(Test[4]);
	c1->Modified();
	c1->Update();

	
	/*Test[5] = new TGraphErrors(80, x5,y5,0,0);

	Test[5]->SetLineColor(2);
	Test[5]->SetLineStyle(1);
	Test[5]->SetLineWidth(2);
	Test[5]->Fit(MyTf1[5],"q");
	Test[5]->SetMarkerColor(2);
   	Test[5]->SetMarkerStyle(20);
	Test[5]->SetMarkerSize(0);
	
	leg1->AddEntry(Test[5],"mass = 2600 GeV/c^{2}");
	mg->Add(Test[5]);
	c1->Modified();
	c1->Update();*/



	

	
	mg->Draw("a");
	cout << "After Draw" << endl;
	c1->Update();
	/*auto stats1 = (TPaveStats*)Test[0]->GetListOfFunctions()->FindObject("stats");
   	auto stats2 = (TPaveStats*)Test[1]->GetListOfFunctions()->FindObject("stats");
	auto stats3 = (TPaveStats*)Test[2]->GetListOfFunctions()->FindObject("stats");
	auto stats4 = (TPaveStats*)Test[3]->GetListOfFunctions()->FindObject("stats");
	auto stats5 = (TPaveStats*)Test[4]->GetListOfFunctions()->FindObject("stats");*/
	cout << " After declaring TPaveStats" << endl;
  	/*stats1->SetTextColor(kBlue);
   	stats2->SetTextColor(kGreen);
	stats3->SetTextColor(kAzure);
	stats4->SetTextColor(kViolet);
	stats5->SetTextColor(kYellow);*/
	cout << "Before modified" << endl;
	/*stats1->SetX1NDC(0.12);
	stats1->SetX2NDC(0.92); 
	stats1->SetY1NDC(0.75);
	stats2->SetX1NDC(0.32);
	stats2->SetX2NDC(0.92);
	stats2->SetY1NDC(0.78);
	stats3->SetX1NDC(0.52);
	stats3->SetX2NDC(0.92); 
	stats3->SetY1NDC(0.78);
	stats4->SetX1NDC(0.72);
	stats4->SetX2NDC(0.92);
	stats4->SetY1NDC(0.78);
	stats5->SetX1NDC(0.92);
	stats5->SetX2NDC(0.92);
	stats5->SetY1NDC(0.78);*/
	c1->Modified();





	mg->GetXaxis()->SetTitle("Ratio p/m");
	mg->GetYaxis()->SetTitle("# HSCP");
	mg->GetYaxis()->SetRange(0,1000);
	mg->GetHistogram()->SetTitle("Ratio p/m de Gluinos pour differentes masses");
	//mg->Fit("landau","FQ");
	
	
	/*auto pad = new TPad("pad","",0,0,1,1);
	pad->Draw();
	pad->cd();*/
	
	/*TH1D *hr = new TH1D("hr","P/m for different masses",100,0,3);
	hr->SetXTitle("P/M");
	hr->SetYTitle("# Gluino");
	hr->SetStats(kFALSE);
	hr->Draw();*/
	c1->cd();
	c1->GetFrame()->SetBorderSize(12);
	
	/*for(int j = 0; j < nbdiffpt ; j++){
		string nametf1 = "g" + to_string(j);
		MyTf1[j] = new TF1(nametf1.c_str(), "landau", 0, 3);
		cout << "after new tf1" << endl;
		//MyTf1[]->SetName(
		if(j==0){
			MyTf1[j]->SetLineColor(9);
			MyTf1[j]->SetLineStyle(1);
			MyTf1[j]->SetLineWidth(2);
			//MyTf1[j]->Draw();
			TempTr[j]->Fit(MyTf1[j],"R");
			TempTr[j]->Draw();
			leg1->AddEntry(MyTf1[j],"mass = 1600 GeV/c^{2}");
			c1->Modified();
			c1->Update();
		}
		if(j==1){
			MyTf1[j]->SetLineColor(8);
			MyTf1[j]->SetLineStyle(1);
			MyTf1[j]->SetLineWidth(2);
			//MyTf1[j]->Draw();
			TempTr[j]->Fit(MyTf1[j],"R+");
			
			TempTr[j]->Draw("SAME");
			leg1->AddEntry(MyTf1[j],"mass = 1800 GeV/c^{2}");		
			c1->Modified();
			c1->Update();
		}
		if(j==2){
			MyTf1[j]->SetLineColor(7);
			MyTf1[j]->SetLineStyle(1);
			MyTf1[j]->SetLineWidth(2);
			TempTr[j]->Fit(MyTf1[j],"R+");
			TempTr[j]->Draw("SAME");
			leg1->AddEntry(MyTf1[j],"mass = 2000 GeV/c^{2}");		
			c1->Modified();
			c1->Update();
			//MyTf1[j]->Draw();
			//c1->Modified();
			//c1->Update();
		}
		if(j==3){
			MyTf1[j]->SetLineColor(6);
			MyTf1[j]->SetLineStyle(1);
			MyTf1[j]->SetLineWidth(2);
			//MyTf1[j]->Draw();
			TempTr[j]->Fit(MyTf1[j],"R+");
			TempTr[j]->Draw("SAME");
			leg1->AddEntry(MyTf1[j],"mass = 2200 GeV/c^{2}");
			c1->Modified();
			c1->Update();
		}
		if(j==4){
			MyTf1[j]->SetLineColor(5);
			MyTf1[j]->SetLineStyle(1);
			MyTf1[j]->SetLineWidth(2);
			//MyTf1[j]->Draw();
			TempTr[j]->Fit(MyTf1[j],"R+");
			TempTr[j]->Draw("SAME");
			leg1->AddEntry(MyTf1[j],"mass = 2400 GeV/c^{2}");
			c1->Modified();
			c1->Update();
		}
		if(j==5){
			MyTf1[j]->SetLineColor(4);
			MyTf1[j]->SetLineStyle(1);
			MyTf1[j]->SetLineWidth(2);
			TempTr[j]->Fit(MyTf1[j],"R+");
			TempTr[j]->Draw("SAME	");
			leg1->AddEntry(MyTf1[j],"mass = 2600 GeV/c^{2}" );
			//MyTf1[j]->Draw();
			c1->Modified();
			c1->Update();
		}
		

		
		//hr->Fit(MyTf1[0],"R");

		//TempTr[j]->Fit(MyTf1[j],"SAME+");
		
		//c1->Modified();
		//c1->Update();

	
	}*/

	leg1->SetBorderSize(0);
	leg1->Draw();

	
	OutputHisto->cd();
	
	c1->Write();
	OutputHisto->Close();
}




int main(){

	Drawpm h;
	h.FitSignalPM();

}


