Bool_t isFileLoaded=kFALSE;
const TString ppacname[12]={"F3-1A","F3-1B","F3-2A","F3-2B","F5-1A","F5-1B","F5-2A","F5-2B","F7-1A","F7-1B","F7-2A","F7-2B"}; 
const Int_t ppacid[12]={4,5,6,7,9,10,11,12,14,15,16,17}; 
std::vector<Int_t> runNolist;
const TString anapath="/home/s015/exp/exp1605_s015/anaroot/users/S22_analysis/";
const TString datapath=anapath+"rootfiles/rawdata/";

const Int_t beamtype=108;

void loadFiles(Int_t);
void estimateGaussPara();
void anaPPACTSum();
void anaPPACTDiff();
void anaPPACnHit();
void anaPPAC_Runbyrun(){
  loadFiles(beamtype);
  anaPPACTSum();
  anaPPACTDiff();
  anaPPACnHit();
}

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
      if(!(irun>=2175&&irun<=2185)&&irun!=2206&&irun!=2207&&irun!=2229&&irun!=2309){ // PPAC didin't work at those runs.
        runNolist.push_back(irun);
        nrun++;
      }
      //if(nrun>50) break;
    }
  }
  isFileLoaded=kTRUE;
}

void anaPPACTSum(){
  
  if(!isFileLoaded) loadFiles(beamtype);  

  
  //gStyle->SetPadGridY(1);

  TH1I* h_tsumx[12];
  TH1I* h_tsumy[12];
  auto f_tsum = new TF1("f_tsum","gaus");
  TH2F* h_runtsumx[12];
  TH2F* h_runtsumy[12];
  Int_t start=runNolist.front();
  Int_t end=runNolist.back();
  for(Int_t i=0; i<12; i++){
    h_runtsumx[i] = new TH2F(Form("h_runtsumx%d",i),ppacname[i]+" TSumX;RUN;TSumX",end+1-start,start,end+1,600,0,300);
    h_runtsumy[i] = new TH2F(Form("h_runtsumy%d",i),ppacname[i]+" TSumY;RUN;TSumY",end+1-start,start,end+1,600,0,300);
  }
  
  TVectorD run(runNolist.size());
  TVectorD xmean[12], ymean[12];
  for(Int_t i=0; i<12; i++){ xmean[i].ResizeTo(runNolist.size()); ymean[i].ResizeTo(runNolist.size());}
  
  Int_t counter=0; 
  auto c = new TCanvas();
  for(auto runNo: runNolist){
    TString fname = Form(datapath+"run%04d.ridf.root",runNo);
    auto raw = new BeamRaw();
    raw->fChain->AddFile(fname.Data(),0,"tree");
    raw->InitPPACTSum();

    std::cout<<"runNo:"<<runNo<<std::endl;
    run(counter)=runNo;
    for(Int_t i=0; i<12; i++){

      h_tsumx[i] = new TH1I(Form("h_tsumx%d",i),ppacname[i].Data(),300,0,300);
      raw->fChain->Project(Form("h_tsumx%d",i),Form("BigRIPSPPAC.fTSumX[%d]",ppacid[i]));
      Double_t mean = h_tsumx[i]->GetBinCenter(h_tsumx[i]->GetMaximumBin());
      f_tsum->SetParameters(h_tsumx[i]->GetMaximum(),mean,2);
      h_tsumx[i]->Fit(f_tsum,"Q","",mean-20,mean+20);
      xmean[i](counter) = f_tsum->GetParameter(1);
      h_tsumx[i]->Clear(); 
      f_tsum->Clear();

      h_tsumy[i] = new TH1I(Form("h_tsumy%d",i),ppacname[i].Data(),300,0,300);
      raw->fChain->Project(Form("h_tsumy%d",i),Form("BigRIPSPPAC.fTSumY[%d]",ppacid[i]));
      mean = h_tsumy[i]->GetBinCenter(h_tsumy[i]->GetMaximumBin());
      f_tsum->SetParameters(h_tsumy[i]->GetMaximum(),mean,2);
      h_tsumy[i]->Fit(f_tsum,"Q","",mean-20,mean+20);
      ymean[i](counter) = f_tsum->GetParameter(1);
      f_tsum->Clear();
      h_tsumy[i]->Clear(); 
    }
    
    for(Int_t i=0; i<raw->fChain->GetEntries(); i++){
      raw->GetEntry(i);
      for(Int_t j=0; j<12; j++){
        h_runtsumx[j]->Fill(runNo,raw->BigRIPSPPAC_fTSumX[ppacid[j]]);
        h_runtsumy[j]->Fill(runNo,raw->BigRIPSPPAC_fTSumY[ppacid[j]]);
      }
    }

    raw->Clear();

    counter++;
  }
  c->Clear(); delete c;
   
  TGraph* g_tsumx[12];
  TGraph* g_tsumy[12];
  for(Int_t i=0; i<12; i++){
    g_tsumx[i] = new TGraph(run,xmean[i]);
    g_tsumy[i] = new TGraph(run,ymean[i]);
    g_tsumx[i]->SetName(Form("g_tsumx%d",i));
    g_tsumy[i]->SetName(Form("g_tsumy%d",i));

    g_tsumx[i]->SetTitle(ppacname[i]+" TSumX");
    g_tsumy[i]->SetTitle(ppacname[i]+" TSumY");
    
    g_tsumx[i]->SetMarkerStyle(3);
    g_tsumx[i]->SetMarkerSize(1.3);
    g_tsumy[i]->SetMarkerStyle(3);
    g_tsumy[i]->SetMarkerSize(1.3);
    if(i<9){
      g_tsumx[i]->SetMarkerColor(i+1);
      g_tsumx[i]->SetLineColor(i+1);
      g_tsumy[i]->SetMarkerColor(i+1);
      g_tsumy[i]->SetLineColor(i+1);
    }else{
      g_tsumx[i]->SetMarkerColor(i+31);
      g_tsumx[i]->SetLineColor(i+31);
      g_tsumy[i]->SetMarkerColor(i+31);
      g_tsumy[i]->SetLineColor(i+31);
    }
  }
  auto legendx = new TLegend(0.85,0.25,1,0.9,"TSumX of each runs","NDC");
  auto legendy = new TLegend(0.85,0.25,1,0.9,"TSumY of each runs","NDC");
  legendx->SetTextSize(0.03);
  legendy->SetTextSize(0.03);
  
  auto mg_tsumx = new TMultiGraph("mg_tsumx","");
  auto mg_tsumy = new TMultiGraph("mg_tsumy","");
  for(int i=0; i<12; i++){
    mg_tsumx->Add(g_tsumx[i]);
    mg_tsumy->Add(g_tsumy[i]);
    legendx->AddEntry(g_tsumx[i]);
    legendy->AddEntry(g_tsumy[i]);
  }

  mg_tsumx->SetTitle("Run vs TSumX");
  mg_tsumy->SetTitle("Run vs TSumY");
  auto cmgtsumx = new TCanvas("cmgtsumx","",1200,700);
  mg_tsumx->Draw("APL");
  legendx->Draw("same");

  auto cmgtsumy = new TCanvas("cmgtsumy","",1200,700);
  mg_tsumy->Draw("APL");
  legendy->Draw("same");
  

  TCanvas* cruntsumx[3];
  for(Int_t i=0; i<3; i++){
    cruntsumx[i] = new TCanvas(Form("cruntsumx%d",i),"");
    cruntsumx[i]->Divide(2,2);
    for(Int_t j=0; j<4; j++){
      cruntsumx[i]->cd(j+1);
      h_runtsumx[i*4+j]->Draw("colz");
    }
  }
  TCanvas* cruntsumy[3];
  for(Int_t i=0; i<3; i++){
    cruntsumy[i] = new TCanvas(Form("cruntsumy%d",i),"");
    cruntsumy[i]->Divide(2,2);
    for(Int_t j=0; j<4; j++){
      cruntsumy[i]->cd(j+1);
      h_runtsumy[i*4+j]->Draw("colz");
    }
  }
  
  auto out = new TFile(Form("supplemental/ppacTSum%d.root",beamtype),"recreate");
  for(Int_t i=0; i<12; i++){
    h_runtsumx[i]->Write();
    h_runtsumy[i]->Write();
    g_tsumx[i]->Write();
    g_tsumy[i]->Write();
  }
  mg_tsumx->Write();
  mg_tsumy->Write();

}

