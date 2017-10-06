// PPAC: detector-id = 1(PPACT), module-id = 24(V1190)
// BigRIPS Plastic: fpl-id = 13, detector-id = 3(PLAT), module-id = 34(MTDC32)
// BigRIPS IC: detector-id = 4(ICE), module-id = 32(MADC32)

void anaPlastic(){

  Int_t runNo=2331;
  TString infile =Form("../ridf/SMDAQ%i.ridf",runNo);

  gSystem->Load("libanaroot.so");

  TArtEventStore *estore = new TArtEventStore();
  estore->Open(infile);
  TArtRawEventObject *rawevent = estore->GetRawEventObject();
  
  
  auto h_ach = new TH1I("h_ach","",16,0,16);
  auto h_aval = new TH1I("h_aval","",1500,0,1500);
  auto h_achval = new TH2I("h_achval","",1500,0,1500,16,0,16);
  
  auto h_tch = new TH1I("h_tch","",6,16,22);
  auto h_tval = new TH1I("h_tval","",10000,0,100000);
  auto h_tchval = new TH2I("h_tchval","",10000,0,100000,6,16,22);

  Int_t neve=0;
  while(estore->GetNextEvent()){
    if(neve%1000==0) std::cout<<"event:"<<neve<<std::endl;
    if(neve==10000) break;
    
    // start to scan raw event tree
    for(int i=0;i<rawevent->GetNumSeg();i++){
      TArtRawSegmentObject *seg = rawevent->GetSegment(i);
      int fpl = seg->GetFP();
      int det = seg->GetDetector();

      if(!(fpl==63&&(det==2||det==3))) continue;//BigRIPS plastic

      for(int j=0;j<seg->GetNumData();j++){
        TArtRawDataObject *d = seg->GetData(j);
        int ch = d->GetCh();
        int val = d->GetVal();
        if(det==2){ //QDC(V792)
          h_ach->Fill(ch);
          h_aval->Fill(val);
          h_achval->Fill(val,ch);
        }else if(det==3){ //TDC(M32TDC)
          h_tch->Fill(ch);
          h_tval->Fill(val);
          h_tchval->Fill(val,ch);
        }
      }


    }


    estore->ClearData();
    neve ++;

  }
  
  auto c = new TCanvas("c","");
  h_tchval->Draw("colz");
}
