// Dear emacs, this is -*- c++ -*-
// $Id: CycleCreators.py 344 2012-12-13 13:10:53Z krasznaa $
#ifndef TauTauAnalysis_H
#define TauTauAnalysis_H

// SFrame include(s):
#include "core/include/SCycleBase.h"

// External include(s):
#include "../NtupleVariables/include/JetNtupleObject.h"
#include "../NtupleVariables/include/Jet.h"
#include "../NtupleVariables/include/GenJetak4NtupleObject.h"
#include "../NtupleVariables/include/GenJetak4.h"
#include "../NtupleVariables/include/EventInfoNtupleObject.h"
#include "../NtupleVariables/include/ElectronNtupleObject.h"
#include "../NtupleVariables/include/Electron.h"
#include "../NtupleVariables/include/MuonNtupleObject.h"
#include "../NtupleVariables/include/Muon.h"
#include "../NtupleVariables/include/MissingEtNtupleObject.h"
#include "../NtupleVariables/include/MissingEt.h"
#include "../NtupleVariables/include/GenParticleNtupleObject.h"
#include "../NtupleVariables/include/GenParticle.h"
#include "../GoodRunsLists/include/TGoodRunsList.h"
#include "../GoodRunsLists/include/TGoodRunsListReader.h"
#include "../PileupReweightingTool/include/PileupReweightingTool.h"
#include "../BTaggingTools/include/BTaggingScaleTool.h"
#include "../LepEff2016/interface/ScaleFactorTool.h"
#include "../RecoilCorrections/interface/RecoilCorrector.h"
#include "../SVFitTools/interface/SVFitTool.h"
#include "../SVFitTools/interface/SVfitStandaloneAlgorithm.h"
//#include "../SVFit/include/NSVfitStandaloneAlgorithm.h"
//#include "../SVfitStandalone/interface/SVfitStandaloneAlgorithm.h"
//#include "../SVfitStandalone/interface/SVfitStandaloneLikelihood.h"
#include "../JetCorrectionTool/interface/JetCorrectionTool.h"

// ROOT include(s):
#include <TFile.h>
#include <TTree.h>
#include <TMath.h>
#include "TRandom3.h"
//#include <TBits.h>
//#include "TMatrixD.h"

//class TH1D;
//class TH2D;
//class TRandom3;
//class TBits;



namespace UZH {
  class Jet;
  class Electron;
  class Muon;
  class MissingEt;
  class GenParticle;
}



/**
 *   @short Class to run Tau Tau Analysis
 *
 *          Put a longer description over here...
 *
 *  @author Izaak Neutelings and Yuta Takahasi (from Camilla Galloni's code)
 *  @version $Revision: 344 $
 */
class TauTauAnalysis : public SCycleBase {

  public:
  
    // enumeration of all cut flags
    typedef enum {
      kBeforeCuts,            // C0
      kJSON,                  // C1
      kTrigger,               // C2
      kMetFilters,            // C3
      kLepton,                // C4
      kOtherLepton,           // C5
      kLepLep,                // C6
      kBeforeCutsWeighted,    // C7
      kNumCuts                // last!
    } SelectionCuts;
  
    // static array of all cut names
    const std::string kCutName[ kNumCuts ] = {
      "BeforeCuts",             // C0
      "JSON",                   // C1
      "Trigger",                // C2
      "MetFilters",             // C3
      "Lepton",                 // C4
      "OtherLepton",            // C5
      "LepLep",                 // C6
      "BeforeCutsWeighted",     // C7
    };
    
    
    // default constructor and destructor
    TauTauAnalysis();  // default constructor
    ~TauTauAnalysis(); // default destructor
    
