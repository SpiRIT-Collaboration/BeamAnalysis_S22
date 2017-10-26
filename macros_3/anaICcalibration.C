Bool_t isFileLoaded=kFALSE;
std::vector<Int_t> runNolist;
const TString anapath="/home/s015/exp/exp1605_s015/anaroot/users/S22_analysis/";
const TString datapath=anapath+"rootfiles/reco0/";

const Int_t nPPAC=12;
const TString ppacname[nPPAC]={"F3_1A","F3_1B","F3_2A","F3_2B","F5_1A","F5_1B","F5_2A","F5_2B","F7_1A","F7_1B","F7_2A","F7_2B"}; 
const Int_t   ppacid[nPPAC]  ={4,5,6,7,9,10,11,12,14,15,16,17}; 
const Int_t beamtype=112;

void loadFiles(Int_t);
void makeICADCMean();
void getZCondition(Int_t,Double_t*,Double_t*);
void estimateGaussPara(TH1*,Int_t*,Double_t*,Double_t*);

const Double_t zlikegap=0.01;
const Double_t adcgap=6.;


void anaICcalibration(){
  
  if(!isFileLoaded) loadFiles(beamtype);
  
  // get run clarification -> which was used to clarify runs by its pseudo-Z.
  auto inZlike = new TFile(Form(anapath+"macros_3/supplemental/icZedlike%d.root",beamtype),"read");
  auto ztr = (TTree*)inZlike->Get("runtree");
  std::vector<Int_t> *runv=0;
  TBranch *brunvec=0;
  ztr->SetBranchAddress("runarray",&runv,&brunvec);
  Int_t nzlikedivision=ztr->GetEntries();
  std::vector<Int_t> zlikerunarray[nzlikedivision];
  for(Int_t i=0; i<ztr->GetEntries(); i++){
    runv->clear();
    ztr->GetEntry(i);
    for(Int_t j=0; j<runv->size(); j++) zlikerunarray[i].push_back(runv->at(j));
  }
  std::cout<<"nzlikedivision:"<<nzlikedivision<<std::endl; // <- number of clarifications for PID condition
  
  // get pseudo Z conditions
  Int_t nZ=3; //Z=50,51,49
  Double_t minzlike[nzlikedivision][nZ];  // pseudoZ condition for each isotope.
  Double_t maxzlike[nzlikedivision][nZ];
  for(Int_t i=0; i<nzlikedivision; i++){
    if(zlikerunarray[i].size()==0) continue;
    getZCondition(i,minzlike[i],maxzlike[i]);
  }



  // get each channels of ADC graph (which PID condition should be used?)
  auto inADCeach = new TFile(Form(anapath+"macros_3/supplemental/icADCeach%d.root",beamtype),"read");
  std::vector<Int_t> runarray[6][40];
  Int_t ndivision[6];
  auto a0tr = (TTree*)inADCeach->Get("adc0");
  auto a1tr = (TTree*)inADCeach->Get("adc1");
  auto a2tr = (TTree*)inADCeach->Get("adc2");
  auto a3tr = (TTree*)inADCeach->Get("adc3");
  auto a4tr = (TTree*)inADCeach->Get("adc4");
  auto a5tr = (TTree*)inADCeach->Get("adc5");
  std::vector<Int_t> *adc0runvec=0; TBranch *badc0runvec=0;
  std::vector<Int_t> *adc1runvec=0; TBranch *badc1runvec=0;
  std::vector<Int_t> *adc2runvec=0; TBranch *badc2runvec=0;
  std::vector<Int_t> *adc3runvec=0; TBranch *badc3runvec=0;
  std::vector<Int_t> *adc4runvec=0; TBranch *badc4runvec=0;
  std::vector<Int_t> *adc5runvec=0; TBranch *badc5runvec=0;
  a0tr->SetBranchAddress("adc0runarray",&adc0runvec,&badc0runvec);
  a1tr->SetBranchAddress("adc1runarray",&adc1runvec,&badc1runvec);
  a2tr->SetBranchAddress("adc2runarray",&adc2runvec,&badc2runvec);
  a3tr->SetBranchAddress("adc3runarray",&adc3runvec,&badc3runvec);
  a4tr->SetBranchAddress("adc4runarray",&adc4runvec,&badc4runvec);
  a5tr->SetBranchAddress("adc5runarray",&adc5runvec,&badc5runvec);
  ndivision[0]=a0tr->GetEntries();
  ndivision[1]=a1tr->GetEntries();
  ndivision[2]=a2tr->GetEntries();
  ndivision[3]=a3tr->GetEntries();
  ndivision[4]=a4tr->GetEntries();
  ndivision[5]=a5tr->GetEntries();
  for(Int_t i=0; i<ndivision[0]; i++){
    adc0runvec->clear();
    a0tr->GetEntry(i);
    for(Int_t j=0; j<adc0runvec->size(); j++) runarray[0][i].push_back(adc0runvec->at(j));
    std::cout<<"runarray"<<i<<std::endl;
  }
  for(Int_t i=0; i<ndivision[1]; i++){
    adc1runvec->clear();
    a1tr->GetEntry(i);
    for(Int_t j=0; j<adc1runvec->size(); j++) runarray[1][i].push_back(adc1runvec->at(j));
    std::cout<<"runarray"<<i<<std::endl;
  }
  for(Int_t i=0; i<ndivision[2]; i++){
    adc2runvec->clear();
    a2tr->GetEntry(i);
    for(Int_t j=0; j<adc2runvec->size(); j++) runarray[2][i].push_back(adc2runvec->at(j));
    std::cout<<"runarray"<<i<<std::endl;
  }
  for(Int_t i=0; i<ndivision[3]; i++){
    adc3runvec->clear();
    a3tr->GetEntry(i);
    for(Int_t j=0; j<adc3runvec->size(); j++) runarray[3][i].push_back(adc3runvec->at(j));
    std::cout<<"runarray"<<i<<std::endl;
  }
  for(Int_t i=0; i<ndivision[4]; i++){
    adc4runvec->clear();
    a4tr->GetEntry(i);
    for(Int_t j=0; j<adc4runvec->size(); j++) runarray[4][i].push_back(adc4runvec->at(j));
    std::cout<<"runarray"<<i<<std::endl;
  }
  for(Int_t i=0; i<ndivision[5]; i++){
    adc5runvec->clear();
    a5tr->GetEntry(i);
    for(Int_t j=0; j<adc5runvec->size(); j++) runarray[5][i].push_back(adc5runvec->at(j));
    std::cout<<"runarray"<<i<<std::endl;
  }


  Int_t npid=7; if(beamtype==112)npid=6; 
  
  
  // each ADC channel*ndivision[i]*each PID
  TH1D* h_adc0[ndivision[0]][npid]; 
  TH1D* h_adc1[ndivision[1]][npid]; 
  TH1D* h_adc2[ndivision[2]][npid]; 
  TH1D* h_adc3[ndivision[3]][npid]; 
  TH1D* h_adc4[ndivision[4]][npid]; 
  TH1D* h_adc5[ndivision[5]][npid];
  for(Int_t ipid=0; ipid<npid; ipid++){
    for(Int_t j=0; j<ndivision[0]; j++) h_adc0[j][ipid] = (TH1D*)inADCeach->Get(Form("h_adc0_%d_%d",j,ipid));
    for(Int_t j=0; j<ndivision[1]; j++) h_adc1[j][ipid] = (TH1D*)inADCeach->Get(Form("h_adc1_%d_%d",j,ipid));
    for(Int_t j=0; j<ndivision[2]; j++) h_adc2[j][ipid] = (TH1D*)inADCeach->Get(Form("h_adc2_%d_%d",j,ipid));
    for(Int_t j=0; j<ndivision[3]; j++) h_adc3[j][ipid] = (TH1D*)inADCeach->Get(Form("h_adc3_%d_%d",j,ipid));
    for(Int_t j=0; j<ndivision[4]; j++) h_adc4[j][ipid] = (TH1D*)inADCeach->Get(Form("h_adc4_%d_%d",j,ipid));
    for(Int_t j=0; j<ndivision[5]; j++) h_adc5[j][ipid] = (TH1D*)inADCeach->Get(Form("h_adc5_%d_%d",j,ipid));
  }
   

//9 28 10 20 26 9
  auto f_adc = new TF1("f_adc","gaus");  
  
  Double_t meanADC0[ndivision[0]][npid];
  Double_t meanADC1[ndivision[1]][npid];
  Double_t meanADC2[ndivision[2]][npid];
  Double_t meanADC3[ndivision[3]][npid];
  Double_t meanADC4[ndivision[4]][npid];
  Double_t meanADC5[ndivision[5]][npid];

  TCanvas* cadc[6][npid];
  for(Int_t i=0; i<6; i++)
    for(Int_t j=0; j<npid; j++)
      cadc[i][j] = new TCanvas(Form("cadc%d_%d",i,j),"",1200,600);
    
  for(Int_t j=0; j<npid; j++){
    cadc[0][j]->Divide(5,4);
    cadc[1][j]->Divide(7,6);
    cadc[2][j]->Divide(5,4);
    cadc[3][j]->Divide(6,6);
    cadc[4][j]->Divide(7,6);
    cadc[5][j]->Divide(5,4);
  }

  //auto f_adc = new TF1("f_adc","gaus");  
  for(Int_t i=0; i<6; i++){
    for(Int_t j=0; j<npid; j++){
      for(Int_t k=0; k<ndivision[i]; k++){
        cadc[i][j]->cd(k+1);
        if(i==0){
          h_adc0[k][j]->Draw();
          f_adc->SetParameters(h_adc0[k][j]->GetMaximum(),
                               h_adc0[k][j]->GetBinCenter(h_adc0[k][j]->GetMaximumBin()),
                               h_adc0[k][j]->GetRMS());
          Int_t isFit = h_adc0[k][j]->Fit(f_adc,"OQ"); std::cout<<"isFit"<<isFit<<std::endl;
          if(!isFit){
            meanADC0[k][j]=f_adc->GetParameter(1);
          }else{
            meanADC0[k][j]=h_adc0[k][j]->GetMean();
          }
        }
        if(i==1){
          h_adc1[k][j]->Draw();
          f_adc->SetParameters(h_adc1[k][j]->GetMaximum(), 
                               h_adc1[k][j]->GetBinCenter(h_adc1[k][j]->GetMaximumBin()),
                               h_adc1[k][j]->GetRMS());
          if(!h_adc1[k][j]->Fit(f_adc,"OQ")){
            meanADC1[k][j]=f_adc->GetParameter(1);
          }else{
            meanADC1[k][j]=h_adc1[k][j]->GetMean();
          }
        }
        if(i==2){
          h_adc2[k][j]->Draw();
          f_adc->SetParameters(h_adc2[k][j]->GetMaximum(),
                               h_adc2[k][j]->GetBinCenter(h_adc2[k][j]->GetMaximumBin()),
                               h_adc2[k][j]->GetRMS());
          if(!h_adc2[k][j]->Fit(f_adc,"OQ")){
            meanADC2[k][j]=f_adc->GetParameter(1);
          }else{
            meanADC2[k][j]=h_adc2[k][j]->GetMean();
          }
        }
        if(i==3){
          h_adc3[k][j]->Draw();
          f_adc->SetParameters(h_adc3[k][j]->GetMaximum(),
                               h_adc3[k][j]->GetBinCenter(h_adc3[k][j]->GetMaximumBin()),
                               h_adc3[k][j]->GetRMS());
          if(!h_adc3[k][j]->Fit(f_adc,"OQ")){
            meanADC3[k][j]=f_adc->GetParameter(1);
          }else{
            meanADC3[k][j]=h_adc3[k][j]->GetMean();
          }
        }
        if(i==4){
          h_adc4[k][j]->Draw();
          f_adc->SetParameters(h_adc4[k][j]->GetMaximum(),
                               h_adc4[k][j]->GetBinCenter(h_adc4[k][j]->GetMaximumBin()),
                               h_adc4[k][j]->GetRMS());
          if(!h_adc4[k][j]->Fit(f_adc,"OQ")){
            meanADC4[k][j]=f_adc->GetParameter(1);
          }else{
            meanADC4[k][j]=h_adc4[k][j]->GetMean();
          }
        }
        if(i==5){
          h_adc5[k][j]->Draw();
          f_adc->SetParameters(h_adc5[k][j]->GetMaximum(),
                               h_adc5[k][j]->GetBinCenter(h_adc5[k][j]->GetMaximumBin()),
                               h_adc5[k][j]->GetRMS());
          if(!h_adc5[k][j]->Fit(f_adc,"OQ")){
            meanADC5[k][j]=f_adc->GetParameter(1);
          }else{
            meanADC5[k][j]=h_adc5[k][j]->GetMean();
          }
        }
        f_adc->Clear();
      }
      cadc[i][j]->Clear(); delete cadc[i][j];
    }
  }
  
  TGraph* g_adcdE0[ndivision[0]];
  TGraph* g_adcdE1[ndivision[1]];
  TGraph* g_adcdE2[ndivision[2]];
  TGraph* g_adcdE3[ndivision[3]];
  TGraph* g_adcdE4[ndivision[4]];
  TGraph* g_adcdE5[ndivision[5]];
  for(Int_t j=0; j<ndivision[0]; j++){
    g_adcdE0[j] = new TGraph(npid);
    g_adcdE0[j]->SetName(Form("g_adcdE0_%d",j));
    g_adcdE0[j]->SetMarkerStyle(8);
  }
  for(Int_t j=0; j<ndivision[1]; j++){
    g_adcdE1[j] = new TGraph(npid);
    g_adcdE1[j]->SetName(Form("g_adcdE1_%d",j));
    g_adcdE1[j]->SetMarkerStyle(8);
  }
  for(Int_t j=0; j<ndivision[2]; j++){
    g_adcdE2[j] = new TGraph(npid);
    g_adcdE2[j]->SetName(Form("g_adcdE2_%d",j));
    g_adcdE2[j]->SetMarkerStyle(8);
  }
  for(Int_t j=0; j<ndivision[3]; j++){
    g_adcdE3[j] = new TGraph(npid);
    g_adcdE3[j]->SetName(Form("g_adcdE3_%d",j));
    g_adcdE3[j]->SetMarkerStyle(8);
  }
  for(Int_t j=0; j<ndivision[4]; j++){
    g_adcdE4[j] = new TGraph(npid);
    g_adcdE4[j]->SetName(Form("g_adcdE4_%d",j));
    g_adcdE4[j]->SetMarkerStyle(8);
  }
  for(Int_t j=0; j<ndivision[5]; j++){
    g_adcdE5[j] = new TGraph(npid);
    g_adcdE5[j]->SetName(Form("g_adcdE5_%d",j));
    g_adcdE5[j]->SetMarkerStyle(8);
  }
  
  
  Double_t dE108[7][6]={//108exp
      68.89,68.99,69.08,69.18,69.28,69.37,  //108Sn
      68.34,68.42,68.5 ,68.59,68.68,68.77,  //107Sn
      69.48,69.58,69.65,69.75,69.84,69.93,  //109Sn
      71.61,71.72,71.81,71.92,72.02,72.12,  //110Sb
      71.07,71.16,71.24,71.33,71.43,71.52,  //109Sb
      66.2 ,66.29,66.38,66.48,66.56,66.68,  //106In
      66.78,66.86,66.96,67.05,67.12,67.22  //107In 
      };
  Double_t dE112[6][6]={
    68.72,68.82,68.91,69.,69.09,69.18,  //112Sn
    68.2,68.28,68.36,68.45,68.52,68.6,  //111Sn
    71.44,71.51,71.6,71.7,71.8,71.9,  //114Sb
    70.88,70.98,71.07,71.16,71.24,71.32,  //113Sb
    66.08,66.16,66.26,66.34,66.43,66.52,  //110In
    66.65,66.74,66.82,66.89,66.98,67.05  //111In
  };
  Double_t dE[7][6];
  for(Int_t ichannel=0; ichannel<6; ichannel++)
    for(Int_t ipid=0; ipid<npid; ipid++){
      if(beamtype==108) dE[ipid][ichannel]=dE108[ipid][ichannel];
      if(beamtype==112) dE[ipid][ichannel]=dE112[ipid][ichannel];
    }
  
  for(Int_t i=0; i<ndivision[0]; i++){
    for(Int_t j=0; j<npid; j++)
      g_adcdE0[i]->SetPoint(j,meanADC0[i][j],dE[j][0]);
  }
  for(Int_t i=0; i<ndivision[1]; i++){
    for(Int_t j=0; j<npid; j++)
      g_adcdE1[i]->SetPoint(j,meanADC1[i][j],dE[j][1]); 
  }
  for(Int_t i=0; i<ndivision[2]; i++){
    for(Int_t j=0; j<npid; j++)
      g_adcdE2[i]->SetPoint(j,meanADC2[i][j],dE[j][2]); 
  }
  for(Int_t i=0; i<ndivision[3]; i++){
    for(Int_t j=0; j<npid; j++)
      g_adcdE3[i]->SetPoint(j,meanADC3[i][j],dE[j][3]); 
  }
  for(Int_t i=0; i<ndivision[4]; i++){
    for(Int_t j=0; j<npid; j++)
      g_adcdE4[i]->SetPoint(j,meanADC4[i][j],dE[j][4]); 
  }
  for(Int_t i=0; i<ndivision[5]; i++){
    for(Int_t j=0; j<npid; j++)
      g_adcdE5[i]->SetPoint(j,meanADC5[i][j],dE[j][5]); 
  }
 
  TCanvas* ccalib[6];
  for(Int_t i=0; i<6; i++)
    ccalib[i] = new TCanvas(Form("ccalib%d",i),"",1200,600);

  ccalib[0]->Divide(5,4);
  ccalib[1]->Divide(7,6);
  ccalib[2]->Divide(5,4);
  ccalib[3]->Divide(6,6);
  ccalib[4]->Divide(7,6);
  ccalib[5]->Divide(5,4);
  
  Double_t calpara0[2][ndivision[0]]; 
  Double_t calpara1[2][ndivision[1]]; 
  Double_t calpara2[2][ndivision[2]]; 
  Double_t calpara3[2][ndivision[3]]; 
  Double_t calpara4[2][ndivision[4]]; 
  Double_t calpara5[2][ndivision[5]]; 
  auto f_calib = new TF1("f_calib","[0]+[1]*x");
  for(Int_t i=0; i<6; i++){
    for(Int_t k=0; k<ndivision[i]; k++){
      ccalib[i]->cd(k+1);
      if(i==0){
        g_adcdE0[k]->Draw("AP");
        g_adcdE0[k]->Fit(f_calib,"OQ");
        calpara0[0][k]=f_calib->GetParameter(0); 
        calpara0[1][k]=f_calib->GetParameter(1); 
      }
      if(i==1){
        g_adcdE1[k]->Draw("AP");
        g_adcdE1[k]->Fit(f_calib,"OQ");
        calpara1[0][k]=f_calib->GetParameter(0); 
        calpara1[1][k]=f_calib->GetParameter(1); 
      }
      if(i==2){
        g_adcdE2[k]->Draw("AP");
        g_adcdE2[k]->Fit(f_calib,"OQ");
        calpara2[0][k]=f_calib->GetParameter(0); 
        calpara2[1][k]=f_calib->GetParameter(1); 
      }
      if(i==3){
        g_adcdE3[k]->Draw("AP");
        g_adcdE3[k]->Fit(f_calib,"OQ");
        calpara3[0][k]=f_calib->GetParameter(0); 
        calpara3[1][k]=f_calib->GetParameter(1); 
      }
      if(i==4){
        g_adcdE4[k]->Draw("AP");
        g_adcdE4[k]->Fit(f_calib,"OQ");
        calpara4[0][k]=f_calib->GetParameter(0); 
        calpara4[1][k]=f_calib->GetParameter(1); 
      }
      if(i==5){
        g_adcdE5[k]->Draw("AP");
        g_adcdE5[k]->Fit(f_calib,"OQ");
        calpara5[0][k]=f_calib->GetParameter(0); 
        calpara5[1][k]=f_calib->GetParameter(1); 
      }
      f_calib->Clear();
    }
  }
 
  Int_t start=runNolist.front();
  Int_t end=runNolist.back();
  TH2D* h_runadc[6];
  auto h_runsqsum = new TH2D("h_runsqsum","",end+1-start,start,end,400,50,90);
  for(Int_t i=0; i<6; i++) h_runadc[i] = new TH2D(Form("h_runadc%d",i),"",end+1-start,start,end,600,40,100);
  auto h_zetcalib = new TH1D("h_zetcalib","",600,1.7,2.3);
  auto h_runzetcal = new TH2D("h_runzetcal","",end+1-start,start,end,600,1.7,2.3);
  Double_t aoqrange[2]={2.11,2.21};
  if(beamtype==112){aoqrange[0]=2.19; aoqrange[1]=2.29;}
  auto h_PID = new TH2D("h_PID","",2000,aoqrange[0],aoqrange[1],2000,46.5,53.5);


  std::vector<Int_t> runvec;
  std::vector<Double_t> meanvec[6];
  std::vector<Double_t> zlikevec;
  TH1D* h_dE[6];
  TH1D* h_zlike;
  auto f_dE = new TF1("f_dE","gaus");
  auto cdE = new TCanvas("cdE","");

  Int_t counter=0;
  Int_t sn108counter=0;
  Int_t sn112counter=0;
  Double_t zedcal[2]={0.24705,25.2875};
  if(beamtype==112){zedcal[0]=0.619267; zedcal[1]=25.1407;}
  
  TFile* out;
  TTree* tree;
  
  for(auto runNo: runNolist){
    //if(runNo>2400) break;
    BeamReco* beam = new BeamReco();
    TString fname = Form(datapath+"run%04d.reco.root",runNo);
    beam->fChain->AddFile(fname.Data(),0,"tree");
    beam->InitRawPID();
    std::cout<<"RUN"<<runNo<<std::endl;
    
    // get PID condition of corresponding run number
    Int_t runID=0;
    for(Int_t l=0; l<nzlikedivision; l++)
      for(Int_t k=0; k<zlikerunarray[l].size(); k++)
        if(runNo==zlikerunarray[l].at(k))
          runID=l;

    Int_t runIDeach[6];
    for(Int_t i=0; i<6; i++)
      for(Int_t j=0; j<ndivision[i]; j++)
        for(Int_t k=0; k<runarray[i][j].size(); k++)
          if(runNo==runarray[i][j].at(k)) runIDeach[i]=j;
    
    auto t = new TChain("t","");
    t->AddFile(Form(anapath+"rootfiles/ppac/run%04d.ppac.root",runNo),0,"tree");
    std::vector<Int_t> *TX1raw[nPPAC]={};
    std::vector<Int_t> *TX2raw[nPPAC]={};
    std::vector<Int_t> *TY1raw[nPPAC]={};
    std::vector<Int_t> *TY2raw[nPPAC]={};
    std::vector<Int_t> *TAraw[nPPAC]={};
    for(Int_t i=0; i<nPPAC; i++){
      t->SetBranchAddress(ppacname[i]+"_TX1",&TX1raw[i]);
      t->SetBranchAddress(ppacname[i]+"_TY1",&TY1raw[i]);
      t->SetBranchAddress(ppacname[i]+"_TX2",&TX2raw[i]);
      t->SetBranchAddress(ppacname[i]+"_TY2",&TY2raw[i]);
      t->SetBranchAddress(ppacname[i]+"_TA",&TAraw[i]);
    }
    
    for(Int_t i=0; i<6; i++) h_dE[i] = new TH1D(Form("h_dE%d",i),"",200,50,90);
    h_zlike = new TH1D("h_zlike","",200,1.7,2.3);
    
    out = new TFile(Form(anapath+"rootfiles/ic/cal/run%04d.iccalibrated.root",runNo),"recreate");   
    tree = new TTree("tree","");
    Int_t run =runNo; tree->Branch("run",&run,"run/I");
    Int_t event; tree->Branch("event",&event,"event/I");
    Double_t calADC[6]; tree->Branch("calADC",calADC,"calADC[6]/D");
    Double_t sqsum; tree->Branch("sqsum",&sqsum,"sqsum/D");
    Double_t avsum; tree->Branch("avsum",&avsum,"avsum/D");
    Double_t beta; tree->Branch("beta",&beta,"beta/D");
    Double_t zlike; tree->Branch("zlike",&zlike,"zlike/D");
    Double_t aoq; tree->Branch("aoq",&aoq,"aoq/D");
    Int_t ppacnhit; tree->Branch("ppacnhit",&ppacnhit,"ppacnhit/I");

    for(Int_t i=0; i<beam->GetEntries(); i++){
      beam->GetEntry(i);
      t->GetEntry(i);
      event=i;
      calADC[0] = calpara0[0][runIDeach[0]] + beam->BigRIPSIC_fADC[2][0]*calpara0[1][runIDeach[0]];
      calADC[1] = calpara1[0][runIDeach[1]] + beam->BigRIPSIC_fADC[2][1]*calpara1[1][runIDeach[1]];
      calADC[2] = calpara2[0][runIDeach[2]] + beam->BigRIPSIC_fADC[2][2]*calpara2[1][runIDeach[2]];
      calADC[3] = calpara3[0][runIDeach[3]] + beam->BigRIPSIC_fADC[2][3]*calpara3[1][runIDeach[3]];
      calADC[4] = calpara4[0][runIDeach[4]] + beam->BigRIPSIC_fADC[2][4]*calpara4[1][runIDeach[4]];
      calADC[5] = calpara5[0][runIDeach[5]] + beam->BigRIPSIC_fADC[2][5]*calpara5[1][runIDeach[5]];
      h_runadc[0]->Fill(runNo,calADC[0]);
      h_runadc[1]->Fill(runNo,calADC[1]);
      h_runadc[2]->Fill(runNo,calADC[2]);
      h_runadc[3]->Fill(runNo,calADC[3]);
      h_runadc[4]->Fill(runNo,calADC[4]);
      h_runadc[5]->Fill(runNo,calADC[5]);
      h_dE[0]->Fill(calADC[0]);
      h_dE[1]->Fill(calADC[1]);
      h_dE[2]->Fill(calADC[2]);
      h_dE[3]->Fill(calADC[3]);
      h_dE[4]->Fill(calADC[4]);
      h_dE[5]->Fill(calADC[5]);
      sqsum=calADC[0];
      sqsum*=calADC[1];
      sqsum*=calADC[2];
      sqsum*=calADC[3];
      sqsum*=calADC[4];
      sqsum*=calADC[5];
      sqsum = TMath::Power(sqsum,1./6.);
      avsum=calADC[0];
      avsum+=calADC[1];
      avsum+=calADC[2];
      avsum+=calADC[3];
      avsum+=calADC[4];
      avsum+=calADC[5];
      avsum = avsum/6.;
      h_runsqsum->Fill(runNo,sqsum);

      Double_t ionpair = beam->BigRIPSIC_ionpair[2];
      beta             = beam->BigRIPSBeam_beta[0];
      aoq              = beam->BigRIPSBeam_aoq[0];
      Double_t aoq1    = beam->BigRIPSBeam_aoq[2];
      if(!(aoq<3&&aoq>1)&&(aoq1<3&&aoq1>1)){
        beta = beam->BigRIPSBeam_beta[2];
        aoq = aoq1;
      }
      Double_t de_v = TMath::Log(ionpair*beta*beta) - TMath::Log(1-beta*beta) - beta*beta;
      zlike=TMath::Sqrt(sqsum/de_v)*beta;
      h_runzetcal->Fill(runNo,TMath::Sqrt(sqsum/de_v)*beta);
      h_zlike->Fill(TMath::Sqrt(sqsum/de_v)*beta);
      Double_t zed = zedcal[0] + zedcal[1]*TMath::Sqrt(sqsum/de_v)*beta;
      Int_t nHit=0;
      for(Int_t ippac=0; ippac<12; ippac++)
        nHit += TX1raw[ippac]->size() + TX2raw[ippac]->size() + TY1raw[ippac]->size() + TY2raw[ippac]->size() + TAraw[ippac]->size();
      ppacnhit=nHit;
      if(nHit<=90){
        if(aoq>=1&&aoq<=3)h_zetcalib->Fill(TMath::Sqrt(sqsum/de_v)*beta);
        h_PID->Fill(beam->BigRIPSBeam_aoq[0],zed);
        counter++;
        if(aoq>=2.155&&aoq<=2.165&&zed>=49.5&&zed<=50.5) sn108counter++;
        if(aoq>=2.235&&aoq<=2.245&&zed>=49.5&&zed<=50.5) sn112counter++;
      }
      tree->Fill();
    }
    runvec.push_back(runNo);
    Int_t constant;
    Double_t mean, fwhm;
    for(Int_t i=0; i<6; i++){
      estimateGaussPara(h_dE[i],&constant,&mean,&fwhm);
      f_dE->SetParameters(constant, mean, fwhm);
      h_dE[i]->Fit(f_dE,"","",mean-5*fwhm,mean+5*fwhm);
      meanvec[i].push_back(f_dE->GetParameter(1));
      f_dE->Clear();
      h_dE[i]->Clear();
    }
    estimateGaussPara(h_zlike,&constant,&mean,&fwhm);
    f_dE->SetParameters(constant,mean,fwhm);
    h_zlike->Fit(f_dE,"","",mean-3.*fwhm,mean+3.*fwhm);
    zlikevec.push_back(f_dE->GetParameter(1));
    h_zlike->Clear();
    f_dE->Clear();
    
    out->Write();
    out->Close();
    delete out;
    
    beam->Clear();
    t->Clear();
  }
/*
  auto crunadc = new TCanvas("crunadc","",1400,600);
  crunadc->Divide(3,2);
  for(Int_t i=0; i<6; i++){
    crunadc->cd(i+1);
    h_runadc[i]->Draw("colz");
  }
  */
  auto crunsqsum = new TCanvas("crunsqsum","",1400,600);
  h_runsqsum->Draw("colz");
  
  auto czetcalib = new TCanvas("czetcalib","");
  czetcalib->SetLogy();
  h_zetcalib->Draw(); 
  //h_runzetcal->Draw("colz"); 
  auto f = new TF1("f","gaus(0)+gaus(3)+gaus(6)");
  auto f1 = new TF1("f1","gaus");
  auto f2 = new TF1("f3","gaus");
  auto f3 = new TF1("f2","gaus");
  f->SetParameters(h_zetcalib->GetMaximum(),h_zetcalib->GetBinCenter(h_zetcalib->GetMaximumBin()),0.01,
                   0.2*h_zetcalib->GetMaximum(),h_zetcalib->GetBinCenter(h_zetcalib->GetMaximumBin())-0.04,0.01,
                   0.2*h_zetcalib->GetMaximum(),h_zetcalib->GetBinCenter(h_zetcalib->GetMaximumBin())+0.04,0.01);
  f1->SetParameters(0.001*h_zetcalib->GetMaximum(),1.845,0.01);
  f2->SetParameters(0.005*h_zetcalib->GetMaximum(),1.885,0.01);
  f3->SetParameters(0.001*h_zetcalib->GetMaximum(),1.8,0.01);
  h_zetcalib->Fit(f); 
  h_zetcalib->Fit(f1,"+","",1.83,1.86); 
  h_zetcalib->Fit(f2,"+","",1.87,1.9); 
  h_zetcalib->Fit(f3,"+","",1.78,1.82);
  auto gzetcalib = new TCanvas("gzetcalib","");
  TGraph* g = new TGraph(6);
  g->SetPoint(0,f->GetParameter(1),50);
  g->SetPoint(1,f->GetParameter(4),49);
  g->SetPoint(2,f->GetParameter(7),51);
  g->SetPoint(3,f1->GetParameter(1),47);
  g->SetPoint(4,f2->GetParameter(1),48);
  g->SetPoint(5,f3->GetParameter(1),46);
  g->SetMarkerStyle(8);
  g->Draw("AP");
  g->SetName("g_zedcalib");
  auto fg = new TF1("fg","[0]+[1]*x");
  g->Fit(fg,"");
  zedcal[0]=fg->GetParameter(0);
  zedcal[1]=fg->GetParameter(1);
  
  
  auto crunzetcalib = new TCanvas("crunzetcalib","");
  h_runzetcal->Draw("colz"); 
  
  /*
  auto cPID = new TCanvas("cPID","");
  cPID->SetGridx(1);
  cPID->SetGridy(1);
  h_PID->Draw("colz");
  std::cout<<"numEvents: "<<counter<<std::endl; 
  if(beamtype==108) std::cout<<"num108Events: "<<sn108counter<<std::endl; 
  else std::cout<<"num112Events: "<<sn112counter<<std::endl; 
  */
  auto c_mg = new TCanvas("c_mg","");
  auto mg_dE = new TMultiGraph("mg_dE","");
  TGraph* g_dE[6];
  for(Int_t i=0; i<6; i++){
    g_dE[i]=new TGraph(runvec.size());
    g_dE[i]->SetMarkerStyle(3); g_dE[i]->SetMarkerColor(i+2); g_dE[i]->SetLineColor(i+2);
    g_dE[i]->SetName(Form("g_dE%d",i));
    for(Int_t j=0; j<runvec.size(); j++){
      g_dE[i]->SetPoint(j,runvec.at(j),meanvec[i].at(j));
    }
    mg_dE->Add(g_dE[i]);
  }
  mg_dE->Draw("APL");
  
  auto c_g = new TCanvas("c_g","");
  auto g_z = new TGraph(runvec.size());
  for(Int_t i=0; i<runvec.size(); i++) g_z->SetPoint(i,runvec.at(i),zlikevec.at(i));
  g_z->SetMarkerStyle(3);
  g_z->SetName("g_pseudozed");
  g_z->Draw("APL");


  
  auto file = new TFile(Form(anapath+"macros_3/supplemental/CalIC%d.2.root",beamtype),"recreate");
  for(Int_t i=0; i<ndivision[0]; i++)  g_adcdE0[i]->Write();
  for(Int_t i=0; i<ndivision[1]; i++)  g_adcdE1[i]->Write();
  for(Int_t i=0; i<ndivision[2]; i++)  g_adcdE2[i]->Write();
  for(Int_t i=0; i<ndivision[3]; i++)  g_adcdE3[i]->Write();
  for(Int_t i=0; i<ndivision[4]; i++)  g_adcdE4[i]->Write();
  for(Int_t i=0; i<ndivision[5]; i++)  g_adcdE5[i]->Write();
  for(Int_t i=0; i<6; i++) h_runadc[i]->Write();
  h_runsqsum->Write();
  h_runzetcal->Write();
  h_zetcalib->Write();
  g->Write();
  for(Int_t i=0; i<6; i++) g_dE[i]->Write();
  g_z->Write();
  h_PID->Write();
 
  
  std::ofstream fout;
  fout.open(Form(anapath+"macros_3/supplemental/icCalibration_%d.txt",beamtype),std::ios::out);
  fout << zedcal[0] << " " << zedcal[1] << std::endl;
  fout.close();

}

