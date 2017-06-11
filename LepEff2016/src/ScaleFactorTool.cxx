#include "../interface/ScaleFactorTool.h"
#include <cstdlib>
#include <limits>
#include <TFile.h>



ScaleFactorTool::ScaleFactorTool(SCycleBase* parent, const char* name ):
 SToolBase( parent ), m_name( name ) 
{
  SetLogName( name );
  // leptons: https://github.com/CMS-HTT/LeptonEfficiencies
  // tau leg: https://github.com/rmanzoni/triggerSF/tree/moriond17
  // Mail Valeria Botta, 20/02/2017
  //   Electron_Ele24_eff.root         = Ele leg in cross trigger
  //   Muon_Mu19leg_2016BtoH_eff.root  = Mu  leg in cross trigger
  //   Muon_Mu22OR_eta2p1*             = IsoMu22, TkIsoMu22 (with and without |eta|< 2.1)
  
  DeclareProperty( m_name+"_MuIdIso",           m_File_MuIdIso           = std::string(std::getenv("SFRAME_DIR")) + "/../LepEff2016/data/Muon/Run2016BtoH/Muon_IdIso_IsoLt0p2_2016BtoH_eff.root"      );
  //  DeclareProperty( m_name+"_MuIdIso",           m_File_MuIdIso           = std::string(std::getenv("SFRAME_DIR")) + "/../LepEff2016/data/Muon/Run2016BtoH/Muon_IdIso_IsoLt0p15_2016BtoH_eff.root"      );
  DeclareProperty( m_name+"_EleIdIso",          m_File_EleIdIso          = std::string(std::getenv("SFRAME_DIR")) + "/../LepEff2016/data/Electron/Run2016BtoH/Electron_IdIso_IsoLt0p15_eff.root"       );
  //  DeclareProperty( m_name+"_EleIdIso",          m_File_EleIdIso          = std::string(std::getenv("SFRAME_DIR")) + "/../LepEff2016/data/Electron/Run2016BtoH/Electron_IdIso_IsoLt0p1_eff.root"       );

  DeclareProperty( m_name+"_EleMuTrig_Ele12Leg", m_File_EleMuTrig_Ele12Leg = std::string(std::getenv("SFRAME_DIR")) + "/../LepEff2016/data/Electron/Run2016BtoH/Electron_Ele12leg_eff.root"            );
  DeclareProperty( m_name+"_EleMuTrig_Ele23Leg", m_File_EleMuTrig_Ele23Leg = std::string(std::getenv("SFRAME_DIR")) + "/../LepEff2016/data/Electron/Run2016BtoH/Electron_Ele23leg_eff.root"            );
  DeclareProperty( m_name+"_EleMuTrig_Mu8Leg",   m_File_EleMuTrig_Mu8Leg   = std::string(std::getenv("SFRAME_DIR")) + "/../LepEff2016/data/Muon/Run2016BtoH/Muon_Mu8leg_2016BtoH_eff.root"             );
  DeclareProperty( m_name+"_EleMuTrig_Mu23Leg",  m_File_EleMuTrig_Mu23Leg  = std::string(std::getenv("SFRAME_DIR")) + "/../LepEff2016/data/Muon/Run2016BtoH/Muon_Mu23leg_2016BtoH_eff.root"            );
}



