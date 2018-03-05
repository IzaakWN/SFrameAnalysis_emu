// $Id: CycleCreators.py 344 2012-12-13 13:10:53Z krasznaa $

#include "../include/TauTauAnalysis.h"

ClassImp( TauTauAnalysis );





TauTauAnalysis::TauTauAnalysis() : SCycleBase(),
    m_eventInfo( this ),
    m_jetAK4( this ),
    m_genJetAK4( this ),
    m_electron( this ),
    m_muon( this ),
    m_tau( this ),
    m_missingEt( this ),
    m_puppimissingEt( this ),
    m_genParticle( this ),
    m_PileupReweightingTool( this ),
    m_BTaggingScaleTool( this ),
    m_ScaleFactorTool( this ),
    m_RecoilCorrector( this ),
    m_JetCorrectionTool( this ),
    m_SVFitTool( this )
{

  m_logger << INFO << "Hello!" << SLogger::endmsg;
  SetLogName( GetName() );
  
  // channels
  channels_.push_back("emu");
  
  // read configuration details from XML file
  DeclareProperty( "RecoTreeName",          m_recoTreeName          = "tree"            );
  DeclareProperty( "JetAK4Name",            m_jetAK4Name            = "jetAK4"          );
  DeclareProperty( "ElectronName",          m_electronName          = "el"              );
  DeclareProperty( "MuonName",              m_muonName              = "mu"              );
  DeclareProperty( "TauName",               m_tauName               = "tau"             );
  DeclareProperty( "MissingEtName",         m_missingEtName         = "MET"             );
  DeclareProperty( "GenParticleName",       m_genParticleName       = "genParticle"     );
  
  DeclareProperty( "IsData",                m_isData                = false             );
  DeclareProperty( "doSVFit",               m_doSVFit               = false             );
  DeclareProperty( "IsSignal",              m_isSignal              = false             );
  DeclareProperty( "doRecoilCorr",          m_doRecoilCorr          = false             );
  DeclareProperty( "doZpt",                 m_doZpt                 = false             );
  DeclareProperty( "doTTpt",                m_doTTpt                = false             );
  DeclareProperty( "doJEC",                 m_doJEC                 = false             );
  DeclareProperty( "EESshift",              m_EESshift              = 0.0               );
  DeclareProperty( "EESshiftEndCap",        m_EESshiftEndCap        = m_EESshift*2.5    );
  DeclareProperty( "doEES",                 m_doEES                 = m_EESshift != 0.0 );
  DeclareProperty( "MC_V1",                 m_MC_V1                 = false             );
  
  DeclareProperty( "AK4JetPtCut",           m_AK4jetPtCut           = 20.               );
  DeclareProperty( "AK4JetEtaCut",          m_AK4jetEtaCut          = 4.7               );
  DeclareProperty( "CSVWorkingPoint",       m_CSVWorkingPoint       = 0.8484            );
  
  DeclareProperty( "ElectronPtCut",         m_electronPtCut         = 20.               );
  DeclareProperty( "ElectronEtaCut",        m_electronEtaCut        = 2.4               );
  DeclareProperty( "ElectronD0Cut",         m_electronD0Cut         = 0.045             );
  DeclareProperty( "ElectronDzCut",         m_electronDzCut         = 0.2               );
  DeclareProperty( "ElectronIsoCut",        m_electronIsoCut        = 0.1               );
  
  DeclareProperty( "MuonPtCut",             m_muonPtCut             = 28.               ); // 23
  DeclareProperty( "MuonEtaCut",            m_muonEtaCut            = 2.4               );
  DeclareProperty( "MuonD0Cut",             m_muonD0Cut             = 0.045             );
  DeclareProperty( "MuonDzCut",             m_muonDzCut             = 0.2               );
  DeclareProperty( "MuonIsoCut",            m_muonIsoCut            = 0.15              );
  
  DeclareProperty( "TauPtCut",              m_tauPtCut              = 20.               );
  DeclareProperty( "TauEtaCut",             m_tauEtaCut             = 2.3               );
  DeclareProperty( "TauDzCut",              m_tauDzCut              = 0.2               );
  
  DeclareProperty( "JSONName",              m_jsonName              = std::string(std::getenv("SFRAME_DIR"))+"/../GoodRunsLists/JSON/Cert_294927-306462_13TeV_PromptReco_Collisions17_JSON.txt" ); // 41.86 /fb
  
}





TauTauAnalysis::~TauTauAnalysis() {
  m_logger << INFO << " " << SLogger::endmsg;
  m_logger << INFO << "Tschoe!" << SLogger::endmsg;
  std::cout << " " << std::endl;
}





void TauTauAnalysis::BeginCycle() throw( SError ) {
  m_logger << INFO << " " << SLogger::endmsg;
  m_logger << INFO << "BeginCycle" << SLogger::endmsg;
  
  e_mu   = 0;

  // Load GRL:
  if (m_isData) {
    m_logger << INFO << "Loading GoodRunsList from file: " << m_jsonName << SLogger::endmsg;
    Root::TGoodRunsListReader reader( TString( m_jsonName.c_str() ) );
    if( ! reader.Interpret() ) {
      m_logger << FATAL << "Couldn't read in the GRL!" << SLogger::endmsg;
      throw SError( ( "Couldn't read in file: " + m_jsonName ).c_str(), SError::SkipCycle );
    }
    m_grl = reader.GetMergedGoodRunsList();
    m_grl.Summary();
    m_grl.SetName( "MyGoodRunsList" );
    
    AddConfigObject( &m_grl );
  }
  
  m_triggers_mutau.clear();
  m_triggers_emu.clear();
  
  // TRIGGERS
  // https://twiki.cern.ch/twiki/bin/viewauth/CMS/HiggsToTauTauWorking2016
  // https://twiki.cern.ch/twiki/bin/viewauth/CMS/SMTauTau2016
  // https://hypernews.cern.ch/HyperNews/CMS/get/AUX/2017/02/10/17:01:24-67933-2016triggerGrandTable.pdf
  // https://indico.cern.ch/event/598433/#day-2017-02-15
  
  // muon triggers 
  m_triggers_mutau.push_back( "HLT_IsoMu27_v"                                           );
  m_triggers_mutau.push_back( "HLT_IsoTkMu27_v"                                         );
  m_triggers_mutau.push_back( "HLT_IsoMu27_eta2p1"                                      );
  m_triggers_mutau.push_back( "HLT_IsoTkMu27_eta2p1"                                    );
  
  // emu triggers
  //m_triggers_emu.push_back("HLT_Mu8_TrkIsoVVL_Ele23_CaloIdL_TrackIdL_IsoVL_v"       );
  //m_triggers_emu.push_back("HLT_Mu23_TrkIsoVVL_Ele12_CaloIdL_TrackIdL_IsoVL_v"      );
  
  return;

}





struct ll_pair {
  Int_t ilepton;
  Float_t lep_iso;
  Float_t lep_pt;
  Int_t olepton;
  Float_t olep_iso;
  Float_t olep_pt;
  Float_t dR;

  //overload comparators
  bool operator<(const ll_pair& another) const {
//     if (dR > 0.3 && another.dR > 0.3 && dR != another.dR)
//       return dR < another.dR;           // take pair with lowest dR, if dR > 0.3
    if (lep_iso != another.lep_iso)
      return lep_iso < another.lep_iso; // take pair with best (lowest) leption iso
    if (lep_pt != another.lep_pt)
      return lep_pt > another.lep_pt;   // take pair with highest lepton pt
    if (olep_iso != another.olep_iso)
      return olep_iso > another.olep_iso; // take pair with best (highest) tau iso
    if (olep_pt != another.olep_pt)
      return olep_pt > another.olep_pt;   // take pair with highest tau pt
    return ilepton < another.ilepton;
  }

};





void TauTauAnalysis::EndCycle() throw( SError ) {
   std::cout << "events in ele_mu " <<e_mu <<std::endl;
   return;
}





