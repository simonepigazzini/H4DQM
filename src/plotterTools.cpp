#include <plotterTools.hpp>
#include <assert.h>
#define VERBOSE 0


float 
plotterTools::getMinimumP (TProfile * p)
{
  float min = p->GetBinContent (1) ;
  for (int i = 2 ; i <= p->GetNbinsX () ; ++i)
    {
      if (p->GetBinError (i) == 0) continue ;
//      cout << min << " " << p->GetBinContent (i) << endl ;
      if (p->GetBinContent (i) < min) min = p->GetBinContent (i) ;
    }
  return min ;
}


float 
plotterTools::getMaximumP (TProfile * p)
{
  float max = p->GetBinContent (1) ;
  for (int i = 2 ; i <= p->GetNbinsX () ; ++i)
    {
      if (p->GetBinError (i) == 0) continue ;
//      cout << max << " " << p->GetBinContent (i) << endl ;
      if (p->GetBinContent (i) > max) max = p->GetBinContent (i) ;
    }
  return max ;
}



plotterTools::plotterTools(TString filename, TString outfname, TString outdname){

  setPlotsFormat () ;
  inputFile_ = TFile::Open(filename);

  if( inputFile_==0 ) {
    std::cout << "ERROR! Din't find file " << filename << std::endl;
    std::cout << "Exiting." << std::endl;
    exit(11);
  }

  inputTree_ = (TChain*) inputFile_->Get("H4tree");
  outputFile_ = TFile::Open(outfname,"RECREATE");
  outputDir_=outdname;

  fillFiberOrder();

  wantADCplots = false;
  wantDigiplots = false;

};

void plotterTools::initIntegrated(TString nameFile){

  gStyle->SetOptStat (0) ;
  isIntegratedNew=true;
  integratedFile_=TFile::Open(nameFile,"new");
  if(integratedFile_==NULL){
    isIntegratedNew=false;
    integratedFile_=TFile::Open(nameFile,"Update");
    }
  assert(integratedFile_!=NULL);

  map<TString, TObject*>::iterator it;

  TH1F* evt_info=NULL;
  TH1F* trg_info=NULL;

  TH1F* hX_info=NULL;
  TH1F* hY_info=NULL;

  TH1F* hSX_info=NULL;
  TH1F* hSY_info=NULL;


  it=outObjects_.find(plotLongNames_["beamPositionX"]);
  if(it!=outObjects_.end()){
   hX_info=(TH1F*)outObjects_[plotLongNames_["beamPositionX"]];
   hY_info=(TH1F*)outObjects_[plotLongNames_["beamPositionY"]];

  }

  it=outObjects_.find(plotLongNames_["nTotalEvts"]);
  if(it!=outObjects_.end()){
    evt_info=(TH1F*)outObjects_[plotLongNames_["nTotalEvts"]];
    trg_info=(TH1F*)outObjects_[plotLongNames_["fractionTakenTrigHisto"]];
  }

  it=outObjects_.find(plotLongNames_["beamPositionSmallX"]);
  if(it!=outObjects_.end()){
   hSX_info=(TH1F*)outObjects_[plotLongNames_["beamPositionSmallX"]];
   hSY_info=(TH1F*)outObjects_[plotLongNames_["beamPositionSmallY"]];

  }


    if(hX_info!=NULL){

      integratedPlots_["hodo_meanX_spill"]=(TH1F*)integratedFile_->Get("hodo_meanX_spill");
      integratedPlots_["hodo_meanY_spill"]=(TH1F*)integratedFile_->Get("hodo_meanY_spill");//add controls
      integratedPlots_["hodo_RMSX_spill"]=(TH1F*)integratedFile_->Get("hodo_RMSX_spill");
      integratedPlots_["hodo_RMSY_spill"]=(TH1F*)integratedFile_->Get("hodo_RMSY_spill");//add controls
     
      if(integratedPlots_["hodo_meanX_spill"]==NULL){
	integratedPlots_["hodo_meanX_spill"]=new TH1F("hodo_meanX_spill","hodo_meanX_spill",2000,0,2000);
	integratedPlots_["hodo_meanY_spill"]=new TH1F("hodo_meanY_spill","hodo_meanY_spill",2000,0,2000);
	integratedPlots_["hodo_RMSX_spill"]=new TH1F("hodo_RMSX_spill","hodo_RMSX_spill",2000,0,2000);
	integratedPlots_["hodo_RMSY_spill"]=new TH1F("hodo_RMSY_spill","hodo_RMSY_spill",2000,0,2000);


      }

    }
    if(evt_info!=NULL){
      integratedPlots_["nTotalEvtsPerSpill"]=(TH1F*)integratedFile_->Get("nTotalEvtsPerSpill");
      integratedPlots_["nTotalEvtsPerSpillHisto"]=(TH1F*)integratedFile_->Get("nTotalEvtsPerSpillHisto");
      integratedPlots_["fractionTakenTrigPerSpill"]=(TH1F*)integratedFile_->Get("fractionTakenTrigPerSpill");
      integratedPlots_["triggerRateHisto"]=(TH1F*)integratedFile_->Get("triggerRateHisto");
      integratedPlots_["triggerRatePerSpill"]=(TH1F*)integratedFile_->Get("triggerRatePerSpill");
      integratedPlots_["growingEventPlot"]=(TH1F*)integratedFile_->Get("growingEventPlot");
      if(integratedPlots_["nTotalEvtsPerSpill"]==NULL){
	integratedPlots_["nTotalEvtsPerSpill"]=new TH1F("nTotalEvtsPerSpill","nTotalEvtsPerSpill",2000,0,2000);
	integratedPlots_["nTotalEvtsPerSpillHisto"]=new TH1F("nTotalEvtsPerSpillHisto","nTotalEvtsPerSpillHisto",2000,100,10000);
	integratedPlots_["fractionTakenTrigPerSpill"]=new TH1F("fractionTakenTrigPerSpill","fractionTakenTrigPerSpill",2000,0,2000);
	integratedPlots_["triggerRateHisto"]=new TH1F("triggerRateHisto","triggerRateHisto",2000,0,1000);
	integratedPlots_["triggerRatePerSpill"]=new TH1F("triggerRatePerSpill","triggerRatePerSpill",2000,0,1000);
	integratedPlots_["growingEventPlot"]=new TH1F("growingEventPlot","growingEventPlot",2000,0,1000);
      }

    }

    if(hSX_info!=NULL){

      integratedPlots_["hodoSmall_meanX_spill"]=(TH1F*)integratedFile_->Get("hodoSmall_meanX_spill");
      integratedPlots_["hodoSmall_meanY_spill"]=(TH1F*)integratedFile_->Get("hodoSmall_meanY_spill");//add controls
      integratedPlots_["hodoSmall_RMSX_spill"]=(TH1F*)integratedFile_->Get("hodoSmall_RMSX_spill");
      integratedPlots_["hodoSmall_RMSY_spill"]=(TH1F*)integratedFile_->Get("hodoSmall_RMSY_spill");//add controls

      if(integratedPlots_["hodoSmall_meanX_spill"]==NULL){
	integratedPlots_["hodoSmall_meanX_spill"]=new TH1F("hodoSmall_meanX_spill","hodoSmall_meanX_spill",2000,0,2000);
	integratedPlots_["hodoSmall_meanY_spill"]=new TH1F("hodoSmall_meanY_spill","hodoSmall_meanY_spill",2000,0,2000);
	integratedPlots_["hodoSmall_RMSX_spill"]=new TH1F("hodoSmall_RMSX_spill","hodoSmall_RMSX_spill",2000,0,2000);
	integratedPlots_["hodoSmall_RMSY_spill"]=new TH1F("hodoSmall_RMSY_spill","hodoSmall_RMSY_spill",2000,0,2000);

      }

    }



  if(hX_info!=NULL){

  int iBin=0;
  for(iBin=1;iBin<integratedPlots_["hodo_meanX_spill"]->GetNbinsX() && integratedPlots_["hodo_meanX_spill"]->GetBinContent(iBin)>0; ++iBin){}
  integratedPlots_["hodo_meanX_spill"]->SetBinContent(iBin,hX_info->GetMean());
  integratedPlots_["hodo_meanX_spill"]->SetBinError(iBin,hX_info->GetRMS());

  integratedPlots_["hodo_meanY_spill"]->SetBinContent(iBin,hY_info->GetMean());
  integratedPlots_["hodo_meanY_spill"]->SetBinError(iBin,hY_info->GetRMS());

  integratedPlots_["hodo_RMSX_spill"]->SetBinContent(iBin,hX_info->GetRMS());
  integratedPlots_["hodo_RMSY_spill"]->SetBinContent(iBin,hX_info->GetRMS());

  setAxisTitles(integratedPlots_["hodo_meanX_spill"], "nSpill","mean X" );
  setAxisTitles(integratedPlots_["hodo_meanY_spill"], "nSpill","mean Y" );

  setAxisTitles(integratedPlots_["hodo_RMSX_spill"], "nSpill","RMS X" );
  setAxisTitles(integratedPlots_["hodo_RMSY_spill"], "nSpill","RMS Y" );


  plotMe(integratedPlots_["hodo_meanX_spill"]);
  plotMe(integratedPlots_["hodo_meanY_spill"]);

  plotMe(integratedPlots_["hodo_RMSX_spill"]);
  plotMe(integratedPlots_["hodo_RMSY_spill"]);

  }

  if(hSX_info!=NULL){
    
    int iBin=0;
    for(iBin=1;iBin<integratedPlots_["hodoSmall_meanX_spill"]->GetNbinsX() && integratedPlots_["hodoSmall_meanX_spill"]->GetBinContent(iBin)>0; ++iBin){}
    integratedPlots_["hodoSmall_meanX_spill"]->SetBinContent(iBin,hX_info->GetMean());
    integratedPlots_["hodoSmall_meanX_spill"]->SetBinError(iBin,hX_info->GetRMS());
    
    integratedPlots_["hodoSmall_meanY_spill"]->SetBinContent(iBin,hY_info->GetMean());
    integratedPlots_["hodoSmall_meanY_spill"]->SetBinError(iBin,hY_info->GetRMS());

    integratedPlots_["hodoSmall_RMSX_spill"]->SetBinContent(iBin,hX_info->GetRMS());    
    integratedPlots_["hodoSmall_RMSY_spill"]->SetBinContent(iBin,hY_info->GetRMS());    
    
    setAxisTitles(integratedPlots_["hodoSmall_meanX_spill"], "nSpill","mean X" );
    setAxisTitles(integratedPlots_["hodoSmall_meanY_spill"], "nSpill","mean Y" );
    
    plotMe(integratedPlots_["hodoSmall_meanX_spill"]);
    plotMe(integratedPlots_["hodoSmall_meanY_spill"]);

    setAxisTitles(integratedPlots_["hodoSmall_RMSX_spill"], "nSpill","RMS X" );
    setAxisTitles(integratedPlots_["hodoSmall_RMSY_spill"], "nSpill","RMS Y" );
    
    plotMe(integratedPlots_["hodoSmall_RMSX_spill"]);
    plotMe(integratedPlots_["hodoSmall_RMSY_spill"]);


  }


  if(evt_info != NULL){

  int iBin=0;
  for(iBin=1;iBin<integratedPlots_["nTotalEvtsPerSpill"]->GetNbinsX() && integratedPlots_["nTotalEvtsPerSpill"]->GetBinContent(iBin)>0; ++iBin){}
  integratedPlots_["nTotalEvtsPerSpill"]->SetBinContent(iBin,evt_info->GetEntries());
  setAxisTitles(integratedPlots_["nTotalEvtsPerSpill"], "nSpill","nEvts" );
  plotMe(integratedPlots_["nTotalEvtsPerSpill"]);



  integratedPlots_["fractionTakenTrigPerSpill"]->SetBinContent(iBin,trg_info->GetMean());
  setAxisTitles(integratedPlots_["fractionTakenTrigPerSpill"], "nSpill","fractionTakenTrig" );
  plotMe(integratedPlots_["fractionTakenTrigPerSpill"]);


  integratedPlots_["triggerRatePerSpill"]->SetBinContent(iBin,100000*evt_info->GetEntries()/(timeEnd_[0]-timeStart_[0]));//it's in Hz
  setAxisTitles(integratedPlots_["triggerRatePerSpill"],"nSpill" ,"trigger Rate (Hz)" );
  plotMe(integratedPlots_["triggerRatePerSpill"]);

  int oldEvents=integratedPlots_["growingEventPlot"]->GetBinContent(iBin-1);
  integratedPlots_["growingEventPlot"]->SetBinContent(iBin,oldEvents+evt_info->GetEntries());
  setAxisTitles(integratedPlots_["growingEventPlot"],"nSpill" ,"n Total Events" );
  plotMe(integratedPlots_["growingEventPlot"]);


  gStyle->SetOptStat ("emr") ;


  integratedPlots_["nTotalEvtsPerSpillHisto"]->Fill(evt_info->GetEntries());
  setAxisTitles(integratedPlots_["nTotalEvtsPerSpillHisto"], "NEvts Per Spill","Entries" );
  plotMe(integratedPlots_["nTotalEvtsPerSpillHisto"]);

  integratedPlots_["triggerRateHisto"]->Fill(100000*evt_info->GetEntries()/(timeEnd_[0]-timeStart_[0]));//it's in Hz
  //  integratedPlots_["triggerRateHisto"]->SetBinError(iBin,evt_info->GetRMS());
  setAxisTitles(integratedPlots_["triggerRateHisto"], "trigger Rate (Hz)","Entries" );
  plotMe(integratedPlots_["triggerRateHisto"]);


  

  }
  integratedFile_->cd();
  
  for(std::map<TString,TH1F*>::const_iterator out=integratedPlots_.begin();out!=integratedPlots_.end();++out)
    out->second->Write(out->first,TObject::kOverwrite);

  integratedFile_->Close();
}


