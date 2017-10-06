//standard
#include <fstream>
#include <string>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
//ROOT
#include "TTree.h"
#include "TFile.h"
#include "TString.h"
#include "TClonesArray.h"
#include "TSystem.h"
#include "TMath.h"
//Local class
#include "../class/TBeamEnergy.h"
#include "../class/TBDCProjection.h"
#include "../class/BeamReco.h"


int main(int argc, char *argv[]){
  char* number = argv[1];
  int runNo = atoi(number);
  TString anapath = "/home/s015/exp/exp1605_s015/anaroot/users/S22_analysis/"; 

  BeamReco* beam = new BeamReco();
  FileStat_t file;
  if(gSystem->GetPathInfo(Form(anapath+"rootfiles/reco0/run%04d.reco.root",runNo),file)) return 0;
  if(gSystem->GetPathInfo(Form(anapath+"rootfiles/ic/cal/run%04d.iccalibrated.root",runNo),file)) return 0;
  if(gSystem->GetPathInfo(Form(anapath+"rootfiles/ppac/run%04d.ppac.root",runNo),file)) return 0;
  
  beam->fChain->AddFile(Form(anapath+"rootfiles/reco0/run%04d.reco.root",runNo),0,"tree");
  beam->InitCombineTree();
  
  auto inic = new TFile(Form(anapath+"rootfiles/ic/cal/run%04d.iccalibrated.root",runNo),"read");
  auto t_ic = (TTree*)inic->Get("tree");
  Double_t asqsum; t_ic->SetBranchAddress("sqsum",&asqsum);
  Double_t acalADC[6]; t_ic->SetBranchAddress("calADC",acalADC);
  auto inppac = new TFile(Form(anapath+"rootfiles/ppac/run%04d.ppac.root",runNo),"read");  
  auto t_ppac = (TTree*)inppac->Get("tree");
  const Int_t nPPAC=12;
  const TString ppacname[nPPAC]={"F3_1A","F3_1B","F3_2A","F3_2B","F5_1A","F5_1B","F5_2A","F5_2B","F7_1A","F7_1B","F7_2A","F7_2B"}; 
  std::vector<Int_t> *TX1raw[nPPAC]={};
  std::vector<Int_t> *TX2raw[nPPAC]={};
  std::vector<Int_t> *TY1raw[nPPAC]={};
  std::vector<Int_t> *TY2raw[nPPAC]={};
  std::vector<Int_t> *TAraw[nPPAC]={};
  for(Int_t i=0; i<nPPAC; i++){
    t_ppac->SetBranchAddress(ppacname[i]+"_TX1",&TX1raw[i]);
    t_ppac->SetBranchAddress(ppacname[i]+"_TY1",&TY1raw[i]);
    t_ppac->SetBranchAddress(ppacname[i]+"_TX2",&TX2raw[i]);
    t_ppac->SetBranchAddress(ppacname[i]+"_TY2",&TY2raw[i]);
    t_ppac->SetBranchAddress(ppacname[i]+"_TA",&TAraw[i]);
  }
  
  auto out = new TFile(Form(anapath+"rootfiles/s22beam/run%04d.s22beam.root",runNo),"recreate");
  auto tree = new TTree("tree","TTree of S22 Beam information");
  Int_t run=runNo;
  Int_t event;
  Double_t z;
  Double_t aoq;
  Double_t tx;
  Double_t ty;
  Double_t ta;
  Double_t tb;
  tree->Branch("run",&run,"run/I");
  tree->Branch("event",&event,"event/I");
  tree->Branch("z",&z,"z/D");
  tree->Branch("aoq",&aoq,"aoq/D");
  tree->Branch("tx",&tx,"tx/D");
  tree->Branch("ty",&ty,"ty/D");
  tree->Branch("ta",&ta,"ta/D");
  tree->Branch("tb",&tb,"tb/D");
  
  // within 3 sigma -> true flag.
  Bool_t IsSngood=kFALSE;
  if(runNo>=2174&&runNo<=2509) tree->Branch("Is108Sn",&IsSngood,"Is108Sn/O");
  else if(runNo>=2520&&runNo<=2653) tree->Branch("Is112Sn",&IsSngood,"Is112Sn/O");

  Double_t beta37;
  Double_t tof37;
  tree->Branch("beta37",&beta37,"beta37/D");
  tree->Branch("tof37",&tof37,"tof37/D");
  Double_t beta;
  tree->Branch("beta",&beta,"beta/D");
  Double_t brho;
  tree->Branch("brho",&brho,"brho/D");

  Double_t F3X,F3Y,F3A,F3B;
  Double_t F5X,F5Y,F5A,F5B;
  Double_t F7X,F7Y,F7A,F7B;
  tree->Branch("F3X",&F3X,"F3X/D");
  tree->Branch("F3Y",&F3Y,"F3Y/D");
  tree->Branch("F3A",&F3A,"F3A/D");
  tree->Branch("F3B",&F3B,"F3B/D");
  tree->Branch("F5X",&F5X,"F5X/D");
  tree->Branch("F5Y",&F5Y,"F5Y/D");
  tree->Branch("F5A",&F5A,"F5A/D");
  tree->Branch("F5B",&F5B,"F5B/D");
  tree->Branch("F7X",&F7X,"F7X/D");
  tree->Branch("F7Y",&F7Y,"F7Y/D");
  tree->Branch("F7A",&F7A,"F7A/D");
  tree->Branch("F7B",&F7B,"F7B/D");
  
  Int_t ppacnHit;
  tree->Branch("ppacnHit",&ppacnHit,"ppacnHit/I");
  
  Double_t calADC[6];
  Double_t sqsum;
  tree->Branch("calADC",calADC,"calADC[6]/D");
  tree->Branch("sqsum",&sqsum,"sqsum/D");
  

  Int_t F3plaQDCL;     tree->Branch("F3plaQDCL",&F3plaQDCL,"F3plaQDCL/I"); 
  Int_t F3plaQDCR;     tree->Branch("F3plaQDCR",&F3plaQDCR,"F3plaQDCR/I"); 
  Int_t F3plaTDCL;     tree->Branch("F3plaTDCL",&F3plaTDCL,"F3plaTDCL/I"); 
  Int_t F3plaTDCR;     tree->Branch("F3plaTDCR",&F3plaTDCR,"F3plaTDCR/I"); 
  Int_t F7plaQDCL;     tree->Branch("F7plaQDCL",&F7plaQDCL,"F7plaQDCL/I"); 
  Int_t F7plaQDCR;     tree->Branch("F7plaQDCR",&F7plaQDCR,"F7plaQDCR/I"); 
  Int_t F7plaTDCL;     tree->Branch("F7plaTDCL",&F7plaTDCL,"F7plaTDCL/I"); 
  Int_t F7plaTDCR;     tree->Branch("F7plaTDCR",&F7plaTDCR,"F7plaTDCR/I"); 
  Int_t F13plaQDCL[2]; tree->Branch("F13plaQDCL",F13plaQDCL,"F13plaQDCL[2]/I"); 
  Int_t F13plaQDCR[2]; tree->Branch("F13plaQDCR",F13plaQDCR,"F13plaQDCR[2]/I");
  Int_t F13plaTDCL[2]; tree->Branch("F13plaTDCL",F13plaTDCL,"F13plaTDCL[2]/I");
  Int_t F13plaTDCR[2]; tree->Branch("F13plaTDCR",F13plaTDCR,"F13plaTDCR[2]/I");
  
  //TBDC branches
  Double_t bdc1x=-9999;    tree -> Branch("bdc1x",&bdc1x,"bdc1x/D");
  Double_t bdc1y=-9999;    tree -> Branch("bdc1y",&bdc1y,"bdc1y/D");
  Double_t bdc2x=-9999;    tree -> Branch("bdc2x",&bdc2x,"bdc2x/D");
  Double_t bdc2y=-9999;    tree -> Branch("bdc2y",&bdc2y,"bdc2y/D");
  Double_t ProjX=-9999;    tree -> Branch("ProjX",&ProjX,"ProjX/D");
  Double_t ProjY=-9999;    tree -> Branch("ProjY",&ProjY,"ProjY/D");
  Double_t ProjZ=-9999;    tree -> Branch("ProjZ",&ProjZ,"ProjZ/D");
  Double_t ProjA=-9999;    tree -> Branch("ProjA",&ProjA,"ProjA/D");
  Double_t ProjB=-9999;    tree -> Branch("ProjB",&ProjB,"ProjB/D");
  Double_t ProjE=-9999;    tree -> Branch("ProjE",&ProjE,"ProjE/D");
  Double_t ProjMeVu=-9999; tree -> Branch("ProjMeVu",&ProjMeVu,"ProjMeVu/D");
  Double_t ProjBeta=-9999; tree -> Branch("ProjBeta",&ProjBeta,"ProjBeta/D");
  Double_t ProjP=-9999;    tree -> Branch("ProjP",&ProjP,"ProjP/D");
  Double_t ProjPX=-9999;   tree -> Branch("ProjPX",&ProjPX,"ProjPX/D");
  Double_t ProjPY=-9999;   tree -> Branch("ProjPY",&ProjPY,"ProjPY/D");
  Double_t ProjPZ=-9999;   tree -> Branch("ProjPZ",&ProjPZ,"ProjPZ/D"); 
   
  auto bdcProj = new TBDCProjection();
  bdcProj->setBeam(runNo);
    
  Double_t zedcal[2]={};
  std::ifstream f_iccal;
  if(runNo>=2174&&runNo<=2509)
    f_iccal.open(anapath+"macros_3/supplemental/icCalibration_108.txt",std::ios::in);
  else if(runNo>=2520&&runNo<=2653)
    f_iccal.open(anapath+"macros_3/supplemental/icCalibration_112.txt",std::ios::in);
  
  f_iccal >> zedcal[0] >> zedcal[1];
  f_iccal.close();
  
  Double_t sigma_Z=0.2018;
  Double_t sigma_aoq=0.00163; 
  if(runNo>=2520&&runNo<=2653){sigma_Z=0.2026;sigma_aoq=0.00149;};
  
  Double_t aoqTin=2.16;
  if(runNo>=2520&&runNo<=2653) aoqTin=2.24;


  double end_projection_z=-592.644;//////mid target = -592.644, start pad plane =-580.4, end of pad plane = 763.6

  std::cout<<"runNo"<<runNo<<std::endl;
  for(Int_t i=0; i<beam->GetEntries(); i++){
  //std::cout<<i<<std::endl;
    beam->GetEntry(i);
    t_ic->GetEntry(i); 
    t_ppac->GetEntry(i); 
    
    event=i;
    
    tx = beam->tx;
    ty = beam->ty;
    ta = beam->ta;
    tb = beam->tb;
    beta37 = beam->BigRIPSBeam_beta[0];
    aoq    = beam->BigRIPSBeam_aoq[0];
    Double_t aoq1 = beam->BigRIPSBeam_aoq[2];
    if(!(aoq<3&&aoq>1)&&(aoq1<3&&aoq1>1)){
      beta37 = beam->BigRIPSBeam_beta[2];
      aoq = aoq1;
    }
    Double_t ionpair = beam->BigRIPSIC_ionpair[2];
    Double_t de_v    = TMath::Log(ionpair*beta37*beta37) - TMath::Log(1-beta37*beta37) - beta37*beta37;
    z = zedcal[0]+zedcal[1]*TMath::Sqrt(asqsum/de_v)*beta37;
    IsSngood = kFALSE;
    if( (z-50.)*(z-50.)/(3.*sigma_Z*3.*sigma_Z) + (aoq-aoqTin)*(aoq-aoqTin)/(3.*sigma_aoq*3.*sigma_aoq) <= 1. ) IsSngood=kTRUE;
 //   std::cout<< (z-50.)*(z-50.)<<" "<<(3.*sigma_Z*3.*sigma_Z) <<" "<<(aoq-aoqTin)*(aoq-aoqTin)<<" "<<(3.*sigma_aoq*3.*sigma_aoq)<<std::endl;
      
    tof37 = beam->BigRIPSTOF_tof[0];

    F3X = beam->BigRIPSFocalPlane_X[3]; F5X = beam->BigRIPSFocalPlane_X[5]; F7X = beam->BigRIPSFocalPlane_X[7];
    F3Y = beam->BigRIPSFocalPlane_Y[3]; F5Y = beam->BigRIPSFocalPlane_Y[5]; F7Y = beam->BigRIPSFocalPlane_Y[7];
    F3A = beam->BigRIPSFocalPlane_A[3]; F5A = beam->BigRIPSFocalPlane_A[5]; F7A = beam->BigRIPSFocalPlane_A[7];
    F3B = beam->BigRIPSFocalPlane_B[3]; F5B = beam->BigRIPSFocalPlane_B[5]; F7B = beam->BigRIPSFocalPlane_B[7];
    
    Int_t nHit=0; 
    for(Int_t ippac=0; ippac<12; ippac++)
      nHit += TX1raw[ippac]->size() + TX2raw[ippac]->size() + TY1raw[ippac]->size() + TY2raw[ippac]->size() + TAraw[ippac]->size();
    
    for(Int_t i=0; i<6; i++) calADC[i] = acalADC[i];
    sqsum = asqsum;
    
    F3plaQDCL = beam->BigRIPSPlastic_fQLRaw[0];
    F3plaQDCR = beam->BigRIPSPlastic_fQRRaw[0];
    F3plaTDCL = beam->BigRIPSPlastic_fTLRaw[0];
    F3plaTDCR = beam->BigRIPSPlastic_fTRRaw[0];
    F7plaQDCL = beam->BigRIPSPlastic_fQLRaw[2];
    F7plaQDCR = beam->BigRIPSPlastic_fQRRaw[2];
    F7plaTDCL = beam->BigRIPSPlastic_fTLRaw[2];
    F7plaTDCR = beam->BigRIPSPlastic_fTRRaw[2];
    for(Int_t i=0; i<2; i++){
      F13plaQDCL[i] = beam->BigRIPSPlastic_fQLRaw[i+3];
      F13plaQDCR[i] = beam->BigRIPSPlastic_fQRRaw[i+3];
      F13plaTDCL[i] = beam->BigRIPSPlastic_fTLRaw[i+3];
      F13plaTDCR[i] = beam->BigRIPSPlastic_fTRRaw[i+3];
    }
    
    
    bdc1x = beam->tx1;
    bdc1y = beam->ty1;
    bdc2x = beam->tx2;
    bdc2y = beam->ty2;

    auto beamE = new TBeamEnergy(z,aoq,beta37);
    beamE->setBeam(runNo);
    beta = beamE->getBeta();
    brho = beamE->getBrho();   
    Double_t bdcax=std::atan((bdc2x-bdc1x)/1000.)*1000.;
    Double_t bdcby=std::atan((bdc2y-bdc1y)/1000.)*1000.;
    
    ///BDC projection///
    ProjX=-9999;ProjY=-9999;ProjZ=-9999;ProjA=-9999;ProjB=-9999;
    ProjPX=-9999;ProjPY=-9999;ProjPZ=-9999;ProjP=-9999;ProjE=-9999;ProjBeta=-9999;
    double E1;
    E1=beamE->getCorrectedEnergy();
    if(z>0 && z<75 && aoq>1. && aoq<3 && bdc1x>-999 && bdc1y>-999 && bdc2x>-999 && bdc2y>-999){
      bdcProj->ProjectParticle(bdc2x, bdc2y, -2160., bdcax, bdcby, z, E1, end_projection_z,beamE->getMass());//-580.4,-583.904
      ProjX=bdcProj->getX();
      ProjY=bdcProj->getY();
      ProjZ=bdcProj->getZ();
      ProjA=bdcProj->getA();
      ProjB=bdcProj->getB();
      ProjPX=bdcProj->getPX();
      ProjPY=bdcProj->getPY();
      ProjPZ=bdcProj->getPZ();
      ProjP=bdcProj->getP();
      ProjE=bdcProj->getE();
      ProjMeVu=bdcProj->getMeVu();
      ProjBeta=bdcProj->getBeta();
    }

    tree->Fill();


  }

  out->Write();
  out->Close();

}
