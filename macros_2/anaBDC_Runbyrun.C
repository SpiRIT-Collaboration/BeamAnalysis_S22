
Bool_t isFileLoaded=kFALSE;
Bool_t isTrkFileLoaded=kFALSE;
std::vector<Int_t> runNolist;
std::vector<Int_t> runNoTrklist;
const TString anapath="/home/s015/exp/exp1605_s015/anaroot/users/108Sn_analysis/BeamAnalysis/mkana/";
const TString datapath=anapath+"rootfile/bdc/";

const Int_t beamtype=108;

void loadFiles(Int_t);
void loadTrkFiles(Int_t);
void anaBDCHit();
void anaBDCTrack();

void anaBDC_Runbyrun(){

  anaBDCHit();
  anaBDCTrack();
  
}


void anaBDCTrack(){
  if(!isTrkFileLoaded) loadTrkFiles(beamtype);
 
  const Int_t nrun = runNoTrklist.size();
  TVectorD runvec(nrun);
  TVectorD tx1vec(nrun), ty1vec(nrun), ta1vec(nrun), tb1vec(nrun);
  TVectorD tx2vec(nrun), ty2vec(nrun), ta2vec(nrun), tb2vec(nrun);
  TVectorD txvec(nrun), tyvec(nrun), tavec(nrun), tbvec(nrun);
  
  TH1 *h_tx, *h_ty, *h_ta, *h_tb;
  auto f_tx = new TF1("f_tx","gaus");

  Int_t start = runNoTrklist.front();
  Int_t end = runNoTrklist.back();
  auto h_runtx1 = new TH2D("h_runtx1","BDC1 X position vs RUN",end+1-start,start,end,200,-50,50);
  auto h_runtx2 = new TH2D("h_runtx2","BDC2 X position vs RUN",end+1-start,start,end,200,-50,50);
  auto h_runtx  = new TH2D("h_runtx", "X position at target plane vs RUN",end+1-start,start,end,200,-50,50);
  auto h_runty1 = new TH2D("h_runty1","BDC1 Y position vs RUN",end+1-start,start,end,200,-50,50);
  auto h_runty2 = new TH2D("h_runty2","BDC2 Y position vs RUN",end+1-start,start,end,200,-50,50);
  auto h_runty  = new TH2D("h_runty", "Y position at target plane vs RUN",end+1-start,start,end,200,-50,50);
  auto h_runta1 = new TH2D("h_runta1","BDC1 X angle vs RUN",end+1-start,start,end,200,-0.05,0.05);
  auto h_runta2 = new TH2D("h_runta2","BDC2 X angle vs RUN",end+1-start,start,end,200,-0.05,0.05);
  auto h_runta  = new TH2D("h_runta", "X angle at target plane vs RUN",end+1-start,start,end,200,-50,50);
  auto h_runtb1 = new TH2D("h_runtb1","BDC1 Y angle vs RUN",end+1-start,start,end,200,-0.05,0.05);
  auto h_runtb2 = new TH2D("h_runtb2","BDC2 Y angle vs RUN",end+1-start,start,end,200,-0.05,0.05);
  auto h_runtb  = new TH2D("h_runtb", "Y angle at target plane vs RUN",end+1-start,start,end,200,-50,50);
  
  auto c = new TCanvas("c","");
  
  Int_t counter=0;
  for(auto runNo: runNoTrklist){
    TString fname = Form(datapath+"run%d.bdctrk.root",runNo);
    auto chain = new TChain("chain");
    chain->AddFile(fname.Data(),0,"tree");
    Double_t tx1, tx2, tx;
    Double_t ty1, ty2, ty;
    Double_t ta1, ta2, ta;
    Double_t tb1, tb2, tb;
    chain->SetBranchAddress("tx1",&tx1);
    chain->SetBranchAddress("ty1",&ty1);
    chain->SetBranchAddress("ta1",&ta1);
    chain->SetBranchAddress("tb1",&tb1);
    chain->SetBranchAddress("tx2",&tx2);
    chain->SetBranchAddress("ty2",&ty2);
    chain->SetBranchAddress("ta2",&ta2);
    chain->SetBranchAddress("tb2",&tb2);
    chain->SetBranchAddress("tx",&tx);
    chain->SetBranchAddress("ty",&ty);
    chain->SetBranchAddress("ta",&ta);
    chain->SetBranchAddress("tb",&tb);
    
    std::cout<<"runNo:"<<runNo<<std::endl;
    runvec(counter)=runNo;
    
    for(Int_t i=0; i<chain->GetEntries(); i++){
      tx1=ty1=ta1=tb1=tx2=ty2=ta2=tb2=tx=ty=ta=tb=-9999.;
      chain->GetEntry(i);
      h_runtx1->Fill(runNo,tx1); h_runty1->Fill(runNo,ty1); h_runta1->Fill(runNo,ta1); h_runtb1->Fill(runNo,tb1);
      h_runtx2->Fill(runNo,tx2); h_runty2->Fill(runNo,ty2); h_runta2->Fill(runNo,ta2); h_runtb2->Fill(runNo,tb2);
      h_runtx->Fill(runNo,tx); h_runty->Fill(runNo,ty); h_runta->Fill(runNo,ta); h_runtb->Fill(runNo,tb);
    }
    h_tx = new TH1D("h_tx","",200,-50,50);
    chain->Draw("tx1>>h_tx");
    Int_t cons = h_tx->GetMaximum();
    Double_t mean = h_tx->GetBinCenter(h_tx->GetMaximumBin());
    f_tx->SetParameters(cons,mean,7);
    h_tx->Fit(f_tx,"","",mean-10,mean+10);
    tx1vec(counter)=f_tx->GetParameter(1);
    h_tx->Clear(); f_tx->Clear();
    
    h_tx = new TH1D("h_tx","",200,-50,50);
    chain->Draw("tx2>>h_tx");
    cons = h_tx->GetMaximum();
    mean = h_tx->GetBinCenter(h_tx->GetMaximumBin());
    f_tx->SetParameters(cons,mean,7);
    h_tx->Fit(f_tx,"","",mean-10,mean+10);
    tx2vec(counter)=f_tx->GetParameter(1);
    h_tx->Clear(); f_tx->Clear();
    
    h_tx = new TH1D("h_tx","",200,-50,50);
    chain->Draw("tx>>h_tx");
    cons = h_tx->GetMaximum();
    mean = h_tx->GetBinCenter(h_tx->GetMaximumBin());
    f_tx->SetParameters(cons,mean,7);
    h_tx->Fit(f_tx,"","",mean-10,mean+10);
    txvec(counter)=f_tx->GetParameter(1);
    h_tx->Clear(); f_tx->Clear();
    
    h_ty = new TH1D("h_ty","",200,-50,50);
    chain->Draw("ty1>>h_ty");
    cons = h_ty->GetMaximum();
    mean = h_ty->GetBinCenter(h_ty->GetMaximumBin());
    f_tx->SetParameters(cons,mean,7);
    h_ty->Fit(f_tx,"","",mean-10,mean+10);
    ty1vec(counter)=f_tx->GetParameter(1);
    h_ty->Clear(); f_tx->Clear();
    
    h_ty = new TH1D("h_ty","",200,-50,50);
    chain->Draw("ty2>>h_ty");
    cons = h_ty->GetMaximum();
    mean = h_ty->GetBinCenter(h_ty->GetMaximumBin());
    f_tx->SetParameters(cons,mean,7);
    h_ty->Fit(f_tx,"","",mean-10,mean+10);
    ty2vec(counter)=f_tx->GetParameter(1);
    h_ty->Clear(); f_tx->Clear();
    
    h_ty = new TH1D("h_ty","",200,-50,50);
    chain->Draw("ty>>h_ty");
    cons = h_ty->GetMaximum();
    mean = h_ty->GetBinCenter(h_ty->GetMaximumBin());
    f_tx->SetParameters(cons,mean,7);
    h_ty->Fit(f_tx,"","",mean-10,mean+10);
    tyvec(counter)=f_tx->GetParameter(1);
    h_ty->Clear(); f_tx->Clear();

    h_ta = new TH1D("h_ta","",200,-0.05,0.05);
    chain->Draw("ta1>>h_ta");
    cons = h_ta->GetMaximum();
    mean = h_ta->GetBinCenter(h_ta->GetMaximumBin());
    f_tx->SetParameters(cons,mean,0.007);
    h_ta->Fit(f_tx,"","",mean-0.01,mean+0.01);
    ta1vec(counter)=f_tx->GetParameter(1);
    h_ta->Clear(); f_tx->Clear();
    
    h_ta = new TH1D("h_ta","",200,-0.05,0.05);
    chain->Draw("ta2>>h_ta");
    cons = h_ta->GetMaximum();
    mean = h_ta->GetBinCenter(h_ta->GetMaximumBin());
    f_tx->SetParameters(cons,mean,0.007);
    h_ta->Fit(f_tx,"","",mean-0.01,mean+0.01);
    ta2vec(counter)=f_tx->GetParameter(1);
    h_ta->Clear(); f_tx->Clear();
    
    h_ta = new TH1D("h_ta","",200,-50,50);
    chain->Draw("ta>>h_ta");
    cons = h_ta->GetMaximum();
    mean = h_ta->GetBinCenter(h_ta->GetMaximumBin());
    f_tx->SetParameters(cons,mean,7);
    h_ta->Fit(f_tx,"","",mean-10,mean+10);
    tavec(counter)=f_tx->GetParameter(1);
    h_ta->Clear(); f_tx->Clear();
    h_tb = new TH1D("h_tb","",200,-0.05,0.05);
    chain->Draw("tb1>>h_tb");
    cons = h_tb->GetMaximum();
    mean = h_tb->GetBinCenter(h_tb->GetMaximumBin());
    f_tx->SetParameters(cons,mean,0.007);
    h_tb->Fit(f_tx,"","",mean-0.01,mean+0.01);
    tb1vec(counter)=f_tx->GetParameter(1);
    h_tb->Clear(); f_tx->Clear();
    h_tb = new TH1D("h_tb","",200,-0.05,0.05);
    chain->Draw("tb2>>h_tb");
    cons = h_tb->GetMaximum();
    mean = h_tb->GetBinCenter(h_tb->GetMaximumBin());
    f_tx->SetParameters(cons,mean,0.007);
    h_tb->Fit(f_tx,"","",mean-0.01,mean+0.01);
    tb2vec(counter)=f_tx->GetParameter(1);
    h_tb->Clear(); f_tx->Clear();
    h_tb = new TH1D("h_tb","",200,-50,50);
    chain->Draw("tb>>h_tb");
    cons = h_tb->GetMaximum();
    mean = h_tb->GetBinCenter(h_tb->GetMaximumBin());
    f_tx->SetParameters(cons,mean,7);
    h_tb->Fit(f_tx,"","",mean-10,mean+10);
    tbvec(counter)=f_tx->GetParameter(1);
    h_tb->Clear(); f_tx->Clear();
    

    chain->Clear();
  
    counter++;
  }
  
  auto crun1 = new TCanvas("crun1","");
  crun1->Divide(2,2);
  crun1->cd(1); h_runtx1->Draw("colz");
  crun1->cd(2); h_runty1->Draw("colz");
  crun1->cd(3); h_runta1->Draw("colz");
  crun1->cd(4); h_runtb1->Draw("colz");
  auto crun2 = new TCanvas("crun2","");
  crun2->Divide(2,2);
  crun2->cd(1); h_runtx2->Draw("colz");
  crun2->cd(2); h_runty2->Draw("colz");
  crun2->cd(3); h_runta2->Draw("colz");
  crun2->cd(4); h_runtb2->Draw("colz");
  auto crun = new TCanvas("crun","");
  crun->Divide(2,2);
  crun->cd(1); h_runtx->Draw("colz");
  crun->cd(2); h_runty->Draw("colz");
  crun->cd(3); h_runta->Draw("colz");
  crun->cd(4); h_runtb->Draw("colz");
 
  auto g_tx1 = new TGraph(runvec,tx1vec); auto g_tx2 = new TGraph(runvec,tx2vec); auto g_tx = new TGraph(runvec,txvec);
  auto g_ty1 = new TGraph(runvec,ty1vec); auto g_ty2 = new TGraph(runvec,ty2vec); auto g_ty = new TGraph(runvec,tyvec);
  auto g_ta1 = new TGraph(runvec,ta1vec); auto g_ta2 = new TGraph(runvec,ta2vec); auto g_ta = new TGraph(runvec,tavec);
  auto g_tb1 = new TGraph(runvec,tb1vec); auto g_tb2 = new TGraph(runvec,tb2vec); auto g_tb = new TGraph(runvec,tbvec);
  
  g_tx1->SetMarkerStyle(3); g_tx2->SetMarkerStyle(3); g_tx->SetMarkerStyle(3);
  g_ty1->SetMarkerStyle(3); g_ty2->SetMarkerStyle(3); g_ty->SetMarkerStyle(3);
  g_ta1->SetMarkerStyle(3); g_ta2->SetMarkerStyle(3); g_ta->SetMarkerStyle(3);
  g_tb1->SetMarkerStyle(3); g_tb2->SetMarkerStyle(3); g_tb->SetMarkerStyle(3);
  
  g_tx1->SetTitle("BDC1 X"); g_tx2->SetTitle("BDC2 X"); g_tx->SetTitle("Target X");
  g_ty1->SetTitle("BDC1 Y"); g_ty2->SetTitle("BDC2 Y"); g_ty->SetTitle("Target Y");
  g_ta1->SetTitle("BDC1 A"); g_ta2->SetTitle("BDC2 A"); g_ta->SetTitle("Target A");
  g_tb1->SetTitle("BDC1 B"); g_tb2->SetTitle("BDC2 B"); g_tb->SetTitle("Target B");
  g_tx1->SetName("g_tx1"); g_tx2->SetName("g_tx2"); g_tx->SetName("g_tx");
  g_ty1->SetName("g_ty1"); g_ty2->SetName("g_ty2"); g_ty->SetName("g_ty");
  g_ta1->SetName("g_ta1"); g_ta2->SetName("g_ta2"); g_ta->SetName("g_ta");
  g_tb1->SetName("g_tb1"); g_tb2->SetName("g_tb2"); g_tb->SetName("g_tb");
   
  auto cg1 = new TCanvas("cg1","");
  cg1->Divide(2,2);
  cg1->cd(1); g_tx1->Draw("APL");
  cg1->cd(2); g_ty1->Draw("APL");
  cg1->cd(3); g_ta1->Draw("APL");
  cg1->cd(4); g_tb1->Draw("APL");
  auto cg2 = new TCanvas("cg2","");
  cg2->Divide(2,2);
  cg2->cd(1); g_tx2->Draw("APL");
  cg2->cd(2); g_ty2->Draw("APL");
  cg2->cd(3); g_ta2->Draw("APL");
  cg2->cd(4); g_tb2->Draw("APL");
  auto cg = new TCanvas("cg","");
  cg->Divide(2,2);
  cg->cd(1); g_tx->Draw("APL");
  cg->cd(2); g_ty->Draw("APL");
  cg->cd(3); g_ta->Draw("APL");
  cg->cd(4); g_tb->Draw("APL");
 

}