void plotterTools::set_palette_fancy (){
  const Int_t NRGBs = 5;
  const Int_t NCont = 255;
  Float_t r[5];
  Float_t g[5];
  Float_t b[5];
  TColor* c[5];
  int Colors[5]={kBlack,kBlue+2,kBlue-3,kBlue-4,kWhite};
  for(int i=0;i<5;++i){
    c[i]=gROOT->GetColor(Colors[i]);
    c[i]->GetRGB(r[i],g[i],b[i]);
  }
  // Double_t stops[NRGBs] = { 0.00, 0.40, 0.50, 0.80, 1.00}; original
  Double_t stops[NRGBs] = { 0.00, 0.15, 0.50, 0.80, 1.00};
  Double_t red[NRGBs] = {r[4],r[3],r[2],r[1],r[0] };
  Double_t green[NRGBs] = {g[4],g[3],g[2],g[1],g[0] };
  Double_t blue[NRGBs] = {b[4],b[3],b[2],b[1],b[0] };
  TColor::CreateGradientColorTable(NRGBs, stops, red, green, blue, NCont);
  gStyle->SetNumberContours(NCont);



}


void plotterTools::set_plot_blue ()
{
    Double_t Red[3]    = { 0.00, 0.00, 0.00};
    Double_t Green[3]  = { 0.00, 0.00, 0.00};
    Double_t Blue[3]   = { 0.00, 0.00, 1.00};
    Double_t Length[3] = { 0.00, 0.00, 1.00};
    Int_t nb=50;
    TColor::CreateGradientColorTable(3,Length,Red,Green,Blue,nb);
    gStyle->SetNumberContours(nb);
}


// ---- ---- ---- ---- ---- ---- ---- ---- ---- ----


void  plotterTools::setPlotsFormat ()
{
    // general root settings
    gROOT->SetStyle ("Plain") ;
    //    gStyle->SetOptStat ("emruo") ;
    gStyle->SetOptStat ("emr") ;
    gStyle->SetOptFit (0001) ;
    gStyle->SetCanvasDefH (300) ; //Height of canvas
    gStyle->SetCanvasDefW (300) ; //Width of canvas
    
    // fonts of titles and labels
    gStyle->SetTitleBorderSize (0) ;
    gStyle->SetTitleX (0.08) ;
    gStyle->SetTitleY (0.97) ;
    gStyle->SetPalette (1,0) ;
    gStyle->SetLabelFont (42,"xyz") ;
    gStyle->SetTextFont (42) ;
    gStyle->SetStatFont (42) ;
    gStyle->SetTitleFont (42,"xyz") ;
    //gStyle->SetPadGridX (1) ;
    //gStyle->SetPadGridY (1) ;

    // positioning
    gStyle->SetTitleXOffset (1.25) ;
    gStyle->SetTitleYOffset (1.4) ;

    gStyle->SetTitleX (0.5) ; // put title box in the middle
    gStyle->SetTitleFont (42) ;
    gStyle->SetTitleAlign (23) ; // center title text in title box
    gStyle->SetTitleSize (0.04, "xyz") ;
    gStyle->SetLabelSize (0.05, "xyz") ;

    // Margins:



    gStyle->SetPadRightMargin (0.05)  ;
    gStyle->SetPadLeftMargin (0.15) ;
    gStyle->SetPadBottomMargin (0.15) ;
    gStyle->SetPadTopMargin (0.1) ;

    // ticks and divisions on the axes
    gStyle->SetPadTickX (1) ;
    gStyle->SetPadTickY (1) ;
    gStyle->SetNdivisions(7, "xyz");

    // frame drawing options
    gStyle->SetLineWidth (2) ;
    gStyle->SetFillStyle (0) ;
    gStyle->SetStatStyle (0) ;
    
    // histogram default drawing options
    gStyle->SetHistFillColor (kOrange) ;
    gStyle->SetHistLineColor (kBlack) ;
    gStyle->SetHistLineStyle (1) ;
    gStyle->SetHistLineWidth (2) ;
    
    // stats box position
    gStyle->SetStatX (0.95) ;
    gStyle->SetStatY (0.9) ;
    gStyle->SetStatW (0.2) ;
    gStyle->SetStatH (0.15) ;

    //    set_plot_blue () ;
    set_palette_fancy () ;
}


