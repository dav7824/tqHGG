#!/usr/bin/env python2
#
# For 2017.

# Names of signal samples
sig_TThad = ["TT_FCNC-TtoHJ_aThad_hct", "TT_FCNC-TtoHJ_aThad_hut", "TT_FCNC-aTtoHJ_Thad_hct", "TT_FCNC-aTtoHJ_Thad_hut"]
sig_SThad = ["ST_FCNC-TH_Thad_hct", "ST_FCNC-TH_Thad_hut"]
sig_TTlep = ["TT_FCNC-TtoHJ_aTlep_hct", "TT_FCNC-TtoHJ_aTlep_hut", "TT_FCNC-aTtoHJ_Tlep_hct", "TT_FCNC-aTtoHJ_Tlep_hut"]
sig_STlep = ["ST_FCNC-TH_Tlep_hct", "ST_FCNC-TH_Tlep_hut"]

sig_had = sig_TThad + sig_SThad
sig_lep = sig_TTlep + sig_STlep
sig_TT = sig_TThad + sig_TTlep
sig_ST = sig_SThad + sig_STlep
sig = sig_had + sig_lep

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