void anaBDCHit(){

  if(!isFileLoaded) loadFiles(beamtype);
  
  
  std::vector<Int_t> runvec;
  std::vector<Double_t> tdc1[8]; //bdc1 tdc for each layer
  std::vector<Double_t> tdc2[8]; 
  
  TH1* h_tdc1[8];
  TH1* h_tdc2[8];
  TF1* f_tdc = new TF1("f_tdc","gaus");

  TH2* h_runtdc1[8];
  TH2* h_runtdc2[8];
  Int_t start = runNolist.front();
  Int_t end = runNolist.back();
  for(Int_t i=0; i<8; i++){
    h_runtdc1[i] = new TH2I(Form("h_runtdc1%d",i),Form("BDC1 Layer%d TDC;RUN;TDC",i),end+1-start,start,end+1,2000,0,2000);
    h_runtdc2[i] = new TH2I(Form("h_runtdc2%d",i),Form("BDC2 Layer%d TDC;RUN;TDC",i),end+1-start,start,end+1,2000,0,2000);
  }
  
  TH1* h_runsummedtdc1[8][7];
  TH1* h_runsummedtdc2[8][7];
  for(Int_t i=0; i<8; i++){
    for(Int_t j=0; j<7; j++){
      h_runsummedtdc1[i][j] = new TH1I(Form("h_runsummedtdc1_%d_%d",i,j),Form("BDC1 TDC layer%d, Group%d",i,j),2000,0,2000);
      h_runsummedtdc2[i][j] = new TH1I(Form("h_runsummedtdc2_%d_%d",i,j),Form("BDC2 TDC layer%d, Group%d",i,j),2000,0,2000);
    } 
  } 
  
  Int_t rungroup[8]={};
  if(beamtype==108){
    rungroup[0]=2174; rungroup[1]=2194; rungroup[2]=2280; rungroup[3]=2360;
    rungroup[4]=2410;rungroup[5]=2470;rungroup[6]=2475;rungroup[7]=2509;
  }else if(beamtype==112){
    rungroup[0]=2520;rungroup[1]=2530;
    rungroup[2]=2570;rungroup[3]=2610;
    rungroup[4]=2653;
  }

    
    
    

  auto c = new TCanvas("c","");
  for(auto runNo: runNolist){
    TString fname = Form(datapath+"run%d.bdc.root",runNo);
    auto chain = new TChain("chain");
    chain->AddFile(fname.Data(),0,"tree");
    TClonesArray* bdc1hit = NULL;
    TClonesArray* bdc2hit = NULL;
    chain->SetBranchAddress("SAMURAIBDC1Hit",&bdc1hit);
    chain->SetBranchAddress("SAMURAIBDC2Hit",&bdc2hit);
    
    std::cout<<"runNo:"<<runNo<<std::endl;
    for(Int_t i=0; i<8; i++){
      h_tdc1[i] = new TH1I(Form("h_tdc1_%d",i),"",500,0,2000);
      h_tdc2[i] = new TH1I(Form("h_tdc2_%d",i),"",500,0,2000);
    }

    for(Int_t i=0; i<chain->GetEntries(); i++){
      chain->GetEntry(i);
      
      for(Int_t j=0; j<bdc1hit->GetEntries(); j++){
        TArtDCHit* hit = (TArtDCHit*)bdc1hit->At(j);
        h_tdc1[hit->GetLayer()]->Fill(hit->GetTDC());
        h_runtdc1[hit->GetLayer()]->Fill(runNo,hit->GetTDC());
        for(Int_t k=0; k<7; k++)
          if(runNo>rungroup[k]&&runNo<=rungroup[k+1])
            if(runNo!=2487&&runNo!=2456)
              h_runsummedtdc1[hit->GetLayer()][k]->Fill(hit->GetTDC());
      }
      for(Int_t j=0; j<bdc2hit->GetEntries(); j++){
        TArtDCHit* hit = (TArtDCHit*)bdc2hit->At(j);
        h_tdc2[hit->GetLayer()]->Fill(hit->GetTDC());
        h_runtdc2[hit->GetLayer()]->Fill(runNo,hit->GetTDC());
        for(Int_t k=0; k<7; k++)
          if(runNo>rungroup[k]&&runNo<=rungroup[k+1])
            if(runNo!=2487&&runNo!=2456)
              h_runsummedtdc2[hit->GetLayer()][k]->Fill(hit->GetTDC());
      }

    }
    
    runvec.push_back(runNo);
    for(Int_t i=0; i<8; i++){
      Double_t tdcmean = h_tdc1[i]->GetBinCenter(h_tdc1[i]->GetMaximumBin());
      f_tdc->SetParameters(h_tdc1[i]->GetMaximum(),tdcmean,30);
      h_tdc1[i]->Fit(f_tdc,"","",tdcmean-60,tdcmean+60);
      tdc1[i].push_back(f_tdc->GetParameter(1));
      f_tdc->Clear();
      h_tdc1[i]->Clear();
      
      tdcmean = h_tdc2[i]->GetBinCenter(h_tdc2[i]->GetMaximumBin());
      f_tdc->SetParameters(h_tdc2[i]->GetMaximum(),tdcmean,30);
      h_tdc2[i]->Fit(f_tdc,"","",tdcmean-60,tdcmean+60);
      tdc2[i].push_back(f_tdc->GetParameter(1));
      f_tdc->Clear();
      h_tdc2[i]->Clear();
    }


    chain->Clear();
    delete chain;
  }
  c->Clear();
  delete c;


  auto mg_tdc1 = new TMultiGraph("mg_tdc1","BDC1 TDC;RUN;TDC");
  auto mg_tdc2 = new TMultiGraph("mg_tdc2","BDC2 TDC;RUN;TDC");
  TGraph* g_tdc1[8];
  TGraph* g_tdc2[8];
  for(Int_t i=0; i<8; i++){
    g_tdc1[i] = new TGraph(runvec.size());
    g_tdc2[i] = new TGraph(runvec.size());
    for(Int_t j=0; j<runvec.size(); j++){
      g_tdc1[i]->SetPoint(j,runvec.at(j),tdc1[i].at(j));
      g_tdc2[i]->SetPoint(j,runvec.at(j),tdc2[i].at(j));
    }
    g_tdc1[i]->SetName(Form("g_tdc1_%d",i));
    g_tdc2[i]->SetName(Form("g_tdc2_%d",i));
    g_tdc1[i]->SetMarkerStyle(3);
    g_tdc1[i]->SetMarkerColor(i+2);
    g_tdc1[i]->SetLineColor(i+2);
    g_tdc2[i]->SetMarkerStyle(3);
    g_tdc2[i]->SetMarkerColor(i+2);
    g_tdc2[i]->SetLineColor(i+2);

    mg_tdc1->Add(g_tdc1[i]);
    mg_tdc2->Add(g_tdc2[i]);
  }
  
  auto cmg1 = new TCanvas("cmg1","",1000,600);
  mg_tdc1->Draw("ALP");
  auto cmg2 = new TCanvas("cmg2","",1000,600);
  mg_tdc2->Draw("ALP");

  
  TCanvas* crun[4];
  for(Int_t i=0; i<4; i++){
    crun[i] = new TCanvas(Form("crun%d",i),"",1000,600);
    crun[i]->Divide(2,2);
    for(Int_t j=0; j<4; j++){
      crun[i]->cd(j+1);
      if(i<2) h_runtdc1[i*4+j]->Draw("colz");
      else h_runtdc2[i*4+j-8]->Draw("colz");
    }
  }
  
  
  auto out = new TFile(Form(anapath+"supplemental/bdcTDC%d.root",beamtype),"recreate");
  for(Int_t i=0; i<8; i++){
    g_tdc1[i]->Write();
    g_tdc2[i]->Write();
    h_runtdc1[i]->Write();
    h_runtdc2[i]->Write();
  }
  mg_tdc1->Write();
  mg_tdc2->Write();
  
  for(Int_t i=0; i<8; i++){
    for(Int_t j=0; j<7; j++){
      h_runsummedtdc1[i][j]->Write();
      h_runsummedtdc2[i][j]->Write();
    }
  }


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
    if( !gSystem->GetPathInfo( Form(datapath+"run%04d.bdc.root",irun),file) ){
      if(irun==2175||irun==2176||irun==2206||irun==2207||irun==2229||irun==2309) continue;
      runNolist.push_back(irun);
      nrun++;
    }
  }
  isFileLoaded=kTRUE;
}

void loadTrkFiles(Int_t bt){
  
  if(!(bt==108||bt==112)) return 0;
  Int_t start=2174;
  Int_t stop=2509;
  if(bt==112){ start=2520; stop=2653;}

  Int_t nrun=0;
  runNoTrklist.clear();
  FileStat_t file;
  for(Int_t irun=start; irun<=stop; irun++){
    if( !gSystem->GetPathInfo( Form(datapath+"run%04d.bdctrk.root",irun),file) ){
      runNoTrklist.push_back(irun);
      nrun++;
    }
  }
  isTrkFileLoaded=kTRUE;
}