void loadFiles(Int_t bt){
  if(!(bt==108||bt==112)) return 0;
  Int_t start=2174;
  Int_t stop=2509;
  if(bt==112){ start=2520; stop=2653;}

  runNolist.clear();
  FileStat_t file;
  for(Int_t irun=start; irun<=stop; irun++){
    if( !gSystem->GetPathInfo( Form(datapath+"run%04d.reco.root",irun),file) ){   
    //if( !gSystem->GetPathInfo( Form(datapath+"run%04d.ridf.root",irun),file) ){   
      if(irun==2260||irun==2296||irun==2308||irun==2309||irun==2345||irun==2404) continue;// those are junk
      if(irun==2541||irun==2577||irun==2630||irun==2639) continue;// those are junk
      if(irun==2612) continue;// this run should be calibrated by hand later.
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

void makeICADCMean(){
  
  if(!isFileLoaded) loadFiles(beamtype);
  
  TH1I* h_adc[6]; auto f_adc = new TF1("f_adc","gaus");
  TH1D* h_sqsum;  auto f_sqsum = new TF1("f_sqsum","gaus");
  TH1D* h_zlike;  auto f_zlike = new TF1("f_zlike","gaus"); 
  Int_t start=runNolist.front(), end=runNolist.back(), nrun=runNolist.size();
  TH2I* h_runadc[6];
  for(Int_t i=0; i<6; i++) h_runadc[i] = new TH2I(Form("h_runadc%d",i),"",end+1-start,start,end+1,1600,3400,5000);
  auto h_runzlike = new TH2D("h_runzlike","",end+1-start,start,end+1,100,14.,17.);
 
  TVector run(nrun), sqmean(nrun), zmean(nrun);
  TVector amean[6]; for(Int_t i=0; i<6; i++) amean[i].ResizeTo(nrun);

  auto c = new TCanvas("c","");
  Int_t counter=0;
  for(auto runNo: runNolist){
    auto beam = new BeamReco();
    beam->fChain->AddFile(Form(datapath+"run%04d.reco.root",runNo),0,"tree");
    beam->InitRawPID();
    std::cout<<"RUN"<<runNo<<std::endl; 
    run(counter)=runNo;
    for(Int_t i=0; i<6; i++){
      h_adc[i] = new TH1I(Form("h_adc%d",i),"",800,3400,5000);
      beam->fChain->Project(Form("h_adc%d",i),Form("BigRIPSIC.fADC[2][%d]",i));
      Int_t constant; Double_t adcMean, fwhm;
      estimateGaussPara(h_adc[i],&constant,&adcMean,&fwhm);                       
      f_adc->SetParameters(constant,adcMean,fwhm);
      h_adc[i]->Fit(f_adc,"OQ","",adcMean-5.*fwhm, adcMean+5.*fwhm);
      amean[i](counter) = f_adc->GetParameter(1);
      h_adc[i]->Clear(); f_adc->Clear(); 
    }
    h_sqsum = new TH1D("h_sqsum","",600,3600,4800);
    beam->fChain->Project("h_sqsum","BigRIPSIC.fRawADCSqSum[2]");
    Int_t constant; Double_t sqsumMean, fwhm;
    estimateGaussPara(h_sqsum,&constant,&sqsumMean,&fwhm);
    f_sqsum->SetParameters(constant,sqsumMean,fwhm);
    h_sqsum->Fit(f_sqsum,"OQ","",sqsumMean-3.*fwhm,sqsumMean+3.*fwhm);
    sqmean(counter) = f_sqsum->GetParameter(1);
    h_sqsum->Clear(); f_sqsum->Clear();
    
    h_zlike = new TH1D("h_zlike","",120,14.,17.);
    for(Int_t i=0; i<beam->GetEntries(); i++){
      beam->GetEntry(i);
      for(Int_t j=0; j<6; j++) h_runadc[j]->Fill(runNo,beam->BigRIPSIC_fADC[2][j]);
      Double_t sqsum   = beam->BigRIPSIC_fRawADCSqSum[2];
      Double_t ionpair = beam->BigRIPSIC_ionpair[2];
      Double_t beta    = beam->BigRIPSBeam_beta[0]; 
      Double_t aoq     = beam->BigRIPSBeam_aoq[0];
      Double_t aoq1    = beam->BigRIPSBeam_aoq[2];
      if(!(aoq<3&&aoq>1)&&(aoq1<3&&aoq1>1)) beta = beam->BigRIPSBeam_beta[2];
      Double_t de_v = TMath::Log(ionpair*beta*beta) - TMath::Log(1-beta*beta) - beta*beta;
      h_zlike->Fill(TMath::Sqrt(sqsum/de_v)*beta);
      h_runzlike->Fill(runNo,TMath::Sqrt(sqsum/de_v)*beta);
    }
    Double_t zlikeMean;
    estimateGaussPara(h_zlike,&constant,&zlikeMean,&fwhm);
    f_zlike->SetParameters(constant,zlikeMean,fwhm);
    h_zlike->Fit(f_zlike,"OQ","",zlikeMean-2.*fwhm,zlikeMean+2.*fwhm);
    zmean(counter) = f_zlike->GetParameter(1);
    h_zlike->Clear(); f_zlike->Clear();
    
    beam->Clear();
    counter++;
  }

  c->Clear(); delete c;

  auto c1 = new TCanvas("c1","",1000,600);
  c1->Divide(3,2);
  for(Int_t i=0; i<6; i++){
    c1->cd(i+1);
    h_runadc[i]->Draw("colz");
  }

  auto c2 = new TCanvas("c2","",1000,600);
  TGraph* g_adc[6];
  auto mg_adc = new TMultiGraph("mg_adc","");
  for(Int_t i=0; i<6; i++){
    g_adc[i] = new TGraph(run,amean[i]);
    g_adc[i]->SetMarkerStyle(3);
    g_adc[i]->SetMarkerColor(3+i);
    g_adc[i]->SetLineColor(3+i);
    g_adc[i]->SetName(Form("g_adc%d",i));
    mg_adc->Add(g_adc[i]);
  }
  mg_adc->Draw("APL");

  
  auto out = new TFile(Form(anapath+"macros_3/supplemental/icADCMean%d.root",beamtype),"recreate");
  for(Int_t i=0; i<6; i++){
    h_runadc[i]->Write();
    g_adc[i]->Write();
  }
  mg_adc->Write();

  auto g_sqsum = new TGraph(run,sqmean);
  g_sqsum->SetName("g_sqsum");
  g_sqsum->Write();
  
  auto czlike = new TCanvas("czlike","");
  auto g_zlike = new TGraph(run,zmean);
  g_zlike->SetName("g_zlike");
  g_zlike->SetMarkerStyle(8);
  g_zlike->Draw("APL");
  g_zlike->Write();
  h_runzlike->Write();

  out->Close();
}



void makeZHistograms(){
  
  if(!isFileLoaded) loadFiles(beamtype);
  
  // clarify runs for Sqsum to make each zed conditions. 
  auto inADCMean = new TFile(Form(anapath+"macros_3/supplemental/icADCMean%d.root",beamtype),"read");
  auto g_zlike = (TGraph*)inADCMean->Get("g_zlike");
  Double_t maxmean=TMath::MaxElement(g_zlike->GetN(),g_zlike->GetY());
  Double_t minmean=TMath::MinElement(g_zlike->GetN(),g_zlike->GetY());
  Int_t npartition = (maxmean-minmean)/zlikegap+1;
  std::vector<Int_t> subrunarray[npartition];
  std::vector<Double_t> zlikearray[npartition];
  std::vector<Double_t> zlikecenterarray[npartition];
  Int_t neve[npartition];
  Bool_t isExist;
  Int_t ndivision=0;

  for(Int_t j=0; j<npartition; j++){
    subrunarray[j].clear();
    zlikearray[j].clear();
    zlikecenterarray[j].clear();
    neve[j]=0;
    isExist=kFALSE;
    for(Int_t k=0; k<g_zlike->GetN(); k++)
      if(g_zlike->GetY()[k]>maxmean-zlikegap*(j+1) && g_zlike->GetY()[k]<=maxmean-zlikegap*j){
        subrunarray[ndivision].push_back(g_zlike->GetX()[k]);
        zlikearray[ndivision].push_back(g_zlike->GetY()[k]);
        zlikecenterarray[ndivision].push_back(maxmean-zlikegap*(j+1./2.));
        auto t = new TChain("t","");
        t->AddFile(Form(anapath+"rootfiles/plastic/run%04d.pla.root",(Int_t)g_zlike->GetX()[k]),0,"tree");
        neve[ndivision]+=t->GetEntries();
        t->Clear(); delete t;
        isExist=kTRUE;
        std::cout<<"Zlike: "<<g_zlike->GetX()[k]<< " " <<g_zlike->GetY()[k]<< std::endl;
      }

    if(isExist){ 
      std::cout<<"ndivision:"<<ndivision<<std::endl;
      std::cout<<"nentry:"<<neve[ndivision]<<std::endl;
      ndivision++;
    }
  }

  Int_t narray=ndivision;
  for(Int_t j=0; j<ndivision; j++){
    if(neve[j]<10000){
      if(j==0){
        for(Int_t k=0; k<subrunarray[j].size(); k++)
          subrunarray[j+1].push_back(subrunarray[j].at(k));
      }else if(j==ndivision-1){ 
        for(Int_t k=0; k<subrunarray[j].size(); k++)
          subrunarray[j-1].push_back(subrunarray[j].at(k));
      }else{
        for(Int_t k=0; k<subrunarray[j].size(); k++){
          if(zlikearray[j].at(k)>=zlikecenterarray[j].at(k)){
            subrunarray[j-1].push_back(subrunarray[j].at(k));
          }else{
            subrunarray[j+1].push_back(subrunarray[j].at(k));
          }
        }
      }
      subrunarray[j].clear();
      narray--;
    }
  }

  std::vector<Int_t> runarray[narray];
  Int_t iarray=0;
  for(Int_t j=0; j<ndivision; j++){
    if(subrunarray[j].size()>0){
      std::sort(subrunarray[j].begin(),subrunarray[j].end());
      runarray[iarray] = subrunarray[j];
      iarray++;
    }
  }
  for(Int_t j=0; j<narray; j++){
    neve[j]=0;
    std::cout<<"arrayID"<<j<<std::endl;
    for(Int_t k=0; k<runarray[j].size(); k++){
      std::cout<<"run"<<runarray[j].at(k)<<std::endl;
      auto t = new TChain("t","");
      t->AddFile(Form(anapath+"rootfiles/plastic/run%04d.pla.root",(Int_t)g_zlike->GetX()[k]),0,"tree");
      neve[j]+=t->GetEntries();
      t->Clear(); delete t;
    }
    std::cout<<"nentry:"<<neve[j]<<std::endl;
  }
  g_zlike->Clear(); 
  delete g_zlike;
  
  inADCMean->Clear();
  delete inADCMean;
  
  ndivision=narray; 

  auto out = new TFile(Form(anapath+"macros_3/supplemental/icZedlike%d.root",beamtype),"recreate");
  auto runtree = new TTree("runtree","");
  std::vector<Int_t> runArray;
  runtree->Branch("runarray",&runArray); 
  for(Int_t j=0; j<ndivision; j++){
      runArray=runarray[j];
      runtree->Fill();
  }
  
  TH1D* h_zlike[ndivision];
  TH2F* h_runzlike[ndivision];
  TH2D* h_zlikeaoq[ndivision];
  
  Double_t aoqrange[2]={2.1,2.22};
  if(beamtype==112){aoqrange[0]=2.18; aoqrange[1]=2.3;}
  for(Int_t j=0; j<ndivision; j++){
    if(runarray[j].size()==0)continue;
    
    h_zlikeaoq[j] = new TH2D(Form("h_zlikeaoq%d",j),"",200,aoqrange[0],aoqrange[1],240,14,17);
    Int_t start=runarray[j].front();
    Int_t end  =runarray[j].back();
    h_zlike[j] = new TH1D(Form("h_zlike%d",j),"",240,14,17);
    h_runzlike[j] = new TH2F(Form("h_runzlike%d",j),"",end+1-start,start,end+1,240,14,17);
    
    
    for(auto runNo: runarray[j]){
      BeamReco* beam = new BeamReco();
      TString fname = Form(datapath+"run%04d.reco.root",runNo);
      beam->fChain->AddFile(fname.Data(),0,"tree");
      std::cout<<"RUN"<<runNo<<std::endl;
      beam->InitRawPID();
      
      auto t = new TChain("t","");
      t->AddFile(Form(anapath+"rootfiles/ppac/run%04d.ppac.root",runNo),0,"tree");
      std::vector<Int_t> *TX1raw[nPPAC]={};
      std::vector<Int_t> *TX2raw[nPPAC]={};
      std::vector<Int_t> *TY1raw[nPPAC]={};
      std::vector<Int_t> *TY2raw[nPPAC]={};
      std::vector<Int_t> *TAraw[nPPAC]={};
      for(Int_t i=0; i<nPPAC; i++){
        t->SetBranchAddress(ppacname[i]+"_TX1",&TX1raw[i]);
        t->SetBranchAddress(ppacname[i]+"_TY1",&TY1raw[i]);
        t->SetBranchAddress(ppacname[i]+"_TX2",&TX2raw[i]);
        t->SetBranchAddress(ppacname[i]+"_TY2",&TY2raw[i]);
        t->SetBranchAddress(ppacname[i]+"_TA",&TAraw[i]);
      }

      for(Int_t k=0; k<beam->GetEntries(); k++){
        beam->GetEntry(k);
        t->GetEntry(k);
        Int_t nHit=0;
        for(Int_t ippac=0; ippac<12; ippac++)
          nHit += TX1raw[ippac]->size() + TX2raw[ippac]->size() + TY1raw[ippac]->size() + TY2raw[ippac]->size() + TAraw[ippac]->size();
        Double_t sqsum   = beam->BigRIPSIC_fRawADCSqSum[2];
        Double_t ionpair = beam->BigRIPSIC_ionpair[2];
        Double_t beta    = beam->BigRIPSBeam_beta[2];
        Double_t aoq     = beam->BigRIPSBeam_aoq[0];
        Double_t aoq1    = beam->BigRIPSBeam_aoq[2];
        if(!(aoq<3&&aoq>1)&&(aoq1<3&&aoq1>1)) beta = beam->BigRIPSBeam_beta[2];
        Double_t de_v    = TMath::Log(ionpair*beta*beta) - TMath::Log(1-beta*beta) - beta*beta;
        Double_t zlike    = TMath::Sqrt(sqsum/de_v)*beta;
        if(sqsum>5000)continue; 
        if(nHit<=90){
          h_zlikeaoq[j]->Fill(beam->BigRIPSBeam_aoq[0],zlike);

          h_zlike[j]->Fill(zlike);
          h_runzlike[j]->Fill(runNo,zlike);
        }
        
      }
      beam->Clear();
      t->Clear();
    }
    
  }
  
  
  out->Write();

  Int_t ncvs = ndivision/9+1;
   
  TCanvas* ca[ncvs];
  for(Int_t i=0; i<ncvs; i++){
    ca[i] = new TCanvas(Form("ca%d",i),"");
    ca[i]->Divide(3,3);
    for(Int_t j=0; j<9; j++){
      ca[i]->cd(j+1);
      if(j+i*9<ndivision)h_runzlike[j+i*9]->Draw("colz");
    }
  }
   

  TCanvas* c[2*ncvs];
  for(Int_t i=0; i<2*ncvs; i++){
    c[i] = new TCanvas(Form("c%d",i),"",1200,600);
    c[i]->Divide(3,3);
  }
  auto f_zlike = new TF1("f_zlike","gaus(0)+gaus(3)+gaus(6)");
  
  
  for(Int_t i=0; i<ncvs; i++){
    for(Int_t j=0; j<9; j++){
      c[i]->cd(j+1);
      if(j+i*9>=ndivision)continue;
      h_zlike[j+i*9]->Draw();
      Int_t height = h_zlike[j+i*9]->GetMaximum();
      Double_t mean = h_zlike[j+i*9]->GetBinCenter(h_zlike[j+i*9]->GetMaximumBin());
      f_zlike->SetParameters(height,mean,0.06,0.2*height,mean+0.3,0.08,0.5*height,mean-0.3,0.06);
      h_zlike[j+i*9]->Fit(f_zlike,"","");
      f_zlike->Clear();
      c[i+ncvs]->cd(j+1);
      h_zlikeaoq[j+i*9]->Draw("colz");
    }
  }

}

void getZCondition(Int_t idiv, Double_t* min, Double_t *max){
  
  auto inzlike = new TFile(Form(anapath+"macros_3/supplemental/icZedlike%d.root",beamtype),"read");
  auto h_zlike = (TH1D*)inzlike->Get(Form("h_zlike%d",idiv));
  auto f_zlike = new TF1("f_zlike","gaus(0)+gaus(3)+gaus(6)");
  auto c = new TCanvas("c","");
  h_zlike->Draw();
  Int_t height; Double_t mean,sigma;
  estimateGaussPara(h_zlike,&height,&mean,&sigma);
  f_zlike->SetParameters(height,mean,sigma,0.2*height,mean+0.3,sigma,0.5*height,mean-0.3,sigma);
  h_zlike->Fit(f_zlike,"OQ","");
  Int_t nZ=3; //Z=50,51,49
  Double_t nsigma[3]={2.5,1.6,1.8};
  for(Int_t i=0; i<nZ; i++){
    min[i] = f_zlike->GetParameter(1+3*i) - nsigma[i]*f_zlike->GetParameter(2+3*i);
    max[i] = f_zlike->GetParameter(1+3*i) + nsigma[i]*f_zlike->GetParameter(2+3*i);
    std::cout<<min[i]<<" "<<max[i]<<std::endl;
  }
  f_zlike->Clear(); delete f_zlike;
  c->Clear(); delete c;  
}


void makeADCeach(){

  if(!isFileLoaded) loadFiles(beamtype);
  
  
  // get run clarification -> which was used to clarify runs by its pseudo-Z.
  auto inZlike = new TFile(Form(anapath+"macros_3/supplemental/icZedlike%d.root",beamtype),"read");
  auto tr = (TTree*)inZlike->Get("runtree");
  std::vector<Int_t> *runvec=0;
  TBranch* brunvec=0;
  tr->SetBranchAddress("runarray",&runvec,&brunvec);
  Int_t nzlikedivision=tr->GetEntries();
  std::vector<Int_t> zlikerunarray[nzlikedivision];
  for(Int_t i=0; i<tr->GetEntries(); i++){
    runvec->clear();
    tr->GetEntry(i);
    for(Int_t j=0; j<runvec->size(); j++) zlikerunarray[i].push_back(runvec->at(j));
  }
  //tr->Clear(); delete tr;
  //inZlike->Clear(); delete inZlike;
   
  // get pseudo Z conditions
  Int_t nZ=3; //Z=50,51,49
  Double_t minzlike[nzlikedivision][nZ];  // pseudoZ condition for each isotope.
  Double_t maxzlike[nzlikedivision][nZ];
  //TH1D* h_aoq_eachZ[nZ][npzeddivision];
  Double_t aoqrange[2]={2.1,2.22};
  if(beamtype==112){aoqrange[0]=2.18; aoqrange[1]=2.3;}
  for(Int_t i=0; i<nzlikedivision; i++){
    if(zlikerunarray[i].size()==0) continue;
    getZCondition(i,minzlike[i],maxzlike[i]);
    //for(Int_t j=0; j<nZ; j++) std::cout<<minpzed[i][j]<<" "<<maxpzed[i][j]<<std::endl;
    /*
    for(Int_t j=0; j<nZ; j++) h_aoq_eachZ[j][i] = new TH1D(Form("h_aoq_eachZ%d_%d",j,i),"",200,aoqrange[0],aoqrange[1]);
    for(auto runNo: pzedrunarray[i]){
      BeamRaw* raw = new BeamRaw(); 
      TString fname = Form(datapath+"run%04d.reco.root",runNo);
      raw->fChain->AddFile(fname.Data(),0,"tree");
      raw->InitRawPID();
      std::cout<<"RUN"<<runNo<<std::endl;
      for(Int_t k=0; k<raw->fChain->GetEntries(); k++){
        raw->GetEntry(k);
        Double_t sqsum   = raw->BigRIPSIC_fRawADCSqSum[2];
        Double_t ionpair = raw->BigRIPSIC_ionpair[2];
        Double_t beta    = raw->BigRIPSBeam_beta[0];
        Double_t de_v    = TMath::Log(ionpair*beta*beta) - TMath::Log(1-beta*beta) - beta*beta;
        Double_t pzed    = TMath::Sqrt(sqsum/de_v)*beta;
        for(Int_t j=0; j<nZ; j++){
          if(pzed>=minpzed[i][j]&&pzed<=maxpzed[i][j]){
            h_aoq_eachZ[j][i]->Fill(raw->BigRIPSBeam_aoq[0]);
          }
        }
      }

      raw->Clear();
    }
    */
  }
 
 /* 
  TCanvas* caoq[nZ];
  for(Int_t i=0; i<nZ; i++){
    caoq[i] = new TCanvas(Form("caoq%d",i),"",1200,700);
    caoq[i]->Divide(7,4);
    for(Int_t j=0; j<npzeddivision; j++){
      caoq[i]->cd(j+1);
      h_aoq_eachZ[i][j]->Draw();
    }
  }*/


  // get each channels of ADC graph (which PID condition should be used?)
  auto inADCMean = new TFile(Form(anapath+"macros_3/supplemental/icADCMean%d.root",beamtype),"read");
  TGraph* g_adc[6];
  Double_t maxmean[6], minmean[6];
  std::vector<Int_t> subrunarray[6][40];
  std::vector<Int_t> runarray[6][40];
  std::vector<Double_t> zlikearray[6][40];
  std::vector<Double_t> zlikecenterarray[6][40];
  Bool_t isExist=kFALSE;
  Int_t ndivision[6];
  Int_t neve[6][40];
  for(Int_t i=0; i<6; i++){
    maxmean[i]=0;
    minmean[i]=0;
    ndivision[i]=0;
    g_adc[i] = (TGraph*)inADCMean->Get(Form("g_adc%d",i));
    maxmean[i]=TMath::MaxElement(g_adc[i]->GetN(),g_adc[i]->GetY());
    minmean[i]=TMath::MinElement(g_adc[i]->GetN(),g_adc[i]->GetY());
    Int_t npartition = (maxmean[i]-minmean[i])/adcgap+1;
    std::cout<<"npartition:"<<npartition<<std::endl;
    for(Int_t j=0; j<npartition; j++){
      neve[i][j]=0;
      isExist=kFALSE;
      subrunarray[i][j].clear();
      runarray[i][j].clear();
      zlikearray[i][j].clear();
      zlikecenterarray[i][j].clear();
      for(Int_t k=0; k<g_adc[i]->GetN(); k++)
        if(g_adc[i]->GetY()[k]>maxmean[i]-adcgap*(j+1) && g_adc[i]->GetY()[k]<=maxmean[i]-adcgap*j){
          if(!isExist)std::cout<<"divID"<<j<<std::endl;
          subrunarray[i][ndivision[i]].push_back(g_adc[i]->GetX()[k]);
          zlikearray[i][ndivision[i]].push_back(g_adc[i]->GetY()[k]);
          zlikecenterarray[i][ndivision[i]].push_back(maxmean[i]-adcgap*(j+1./2.));
          auto t = new TChain("t","");
          t->AddFile(Form(anapath+"rootfiles/ppac/run%04d.ppac.root",(Int_t)g_adc[i]->GetX()[k]),0,"tree");
          neve[i][ndivision[i]]+=t->GetEntries();
          t->Clear(); delete t;
          isExist=kTRUE;
          std::cout<<"RUN"<<g_adc[i]->GetX()[k]<<std::endl;
        }

      if(isExist){
        std::cout<<"neve:"<<neve[i][ndivision[i]]<<std::endl;
        ndivision[i]++;
      }
    }
    g_adc[i]->Clear();
    std::cout<<"ADCchannel"<<i<<" ndivision:"<<ndivision[i]<<std::endl; 
    
    if(neve[i][0]<5000){
      for(Int_t k=0; k<ndivision[i]-1; k++){
        for(Int_t l=0; l<subrunarray[i][k+1].size(); l++)
          subrunarray[i][k].push_back(subrunarray[i][k+1].at(l));
        subrunarray[i][k+1].clear();
        neve[i][k]+=neve[i][k+1];
        neve[i][k+1]=0;
      }
      ndivision[i]--;
    }

    if(neve[i][ndivision[i]-1]<5000){
      for(Int_t j=0; j<subrunarray[i][ndivision[i]-1].size(); j++)
        subrunarray[i][ndivision[i]-2].push_back(subrunarray[i][ndivision[i]-1].at(j));
      subrunarray[i][ndivision[i]-1].clear();
      neve[i][ndivision[i]-2]+=neve[i][ndivision[i]-1];
      neve[i][ndivision[i]-1]=0;

      ndivision[i]--;
    }
      
    
     
    //Int_t narray=ndivision[i];
    for(Int_t j=0; j<ndivision[i]; j++){
      if(neve[i][j]<5000){
        if(j==0){
          for(Int_t k=0; k<subrunarray[i][j].size(); k++)
            subrunarray[i][j+1].push_back(subrunarray[i][j].at(k));
        }else if(j==ndivision[i]-1){ 
          for(Int_t k=0; k<subrunarray[i][j].size(); k++)
            subrunarray[i][j-1].push_back(subrunarray[i][j].at(k));
        }else{
          for(Int_t k=0; k<subrunarray[i][j].size(); k++){
            if(subrunarray[i][j-1].size()!=0&&zlikearray[i][j].at(k)>=zlikecenterarray[i][j].at(k)){
              subrunarray[i][j-1].push_back(subrunarray[i][j].at(k));
            }else{
              subrunarray[i][j+1].push_back(subrunarray[i][j].at(k));
            }
          }
        }
        subrunarray[i][j].clear();
      //  narray--;
      }
    }

    Int_t iarray=0;
    for(Int_t j=0; j<ndivision[i]; j++){
      if(subrunarray[i][j].size()>0){
        std::sort(subrunarray[i][j].begin(),subrunarray[i][j].end());
        runarray[i][iarray] = subrunarray[i][j];
        iarray++;
      }
    }
  
    for(Int_t j=0; j<iarray; j++){
      neve[i][j]=0;
      std::cout<<"arrayID"<<j<<std::endl;
      for(Int_t k=0; k<runarray[i][j].size(); k++){
        std::cout<<"run"<<runarray[i][j].at(k)<<std::endl;
        auto t = new TChain("t","");
        t->AddFile(Form(anapath+"rootfiles/plastic/run%04d.pla.root",runarray[i][j].at(k)),0,"tree");
        neve[i][j]+=t->GetEntries();
        t->Clear(); delete t;
      }
      std::cout<<"nentry:"<<neve[i][j]<<std::endl;
    }
    ndivision[i]=iarray;

  }
  inADCMean->Clear();
  delete inADCMean;



  Int_t npid=7; if(beamtype==112)npid=6; 
  // each ADC channel*ndivision[i]*each PID
  TH1D *h_adc0_nosel[ndivision[0]], *h_adc1_nosel[ndivision[1]], *h_adc2_nosel[ndivision[2]];
  TH1D *h_adc3_nosel[ndivision[3]], *h_adc4_nosel[ndivision[4]], *h_adc5_nosel[ndivision[5]];
  TH1D *h_adc0[ndivision[0]][npid], *h_adc1[ndivision[1]][npid], *h_adc2[ndivision[2]][npid];
  TH1D *h_adc3[ndivision[3]][npid], *h_adc4[ndivision[4]][npid], *h_adc5[ndivision[5]][npid];
  for(Int_t ipid=0; ipid<npid; ipid++){
    for(Int_t j=0; j<ndivision[0]; j++) h_adc0[j][ipid] = new TH1D(Form("h_adc0_%d_%d",j,ipid),"",300,3600,4800);
    for(Int_t j=0; j<ndivision[1]; j++) h_adc1[j][ipid] = new TH1D(Form("h_adc1_%d_%d",j,ipid),"",300,3600,4800);
    for(Int_t j=0; j<ndivision[2]; j++) h_adc2[j][ipid] = new TH1D(Form("h_adc2_%d_%d",j,ipid),"",300,3600,4800);
    for(Int_t j=0; j<ndivision[3]; j++) h_adc3[j][ipid] = new TH1D(Form("h_adc3_%d_%d",j,ipid),"",300,3600,4800);
    for(Int_t j=0; j<ndivision[4]; j++) h_adc4[j][ipid] = new TH1D(Form("h_adc4_%d_%d",j,ipid),"",300,3600,4800);
    for(Int_t j=0; j<ndivision[5]; j++) h_adc5[j][ipid] = new TH1D(Form("h_adc5_%d_%d",j,ipid),"",300,3600,4800);
  }
  for(Int_t j=0; j<ndivision[0]; j++) h_adc0_nosel[j] = new TH1D(Form("h_adc0_nosel%d",j),"",400,3600,4800);
  for(Int_t j=0; j<ndivision[1]; j++) h_adc1_nosel[j] = new TH1D(Form("h_adc1_nosel%d",j),"",400,3600,4800);
  for(Int_t j=0; j<ndivision[2]; j++) h_adc2_nosel[j] = new TH1D(Form("h_adc2_nosel%d",j),"",400,3600,4800);
  for(Int_t j=0; j<ndivision[3]; j++) h_adc3_nosel[j] = new TH1D(Form("h_adc3_nosel%d",j),"",400,3600,4800);
  for(Int_t j=0; j<ndivision[4]; j++) h_adc4_nosel[j] = new TH1D(Form("h_adc4_nosel%d",j),"",400,3600,4800);
  for(Int_t j=0; j<ndivision[5]; j++) h_adc5_nosel[j] = new TH1D(Form("h_adc5_nosel%d",j),"",400,3600,4800);
  
  Double_t aoqmin[npid], aoqmax[npid]; 
  Double_t aoqmin108[7]={2.156,2.136,2.176,2.15, 2.132,2.158,2.178};//108Sn, 107Sn, 109Sn, 110Sb, 109Sb, 106In, 107In
  Double_t aoqmax108[7]={2.164,2.144,2.182,2.159,2.141,2.17, 2.19};
  Double_t aoqmin112[6]={2.236,2.216,2.23,2.21,2.24,2.26};//112Sn, 111Sn, 114Sb, 113Sb, 110In, 111In
  Double_t aoqmax112[6]={2.244,2.224,2.24,2.22,2.25,2.27};
  if(beamtype==108)
    for(Int_t i=0; i<npid; i++){aoqmin[i]=aoqmin108[i]; aoqmax[i]=aoqmax108[i];}
  if(beamtype==112)
    for(Int_t i=0; i<npid; i++){aoqmin[i]=aoqmin112[i]; aoqmax[i]=aoqmax112[i];}
  Int_t nSn=3;
  Int_t nSb=2;
  Int_t nIn=2;
  if(beamtype==112){nSn=2; nSb=2; nIn=2;}
  for(auto runNo: runNolist){
    BeamReco* beam = new BeamReco();
    TString fname = Form(datapath+"run%04d.reco.root",runNo);
    beam->fChain->AddFile(fname.Data(),0,"tree");
    beam->InitRawPID();
    std::cout<<"RUN"<<runNo<<std::endl;

    // get PID condition of corresponding run number
    Int_t runID=0;
    for(Int_t l=0; l<nzlikedivision; l++)
      for(Int_t k=0; k<zlikerunarray[l].size(); k++)
        if(runNo==zlikerunarray[l].at(k))
          runID=l;

    Int_t runIDeach[6];
    for(Int_t i=0; i<6; i++)
      for(Int_t j=0; j<ndivision[i]; j++)
        for(Int_t k=0; k<runarray[i][j].size(); k++)
          if(runNo==runarray[i][j].at(k)) runIDeach[i]=j;

    auto t = new TChain("t","");
    t->AddFile(Form(anapath+"rootfiles/ppac/run%04d.ppac.root",runNo),0,"tree");
    std::vector<Int_t> *TX1raw[nPPAC]={};
    std::vector<Int_t> *TX2raw[nPPAC]={};
    std::vector<Int_t> *TY1raw[nPPAC]={};
    std::vector<Int_t> *TY2raw[nPPAC]={};
    std::vector<Int_t> *TAraw[nPPAC]={};
    for(Int_t i=0; i<nPPAC; i++){
      t->SetBranchAddress(ppacname[i]+"_TX1",&TX1raw[i]);
      t->SetBranchAddress(ppacname[i]+"_TY1",&TY1raw[i]);
      t->SetBranchAddress(ppacname[i]+"_TX2",&TX2raw[i]);
      t->SetBranchAddress(ppacname[i]+"_TY2",&TY2raw[i]);
      t->SetBranchAddress(ppacname[i]+"_TA",&TAraw[i]);
    }

    for(Int_t i=0; i<beam->GetEntries(); i++){
      beam->GetEntry(i);
      t->GetEntry(i);
      Int_t nHit=0;
      for(Int_t ippac=0; ippac<12; ippac++)
        nHit += TX1raw[ippac]->size() + TX2raw[ippac]->size() + TY1raw[ippac]->size() + TY2raw[ippac]->size() + TAraw[ippac]->size();
      Double_t TOF     = beam->BigRIPSTOF_tof[0];
      Double_t aoq     = beam->BigRIPSBeam_aoq[0];
      Double_t sqsum   = beam->BigRIPSIC_fRawADCSqSum[2];
      Double_t ionpair = beam->BigRIPSIC_ionpair[2];
      Double_t beta    = beam->BigRIPSBeam_beta[0];
      Double_t aoq1    = beam->BigRIPSBeam_aoq[2];
      if(!(aoq<3&&aoq>1)&&(aoq1<3&&aoq1>1)){
        beta = beam->BigRIPSBeam_beta[2];
        aoq = aoq1;
      }
      Double_t de_v    = TMath::Log(ionpair*beta*beta) - TMath::Log(1-beta*beta) - beta*beta;
      Double_t zlike    = TMath::Sqrt(sqsum/de_v)*beta;
      
      if(sqsum>5000)continue;
      if(nHit<=90){
        if(zlike>=minzlike[runID][0]&&zlike<=maxzlike[runID][0]){ // Z=50
          for(Int_t ipid=0; ipid<nSn; ipid++){
            if(aoq>=aoqmin[ipid]&&aoq<=aoqmax[ipid]){
              h_adc0[runIDeach[0]][ipid]->Fill(beam->BigRIPSIC_fADC[2][0]);
              h_adc1[runIDeach[1]][ipid]->Fill(beam->BigRIPSIC_fADC[2][1]);
              h_adc2[runIDeach[2]][ipid]->Fill(beam->BigRIPSIC_fADC[2][2]);
              h_adc3[runIDeach[3]][ipid]->Fill(beam->BigRIPSIC_fADC[2][3]);
              h_adc4[runIDeach[4]][ipid]->Fill(beam->BigRIPSIC_fADC[2][4]);
              h_adc5[runIDeach[5]][ipid]->Fill(beam->BigRIPSIC_fADC[2][5]);
            }
          }
        }
        if(zlike>=minzlike[runID][1]&&zlike<=maxzlike[runID][1]){ // Z=51
          for(Int_t ipid=nSn; ipid<nSn+nSb; ipid++){
            if(aoq>=aoqmin[ipid]&&aoq<=aoqmax[ipid]){
              h_adc0[runIDeach[0]][ipid]->Fill(beam->BigRIPSIC_fADC[2][0]);
              h_adc1[runIDeach[1]][ipid]->Fill(beam->BigRIPSIC_fADC[2][1]);
              h_adc2[runIDeach[2]][ipid]->Fill(beam->BigRIPSIC_fADC[2][2]);
              h_adc3[runIDeach[3]][ipid]->Fill(beam->BigRIPSIC_fADC[2][3]);
              h_adc4[runIDeach[4]][ipid]->Fill(beam->BigRIPSIC_fADC[2][4]);
              h_adc5[runIDeach[5]][ipid]->Fill(beam->BigRIPSIC_fADC[2][5]);
            }
          }
        }
        if(zlike>=minzlike[runID][2]&&zlike<=maxzlike[runID][2]){ // Z=49
          for(Int_t ipid=nSn+nSb; ipid<npid; ipid++){
            if(aoq>=aoqmin[ipid]&&aoq<=aoqmax[ipid]){
              h_adc0[runIDeach[0]][ipid]->Fill(beam->BigRIPSIC_fADC[2][0]);
              h_adc1[runIDeach[1]][ipid]->Fill(beam->BigRIPSIC_fADC[2][1]);
              h_adc2[runIDeach[2]][ipid]->Fill(beam->BigRIPSIC_fADC[2][2]);
              h_adc3[runIDeach[3]][ipid]->Fill(beam->BigRIPSIC_fADC[2][3]);
              h_adc4[runIDeach[4]][ipid]->Fill(beam->BigRIPSIC_fADC[2][4]);
              h_adc5[runIDeach[5]][ipid]->Fill(beam->BigRIPSIC_fADC[2][5]);
            }

          }
        }
        h_adc0_nosel[runIDeach[0]]->Fill(beam->BigRIPSIC_fADC[2][0]);
        h_adc1_nosel[runIDeach[1]]->Fill(beam->BigRIPSIC_fADC[2][1]);
        h_adc2_nosel[runIDeach[2]]->Fill(beam->BigRIPSIC_fADC[2][2]);
        h_adc3_nosel[runIDeach[3]]->Fill(beam->BigRIPSIC_fADC[2][3]);
        h_adc4_nosel[runIDeach[4]]->Fill(beam->BigRIPSIC_fADC[2][4]);
        h_adc5_nosel[runIDeach[5]]->Fill(beam->BigRIPSIC_fADC[2][5]);
      }
    }
  }

/*


//19 42 19 38 40 18
  TCanvas* cadc[6][npid];
  for(Int_t i=0; i<6; i++)
    for(Int_t j=0; j<npid; j++)
      cadc[i][j] = new TCanvas(Form("cadc%d_%d",i,j),"",1200,600);
    
  for(Int_t j=0; j<npid; j++){
    cadc[0][j]->Divide(5,3);
    cadc[1][j]->Divide(6,6);
    cadc[2][j]->Divide(5,3);
    cadc[3][j]->Divide(6,5);
    cadc[4][j]->Divide(7,5);
    cadc[5][j]->Divide(5,3);
  }

  auto f_adc = new TF1("f_adc","gaus");  
  for(Int_t i=0; i<6; i++){
    for(Int_t j=0; j<npid; j++){
      for(Int_t k=0; k<ndivision[i]; k++){
        cadc[i][j]->cd(k+1);
        if(i==0){
          h_adc0[k][j]->Draw();
          f_adc->SetParameters(h_adc0[k][j]->GetMaximum(),h_adc0[k][j]->GetBinCenter(h_adc0[k][j]->GetMaximumBin()),h_adc0[k][j]->GetRMS());
          h_adc0[k][j]->Fit(f_adc);
        }
        if(i==1){
          h_adc1[k][j]->Draw();
          f_adc->SetParameters(h_adc1[k][j]->GetMaximum(),h_adc1[k][j]->GetBinCenter(h_adc1[k][j]->GetMaximumBin()),h_adc1[k][j]->GetRMS());
          h_adc1[k][j]->Fit(f_adc);
        }
        if(i==2){
          h_adc2[k][j]->Draw();
          f_adc->SetParameters(h_adc2[k][j]->GetMaximum(),h_adc2[k][j]->GetBinCenter(h_adc2[k][j]->GetMaximumBin()),h_adc2[k][j]->GetRMS());
          h_adc2[k][j]->Fit(f_adc);
        }
        if(i==3){
          h_adc3[k][j]->Draw();
          f_adc->SetParameters(h_adc3[k][j]->GetMaximum(),h_adc3[k][j]->GetBinCenter(h_adc3[k][j]->GetMaximumBin()),h_adc3[k][j]->GetRMS());
          h_adc3[k][j]->Fit(f_adc);
        }
        if(i==4){
          h_adc4[k][j]->Draw();
          f_adc->SetParameters(h_adc4[k][j]->GetMaximum(),h_adc4[k][j]->GetBinCenter(h_adc4[k][j]->GetMaximumBin()),h_adc4[k][j]->GetRMS());
          h_adc4[k][j]->Fit(f_adc);
        }
        if(i==5){
          h_adc5[k][j]->Draw();
          f_adc->SetParameters(h_adc5[k][j]->GetMaximum(),h_adc5[k][j]->GetBinCenter(h_adc5[k][j]->GetMaximumBin()),h_adc5[k][j]->GetRMS());
          h_adc5[k][j]->Fit(f_adc);
        }
        f_adc->Clear();
      }
    }
  }
*/
/* 
  TCanvas *cadcnosel[6];
  for(Int_t i=0; i<6; i++)
    cadcnosel[i] = new TCanvas(Form("cadcnosel%d",i),"",1200,600);
  cadcnosel[0]->Divide(3,3);
  cadcnosel[1]->Divide(7,4);
  cadcnosel[2]->Divide(4,3);
  cadcnosel[3]->Divide(5,4);
  cadcnosel[4]->Divide(7,4);
  cadcnosel[5]->Divide(3,3);
  for(Int_t i=0; i<6; i++){
    for(Int_t k=0; k<ndivision[i]; k++){
      cadcnosel[i]->cd(k+1);
      if(i==0) h_adc0_nosel[k]->Draw();
      if(i==1) h_adc1_nosel[k]->Draw();
      if(i==2) h_adc2_nosel[k]->Draw();
      if(i==3) h_adc3_nosel[k]->Draw();
      if(i==4) h_adc4_nosel[k]->Draw();
      if(i==5) h_adc5_nosel[k]->Draw();
    }
  }

*/

  auto out = new TFile(Form(anapath+"macros_3/supplemental/icADCeach%d.root",beamtype),"recreate");
  for(Int_t k=0; k<ndivision[0]; k++){
    for(Int_t j=0; j<npid; j++)
      h_adc0[k][j]->Write();
    h_adc0_nosel[k]->Write();
  }
  for(Int_t k=0; k<ndivision[1]; k++){
    for(Int_t j=0; j<npid; j++)
      h_adc1[k][j]->Write();
    h_adc1_nosel[k]->Write();
  }
  for(Int_t k=0; k<ndivision[2]; k++){
    for(Int_t j=0; j<npid; j++)
      h_adc2[k][j]->Write();
    h_adc2_nosel[k]->Write();
  }
  for(Int_t k=0; k<ndivision[3]; k++){
    for(Int_t j=0; j<npid; j++)
      h_adc3[k][j]->Write();
    h_adc3_nosel[k]->Write();
  }
  for(Int_t k=0; k<ndivision[4]; k++){
    for(Int_t j=0; j<npid; j++)
      h_adc4[k][j]->Write();
    h_adc4_nosel[k]->Write();
  }
  for(Int_t k=0; k<ndivision[5]; k++){
    for(Int_t j=0; j<npid; j++)
      h_adc5[k][j]->Write();
    h_adc5_nosel[k]->Write();
  }
  
  auto adc0 = new TTree("adc0","");
  auto adc1 = new TTree("adc1","");
  auto adc2 = new TTree("adc2","");
  auto adc3 = new TTree("adc3","");
  auto adc4 = new TTree("adc4","");
  auto adc5 = new TTree("adc5","");
  std::vector<Int_t> runArray;
  adc0->Branch("adc0runarray",&runArray);
  adc1->Branch("adc1runarray",&runArray);
  adc2->Branch("adc2runarray",&runArray);
  adc3->Branch("adc3runarray",&runArray);
  adc4->Branch("adc4runarray",&runArray);
  adc5->Branch("adc5runarray",&runArray);
  for(Int_t i=0; i<ndivision[0]; i++){
    runArray.clear();
    runArray = runarray[0][i];  
    adc0->Fill();
  }
  for(Int_t i=0; i<ndivision[1]; i++){
    runArray.clear();
    runArray = runarray[1][i];  
    adc1->Fill();
  }
  for(Int_t i=0; i<ndivision[2]; i++){
    runArray.clear();
    runArray = runarray[2][i];  
    adc2->Fill();
  }
  for(Int_t i=0; i<ndivision[3]; i++){
    runArray.clear();
    runArray = runarray[3][i];  
    adc3->Fill();
  }
  for(Int_t i=0; i<ndivision[4]; i++){
    runArray.clear();
    runArray = runarray[4][i];  
    adc4->Fill();
  }
  for(Int_t i=0; i<ndivision[5]; i++){
    runArray.clear();
    runArray = runarray[5][i];  
    adc5->Fill();
  }
  adc0->Write(); 
  adc1->Write(); 
  adc2->Write(); 
  adc3->Write(); 
  adc4->Write(); 
  adc5->Write(); 
  
  out->Close();

}
