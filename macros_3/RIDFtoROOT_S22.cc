//standart
#include <fstream>
#include <string>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
//ANAROOT
#include "TArtStoreManager.hh"
#include "TArtEventStore.hh"
#include "TArtRunInfo.hh"
#include "TArtEventInfo.hh"
#include "TArtBigRIPSParameters.hh"
#include "TArtPPAC.hh"
#include "TArtIC.hh"
#include "TArtPlastic.hh"
#include "TArtFocalPlane.hh"
#include "TArtCalibPPAC.hh"
#include "TArtCalibPlastic.hh"
#include "TArtCalibIC.hh"
#include "TArtCalibFocalPlane.hh"
#include "TArtCalibPID.hh"
#include "TArtPPACPara.hh"
#include "TArtPlasticPara.hh"
#include "TArtICPara.hh"
#include "TArtRecoPID.hh"
#include "TArtRecoBeam.hh"
#include "TArtRecoRIPS.hh"
#include "TArtRecoTOF.hh"
#include "TArtBeam.hh"
#include "TArtRIPS.hh"
#include "TArtTOF.hh"
#include "TArtSAMURAIParameters.hh"
#include "TArtCalibBDC1Hit.hh"
#include "TArtCalibBDC2Hit.hh"
#include "TArtDCHit.hh"
#include "TArtDCHitPara.hh"
#include "TArtCalibBDC1Track.hh"
#include "TArtCalibBDC2Track.hh"
#include "TArtDCTrack.hh"
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
    TString anadir = "/home/s015/exp/exp1605_s015/anaroot/users/S22_analysis/";
    //TString outdir=anadir+"rootfile/data/";
    // Create StoreManager both for calibration "TArtCalib..." and treatment "TArtReco..."
    //------------------------------------------------------------------------------------
    TArtStoreManager * storeMan = TArtStoreManager::Instance();

    // Create EventStore to control the loop and get the EventInfo
    //------------------------------------------------------------
    TArtEventStore * eventStore = new TArtEventStore();
    eventStore->SetInterrupt(&stoploop);
    TString ridfdir="/home/s015/ridf/sdaq02/";
    TString ridfFile = Form(ridfdir+"SMDAQ%04d.ridf", runNo);
    eventStore -> Open(ridfFile.Data());
    //  eventStore -> Open();// for online analysis

    // Create BigRIPSParameters to get Plastics, PPACs, ICs and FocalPlanes parameters from ".xml" files
    //--------------------------------------------------------------------------------------------------
    TArtBigRIPSParameters *bigripsParameters = TArtBigRIPSParameters::Instance();
    TString dbdir=anadir+"db/";
    Char_t* ppacdb = NULL;
    Char_t* plasticdb = NULL;
    Char_t* icdb = (Char_t*) Form(dbdir+"reco0/BigRIPSIC.0.xml");
    Char_t* focalplanedb = (Char_t*) Form(dbdir+"FocalPlane.xml");
    
   
    // ppac calibration parameter files
    if(runNo>=2174&&runNo<=2194) ppacdb = (Char_t*) Form(dbdir+"reco0/BigRIPSPPAC.2174-2194.xml");
    else if((runNo>=2195&&runNo<=2201)||(runNo>=2204&&runNo<=2391)) ppacdb = (Char_t*) Form(dbdir+"reco0/BigRIPSPPAC.2195-2201_2204-2391.xml");
    else if(runNo>=2202&&runNo<=2203) ppacdb = (Char_t*) Form(dbdir+"reco0/BigRIPSPPAC.2202-2203.xml");
    else if(runNo>=2392&&runNo<=2394) ppacdb = (Char_t*) Form(dbdir+"reco0/BigRIPSPPAC.2392-2394.xml");
    else if((runNo>=2395&&runNo<=2470)||(runNo>=2476&&runNo<=2509)) ppacdb = (Char_t*) Form(dbdir+"reco0/BigRIPSPPAC.2395-2470_2476-2509.xml");
    else if(runNo>=2471&&runNo<=2475) ppacdb = (Char_t*) Form(dbdir+"reco0/BigRIPSPPAC.2471-2475.xml");
    else if(runNo>=2520&&runNo<=2529) ppacdb = (Char_t*) Form(dbdir+"reco0/BigRIPSPPAC.2520-2529.xml");
    else if(runNo>=2530&&runNo<=2653) ppacdb = (Char_t*) Form(dbdir+"reco0/BigRIPSPPAC.2530-2653.xml");
    else ppacdb = (Char_t*) Form(dbdir+"BigRIPSPPAC/BigRIPSPPAC.0.xml");
    
    Double_t TOFoffset=0.;
    // plasitc cal. parameter files.
    // 108Sn runs
    if(runNo>=2174&&runNo<=2194){
      plasticdb=(Char_t *) Form(dbdir+"reco0/BigRIPSPlastic.2174-2194.xml");
      TOFoffset=306.8176;
    }else if((runNo>=2195&&runNo<=2201)||(runNo>=2204&&runNo<=2226)){ // QDC weied
      plasticdb=(Char_t *) Form(dbdir+"reco0/BigRIPSPlastic.2195-2201_2204-2226.xml");
      TOFoffset=305.957;
    }else if(runNo>=2202&&runNo<=2203){ // QDC weied
      plasticdb=(Char_t *) Form(dbdir+"reco0/BigRIPSPlastic.2202-2203.xml");
      TOFoffset=305.957;
    }else if(runNo>=2227&&runNo<=2276){
      plasticdb=(Char_t *) Form(dbdir+"reco0/BigRIPSPlastic.2227-2276.xml");
      TOFoffset=306.3427;
    }else if(runNo>=2277&&runNo<=2299){
      plasticdb=(Char_t *) Form(dbdir+"reco0/BigRIPSPlastic.2277-2299.xml");
      TOFoffset=306.3754;
    }else if(runNo>=2300&&runNo<=2395){
      plasticdb=(Char_t *) Form(dbdir+"reco0/BigRIPSPlastic.2300-2395.xml");
      TOFoffset=306.4308;
    }else if(runNo>=2396&&runNo<=2441){
      plasticdb=(Char_t *) Form(dbdir+"reco0/BigRIPSPlastic.2396-2441.xml");
      TOFoffset=306.2013;
    }else if((runNo>=2442&&runNo<=2470)||(runNo>=2476&&runNo<=2509&&runNo!=2487)){
      plasticdb=(Char_t *) Form(dbdir+"reco0/BigRIPSPlastic.2442-2470_2476-2509.xml");
      TOFoffset=306.7152;
    }else if(runNo>=2471&&runNo<=2475){
      plasticdb=(Char_t *) Form(dbdir+"reco0/BigRIPSPlastic.2471-2475.xml");
      TOFoffset=306.2936;
    }else if(runNo==2487){ // pedestal & weird TDC timing
      plasticdb=(Char_t *) Form(dbdir+"BigRIPSPlastic/BigRIPSPlastic.0.xml");
      TOFoffset=306.2816;
    }

    // 112 runs
    if( runNo>=2520&&runNo<=2529){ // QDC pedestal runs -> no correction
      plasticdb=(Char_t *) Form(dbdir+"reco0/BigRIPSPlastic.2520-2529.xml");
      if(runNo>=2520&&runNo<=2523)TOFoffset = 305.6227;
      if(runNo>=2524&&runNo<=2529)TOFoffset = 305.4227;

    // gain original
    }else if( (runNo>=2530&&runNo<=2568)||
       (runNo>=2601 && runNo<=2610)||runNo==2634 ){
      plasticdb=(Char_t *) Form(dbdir+"reco0/BigRIPSPlastic.2530-2653_GainOriginal.xml");
      TOFoffset = 305.8917;
    }else if(runNo==2569||runNo==2570||runNo==2600||(runNo>=2611 && runNo<=2618)||
             (runNo>=2621 && runNo<=2633)||(runNo>=2639 && runNo<=2646)){
      plasticdb=(Char_t *) Form(dbdir+"reco0/BigRIPSPlastic.2530-2653_GainDropped.xml");
      TOFoffset= 305.4605;
    }else if((runNo>=2571 && runNo<=2599)||
              runNo==2619||runNo==2620||(runNo>=2647&&runNo<=2653) ){
      plasticdb=(Char_t *) Form(dbdir+"reco0/BigRIPSPlastic.2530-2653_GainMedium.xml");
      TOFoffset= 305.7963;
    }
    
    // offset to adjust AoQ 
    Double_t additional_offset=0.; 
    if(runNo>=2174&&runNo<=2509) additional_offset=0.17;
    if(runNo>=2520&&runNo<=2653) additional_offset=0.595;
      
    TOFoffset+=additional_offset;

    // if use beam35 or beam57 in place of beam37.
    Double_t additional_offset_for_RIPS35=0.09;
    //TOFoffset-=additional_offset_for_RIPS35;
    Double_t additional_offset_for_RIPS57=0.08;
    //TOFoffset+=additional_offset_for_RIPS57;


    // Load each cal. parameter files.
    bigripsParameters -> LoadParameter(ppacdb);
    bigripsParameters -> LoadParameter(plasticdb);
    bigripsParameters -> LoadParameter(icdb);   // ch2mev and zcoeff should be calibrated
    bigripsParameters -> LoadParameter(focalplanedb);


    // Create CalibPID to get and calibrate raw data ( CalibPID ->
    //[CalibPPAC , CalibIC, CalibPlastic , CalibFocalPlane] )
    TArtCalibPID *calibPID  = new TArtCalibPID();
    
    // Create RecoPID to get calibrated data and to reconstruct TOF, AoQ, Z, ... (RecoPID ->
    //[ RecoTOF , RecoRIPS , RecoBeam] )
    TArtRecoPID *recoPID = new TArtRecoPID();
    TArtRIPS *rips35 = recoPID -> DefineNewRIPS(3, 5, (Char_t *) Form(anadir+"matrix/mat1.mat"), (Char_t *) "D3"); // F3 - F5
    TArtRIPS *rips57 = recoPID -> DefineNewRIPS(5, 7, (Char_t *) Form(anadir+"matrix/mat2.mat"), (Char_t *) "D5"); // F5 - F7
    TArtTOF  *tof37  = recoPID -> DefineNewTOF((Char_t *) "F3pl", (Char_t *) "F7pl", TOFoffset, 5);
    TArtTOF  *tof37_35  = recoPID -> DefineNewTOF((Char_t *) "F3pl", (Char_t *) "F7pl", TOFoffset-additional_offset_for_RIPS35, 5);
    TArtTOF  *tof37_57  = recoPID -> DefineNewTOF((Char_t *) "F3pl", (Char_t *) "F7pl", TOFoffset+additional_offset_for_RIPS57, 5);
    TArtBeam *beam_br37 = recoPID -> DefineNewBeam(rips35, rips57, tof37, (Char_t *) "F7IC");
    TArtBeam *beam_br35 = recoPID -> DefineNewBeam(rips35, tof37_35, (Char_t *) "F7IC");
    TArtBeam *beam_br57 = recoPID -> DefineNewBeam(rips57, tof37_57, (Char_t *) "F7IC");
    
    TArtSAMURAIParameters *samuraiparameters = TArtSAMURAIParameters::Instance();
    samuraiparameters->LoadParameter(dbdir+"SAMURAIBDC1.xml");
    samuraiparameters->LoadParameter(dbdir+"SAMURAIBDC2.xml");
    auto fCalibBDC1Hit = new TArtCalibBDC1Hit;
    auto fCalibBDC2Hit = new TArtCalibBDC2Hit;
    auto fCalibBDC1Track = new TArtCalibBDC1Track;
    auto fCalibBDC2Track = new TArtCalibBDC2Track;
    
    Int_t tdcwindow[2]={};
    Int_t groupID=-1;
    if(runNo>=2174&&runNo<=2194){tdcwindow[0]=750;tdcwindow[1]=860;groupID=0;}
    else if(runNo>=2195&&runNo<=2280){tdcwindow[0]=960;tdcwindow[1]=1110;groupID=1;}
    else if(runNo>=2281&&runNo<=2360){tdcwindow[0]=960;tdcwindow[1]=1110;groupID=2;}
    else if(runNo>=2361&&runNo<=2410){tdcwindow[0]=960;tdcwindow[1]=1110;groupID=3;}
    else if(runNo>=2411&&runNo<=2470){tdcwindow[0]=960;tdcwindow[1]=1110;groupID=4;}
    else if(runNo>=2471&&runNo<=2475){tdcwindow[0]=750;tdcwindow[1]=860;groupID=5;}
    else if(runNo>=2476&&runNo<=2509){tdcwindow[0]=960;tdcwindow[1]=1110;groupID=6;}
    else if(runNo>=2520&&runNo<=2530){tdcwindow[0]=750;tdcwindow[1]=860;groupID=0;}
    else if(runNo>=2531&&runNo<=2570){tdcwindow[0]=960;tdcwindow[1]=1110;groupID=1;}
    else if(runNo>=2571&&runNo<=2610){tdcwindow[0]=960;tdcwindow[1]=1110;groupID=2;}
    else if(runNo>=2611&&runNo<=2653){tdcwindow[0]=960;tdcwindow[1]=1110;groupID=3;}
    fCalibBDC1Track->SetTDCWindow(tdcwindow[0],tdcwindow[1]);
    fCalibBDC2Track->SetTDCWindow(tdcwindow[0],tdcwindow[1]);
    
    TFile *bdcin=NULL;
    if(runNo>=2174&&runNo<=2509) bdcin = new TFile(anadir+"macros_2/supplemental/bdcTDC108.root","read");
    else if(runNo>=2520&&runNo<=2653) bdcin = new TFile(anadir+"macros_2/supplemental/bdcTDC112.root","read");
    
    if(bdcin->IsOpen()){
      TH1* h_tdc = NULL;
      for(Int_t i=0; i<8; i++){
        h_tdc = (TH1I*)bdcin->Get(Form("h_runsummedtdc1_%d_%d",i,groupID));
        fCalibBDC1Track->SetTDCDistribution(h_tdc,i);
        h_tdc->Clear(); h_tdc = NULL;
        
        h_tdc = (TH1I*)bdcin->Get(Form("h_runsummedtdc2_%d_%d",i,groupID));
        fCalibBDC2Track->SetTDCDistribution(h_tdc,i);
        h_tdc->Clear(); h_tdc = NULL;
      }
      delete h_tdc;
    }else return 1;
    delete bdcin; 
    TString outdir=anadir+"rootfiles/reco0/";
    auto outfile = new TFile( Form(outdir+"run%04d.reco.root",runNo),"recreate");
    auto tree = new TTree("tree", "");
    
    Int_t run;
    tree->Branch("run",&run,"run/I");
    Int_t event;
    tree->Branch("event",&event,"event/I");

    //BigRIPS
    auto ppac_array = (TClonesArray *)storeMan->FindDataContainer("BigRIPSPPAC");
    tree->Branch(ppac_array->GetName(),&ppac_array);
    auto pla_array = (TClonesArray *)storeMan->FindDataContainer("BigRIPSPlastic");
    tree->Branch(pla_array->GetName(),&pla_array);
    auto ic_array = (TClonesArray *)storeMan->FindDataContainer("BigRIPSIC");
    tree->Branch(ic_array->GetName(),&ic_array);
    auto fpl_array = (TClonesArray *)storeMan->FindDataContainer("BigRIPSFocalPlane");
    tree->Branch(fpl_array->GetName(),&fpl_array);
    auto rips_array = (TClonesArray *)storeMan->FindDataContainer("BigRIPSRIPS");
    tree->Branch(rips_array->GetName(),&rips_array);
    auto tof_array  = (TClonesArray *)storeMan->FindDataContainer("BigRIPSTOF");
    tree->Branch(tof_array->GetName(),&tof_array);
    auto beam_array =(TClonesArray *)storeMan->FindDataContainer("BigRIPSBeam");
    tree->Branch(beam_array->GetName(),&beam_array);

    auto bdc1track_array = (TClonesArray *)storeMan->FindDataContainer("SAMURAIBDC1Track");
    auto bdc2track_array = (TClonesArray *)storeMan->FindDataContainer("SAMURAIBDC2Track");
    tree->Branch(bdc1track_array->GetName(),&bdc1track_array);
    tree->Branch(bdc2track_array->GetName(),&bdc2track_array);
    
    Double_t tx1,tx2,tx;
    Double_t ty1,ty2,ty;
    Double_t ta1,ta2,ta;
    Double_t tb1,tb2,tb;
    tree->Branch("tx1",&tx1,"tx1/D");
    tree->Branch("ty1",&ty1,"ty1/D");
    tree->Branch("ta1",&ta1,"ta1/D");
    tree->Branch("tb1",&tb1,"tb1/D");
    tree->Branch("tx2",&tx2,"tx2/D");
    tree->Branch("ty2",&ty2,"ty2/D");
    tree->Branch("ta2",&ta2,"ta2/D");
    tree->Branch("tb2",&tb2,"tb2/D");
    tree->Branch("tx",&tx,"tx/D");
    tree->Branch("ty",&ty,"ty/D");
    tree->Branch("ta",&ta,"ta/D");
    tree->Branch("tb",&tb,"tb/D");

    TArtRawEventObject *rawevent = eventStore->GetRawEventObject();
    TArtRunInfo *runinfo = eventStore->GetRunInfo();

    run=(Int_t)runinfo->GetRunNumber()->Atoi();
    auto plapara = (TArtPlasticPara*)bigripsParameters->GetListOfPlasticPara()->At(2); // get the pointer of F7 plastic parameter 
    Int_t numEvents = 0;
    Double_t dist_BDCs = 1000; //mm
    Double_t dist_BDC1_TGT = 2570.660; //mm
    while (eventStore -> GetNextEvent()) {
        if (numEvents%1000 == 0) {
            std::cout << "Event number = " << numEvents << std::endl;
        }
        
        event=rawevent->GetEventNumber();
        
        calibPID -> ClearData();
        recoPID -> ClearData();
        

        if( (runNo==2395 && event==79963) ){
          plapara->SetTLSlewA(-523.631);
          plapara->SetTRSlewA(-808.182);
          tof37->SetTimeOffset(306.0076+additional_offset);
          tof37_35->SetTimeOffset(306.0076+additional_offset-additional_offset_for_RIPS35);
          tof37_57->SetTimeOffset(306.0076+additional_offset+additional_offset_for_RIPS57);
        } 
        if( (runNo==2395 && event==87497) ){
          plapara->SetTLSlewA(-1379.13);
          plapara->SetTRSlewA(-710.779);
          tof37->SetTimeOffset(306.2013+additional_offset);
          tof37_35->SetTimeOffset(306.2013+additional_offset-additional_offset_for_RIPS35);
          tof37_57->SetTimeOffset(306.2013+additional_offset+additional_offset_for_RIPS57);
        } 
        
        // gain original
        if( (runNo==2600 && event==74814) || (runNo==2633 && event==12498) ){
          plapara->SetTLSlewA(-1642.38);
          tof37->SetTimeOffset(305.8917+additional_offset);
          tof37_35->SetTimeOffset(305.8917+additional_offset-additional_offset_for_RIPS35);
          tof37_57->SetTimeOffset(305.8917+additional_offset+additional_offset_for_RIPS57);
        } 
        // gain dropped 
        if( (runNo==2568 && event==34808) || (runNo==2599 && event==25171) || (runNo==2601 && event==4275)  ||
            (runNo==2610 && event==3045)  || (runNo==2613 && event==54313) || (runNo==2618 && event==39010) ||
            (runNo==2620 && event==41946) || (runNo==2623 && event==40334)){
          plapara->SetTLSlewA(-679.992);
          tof37->SetTimeOffset(305.4605+additional_offset);
          tof37_35->SetTimeOffset(305.4605+additional_offset-additional_offset_for_RIPS35);
          tof37_57->SetTimeOffset(305.4605+additional_offset+additional_offset_for_RIPS57);
        } 
        // gain incompletely recovered
        if( (runNo==2569 && event==41106) || (runNo==2570 && event==5837) || (runNo==2615 && event==1112) ||
            (runNo==2618 && (event==21740||event==48960)) || (runNo==2623 && event==6347) || (runNo==2646 && event==46926)){
          plapara->SetTLSlewA(-1463.38);
          tof37->SetTimeOffset(305.7963+additional_offset);
          tof37_35->SetTimeOffset(305.7963+additional_offset-additional_offset_for_RIPS35);
          tof37_57->SetTimeOffset(305.7963+additional_offset+additional_offset_for_RIPS57);
        } 

        if( runNo==2613&&event==20736 ){
          plapara->SetTLSlewA(-995.699);
          tof37->SetTimeOffset(305.5063+additional_offset);
          tof37_35->SetTimeOffset(305.5063+additional_offset-additional_offset_for_RIPS35);
          tof37_57->SetTimeOffset(305.5063+additional_offset+additional_offset_for_RIPS57);
        }
      
           
        calibPID -> ReconstructData();
        recoPID -> ReconstructData();
        
        fCalibBDC1Hit->ClearData();
        fCalibBDC2Hit->ClearData();
        fCalibBDC1Track->ClearData();
        fCalibBDC2Track->ClearData();
        fCalibBDC1Hit->ReconstructData();
        fCalibBDC2Hit->ReconstructData();
        fCalibBDC1Track->ReconstructData();
        fCalibBDC2Track->ReconstructData();

        tx1=tx2=tx=ty1=ty2=ty=-9999.;
        ta1=ta2=ta=tb1=tb2=tb=-9999.;
        if (bdc1track_array) {
          Int_t bdc1ntr = bdc1track_array -> GetEntries();
          for (Int_t itr = 0; itr < bdc1ntr; ++itr) {
            auto trk = (TArtDCTrack *) bdc1track_array -> At(itr);
            if (trk -> GetPosition(0) > -9999){
              tx1 = trk -> GetPosition(0);
              ta1 = trk -> GetAngle(0);
            }else if (trk -> GetPosition(1) > -9999){
              ty1 = trk -> GetPosition(1);
              tb1 = trk -> GetAngle(1);
            }
          }
        }

        if (bdc2track_array) {
          Int_t bdc2ntr = bdc2track_array -> GetEntries();
          for (Int_t itr = 0; itr < bdc2ntr; ++itr) {
            auto trk = (TArtDCTrack *) bdc2track_array -> At(itr);
            if (trk -> GetPosition(0) > -9999){
              tx2 = trk -> GetPosition(0);
              ta2 = trk -> GetAngle(0);
            }else if (trk -> GetPosition(1) > -9999){
              ty2 = trk -> GetPosition(1);
              tb2 = trk -> GetAngle(1);
            }
          }
        }

        if (tx1 > -1000 && ty1 > -1000 && tx2 > -1000 && ty2 > -1000) {
          tx = ( tx2 - tx1 )/dist_BDCs*dist_BDC1_TGT + tx1;
          ty = ( ty2 - ty1 )/dist_BDCs*dist_BDC1_TGT + ty1;
          ta = ( ta2 - ta1 )*1000.; // mrad
          tb = ( tb2 - tb1 )*1000.; // mrad
        }
        tree->Fill();

        numEvents++;

    }

    outfile -> Write();
    outfile -> Close();

}
