/*
Code to call historams from root files and make overlaped output image
--------------------------------------------------------------------------
Most of parts are same with histogram_stack.C file,
stacking function is changed to overlaping function
*/

#include <TH2.h>
#include <TH1F.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <iostream>
#include <stdlib.h>
#include <TMath.h>
#include <TLorentzVector.h>
#include <TROOT.h>
#include <TGraph.h>
#include <TFile.h>
#include <TChain.h>
#include <THStack.h>
#include <TLegend.h>
#include <TLine.h>

using namespace std;

map<TString, TH1*> maphist;
map<TString, TFile*> mapfile;
map<TString, TCanvas*> mapcanvas;
map<TString, TPad*> mappad;
map<TString, THStack*> maphstack;
map<TString, TLegend*> maplegend;
map<TString, TH1F*> mapfunc;
map<TString, TLine*> mapline;

//cycle names                                 
TString head = "analyzer_histograms";

//sample names                                
TString ttjets = "ttjets";
TString gluino = "gluino";
TString stop = "stop";

//histogram names
TString Njets = "Njets";
TString Nbjets = "Nbjets";
TString N_Ws = "N_Ws";
TString NlooseLepton = "NlooseLepton";
TString Pt_1st_jet = "Pt_1st_jet";
TString M_R = "M_R";


/// Getting Histogram Function ///////////////////////////////////////////////
TH1 * GetHist(TString hname){

  TH1 *h = NULL;
  std::map<TString, TH1*>::iterator mapit = maphist.find(hname);
  if(mapit != maphist.end()) return mapit-> second;
  
 return h;

}
//////////////////////////////////////////////////////////////////////////////


/// Open ROOT file ///////////////////////////////////////////////////////////
void openfile(TString cyclename, TString samplename){
  TString underbar = "_";
  TString root = ".root";
  TString filename = cyclename + underbar + samplename + root;

  cout << "opening : " << filename << endl;

  mapfile[filename] = new TFile ((filename)) ;
  
  TString Njets = "Njets";
  TString Nbjets = "Nbjets";
  TString N_Ws = "N_Ws";
  TString NlooseLepton = "NlooseLepton";
  TString Pt_1st_jet = "Pt_1st_jet";
  TString M_R = "M_R";
  
  maphist[Njets + cyclename + samplename] =  (TH1*)gDirectory -> Get(Njets);
  maphist[Nbjets + cyclename + samplename] =  (TH1*)gDirectory -> Get(Nbjets);
  maphist[N_Ws + cyclename + samplename] =  (TH1*)gDirectory -> Get(N_Ws);
  maphist[NlooseLepton + cyclename + samplename] =  (TH1*)gDirectory -> Get(NlooseLepton);
  maphist[Pt_1st_jet + cyclename + samplename] =  (TH1*)gDirectory -> Get(Pt_1st_jet);
  
  
}
//////////////////////////////////////////////////////////////////////////////