    // SFrame
    virtual void BeginCycle() throw( SError );                        // called beginning of the cycle
    virtual void EndCycle()   throw( SError );                        // called at the end of the cycle
    virtual void BeginInputData( const SInputData& ) throw( SError ); // called at the beginning of a new input data
    virtual void EndInputData  ( const SInputData& ) throw( SError ); // called after finishing to process an input data
    virtual void BeginInputFile( const SInputData& ) throw( SError ); // called after opening each new input file
    virtual void ExecuteEvent  ( const SInputData&, Double_t    ) throw( SError ); // called for every event
    virtual bool isGoodEvent   ( int runNumber, int lumiSection );    // check good lumi section
    
    
    /// Function to book tree branches
    //virtual void FillBranches(const std::string& channel,  const std::vector<UZH::Jet>& Jet, const UZH::Tau& tau, const  TLorentzVector& lepton, const UZH::MissingEt& met );
    virtual void FillBranches( const std::string& channel,
                               const UZH::Muon& muon,      const UZH::Electron& electron,
                               std::vector<UZH::Jet>& Jets, UZH::MissingEt& met, UZH::MissingEt& puppimet );//, const UZH::MissingEt& mvamet=NULL);
    virtual void FillJetBranches(  const char* ch, std::vector<UZH::Jet>& Jets, UZH::MissingEt& met,
                                   const TLorentzVector& muon_tlv, const TLorentzVector& electron_tlv );
    virtual void FillJetBranches_JEC( double& jpt, double& jeta, const TLorentzVector& jet, bool save=true );
    virtual void FillJetBranches_JEC( double& jpt_1, double& jeta_1, double& jpt_2, double& jeta_2, const TLorentzVector& jet1, const TLorentzVector& jet2 ); //, bool swap = false
    
    // check pass of triggers / MET filters
    //virtual TString passTrigger( int runNumber, int lumiSection );
    virtual TString passTrigger();
    virtual bool TrigMatch(const UZH::Muon& muon, const UZH::Electron& electron, int runnumber);
    //virtual bool TrigMatchCheck();
    virtual bool passMETFilters();
    
    // obtain event weights for MC
    virtual void getEventWeight();
    
    // GenFilter to select Z to tautau events
    //virtual void genFilterZtautau();
    
    // set tlv of generator boson for recoil corrections
    virtual void setGenBosonTLVs();
    
    // match reco objects to taus
    virtual int genMatch(Float_t lep_eta, Float_t lep_phi);
    
    // help function
    virtual Float_t deltaPhi(Float_t p1, Float_t p2);
    virtual Float_t deltaR(Float_t p1, Float_t p2);
    virtual void countJets(  const TLorentzVector& jet_tlv, Int_t& ncjets, Int_t& nfjets, Int_t& ncbtags, TLorentzVector& bjet_tlv, TLorentzVector& jet2_tlv, const bool isBTagged );
    virtual void shiftLeptonAndMET( const float shift, TLorentzVector& lep_shifted, TLorentzVector& met_shifted, bool shiftEnergy = true );
    
    // IDs
    //virtual bool isNonTrigElectronID( const UZH::Electron& electron );
    virtual bool LooseJetID( const UZH::Jet& jet );
    
    // extra scaling factors
    virtual float genMatchSF( const std::string& channel, const int genmatch_2, const float tau_eta = 0. );
    virtual bool  getBTagWeight_promote_demote( const UZH::Jet& jet );
    
    // checks
    virtual void printRow( const std::vector<std::string> svec = {}, const std::vector<int> ivec = {}, const std::vector<double> dvec = {}, const std::vector<float> fvec = {}, const int w=10 );
    
    /// fill cut flow
    virtual void fillCutflow( TString histName, TString dirName, const Int_t id, const Double_t weight = 1. );
    //virtual void printCutFlow( const std::string& ch, const std::string& name, const TString hname, const TString dirname, std::vector<std::string> cutName );
    
    
    
  private:
    
    
    ///
    /// INPUT VARIABLE OBJECTS:
    ///
    
    Ntuple::EventInfoNtupleObject   m_eventInfo;      ///< event info container
    Ntuple::JetNtupleObject         m_jetAK4;         ///< jet container
    Ntuple::GenJetak4NtupleObject   m_genJetAK4;      ///< Gen jet container
    Ntuple::ElectronNtupleObject    m_electron;       ///< electron container
    Ntuple::MuonNtupleObject        m_muon;           ///< muon container
    Ntuple::MissingEtNtupleObject   m_missingEt;      ///< missing E_T container
    Ntuple::MissingEtNtupleObject   m_puppimissingEt; ///< missing E_T container
    //Ntuple::MissingEtNtupleObject   m_mvamissingEt;   ///< missing E_T container
    Ntuple::GenParticleNtupleObject m_genParticle;    ///< gen particle container
    
    
    ///
    /// OTHER OBJECTS
    ///
    
