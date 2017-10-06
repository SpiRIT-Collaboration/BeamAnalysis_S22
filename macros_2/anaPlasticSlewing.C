Bool_t isFileLoaded=kFALSE;
const TString planame[4]={"F3","F7","F13-1","F13-2"}; 
const Int_t plaid[4]={0,2,3,4}; 
std::vector<Int_t> runNolist;
const TString anapath="/home/s015/exp/exp1605_s015/anaroot/users/S22_analysis/";
const TString datapath=anapath+"rootfiles/rawdata/";

const Int_t beamtype=108;
// use 0~3 for corresponding PMT behaviors
const Int_t datatype=4;

void loadFiles(Int_t);
void makeQDCPedestal();
void getQDCPedestal(Double_t*, Double_t*);
void makeADCTDC();
Int_t whichDataType(Int_t,Int_t,Int_t);

void anaPlasticSlewing(){
  
  if(!isFileLoaded) loadFiles(beamtype);
  
  Double_t lpedestal[2], rpedestal[2];
  getQDCPedestal(lpedestal, rpedestal);

  // get slweing parameter
  TH2F* h_qltl[2];
  TH2F* h_qrtr[2];
  FileStat_t file;
  if( gSystem->GetPathInfo(Form(anapath+"macros_2/supplemental/plaADCTDC%d.root",beamtype),file) ) makeADCTDC(); 
  auto atfile = new TFile( Form(anapath+"macros_2/supplemental/plaADCTDC%d.root",beamtype),"read");
  if(beamtype==108){
    if(datatype==0){ // 2175 - 2194
      h_qltl[0]=(TH2F*)atfile->Get("h_f3qltl0");
      h_qrtr[0]=(TH2F*)atfile->Get("h_f3qrtr0");
      h_qltl[1]=(TH2F*)atfile->Get("h_f7qltl0");
      h_qrtr[1]=(TH2F*)atfile->Get("h_f7qrtr0");
    }else if(datatype==1){ // 2227 - 2395 
      h_qltl[0]=(TH2F*)atfile->Get("h_f3qltl1");
      h_qrtr[0]=(TH2F*)atfile->Get("h_f3qrtr1");
      h_qltl[1]=(TH2F*)atfile->Get("h_f7qltl1");
      h_qrtr[1]=(TH2F*)atfile->Get("h_f7qrtr1");
    }else if(datatype==2){ // 2227 - 2395 
      h_qltl[0]=(TH2F*)atfile->Get("h_f3qltl2");
      h_qrtr[0]=(TH2F*)atfile->Get("h_f3qrtr2");
      h_qltl[1]=(TH2F*)atfile->Get("h_f7qltl2");
      h_qrtr[1]=(TH2F*)atfile->Get("h_f7qrtr2");
    }else if(datatype==3){ // 2227 - 2395 
      h_qltl[0]=(TH2F*)atfile->Get("h_f3qltl3");
      h_qrtr[0]=(TH2F*)atfile->Get("h_f3qrtr3");
      h_qltl[1]=(TH2F*)atfile->Get("h_f7qltl3");
      h_qrtr[1]=(TH2F*)atfile->Get("h_f7qrtr3");
    }else if(datatype==4){ // 2395 F7 gain dropped
      h_qltl[0]=(TH2F*)atfile->Get("h_f3qltl3");
      h_qrtr[0]=(TH2F*)atfile->Get("h_f3qrtr3");
      h_qltl[1]=(TH2F*)atfile->Get("h_f7qltl4");
      h_qrtr[1]=(TH2F*)atfile->Get("h_f7qrtr4");
    }else if(datatype==5){ // 2395 - 2441
      h_qltl[0]=(TH2F*)atfile->Get("h_f3qltl3");
      h_qrtr[0]=(TH2F*)atfile->Get("h_f3qrtr3");
      h_qltl[1]=(TH2F*)atfile->Get("h_f7qltl5");
      h_qrtr[1]=(TH2F*)atfile->Get("h_f7qrtr5");
    }else if(datatype==6){ // after 2442
      h_qltl[0]=(TH2F*)atfile->Get("h_f3qltl3");
      h_qrtr[0]=(TH2F*)atfile->Get("h_f3qrtr3");
      h_qltl[1]=(TH2F*)atfile->Get("h_f7qltl5");
      h_qrtr[1]=(TH2F*)atfile->Get("h_f7qrtr6");
    }else return;
  
  }else if(beamtype==112){
    h_qltl[0]=(TH2F*)atfile->Get("h_f3qltl0");
    h_qrtr[0]=(TH2F*)atfile->Get("h_f3qrtr0");
    h_qrtr[1]=(TH2F*)atfile->Get("h_f7qrtr0");
    if(datatype==0){ // original gain
      h_qltl[1]=(TH2F*)atfile->Get("h_f7qltl0");
    }else if(datatype==1){ // dropped gain
      h_qltl[1]=(TH2F*)atfile->Get("h_f7qltl1");
    }else if(datatype==2){ // incompletely recovered gain
      h_qltl[1]=(TH2F*)atfile->Get("h_f7qltl2");
    }else if(datatype==3){ // 2613 incompletely recovered gain
      h_qltl[1]=(TH2F*)atfile->Get("h_f7qltl3");
    }else return;
    
  }

  TF1* f_qt1 = new TF1("f_qt1","[0]+[1]/TMath::Sqrt(x)");
 /* TF1* f_qt2 = new TF1("f_qt2","[0]+[1]/TMath::Sqrt(x+[2])");
  TF1* f_qt3 = new TF1("f_qt3","[0]+[1]*TMath::Exp([2]/TMath::Sqrt(x))");
  TF1* f_qt4 = new TF1("f_qt4","[0]+[1]/TMath::Sqrt(x)+[2]/TMath::Sqrt(x)/x");
  TF1* f_qt5 = new TF1("f_qt5","[0]+[1]/x");
  */

  auto c2 = new TCanvas("c2","",1400,700);
  c2->Divide(2,2);
  Float_t slew_coeffl[2];
  Float_t slew_coeffr[2];
  for(int i=0; i<2; i++){
    c2->cd(2*i+1);
    h_qltl[i]->Draw("");
    h_qltl[i]->Fit(f_qt1,"","",10,300);
    slew_coeffl[i]=f_qt1->GetParameter(1);
    c2->cd(2*i+2);
    h_qrtr[i]->Draw("");
    h_qrtr[i]->Fit(f_qt1,"","",10,300);
    slew_coeffr[i]=f_qt1->GetParameter(1);
  }

  
  // apply slewing correction
  TH2F* h_sqltl[2];
  TH2F* h_sqrtr[2];
  h_sqltl[0] = new TH2F( "h_sqltl0","slew F3PLA_L;QDC-pedestal;Time ",300,0.,300,3000,32000,35000);
  h_sqrtr[0] = new TH2F( "h_sqrtr0","slew F3PLA_R;QDC-pedestal;Time ",300,0.,300,3000,32000,35000);
  h_sqltl[1] = new TH2F( "h_sqltl1","slew F7PLA_L;QDC-pedestal;Time ",300,0.,300,2000,28000,30000);
  h_sqrtr[1] = new TH2F( "h_sqrtr1","slew F7PLA_R;QDC-pedestal;Time ",300,0.,300,2000,28000,30000);
 
  
  TH2D* h_tofql[2];
  TH2D* h_tofqr[2];
  TH2D* h_stofql[2];
  TH2D* h_stofqr[2];
  for(int i=0; i<2; i++){
    h_tofql[i] = new TH2D( Form("h_tofql%d",i),"TOF vs "+planame[i]+"-QL;QDC-pedestal;TOF(ns)",300,0,300,200,-80,-60);
    h_tofqr[i] = new TH2D( Form("h_tofqr%d",i),"TOF vs "+planame[i]+"-QR;QDC-pedestal;TOF(ns)",300,0,300,200,-80,-60);
    h_stofql[i] = new TH2D( Form("h_stofql%d",i),"slew TOF vs "+planame[i]+"-QL;QDC-pedestal;slew TOF(ns)",300,0,300,200,-80,-60);
    h_stofqr[i] = new TH2D( Form("h_stofqr%d",i),"slew TOF vs "+planame[i]+"-QR;QDC-pedestal;slew TOF(ns)",300,0,300,200,-80,-60);
  }
  
  TH1D* h_tof = new TH1D("h_tof","TOF;raw TOF: F7-F3 (ns);counts/500ps",200,-73,-63);
  TH1D* h_stof = new TH1D("h_stof","slew TOF;slew raw TOF: F7-F3 (ns);counts/500ps",200,-73,-63);
  
  TH1D* h_stof_108Sn = new TH1D("h_stof_108Sn","108Sn slew TOF;slew raw TOF: F7-F3 (ns);counts/0.1ns",100,-75,-65);
  TH1D* h_adcsqsum = new TH1D("h_adcsqsum","",2000,3000,5000);
  
  Int_t start=runNolist.front();
  Int_t end=runNolist.back();
  auto h_runstof = new TH2F("h_runstof","RUN vs TOF w. slewing;RUN;TOF",end+1-start,start,end+1,200,-73,-63);
   
  std::vector<Int_t> runnum;
  std::vector<Double_t> tofmean;
  std::vector<Double_t> stofmean;
  
  TH1D* hg_tof;
  TH1D* hg_stof;
  TF1* fg_tof = new TF1("fg_tof","gaus");
  
  auto cvs = new TCanvas("cvs","");

  for(auto runNo: runNolist){
    if( beamtype==108 && ( (runNo>=2195&&runNo<=2226)||runNo==2487||(runNo>=2471&&runNo<=2475)) ) continue;
    if( beamtype==112 && runNo<2530 ) continue;
    if( beamtype==108 && datatype==0 && runNo>=2195 ) continue;  // ~2194
    if( beamtype==108 && datatype==1 && (runNo<=2226||runNo>=2277) ) continue;  // 2227 - 2276
    if( beamtype==108 && datatype==2 && (runNo<=2278||runNo>=2300) ) continue;  // 2277 - 2299
    if( beamtype==108 && datatype==3 && (runNo<=2299||runNo>=2396) ) continue;  // 2300 - 2395
    if( beamtype==108 && datatype==4 && runNo!=2395 ) continue; // 2395 gain dropped.
    if( beamtype==108 && datatype==5 && (runNo<=2394||runNo>=2442) ) continue; // 2395 - 2441
    if( beamtype==108 && datatype==6 && runNo<=2441 ) continue; // ~2442
    BeamRaw* sraw = new BeamRaw();
    TString fname = Form(datapath+"run%04d.ridf.root",runNo);
    sraw->fChain->AddFile(fname.Data(),0,"tree");
    sraw->InitPlaADC();

    std::cout<<"RUN"<<runNo<<std::endl; 

    hg_tof = new TH1D("hg_tof","",200,-80,-60);
    hg_stof = new TH1D("hg_stof","",200,-80,-60);
    
    Bool_t isDataExist=kFALSE; 
    for(Int_t i=0; i<sraw->fChain->GetEntries(); i++){
      
      if( datatype != whichDataType(beamtype,runNo,i) ) continue;
      sraw->fChain->GetEntry(i);
      Double_t TOF = sraw->BigRIPSPlastic_fTime[2]-sraw->BigRIPSPlastic_fTime[0];
      Double_t adcl[2], adcr[2], stimel[2], stimer[2];
      for(Int_t j=0; j<2; j++){
        adcl[j] = sraw->BigRIPSPlastic_fQLRaw[plaid[j]]-lpedestal[j];
        adcr[j] = sraw->BigRIPSPlastic_fQRRaw[plaid[j]]-rpedestal[j];
        stimel[j] = sraw->BigRIPSPlastic_fTLRaw[plaid[j]] -slew_coeffl[j]/TMath::Sqrt(adcl[j]);
        stimer[j] = sraw->BigRIPSPlastic_fTRRaw[plaid[j]] -slew_coeffr[j]/TMath::Sqrt(adcr[j]);
      }
      
      Double_t sTOF = 0.0156253*(0.5*(stimel[1]+stimer[1])-0.5*(stimel[0]+stimer[0]));
      
      h_tof->Fill(TOF); 
      h_stof->Fill(sTOF); 
      hg_tof->Fill(TOF); 
      hg_stof->Fill(sTOF); 
      h_runstof->Fill(runNo,sTOF); 

      for(Int_t j=0; j<2; j++){
        h_sqltl[j]->Fill(adcl[j],stimel[j]);
        h_sqrtr[j]->Fill(adcr[j],stimer[j]);

        h_tofql[j]->Fill(adcl[j],TOF);
        h_tofqr[j]->Fill(adcr[j],TOF);
        h_stofql[j]->Fill(adcl[j],sTOF);
        h_stofqr[j]->Fill(adcr[j],sTOF);
        
      }
      isDataExist=kTRUE;

    }
    
    if(isDataExist){
      runnum.push_back(runNo);
      cvs->cd();
      hg_tof->Draw();
      Double_t mean = hg_tof->GetBinCenter(hg_tof->GetMaximumBin());
      fg_tof->SetParameters(hg_tof->GetMaximum(),mean,0.2);
      hg_tof->Fit(fg_tof,"OQ","",mean-0.5,mean+0.5);
      tofmean.push_back(fg_tof->GetParameter(1));
      fg_tof->Clear();

      hg_stof->Draw();
      mean = hg_stof->GetBinCenter(hg_stof->GetMaximumBin());
      fg_tof->SetParameters(hg_stof->GetMaximum(),mean,0.15);
      hg_stof->Fit(fg_tof,"OQ","",mean-0.5,mean+0.5);
      stofmean.push_back(fg_tof->GetParameter(1));
    }
    hg_tof->Clear();
    hg_stof->Clear();
    fg_tof->Clear();
    sraw->Clear();
    
  }
  
  

  auto sc2 = new TCanvas("sc2","",1400,700);
  sc2->Divide(2,2);
  for(int i=0; i<2; i++){
    sc2->cd(2*i+1);
    h_sqltl[i]->Draw("colz");
    sc2->cd(2*i+2);
    h_sqrtr[i]->Draw("colz");
  }
  auto sc4 = new TCanvas("sc4","",1400,700);
  sc4->Divide(2,2);
  for(int i=0; i<2; i++){
    sc4->cd(2*i+1);
    h_stofql[i]->Draw("colz");
    sc4->cd(2*i+2);
    h_stofqr[i]->Draw("colz");
  }
  auto sc5 = new TCanvas("sc5","",1400,700);
  sc5->Divide(2,2);
  for(int i=0; i<2; i++){
    sc5->cd(2*i+1);
    h_tofql[i]->Draw("colz");
    sc5->cd(2*i+2);
    h_tofqr[i]->Draw("colz");
  }

  
  auto f_tof = new TF1("f_tof","gaus");
  auto sc7 = new TCanvas("sc7","",1000,500);
  sc7->Divide(2,1);
  sc7->cd(1); h_tof->Draw();
  Double_t mean = h_tof->GetBinCenter(h_tof->GetMaximumBin());
  f_tof->SetParameters(h_tof->GetMaximum(),mean,0.2);
  h_tof->Fit(f_tof,"","",mean-1,mean+1);
  sc7->cd(2); h_stof->Draw();
  mean = h_stof->GetBinCenter(h_stof->GetMaximumBin());
  f_tof->SetParameters(h_stof->GetMaximum(),mean,0.15);
  h_stof->Fit(f_tof,"","",mean-1,mean+1);


  
  auto sc8 = new TCanvas("sc8","");

  TGraph* g_tof = new TGraph(runnum.size());
  TGraph* g_stof = new TGraph(runnum.size());
  for(Int_t i=0; i<runnum.size(); i++){
    g_tof->SetPoint(i,runnum.at(i),tofmean.at(i));
    g_stof->SetPoint(i,runnum.at(i),stofmean.at(i));
  }
  auto mg_tof = new TMultiGraph("mg_tof","");
  g_tof->SetMarkerStyle(3);
  mg_tof->Add(g_tof);
  g_stof->SetLineColor(kRed);
  g_stof->SetMarkerStyle(3);
  g_stof->SetMarkerColor(kRed);
  mg_tof->Add(g_stof);
  mg_tof->Draw("APL");

  auto crunstof = new TCanvas("crunstof","");
  h_runstof->Draw("colz");
  
  
  std::ofstream out;
  out.open(Form(anapath+"macros_2/supplemental/plasticslewing_%d_%d.txt",beamtype,datatype),std::ios::out);
  for(Int_t i=0; i<2; i++)
    out << lpedestal[i] << "," << rpedestal[i] << "," << -1.*slew_coeffl[i] << "," << -1.*slew_coeffr[i] << std::endl;

  out.close();

}

