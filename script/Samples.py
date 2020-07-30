#!/usr/bin/env python2
#
# For 2017.
#
# Attention!!!
# Do NOT use any space in the file names or legends, or they will cause problems when used as arguments when calling C++ programs.

# Signal MC
'''
sig_TThad = ['TT_FCNC-TtoHJ_aThad_hct', 'TT_FCNC-TtoHJ_aThad_hut', 'TT_FCNC-aTtoHJ_Thad_hct', 'TT_FCNC-aTtoHJ_Thad_hut']
sig_SThad = ['ST_FCNC-TH_Thad_hct', 'ST_FCNC-TH_Thad_hut']
sig_TTlep = ['TT_FCNC-TtoHJ_aTlep_hct', 'TT_FCNC-TtoHJ_aTlep_hut', 'TT_FCNC-aTtoHJ_Tlep_hct', 'TT_FCNC-aTtoHJ_Tlep_hut']
sig_STlep = ['ST_FCNC-TH_Tlep_hct', 'ST_FCNC-TH_Tlep_hut']
'''
sig_MC = {
	('TT', 'had', 'hct'): [
		('TT_FCNC-TtoHJ_aThad_hct', 'TT_FCNC-TtoHJ_aThadronic_HToaa_eta_hct-MadGraph5-pythia8.root'),
		('TT_FCNC-aTtoHJ_Thad_hct', 'TT_FCNC-aTtoHJ_Thadronic_HToaa_eta_hct-MadGraph5-pythia8.root')
	],
	('TT', 'had', 'hut'): [
		('TT_FCNC-TtoHJ_aThad_hut', 'TT_FCNC-TtoHJ_aThadronic_HToaa_eta_hut-MadGraph5-pythia8.root'),
		('TT_FCNC-aTtoHJ_Thad_hut', 'TT_FCNC-aTtoHJ_Thadronic_HToaa_eta_hut-MadGraph5-pythia8.root')
	],
	('TT', 'lep', 'hct'): [
		('TT_FCNC-TtoHJ_aTlep_hct', 'TT_FCNC-T2HJ_aTleptonic_HToaa_eta_hct-MadGraph5-pythia8.root'),
		('TT_FCNC-aTtoHJ_Tlep_hct', 'TT_FCNC-aTtoHJ_Tleptonic_HToaa_eta_hct-MadGraph5-pythia8.root')
	],
	('TT', 'lep', 'hut'): [
		('TT_FCNC-TtoHJ_aTlep_hut', 'TT_FCNC-TtoHJ_aTleptonic_HToaa_eta_hut-MadGraph5-pythia8.root'),
		('TT_FCNC-aTtoHJ_Tlep_hut', 'TT_FCNC-aTtoHJ_Tleptonic_HToaa_eta_hut-MadGraph5-pythia8.root')
	],
	('ST', 'had', 'hct'): [('ST_FCNC-TH_Thad_hct', 'ST_FCNC-TH_Thadronic_HToaa_eta_hct-MadGraph5-pythia8.root')],
	('ST', 'had', 'hut'): [('ST_FCNC-TH_Thad_hut', 'ST_FCNC-TH_Thadronic_HToaa_eta_hut-MadGraph5-pythia8.root')],
	('ST', 'lep', 'hct'): [('ST_FCNC-TH_Tlep_hct', 'ST_FCNC-TH_Tleptonic_HToaa_eta_hct-MadGraph5-pythia8.root')],
	('ST', 'lep', 'hut'): [('ST_FCNC-TH_Tlep_hut', 'ST_FCNC-TH_Tleptonic_HToaa_eta_hut-MadGraph5-pythia8.root')],
}

sig_MC_s = {}
for nt_type in sig_MC:
	tmp = []
	for nt in sig_MC[nt_type]:
		tmp.append(nt[0])
	sig_MC_s[nt_type] = tmp

'''
sig_had = sig_TThad + sig_SThad
sig_lep = sig_TTlep + sig_STlep
sig_TT = sig_TThad + sig_TTlep
sig_ST = sig_SThad + sig_STlep
sig = sig_had + sig_lep
'''

'''
# bkg MC & data (2017)
data = ["data"]
bkg_GGJets = ["DiPhotonJetsBox"]
bkg_GJet = ["GJet_Pt-20to40_MGG-80toInf", "GJet_Pt-20toInf_MGG-40to80", "GJet_Pt-40toInf_MGG-80toInf"]
bkg_QCD = ["QCD_Pt-30to40_MGG-80toInf", "QCD_Pt-30toInf_MGG-40to80", "QCD_Pt-40toInf_MGG-80toInf"]
bkg_TGJets = ["TGJets"]
bkg_tW = ["tW_antitop", "tW_top"]
bkg_TTGG = ["TTGG"]
bkg_TTGJets = ["TTGJets"]
bkg_TTJets = ["TTJets"]
bkg_TTV = ["TTWJetsToLNu", "TTZToLLNuNu"]
bkg_DY = ["DYJetsToLL"]
bkg_VG = ["WGToLNuG", "ZGToLLG"]
bkg_VV = ["WW", "WZ", "ZZ"]
bkg_Higgs = ["GluGluHToGG", "VBFHToGG", "VHToGG", "ttHJetToGG", "THQ_HToGG", "THW_HToGG"]

bkg = bkg_GGJets + bkg_GJet + bkg_QCD + bkg_TGJets + bkg_tW \
	+ bkg_TTGG + bkg_TTGJets + bkg_TTJets + bkg_TTV \
	+ bkg_DY + bkg_VG + bkg_VV \
	+ bkg_Higgs
bkg_noQCD = bkg_GGJets + bkg_GJet + bkg_TGJets + bkg_tW \
	+ bkg_TTGG + bkg_TTGJets + bkg_TTJets + bkg_TTV \
	+ bkg_DY + bkg_VG + bkg_VV \
	+ bkg_Higgs

bkg_data = data + bkg
bkg_noQCD_data = data + bkg_noQCD

bkg_cat_file = ['DiPhotonJetsBox', 'GJet', 'QCD', 'TGJets', 'tW', 'TTGG', 'TTGJets', 'TTJets', 'TTV', 'DYJetsToLL', 'VG', 'VV', 'Higgs']
bkg_cat_name = ['GGJets', 'GJet', 'QCD', 'TGJets', 'tW', 'TTGG', 'TTGJets', 'TTJets', 'TTV', 'DY', 'VG', 'VV', 'Higgs']

bkg_file_multi = [ ['DiPhotonJetsBox', 'DiPhotonJetsBox_MGG-80toInf_13TeV-Sherpa/*.root'] ]
'''
# Data
data = {
	'data': [('data', 'DoubleEG.root')]
}

