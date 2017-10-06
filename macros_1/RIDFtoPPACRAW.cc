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

  TArtEventStore* estore = new TArtEventStore();
  estore->SetInterrupt(&stoploop);
  TString workdir = "/home/s015/exp/exp1605_s015/anaroot/users/S22_analysis/";
  TString infile =Form(workdir+"ridf/SMDAQ%d.ridf",runNo);
  estore->Open(infile.Data());
  TArtRawEventObject *rawevent = estore->GetRawEventObject();

  Int_t TOverFlow= 200000;
  TFile* file = new TFile(Form(workdir+"rootfiles/ppac/run%04d.ppac.root",runNo),"recreate");
  TTree* tree = new TTree("tree","");      
  Int_t event;
  tree->Branch("run",&runNo,"run/I");
  tree->Branch("event",&event,"event/I");

  TString ppacname[12]={"F3_1A","F3_1B","F3_2A","F3_2B","F5_1A","F5_1B","F5_2A","F5_2B","F7_1A","F7_1B","F7_2A","F7_2B"}; 


  // doublePPAC*2 1A,1B,2A,2B for each FPL: F3,F5,F7,  V1190 MTDC data
  std::vector<Int_t> TX1[12];
  std::vector<Int_t> TX2[12];
  std::vector<Int_t> TY1[12];
  std::vector<Int_t> TY2[12];
  std::vector<Int_t> TA[12];
  for(Int_t i=0; i<12; i++){
    tree->Branch( ppacname[i]+"_TX1", &TX1[i]);
    tree->Branch( ppacname[i]+"_TY1", &TY1[i]);
    tree->Branch( ppacname[i]+"_TX2", &TX2[i]);
    tree->Branch( ppacname[i]+"_TY2", &TY2[i]);
    tree->Branch( ppacname[i]+"_TA",  &TA[i]);
  }

  Int_t neve=0;
  while(estore->GetNextEvent()){
    if(neve%1000==0) std::cout<<"event:"<<neve<<std::endl;
    //  if(neve==10000) break;
    event=neve;
    // start to scan raw event tree
    for(int i=0;i<rawevent->GetNumSeg();i++){
      TArtRawSegmentObject *seg = rawevent->GetSegment(i);
      int det = seg->GetDetector();
    //  int device = seg->GetDevice();
      if(det == 1){//PPAC
        std::vector<Int_t> valbuf[70];
        for(int b=0; b<70; b++) valbuf[b].clear();
        for(int j=0;j<seg->GetNumData();j++){
          TArtRawDataObject *d = seg->GetData(j);
          int ch = d->GetCh();
          if(ch<4||ch>64) continue; //used PPAC channel: 4~64
          int val = d->GetVal();
          if(val<0||val>TOverFlow) continue; 
          valbuf[ch].push_back( val );
        }

        // PPAC F3 1A,1B,2A,2B, ...
        // TX1: ch=16,20,24,28, ...
        // TX2: ch=18,22,26,30, ...
        // TY1: ch=17,21,25,29, ...
        // TY2: ch=19,23,27,31, ...
        // TA:  ch=4,5,6,7, ...

        for(Int_t j=0; j<12; j++){
          TX1[j].clear();
          TX2[j].clear();
          TY1[j].clear();
          TY2[j].clear();
          TA[j].clear();
          for(Int_t k=0; k<valbuf[4*j+16].size(); k++) TX1[j].push_back(valbuf[4*j+16].at(k));
          for(Int_t k=0; k<valbuf[4*j+17].size(); k++) TY1[j].push_back(valbuf[4*j+17].at(k));
          for(Int_t k=0; k<valbuf[4*j+18].size(); k++) TX2[j].push_back(valbuf[4*j+18].at(k));
          for(Int_t k=0; k<valbuf[4*j+19].size(); k++) TY2[j].push_back(valbuf[4*j+19].at(k));
          for(Int_t k=0; k<valbuf[j+4].size(); k++) TA[j].push_back(valbuf[j+4].at(k));
        }

        //F31A_TX1.clear();
        //for(Int_t j=0; j<valbuf[16].size(); j++) F31A_TX1.push_back(valbuf[16].at(j));
      }

      //if(device != USERGR) continue;

    }

    tree->Fill();
    estore->ClearData();
    neve ++;

  }
  tree->Write();
  file->Close();

}
