import os, copy
from ROOT import TFile, TH1F
from DisplayManager import DisplayManager
from DataMCPlot import *

gROOT.Macro('./functionmacro.C+') 

#prefix = 'Summer2016'
#prefix = 'ICHEP'
prefix = 'Moriond'
#tauidsf = 0.95
tauidsf = 1.

class config(object):

    def __init__(self, basedir, lumi, tes=None):
        
        self.tes = tes

        self.basedir = basedir

        self.lumi = lumi

        self.hists = {}

        self.process = {

            'TT':{'name':'TT', 
                   'file':self.basedir + '/TT/TauTauAnalysis.TT_TuneCUETP8M1_' + prefix + '.root',
                   'additional_cut':'1',
                   'cross-section':831.76,
                   'isSignal':0, 
                   'order':1},

#            'ZTT10to50':{'name':'ZTT10to50', 
#                         'file':self.basedir + '/DY/TauTauAnalysis.DYJetsToLL_M-10to50_TuneCUETP8M1_' + prefix + '.root', 
#                         'additional_cut':'1',
#                         'cross-section':18610.0,
#                         'isSignal':0, 
#                         'order':2},

            'ZTT':{'name':'ZTT', 
                   'file':self.basedir + '/DY/TauTauAnalysis.DYJetsToLL_M-50_TuneCUETP8M1_' + prefix + '.root',
                   'cross-section':1.,
                   'additional_cut':'gen_match_2==5',
                   'isSignal':0, 
                   'order':3},

            'ZTT1':{'name':'ZTT1', 
                   'file':self.basedir + '/DY/TauTauAnalysis.DY1JetsToLL_M-50_TuneCUETP8M1_' + prefix + '.root',
                   'cross-section':1.,
                   'additional_cut':'gen_match_2==5',
                   'isSignal':0, 
                   'order':3},

            'ZTT2':{'name':'ZTT2', 
                   'file':self.basedir + '/DY/TauTauAnalysis.DY2JetsToLL_M-50_TuneCUETP8M1_' + prefix + '.root',
                   'cross-section':1.,
                   'additional_cut':'gen_match_2==5',
                   'isSignal':0, 
                   'order':3},

            'ZTT3':{'name':'ZTT3', 
                   'file':self.basedir + '/DY/TauTauAnalysis.DY3JetsToLL_M-50_TuneCUETP8M1_' + prefix + '.root',
                   'cross-section':1.,
                   'additional_cut':'gen_match_2==5',
                   'isSignal':0, 
                   'order':3},

            'ZTT4':{'name':'ZTT4', 
                   'file':self.basedir + '/DY/TauTauAnalysis.DY4JetsToLL_M-50_TuneCUETP8M1_' + prefix + '.root',
                   'cross-section':1.,
                   'additional_cut':'gen_match_2==5',
                   'isSignal':0, 
                   'order':3},

            'ZL':{'name':'ZL', 
                  'file':self.basedir + '/DY/TauTauAnalysis.DYJetsToLL_M-50_TuneCUETP8M1_' + prefix + '.root',
                  'cross-section':1.,
                  'additional_cut':'gen_match_2<5',
                  'isSignal':0, 
                  'order':3},

            'ZL1':{'name':'ZL1', 
                  'file':self.basedir + '/DY/TauTauAnalysis.DY1JetsToLL_M-50_TuneCUETP8M1_' + prefix + '.root',
                  'cross-section':1.,
                  'additional_cut':'gen_match_2<5',
                  'isSignal':0, 
                  'order':3},

            'ZL2':{'name':'ZL2', 
                   'file':self.basedir + '/DY/TauTauAnalysis.DY2JetsToLL_M-50_TuneCUETP8M1_' + prefix + '.root',
                   'cross-section':1.,
                   'additional_cut':'gen_match_2<5',
                   'isSignal':0, 
                   'order':3},

            'ZL3':{'name':'ZL3', 
                   'file':self.basedir + '/DY/TauTauAnalysis.DY3JetsToLL_M-50_TuneCUETP8M1_' + prefix + '.root',
                   'cross-section':1.,
                   'additional_cut':'gen_match_2<5',
                   'isSignal':0, 
                   'order':3},

            'ZL4':{'name':'ZL4', 
                   'file':self.basedir + '/DY/TauTauAnalysis.DY4JetsToLL_M-50_TuneCUETP8M1_' + prefix + '.root',
                   'cross-section':1.,
                   'additional_cut':'gen_match_2<5',
                   'isSignal':0, 
                   'order':3},


            'ZJ':{'name':'ZJ', 
                  'file':self.basedir + '/DY/TauTauAnalysis.DYJetsToLL_M-50_TuneCUETP8M1_' + prefix + '.root',
                  'additional_cut':'gen_match_2==6',
                  'cross-section':1.,
                  'isSignal':0, 
                  'order':3},

            'ZJ1':{'name':'ZJ1', 
                  'file':self.basedir + '/DY/TauTauAnalysis.DY1JetsToLL_M-50_TuneCUETP8M1_' + prefix + '.root',
                  'additional_cut':'gen_match_2==6',
                  'cross-section':1.,
                  'isSignal':0, 
                  'order':3},

            'ZJ2':{'name':'ZJ2', 
                  'file':self.basedir + '/DY/TauTauAnalysis.DY2JetsToLL_M-50_TuneCUETP8M1_' + prefix + '.root',
                  'additional_cut':'gen_match_2==6',
                  'cross-section':1.,
                  'isSignal':0, 
                  'order':3},

            'ZJ3':{'name':'ZJ3', 
                  'file':self.basedir + '/DY/TauTauAnalysis.DY3JetsToLL_M-50_TuneCUETP8M1_' + prefix + '.root',
                  'additional_cut':'gen_match_2==6',
                  'cross-section':1.,
                  'isSignal':0, 
                  'order':3},

            'ZJ4':{'name':'ZJ4', 
                  'file':self.basedir + '/DY/TauTauAnalysis.DY4JetsToLL_M-50_TuneCUETP8M1_' + prefix + '.root',
                  'additional_cut':'gen_match_2==6',
                  'cross-section':1.,
                  'isSignal':0, 
                  'order':3},
            
            'WWTo1L1Nu2Q':{'name':'WWTo1L1Nu2Q', 
                           'file':self.basedir + '/WW/TauTauAnalysis.WWTo1L1Nu2Q_13TeV_nlo_' + prefix + '.root',
                           'cross-section':49.997,
                           'additional_cut':'1',
                           'isSignal':0, 
                           'order':4},
           
           
            'WZTo3LNu':{'name':'WZTo3LNu', 
                        'file':self.basedir + '/WZ/TauTauAnalysis.WZTo3LNu_TuneCUETP8M1_13TeV_nlo_' + prefix + '.root', 
                        'cross-section':3.05,
                        'additional_cut':'1',
                        'isSignal':0, 
                        'order':6},

            'WZTo2L2Q':{'name':'WZTo2L2Q', 
                        'file':self.basedir + '/WZ/TauTauAnalysis.WZTo2L2Q_13TeV_nlo_' + prefix + '.root', 
                        'cross-section':5.595,
                        'additional_cut':'1',
                        'isSignal':0, 
                        'order':6},

            'WZTo1L1Nu2Q':{'name':'WZTo1L1Nu2Q', 
                           'file':self.basedir + '/WZ/TauTauAnalysis.WZTo1L1Nu2Q_13TeV_nlo_' + prefix + '.root', 
                           'cross-section':10.71,
                           'additional_cut':'1',
                           'isSignal':0, 
                           'order':6},
           
#            'ZZTo2Q2Nu':{'name':'ZZTo2Q2Nu', 
#                  'file':self.basedir + '/ZZ/TauTauAnalysis.ZZTo2Q2Nu_13TeV_nlo_' + prefix + '.root', 
#                  'cross-section':3.22,
#                  'additional_cut':'1',
#                  'isSignal':0, 
#                  'order':7},

            'ZZTo4L':{'name':'ZZTo4L', 
                  'file':self.basedir + '/ZZ/TauTauAnalysis.ZZTo4L_13TeV_nlo_' + prefix + '.root', 
                  'cross-section':1.212,
                  'additional_cut':'1',
                  'isSignal':0, 
                  'order':7},

#            'ZZTo4Q':{'name':'ZZTo4Q', 
#                  'file':self.basedir + '/ZZ/TauTauAnalysis.ZZTo4Q_13TeV_nlo_' + prefix + '.root', 
#                  'cross-section':,
#                  'additional_cut':'1',
#                  'isSignal':0, 
#                  'order':7},

            'VVTo2L2Nu':{'name':'VVTo2L2Nu', 
                      'file':self.basedir + '/ZZ/TauTauAnalysis.VVTo2L2Nu_13TeV_nlo_' + prefix + '.root', 
                      'cross-section':11.95,
                      'additional_cut':'1',
                      'isSignal':0, 
                      'order':7},

            
            'W':{'name':'W', 
                 'file':self.basedir + 'WJ/TauTauAnalysis.WJetsToLNu_TuneCUETP8M1_' + prefix + '.root', 
                 'cross-section':1.,
                 'additional_cut':'1',
                 'isSignal':0, 
                 'order':8},

            'W1':{'name':'W1', 
                 'file':self.basedir + 'WJ/TauTauAnalysis.W1JetsToLNu_TuneCUETP8M1_' + prefix + '.root', 
                 'cross-section':1.,
                 'additional_cut':'1',
                 'isSignal':0, 
                 'order':8},

            'W2':{'name':'W2', 
                 'file':self.basedir + 'WJ/TauTauAnalysis.W2JetsToLNu_TuneCUETP8M1_' + prefix + '.root', 
                 'cross-section':1.,
                 'additional_cut':'1',
                 'isSignal':0, 
                 'order':8},

            'W3':{'name':'W3', 
                 'file':self.basedir + 'WJ/TauTauAnalysis.W3JetsToLNu_TuneCUETP8M1_' + prefix + '.root', 
                 'cross-section':1.,
                 'additional_cut':'1',
                 'isSignal':0, 
                 'order':8},

            'W4':{'name':'W4', 
                 'file':self.basedir + 'WJ/TauTauAnalysis.W4JetsToLNu_TuneCUETP8M1_' + prefix + '.root', 
                 'cross-section':1.,
                 'additional_cut':'1',
                 'isSignal':0, 
                 'order':8},
    
            'ST_t_top':{'name':'ST_t_top', 
                        'file':self.basedir + 'ST/TauTauAnalysis.ST_t-channel_top_4f_inclusiveDecays_' + prefix + '.root', 
                        'cross-section':136.02,
                        'additional_cut':'1',
                        'isSignal':0, 
                        'order':9},
            
            
            'ST_t_antitop':{'name':'ST_t_antitop', 
                            'file':self.basedir + 'ST/TauTauAnalysis.ST_t-channel_antitop_4f_inclusiveDecays_' + prefix + '.root', 
                            'cross-section':80.95,
                            'additional_cut':'1',
                            'isSignal':0, 
                            'order':10},
            
            
            'ST_tw_top':{'name':'ST_tw_top', 
                         'file':self.basedir + 'ST/TauTauAnalysis.ST_tW_top_5f_inclusiveDecays_' + prefix + '.root', 
                         'cross-section':35.60,
                         'additional_cut':'1',
                         'isSignal':0, 
                         'order':11},
            
            'ST_tw_antitop':{'name':'ST_tw_antitop', 
                             'file':self.basedir + 'ST/TauTauAnalysis.ST_tW_antitop_5f_inclusiveDecays_' + prefix + '.root', 
                             'cross-section':35.60,
                             'additional_cut':'1',
                             'isSignal':0, 
                             'order':12},
            
            'Signal':{'name':'Signal', 
                      'file':self.basedir + 'LowMass/TauTauAnalysis.LowMass_30GeV_DiTauResonance_' + prefix + '.root', 
                      'cross-section':30.,
                      'additional_cut':'1',
                      'isSignal':3000, 
                      'order':4},
            
            'data_obs':{'name':'data_obs', 
                        'file':self.basedir + 'MuonEG/TauTauAnalysis.MuonEG_Run2016_' + prefix + '.root',
                        'cross-section':1.,
                        'additional_cut':'1',
                        'isSignal':0, 
                        'order':2999},

            'QCD':{'name':'QCD', 
                   'file':None,
                   'cross-section':None,
                   'additional_cut':'1',
                   'isSignal':0, 
                   'order':0},

            }



        print 'Retrieve normalization'

        for processname, val in self.process.iteritems():
            if processname=='QCD': continue
            
            filename = val['file']
            if self.tes!=None and processname != 'ZTT10to50':
                filename = filename.replace('DY/', 'DY_tes_' + str(self.tes) + '/')

