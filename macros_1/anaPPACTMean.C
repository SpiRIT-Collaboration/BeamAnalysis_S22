Bool_t isFileLoaded=kFALSE;
std::vector<Int_t> runNolist;
const TString anapath="/home/s015/exp/exp1605_s015/anaroot/users/S22_analysis/";
const TString datapath=anapath+"rootfiles/ppac/";

const Int_t nPPAC=12;
TString ppacname[12]={"F3_1A","F3_1B","F3_2A","F3_2B","F5_1A","F5_1B","F5_2A","F5_2B","F7_1A","F7_1B","F7_2A","F7_2B"}; 
Int_t ppacid[12]={4,5,6,7,9,10,11,12,14,15,16,17}; 
const Int_t beamtype=108;

void loadFiles(Int_t bt){
  
  if(!(bt==108||bt==112)) return;
  Int_t start=2174;
  Int_t stop=2509;
  if(bt==112){ start=2520; stop=2653;}

  Int_t nrun=0;
  runNolist.clear();
  FileStat_t file;
  for(Int_t irun=start; irun<=stop; irun++){
    if( !gSystem->GetPathInfo( Form(datapath+"run%04d.ppac.root",irun),file) ){
      if(!(irun>=2175&&irun<=2185)&&irun!=2206&&irun!=2207&&irun!=2229&&irun!=2309){ // PPAC didin't work at those runs.
        runNolist.push_back(irun);
        nrun++;
      }
      //if(nrun>50) break;
    }
  }
  isFileLoaded=kTRUE;
}

Double_t GetGaussMean(TH1* h){

  Int_t constant = h->GetMaximum();
  Double_t mean = h->GetBinCenter(h->GetMaximumBin());
  Double_t fwhm = 0.5*(h->GetBinCenter(h->FindLastBinAbove(0.5*constant)) - h->GetBinCenter(h->FindFirstBinAbove(0.5*constant)));
  auto f = new TF1("f","gaus");
  f->SetParameters(constant,mean,fwhm);
  h->Fit(f,"Q","",mean-2*fwhm,mean+2*fwhm);
  Double_t gmean = f->GetParameter(1);
  if(h->GetEntries()<=100) gmean = mean;
  return gmean;
}

Double_t GetRecMean(TH1* h){
  Int_t constant = h->GetMaximum();
  Double_t mean = h->GetBinCenter(h->GetMaximumBin());
  Double_t maxedge = (double)h->GetBinCenter(h->FindLastBinAbove(0.5*constant));
  Double_t minedge = (double)h->GetBinCenter(h->FindFirstBinAbove(0.5*constant));
  return (double)(maxedge+minedge)/2.;
}