# Bkg MC
bkg_MC = {
	'GGJets': [('DiPhotonJetsBox', 'DiPhotonJetsBox_MGG-80toInf_13TeV-Sherpa/*.root')],
	'GJet': [
		('GJet_Pt-20to40_MGG-80toInf', 'GJet_Pt-20to40_DoubleEMEnriched_MGG-80toInf_TuneCP5_13TeV_Pythia8.root'),
		('GJet_Pt-20toInf_MGG-40to80', 'GJet_Pt-20toInf_DoubleEMEnriched_MGG-40to80_TuneCP5_13TeV_Pythia8.root'),
		('GJet_Pt-40toInf_MGG-80toInf', 'GJet_Pt-40toInf_DoubleEMEnriched_MGG-80toInf_TuneCP5_13TeV_Pythia8.root')
	],
	'QCD': [
		('QCD_Pt-30to40_MGG-80toInf', 'QCD_Pt-30to40_DoubleEMEnriched_MGG-80toInf_TuneCP5_13TeV_Pythia8.root'),
		('QCD_Pt-30toInf_MGG-40to80', 'QCD_Pt-30toInf_DoubleEMEnriched_MGG-40to80_TuneCP5_13TeV_Pythia8.root'),
		('QCD_Pt-40toInf_MGG-80toInf', 'QCD_Pt-40toInf_DoubleEMEnriched_MGG-80toInf_TuneCP5_13TeV_Pythia8.root')
	],
	'TGJets': [('TGJets', 'TGJets_TuneCP5_13TeV_amcatnlo_madspin_pythia8.root')],
	'tW': [
		('tW_antitop', 'ST_tW_antitop_5f_inclusiveDecays_TuneCP5_13TeV-powheg-pythia8.root'),
		('tW_top', 'ST_tW_top_5f_inclusiveDecays_TuneCP5_13TeV-powheg-pythia8.root')
	],
	'TTGG': [('TTGG', 'TTGG_0Jets_TuneCP5_13TeV_amcatnlo_madspin_pythia8.root')],
	'TTGJets': [('TTGJets', 'TTGJets_TuneCP5_13TeV-amcatnloFXFX-madspin-pythia8.root')],
	'TTJets': [('TTJets', 'TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8.root')],
	'TTV': [
		('TTWJetsToLNu', 'TTWJetsToLNu_TuneCP5_13TeV-amcatnloFXFX-madspin-pythia8.root'),
		('TTZToLLNuNu', 'TTZToLLNuNu_M-10_TuneCP5_13TeV-amcatnlo-pythia8.root')
	],
	'DY': [('DYJetsToLL', 'DYJetsToLL_M-50_TuneCP5_13TeV-amcatnloFXFX-pythia8.root')],
	'VG': [
		('WGToLNuG', 'WGToLNuG_01J_5f_TuneCP5_13TeV-amcatnloFXFX-pythia8.root'),
		('ZGToLLG', 'ZGToLLG_01J_5f_TuneCP5_13TeV-amcatnloFXFX-pythia8.root')
	],
	'VV': [
		('WW', 'WW_TuneCP5_13TeV-pythia8.root'),
		('WZ', 'WZ_TuneCP5_13TeV-pythia8.root'),
		('ZZ', 'ZZ_TuneCP5_13TeV-pythia8.root')
	],
	'Higgs': [
		('GluGluHToGG', 'GluGluHToGG_M125_13TeV_amcatnloFXFX_pythia8.root'),
		('VBFHToGG', 'VBFHToGG_M125_13TeV_amcatnlo_pythia8.root'),
		('VHToGG', 'VHToGG_M125_13TeV_amcatnloFXFX_madspin_pythia8.root'),
		('ttHJetToGG', 'ttHJetToGG_M125_13TeV_amcatnloFXFX_madspin_pythia8.root'),
		('THQ_HToGG', 'THQ_ctcvcp_HToGG_M125_13TeV-madgraph-pythia8_TuneCP5.root'),
		('THW_HToGG', 'THW_ctcvcp_HToGG_M125_13TeV-madgraph-pythia8_TuneCP5.root')
	],
}

bkg_MC_s = {}
for cat in bkg_MC:
	tmp = []
	for nt in bkg_MC[cat]:
		tmp.append(nt[0])
	bkg_MC_s[cat] = tmp

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