void anaPPACTDiff(){
  
  if(!isFileLoaded) loadFiles(beamtype);  

  
  //gStyle->SetPadGridY(1);

  TH1I* h_tdiffx[12];
  TH1I* h_tdiffy[12];
  auto f_tdiff = new TF1("f_tdiff","gaus");
  TH2F* h_runtdiffx[12];
  TH2F* h_runtdiffy[12];
  Int_t start=runNolist.front();
  Int_t end=runNolist.back();
  for(Int_t i=0; i<12; i++){
    h_runtdiffx[i] = new TH2F(Form("h_runtdiffx%d",i),ppacname[i]+" TDiffX;RUN;TDiffX",end+1-start,start,end+1,300,-50,50);
    h_runtdiffy[i] = new TH2F(Form("h_runtdiffy%d",i),ppacname[i]+" TDiffY;RUN;TDiffY",end+1-start,start,end+1,300,-50,50);
  }

  std::vector<Int_t> run;
  std::vector<Double_t> xmean[12];
  std::vector<Double_t> ymean[12];
  run.clear();
  for(Int_t i=0; i<12; i++){ xmean[i].clear(); ymean[i].clear(); }
  
  auto c = new TCanvas("c","");

  Int_t counter=0; 
  for(auto runNo: runNolist){
    TString fname = Form(datapath+"run%04d.ridf.root",runNo);
    BeamRaw* raw = new BeamRaw();
    raw->fChain->AddFile(fname.Data(),0,"tree");
    //raw->fChain->SetProof();
    raw->InitPPACTSum();

    std::cout<<"runNo:"<<runNo<<std::endl;
    run.push_back(runNo);
    for(Int_t i=0; i<12; i++){

      h_tdiffx[i] = new TH1I(Form("h_tdiffx%d",i),ppacname[i].Data(),300,-50,50);
      raw->fChain->Draw(Form("BigRIPSPPAC.fTDiffX[%d]>>h_tdiffx%d",ppacid[i],i));
      Double_t mean = h_tdiffx[i]->GetBinCenter(h_tdiffx[i]->GetMaximumBin());
      f_tdiff->SetParameters(h_tdiffx[i]->GetMaximum(),mean,2);
      h_tdiffx[i]->Fit(f_tdiff,"Q","",mean-20,mean+20);
      xmean[i].push_back( f_tdiff->GetParameter(1) );
      h_tdiffx[i]->Clear(); 
      f_tdiff->Clear();

      h_tdiffy[i] = new TH1I(Form("h_tdiffy%d",i),ppacname[i].Data(),300,-50,50);
      raw->fChain->Draw(Form("BigRIPSPPAC.fTDiffY[%d]>>h_tdiffy%d",ppacid[i],i));
      mean = h_tdiffy[i]->GetBinCenter(h_tdiffy[i]->GetMaximumBin());
      f_tdiff->SetParameters(h_tdiffy[i]->GetMaximum(),mean,2);
      h_tdiffy[i]->Fit(f_tdiff,"Q","",mean-20,mean+20);
      ymean[i].push_back( f_tdiff->GetParameter(1) );
      f_tdiff->Clear();
      h_tdiffy[i]->Clear(); 
    }
    
    for(Int_t i=0; i<raw->fChain->GetEntries(); i++){
      raw->GetEntry(i);
      for(Int_t j=0; j<12; j++){
        h_runtdiffx[j]->Fill(runNo,raw->BigRIPSPPAC_fTDiffX[ppacid[j]]);
        h_runtdiffy[j]->Fill(runNo,raw->BigRIPSPPAC_fTDiffY[ppacid[j]]);
      }
    }

    raw->Clear();

    counter++;
  }
  c->Clear(); delete c;
   
  TGraph* g_tdiffx[12];
  TGraph* g_tdiffy[12];
  for(Int_t i=0; i<12; i++){
    g_tdiffx[i] = new TGraph(run.size());
    g_tdiffy[i] = new TGraph(run.size());
    g_tdiffx[i]->SetName(Form("g_tdiffx%d",i));
    g_tdiffy[i]->SetName(Form("g_tdiffy%d",i));
    for(Int_t j=0; j<run.size(); j++){
      g_tdiffx[i]->SetPoint(j,run.at(j),xmean[i].at(j));
      g_tdiffy[i]->SetPoint(j,run.at(j),ymean[i].at(j));
    }
    g_tdiffx[i]->SetName(Form("g_tdiffx%d",i));
    g_tdiffy[i]->SetName(Form("g_tdiffy%d",i));
    g_tdiffx[i]->SetTitle(ppacname[i]+" TDiffX");
    g_tdiffy[i]->SetTitle(ppacname[i]+" TDiffY");
    
    g_tdiffx[i]->SetMarkerStyle(3);
    g_tdiffx[i]->SetMarkerSize(1.3);
    g_tdiffy[i]->SetMarkerStyle(3);
    g_tdiffy[i]->SetMarkerSize(1.3);
    if(i<9){
      g_tdiffx[i]->SetMarkerColor(i+1);
      g_tdiffx[i]->SetLineColor(i+1);
      g_tdiffy[i]->SetMarkerColor(i+1);
      g_tdiffy[i]->SetLineColor(i+1);
    }else{
      g_tdiffx[i]->SetMarkerColor(i+31);
      g_tdiffx[i]->SetLineColor(i+31);
      g_tdiffy[i]->SetMarkerColor(i+31);
      g_tdiffy[i]->SetLineColor(i+31);
    }
  }
  auto legendx = new TLegend(0.85,0.25,1,0.9,"TDiffX of each runs","NDC");
  auto legendy = new TLegend(0.85,0.25,1,0.9,"TDiffY of each runs","NDC");
  legendx->SetTextSize(0.03);
  legendy->SetTextSize(0.03);
  
  auto mg_tdiffx = new TMultiGraph("mg_tdiffx","");
  auto mg_tdiffy = new TMultiGraph("mg_tdiffy","");
  for(int i=0; i<12; i++){
    mg_tdiffx->Add(g_tdiffx[i]);
    mg_tdiffy->Add(g_tdiffy[i]);
    legendx->AddEntry(g_tdiffx[i]);
    legendy->AddEntry(g_tdiffy[i]);
  }

  mg_tdiffx->SetTitle("Run vs TDiffX");
  mg_tdiffy->SetTitle("Run vs TDiffY");
  auto cmgtdiffx = new TCanvas("cmgtidffx","",1200,700);
  mg_tdiffx->Draw("APL");
  legendx->Draw("same");

  auto cmgtdiffy = new TCanvas("cmgtdiffy","",1200,700);
  mg_tdiffy->Draw("APL");
  legendy->Draw("same");
  

  TCanvas* cruntdiffx[3];
  for(Int_t i=0; i<3; i++){
    cruntdiffx[i] = new TCanvas(Form("cruntdiffx%d",i),"");
    cruntdiffx[i]->Divide(2,2);
    for(Int_t j=0; j<4; j++){
      cruntdiffx[i]->cd(j+1);
      h_runtdiffx[i*4+j]->Draw("colz");
    }
  }
  TCanvas* cruntdiffy[3];
  for(Int_t i=0; i<3; i++){
    cruntdiffy[i] = new TCanvas(Form("cruntdiffy%d",i),"");
    cruntdiffy[i]->Divide(2,2);
    for(Int_t j=0; j<4; j++){
      cruntdiffy[i]->cd(j+1);
      h_runtdiffy[i*4+j]->Draw("colz");
    }
  }
  
  auto out = new TFile(Form("supplemental/ppacTDiff%d.root",beamtype),"recreate");
  for(Int_t i=0; i<12; i++){
    h_runtdiffx[i]->Write();
    h_runtdiffy[i]->Write();
    g_tdiffx[i]->Write();
    g_tdiffy[i]->Write();
  }
  mg_tdiffx->Write();
  mg_tdiffy->Write();


}

