#ifndef BeamReco_h
#define BeamReco_h

#include <vector>
#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>

// Header file for the classes stored in the TTree if any.
#include "TClonesArray.h"
#include "TObject.h"
#include "TVectorT.h"

class BeamReco {
public :
   TChain          *fChain;   //!pointer to the analyzed TTree or TChain
   Int_t           fCurrent; //!current Tree number in a TChain

// Fixed size dimensions of array or collections stored in the TTree if any.
   static const Int_t kMaxBigRIPSPPAC = 36;
   static const Int_t kMaxBigRIPSPlastic = 6;
   static const Int_t kMaxBigRIPSIC = 9;
   static const Int_t kMaxBigRIPSFocalPlane = 14;
   static const Int_t kMaxBigRIPSRIPS = 2;
   static const Int_t kMaxBigRIPSTOF = 3;
   static const Int_t kMaxBigRIPSBeam = 3;
   static const Int_t kMaxSAMURAIBDC1Track = 20;
   static const Int_t kMaxSAMURAIBDC2Track = 20;
   
   Int_t run;
   Int_t event; 
   // Declaration of leaf types
   Int_t           BigRIPSPPAC_;
   UInt_t          BigRIPSPPAC_fUniqueID[kMaxBigRIPSPPAC];   //[BigRIPSPPAC_]
   UInt_t          BigRIPSPPAC_fBits[kMaxBigRIPSPPAC];   //[BigRIPSPPAC_]
   Int_t           BigRIPSPPAC_id[kMaxBigRIPSPPAC];   //[BigRIPSPPAC_]
   Int_t           BigRIPSPPAC_fpl[kMaxBigRIPSPPAC];   //[BigRIPSPPAC_]
   TString         BigRIPSPPAC_name[kMaxBigRIPSPPAC];
   Int_t           BigRIPSPPAC_fDataState[kMaxBigRIPSPPAC];   //[BigRIPSPPAC_]
   Double_t        BigRIPSPPAC_xzpos[kMaxBigRIPSPPAC];   //[BigRIPSPPAC_]
   Double_t        BigRIPSPPAC_yzpos[kMaxBigRIPSPPAC];   //[BigRIPSPPAC_]
   Int_t           BigRIPSPPAC_fTX1Raw[kMaxBigRIPSPPAC];   //[BigRIPSPPAC_]
   Int_t           BigRIPSPPAC_fTX2Raw[kMaxBigRIPSPPAC];   //[BigRIPSPPAC_]
   Int_t           BigRIPSPPAC_fTY1Raw[kMaxBigRIPSPPAC];   //[BigRIPSPPAC_]
   Int_t           BigRIPSPPAC_fTY2Raw[kMaxBigRIPSPPAC];   //[BigRIPSPPAC_]
   Int_t           BigRIPSPPAC_fTARaw[kMaxBigRIPSPPAC];   //[BigRIPSPPAC_]
   Int_t           BigRIPSPPAC_fQX1Raw[kMaxBigRIPSPPAC];   //[BigRIPSPPAC_]
   Int_t           BigRIPSPPAC_fQX2Raw[kMaxBigRIPSPPAC];   //[BigRIPSPPAC_]
   Int_t           BigRIPSPPAC_fQY1Raw[kMaxBigRIPSPPAC];   //[BigRIPSPPAC_]
   Int_t           BigRIPSPPAC_fQY2Raw[kMaxBigRIPSPPAC];   //[BigRIPSPPAC_]
   Int_t           BigRIPSPPAC_fQARaw[kMaxBigRIPSPPAC];   //[BigRIPSPPAC_]
   Double_t        BigRIPSPPAC_fTX1[kMaxBigRIPSPPAC];   //[BigRIPSPPAC_]
   Double_t        BigRIPSPPAC_fTX2[kMaxBigRIPSPPAC];   //[BigRIPSPPAC_]
   Double_t        BigRIPSPPAC_fTY1[kMaxBigRIPSPPAC];   //[BigRIPSPPAC_]
   Double_t        BigRIPSPPAC_fTY2[kMaxBigRIPSPPAC];   //[BigRIPSPPAC_]
   Double_t        BigRIPSPPAC_fTA[kMaxBigRIPSPPAC];   //[BigRIPSPPAC_]
   Double_t        BigRIPSPPAC_fTSumX[kMaxBigRIPSPPAC];   //[BigRIPSPPAC_]
   Double_t        BigRIPSPPAC_fTSumY[kMaxBigRIPSPPAC];   //[BigRIPSPPAC_]
   Double_t        BigRIPSPPAC_fTDiffX[kMaxBigRIPSPPAC];   //[BigRIPSPPAC_]
   Double_t        BigRIPSPPAC_fTDiffY[kMaxBigRIPSPPAC];   //[BigRIPSPPAC_]
   Double_t        BigRIPSPPAC_fX[kMaxBigRIPSPPAC];   //[BigRIPSPPAC_]
   Double_t        BigRIPSPPAC_fY[kMaxBigRIPSPPAC];   //[BigRIPSPPAC_]
   Bool_t          BigRIPSPPAC_fFiredX[kMaxBigRIPSPPAC];   //[BigRIPSPPAC_]
   Bool_t          BigRIPSPPAC_fFiredY[kMaxBigRIPSPPAC];   //[BigRIPSPPAC_]
   Int_t           BigRIPSPlastic_;
   UInt_t          BigRIPSPlastic_fUniqueID[kMaxBigRIPSPlastic];   //[BigRIPSPlastic_]
   UInt_t          BigRIPSPlastic_fBits[kMaxBigRIPSPlastic];   //[BigRIPSPlastic_]
   Int_t           BigRIPSPlastic_id[kMaxBigRIPSPlastic];   //[BigRIPSPlastic_]
   Int_t           BigRIPSPlastic_fpl[kMaxBigRIPSPlastic];   //[BigRIPSPlastic_]
   TString         BigRIPSPlastic_name[kMaxBigRIPSPlastic];
   Int_t           BigRIPSPlastic_fDataState[kMaxBigRIPSPlastic];   //[BigRIPSPlastic_]
   Double_t        BigRIPSPlastic_zposition[kMaxBigRIPSPlastic];   //[BigRIPSPlastic_]
   Double_t        BigRIPSPlastic_zoffset[kMaxBigRIPSPlastic];   //[BigRIPSPlastic_]
   Int_t           BigRIPSPlastic_fTLRaw[kMaxBigRIPSPlastic];   //[BigRIPSPlastic_]
   Int_t           BigRIPSPlastic_fTRRaw[kMaxBigRIPSPlastic];   //[BigRIPSPlastic_]
   Int_t           BigRIPSPlastic_fQLRaw[kMaxBigRIPSPlastic];   //[BigRIPSPlastic_]
   Int_t           BigRIPSPlastic_fQRRaw[kMaxBigRIPSPlastic];   //[BigRIPSPlastic_]
   Double_t        BigRIPSPlastic_fTime[kMaxBigRIPSPlastic];   //[BigRIPSPlastic_]
   Double_t        BigRIPSPlastic_fTimeL[kMaxBigRIPSPlastic];   //[BigRIPSPlastic_]
   Double_t        BigRIPSPlastic_fTimeR[kMaxBigRIPSPlastic];   //[BigRIPSPlastic_]
   Double_t        BigRIPSPlastic_fTimeLSlew[kMaxBigRIPSPlastic];   //[BigRIPSPlastic_]
   Double_t        BigRIPSPlastic_fTimeRSlew[kMaxBigRIPSPlastic];   //[BigRIPSPlastic_]
   Double_t        BigRIPSPlastic_fTimeSlew[kMaxBigRIPSPlastic];   //[BigRIPSPlastic_]
   Int_t           BigRIPSIC_;
   UInt_t          BigRIPSIC_fUniqueID[kMaxBigRIPSIC];   //[BigRIPSIC_]
   UInt_t          BigRIPSIC_fBits[kMaxBigRIPSIC];   //[BigRIPSIC_]
   Int_t           BigRIPSIC_id[kMaxBigRIPSIC];   //[BigRIPSIC_]
   Int_t           BigRIPSIC_fpl[kMaxBigRIPSIC];   //[BigRIPSIC_]
   TString         BigRIPSIC_name[kMaxBigRIPSIC];
   Int_t           BigRIPSIC_fDataState[kMaxBigRIPSIC];   //[BigRIPSIC_]
   Double_t        BigRIPSIC_zcoef[kMaxBigRIPSIC][2];   //[BigRIPSIC_]
   Double_t        BigRIPSIC_ionpair[kMaxBigRIPSIC];   //[BigRIPSIC_]
   Int_t           BigRIPSIC_nhitchannel[kMaxBigRIPSIC];   //[BigRIPSIC_]
   Int_t           BigRIPSIC_fADC[kMaxBigRIPSIC][12];   //[BigRIPSIC_]
   Double_t        BigRIPSIC_fEnergy[kMaxBigRIPSIC][12];   //[BigRIPSIC_]
   Double_t        BigRIPSIC_fRawADCSqSum[kMaxBigRIPSIC];   //[BigRIPSIC_]
   Double_t        BigRIPSIC_fRawADCAvSum[kMaxBigRIPSIC];   //[BigRIPSIC_]
   Double_t        BigRIPSIC_fCalMeVSqSum[kMaxBigRIPSIC];   //[BigRIPSIC_]
   Double_t        BigRIPSIC_fCalMeVAvSum[kMaxBigRIPSIC];   //[BigRIPSIC_]
   Int_t           BigRIPSFocalPlane_;
   UInt_t          BigRIPSFocalPlane_fUniqueID[kMaxBigRIPSFocalPlane];   //[BigRIPSFocalPlane_]
   UInt_t          BigRIPSFocalPlane_fBits[kMaxBigRIPSFocalPlane];   //[BigRIPSFocalPlane_]
   Int_t           BigRIPSFocalPlane_id[kMaxBigRIPSFocalPlane];   //[BigRIPSFocalPlane_]
   Int_t           BigRIPSFocalPlane_fpl[kMaxBigRIPSFocalPlane];   //[BigRIPSFocalPlane_]
   TString         BigRIPSFocalPlane_name[kMaxBigRIPSFocalPlane];
   Int_t           BigRIPSFocalPlane_fDataState[kMaxBigRIPSFocalPlane];   //[BigRIPSFocalPlane_]
   TVectorT<double> BigRIPSFocalPlane_opt_vector[kMaxBigRIPSFocalPlane];
   Double_t        BigRIPSFocalPlane_X[kMaxBigRIPSFocalPlane];   //[BigRIPSFocalPlane_]
   Double_t        BigRIPSFocalPlane_A[kMaxBigRIPSFocalPlane];   //[BigRIPSFocalPlane_]
   Double_t        BigRIPSFocalPlane_Y[kMaxBigRIPSFocalPlane];   //[BigRIPSFocalPlane_]
   Double_t        BigRIPSFocalPlane_B[kMaxBigRIPSFocalPlane];   //[BigRIPSFocalPlane_]
   Int_t           BigRIPSFocalPlane_nfired_ppacx[kMaxBigRIPSFocalPlane];   //[BigRIPSFocalPlane_]
   Int_t           BigRIPSFocalPlane_nfired_ppacy[kMaxBigRIPSFocalPlane];   //[BigRIPSFocalPlane_]
   Double_t        BigRIPSFocalPlane_zpos[kMaxBigRIPSFocalPlane];   //[BigRIPSFocalPlane_]
   Double_t        BigRIPSFocalPlane_zpos_offset[kMaxBigRIPSFocalPlane];   //[BigRIPSFocalPlane_]
   Int_t           BigRIPSRIPS_;
   UInt_t          BigRIPSRIPS_fUniqueID[kMaxBigRIPSRIPS];   //[BigRIPSRIPS_]
   UInt_t          BigRIPSRIPS_fBits[kMaxBigRIPSRIPS];   //[BigRIPSRIPS_]
   Int_t           BigRIPSRIPS_id[kMaxBigRIPSRIPS];   //[BigRIPSRIPS_]
   Int_t           BigRIPSRIPS_fpl[kMaxBigRIPSRIPS];   //[BigRIPSRIPS_]
   TString         BigRIPSRIPS_name[kMaxBigRIPSRIPS];
   Int_t           BigRIPSRIPS_fDataState[kMaxBigRIPSRIPS];   //[BigRIPSRIPS_]
   Int_t           BigRIPSRIPS_upstream_fpl[kMaxBigRIPSRIPS];   //[BigRIPSRIPS_]
   Int_t           BigRIPSRIPS_downstream_fpl[kMaxBigRIPSRIPS];   //[BigRIPSRIPS_]
   Double_t        BigRIPSRIPS_center_brho[kMaxBigRIPSRIPS];   //[BigRIPSRIPS_]
   Double_t        BigRIPSRIPS_brho[kMaxBigRIPSRIPS];   //[BigRIPSRIPS_]
   Double_t        BigRIPSRIPS_length[kMaxBigRIPSRIPS];   //[BigRIPSRIPS_]
   TMatrixT<double> BigRIPSRIPS_matrix[kMaxBigRIPSRIPS];
   Double_t        BigRIPSRIPS_delta[kMaxBigRIPSRIPS];   //[BigRIPSRIPS_]
   Double_t        BigRIPSRIPS_angle[kMaxBigRIPSRIPS];   //[BigRIPSRIPS_]
   TString         BigRIPSRIPS_dipolename[kMaxBigRIPSRIPS];
   Int_t           BigRIPSTOF_;
   UInt_t          BigRIPSTOF_fUniqueID[kMaxBigRIPSTOF];   //[BigRIPSTOF_]
   UInt_t          BigRIPSTOF_fBits[kMaxBigRIPSTOF];   //[BigRIPSTOF_]
   Int_t           BigRIPSTOF_id[kMaxBigRIPSTOF];   //[BigRIPSTOF_]
   Int_t           BigRIPSTOF_fpl[kMaxBigRIPSTOF];   //[BigRIPSTOF_]
   TString         BigRIPSTOF_name[kMaxBigRIPSTOF];
   Int_t           BigRIPSTOF_fDataState[kMaxBigRIPSTOF];   //[BigRIPSTOF_]
   Double_t        BigRIPSTOF_tof[kMaxBigRIPSTOF];   //[BigRIPSTOF_]
   Double_t        BigRIPSTOF_clight[kMaxBigRIPSTOF];   //[BigRIPSTOF_]
   Double_t        BigRIPSTOF_length[kMaxBigRIPSTOF];   //[BigRIPSTOF_]
   Double_t        BigRIPSTOF_ulength[kMaxBigRIPSTOF];   //[BigRIPSTOF_]
   Double_t        BigRIPSTOF_dlength[kMaxBigRIPSTOF];   //[BigRIPSTOF_]
   TString         BigRIPSTOF_upstream_plname[kMaxBigRIPSTOF];
   TString         BigRIPSTOF_downstream_plname[kMaxBigRIPSTOF];
   Int_t           BigRIPSTOF_upstream_plfpl[kMaxBigRIPSTOF];   //[BigRIPSTOF_]
   Int_t           BigRIPSTOF_downstream_plfpl[kMaxBigRIPSTOF];   //[BigRIPSTOF_]
   Double_t        BigRIPSTOF_time_offset[kMaxBigRIPSTOF];   //[BigRIPSTOF_]
   Int_t           BigRIPSBeam_;
   UInt_t          BigRIPSBeam_fUniqueID[kMaxBigRIPSBeam];   //[BigRIPSBeam_]
   UInt_t          BigRIPSBeam_fBits[kMaxBigRIPSBeam];   //[BigRIPSBeam_]
   Int_t           BigRIPSBeam_id[kMaxBigRIPSBeam];   //[BigRIPSBeam_]
   Int_t           BigRIPSBeam_fpl[kMaxBigRIPSBeam];   //[BigRIPSBeam_]
   TString         BigRIPSBeam_name[kMaxBigRIPSBeam];
   Int_t           BigRIPSBeam_fDataState[kMaxBigRIPSBeam];   //[BigRIPSBeam_]
   Double_t        BigRIPSBeam_brho[kMaxBigRIPSBeam];   //[BigRIPSBeam_]
   Double_t        BigRIPSBeam_aoq[kMaxBigRIPSBeam];   //[BigRIPSBeam_]
   Double_t        BigRIPSBeam_zet[kMaxBigRIPSBeam];   //[BigRIPSBeam_]
   Double_t        BigRIPSBeam_zdeg[kMaxBigRIPSBeam];   //[BigRIPSBeam_]
   Double_t        BigRIPSBeam_beta[kMaxBigRIPSBeam];   //[BigRIPSBeam_]
   Double_t        BigRIPSBeam_clight[kMaxBigRIPSBeam];   //[BigRIPSBeam_]
   Double_t        BigRIPSBeam_mnucleon[kMaxBigRIPSBeam];   //[BigRIPSBeam_]
   Int_t           BigRIPSBeam_nrips[kMaxBigRIPSBeam];   //[BigRIPSBeam_]
   TString         BigRIPSBeam_ripsname[2][kMaxBigRIPSBeam];
   TString         BigRIPSBeam_tofname[kMaxBigRIPSBeam];
   TString         BigRIPSBeam_icname[kMaxBigRIPSBeam];
   Int_t           SAMURAIBDC1Track_;
   UInt_t          SAMURAIBDC1Track_fUniqueID[kMaxSAMURAIBDC1Track];   //[SAMURAIBDC1Track_]
   UInt_t          SAMURAIBDC1Track_fBits[kMaxSAMURAIBDC1Track];   //[SAMURAIBDC1Track_]
   Int_t           SAMURAIBDC1Track_id[kMaxSAMURAIBDC1Track];   //[SAMURAIBDC1Track_]
   Int_t           SAMURAIBDC1Track_fpl[kMaxSAMURAIBDC1Track];   //[SAMURAIBDC1Track_]
   TString         SAMURAIBDC1Track_name[kMaxSAMURAIBDC1Track];
   Int_t           SAMURAIBDC1Track_fDataState[kMaxSAMURAIBDC1Track];   //[SAMURAIBDC1Track_]
   Int_t           SAMURAIBDC1Track_nhitlayer[kMaxSAMURAIBDC1Track];   //[SAMURAIBDC1Track_]
   Double_t        SAMURAIBDC1Track_chi2[kMaxSAMURAIBDC1Track];   //[SAMURAIBDC1Track_]
   Int_t           SAMURAIBDC1Track_ndf[kMaxSAMURAIBDC1Track];   //[SAMURAIBDC1Track_]
   Double_t        SAMURAIBDC1Track_cx[kMaxSAMURAIBDC1Track][2];   //[SAMURAIBDC1Track_]
   Double_t        SAMURAIBDC1Track_ca[kMaxSAMURAIBDC1Track][2];   //[SAMURAIBDC1Track_]
   std::vector<int>     SAMURAIBDC1Track_hitid[kMaxSAMURAIBDC1Track];
   std::vector<double>  SAMURAIBDC1Track_dl[kMaxSAMURAIBDC1Track];
   std::vector<double>  SAMURAIBDC1Track_lx[kMaxSAMURAIBDC1Track];
   std::vector<double>  SAMURAIBDC1Track_lz[kMaxSAMURAIBDC1Track];
   std::vector<int>     SAMURAIBDC1Track_plane_id[kMaxSAMURAIBDC1Track];
   std::vector<double>  SAMURAIBDC1Track_dcangle[kMaxSAMURAIBDC1Track];
   Int_t           SAMURAIBDC2Track_;
   UInt_t          SAMURAIBDC2Track_fUniqueID[kMaxSAMURAIBDC2Track];   //[SAMURAIBDC2Track_]
   UInt_t          SAMURAIBDC2Track_fBits[kMaxSAMURAIBDC2Track];   //[SAMURAIBDC2Track_]
   Int_t           SAMURAIBDC2Track_id[kMaxSAMURAIBDC2Track];   //[SAMURAIBDC2Track_]
   Int_t           SAMURAIBDC2Track_fpl[kMaxSAMURAIBDC2Track];   //[SAMURAIBDC2Track_]
   TString         SAMURAIBDC2Track_name[kMaxSAMURAIBDC2Track];
   Int_t           SAMURAIBDC2Track_fDataState[kMaxSAMURAIBDC2Track];   //[SAMURAIBDC2Track_]
   Int_t           SAMURAIBDC2Track_nhitlayer[kMaxSAMURAIBDC2Track];   //[SAMURAIBDC2Track_]
   Double_t        SAMURAIBDC2Track_chi2[kMaxSAMURAIBDC2Track];   //[SAMURAIBDC2Track_]
   Int_t           SAMURAIBDC2Track_ndf[kMaxSAMURAIBDC2Track];   //[SAMURAIBDC2Track_]
   Double_t        SAMURAIBDC2Track_cx[kMaxSAMURAIBDC2Track][2];   //[SAMURAIBDC2Track_]
   Double_t        SAMURAIBDC2Track_ca[kMaxSAMURAIBDC2Track][2];   //[SAMURAIBDC2Track_]
   std::vector<int>     SAMURAIBDC2Track_hitid[kMaxSAMURAIBDC2Track];
   std::vector<double>  SAMURAIBDC2Track_dl[kMaxSAMURAIBDC2Track];
   std::vector<double>  SAMURAIBDC2Track_lx[kMaxSAMURAIBDC2Track];
   std::vector<double>  SAMURAIBDC2Track_lz[kMaxSAMURAIBDC2Track];
   std::vector<int>     SAMURAIBDC2Track_plane_id[kMaxSAMURAIBDC2Track];
   std::vector<double>  SAMURAIBDC2Track_dcangle[kMaxSAMURAIBDC2Track];
   Double_t        tx1;
   Double_t        ty1;
   Double_t        ta1;
   Double_t        tb1;
   Double_t        tx2;
   Double_t        ty2;
   Double_t        ta2;
   Double_t        tb2;
   Double_t        tx;
   Double_t        ty;
   Double_t        ta;
   Double_t        tb;


