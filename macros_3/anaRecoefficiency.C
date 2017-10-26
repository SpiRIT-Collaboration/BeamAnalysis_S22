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
  
  Double_t betacal[2]={};
  std::ifstream f_betacal;
  f_betacal.open(Form(anapath+"macros_3/supplemental/betaCalibration_%d.txt",beamtype),std::ios::in);
  f_betacal >> betacal[0] >> betacal[1];
  f_betacal.close();
  
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
      Double_t aoq = beam->BigRIPSBeam_aoq[0];
      if(aoq>=1&&aoq<=3) count_AoqGood[0]++;
    
      // if F3 tracking unavailable
      // TOF = L35/beta35/c + L57/beta57/c
      //     = L35/(a*beta57+b)/c + L57/beta57/c
      Double_t brho2 = beam->BigRIPSRIPS_brho[1];
      Double_t ftime = beam->BigRIPSTOF_tof[0];
      Double_t fl1 = beam->BigRIPSTOF_ulength[0];
      Double_t fl2 = beam->BigRIPSTOF_dlength[0];
      Double_t clight = 299.792458; // mm/nsec
      Double_t mnucleon = 931.49432;

      Double_t rbeta2_F57 = ((fl1+betacal[1]*fl2-betacal[0]*clight*ftime)+TMath::Sqrt(pow(fl1+betacal[1]*fl2-betacal[0]*clight*ftime,2)+4.*betacal[0]*betacal[1]*clight*ftime*fl2))/(2.*betacal[1]*clight*ftime);
      Double_t gammab = 1/sqrt(1-pow(rbeta2_F57,2));
      Double_t aoq_F57 = brho2*clight/mnucleon/rbeta2_F57/gammab;
      // apply beta2_F57 only when F3 is dead and F5, F7 are alive.
      if(beam->BigRIPSFocalPlane_X[3]==-99999&&beam->BigRIPSFocalPlane_X[5]!=-99999&&beam->BigRIPSFocalPlane_X[7]!=-99999)
        aoq=aoq_F57;
      if(aoq>=1&&aoq<=3) count_AoqGood[1]++;
    
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
    g[i]->SetMarkerColor(3+i);
    g[i]->SetLineColor(3+i);
    g[i]->SetMarkerStyle(8);
    mg->Add(g[i]);
  }
  auto c = new TCanvas("c");
  mg->Draw("APL"); 
  mg->GetYaxis()->SetRangeUser(40,100);
  c->SetGridy();

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
