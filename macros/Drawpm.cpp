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
	string filepath = "/home/raph/CMS/HSCPtriggerStudies/data/MergedMET/RENDU_5/Gluino/ReconsNosel/Eff/";
	string filepathsel = "/home/raph/CMS/HSCPtriggerStudies/data/MergedMET/RENDU_5/Gluino/ReconsPresel/Eff/";
	string pointofmass,pointofmasssel,DataType="Gluino",Date = "1105", All = "all", ExtRoot = ".root",Distribz = "DistribZpeak";
	int minm=1800,maxm=2600,nbmbin=200, nbbing = 80;
	double x0[nbbing] = {0} ,y0[nbbing]= {0},x1[nbbing]= {0},y1[nbbing]= {0},x2[nbbing]= {0},y2[nbbing]= {0},x3[nbbing]= {0},y3[nbbing]= {0},x4[nbbing]= {0},y4[nbbing]= {0},x5[nbbing]= {0},y5[nbbing]= {0};

	double x0pre[nbbing] = {0} ,y0pre[nbbing]= {0},x1pre[nbbing]= {0},y1pre[nbbing]= {0},x2pre[nbbing]= {0},y2pre[nbbing]= {0},x3pre[nbbing]= {0},y3pre[nbbing]= {0},x4pre[nbbing]= {0},y4pre[nbbing]= {0},x5pre[nbbing]= {0},y5pre[nbbing]= {0};


	double x0err[nbbing] = {0} ,y0err[nbbing]= {0},x1err[nbbing]= {0},y1err[nbbing]= {0},x2err[nbbing]= {0},y2err[nbbing]= {0},x3err[nbbing]= {0},y3err[nbbing]= {0},x4err[nbbing]= {0},y4err[nbbing]= {0},x5err[nbbing]= {0},y5err[nbbing]= {0};

	double x0preerr[nbbing] = {0} ,y0preerr[nbbing]= {0},x1preerr[nbbing]= {0},y1preerr[nbbing]= {0},x2preerr[nbbing]= {0},y2preerr[nbbing]= {0},x3preerr[nbbing]= {0},y3preerr[nbbing]= {0},x4preerr[nbbing]= {0},y4preerr[nbbing]= {0},x5preerr[nbbing]= {0},y5preerr[nbbing]= {0};

	
	for (int a =0 ; a < 2; a++){
		//propagation des incertutes
		
	
		
	}





	int nbdiffpt = ((maxm- minm)/nbmbin) +1;
	cout << nbdiffpt << endl;
	AllFiles.resize(nbdiffpt);
	TempTr.resize(nbdiffpt);
	TempTr2.resize(nbdiffpt);
	MyMyF.resize(nbdiffpt);
	MyMyF2.resize(nbdiffpt);
	MyTf1.resize(nbdiffpt);
	Test.resize(nbdiffpt);
	Test2.resize(nbdiffpt);
	TString outputfilename="/home/raph/CMS/HSCPtriggerStudies/data/MergedMET/RENDU_5/Gluino/Ratio_eff/Povermwithmass.root";
	

	OutputHisto = new TFile(outputfilename,"RECREATE");

	


	int nbcount = 0;
	for(int i = minm; i <= maxm ; i+=nbmbin){
		
		pointofmass = to_string(i);
		string PathPom = filepath + Distribz  + DataType + pointofmass + Date + All + ExtRoot;
		string Title = "Ratio p/m distribution for no selection" + DataType + pointofmass;
		cout << PathPom << endl;
		string PathPomSel = filepathsel + Distribz  + DataType + pointofmass + Date + All + ExtRoot;
		string TitleSel = "Ratio p/m distribution for Preselection" + DataType + pointofmass;

		MyMyF[nbcount] = new TFile(PathPom.c_str());
		MyMyF[nbcount]->cd();
		if (!MyMyF[nbcount] || !MyMyF[nbcount]->IsOpen()) {
			cout << "There was a problem opening the input file!" << PathPom << endl;
		}
		
		else{
			cout << nbcount << endl;
			TempTr[nbcount] = (TH1D*)gROOT->FindObject("DISTRIB_POVERMASSO1");
			Double_t scale = (1.0/TempTr[nbcount]->Integral());
			cout << "scale factor 1 : " << scale << " , and integral 1 : " << TempTr[nbcount]->Integral() <<  endl;
			TempTr[nbcount]->Scale(scale);
			Double_t intbnosel = TempTr[nbcount]->Integral(10,30);
			Double_t intb2nosel = TempTr[nbcount]->Integral();

			cout << " No sel, point de masse " << i << "  : " << intbnosel << " / " << intb2nosel << " = " << intbnosel*1.0/intb2nosel << endl;

			TempTr[nbcount]->SetTitle(Title.c_str());
			cout << "all good" << endl;
		
		}
		cout << PathPomSel << endl;
		MyMyF2[nbcount] = new TFile(PathPomSel.c_str());
		if (!MyMyF2[nbcount] || !MyMyF2[nbcount]->IsOpen()) {
			cout << "There was a problem opening the input file!" << PathPomSel << endl;
		}
		else{
			cout << nbcount << endl;
			TempTr2[nbcount] = (TH1D*)gROOT->FindObject("DISTRIB_POVERMASSO1");
			Double_t scale2 = (1/TempTr2[nbcount]->Integral());
			cout << "scale factor 2 : " << scale2 << " , and integral 2 : " << TempTr2[nbcount]->Integral() << endl;
			TempTr2[nbcount]->Scale(scale2);
			
			Double_t intb = TempTr2[nbcount]->Integral(10,30);
			Double_t intb2 = TempTr2[nbcount]->Integral();

			cout << " Presel, point de masse " << i << "  : " << intb << " / " << intb2 << " = " << intb*1.0/intb2 << endl;

			TempTr2[nbcount]->SetTitle(TitleSel.c_str());
			cout << "all good" << endl;
		}

		//MyMyF[nbcount]->Close();
		nbcount+=1;
	}
	

	double moy=0,moy1=0,moy2=0,moy3=0,moy4=0,moy5=0,moypre=0,moypre1=0,moypre2=0,moypre3=0,moypre4=0,moypre5=0;
	double sumy=0,sumy1=0,sumy2=0,sumy3=0,sumy4=0,sumypre=0,sumypre1=0,sumypre2=0,sumypre3=0,sumypre4=0;
	//cout << "before for bins" << endl;
	double trialint=0,trialint2=0;
	cout << " For mass = 1800 : " << endl;
	for ( int i = 0; i< nbbing ; i++){
			//cout << "in loop nb : " << i << endl;
			double s = TempTr[0]->GetBinContent(i);
			trialint+=s;
			x0[i] = (i*0.03125)-0.02;
			y0[i] = s;
			//cout << "no sel"  <<x0[i] << "," << y0[i] << endl;
			moy += x0[i] * y0[i];

			//cout << s << " , " << i*0.03125 << endl;
			sumy += y0[i];
			double s1 = TempTr[1]->GetBinContent(i);
			//cout << s1 << endl;
			x1[i] = (i*0.03125) -0.02; 
			y1[i] = s1;
			moy1 += x1[i] * y1[i];
			sumy1 += y1[i];
			double s2 = TempTr[2]->GetBinContent(i);

			//cout << s2 << endl;
			x2[i] = (i*0.03125) -0.02;
			y2[i] = s2;
			moy2 += x2[i] * y2[i];
			sumy2 += y2[i];
			double s3 = TempTr[3]->GetBinContent(i);

			//cout << s3 << endl;
			x3[i] = (i*0.03125) -0.02;
			y3[i] = s3;
			moy3 += x3[i] * y3[i];
			sumy3 += y3[i];
			double s4 = TempTr[4]->GetBinContent(i);

			//cout << s4 << endl;
			x4[i] = (i*0.03125) -0.02;
			y4[i] = s4;
			moy4 += x4[i] * y4[i];
			sumy4 += y4[i];
			/*int s5 = TempTr[5]->GetBinContent(i);

			//cout << s5 << endl;
			x5[i] = i*0.03125;
			y5[i] = s5;*/
			
			double spre = TempTr2[0]->GetBinContent(i);
			
			moypre += spre * ((i*0.03125) -0.02);
			sumypre += spre;
			x0pre[i] = (i*0.03125) -0.02;
			y0pre[i] = spre;
			//cout << "presel "  << x0pre[i] << "," << y0pre[i] << endl;
			trialint2 += spre;

			double s1pre = TempTr2[1]->GetBinContent(i);
			moypre1 += s1pre * ((i*0.03125) -0.02);
			sumypre1 += s1pre;
			x1pre[i] = (i*0.03125) -0.02;
			y1pre[i] = s1pre;


			double s2pre = TempTr2[2]->GetBinContent(i);
			moypre2 += s2pre * ((i*0.03125) -0.02);
			sumypre2 += s2pre;
			x2pre[i] = (i*0.03125) -0.02;
			y2pre[i] = s2pre;


			double s3pre = TempTr2[3]->GetBinContent(i);
			moypre3 += s3pre * ((i*0.03125) -0.02);
			sumypre3 += s3pre;
			x3pre[i] = (i*0.03125) -0.02;
			y3pre[i] = s3pre;

			double s4pre = TempTr2[4]->GetBinContent(i);
			moypre4 += s4pre * ((i*0.03125) -0.02);
			sumypre4 += s4pre;
			x4pre[i] = (i*0.03125) -0.02;
			y4pre[i] = s4pre;


	}
	double effective_moy = moy*1.0/sumy;
	double effective_moypresel = moypre*1.0/sumypre;
	double transfereff=0;
	for ( int i = 0; i< nbbing ; i++){
		transfereff += (x0[i] - effective_moy) * (x0[i] - effective_moy);

	}

	double sigma = sqrt((1.0/nbbing) * transfereff );
	cout << sigma << endl;
	//cout << "my integral of sel and no sel : " << trialint << " and " << trialint2 << endl;

	cout << " No selection : " << " moyenne 1800 = " << effective_moy << " moyenne 2000 = " << moy1*1.0/sumy1 << " moyenne 2200 = " << moy2*1.0/sumy2 << " moyenne 2400 = " << moy3*1.0/sumy3 << " moyenne 2600 = " << moy4*1.0/sumy4 << endl;

	cout << " Presel : " << " moyenne 1800 = " << effective_moypresel << " moyenne 2000 = " << moypre1*1.0/sumypre1 << " moyenne 2200 = " << moypre2*1.0/sumypre2 << " moyenne 2400 = " << moypre3*1.0/sumypre3 << " moyenne 2600 = " << moypre4*1.0/sumypre4 << endl;

	auto c2 = new TCanvas("c2","Ratio p/m",1300,700);
	c2->SetTitle("Ratio impulsion / Mass");
	TLegend* leg2 = new TLegend(0.7, 0.8, .5, .6);

	TMultiGraph *mg2 = new TMultiGraph();



	Test2[0] = new TGraphErrors(80, x0,y0);
	
	Test2[0]->SetLineColor(9);
	Test2[0]->SetLineStyle(1);
	Test2[0]->SetLineWidth(1);
	//Test[0]->Fit(MyTf1[0],"q");
	Test2[0]->SetMarkerColor(9);
   	Test2[0]->SetMarkerStyle(49);
	Test2[0]->SetMarkerSize(2);

	leg2->AddEntry(Test2[0],"No selection, mass = 1800 GeV/c^{2}");
	mg2->Add(Test2[0],"lp");
	c2->Modified();
	c2->Update();

	Test2[1] = new TGraphErrors(80, x0pre,y0pre);
	Test2[1]->SetLineColor(8);
	Test2[1]->SetLineStyle(1);
	Test2[1]->SetLineWidth(1);
	//Test[0]->Fit(MyTf1[0],"q");
	Test2[1]->SetMarkerColor(8);
   	Test2[1]->SetMarkerStyle(49);
	Test2[1]->SetMarkerSize(2);

	leg2->AddEntry(Test2[1]," Preselection, mass = 1800 GeV/c^{2}");
	mg2->Add(Test2[1],"lp");
	c2->Modified();
	c2->Update();

	mg2->Draw("a");
	c2->Update();
	c2->Modified();
	mg2->GetXaxis()->SetTitle("Ratio p/m");
	mg2->GetYaxis()->SetTitle("# HSCP");
	//mg2->GetYaxis()->SetRange(0,1.1);
	mg2->GetHistogram()->SetTitle("Ratio p/m de Gluinos pour differentes selections");



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

	leg1->AddEntry(Test[0],"mass = 1800 GeV/c^{2}");
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
   	
	leg1->AddEntry(Test[1],"mass = 2000 GeV/c^{2}");
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
	
	leg1->AddEntry(Test[2],"mass = 2200 GeV/c^{2}");
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
	
	leg1->AddEntry(Test[3],"mass = 2400 GeV/c^{2}");
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
	
	leg1->AddEntry(Test[4],"mass = 2600 GeV/c^{2}");
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
	c1->Update();
	/*auto stats1 = (TPaveStats*)Test[0]->GetListOfFunctions()->FindObject("stats");
   	auto stats2 = (TPaveStats*)Test[1]->GetListOfFunctions()->FindObject("stats");
	auto stats3 = (TPaveStats*)Test[2]->GetListOfFunctions()->FindObject("stats");
	auto stats4 = (TPaveStats*)Test[3]->GetListOfFunctions()->FindObject("stats");
	auto stats5 = (TPaveStats*)Test[4]->GetListOfFunctions()->FindObject("stats");*/

  	/*stats1->SetTextColor(kBlue);
   	stats2->SetTextColor(kGreen);
	stats3->SetTextColor(kAzure);
	stats4->SetTextColor(kViolet);
	stats5->SetTextColor(kYellow);*/
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
	leg1->SetBorderSize(0);
	leg1->Draw();

	c2->cd();
	c2->GetFrame()->SetBorderSize(12);
	leg2->SetBorderSize(0);
	leg2->Draw();
	
	OutputHisto->cd();
	c2->Write();
	c1->Write();
	OutputHisto->Close();
}




int main(){

	Drawpm h;
	h.FitSignalPM();

}


