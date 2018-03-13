// Dear emacs, this is -*- c++ -*-
// $Id: TauNtupleObject.h 37457 2010-07-05 12:04:33Z mann $

// THIS FILE HAS BEEN GENERATED AUTOMATICALLY. DO NOT EDIT DIRECTLY, CHANGES WILL BE LOST UPON NEXT CODE GENERATION.
// Code produced by Id: CodeIt.py 494 2010-07-30 13:41:32Z svn 


#ifndef SFRAME_NtupleVARIABLES_TauNtupleObject_H
#define SFRAME_NtupleVARIABLES_TauNtupleObject_H

// Local include(s):
#include "NtupleObjectNames.h"

// STL include(s):
#include <vector>
#include <string>

// ROOT include(s):
#include <TString.h>

// SFrame include(s):
#include "core/include/SError.h"
#include "core/include/SCycleBaseNTuple.h"
#include "plug-ins/include/SInputVariables.h"

namespace Ntuple {
  
  /**
   *  @short Class that can read the variables produced by TauNtupleObject
   *
   *        This class can be used to read the offline muon information from
   *        an ntuple produced by the SingleTopDPDMaker code.
   *
   *  @author Code produced by Id: CodeIt.py 494 2010-07-30 13:41:32Z svn   
   *
   */
  
  enum TauDetails {
    TauBasic = 1,
    TauID = 2,
    TauAdvancedID = 4,
    TauAll = 7,
  };
  
  // forward declaration of NtupleObjectNames
  class NtupleObjectNames;
  class TauNtupleObject : public SInputVariables< SCycleBaseNTuple > {
    
    public:
    
    // constructor specifying the parent of the object
    TauNtupleObject( SCycleBaseNTuple* parent );
    
    // remember if connect succeeded
    void setConnectSucceeded(const unsigned int index, const bool success);
    bool getConnectSucceeded(const unsigned int index) const {return m_connectsucceeded.at(index);}  
    
    // connect the variables to the input branches
    void ConnectVariables( const TString& treeName,
                           UInt_t detail_level = 0,
                           const TString& prefix = "Tau_",
                           const TString& ntupleType = "NtupleMakerNtuple" ) throw( SError );
    
    void ConnectVariables( const TString& treeName,
                           const TString& prefix = "Tau_",
                           const TString& ntupleType = "NtupleMakerNtuple" ) throw( SError );
    
    int getDetailLevel() const { return detailLevel; }
    
    // particle vector size
    Int_t N;
    