void TauTauAnalysis::BeginInputData( const SInputData& id ) throw( SError ) {
  //std::cout << "BeginInputData" << std::endl;
  m_logger << INFO << " " << SLogger::endmsg;
  m_logger << INFO << "BeginInputData" << SLogger::endmsg;

  m_logger << INFO << "RecoTreeName:        " <<    m_recoTreeName      << SLogger::endmsg;
  m_logger << INFO << "JetAK4Name:          " <<    m_jetAK4Name        << SLogger::endmsg;
  m_logger << INFO << "ElectronName:        " <<    m_electronName      << SLogger::endmsg;
  m_logger << INFO << "MuonName:            " <<    m_muonName          << SLogger::endmsg;
  m_logger << INFO << "TauName:             " <<    m_tauName           << SLogger::endmsg;
  m_logger << INFO << "GenParticleName:     " <<    m_genParticleName   << SLogger::endmsg;
  
  m_doEES = m_doEES and !m_isData;
  m_doJEC = m_doJEC and !(m_doEES or m_isData);
  m_logger << INFO << "IsData:              " <<    (m_isData   ?   "TRUE" : "FALSE") << SLogger::endmsg;
  m_logger << INFO << "IsSignal:            " <<    (m_isSignal ?   "TRUE" : "FALSE") << SLogger::endmsg;
  m_logger << INFO << "MC_V1:               " <<    (m_MC_V1    ?   "TRUE" : "FALSE") << SLogger::endmsg;
  m_logger << INFO << "doSVFit:             " <<    (m_doSVFit  ?   "TRUE" : "FALSE") << SLogger::endmsg;
  m_logger << INFO << "doRecoilCorr:        " <<    (m_doRecoilCorr ? "TRUE" : "FALSE") << SLogger::endmsg;
  m_logger << INFO << "doZpt:               " <<    (m_doZpt    ?   "TRUE" : "FALSE") << SLogger::endmsg;
  m_logger << INFO << "doTTpt:              " <<    (m_doTTpt   ?   "TRUE" : "FALSE") << SLogger::endmsg;
  m_logger << INFO << "doEES:               " <<    (m_doEES    ?   "TRUE" : "FALSE") << SLogger::endmsg;
  m_logger << INFO << "doJEC:               " <<    (m_doJEC    ?   "TRUE" : "FALSE") << SLogger::endmsg;
  m_logger << INFO << "EESshift:            " <<    m_EESshift          << SLogger::endmsg;
  m_logger << INFO << "EESshiftEndCap:      " <<    m_EESshiftEndCap    << SLogger::endmsg;
  
  m_logger << INFO << "ElectronPtCut:       " <<    m_electronPtCut     << SLogger::endmsg;
  m_logger << INFO << "ElectronEtaCut:      " <<    m_electronEtaCut    << SLogger::endmsg;
  m_logger << INFO << "ElectronD0Cut:       " <<    m_electronD0Cut     << SLogger::endmsg;
  m_logger << INFO << "ElectronDzCut:       " <<    m_electronDzCut     << SLogger::endmsg;
  m_logger << INFO << "ElectronIsoCut:      " <<    m_electronIsoCut    << SLogger::endmsg;
  
  m_logger << INFO << "MuonPtCut:           " <<    m_muonPtCut         << SLogger::endmsg;
  m_logger << INFO << "MuonEtaCut:          " <<    m_muonEtaCut        << SLogger::endmsg;
  m_logger << INFO << "MuonD0Cut:           " <<    m_muonD0Cut         << SLogger::endmsg;
  m_logger << INFO << "MuonDzCut:           " <<    m_muonDzCut         << SLogger::endmsg;
  m_logger << INFO << "MuonIsoCut:          " <<    m_muonIsoCut        << SLogger::endmsg;
  
  m_logger << INFO << "JSONName:            " <<    m_jsonName          << SLogger::endmsg;  
  
  
  // MARK: Branches
  m_logger << INFO << "Declaring variables for branches" << SLogger::endmsg;
  for(int chi = 0; chi < (int)channels_.size(); chi++){
    
    TString treeName = "tree_" + channels_[chi];
    const char* ch = channels_[chi].c_str();
    
    DeclareVariable( b_channel[ch],             "channel",              treeName);
    DeclareVariable( b_isData[ch],              "isData",               treeName);
    
    DeclareVariable( b_weight[ch],              "weight",               treeName);
    DeclareVariable( b_genweight[ch],           "genweight",            treeName);
    DeclareVariable( b_puweight[ch],            "puweight",             treeName);
    DeclareVariable( b_weightbtag[ch],          "weightbtag",           treeName);
    DeclareVariable( b_zptweight[ch],           "zptweight",            treeName);
    DeclareVariable( b_ttptweight[ch],          "ttptweight",           treeName);
    DeclareVariable( b_ttptweight_runI[ch],     "ttptweight_runI",      treeName);
    DeclareVariable( b_trigweight_1[ch],        "trigweight_1",         treeName);
    DeclareVariable( b_trigweight_or_1[ch],     "trigweight_or_1",      treeName);
    DeclareVariable( b_idisoweight_1[ch],       "idisoweight_1",        treeName);
    DeclareVariable( b_trigweight_2[ch],        "trigweight_2",         treeName);
    DeclareVariable( b_idisoweight_2[ch],       "idisoweight_2",        treeName);
    
    DeclareVariable( b_run[ch],                 "run",                  treeName);
    DeclareVariable( b_evt[ch],                 "evt",                  treeName);
    DeclareVariable( b_lum[ch],                 "lum",                  treeName);
    
    DeclareVariable( b_npv[ch],                 "npv",                  treeName);
    DeclareVariable( b_npu[ch],                 "npu",                  treeName);
    DeclareVariable( b_NUP[ch],                 "NUP",                  treeName);
    DeclareVariable( b_rho[ch],                 "rho",                  treeName);
    
    DeclareVariable( b_njets[ch],               "njets",                treeName);
    DeclareVariable( b_nfjets[ch],              "nfjets",               treeName);
    DeclareVariable( b_ncjets[ch],              "ncjets",               treeName);
    DeclareVariable( b_nbtag[ch],               "nbtag",                treeName);
    DeclareVariable( b_ncbtag[ch],              "ncbtag",               treeName);
    DeclareVariable( b_njets20[ch],             "njets20",              treeName);
    DeclareVariable( b_nfjets20[ch],            "nfjets20",             treeName);
    DeclareVariable( b_ncjets20[ch],            "ncjets20",             treeName);
    DeclareVariable( b_nbtag20[ch],             "nbtag20",              treeName);
    DeclareVariable( b_ncbtag20[ch],            "ncbtag20",             treeName);
    
    DeclareVariable( b_pt_1[ch],                "pt_1",                 treeName);
    DeclareVariable( b_eta_1[ch],               "eta_1",                treeName);
    DeclareVariable( b_phi_1[ch],               "phi_1",                treeName);
    DeclareVariable( b_m_1[ch],                 "m_1",                  treeName);
    DeclareVariable( b_q_1[ch],                 "q_1",                  treeName);
    DeclareVariable( b_d0_1[ch],                "d0_1",                 treeName);
    DeclareVariable( b_dz_1[ch],                "dz_1",                 treeName);
    DeclareVariable( b_pfmt_1[ch],              "pfmt_1",               treeName);
    DeclareVariable( b_puppimt_1[ch],           "puppimt_1",            treeName);
    DeclareVariable( b_iso_1[ch],               "iso_1",                treeName);
    DeclareVariable( b_id_e_mva_nt_loose_1[ch], "id_e_mva_nt_loose_1",  treeName);
    DeclareVariable( b_gen_match_1[ch],         "gen_match_1",          treeName);
    
    DeclareVariable( b_pt_2[ch],                "pt_2",                 treeName);
    DeclareVariable( b_eta_2[ch],               "eta_2",                treeName);
    DeclareVariable( b_phi_2[ch],               "phi_2",                treeName);
    DeclareVariable( b_m_2[ch],                 "m_2",                  treeName);
    DeclareVariable( b_q_2[ch],                 "q_2",                  treeName);
    DeclareVariable( b_d0_2[ch],                "d0_2",                 treeName);
    DeclareVariable( b_dz_2[ch],                "dz_2",                 treeName);
    DeclareVariable( b_pfmt_2[ch],              "pfmt_2",               treeName);
    DeclareVariable( b_puppimt_2[ch],           "puppimt_2",            treeName);
    DeclareVariable( b_iso_2[ch],               "iso_2",                treeName);
    DeclareVariable( b_iso_2_medium[ch],        "iso_2_medium",         treeName);
    DeclareVariable( b_gen_match_2[ch],         "gen_match_2",          treeName);
    
    DeclareVariable( b_pt_3[ch],                "pt_3",                 treeName);
    DeclareVariable( b_eta_3[ch],               "eta_3",                treeName);
    DeclareVariable( b_decayMode_3[ch],         "decayMode_3",          treeName);
    DeclareVariable( b_gen_match_3[ch],         "gen_match_3",          treeName);
    DeclareVariable( b_againstLepton_3[ch],     "againstLepton_3",      treeName);
    DeclareVariable( b_byIsolationMVArun2v1DBoldDMwLTraw_3[ch],           "byIsolationMVArun2v1DBoldDMwLTraw_3",           treeName);
    DeclareVariable( b_byIsolationMVArun2v1DBnewDMwLTraw_3[ch],           "byIsolationMVArun2v1DBnewDMwLTraw_3",           treeName);
    DeclareVariable( b_byVLooseIsolationMVArun2v1DBoldDMwLT_3[ch],        "byVLooseIsolationMVArun2v1DBoldDMwLT_3",        treeName);
    DeclareVariable( b_byLooseIsolationMVArun2v1DBoldDMwLT_3[ch],         "byLooseIsolationMVArun2v1DBoldDMwLT_3",         treeName);
    DeclareVariable( b_byMediumIsolationMVArun2v1DBoldDMwLT_3[ch],        "byMediumIsolationMVArun2v1DBoldDMwLT_3",        treeName);
    DeclareVariable( b_byTightIsolationMVArun2v1DBoldDMwLT_3[ch],         "byTightIsolationMVArun2v1DBoldDMwLT_3",         treeName);
    DeclareVariable( b_byVTightIsolationMVArun2v1DBoldDMwLT_3[ch],        "byVTightIsolationMVArun2v1DBoldDMwLT_3",        treeName);
    DeclareVariable( b_byVVTightIsolationMVArun2v1DBoldDMwLT_3[ch],       "byVVTightIsolationMVArun2v1DBoldDMwLT_3",       treeName);
    DeclareVariable( b_byCombinedIsolationDeltaBetaCorrRaw3Hits_3[ch],    "byCombinedIsolationDeltaBetaCorrRaw3Hits_3",    treeName);
    DeclareVariable( b_byLooseCombinedIsolationDeltaBetaCorr3Hits_3[ch],  "byLooseCombinedIsolationDeltaBetaCorr3Hits_3",  treeName);
    DeclareVariable( b_byMediumCombinedIsolationDeltaBetaCorr3Hits_3[ch], "byMediumCombinedIsolationDeltaBetaCorr3Hits_3", treeName);
    DeclareVariable( b_byTightCombinedIsolationDeltaBetaCorr3Hits_3[ch],  "byTightCombinedIsolationDeltaBetaCorr3Hits_3",  treeName);
    
    DeclareVariable( b_dilepton_veto[ch],       "dilepton_veto",        treeName);
    DeclareVariable( b_extraelec_veto[ch],      "extraelec_veto",       treeName);
    DeclareVariable( b_extramuon_veto[ch],      "extramuon_veto",       treeName);
    DeclareVariable( b_lepton_vetos[ch],        "lepton_vetos",         treeName);
    DeclareVariable( b_iso_cuts[ch],            "iso_cuts",             treeName);
    
    DeclareVariable( b_jpt_1[ch],               "jpt_1",                treeName);
    DeclareVariable( b_jeta_1[ch],              "jeta_1",               treeName);
    DeclareVariable( b_jphi_1[ch],              "jphi_1",               treeName);
    DeclareVariable( b_jpt_2[ch],               "jpt_2",                treeName);
    DeclareVariable( b_jeta_2[ch],              "jeta_2",               treeName);
    DeclareVariable( b_jphi_2[ch],              "jphi_2",               treeName);
    
    DeclareVariable( b_bpt_1[ch],               "bpt_1",                treeName);
    DeclareVariable( b_beta_1[ch],              "beta_1",               treeName);
    DeclareVariable( b_bphi_1[ch],              "bphi_1",               treeName);
    DeclareVariable( b_bcsv_1[ch],              "bcsv_1",               treeName);
    DeclareVariable( b_bpt_2[ch],               "bpt_2",                treeName);
    DeclareVariable( b_beta_2[ch],              "beta_2",               treeName);
    DeclareVariable( b_bphi_2[ch],              "bphi_2",               treeName);
    DeclareVariable( b_bcsv_2[ch],              "bcsv_2",               treeName);
    
    DeclareVariable( b_met[ch],                 "met",                  treeName);
    DeclareVariable( b_metphi[ch],              "metphi",               treeName);
    DeclareVariable( b_puppimet[ch],            "puppimet",             treeName);
    DeclareVariable( b_puppimetphi[ch],         "puppimetphi",          treeName);
    
    DeclareVariable( b_fjpt_1[ch],              "fjpt_1",               treeName);
    DeclareVariable( b_fjeta_1[ch],             "fjeta_1",              treeName);
    DeclareVariable( b_fjpt_2[ch],              "fjpt_2",               treeName);
    DeclareVariable( b_fjeta_2[ch],             "fjeta_2",              treeName);
    
    DeclareVariable( b_jpt_1_nom[ch],           "jpt_1_nom",            treeName);
    DeclareVariable( b_jeta_1_nom[ch],          "jeta_1_nom",           treeName);
    DeclareVariable( b_jpt_2_nom[ch],           "jpt_2_nom",            treeName);
    DeclareVariable( b_jeta_2_nom[ch],          "jeta_2_nom",           treeName);
    DeclareVariable( b_jpt_1_uo[ch],            "jpt_1_uo",             treeName);
    DeclareVariable( b_jeta_1_uo[ch],           "jeta_1_uo",            treeName);
    DeclareVariable( b_jpt_2_uo[ch],            "jpt_2_uo",             treeName);
    DeclareVariable( b_jeta_2_uo[ch],           "jeta_2_uo",            treeName);
    
    DeclareVariable( b_njets_nom[ch],           "njets_nom",            treeName);
    DeclareVariable( b_njets20_nom[ch],         "njets20_nom",          treeName);
    DeclareVariable( b_ncjets_nom[ch],          "ncjets_nom",           treeName);
    DeclareVariable( b_ncbtag_nom[ch],          "ncbtag_nom",           treeName);
    DeclareVariable( b_nfjets_nom[ch],          "nfjets_nom",           treeName);
    DeclareVariable( b_met_nom[ch],             "met_nom",              treeName);
    DeclareVariable( b_pfmt_1_nom[ch],          "pfmt_1_nom",           treeName);
    DeclareVariable( b_dphi_ll_bj_nom[ch],      "dphi_ll_bj_nom",       treeName);
    
    if (m_doJEC){
      DeclareVariable( b_puweight_Up[ch],           "puweight_Up",          treeName);
      DeclareVariable( b_puweight_Down[ch],         "puweight_Down",        treeName);
      
      DeclareVariable( b_njets_jesUp[ch],           "njets_jesUp",          treeName);
      DeclareVariable( b_njets_jesDown[ch],         "njets_jesDown",        treeName);
      DeclareVariable( b_njets_jerUp[ch],           "njets_jerUp",          treeName);
      DeclareVariable( b_njets_jerDown[ch],         "njets_jerDown",        treeName);
      DeclareVariable( b_njets20_jesUp[ch],         "njets20_jesUp",        treeName);
      DeclareVariable( b_njets20_jesDown[ch],       "njets20_jesDown",      treeName);
      DeclareVariable( b_njets20_jerUp[ch],         "njets20_jerUp",        treeName);
      DeclareVariable( b_njets20_jerDown[ch],       "njets20_jerDown",      treeName);
      DeclareVariable( b_ncjets_jesUp[ch],          "ncjets_jesUp",         treeName);
      DeclareVariable( b_ncjets_jesDown[ch],        "ncjets_jesDown",       treeName);
      DeclareVariable( b_ncjets_jerUp[ch],          "ncjets_jerUp",         treeName);
      DeclareVariable( b_ncjets_jerDown[ch],        "ncjets_jerDown",       treeName);
      DeclareVariable( b_ncbtag_jesUp[ch],          "ncbtag_jesUp",         treeName);
      DeclareVariable( b_ncbtag_jesDown[ch],        "ncbtag_jesDown",       treeName);
      DeclareVariable( b_ncbtag_jerUp[ch],          "ncbtag_jerUp",         treeName);
      DeclareVariable( b_ncbtag_jerDown[ch],        "ncbtag_jerDown",       treeName);
      DeclareVariable( b_nfjets_jesUp[ch],          "nfjets_jesUp",         treeName);
      DeclareVariable( b_nfjets_jesDown[ch],        "nfjets_jesDown",       treeName);
      DeclareVariable( b_nfjets_jerUp[ch],          "nfjets_jerUp",         treeName);
      DeclareVariable( b_nfjets_jerDown[ch],        "nfjets_jerDown",       treeName);
      
      DeclareVariable( b_jpt_1_jesUp[ch],           "jpt_1_jesUp",          treeName);
      DeclareVariable( b_jpt_1_jesDown[ch],         "jpt_1_jesDown",        treeName);
      DeclareVariable( b_jpt_1_jerUp[ch],           "jpt_1_jerUp",          treeName);
      DeclareVariable( b_jpt_1_jerDown[ch],         "jpt_1_jerDown",        treeName);
      DeclareVariable( b_jeta_1_jesUp[ch],          "jeta_1_jesUp",         treeName);
      DeclareVariable( b_jeta_1_jesDown[ch],        "jeta_1_jesDown",       treeName);
      DeclareVariable( b_jeta_1_jerUp[ch],          "jeta_1_jerUp",         treeName);
      DeclareVariable( b_jeta_1_jerDown[ch],        "jeta_1_jerDown",       treeName);
      DeclareVariable( b_jpt_2_jesUp[ch],           "jpt_2_jesUp",          treeName);
      DeclareVariable( b_jpt_2_jesDown[ch],         "jpt_2_jesDown",        treeName);
      DeclareVariable( b_jpt_2_jerUp[ch],           "jpt_2_jerUp",          treeName);
      DeclareVariable( b_jpt_2_jerDown[ch],         "jpt_2_jerDown",        treeName);
      DeclareVariable( b_jeta_2_jesUp[ch],          "jeta_2_jesUp",         treeName);
      DeclareVariable( b_jeta_2_jesDown[ch],        "jeta_2_jesDown",       treeName);
      DeclareVariable( b_jeta_2_jerUp[ch],          "jeta_2_jerUp",         treeName);
      DeclareVariable( b_jeta_2_jerDown[ch],        "jeta_2_jerDown",       treeName);
      
      DeclareVariable( b_met_jesUp[ch],             "met_jesUp",            treeName);
      DeclareVariable( b_met_jesDown[ch],           "met_jesDown",          treeName);
      DeclareVariable( b_met_jerUp[ch],             "met_jerUp",            treeName);
      DeclareVariable( b_met_jerDown[ch],           "met_jerDown",          treeName);
      DeclareVariable( b_met_UncEnUp[ch],           "met_UncEnUp",          treeName);
      DeclareVariable( b_met_UncEnDown[ch],         "met_UncEnDown",        treeName);
      
      DeclareVariable( b_pfmt_1_jesUp[ch],          "pfmt_1_jesUp",         treeName);
      DeclareVariable( b_pfmt_1_jesDown[ch],        "pfmt_1_jesDown",       treeName);
      DeclareVariable( b_pfmt_1_jerUp[ch],          "pfmt_1_jerUp",         treeName);
      DeclareVariable( b_pfmt_1_jerDown[ch],        "pfmt_1_jerDown",       treeName);
      DeclareVariable( b_pfmt_1_UncEnUp[ch],        "pfmt_1_UncEnUp",       treeName);
      DeclareVariable( b_pfmt_1_UncEnDown[ch],      "pfmt_1_UncEnDown",     treeName);
      
      DeclareVariable( b_dphi_ll_bj_jesUp[ch],      "dphi_ll_bj_jesUp",     treeName);
      DeclareVariable( b_dphi_ll_bj_jesDown[ch],    "dphi_ll_bj_jesDown",   treeName);
      DeclareVariable( b_dphi_ll_bj_jerUp[ch],      "dphi_ll_bj_jerUp",     treeName);
      DeclareVariable( b_dphi_ll_bj_jerDown[ch],    "dphi_ll_bj_jerDown",   treeName);
      
      DeclareVariable( b_weightbtag_bcUp[ch],       "weightbtag_bcUp",      treeName);
      DeclareVariable( b_weightbtag_bcDown[ch],     "weightbtag_bcDown",    treeName);
      DeclareVariable( b_weightbtag_udsgUp[ch],     "weightbtag_udsgUp",    treeName);
      DeclareVariable( b_weightbtag_udsgDown[ch],   "weightbtag_udsgDown",  treeName);
    }
    
    DeclareVariable( b_m_vis[ch],               "m_vis",                treeName);
    DeclareVariable( b_pt_tt[ch],               "pt_tt",                treeName);
    DeclareVariable( b_pt_tt_vis[ch],           "pt_tt_vis",            treeName);
    
    DeclareVariable( b_m_sv[ch],                "m_sv",                 treeName);
    DeclareVariable( b_pt_tt_sv[ch],            "pt_tt_sv",             treeName);
    
    DeclareVariable( b_dR_ll[ch],               "dR_ll",                treeName);
    DeclareVariable( b_dR_ll_gen[ch],           "dR_ll_gen",            treeName);
    DeclareVariable( b_dphi_ll_bj[ch],          "dphi_ll_bj",           treeName);
    DeclareVariable( b_mt_tot[ch],              "mt_tot",               treeName);
    DeclareVariable( b_ht[ch],                  "ht",                   treeName);
    
    DeclareVariable( b_m_genboson[ch],          "m_genboson",           treeName);
    DeclareVariable( b_pt_genboson[ch],         "pt_genboson",          treeName);
    DeclareVariable( b_pt_top_1[ch],            "pt_top_1",             treeName);
    DeclareVariable( b_pt_top_2[ch],            "pt_top_2",             treeName);
    
    DeclareVariable( b_pzetamiss[ch],           "pzetamiss",            treeName);
    DeclareVariable( b_pzetavis[ch],            "pzetavis",             treeName);
    DeclareVariable( b_pzeta_disc[ch],          "pzeta_disc",           treeName);
    DeclareVariable( b_vbf_mjj[ch],             "vbf_mjj",              treeName);
    DeclareVariable( b_vbf_deta[ch],            "vbf_deta",             treeName);
    DeclareVariable( b_vbf_jdphi[ch],           "vbf_jdphi",            treeName);
    DeclareVariable( b_vbf_ncentral[ch],        "vbf_ncentral",         treeName);
    DeclareVariable( b_vbf_ncentral20[ch],      "vbf_ncentral20",       treeName);
    
  }
  
  
  // MARK: Histograms
  m_logger << INFO << "Declaring histograms" << SLogger::endmsg;
  
  // histograms - cutflow
  for (auto ch: channels_){
    TString hname = "cutflow_" + ch;
    TString dirname = "histogram_" + ch;
    TString tch = ch;
    //std::cout << hname << " " << dirname << std::endl;
    Book( TH1F(hname, hname, 12, 0.5, 12.5 ), dirname);
  }
  
  if (!m_isData) m_PileupReweightingTool.BeginInputData( id, m_MC_V1 );
  else{
    TObject* grl;
    if( ! ( grl = GetConfigObject( "MyGoodRunsList" ) ) ) {
      m_logger << FATAL << "Can't access the GRL!" << SLogger::endmsg;
      throw SError( "Can't access the GRL!", SError::SkipCycle );
    }
    m_grl = *( dynamic_cast< Root::TGoodRunsList* >( grl ) );
  }
  
  m_BTaggingScaleTool.BeginInputData( id );
  m_BTaggingScaleTool.bookHistograms(); // to measure b tag efficiencies for our selections
  m_ScaleFactorTool.BeginInputData( id );
  m_JetCorrectionTool.BeginInputData( id );
  m_RecoilCorrector.BeginInputData( id );
  m_SVFitTool.BeginInputData( id );
  
  return;
  
}





