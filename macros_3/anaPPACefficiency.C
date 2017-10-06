Bool_t isFileLoaded=kFALSE;
std::vector<Int_t> runNolist;
const TString anapath="/home/s015/exp/exp1605_s015/anaroot/users/S22_analysis/";
const TString datapath=anapath+"rootfiles/reco0/";
const Int_t ppacid[12]={4,5,6,7,9,10,11,12,14,15,16,17}; 

const Int_t beamtype=108;
void loadFiles(Int_t);

void anaPPACefficiency(){
  
  if(!isFileLoaded) loadFiles(beamtype);
  

  Int_t start = runNolist.front();
  Int_t stop  = runNolist.back();
  
  TVectorD run(runNolist.size()), F[3], FX[3], FY[3], pxf[12], pyf[12];
  for(Int_t i=0; i<3; i++){F[i].ResizeTo(runNolist.size()); FX[i].ResizeTo(runNolist.size()); FY[i].ResizeTo(runNolist.size());} 
  for(Int_t i=0; i<12; i++){pxf[i].ResizeTo(runNolist.size()); pyf[i].ResizeTo(runNolist.size());} 
  
  Int_t count_run=0;
  for(auto runNo: runNolist){
    std::cout<<"RUN"<<runNo<<std::endl; 
    auto beam = new BeamReco();
    TString fname = Form(datapath+"run%04d.reco.root",runNo);
    beam->fChain->AddFile(fname.Data(),0,"tree");
    beam->InitPPACeff();
    
    Int_t count_F[3]={0,0,0}, count_FX[3]={0,0,0}, count_FY[3]={0,0,0};
    Int_t count_pxf[12], count_pyf[12]; for(Int_t i=0; i<12; i++){count_pxf[i]=0; count_pyf[i]=0;}
    Int_t count_evnum=beam->GetEntries();
    for(Int_t i=0; i<beam->GetEntries(); i++){
      beam->GetEntry(i);
      for(Int_t j=0; j<3; j++){
        if(beam->BigRIPSFocalPlane_X[2*j+3]!=-99999) count_FX[j]++;
        if(beam->BigRIPSFocalPlane_Y[2*j+3]!=-99999) count_FY[j]++;
        if(beam->BigRIPSFocalPlane_X[2*j+3]!=-99999&&beam->BigRIPSFocalPlane_Y[2*j+3]!=-99999) count_F[j]++;
      }
      for(Int_t j=0; j<12; j++){
        if(beam->BigRIPSPPAC_fFiredX[ppacid[j]]) count_pxf[j]++;
        if(beam->BigRIPSPPAC_fFiredY[ppacid[j]]) count_pyf[j]++;
      }
    }
    
    run(count_run)=runNo; 
    for(Int_t i=0; i<3; i++){
      F[i](count_run)=(double)100.*count_F[i]/count_evnum;
      FX[i](count_run)=(double)100.*count_FX[i]/count_evnum;
      FY[i](count_run)=(double)100.*count_FY[i]/count_evnum;
    }
    for(Int_t i=0; i<12; i++){
      pxf[i](count_run)=(double)100.*count_pxf[i]/count_evnum;
      pyf[i](count_run)=(double)100.*count_pyf[i]/count_evnum;
    }
    
     
    
    count_run++;
    beam->Clear();
  }
  
  TGraph *g_F[3], *g_FX[3], *g_FY[3], *g_ppacfiredx[12], *g_ppacfiredy[12];
  auto mg_F = new TMultiGraph("mg_F","");
  auto mg_FX = new TMultiGraph("mg_FX","");
  auto mg_FY = new TMultiGraph("mg_FY","");
  auto mg_pxf = new TMultiGraph("mg_pxf","");
  auto mg_pyf = new TMultiGraph("mg_pyf","");
  for(Int_t i=0; i<3; i++){
    g_F[i] = new TGraph(run,F[i]);
    g_FX[i] = new TGraph(run,FX[i]);
    g_FY[i] = new TGraph(run,FY[i]);
    g_F[i]->SetLineColor(i+2); g_F[i]->SetMarkerStyle(3); g_F[i]->SetMarkerColor(i+2);
    g_FX[i]->SetLineColor(i+2); g_FX[i]->SetMarkerStyle(3); g_FX[i]->SetMarkerColor(i+2);
    g_FY[i]->SetLineColor(i+2); g_FY[i]->SetMarkerStyle(3); g_FY[i]->SetMarkerColor(i+2);
    mg_F->Add(g_F[i]); mg_FX->Add(g_FX[i]); mg_FY->Add(g_FY[i]);
  }
  for(Int_t i=0; i<12; i++){
    g_ppacfiredx[i] = new TGraph(run,pxf[i]);
    g_ppacfiredy[i] = new TGraph(run,pyf[i]);
    g_ppacfiredx[i]->SetLineColor(i+2); g_ppacfiredx[i]->SetMarkerStyle(3); g_ppacfiredx[i]->SetMarkerColor(i+2);
    g_ppacfiredy[i]->SetLineColor(i+2); g_ppacfiredy[i]->SetMarkerStyle(3); g_ppacfiredy[i]->SetMarkerColor(i+2);
    mg_pxf->Add(g_ppacfiredx[i]); mg_pyf->Add(g_ppacfiredy[i]);
  }

  TCanvas *c[5];
  for(Int_t i=0; i<5; i++){
    c[i] = new TCanvas(Form("c%d",i),"",1000,600);
    c[i]->SetGridy();
  }
  c[0]->cd(); mg_FX->Draw("APL");
  c[1]->cd(); mg_FY->Draw("APL");
  c[2]->cd(); mg_pxf->Draw("APL");
  c[3]->cd(); mg_pyf->Draw("APL");
  c[4]->cd(); mg_F->Draw("APL");




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
      
      //if(goodrun)
      runNolist.push_back(irun); // runs in reco/ dir. should be already selected 
    }
  }
  
  isFileLoaded=kTRUE;
}
