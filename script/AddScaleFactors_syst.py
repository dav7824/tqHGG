#!/usr/bin/env python2

import Path, Util, SystUnc
from Samples import sig_MC, bkg_MC
import os, sys
from os.path import exists, join


def Process(cmd, nt):
    print 'Processing:', nt
    fp = os.popen( cmd.format(nt=nt) )
    #print fp.read()
    fp.close()


def ApplyPileupSF(indir_name, ch, syst, systdir):
    print '---Calculating pileup SF:', indir_name
    print '---Systematic:', syst+systdir
    exe = join(Path.dir_bin, 'SFcalc_PU')
    fhist = join(Path.dir_2017, 'SF', 'MCPileUp.root')
    indir = join(Path.dir_2017, 'Systematics', indir_name, syst+systdir)
    if systdir!='Up' and systdir!='Down':
        print '[ERROR] Systematic variation has to be "Up" or "Down"!'
        sys.exit(1)
    if not (syst in SystUnc.Syst_tree_1 or syst in SystUnc.Syst_tree_2):
        print '[ERROR] Invalid systematic name!'
        sys.exit(1)
    if ch!='had' and ch!='lep':
        print '[ERROR] Invalid channel!'
        sys.exit(1)
    if not exists(exe):
        print '[ERROR] Executable does not exist!'
        sys.exit(1)
    if not exists(fhist):
        print '[ERROR] Histogram does not exist!'
        sys.exit(1)
    if not exists(indir):
        print '[ERROR] Input directory does not exist!'
        sys.exit(1)

    # Command template
    cmd = '{exe} {indir}/{{nt}}.root {fhist}'.format(exe=exe, indir=indir, fhist=fhist)

    # Process signal MC
    for sigtype in sig_MC:
        if sigtype[1] != ch:
            continue
        for nt in sig_MC[sigtype]:
            Process(cmd, nt)
    # Process resonant bkg
    for nt in bkg_MC['Higgs']:
        Process(cmd, nt)
# End of function ApplyPileupSF


def ApplyElecSF(indir_name, ch, syst, systdir):
    print '---Calculating electron SF:', indir_name
    print '---Systematic:', syst+systdir
    exe = join(Path.dir_bin, 'SFcalc_Electron')
    freco0 = join(Path.dir_2017, 'SF', 'egammaEffi.txt_EGM2D_runBCDEF_passingRECO_lowEt.root')
    freco1 = join(Path.dir_2017, 'SF', 'egammaEffi.txt_EGM2D_runBCDEF_passingRECO.root')
    fid = join(Path.dir_2017, 'SF', '2017_ElectronMedium.root')
    indir = join(Path.dir_2017, 'Systematics', indir_name, syst+systdir)
    if systdir!='Up' and systdir!='Down':
        print '[ERROR] Systematic variation has to be "Up" or "Down"!'
        sys.exit(1)
    if not (syst in SystUnc.Syst_tree_1 or syst in SystUnc.Syst_tree_2):
        print '[ERROR] Invalid systematic name!'
        sys.exit(1)
    if ch!='had' and ch!='lep':
        print '[ERROR] Invalid channel!'
        sys.exit(1)
    if not exists(exe):
        print '[ERROR] Executable does not exist!'
        sys.exit(1)
    if not (exists(freco0) and exists(freco1) and exists(fid)):
        print '[ERROR] Histogram does not exist!'
        sys.exit(1)
    if not exists(indir):
        print '[ERROR] Input directory does not exist!'
        sys.exit(1)

    # Command template
    cmd = '{exe} {indir}/{{nt}}.root {ch} {freco0} {freco1} {fid} nosyst'.format(
            exe=exe, indir=indir, ch=ch, freco0=freco0, freco1=freco1, fid=fid)

    # Process signal MC
    for sigtype in sig_MC:
        if sigtype[1] != ch:
            continue
        for nt in sig_MC[sigtype]:
            Process(cmd, nt)
    # Process resonant bkg
    for nt in bkg_MC['Higgs']:
        Process(cmd, nt)
# End of function ApplyElecSF


