#!/usr/bin/env python2

import Path, Util, SystUnc
from Samples import sig_MC, bkg_MC
import os, sys
from os.path import exists, join


def Process(cmd, nt):
    fp = os.popen( cmd.format(nt=nt) )
    fp.close()


# Add pileup SF to samples of given channel, systematic type, and variation (Up or Down)
def ApplyPileupSF(indir_name, ch, syst, systdir):
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


# Add electron SF to samples of given channel, systematic type, and variation (Up or Down)
def ApplyElecSF(indir_name, ch, syst, systdir):
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


# Add muon SF to samples of given channel, systematic type, and variation (Up or Down)
def ApplyMuonSF(indir_name, ch, syst, systdir):
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


# Add b-tag SF to samples of given channel, systematic type, and variation (Up or Down)
def ApplyBtagSF(indir_name, facdir_name, ch, syst, systdir):
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


# Add event weight to samples of given channel, systematic type, and variation (Up or Down)
def MakeWeight(indir_name, ch, syst, systdir):
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


# Add all SFs and weights to samples of given channel, systematic type, and variation (Up or Down)
def ProcSystSF(ch, syst, systdir):
    #print '---Processing: {}{}  ({})'.format(syst, systdir, ch)
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
