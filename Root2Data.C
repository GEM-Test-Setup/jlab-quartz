#include "TFile.h"
#include "TTree.h"
#include "TH1D.h"
#include "TString.h"
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include "TCanvas.h"
#include <string>
#include <sstream>

int Root2Data()
{
	gDirectory->DeleteAll();
	int ij;
	double charge,chPlas1,chPlas2;//peledtal corrected charge
	double ped,pedPlas1,pedPlas2;//normalized pedestal
	double rawCh,rawChPlas1,rawChPlas2;//raw adc charge
	
	//***** Constants (gain subject to change via given PMT) *****//
    double R    = 50.;       // in Ohms
    double q    = 1.602e-19; // in eV (electron charge)
	double gain = 7.765e5;    // PMT gain value (in Coulombs)
	
	TH1F *charge1 = new TH1F("charge1","charge1",2000,-0.5,1999.5);
    TH1F *ChargeCor = new TH1F("ChargeCor","ChargeCor",2000,-0.5,1999.5);
    TH1F *peds = new TH1F("peds","peds",2000,-0.5,1999.5);
  
    TH1F *chargePlas1 = new TH1F("Plastic1","Plastic1",3000,-0.5,2999.5);  
    TH1F *chargePlas2 = new TH1F("Plastic2","Plastic2",3000,-0.5,2999.5);

    TH2F *TopVsBottom = new TH2F("TopVsBottom","TopVsBottom", 3000,-0.5,2999.5,3000,-0.5,2999.5);
	
	string myfile;
	int nofile; 
	
	cout << "ROOT file number?" << endl; // Input file name
			getline(cin,myfile);
				stringstream(myfile) >> nofile;
			
	TString infile;
	
	infile=Form("dummy%d.root",nofile);
		
	TFile* fh_file = new TFile(infile, "read");
	
	TTree *Tin = (TTree*)fh_file->Get("T");
	
	T->SetBranchAddress("ChargeCor",&charge);
    T->SetBranchAddress("inte",&rawCh);
    T->SetBranchAddress("ped",&ped);
	
	//For top plastic
    T->SetBranchAddress("chPlas1",&chPlas1);
    T->SetBranchAddress("pedPlas1",&pedPlas1);
    T->SetBranchAddress("intePlastic1",&rawChPlas1);

    //For bottom plastic
    T->SetBranchAddress("chPlas2",&chPlas2);
    T->SetBranchAddress("pedPlas2",&pedPlas2);
    T->SetBranchAddress("intePlastic2",&rawChPlas2);

  	long entries = T->GetEntries();
  	
  	for(ij=0;ij<entries;ij++)
    //for(ij=0;ij<50;ij++)
    {
		T->GetEntry(ij);
		{
			// ADC values
			charge1->Fill(rawCh);
			peds->Fill(ped);
			ChargeCor->Fill(charge);
			
			//ADC values of plasctics
			chargePlas1->Fill(chPlas1);
			chargePlas2->Fill(chPlas2);
			TopVsBottom->Fill(chPlas1,chPlas2);
		}
		
	}
	
	TCanvas *plot =new TCanvas("plot","plot",800,600);
	plot->Divide(1,3);
	plot->cd(1);
	charge1->Draw();
	
	plot->cd(2);
	peds->Draw();
	
	plot->cd(3);
	ChargeCor->Draw();
	
	TCanvas *plot2 =new TCanvas("plot2","plot2",800,600);
	plot2->Divide(1,3);
	plot2->cd(1);
	chargePlas1->Draw();	

	plot2->cd(2);
	chargePlas2->Draw();
	
	plot2->cd(3);
	TopVsBottom->Draw("COLZ");
  	
}