def ApplyMuonSF(indir_name, ch, syst, systdir):
    print '---Calculating muon SF:', indir_name
    print '---Systematic:', syst+systdir
    exe = join(Path.dir_bin, 'SFcalc_Muon')
    fid = join(Path.dir_2017, 'SF', 'RunBCDEF_SF_ID.root')
    fiso = join(Path.dir_2017, 'SF', 'RunBCDEF_SF_ISO.root')
    indir = join(Path.dir_2017, 'Systematics', indir_name, syst+systdir)
    if systdir!='Up' and systdir!='Down':
        print '[ERROR] Systematic variation has to be "Up" or "Down"!'
        sys.exit(1)
    if not (syst in SystUnc.Syst_tree_1 or syst in SystUnc.Syst_tree_2):
        print '[ERROR] Invalid systematic name!'
        sys.exit(1)
    if ch!='had' and ch!='lep':
        print '[ERROR] Invalid channel!'
        sys.exit(1)
    if not exists(exe):
        print '[ERROR] Executable does not exist!'
        sys.exit(1)
    if not (exists(fid) and exists(fiso)):
        print '[ERROR] Histogram does not exist!'
        sys.exit(1)
    if not exists(indir):
        print '[ERROR] Input directory does not exist!'
        sys.exit(1)

    # Command template
    cmd = '{exe} {indir}/{{nt}}.root {ch} {fid} {fiso} nosyst'.format(
            exe=exe, indir=indir, ch=ch, fid=fid, fiso=fiso)

    # Process signal MC
    for sigtype in sig_MC:
        if sigtype[1] != ch:
            continue
        for nt in sig_MC[sigtype]:
            Process(cmd, nt)
    # Process resonant bkg
    for nt in bkg_MC['Higgs']:
        Process(cmd, nt)
# End of function ApplyMuonSF


def ApplyBtagSF(indir_name, facdir_name, ch, syst, systdir):
    print '---Calculating b-tagging SF:', indir_name
    print '---Systematic:', syst+systdir
    exe = join(Path.dir_bin, 'SFcalc_btag')
    fcsv = join(Path.dir_2017, 'SF', 'DeepCSV_94XSF_V5_B_F.csv')
    indir = join(Path.dir_2017, 'Systematics', indir_name, syst+systdir)
    facdir = join(Path.dir_2017, facdir_name)
    if systdir!='Up' and systdir!='Down':
        print '[ERROR] Systematic variation has to be "Up" or "Down"!'
        sys.exit(1)
    if not (syst in SystUnc.Syst_tree_1 or syst in SystUnc.Syst_tree_2):
        print '[ERROR] Invalid systematic name!'
        sys.exit(1)
    if ch!='had' and ch!='lep':
        print '[ERROR] Invalid channel!'
        sys.exit(1)
    if not exists(exe):
        print '[ERROR] Executable does not exist!'
        sys.exit(1)
    if not exists(fcsv):
        print '[ERROR] CSV does not exist!'
        sys.exit(1)
    if not (exists(indir) and exists(facdir)):
        print '[ERROR] Input directory does not exist!'
        sys.exit(1)

    # Command template
    cmd = '{exe} {indir}/{{nt}}.root {facdir}/{{nt}}.root {fcsv} nosyst'.format(
            exe=exe, indir=indir, facdir=facdir, fcsv=fcsv)

    # Process signal MC
    for sigtype in sig_MC:
        if sigtype[1] != ch:
            continue
        for nt in sig_MC[sigtype]:
            Process(cmd, nt)
    # Process resonant bkg
    for nt in bkg_MC['Higgs']:
        Process(cmd, nt)
# End of function ApplyBtagSF