// ---- ---- ---- ---- ---- ---- ---- ---- ---- ----


void  plotterTools::plotMe (TH1F * histo)
{
  TString hname = histo->GetName () ;
  TString  canvasName =outputDir_+ "/"+ hname + ".png" ;

  TCanvas*  c1 = new TCanvas ("c1", "c1", 800, 800) ;
  histo->Draw () ;
  c1->Print (canvasName, "png") ;


  delete c1 ;
  return ;
}


// ---- ---- ---- ---- ---- ---- ---- ---- ---- ----


void  plotterTools::plotMe (TH2F * histo, bool makeProfile)
{

  gStyle->SetPadTopMargin(0.05);
  gStyle->SetPadBottomMargin(0.13);//0.13);
  gStyle->SetPadLeftMargin(0.17);//0.16);
  gStyle->SetPadRightMargin(0.13);//0.02);
  
  gStyle->SetStatX (0.85) ;
  gStyle->SetStatY (0.45) ;
  gStyle->SetStatW (0.2) ;
  gStyle->SetStatH (0.15) ;

  TString hname = histo->GetName () ;
  TString  canvasName = outputDir_+ "/"+hname + ".png" ;
  TCanvas*  c1 = new TCanvas ("c1", "c1", 800, 800) ;

  if (!makeProfile)
    {
       histo->Draw ("colz") ;
       c1->Print (canvasName, "png") ;
    }
  else
    {

      TProfile * dummy = histo->ProfileX () ;
      dummy->SetMarkerColor (2) ;
      dummy->SetMarkerStyle (5) ;
      float Xmin = dummy->GetBinLowEdge (1) ;
      float Xmax = dummy->GetBinLowEdge (dummy->GetNbinsX ()) + dummy->GetBinWidth (1) ;
      float Ymin = getMinimumP (dummy) ; 
      float Ymax = getMaximumP (dummy) ; 
      float delta = Ymax - Ymin ;
      Ymin -= 0.1 * delta ;
      Ymax += 0.1 * delta ;
      
      TH1F * hdummy = c1->DrawFrame (Xmin, Ymin, Xmax, Ymax) ;
      histo->Draw ("colz same") ;
      dummy->Draw ("same") ;
      c1->Print (canvasName, "png") ;
      delete hdummy ;
    }
  delete c1 ;
  return ;

  setPlotsFormat();

}


// ---- ---- ---- ---- ---- ---- ---- ---- ---- ----


void  plotterTools::plotMe (TGraph * graph, const TString & name)
{
  TString  canvasName = outputDir_+ "/"+name + ".png" ;
  TCanvas*  c1 = new TCanvas ("c1", "c1", 800, 800) ;
  graph->SetMarkerStyle (8) ;
  graph->SetMarkerSize (1.5) ;
  graph->SetMarkerColor (kBlue) ;  
  graph->Draw ("ALP") ;
  c1->Print (canvasName, "png") ;
  delete c1 ;
  return ;
}


// ---- ---- ---- ---- ---- ---- ---- ---- ---- ----


void  plotterTools::setAxisTitles (TH1 * histo, const TString  xTitle, const TString  yTitle) 
{
  histo->GetXaxis ()->SetTitle (xTitle) ;
  histo->GetYaxis ()->SetTitle (yTitle) ;
  return ;
}

// ---- ---- ---- ---- ---- ---- ---- ---- ---- ----

void  plotterTools::setAxisTitles (TH2 * histo, const TString  xTitle, const TString  yTitle) 
{
  histo->GetXaxis ()->SetTitle (xTitle) ;
  histo->GetYaxis ()->SetTitle (yTitle) ;
  return ;
}

// ---- ---- ---- ---- ---- ---- ---- ---- ---- ----

void  plotterTools::setAxisTitles (TGraph * histo, const TString  xTitle, const TString  yTitle) 
{
  histo->GetXaxis ()->SetTitle (xTitle) ;
  histo->GetYaxis ()->SetTitle (yTitle) ;
  return ;
}

// ---- ---- ---- ---- ---- ---- ---- ---- ---- ----

void plotterTools::initVariable(TString name,int varDim){
  variablesIterator_[name]=variables_.size();
  variables_.resize(variables_.size()+1);
  variablesMap_[name]=&variables_[variables_.size()];
  variablesContainer_[variablesIterator_[name]].resize(varDim);
}

// ---- ---- ---- ---- ---- ---- ---- ---- ---- ----

