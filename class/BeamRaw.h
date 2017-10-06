#ifndef BeamRaw_h
#define BeamRaw_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>

// Header file for the classes stored in the TTree if any.
#include "TClonesArray.h"
#include "TObject.h"
#include "TVectorT.h"

class BeamRaw {
public :
   TChain          *fChain;   //!pointer to the analyzed TTree or TChain
   Int_t           fCurrent; //!current Tree number in a TChain

// Fixed size dimensions of array or collections stored in the TTree if any.
   static const Int_t kMaxBigRIPSPPAC = 36;
   static const Int_t kMaxBigRIPSPlastic = 6;
   static const Int_t kMaxBigRIPSIC = 9;
    
   static const Int_t kMaxSAMURAIBDC1Hit = 40;
   static const Int_t kMaxSAMURAIBDC2Hit = 40;

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
   Int_t           SAMURAIBDC1Hit_;
   UInt_t          SAMURAIBDC1Hit_fUniqueID[kMaxSAMURAIBDC1Hit];   //[SAMURAIBDC1Hit_]
   UInt_t          SAMURAIBDC1Hit_fBits[kMaxSAMURAIBDC1Hit];   //[SAMURAIBDC1Hit_]
   Int_t           SAMURAIBDC1Hit_id[kMaxSAMURAIBDC1Hit];   //[SAMURAIBDC1Hit_]
   Int_t           SAMURAIBDC1Hit_fpl[kMaxSAMURAIBDC1Hit];   //[SAMURAIBDC1Hit_]
   TString         SAMURAIBDC1Hit_name[kMaxSAMURAIBDC1Hit];
   Int_t           SAMURAIBDC1Hit_fDataState[kMaxSAMURAIBDC1Hit];   //[SAMURAIBDC1Hit_]
   Int_t           SAMURAIBDC1Hit_layer[kMaxSAMURAIBDC1Hit];   //[SAMURAIBDC1Hit_]
   Int_t           SAMURAIBDC1Hit_id_plane[kMaxSAMURAIBDC1Hit];   //[SAMURAIBDC1Hit_]
   TString         SAMURAIBDC1Hit_anodedir[kMaxSAMURAIBDC1Hit];
   Double_t        SAMURAIBDC1Hit_wirepos[kMaxSAMURAIBDC1Hit];   //[SAMURAIBDC1Hit_]
   Double_t        SAMURAIBDC1Hit_wirez[kMaxSAMURAIBDC1Hit];   //[SAMURAIBDC1Hit_]
   Int_t           SAMURAIBDC1Hit_wireid[kMaxSAMURAIBDC1Hit];   //[SAMURAIBDC1Hit_]
   Int_t           SAMURAIBDC1Hit_id_hit[kMaxSAMURAIBDC1Hit];   //[SAMURAIBDC1Hit_]
   Int_t           SAMURAIBDC1Hit_fTDC[kMaxSAMURAIBDC1Hit];   //[SAMURAIBDC1Hit_]
   Int_t           SAMURAIBDC1Hit_fTrailTDC[kMaxSAMURAIBDC1Hit];   //[SAMURAIBDC1Hit_]
   Double_t        SAMURAIBDC1Hit_fPosition[kMaxSAMURAIBDC1Hit];   //[SAMURAIBDC1Hit_]
   Int_t           SAMURAIBDC2Hit_;
   UInt_t          SAMURAIBDC2Hit_fUniqueID[kMaxSAMURAIBDC2Hit];   //[SAMURAIBDC2Hit_]
   UInt_t          SAMURAIBDC2Hit_fBits[kMaxSAMURAIBDC2Hit];   //[SAMURAIBDC2Hit_]
   Int_t           SAMURAIBDC2Hit_id[kMaxSAMURAIBDC2Hit];   //[SAMURAIBDC2Hit_]
   Int_t           SAMURAIBDC2Hit_fpl[kMaxSAMURAIBDC2Hit];   //[SAMURAIBDC2Hit_]
   TString         SAMURAIBDC2Hit_name[kMaxSAMURAIBDC2Hit];
   Int_t           SAMURAIBDC2Hit_fDataState[kMaxSAMURAIBDC2Hit];   //[SAMURAIBDC2Hit_]
   Int_t           SAMURAIBDC2Hit_layer[kMaxSAMURAIBDC2Hit];   //[SAMURAIBDC2Hit_]
   Int_t           SAMURAIBDC2Hit_id_plane[kMaxSAMURAIBDC2Hit];   //[SAMURAIBDC2Hit_]
   TString         SAMURAIBDC2Hit_anodedir[kMaxSAMURAIBDC2Hit];
   Double_t        SAMURAIBDC2Hit_wirepos[kMaxSAMURAIBDC2Hit];   //[SAMURAIBDC2Hit_]
   Double_t        SAMURAIBDC2Hit_wirez[kMaxSAMURAIBDC2Hit];   //[SAMURAIBDC2Hit_]
   Int_t           SAMURAIBDC2Hit_wireid[kMaxSAMURAIBDC2Hit];   //[SAMURAIBDC2Hit_]
   Int_t           SAMURAIBDC2Hit_id_hit[kMaxSAMURAIBDC2Hit];   //[SAMURAIBDC2Hit_]
   Int_t           SAMURAIBDC2Hit_fTDC[kMaxSAMURAIBDC2Hit];   //[SAMURAIBDC2Hit_]
   Int_t           SAMURAIBDC2Hit_fTrailTDC[kMaxSAMURAIBDC2Hit];   //[SAMURAIBDC2Hit_]
   Double_t        SAMURAIBDC2Hit_fPosition[kMaxSAMURAIBDC2Hit];   //[SAMURAIBDC2Hit_]


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
   TBranch        *b_SAMURAIBDC1Hit_;   //!
   TBranch        *b_SAMURAIBDC1Hit_fUniqueID;   //!
   TBranch        *b_SAMURAIBDC1Hit_fBits;   //!
   TBranch        *b_SAMURAIBDC1Hit_id;   //!
   TBranch        *b_SAMURAIBDC1Hit_fpl;   //!
   TBranch        *b_SAMURAIBDC1Hit_name;   //!
   TBranch        *b_SAMURAIBDC1Hit_fDataState;   //!
   TBranch        *b_SAMURAIBDC1Hit_layer;   //!
   TBranch        *b_SAMURAIBDC1Hit_id_plane;   //!
   TBranch        *b_SAMURAIBDC1Hit_anodedir;   //!
   TBranch        *b_SAMURAIBDC1Hit_wirepos;   //!
   TBranch        *b_SAMURAIBDC1Hit_wirez;   //!
   TBranch        *b_SAMURAIBDC1Hit_wireid;   //!
   TBranch        *b_SAMURAIBDC1Hit_id_hit;   //!
   TBranch        *b_SAMURAIBDC1Hit_fTDC;   //!
   TBranch        *b_SAMURAIBDC1Hit_fTrailTDC;   //!
   TBranch        *b_SAMURAIBDC1Hit_fPosition;   //!
   TBranch        *b_SAMURAIBDC2Hit_;   //!
   TBranch        *b_SAMURAIBDC2Hit_fUniqueID;   //!
   TBranch        *b_SAMURAIBDC2Hit_fBits;   //!
   TBranch        *b_SAMURAIBDC2Hit_id;   //!
   TBranch        *b_SAMURAIBDC2Hit_fpl;   //!
   TBranch        *b_SAMURAIBDC2Hit_name;   //!
   TBranch        *b_SAMURAIBDC2Hit_fDataState;   //!
   TBranch        *b_SAMURAIBDC2Hit_layer;   //!
   TBranch        *b_SAMURAIBDC2Hit_id_plane;   //!
   TBranch        *b_SAMURAIBDC2Hit_anodedir;   //!
   TBranch        *b_SAMURAIBDC2Hit_wirepos;   //!
   TBranch        *b_SAMURAIBDC2Hit_wirez;   //!
   TBranch        *b_SAMURAIBDC2Hit_wireid;   //!
   TBranch        *b_SAMURAIBDC2Hit_id_hit;   //!
   TBranch        *b_SAMURAIBDC2Hit_fTDC;   //!
   TBranch        *b_SAMURAIBDC2Hit_fTrailTDC;   //!
   TBranch        *b_SAMURAIBDC2Hit_fPosition;   //!

   BeamRaw();
   virtual ~BeamRaw();
   virtual Int_t    GetEntry(Long64_t entry);
   virtual Long64_t    GetEntries();
   virtual void     Init();
   virtual void     Clear();
   virtual void     InitPPAC();
   virtual void     InitPPACTSum();
   virtual void     InitPlastic();
   virtual void     InitPlaADC();
   virtual void     InitIC();
   virtual Bool_t   Notify();
};

#endif
