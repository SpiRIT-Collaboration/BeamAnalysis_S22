Bool_t isFileLoaded=kFALSE;
std::vector<Int_t> runNolist;
const TString anapath="/home/s015/exp/exp1605_s015/anaroot/users/S22_analysis/";
const TString datapath=anapath+"rootfiles/reco0/";
const Int_t beamtype=108;
void loadFiles(Int_t);
void anaRecoefficiency(){
  
  if(!isFileLoaded) loadFiles(beamtype);

  Int_t start = runNolist.front();
  Int_t stop  = runNolist.back();
  
  TVectorD run(runNolist.size()), AoqGood[2];
  for(Int_t i=0; i<2; i++) AoqGood[i].ResizeTo(runNolist.size());
  
  Int_t count_run=0;
  for(auto runNo: runNolist){
    std::cout<<"RUN"<<runNo<<std::endl; 
    auto beam = new BeamReco();
    TString fname = Form(datapath+"run%04d.reco.root",runNo);
    beam->fChain->AddFile(fname.Data(),0,"tree");
    beam->InitRecoeff();
    
    Int_t count_AoqGood[2]={};
    Int_t count_evnum=beam->GetEntries();
    for(Int_t i=0; i<beam->GetEntries(); i++){
      beam->GetEntry(i);
      if(beam->BigRIPSBeam_aoq[0]>=1&&beam->BigRIPSBeam_aoq[0]<=3) count_AoqGood[0]++;
      if((beam->BigRIPSBeam_aoq[0]>=1&&beam->BigRIPSBeam_aoq[0]<=3)
        ||(beam->BigRIPSBeam_aoq[2]>=1&&beam->BigRIPSBeam_aoq[2]<=3)) count_AoqGood[1]++;
    }
    
    run(count_run)=runNo; 
    AoqGood[0](count_run)=(double)100.*count_AoqGood[0]/count_evnum;
    AoqGood[1](count_run)=(double)100.*count_AoqGood[1]/count_evnum;
    
    count_run++;
    beam->Clear();
  }
  
  TGraph* g[2]; auto mg = new TMultiGraph("mg","");
  for(Int_t i=0; i<2; i++){
    g[i] = new TGraph(run,AoqGood[i]);
    g[i]->SetMarkerColor(3);
    g[i]->SetLineColor(3);
    g[i]->SetMarkerStyle(8);
    mg->Add(g[i]);
  }
  auto c = new TCanvas("c");
  g[1]->Draw("APL"); 

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
