#ifndef PLOT_TOOLS
#define PLOT_TOOLS

#include <vector>
#include <TStyle.h>
#include <TROOT.h>
#include <TColor.h>
#include <TCanvas.h>
#include <TH1.h>
#include <TH2.h>
#include <TProfile.h>
#include <TGraph.h>
#include <TRandom.h>
#include <TChain.h>
#include <TString.h>
#include <TLegend.h>

#include <set>
#include <tuple>

#include <Event.hpp>

#include "interface/Waveform.hpp"

#define nActiveDigitizerChannels 4

#define nPlanesHodo 4
#define nFibersHodo 64

// not the real hodoscope mapping, just for internal DQM use
#define hodoX1 0
#define hodoY1 1
#define hodoX2 2
#define hodoY2 3

#define hodoSmallX 0
#define hodoSmallY 1

#define nPlanesSmallHodo 2
#define nFibersSmallHodo 8

//schema to be checked: Xleft, Xright, Ydown, Yup
#define wcXl 0
#define wcXr 1
#define wcYd 2
#define wcYu 3

typedef enum PlotType {
  kPlot1D,
  kPlot2D,
  kPlotGraph,
  kPlotHistory
} PlotType;

class varPlot{

public:

  float* Get(uint i);
  std::pair<float*,float*> Get2D(uint i);
  uint Size();
  TObject* Plot();
  void Fill(float val, int i=-1);
  void Fill2D(float valX, float valY, int i=-1);
  void ClearVectors();
  varPlot();
  varPlot(int *iThisEntry, int *iHistEntry_, PlotType type_, bool profile_=false, uint size_=0);
  ~varPlot();

  TString name;
  int *iThisEntry;
  int *iHistEntry;

  std::vector<float>* Get();
  std::pair<std::vector<float>*, std::vector<float>*> Get2D();

  void SetPlot(TObject* plot_);
  TObject* GetPlot();
  void SetName(TString name_);

  void SetGM(TString group_, TString module_);
  bool doPlot;
  TString group;
  TString module;

  bool doProfile;
  int type;
  std::vector<float> x;
  std::vector<float> y;
  TObject *plot;

  std::vector<float> *xptr;
  std::vector<float> *yptr;

  Waveform *waveform;

private:

};

class outTreeBranch {

public:

  outTreeBranch();
  outTreeBranch(TString name_, std::map<TString,varPlot*> *varplots_);
  ~outTreeBranch();

  TString name;
  std::vector<float> data;
  std::vector<float> *dataptr;

  std::vector<std::pair<TString,int> > members;

  void Fill();
  void addMember(TString name, int pos = 0);
  void addDummy(int howmany);

  std::map<TString,varPlot*> *varplots;

};

class plotterTools{

public:

  plotterTools (TString filename, TString outfname, TString outdname);

  TChain* inputTree_;
  TFile* inputFile_;
  TFile* outputFile_;
  TFile* integratedFile_;
  TString outputDir_;

  TString module_, group_;
  int historyStep_;//set the step of events for history plots
  bool isIntegratedNew;
  std::map<TString,TH1F*> integratedPlots_;

  treeStructData treeStruct_;

  ULong64_t timeStart_[MAX_RO];
  ULong64_t timeEnd_[MAX_RO];

  std::vector<int> fiberOrderA;
  std::vector<int> fiberOrderB;
//  std::vector<int> fiberOrderC;
//  std::vector<int> fiberOrderD;
  std::map<TString,bool> makeProfile_;
  std::map<TString,TObject*> outObjects_;
  std::map<TString,bool> vetoFillObjects;
  std::map<TString,TString> plotShortNames_;
  std::map<TString,TString> plotLongNames_;
  std::vector<float> variables_;//<value,dimension>
  std::map<TString,float*> variablesMap_;
  std::map<TString,int> variablesIterator_;
  std::map<TString,std::vector<float> > variablesContainer_;
  std::map<TString,TString > variablesContainerTitles_;
  std::map<TString,varPlot*> varplots;
  std::map<TString,outTreeBranch*> treevars;

  void initTreeVars();
  void fillTreeVars();
  void initTreeDQMBranches();

  int iThisEntry;
  int iHistEntry;

