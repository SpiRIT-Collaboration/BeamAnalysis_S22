// PPAC: detector-id = 1(PPACT), module-id = 24(V1190)
// BigRIPS Plastic: fpl-id = 13, detector-id = 3(PLAT), module-id = 34(MTDC32)
// BigRIPS IC: detector-id = 4(ICE), module-id = 32(MADC32)

void anaMUSIC(){

  Int_t runNo=2331;
  TString infile =Form("../ridf/SMDAQ%i.ridf",runNo);

  gSystem->Load("libanaroot.so");

  TArtEventStore *estore = new TArtEventStore();
  estore->Open(infile);
  TArtRawEventObject *rawevent = estore->GetRawEventObject();
  

  auto h_val = new TH1I("h_val","",800,0,8000);
  auto h_chval = new TH2I("h_chval","",800,0,8000,6,0,6);

  Int_t neve=0;
  while(estore->GetNextEvent()){
    if(neve%1000==0) std::cout<<"event:"<<neve<<std::endl;
    if(neve==10000) break;
    
    // start to scan raw event tree
    for(int i=0;i<rawevent->GetNumSeg();i++){
      TArtRawSegmentObject *seg = rawevent->GetSegment(i);
      int det = seg->GetDetector();

      if(det != 4) continue;//MUSIC

      for(int j=0;j<seg->GetNumData();j++){
        TArtRawDataObject *d = seg->GetData(j);
        int ch = d->GetCh();
        int val = d->GetVal();
        h_val->Fill(val);
        h_chval->Fill(val,ch);

      }


    }


    estore->ClearData();
    neve ++;

  }
  
  auto c = new TCanvas("c","");
  h_chval->Draw("colz");
}
