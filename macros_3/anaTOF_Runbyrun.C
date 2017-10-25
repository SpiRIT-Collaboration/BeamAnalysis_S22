void anaTOF_Runbyrun(){
  
  Int_t beamtype=108; //0: commissioning,  108,112,132,124: beam,  1: cocktail
  
  Int_t start=2174;
  Int_t stop=2509;
  if(beamtype==112){start=2520; stop=2653;}

  Int_t nrun=0;
  std::vector<Int_t> runNolist;
  TString filepath="/home/s015/exp/exp1605_s015/anaroot/users/S22_analysis/rootfiles/reco0/";
 
  FileStat_t file;
  for(Int_t irun=start; irun<=stop; irun++){
    if( !gSystem->GetPathInfo( Form(filepath+"run%04d.reco.root",irun),file) ){
      runNolist.push_back(irun);
      nrun++;
      //if(nrun>10) break;
    }
  }
  
  TString planame[2]={"F3","F7"}; 
  Int_t plaid[2]={0,2}; 
  
  start=runNolist.front();
  stop=runNolist.back();
  TH1D* h_stof = new TH1D("h_stof","slew TOF;slew TOF (ns);counts/50ps",200,232,242);
  TH1D* h_tof = new TH1D("h_tof","raw TOF;raw TOF (ns);counts/50ps",200,232,242);
  Double_t aoqrange[2]={2.1,2.2};
  if(beamtype==112){aoqrange[0]=2.2; aoqrange[1]=2.3;}
  TH1D* h_aoq = new TH1D("h_aoq","AoQ;AoQ;counts",400,aoqrange[0],aoqrange[1]);
  TH1D* h_beta = new TH1D("h_beta","Beta;Beta;counts",400,0.64,0.68);
  auto  h_runbeta = new TH2D("h_runbeta","Beta vs run;RUN;Beta",stop+1-start,start,stop+1,200,0.64,0.68);
  auto  h_runstof = new TH2D("h_runstof","slew TOF vs run;RUN;slew TOF (ns)",stop+1-start,start,stop+1,200,232,242);
  
  TH2D* h_sqsumaoq = new TH2D("h_sqsumaoq","Sqsum vs AoQ;AoQ;Sqsum",400,aoqrange[0],aoqrange[1],4800,3600,4800);
  TH2D* h_sqsumstof = new TH2D("h_sqsumstof","Sqsum vs slew TOF;slew TOF (ns);Sqsum",400,232,242,2400,3600,4800);
  
  std::vector<Int_t> runnum;
  std::vector<Double_t> tofmean;
  std::vector<Double_t> stofmean;
  TH1D* hg_tof;
  TH1D* hg_stof;
  TF1* fg_tof = new TF1("fg_tof","gaus");
  auto cvs = new TCanvas("cvs","");
  
  for(auto runNo: runNolist){
    std::cout<<"RUN"<<runNo<<std::endl; 
    auto beam = new BeamReco();
    TString fname = Form(filepath+"run%04d.reco.root",runNo);
    beam->fChain->AddFile(fname.Data(),0,"tree");
    beam->InitTOF();
    beam->InitIC();
    
    
    hg_tof = new TH1D("hg_tof","",100,230,240);
    hg_stof = new TH1D("hg_stof","",100,230,240);
    
    for(Int_t i=0; i<beam->fChain->GetEntries(); i++){
      beam->fChain->GetEntry(i);
      Double_t TOF = 305.+beam->BigRIPSPlastic_fTime[2]-beam->BigRIPSPlastic_fTime[0];
      Double_t sTOF = beam->BigRIPSTOF_tof[0];
      
      h_stof->Fill(sTOF); 
      h_tof->Fill(TOF); 
      h_aoq->Fill(beam->BigRIPSBeam_aoq[0]); 
      h_sqsumaoq->Fill(beam->BigRIPSBeam_aoq[0],beam->BigRIPSIC_fRawADCSqSum[2]); 
      h_sqsumstof->Fill(sTOF,beam->BigRIPSIC_fRawADCSqSum[2]); 
      h_beta->Fill(beam->BigRIPSBeam_beta[0]); 
      hg_tof->Fill(TOF); 
      hg_stof->Fill(sTOF);
      h_runstof->Fill(runNo,sTOF);
      h_runbeta->Fill(runNo,beam->BigRIPSBeam_beta[0]);

    }
    
    runnum.push_back(runNo);
    //cvs->cd();
    hg_tof->Draw();
    fg_tof->SetParameters(hg_tof->GetMaximum(),hg_tof->GetBinCenter(hg_tof->GetMaximumBin()),0.2);
    Double_t maxpos = hg_tof->GetBinCenter(hg_tof->GetMaximumBin());
    hg_tof->Fit(fg_tof,"OQ","",maxpos-0.6,maxpos+0.6);
    tofmean.push_back(fg_tof->GetParameter(1));
    hg_tof->Clear();
    hg_tof=NULL;

    hg_stof->Draw();
    fg_tof->SetParameters(hg_stof->GetMaximum(),hg_stof->GetBinCenter(hg_stof->GetMaximumBin()),0.15);
    maxpos = hg_stof->GetBinCenter(hg_stof->GetMaximumBin());
    hg_stof->Fit(fg_tof,"OQ","",maxpos-0.6,maxpos+0.6);
    stofmean.push_back(fg_tof->GetParameter(1));
    hg_stof->Clear();
    hg_stof=NULL;
    
    
    beam->Clear();
    
  }

  auto c = new TCanvas("c","");
  h_stof->Draw();
  fg_tof->SetParameters(h_stof->GetMaximum(),h_stof->GetBinCenter(h_stof->GetMaximumBin()),0.15);
  h_stof->Fit("fg_tof","","");
  
  auto c00 = new TCanvas("c00","");
  h_tof->Draw();
  fg_tof->SetParameters(h_tof->GetMaximum(),h_tof->GetBinCenter(h_tof->GetMaximumBin()),0.15);
  h_tof->Fit("fg_tof","","");

  auto c1 = new TCanvas("c1","");
  auto g_tof = new TGraph(runnum.size());
  auto g_stof = new TGraph(runnum.size());
  for(Int_t i=0; i<runnum.size(); i++){
    g_tof->SetPoint(i,runnum.at(i),tofmean.at(i));
    g_stof->SetPoint(i,runnum.at(i),stofmean.at(i));
  }
  auto mg_tof = new TMultiGraph("mg_tof","Reconstrunted TOF");
  g_tof->SetMarkerStyle(3);
  g_stof->SetMarkerStyle(3);
  g_stof->SetMarkerColor(kRed);
  g_stof->SetLineColor(kRed);
  mg_tof->Add(g_tof);
  mg_tof->Add(g_stof);
  mg_tof->Draw("APL");
  
  auto c2 = new TCanvas("c2","");
  h_aoq->Draw();
  auto c3 = new TCanvas("c3","");
  h_beta->Draw();
  auto c4 = new TCanvas("c4","");
  h_sqsumaoq->Draw("colz");
  auto c5 = new TCanvas("c5","");
  h_sqsumstof->Draw("colz");
  auto c6 = new TCanvas("c6","");
  h_runbeta->Draw("colz");
  auto c7 = new TCanvas("c7","");
  h_runstof->Draw("colz");

}
