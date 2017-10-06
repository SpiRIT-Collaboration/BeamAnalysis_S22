Bool_t isFileLoaded=kFALSE;
std::vector<Int_t> runNolist;
const TString anapath="/home/s015/exp/exp1605_s015/anaroot/users/S22_analysis/";
const TString datapath=anapath+"rootfiles/rawdata/";

const Int_t beamtype=112;

void loadFiles(Int_t);

void anaBDCTimeWindow(){

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
  
  TH1* h_runsummedtdc1[8][7]; //bdc1 tdc summed up by run for each layer
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
    TString fname = Form(datapath+"run%d.ridf.root",runNo);
    auto chain = new TChain("chain");
    chain->AddFile(fname.Data(),0,"tree");
    TClonesArray* bdc1hit = NULL;
    TClonesArray* bdc2hit = NULL;
    chain->SetBranchStatus("*",0);
    chain->SetBranchStatus("SAMURAIBDC*",1);
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
  
  
  auto out = new TFile(Form(anapath+"macros_2/supplemental/bdcTDC%d.root",beamtype),"recreate");
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
    if( !gSystem->GetPathInfo( Form(datapath+"run%04d.ridf.root",irun),file) ){
      if(irun==2175||irun==2176||irun==2206||irun==2207||irun==2229||irun==2309) continue;
      runNolist.push_back(irun);
      nrun++;
    }
  }
  isFileLoaded=kTRUE;
}