    Root::TGoodRunsList     m_grl;
    PileupReweightingTool   m_PileupReweightingTool;
    BTaggingScaleTool       m_BTaggingScaleTool;
    ScaleFactorTool         m_ScaleFactorTool;
    RecoilCorrectorTool     m_RecoilCorrector;
    JetCorrectionTool       m_JetCorrectionTool;
    SVFitTool               m_SVFitTool;
    
    //TLorentzVector applySVFitSemileptonic (float cov00, float cov10, float cov11, float met, float met_phi, TLorentzVector lep1 , TLorentzVector lep2);
    //TLorentzVector applySVFitHadronic     (float cov00, float cov10, float cov11, float met, float met_phi, TLorentzVector lep1 , TLorentzVector lep2);
    //TLorentzVector applySVFit             (float cov00, float cov10, float cov11, float met, float met_phi, TLorentzVector lep1 , TLorentzVector lep2);
    //float applySVFit(float cov00, float cov10, float cov11, float met, float met_phi, TLorentzVector lep1 , TLorentzVector lep2, const std::string& channel);
    
    void extraLeptonVetos(const std::string& channel, const UZH::Muon& muon, const UZH::Electron& electron);
    
    // naming
    std::string m_recoTreeName;         ///< name of tree with reconstructed objects in ntuple
    std::vector<std::string> channels_;

    int m_ntupleLevel;                  ///< cut at which branches for ntuple are written out
    std::string m_jetAK4Name;           ///< name of AK4 jet collection in tree with reconstructed objects
    std::string m_genJetAK4Name;        ///< name of gen AK4 jet collection in tree with reconstructed objects
    std::string m_electronName;         ///< name of electron collection in tree with reconstructed objects
    std::string m_muonName;             ///< name of muon collection in tree with reconstructed objects
    std::string m_missingEtName;        ///< name of missing E_T collection in tree with reconstructed objects
    std::string m_genParticleName;      ///< name of gen particle collection in tree with reconstructed objects
  
    // XML flags for TauTauAnalysis
    bool      m_isData;
    bool      m_isSignal;
    bool      m_applyMETFilters;
    bool      m_doSVFit;
    bool      m_doRecoilCorr;
    bool      m_doZpt;
    bool      m_doTTpt;
    bool      m_doEES;
    double    m_EESshift;
    double    m_EESshiftEndCap;
    bool      m_doJEC;

    ///
    /// CUTS
    ///
     
    // jet
    double    m_jetPtCut;         ///< cut on jet pT
    double    m_jetEtaCut;        ///< cut on jet eta
    double    m_AK4jetPtCut;      ///< cut on jet pT
    double    m_AK4jetEtaCut;     ///< cut on jet eta
    double    m_CSVWorkingPoint;
    
    // b-tagging
    double    m_csvMin;
    
    // electrons
    double    m_electronPtCut;
    double    m_electronEtaCut;
    double    m_electronD0Cut;
    double    m_electronDzCut;
    double    m_electronIsoCut;
    
    // muons
    double    m_muonPtCut;
    double    m_muonEtaCut;
    double    m_muonD0Cut;
    double    m_muonDzCut;
    double    m_muonIsoCut;
    
    // leptons = muons and electrons
    double    m_leptonPtCut;
    double    m_leptonEtaCut;
    
    // MET
    double    m_metCut;
    
    
    ///
    /// FILE NAME
    ///
    
    std::string m_jsonName;
    //std::string m_TrigSF_muonName;
    //std::string m_IDSF_muonName;
    //std::string m_IsoSF_muonName;
    //std::string m_IDSF_eleName;

    // other variables needed
    std::vector<std::string> m_triggerNames_emu_mc;
    std::vector<std::string> m_triggerNames_emu_data;
    std::vector<std::string> m_triggerNames_emu;
    std::string m_trigger_Flags;
    
    int e_mu;
    
    
    ///
    /// BRANCHES
    ///
    