/*
void anaPPACnHit(){
  
  if(!isFileLoaded) loadFiles(beamtype);  

  Int_t start=runNolist.front();
  Int_t end=runNolist.back();
  auto h_f3nhitvsrun = new TH2F("h_f3nhitvsrun","F3 double PPACs nHit;RUN;F3PPAC nhit",end+1-start,start,end+1,80,0,80);
  auto h_f5nhitvsrun = new TH2F("h_f5nhitvsrun","F5 double PPACs nHit;RUN;F5PPAC nhit",end+1-start,start,end+1,80,0,80);
  auto h_f7nhitvsrun = new TH2F("h_f7nhitvsrun","F7 double PPACs nHit;RUN;F7PPAC nhit",end+1-start,start,end+1,80,0,80);

  TString ppacnhitfile=anapath+"/rootfiles/ppac/";
  for(auto runNo: runNolist){
    std::cout<<"runNo:"<<runNo<<std::endl;
    auto chain = new TChain("tree","");
    chain->AddFile(Form(ppacnhitfile+"run%04d.ppac.root",runNo),0,"tree");
    Int_t f3tot, f5tot, f7tot;
    chain->SetBranchAddress("f3tot",&f3tot); 
    chain->SetBranchAddress("f5tot",&f5tot); 
    chain->SetBranchAddress("f7tot",&f7tot); 
    
    for(Int_t i=0; i<chain->GetEntries(); i++){
      chain->GetEntry(i);
      h_f3nhitvsrun->Fill(runNo,f3tot);
      h_f5nhitvsrun->Fill(runNo,f5tot);
      h_f7nhitvsrun->Fill(runNo,f7tot);

    }
    chain->Clear();
    delete chain;
  }

  TCanvas* cnhit[3];
  for(Int_t i=0; i<3; i++) cnhit[i] = new TCanvas(Form("cnhit%d",i),"");
  
  cnhit[0]->cd(); h_f3nhitvsrun->Draw("colz");
  cnhit[1]->cd(); h_f5nhitvsrun->Draw("colz");
  cnhit[2]->cd(); h_f7nhitvsrun->Draw("colz");
  
  auto out = new TFile(Form("supplemental/ppacnHit%d.root",beamtype),"recreate");
  h_f3nhitvsrun->Write();
  h_f5nhitvsrun->Write();
  h_f7nhitvsrun->Write();
  
}
*/