void plotterTools::computeVariable(TString name, int varDim){
  //TODO use same structure for multi and unidimensional variables
  if(varDim != 1) {
    variablesContainer_[variablesIterator_[name]].resize(varDim);
    variables_[variablesIterator_[name]]=variablesContainer_[variablesIterator_[name]][0];
  }
 
 if(name=="triggerEff"){
    variables_[variablesIterator_[name]]=((float)treeStruct_.scalerWord[2]/treeStruct_.scalerWord[1]);
    //    *(variablesMap_.find(name)->second)=((float)treeStruct_.scalerWord[2]/treeStruct_.scalerWord[1]);
 }else if(name=="nEvts"){
    variables_[variablesIterator_[name]]=((float)treeStruct_.evtNumber);
  }else if(name=="nTrigSPS"){
    variables_[variablesIterator_[name]]=((float)treeStruct_.scalerWord[1]);
  }else if(name=="nTrigSPSVsnTrig"){
    variablesContainer_[variablesIterator_[name]][0]=((float)treeStruct_.scalerWord[1]);
    variablesContainer_[variablesIterator_[name]][1]=((float)treeStruct_.scalerWord[2]);
  }else if(name=="nTrigSPSVsnTrig3D"){
    variablesContainer_[variablesIterator_[name]][0]=((float)treeStruct_.scalerWord[1]);
    variablesContainer_[variablesIterator_[name]][1]=((float)treeStruct_.scalerWord[2]);
    variablesContainer_[variablesIterator_[name]][2]=((float)treeStruct_.scalerWord[0]);
 }else if(name=="nFibersOnX1"){




   int fibersOn=0;
   for(int i=0;i<64;i++){
     if(fibersOn_[hodoX1][i]==1) fibersOn++;
   }
   variables_[variablesIterator_[name]]=fibersOn;

 }else if(name=="nFibersOnY1"){

   int fibersOn=0;
   for(int i=0;i<64;i++){
     if(fibersOn_[hodoY1][i]==1) fibersOn++;
   }
   variables_[variablesIterator_[name]]=fibersOn;

 }else if(name=="nFibersOnX2"){

   int fibersOn=0;
   for(int i=0;i<64;i++){
     if(fibersOn_[hodoX2][i]==1) fibersOn++;
   }
   variables_[variablesIterator_[name]]=fibersOn;

 }else if(name=="nFibersOnY2"){

   int fibersOn=0;
   for(int i=0;i<64;i++){
     if(fibersOn_[hodoY2][i]==1) fibersOn++;
   }
   variables_[variablesIterator_[name]]=fibersOn;

 }else if(name=="beamProfileX1"){

   for(int i=0;i<64;i++){
     variablesContainer_[variablesIterator_[name]][i]=-1;
     if(fibersOn_[hodoX1][i]==1) variablesContainer_[variablesIterator_[name]][i]=i;
   }

 }else if(name=="beamProfileY1"){

   for(int i=0;i<64;i++){
     variablesContainer_[variablesIterator_[name]][i]=-1;
     if(fibersOn_[hodoY1][i]==1) variablesContainer_[variablesIterator_[name]][i]=i;
   }

 }else if(name=="beamProfileX2"){

   for(int i=0;i<64;i++){
     variablesContainer_[variablesIterator_[name]][i]=-1;
     if(fibersOn_[hodoX2][i]==1) variablesContainer_[variablesIterator_[name]][i]=i;
   }

 }else if(name=="beamProfileY2"){

   for(int i=0;i<64;i++){
     variablesContainer_[variablesIterator_[name]][i]=-1;
     if(fibersOn_[hodoY2][i]==1) variablesContainer_[variablesIterator_[name]][i]=i;
   }

 }else if(name=="beamPositionX1"){

   float pos=0;
   int nFibersOn=0;
   for (int i=0;i<64;++i){   
       if(fibersOn_[hodoX1][i]==1){
	 nFibersOn++;
	 pos+=i; 
       }
     }
   if(nFibersOn>1){
     pos=pos/nFibersOn;
   }else{
     pos=-1;
   }
   variables_[variablesIterator_[name]]=pos;


 }else if(name=="beamPositionY1"){

   float pos=0;
   int nFibersOn=0;
   for (int i=0;i<64;++i){   
       if(fibersOn_[hodoY1][i]==1){
	 nFibersOn++;
	 pos+=i; 
       }
     }
   if(nFibersOn>1){
     pos=pos/nFibersOn;
   }else{
     pos=-1;
   }
   variables_[variablesIterator_[name]]=pos;


 }else if(name=="beamPositionX2"){

   float pos=0;
   int nFibersOn=0;
   for (int i=0;i<64;++i){   
       if(fibersOn_[hodoX2][i]==1){
	 nFibersOn++;
	 pos+=i; 
       }
     }
   if(nFibersOn>1){
     pos=pos/nFibersOn;
   }else{
     pos=-1;
   }
   variables_[variablesIterator_[name]]=pos;


 }else if(name=="beamPositionY2"){

   float pos=0;
   int nFibersOn=0;
   for (int i=0;i<64;++i){   
       if(fibersOn_[hodoY2][i]==1){
	 nFibersOn++;
	 pos+=i; 
       }
     }
   if(nFibersOn>1){
     pos=pos/nFibersOn;
   }else{
     pos=-1;
   }
   variables_[variablesIterator_[name]]=pos;


 }else if(name == "beamProfileSmallX"){//small hodo
   
   for(int i =0 ;i<nFibersSmallHodo;i++){
     variablesContainer_[variablesIterator_[name]][i]=-1;
     if(fibersOnSmall_[hodoSmallX][i]==1){
       variablesContainer_[variablesIterator_[name]][i]=i;
     }
   }
 }else if(name == "beamProfileSmallY"){

   for(int i =0 ;i<nFibersSmallHodo;i++){
     variablesContainer_[variablesIterator_[name]][i]=-1;
     if(fibersOnSmall_[hodoSmallY][i]==1) variablesContainer_[variablesIterator_[name]][i]=i;
   }
 }else if(name=="nFibersOnSmallX"){

   int fibersOn=0;
   for(int i=0;i<nFibersSmallHodo;i++){
     if(fibersOnSmall_[hodoSmallX][i]==1) fibersOn++;
   }
   variables_[variablesIterator_[name]]=fibersOn;

 }else if(name=="nFibersOnSmallY"){

   int fibersOn=0;
   for(int i=0;i<nFibersSmallHodo;i++){
     if(fibersOnSmall_[hodoSmallY][i]==1) fibersOn++;
   }
   variables_[variablesIterator_[name]]=fibersOn;

 }else if(name=="beamPositionSmallX"){

   float pos=0;
   int nFibersOn=0;
   
   for (int i=0;i<8;++i){   
     if(fibersOnSmall_[hodoSmallX][i]==1){
       nFibersOn++;
       pos+=i; 
     }
   }


   if(nFibersOn>1){
     pos=pos/nFibersOn;
   }else{
     pos=-1;
   }
   variables_[variablesIterator_[name]]=pos;

 }else if(name=="beamPositionSmallY"){

   float pos=0;
   int nFibersOn=0;
   
   for (int i=0;i<8;++i){   
     if(fibersOnSmall_[hodoSmallY][i]==1){
       nFibersOn++;
       pos+=i; 
     }
   }


   if(nFibersOn>1){
     pos=pos/nFibersOn;
   }else{
     pos=-1;
   }
   variables_[variablesIterator_[name]]=pos;

 }else if(name=="beamPositionX"){

   float pos=0;
   int nFibersOn=0;
   for(int j=0;j<4;j++){
     if(j!=hodoX1 && j!=hodoX2)continue;
     for (int i=0;i<64;++i){   
       if(fibersOn_[j][i]==1){
	 nFibersOn++;
	 pos+=i; 
       }
     }
   }

   if(nFibersOn>1){
     pos=pos/nFibersOn;
   }else{
     pos=-1;
   }
   variables_[variablesIterator_[name]]=pos;

 }else if(name=="beamPositionY"){

   float pos=0;
   int nFibersOn=0;
   for(int j=0;j<4;j++){
     if(j!=hodoY1 && j!=hodoY2)continue;
     for (int i=0;i<64;++i){   
       if(fibersOn_[j][i]==1){
	 nFibersOn++;
	 pos+=i; 
       }
     }
   }

   if(nFibersOn>1){
     pos=pos/nFibersOn;
   }else{
     pos=-1;
   }
   variables_[variablesIterator_[name]]=pos;

 }else if(name=="fractionTakenTrig"){//DAQ Status
    variables_[variablesIterator_[name]]=((float)treeStruct_.scalerWord[2]/treeStruct_.scalerWord[1]);
 }else if(name=="fractionTakenTrigHisto"){//DAQ Status
    variables_[variablesIterator_[name]]=((float)treeStruct_.scalerWord[2]/treeStruct_.scalerWord[1]);
 }else if(name=="deltaTime10"){
   variables_[variablesIterator_[name]]=((int64_t)treeStruct_.evtTime[1]-(int64_t)treeStruct_.evtTime[0])-((int64_t)timeStart_[1]-(int64_t)timeStart_[0]);

 }else if(name=="deltaTime20"){
   variables_[variablesIterator_[name]]=((int64_t)treeStruct_.evtTime[2]-(int64_t)treeStruct_.evtTime[0])-((int64_t)timeStart_[2]-(int64_t)timeStart_[0]);

 }else if(name=="deltaTime21"){
   variables_[variablesIterator_[name]]=((int64_t)treeStruct_.evtTime[2]-(int64_t)treeStruct_.evtTime[1])-((int64_t)timeStart_[2]-(int64_t)timeStart_[1]);

 }else if(name=="nTotalEvts"){
    variables_[variablesIterator_[name]]=((float)1.);
  }
 else if(name=="TDCinputTime1"){//TDC
   for (uint j=0; j<MaxTdcReadings; j++) variablesContainer_[variablesIterator_[name]][j]=-999;
   for (uint j=0; j<tdc_readings[0].size() && j<MaxTdcReadings; j++) variablesContainer_[variablesIterator_[name]][j]=tdc_readings[0].at(j);
 }
 else if(name=="TDCinputTime2"){
   for (uint j=0; j<MaxTdcReadings; j++) variablesContainer_[variablesIterator_[name]][j]=-999;
   for (uint j=0; j<tdc_readings[1].size() && j<MaxTdcReadings; j++) variablesContainer_[variablesIterator_[name]][j]=tdc_readings[1].at(j);
 }
 else if(name=="TDCinputTime3"){
   for (uint j=0; j<MaxTdcReadings; j++) variablesContainer_[variablesIterator_[name]][j]=-999;
   for (uint j=0; j<tdc_readings[2].size() && j<MaxTdcReadings; j++) variablesContainer_[variablesIterator_[name]][j]=tdc_readings[2].at(j);
 }
 else if(name=="TDCinputTime4"){
   for (uint j=0; j<MaxTdcReadings; j++) variablesContainer_[variablesIterator_[name]][j]=-999;
   for (uint j=0; j<tdc_readings[3].size() && j<MaxTdcReadings; j++) variablesContainer_[variablesIterator_[name]][j]=tdc_readings[3].at(j);
 }
 else if(name=="TDCtimeDiffX"){
   for (uint j=0; j<MaxTdcReadings; j++) variablesContainer_[variablesIterator_[name]][j]=-999;
   for (uint j=0; j<tdc_readings[0].size() && j<tdc_readings[2].size() && j<MaxTdcReadings; j++) variablesContainer_[variablesIterator_[name]][j]=tdc_readings[0].at(j)-tdc_readings[2].at(j); // CHECK WHO IS X, Y
 }
 else if(name=="TDCtimeDiffY"){
   for (uint j=0; j<MaxTdcReadings; j++) variablesContainer_[variablesIterator_[name]][j]=-999;
   for (uint j=0; j<tdc_readings[1].size() && j<tdc_readings[3].size()&& j<MaxTdcReadings; j++) variablesContainer_[variablesIterator_[name]][j]=tdc_readings[1].at(j)-tdc_readings[3].at(j); // CHECK WHO IS X, Y
 }
 else if(name=="TDCrecoX"){ // WE NEED THE WIRE CHAMBER CALIBRATION TO FILL THESE
   //   variablesContainer_[variablesIterator_[name]][???]=0
 }
 else if(name=="TDCrecoY"){
   //   variablesContainer_[variablesIterator_[name]][???]=0
 }
 else if(name=="TDChistoryRecoX"){
   //   variablesContainer_[variablesIterator_[name]][???]
 }
 else if(name=="TDChistoryRecoY"){
   //   variablesContainer_[variablesIterator_[name]][???]
 }
 else if(name=="TDChistoryRMSX"){
   //   variablesContainer_[variablesIterator_[name]][???]
 }
 else if(name=="TDChistoryRMSY"){
   //   variablesContainer_[variablesIterator_[name]][???]
 }


 {
   std::map<TString,UInt_t*>::const_iterator it = adc_channelnames.find(name);
   if (it!=adc_channelnames.end()){
     variables_[variablesIterator_[name]]=*(it->second);
   }
 }

}