void TauTauAnalysis::EndInputData( const SInputData& ) throw( SError ) {
  //std::cout << "EndInputData" << std::endl;
  m_logger << INFO << " " << SLogger::endmsg;
  m_logger << INFO << "EndInputData" << SLogger::endmsg;
  m_logger << INFO << " " << SLogger::endmsg;
  
  for(auto ch: channels_) {
    m_logger << INFO << "cut flow for " << ch << SLogger::endmsg;
    m_logger << INFO << Form( "Cut\t%25.25s\tEvents\tRelEff\tAbsEff", "Name" ) << SLogger::endmsg;
    
    TString hname = "cutflow_" + ch;
    TString dirname = "histogram_" + ch;
    Double_t ntot = Hist(hname, dirname)->GetBinContent( 1 );
    Hist(hname, dirname)->GetXaxis()->SetBinLabel(1,kCutName[0].c_str());
    Hist(hname, dirname)->GetXaxis()->SetLabelSize(0.044);
    m_logger << INFO << Form( "\t%25.25s\t%6.0f", "start", ntot ) << SLogger::endmsg;
    for( Int_t ibin = 1; ibin < kNumCuts; ++ibin ) {
      Int_t    icut    = ibin;
      Double_t nevents = Hist(hname, dirname)->GetBinContent( ibin+1 );
      Double_t releff  = 100. * nevents / Hist(hname, dirname)->GetBinContent( ibin );
      Double_t abseff  = 100. * nevents / ntot;
      m_logger << INFO  << Form( "C%d\t%25.25s\t%6.0f\t%6.2f\t%6.2f", icut-1, kCutName[icut].c_str(), nevents, releff, abseff ) << SLogger::endmsg;
      Hist(hname, dirname)->GetXaxis()->SetBinLabel(ibin+1, kCutName[icut].c_str());
    }
    
    m_logger << INFO << " " << SLogger::endmsg;
    
  }

  return;
}


void TauTauAnalysis::BeginInputFile( const SInputData& ) throw( SError ) {
//   std::cout << "BeginInputFile" << std::endl;

  m_logger << INFO << "Connecting input variables" << SLogger::endmsg;
  if (m_isData) {
    //std::cout << "connect variables for data" << std::endl;
    m_jetAK4.ConnectVariables(      m_recoTreeName.c_str(), Ntuple::JetBasic|Ntuple::JetAnalysis, (m_jetAK4Name + "_").c_str() );
    m_eventInfo.ConnectVariables(   m_recoTreeName.c_str(), Ntuple::EventInfoBasic|Ntuple::EventInfoTrigger, "" );
  }
  else {
    //std::cout << "connect variables for MC" << std::endl;
    m_jetAK4.ConnectVariables(      m_recoTreeName.c_str(), Ntuple::JetBasic|Ntuple::JetAnalysis|Ntuple::JetTruth|Ntuple::JetJER, (m_jetAK4Name + "_").c_str() );
    m_genJetAK4.ConnectVariables(   m_recoTreeName.c_str(), Ntuple::GenJetak4Truth,"");
    m_eventInfo.ConnectVariables(   m_recoTreeName.c_str(), Ntuple::EventInfoBasic|Ntuple::EventInfoTrigger|Ntuple::EventInfoTruth, "" );
    m_genParticle.ConnectVariables( m_recoTreeName.c_str(), Ntuple::GenParticleBasic|Ntuple::GenParticleTauDecayAnalysis, (m_genParticleName + "_").c_str() );
  }
  m_electron.ConnectVariables(      m_recoTreeName.c_str(), Ntuple::ElectronBasic|Ntuple::ElectronID|Ntuple::ElectronAdvancedID|Ntuple::ElectronBoostedIsolation|Ntuple::ElectronSuperCluster, (m_electronName + "_").c_str() );
  m_muon.ConnectVariables(          m_recoTreeName.c_str(), Ntuple::MuonBasic|Ntuple::MuonID|Ntuple::MuonIsolation|Ntuple::MuonTrack|Ntuple::MuonBoostedIsolation, (m_muonName + "_").c_str() );
  m_tau.ConnectVariables(           m_recoTreeName.c_str(), Ntuple::TauBasic|Ntuple::TauID|Ntuple::TauAdvancedID, (m_tauName + "_").c_str() );
  
  m_missingEt.ConnectVariables(     m_recoTreeName.c_str(), Ntuple::MissingEtBasic|Ntuple::MissingEtAnalysis|Ntuple::MissingEtAnalysisSyst|Ntuple::MissingEtCovAnalysis, (m_missingEtName + "_").c_str() );
  m_puppimissingEt.ConnectVariables(m_recoTreeName.c_str(), Ntuple::MissingEtBasic, (m_missingEtName + "_puppi_").c_str() );
  //m_mvamissingEt.ConnectVariables(  m_recoTreeName.c_str(), Ntuple::MissingEtBasic|Ntuple::MissingEtMVAAnalysis|Ntuple::MissingEtCovAnalysis, (m_missingEtName + "_mva_").c_str() );
  
  m_logger << INFO << "Connecting input variables completed" << SLogger::endmsg;
  
  return;

}





