void anaPPACTSumGate(){
  
  Int_t beamtype=112; //0: commissioning,  108,112,132,124: beam,  1: cocktail
  Int_t start=2174;
  Int_t stop=2509;
  if(beamtype==112){start=2520; stop=2653;}
  
  Int_t nrun=0;
  std::vector<Int_t> runNolist;
  TString anapath="/home/s015/exp/exp1605_s015/anaroot/users/S22_analysis/";
  TString filepath=anapath+"rootfiles/rawdata/";
  FileStat_t file;
  for(Int_t irun=start; irun<=stop; irun++){
    if( !gSystem->GetPathInfo( Form(filepath+"run%04d.ridf.root",irun),file) ){
      if(!(irun>=2175&&irun<=2185)&&irun!=2206&&irun!=2207&&irun!=2229&&irun!=2309){ // PPAC didin't work at those runs.
        runNolist.push_back(irun);
        nrun++;
        //if(nrun>30) break;
      }
    }
  }
  
  const Int_t nPPAC=12;
  const TString ppacname[nPPAC]={"F3_1A","F3_1B","F3_2A","F3_2B","F5_1A","F5_1B","F5_2A","F5_2B","F7_1A","F7_1B","F7_2A","F7_2B"}; 
  const Int_t   ppacid[nPPAC]  ={4,5,6,7,9,10,11,12,14,15,16,17}; 
  
  TH1D* h_tsumx[12];  // TSumX histograms for each PPAC
  TH1D* h_tsumy[12];  // TSumY
  TH1D* h_tsumxsingle[12];  // TSumX under single beam event by the condition on PPAC nHit
  TH1D* h_tsumysingle[12];  //
  Int_t tsumXrange_0[2]={50,150};
  Int_t tsumXrange_1[2]={120,220};
  Int_t tsumXrange[2];
  for(Int_t ippac=0; ippac<12; ippac++){
    if(ippac<2||ippac>9){tsumXrange[0]=tsumXrange_0[0]; tsumXrange[1]=tsumXrange_0[1];}
    else{tsumXrange[0]=tsumXrange_1[0]; tsumXrange[1]=tsumXrange_1[1];}
    h_tsumx[ippac] = new TH1D(Form("h_tsumx%d",ippac),ppacname[ippac]+" TSumX;TSumX (ns);counts/500ps",200,tsumXrange[0],tsumXrange[1]);
    h_tsumy[ippac] = new TH1D(Form("h_tsumy%d",ippac),ppacname[ippac]+" TSumY;TSumY (ns);counts/500ps",200,50,150);
    h_tsumxsingle[ippac] = new TH1D(Form("h_tsumxsingle%d",ippac),ppacname[ippac]+" TSumX single hit;TSumX (ns);counts/500ps",200,tsumXrange[0],tsumXrange[1]);
    h_tsumysingle[ippac] = new TH1D(Form("h_tsumysingle%d",ippac),ppacname[ippac]+" TSumY single hit;TSumY (ns);counts/500ps",200,50,150);
  }
  
  TString ppacfile="/home/s015/exp/exp1605_s015/anaroot/users/S22_analysis/rootfiles/ppac/";
  for(auto runNo: runNolist){
    std::cout<<"runNo:"<<runNo<<std::endl;
    auto raw = new BeamRaw();
    TString filename = Form(filepath+"run%04d.ridf.root",runNo);
    raw->fChain->AddFile(filename.Data(),0,"tree");
    raw->InitPPACTSum();
    auto chain = new TChain("chain","");
    chain->AddFile(Form(ppacfile+"run%04d.ppac.root",runNo),0,"tree");
    std::vector<Int_t> *TX1raw[nPPAC]={};
    std::vector<Int_t> *TX2raw[nPPAC]={};
    std::vector<Int_t> *TY1raw[nPPAC]={};
    std::vector<Int_t> *TY2raw[nPPAC]={};
    std::vector<Int_t> *TAraw[nPPAC]={};
    for(Int_t i=0; i<nPPAC; i++){
      chain->SetBranchAddress(ppacname[i]+"_TX1",&TX1raw[i]);
      chain->SetBranchAddress(ppacname[i]+"_TY1",&TY1raw[i]);
      chain->SetBranchAddress(ppacname[i]+"_TX2",&TX2raw[i]);
      chain->SetBranchAddress(ppacname[i]+"_TY2",&TY2raw[i]);
      chain->SetBranchAddress(ppacname[i]+"_TA",&TAraw[i]);
    }

    for(Int_t i=0; i<raw->fChain->GetEntries(); i++){
      raw->GetEntry(i);
      chain->GetEntry(i);

      Int_t nHit[nPPAC];
      for(Int_t ippac=0; ippac<nPPAC; ippac++){
        nHit[ippac] = TX1raw[ippac]->size() + TX2raw[ippac]->size() + TY1raw[ippac]->size() + TY2raw[ippac]->size() + TAraw[ippac]->size();
        h_tsumx[ippac]->Fill(raw->BigRIPSPPAC_fTSumX[ppacid[ippac]]);
        if(nHit[ippac]>10&&nHit[ippac]<25)
          h_tsumxsingle[ippac]->Fill(raw->BigRIPSPPAC_fTSumX[ppacid[ippac]]);

        h_tsumy[ippac]->Fill(raw->BigRIPSPPAC_fTSumY[ppacid[ippac]]);
        if(nHit[ippac]>10&&nHit[ippac]<25)
          h_tsumysingle[ippac]->Fill(raw->BigRIPSPPAC_fTSumY[ppacid[ippac]]);

      }

    }
    raw->Clear();
    chain->Clear();
  }

/*
  auto cx = new TCanvas("cx","",1200,600);
  cx->Divide(4,3);
  for(Int_t ippac=0; ippac<12; ippac++){
    cx->cd(ippac+1)->SetLogy();
    h_tsumx[ippac]->Draw();
  }

  auto cy = new TCanvas("cy","",1200,600);
  cy->Divide(4,3);
  for(Int_t ippac=0; ippac<12; ippac++){
    cy->cd(ippac+1)->SetLogy();
    h_tsumy[ippac]->Draw();
  }
  */

  
  std::ofstream out;
  out.open(Form(anapath+"macros_2/supplemental/ppactsumgate_%d.txt",beamtype),std::ios::out);
 
  auto f_tsum = new TF1("f_tsum","gaus");
  auto cxsingle = new TCanvas("cxsingle","",1200,600);
  cxsingle->Divide(4,3);
  auto cysingle = new TCanvas("cysingle","",1200,600);
  cysingle->Divide(4,3);
  for(Int_t ippac=0; ippac<12; ippac++){
    cxsingle->cd(ippac+1)->SetLogy();
    h_tsumxsingle[ippac]->Draw();
    Int_t constant = h_tsumxsingle[ippac]->GetMaximum();
    Double_t mean = h_tsumxsingle[ippac]->GetBinCenter(h_tsumxsingle[ippac]->GetMaximumBin());
    Double_t fwhm = 0.5*(h_tsumxsingle[ippac]->GetBinCenter(h_tsumxsingle[ippac]->FindLastBinAbove(constant*0.5))-h_tsumxsingle[ippac]->GetBinCenter(h_tsumxsingle[ippac]->FindFirstBinAbove(0.5*constant)));
    f_tsum->SetParameters(constant,mean,fwhm);                      
    h_tsumxsingle[ippac]->Fit(f_tsum,"","",mean-3*fwhm,mean+3*fwhm);
    
    Double_t tsumxmin = f_tsum->GetParameter(1) - 5.*f_tsum->GetParameter(2);
    Double_t tsumxmax = f_tsum->GetParameter(1) + 5.*f_tsum->GetParameter(2);
    
      
    cysingle->cd(ippac+1)->SetLogy();
    h_tsumysingle[ippac]->Draw();
    constant = h_tsumysingle[ippac]->GetMaximum();
    mean = h_tsumysingle[ippac]->GetBinCenter(h_tsumysingle[ippac]->GetMaximumBin());
    fwhm = 0.5*(h_tsumysingle[ippac]->GetBinCenter(h_tsumysingle[ippac]->FindLastBinAbove(constant*0.5))-h_tsumysingle[ippac]->GetBinCenter(h_tsumysingle[ippac]->FindFirstBinAbove(0.5*constant)));
    f_tsum->SetParameters(constant,mean,fwhm);
    h_tsumysingle[ippac]->Fit(f_tsum,"","",mean-3*fwhm,mean+3*fwhm);
    
    Double_t tsumymin = f_tsum->GetParameter(1) - 5.*f_tsum->GetParameter(2);
    Double_t tsumymax = f_tsum->GetParameter(1) + 5.*f_tsum->GetParameter(2);
    
    out << tsumxmin << "," << tsumxmax << "," << tsumymin << "," << tsumymax << std::endl;
  }

 

}