void plotterTools::fillObjects(){

  fillHodo();
  fillTdc();

  //if you want to make a test
  //   fibersOn_[0][10]=1;
  //   fibersOn_[0][11]=1;
  //   fibersOn_[0][12]=1;

  //   fibersOn_[1][20]=1;
  //   fibersOn_[1][21]=1;
  //   fibersOn_[1][22]=1;

  //   fibersOn_[2][10]=1;
  //   fibersOn_[2][11]=1;
  //   fibersOn_[2][12]=1;

  //   fibersOn_[3][20]=1;
  //   fibersOn_[3][21]=1;
  //   fibersOn_[3][22]=1;


  // fibersOnSmall_[0][4]=1;
  // fibersOnSmall_[0][5]=1;
  // fibersOnSmall_[0][6]=1;

  // fibersOnSmall_[1][2]=1;
  // fibersOnSmall_[1][3]=1;
  // fibersOnSmall_[1][4]=1;


}

void plotterTools::fillHodo(){


   for(int i=0;i<nPlanesHodo;++i){
     for(int j=0;j<nFibersHodo;++j){
       fibersOn_[i][j]=0;
     }
   }

   for(int i =0 ; i <nPlanesSmallHodo;++i){
     for(int j=0; j<nFibersSmallHodo;j++){
       fibersOnSmall_[i][j]=0;
     }
   }


   for(uint i=0;i<treeStruct_.nPatterns;++i){

     if(treeStruct_.patternBoard[i]==0x08030001 || treeStruct_.patternBoard[i]==0x08030002){
       int planecouple = (treeStruct_.patternBoard[i]==0x08030001) ? 0 : 1;
       for (unsigned int j=0; j<32; j++){
	 std::vector<int> *fiberorder =(bool)( treeStruct_.patternChannel[i]&0b1) ? &fiberOrderB : &fiberOrderA;
	 bool thisfibon = (treeStruct_.pattern[i]>>j)&0b1;
	 fibersOn_[2*planecouple+treeStruct_.patternChannel[i]/2][fiberorder->at(j)-1]=thisfibon;
       }
     }else if(treeStruct_.patternBoard[i]==0x08010001){
     
     if(treeStruct_.patternChannel[i]!=1)continue;
       WORD wordX=(treeStruct_.pattern[i]& 0x0000FF00)>>8;
       WORD wordY= (treeStruct_.pattern[i] & 0x000000FF);

       for(int j=0;j<8;j++){
	 fibersOnSmall_[0][j]=(bool)((wordX>>j)&0b1);
	 fibersOnSmall_[1][j]=(bool)((wordY>>j)&0b1);
	 //	 std::cout<<fibersOnSmall_[0][i]<<" "<<fibersOnSmall_[1][i]<<"----";
       }
     }

   
   }

   
//   for (int i=0; i<4; i++) {
//     std::cout << "this is " << i << std::endl;
//     for (int j=0; j<64; j++){
//       std::cout << j+1 << "-" << fibersOn_[i][j] << " ";
//     }
//     std::cout << std::endl;
//   }



   //   for(int i=0;i<8;i++)	 std::cout<<fibersOnSmall_[0][i]<<" "<<fibersOnSmall_[1][i]<<"----";
}


void plotterTools::fillTdc(){

  for (uint j=0; j<MaxTdcChannels; j++){
    tdc_readings[j].clear();
  }

  for (uint i=0; i<treeStruct_.nTdcChannels; i++){
    if (treeStruct_.tdcBoard[i]==0x07030001 && treeStruct_.tdcChannel[i]<MaxTdcChannels){
      tdc_readings[treeStruct_.tdcChannel[i]].push_back((float)treeStruct_.tdcData[i]);
    }
  }

}

void plotterTools::initAdcChannelNames(int nBinsHistory){

  adc_channelnames.clear();

  for (UInt_t i=0; i<treeStruct_.nAdcChannels && i<MAX_ADC_CHANNELS; i++){
    TString name("ADC_board_");
    for (Int_t j=3; j>=0; j--){
      UInt_t field = ((treeStruct_.adcBoard[i])>>(8*j))&(0x000000FF);
      name+=field;
    }
    name+='_';
    name+=treeStruct_.adcChannel[i];
    adc_channelnames.insert(std::make_pair<TString,UInt_t*>(name,&(treeStruct_.adcData[i])));
    addPlot(name.Data(),4096,0,4096,"1D",group_,module_);
    name.Append("_hist");
    adc_channelnames.insert(std::make_pair<TString,UInt_t*>(name,&(treeStruct_.adcData[i])));
    addPlot(name.Data(), nBinsHistory, "history", group_,module_);
  }

}


template <class T>
set<int>
plotterTools::listElements (T * array, int Nmax)
{
  set<int> elements ;
  for (int i = 0 ; i < Nmax ; ++i)
    {
      if (elements.find (array[i]) != elements.end ()) continue ;
      elements.insert (array[i]) ;
    }
  return elements ;
}


void plotterTools::initDigiPlots(){

  std::set<int> channels = listElements (treeStruct_.digiChannel,  treeStruct_.nDigiSamples) ;
  std::set<int> groups = listElements (treeStruct_.digiGroup,  treeStruct_.nDigiSamples) ;
  
  int xNbins = 1024 ;
  float xmin = 0 ;
  float xmax = 1024 ;

  int yNbins = 100 ;
  float ymin = (*std::min_element(treeStruct_.digiSampleValue,treeStruct_.digiSampleValue+treeStruct_.nDigiSamples));
  float ymax = (*std::max_element(treeStruct_.digiSampleValue,treeStruct_.digiSampleValue+treeStruct_.nDigiSamples));
  ymin -= 0.1*fabs(ymin);
  ymax += 0.1*fabs(ymax);

  for (set<int>::iterator iGroup = groups.begin () ; 
       iGroup != groups.end () ; ++iGroup)
    {
      for (set<int>::iterator iChannel = channels.begin () ; 
           iChannel != channels.end () ; ++iChannel)
        {
          TString name = "digiPersPlot_gr" ;
          name += *iGroup ;
          name += "_ch" ;
          name += *iChannel ;
          TH2F * dummy = addPlot (name, xNbins, xmin, xmax, yNbins, ymin, ymax, 
				  "time", "voltage",
				  "2D", group_, module_, 1, true) ;
          digi_histos[10 * (*iGroup) + (*iChannel)] = dummy ;
        }
    }
  
}




// ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
void  plotterTools::readInputTree ()
{
  //Instantiate the tree branches
  //  inputTree_->Print();

  inputTree_->SetBranchAddress("evtNumber"    ,&treeStruct_.evtNumber);
  inputTree_->SetBranchAddress("evtTimeDist"    ,&treeStruct_.evtTimeDist);
  inputTree_->SetBranchAddress("evtTimeStart"    ,&treeStruct_.evtTimeStart);

  inputTree_->SetBranchAddress("nEvtTimes"    ,&treeStruct_.nEvtTimes);
  inputTree_->SetBranchAddress("evtTime"    ,&treeStruct_.evtTime);
  inputTree_->SetBranchAddress("evtTimeBoard"    ,&treeStruct_.evtTimeBoard);

  inputTree_->SetBranchAddress("nAdcChannels"    ,&treeStruct_.nAdcChannels);
  inputTree_->SetBranchAddress("adcBoard"    ,treeStruct_.adcBoard);
  inputTree_->SetBranchAddress("adcChannel"    ,treeStruct_.adcChannel);
  inputTree_->SetBranchAddress("adcData"    ,treeStruct_.adcData);

  inputTree_->SetBranchAddress("nTdcChannels"    ,&treeStruct_.nTdcChannels);
  inputTree_->SetBranchAddress("tdcBoard"    ,treeStruct_.tdcBoard);
  inputTree_->SetBranchAddress("tdcChannel"    ,treeStruct_.tdcChannel);
  inputTree_->SetBranchAddress("tdcData"    ,treeStruct_.tdcData);

  inputTree_->SetBranchAddress("nDigiSamples"    ,&treeStruct_.nDigiSamples);
  inputTree_->SetBranchAddress("digiGroup"    ,treeStruct_.digiGroup);
  inputTree_->SetBranchAddress("digiChannel"    ,treeStruct_.digiChannel);
  inputTree_->SetBranchAddress("digiSampleIndex",treeStruct_.digiSampleIndex);
  inputTree_->SetBranchAddress("digiSampleValue",treeStruct_.digiSampleValue);
  inputTree_->SetBranchAddress("digiBoard"    ,treeStruct_.digiBoard);

  inputTree_->SetBranchAddress("nScalerWords"    ,&treeStruct_.nScalerWords);
  inputTree_->SetBranchAddress("scalerWord"    ,treeStruct_.scalerWord);
  inputTree_->SetBranchAddress("scalerBoard"    ,treeStruct_.scalerBoard);

  inputTree_->SetBranchAddress("nPatterns"    ,&treeStruct_.nPatterns);
  inputTree_->SetBranchAddress("pattern"    ,treeStruct_.pattern);
  inputTree_->SetBranchAddress("patternBoard"    ,treeStruct_.patternBoard);
  inputTree_->SetBranchAddress("patternChannel"    ,treeStruct_.patternChannel);

  inputTree_->SetBranchAddress("nTriggerWords"    ,&treeStruct_.nTriggerWords);
  inputTree_->SetBranchAddress("triggerWords"    ,treeStruct_.triggerWords);
  inputTree_->SetBranchAddress("triggerWordsBoard",treeStruct_.triggerWordsBoard);


  return ;
} 

void plotterTools::bookPlotsADC(){
  wantADCplots=true;
}

void plotterTools::bookPlotsDigitizer(){
  wantDigiplots=true;
}

void  plotterTools::Loop()
{


  uint nentries = getTreeEntries();
  int nBinsHistory=nentries/getStepHistoryPlots();

  //  nentries=1;

  //loop and fill histos
  for (unsigned iEntry = 0 ; iEntry < nentries ; ++iEntry) 
    {
      inputTree_->GetEntry(iEntry);

      if (iEntry%1000==0) std::cout<<"iEntry: "<<iEntry<<"/"<<nentries<<endl;

      if(iEntry==0){
	for(int i =0;i<treeStruct_.nEvtTimes;++i)
	  timeStart_[i]=treeStruct_.evtTime[i];
      }
      if(iEntry==0 && wantADCplots) initAdcChannelNames(nBinsHistory);
      if(iEntry==0 && wantDigiplots) initDigiPlots();
      if(iEntry==(nentries -1)){
	for(int i =0;i<treeStruct_.nEvtTimes;++i)
	  timeEnd_[i]=treeStruct_.evtTime[i];
      }

      fillObjects();

      if (wantDigiplots){

	for (int iSample = 0 ; iSample < treeStruct_.nDigiSamples ; ++iSample)
	  {
	    digi_histos[10 * treeStruct_.digiGroup[iSample] + treeStruct_.digiChannel[iSample]]->Fill (treeStruct_.digiSampleIndex[iSample], treeStruct_.digiSampleValue[iSample]) ;
	  }
      }
    
  
      for (std::map<TString,float*>::const_iterator iter = variablesMap_.begin ();
           iter != variablesMap_.end () ; ++iter)
        {
          if(plotLongNames_[iter->first].Contains("1D"))
            {

              FillPlot (iter->first, false, variablesContainer_[variablesIterator_[iter->first]].size ()) ;
            } else if (plotLongNames_[iter->first].Contains ("2D"))
            {
              FillPlot (iter->first, true);
            }
           
          if (iEntry%historyStep_==0 && iEntry!=0)
            {
              if ( (int)iEntry/historyStep_-1 < nBinsHistory) //all history plots should go here
                {
                  if (plotLongNames_[iter->first].Contains("history"))
                    {
                      FillPlot(iter->first,(int)iEntry/historyStep_-1,iEntry);
                    }
                }
            }
        }


    } // loop over the events


}




//for TGraph
void plotterTools::FillPlot(TString name, int point, float X)
{
  computeVariable (name) ;
  if (!(vetoFillObjects[name]))
    ((TGraph*) outObjects_[plotLongNames_[name]])->SetPoint(point, X, variables_[variablesIterator_[name]]) ;
}

//for TH1F and TH2F
void plotterTools::FillPlot(TString name, bool is2D, int varDim){

  if(!is2D){
    computeVariable(name,varDim);  
    if(!(varDim>1)){
      if (!(vetoFillObjects[name]))
	((TH1F*) outObjects_[plotLongNames_[name]])->Fill(variables_[variablesIterator_[name]]);
    }else{
      for(int i=0;i<varDim;i++){
      if (!(vetoFillObjects[name]))
	  ((TH1F*) outObjects_[plotLongNames_[name]])->Fill(variablesContainer_[variablesIterator_[name]][i]);
      }
    }
  }else {
      computeVariable(name,2);
      if (!(vetoFillObjects[name]))
	((TH2F*) outObjects_[plotLongNames_[name]])->Fill(variablesContainer_[variablesIterator_[name]][0],variablesContainer_[variablesIterator_[name]][1]);
  }
  
}


void plotterTools::bookPlotsScaler(int nBinsHistory){
  //in this function you define all the objects for the scaler
  addPlot("nEvts", nBinsHistory, "history", group_,module_);//simple TGraph
  addPlot("triggerEff",nBinsHistory, "history", group_,module_);//TGraph with more complex variable
  addPlot("nTrigSPS", 100,0,3000, "1D",group_,module_);//simple TH1F
  addPlot("nTrigSPSVsnTrig", 100,0,3000, 100,0,3000,"nTrigSPS","nTrig","2D",group_,module_);//simple TH2F
  addPlot("nTrigSPSVsnTrig3D", 100,0,3000, "1D",group_,module_,3);// TH1F with more than one variable to fill per event
}

void plotterTools::bookPlotsHodo(int nBinsHistory){

  addPlot("beamProfileX1", 64,-0.5, 63.5,"1D",group_,module_,64);//simple TH1F
  addPlot("beamProfileY1", 64,-0.5, 63.5,"1D",group_,module_,64);//simple TH1F
  addPlot("beamProfileX2", 64,-0.5, 63.5,"1D",group_,module_,64);//simple TH1F
  addPlot("beamProfileY2", 64,-0.5, 63.5,"1D",group_,module_,64);//simple TH1F

  addPlot("nFibersOnX1", 64,-0.5, 63.5,"1D",group_,module_);//simple TH1F
  addPlot("nFibersOnY1", 64,-0.5, 63.5,"1D",group_,module_);//simple TH1F
  addPlot("nFibersOnX2", 64,-0.5, 63.5,"1D",group_,module_);//simple TH1F
  addPlot("nFibersOnY2", 64,-0.5, 63.5,"1D",group_,module_);//simple TH1F

  addPlot("beamPositionX1", 64,-0.5, 63.5,"1D",group_,module_);//simple TH1F
  addPlot("beamPositionX2", 64,-0.5, 63.5,"1D",group_,module_);//simple TH1F
  addPlot("beamPositionY1", 64,-0.5, 63.5,"1D",group_,module_);//simple TH1F
  addPlot("beamPositionY2", 64,-0.5, 63.5,"1D",group_,module_);//simple TH1F

  addPlot("beamPositionX", 64,-0.5, 63.5,"1D",group_,module_);//simple TH1F
  addPlot("beamPositionY", 64,-0.5, 63.5,"1D",group_,module_);//simple TH1F

}