void TauTauAnalysis::ExecuteEvent( const SInputData&, Double_t ) throw( SError ) {
  //std::cout << "ExecuteEvent" << std::endl;
  //m_logger << VERBOSE << "ExecuteEvent" << SLogger::endmsg;
  
  b_weight_     =  1.;
  b_puweight_   =  1.;
  b_genweight_  = (m_isData) ? 1 : m_eventInfo.genEventWeight;
  b_npu_        = -1.;
  
  
  // Cut 0: no cuts
  for (auto ch: channels_){
    fillCutflow("cutflow_" + ch, "histogram_" + ch, kBeforeCuts, 1);
    fillCutflow("cutflow_" + ch, "histogram_" + ch, kBeforeCutsUnweighted, 1 );
    fillCutflow("cutflow_" + ch, "histogram_" + ch, kBeforeCutsWeighted, b_genweight_ );
    b_channel[ch] = 0;
  }
  if (m_isData){
    if(m_eventInfo.PV_N<=0) throw SError( SError::SkipEvent );
  }else{
    for (auto ch: channels_)
      fillCutflow("cutflow_" + ch, "histogram_" + ch, kJSON, 1);
    getEventWeight();
  }
  for (auto ch: channels_){
    fillCutflow("cutflow_" + ch, "histogram_" + ch, kNo0PUUnweighted, 1);
    fillCutflow("cutflow_" + ch, "histogram_" + ch, kNo0PUWeighted, b_genweight_);
  }
  
  
  // Cut 1: check for data if run/lumiblock in JSON
  if (m_isData){
    if(!(isGoodEvent(m_eventInfo.runNumber, m_eventInfo.lumiBlock))) throw SError( SError::SkipEvent );
    for (auto ch: channels_){
      fillCutflow("cutflow_" + ch, "histogram_" + ch, kJSON, 1);
    }
  }
  
  
  // Cut 2: pass trigger
  //std::cout << ">>> ExecuteEvent - Cut 2" << std::endl;
  m_trigger_Flags = passTrigger();
  if(m_trigger_Flags == "none"){
    throw SError( SError::SkipEvent );
  }
  
  for (auto ch: channels_){
    fillCutflow("cutflow_" + ch, "histogram_" + ch, kTrigger, 1);
  }
  
  
  // Cut 4: lepton (muon)
  std::vector<UZH::Muon> goodMuons;
  for( int i = 0; i < m_muon.N; ++i ){
    UZH::Muon mymuon( &m_muon, i );
    
    if(mymuon.pt() < m_muonPtCut) continue;
    if(fabs(mymuon.eta()) > m_muonEtaCut) continue;
    if(fabs(mymuon.d0_allvertices()) > m_muonD0Cut) continue;
    if(fabs(mymuon.dz_allvertices()) > m_muonDzCut) continue;
    if(mymuon.isMediumMuonGH() < 0.5) continue;   // for period GH and MC (see AN)
    //if (mymuon.SemileptonicPFIso() / mymuon.pt() > m_muonIsoCut) continue;
    
    goodMuons.push_back(mymuon);
  }
  
  if(goodMuons.size()!=0){
    fillCutflow("cutflow_emu", "histogram_emu", kLepton, 1);
  }else throw SError(SError::SkipEvent);
  
  
  // Cut 4: lepton (electron)
  std::vector<UZH::Electron> goodElectrons;
  for ( int i = 0; i < m_electron.N; ++i ) {
    UZH::Electron myelectron( &m_electron, i );
    
    Float_t elept = myelectron.pt();
    //if(!m_isData and m_doEES){
    //  if(fabs(myelectron.eta())<1.479) elept *= (1+m_EESshift);
    //  else                             elept *= (1+m_EESshiftEndCap);
    //}
    
    if(elept < m_electronPtCut) continue;
    if(fabs(myelectron.eta()) > m_electronEtaCut) continue;
    if(fabs(myelectron.d0_allvertices()) > m_electronD0Cut) continue;
    if(fabs(myelectron.dz_allvertices()) > m_electronDzCut) continue;
    if(myelectron.passConversionVeto()!=1) continue;
    if(myelectron.expectedMissingInnerHits()>1) continue;
    if(myelectron.isMVATightElectron() < 0.5) continue;
    //if (myelectron.SemileptonicPFIso() / myelectron.pt() > m_electronIsoCut) continue;
	
    goodElectrons.push_back(myelectron);
  }
  
  if(goodElectrons.size()!=0){
    fillCutflow("cutflow_emu", "histogram_emu", kOtherLepton, 1);
  }else throw SError(SError::SkipEvent);
    
  
  // Cut 6: lepton - lepton pair

  std::vector<ll_pair> emu_pair;
  bool passedDeltaRCut = false; // check
  for(int imuon=0; imuon < (int)goodMuons.size(); imuon++){
    //if(m_trigger_Flags == "none") break; // trigger
    for(int ielectron=0; ielectron < (int)goodElectrons.size(); ielectron++){
      
      Float_t dR = goodMuons[imuon].tlv().DeltaR(goodElectrons[ielectron].tlv());
      if(dR < 0.3) continue; // remove or lower for boosted ID
      
      Float_t mupt = goodMuons[imuon].pt();
      Float_t reliso = goodMuons[imuon].SemileptonicPFIso() / mupt;
      Float_t ept = goodElectrons[ielectron].pt();      
      //Float_t eiso = goodElectrons[ielectron].SemileptonicPFIso() / ept;
      Float_t eiso = goodElectrons[ielectron].relIsoWithDBeta();
      ll_pair pair = {imuon, reliso, mupt, ielectron, eiso, ept, dR};

      //bool isMatch = TrigMatch(goodMuons[imuon], goodElectrons[ielectron], m_eventInfo.runNumber);
      //if(isMatch==false) continue;

      emu_pair.push_back(pair);
    }
  }
  
  if(emu_pair.size()==0){
    throw SError( SError::SkipEvent );
  }
  

  UZH::MissingEt Met( &m_missingEt, 0 );
  UZH::MissingEt PuppiMet( &m_puppimissingEt, 0 );
  
  // For e-mu
  if(emu_pair.size()!=0){
    
    fillCutflow("cutflow_emu", "histogram_emu", kLepLep, 1);
    sort(emu_pair.begin(), emu_pair.end());
    
    // For Jets
    std::vector<UZH::Jet> goodJetsAK4;
    for ( int i = 0; i < (m_jetAK4.N); ++i ) {
      UZH::Jet myjetak4( &m_jetAK4, i );
      
      Float_t dr_mj = deltaR(myjetak4.eta() - goodMuons[emu_pair[0].ilepton].eta(), 
                      deltaPhi(myjetak4.phi(), goodMuons[emu_pair[0].ilepton].phi()));
      if(dr_mj < 0.5) continue;
      
      Float_t dr_ej = deltaR(myjetak4.eta() - goodElectrons[emu_pair[0].olepton].eta(), 
			     deltaPhi(myjetak4.phi(), goodElectrons[emu_pair[0].olepton].phi()));
      if(dr_ej < 0.5) continue;
      
      if (fabs(myjetak4.eta()) > m_AK4jetEtaCut) continue;
      if (myjetak4.pt() < m_AK4jetPtCut) continue;
      if (!LooseJetID(myjetak4)) continue; // !myjetak4.IDLoose()
      
      goodJetsAK4.push_back(myjetak4);
    }
    
    if(!m_isData and emu_pair[0].lep_iso<0.2 && emu_pair[0].olep_iso<0.15){
      m_BTaggingScaleTool.fillEfficiencies(goodJetsAK4); // to measure b tag efficiencies for our selections
    }
    
    FillBranches( "emu", goodMuons[emu_pair[0].ilepton], goodElectrons[emu_pair[0].olepton], goodJetsAK4, Met, PuppiMet );//, MvaMet);
    e_mu++;
    
  }
  
  return;
  
}





bool TauTauAnalysis::isGoodEvent(int runNumber, int lumiSection) {
//   std::cout << "isGoodEvent" << std::endl;
  
  bool isGood = true;
  if (m_isData) {
    isGood = m_grl.HasRunLumiBlock( runNumber, lumiSection );
    //if( !isGood ) {
      //m_logger << WARNING << "Bad event! Run: " << runNumber <<  " - Lumi Section: " << lumiSection << SLogger::endmsg;
      //throw SError( SError::SkipEvent );
    //}
    //else m_logger << VERBOSE << "Good event! Run: " << runNumber <<  " - Lumi Section: " << lumiSection << SLogger::endmsg;
  }
  
  return isGood;
  
}





TString TauTauAnalysis::passTrigger() {
  //std::cout << "TauTauAnalysis::passTrigger" << std::endl;
  
  // triggerFlag = mt22-mt24-mtx-mt-et25-et45-etx-et-e12mu23-e23mu8-em-
  std::string triggerFlags = ""; // std::to_string(pt)
  for (std::map<std::string,bool>::iterator it = (m_eventInfo.trigDecision)->begin(); it != (m_eventInfo.trigDecision)->end(); ++it){
    if (it->second){ 
      
      // mutau
      for( auto const& trigger: m_triggers_mutau ){
        if ((it->first).find(trigger) != std::string::npos){
          //std::cout << trigger->name << ": " << it->first << std::endl;
          triggerFlags += "mt24";
      }}
          
  }}
  
  if( triggerFlags == "" ) triggerFlags = "none";
  return triggerFlags;
  
}



void TauTauAnalysis::getEventWeight() {
  //std::cout << "getEventWeight" << std::endl;
  
  b_npu_ = -1.;
  for( unsigned int v = 0; v < (m_eventInfo.actualIntPerXing)->size(); ++v ){
    if ( (*m_eventInfo.bunchCrossing)[v] == 0 ) {
      b_npu_ = (*m_eventInfo.actualIntPerXing)[v]; // averageIntPerXing
      
      if(b_npu_<=0)
        throw SError( SError::SkipEvent );
      
      b_puweight_ = m_PileupReweightingTool.getPileUpweight( b_npu_ );
      break;
    }
  }
  
  b_weight_ *= b_puweight_*b_genweight_;
  
}





void TauTauAnalysis::fillCutflow(TString histName, TString dirName, const Int_t id, const Double_t weight){
  Hist( histName, dirName )->Fill( id+1, weight );
}





