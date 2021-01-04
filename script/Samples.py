#!/usr/bin/env python2
#
# For 2017.
#
# Attention!!!
# Do NOT use any space in the file names or legends, or they will cause problems when used as arguments when calling C++ programs.
# If there's space in some file names or legends, quote them when used as arguments.

# Signal MC
sig_MC = {
        ('TT', 'had', 'hct'): [ 'TT_FCNC-TtoHJ_aThad_hct', 'TT_FCNC-aTtoHJ_Thad_hct' ],
        ('TT', 'had', 'hut'): [ 'TT_FCNC-TtoHJ_aThad_hut', 'TT_FCNC-aTtoHJ_Thad_hut' ],
        ('TT', 'lep', 'hct'): [ 'TT_FCNC-TtoHJ_aTlep_hct', 'TT_FCNC-aTtoHJ_Tlep_hct' ],
        ('TT', 'lep', 'hut'): [ 'TT_FCNC-TtoHJ_aTlep_hut', 'TT_FCNC-aTtoHJ_Tlep_hut' ],
        ('ST', 'had', 'hct'): [ 'ST_FCNC-TH_Thad_hct' ],
        ('ST', 'had', 'hut'): [ 'ST_FCNC-TH_Thad_hut' ],
        ('ST', 'lep', 'hct'): [ 'ST_FCNC-TH_Tlep_hct' ],
        ('ST', 'lep', 'hut'): [ 'ST_FCNC-TH_Tlep_hut' ],
        }
sig_MC_file = {
        ('TT', 'had', 'hct'): [
            'TT_FCNC-TtoHJ_aThadronic_HToaa_eta_hct-MadGraph5-pythia8/*.root',
            'TT_FCNC-aTtoHJ_Thadronic_HToaa_eta_hct-MadGraph5-pythia8/*.root'
            ],
        ('TT', 'had', 'hut'): [
            'TT_FCNC-TtoHJ_aThadronic_HToaa_eta_hut-MadGraph5-pythia8/*.root',
            'TT_FCNC-aTtoHJ_Thadronic_HToaa_eta_hut-MadGraph5-pythia8/*.root'
            ],
        ('TT', 'lep', 'hct'): [
            'TT_FCNC-T2HJ_aTleptonic_HToaa_eta_hct-MadGraph5-pythia8/*.root',
            'TT_FCNC-aTtoHJ_Tleptonic_HToaa_eta_hct-MadGraph5-pythia8/*.root'
            ],
        ('TT', 'lep', 'hut'): [
            'TT_FCNC-TtoHJ_aTleptonic_HToaa_eta_hut-MadGraph5-pythia8/*.root',
            'TT_FCNC-aTtoHJ_Tleptonic_HToaa_eta_hut-MadGraph5-pythia8/*.root'
            ],
        ('ST', 'had', 'hct'): [ 'ST_FCNC-TH_Thadronic_HToaa_eta_hct-MadGraph5-pythia8/*.root' ],
        ('ST', 'had', 'hut'): [ 'ST_FCNC-TH_Thadronic_HToaa_eta_hut-MadGraph5-pythia8/*.root' ],
        ('ST', 'lep', 'hct'): [ 'ST_FCNC-TH_Tleptonic_HToaa_eta_hct-MadGraph5-pythia8/*.root' ],
        ('ST', 'lep', 'hut'): [ 'ST_FCNC-TH_Tleptonic_HToaa_eta_hut-MadGraph5-pythia8/*.root' ],
        }

# Bkg MC
bkg_MC = {
        'GGJets': [ 'DiPhotonJetsBox' ],
        'GJet':   [ 'GJet_Pt-20to40_MGG-80toInf', 'GJet_Pt-20toInf_MGG-40to80', 'GJet_Pt-40toInf_MGG-80toInf' ],
        'QCD':    [ 'QCD_Pt-30to40_MGG-80toInf',  'QCD_Pt-30toInf_MGG-40to80', 'QCD_Pt-40toInf_MGG-80toInf' ],
        'TGJets': [ 'TGJets' ],
        'tW':     [ 'tW_antitop', 'tW_top' ],
        'TTGG':   [ 'TTGG' ],
        'TTGJets': [ 'TTGJets' ],
        'TTJets': [ 'TTJets' ],
        'TTV':    [ 'TTWJetsToLNu', 'TTZToLLNuNu' ],
        'DY':     [ 'DYJetsToLL' ],
        'VG':     [ 'WGToLNuG', 'ZGToLLG' ],
        'VV':     [ 'WW', 'WZ', 'ZZ' ],
        'Higgs':  [ 'GluGluHToGG', 'VBFHToGG', 'VHToGG', 'ttHJetToGG' ],
        }