Int_t whichDataType(Int_t beam, Int_t run, Int_t event){
  
  Int_t datatype=-1;

  if(beam==108){
    
    if( run>=2175&&run<=2194 ) datatype=0;                                       // 2175 ~ 2194
    else if( run>=2227&&run<=2276 ) datatype=1;   // before 2395, eve<=79962
    else if( run>=2277&&run<=2299 ) datatype=2;   // before 2395, eve<=79962
    else if( (run>=2300&&run<=2394) || (run==2395&&event<=79962) ) datatype=3;   // before 2395, eve<=79962
    else if( run==2395&&event>=79963&&event<=87496 ) datatype=4;                 // gain dropped in 2395
    else if( (run==2395&&event>=87497) || (run>=2396&&run<=2441) ) datatype=5;   // 2395, eve>=87497 - 2441
    else if( run>=2442 || !(run>=2471&&run<=2475) || run!=2487) datatype=6;      // after 2442
    else datatype=-1;                                                            // no correction will be applied
  
  }else if(beam==112){

    // gain original
    if( run<=2567 || (run==2568 && event<=34807) ||
       (run==2600 && event>=74814) || (run==2601 && event<=4274) ||
       (run>=2602 && run<=2609) || (run==2610 && event<=3044) ||
       (run==2633 && event>=12498) || run==2634 ) datatype=0;
    // gain dropped 
    if( (run==2568 && event>=34808) || (run==2569 && event<=41105) || (run==2570 && event<=5836) ||
        (run==2599 && event>=25171) || (run==2600 && event<=74814) || (run==2601 && event>=4275) ||
        (run==2610 && event>=3045)  || (run>=2611 && run<=2612)    || 
        (run==2613 && (event<=20735||event>=54313)) || (run==2615 && event<=1111) ||
        (run>=2616 && run<=2617)    || (run==2618 && (event<=21739||(event>=39010&&event<=48959))) ||
        (run==2620 && event>=41946) || (run>=2621 && run<=2622)    ||
        (run==2623 && (event<=6346 || event>=40334)) || (run>=2624 && run<=2632) ||
        (run==2633 && event<=12497) || (run>=2639 && run<=2645) || (run==2646 && event<=46925) ) datatype=1;    
    // gain incompletely recovered
    if( (run==2569 && event>=41106) || (run==2570 && event>=5837) || (run>=2571 && run<=2598) ||
        (run==2599 && event<=25170) || (run==2615 && event>=1112) ||
        (run==2618 && ((event>=21740&&event<=39009)||event>=48960)) || run==2619 || (run==2620 && event<=41945) || 
        (run==2623 && (event>=6347&&event<=40333)) || (run==2646 && event>=46926) || run>=2647 ) datatype=2;    
    if( (run==2613 && (event>=20736&&event<=54312)) ) datatype=3;
  }

  return datatype;

}