void TauTauAnalysis::FillBranches(const std::string& channel,
                                  const UZH::Muon& muon, const UZH::Electron& electron,
                                  std::vector<UZH::Jet> &Jets, UZH::MissingEt& met, UZH::MissingEt& puppimet){//, const UZH::MissingEt& mvamet){
  //std::cout << "FillBranches" << std::endl;
  
  const char* ch = channel.c_str();
  if(m_doRecoilCorr || m_doZpt) setGenBosonTLVs(); // only for HTT, DY and WJ
  
  b_weight[ch]      = b_weight_;
  b_genweight[ch]   = b_genweight_;
  b_puweight[ch]    = b_puweight_;
  b_evt[ch]         = m_eventInfo.eventNumber;
  b_run[ch]         = m_eventInfo.runNumber;
  b_lum[ch]         = m_eventInfo.lumiBlock;
  b_isData[ch]      = (Int_t) m_isData;
  
  b_npu[ch]         = b_npu_; // for MC defined in getEventWeight
  b_npv[ch]         = m_eventInfo.PV_N;
  b_NUP[ch]         = m_eventInfo.lheNj;
  b_rho[ch]         = m_eventInfo.rho;
  
  if(m_doJEC){
    b_puweight_Up[ch]   = b_puweight_Up_;
    b_puweight_Down[ch] = b_puweight_Down_;  
  }
  
  
  
  
  ///////////////////
  // MARK: Leptons //
  ///////////////////
  
  b_pt_2[ch]                = electron.tlv().Pt();
  b_eta_2[ch]               = electron.tlv().Eta();
  b_phi_2[ch]               = electron.tlv().Phi();
  b_m_2[ch]                 = electron.tlv().M();
  b_q_2[ch]                 = electron.charge();
  b_d0_2[ch]                = electron.d0();
  b_dz_2[ch]                = electron.dz();
  b_iso_2[ch]               = electron.relIsoWithDBeta(); //electron.SemileptonicPFIso() / electron.tlv().Pt();
  
  b_id_e_mva_nt_loose_1[ch] = -1;
  extraLeptonVetos(channel, muon, electron);
  b_dilepton_veto[ch]       = b_dilepton_veto_;
  b_extraelec_veto[ch]      = b_extraelec_veto_;
  b_extramuon_veto[ch]      = b_extramuon_veto_;
  b_lepton_vetos[ch]        = ( b_dilepton_veto_ || b_extraelec_veto_ || b_extramuon_veto_ );
  b_iso_cuts[ch]            = b_iso_1[ch]<0.2 && b_iso_2[ch]<0.15;
  
  b_pt_1[ch]                = muon.tlv().Pt();
  b_eta_1[ch]               = muon.tlv().Eta();
  b_phi_1[ch]               = muon.tlv().Phi();
  b_m_1[ch]                 = muon.tlv().M();
  b_q_1[ch]                 = muon.charge();
  b_d0_1[ch]                = muon.d0();
  b_dz_1[ch]                = muon.dz();
  b_iso_1[ch]               = muon.SemileptonicPFIso() / muon.pt();
  b_channel[ch]             = 1;
  TLorentzVector muon_tlv;
  muon_tlv.SetPtEtaPhiM(b_pt_1[ch], b_eta_1[ch], b_phi_1[ch], b_m_1[ch]);
  
  
  
  ////////////////
  // MARK: Taus //
  ////////////////
  
  float maxIso = -1.;
  int maxIndex = -1;
  for(int i=0; i<(m_tau.N); ++i){
    UZH::Tau tau( &m_tau, i );
    if(tau.byIsolationMVArun2v1DBoldDMwLTraw()<maxIso) continue;
    if(tau.DeltaR(electron)<0.5) continue;
    if(tau.DeltaR(muon)<0.5) continue;
    if(tau.pt() < m_tauPtCut) continue;
    if(abs(tau.eta()) > m_tauEtaCut) continue;
    if(tau.TauType() != 1) continue; // 1 for standard ID, 2 for boosted ID
    if(fabs(tau.dz()) > m_tauDzCut) continue;
    if(tau.decayModeFinding() < 0.5 and tau.decayMode()!=11) continue;
    if(fabs(tau.charge()) != 1) continue; // remove for boosted ID
    //if(tau.againstElectronVLooseMVA6() < 0.5 or tau.againstMuonTight3() < 0.5) continue; // same WPs as mutau; needs SFs!
    maxIndex = i;
    maxIso   = tau.byIsolationMVArun2v1DBoldDMwLTraw();
  }
  if(maxIndex>0){
    UZH::Tau tau( &m_tau, maxIndex );
    b_pt_3[ch]                                          = tau.tlv().Pt();
    b_eta_3[ch]                                         = tau.tlv().Eta();
    b_decayMode_3[ch]                                   = tau.decayMode(); // 0, 1, 10
    b_againstLepton_3[ch]                               = tau.againstElectronVLooseMVA6() > 0.5 and tau.againstMuonTight3() > 0.5;
    if(m_isData) b_gen_match_3[ch] = -1;
    else         b_gen_match_3[ch] = genMatch(tau.eta(),tau.phi());
    // TODO: againstLepton SFs!
    b_byIsolationMVArun2v1DBoldDMwLTraw_3[ch]           = tau.byIsolationMVArun2v1DBoldDMwLTraw();
    b_byIsolationMVArun2v1DBnewDMwLTraw_3[ch]           = tau.byIsolationMVArun2v1DBnewDMwLTraw();
    b_byLooseIsolationMVArun2v1DBoldDMwLT_3[ch]         = tau.byLooseIsolationMVArun2v1DBoldDMwLT();
    b_byMediumIsolationMVArun2v1DBoldDMwLT_3[ch]        = tau.byMediumIsolationMVArun2v1DBoldDMwLT();
    b_byTightIsolationMVArun2v1DBoldDMwLT_3[ch]         = tau.byTightIsolationMVArun2v1DBoldDMwLT();
    b_byVTightIsolationMVArun2v1DBoldDMwLT_3[ch]        = tau.byVTightIsolationMVArun2v1DBoldDMwLT();
    b_byVVTightIsolationMVArun2v1DBoldDMwLT_3[ch]       = tau.byVVTightIsolationMVArun2v1DBoldDMwLT();
    b_byCombinedIsolationDeltaBetaCorrRaw3Hits_3[ch]    = tau.byCombinedIsolationDeltaBetaCorrRaw3Hits();
    b_byLooseCombinedIsolationDeltaBetaCorr3Hits_3[ch]  = tau.byLooseCombinedIsolationDeltaBetaCorr3Hits();
    b_byMediumCombinedIsolationDeltaBetaCorr3Hits_3[ch] = tau.byMediumCombinedIsolationDeltaBetaCorr3Hits();
    b_byTightCombinedIsolationDeltaBetaCorr3Hits_3[ch]  = tau.byTightCombinedIsolationDeltaBetaCorr3Hits();
  }else{
    b_pt_3[ch]                                          = -1;
    b_eta_3[ch]                                         = -9;
    b_decayMode_3[ch]                                   = -1;
    b_againstLepton_3[ch]                               = -1;
    b_gen_match_3[ch]                                   = -1;
    b_byIsolationMVArun2v1DBoldDMwLTraw_3[ch]           = -9;
    b_byIsolationMVArun2v1DBnewDMwLTraw_3[ch]           = -9;
    b_byLooseIsolationMVArun2v1DBoldDMwLT_3[ch]         = -1;
    b_byMediumIsolationMVArun2v1DBoldDMwLT_3[ch]        = -1;
    b_byTightIsolationMVArun2v1DBoldDMwLT_3[ch]         = -1;
    b_byVTightIsolationMVArun2v1DBoldDMwLT_3[ch]        = -1;
    b_byVVTightIsolationMVArun2v1DBoldDMwLT_3[ch]       = -1;
    b_byCombinedIsolationDeltaBetaCorrRaw3Hits_3[ch]    = -9;
    b_byLooseCombinedIsolationDeltaBetaCorr3Hits_3[ch]  = -1;
    b_byMediumCombinedIsolationDeltaBetaCorr3Hits_3[ch] = -1;
    b_byTightCombinedIsolationDeltaBetaCorr3Hits_3[ch]  = -1;
  }  
  
  
  
  //////////////////
  // MARK: Shifts //
  //////////////////
  //std::cout << ">>> Shifts " << std::endl;
  
  // RECOIL CORRECTIONS
  TLorentzVector met_tlv;
  met_tlv.SetPxPyPzE(met.et()*TMath::Cos(met.phi()), met.et()*TMath::Sin(met.phi()), 0, met.et());
  TLorentzVector met_tlv_corrected;
  //TLorentzVector mvamet_tlv_corrected;
  if(m_doRecoilCorr){
    met_tlv_corrected = m_RecoilCorrector.CorrectPFMETByMeanResolution( met_tlv.Px(),       met_tlv.Py(),
									                                    boson_tlv.Px(),     boson_tlv.Py(),
									                                    boson_tlv_vis.Px(), boson_tlv_vis.Py(),
									                                    m_jetAK4.N ); //m_eventInfo.lheNj
    b_m_genboson[ch]  = boson_tlv.M();
    b_pt_genboson[ch] = boson_tlv.Pt();
  }else{
    met_tlv_corrected = met_tlv;
  }
  
  // SHIFTS
  // apply shifts to tau_tlv_shifted, lep_tlv_shifted, met_tlv_corrected
  //std::cout << ">>> Shifts " << std::endl;
  TLorentzVector electron_tlv; //_shifted
  electron_tlv.SetPtEtaPhiM(b_pt_2[ch], b_eta_2[ch], b_phi_2[ch], b_m_2[ch]);
  if(!m_isData and m_doEES){ // Electron Energy Scale
    if(fabs(electron.tlv().Eta())<1.479) shiftLeptonAndMET(m_EESshift,      electron_tlv,met_tlv_corrected);
    else                                 shiftLeptonAndMET(m_EESshiftEndCap,electron_tlv,met_tlv_corrected);
    b_pt_2[ch]    = electron_tlv.Pt();
    b_m_2[ch]     = electron_tlv.M();
  }
  //printRow({"after"},{tau_tlv.Pt(),tau_tlv.M()});
  
  // save corrections to UZH::MET object  
  met.et(met_tlv_corrected.E());
  met.phi(met_tlv_corrected.Phi());
  
  
  
  ////////////////
  // MARK: Jets //
  ////////////////
  
  //if(b_isolated_) std::sort(Jets.begin(), Jets.end(), UZH::sortJetPt() );
  float fmet_nom    = met.et();
  float fmetphi_nom = met.phi();
  FillJetBranches( ch, Jets, met, electron_tlv, muon_tlv );
  
  
  
  ///////////////
  // MARK: MET //
  ///////////////
  
  //met_tlv.SetPxPyPzE(met.et()*TMath::Cos(met.phi()), met.et()*TMath::Sin(met.phi()), 0, met.et());
  float fmet         = met.et();  //met_tlv_corrected.E();
  float fmetphi      = met.phi(); //met_tlv_corrected.Phi();
  float fpuppimet    = puppimet.et();
  float fpuppimetphi = puppimet.phi();
  
  b_met[ch]         = fmet;
  b_met_nom[ch]     = fmet_nom;
  b_metphi[ch]      = fmetphi;
  b_puppimet[ch]    = fpuppimet;
  b_puppimetphi[ch] = fpuppimetphi;
  
  b_pfmt_1[ch]      = TMath::Sqrt(2*muon_tlv.Pt()*fmet*(      1-TMath::Cos(deltaPhi(muon_tlv.Phi(), fmetphi      ))));
  b_puppimt_1[ch]   = TMath::Sqrt(2*muon_tlv.Pt()*fpuppimet*( 1-TMath::Cos(deltaPhi(muon_tlv.Phi(), fpuppimetphi ))));
  b_pfmt_1_nom[ch]  = TMath::Sqrt(2*muon_tlv.Pt()*fmet_nom*(  1-TMath::Cos(deltaPhi(muon_tlv.Phi(),  fmetphi_nom  ))));
  
  b_pfmt_2[ch]      = TMath::Sqrt(2*electron_tlv.Pt()*fmet*(      1-TMath::Cos(deltaPhi(electron_tlv.Phi(), fmetphi      ))));
  b_puppimt_2[ch]   = TMath::Sqrt(2*electron_tlv.Pt()*fpuppimet*( 1-TMath::Cos(deltaPhi(electron_tlv.Phi(), fpuppimetphi ))));
  
  // discriminating variables
  b_m_vis[ch]       = (muon_tlv + electron_tlv).M();
  b_pt_tt[ch]       = (muon_tlv + electron_tlv + met_tlv_corrected).Pt();
  b_pt_tt_vis[ch]   = (muon_tlv + electron_tlv).Pt();
  b_dR_ll[ch]       = electron_tlv.DeltaR(muon_tlv);
  b_dR_ll_gen[ch]   = b_dR_ll_gen_;
  b_mt_tot[ch]      = TMath::Sqrt(TMath::Power(b_pfmt_1[ch],2) + TMath::Power(b_pfmt_2[ch],2) + 2*muon_tlv.Pt()*b_pt_2[ch]*(1-TMath::Cos(deltaPhi(muon_tlv.Phi(), b_phi_2[ch]))));
  
  // discriminating variables
  TVector3 leg1(muon_tlv.Px(), muon_tlv.Py(), 0.);
  TVector3 leg2(electron_tlv.Px(), electron_tlv.Py(), 0.);
  TVector3 metleg(met_tlv_corrected.Px(), met_tlv_corrected.Py(), 0.);
  TVector3 zetaAxis = (leg1.Unit() + leg2.Unit()).Unit();
  Float_t pZetaVis_ = leg1*zetaAxis + leg2*zetaAxis;
  Float_t pZetaMET_ = metleg*zetaAxis;
  b_pzetamiss[ch]   = pZetaMET_;
  b_pzetavis[ch]    = pZetaVis_;
  b_pzeta_disc[ch]  = pZetaMET_ - 0.5*pZetaVis_;
  
  if(m_doJEC){ // no need to substract shifts from met, use shifts available in ntuple instead:
    TLorentzVector met_nom, met_jesUp, met_jesDown, met_jerUp, met_jerDown, met_UncEnUp, met_UncEnDown;
    //met_nom.SetPtEtaPhiE(       fmet_nom,                           0., fmetphi_nom, fmet_nom                           );
    met_jesUp.SetPtEtaPhiE(     fmet_nom * met.JetEnUp(),           0., fmetphi_nom, fmet_nom * met.JetEnUp()           );
    met_jesDown.SetPtEtaPhiE(   fmet_nom * met.JetEnDown(),         0., fmetphi_nom, fmet_nom * met.JetEnDown()         );
    met_jerUp.SetPtEtaPhiE(     fmet_nom * met.JetResUp(),          0., fmetphi_nom, fmet_nom * met.JetResUp()          );
    met_jerDown.SetPtEtaPhiE(   fmet_nom * met.JetResDown(),        0., fmetphi_nom, fmet_nom * met.JetResDown()        );
    met_UncEnUp.SetPtEtaPhiE(   fmet_nom * met.UnclusteredEnUp(),   0., fmetphi_nom, fmet_nom * met.UnclusteredEnUp()   );
    met_UncEnDown.SetPtEtaPhiE( fmet_nom * met.UnclusteredEnDown(), 0., fmetphi_nom, fmet_nom * met.UnclusteredEnDown() );
    b_met_jesUp[ch]        = met_jesUp.Et();
    b_met_jesDown[ch]      = met_jesDown.Et();
    b_met_jerUp[ch]        = met_jerUp.Et();
    b_met_jerDown[ch]      = met_jerDown.Et();
    b_met_UncEnUp[ch]      = met_UncEnUp.Et();
    b_met_UncEnDown[ch]    = met_UncEnDown.Et();
    b_pfmt_1_jesUp[ch]     = TMath::Sqrt( 2*muon_tlv.Pt()*met_jesUp.Et()    *( 1-TMath::Cos(deltaPhi(muon_tlv.Phi(),met_jesUp.Phi()     ))));
    b_pfmt_1_jesDown[ch]   = TMath::Sqrt( 2*muon_tlv.Pt()*met_jesDown.Et()  *( 1-TMath::Cos(deltaPhi(muon_tlv.Phi(),met_jesDown.Phi()   ))));
    b_pfmt_1_jerUp[ch]     = TMath::Sqrt( 2*muon_tlv.Pt()*met_jerUp.Et()    *( 1-TMath::Cos(deltaPhi(muon_tlv.Phi(),met_jerUp.Phi()     ))));
    b_pfmt_1_jerDown[ch]   = TMath::Sqrt( 2*muon_tlv.Pt()*met_jerDown.Et()  *( 1-TMath::Cos(deltaPhi(muon_tlv.Phi(),met_jerDown.Phi()   ))));
    b_pfmt_1_UncEnUp[ch]   = TMath::Sqrt( 2*muon_tlv.Pt()*met_UncEnUp.Et()  *( 1-TMath::Cos(deltaPhi(muon_tlv.Phi(),met_UncEnUp.Phi()   ))));
    b_pfmt_1_UncEnDown[ch] = TMath::Sqrt( 2*muon_tlv.Pt()*met_UncEnDown.Et()*( 1-TMath::Cos(deltaPhi(muon_tlv.Phi(),met_UncEnDown.Phi() ))));
  }
  
  
  
  
  
    
  ///////////////////
  // MARK: Weights //
  ///////////////////
  
  b_idisoweight_1[ch]       = 1.;
  b_trigweight_1[ch]        = 1.;
  b_trigweight_or_1[ch]     = 1.;
  b_idisoweight_2[ch]       = 1.;
  b_trigweight_2[ch]        = 1.;
  b_zptweight[ch]           = 1.;
  b_ttptweight[ch]          = 1.;
  b_weightbtag[ch]          = 1.;
  b_gen_match_1[ch]         = -1;
  b_gen_match_2[ch]         = -1;
  
  if (m_isData) b_gen_match_1[ch]   = -1;
  else{
    b_gen_match_1[ch]               = genMatch(b_eta_1[ch], b_phi_1[ch]);
    b_gen_match_2[ch]               = genMatch(b_eta_2[ch], b_phi_2[ch]);
    //b_trigweight_1[ch]              = m_ScaleFactorTool.get_ScaleFactor_EleMuTrig(b_pt_2[ch], fabs(b_eta_2[ch]), muon_tlv.Pt(),fabs(muon_tlv.Eta()), m_trigger_Flags);
    //b_trigweight_or_1[ch]           = m_ScaleFactorTool.get_ScaleFactor_EleMuTrig_OR(b_pt_2[ch], fabs(b_eta_2[ch]), muon_tlv.Pt(),fabs(muon_tlv.Eta()));
    //b_idisoweight_1[ch]             = m_ScaleFactorTool.get_ScaleFactor_MuIdIso(muon_tlv.Pt(),fabs(muon_tlv.Eta()));
    //b_idisoweight_2[ch]             = m_ScaleFactorTool.get_ScaleFactor_EleIdIso(b_pt_2[ch], fabs(b_eta_2[ch]));
    if(m_doZpt)  b_zptweight[ch]    = m_RecoilCorrector.ZptWeight( boson_tlv.M(), boson_tlv.Pt() );
    if(m_doTTpt) b_ttptweight[ch]   = genMatchSF(channel, -36); // 6*-6 = -36
    b_weightbtag[ch]                = b_weightbtag_; // do not apply b tag weight when using promote-demote method !!!
    //b_weightbtag[ch]                = m_BTaggingScaleTool.getScaleFactor_veto(Jets); // getScaleFactor_veto for AK4, getScaleFactor for AK8
    b_weight[ch] *= b_idisoweight_1[ch] * b_trigweight_2[ch] * b_idisoweight_2[ch] * b_zptweight[ch] * b_ttptweight[ch]; // * b_weightbtag[ch]
  }
  
  
  
  
  //////////////////
  // MARK: SVFit  //
  //////////////////
  //std::cout << ">>> SVFit" << std::endl;
  
  bool doSVFit = m_doSVFit and !(b_extraelec_veto_ or b_extramuon_veto_) and b_iso_1[ch]<0.50 and b_iso_2[ch]<0.50;
  if(m_doEES) doSVFit = doSVFit and b_ncbtag[ch]>0;
  
  double m_sv = -1;
  double pt_tt_sv = -1;
  if( doSVFit ){
    m_SVFitTool.addMeasuredLeptonTau("emu",electron.tlv(),muon.tlv());
    m_SVFitTool.getSVFitMassAndPT(m_sv,pt_tt_sv,met_tlv_corrected.Px(),met_tlv_corrected.Py(),met.cov00(),met.cov10(),met.cov11());
  }
  b_m_sv[ch] = m_sv;
  b_pt_tt_sv[ch] = pt_tt_sv;
  
}