#            file = TFile(val['file']) 
            print filename
            file = TFile(filename) 
            ntot = file.Get("histogram_emu/cutflow_emu").GetBinContent(8)
            self.process[processname]['ntot'] = ntot
            self.process[processname]['file'] = file

            print 'Register', processname.ljust(20), str(self.process[processname]['ntot']).ljust(15), 'SF = ', str(self.process[processname]['cross-section']*self.lumi*1000/self.process[processname]['ntot']).ljust(15)



        # DY stitching

        self.dy_xs = [4954.0, 1012.5, 332.8, 101.8, 54.8]

        self.dy_nlo_xs = 5765.4

        self.dykfactor = self.dy_nlo_xs/self.dy_xs[0]       

        print 'k-factor : DY = ', self.dykfactor, self.dy_nlo_xs, '(NLO) / ', self.dy_xs[0], '(LO)'

        self.dy_efflumi = []

        for ijet, processname in enumerate(['ZTT', 'ZTT1', 'ZTT2', 'ZTT3', 'ZTT4']):
            self.dy_efflumi.append(self.process[processname]['ntot']/self.dy_xs[ijet])


        # calculate DY weight 

        self.dy_weight = []
        for ijet, processname in enumerate(['ZTT', 'ZTT1', 'ZTT2', 'ZTT3', 'ZTT4']):
            if processname=='ZTT':
                self.dy_weight.append(self.dykfactor*self.lumi*1000/self.dy_efflumi[ijet])
            else:
                self.dy_weight.append(self.dykfactor*self.lumi*1000/(self.dy_efflumi[0] + self.dy_efflumi[ijet]))


        self.dy_weightstr = '('

        for ii, _weight in enumerate(self.dy_weight):
            self.dy_weightstr += '(NUP==' + str(ii) + ' ? ' + str(self.dy_weight[ii]) + ': 1)*'


        self.dy_weightstr += '1)'
        print self.dy_weightstr



        # W stitching 

        self.w_xs = [50380, 9644.5, 3144.5, 954.8, 485.6]

        self.w_nlo_xs = 61526.7

        self.wkfactor = self.w_nlo_xs/self.w_xs[0]       

        print 'k-factor : W = ', self.wkfactor, self.w_nlo_xs, '(NLO) / ', self.w_xs[0], '(LO)'

        self.w_efflumi = []

        for ijet, processname in enumerate(['W', 'W1', 'W2', 'W3', 'W4']):
            self.w_efflumi.append(self.process[processname]['ntot']/self.w_xs[ijet])


        # calculate W weight 

        self.w_weight = []
        for ijet, processname in enumerate(['W', 'W1', 'W2', 'W3', 'W4']):
            if processname=='W':
                self.w_weight.append(self.wkfactor*self.lumi*1000/self.w_efflumi[ijet])
            else:
                self.w_weight.append(self.wkfactor*self.lumi*1000/(self.w_efflumi[0] + self.w_efflumi[ijet]))


        self.w_weightstr = '('

        for ii, _weight in enumerate(self.w_weight):
            self.w_weightstr += '(NUP==' + str(ii) + ' ? ' + str(self.w_weight[ii]) + ': 1)*'


        self.w_weightstr += '1)'
        print self.w_weightstr


    def returnWeight(self, val):
        weight = 'weight*(trigweight_1_or/trigweight_1)*' + str(val['cross-section']*self.lumi*1000/val['ntot'])  + '*(gen_match_2==5 ? ' + str(tauidsf) + ' : 1)'

        if val['name'] in ['ZTT', 'ZTT1', 'ZTT2', 'ZTT3', 'ZTT4',
                           'ZL', 'ZL1', 'ZL2', 'ZL3', 'ZL4',
                           'ZJ', 'ZJ1', 'ZJ2', 'ZJ3', 'ZJ4']:            
            weight = 'weight*' + self.dy_weightstr + '*(gen_match_2==5 ? ' + str(tauidsf) + ' : 1)'

        elif val['name'] in ['W', 'W1', 'W2', 'W3', 'W4']:
            weight = 'weight*' + self.w_weightstr + '*(gen_match_2==5 ? ' + str(tauidsf) + ' : 1)'

        elif val['name'] in ['data_obs']:        
            weight = '1'

        return weight


    def createHistograms(self, catname, selection, exceptionList, vardir, qcdratio = False, blind = False, dcvar = None):

        print '-'*80
        print 'category : ', catname
        print 'selection : ', selection
        print 'exceptionList : ', exceptionList
        print 'vardir = ', vardir
        print '-'*80    

        for processname, val in self.process.iteritems():
            if processname in exceptionList: 
                print 'Remove', processname
                continue
        
            tree = val['file'].Get('tree_emu')

            var_tuples = []
    
            for varname, var in vardir.iteritems():
                print catname, processname, varname, var['drawname']
                hname = 'hist_' + catname + '_' + processname + '_' + var['drawname']
        
                hist_register = TH1F(hname, hname, var['nbins'], var['min'], var['max'])
                hist_register.GetXaxis().SetTitle(var['label'])
                hist_register.GetXaxis().SetLabelSize(0)
                hist_register.Sumw2()
    
                if val['name'] in ['data_obs']:
                    hist_register.SetMarkerStyle(20)
                    hist_register.SetMarkerSize(0.5)
        
                self.hists[hname] = hist_register

                var_tuples.append('{var} >> {hist}'.format(var=var['drawname'], hist=hname))

            weight = self.returnWeight(val)
            if processname in ['W', 'W1', 'W2', 'W3', 'W4']:
                weight += '*' + str(1)

            if qcdratio:
                weight += '*(getQCDWeight(pt_2, pt_1, dR_ll))'