    double b_weight_;
    double b_genweight_;
    double b_puweight_;
    double b_weightbtag_;
    double b_npu_;
    double b_dR_ll_gen_ = -1;
    Int_t b_isData_;
    
    bool b_dilepton_veto_;
    bool b_extraelec_veto_;
    bool b_extramuon_veto_;
    
    //double b_weightBtag;
    //double b_weightBtag_veto;
    
    TLorentzVector boson_tlv;
    TLorentzVector boson_tlv_vis;
    
    
    // synchronisation:
    // https://twiki.cern.ch/twiki/bin/viewauth/CMS/HiggsToTauTauWorking2016#Synchronisation
    
    std::map<std::string,Int_t>    b_channel;  // 1 mumu; 2 emu;
    std::map<std::string,Int_t>    b_isData;
    
    std::map<std::string,Double_t> b_weight;
    std::map<std::string,Double_t> b_genweight;
    std::map<std::string,Double_t> b_puweight;
    std::map<std::string,Double_t> b_weightbtag;
    //std::map<std::string,Double_t> b_genmatchweight;
    std::map<std::string,Double_t> b_zptweight;
    std::map<std::string,Double_t> b_ttptweight;
    std::map<std::string,Double_t> b_ttptweight_runI;
    std::map<std::string,Double_t> b_trigweight_1;
    std::map<std::string,Double_t> b_trigweight_or_1;
    std::map<std::string,Double_t> b_idisoweight_1;
    std::map<std::string,Int_t>    b_trigweight_2;
    std::map<std::string,Double_t> b_idisoweight_2;
    
    std::map<std::string,Int_t>    b_run;
    std::map<std::string,Int_t>    b_evt;
    std::map<std::string,Int_t>    b_lum;
    
    std::map<std::string,Int_t>    b_npv;
    std::map<std::string,Int_t>    b_npu; 
    std::map<std::string,Int_t>    b_NUP;
    std::map<std::string,Double_t> b_rho;
    
    std::map<std::string,Int_t>    b_njets;
    std::map<std::string,Int_t>    b_nfjets;
    std::map<std::string,Int_t>    b_ncjets;
    std::map<std::string,Int_t>    b_nbtag;
    std::map<std::string,Int_t>    b_ncbtag;
    std::map<std::string,Int_t>    b_njets20;
    std::map<std::string,Int_t>    b_nfjets20;
    std::map<std::string,Int_t>    b_ncjets20;
    std::map<std::string,Int_t>    b_nbtag20;
    std::map<std::string,Int_t>    b_ncbtag20;
    
    std::map<std::string,Int_t>    b_njets_jesUp;
    std::map<std::string,Int_t>    b_njets_jesDown;
    std::map<std::string,Int_t>    b_njets_nom;
    std::map<std::string,Int_t>    b_njets_jerUp;
    std::map<std::string,Int_t>    b_njets_jerDown;
    std::map<std::string,Int_t>    b_njets20_jesUp;
    std::map<std::string,Int_t>    b_njets20_jesDown;
    std::map<std::string,Int_t>    b_njets20_nom;
    std::map<std::string,Int_t>    b_njets20_jerUp;
    std::map<std::string,Int_t>    b_njets20_jerDown;
    std::map<std::string,Int_t>    b_ncbtag_jesUp;
    std::map<std::string,Int_t>    b_ncbtag_jesDown;
    std::map<std::string,Int_t>    b_ncbtag_nom;
    std::map<std::string,Int_t>    b_ncbtag_jerUp;
    std::map<std::string,Int_t>    b_ncbtag_jerDown;
    std::map<std::string,Int_t>    b_ncjets_jesUp;
    std::map<std::string,Int_t>    b_ncjets_jesDown;
    std::map<std::string,Int_t>    b_ncjets_nom;
    std::map<std::string,Int_t>    b_ncjets_jerUp;
    std::map<std::string,Int_t>    b_ncjets_jerDown;
    std::map<std::string,Int_t>    b_nfjets_jesUp;
    std::map<std::string,Int_t>    b_nfjets_jesDown;
    std::map<std::string,Int_t>    b_nfjets_nom;
    std::map<std::string,Int_t>    b_nfjets_jerUp;
    std::map<std::string,Int_t>    b_nfjets_jerDown;
    