    enum ConnectionIndex { 
      kagainstElectronVLooseMVA6 = 9,
      kagainstElectronLooseMVA6 = 10,
      kagainstElectronMediumMVA6 = 11,
      kagainstElectronTightMVA6 = 12,
      kagainstElectronVTightMVA6 = 13,
      kagainstMuonLoose3 = 14,
      kagainstMuonTight3 = 15,
      kagainstElectronMVA6raw = 16,
      kagainstElectronMVA6category = 17,
      kbyCombinedIsolationDeltaBetaCorrRaw3Hits = 18,
      kbyLooseCombinedIsolationDeltaBetaCorr3Hits = 19,
      kbyMediumCombinedIsolationDeltaBetaCorr3Hits = 20,
      kbyTightCombinedIsolationDeltaBetaCorr3Hits = 21,
      kbyIsolationMVArun2v1DBdR03oldDMwLTraw = 22,
      kbyIsolationMVArun2v1DBnewDMwLTraw = 23,
      kbyIsolationMVArun2v1DBoldDMwLTraw = 24,
      kbyIsolationMVArun2v1PWnewDMwLTraw = 25,
      kbyLooseIsolationMVArun2v1DBdR03oldDMwLT = 26,
      kbyLooseIsolationMVArun2v1DBnewDMwLT = 27,
      kbyLooseIsolationMVArun2v1DBoldDMwLT = 28,
      kbyLooseIsolationMVArun2v1PWnewDMwLT = 29,
      kbyMediumIsolationMVArun2v1DBdR03oldDMwLT = 30,
      kbyMediumIsolationMVArun2v1DBnewDMwLT = 31,
      kbyMediumIsolationMVArun2v1DBoldDMwLT = 32,
      kbyMediumIsolationMVArun2v1PWnewDMwLT = 33,
      kbyTightIsolationMVArun2v1DBdR03oldDMwLT = 34,
      kbyTightIsolationMVArun2v1DBnewDMwLT = 35,
      kbyTightIsolationMVArun2v1DBoldDMwLT = 36,
      kbyTightIsolationMVArun2v1PWnewDMwLT = 37,
      kbyVLooseIsolationMVArun2v1DBdR03oldDMwLT = 38,
      kbyVLooseIsolationMVArun2v1DBnewDMwLT = 39,
      kbyVLooseIsolationMVArun2v1DBoldDMwLT = 40,
      kbyVLooseIsolationMVArun2v1PWnewDMwLT = 41,
      kbyVTightIsolationMVArun2v1DBdR03oldDMwLT = 42,
      kbyVTightIsolationMVArun2v1DBnewDMwLT = 43,
      kbyVTightIsolationMVArun2v1DBoldDMwLT = 44,
      kbyVTightIsolationMVArun2v1PWnewDMwLT = 45,
      kbyVVTightIsolationMVArun2v1DBdR03oldDMwLT = 46,
      kbyVVTightIsolationMVArun2v1DBnewDMwLT = 47,
      kbyVVTightIsolationMVArun2v1DBoldDMwLT = 48,
      kbyVVTightIsolationMVArun2v1PWnewDMwLT = 49,
      kchargedIsoPtSum = 50,
      kneutralIsoPtSum = 51,
      kpuCorrPtSum = 52,
      kchargedIsoPtSumdR03 = 53,
      kneutralIsoPtSumdR03 = 54,
      kphotonPtSumOutsideSignalCone = 55,
      kphotonPtSumOutsideSignalConedR03 = 56,
      kbyPhotonPtSumOutsideSignalCone = 57,
      kchargedPionPt = 58,
      kneutralPionPt = 59,
      knPhoton = 60,
      kptWeightedDetaStrip = 61,
      kptWeightedDphiStrip = 62,
      kptWeightedDrSignal = 63,
      kptWeightedDrIsolation = 64,
      kleadingTrackChi2 = 65,
      kleadingTrackPt = 66,
      keRatio = 67,
      kdxy_Sig = 68,
      kip3d = 69,
      kip3d_Sig = 70,
      khasSecondaryVertex = 71,
      kdecayDistMag = 72,
      kflightLenthSig = 73,
      kpdgId = 1,
      kcharge = 2,
      kd0 = 3,
      kdz = 4,
      kTauType = 5,
      kdecayMode = 6,
      kdecayModeFindingNewDMs = 7,
      kdecayModeFinding = 8,
      kEnd
    }; 
    
    
    // vectors of properties defined in Particle.h
    std::vector< floatingnumber >  *pt;
    std::vector< floatingnumber >  *eta;
    std::vector< floatingnumber >  *phi;
    std::vector< floatingnumber >  *m;
    std::vector< floatingnumber >  *e;
    