void ScaleFactorTool::BeginInputData( const SInputData& ) throw( SError ) {
  
  m_logger << INFO << "Initializing ScaleFactor for lepons"     << SLogger::endmsg;
  m_logger << INFO << "Efficiency file Mu IdIso: "              << m_File_MuIdIso           << SLogger::endmsg;
  m_logger << INFO << "Efficiency file Ele IdIso: "             << m_File_EleIdIso          << SLogger::endmsg;
  m_logger << INFO << "Efficiency file EleMu Trig Ele12 Leg: "  << m_File_EleMuTrig_Ele12Leg << SLogger::endmsg;
  m_logger << INFO << "Efficiency file EleMu Trig Ele23 Leg: "  << m_File_EleMuTrig_Ele23Leg << SLogger::endmsg;
  m_logger << INFO << "Efficiency file EleMu Trig Mu8 Leg: "    << m_File_EleMuTrig_Mu8Leg   << SLogger::endmsg;
  m_logger << INFO << "Efficiency file EleMu Trig Mu23 Leg: "   << m_File_EleMuTrig_Mu23Leg  << SLogger::endmsg;
    
  m_ScaleFactor_MuIdIso           = new ScaleFactor( m_File_MuIdIso );
  m_logger << INFO << "Scale factor Mu IdIso initialised" << SLogger:: endmsg;
    
  m_ScaleFactor_EleIdIso          = new ScaleFactor( m_File_EleIdIso );
  m_logger << INFO << "Scale factor Ele IdIso initialised" << SLogger:: endmsg;
  
  m_ScaleFactor_EleMuTrig_Ele12Leg = new ScaleFactor( m_File_EleMuTrig_Ele12Leg );
  m_logger << INFO << "Scale factor EleMu Trig Ele 12 Leg initialised" << SLogger:: endmsg;
  
  m_ScaleFactor_EleMuTrig_Ele23Leg = new ScaleFactor( m_File_EleMuTrig_Ele23Leg );
  m_logger << INFO << "Scale factor EleMu Trig Ele 23 Leg initialised" << SLogger:: endmsg;
  
  m_ScaleFactor_EleMuTrig_Mu8Leg   = new ScaleFactor( m_File_EleMuTrig_Mu8Leg );
  m_logger << INFO << "Scale factor EleMu Trig Mu 8 Leg initialised" << SLogger:: endmsg;
  
  m_ScaleFactor_EleMuTrig_Mu23Leg  = new ScaleFactor( m_File_EleMuTrig_Mu23Leg );
  m_logger << INFO << "Scale factor EleMu Trig Mu 23 Leg initialised" << SLogger:: endmsg;
  
  return;
}



ScaleFactorTool::~ScaleFactorTool(){
  //delete  eff_mc[etaLabel]; delete eff_data[etaLabel]
}



double ScaleFactorTool::get_ScaleFactor_MuIdIso(double pt, double eta){
  return m_ScaleFactor_MuIdIso->get_ScaleFactor(pt,eta);
}



double ScaleFactorTool::get_ScaleFactor_EleIdIso(double pt, double eta){
  return m_ScaleFactor_EleIdIso->get_ScaleFactor(pt,eta);
}