def MakeWeight(indir_name, ch, syst, systdir):
    print '---Calculating event weight:', indir_name
    print '---Systematic:', syst+systdir
    exe = join(Path.dir_bin, 'MakeWeight')
    indir = join(Path.dir_2017, 'Systematics', indir_name, syst+systdir)
    if systdir!='Up' and systdir!='Down':
        print '[ERROR] Systematic variation has to be "Up" or "Down"!'
        sys.exit(1)
    if not (syst in SystUnc.Syst_tree_1 or syst in SystUnc.Syst_tree_2):
        print '[ERROR] Invalid systematic name!'
        sys.exit(1)
    if ch!='had' and ch!='lep':
        print '[ERROR] Invalid channel!'
        sys.exit(1)
    if not exists(exe):
        print '[ERROR] Executable does not exist!'
        sys.exit(1)
    if not exists(indir):
        print '[ERROR] Input directory does not exist!'
        sys.exit(1)

    # Commamd template
    cmd = '{exe} {indir}/{{nt}}.root nosyst'.format(exe=exe, indir=indir)

    # Process signal MC
    for sigtype in sig_MC:
        if sigtype[1] != ch:
            continue
        for nt in sig_MC[sigtype]:
            Process(cmd, nt)
    # Process resonant bkg
    for nt in bkg_MC['Higgs']:
        Process(cmd, nt)
# End of function MakeWeight


# Apply all SFs for given systematics
def ProcSystSF(ch, syst, systdir):
    print '---Process systematic:', syst+systdir
    if ch == 'had':
        indir_name = 'Presel_had_phID_btag-L'
        facdir_name = 'SFbtag_extrapolation_had'
    elif ch == 'lep':
        indir_name = 'Presel_lep_phID'
        facdir_name = 'SFbtag_extrapolation_lep'
    else:
        print '[ERROR] Invalid channel'
        sys.exit(1)
    indir = join(Path.dir_2017, 'Systematics', indir_name, syst+systdir)
    facdir = join(Path.dir_2017, facdir_name)
    if systdir!='Up' and systdir!='Down':
        print '[ERROR] Systematic variation has to be "Up" or "Down"!'
        sys.exit(1)
    if not (syst in SystUnc.Syst_tree_1 or syst in SystUnc.Syst_tree_2):
        print '[ERROR] Invalid systematic name!'
        sys.exit(1)
    if not (exists(indir) and exists(facdir)):
        print '[ERROR] Input directory does not exist!'
        sys.exit(1)

    ApplyPileupSF(indir_name, ch, syst, systdir)
    ApplyElecSF(indir_name, ch, syst, systdir)
    ApplyMuonSF(indir_name, ch, syst, systdir)
    ApplyBtagSF(indir_name, facdir_name, ch, syst, systdir)
    MakeWeight(indir_name, ch, syst, systdir)
# End of function ProcSystSF


