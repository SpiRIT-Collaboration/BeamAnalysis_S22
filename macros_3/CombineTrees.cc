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


Int_t *GetIsotope(Double_t myZ,Double_t myAoq){
  Double_t myMass=myAoq*myZ;
  Int_t static Arr[2]={0,0};//array of Z,mass
  int iZ1=(int)(myZ-1.);
  int iZ2=(int)(myZ+1.);
  int iM1=(int)(myMass-1.);
  int iM2=(int)(myMass+1.);
  Double_t ellipse=100;
  for(int iz=iZ1;iz<iZ2;iz++){
    for(int im=iM1;im<iM2;im++){
      ellipse = (myZ-(double)iz)*(myZ-(double)iz)/0.5/0.5+(myMass-(double)im)*(myMass-(double)im)/0.5/0.5;//the ellipse can be numerically changed here
      if(ellipse<1.){
        Arr[0]=iz;
        Arr[1]=im;
        break;
      }
    }//mass loop
    if(ellipse<1.){
      break;
    }
  }//charge loop
  return Arr;
}

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
 /*
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
 */ 
  auto out = new TFile(Form(anapath+"rootfiles/s22beam/beam_run%04d.ridf.root",runNo),"recreate");

  auto TBeam = new TTree("TBeam","Beam information");
  auto TBDC  = new TTree("TBDC","BDC information");
  auto TFocalPlane = new TTree("TFocalPlane","Focal Plane information");

  //TBeam branches
  Int_t neve=0; TBeam-> Branch("neve", &neve, "neve/I");
  Double_t z=-9999; TBeam -> Branch("z", &z, "z/D");
  Double_t aoq=-9999; TBeam -> Branch("aoq", &aoq, "aoq/D");
  Double_t beta37=-9999; TBeam -> Branch("beta37", &beta37, "beta37/D");
  Double_t brho78=-9999; TBeam -> Branch("brho78", &brho78, "brho78/D");
  Double_t beta=-9999; TBeam -> Branch("beta", &beta, "beta/D");
  Double_t brho=-9999; TBeam -> Branch("brho", &brho, "brho/D");
  Bool_t isGood=false; TBeam -> Branch("isGood", &isGood, "isGood/B");
  Int_t intZ=-9999; TBeam-> Branch("intZ", &intZ, "intZ/I");
  Int_t intA=-9999; TBeam-> Branch("intA", &intA, "intA/I");

  //TBDC branches
  Double_t bdc1x=-9999; TBDC -> Branch("bdc1x",&bdc1x,"bdc1x/D");
  Double_t bdc1y=-9999; TBDC -> Branch("bdc1y",&bdc1y,"bdc1y/D");
  Double_t bdc2x=-9999; TBDC -> Branch("bdc2x",&bdc2x,"bdc2x/D");
  Double_t bdc2y=-9999; TBDC -> Branch("bdc2y",&bdc2y,"bdc2y/D");
  Double_t bdcax=-9999; TBDC -> Branch("bdcax",&bdcax,"bdcax/D");
  Double_t bdcby=-9999; TBDC -> Branch("bdcby",&bdcby,"bdcby/D");
  Double_t ProjX=-9999; TBDC -> Branch("ProjX",&ProjX,"ProjX/D");
  Double_t ProjY=-9999; TBDC -> Branch("ProjY",&ProjY,"ProjY/D");
  Double_t ProjZ=-9999; TBDC -> Branch("ProjZ",&ProjZ,"ProjZ/D");
  Double_t ProjA=-9999; TBDC -> Branch("ProjA",&ProjA,"ProjA/D");
  Double_t ProjB=-9999; TBDC -> Branch("ProjB",&ProjB,"ProjB/D");
  Double_t ProjE=-9999; TBDC -> Branch("ProjE",&ProjE,"ProjE/D");
  Double_t ProjMeVu=-9999; TBDC -> Branch("ProjMeVu",&ProjMeVu,"ProjMeVu/D");
  Double_t ProjBeta=-9999; TBDC -> Branch("ProjBeta",&ProjBeta,"ProjBeta/D");
  Double_t ProjP=-9999; TBDC -> Branch("ProjP",&ProjP,"ProjP/D");
  Double_t ProjPX=-9999; TBDC -> Branch("ProjPX",&ProjPX,"ProjPX/D");
  Double_t ProjPY=-9999; TBDC -> Branch("ProjPY",&ProjPY,"ProjPY/D");
  Double_t ProjPZ=-9999; TBDC -> Branch("ProjPZ",&ProjPZ,"ProjPZ/D");

  //Focal Plane Branch
  Double_t F3X; TFocalPlane -> Branch("F3X", &F3X, "F3X/D");
  Double_t F3A; TFocalPlane -> Branch("F3A", &F3A, "F3A/D");
  Double_t F3Y; TFocalPlane -> Branch("F3Y", &F3Y, "F3Y/D");
  Double_t F3B; TFocalPlane -> Branch("F3B", &F3B, "F3B/D");
  Double_t F5X; TFocalPlane -> Branch("F5X", &F5X, "F5X/D");
  Double_t F5A; TFocalPlane -> Branch("F5A", &F5A, "F5A/D");
  Double_t F5Y; TFocalPlane -> Branch("F5Y", &F5Y, "F5Y/D");
  Double_t F5B; TFocalPlane -> Branch("F5B", &F5B, "F5B/D");
  Double_t F7X; TFocalPlane -> Branch("F7X", &F7X, "F7X/D");
  Double_t F7A; TFocalPlane -> Branch("F7A", &F7A, "F7A/D");
  Double_t F7Y; TFocalPlane -> Branch("F7Y", &F7Y, "F7Y/D");
  Double_t F7B; TFocalPlane -> Branch("F7B", &F7B, "F7B/D");

   
  auto bdcProj = new TBDCProjection();
  bdcProj->setBeam(runNo);
    
  Double_t zedcal[2]={}, betacal[2]={};
  std::ifstream f_iccal;
  std::ifstream f_betacal;
  if(runNo>=2174&&runNo<=2509){
    f_iccal.open(anapath+"macros_3/supplemental/icCalibration_108.txt",std::ios::in);
    f_betacal.open(anapath+"macros_3/supplemental/betaCalibration_108.txt",std::ios::in);
  }
  else if(runNo>=2520&&runNo<=2653){
    f_iccal.open(anapath+"macros_3/supplemental/icCalibration_112.txt",std::ios::in);
    f_betacal.open(anapath+"macros_3/supplemental/betaCalibration_112.txt",std::ios::in);
  }
  
  f_iccal >> zedcal[0] >> zedcal[1];
  f_betacal >> betacal[0] >> betacal[1];
  f_iccal.close();
  f_betacal.close();
  
  double end_projection_z=-592.644;//////mid target = -592.644, start pad plane =-580.4, end of pad plane = 763.6

  std::cout<<"runNo"<<runNo<<std::endl;
  for(Int_t i=0; i<beam->GetEntries(); i++){
  //std::cout<<i<<std::endl;
    beam->GetEntry(i);
    t_ic->GetEntry(i); 
  //  t_ppac->GetEntry(i); 
    
    neve=i;
    //fill some TBeam items
    intZ=-9999;
    intA=-9999;
    
    beta37 = beam->BigRIPSBeam_beta[0];
    aoq    = beam->BigRIPSBeam_aoq[0];
      
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
      {beta37=rbeta2_F57; aoq=aoq_F57;} 
    
    Double_t ionpair = beam->BigRIPSIC_ionpair[2];
    Double_t de_v    = TMath::Log(ionpair*beta37*beta37) - TMath::Log(1-beta37*beta37) - beta37*beta37;
    z = zedcal[0]+zedcal[1]*TMath::Sqrt(asqsum/de_v)*beta37;
   
    brho78 = beam->BigRIPSRIPS_brho[1];

    F3X = beam->BigRIPSFocalPlane_X[3]; F5X = beam->BigRIPSFocalPlane_X[5]; F7X = beam->BigRIPSFocalPlane_X[7];
    F3Y = beam->BigRIPSFocalPlane_Y[3]; F5Y = beam->BigRIPSFocalPlane_Y[5]; F7Y = beam->BigRIPSFocalPlane_Y[7];
    F3A = beam->BigRIPSFocalPlane_A[3]; F5A = beam->BigRIPSFocalPlane_A[5]; F7A = beam->BigRIPSFocalPlane_A[7];
    F3B = beam->BigRIPSFocalPlane_B[3]; F5B = beam->BigRIPSFocalPlane_B[5]; F7B = beam->BigRIPSFocalPlane_B[7];
    
    bdc1x = beam->tx1;
    bdc1y = beam->ty1;
    bdc2x = beam->tx2;
    bdc2y = beam->ty2;

    auto beamE = new TBeamEnergy(z,aoq,beta37);
    beamE->setBeam(runNo);
    beta = beamE->getBeta();
    brho = beamE->getBrho();   
    bdcax=std::atan((bdc2x-bdc1x)/1000.)*1000.;
    bdcby=std::atan((bdc2y-bdc1y)/1000.)*1000.;
    
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

    TBeam->Fill();
    TBDC->Fill();
    TFocalPlane->Fill();


  }

  TBeam->Write();
  TBDC->Write();
  TFocalPlane->Write();
  out->Write();
  out->Close();

}
