import os, math, copy, sys, collections
from ROOT import gStyle, TCanvas, TLegend, TH1F
from officialStyle import officialStyle
from config import *
from DataMCPlot import *
from DisplayManager import DisplayManager
import MultiDraw

lumi=35.9*0.9761
#lumi=35.9
#lumi=19.7 # up to period F
#lumi=16.2 # up to period F

basedir = '/scratch/ytakahas/SFrameAnalysis/AnalysisOutputEM/'

gROOT.SetBatch(True)
#gROOT.SetBatch(False)
officialStyle(gStyle)
gStyle.SetOptTitle(0)

config = config(basedir, lumi)
process = config.process


# leg1 is muon, leg2 is electron
baseselection = 'extraelec_veto == 0 && extramuon_veto == 0 && iso_1 < 0.2 && iso_2 < 0.15 && channel==1'

#baseselection += ' && ncbtag > 0 && ncjets == 1 && nfjets  > 0 && met < 60 && pfmt_1 < 60'
#baseselection += ' && ncbtag > 0 && ncjets  > 1 && nfjets == 0 && dphi_ll_bj > 2 && met < 60 && pfmt_1 < 60'

#baseselection = 'extraelec_veto == 0 && extramuon_veto == 0 && iso_1 < 0.2 && iso_2 < 0.15 && channel==1 && (isData==0 || (isData==1 && run > 278820 ))'

categories = collections.OrderedDict() 
categories['inclusive'] = {'sel':baseselection}
categories['cat1'] = {'sel':baseselection + ' && ncbtag > 0 && ncjets == 1 && nfjets  > 0 && met < 60 && pfmt_1 < 60'}
categories['cat2'] = {'sel':baseselection + ' && ncbtag > 0 && ncjets  > 1 && nfjets == 0 && dphi_ll_bj > 2 && met < 60 && pfmt_1 < 60'}



vardir = {
#    'm_vis':{'drawname':'m_vis', 'nbins':9, 'min':40, 'max':85, 'label':'visible mass (GeV)'},
    'm_vis':{'drawname':'m_vis', 'nbins':24, 'min':0, 'max':120, 'label':'visible mass (GeV)'},
    'pfmt_1':{'drawname':'pfmt_1', 'nbins':40, 'min':0, 'max':200, 'label':'PF MET Transverse mass (GeV)'},
    'met':{'drawname':'met', 'nbins':30, 'min':0, 'max':200, 'label':'missing E_{T} (GeV)'},
    'dR_ll':{'drawname':'dR_ll', 'nbins':30, 'min':0, 'max':math.pi, 'label':'#Delta R (l, #tau)'},
    'pt_1':{'drawname':'pt_1', 'nbins':30, 'min':0, 'max':150, 'label':'muon pT (GeV)'},
    'pt_2':{'drawname':'pt_2', 'nbins':30, 'min':0, 'max':150, 'label':'electron pT (GeV)'},
    'm_2':{'drawname':'m_2', 'nbins':30, 'min':0., 'max':2, 'label':'electron mass (GeV)'},
    'gen_match_2':{'drawname':'gen_match_2', 'nbins':7, 'min':0, 'max':7, 'label':'gen_match_2'},
    'eta_1':{'drawname':'eta_1', 'nbins':30, 'min':-2.5, 'max':2.5, 'label':'muon eta'},
    'eta_2':{'drawname':'eta_2', 'nbins':30, 'min':-2.5, 'max':2.5, 'label':'electron eta'},
    'njets':{'drawname':'njets', 'nbins':10, 'min':0, 'max':10, 'label':'# of jets (pT > 30)'},
    'nbtag':{'drawname':'nbtag', 'nbins':10, 'min':0, 'max':10, 'label':'# of b-tagged jets (pT > 30)'},
    'ncjets':{'drawname':'ncjets', 'nbins':10, 'min':0, 'max':10, 'label':'# of central jets (pT > 30)'},
    'nfjets':{'drawname':'nfjets', 'nbins':10, 'min':0, 'max':10, 'label':'# of forward jets (pT > 30)'},
#    'byIsolationMVA3oldDMwLTraw_2':{'drawname':'byIsolationMVA3oldDMwLTraw_2', 'nbins':30, 'min':-1, 'max':1, 'label':'Tau MVA isolation'},
#    'byCombinedIsolationDeltaBetaCorrRaw3Hits_2':{'drawname':'byCombinedIsolationDeltaBetaCorrRaw3Hits_2', 'nbins':30, 'min':0, 'max':200, 'label':'Tau dbeta isolation'},
    'jpt_1':{'drawname':'jpt_1', 'nbins':30, 'min':0, 'max':500, 'label':'leading jet pT (GeV)'},
    'jeta_1':{'drawname':'jeta_1', 'nbins':30, 'min':-5, 'max':5, 'label':'leading jet eta'},
    'jpt_2':{'drawname':'jpt_2', 'nbins':30, 'min':0, 'max':500, 'label':'sub leading jet pT (GeV)'},
    'jeta_2':{'drawname':'jeta_2', 'nbins':30, 'min':-5, 'max':5, 'label':'sub leading jet eta'},
    'pzeta_disc':{'drawname':'pzeta_disc', 'nbins':40, 'min':-250, 'max':150, 'label':'pZeta discriminator'},
    }


for catid, cat in categories.iteritems():

#    if catid != 'inclusive' and vardir.has_key('m_vis'):
#        del vardir['m_vis']

# usual way of deriving QCD
#    catname = 'signal_' + catid + '_ss'
#    sel_signal_ss = cat['sel'] + '&& q_1*q_2 > 0'
#            
#    config.createHistograms(catname, sel_signal_ss, ['QCD'], vardir, True)
#    config.extractQCD(catname, ['QCD'], vardir)
#        
#    catname = 'signal_' + catid + '_os'
#    sel_signal_os = sel_signal_ss.replace('q_1*q_2 > 0', 'q_1*q_2 < ')
#    
#    config.createHistograms(catname, sel_signal_os, ['QCD'], vardir)


# new way of deriving QCD

    # derive QCD yield with normal way
    catname = 'signal_' + catid + '_ss'
    sel_signal_ss = cat['sel'] + '&& q_1*q_2 > 0'
            
    config.createHistograms(catname, sel_signal_ss, ['QCD', 'Signal'], vardir, True)
    config.extractQCD(catname, ['QCD', 'Signal'], vardir)

    # derive QCD shape with relaxed isolation region
    catname = 'signal_relaxed_' + catid + '_ss'
    sel_signal_relaxed_ss = cat['sel'].replace('iso_1 < 0.2 && iso_2 < 0.15', 'iso_1 > 0.2 && iso_1 < 0.5 && iso_2 < 0.5') + '&& q_1*q_2 > 0'
    
    config.createHistograms(catname, sel_signal_relaxed_ss, ['QCD', 'Signal'], vardir, True)
    config.extractQCD(catname, ['QCD', 'Signal'], vardir)

    # draw in the signal region
    catname = 'signal_' + catid + '_os'
    sel_signal_os = sel_signal_ss.replace('q_1*q_2 > 0', 'q_1*q_2 < 0')
    
    # config.createHistograms(catname, sel_signal_os, ['QCD'], vardir, qcdratio, blind)

    if catid!='inclusive':
        config.createHistograms(catname, sel_signal_os, ['QCD'], vardir, False, True)
    else:
        config.createHistograms(catname, sel_signal_os, ['QCD'], vardir, False, False)