void loadFiles(Int_t bt){
  
  if(!(bt==108||bt==112)) return 0;
  Int_t start=2174;
  Int_t stop=2509;
  if(bt==112){ start=2520; stop=2653;}

  Int_t nrun=0;
  runNolist.clear();
  FileStat_t file;
  for(Int_t irun=start; irun<=stop; irun++){
    if( !gSystem->GetPathInfo( Form(datapath+"run%04d.ridf.root",irun),file) ){
      if(irun!=2175&&irun!=2176){  // plastic didn't work then.
        runNolist.push_back(irun);
        nrun++;
      }
      //if(nrun>50) break;
    }
  }
  isFileLoaded=kTRUE;
}

void makeQDCPedestal(){
  
  Int_t beam[2]={108,112};
  std::vector<Int_t> pedestallist[2];
  pedestallist[0].assign({2471,2472,2473,2474,2475});
  pedestallist[1].assign({2520,2521,2522,2523,2524,2525,2526,2529});
  BeamRaw* pedestal[2];
  TFile* file[2];
  auto cped = new TCanvas("cped","");
  cped->Divide(2,2);
  for(Int_t i=0; i<2; i++){
    pedestal[i] = new BeamRaw();
    for(auto prun: pedestallist[i]){
      TString fname = Form(datapath+"run%04d.ridf.root",prun);
      pedestal[i]->fChain->AddFile(fname.Data(),0,"tree");
    }
    pedestal[i]->InitPlastic();

    file[i] = new TFile(Form(anapath+"macros_2/supplemental/plaPedestal%d.root",beam[i]),"recreate");
    TH1I* h_pedestall[2];
    TH1I* h_pedestalr[2];
    for(Int_t j=0; j<2; j++){
      cped->cd(2*j+1);
      h_pedestall[j] = new TH1I( Form("h_pedestall%d",j),planame[j]+"-QL;QDC;counts/QDC",200,100,300);
      pedestal[i]->fChain->Draw( Form("BigRIPSPlastic.fQLRaw[%d]>>h_pedestall%d",plaid[j],j));
      h_pedestall[j]->Write();
      cped->cd(2*j+2);
      h_pedestalr[j] = new TH1I( Form("h_pedestalr%d",j),planame[j]+"-QR;QDC;counts/QDC",200,100,300);
      pedestal[i]->fChain->Draw( Form("BigRIPSPlastic.fQRRaw[%d]>>h_pedestalr%d",plaid[j],j));
      h_pedestalr[j]->Write();
    }
    file[i]->Close();
    
    pedestal[i]->Clear(); delete pedestal[i];
  }
  cped->Clear(); delete cped;
}