double ScaleFactorTool::get_Efficiency_EleMuTrig_MC(double pt1, double eta1, double pt2, double eta2, std::string triggerFlags){

  // assume:
  //  - eff(X1) = eff(Ele12Mu23) = eff(Ele12)*eff(Mu23)
  //  - eff(X2) = eff(Ele23Mu8)  = eff(Ele23)*eff(Mu8)
  //  - Ele23 fired  => Ele12 fired
  //  - eff(Ele23) < eff(Ele12)
  //  - Mu23 fired  => Mu8 fired
  //  - eff(Mu23)  < eff(Mu8)
  //
  // https://twiki.cern.ch/twiki/bin/viewauth/CMS/HiggsToTauTauWorking2016#Muon_efficiencies
  //
  //  P( X1  OR  X2) = eff(Ele12)*eff(Mu23) + eff(Ele23)*eff(Mu8) - eff(Ele23)*eff(Mu23)
  //  P( X1 AND  X2) = eff(Ele23)*eff(Mu23)
  //  P( X1 AND !X2) = eff(Ele12)*eff(Mu23)*[eff(Ele8)-eff(Ele23)] 
  //  P(!X1 AND  X2) = eff(Ele23)*eff(Mu8) *[eff(Mu12)-eff(Mu8)  ]
  //
  // numerical protection:
  //  P( X1 AND  X2) = eff(Ele23)*eff(Mu23)
  //  P( X1 AND !X2) = max[1.e-2, eff(Ele12)*eff(Mu23)*[eff(Ele8)-eff(Ele23)]]
  //  P(!X1 AND  X2) = max[1.e-2, eff(Ele23)*eff(Mu8) *[eff(Mu12)-eff(Mu8)  ]]
  //
  
  float old = -1.;
  if( triggerFlags.find("e12m23") != std::string::npos ){
    if( triggerFlags.find("e23m8") != std::string::npos )
      //      return m_ScaleFactor_EleMuTrig_Ele23Leg->get_EfficiencyMC(pt1,eta1) * m_ScaleFactor_EleMuTrig_Mu23Leg->get_EfficiencyMC(pt2,eta2);
      old = m_ScaleFactor_EleMuTrig_Ele23Leg->get_EfficiencyMC(pt1,eta1) * m_ScaleFactor_EleMuTrig_Mu23Leg->get_EfficiencyMC(pt2,eta2);
    else
      //      return std::max(0.01, (m_ScaleFactor_EleMuTrig_Ele12Leg->get_EfficiencyMC(pt1,eta1) - m_ScaleFactor_EleMuTrig_Ele23Leg->get_EfficiencyMC(pt1,eta1))
      //		      * m_ScaleFactor_EleMuTrig_Ele12Leg->get_EfficiencyMC(pt1,eta1) * m_ScaleFactor_EleMuTrig_Mu23Leg->get_EfficiencyMC(pt2,eta2));
      old = std::max(0.01, (m_ScaleFactor_EleMuTrig_Ele12Leg->get_EfficiencyMC(pt1,eta1) - m_ScaleFactor_EleMuTrig_Ele23Leg->get_EfficiencyMC(pt1,eta1))
		     * m_ScaleFactor_EleMuTrig_Ele12Leg->get_EfficiencyMC(pt1,eta1) * m_ScaleFactor_EleMuTrig_Mu23Leg->get_EfficiencyMC(pt2,eta2));
  }else{
    if( triggerFlags.find("e23m8") != std::string::npos )
      //      return std::max(0.01, (m_ScaleFactor_EleMuTrig_Mu8Leg->get_EfficiencyMC(pt2,eta2)   - m_ScaleFactor_EleMuTrig_Mu23Leg->get_EfficiencyMC(pt2,eta2))
      //                           * m_ScaleFactor_EleMuTrig_Ele23Leg->get_EfficiencyMC(pt1,eta1) * m_ScaleFactor_EleMuTrig_Mu8Leg->get_EfficiencyMC(pt2,eta2));
      old = std::max(0.01, (m_ScaleFactor_EleMuTrig_Mu8Leg->get_EfficiencyMC(pt2,eta2)   - m_ScaleFactor_EleMuTrig_Mu23Leg->get_EfficiencyMC(pt2,eta2))
		     * m_ScaleFactor_EleMuTrig_Ele23Leg->get_EfficiencyMC(pt1,eta1) * m_ScaleFactor_EleMuTrig_Mu8Leg->get_EfficiencyMC(pt2,eta2));
  }

  float Ele23EffMC = (float)m_ScaleFactor_EleMuTrig_Ele23Leg->get_EfficiencyMC(double(pt1),double(eta1));
  float Ele12EffMC = (float)m_ScaleFactor_EleMuTrig_Ele12Leg->get_EfficiencyMC(double(pt1),double(eta1));
  float Mu23EffMC = (float)m_ScaleFactor_EleMuTrig_Mu23Leg->get_EfficiencyMC(double(pt2),double(eta2));
  float Mu8EffMC = (float)m_ScaleFactor_EleMuTrig_Mu8Leg->get_EfficiencyMC(double(pt2),double(eta2));
  float trigWeightMC = Mu23EffMC*Ele12EffMC + Mu8EffMC*Ele23EffMC - Mu23EffMC*Ele23EffMC;


  //  std::cout << "trigWeightMC (new, after) = " << trigWeightMC << " " << old << std::endl;
  return trigWeightMC;


//  std::cout << ">>> ScaleFactorTool::get_Efficiency_EleMuTrig_MC - no trigger fired? triggerFlags=" << triggerFlags << std::endl;
//  return 0.;
}



