
Bool_t isFileLoaded=kFALSE;
std::vector<Int_t> runNolist;
const TString anapath="/home/s015/exp/exp1605_s015/anaroot/users/S22_analysis/";
const TString datapath=anapath+"rootfiles/plastic/";
const Int_t beamtype=112;

void loadFiles(Int_t bt){
  
  if(!(bt==108||bt==112)) return;
  Int_t start=2174;
  Int_t stop=2509;
  if(bt==112){ start=2520; stop=2653;}

  Int_t nrun=0;
  runNolist.clear();
  FileStat_t file;
  for(Int_t irun=start; irun<=stop; irun++){
    if( !gSystem->GetPathInfo( Form(datapath+"run%04d.pla.root",irun),file) ){
      if(irun!=2175&&irun!=2176){ // Plastic didin't work at those runs.
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
  h->Fit(f,"Q","",mean-3*fwhm,mean+3*fwhm);
  Double_t gmean = f->GetParameter(1);
  if(h->GetEntries()<=100) gmean = mean;
  return gmean;
}


void anaPlasticTMean(){
  
  if(!isFileLoaded) loadFiles(beamtype); 
  
  TH2* h_runtl[2];
  TH2* h_runtr[2];
  TVectorD runvec(runNolist.size());
  TVectorD tlvec[2];
  TVectorD trvec[2];
  TString planame[2]={"F3","F7"};
  for(Int_t i=0; i<2; i++){
    h_runtl[i] = new TH2I(Form("h_runtl_%d",i),planame[i]+"TL vs run",runNolist.back()+1-runNolist.front(),runNolist.front(),runNolist.back()+1,3500,0,70000);
    h_runtr[i] = new TH2I(Form("h_runtr_%d",i),planame[i]+"TR vs run",runNolist.back()+1-runNolist.front(),runNolist.front(),runNolist.back()+1,3500,0,70000);
    tlvec[i].ResizeTo(runNolist.size());
    trvec[i].ResizeTo(runNolist.size());
  }
  
  Int_t labeledrun[7]={2174,2195,2202,2204,2471,2476,2510};
  if(beamtype==112){labeledrun[0]=2520; labeledrun[1]=2530; labeledrun[2]=2654;} 
  
  const Int_t nHist = 4;     
  TH1* h_tl[2][nHist];
  TH1* h_tr[2][nHist];
  for(Int_t i=0; i<2; i++){
    for(Int_t j=0; j<nHist; j++){
      h_tl[i][j] = new TH1I(Form("h_tl_%d_%d",i,j),Form(planame[i]+"TL_%d",j),10000,0,70000);
      h_tr[i][j] = new TH1I(Form("h_tr_%d_%d",i,j),Form(planame[i]+"TR_%d",j),10000,0,70000);
    }
  }
  Int_t counter=0;
  
  Int_t nevent=0;
  
  
  for(auto runNo: runNolist){ 
    std::cout<<"runNo:"<<runNo<<std::endl;
    auto chain = new TChain("chain","");
    chain->AddFile(Form(datapath+"run%04d.pla.root",runNo),0,"tree");
    std::vector<Int_t> *TLraw[2]={};
    std::vector<Int_t> *TRraw[2]={};
    chain->SetBranchAddress("F3TDCL",&TLraw[0]);
    chain->SetBranchAddress("F3TDCR",&TRraw[0]);
    chain->SetBranchAddress("F7TDCL",&TLraw[1]);
    chain->SetBranchAddress("F7TDCR",&TRraw[1]);
  
    Bool_t whichGroup[nHist];
    for(Int_t i=0; i<nHist; i++) whichGroup[i]=kFALSE;
    if(runNo>=2174&&runNo<2195) whichGroup[0]=kTRUE;
    else if((runNo>=2195&&runNo<2202)||(runNo>=2204&&runNo<2471)||(runNo>=2476&&runNo<2510)) whichGroup[1]=kTRUE;
    else if(runNo>=2202&&runNo<2204) whichGroup[2]=kTRUE;
    else if(runNo>=2471&&runNo<2476) whichGroup[3]=kTRUE;
    else if(runNo>=2520&&runNo<2530) whichGroup[0]=kTRUE;
    else if(runNo>=2530&&runNo<2654) whichGroup[1]=kTRUE;
    
    for(Int_t i=0; i<chain->GetEntries(); i++){
      chain->GetEntry(i);
      for(Int_t j=0; j<2; j++){
        for(Int_t k=0; k<TLraw[j]->size(); k++){
          h_runtl[j]->Fill(runNo,TLraw[j]->at(k));
          for(Int_t l=0; l<nHist; l++)
            if(whichGroup[l]) h_tl[j][l]->Fill(TLraw[j]->at(k));
        }
        for(Int_t k=0; k<TRraw[j]->size(); k++){
          h_runtr[j]->Fill(runNo,TRraw[j]->at(k));
          for(Int_t l=0; l<nHist; l++)
            if(whichGroup[l]) h_tr[j][l]->Fill(TRraw[j]->at(k));
        }
      }
    
    }
    
    nevent+=chain->GetEntries();

    chain->Clear();
    delete chain;
    counter++; 
  }

/*
  TCanvas* c[4];
  for(Int_t i=0; i<2; i++){
    c[i] = new TCanvas(Form("c%d",i),"");
    h_runtl[i]->Draw("colz");
  }
  for(Int_t i=0; i<2; i++){
    c[i+2] = new TCanvas(Form("c%d",i+2),"");
    h_runtr[i]->Draw("colz");
  }
*/

  auto file = new TFile(Form("supplemental/plaTDC.%d.root",beamtype),"recreate");
  for(Int_t j=0; j<2; j++){
    h_runtl[j]->Write();
    h_runtr[j]->Write();
    for(Int_t i=0; i<nHist; i++){
      h_tl[j][i]->Write();
      h_tr[j][i]->Write();
    }
  }
  file->Close();
  TCanvas* ctl[nHist];
  TCanvas* ctr[nHist];
  for(Int_t i=0; i<nHist; i++){
    ctl[i] = new TCanvas(Form("ctl_%d",i),"");
    ctl[i]->Divide(1,2);
    ctr[i] = new TCanvas(Form("ctr_%d",i),"");
    ctr[i]->Divide(1,2);
    for(Int_t j=0; j<2; j++){
      ctl[i]->cd(j+1);
      h_tl[j][i]->GetXaxis()->SetRangeUser(h_tl[j][i]->GetBinCenter(h_tl[j][i]->GetMaximumBin())-500,h_tl[j][i]->GetBinCenter(h_tl[j][i]->GetMaximumBin())+500);
      Double_t tx1gmean = GetGaussMean(h_tl[j][i]);
      ctr[i]->cd(j+1);
      h_tr[j][i]->GetXaxis()->SetRangeUser(h_tr[j][i]->GetBinCenter(h_tr[j][i]->GetMaximumBin())-500,h_tr[j][i]->GetBinCenter(h_tr[j][i]->GetMaximumBin())+500);
      Double_t tx2gmean = GetGaussMean(h_tr[j][i]);
      std::cout<<","<<tx1gmean<<","<<tx2gmean<<std::endl;
    }
  }
  

}

void GetTMean(){
  
  FileStat_t file;
  TString fname = Form("plaTDC.%d.root",beamtype);
  if(gSystem->GetPathInfo("./supplemental/"+fname,file)!=0) return;
  auto f = new TFile("./supplemental/"+fname,"read");
  
  auto c = new TCanvas("c","");
  auto c1 = new TCanvas("c1","");
  c->Divide(2,6);
  c1->Divide(2,6);
  TH1 *h_tl[2][6], *h_tr[2][6];
  Int_t nHist=6;
  if(beamtype==112) nHist=2;
  for(Int_t i=0; i<2; i++)
    for(Int_t j=0; j<nHist; j++){
      h_tl[i][j] = (TH1I*)f->Get(Form("h_tl_%d_%d",i,j));
      Double_t tm = h_tl[i][j]->GetMean();
      h_tl[i][j]->GetXaxis()->SetRangeUser(tm-1000,tm+1000);
      h_tr[i][j] = (TH1I*)f->Get(Form("h_tr_%d_%d",i,j));
      tm = h_tr[i][j]->GetMean();
      h_tr[i][j]->GetXaxis()->SetRangeUser(tm-1000,tm+1000);
      c->cd(i+2*j+1);
      Double_t tx1gmean = GetGaussMean(h_tl[i][j]);
      c1->cd(i+2*j+1);
      Double_t tx2gmean = GetGaussMean(h_tr[i][j]);
      std::cout<<","<<tx1gmean<<","<<tx2gmean<<std::endl;
    }
  
   

}
