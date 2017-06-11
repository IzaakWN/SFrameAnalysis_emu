#ifndef ScaleFactorTool_h
#define ScaleFactorTool_h

// include(s):
#include "../interface/ScaleFactor.h"
#include "TROOT.h"
#include "TFile.h"
#include <iostream>
#include <map>
#include <cmath>
#include <string>

// SFrame include(s):
#include "core/include/SError.h"
#include "plug-ins/include/SToolBase.h"




class ScaleFactorTool : public SToolBase {
  
  
  
  public:
    
    ScaleFactorTool( SCycleBase* parent, const char* name = "ScaleFactorTool" );
    ~ScaleFactorTool();
    void BeginInputData( const SInputData& id ) throw( SError );
    
    // MUON
    double get_ScaleFactor_MuIdIso(       double pt, double eta );
    std::string m_File_MuIdIso;
    
    // ELECTRON
    double get_ScaleFactor_EleIdIso(       double pt, double eta );
    std::string m_File_EleIdIso;
    
    // EMU
    double get_Efficiency_EleMuTrig_MC(   double pt1, double eta1, double pt2, double eta2, std::string triggerFlags="em" );
    double get_Efficiency_EleMuTrig_Data( double pt1, double eta1, double pt2, double eta2, std::string triggerFlags="em" );
    double get_ScaleFactor_EleMuTrig(     double pt1, double eta1, double pt2, double eta2, std::string triggerFlags="em" );
    double get_Efficiency_EleMuTrig_OR_MC(   double pt1, double eta1, double pt2, double eta2 );
    double get_Efficiency_EleMuTrig_OR_Data( double pt1, double eta1, double pt2, double eta2 );
    double get_ScaleFactor_EleMuTrig_OR(     double pt1, double eta1, double pt2, double eta2 );
    std::string m_File_EleMuTrig_Ele12Leg;
    std::string m_File_EleMuTrig_Ele23Leg;
    std::string m_File_EleMuTrig_Mu8Leg;
    std::string m_File_EleMuTrig_Mu23Leg;
    
    bool verbose = true;
    
    
    
  private:
    
    std::string m_name;
    ScaleFactor* m_ScaleFactor_MuIdIso;
    ScaleFactor* m_ScaleFactor_EleMuTrig_Ele12Leg;
    ScaleFactor* m_ScaleFactor_EleMuTrig_Ele23Leg;
    ScaleFactor* m_ScaleFactor_EleMuTrig_Mu8Leg;
    ScaleFactor* m_ScaleFactor_EleMuTrig_Mu23Leg;
    ScaleFactor* m_ScaleFactor_EleIdIso;
    
    
    
};


#endif // ScaleFactorTool_h
