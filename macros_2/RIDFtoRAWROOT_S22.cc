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
#include "TArtCalibPPAC.hh"
#include "TArtCalibPlastic.hh"
#include "TArtCalibIC.hh"
#include "TArtCalibPID.hh"
#include "TArtPPACPara.hh"
#include "TArtPlasticPara.hh"
#include "TArtICPara.hh"
#include "TArtSAMURAIParameters.hh"
#include "TArtCalibBDC1Hit.hh"
#include "TArtCalibBDC2Hit.hh"
#include "TArtDCHit.hh"
#include "TArtDCHitPara.hh"
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
    if(runNo>=2174&&runNo<2195) ppacdb = (Char_t*) Form(dbdir+"raw/BigRIPSPPAC.raw0.xml");
    else if((runNo>=2195&&runNo<2202)||(runNo>=2204&&runNo<2392)) ppacdb = (Char_t*) Form(dbdir+"raw/BigRIPSPPAC.raw1.xml");
    else if(runNo>=2202&&runNo<2204) ppacdb = (Char_t*) Form(dbdir+"raw/BigRIPSPPAC.raw2.xml");
    else if(runNo>=2392&&runNo<2395) ppacdb = (Char_t*) Form(dbdir+"raw/BigRIPSPPAC.raw3.xml");
    else if((runNo>=2395&&runNo<2471)||(runNo>=2476&&runNo<2509)) ppacdb = (Char_t*) Form(dbdir+"raw/BigRIPSPPAC.raw4.xml");
    else if(runNo>=2471&&runNo<2476) ppacdb = (Char_t*) Form(dbdir+"raw/BigRIPSPPAC.raw5.xml");
    else if(runNo>=2520&&runNo<2530) ppacdb = (Char_t*) Form(dbdir+"raw/BigRIPSPPAC.raw6.xml");
    else if(runNo>=2530&&runNo<2654) ppacdb = (Char_t*) Form(dbdir+"raw/BigRIPSPPAC.raw7.xml");
    Char_t* pladb = NULL;
    if(runNo>=2174&&runNo<2195) pladb  = (Char_t*) Form(dbdir+"raw/BigRIPSPlastic.raw0.xml");
    else if((runNo>=2195&&runNo<2202)||(runNo>=2204&&runNo<2471)||(runNo>=2476&&runNo<2510)) pladb  = (Char_t*) Form(dbdir+"raw/BigRIPSPlastic.raw1.xml");
    else if(runNo>=2202&&runNo<2204) pladb  = (Char_t*) Form(dbdir+"raw/BigRIPSPlastic.raw2.xml");
    else if(runNo>=2471&&runNo<2476) pladb  = (Char_t*) Form(dbdir+"raw/BigRIPSPlastic.raw3.xml");
    else if(runNo>=2520&&runNo<2530) pladb  = (Char_t*) Form(dbdir+"raw/BigRIPSPlastic.raw4.xml");
    else if(runNo>=2530&&runNo<2654) pladb  = (Char_t*) Form(dbdir+"raw/BigRIPSPlastic.raw5.xml");
    Char_t* icdb   = (Char_t*) Form(dbdir+"raw/BigRIPSIC.0.xml");
    
    // Load each cal. parameter files.
    bigripsParameters -> LoadParameter(ppacdb);
    bigripsParameters -> LoadParameter(pladb);
    bigripsParameters -> LoadParameter(icdb);

    // Create CalibPID to get and calibrate raw data ( CalibPID ->
    //[CalibPPAC , CalibIC, CalibPlastic , CalibFocalPlane] )
    TArtCalibPID *calibPID  = new TArtCalibPID();

    TArtSAMURAIParameters *samuraiparameters = TArtSAMURAIParameters::Instance();
    samuraiparameters->LoadParameter(dbdir+"SAMURAIBDC1.xml");
    samuraiparameters->LoadParameter(dbdir+"SAMURAIBDC2.xml");
    auto fCalibBDC1Hit = new TArtCalibBDC1Hit;
    auto fCalibBDC2Hit = new TArtCalibBDC2Hit;

    
    TString outdir=anadir+"rootfiles/rawdata/";
    auto outfile = new TFile( Form(outdir+"run%04d.ridf.root",runNo),"recreate");
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
    
    auto bdc1hit_array = (TClonesArray *)storeMan->FindDataContainer("SAMURAIBDC1Hit");
    auto bdc2hit_array = (TClonesArray *)storeMan->FindDataContainer("SAMURAIBDC2Hit");
    tree->Branch(bdc1hit_array->GetName(),&bdc1hit_array);
    tree->Branch(bdc2hit_array->GetName(),&bdc2hit_array);


    TArtRawEventObject *rawevent = eventStore->GetRawEventObject();
    TArtRunInfo *runinfo = eventStore->GetRunInfo();

    run=(Int_t)runinfo->GetRunNumber()->Atoi();

    while (eventStore -> GetNextEvent()) {
        if (event%1000 == 0) {
            std::cout << "Event number = " << event << std::endl;
        }
        event=rawevent->GetEventNumber();
        
        calibPID -> ClearData();
        calibPID -> ReconstructData();
        
        fCalibBDC1Hit->ClearData();
        fCalibBDC2Hit->ClearData();
        fCalibBDC1Hit->ReconstructData();
        fCalibBDC2Hit->ReconstructData();

        tree->Fill();

    }

    outfile -> Write();
    outfile -> Close();

}