void getQDCPedestal(Double_t* pl, Double_t* pr){

  FileStat_t file;
  if( gSystem->GetPathInfo(Form(anapath+"macros_2/supplemental/plaPedestal%d.root",beamtype),file) ) makeQDCPedestal(); 
  
  auto pfile = new TFile(Form(anapath+"macros_2/supplemental/plaPedestal%d.root",beamtype),"read");
  TH1I* h_pl[2];
  TH1I* h_pr[2];
  TF1* f_ped = new TF1("f_ped","gaus");
  auto cp = new TCanvas("cp","");
  cp->Divide(2,2);
  for(Int_t i=0; i<2; i++){
    cp->cd(2*i+1);
    h_pl[i]=(TH1I*)pfile->Get(Form("h_pedestall%d",i));
    h_pl[i]->Draw();
    f_ped->SetParameters(h_pl[i]->GetMaximum(),h_pl[i]->GetBinCenter(h_pl[i]->GetMaximumBin()),h_pl[i]->GetRMS());
    h_pl[i]->Fit(f_ped);
    pl[i] = f_ped->GetParameter(1);
    cp->cd(2*i+2);
    h_pr[i]=(TH1I*)pfile->Get(Form("h_pedestalr%d",i));
    h_pr[i]->Draw();
    f_ped->SetParameters(h_pr[i]->GetMaximum(),h_pr[i]->GetBinCenter(h_pr[i]->GetMaximumBin()),h_pr[i]->GetRMS());
    h_pr[i]->Fit(f_ped);
    pr[i] = f_ped->GetParameter(1);
  }
  pfile->Close(); delete pfile;
  cp->Clear(); delete cp; 
  
}

