// BigRIPS Plastic: fpl-id = 13, detector-id = 3(PLAT), module-id = 34(MTDC32)
// BigRIPS Plastic: fpl-id = 13, detector-id = 2(PLAQ), module-id = 34(MQDC32)
// BigRIPS Plastic: fpl-id = 63, detector-id = 2(PLAQ), module-id = 21(V792)
// BigRIPS Plastic: fpl-id = 63, detector-id = 3(PLAT), module-id = 25(V1290)

//standart
#include <fstream>
#include <string>
#include <vector>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
//ANAROOT
#include "TArtStoreManager.hh"
#include "TArtEventStore.hh"
#include "TArtRunInfo.hh"
#include "TArtEventInfo.hh"
#include "TArtRawSegmentObject.hh"
#include "TArtRawDataObject.hh"
//ROOT
#include "TTree.h"
#include "TFile.h"
#include "TString.h"
#include "TClonesArray.h"

// function to exit loop at keyboard interrupt.
bool stoploop = false;

void stop_interrupt(){
  printf("keyboard interrupt\n");
  stoploop = true;
}

int main(int argc, char *argv[]){

  char* number = argv[1];
  int runNo = atoi(number);


  TString workdir = "/home/s015/exp/exp1605_s015/anaroot/users/S22_analysis/";
  TString infile =Form(workdir+"ridf/SMDAQ%i.ridf",runNo);

  TArtEventStore *estore = new TArtEventStore();
  estore->SetInterrupt(&stoploop);
  estore->Open(infile);
  TArtRawEventObject *rawevent = estore->GetRawEventObject();
  
  Int_t QDCL[4];
  Int_t QDCR[4];
  std::vector<Int_t> TDCL[4];
  std::vector<Int_t> TDCR[4];
  std::vector<Int_t> TDCL_V1290[2];
  std::vector<Int_t> TDCR_V1290[2];

  TFile* f = new TFile(Form(workdir+"rootfiles/plastic/run%04d.pla.root",runNo),"recreate");
  TTree* tree = new TTree("tree","");
  Int_t run=runNo;
  Int_t event;
  tree->Branch("run",&run,"run/I");
  tree->Branch("event",&event,"event/I");
  tree->Branch("QDCL",QDCL,"QDCL[4]/I");
  tree->Branch("QDCR",QDCR,"QDCR[4]/I");
  tree->Branch("F3TDCL",&TDCL[0]);
  tree->Branch("F3TDCR",&TDCR[0]);
  tree->Branch("F7TDCL",&TDCL[1]);
  tree->Branch("F7TDCR",&TDCR[1]);
  tree->Branch("F13_1TDCL",&TDCL[2]);
  tree->Branch("F13_1TDCR",&TDCR[2]);
  tree->Branch("F13_2TDCL",&TDCL[3]);
  tree->Branch("F13_2TDCR",&TDCR[3]);
  tree->Branch("F13_1TDCL_V1290",&TDCL_V1290[0]);
  tree->Branch("F13_1TDCR_V1290",&TDCR_V1290[0]);
  tree->Branch("F13_2TDCL_V1290",&TDCL_V1290[1]);
  tree->Branch("F13_2TDCR_V1290",&TDCR_V1290[1]);

  Int_t neve=0;
  while(estore->GetNextEvent()){
    if(neve%1000==0) std::cout<<"event:"<<neve<<std::endl;
    //if(neve==10000) break;


    for(Int_t i=0; i<4; i++){
      QDCL[i]=-1;
      QDCR[i]=-1;
      TDCL[i].clear();
      TDCR[i].clear();
    }
    for(Int_t i=0; i<2; i++){
      TDCL_V1290[i].clear();
      TDCR_V1290[i].clear();
    }


    event=rawevent->GetEventNumber();
    // start to scan raw event tree
    for(int i=0;i<rawevent->GetNumSeg();i++){
      TArtRawSegmentObject *seg = rawevent->GetSegment(i);
      int fpl = seg->GetFP();
      int det = seg->GetDetector();
      if( !((fpl==63||fpl==13)&&(det==2||det==3)) ) continue; //BigRIPS plastic
      for(int j=0;j<seg->GetNumData();j++){
        TArtRawDataObject *d = seg->GetData(j);
        int ch = d->GetCh();
        int val = d->GetVal();

        if(det==2&&fpl==13){ //QDC

          if(ch==0) QDCL[0]=val;
          else if(ch==1) QDCR[0]=val;
          else if(ch==4) QDCL[1]=val;
          else if(ch==5) QDCR[1]=val;
          else if(ch==6) QDCL[2]=val;
          else if(ch==7) QDCR[2]=val;
          else if(ch==8) QDCL[3]=val;
          else if(ch==9) QDCR[3]=val;

        }else if(det==3){ //TDC
          if(fpl==13){ 
            if(ch==0) TDCL[0].push_back(val);
            else if(ch==1) TDCR[0].push_back(val);
            else if(ch==4) TDCL[1].push_back(val);
            else if(ch==5) TDCR[1].push_back(val);
            else if(ch==6) TDCL[2].push_back(val);
            else if(ch==7) TDCR[2].push_back(val);
            else if(ch==8) TDCL[3].push_back(val);
            else if(ch==9) TDCR[3].push_back(val);
          } 
          else if(fpl==63){ 
            if(ch==16) TDCL_V1290[0].push_back(val);
            else if(ch==17) TDCR_V1290[0].push_back(val);
            else if(ch==20) TDCL_V1290[1].push_back(val);
            else if(ch==21) TDCR_V1290[1].push_back(val);
          } 

        }
      }
    }
    tree->Fill();
    estore->ClearData();
    neve ++;

  }
  tree->Write();
  f->Close();
}