#                weight += '*(getQCDWeight(100., 100., 0.2))'
            if blind:
                weight += '*(isData==1 ? 0 : 1)'

            print processname, weight



            cutstr = selection + ' && ' + val['additional_cut']
            cut = '({c}) * {we}'.format(c=cutstr, we=weight)
            tree.MultiDraw(var_tuples, cut)



        print 'making plots ...'
        ensureDir('fig/' + catname)

        for varname, var in vardir.iteritems():        

            stackname = 'stackhist_' + catname + '_' + varname
            hist = DataMCPlot(stackname)
            hist.legendBorders = 0.55, 0.55, 0.88, 0.88

            for processname, val in self.process.iteritems():

                pname = val['name']

                hname = 'hist_' + catname + '_' + processname + '_' + varname

                # added by YT
                if pname in ['QCD']:
                    hname = 'hist_' + catname.replace('signal', 'signal_relaxed') + '_' + processname + '_' + varname
                # added by YT end

                if not self.hists.has_key(hname): continue

                # added by YT
                if pname in ['QCD']:
                    # scale QCD histogram
                    hname_original = 'hist_' + catname + '_' + processname + '_' + varname
                    entries_original = self.hists[hname_original].Integral(0, self.hists[hname_original].GetNbinsX()+1)

                    entries_relaxed = self.hists[hname].Integral(0, self.hists[hname].GetNbinsX()+1)
                    qcdsf = entries_original/entries_relaxed

                    print '*'*80 
                    print hname, hname_original
                    print 'QCD SF (original, relaxed) ', entries_original, entries_relaxed, ' => ', qcdsf
                    print '*'*80 

                    self.hists[hname].Scale(qcdsf)
                # added by YT end


                hist.AddHistogram(pname, self.hists[hname], val['order'])
                if pname in ['data_obs', 'Signal']:
                    hist.Hist(pname).stack = False