void makeADCTDC(){

  if(!isFileLoaded) loadFiles(beamtype);
  
  Double_t lpedestal[2], rpedestal[2];
  getQDCPedestal(lpedestal, rpedestal);
  
  auto ofile = new TFile( Form(anapath+"macros_2/supplemental/plaADCTDC%d.root",beamtype),"recreate");

  TF1* f_tof = new TF1("f_tof","gaus");
  TH1D* h_tof = NULL;
  
  TH2F* h_f3qltl[4];
  TH2F* h_f3qrtr[4];
  TH2F* h_f7qltl[6];
  TH2F* h_f7qrtr[7];
  
  Int_t n=100;
  TVectorD f3qlvec[7];
  TVectorD f3tlvec[7];
  TVectorD f3qrvec[7];
  TVectorD f3trvec[7];
  TVectorD f7qlvec[7];
  TVectorD f7tlvec[7];
  TVectorD f7qrvec[7];
  TVectorD f7trvec[7];
  for(Int_t i=0; i<7; i++){
    f3qlvec[i].ResizeTo(n); 
    f3qrvec[i].ResizeTo(n); 
    f3tlvec[i].ResizeTo(n); 
    f3trvec[i].ResizeTo(n); 
    f7qlvec[i].ResizeTo(n); 
    f7qrvec[i].ResizeTo(n); 
    f7tlvec[i].ResizeTo(n); 
    f7trvec[i].ResizeTo(n); 
  }
  if(beamtype==108){
    // F3 L
    h_f3qltl[0] = new TH2F( "h_f3qltl0","F3PLA_L 2175 - 2194;QDC-pedestal;TDC",300,0.,300,3000,44000,47000);
    h_f3qltl[1] = new TH2F( "h_f3qltl1","F3PLA_L 2227 - 2276;QDC-pedestal;TDC",300,0.,300,3000,32000,35000);
    h_f3qltl[2] = new TH2F( "h_f3qltl2","F3PLA_L 2277 - 2299;QDC-pedestal;TDC",300,0.,300,3000,32000,35000);
    h_f3qltl[3] = new TH2F( "h_f3qltl3","F3PLA_L after 2300 ;QDC-pedestal;TDC",300,0.,300,3000,32000,35000);
    // F3 R
    h_f3qrtr[0] = new TH2F( "h_f3qrtr0","F3PLA_R 2175 - 2194;QDC-pedestal;TDC",300,0.,300,3000,44000,47000);
    h_f3qrtr[1] = new TH2F( "h_f3qrtr1","F3PLA_R 2227 - 2276;QDC-pedestal;TDC",300,0.,300,3000,32000,35000);
    h_f3qrtr[2] = new TH2F( "h_f3qrtr2","F3PLA_R 2277 - 2299;QDC-pedestal;TDC",300,0.,300,3000,32000,35000);
    h_f3qrtr[3] = new TH2F( "h_f3qrtr3","F3PLA_R after 2300 ;QDC-pedestal;TDC",300,0.,300,3000,32000,35000);
    // F7 L
    h_f7qltl[0] = new TH2F( "h_f7qltl0","F7PLA_L 2175 - 2194     ;QDC-pedestal;TDC",300,0.,300,2000,40000,42000);
    h_f7qltl[1] = new TH2F( "h_f7qltl1","F7PLA_L 2227 - 2276     ;QDC-pedestal;TDC",300,0.,300,2000,28000,30000);
    h_f7qltl[2] = new TH2F( "h_f7qltl2","F7PLA_L 2277 - 2299     ;QDC-pedestal;TDC",300,0.,300,2000,28000,30000);
    h_f7qltl[3] = new TH2F( "h_f7qltl3","F7PLA_L 2300 - 2395     ;QDC-pedestal;TDC",300,0.,300,2000,28000,30000);
    h_f7qltl[4] = new TH2F( "h_f7qltl4","F7PLA_L 2395 gaindropped;QDC-pedestal;TDC",300,0.,300,2000,28000,30000);
    h_f7qltl[5] = new TH2F( "h_f7qltl5","F7PLA_L after 2395      ;QDC-pedestal;TDC",300,0.,300,2000,28000,30000);
    // F7 R
    h_f7qrtr[0] = new TH2F( "h_f7qrtr0","F7PLA_R 2175 - 2194      ;QDC-pedestal;TDC",300,0.,300,2000,40000,42000);
    h_f7qrtr[1] = new TH2F( "h_f7qrtr1","F7PLA_R 2227 - 2276      ;QDC-pedestal;TDC",300,0.,300,2000,28000,30000);
    h_f7qrtr[2] = new TH2F( "h_f7qrtr2","F7PLA_R 2277 - 2299      ;QDC-pedestal;TDC",300,0.,300,2000,28000,30000);
    h_f7qrtr[3] = new TH2F( "h_f7qrtr3","F7PLA_R 2300 - 2395      ;QDC-pedestal;TDC",300,0.,300,2000,28000,30000);
    h_f7qrtr[4] = new TH2F( "h_f7qrtr4","F7PLA_R 2395 gain dropped;QDC-pedestal;TDC",300,0.,300,2000,28000,30000);
    h_f7qrtr[5] = new TH2F( "h_f7qrtr5","F7PLA_R 2395 - 2441      ;QDC-pedestal;TDC",300,0.,300,2000,28000,30000);
    h_f7qrtr[6] = new TH2F( "h_f7qrtr6","F7PLA_R after 2442       ;QDC-pedestal;TDC",300,0.,300,2000,28000,30000);
  }else if(beamtype==112){
    h_f3qltl[0] = new TH2F( "h_f3qltl0","F3PLA_L;QDC-pedestal;TDC",300,0.,300,3000,32000,35000);
    h_f3qrtr[0] = new TH2F( "h_f3qrtr0","F3PLA_R;QDC-pedestal;TDC",300,0.,300,3000,32000,35000);
    h_f7qrtr[0] = new TH2F( "h_f7qrtr0","F7PLA_R;QDC-pedestal;TDC",300,0.,300,2000,28000,30000);
    h_f7qltl[0] = new TH2F( "h_f7qltl0","F7PLA_L gain original ;QDC-pedestal;TDC",300,0.,300,2000,28000,30000);
    h_f7qltl[1] = new TH2F( "h_f7qltl1","F7PLA_L gain dropped;QDC-pedestal;TDC",300,0.,300,2000,28000,30000);
    h_f7qltl[2] = new TH2F( "h_f7qltl2","F7PLA_L gain incompletely recovered;QDC-pedestal;TDC",300,0.,300,2000,28000,30000);
    h_f7qltl[3] = new TH2F( "h_f7qltl3","F7PLA_L 2613 gain incompletely recovered;QDC-pedestal;TDC",300,0.,300,2000,28000,30000);
  }
  
  TH2F* h_plaqlavsum[2];
  TH2F* h_plaqravsum[2];
  for(int i=0; i<2; i++){
    h_plaqlavsum[i] = new TH2F(Form("h_plaqlavsum%d",i),planame[i]+"QL vs IC;pla QDC-pedestal;IC RawADCAvSum",300,0,300,8000,0,8000);
    h_plaqravsum[i] = new TH2F(Form("h_plaqravsum%d",i),planame[i]+"QR vs IC;pla QDC-pedestal;IC RawADCAvSum",300,0,300,8000,0,8000);
  }
 
  TH2F* h_qltlnocut[2];
  TH2F* h_qrtrnocut[2];
  h_qltlnocut[0] = new TH2F( "h_qltlnocut0","F3PLA_L;QDC-pedestal;Time(ns)",300,0.,300,3000,32000,35000);
  h_qrtrnocut[0] = new TH2F( "h_qrtrnocut0","F3PLA_R;QDC-pedestal;Time(ns)",300,0.,300,3000,32000,35000);
  h_qltlnocut[1] = new TH2F( "h_qltlnocut1","F7PLA_L;QDC-pedestal;Time(ns)",300,0.,300,2000,28000,30000);
  h_qrtrnocut[1] = new TH2F( "h_qrtrnocut1","F7PLA_R;QDC-pedestal;Time(ns)",300,0.,300,2000,28000,30000);
 
  auto c = new TCanvas("c","");
  Int_t counter[7]={};
  // get slweing parameter
  for(auto runNo: runNolist){
    if( beamtype==108 && ((runNo>=2195&&runNo<=2226)||runNo==2487||(runNo>=2471&&runNo<=2475)) ) continue;
    if( beamtype==112 && runNo<2530 ) continue;
    BeamRaw* raw = new BeamRaw();
    TString fname = Form(datapath+"run%04d.ridf.root",runNo);
    raw->fChain->AddFile(fname.Data(),0,"tree");
    raw->InitPlaADC();

    //if(raw->fChain->GetEntries()<10000) continue;

    std::cout<<"RUN"<<runNo<<std::endl; 
    
    h_tof = new TH1D( "h_tof","",200,-80,-60);
    raw->fChain->Draw("BigRIPSPlastic.fTime[2]-BigRIPSPlastic.fTime[0]>>h_tof");
    Int_t constant = h_tof->GetMaximum();
    Double_t mean = h_tof->GetBinCenter(h_tof->GetMaximumBin());
    Double_t fwhm = 0.5*(h_tof->GetBinCenter(h_tof->FindLastBinAbove(0.5*constant))-h_tof->GetBinCenter(h_tof->FindFirstBinAbove(0.5*constant)));
    f_tof->SetParameters(constant,mean,fwhm);
    h_tof->Fit(f_tof,"","",mean-3.*fwhm,mean+3.*fwhm);
    Float_t tofmin, tofmax;
    tofmin = f_tof->GetParameter(1) - 3.*f_tof->GetParameter(2);
    tofmax = f_tof->GetParameter(1) + 3.*f_tof->GetParameter(2);
    f_tof->Clear();
    h_tof->Clear();
     
    for(Int_t i=0; i<raw->fChain->GetEntries(); i++){
      raw->fChain->GetEntry(i);
      Float_t TOF = raw->BigRIPSPlastic_fTime[2]-raw->BigRIPSPlastic_fTime[0];
      Float_t avsum = raw->BigRIPSIC_fRawADCAvSum[2];
      Float_t adcl[2], adcr[2];
      Int_t   tdcl[2], tdcr[2];
      for(Int_t j=0; j<2; j++){//0:F3, 1:F7
        adcl[j] = raw->BigRIPSPlastic_fQLRaw[plaid[j]]-lpedestal[j];
        adcr[j] = raw->BigRIPSPlastic_fQRRaw[plaid[j]]-rpedestal[j];
        tdcl[j] = raw->BigRIPSPlastic_fTLRaw[plaid[j]];
        tdcr[j] = raw->BigRIPSPlastic_fTRRaw[plaid[j]];
        h_qltlnocut[j]->Fill(adcl[j],tdcl[j]);
        h_qrtrnocut[j]->Fill(adcr[j],tdcr[j]);
        h_plaqlavsum[j]->Fill(adcl[j],avsum);
        h_plaqravsum[j]->Fill(adcr[j],avsum);
      }
      if(TOF>tofmin&&TOF<tofmax){
        if(avsum>4000&&avsum<4400){  //select 108Sn

          if(beamtype==108){
            
            Int_t dt = whichDataType(beamtype,runNo,i); 
            if( dt==0 ){ // 2175 ~ 2194
              h_f3qltl[0]->Fill(adcl[0],tdcl[0]);
              h_f3qrtr[0]->Fill(adcr[0],tdcr[0]);
              h_f7qltl[0]->Fill(adcl[1],tdcl[1]);
              h_f7qrtr[0]->Fill(adcr[1],tdcr[1]);
              if(counter[0]>=n){n*=2; 
              f3qlvec[0].ResizeTo(n);
              f3qrvec[0].ResizeTo(n);
              f3tlvec[0].ResizeTo(n);
              f3trvec[0].ResizeTo(n);
              f7qlvec[0].ResizeTo(n);
              f7qrvec[0].ResizeTo(n);
              f7tlvec[0].ResizeTo(n);
              f7trvec[0].ResizeTo(n);
              }
              f3qlvec[0](counter[0])=adcl[0];
              f3qrvec[0](counter[0])=adcr[0];
              f3tlvec[0](counter[0])=tdcl[0];
              f3trvec[0](counter[0])=tdcr[0];
              f7qlvec[0](counter[0])=adcl[1];
              f7qrvec[0](counter[0])=adcr[1];
              f7tlvec[0](counter[0])=tdcl[1];
              f7trvec[0](counter[0])=tdcr[1];
              counter[0]++;
            }else if( dt==1 ){ // 2227 - 2276
              h_f3qltl[1]->Fill(adcl[0],tdcl[0]);
              h_f3qrtr[1]->Fill(adcr[0],tdcr[0]);
              h_f7qltl[1]->Fill(adcl[1],tdcl[1]);
              h_f7qrtr[1]->Fill(adcr[1],tdcr[1]);
            }else if( dt==2 ){ // 2277 - 2299
              h_f3qltl[2]->Fill(adcl[0],tdcl[0]);
              h_f3qrtr[2]->Fill(adcr[0],tdcr[0]);
              h_f7qltl[2]->Fill(adcl[1],tdcl[1]);
              h_f7qrtr[2]->Fill(adcr[1],tdcr[1]);
            }else if( dt==3 ){ // 2300 - 2394, 2395 eve<=79962
              h_f3qltl[3]->Fill(adcl[0],tdcl[0]);
              h_f3qrtr[3]->Fill(adcr[0],tdcr[0]);
              h_f7qltl[3]->Fill(adcl[1],tdcl[1]);
              h_f7qrtr[3]->Fill(adcr[1],tdcr[1]);
            }else if( dt==4 ){ // gain dropped in 2395
              h_f3qltl[3]->Fill(adcl[0],tdcl[0]);
              h_f3qrtr[3]->Fill(adcr[0],tdcr[0]);
              h_f7qltl[4]->Fill(adcl[1],tdcl[1]);
              h_f7qrtr[4]->Fill(adcr[1],tdcr[1]);
            }else if( dt==5 ){ // 2395, eve>=87497 - 2441
              h_f3qltl[3]->Fill(adcl[0],tdcl[0]);
              h_f3qrtr[3]->Fill(adcr[0],tdcr[0]);
              h_f7qltl[5]->Fill(adcl[1],tdcl[1]);
              h_f7qrtr[5]->Fill(adcr[1],tdcr[1]);
            }else if ( dt==6 ){ // after 2442
              h_f3qltl[3]->Fill(adcl[0],tdcl[0]);
              h_f3qrtr[3]->Fill(adcr[0],tdcr[0]);
              h_f7qltl[5]->Fill(adcl[1],tdcl[1]);
              h_f7qrtr[6]->Fill(adcr[1],tdcr[1]);
            }
          
          }else if(beamtype==112){
            
            h_f3qltl[0]->Fill(adcl[0],tdcl[0]);
            h_f3qrtr[0]->Fill(adcr[0],tdcr[0]);
            h_f7qrtr[0]->Fill(adcr[1],tdcr[1]);
            if(whichDataType(beamtype,runNo,i)!=-1) h_f7qltl[whichDataType(beamtype,runNo,i)]->Fill(adcl[1],tdcl[1]);
          
          }
        }
      }
    }
    raw->Clear();
    delete raw; raw=NULL;
    
  }
 
  c->Clear();
  delete c;


  
  TGraph* g_f3qltl[4];
  TGraph* g_f3qrtr[4];
  TGraph* g_f7qltl[6];
  TGraph* g_f7qrtr[7];
  g_f3qltl[0] = new TGraph(f3qlvec[0],f3tlvec[0]);
  g_f3qrtr[0] = new TGraph(f3qrvec[0],f3trvec[0]);
  g_f7qltl[0] = new TGraph(f7qlvec[0],f7tlvec[0]);
  g_f7qrtr[0] = new TGraph(f7qrvec[0],f7trvec[0]);
  g_f3qltl[0]->SetName("g_f3qltl0");
  g_f3qrtr[0]->SetName("g_f3qrtr0");
  g_f7qltl[0]->SetName("g_f7qltl0");
  g_f7qrtr[0]->SetName("g_f7qrtr0");

  if(beamtype==108){
    h_f3qltl[0]->Write();
    h_f3qltl[1]->Write();
    h_f3qltl[2]->Write();
    h_f3qltl[3]->Write();
    h_f3qrtr[0]->Write();
    h_f3qrtr[1]->Write();
    h_f3qrtr[2]->Write();
    h_f3qrtr[3]->Write();
    h_f7qltl[0]->Write(); 
    h_f7qltl[1]->Write(); 
    h_f7qltl[2]->Write();
    h_f7qltl[3]->Write();
    h_f7qltl[4]->Write();
    h_f7qltl[5]->Write();
    h_f7qrtr[0]->Write();   
    h_f7qrtr[1]->Write();   
    h_f7qrtr[2]->Write();   
    h_f7qrtr[3]->Write();
    h_f7qrtr[4]->Write();   
    h_f7qrtr[5]->Write();
    h_f7qrtr[6]->Write();   
    g_f3qltl[0]->Write();
    g_f3qrtr[0]->Write();
    g_f7qltl[0]->Write();
    g_f7qrtr[0]->Write();
  }else if(beamtype==112){
    h_f3qltl[0]->Write();
    h_f3qrtr[0]->Write();
    h_f7qrtr[0]->Write(); 
    h_f7qltl[0]->Write();
    h_f7qltl[1]->Write();
    h_f7qltl[2]->Write(); 
    h_f7qltl[3]->Write(); 
  }
  for(Int_t i=0; i<2; i++){ 
    h_qltlnocut[i]->Write();
    h_qrtrnocut[i]->Write();
    h_plaqlavsum[i]->Write();
    h_plaqravsum[i]->Write();
  }

  ofile->Close();
}