    // vectors of object specific variables
    std::vector< bool >  *againstElectronVLooseMVA6;
    std::vector< bool >  *againstElectronLooseMVA6;
    std::vector< bool >  *againstElectronMediumMVA6;
    std::vector< bool >  *againstElectronTightMVA6;
    std::vector< bool >  *againstElectronVTightMVA6;
    std::vector< bool >  *againstMuonLoose3;
    std::vector< bool >  *againstMuonTight3;
    std::vector< floatingnumber >  *againstElectronMVA6raw;
    std::vector< floatingnumber >  *againstElectronMVA6category;
    std::vector< floatingnumber >  *byCombinedIsolationDeltaBetaCorrRaw3Hits;
    std::vector< bool >  *byLooseCombinedIsolationDeltaBetaCorr3Hits;
    std::vector< bool >  *byMediumCombinedIsolationDeltaBetaCorr3Hits;
    std::vector< bool >  *byTightCombinedIsolationDeltaBetaCorr3Hits;
    std::vector< floatingnumber >  *byIsolationMVArun2v1DBdR03oldDMwLTraw;
    std::vector< floatingnumber >  *byIsolationMVArun2v1DBnewDMwLTraw;
    std::vector< floatingnumber >  *byIsolationMVArun2v1DBoldDMwLTraw;
    std::vector< floatingnumber >  *byIsolationMVArun2v1PWnewDMwLTraw;
    std::vector< bool >  *byLooseIsolationMVArun2v1DBdR03oldDMwLT;
    std::vector< bool >  *byLooseIsolationMVArun2v1DBnewDMwLT;
    std::vector< bool >  *byLooseIsolationMVArun2v1DBoldDMwLT;
    std::vector< bool >  *byLooseIsolationMVArun2v1PWnewDMwLT;
    std::vector< bool >  *byMediumIsolationMVArun2v1DBdR03oldDMwLT;
    std::vector< bool >  *byMediumIsolationMVArun2v1DBnewDMwLT;
    std::vector< bool >  *byMediumIsolationMVArun2v1DBoldDMwLT;
    std::vector< bool >  *byMediumIsolationMVArun2v1PWnewDMwLT;
    std::vector< bool >  *byTightIsolationMVArun2v1DBdR03oldDMwLT;
    std::vector< bool >  *byTightIsolationMVArun2v1DBnewDMwLT;
    std::vector< bool >  *byTightIsolationMVArun2v1DBoldDMwLT;
    std::vector< bool >  *byTightIsolationMVArun2v1PWnewDMwLT;
    std::vector< bool >  *byVLooseIsolationMVArun2v1DBdR03oldDMwLT;
    std::vector< bool >  *byVLooseIsolationMVArun2v1DBnewDMwLT;
    std::vector< bool >  *byVLooseIsolationMVArun2v1DBoldDMwLT;
    std::vector< bool >  *byVLooseIsolationMVArun2v1PWnewDMwLT;
    std::vector< bool >  *byVTightIsolationMVArun2v1DBdR03oldDMwLT;
    std::vector< bool >  *byVTightIsolationMVArun2v1DBnewDMwLT;
    std::vector< bool >  *byVTightIsolationMVArun2v1DBoldDMwLT;
    std::vector< bool >  *byVTightIsolationMVArun2v1PWnewDMwLT;
    std::vector< bool >  *byVVTightIsolationMVArun2v1DBdR03oldDMwLT;
    std::vector< bool >  *byVVTightIsolationMVArun2v1DBnewDMwLT;
    std::vector< bool >  *byVVTightIsolationMVArun2v1DBoldDMwLT;
    std::vector< bool >  *byVVTightIsolationMVArun2v1PWnewDMwLT;
    std::vector< floatingnumber >  *chargedIsoPtSum;
    std::vector< floatingnumber >  *neutralIsoPtSum;
    std::vector< floatingnumber >  *puCorrPtSum;
    std::vector< floatingnumber >  *chargedIsoPtSumdR03;
    std::vector< floatingnumber >  *neutralIsoPtSumdR03;
    std::vector< floatingnumber >  *photonPtSumOutsideSignalCone;
    std::vector< floatingnumber >  *photonPtSumOutsideSignalConedR03;
    std::vector< floatingnumber >  *byPhotonPtSumOutsideSignalCone;
    std::vector< floatingnumber >  *chargedPionPt;
    std::vector< floatingnumber >  *neutralPionPt;
    std::vector< int >  *nPhoton;
    std::vector< floatingnumber >  *ptWeightedDetaStrip;
    std::vector< floatingnumber >  *ptWeightedDphiStrip;
    std::vector< floatingnumber >  *ptWeightedDrSignal;
    std::vector< floatingnumber >  *ptWeightedDrIsolation;
    std::vector< floatingnumber >  *leadingTrackChi2;
    std::vector< floatingnumber >  *leadingTrackPt;
    std::vector< floatingnumber >  *eRatio;
    std::vector< floatingnumber >  *dxy_Sig;
    std::vector< floatingnumber >  *ip3d;
    std::vector< floatingnumber >  *ip3d_Sig;
    std::vector< bool >  *hasSecondaryVertex;
    std::vector< floatingnumber >  *decayDistMag;
    std::vector< floatingnumber >  *flightLenthSig;
    std::vector< int >  *pdgId;
    std::vector< floatingnumber >  *charge;
    std::vector< floatingnumber >  *d0;
    std::vector< floatingnumber >  *dz;
    std::vector< int >  *TauType;
    std::vector< int >  *decayMode;
    std::vector< bool >  *decayModeFindingNewDMs;
    std::vector< bool >  *decayModeFinding;
    
    std::vector<int> m_connectsucceeded;
    
    // save actual detail level and group
    Int_t detailLevel;
    std::string detailGroup;
    
  }; // class TauNtupleObject

} // namespace Ntuple

#endif // SFRAME_NtupleVARIABLES_TauNtupleObject_H