void plotterTools::bookCombinedPlotsHodo(){

  addPlotCombined("hodoCorrelationX","beamProfileX1","beamProfileX2","2D",group_,module_);//correlation plots it uses TH1F done before to build this TH2
  addPlotCombined("hodoCorrelationY","beamProfileY1","beamProfileY2","2D",group_,module_);//correlation plots it uses TH1F done before to build this TH2

}

void plotterTools::bookPlotsSmallHodo(int nBinsHistory){

  addPlot("beamProfileSmallX", 8,-0.5, 7.5,"1D",group_,module_,8);//simple TH1F
  addPlot("beamProfileSmallY", 8,-0.5, 7.5,"1D",group_,module_,8);//simple TH1F

  addPlot("nFibersOnSmallX", 8,-0.5, 7.5,"1D",group_,module_);//simple TH1F
  addPlot("nFibersOnSmallY", 8,-0.5, 7.5,"1D",group_,module_);//simple TH1F

  addPlot("beamPositionSmallX", 8,-0.5, 7.5,"1D",group_,module_);//simple TH1F
  addPlot("beamPositionSmallY", 8,-0.5, 7.5,"1D",group_,module_);//simple TH1F


}


void plotterTools::bookPlotsDAQStatus(int nBinsHistory){
  addPlot("fractionTakenTrig",nBinsHistory, "history", group_,module_);//TGraph with more complex variable
  addPlot("fractionTakenTrigHisto",100,0,1,"1D",group_,module_);//simple TH1F
  addPlot("nTotalEvts", 1,-0.5, 1.5,"1D",group_,module_);//simple TH1F
  addPlot("deltaTime10", 100,-60.5, 59.5,"1D",group_,module_);//simple TH1F          
  addPlot("deltaTime20", 100,-60.5, 59.5,"1D",group_,module_);//simple TH1F          
  addPlot("deltaTime21", 100,-60.5, 59.5,"1D",group_,module_);//simple TH1F          
 }

void plotterTools::bookPlotsTDC(int nBinsHistory){
  addPlot("TDCinputTime1",100,0,50000,"1D",group_,module_,MaxTdcReadings);
  addPlot("TDCinputTime2",100,0,50000,"1D",group_,module_,MaxTdcReadings);
  addPlot("TDCinputTime3",100,0,50000,"1D",group_,module_,MaxTdcReadings);
  addPlot("TDCinputTime4",100,0,50000,"1D",group_,module_,MaxTdcReadings);
  addPlot("TDCtimeDiffX",100,0,50000,"1D",group_,module_ ,MaxTdcReadings);
  addPlot("TDCtimeDiffY",100,0,50000,"1D",group_,module_,MaxTdcReadings);
  addPlot("TDCrecoX",100,0,1,"1D",group_,module_);
  addPlot("TDCrecoY",100,0,1,"1D",group_,module_);
  addPlot("TDChistoryRecoX",nBinsHistory,"history",group_,module_);
  addPlot("TDChistoryRecoY",nBinsHistory,"history",group_,module_);
  addPlot("TDChistoryRMSX",nBinsHistory,"history",group_,module_);
  addPlot("TDChistoryRMSY",nBinsHistory,"history",group_,module_);
}

void plotterTools::bookCombinedPlots(){
  addPlotCombined("nTrigSPSVsnTrig3DvsnEvts","nTrigSPS","nTrigSPSVsnTrig3D","2D",group_,module_);//correlation plots it uses TH1F done before to build this TH2
//  addPlotCombined("HodoWireCorrelationX","beamProfileX","TDCrecoX","2D",group_,module_); // TO BE ENABLED IF RUNNING ALL REQUIRED 1D PLOTTERS
//  addPlotCombined("HodoWireCorrelationY","beamProfileY","TDCrecoY","2D",group_,module_);

}

void plotterTools::fitHisto(TString name,TString func){

  map<TString, TObject*>::iterator it;
  it=outObjects_.find(plotLongNames_[name]);
  if(it!=outObjects_.end())  ((TH1F*) outObjects_[plotLongNames_[name]])->Fit(func);

}

void plotterTools::addPlotCombined(TString name, TString name1, TString name2,TString type, TString group , TString module){
  initVariable(name);
  TString longName=group+TString("_")+module+TString("_")+type+TString("_")+name;
  outObjects_[longName]=((TObject*) bookHistoCombined(name,name1,name2));
  plotLongNames_[name]=longName;
  plotShortNames_[longName]=name;

  ((TH2F*)outObjects_[longName])->SetTitle(name);
  ((TH2F*)outObjects_[longName])->SetName(plotLongNames_[name]);


  ((TH2F*)outObjects_[longName])->SetXTitle(((TH1F* )outObjects_[plotLongNames_[name1]])->GetTitle());
  ((TH2F*)outObjects_[longName])->SetYTitle(((TH1F* )outObjects_[plotLongNames_[name2]])->GetTitle());

}

void plotterTools::setPlotAxisRange(TString name, TString axis,float min, float max){
  if(plotLongNames_[name].Contains("1D")){
    ((TH1F*) outObjects_[plotLongNames_[name]])->SetAxisRange(min,max,axis);
  }else   if(plotLongNames_[name].Contains("history")){
    if(axis=="X"){
      ((TGraph*) outObjects_[plotLongNames_[name]])->GetXaxis()->SetRangeUser(min,max);
    }else if(axis=="Y"){
      std::cout<<"setting range"<<endl;
      ((TGraph*) outObjects_[plotLongNames_[name]])->GetYaxis()->SetRangeUser(min,max);
    }
  }else if (plotLongNames_[name].Contains("2D")){
    ((TH2F*) outObjects_[plotLongNames_[name]])->SetAxisRange(min,max,axis);
  }
}

//for TGraph
TGraph *
plotterTools::addPlot(TString name,int nPoints,TString type, TString group, TString module, bool vetoFill){

  initVariable(name);

    TString longName=group+TString("_")+module+TString("_")+type+TString("_")+name;
    outObjects_[longName]=((TObject*)  bookGraph(name,nPoints,type, group_,module_));
    if (vetoFill) vetoFillObjects[name]=true;
    else vetoFillObjects[name]=false;
    plotLongNames_[name]=longName;
    plotShortNames_[longName]=name;
    return dynamic_cast<TGraph *> (outObjects_[longName]) ;
}

//for TH1F
TH1F * plotterTools::addPlot(TString name,int nBinsX, float xMin, float xMax, TString type, TString group, TString module, int varDim, bool vetoFill){
  initVariable(name,varDim);

   TString longName=group+TString("_")+module+TString("_")+type+TString("_")+name;
   outObjects_[longName]=((TObject*) bookHisto(name,nBinsX, xMin, xMax, type, group_,module_));
   if (vetoFill) vetoFillObjects[name]=true;
   else vetoFillObjects[name]=false;
   plotLongNames_[name]=longName;
   plotShortNames_[longName]=name;
   return dynamic_cast<TH1F *> (outObjects_[longName]) ;


}

//for TH2F
TH2F * plotterTools::addPlot(TString name,int nBinsX, float xMin, float xMax, int nBinsY, float yMin, float yMax, 
                             TString xTitle, TString yTitle,TString type, TString group, TString module, bool addProfile, bool vetoFill)
{
  initVariable(name);

   TString longName=group+TString("_")+module+TString("_")+type+TString("_")+name;
   outObjects_[longName]=((TObject*) bookHisto2D(name,nBinsX, xMin, xMax,nBinsY,yMin, yMax,xTitle,yTitle, type, group_,module_));
   if (vetoFill) vetoFillObjects[name]=true;
   else vetoFillObjects[name]=false;
   plotLongNames_[name]=longName;
   plotShortNames_[longName]=name;
   makeProfile_[longName] = addProfile ;
   return dynamic_cast<TH2F *> (outObjects_[longName]) ;

}


//TH1F
TH1F* plotterTools::bookHisto(TString name,int nBinsX,float xMin, float xMax, TString type, TString group, TString module){
  TString longName=group+TString("_")+module+TString("_")+type+TString("_")+name;
  TH1F* histo = new TH1F (name, longName , nBinsX, xMin, xMax);
  histo->SetTitle(name);
  histo->SetName(longName);

  return histo;

}

//TH2F
TH2F* plotterTools::bookHisto2D(TString name,int nBinsX,float xMin, float xMax,int nBinsY, float yMin, float yMax,TString xTitle, TString yTitle, TString type, TString group, TString module){
  TString longName=group+TString("_")+module+TString("_")+type+TString("_")+name;
  TH2F* histo = new TH2F (name, longName , nBinsX, xMin, xMax, nBinsY, yMin, yMax);
  histo->SetTitle(name);
  histo->SetName(longName);
  histo->SetXTitle(xTitle);
  histo->SetYTitle(yTitle);
  return histo;

}