if __name__ == '__main__':
    ## hadronic channel
    #ProcSystSF('had', 'MvaShift', 'Up')
    #ProcSystSF('had', 'MvaShift', 'Down')
    #ProcSystSF('had', 'SigmaEOverEShift', 'Up')
    #ProcSystSF('had', 'SigmaEOverEShift', 'Down')
    #ProcSystSF('had', 'MaterialCentralBarrel', 'Up')
    #ProcSystSF('had', 'MaterialCentralBarrel', 'Down')
    #ProcSystSF('had', 'MaterialOuterBarrel', 'Up')
    #ProcSystSF('had', 'MaterialOuterBarrel', 'Down')
    #ProcSystSF('had', 'MaterialForward', 'Up')
    #ProcSystSF('had', 'MaterialForward', 'Down')
    #ProcSystSF('had', 'FNUFEB', 'Up')
    #ProcSystSF('had', 'FNUFEB', 'Down')
    #ProcSystSF('had', 'FNUFEE', 'Up')
    #ProcSystSF('had', 'FNUFEE', 'Down')
    #ProcSystSF('had', 'MCScaleGain6EB', 'Up')
    #ProcSystSF('had', 'MCScaleGain6EB', 'Down')
    #ProcSystSF('had', 'MCScaleGain1EB', 'Up')
    #ProcSystSF('had', 'MCScaleGain1EB', 'Down')
    #ProcSystSF('had', 'ShowerShapeHighR9EB', 'Up')
    #ProcSystSF('had', 'ShowerShapeHighR9EB', 'Down')
    #ProcSystSF('had', 'MCScaleHighR9EB', 'Up')
    #ProcSystSF('had', 'MCScaleHighR9EB', 'Down')
    #ProcSystSF('had', 'MCSmearHighR9EBRho', 'Up')
    #ProcSystSF('had', 'MCSmearHighR9EBRho', 'Down')
    #ProcSystSF('had', 'MCSmearHighR9EBPhi', 'Up')
    #ProcSystSF('had', 'MCSmearHighR9EBPhi', 'Down')
    #ProcSystSF('had', 'ShowerShapeHighR9EE', 'Up')
    #ProcSystSF('had', 'ShowerShapeHighR9EE', 'Down')
    #ProcSystSF('had', 'MCScaleHighR9EE', 'Up')
    #ProcSystSF('had', 'MCScaleHighR9EE', 'Down')
    #ProcSystSF('had', 'MCSmearHighR9EERho', 'Up')
    #ProcSystSF('had', 'MCSmearHighR9EERho', 'Down')
    #ProcSystSF('had', 'MCSmearHighR9EEPhi', 'Up')
    #ProcSystSF('had', 'MCSmearHighR9EEPhi', 'Down')
    #ProcSystSF('had', 'ShowerShapeLowR9EB', 'Up')
    #ProcSystSF('had', 'ShowerShapeLowR9EB', 'Down')
    #ProcSystSF('had', 'MCScaleLowR9EB', 'Up')
    #ProcSystSF('had', 'MCScaleLowR9EB', 'Down')
    #ProcSystSF('had', 'MCSmearLowR9EBRho', 'Up')
    #ProcSystSF('had', 'MCSmearLowR9EBRho', 'Down')
    #ProcSystSF('had', 'MCSmearLowR9EBPhi', 'Up')
    #ProcSystSF('had', 'MCSmearLowR9EBPhi', 'Down')
    #ProcSystSF('had', 'ShowerShapeLowR9EE', 'Up')
    #ProcSystSF('had', 'ShowerShapeLowR9EE', 'Down')
    #ProcSystSF('had', 'MCScaleLowR9EE', 'Up')
    #ProcSystSF('had', 'MCScaleLowR9EE', 'Down')
    #ProcSystSF('had', 'MCSmearLowR9EERho', 'Up')
    #ProcSystSF('had', 'MCSmearLowR9EERho', 'Down')
    #ProcSystSF('had', 'MCSmearLowR9EEPhi', 'Up')
    #ProcSystSF('had', 'MCSmearLowR9EEPhi', 'Down')
    #ProcSystSF('had', 'JEC', 'Up')
    #ProcSystSF('had', 'JEC', 'Down')
    #ProcSystSF('had', 'JER', 'Up')
    #ProcSystSF('had', 'JER', 'Down')
    #ProcSystSF('had', 'METJetEn', 'Up')
    #ProcSystSF('had', 'METJetEn', 'Down')
    #ProcSystSF('had', 'METJetRes', 'Up')
    #ProcSystSF('had', 'METJetRes', 'Down')
    #ProcSystSF('had', 'METUncEn', 'Up')
    #ProcSystSF('had', 'METUncEn', 'Down')
    #ProcSystSF('had', 'METPhoEn', 'Up')
    #ProcSystSF('had', 'METPhoEn', 'Down')

    # leptonic channel
    #ProcSystSF('lep', 'MvaShift', 'Up')
    #ProcSystSF('lep', 'MvaShift', 'Down')
    #ProcSystSF('lep', 'SigmaEOverEShift', 'Up')
    #ProcSystSF('lep', 'SigmaEOverEShift', 'Down')
    #ProcSystSF('lep', 'MaterialCentralBarrel', 'Up')
    #ProcSystSF('lep', 'MaterialCentralBarrel', 'Down')
    #ProcSystSF('lep', 'MaterialOuterBarrel', 'Up')
    #ProcSystSF('lep', 'MaterialOuterBarrel', 'Down')
    #ProcSystSF('lep', 'MaterialForward', 'Up')
    #ProcSystSF('lep', 'MaterialForward', 'Down')
    #ProcSystSF('lep', 'FNUFEB', 'Up')
    #ProcSystSF('lep', 'FNUFEB', 'Down')
    #ProcSystSF('lep', 'FNUFEE', 'Up')
    #ProcSystSF('lep', 'FNUFEE', 'Down')
    #ProcSystSF('lep', 'MCScaleGain6EB', 'Up')
    #ProcSystSF('lep', 'MCScaleGain6EB', 'Down')
    #ProcSystSF('lep', 'MCScaleGain1EB', 'Up')
    #ProcSystSF('lep', 'MCScaleGain1EB', 'Down')
    #ProcSystSF('lep', 'ShowerShapeHighR9EB', 'Up')
    #ProcSystSF('lep', 'ShowerShapeHighR9EB', 'Down')
    #ProcSystSF('lep', 'MCScaleHighR9EB', 'Up')
    #ProcSystSF('lep', 'MCScaleHighR9EB', 'Down')
    #ProcSystSF('lep', 'MCSmearHighR9EBRho', 'Up')
    #ProcSystSF('lep', 'MCSmearHighR9EBRho', 'Down')
    #ProcSystSF('lep', 'MCSmearHighR9EBPhi', 'Up')
    #ProcSystSF('lep', 'MCSmearHighR9EBPhi', 'Down')
    #ProcSystSF('lep', 'ShowerShapeHighR9EE', 'Up')
    #ProcSystSF('lep', 'ShowerShapeHighR9EE', 'Down')
    #ProcSystSF('lep', 'MCScaleHighR9EE', 'Up')
    #ProcSystSF('lep', 'MCScaleHighR9EE', 'Down')
    #ProcSystSF('lep', 'MCSmearHighR9EERho', 'Up')
    #ProcSystSF('lep', 'MCSmearHighR9EERho', 'Down')
    #ProcSystSF('lep', 'MCSmearHighR9EEPhi', 'Up')
    #ProcSystSF('lep', 'MCSmearHighR9EEPhi', 'Down')
    #ProcSystSF('lep', 'ShowerShapeLowR9EB', 'Up')
    #ProcSystSF('lep', 'ShowerShapeLowR9EB', 'Down')
    #ProcSystSF('lep', 'MCScaleLowR9EB', 'Up')
    #ProcSystSF('lep', 'MCScaleLowR9EB', 'Down')
    #ProcSystSF('lep', 'MCSmearLowR9EBRho', 'Up')
    #ProcSystSF('lep', 'MCSmearLowR9EBRho', 'Down')
    #ProcSystSF('lep', 'MCSmearLowR9EBPhi', 'Up')
    #ProcSystSF('lep', 'MCSmearLowR9EBPhi', 'Down')
    #ProcSystSF('lep', 'ShowerShapeLowR9EE', 'Up')
    #ProcSystSF('lep', 'ShowerShapeLowR9EE', 'Down')
    #ProcSystSF('lep', 'MCScaleLowR9EE', 'Up')
    #ProcSystSF('lep', 'MCScaleLowR9EE', 'Down')
    #ProcSystSF('lep', 'MCSmearLowR9EERho', 'Up')
    #ProcSystSF('lep', 'MCSmearLowR9EERho', 'Down')
    #ProcSystSF('lep', 'MCSmearLowR9EEPhi', 'Up')
    #ProcSystSF('lep', 'MCSmearLowR9EEPhi', 'Down')
    ProcSystSF('lep', 'JEC', 'Up')
    ProcSystSF('lep', 'JEC', 'Down')
    ProcSystSF('lep', 'JER', 'Up')
    ProcSystSF('lep', 'JER', 'Down')
    ProcSystSF('lep', 'METJetEn', 'Up')
    ProcSystSF('lep', 'METJetEn', 'Down')
    ProcSystSF('lep', 'METJetRes', 'Up')
    ProcSystSF('lep', 'METJetRes', 'Down')
    ProcSystSF('lep', 'METUncEn', 'Up')
    ProcSystSF('lep', 'METUncEn', 'Down')
    ProcSystSF('lep', 'METPhoEn', 'Up')
    ProcSystSF('lep', 'METPhoEn', 'Down')