void TauTauAnalysis::FillJetBranches( const char* ch, std::vector<UZH::Jet>& Jets, UZH::MissingEt& met, const TLorentzVector& electron_tlv, const TLorentzVector& muon_tlv ){
  //std::cout << "FillJetBranches " << ch << std::endl;
  
  // jet multiplicities
  Int_t njets  = 0;    Int_t njets20  = 0;
  Int_t nfjets = 0;    Int_t nfjets20 = 0;
  Int_t ncjets = 0;    Int_t ncjets20 = 0;
  Int_t nbtag  = 0;    Int_t nbtag20  = 0;
  Int_t nfbtag = 0;    Int_t nfbtag20 = 0;
  Int_t ncbtag = 0;    Int_t ncbtag20 = 0;
  
  // JEC variables
  Int_t nfjets_nom     = 0;  Int_t ncjets_nom     = 0;  Int_t ncbtag_nom     = 0;  Int_t njets20_nom     = 0;
  Int_t nfjets_jesUp   = 0;  Int_t ncjets_jesUp   = 0;  Int_t ncbtag_jesUp   = 0;  Int_t njets20_jesUp   = 0;
  Int_t nfjets_jesDown = 0;  Int_t ncjets_jesDown = 0;  Int_t ncbtag_jesDown = 0;  Int_t njets20_jesDown = 0;
  Int_t nfjets_jerUp   = 0;  Int_t ncjets_jerUp   = 0;  Int_t ncbtag_jerUp   = 0;  Int_t njets20_jerUp   = 0;
  Int_t nfjets_jerDown = 0;  Int_t ncjets_jerDown = 0;  Int_t ncbtag_jerDown = 0;  Int_t njets20_jerDown = 0;
  
  // to compare to uncorrected "nominal" jets
  TLorentzVector jet1,         jet2,         // default jets (JER on top of JES)
                 jet1_uo,      jet2_uo,      // unordered
                 jet1_nom,     jet2_nom,     // nominal jets (JES, no JER)
                 jet1_jesUp,   jet2_jesUp,
                 jet1_jesDown, jet2_jesDown,
                 jet1_jerUp,   jet2_jerUp,
                 jet1_jerDown, jet2_jerDown,
                 fjet1,        fjet2,        // two leading forward jets
                 bjet1,        bjet2;        // two leading b tagged jets
  double bcsv1 = 0;
  double bcsv2 = 0;
  
  // for dphi_ll_bj: get two leading central jets, one of which b tagged
  TLorentzVector                    jet2_dphi,
                 bjet_dphi_nom,     jet2_dphi_nom,
                 bjet_dphi_jesUp,   jet2_dphi_jesUp,
                 bjet_dphi_jesDown, jet2_dphi_jesDown,
                 bjet_dphi_jerUp,   jet2_dphi_jerUp,
                 bjet_dphi_jerDown, jet2_dphi_jerDown;
  
  // doJEC
  bool doJEC = m_doJEC; //and;
  TLorentzVector dtlv_jer; // difference in pt after smearing, to propagate to met
  
  Float_t phi_ll = (electron_tlv + muon_tlv).Phi(); // for dphi_ll_bj
  Float_t ht     = electron_tlv.E() + muon_tlv.E(); // total scalar energy HT
  
  b_weightbtag_             = 1.;
  b_weightbtag_bcUp[ch]     = 1.; //m_BTaggingScaleTool.getScaleFactor(Jets,+1., 0.);
  b_weightbtag_bcDown[ch]   = 1.; //m_BTaggingScaleTool.getScaleFactor(Jets,-1., 0.);
  b_weightbtag_udsgUp[ch]   = 1.; //m_BTaggingScaleTool.getScaleFactor(Jets, 0.,+1.);
  b_weightbtag_udsgDown[ch] = 1.; //m_BTaggingScaleTool.getScaleFactor(Jets, 0.,-1.);
  
  // https://twiki.cern.ch/twiki/bin/viewauth/CMS/HiggsToTauTauWorking2016#Other_common_selections
  //if(Jets.size()>0) printRow({"ijet","jet pt","jerDown","jer","jerUp","jesDown","jesUp"});
  //printRow({"ijet","jer sf DOWN","jer sf","jer sf UP"},{},{},{},15);
  for( int ijet = 0; ijet < (int)Jets.size(); ++ijet ){ // already |eta|<4.7 jets
      
      // "nominal" jet
      Float_t abseta     = fabs(Jets.at(ijet).eta());
      Float_t pt         = Jets.at(ijet).pt();
      TLorentzVector jet = Jets.at(ijet).tlv();
      bool isBTagged     = getBTagWeight_promote_demote(Jets.at(ijet));
      countJets( jet, ncjets_nom, nfjets_nom, ncbtag_nom, bjet_dphi_nom, jet2_dphi_nom, isBTagged );
      
      // smeared jet
      if(m_isData){ // no SMEARING
        if(pt<m_AK4jetPtCut) continue;
        if(pt > jet1.Pt()){
          if(jet1.Pt()<20){ // jet1 is unset
            jet1 = jet;  jet1_nom = jet;
          }else{ // jet1 is already set - reorder
            jet2 = jet1; jet2_nom = jet1_nom; // reorder
            jet1 = jet;  jet1_nom = jet;
          }
        }else if(pt > jet2.Pt()){
            jet2 = jet;  jet2_nom = jet;
        }
      }else{
        TLorentzVector jet_jer;
        if(doJEC){ // do SMEARING and SHIFTS
        
          // b tag weight before smearing  
          if(pt>m_AK4jetPtCut){
            b_weightbtag_             *= m_BTaggingScaleTool.getScaleFactor(Jets.at(ijet));
            b_weightbtag_bcUp[ch]     *= m_BTaggingScaleTool.getScaleFactor(Jets.at(ijet),+1., 0.);
            b_weightbtag_bcDown[ch]   *= m_BTaggingScaleTool.getScaleFactor(Jets.at(ijet),-1., 0.);
            b_weightbtag_udsgUp[ch]   *= m_BTaggingScaleTool.getScaleFactor(Jets.at(ijet), 0.,+1.);
            b_weightbtag_udsgDown[ch] *= m_BTaggingScaleTool.getScaleFactor(Jets.at(ijet), 0.,-1.);
          }
          
          // get shifts
          std::vector<TLorentzVector> jets_jer = m_JetCorrectionTool.GetCorrectedJetJERShifted(Jets.at(ijet),m_genJetAK4);
          std::vector<TLorentzVector> jets_jes = m_JetCorrectionTool.GetCorrectedJet(Jets.at(ijet));
          jet_jer = jets_jer.at(0);
          TLorentzVector jet_jerUp(jets_jer.at(1)), jet_jerDown(jets_jer.at(2));
          TLorentzVector jet_jesUp(jets_jes.at(0)), jet_jesDown(jets_jes.at(1));
          pt = jet_jer.Pt();
          
          // reorder pt and save nominal & shifts
          if(pt > jet1.Pt()){
            if(jet1.Pt()<20){ // jet1 is unset
              jet1         = jet_jer;       jet1_nom     = jet;
              jet1_jerUp   = jet_jerUp;     jet1_jesUp   = jet_jesUp;
              jet1_jerDown = jet_jerDown;   jet1_jesDown = jet_jesDown;
            }else{ // jet1 is already set - reorder
              jet2         = jet1;          jet2_nom     = jet1_nom;     // reorder
              jet2_jerUp   = jet1_jerUp;    jet2_jesUp   = jet1_jesUp;   // reorder
              jet2_jerDown = jet1_jerDown;  jet2_jesDown = jet1_jesDown; // reorder
              jet1         = jet_jer;       jet1_nom     = jet;
              jet1_jerUp   = jet_jerUp;     jet1_jesUp   = jet_jesUp;
              jet1_jerDown = jet_jerDown;   jet1_jesDown = jet_jesDown;
            }
          }else if(pt > jet2.Pt()){
              jet2         = jet_jer;       jet2_nom     = jet;
              jet2_jerUp   = jet_jerUp;     jet2_jesUp   = jet_jesUp;
              jet2_jerDown = jet_jerDown;   jet2_jesDown = jet_jesDown;
          }
          
          // count shifted >30 GeV jets
          countJets( jet_jesUp,   ncjets_jesUp,   nfjets_jesUp,   ncbtag_jesUp,   bjet_dphi_jesUp,   jet2_dphi_jesUp,   isBTagged );
          countJets( jet_jesDown, ncjets_jesDown, nfjets_jesDown, ncbtag_jesDown, bjet_dphi_jesDown, jet2_dphi_jesDown, isBTagged );
          countJets( jet_jerUp,   ncjets_jerUp,   nfjets_jerUp,   ncbtag_jerUp,   bjet_dphi_jerUp,   jet2_dphi_jerUp,   isBTagged );
          countJets( jet_jerDown, ncjets_jerDown, nfjets_jerDown, ncbtag_jerDown, bjet_dphi_jerDown, jet2_dphi_jerDown, isBTagged );
        }else{ // do SMEARING only
          
          // b tag weight before smearing  
          if(pt>m_AK4jetPtCut)
            b_weightbtag_ *= m_BTaggingScaleTool.getScaleFactor(Jets.at(ijet));
          
          jet_jer = m_JetCorrectionTool.GetCorrectedJetJER(Jets.at(ijet),m_genJetAK4);
          pt = jet_jer.Pt();
                    
          // reorder pt
          if(pt > jet1.Pt()){
            if(jet1.Pt()<20){ // jet1 is unset
              jet1 = jet_jer; jet1_nom = jet;
            }else{ // jet1 is already set - reorder
              jet2 = jet1;    jet2_nom = jet1_nom; // reorder
              jet1 = jet_jer; jet1_nom = jet;
            }
          }
          else if(pt > jet2.Pt()){
              jet2 = jet_jer; jet2_nom = jet;
          }
          
        }
        
        if(pt<m_AK4jetPtCut){     // only count >20 GeV jets
            Jets.at(ijet).pt(pt); // correct UZH::Jet object's pt
            continue;
        }
        Jets.at(ijet).pt(pt); // correct UZH::Jet object's pt
        dtlv_jer = jet - jet_jer; // tlv difference
        jet = jet_jer;
        
      }
      if     ( ijet == 0 ){ jet1_uo = jet; } // unordered
      else if( ijet == 1 ){ jet2_uo = jet; }
      ht += Jets.at(ijet).e();
      
      // count jets
      if(abseta < 2.4){             // CENTRAL 20 GeV
        ncjets20++;                 //  jets
        if(pt > 30){                // CENTRAL 30 GeV
          if(isBTagged) ncbtag++;   //  btag
          ncjets++;                 //  jets
        }
        if(isBTagged){              // CENTRAL 20 GeV b jet
          ncbtag20++;               //  btag
          if(pt>bjet1.Pt()){ // bjet TLorentzVectors
            if(bjet1.Pt()<20){
              bjet1 = jet; // set bjet1 for first time
              bcsv1 = Jets.at(ijet).csv();
            }else{ // bjet1 is already set - reorder
              bjet2 = bjet1; bjet1 = jet;
              bcsv2 = bcsv1; bcsv1 = Jets.at(ijet).csv(); }
          }else if(pt>bjet2.Pt()){
              bjet2 = jet;
              bcsv2 = Jets.at(ijet).csv();
          }
        }
        if(pt>jet2_dphi.Pt() and bjet1!=jet){ jet2_dphi = jet; }
      }
      else if(abseta > 2.4){        // FORWARD 20 GeV
        if(isBTagged) nfbtag20++;   //  btag
        nfjets20++;                 //  jets
        if(pt > 30){                // FORWARD 30 GeV
          if(isBTagged) nfbtag++;   //  btag
          nfjets++;                 //  jets
        }
        if(pt>fjet1.Pt()){ // fjet TLorentzVectors
          if(jet1.Pt()<20){      fjet1 = jet; } // set fjet1 for first time
          else{   fjet2 = fjet1; fjet1 = jet; } // reorder
        }else if(pt>fjet2.Pt()){ fjet2 = jet; }
      }
  }
  
  // propagate smearing to MET
  if(!m_isData) shiftMET(dtlv_jer,met);
  
  // jet multiplicities
  njets         = ncjets + nfjets;    njets20         = ncjets20 + nfjets20;
  nbtag         = ncbtag + nfbtag;    nbtag20         = ncbtag20 + nfbtag20;
  b_njets[ch]   = njets;              b_njets20[ch]   = njets20;
  b_nfjets[ch]  = nfjets;             b_nfjets20[ch]  = nfjets20;
  b_ncjets[ch]  = ncjets;             b_ncjets20[ch]  = ncjets20;
  b_nbtag[ch]   = nbtag;              b_nbtag20[ch]   = nbtag20;
  b_ncbtag[ch]  = ncbtag;             b_ncbtag20[ch]  = ncbtag20;
  b_ht[ch]      = ht;
  
  // jet kinematics
  if(njets20 > 1){
    b_jpt_1[ch] = jet1.Pt();    b_jeta_1[ch] = jet1.Eta();    b_jphi_1[ch] = jet1.Phi();
    b_jpt_2[ch] = jet2.Pt();    b_jeta_2[ch] = jet2.Eta();    b_jphi_2[ch] = jet2.Phi();
    b_jpt_1_uo[ch] = jet1_uo.Pt();    b_jeta_1_uo[ch] = jet1_uo.Eta();
    b_jpt_2_uo[ch] = jet2_uo.Pt();    b_jeta_2_uo[ch] = jet2_uo.Eta();
  }
  else if(njets20 == 1){
    b_jpt_1[ch] = jet1.Pt();    b_jeta_1[ch] = jet1.Eta();    b_jphi_1[ch] = jet1.Phi();
    b_jpt_2[ch] = -1;           b_jeta_2[ch] = -9;            b_jphi_2[ch] = -9;
    b_jpt_1_uo[ch] = jet1_uo.Pt();    b_jeta_1_uo[ch] = jet1_uo.Eta();
    b_jpt_2_uo[ch] = -1;              b_jeta_2_uo[ch] = -9;
  }
  else{
    b_jpt_1[ch] = -1;           b_jeta_1[ch] = -9;            b_jphi_1[ch] = -9;
    b_jpt_2[ch] = -1;           b_jeta_2[ch] = -9;            b_jphi_2[ch] = -9;
    b_jpt_1_uo[ch] = -1;        b_jeta_1_uo[ch] = -9;
    b_jpt_2_uo[ch] = -1;        b_jeta_2_uo[ch] = -9;
  }
  
  // JEC
  if(doJEC){
    b_nfjets_jesUp[ch]   = nfjets_jesUp;      b_ncjets_jesUp[ch]   = ncjets_jesUp;     b_ncbtag_jesUp[ch]   = ncbtag_jesUp;  
    b_nfjets_jesDown[ch] = nfjets_jesDown;    b_ncjets_jesDown[ch] = ncjets_jesDown;   b_ncbtag_jesDown[ch] = ncbtag_jesDown;
    b_nfjets_nom[ch]     = nfjets_nom;        b_ncjets_nom[ch]     = ncjets_nom;       b_ncbtag_nom[ch]     = ncbtag_nom;    
    b_nfjets_jerUp[ch]   = nfjets_jerUp;      b_ncjets_jerUp[ch]   = ncjets_jerUp;     b_ncbtag_jerUp[ch]   = ncbtag_jerUp;  
    b_nfjets_jerDown[ch] = nfjets_jerDown;    b_ncjets_jerDown[ch] = ncjets_jerDown;   b_ncbtag_jerDown[ch] = ncbtag_jerDown;
    b_njets_jesUp[ch]    = nfjets_jesUp   + ncjets_jesUp;           //b_njets20_jesUp[ch]   = njets20_jesUp;
    b_njets_jesDown[ch]  = nfjets_jesDown + ncjets_jesDown;         //b_njets20_jesDown[ch] = njets20_jesDown;
    b_njets_nom[ch]      = nfjets_nom     + ncjets_nom;             //b_njets20_nom[ch]     = njets20_jer;
    b_njets_jerUp[ch]    = nfjets_jerUp   + ncjets_jerUp;           //b_njets20_jerUp[ch]   = njets20_jerUp;
    b_njets_jerDown[ch]  = nfjets_jerDown + ncjets_jerDown;         //b_njets20_jerDown[ch] = njets20_jerDown;
    FillJetBranches_JEC( b_jpt_1_nom[ch],     b_jeta_1_nom[ch],     jet1_nom,     jet1_nom.Pt()    >m_AK4jetPtCut);
    FillJetBranches_JEC( b_jpt_2_nom[ch],     b_jeta_2_nom[ch],     jet2_nom,     jet2_nom.Pt()    >m_AK4jetPtCut);
    FillJetBranches_JEC( b_jpt_1_jesUp[ch],   b_jeta_1_jesUp[ch],   jet1_jesUp,   jet1_jesUp.Pt()  >m_AK4jetPtCut);
    FillJetBranches_JEC( b_jpt_2_jesUp[ch],   b_jeta_2_jesUp[ch],   jet2_jesUp,   jet2_jesUp.Pt()  >m_AK4jetPtCut);
    FillJetBranches_JEC( b_jpt_1_jesDown[ch], b_jeta_1_jesDown[ch], jet1_jesDown, jet1_jesDown.Pt()>m_AK4jetPtCut);
    FillJetBranches_JEC( b_jpt_2_jesDown[ch], b_jeta_2_jesDown[ch], jet2_jesDown, jet2_jesDown.Pt()>m_AK4jetPtCut);
    FillJetBranches_JEC( b_jpt_1_jerUp[ch],   b_jeta_1_jerUp[ch],   jet1_jerUp,   jet1_jerUp.Pt()  >m_AK4jetPtCut);
    FillJetBranches_JEC( b_jpt_2_jerUp[ch],   b_jeta_2_jerUp[ch],   jet2_jerUp,   jet2_jerUp.Pt()  >m_AK4jetPtCut);
    FillJetBranches_JEC( b_jpt_1_jerDown[ch], b_jeta_1_jerDown[ch], jet1_jerDown, jet1_jerDown.Pt()>m_AK4jetPtCut);
    FillJetBranches_JEC( b_jpt_2_jerDown[ch], b_jeta_2_jerDown[ch], jet2_jerDown, jet2_jerDown.Pt()>m_AK4jetPtCut);
  }
  
  // forward jets
  if(fjet1.Pt() > m_AK4jetPtCut){
    b_fjpt_1[ch]    = fjet1.Pt();
    b_fjeta_1[ch]   = fjet1.Eta();
  }else{
    b_fjpt_1[ch]    = -1;
    b_fjeta_1[ch]   = -9;
  }
  if(fjet2.Pt() > m_AK4jetPtCut){
    b_fjpt_2[ch]    = fjet2.Pt();
    b_fjeta_2[ch]   = fjet2.Eta();
  }else{
    b_fjpt_2[ch]    = -1;
    b_fjeta_2[ch]   = -9;
  }
  
  // b jets
  if(bjet1.Pt() > m_AK4jetPtCut){
    b_bpt_1[ch]     = bjet1.Pt();
    b_beta_1[ch]    = bjet1.Eta();
    b_bphi_1[ch]    = bjet1.Phi();
    b_bcsv_1[ch]    = bcsv1;
  }else{
    b_bpt_1[ch]     = -1;
    b_beta_1[ch]    = -9;
    b_bphi_1[ch]    = -9;
    b_bcsv_1[ch]    = -1;
  }
  if(bjet2.Pt() > m_AK4jetPtCut){
    b_bpt_2[ch]     = bjet2.Pt();
    b_beta_2[ch]    = bjet2.Eta();
    b_bphi_2[ch]    = bjet2.Phi();
    b_bcsv_2[ch]    = bcsv2;
  }else{
    b_bpt_2[ch]     = -1;
    b_beta_2[ch]    = -9;
    b_bphi_2[ch]    = -9;
    b_bcsv_2[ch]    = -1;
  }
  
  // VBF
  if(b_njets[ch]>=2){
    b_vbf_mjj[ch]   = (jet1 + jet2).M();
    b_vbf_deta[ch]  = jet1.Eta() - jet2.Eta();
    b_vbf_jdphi[ch] = deltaPhi(jet1.Phi(), jet2.Phi());
    Float_t min_eta = jet1.Eta();
    Float_t max_eta = jet2.Eta();
    if(min_eta > max_eta){
      min_eta = jet2.Eta(); 
      max_eta = jet1.Eta(); 
    }
    int ncentral    = 0;
    int ncentral20  = 0;
    for( int ijet = 0; ijet < (int)Jets.size(); ++ijet ){
      Float_t jeteta = Jets.at(ijet).eta();
      Float_t jetpt  = Jets.at(ijet).pt();
      if(min_eta < jeteta && jeteta < max_eta){
        if(jetpt > 30.) ncentral++;
        if(jetpt > 20.) ncentral20++;
      }
    }
    b_vbf_ncentral[ch]   = ncentral;
    b_vbf_ncentral20[ch] = ncentral20;
  }else{
    b_vbf_mjj[ch]        = -1;
    b_vbf_deta[ch]       = -9;
    b_vbf_jdphi[ch]      = -9;
    b_vbf_ncentral[ch]   = -1;
    b_vbf_ncentral20[ch] = -1;
  }
  
  // Delta phi( lep+tau, bj+cj ) if there is one central b jet and one central jet
  // jet2_dphi = central jet that is not the same as leading b jet
  if(bjet1.Pt() > m_AK4jetPtCut and jet2_dphi.Pt() > m_AK4jetPtCut)
    b_dphi_ll_bj[ch] = fabs(deltaPhi( phi_ll, (bjet1+jet2_dphi).Phi() ));
  else
    b_dphi_ll_bj[ch] = -1;
  
  // Delta phi JEC
  if(doJEC){
    if( bjet_dphi_nom.Pt() > m_AK4jetPtCut and jet2_dphi_nom.Pt() > m_AK4jetPtCut )
      b_dphi_ll_bj_nom[ch] = fabs(deltaPhi( phi_ll, (bjet_dphi_nom+jet2_dphi_nom).Phi() ));
    else
      b_dphi_ll_bj_nom[ch] = -1;
    if( bjet_dphi_jesUp.Pt() > m_AK4jetPtCut and jet2_dphi_jesUp.Pt() > m_AK4jetPtCut )
      b_dphi_ll_bj_jesUp[ch] = fabs(deltaPhi( phi_ll, (bjet_dphi_jesUp+jet2_dphi_jesUp).Phi() ));
    else
      b_dphi_ll_bj_jesUp[ch] = -1;
    if( bjet_dphi_jesDown.Pt() > m_AK4jetPtCut and jet2_dphi_jesDown.Pt() > m_AK4jetPtCut )
      b_dphi_ll_bj_jesDown[ch] = fabs(deltaPhi( phi_ll, (bjet_dphi_jesDown+jet2_dphi_jesDown).Phi() ));
    else
      b_dphi_ll_bj_jesDown[ch] = -1;
    if( bjet_dphi_jerUp.Pt() > m_AK4jetPtCut and jet2_dphi_jerUp.Pt() > m_AK4jetPtCut )
      b_dphi_ll_bj_jerUp[ch] = fabs(deltaPhi( phi_ll, (bjet_dphi_jerUp+jet2_dphi_jerUp).Phi() ));
    else
      b_dphi_ll_bj_jerUp[ch] = -1;
    if( bjet_dphi_jerDown.Pt() > m_AK4jetPtCut and jet2_dphi_jerDown.Pt() > m_AK4jetPtCut )
      b_dphi_ll_bj_jerDown[ch] = fabs(deltaPhi( phi_ll, (bjet_dphi_jerDown+jet2_dphi_jerDown).Phi() ));
    else
      b_dphi_ll_bj_jerDown[ch] = -1;
  }
  
  // data
  if(m_isData){
    b_njets_nom[ch]      = b_njets[ch];         b_njets20_nom[ch]    = b_njets20[ch];
    b_ncjets_nom[ch]     = b_ncjets[ch];        b_nfjets_nom[ch]     = b_nfjets[ch];
    b_ncbtag_nom[ch]     = b_ncbtag[ch];
    b_jpt_1_nom[ch]      = b_jpt_1[ch];         b_jpt_2_nom[ch]      = b_jpt_2[ch];
    b_jeta_1_nom[ch]     = b_jeta_1[ch];        b_jeta_2_nom[ch]     = b_jeta_2[ch];
    b_met_nom[ch]        = b_met[ch];
    b_pfmt_1_nom[ch]     = b_pfmt_1[ch];
    b_dphi_ll_bj_nom[ch] = b_dphi_ll_bj[ch];
  }
  
}