double ScaleFactorTool::get_Efficiency_EleMuTrig_Data(double pt1, double eta1, double pt2, double eta2, std::string triggerFlags){

  // assume:
  //  - eff(X1) = eff(Ele12Mu23) = eff(Ele12)*eff(Mu23)
  //  - eff(X2) = eff(Ele23Mu8)  = eff(Ele23)*eff(Mu8)
  //  - Ele23 fired  => Ele12 fired
  //  - eff(Ele23) < eff(Ele12)
  //  - Mu23 fired  => Mu8 fired
  //  - eff(Mu23)  < eff(Mu8)
  //
  // https://twiki.cern.ch/twiki/bin/viewauth/CMS/HiggsToTauTauWorking2016#Muon_efficiencies
  //
  // P( X1  OR  X2) = eff(Ele12)*eff(Mu23) + eff(Ele23)*eff(Mu8) - eff(Ele23)*eff(Mu23)
  // P( X1 AND  X2) = eff(Ele23)*eff(Mu23)
  // P( X1 AND !X2) = eff(Ele12)*eff(Mu23)*[eff(Ele8)-eff(Ele23)] 
  // P(!X1 AND  X2) = eff(Ele23)*eff(Mu8) *[eff(Mu12)-eff(Mu8)  ]
  //
  // numerical protection:
  //  P( X1 AND  X2) = eff(Ele23)*eff(Mu23)
  //  P( X1 AND !X2) = max[1.e-2, eff(Ele12)*eff(Mu23)*[eff(Ele8)-eff(Ele23)]]
  //  P(!X1 AND  X2) = max[1.e-2, eff(Ele23)*eff(Mu8) *[eff(Mu12)-eff(Mu8)  ]]
  //

  float old = -1.;  
  if( triggerFlags.find("e12m23") != std::string::npos ){
    if( triggerFlags.find("e23m8") != std::string::npos )
      //      return m_ScaleFactor_EleMuTrig_Ele23Leg->get_EfficiencyData(pt1,eta1) * m_ScaleFactor_EleMuTrig_Mu23Leg->get_EfficiencyData(pt2,eta2);
      old = m_ScaleFactor_EleMuTrig_Ele23Leg->get_EfficiencyData(pt1,eta1) * m_ScaleFactor_EleMuTrig_Mu23Leg->get_EfficiencyData(pt2,eta2);
    else
      old = std::max(0.01, (m_ScaleFactor_EleMuTrig_Ele12Leg->get_EfficiencyData(pt1,eta1) - m_ScaleFactor_EleMuTrig_Ele23Leg->get_EfficiencyData(pt1,eta1))
		     * m_ScaleFactor_EleMuTrig_Ele12Leg->get_EfficiencyData(pt1,eta1) * m_ScaleFactor_EleMuTrig_Mu23Leg->get_EfficiencyData(pt2,eta2));
    //      return std::max(0.01, (m_ScaleFactor_EleMuTrig_Ele12Leg->get_EfficiencyData(pt1,eta1) - m_ScaleFactor_EleMuTrig_Ele23Leg->get_EfficiencyData(pt1,eta1))
    //                           * m_ScaleFactor_EleMuTrig_Ele12Leg->get_EfficiencyData(pt1,eta1) * m_ScaleFactor_EleMuTrig_Mu23Leg->get_EfficiencyData(pt2,eta2));
  }else{
    if( triggerFlags.find("e23m8") != std::string::npos )
      old = std::max(0.01, (m_ScaleFactor_EleMuTrig_Mu8Leg->get_EfficiencyData(pt2,eta2)   - m_ScaleFactor_EleMuTrig_Mu23Leg->get_EfficiencyData(pt2,eta2))
                           * m_ScaleFactor_EleMuTrig_Ele23Leg->get_EfficiencyData(pt1,eta1) * m_ScaleFactor_EleMuTrig_Mu8Leg->get_EfficiencyData(pt2,eta2));
//      return std::max(0.01, (m_ScaleFactor_EleMuTrig_Mu8Leg->get_EfficiencyData(pt2,eta2)   - m_ScaleFactor_EleMuTrig_Mu23Leg->get_EfficiencyData(pt2,eta2))
//                           * m_ScaleFactor_EleMuTrig_Ele23Leg->get_EfficiencyData(pt1,eta1) * m_ScaleFactor_EleMuTrig_Mu8Leg->get_EfficiencyData(pt2,eta2));
  }

  float Ele23EffData = (float)m_ScaleFactor_EleMuTrig_Ele23Leg->get_EfficiencyData(double(pt1),double(eta1));
  float Ele12EffData = (float)m_ScaleFactor_EleMuTrig_Ele12Leg->get_EfficiencyData(double(pt1),double(eta1));
  float Mu23EffData = (float)m_ScaleFactor_EleMuTrig_Mu23Leg->get_EfficiencyData(double(pt2),double(eta2));
  float Mu8EffData = (float)m_ScaleFactor_EleMuTrig_Mu8Leg->get_EfficiencyData(double(pt2),double(eta2));
  float trigWeightData = Mu23EffData*Ele12EffData + Mu8EffData*Ele23EffData - Mu23EffData*Ele23EffData;

  //  std::cout << "trigWeightdata (new, after) = " << trigWeightData << " " << old << std::endl;
  return trigWeightData;

  //  return m_ScaleFactor_EleMuTrig_Ele23Leg->get_EfficiencyData(pt1,eta1) * m_ScaleFactor_EleMuTrig_Mu23Leg->get_EfficiencyData(pt2,eta2);
  
  //  std::cout << ">>> ScaleFactorTool::get_Efficiency_EleMuTrig_Data - no trigger fired? triggerFlags=" << triggerFlags << std::endl;
  //  return 0.;
}



