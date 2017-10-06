// PPAC: detector-id = 1(PPACT), module-id = 24(V1190)
// BigRIPS Plastic: fpl-id = 13, detector-id = 3(PLAT), module-id = 34(MTDC32)
// BigRIPS IC: detector-id = 4(ICE), module-id = 32(MADC32)

void anaPPAC(){

  Int_t runNo=2331;
  TString infile =Form("../ridf/SMDAQ%d.ridf",runNo);

  gSystem->Load("libanaroot.so");

  TArtEventStore *estore = new TArtEventStore();
  estore->Open(infile);
  TArtRawEventObject *rawevent = estore->GetRawEventObject();
  

  Double_t ch2ns = 0.09765;
  Int_t TOverFlow= 50000;

  auto h_ch = new TH1I("h_ch","",400,-200,200);
  auto h_val = new TH1I("h_val","",3000,0,300000);
  auto h_chval = new TH2I("h_chval","",3000,0,30000,70,0,70);

  auto h_chtime = new TH2D("h_chtime","",3000,0,3000,70,0,70);

  //F3PPAC
  //[4]: 1A, 1B, 2A, 2B
  TH2I* h_f3ppacnhit[4]; // X1, Y1, X2, Y2, A
  TH2D* h_f3ppactime[4]; // X1, Y1, X2, Y2, A
  TH2D* h_f3ppactsum[4]; // TSumX, TSumY
  for(int i=0; i<4; i++){
    h_f3ppacnhit[i] = new TH2I( Form("h_f3ppacnhit%d",i),"",5,0,5,5,0,5);
    h_f3ppacnhit[i]->GetYaxis()->SetBinLabel(1,"X1");
    h_f3ppacnhit[i]->GetYaxis()->SetBinLabel(2,"Y1");
    h_f3ppacnhit[i]->GetYaxis()->SetBinLabel(3,"X2");
    h_f3ppacnhit[i]->GetYaxis()->SetBinLabel(4,"Y2");
    h_f3ppacnhit[i]->GetYaxis()->SetBinLabel(5,"Anode");
    h_f3ppactime[i] = new TH2D( Form("h_f3ppactime%d",i),"",300,0,3000,5,0,5);
    h_f3ppactime[i]->GetYaxis()->SetBinLabel(1,"X1");
    h_f3ppactime[i]->GetYaxis()->SetBinLabel(2,"Y1");
    h_f3ppactime[i]->GetYaxis()->SetBinLabel(3,"X2");
    h_f3ppactime[i]->GetYaxis()->SetBinLabel(4,"Y2");
    h_f3ppactime[i]->GetYaxis()->SetBinLabel(5,"Anode");
    h_f3ppactsum[i] = new TH2D( Form("h_f3ppactsum%d",i),"",1000,0,200,2,0,2);
    h_f3ppactsum[i]->GetYaxis()->SetBinLabel(1,"TSumX");
    h_f3ppactsum[i]->GetYaxis()->SetBinLabel(2,"TSumY");
  }


  auto h_f3ppac1nhit = new TH1I("h_f3ppac1nhit","",25,0,25);
  auto h_f3ppac2nhit = new TH1I("h_f3ppac2nhit","",25,0,25);
  auto h_f3ppac12nhit = new TH2I("h_f3ppac12nhit","",25,0,25,25,0,25);


  Int_t neve=0;
  while(estore->GetNextEvent()){
    if(neve%1000==0) std::cout<<"event:"<<neve<<std::endl;
    //if(neve==10000) break;

    // start to scan raw event tree
    for(int i=0;i<rawevent->GetNumSeg();i++){
      TArtRawSegmentObject *seg = rawevent->GetSegment(i);
      int det = seg->GetDetector();
      int device = seg->GetDevice();


      if(det == 1){//PPAC

        std::vector<Int_t> valbuf[70];
        for(int b=0; b<70; b++) valbuf[b].clear();
        for(int j=0;j<seg->GetNumData();j++){
          TArtRawDataObject *d = seg->GetData(j);
          int ch = d->GetCh();
          if(ch<4||ch>64) continue; //used PPAC channel: 4~64
          int val = d->GetVal();
          if(val<0||val>TOverFlow) continue; 
          h_ch->Fill(ch);
          h_val->Fill(val);
          h_chval->Fill(val,ch);
          h_chtime->Fill(val*ch2ns,ch);

          valbuf[ch].push_back( val );
        }

        //F3 doublePPAC*2 1A,1B,2A,2B V1190 MTDC
        std::vector<Int_t> TX1raw[4];
        std::vector<Int_t> TY1raw[4];
        std::vector<Int_t> TX2raw[4];
        std::vector<Int_t> TY2raw[4];
        std::vector<Int_t> TAraw[4];
        
        for(int ich=4; ich<8; ich++){
          TAraw[ich-4] = valbuf[ich];
          TX1raw[ich-4] = valbuf[ich*4];
          TY1raw[ich-4] = valbuf[ich*4+1];
          TX2raw[ich-4] = valbuf[ich*4+2];
          TY2raw[ich-4] = valbuf[ich*4+3];
        }
        Int_t f3ppac1nhit=0;
        Int_t f3ppac2nhit=0;
        
        for(int ipl=0; ipl<4; ipl++){
          h_f3ppacnhit[ipl]->Fill(TX1raw[ipl].size(),0);
          h_f3ppacnhit[ipl]->Fill(TY1raw[ipl].size(),1);
          h_f3ppacnhit[ipl]->Fill(TX2raw[ipl].size(),2);
          h_f3ppacnhit[ipl]->Fill(TY2raw[ipl].size(),3);
          h_f3ppacnhit[ipl]->Fill(TAraw[ipl].size(),4);
          f3ppac1nhit+= TX1raw[ipl].size()+TY1raw[ipl].size();
          f3ppac2nhit+= TX2raw[ipl].size()+TY2raw[ipl].size();

          for(int ihit=0; ihit<TX1raw[ipl].size(); ihit++) h_f3ppactime[ipl]->Fill(ch2ns*TX1raw[ipl].at(ihit),0);
          for(int ihit=0; ihit<TY1raw[ipl].size(); ihit++) h_f3ppactime[ipl]->Fill(ch2ns*TY1raw[ipl].at(ihit),1);
          for(int ihit=0; ihit<TX2raw[ipl].size(); ihit++) h_f3ppactime[ipl]->Fill(ch2ns*TX2raw[ipl].at(ihit),2);
          for(int ihit=0; ihit<TY2raw[ipl].size(); ihit++) h_f3ppactime[ipl]->Fill(ch2ns*TY2raw[ipl].at(ihit),3);
          for(int ihit=0; ihit<TAraw[ipl].size(); ihit++) h_f3ppactime[ipl]->Fill(ch2ns*TAraw[ipl].at(ihit),4);

          for(int ihit=0; ihit<TAraw[ipl].size(); ihit++){
            for(int x1hit=0; x1hit<TX1raw[ipl].size(); x1hit++){
              for(int x2hit=0; x2hit<TX2raw[ipl].size(); x2hit++){
                h_f3ppactsum[ipl]->Fill(ch2ns*(TX1raw[ipl].at(x1hit)+TX2raw[ipl].at(x2hit)-2.*TAraw[ipl].at(ihit)),0);
              }
            }
            for(int y1hit=0; y1hit<TY1raw[ipl].size(); y1hit++){
              for(int y2hit=0; y2hit<TY2raw[ipl].size(); y2hit++){
                h_f3ppactsum[ipl]->Fill(ch2ns*(TY1raw[ipl].at(y1hit)+TY2raw[ipl].at(y2hit)-2.*TAraw[ipl].at(ihit)),1);
              }
            }
          }
        }
        h_f3ppac1nhit->Fill(f3ppac1nhit);
        h_f3ppac2nhit->Fill(f3ppac2nhit);
        h_f3ppac12nhit->Fill(f3ppac1nhit,f3ppac2nhit);
      
      }

      if(device != USERGR) continue;

    }


    estore->ClearData();
    neve ++;

  }
 
 /*
  auto c = new TCanvas("c","");
  c->Divide(2,2);
  for(int i=0; i<4; i++){
    c->cd(i+1);
    h_f3ppacnhit[i]->Draw("colz");
  }
  auto c1 = new TCanvas("c1","",700,400);
  c1->Divide(2,1);
  c1->cd(1)->SetLogy(); h_f3ppac1nhit->Draw();
  c1->cd(2)->SetLogy(); h_f3ppac2nhit->Draw();
 
  
  auto c2 = new TCanvas("c2","");
  c2->Divide(2,2);
  for(int i=0; i<4; i++){
    c2->cd(i+1);
    h_f3ppactime[i]->Draw("colz");
  }
  
  
  auto c3 = new TCanvas("c3","");
  c3->Divide(2,2);
  for(int i=0; i<4; i++){
    c3->cd(i+1);
    h_f3ppactsum[i]->Draw("colz");
  }*/
    
}
