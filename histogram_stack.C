/*
Code to call root files containing histograms and to stack them
-----------------------------------------------------------------------------------------------
openfile : open root file with certain name and call histograms in it
gethist : call histogram saved in maphist
makehistogram : make stacked histogrtam with applied xmin, xmax, rebin, ymax of nameofhistogram
plot() : main working function
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
TString Emu_HN = "Emu_HN";
TString TruthCorrection = "TruthCorrection";
TString FakeEstimation = "FakeEstimation";
TString ChargeFlip = "ChargeFlip";

//sample names                                
TString periodA = "periodA_SKemu";
TString periodB = "periodB_SKemu";
TString periodC = "periodC_SKemu";
TString periodD = "periodD_SKemu";
TString WpWp = "SKSSWpWp";
TString WmWm = "SKSSWmWm";
TString WW = "SKWW_py";
TString WZ = "SKWZ_py";
TString ZZ = "SKZZ_py";
TString WWW = "SKWWW";
TString WWZ = "SKWWZ";
TString ZZZ = "SKZZZ";
TString WZZ = "SKWZZ";
TString ggHtoZZ = "SKggHtoZZ";
TString HtoTauTau = "SKHtoTauTau";
TString HtoWW = "SKHtoWW";
TString ttbar = "SKttbar";
TString ttW = "SKttW";
TString ttZ = "SKttZ";
TString DY_low = "SKDY10to50";
TString DY_high = "SKDY50plus";


//histogram names
TString MET = "MET";
TString Njets = "Njets";
TString Pt_1st_jet = "Pt_1st_jet";
TString Pt_2nd_jet = "Pt_2nd_jet";
TString ejjmass_jets = "ejjmass_jets";
TString electron_Pt = "electron_Pt";
TString electron_eta = "electron_eta";
TString emu_mass = "emu_mass";
TString emujjmass_jets = "emujjmass_jets";
TString eta_1st_jet = "eta_1st_jet";
TString eta_2nd_jet = "eta_2nd_jet";
TString jjmass_jets = "jjmass_jets";
TString mujjmass_jets = "mujjmass_jets";
TString muon_Pt = "muon_Pt";
TString muon_eta = "muon_eta";


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
  TString version = "dilep_5_3_14.root";
  TString filename = cyclename + underbar + samplename + underbar + version;

  cout << "opening : " << filename << endl;

  mapfile[filename] = new TFile ((filename)) ;
  
  TString MET = "MET";
  TString Njets = "Njets";
  TString Pt_1st_jet = "Pt_1st_jet";
  TString Pt_2nd_jet = "Pt_2nd_jet";
  TString ejjmass_jets = "ejjmass_jets";
  TString electron_Pt = "electron_Pt";
  TString electron_eta = "electron_eta";
  TString emu_mass = "emu_mass";
  TString emujjmass_jets = "emujjmass_jets";
  TString eta_1st_jet = "eta_1st_jet";
  TString eta_2nd_jet = "eta_2nd_jet";
  TString jjmass_jets = "jjmass_jets";
  TString mujjmass_jets = "mujjmass_jets";
  TString muon_Pt = "muon_Pt";
  TString muon_eta = "muon_eta";
  
  maphist[MET + cyclename + samplename] =  (TH1*)gDirectory -> Get(MET);
  maphist[Njets + cyclename + samplename] =  (TH1*)gDirectory -> Get(Njets);
  maphist[Pt_1st_jet + cyclename + samplename] =  (TH1*)gDirectory -> Get(Pt_1st_jet);
  maphist[Pt_2nd_jet + cyclename + samplename] =  (TH1*)gDirectory -> Get(Pt_2nd_jet);
  maphist[ejjmass_jets + cyclename + samplename] =  (TH1*)gDirectory -> Get(ejjmass_jets);
  maphist[electron_Pt + cyclename + samplename] =  (TH1*)gDirectory -> Get(electron_Pt);
  maphist[electron_eta + cyclename + samplename] =  (TH1*)gDirectory -> Get(electron_eta);
  maphist[emu_mass + cyclename + samplename] =  (TH1*)gDirectory -> Get(emu_mass);
  maphist[emujjmass_jets + cyclename + samplename] =  (TH1*)gDirectory -> Get(emujjmass_jets);
  maphist[eta_1st_jet + cyclename + samplename] =  (TH1*)gDirectory -> Get(eta_1st_jet);
  maphist[eta_2nd_jet + cyclename + samplename] =  (TH1*)gDirectory -> Get(eta_2nd_jet);
  maphist[jjmass_jets + cyclename + samplename] =  (TH1*)gDirectory -> Get(jjmass_jets);
  maphist[mujjmass_jets + cyclename + samplename] =  (TH1*)gDirectory -> Get(mujjmass_jets);
  maphist[muon_Pt + cyclename + samplename] =  (TH1*)gDirectory -> Get(muon_Pt);
  maphist[muon_eta + cyclename + samplename] =  (TH1*)gDirectory -> Get(muon_eta);

  
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

  mappad[pad1] = new TPad("", "", 0, 0.3, 1, 1);
  mappad[pad1] -> SetBottomMargin(0);
  mappad[pad1] -> SetTopMargin(0.05);
  mappad[pad1] -> SetLeftMargin(0.09);
  mappad[pad1] -> SetRightMargin(0.09);
  gStyle -> SetPadTickY(1);
  gStyle -> SetPadTickX(1);
  mappad[pad1] -> Draw();
  mappad[pad1] -> cd();
  //mappad[pad1] -> SetLogy();

  GetHist(nameofhistogram + TruthCorrection + DY_high) -> Add(GetHist(nameofhistogram + TruthCorrection + DY_low));
  GetHist(nameofhistogram + TruthCorrection + DY_high) -> SetFillColor(kPink+6);
  GetHist(nameofhistogram + TruthCorrection + DY_high) -> SetMarkerStyle(21);

  //VV _ WpWp is captain
  GetHist(nameofhistogram + TruthCorrection + WW) -> Add(GetHist(nameofhistogram + TruthCorrection + WZ));
  GetHist(nameofhistogram + TruthCorrection + WW) -> Add(GetHist(nameofhistogram + TruthCorrection + ZZ));
  GetHist(nameofhistogram + TruthCorrection + WW) -> SetFillColor(kGreen);
  GetHist(nameofhistogram + TruthCorrection + WW) -> SetMarkerStyle(21);

  //VVV _ WWW is captain
  GetHist(nameofhistogram + TruthCorrection + WWW) -> Add(GetHist(nameofhistogram + TruthCorrection + WWZ));
  GetHist(nameofhistogram + TruthCorrection + WWW) -> Add(GetHist(nameofhistogram + TruthCorrection + ZZZ));
  GetHist(nameofhistogram + TruthCorrection + WWW) -> Add(GetHist(nameofhistogram + TruthCorrection + WZZ));
  GetHist(nameofhistogram + TruthCorrection + WWW) -> SetFillColor(kSpring+10);
  GetHist(nameofhistogram + TruthCorrection + WWW) -> SetMarkerStyle(21);

  //Higgs _ ggHtoZZ is captain
  GetHist(nameofhistogram + TruthCorrection + ggHtoZZ) -> Add(GetHist(nameofhistogram + TruthCorrection + HtoTauTau));
  GetHist(nameofhistogram + TruthCorrection + ggHtoZZ) -> Add(GetHist(nameofhistogram + TruthCorrection + HtoWW));
  GetHist(nameofhistogram + TruthCorrection + ggHtoZZ) -> SetFillColor(kCyan+2);
  GetHist(nameofhistogram + TruthCorrection + ggHtoZZ) -> SetMarkerStyle(21);

  //ttbar _ ttW is captain
  GetHist(nameofhistogram + TruthCorrection + ttbar) -> Add(GetHist(nameofhistogram + TruthCorrection + ttW));
  GetHist(nameofhistogram + TruthCorrection + ttbar) -> Add(GetHist(nameofhistogram + TruthCorrection + ttZ));
  GetHist(nameofhistogram + TruthCorrection + ttbar) -> SetFillColor(kYellow);
  GetHist(nameofhistogram + TruthCorrection + ttbar) -> SetMarkerStyle(21);

  //fake _ periodA is captain
  GetHist(nameofhistogram + FakeEstimation + periodA) -> Add(GetHist(nameofhistogram + FakeEstimation + periodB));
  GetHist(nameofhistogram + FakeEstimation + periodA) -> Add(GetHist(nameofhistogram + FakeEstimation + periodC));
  GetHist(nameofhistogram + FakeEstimation + periodA) -> Add(GetHist(nameofhistogram + FakeEstimation + periodD));
  GetHist(nameofhistogram + FakeEstimation + periodA) -> SetFillColor(kAzure+10);
  GetHist(nameofhistogram + FakeEstimation + periodA) -> SetMarkerStyle(21);

  //charge flip _ periodA is captain
  GetHist(nameofhistogram + ChargeFlip + periodA) -> Add(GetHist(nameofhistogram + ChargeFlip + periodB));
  GetHist(nameofhistogram + ChargeFlip + periodA) -> Add(GetHist(nameofhistogram + ChargeFlip + periodC));
  GetHist(nameofhistogram + ChargeFlip + periodA) -> Add(GetHist(nameofhistogram + ChargeFlip + periodD));
  GetHist(nameofhistogram + ChargeFlip + periodA) -> SetFillColor(kRed-7);
  GetHist(nameofhistogram + ChargeFlip + periodA) -> SetMarkerStyle(21);

  //rebining
  GetHist(nameofhistogram + TruthCorrection + DY_high) -> Rebin(rebin);
  GetHist(nameofhistogram + TruthCorrection + WW) -> Rebin(rebin);
  GetHist(nameofhistogram + TruthCorrection + WWW) -> Rebin(rebin); 
  GetHist(nameofhistogram + TruthCorrection + ggHtoZZ) -> Rebin(rebin);
  GetHist(nameofhistogram + TruthCorrection + ttbar) -> Rebin(rebin);
  GetHist(nameofhistogram + FakeEstimation + periodA) -> Rebin(rebin);
  GetHist(nameofhistogram + ChargeFlip + periodA) -> Rebin(rebin);

  //stack backgrounds
  maphstack[hstack] = new THStack(hstack, "Stacked_" + nameofhistogram);
  maphstack[hstack] -> Add(GetHist(nameofhistogram + ChargeFlip + periodA));
  maphstack[hstack] -> Add(GetHist(nameofhistogram + TruthCorrection + WWW));
  maphstack[hstack] -> Add(GetHist(nameofhistogram + TruthCorrection + ggHtoZZ));
  maphstack[hstack] -> Add(GetHist(nameofhistogram + TruthCorrection + ttbar));
  maphstack[hstack] -> Add(GetHist(nameofhistogram + TruthCorrection + WW));
  maphstack[hstack] -> Add(GetHist(nameofhistogram + FakeEstimation + periodA));
  maphstack[hstack] -> Add(GetHist(nameofhistogram + TruthCorrection + DY_high));

  //stack data
  GetHist(nameofhistogram + Emu_HN + periodA) -> Add(GetHist(nameofhistogram + Emu_HN + periodB));
  GetHist(nameofhistogram + Emu_HN + periodA) -> Add(GetHist(nameofhistogram + Emu_HN + periodC));
  GetHist(nameofhistogram + Emu_HN + periodA) -> Add(GetHist(nameofhistogram + Emu_HN + periodD));
  GetHist(nameofhistogram + Emu_HN + periodA) -> Rebin(rebin);
  GetHist(nameofhistogram + Emu_HN + periodA) -> SetMarkerStyle(20);
  GetHist(nameofhistogram + Emu_HN + periodA) -> SetMarkerSize(1.0);
  GetHist(nameofhistogram + Emu_HN + periodA) -> Draw("p9");

  //draw
  maphstack[hstack] -> Draw("hist");
  maphstack[hstack] -> GetXaxis() -> SetRangeUser(xmin, xmax);
  maphstack[hstack] -> SetMaximum(ymax);
  maphstack[hstack] -> SetMinimum(1.0);
  maphstack[hstack] -> GetXaxis() -> SetTitle(nameofhistogram);
  maphstack[hstack] -> GetYaxis() -> SetTitle("Entries");
  maphstack[hstack] -> Draw("histsame");

  GetHist(nameofhistogram + Emu_HN + periodA) -> GetXaxis() -> SetRangeUser(xmin, xmax);
  GetHist(nameofhistogram + Emu_HN + periodA) -> SetMinimum(1.0);

  mappad[pad1] -> Update();

  GetHist(nameofhistogram + Emu_HN + periodA) -> Draw("epsame");
  
  maplegend[legend] = new TLegend(0.6,0.7,0.9,0.9);
  maplegend[legend] -> AddEntry(GetHist(nameofhistogram + Emu_HN + periodA), "Data", "p");
  maplegend[legend] -> AddEntry(GetHist(nameofhistogram + FakeEstimation + periodA), "Fake Background", "f");
  maplegend[legend] -> AddEntry(GetHist(nameofhistogram + TruthCorrection + WW), "VV", "f");
  maplegend[legend] -> AddEntry(GetHist(nameofhistogram + TruthCorrection + ttbar), "ttV", "f");
  maplegend[legend] -> AddEntry(GetHist(nameofhistogram + TruthCorrection + ggHtoZZ), "Higgs", "f");
  maplegend[legend] -> AddEntry(GetHist(nameofhistogram + TruthCorrection + WWW), "VVV", "f");
  maplegend[legend] -> AddEntry(GetHist(nameofhistogram + ChargeFlip + periodA), "Charge Flip", "f");
  maplegend[legend] -> AddEntry(GetHist(nameofhistogram + TruthCorrection + DY_high), "DY", "f");

  maplegend[legend] -> SetFillColor(kWhite);
  maplegend[legend] -> SetLineColor(kBlack);
  maplegend[legend] -> SetBorderSize(1);
  maplegend[legend] -> SetFillStyle(1001);
  maplegend[legend] -> SetShadowColor(0); // 0 = transparent
  maplegend[legend] -> SetEntrySeparation(0.3);
  maplegend[legend] -> Draw("same");

  mapcanvas[canvas] -> cd();

  mappad[pad2] = new TPad(pad2, "", 0, 0, 1, 0.29);
  mappad[pad2] -> SetTopMargin(0.05); 
  mappad[pad2] -> SetBottomMargin(0.3);
  mappad[pad2] -> SetLeftMargin(0.09);
  mappad[pad2] -> SetRightMargin(0.09);
  mappad[pad2] -> Draw();
  mappad[pad2] -> cd();

  mapfunc[func] = new TH1F("", "", GetHist(nameofhistogram + Emu_HN + periodA) -> GetNbinsX(),  GetHist(nameofhistogram + Emu_HN + periodA) -> GetXaxis() -> GetXmin(), GetHist(nameofhistogram + Emu_HN + periodA) -> GetXaxis() -> GetXmax());
  mapfunc[func] -> Add(GetHist(nameofhistogram + ChargeFlip + periodA));
  mapfunc[func] -> Add(GetHist(nameofhistogram + TruthCorrection + WWW));
  mapfunc[func] -> Add(GetHist(nameofhistogram + TruthCorrection + ggHtoZZ));
  mapfunc[func] -> Add(GetHist(nameofhistogram + TruthCorrection + ttbar)); 
  mapfunc[func] -> Add(GetHist(nameofhistogram + TruthCorrection + WW)); 
  mapfunc[func] -> Add(GetHist(nameofhistogram + FakeEstimation + periodA)); 
  mapfunc[func] -> Add(GetHist(nameofhistogram + TruthCorrection + DY_high));
  
  mapfunc[clone] = (TH1F*)GetHist(nameofhistogram + Emu_HN + periodA) -> Clone(clone);
  mapline[line] = new TLine(xmin, 1, xmax, 1);
  mapfunc[clone] -> Draw("ep");
  mapline[line] -> Draw();
  mapline[line] -> SetLineStyle(1);
  mapline[line] -> SetLineColor(kRed);
  mapfunc[clone] -> SetTitle(""); 
  mapfunc[clone] -> GetYaxis() -> SetTitle("Data / MC");
  mapfunc[clone] -> GetXaxis() -> SetTitle(nameofhistogram);
  mapfunc[clone] -> GetXaxis() -> SetLabelSize(0.15);
  mapfunc[clone] -> GetYaxis() -> SetLabelSize(0.15);
  mapfunc[clone] -> GetXaxis() -> SetRangeUser(xmin, xmax);
  mapfunc[clone] -> GetYaxis() -> SetTitleOffset(1.0);
  mapfunc[clone] -> GetXaxis() -> SetTitleFont(43);
  mapfunc[clone] -> GetXaxis() -> SetTitleOffset(3.4);
  mapfunc[clone] -> GetXaxis() -> SetTitleSize(20);
  mapfunc[clone] -> GetYaxis() -> SetNdivisions(505);
  mapfunc[clone] -> GetYaxis() -> SetTitleFont(43);
  mapfunc[clone] -> GetYaxis() -> SetTitleSize(25);
  mapfunc[clone] -> SetMinimum(0.0);
  mapfunc[clone] -> SetMaximum(2.0);
  mapfunc[clone] -> SetStats(0);
  mapfunc[clone] -> Divide(mapfunc[func]);
  mapfunc[clone] -> SetMarkerStyle(20);
  mapfunc[clone] -> SetMarkerSize(0.5);
  mapfunc[clone] -> SetMarkerColor(1);

  mapcanvas[canvas] -> cd();
  TString pdfname = "../pdfs/";
  pdfname.Append(nameofhistogram);
  pdfname.Append(".pdf");
  mapcanvas[canvas] -> SaveAs(pdfname);

}
//////////////////////////////////////////////////////////////////////////////




/// Main Function ////////////////////////////////////////////////////////////
void plot(){
  
  //open files(cycle name, sample name)
  openfile(Emu_HN, periodA);
  openfile(Emu_HN, periodB);
  openfile(Emu_HN, periodC);
  openfile(Emu_HN, periodD);
  openfile(FakeEstimation, periodA);
  openfile(FakeEstimation, periodB);
  openfile(FakeEstimation, periodC);
  openfile(FakeEstimation, periodD);
  openfile(ChargeFlip, periodA);
  openfile(ChargeFlip, periodB);
  openfile(ChargeFlip, periodC);
  openfile(ChargeFlip, periodD);
  openfile(TruthCorrection, WW);
  openfile(TruthCorrection, WpWp);
  openfile(TruthCorrection, WmWm);
  openfile(TruthCorrection, WZ);
  openfile(TruthCorrection, ZZ);
  openfile(TruthCorrection, WWW);
  openfile(TruthCorrection, WWZ);
  openfile(TruthCorrection, ZZZ);
  openfile(TruthCorrection, WZZ);
  openfile(TruthCorrection, ggHtoZZ);
  openfile(TruthCorrection, HtoTauTau);
  openfile(TruthCorrection, HtoWW);
  openfile(TruthCorrection, ttbar);
  openfile(TruthCorrection, ttW);
  openfile(TruthCorrection, ttZ);
  openfile(TruthCorrection, DY_low);
  openfile(TruthCorrection, DY_high);

  cout << "open files complete" << endl;

  // makehistogram(TString nameofhistogram, float xmin, float xmax, float rebin, float ymax)
  makehistogram(MET, 0., 200., 10., 5000.);
  makehistogram(Pt_1st_jet, 0., 200., 10., 10000.);
  makehistogram(Pt_2nd_jet, 0., 200., 10., 10000.);
  makehistogram(ejjmass_jets, 0., 1000., 10., 5000.);
  makehistogram(electron_Pt, 0., 200., 10., 10000.);
  makehistogram(muon_Pt, 0., 200., 10., 10000.);
  makehistogram(electron_eta, -3., 3., 1., 2000.);
  makehistogram(muon_eta, -3., 3., 1., 2000.);
  makehistogram(Njets, 0., 10., 1., 20000.);
  makehistogram(emu_mass, 0., 500., 10., 5000.);
  makehistogram(emujjmass_jets, 0., 1000., 10., 5000.);
  makehistogram(eta_1st_jet, -3., 3., 1., 2000.);
  makehistogram(eta_2nd_jet, -3., 3., 1., 2000.);
  makehistogram(jjmass_jets, 0., 500., 10., 7000.);
  makehistogram(mujjmass_jets, 0., 1000., 10., 5000.);


}// End of Main Function ////////////////////////////////////////////////////// 
