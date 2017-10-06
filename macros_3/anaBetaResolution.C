Bool_t isFileLoaded=kFALSE;
std::vector<Int_t> runNolist;
const TString anapath="/home/s015/exp/exp1605_s015/anaroot/users/S22_analysis/";
const TString datapath=anapath+"rootfiles/reco0/";
const Int_t beamtype=108;
void loadFiles(Int_t);
void estimateGaussPara(TH1*,Int_t*,Double_t*,Double_t*);
void anaBetaResolution(){
  
  if(!isFileLoaded) loadFiles(beamtype);

  Int_t start = runNolist.front();
  Int_t stop  = runNolist.back();
  
  TH1 *h_beta[3], *h_b[3];
  TH1 *h_beta_F357Good[2]; // F357 and F57 comparison when AoQ357 is good.
  TVectorD run(runNolist.size()), betares[3];
  for(Int_t i=0; i<3; i++){
    h_beta[i] = new TH1D(Form("h_beta%d",i),"",500,0.64,0.69);
    if(i<2) h_beta_F357Good[i] = new TH1D(Form("h_beta_F357Good%d",i),"",500,0.64,0.69);
    betares[i].ResizeTo(runNolist.size());
  }
  auto f = new TF1("f","gaus");

  Int_t count_run=0;
  for(auto runNo: runNolist){
    std::cout<<"RUN"<<runNo<<std::endl; 
    auto beam = new BeamReco();
    TString fname = Form(datapath+"run%04d.reco.root",runNo);
    beam->fChain->AddFile(fname.Data(),0,"tree");
    beam->InitBetaRes();
  
    for(Int_t i=0; i<3; i++){
      h_b[i] = new TH1D(Form("h_b%d",i),"",200,0.64,0.69);
    }
    
    for(Int_t i=0; i<beam->GetEntries(); i++){
      beam->GetEntry(i);
      h_beta[0]->Fill(beam->BigRIPSBeam_beta[0]);
      h_beta[1]->Fill(beam->BigRIPSBeam_beta[2]);
      if(beam->BigRIPSBeam_aoq[0]>=1&&beam->BigRIPSBeam_aoq[0]<=3){
        h_beta[2]->Fill(beam->BigRIPSBeam_beta[0]);
        h_beta_F357Good[0]->Fill(beam->BigRIPSBeam_beta[0]);
        h_beta_F357Good[1]->Fill(beam->BigRIPSBeam_beta[2]);
      }
      else if(beam->BigRIPSBeam_aoq[2]>=1&&beam->BigRIPSBeam_aoq[2]<=3) h_beta[2]->Fill(beam->BigRIPSBeam_beta[2]);
      else h_beta[2]->Fill(beam->BigRIPSBeam_beta[0]);
      
      // run by run
      h_b[0]->Fill(beam->BigRIPSBeam_beta[0]);
      h_b[1]->Fill(beam->BigRIPSBeam_beta[2]);
      if(beam->BigRIPSBeam_aoq[0]>=1&&beam->BigRIPSBeam_aoq[0]<=3) h_b[2]->Fill(beam->BigRIPSBeam_beta[0]);
      else if(beam->BigRIPSBeam_aoq[2]>=1&&beam->BigRIPSBeam_aoq[2]<=3) h_b[2]->Fill(beam->BigRIPSBeam_beta[2]);
      else h_b[2]->Fill(beam->BigRIPSBeam_beta[0]);
    }
    
    run(count_run)=runNo; 
    for(Int_t i=0; i<3; i++){
      Int_t constant; Double_t mean, sigma;
      estimateGaussPara(h_b[i],&constant,&mean,&sigma);
      f->SetParameters(constant, mean, sigma);
      h_b[i]->Fit(f,"QO","",mean-3.*sigma,mean+3.*sigma);
      betares[i](count_run)=f->GetParameter(2);
      h_b[i]->Clear();
    }
    count_run++;
    beam->Clear();
  }
  
  TGraph *g[3]; auto mg = new TMultiGraph("mg","");
  auto c = new TCanvas("c");
  c->Divide(2,2);
  Int_t constant; Double_t mean,sigma;
  for(Int_t i=0; i<3; i++){
    c->cd(i+1);
    h_beta[i]->Draw();
    estimateGaussPara(h_beta[i],&constant,&mean,&sigma);
    f->SetParameters(constant,mean,sigma);
    h_beta[i]->Fit(f,"","",mean-3.*sigma,mean+3.*sigma);
    
    g[i] = new TGraph(run,betares[i]);
    g[i]->SetLineColor(i+2);
    g[i]->SetMarkerStyle(3);
    g[i]->SetMarkerColor(i+2);
    mg->Add(g[i]);
  }

  auto c2 = new TCanvas("c2");
  mg->Draw("APL"); 
  

  auto c3 = new TCanvas("c3");
  c3->Divide(1,2);
  for(Int_t i=0; i<2; i++){
    c3->cd(i+1); h_beta_F357Good[i]->Draw();
    estimateGaussPara(h_beta_F357Good[i],&constant,&mean,&sigma);
    f->SetParameters(constant,mean,sigma);
    h_beta_F357Good[i]->Fit(f,"","",mean-3.*sigma,mean+3.*sigma);
  }
}

void loadFiles(Int_t bt){
  if( !(bt==108||bt==112) ) return 0;
  Int_t start=2230;
  Int_t stop=2509;
  if(bt==112){ start=2520; stop=2653;}

  runNolist.clear();
  FileStat_t file;
  
  for(Int_t irun=start; irun<=stop; irun++){
    if( !gSystem->GetPathInfo( Form(datapath+"run%04d.reco.root",irun),file) ){   
      if(irun>=2471&&irun<=2475) continue;
      if(irun>=2520&&irun<=2530) continue;
      
      runNolist.push_back(irun); // runs in reco/ dir. should be already selected 
    }
  }
  
  isFileLoaded=kTRUE;
}
void estimateGaussPara(TH1* h, Int_t* c, Double_t* m, Double_t* f){
  *c = h->GetMaximum();
  *m = h->GetBinCenter(h->GetMaximumBin());
  *f = (h->GetBinCenter(h->FindLastBinAbove(0.5*((double)*c)))-h->GetBinCenter(h->FindFirstBinAbove(0.5*((double)*c))))/2.35;
}