    std::map<std::string,Double_t> b_dphi_ll_bj_jesUp;
    std::map<std::string,Double_t> b_dphi_ll_bj_jesDown;
    std::map<std::string,Double_t> b_dphi_ll_bj_nom;
    std::map<std::string,Double_t> b_dphi_ll_bj_jerUp;
    std::map<std::string,Double_t> b_dphi_ll_bj_jerDown;
    std::map<std::string,Double_t> b_met_jesUp;
    std::map<std::string,Double_t> b_met_jesDown;
    std::map<std::string,Double_t> b_met_nom;
    std::map<std::string,Double_t> b_met_jerUp;
    std::map<std::string,Double_t> b_met_jerDown;
    std::map<std::string,Double_t> b_met_UncEnUp;
    std::map<std::string,Double_t> b_met_UncEnDown;
    std::map<std::string,Double_t> b_pfmt_1_jesUp;
    std::map<std::string,Double_t> b_pfmt_1_jesDown;
    std::map<std::string,Double_t> b_pfmt_1_nom;
    std::map<std::string,Double_t> b_pfmt_1_jerUp;
    std::map<std::string,Double_t> b_pfmt_1_jerDown;
    std::map<std::string,Double_t> b_pfmt_1_UncEnUp;
    std::map<std::string,Double_t> b_pfmt_1_UncEnDown;
    
    std::map<std::string,Double_t> b_weightbtag_bcUp;
    std::map<std::string,Double_t> b_weightbtag_bcDown;
    std::map<std::string,Double_t> b_weightbtag_udsgUp;
    std::map<std::string,Double_t> b_weightbtag_udsgDown;
    
    std::map<std::string,Double_t> b_pt_1;
    std::map<std::string,Double_t> b_eta_1;
    std::map<std::string,Double_t> b_phi_1;
    std::map<std::string,Double_t> b_m_1;
    std::map<std::string,Int_t>    b_q_1;
    std::map<std::string,Double_t> b_d0_1;
    std::map<std::string,Double_t> b_dz_1;
    //std::map<std::string,Double_t> b_mt_1;
    std::map<std::string,Double_t> b_pfmt_1;
    std::map<std::string,Double_t> b_puppimt_1;
    std::map<std::string,Double_t> b_iso_1;
    std::map<std::string,Int_t>    b_id_e_mva_nt_loose_1;
    std::map<std::string,Int_t>    b_id_e_mva_nt_loose_1_old;
    std::map<std::string,Int_t>    b_gen_match_1;
    
    std::map<std::string,Double_t> b_pt_2;
    std::map<std::string,Double_t> b_eta_2;
    std::map<std::string,Double_t> b_phi_2;
    std::map<std::string,Double_t> b_m_2;
    std::map<std::string,Int_t>    b_q_2;
    std::map<std::string,Double_t> b_d0_2;
    std::map<std::string,Double_t> b_dz_2;
    std::map<std::string,Double_t> b_mt_2;
    std::map<std::string,Double_t> b_pfmt_2;
    std::map<std::string,Double_t> b_puppimt_2;
    std::map<std::string,Double_t> b_iso_2;
    std::map<std::string,Double_t> b_iso_2_medium;
    std::map<std::string,Int_t>    b_gen_match_2;
    
    std::map<std::string,Int_t>    b_dilepton_veto;
    std::map<std::string,Int_t>    b_extraelec_veto;
    std::map<std::string,Int_t>    b_extramuon_veto;
    std::map<std::string,Int_t>    b_lepton_vetos;
    std::map<std::string,Int_t>    b_iso_cuts;
    
    std::map<std::string,Double_t> b_jpt_1;
    std::map<std::string,Double_t> b_jeta_1;
    std::map<std::string,Double_t> b_jphi_1;
    std::map<std::string,Double_t> b_jpt_2;
    std::map<std::string,Double_t> b_jeta_2;
    std::map<std::string,Double_t> b_jphi_2;
    
