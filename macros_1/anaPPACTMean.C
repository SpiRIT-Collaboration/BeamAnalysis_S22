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

  




}
