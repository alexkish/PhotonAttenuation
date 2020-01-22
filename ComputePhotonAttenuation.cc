#include "TTree.h"
#include "TFile.h"
#include "TROOT.h"
#include "TSystem.h"
#include "TObject.h"
#include "TMath.h"
#include "TPad.h"
#include "TStyle.h"
#include "TCanvas.h"
#include "TGraph.h"
#include "TGraphErrors.h"
#include "TLine.h"
#include "TLegend.h"
#include "TCut.h"
#include "TF1.h"
#include "TAxis.h"

#include "vector"
#include "string"
#include "iostream"
#include "fstream"
#include "stdio.h"
#include "stdlib.h"


void ComputePhotonAttenuation()
{	
	gStyle	->SetOptStat(0);
	//gStyle	->SetOptFit(0);
	//gStyle	->SetOptStat("m");
	gStyle	->SetOptFit(0);

	gStyle	->SetPalette(1);
	
	gStyle->SetStatBorderSize(0);
	gStyle->SetTitleBorderSize(0);
	gStyle->SetTitleFillColor(10);
	gStyle->SetStatColor(10);
	gStyle->SetStatFont(42);
	gStyle->SetMarkerStyle(1);
	gStyle->SetMarkerColor(2);
	
	////////////////////////////////////////////////////////////////////////
	// DATA
	const char *DataFolder 	= "./";	// idark05x
	
	Char_t  DataFile[500];
	sprintf(DataFile, "%s/MassAttCoeff.dat", DataFolder);			

	TTree *t1	= new TTree("t1","t1");
	t1->ReadFile(DataFile, "A:B:C");
	
	int k = t1->GetEntries();
	cout << "Length of the DATA Tree   k = "	<< k << endl;

	float A;
	float B;
	float C;
	
	t1->SetBranchAddress("A",	&A);
	t1->SetBranchAddress("B",	&B);
	t1->SetBranchAddress("C",	&C);
	////////////////////////////////////////////////////////////////////////

	double a1[k];
	double b1[k];
	double c1[k];

	double a2[k];
	double b2[k];
	double c2[k];
	
	const double rho_xe = 2.9172;

	// output file
   	char outfile_char[200];
	sprintf(outfile_char, "./PhotonAttenuation.dat");
	ofstream outfile;
	outfile.open(outfile_char);
	
	for(int i=0;i<k;i++){
		t1->GetEntry(i);
		a1[i] = A;
		b1[i] = B;
		c1[i] = C;

		a2[i] = A;
		b2[i] = 1/(B*rho_xe)*10;
		c2[i] = 1/(C*rho_xe)*10;

		outfile << a2[i] <<"  "<< b2[i] <<"  "<< c2[i] << endl;

		//cout <<"a1 = "<< a1[i] <<" ;  "<<"b1 = "<< b1[i] <<" ;  "<<"c1 = "<< c1[i] << endl;
		//cout <<"a2 = "<< a2 <<" ;  "<<"b2 = "<< b2 <<" ;  "<<"c2 = "<< c2 << endl;
	}
	outfile.close();	

	
	TGraph *gr1ab = new TGraph(k, a1, b1);
	TGraph *gr1ac = new TGraph(k, a1, c1);
			gr1ab->SetLineColor(4);
			gr1ac->SetLineColor(2);

	TGraph *gr2ab = new TGraph(k, a2, b2);
	TGraph *gr2ac = new TGraph(k, a2, c2);
			gr2ab->SetLineColor(4);
			gr2ac->SetLineColor(2);
	
	TCanvas *canvas1 = new TCanvas("canvas1", "canvas1", 0, 0, 700, 500);
			 canvas1->SetFillColor(10);
			 canvas1->SetGrid();
			 canvas1->SetLogy();
			 canvas1->SetLogx();
		gr1ab	->	Draw("APC");
		gr1ac	->	Draw("PC");
		gr1ab	->	SetTitle(0);
		gr1ab	->	GetXaxis()	->	SetTitle("Energy [MeV]");
		gr1ab	->	GetYaxis()	->	SetTitle("#mu/#rho [cm^{2}/g]");
		gr1ab	-> 	GetXaxis() 	-> 	CenterTitle(true);
		gr1ab	-> 	GetYaxis() 	-> 	CenterTitle(true);
	canvas1->Update();

	TLegend *legend2 = new TLegend(0.2816092,0.6716102,0.6925287,0.8877119,NULL,"brNDC"); // dbeta
			 legend2 ->SetBorderSize(0);
			 legend2 ->SetFillColor(10);
			 legend2 ->AddEntry(gr2ab,	"attenuation length",		"l");
			 legend2 ->AddEntry(gr2ac,	"energy-absorption length", "l");

	TCanvas *canvas2 = new TCanvas("canvas2", "canvas2", 705, 0, 700, 500);
			 canvas2->SetFillColor(10);
			 //canvas2->SetGrid();
			 canvas2->SetLogy();
			 canvas2->SetLogx();
		gr2ab	->	Draw("APC");
		gr2ac	->	Draw("PC");
		gr2ab	->	SetTitle(0);
		gr2ab	->	GetXaxis()	->	SetTitle("Energy [MeV]");
		gr2ab	->	GetYaxis()	->	SetTitle("Attenuation length [mm]");
		gr2ab	-> 	GetXaxis() 	-> 	CenterTitle(true);
		gr2ab	-> 	GetYaxis() 	-> 	CenterTitle(true);
		gr2ab	-> 	GetXaxis() 	-> 	SetTitleOffset(1.25);
		gr2ab	-> 	GetYaxis() 	-> 	SetTitleOffset(1.25);
		gr2ab	->	SetMaximum(1e3);
		gr2ab	->	SetMinimum(1e-4);
		legend2 -> Draw("same");
	canvas2->Update();
	
}

		