    std::map<std::string,Double_t> b_jpt_1_jesUp;
    std::map<std::string,Double_t> b_jpt_1_jesDown;
    std::map<std::string,Double_t> b_jpt_1_nom;
    std::map<std::string,Double_t> b_jpt_1_jerUp;
    std::map<std::string,Double_t> b_jpt_1_jerDown;
    std::map<std::string,Double_t> b_jeta_1_jesUp;
    std::map<std::string,Double_t> b_jeta_1_jesDown;
    std::map<std::string,Double_t> b_jeta_1_nom;
    std::map<std::string,Double_t> b_jeta_1_jerUp;
    std::map<std::string,Double_t> b_jeta_1_jerDown;
    std::map<std::string,Double_t> b_jpt_2_jesUp;
    std::map<std::string,Double_t> b_jpt_2_jesDown;
    std::map<std::string,Double_t> b_jpt_2_nom;
    std::map<std::string,Double_t> b_jpt_2_jerUp;
    std::map<std::string,Double_t> b_jpt_2_jerDown;
    std::map<std::string,Double_t> b_jeta_2_jesUp;
    std::map<std::string,Double_t> b_jeta_2_jesDown;
    std::map<std::string,Double_t> b_jeta_2_nom;
    std::map<std::string,Double_t> b_jeta_2_jerUp;
    std::map<std::string,Double_t> b_jeta_2_jerDown;
    
    // std::map<std::string,Double_t> b_fjpt_1;
    // std::map<std::string,Double_t> b_fjeta_1;
    // std::map<std::string,Double_t> b_fjphi_1;
    
    std::map<std::string,Double_t> b_bpt_1;
    std::map<std::string,Double_t> b_beta_1;
    std::map<std::string,Double_t> b_bphi_1;
    std::map<std::string,Double_t> b_bcsv_1;
    std::map<std::string,Double_t> b_bpt_2;
    std::map<std::string,Double_t> b_beta_2;
    std::map<std::string,Double_t> b_bphi_2;
    std::map<std::string,Double_t> b_bcsv_2;
    
    std::map<std::string,Double_t> b_met;
    std::map<std::string,Double_t> b_met_old;
    std::map<std::string,Double_t> b_metphi;
    //std::map<std::string,Double_t> b_metcorrphi;
    std::map<std::string,Double_t> b_puppimet;
    std::map<std::string,Double_t> b_puppimetphi;
    //std::map<std::string,Double_t> b_mvamet;
    //std::map<std::string,Double_t> b_mvamet_old;
    //std::map<std::string,Double_t> b_mvametphi;
    
    std::map<std::string,Double_t> b_m_vis;
    std::map<std::string,Double_t> b_pt_tt;
    std::map<std::string,Double_t> b_pt_tt_vis;
    
    std::map<std::string,Double_t> b_m_sv; 
    //std::map<std::string,Double_t> b_m_sv_pfmet;
    std::map<std::string,Double_t> b_pt_tt_sv;
    
    std::map<std::string,Double_t> b_dR_ll;
    std::map<std::string,Double_t> b_dR_ll_gen;
    std::map<std::string,Double_t> b_dphi_ll_bj;
    std::map<std::string,Double_t> b_mt_tot;
    std::map<std::string,Double_t> b_ht;
    
    std::map<std::string,Double_t> b_m_genboson;
    std::map<std::string,Double_t> b_pt_genboson;
    std::map<std::string,Double_t> b_pt_top_1;
    std::map<std::string,Double_t> b_pt_top_2;
    
    //std::map<std::string,Double_t> b_pt_sv; 
    //std::map<std::string,Double_t> b_eta_sv; 
    //std::map<std::string,Double_t> b_phi_sv;
    std::map<std::string,Double_t> b_pzetamiss;
    std::map<std::string,Double_t> b_pzetavis;
    std::map<std::string,Double_t> b_pzeta_disc;
    
    std::map<std::string,Double_t> b_vbf_mjj;
    std::map<std::string,Double_t> b_vbf_deta;
    std::map<std::string,Double_t> b_vbf_jdphi;
    std::map<std::string,Int_t>    b_vbf_ncentral;
    std::map<std::string,Int_t>    b_vbf_ncentral20;

    // Macro adding the functions for dictionary generation
    ClassDef( TauTauAnalysis, 0 );
  
}; // class TauTauAnalysis

#endif // TauTauAnalysis_H