#            hist.Group('VV', ['WZ', 'ZZ', 'WWTo1L1Nu2Q', 'ST_t_top', 'ST_t_antitop', 'ST_tw_top', 'ST_tw_antitop'])
            hist.Group('VV', ['WWTo1L1Nu2Q', 'WZTo3LNu', 'WZTo2L2Q', 'WZTo1L1Nu2Q', 'ZZTo2Q2Nu', 'ZZTo4L', 'ZZTo4Q', 'VVTo2L2Nu', 'ST_t_top', 'ST_t_antitop', 'ST_tw_top', 'ST_tw_antitop'])
            hist.Group('ZTT', ['ZTT', 'ZTT1', 'ZTT2', 'ZTT3', 'ZTT4'])
            hist.Group('ZL', ['ZL', 'ZL1', 'ZL2', 'ZL3', 'ZL4'])
            hist.Group('ZJ', ['ZJ', 'ZJ1', 'ZJ2', 'ZJ3', 'ZJ4'])
            hist.Group('W', ['W', 'W1', 'W2', 'W3', 'W4'])

            print hist
            self.comparisonPlots(hist, 'fig/' + catname + '/' + stackname + '.gif')
#            display = DisplayManager('fig_' + catname + '/' + stackname + '.gif', True, self.lumi, 0.42, 0.65)
#            display.Draw(hist)


            if varname==dcvar:
                hist.WriteDataCard(filename='datacard/datacard_tes_' + str(self.tes) + '_'+ catname +'_{}.root'.format(varname), dir='mt_signal', mode='recreate')







    def extractQCD(self, catname, exceptionList, vardir):

        for varname, var in vardir.iteritems(): 

            h_QCD = None

            for processname, val in self.process.iteritems():
                if processname in exceptionList: continue
        
                hname = 'hist_' + catname + '_' + processname + '_' + varname
        
                addfactor = -1.
                if val['name'] == 'data_obs':
                    addfactor = 1.
            
                if h_QCD == None:
                    h_QCD = copy.deepcopy(self.hists[hname])
                    h_QCD.Scale(addfactor)