void TauTauAnalysis::countJets(const TLorentzVector& jet, Int_t& ncjets, Int_t& nfjets, Int_t& ncbtags, TLorentzVector& bjet_dphi, TLorentzVector& jet2_dphi, const bool isBTagged){
  //std::cout << "countJets" << std::endl;
  if(jet.Pt()<30) return;
  Float_t abseta = fabs(jet.Eta());
  if(abseta < 2.4){         // CENTRAL 30 GeV
    if(isBTagged) ncbtags++;
    ncjets++;
    if(jet.Pt()>bjet_dphi.Pt() and isBTagged) 
        bjet_dphi.SetPtEtaPhiM(jet.Pt(),jet.Eta(),jet.Phi(),jet.M());
    else if(jet.Pt()>jet2_dphi.Pt())
        jet2_dphi.SetPtEtaPhiM(jet.Pt(),jet.Eta(),jet.Phi(),jet.M());
  }
  else if(abseta > 2.4){    // FORWARD 30 GeV
    nfjets++;
  }
  return;
}




void TauTauAnalysis::FillJetBranches_JEC( Float_t& jpt, Float_t& jeta, const TLorentzVector& jet, bool save ){
  // Helpfunction to fill jet pt/eta branches for JEC corrections and reduce code clutter
  if(save){ jpt  = jet.Pt();  jeta = jet.Eta(); }
  else{     jpt  = -1;        jeta = -9;        }
}





void TauTauAnalysis::setGenBosonTLVs(){
  // std::cout << "recoilCorrection" << std::endl;
  // https://twiki.cern.ch/twiki/bin/viewauth/CMS/HiggsToTauTauWorking2016#Computation_of_generator_Z_W_Hig
  // TODO: check case of more than one boson
  
  boson_tlv     = TLorentzVector();
  boson_tlv_vis = TLorentzVector();
  
  for ( int p = 0; p < (m_genParticle.N); ++p ) {
    UZH::GenParticle mygoodGenPart( &m_genParticle, p );
    float pdg = fabs(mygoodGenPart.pdgId());
    bool isNeutrino = (pdg == 12 || pdg == 14 || pdg == 16);
    
    if( (mygoodGenPart.fromHardProcessFinalState() && (pdg == 11 || pdg == 13 || isNeutrino)) ||
        mygoodGenPart.isDirectHardProcessTauDecayProductFinalState() ){
      boson_tlv += mygoodGenPart.tlv();
      if(!isNeutrino)
        boson_tlv_vis += mygoodGenPart.tlv();
    }
  }
}