void anaPPACTMean(){
  
  if(!isFileLoaded) loadFiles(beamtype); 
  

  TH2* h_runtx1[12];
  TH2* h_runtx2[12];
  TH2* h_runty1[12];
  TH2* h_runty2[12];
  TH2* h_runta[12];
  TVectorD runvec(runNolist.size());
  TVectorD tx1vec[12];
  TVectorD tx2vec[12];
  TVectorD ty1vec[12];
  TVectorD ty2vec[12];
  TVectorD tavec[12];
  for(Int_t i=0; i<12; i++){
    h_runtx1[i] = new TH2I(Form("h_runtx1_%d",i),ppacname[i]+"TX1 vs run",runNolist.back()+1-runNolist.front(),runNolist.front(),runNolist.back()+1,3200,0,160000);
    h_runtx2[i] = new TH2I(Form("h_runtx2_%d",i),ppacname[i]+"TX2 vs run",runNolist.back()+1-runNolist.front(),runNolist.front(),runNolist.back()+1,3200,0,160000);
    h_runty1[i] = new TH2I(Form("h_runty1_%d",i),ppacname[i]+"TY1 vs run",runNolist.back()+1-runNolist.front(),runNolist.front(),runNolist.back()+1,3200,0,160000);
    h_runty2[i] = new TH2I(Form("h_runty2_%d",i),ppacname[i]+"TY2 vs run",runNolist.back()+1-runNolist.front(),runNolist.front(),runNolist.back()+1,3200,0,160000);
    h_runta[i]  = new TH2I(Form("h_runta_%d",i), ppacname[i]+"TA vs run",runNolist.back()+1-runNolist.front(),runNolist.front(),runNolist.back()+1,3200,0,160000);
    tx1vec[i].ResizeTo(runNolist.size());
    tx2vec[i].ResizeTo(runNolist.size());
    ty1vec[i].ResizeTo(runNolist.size());
    ty2vec[i].ResizeTo(runNolist.size());
    tavec[i].ResizeTo(runNolist.size());
  }
  
  const Int_t nGroup=6;
  TH1* h_tx1[nPPAC][nGroup];
  TH1* h_tx2[nPPAC][nGroup];
  TH1* h_ty1[nPPAC][nGroup];
  TH1* h_ty2[nPPAC][nGroup];
  TH1* h_ta[nPPAC][nGroup];
  for(Int_t i=0; i<nPPAC; i++){
    for(Int_t j=0; j<nGroup; j++){
      h_tx1[i][j] = new TH1I(Form("h_tx1_%d_%d",i,j),ppacname[i]+"TX1",16000,0,160000); 
      h_tx2[i][j] = new TH1I(Form("h_tx2_%d_%d",i,j),ppacname[i]+"TX2",16000,0,160000); 
      h_ty1[i][j] = new TH1I(Form("h_ty1_%d_%d",i,j),ppacname[i]+"TY1",16000,0,160000); 
      h_ty2[i][j] = new TH1I(Form("h_ty2_%d_%d",i,j),ppacname[i]+"TY2",16000,0,160000); 
      h_ta[i][j] = new TH1I(Form("h_ta_%d_%d",i,j),ppacname[i]+"TA",16000,0,160000); 
    }
  }
  Int_t labeledrun[9]={2174,2195,2202,2204,2392,2395,2471,2476,2510};
  if(beamtype==112){labeledrun[0]=2520; labeledrun[1]=2530; labeledrun[2]=2654;} 
  auto cvs = new TCanvas("cvs","");
  
  Int_t counter=0;
  
  auto h_eff = new TH1F("h_eff","",60,0,60);
  Int_t nevent=0;
  for(auto runNo: runNolist){ 
    std::cout<<"runNo:"<<runNo<<std::endl;
    auto chain = new TChain("chain","");
    chain->AddFile(Form(datapath+"run%04d.ppac.root",runNo),0,"tree");
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
    
    Bool_t whichGroup[nGroup];
    for(Int_t i=0; i<nGroup; i++) whichGroup[i]=kFALSE;
    if(runNo>=2174&&runNo<2195) whichGroup[0]=kTRUE;
    else if((runNo>=2195&&runNo<2202)||(runNo>=2204&&runNo<2392)) whichGroup[1]=kTRUE;
    else if(runNo>=2202&&runNo<2204) whichGroup[2]=kTRUE;
    else if(runNo>=2392&&runNo<2395) whichGroup[3]=kTRUE;
    else if((runNo>=2395&&runNo<2471)||(runNo>=2476&&runNo<2510)) whichGroup[4]=kTRUE;
    else if(runNo>=2471&&runNo<2476) whichGroup[5]=kTRUE;
    else if(runNo>=2520&&runNo<2530) whichGroup[0]=kTRUE;
    else if(runNo>=2530&&runNo<2654) whichGroup[1]=kTRUE;




    for(Int_t i=0; i<chain->GetEntries(); i++){
      chain->GetEntry(i);
      for(Int_t j=0; j<nPPAC; j++){
        for(Int_t k=0; k<TX1raw[j]->size(); k++){
          h_runtx1[j]->Fill(runNo,TX1raw[j]->at(k));
          for(Int_t l=0; l<nGroup; l++)
            if(whichGroup[l]) h_tx1[j][l]->Fill(TX1raw[j]->at(k));
        }
        for(Int_t k=0; k<TX2raw[j]->size(); k++){
          h_runtx2[j]->Fill(runNo,TX2raw[j]->at(k));
          for(Int_t l=0; l<nGroup; l++)
            if(whichGroup[l]) h_tx2[j][l]->Fill(TX2raw[j]->at(k));
        }
        for(Int_t k=0; k<TY1raw[j]->size(); k++){
          h_runty1[j]->Fill(runNo,TY1raw[j]->at(k));
          for(Int_t l=0; l<nGroup; l++)
            if(whichGroup[l]) h_ty1[j][l]->Fill(TY1raw[j]->at(k));
        }
        for(Int_t k=0; k<TY2raw[j]->size(); k++){
          h_runty2[j]->Fill(runNo,TY2raw[j]->at(k));
          for(Int_t l=0; l<nGroup; l++)
            if(whichGroup[l]) h_ty2[j][l]->Fill(TY2raw[j]->at(k));
        }
        for(Int_t k=0; k<TAraw[j]->size(); k++){
          h_runta[j]->Fill(runNo,TAraw[j]->at(k));
          for(Int_t l=0; l<nGroup; l++)
            if(whichGroup[l]) h_ta[j][l]->Fill(TAraw[j]->at(k));
        }
      
      if(TX1raw[j]->size()>0) h_eff->Fill(5*j);
      if(TX2raw[j]->size()>0) h_eff->Fill(5*j+1);
      if(TY1raw[j]->size()>0) h_eff->Fill(5*j+2);
      if(TY2raw[j]->size()>0) h_eff->Fill(5*j+3);
      if(TAraw[j]->size()>0) h_eff->Fill(5*j+4);
     
      }
    
    }
    
    nevent+=chain->GetEntries();


    chain->Clear();
    delete chain;
    counter++; 
  }
 
  cvs->Clear();
  delete cvs;

/* 
  TCanvas* c[5];
  for(Int_t i=0; i<5; i++){
    c[i] = new TCanvas(Form("c%d",i),"");
    c[i]->Divide(4,3);
  }
  for(Int_t j=0; j<12; j++){
    c[0]->cd(j+1);
    h_runtx1[j]->Draw("colz");
    c[1]->cd(j+1);
    h_runtx2[j]->Draw("colz");
    c[2]->cd(j+1);
    h_runty1[j]->Draw("colz");
    c[3]->cd(j+1);
    h_runty2[j]->Draw("colz");
    c[4]->cd(j+1);
    h_runta[j]->Draw("colz");
  }
*/
  
  auto file = new TFile(Form("supplemental/ppacTDC.%d.root",beamtype),"recreate");
  for(Int_t j=0; j<12; j++){
    h_runtx1[j]->Write();
    h_runtx2[j]->Write();
    h_runty1[j]->Write();
    h_runty2[j]->Write();
    h_runta[j]->Write();
    for(Int_t i=0; i<nGroup; i++){
      h_tx1[j][i]->Write();
      h_tx2[j][i]->Write();
      h_ty1[j][i]->Write();
      h_ty2[j][i]->Write();
      h_ta[j][i]->Write();
    }
  }
  file->Close();
  
  TCanvas* cx1[nGroup];
  TCanvas* cx2[nGroup];
  TCanvas* cy1[nGroup];
  TCanvas* cy2[nGroup];
  TCanvas* ca[nGroup];
  auto line = new TLine();
  line->SetLineColor(kRed);
  for(Int_t i=0; i<nGroup; i++){
    cx1[i] = new TCanvas(Form("cx1_%d",i),"");
    cx1[i]->Divide(4,3);
    cx2[i] = new TCanvas(Form("cx2_%d",i),"");
    cx2[i]->Divide(4,3);
    cy1[i] = new TCanvas(Form("cy1_%d",i),"");
    cy1[i]->Divide(4,3);
    cy2[i] = new TCanvas(Form("cy2_%d",i),"");
    cy2[i]->Divide(4,3);
    ca[i] = new TCanvas(Form("ca_%d",i),"");
    ca[i]->Divide(4,3);
    for(Int_t j=0; j<12; j++){
      cx1[i]->cd(j+1);
      Double_t maxpos = h_tx1[j][i]->GetBinCenter(h_tx1[j][i]->GetMaximumBin());
      h_tx1[j][i]->GetXaxis()->SetRangeUser(maxpos-500,maxpos+500);
      h_tx1[j][i]->Draw();
      //Double_t tx1gmean = GetGaussMean(h_tx1[j][i]);
      Double_t tx1recmean = GetRecMean(h_tx1[j][i]);
      line->DrawLine(tx1recmean,0,tx1recmean,h_tx1[j][i]->GetMaximum());

      cx2[i]->cd(j+1);
      maxpos = h_tx2[j][i]->GetBinCenter(h_tx2[j][i]->GetMaximumBin());
      h_tx2[j][i]->GetXaxis()->SetRangeUser(maxpos-500,maxpos+500);
      h_tx2[j][i]->Draw();
      //Double_t tx2gmean = GetGaussMean(h_tx2[j][i]);
      Double_t tx2recmean = GetRecMean(h_tx2[j][i]);
      line->DrawLine(tx2recmean,0,tx2recmean,h_tx2[j][i]->GetMaximum());

      cy1[i]->cd(j+1);
      maxpos = h_ty1[j][i]->GetBinCenter(h_ty1[j][i]->GetMaximumBin());
      h_ty1[j][i]->GetXaxis()->SetRangeUser(maxpos-500,maxpos+500);
      h_ty1[j][i]->Draw();
      //Double_t ty1gmean = GetGaussMean(h_ty1[j][i]);
      Double_t ty1recmean = GetRecMean(h_ty1[j][i]);
      line->DrawLine(ty1recmean,0,ty1recmean,h_ty1[j][i]->GetMaximum());
      
      cy2[i]->cd(j+1);
      maxpos = h_ty2[j][i]->GetBinCenter(h_ty2[j][i]->GetMaximumBin());
      h_ty2[j][i]->GetXaxis()->SetRangeUser(maxpos-500,maxpos+500);
      h_ty2[j][i]->Draw();
      //Double_t ty2gmean = GetGaussMean(h_ty2[j][i]);
      Double_t ty2recmean = GetRecMean(h_ty2[j][i]);
      line->DrawLine(ty2recmean,0,ty2recmean,h_ty2[j][i]->GetMaximum());
      
      ca[i]->cd(j+1);
      maxpos = h_ta[j][i]->GetBinCenter(h_ta[j][i]->GetMaximumBin());
      h_ta[j][i]->GetXaxis()->SetRangeUser(maxpos-500,maxpos+500);
      h_ta[j][i]->Draw();
      //Double_t tagmean = GetGaussMean(h_ta[j][i]);
      Double_t tarecmean = GetRecMean(h_ta[j][i]);
      line->DrawLine(tarecmean,0,tarecmean,h_ta[j][i]->GetMaximum());
      std::cout<<","<<tx1recmean<<","<<tx2recmean<<","<<ty1recmean<<","<<ty2recmean<<","<<tarecmean<<std::endl;
    }
  }

  

  /*
  Int_t runNo=2333;
  auto infile = new TFile(Form("rootfile/ppac/run%04d.ppac.root",runNo),"read");
  auto tree = (TTree*)infile->Get("tree");
  std::vector<Int_t> *TX1raw[12]={};
  std::vector<Int_t> *TX2raw[12]={};
  std::vector<Int_t> *TY1raw[12]={};
  std::vector<Int_t> *TY2raw[12]={};
  std::vector<Int_t> *TAraw[12]={};
  TString ppacname[12]={"F3_1A","F3_1B","F3_2A","F3_2B","F5_1A","F5_1B","F5_2A","F5_2B","F7_1A","F7_1B","F7_2A","F7_2B"}; 
  Int_t ppacid[12]={4,5,6,7,9,10,11,12,14,15,16,17}; 
  for(Int_t i=0; i<12; i++){
    tree->SetBranchAddress(ppacname[i]+"_TX1",&TX1raw[i]);
    tree->SetBranchAddress(ppacname[i]+"_TY1",&TY1raw[i]);
    tree->SetBranchAddress(ppacname[i]+"_TX2",&TX2raw[i]);
    tree->SetBranchAddress(ppacname[i]+"_TY2",&TY2raw[i]);
    tree->SetBranchAddress(ppacname[i]+"_TA",&TAraw[i]);
  }
  
  Double_t tamin[12],tamax[12];
  auto ctest = new TCanvas("ctest","");
  for(Int_t i=0; i<12; i++){
    auto h_ta = new TH1D("h_ta","",1000,0,50000);
    tree->Draw(ppacname[i]+"_TA>>h_ta");
    tamin[i] = h_ta->GetBinCenter(h_ta->GetMaximumBin())-500.;
    tamax[i] = h_ta->GetBinCenter(h_ta->GetMaximumBin())+500.;
    h_ta->Clear();
  }


  TH1* h_tsumx[12];
  TH1* h_tsumy[12];
  TH1* h_tdiffx[12];
  TH1* h_tdiffy[12];
  TH1* hx[12];
  TH1* hy[12];
  for(Int_t i=0; i<12; i++){
    h_tsumx[i] = new TH1D(Form("h_tsumx%d",i),"",2000,-3000,3000);
    h_tsumy[i] = new TH1D(Form("h_tsumy%d",i),"",2000,-3000,3000);
    h_tdiffx[i] = new TH1D(Form("h_tdiffx%d",i),"",100,-300,300);
    h_tdiffy[i] = new TH1D(Form("h_tdiffy%d",i),"",100,-300,300);
    hx[i] = new TH1D(Form("hx%d",i),"",100,-30,30);
    hy[i] = new TH1D(Form("hy%d",i),"",100,-30,30);
  }
  
  Double_t ach2ns[12], tx1ch2ns[12], ty1ch2ns[12], tx2ch2ns[12], ty2ch2ns[12];
  Double_t tsumxmin[12], tsumxmax[12], tsumymin[12], tsumymax[12];
  Double_t xdtoffset[12], ydtoffset[12];
  Double_t xfactor[12], yfactor[12], xoffset[12], yoffset[12], xposoffset[12], yposoffset[12];
  Double_t xzpos[12], yzpos[12];
  auto bigripspara = TArtBigRIPSParameters::Instance();
  TString anadir = "/home/s015/exp/exp1605_s015/anaroot/users/108Sn_analysis/BeamAnalysis/mkana/";
  TString dbdir = anadir+"db/";
  bigripspara->LoadParameter((Char_t*) Form(dbdir+"BigRIPSPPAC/BigRIPSPPAC.108.xml"));
  TArtPPACPara* ppacpara=nullptr;
  for(Int_t i=0; i<12; i++){
    ppacpara = (TArtPPACPara*)bigripspara->GetListOfPPACPara()->At(ppacid[i]);
    ach2ns[i]=ppacpara->GetCh2NsA();
    tx1ch2ns[i]=ppacpara->GetCh2NsX1();
    tx2ch2ns[i]=ppacpara->GetCh2NsX2();
    ty1ch2ns[i]=ppacpara->GetCh2NsY1();
    ty2ch2ns[i]=ppacpara->GetCh2NsY2();
    tsumxmin[i]=ppacpara->GetTXSumMin();
    tsumxmax[i]=ppacpara->GetTXSumMax();
    tsumymin[i]=ppacpara->GetTYSumMin();
    tsumymax[i]=ppacpara->GetTYSumMax();
    xdtoffset[i]=ppacpara->GetXDTimeOffset();
    ydtoffset[i]=ppacpara->GetYDTimeOffset();
    xfactor[i]=ppacpara->GetXFactor();
    yfactor[i]=ppacpara->GetYFactor();
    xoffset[i]=ppacpara->GetXOffset();
    yoffset[i]=ppacpara->GetYOffset();
    xposoffset[i]=ppacpara->GetXPosOffset();
    yposoffset[i]=ppacpara->GetYPosOffset();
    xzpos[i]>ppacpara->GetXZPos();
    yzpos[i]>ppacpara->GetYZPos();
  }
  auto o = new TFile("test.root","recreate"); 
  auto t = new TTree("t","");
  std::vector<Double_t> X[12], Y[12], TX[12], TY[12];
  for(Int_t i=0; i<12; i++){
    t->Branch(Form("X%d",i),&X[i]);
    t->Branch(Form("Y%d",i),&Y[i]);
    t->Branch(Form("XT%d",i),&TX[i]);
    t->Branch(Form("YT%d",i),&TY[i]);
  }
  
  auto h_f3xy = new TH2D("h_f3xy","",100,-40,40,100,-40,40);

  for(Int_t i=0 ;i<tree->GetEntries(); i++){
    tree->GetEntry(i);
   
    std::vector<Double_t> TX1[12], TX2[12], TY1[12], TY2[12], TA[12];
    for(Int_t j=0; j<12; j++){
      X[j].clear();Y[j].clear();TX[j].clear();TY[j].clear();
      for(Int_t k=0; k<TX1raw[j]->size(); k++) TX1[j].push_back( (Double_t)TX1raw[j]->at(k)*tx1ch2ns[j] ); 
      for(Int_t k=0; k<TX2raw[j]->size(); k++) TX2[j].push_back( (Double_t)TX2raw[j]->at(k)*tx2ch2ns[j] ); 
      for(Int_t k=0; k<TY1raw[j]->size(); k++) TY1[j].push_back( (Double_t)TY1raw[j]->at(k)*ty1ch2ns[j] ); 
      for(Int_t k=0; k<TY2raw[j]->size(); k++) TY2[j].push_back( (Double_t)TY2raw[j]->at(k)*ty2ch2ns[j] ); 
      for(Int_t k=0; k<TAraw[j]->size();  k++) TA[j].push_back( (Double_t)TAraw[j]->at(k)*ach2ns[j] ); 
      
      for(Int_t k=0; k<TX1[j].size(); k++)
        for(Int_t l=0; l<TX2[j].size(); l++){
          Bool_t istsumxgood=kFALSE;
          for(Int_t m=0; m<TA[j].size(); m++){
            Double_t tsumx = TX1[j].at(k)+TX2[j].at(l)-2.*TA[j].at(m);
            h_tsumx[j]->Fill(tsumx);
            if(tsumx>=tsumxmin[j]&&tsumx<=tsumxmax[j]) istsumxgood=kTRUE;
      //       && TAraw[j]->at(m)>=tamin[j]&&TAraw[j]->at(m)<=tamax[j])istsumxgood=kTRUE;
          }
          Double_t tdiffx = TX1[j].at(k)-TX2[j].at(l);
          h_tdiffx[j]->Fill(tdiffx);
          Double_t fX=-9999.;
          if(istsumxgood){
            tdiffx = tdiffx - xdtoffset[j];
            fX = tdiffx*xfactor[j]/2.;
            fX -= xoffset[j];
            fX -= xposoffset[j];
            fX = -fX;
            hx[j]->Fill(fX);
            X[j].push_back(fX);
            TX[j].push_back(TX1[j].at(k)+TX2[j].at(l));
          }

        }
      
      for(Int_t k=0; k<TY1[j].size(); k++)
        for(Int_t l=0; l<TY2[j].size(); l++){
          Bool_t istsumygood=kFALSE;
          for(Int_t m=0; m<TA[j].size(); m++){
            Double_t tsumy = TY1[j].at(k)+TY2[j].at(l)-2.*TA[j].at(m);
            h_tsumy[j]->Fill(tsumy);
            if(tsumy>=tsumymin[j]&&tsumy<=tsumymax[j]) istsumygood=kTRUE;
              //&&TAraw[j]->at(m)>=tamin[j]&&TAraw[j]->at(m)<=tamax[j])istsumygood=kTRUE;
          }
          Double_t tdiffy = TY1[j].at(k)-TY2[j].at(l);
          h_tdiffy[j]->Fill(tdiffy);
          Double_t fY=-9999.; 
          if(istsumygood){
            tdiffy = tdiffy - xdtoffset[j];
            fY = tdiffy*yfactor[j]/2.;
            fY -= yoffset[j];
            fY -= yposoffset[j];
            hy[j]->Fill(fY);
            Y[j].push_back(fY);
            TY[j].push_back(TY1[j].at(k)+TY2[j].at(l));
          }
        }
          
    }
    t->Fill();
  
    // reconstruct X,Y,A,B
    std::vector<Double_t> FX[3], FY[3], FA[3], FB[3], FTX[3], FTY[3];
    for(Int_t j=0; j<TX[0].size(); j++)
      for(Int_t k=0; k<TX[1].size(); k++)
        for(Int_t l=0; l<TX[2].size(); l++)
          for(Int_t m=0; m<TX[3].size(); m++)
            if(TMath::Abs(TX[0].at(j)-TX[1].at(k))<=200.
              &&TMath::Abs(TX[0].at(j)-TX[2].at(l))<=200.
              &&TMath::Abs(TX[0].at(j)-TX[3].at(m))<=200.){
                FTX[0].push_back( (TX[0].at(j)+TX[1].at(k)+TX[2].at(l)+TX[3].at(m))/4. );
                TMatrixD xvec(2,1); xvec.Zero();
                TMatrixD xmat(2,2); xmat.Zero();
                xvec(0,0) = xzpos[0]*X[0].at(j) + xzpos[1]*X[1].at(k) + xzpos[2]*X[2].at(l) + xzpos[3]*X[3].at(m);
                xvec(1,0) = X[0].at(j) + X[1].at(k) + X[2].at(l) + X[3].at(m);
                xmat(0,1) = xzpos[0]+xzpos[1]+xzpos[2]+xzpos[3];
                xmat(1,0) = xzpos[0]+xzpos[1]+xzpos[2]+xzpos[3];
                xmat(0,0) = xzpos[0]*xzpos[0]+xzpos[1]*xzpos[1]+xzpos[2]*xzpos[2]+xzpos[3]*xzpos[3];
                xmat(1,1) = 3;
                TMatrixD rxvec = xmat.Invert()*xvec;
                FX[0].push_back(rxvec(1,0));
                FA[0].push_back(TMath::ATan(rxvec(0,0))*1000.);
              }

    for(Int_t j=0; j<TY[0].size(); j++)
      for(Int_t k=0; k<TY[1].size(); k++)
        for(Int_t l=0; l<TY[2].size(); l++)
          for(Int_t m=0; m<TY[3].size(); m++)
            if(TMath::Abs(TY[0].at(j)-TY[1].at(k))<=200.
              &&TMath::Abs(TY[0].at(j)-TY[2].at(l))<=200.
              &&TMath::Abs(TY[0].at(j)-TY[3].at(m))<=200.){
                FTY[0].push_back( (TY[0].at(j)+TY[1].at(k)+TY[2].at(l)+TY[3].at(m))/4. );
                TMatrixD yvec(2,1); yvec.Zero();
                TMatrixD ymat(2,2); ymat.Zero();
                yvec(0,0) = yzpos[0]*Y[0].at(j) + yzpos[1]*Y[1].at(k) + yzpos[2]*Y[2].at(l) + yzpos[3]*Y[3].at(m);
                yvec(1,0) = Y[0].at(j) + Y[1].at(k) + Y[2].at(l) + Y[3].at(m);
                ymat(0,1) = yzpos[0]+yzpos[1]+yzpos[2]+yzpos[3];
                ymat(1,0) = yzpos[0]+yzpos[1]+yzpos[2]+yzpos[3];
                ymat(0,0) = yzpos[0]*yzpos[0]+yzpos[1]*yzpos[1]+yzpos[2]*yzpos[2]+yzpos[3]*yzpos[3];
                ymat(1,1) = 3;
                TMatrixD ryvec = ymat.Invert()*yvec;
                FY[0].push_back(ryvec(1,0));
                FB[0].push_back(TMath::ATan(ryvec(0,0))*1000.);
              }
    
    for(Int_t j=0; j<FTX[0].size(); j++)
      for(Int_t k=0; k<FTY[0].size(); k++)
        if(TMath::Abs(FTX[0].at(j)-FTY[0].at(k))<=200.)
          h_f3xy->Fill(FX[0].at(j),FY[0].at(k));
    
  
  }
 
 
  o->Write();
   
  auto ctsumx = new TCanvas("ctsumx","",1000,500);
  auto ctsumy = new TCanvas("ctsumy","",1000,500);
  ctsumx->Divide(4,3);
  ctsumy->Divide(4,3);
  for(Int_t i=0; i<12; i++){
    ctsumx->cd(i+1)->SetLogy();
    h_tsumx[i]->Draw();
    ctsumy->cd(i+1)->SetLogy();
    h_tsumy[i]->Draw();
  }
  auto ctdiffx = new TCanvas("ctdiffx","",1000,500);
  auto ctdiffy = new TCanvas("ctdiffy","",1000,500);
  ctdiffx->Divide(4,3);
  ctdiffy->Divide(4,3);
  for(Int_t i=0; i<12; i++){
    ctdiffx->cd(i+1)->SetLogy();
    h_tdiffx[i]->Draw();
    ctdiffy->cd(i+1)->SetLogy();
    h_tdiffy[i]->Draw();
  }
  auto cx = new TCanvas("cx","",1000,500);
  auto cy = new TCanvas("cy","",1000,500);
  cx->Divide(4,3);
  cy->Divide(4,3);
  for(Int_t i=0; i<12; i++){
    cx->cd(i+1)->SetLogy();
    hx[i]->Draw();
    cy->cd(i+1)->SetLogy();
    hy[i]->Draw();
  }
  */



}