#                print 'subtract', hname, self.hists[hname].Integral(0, self.hists[hname].GetNbinsX()+1)
#                print 'subtract_copy', h_QCD.Integral(0, h_QCD.GetNbinsX()+1)
                else:
                    h_QCD.Add(self.hists[hname], addfactor)
#                print 'subtract', hname, self.hists[hname].Integral(0, self.hists[hname].GetNbinsX()+1)
#                print 'subtract_copy', h_QCD.Integral(0, h_QCD.GetNbinsX()+1)

            
            h_QCD_os = copy.deepcopy(h_QCD)
#            h_QCD_os.Scale(2.4) 

            osname = 'hist_' + catname.replace('ss','os') + '_QCD_' + varname
            h_QCD_os.SetName(osname)

#            entries = h_QCD.Integral(0, h_QCD_os.GetNbinsX()+1)
#            print '-'*80
#            print 'Original yield = ', h_QCD.Integral(0, h_QCD_os.GetNbinsX()+1)
#        #        print 'OS/SS ratio = ', os_ss_ratio
#            print 'Estimated QCD yield at ', catname, ' : ', h_QCD_os.Integral(0, h_QCD_os.GetNbinsX()+1)
#            print '-'*80

            self.hists[osname] = h_QCD_os


    def comparisonPlots(self, hist, pname='sync.pdf', isRatio=True):
        display = DisplayManager(pname, isRatio, self.lumi, 0.42, 0.65)
        display.Draw(hist)


def ensureDir(directory):
    if not os.path.exists(directory):
        os.makedirs(directory)



def returnIntegral(hist):
    return hist.Integral(0, hist.GetNbinsX()+1)