  //fibers
  bool fibersOn_[nPlanesHodo][nFibersHodo];
  bool fibersOnSmall_[nPlanesSmallHodo][nFibersSmallHodo];

  //tdc readings
  static const UInt_t MaxTdcChannels = 4;
  static const UInt_t MaxTdcReadings = 20;
  std::vector<float> tdc_readings[MaxTdcChannels];
  float tdc_recox, tdc_recoy;

  //adc channel names
  std::map<TString,UInt_t*> adc_channelnames;
  void initAdcChannelNames(int nBinsHistory);

  //digitizer channel names
  template <class T> set<int> listElements (T * array, int Nmax);
  map <int, TH2F *> digi_histos ;
  map <int, Waveform *> digi_waveforms;
  void initDigiPlots();
  TString getDigiChannelName(int group, int channel);

  void bookPlotsADC();
  bool wantADCplots;

  void bookPlotsDigitizer();
  bool wantDigiplots;

  inline float timeSampleUnit(int drs4Freq)
  {
    if (drs4Freq == 0)
      return 0.2E-9;
    else if (drs4Freq == 1)
      return 0.4E-9;
    else if (drs4Freq == 2)
      return 1.E-9;
    
    return -999.;
  }

  void initOutputTree();
  TTree *outputTree;

  void fillObjects();
  void fillHodo();
  void fillTdc();
  void fillFiberOrder();
  void set_plot_blue ();
  void set_palette_fancy ();
  void setPlotsFormat ();
  void plotMe (TH1F * histo, TString name="");
  void plotMe (TH2F * histo, bool makeProfile = false, TString name="");
  void plotMe (TGraph * graph, const TString & name);
  void setAxisTitles (TH1 * histo, const TString  xTitle, const TString  yTitle);
  void setAxisTitles (TH2 * histo, const TString  xTitle, const TString  yTitle);
  void setAxisTitles (TGraph * graph, const TString  xTitle, const TString  yTitle);
  void setPlotAxisRange(TString name, TString axis,float min, float max);
  void readInputTree ();
  void Loop();
  void saveHistos();
  void plotHistos();
  void printHistos();
  void bookPlotsScaler (int nBinsHistory);
  void bookPlotsHodo (int nBinsHistory);
  void bookPlotsSmallHodo (int nBinsHistory);
  void bookPlotsDAQStatus (int nBinsHistory);
  void bookPlotsTDC(int nBinsHistory);
  void bookCombinedPlots ();
  void bookCombinedPlotsHodo ();
  TGraph* bookGraph (TString name, int nPoints, TString type, TString group, TString module);
  void setGroup(TString group);
  void setModule(TString module);
  void setStepHistoryPlots(int n);
  int getTreeEntries();
  int getStepHistoryPlots();
  TGraph * addPlot(bool doPlot, TString name,int nPoints,TString type, TString group, TString module, bool vetoFill=false);//TGraph
  TH1F * addPlot(bool doPlot, TString name,int nBinsX, float xMin, float xMax, TString type, TString group, TString module, int varDim=1, bool vetoFill=false);//TH1F
  TH2F * addPlot (bool doPlot, TString name,int nBinsX, float xMin, float xMax, int nBinsY, float yMin, float yMax, 
                  TString xTitle, TString yTitle, TString type, TString group, TString module, bool addProfile = false, bool vetoFill=false);
  void addPlotCombined(bool doPlot, TString name, TString name1, TString name2,TString type, TString group , TString module);
  TH1F* bookHisto(TString name,int nBinsX,float xMin, float xMax, TString type, TString group, TString module);
  TH2F* bookHisto2D(TString name,int nBinsX,float xMin, float xMax,int nBinsY, float yMin, float yMax,TString xTitle, TString yTitle, TString type, TString group, TString module);
  TH2F* bookHistoCombined(TString name,TString name1, TString name2);
  void initVariable(TString name, int varDim=1);
  void computeVariable(TString name);
  pair<int, string> execute (const string & command);
  void fitHisto(TString name, TString func);
  float getMinimumP (TProfile * p) ;
  float getMaximumP (TProfile * p) ;

  //fibers
  int findPosition(std::vector<int>* fiberVec, int n);

  //integratedfile
  void initIntegrated(TString nameFile);
};

#endif
