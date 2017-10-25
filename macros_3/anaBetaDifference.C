Bool_t isFileLoaded=kFALSE;
std::vector<Int_t> runNolist;
const TString anapath="/home/s015/exp/exp1605_s015/anaroot/users/S22_analysis/";
const TString datapath=anapath+"rootfiles/reco0/";
const Int_t beamtype=108;
void loadFiles(Int_t);
void estimateGaussPara(TH1*,Int_t*,Double_t*,Double_t*);
void anaBetaDifference(){
  
  if(!isFileLoaded) loadFiles(beamtype);

  Int_t start = runNolist.front();
  Int_t stop  = runNolist.back();
  
  TH2D* h_beta35_beta57 = new TH2D("h_beta35_beta57",";beta57;beta35",500,0.64,0.69,500,0.64,0.69);
  
  for(auto runNo: runNolist){
    std::cout<<"RUN"<<runNo<<std::endl; 
    auto beam = new BeamReco();
    TString fname = Form(datapath+"run%04d.reco.root",runNo);
    beam->fChain->AddFile(fname.Data(),0,"tree");
    beam->InitBetaDif();
    //beam->Init();
  
    for(Int_t i=0; i<beam->GetEntries(); i++){
      beam->GetEntry(i);
      Double_t aoq = beam->BigRIPSBeam_aoq[0];
      if(aoq<1||aoq>3) continue; 
      
      Double_t brho1 = beam->BigRIPSRIPS_brho[0];
      Double_t brho2 = beam->BigRIPSRIPS_brho[1];
      Double_t ftime = beam->BigRIPSTOF_tof[0];
      Double_t fl1 = beam->BigRIPSTOF_ulength[0];
      Double_t fl2 = beam->BigRIPSTOF_dlength[0];
      Double_t rbeta1;
      Double_t rbeta2;
      Double_t clight = 299.792458; // mm/nsec

      Double_t alpha  = brho2/brho1;
      Double_t a1     = TMath::Sqrt(alpha*alpha * clight*clight * ftime*ftime
          + (TMath::Power(alpha,4) - alpha*alpha) * fl1*fl1
          + (1 - alpha*alpha) * fl2*fl2);

      rbeta1 = ( a1*fl1 + fl2*clight*ftime ) / ( a1*clight*ftime + (1-alpha*alpha)*fl1*fl2 );
      rbeta2  = ( a1*fl1 + fl2*clight*ftime )/ ( clight*clight * ftime*ftime + (alpha*alpha-1)*fl1*fl1 );

      h_beta35_beta57->Fill(rbeta2,rbeta1);
      
    }
    
    
    beam->Clear();
  }
  
  auto c = new TCanvas("c","");
  h_beta35_beta57->Draw("scat");
  auto f = new TF1("f","pol1");
  h_beta35_beta57->Fit("f");
  
  Double_t a = f->GetParameter(1), b = f->GetParameter(0); // for F5F7 reco.
  Double_t a2 = 1./a, b2=-1.*b/a; // for F3F5 reco. 
  
  TH1 *h_aoq[3];
  TH1D* h_beta57[2];
  for(Int_t i=0; i<2; i++) h_beta57[i] = new TH1D(Form("h_beta57%d",i),";beta57;",500,0.64,0.69); // conventional and beta35 estimate
  Double_t aoqrange[2]={2.1,2.22};
  if(beamtype==112){ aoqrange[0]=2.18; aoqrange[1]=2.3;}
  for(Int_t i=0; i<3; i++) h_aoq[i] = new TH1D(Form("h_aoq%d",i),"",500,aoqrange[0],aoqrange[1]);
  auto h_aoq_out_F57 = new TH1D("h_aoq_out_F57","",500,aoqrange[0],aoqrange[1]);
  auto h_aoq_out_F35 = new TH1D("h_aoq_out_F35","",500,aoqrange[0],aoqrange[1]);
  TH2D* h_aoq2_F57 = new TH2D("h_aoq2_F57",";aoq_conventional;aoqF57",500,aoqrange[0],aoqrange[1],500,aoqrange[0],aoqrange[1]);
  TH2D* h_aoq2_F35 = new TH2D("h_aoq2_F35",";aoq_conventional;aoqF35",500,aoqrange[0],aoqrange[1],500,aoqrange[0],aoqrange[1]);
  
  for(auto runNo: runNolist){
    std::cout<<"RUN"<<runNo<<std::endl; 
    auto beam = new BeamReco();
    TString fname = Form(datapath+"run%04d.reco.root",runNo);
    beam->fChain->AddFile(fname.Data(),0,"tree");
    beam->InitBetaDif();
  
    for(Int_t i=0; i<beam->GetEntries(); i++){
      beam->GetEntry(i);
      Double_t aoq = beam->BigRIPSBeam_aoq[0];

      Double_t brho1 = beam->BigRIPSRIPS_brho[0];
      Double_t brho2 = beam->BigRIPSRIPS_brho[1];
      Double_t ftime = beam->BigRIPSTOF_tof[0];
      Double_t fl1 = beam->BigRIPSTOF_ulength[0];
      Double_t fl2 = beam->BigRIPSTOF_dlength[0];
      Double_t clight = 299.792458; // mm/nsec
      Double_t mnucleon = 931.49432;

      // if F3 tracking unavailable
      // TOF = L35/beta35/c + L57/beta57/c
      //     = L35/(a*beta57+b)/c + L57/beta57/c
      Double_t rbeta2_F57 = ((fl1+a*fl2-b*clight*ftime)+TMath::Sqrt(pow(fl1+a*fl2-b*clight*ftime,2)+4.*a*b*clight*ftime*fl2))/(2.*a*clight*ftime);
      Double_t rbeta2_F35 = ((fl2+a2*fl1-b2*clight*ftime)+TMath::Sqrt(pow(fl2+a2*fl1-b2*clight*ftime,2)+4.*a2*b2*clight*ftime*fl1))/(2.*a2*clight*ftime);
      Double_t gammab = 1/sqrt(1-pow(rbeta2_F57,2));
      Double_t gammaf = 1/sqrt(1-pow(rbeta2_F35,2));
      Double_t aoq_F57 = brho2*clight/mnucleon/rbeta2_F57/gammab;
      Double_t aoq_F35 = brho1*clight/mnucleon/rbeta2_F35/gammaf;
      

      if(beam->BigRIPSFocalPlane_X[3]!=-99999&&beam->BigRIPSFocalPlane_X[5]!=-99999&&beam->BigRIPSFocalPlane_X[7]!=-99999){ 
        h_aoq[0]->Fill(aoq);
        h_aoq[1]->Fill(aoq_F57);
        h_aoq[2]->Fill(aoq_F35);
        h_aoq2_F57->Fill(aoq,aoq_F57); 
        h_aoq2_F35->Fill(aoq,aoq_F35);
        h_beta57[0]->Fill(rbeta2_F57);
      }
      if(beam->BigRIPSFocalPlane_X[3]==-99999&&beam->BigRIPSFocalPlane_X[5]!=-99999&&beam->BigRIPSFocalPlane_X[7]!=-99999)
          h_aoq_out_F57->Fill(aoq_F57);
      if(beam->BigRIPSFocalPlane_X[3]!=-99999&&beam->BigRIPSFocalPlane_X[5]!=-99999&&beam->BigRIPSFocalPlane_X[7]==-99999)
          h_aoq_out_F35->Fill(aoq_F35);
      
      
      Double_t alpha  = brho2/brho1;
      Double_t a1     = TMath::Sqrt(alpha*alpha * clight*clight * ftime*ftime
          + (TMath::Power(alpha,4) - alpha*alpha) * fl1*fl1
          + (1 - alpha*alpha) * fl2*fl2);

      Double_t rbeta2  = ( a1*fl1 + fl2*clight*ftime )/ ( clight*clight * ftime*ftime + (alpha*alpha-1)*fl1*fl1 );
      if(beam->BigRIPSFocalPlane_X[3]!=-99999&&beam->BigRIPSFocalPlane_X[5]!=-99999&&beam->BigRIPSFocalPlane_X[7]!=-99999){ 
        h_beta57[1]->Fill(rbeta2);
      }
      
    }
    
    
    beam->Clear();
  }
  auto c1 = new TCanvas("c1","");
  h_aoq[0]->Draw();
  h_aoq[1]->SetLineColor(kRed);
  h_aoq[1]->Draw("same");
  h_aoq[2]->SetLineColor(kGreen);
  h_aoq[2]->Draw("same");
  
  auto c6 = new TCanvas("c6","");
  h_beta57[0]->Draw();
  h_beta57[1]->SetLineColor(kRed);
  h_beta57[1]->Draw("same");
  
  auto c4 = new TCanvas("c4",""); 
  h_aoq[0]->Draw();
  h_aoq_out_F57->SetLineColor(kCyan);
  h_aoq_out_F57->Scale(h_aoq[0]->GetEntries()/h_aoq_out_F57->GetEntries());
  h_aoq_out_F57->Draw("hist same");
  
  
  auto c5 = new TCanvas("c5",""); 
  h_aoq[0]->Draw();
  h_aoq_out_F35->SetLineColor(kMagenta);
  h_aoq_out_F35->Scale(h_aoq[0]->GetEntries()/h_aoq_out_F35->GetEntries());
  h_aoq_out_F35->Draw("hist same");

  auto c2 = new TCanvas("c2","");
  h_aoq2_F57->Draw("colz");
  auto c3 = new TCanvas("c3","");
  h_aoq2_F35->Draw("colz");
  
  std::ofstream fout;
  fout.open(Form(anapath+"macros_3/supplemental/betaCalibration_%d.txt",beamtype),std::ios::out);
  fout << b << " " << a << std::endl;
  fout.close();
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