//combined plots
TH2F* plotterTools::bookHistoCombined(TString name,TString name1, TString name2){

  int  nBinsX=((TH1F* )outObjects_[plotLongNames_[name1]])->GetNbinsX();
  float xMin=((TH1F* )outObjects_[plotLongNames_[name1]])->GetXaxis()->GetBinLowEdge(1);
  float xMax=((TH1F* )outObjects_[plotLongNames_[name1]])->GetXaxis()->GetBinLowEdge(nBinsX)+((TH1F* )outObjects_[plotLongNames_[name1]])->GetXaxis()->GetBinWidth(nBinsX);
                                        
  int nBinsY=((TH1F* )outObjects_[plotLongNames_[name2]])->GetNbinsX();
  float yMin=((TH1F* )outObjects_[plotLongNames_[name2]])->GetXaxis()->GetBinLowEdge(1);
  float yMax=((TH1F* )outObjects_[plotLongNames_[name2]])->GetXaxis()->GetBinLowEdge(nBinsY)+((TH1F* )outObjects_[plotLongNames_[name2]])->GetXaxis()->GetBinWidth(nBinsY);

  TH2F* histo = new TH2F (name, plotLongNames_[name] ,nBinsX, xMin,xMax, nBinsY, yMin, yMax);
  for(int i =1;i<=nBinsX;i++){
    for(int j =1;j<=nBinsY;j++){
      float X=((TH1F* )outObjects_[plotLongNames_[name1]])->GetBinContent(i);
      float Y=((TH1F* )outObjects_[plotLongNames_[name2]])->GetBinContent(j);
      histo->SetBinContent(i,j,X*Y);
    }
  }



  return histo;

}

//TGraph
TGraph* plotterTools::bookGraph(TString name,int nPoints,TString type, TString group, TString module){

  TGraph* graph=new TGraph (nPoints);
  

  graph->SetTitle(name);
  graph->GetXaxis()->SetTitle("Event");

  graph->SetName(group+TString("_")+module+TString("_")+type+TString("_")+TString(graph->GetTitle()));

  return graph;

}

void plotterTools::setModule(TString module){
  module_=module;
}

void plotterTools::setGroup(TString group){
  group_=group;
}

void plotterTools::setStepHistoryPlots(int n){
  historyStep_=n;
}

int plotterTools::getTreeEntries(){
  return  inputTree_->GetEntries();
}

int plotterTools::getStepHistoryPlots(){
  return  historyStep_;
}




void plotterTools::saveHistos(){
  if(VERBOSE){  std::cout << "==================== Saving histograms =======================" << std::endl;
    std::cout << "outputFile " << outputFile_->GetName() << " opened" << std::endl;}
  outputFile_->cd();
  for (std::map<TString,TObject*>::const_iterator out=outObjects_.begin();out!=outObjects_.end();++out)
    out->second->Write(out->first);
  outputFile_->Close();
  if(VERBOSE){  std::cout << "outputFile " << outputFile_->GetName() << " closed" << std::endl;
  std::cout << "==================== DQM analysis is done =======================" << std::endl;
  }
}


pair<int, string> plotterTools::execute (const string & command) 
{
    FILE *in;
    char buff[512] ;
    if (!(in = popen (command.c_str (), "r")))
      {
        return pair<int, string> (-99, "") ;
      }

    std::string result, tempo ;
    while (fgets (buff, sizeof (buff), in)!=NULL)
      {
        tempo = buff ;
//        result += tempo.substr (0, tempo.size () - 1) ;
        result += tempo ;
      }
    int returnCode = pclose (in) ;
 
    return pair<int, string> (returnCode, result) ;
}


// ---- ---- ---- ---- ---- ---- ---- ---- ---- ----


void plotterTools::plotHistos(){
  if(VERBOSE)  std::cout << "==================== Plotting histograms =======================" << std::endl;
  pair <int, string> outCode = execute ("ls " + string(outputDir_)) ;
  if (outCode.first != 0) outCode = execute ("mkdir " + string(outputDir_)) ;
  if (outCode.first != 0) 
    {
      cerr << "[PLOTTER] ERROR FILE " << outputFile_ 
           << ", problems creating the output folder:\n"
           << outCode.second << "\n"
           << "exiting\n" ;
      exit (1) ;
    }



  for (std::map<TString,TObject*>::const_iterator out=outObjects_.begin();out!=outObjects_.end();++out){
    if(out->first.Contains("history"))  {
      setAxisTitles((TGraph*)out->second,"Event",plotShortNames_[out->first]);
      plotMe((TGraph*)out->second, out->first);
    }else if(out->first.Contains("1D"))  {
      setAxisTitles((TH1F*)out->second,plotShortNames_[out->first],"Events");
      plotMe((TH1F*)out->second);
    }else if(out->first.Contains("2D"))  {
      setAxisTitles((TH2F*)out->second,((TAxis*)((TH2F*)out->second)->GetXaxis())->GetTitle(),((TAxis*)((TH2F*)out->second)->GetYaxis())->GetTitle());
      plotMe((TH2F*)out->second, makeProfile_[out->first]);
    }
  if(VERBOSE)   std::cout << "==================== Canvas saved in \" "<< outputDir_<<"\" =======================" << std::endl;
}
}

void plotterTools::printHistos(){
  if(VERBOSE){
    std::cout << "==================== Booked histograms =======================" << std::endl;
    for (std::map<TString,TObject*>::const_iterator out=outObjects_.begin();out!=outObjects_.end();++out)
      std::cout << out->second->GetName() << std::endl;
    std::cout << "==================== Loop over events =======================" << std::endl;
  }
}

void::plotterTools::fillFiberOrder(){

  fiberOrderA.clear();
  fiberOrderB.clear();

  fiberOrderA.push_back(31);
  fiberOrderA.push_back(29);
  fiberOrderA.push_back(23);
  fiberOrderA.push_back(21);
  fiberOrderA.push_back(5);
  fiberOrderA.push_back(7);
  fiberOrderA.push_back(15);
  fiberOrderA.push_back(13);
  fiberOrderA.push_back(1);
  fiberOrderA.push_back(3);
  fiberOrderA.push_back(11);
  fiberOrderA.push_back(9);
  fiberOrderA.push_back(6);
  fiberOrderA.push_back(8);
  fiberOrderA.push_back(16);
  fiberOrderA.push_back(14);
  fiberOrderA.push_back(17);
  fiberOrderA.push_back(27);
  fiberOrderA.push_back(19);
  fiberOrderA.push_back(25);
  fiberOrderA.push_back(24);
  fiberOrderA.push_back(22);
  fiberOrderA.push_back(32);
  fiberOrderA.push_back(30);
  fiberOrderA.push_back(4);
  fiberOrderA.push_back(2);
  fiberOrderA.push_back(12);
  fiberOrderA.push_back(10);
  fiberOrderA.push_back(20);
  fiberOrderA.push_back(18);
  fiberOrderA.push_back(28);
  fiberOrderA.push_back(26);

  fiberOrderB.push_back(54);
  fiberOrderB.push_back(64);
  fiberOrderB.push_back(56);
  fiberOrderB.push_back(62);
  fiberOrderB.push_back(49);
  fiberOrderB.push_back(51);
  fiberOrderB.push_back(59);
  fiberOrderB.push_back(57);
  fiberOrderB.push_back(53);
  fiberOrderB.push_back(55);
  fiberOrderB.push_back(63);
  fiberOrderB.push_back(61);
  fiberOrderB.push_back(45);
  fiberOrderB.push_back(47);
  fiberOrderB.push_back(37);
  fiberOrderB.push_back(39);
  fiberOrderB.push_back(34);
  fiberOrderB.push_back(42);
  fiberOrderB.push_back(36);
  fiberOrderB.push_back(44);
  fiberOrderB.push_back(50);
  fiberOrderB.push_back(52);
  fiberOrderB.push_back(58);
  fiberOrderB.push_back(60);
  fiberOrderB.push_back(38);
  fiberOrderB.push_back(48);
  fiberOrderB.push_back(40);
  fiberOrderB.push_back(46);
  fiberOrderB.push_back(41);
  fiberOrderB.push_back(43);
  fiberOrderB.push_back(33);
  fiberOrderB.push_back(35);

}

int plotterTools::findPosition(std::vector<int>* fiberVec, int n){

  std::vector<int>::iterator iter=  std::find(fiberVec->begin(), fiberVec->end(),n);

  if(iter==fiberVec->end()){
    return -1;
  }else{
    return iter-fiberVec->begin();
  }

}