bkg_MC_file = {
        'GGJets': [ 'DiPhotonJetsBox_MGG-80toInf_13TeV-Sherpa/*.root' ],
        'GJet': [
            'GJet_Pt-20to40_DoubleEMEnriched_MGG-80toInf_TuneCP5_13TeV_Pythia8.root',
            'GJet_Pt-20toInf_DoubleEMEnriched_MGG-40to80_TuneCP5_13TeV_Pythia8.root',
            'GJet_Pt-40toInf_DoubleEMEnriched_MGG-80toInf_TuneCP5_13TeV_Pythia8.root'
            ],
        'QCD': [
            'QCD_Pt-30to40_DoubleEMEnriched_MGG-80toInf_TuneCP5_13TeV_Pythia8.root',
            'QCD_Pt-30toInf_DoubleEMEnriched_MGG-40to80_TuneCP5_13TeV_Pythia8.root',
            'QCD_Pt-40toInf_DoubleEMEnriched_MGG-80toInf_TuneCP5_13TeV_Pythia8.root'
            ],
        'TGJets': [ 'TGJets_TuneCP5_13TeV_amcatnlo_madspin_pythia8.root' ],
        'tW': [
            'ST_tW_antitop_5f_inclusiveDecays_TuneCP5_13TeV-powheg-pythia8.root',
            'ST_tW_top_5f_inclusiveDecays_TuneCP5_13TeV-powheg-pythia8.root'
            ],
        'TTGG': [ 'TTGG_0Jets_TuneCP5_13TeV_amcatnlo_madspin_pythia8.root' ],
        'TTGJets': [ 'TTGJets_TuneCP5_13TeV-amcatnloFXFX-madspin-pythia8.root' ],
        'TTJets': [ 'TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8.root' ],
        'TTV': [
            'TTWJetsToLNu_TuneCP5_13TeV-amcatnloFXFX-madspin-pythia8.root',
            'TTZToLLNuNu_M-10_TuneCP5_13TeV-amcatnlo-pythia8.root'
            ],
        'DY': [ 'DYJetsToLL_M-50_TuneCP5_13TeV-amcatnloFXFX-pythia8.root' ],
        'VG': [
            'WGToLNuG_01J_5f_TuneCP5_13TeV-amcatnloFXFX-pythia8.root',
            'ZGToLLG_01J_5f_TuneCP5_13TeV-amcatnloFXFX-pythia8.root'
            ],
        'VV': [
            'WW_TuneCP5_13TeV-pythia8.root',
            'WZ_TuneCP5_13TeV-pythia8.root',
            'ZZ_TuneCP5_13TeV-pythia8.root'
            ],
        'Higgs': [
            'GluGluHToGG_M125_13TeV_amcatnloFXFX_pythia8/*.root',
            'VBFHToGG_M125_13TeV_amcatnlo_pythia8/*.root',
            'VHToGG_M125_13TeV_amcatnloFXFX_madspin_pythia8/*.root',
            'ttHJetToGG_M125_13TeV_amcatnloFXFX_madspin_pythia8/*.root'
            ],
        }

'''
bkg_MC_leg = {
        'GGJets': '#gamma#gamma+jets',
        'GJet': '#gamma+jets',
        'QCD': 'QCD',
        'TGJets': 't#gamma+jets',
        'tW': 't+W',
        'TTGG': 't#bar{t}#gamma#gamma',
        'TTGJets': 't#bar{t}#gamma+jets',
        'TTJets': 't#bar{t}+jets',
        'TTV': 't#bar{t}+V',
        'DY': 'DY',
        'VG': 'V#gamma',
        'VV': 'VV',
        'Higgs': 'SM_Higgs',
        }

bkg_MC_stack_order = [
        'Higgs',
        'TTGG',
        'TTGJets',
        'TTJets',
        'TTV',
        'TGJets',
        'tW',
        'VG',
        'VV',
        'DY',
        'GGJets',
        'GJet',
        'QCD',
        ]
        '''