int TauTauAnalysis::genMatch(Float_t lep_eta, Float_t lep_phi) {
  //std::cout << "cutflowCheck" << std::endl;
  // https://twiki.cern.ch/twiki/bin/viewauth/CMS/HiggsToTauTauWorking2016#MC_Matching
  //  1: prompt electron
  //  2: prompt muon
  //  3: tau -> e
  //  4. tau -> mu
  //  5: tau -> hadr.
  //  6: fake jet / PU
  
  Float_t min_dR = 1000;
  int id = 6;
  
  // check for lepton matching, first
  for ( int p = 0; p < (m_genParticle.N); ++p ) {
    UZH::GenParticle particle( &m_genParticle, p );
    
    Float_t pt      = particle.pt();
    Float_t eta     = particle.eta();
    Float_t phi     = particle.phi();
    Int_t pdgId     = abs(particle.pdgId());
    Int_t isPrompt  = particle.isPrompt();
    Int_t isDirectPromptTauDecayProduct = particle.isDirectPromptTauDecayProduct();
    
    if(particle.status()!=1) continue;
    if( !((pdgId==11 || pdgId==13) && (isPrompt > 0.5 || isDirectPromptTauDecayProduct > 0.5) && pt > 8) ) continue;
    
    Float_t dr = deltaR(lep_eta-eta, deltaPhi(lep_phi, phi));
    if(dr < min_dR){
      min_dR = dr;
      if(pdgId==11 && isPrompt > 0.5) id = 1;
      if(pdgId==13 && isPrompt > 0.5) id = 2;
      if(pdgId==11 && isDirectPromptTauDecayProduct > 0.5) id = 3;
      if(pdgId==13 && isDirectPromptTauDecayProduct > 0.5) id = 4;
    }
  }
  
  for ( int itau = 0; itau < (int)m_genParticle.tauvispt->size(); ++itau ) {
    Float_t dr = deltaR(lep_eta - m_genParticle.tauviseta->at(itau),
			deltaPhi(lep_phi, m_genParticle.tauvisphi->at(itau)));
    if(dr < min_dR){
      min_dR = dr;
      id = 5;
    }
  }
  
  if(min_dR > 0.2) id = 6;
  
  return id;

}





Float_t TauTauAnalysis::deltaPhi(Float_t p1, Float_t p2){
//std::cout << "deltaPhi" << std::endl;

  Float_t res = p1 - p2;
  while(res > TMath::Pi()){
    res -= 2*TMath::Pi();
  }
  while(res < -TMath::Pi()){
    res += 2*TMath::Pi();
  }
  
  return res;
}





Float_t TauTauAnalysis::deltaR(Float_t deta, Float_t dphi){
  //std::cout << "deltaR" << std::endl;
  return TMath::Sqrt(TMath::Power(deta,2) + TMath::Power(dphi,2));
}





void TauTauAnalysis::printRow(const std::vector<std::string> svec, const std::vector<int> ivec, const std::vector<double> dvec, const std::vector<float> fvec, const int w){
    for(auto const& el: svec) std::cout << std::setw(w) << el;
    for(auto const& el: ivec) std::cout << std::setw(w) << el;
    for(auto const& el: fvec) std::cout << std::setw(w) << el;
    for(auto const& el: dvec) std::cout << std::setw(w) << el;
    std::cout << std::endl;
}





bool TauTauAnalysis::LooseJetID(const UZH::Jet& jet) {
  // https://twiki.cern.ch/twiki/bin/viewauth/CMS/JetID#Recommendations_for_13_TeV_data
  // https://twiki.cern.ch/twiki/bin/view/CMS/JetID13TeVRun2016
  Float_t eta  = fabs(jet.eta());
  Float_t NHF  = jet.nhf();  //neutralHadronEnergyFraction();
  Float_t NEMF = jet.nemf(); //neutralEmEnergyFraction();
  Float_t NM   = jet.nm();   //neutralMultiplicity();
  Float_t CM   = jet.cm();   //chargedMultiplicity();
  
  if(eta <= 2.4){
    Float_t CHF  = jet.chf();  //chargedHadronEnergyFraction();
    Float_t CEMF = jet.cemf(); //chargedEmEnergyFraction();
    return NHF < 0.99 and NEMF < 0.99 and NM+CM > 1 and
           CHF > 0    and CEMF < 0.99 and    CM > 0;
  }
  else if(eta <= 2.7){
    return NHF < 0.99 and NEMF < 0.99 and NM+CM > 1;
  }
  else if(eta <= 3.0){
    //return NEMF < 0.90 && NM > 2; // old jet ID
    return NEMF > 0.01 and NHF < 0.98 and NM > 2; // new jet ID
  }
  else if(eta  < 4.7){
    return NEMF < 0.90 and NM > 10;
  }
  return false;
}





float TauTauAnalysis::genMatchSF(const std::string& channel, const int genmatch_2, const float tau_eta){

  // top pt reweighting:    https://twiki.cern.ch/twiki/bin/view/CMS/MSSMAHTauTauEarlyRun2#Top_quark_pT_reweighting
  
  // real top
  if (genmatch_2 == -36) {
    double pt_top_1 = 0;
    double pt_top_2 = 0;
    int qq = 0;
    for ( int p = 0; p < (m_genParticle.N); ++p ) {
      UZH::GenParticle top( &m_genParticle, p );
      if( abs(top.pdgId()) == 6 ){
        if(qq==0) { pt_top_1 = top.pt(); qq = top.pdgId(); }
        else if(qq*top.pdgId()<0) { pt_top_2 = top.pt(); qq*=top.pdgId(); break; }
      }
    }
    if(qq==-36){
      const char* ch = channel.c_str();
      b_pt_top_1[ch] = pt_top_1;
      b_pt_top_2[ch] = pt_top_2;
      b_ttptweight_runI[ch] = TMath::Sqrt(TMath::Exp(0.156-0.00137*TMath::Min(pt_top_1,400.0))*TMath::Exp(0.156-0.00137*TMath::Min(pt_top_2,400.0)));
      return TMath::Sqrt(TMath::Exp(0.0615-0.0005*TMath::Min(pt_top_1,400.0))*TMath::Exp(0.0615-0.0005*TMath::Min(pt_top_2,400.0)));
    }else{
      std::cout << ">>> TauTauAnalysis::genMatchSF: genmatch_2 = 66, qq = " << qq << " != -36 !!!" << std::endl;
    }
  }
  
  return 1.0;
}




void TauTauAnalysis::shiftLeptonAndMET(const float shift, TLorentzVector& lep_shifted, TLorentzVector& met_shifted, bool shiftEnergy){
  //std::cout << "shiftLeptonAndMET" << std::endl;
  
  //std::cout << ">>> after:  lep_shifted pt = " << lep_shifted.Pt()  << ", m   = " << lep_shifted.M() << std::endl;
  TLorentzVector Delta_lep_tlv(lep_shifted.Px()*shift, lep_shifted.Py()*shift, 0, 0); // (dpx,dpy,0,0)
  if(shiftEnergy) lep_shifted.SetPtEtaPhiM((1.+shift)*lep_shifted.Pt(),lep_shifted.Eta(),lep_shifted.Phi(),(1.+shift)*lep_shifted.M());
  else            lep_shifted.SetPtEtaPhiM((1.+shift)*lep_shifted.Pt(),lep_shifted.Eta(),lep_shifted.Phi(),           lep_shifted.M());
  TLorentzVector met_diff;
  met_diff.SetPtEtaPhiM(met_shifted.Pt(),met_shifted.Eta(),met_shifted.Phi(),0.); // MET(px,dpy,0,0) - (dpx,dpy,0,0)
  met_diff -= Delta_lep_tlv;
  met_shifted.SetPtEtaPhiM(met_diff.Pt(),met_diff.Eta(),met_diff.Phi(),0.); // keep E = |p| !
  //std::cout << ">>> after:  lep_shifted pt = " << lep_shifted.Pt()  << ", m   = " << lep_shifted.M() << ", shift = " << shift << std::endl;
  
}





void TauTauAnalysis::shiftMET(TLorentzVector& shift, UZH::MissingEt& met){
  TLorentzVector met_tlv_shifted;
  met_tlv_shifted.SetPxPyPzE(met.et()*TMath::Cos(met.phi()), met.et()*TMath::Sin(met.phi()), 0, met.et());
  met_tlv_shifted -= shift;
  met_tlv_shifted.SetPtEtaPhiM(met_tlv_shifted.Pt(),0,met_tlv_shifted.Phi(),0.); // keep E = |p| !
  met.et(met_tlv_shifted.Pt());
  met.phi(met_tlv_shifted.Phi());
}





void TauTauAnalysis::extraLeptonVetos(const std::string& channel, const UZH::Muon& muon, const UZH::Electron& electron){
  //std::cout << "extraLeptonVetos" << std::endl;
  
  b_dilepton_veto_  = false;
  b_extraelec_veto_ = false;
  b_extramuon_veto_ = false;
  
  // extra leptons
  // https://twiki.cern.ch/twiki/bin/view/CMS/HiggsToTauTauWorking2016#Baseline_mu_tau_h
  // https://twiki.cern.ch/twiki/bin/view/CMS/HiggsToTauTauWorking2016#Baseline_e_tau_h
  // https://twiki.cern.ch/twiki/bin/view/CMS/HiggsToTauTauWorking2016#Third_lepton_vetoes
  
  std::vector<UZH::Muon> passedMuons;
  for( int i = 0; i < m_muon.N; ++i ){
    UZH::Muon mymuon( &m_muon, i );  
    
    if(mymuon.pt() < 10) continue;
    if(fabs(mymuon.eta()) > 2.4) continue;
    if(fabs(mymuon.dz_allvertices()) > 0.2) continue;
    if(fabs(mymuon.d0_allvertices()) > 0.045) continue;
    if(mymuon.SemileptonicPFIso() / mymuon.pt() > 0.3) continue;
    
    // dilepton veto: match with other muons
    if( mymuon.pt() > 15 && mymuon.isGlobalMuon()
	                     && mymuon.isTrackerMuon() 
	                     && mymuon.isPFMuon() ){
      passedMuons.push_back(mymuon);
    }
    
    // extra muon veto
    if(m_isData and m_eventInfo.runNumber < 278820)
      {  if(mymuon.isMediumMuon()   < 0.5) continue; } // for period B-F
    else if(mymuon.isMediumMuonGH() < 0.5) continue;   // for period GH and MC (see AN)
    if(mymuon.pt()!=muon.pt() && mymuon.eta()!=muon.eta() && mymuon.phi()!=muon.phi()){
      b_extramuon_veto_ = true;
    }
  }
  
  std::vector<UZH::Electron> passedElectrons;
  for( int i = 0; i < m_electron.N; ++i ){
    UZH::Electron myelectron( &m_electron, i );
    
    if(myelectron.pt() < 10) continue;
    if(fabs(myelectron.eta()) > 2.5) continue;
    if(fabs(myelectron.dz_allvertices()) > 0.2) continue;
    if(fabs(myelectron.d0_allvertices()) > 0.045) continue;
    if(myelectron.relIsoWithDBeta() > 0.3) continue;
    if(!myelectron.isMVAMediumElectron()) continue; // Moriond
    
    // extra electron veto
    if(myelectron.passConversionVeto() &&
       myelectron.isMVAMediumElectron() && 
       myelectron.expectedMissingInnerHits() <= 1){
      if( myelectron.pt() != electron.pt() && myelectron.eta() != electron.eta() && myelectron.phi() != electron.phi())
        b_extraelec_veto_ = true;
    }
    
    // dilepton veto: match with other muons
    if(myelectron.pt() > 15 && myelectron.isMVAMediumElectron())
      passedElectrons.push_back(myelectron);
  }
  
  
  // dilepton veto
  for(int ielectron = 0; ielectron < (int)passedElectrons.size(); ielectron++){
    for(int jelectron = 0; jelectron < ielectron; jelectron++){
      if(passedElectrons[ielectron].charge() * passedElectrons[jelectron].charge() < 0 &&
	 passedElectrons[ielectron].tlv().DeltaR(passedElectrons[jelectron].tlv()) > 0.15)
	b_dilepton_veto_ = true;
    }
  }


}





bool TauTauAnalysis::getBTagWeight_promote_demote( const UZH::Jet& jet ) {
  //std::cout << "getBTagSF_promote_demote" << std::endl;
  // https://twiki.cern.ch/twiki/bin/viewauth/CMS/HiggsToTauTauWorking2016#B_tag_scale_factors
  // example: https://github.com/rappoccio/usercode/blob/Dev_53x/EDSHyFT/plugins/BTagSFUtil_tprime.h
  //
  // instead of calculating the event weights,
  // scale factors are used to update the b-tagging status on a jet-by-jet basis
  // advantage: 1) no need to apply event weights
  //            2) reproducibility with seed
  
  bool isBTagged = (jet.csv() > m_CSVWorkingPoint);
  if (m_isData) return isBTagged;
  
  //if (isBTagged) std::cout << "Jet b tagged" << std::endl;
  //else           std::cout << "Jet b not tagged" << std::endl;
  
  TRandom3* generator = new TRandom3( (int) ((jet.eta()+5)*100000) );
  double rand = generator->Uniform(1.);
  
  double BTag_SF  = m_BTaggingScaleTool.getScaleFactor_noWeight(jet);
  double BTag_eff = m_BTaggingScaleTool.getEfficiency(jet,"jet_ak4");
  double BTag_SFweight  = m_BTaggingScaleTool.getScaleFactor(jet);
  b_weightbtag_ *= BTag_SFweight;
  
  if (BTag_SF == 1) return isBTagged; // no correction
  else if(BTag_SF > 1){
    if(isBTagged) return isBTagged;
    float mistagPercentage = (1.0 - BTag_SF) / (1.0 - (1.0/BTag_eff)); // fraction of jets to be promoted
    if( rand < mistagPercentage ) isBTagged = true; // PROMOTE
  }
  else{//(BTag_SF < 1)
    if(!isBTagged) return isBTagged;
    if( rand < 1 - BTag_SF ) isBTagged = false; // DEMOTE: 1-SF fraction of jets to be demoted
  }
  
  return isBTagged;
}
