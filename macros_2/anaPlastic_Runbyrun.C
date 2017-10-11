#include "class/BeamRaw.h"

Bool_t isFileLoaded=kFALSE;
const TString planame[4]={"F3","F7","F13-1","F13-2"}; 
const Int_t plaid[4]={0,2,3,4}; 
std::vector<Int_t> runNolist;
const TString anapath = "/home/s015/exp/exp1605_s015/anaroot/users/108Sn_analysis/BeamAnalysis/mkana/";
//const TString datapath=anapath+"rootfile/data/";
const TString datapath=anapath+"rootfile/raw/";

const Int_t beamtype=112;

void loadFiles(Int_t bt){
  
  if(!(bt==108||bt==112)) return 0;
  Int_t start=2174;
  Int_t stop=2509;
  if(bt==112){ start=2520; stop=2653;}

  runNolist.clear();
  FileStat_t file;
  for(Int_t irun=start; irun<=stop; irun++){
    if( !gSystem->GetPathInfo( Form(datapath+"run%04d.ridf.root",irun),file) ){
      if(irun!=2175&&irun!=2176){  // plastic didn't work then.
        runNolist.push_back(irun);
      }
    }
  }
  isFileLoaded=kTRUE;
}


void anaPlastic_Runbyrun(){
  
  if(!isFileLoaded) loadFiles(beamtype);
  
  TH1I* h_qdcl; 
  TH1I* h_qdcr; 
  TH1D* h_tdcl; 
  TH1D* h_tdcr; 
  TH1D* h_tof; 

  auto f_qdc = new TF1("f_qdc","gaus");
  auto f_tdc = new TF1("f_tdc","gaus");
  auto f_tof = new TF1("f_tof","gaus");
   
  std::vector<Int_t> run;
  std::vector<Double_t> qlmean[4], qrmean[4], tlmean[4], trmean[4];
  std::vector<Double_t> tofmean;
  
  Int_t start=runNolist.front();
  Int_t end=runNolist.back();
  TH2I* h_runqdcl[4];
  TH2I* h_runqdcr[4];
  TH2F* h_runtdcl[4];
  TH2F* h_runtdcr[4];
  TH2F* h_runtof = new TH2F("h_runtof","raw TOF;RUN;TOF(ns)",end+1-start,start,end+1,200,-75,-63);
  for(Int_t i=0; i<2; i++){
     h_runqdcl[i] = new TH2I(Form("h_runqdcl%d",i),planame[i]+"-QDC left;RUN;QDC",end+1-start,start,end+1,500,0,500); 
     h_runqdcr[i] = new TH2I(Form("h_runqdcr%d",i),planame[i]+"-QDC right;RUN;QDC",end+1-start,start,end+1,500,0,500); 
     h_runtdcl[i] = new TH2F(Form("h_runtdcl%d",i),planame[i]+"-Time left;RUN;raw Time(ns)",end+1-start,start,end+1,800,0,800);
     h_runtdcr[i] = new TH2F(Form("h_runtdcr%d",i),planame[i]+"-Time right;RUN;raw Time(ns)",end+1-start,start,end+1,800,0,800);
  }
  for(Int_t i=2; i<4; i++){
     h_runqdcl[i] = new TH2I(Form("h_runqdcl%d",i),planame[i]+"-QDC left;RUN;QDC",end+1-start,start,end+1,500,100,600); 
     h_runqdcr[i] = new TH2I(Form("h_runqdcr%d",i),planame[i]+"-QDC right;RUN;QDC",end+1-start,start,end+1,500,100,600); 
     h_runtdcl[i] = new TH2F(Form("h_runtdcl%d",i),planame[i]+"-Time left;RUN;raw Time(ns)",end+1-start,start,end+1,800,0,400);
     h_runtdcr[i] = new TH2F(Form("h_runtdcr%d",i),planame[i]+"-Time right;RUN;raw Time(ns)",end+1-start,start,end+1,800,0,400);
  }
  auto c = new TCanvas("c","");

  for(auto runNo: runNolist){
    TString fname = Form(datapath+"run%04d.ridf.root",runNo);
    BeamRaw* raw = new BeamRaw();
    raw->fChain->AddFile(fname.Data(),0,"tree");
    raw->InitPlastic();
    
    std::cout<<"runNo:"<<runNo<<std::endl;
    run.push_back(runNo);
    for(Int_t i=0; i<4; i++){
      h_qdcl = new TH1I("h_qdcl",planame[i]+"-QL",800,0,800);
      raw->fChain->Draw( Form("BigRIPSPlastic.fQLRaw[%d]>>h_qdcl",plaid[i]) );
      Double_t qdcmean = h_qdcl->GetBinCenter(h_qdcl->GetMaximumBin());
      f_qdc->SetParameters(h_qdcl->GetMaximum(),qdcmean,20);
      h_qdcl->Fit(f_qdc,"OQ","",qdcmean-100,qdcmean+100);
      qlmean[i].push_back(f_qdc->GetParameter(1));
      h_qdcl->Clear();
      f_qdc->Clear();

      h_qdcr = new TH1I("h_qdcr",planame[i]+"-QR",800,0,800);
      raw->fChain->Draw( Form("BigRIPSPlastic.fQRRaw[%d]>>h_qdcr",plaid[i]));
      qdcmean = h_qdcr->GetBinCenter(h_qdcr->GetMaximumBin());
      f_qdc->SetParameters(h_qdcr->GetMaximum(),qdcmean,20);
      h_qdcr->Fit(f_qdc,"OQ","",qdcmean-100,qdcmean+100);
      qrmean[i].push_back(f_qdc->GetParameter(1));
      h_qdcr->Clear();
      f_qdc->Clear();


      if(i<2) h_tdcl = new TH1D("h_tdcl",planame[i]+"-TL",1600,0,800);
      else h_tdcl = new TH1D("h_tdcl",planame[i]+"-TL",1200,0,400);
      raw->fChain->Draw( Form("BigRIPSPlastic.fTimeL[%d]>>h_tdcl",plaid[i]));
      Double_t timemean = h_tdcl->GetBinCenter(h_tdcl->GetMaximumBin());
      if(i<2) f_tdc->SetParameters(h_tdcl->GetMaximum(),timemean,0.2);
      else f_tdc->SetParameters(h_tdcl->GetMaximum(),timemean,0.05);
      h_tdcl->Fit(f_tdc,"OQ","",timemean-50,timemean+50);
      tlmean[i].push_back(f_tdc->GetParameter(1));
      h_tdcl->Clear();
      f_tdc->Clear();

      if(i<2) h_tdcr = new TH1D("h_tdcr",planame[i]+"-TR",1600,0,800);
      else h_tdcr = new TH1D("h_tdcr",planame[i]+"-TR",1200,0,400);
      raw->fChain->Draw( Form("BigRIPSPlastic.fTimeR[%d]>>h_tdcr",plaid[i]));
      timemean = h_tdcr->GetBinCenter(h_tdcr->GetMaximumBin()); 
      if(i<2) f_tdc->SetParameters(h_tdcr->GetMaximum(),timemean,0.2);
      else f_tdc->SetParameters(h_tdcr->GetMaximum(),timemean,0.05);
      h_tdcr->Fit(f_tdc,"OQ","",timemean-50,timemean+50);
      trmean[i].push_back(f_tdc->GetParameter(1));
      h_tdcr->Clear();
      f_tdc->Clear();

    }
    
    h_tof = new TH1D("h_tof","",500,-80,-30);
    raw->fChain->Draw("BigRIPSPlastic.fTime[2]-BigRIPSPlastic.fTime[0]>>h_tof");
    Double_t rawtofmean = h_tof->GetBinCenter(h_tof->GetMaximumBin());
    f_tof->SetParameters(h_tof->GetMaximum(),rawtofmean,0.2);
    h_tof->Fit(f_tof,"OQ","",rawtofmean-0.7,rawtofmean+0.7);
    tofmean.push_back(f_tof->GetParameter(1));
    h_tof->Clear();
    f_tof->Clear();

    for(Int_t i=0; i<raw->fChain->GetEntries(); i++){
      raw->GetEntry(i);
      for(Int_t j=0; j<4; j++){
        h_runqdcl[j]->Fill(runNo,raw->BigRIPSPlastic_fQLRaw[plaid[j]]);
        h_runqdcr[j]->Fill(runNo,raw->BigRIPSPlastic_fQRRaw[plaid[j]]);
        h_runtdcl[j]->Fill(runNo,raw->BigRIPSPlastic_fTimeL[plaid[j]]);
        h_runtdcr[j]->Fill(runNo,raw->BigRIPSPlastic_fTimeR[plaid[j]]);
      }
      h_runtof->Fill(runNo,raw->BigRIPSPlastic_fTime[2]-raw->BigRIPSPlastic_fTime[0]);
    }
    raw->Clear();
  }
  c->Clear(); delete c;

  gStyle->SetPadGridY(1);
  auto qlegend = new TLegend(0.85,0.3,0.97,0.9,"Each plastic QDC","NDC");
  auto tlegend = new TLegend(0.85,0.3,0.97,0.9,"Each plastic TDC","NDC");
  qlegend->SetTextSize(0.03);
  tlegend->SetTextSize(0.03);
  
  auto mg_qdc = new TMultiGraph("mg_qdc","");
  auto mg_tdc = new TMultiGraph("mg_tdc","");
  TGraph* g_qdcl[4];
  TGraph* g_qdcr[4];
  TGraph* g_tdcl[4];
  TGraph* g_tdcr[4];
  for(Int_t i=0; i<4; i++){
    g_qdcl[i] = new TGraph(run.size()); 
    g_qdcr[i] = new TGraph(run.size()); 
    g_tdcl[i] = new TGraph(run.size()); 
    g_tdcr[i] = new TGraph(run.size());
    g_qdcl[i]->SetName(Form("g_qdcl%d",i));   g_qdcl[i]->SetTitle(planame[i]+" QL");
    g_qdcr[i]->SetName(Form("g_qdcr%d",i));   g_qdcr[i]->SetTitle(planame[i]+" QR");
    g_tdcl[i]->SetName(Form("g_tdcl%d",i));   g_tdcl[i]->SetTitle(planame[i]+" TL");
    g_tdcr[i]->SetName(Form("g_tdcr%d",i));   g_tdcr[i]->SetTitle(planame[i]+" TR");

    g_qdcl[i]->SetMarkerStyle(3);   g_qdcr[i]->SetMarkerStyle(3);
    g_qdcl[i]->SetMarkerSize(1.3);  g_qdcr[i]->SetMarkerSize(1.3);
    g_qdcl[i]->SetMarkerColor(i+1); g_qdcr[i]->SetMarkerColor(i+5);
    g_qdcl[i]->SetLineColor(i+1);   g_qdcr[i]->SetLineColor(i+5);
    
    g_tdcl[i]->SetMarkerStyle(3);   g_tdcr[i]->SetMarkerStyle(3);
    g_tdcl[i]->SetMarkerSize(1.3);  g_tdcr[i]->SetMarkerSize(1.3);
    g_tdcl[i]->SetMarkerColor(i+1); g_tdcr[i]->SetMarkerColor(i+5);
    g_tdcl[i]->SetLineColor(i+1);   g_tdcr[i]->SetLineColor(i+5);
    
    for(Int_t j=0; j<run.size(); j++){
      g_qdcl[i]->SetPoint(j,run.at(j),qlmean[i].at(j));
      g_qdcr[i]->SetPoint(j,run.at(j),qrmean[i].at(j));
      g_tdcl[i]->SetPoint(j,run.at(j),tlmean[i].at(j));
      g_tdcr[i]->SetPoint(j,run.at(j),trmean[i].at(j));
    }
    
    mg_qdc->Add(g_qdcl[i]);  mg_qdc->Add(g_qdcr[i]);
    mg_tdc->Add(g_tdcl[i]);  mg_tdc->Add(g_tdcr[i]);
    qlegend->AddEntry(g_qdcl[i]); qlegend->AddEntry(g_qdcr[i]);
    tlegend->AddEntry(g_tdcl[i]); tlegend->AddEntry(g_tdcr[i]);
  }

  mg_qdc->SetTitle("Run vs pla. QDC");
  mg_tdc->SetTitle("Run vs pla. TDC");
  auto cmg = new TCanvas("cmg","");
  mg_qdc->Draw("APL");
  qlegend->Draw("same");
    
  auto ctmg = new TCanvas("ctmg","");
  mg_tdc->Draw("APL");
  tlegend->Draw("same");
  
  
  auto ctof = new TCanvas("ctof","");
  TGraph* g_tof = new TGraph(run.size());
  for(Int_t i=0; i<run.size(); i++) g_tof->SetPoint(i,run.at(i),tofmean.at(i));
  g_tof->SetName("g_tof");
  g_tof->SetTitle("raw TOF vs run");
  g_tof->SetMarkerStyle(3);
  g_tof->SetMarkerSize(1.3);
  g_tof->Draw("APL");
  
  auto crql = new TCanvas("crql","");
  crql->Divide(2,2);
  for(Int_t i=0; i<4; i++){
    crql->cd(i+1);
    h_runqdcl[i]->Draw("colz");
  }
  auto crqr = new TCanvas("crqr","");
  crqr->Divide(2,2);
  for(Int_t i=0; i<4; i++){
    crqr->cd(i+1);
    h_runqdcr[i]->Draw("colz");
  }
  auto crtl = new TCanvas("crtl","");
  crtl->Divide(2,2);
  for(Int_t i=0; i<4; i++){
    crtl->cd(i+1);
    h_runtdcl[i]->Draw("colz");
  }
  auto crtr = new TCanvas("crtr","");
  crtr->Divide(2,2);
  for(Int_t i=0; i<4; i++){
    crtr->cd(i+1);
    h_runtdcr[i]->Draw("colz");
  }
  auto crtof = new TCanvas("frtof","");
  h_runtof->Draw("colz");
  
  auto out = new TFile(Form("supplemental/plaRawData%d.root",beamtype),"recreate");
  for(Int_t i=0; i<4; i++){
    g_qdcl[i]->Write();
    g_qdcr[i]->Write();
    g_tdcl[i]->Write();
    g_tdcr[i]->Write();
    h_runqdcl[i]->Write(); 
    h_runqdcr[i]->Write();
    h_runtdcl[i]->Write();
    h_runtdcr[i]->Write();
  }
  mg_qdc->Write();
  mg_tdc->Write();
  g_tof->Write();
  h_runtof->Write();
}