double ScaleFactorTool::get_ScaleFactor_EleMuTrig(double pt1, double eta1, double pt2, double eta2, std::string triggerFlags){
  return std::min(10.0, get_Efficiency_EleMuTrig_Data(pt1,eta1,pt2,eta2,triggerFlags)/get_Efficiency_EleMuTrig_MC(pt1,eta1,pt2,eta2,triggerFlags));
}



double ScaleFactorTool::get_Efficiency_EleMuTrig_OR_MC(double pt1, double eta1, double pt2, double eta2){
  //  P( X1 OR X2) = eff(Ele12)*eff(Mu23) + eff(Ele23)*eff(Mu8) - eff(Ele23)*eff(Mu23)
  return   m_ScaleFactor_EleMuTrig_Ele12Leg->get_EfficiencyMC(pt1,eta1) * m_ScaleFactor_EleMuTrig_Mu23Leg->get_EfficiencyMC(pt2,eta2)
         + m_ScaleFactor_EleMuTrig_Ele23Leg->get_EfficiencyMC(pt1,eta1) * m_ScaleFactor_EleMuTrig_Mu8Leg->get_EfficiencyMC(pt2,eta2)
         - m_ScaleFactor_EleMuTrig_Ele23Leg->get_EfficiencyMC(pt1,eta1) * m_ScaleFactor_EleMuTrig_Mu23Leg->get_EfficiencyMC(pt2,eta2);
}



double ScaleFactorTool::get_Efficiency_EleMuTrig_OR_Data(double pt1, double eta1, double pt2, double eta2){
  //  P( X1 OR X2) = eff(Ele12)*eff(Mu23) + eff(Ele23)*eff(Mu8) - eff(Ele23)*eff(Mu23)
  return   m_ScaleFactor_EleMuTrig_Ele12Leg->get_EfficiencyData(pt1,eta1) * m_ScaleFactor_EleMuTrig_Mu23Leg->get_EfficiencyData(pt2,eta2)
         + m_ScaleFactor_EleMuTrig_Ele23Leg->get_EfficiencyData(pt1,eta1) * m_ScaleFactor_EleMuTrig_Mu8Leg->get_EfficiencyData(pt2,eta2)
         - m_ScaleFactor_EleMuTrig_Ele23Leg->get_EfficiencyData(pt1,eta1) * m_ScaleFactor_EleMuTrig_Mu23Leg->get_EfficiencyData(pt2,eta2);
}



double ScaleFactorTool::get_ScaleFactor_EleMuTrig_OR(double pt1, double eta1, double pt2, double eta2){
  return std::min(10.0, get_Efficiency_EleMuTrig_OR_Data(pt1,eta1,pt2,eta2)
                         / std::max(0.01, get_Efficiency_EleMuTrig_OR_MC(pt1,eta1,pt2,eta2)));
}