   // List of branches
   TBranch        *b_run;   //!
   TBranch        *b_event;   //!
   TBranch        *b_BigRIPSPPAC_;   //!
   TBranch        *b_BigRIPSPPAC_fUniqueID;   //!
   TBranch        *b_BigRIPSPPAC_fBits;   //!
   TBranch        *b_BigRIPSPPAC_id;   //!
   TBranch        *b_BigRIPSPPAC_fpl;   //!
   TBranch        *b_BigRIPSPPAC_name;   //!
   TBranch        *b_BigRIPSPPAC_fDataState;   //!
   TBranch        *b_BigRIPSPPAC_xzpos;   //!
   TBranch        *b_BigRIPSPPAC_yzpos;   //!
   TBranch        *b_BigRIPSPPAC_fTX1Raw;   //!
   TBranch        *b_BigRIPSPPAC_fTX2Raw;   //!
   TBranch        *b_BigRIPSPPAC_fTY1Raw;   //!
   TBranch        *b_BigRIPSPPAC_fTY2Raw;   //!
   TBranch        *b_BigRIPSPPAC_fTARaw;   //!
   TBranch        *b_BigRIPSPPAC_fQX1Raw;   //!
   TBranch        *b_BigRIPSPPAC_fQX2Raw;   //!
   TBranch        *b_BigRIPSPPAC_fQY1Raw;   //!
   TBranch        *b_BigRIPSPPAC_fQY2Raw;   //!
   TBranch        *b_BigRIPSPPAC_fQARaw;   //!
   TBranch        *b_BigRIPSPPAC_fTX1;   //!
   TBranch        *b_BigRIPSPPAC_fTX2;   //!
   TBranch        *b_BigRIPSPPAC_fTY1;   //!
   TBranch        *b_BigRIPSPPAC_fTY2;   //!
   TBranch        *b_BigRIPSPPAC_fTA;   //!
   TBranch        *b_BigRIPSPPAC_fTSumX;   //!
   TBranch        *b_BigRIPSPPAC_fTSumY;   //!
   TBranch        *b_BigRIPSPPAC_fTDiffX;   //!
   TBranch        *b_BigRIPSPPAC_fTDiffY;   //!
   TBranch        *b_BigRIPSPPAC_fX;   //!
   TBranch        *b_BigRIPSPPAC_fY;   //!
   TBranch        *b_BigRIPSPPAC_fFiredX;   //!
   TBranch        *b_BigRIPSPPAC_fFiredY;   //!
   TBranch        *b_BigRIPSPlastic_;   //!
   TBranch        *b_BigRIPSPlastic_fUniqueID;   //!
   TBranch        *b_BigRIPSPlastic_fBits;   //!
   TBranch        *b_BigRIPSPlastic_id;   //!
   TBranch        *b_BigRIPSPlastic_fpl;   //!
   TBranch        *b_BigRIPSPlastic_name;   //!
   TBranch        *b_BigRIPSPlastic_fDataState;   //!
   TBranch        *b_BigRIPSPlastic_zposition;   //!
   TBranch        *b_BigRIPSPlastic_zoffset;   //!
   TBranch        *b_BigRIPSPlastic_fTLRaw;   //!
   TBranch        *b_BigRIPSPlastic_fTRRaw;   //!
   TBranch        *b_BigRIPSPlastic_fQLRaw;   //!
   TBranch        *b_BigRIPSPlastic_fQRRaw;   //!
   TBranch        *b_BigRIPSPlastic_fTime;   //!
   TBranch        *b_BigRIPSPlastic_fTimeL;   //!
   TBranch        *b_BigRIPSPlastic_fTimeR;   //!
   TBranch        *b_BigRIPSPlastic_fTimeLSlew;   //!
   TBranch        *b_BigRIPSPlastic_fTimeRSlew;   //!
   TBranch        *b_BigRIPSPlastic_fTimeSlew;   //!
   TBranch        *b_BigRIPSIC_;   //!
   TBranch        *b_BigRIPSIC_fUniqueID;   //!
   TBranch        *b_BigRIPSIC_fBits;   //!
   TBranch        *b_BigRIPSIC_id;   //!
   TBranch        *b_BigRIPSIC_fpl;   //!
   TBranch        *b_BigRIPSIC_name;   //!
   TBranch        *b_BigRIPSIC_fDataState;   //!
   TBranch        *b_BigRIPSIC_zcoef;   //!
   TBranch        *b_BigRIPSIC_ionpair;   //!
   TBranch        *b_BigRIPSIC_nhitchannel;   //!
   TBranch        *b_BigRIPSIC_fADC;   //!
   TBranch        *b_BigRIPSIC_fEnergy;   //!
   TBranch        *b_BigRIPSIC_fRawADCSqSum;   //!
   TBranch        *b_BigRIPSIC_fRawADCAvSum;   //!
   TBranch        *b_BigRIPSIC_fCalMeVSqSum;   //!
   TBranch        *b_BigRIPSIC_fCalMeVAvSum;   //!
   TBranch        *b_BigRIPSFocalPlane_;   //!
   TBranch        *b_BigRIPSFocalPlane_fUniqueID;   //!
   TBranch        *b_BigRIPSFocalPlane_fBits;   //!
   TBranch        *b_BigRIPSFocalPlane_id;   //!
   TBranch        *b_BigRIPSFocalPlane_fpl;   //!
   TBranch        *b_BigRIPSFocalPlane_name;   //!
   TBranch        *b_BigRIPSFocalPlane_fDataState;   //!
   TBranch        *b_BigRIPSFocalPlane_opt_vector;   //!
   TBranch        *b_BigRIPSFocalPlane_X;   //!
   TBranch        *b_BigRIPSFocalPlane_A;   //!
   TBranch        *b_BigRIPSFocalPlane_Y;   //!
   TBranch        *b_BigRIPSFocalPlane_B;   //!
   TBranch        *b_BigRIPSFocalPlane_nfired_ppacx;   //!
   TBranch        *b_BigRIPSFocalPlane_nfired_ppacy;   //!
   TBranch        *b_BigRIPSFocalPlane_zpos;   //!
   TBranch        *b_BigRIPSFocalPlane_zpos_offset;   //!
   TBranch        *b_BigRIPSRIPS_;   //!
   TBranch        *b_BigRIPSRIPS_fUniqueID;   //!
   TBranch        *b_BigRIPSRIPS_fBits;   //!
   TBranch        *b_BigRIPSRIPS_id;   //!
   TBranch        *b_BigRIPSRIPS_fpl;   //!
   TBranch        *b_BigRIPSRIPS_name;   //!
   TBranch        *b_BigRIPSRIPS_fDataState;   //!
   TBranch        *b_BigRIPSRIPS_upstream_fpl;   //!
   TBranch        *b_BigRIPSRIPS_downstream_fpl;   //!
   TBranch        *b_BigRIPSRIPS_center_brho;   //!
   TBranch        *b_BigRIPSRIPS_brho;   //!
   TBranch        *b_BigRIPSRIPS_length;   //!
   TBranch        *b_BigRIPSRIPS_matrix;   //!
   TBranch        *b_BigRIPSRIPS_delta;   //!
   TBranch        *b_BigRIPSRIPS_angle;   //!
   TBranch        *b_BigRIPSRIPS_dipolename;   //!
   TBranch        *b_BigRIPSTOF_;   //!
   TBranch        *b_BigRIPSTOF_fUniqueID;   //!
   TBranch        *b_BigRIPSTOF_fBits;   //!
   TBranch        *b_BigRIPSTOF_id;   //!
   TBranch        *b_BigRIPSTOF_fpl;   //!
   TBranch        *b_BigRIPSTOF_name;   //!
   TBranch        *b_BigRIPSTOF_fDataState;   //!
   TBranch        *b_BigRIPSTOF_tof;   //!
   TBranch        *b_BigRIPSTOF_clight;   //!
   TBranch        *b_BigRIPSTOF_length;   //!
   TBranch        *b_BigRIPSTOF_ulength;   //!
   TBranch        *b_BigRIPSTOF_dlength;   //!
   TBranch        *b_BigRIPSTOF_upstream_plname;   //!
   TBranch        *b_BigRIPSTOF_downstream_plname;   //!
   TBranch        *b_BigRIPSTOF_upstream_plfpl;   //!
   TBranch        *b_BigRIPSTOF_downstream_plfpl;   //!
   TBranch        *b_BigRIPSTOF_time_offset;   //!
   TBranch        *b_BigRIPSBeam_;   //!
   TBranch        *b_BigRIPSBeam_fUniqueID;   //!
   TBranch        *b_BigRIPSBeam_fBits;   //!
   TBranch        *b_BigRIPSBeam_id;   //!
   TBranch        *b_BigRIPSBeam_fpl;   //!
   TBranch        *b_BigRIPSBeam_name;   //!
   TBranch        *b_BigRIPSBeam_fDataState;   //!
   TBranch        *b_BigRIPSBeam_brho;   //!
   TBranch        *b_BigRIPSBeam_aoq;   //!
   TBranch        *b_BigRIPSBeam_zet;   //!
   TBranch        *b_BigRIPSBeam_zdeg;   //!
   TBranch        *b_BigRIPSBeam_beta;   //!
   TBranch        *b_BigRIPSBeam_clight;   //!
   TBranch        *b_BigRIPSBeam_mnucleon;   //!
   TBranch        *b_BigRIPSBeam_nrips;   //!
   TBranch        *b_BigRIPSBeam_ripsname;   //!
   TBranch        *b_BigRIPSBeam_tofname;   //!
   TBranch        *b_BigRIPSBeam_icname;   //!
   TBranch        *b_SAMURAIBDC1Track_;   //!
   TBranch        *b_SAMURAIBDC1Track_fUniqueID;   //!
   TBranch        *b_SAMURAIBDC1Track_fBits;   //!
   TBranch        *b_SAMURAIBDC1Track_id;   //!
   TBranch        *b_SAMURAIBDC1Track_fpl;   //!
   TBranch        *b_SAMURAIBDC1Track_name;   //!
   TBranch        *b_SAMURAIBDC1Track_fDataState;   //!
   TBranch        *b_SAMURAIBDC1Track_nhitlayer;   //!
   TBranch        *b_SAMURAIBDC1Track_chi2;   //!
   TBranch        *b_SAMURAIBDC1Track_ndf;   //!
   TBranch        *b_SAMURAIBDC1Track_cx;   //!
   TBranch        *b_SAMURAIBDC1Track_ca;   //!
   TBranch        *b_SAMURAIBDC1Track_hitid;   //!
   TBranch        *b_SAMURAIBDC1Track_dl;   //!
   TBranch        *b_SAMURAIBDC1Track_lx;   //!
   TBranch        *b_SAMURAIBDC1Track_lz;   //!
   TBranch        *b_SAMURAIBDC1Track_plane_id;   //!
   TBranch        *b_SAMURAIBDC1Track_dcangle;   //!
   TBranch        *b_SAMURAIBDC2Track_;   //!
   TBranch        *b_SAMURAIBDC2Track_fUniqueID;   //!
   TBranch        *b_SAMURAIBDC2Track_fBits;   //!
   TBranch        *b_SAMURAIBDC2Track_id;   //!
   TBranch        *b_SAMURAIBDC2Track_fpl;   //!
   TBranch        *b_SAMURAIBDC2Track_name;   //!
   TBranch        *b_SAMURAIBDC2Track_fDataState;   //!
   TBranch        *b_SAMURAIBDC2Track_nhitlayer;   //!
   TBranch        *b_SAMURAIBDC2Track_chi2;   //!
   TBranch        *b_SAMURAIBDC2Track_ndf;   //!
   TBranch        *b_SAMURAIBDC2Track_cx;   //!
   TBranch        *b_SAMURAIBDC2Track_ca;   //!
   TBranch        *b_SAMURAIBDC2Track_hitid;   //!
   TBranch        *b_SAMURAIBDC2Track_dl;   //!
   TBranch        *b_SAMURAIBDC2Track_lx;   //!
   TBranch        *b_SAMURAIBDC2Track_lz;   //!
   TBranch        *b_SAMURAIBDC2Track_plane_id;   //!
   TBranch        *b_SAMURAIBDC2Track_dcangle;   //!
   TBranch        *b_tx1;   //!
   TBranch        *b_ty1;   //!
   TBranch        *b_ta1;   //!
   TBranch        *b_tb1;   //!
   TBranch        *b_tx2;   //!
   TBranch        *b_ty2;   //!
   TBranch        *b_ta2;   //!
   TBranch        *b_tb2;   //!
   TBranch        *b_tx;   //!
   TBranch        *b_ty;   //!
   TBranch        *b_ta;   //!
   TBranch        *b_tb;   //!

   BeamReco();
   virtual ~BeamReco();
   virtual Int_t    GetEntry(Long64_t entry);
   virtual Long64_t    GetEntries();
   virtual void     Init();
   virtual void     Clear();
   virtual void     InitPPAC();
   virtual void     InitPPACTSum();
   virtual void     InitPlastic();
   virtual void     InitPlaADC();
   virtual void     InitIC();
   virtual void     InitTOF();
   virtual void     InitPPACeff();
   virtual void     InitRecoeff();
   virtual void     InitBetaRes();
   virtual void     InitRawPID();
   virtual void     InitCombineTree();

   virtual Bool_t   Notify();
};

#endif
