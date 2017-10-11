#include "class/BeamRaw.h"

Bool_t isFileLoaded=kFALSE;
std::vector<Int_t> runNolist;
const TString anapath="/home/s015/exp/exp1605_s015/anaroot/users/108Sn_analysis/BeamAnalysis/mkana/";
//const TString datapath=anapath+"rootfile/data/";
const TString datapath=anapath+"rootfile/raw/";

const Int_t beamtype=112;

void loadFiles(Int_t bt){
  
  if(!(bt==108||bt==112)) return 0;
  Int_t start=2174;
  Int_t stop=2509;
  if(bt==112){ start=2520; stop=2653;}

  Int_t nrun=0;
  runNolist.clear();
  FileStat_t file;
  for(Int_t irun=start; irun<=stop; irun++){
    if( !gSystem->GetPathInfo( Form(datapath+"run%04d.ridf.root",irun),file) ){
      if(!(irun>=2175&&irun<=2185)&&irun!=2206&&irun!=2207&&irun!=2229){  // ic didn't work then.
        runNolist.push_back(irun);
        nrun++;
      }
      //if(nrun>50) break;
    }
  }
  isFileLoaded=kTRUE;
}

void anaIC_Runbyrun(){
  if(!isFileLoaded) loadFiles(beamtype);
  

  Int_t start=runNolist.front();
  Int_t end=runNolist.back();
  TH2I* h_runadc[6];
  for(Int_t i=0; i<6; i++)
    h_runadc[i] = new TH2I(Form("h_runadc%d",i),Form("Channel%d;RUN;ADC",i),end+1-start,start,end+1,2000,3200,5200);
  auto h_runsqsum = new TH2F("h_runsqsum","ADCSqSum;RUN;SqSum",end+1-start,start,end+1,2000,3200,5000);
  auto h_runavsum = new TH2F("h_runavsum","ADCAvSum;RUN;AvSum",end+1-start,start,end+1,2000,3200,5000);
  
  for(auto runNo: runNolist){
  //for(auto runNo: testlist){
    TString fname = Form(datapath+"run%04d.ridf.root",runNo);
    BeamRaw* raw = new BeamRaw();
    raw->fChain->AddFile(fname.Data(),0,"tree");
    raw->InitIC();
    std::cout<<"runNo:"<<runNo<<std::endl;
    for(Int_t i=0; i<raw->fChain->GetEntries(); i++){
      raw->fChain->GetEntry(i);
      for(Int_t j=0; j<6; j++) h_runadc[j]->Fill(runNo,raw->BigRIPSIC_fADC[2][j]);
      h_runsqsum->Fill(runNo,raw->BigRIPSIC_fRawADCSqSum[2]);
      h_runavsum->Fill(runNo,raw->BigRIPSIC_fRawADCAvSum[2]);
    }
    raw->Clear();
  }
  
  auto cadc = new TCanvas("cadc","");
  cadc->Divide(3,2);
  for(Int_t i=0; i<6; i++){
    cadc->cd(i+1);
    h_runadc[i]->Draw("colz");
  }
  
  auto csqsum = new TCanvas("csqsum","");
  h_runsqsum->Draw("colz");

  auto cavsum = new TCanvas("cavsum","");
  h_runavsum->Draw("colz");
  
  auto out = new TFile(Form("supplemental/icADC%d.root",beamtype),"recreate");
  for(Int_t i=0; i<6; i++)h_runadc[i]->Write();
  h_runsqsum->Write();
  h_runavsum->Write();
  
}