/// Make Output Histogram ////////////////////////////////////////////////////
void makehistogram(TString nameofhistogram, float xmin, float xmax, float rebin, float ymax){

  TString pad1 = nameofhistogram;
  TString pad2 = nameofhistogram;
  TString canvas = nameofhistogram;
  TString hstack = nameofhistogram;
  TString legend = nameofhistogram;
  TString func = nameofhistogram;
  TString clone = nameofhistogram;
  TString line = nameofhistogram;
  pad1.Insert(0, "pad1_");
  pad2.Insert(0, "pad2_");
  canvas.Insert(0, "c_");
  hstack.Insert(0, "hs_");
  legend.Insert(0, "legend_");
  func.Insert(0, "ratio_");
  clone.Insert(0, "h3_");
  line.Insert(0, "l_");

  mapcanvas[canvas] = new TCanvas(canvas,"",800,600); 
  gStyle -> SetOptStat(1111);
  gStyle -> SetPadTickY(1);
  gStyle -> SetPadTickX(1);

  mappad[pad1] = new TPad("", "", 0, 0, 1, 1);
  mappad[pad1] -> SetBottomMargin(0.05);
  mappad[pad1] -> SetTopMargin(0.05);
  mappad[pad1] -> SetLeftMargin(0.09);
  mappad[pad1] -> SetRightMargin(0.09);
  gStyle -> SetPadTickY(1);
  gStyle -> SetPadTickX(1);
  mappad[pad1] -> Draw();
  mappad[pad1] -> cd();
  mappad[pad1] -> SetLogy();
  
  GetHist(nameofhistogram + head + ttjets) -> SetLineColor(kBlue);
  GetHist(nameofhistogram + head + gluino) -> SetLineColor(kGreen);
  GetHist(nameofhistogram + head + stop) -> SetLineColor(kRed);
  
  GetHist(nameofhistogram + head + ttjets) -> SetLineWidth(3);
  GetHist(nameofhistogram + head + gluino) -> SetLineWidth(3);
  GetHist(nameofhistogram + head + stop) -> SetLineWidth(3);

  GetHist(nameofhistogram + head + ttjets) -> SetFillColor(kBlue);
  GetHist(nameofhistogram + head + gluino) -> SetFillColor(kGreen);
  GetHist(nameofhistogram + head + stop) -> SetFillColor(kRed);
  
  GetHist(nameofhistogram + head + ttjets) -> SetFillStyle(3004);
  GetHist(nameofhistogram + head + gluino) -> SetFillStyle(3007);
  GetHist(nameofhistogram + head + stop) -> SetFillStyle(3005);


  //rebining
  //GetHist(nameofhistogram + TruthCorrection + DY_high) -> Rebin(rebin);
  
  //draw
  GetHist(nameofhistogram + head + ttjets) -> SetStats(0);
  GetHist(nameofhistogram + head + ttjets) -> Draw();
  GetHist(nameofhistogram + head + gluino) -> Draw("same");
  GetHist(nameofhistogram + head + stop) -> Draw("same");

  GetHist(nameofhistogram + head + ttjets) -> GetXaxis() -> SetRangeUser(xmin, xmax);
  GetHist(nameofhistogram + head + ttjets) -> SetMinimum(1.);

  

  mappad[pad1] -> Update();
  
  maplegend[legend] = new TLegend(0.6,0.7,0.9,0.9);
  maplegend[legend] -> AddEntry(GetHist(nameofhistogram + head + ttjets), "ttjets", "l");
  maplegend[legend] -> AddEntry(GetHist(nameofhistogram + head + gluino), "gluino", "l");
  maplegend[legend] -> AddEntry(GetHist(nameofhistogram + head + stop), "stop", "l");

  maplegend[legend] -> SetFillColor(kWhite);
  maplegend[legend] -> SetLineColor(kBlack);
  maplegend[legend] -> SetBorderSize(1);
  maplegend[legend] -> SetFillStyle(1001);
  maplegend[legend] -> SetShadowColor(0); // 0 = transparent
  maplegend[legend] -> SetEntrySeparation(0.3);
  maplegend[legend] -> Draw("same");

  mapcanvas[canvas] -> cd();

  TString pdfname = "./pdfs/";
  pdfname.Append(nameofhistogram);
  pdfname.Append(".pdf");
  mapcanvas[canvas] -> SaveAs(pdfname);

}
//////////////////////////////////////////////////////////////////////////////




/// Main Function ////////////////////////////////////////////////////////////
void plot(){
  
  //open files
  openfile(head, ttjets);
  openfile(head, gluino);
  openfile(head, stop);

  cout << "open files complete" << endl;

  // makehistogram(TString nameofhistogram, float xmin, float xmax, float rebin, float ymax)
  makehistogram(Njets, 0., 20., 1., 100000.);
  makehistogram(Nbjets, 0., 10., 1., 100000.);
  makehistogram(N_Ws, 0., 10., 1., 100000.);
  makehistogram(NlooseLepton, 0., 10., 1., 100000.);
  makehistogram(Pt_1st_jet, 0., 2000., 1., 10000.);
  makehistogram(M_R, 0., 2000., 1., 10000.);


}// End of Main Function ////////////////////////////////////////////////////// 
