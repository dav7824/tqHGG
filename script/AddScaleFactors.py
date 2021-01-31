#!/usr/bin/env python2

import Path, Util
from Samples import sig_MC, bkg_MC
import os, sys
from os.path import join, exists


def Process(cmd, nt, printout=False):
    print 'Processing:', nt
    fp = os.popen( cmd.format(nt=nt) )
    if printout:
        print fp.read()
    fp.close()


def ApplyPileupSF(indir_name, ch):
    print '---Calculating pileup SF:', indir_name
    exe = join(Path.dir_bin, 'SFcalc_PU')
    fhist = join(Path.dir_2017, 'SF', 'MCPileUp.root')
    indir = join(Path.dir_2017, indir_name)
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
    # Process bkg MC
    for cat in bkg_MC:
        for nt in bkg_MC[cat]:
            if nt == 'QCD_Pt-30to40_MGG-80toInf' and ch == 'lep':
                continue  # No event left in the sample
            Process(cmd, nt)
# End of function ApplyPileupSF


def ApplyElecSF(indir_name, ch):
    print '---Calculating electron SF:', indir_name
    exe = join(Path.dir_bin, 'SFcalc_Electron')
    freco0 = join(Path.dir_2017, 'SF', 'egammaEffi.txt_EGM2D_runBCDEF_passingRECO_lowEt.root')
    freco1 = join(Path.dir_2017, 'SF', 'egammaEffi.txt_EGM2D_runBCDEF_passingRECO.root')
    fid = join(Path.dir_2017, 'SF', '2017_ElectronMedium.root')
    indir = join(Path.dir_2017, indir_name)
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
    cmd_syst = '{exe} {indir}/{{nt}}.root {ch} {freco0} {freco1} {fid} syst'.format(
            exe=exe, indir=indir, ch=ch, freco0=freco0, freco1=freco1, fid=fid)
    cmd = '{exe} {indir}/{{nt}}.root {ch} {freco0} {freco1} {fid} nosyst'.format(
            exe=exe, indir=indir, ch=ch, freco0=freco0, freco1=freco1, fid=fid)

    # Process signal MC
    for sigtype in sig_MC:
        if sigtype[1] != ch:
            continue
        for nt in sig_MC[sigtype]:
            Process(cmd_syst, nt)
    # Process resonant bkg MC
    for nt in bkg_MC['Higgs']:
        Process(cmd_syst, nt)
    # Process non-resonant bkg
    for cat in bkg_MC:
        if cat == 'Higgs':
            continue
        for nt in bkg_MC[cat]:
            if nt == 'QCD_Pt-30to40_MGG-80toInf' and ch == 'lep':
                continue  # No event left in the sample
            Process(cmd, nt)
# End of function ApplyElecSF


def ApplyMuonSF(indir_name, ch):
    print '---Calculating muon SF:', indir_name
    exe = join(Path.dir_bin, 'SFcalc_Muon')
    fid = join(Path.dir_2017, 'SF', 'RunBCDEF_SF_ID.root')
    fiso = join(Path.dir_2017, 'SF', 'RunBCDEF_SF_ISO.root')
    indir = join(Path.dir_2017, indir_name)
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
    cmd_syst = '{exe} {indir}/{{nt}}.root {ch} {fid} {fiso} syst'.format(
            exe=exe, indir=indir, ch=ch, fid=fid, fiso=fiso)
    cmd = '{exe} {indir}/{{nt}}.root {ch} {fid} {fiso} nosyst'.format(
            exe=exe, indir=indir, ch=ch, fid=fid, fiso=fiso)

    # Process signal MC
    for sigtype in sig_MC:
        if sigtype[1] != ch:
            continue
        for nt in sig_MC[sigtype]:
            Process(cmd_syst, nt)
    # Process resonant bkg MC
    for nt in bkg_MC['Higgs']:
        Process(cmd_syst, nt)
    # Process non-resonant bkg
    for cat in bkg_MC:
        if cat == 'Higgs':
            continue
        for nt in bkg_MC[cat]:
            if nt == 'QCD_Pt-30to40_MGG-80toInf' and ch == 'lep':
                continue  # No event left in the sample
            Process(cmd, nt)
# End of function ApplyMuonSF


def GetBtagFactor(indir_name, outdir_name, ch):
    print '---Calculating b-tagging extrapolation:', indir_name
    exe = join(Path.dir_bin, 'SFcalc_btag_factor')
    fcsv = join(Path.dir_2017, 'SF', 'DeepCSV_94XSF_V5_B_F.csv')
    indir = join(Path.dir_2017, indir_name)
    outdir = join(Path.dir_2017, outdir_name)
    if ch!='had' and ch!='lep':
        print '[ERROR] Invalid channel!'
        sys.exit(1)
    if not exists(exe):
        print '[ERROR] Executable does not exist!'
        sys.exit(1)
    if not exists(fcsv):
        print '[ERROR] CSV does not exist!'
        sys.exit(1)
    if not exists(indir):
        print '[ERROR] Input directory does not exist!'
        sys.exit(1)
    Util.CreateDirs(outdir)

    # Command template
    cmd = '{exe} {indir}/{{nt}}.root {outdir}/{{nt}}.root {fcsv}'.format(
            exe=exe, indir=indir, outdir=outdir, fcsv=fcsv)

    # Process signal MC
    for sigtype in sig_MC:
        if sigtype[1] != ch:
            continue
        for nt in sig_MC[sigtype]:
            Process(cmd, nt, True)
    # Process bkg MC
    for cat in bkg_MC:
        for nt in bkg_MC[cat]:
            if nt == 'QCD_Pt-30to40_MGG-80toInf' and ch == 'lep':
                continue  # No event left in the sample
            Process(cmd, nt, True)
# End of function GetBtagFactor


def ApplyBtagSF(indir_name, facdir_name, ch):
    print '---Calculating b-tagging SF:', indir_name
    exe = join(Path.dir_bin, 'SFcalc_btag')
    fcsv = join(Path.dir_2017, 'SF', 'DeepCSV_94XSF_V5_B_F.csv')
    indir = join(Path.dir_2017, indir_name)
    facdir = join(Path.dir_2017, facdir_name)
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
    cmd_syst = '{exe} {indir}/{{nt}}.root {facdir}/{{nt}}.root {fcsv} syst'.format(
            exe=exe, indir=indir, facdir=facdir, fcsv=fcsv)
    cmd = '{exe} {indir}/{{nt}}.root {facdir}/{{nt}}.root {fcsv} nosyst'.format(
            exe=exe, indir=indir, facdir=facdir, fcsv=fcsv)

    # Process signal MC
    for sigtype in sig_MC:
        if sigtype[1] != ch:
            continue
        for nt in sig_MC[sigtype]:
            Process(cmd_syst, nt)
    # Process resonant bkg MC
    for nt in bkg_MC['Higgs']:
        Process(cmd_syst, nt)
    # Process non-resonant bkg
    for cat in bkg_MC:
        if cat == 'Higgs':
            continue
        for nt in bkg_MC[cat]:
            if nt == 'QCD_Pt-30to40_MGG-80toInf' and ch == 'lep':
                continue  # No event left in the sample
            Process(cmd, nt)
# End of function ApplyBtagSF


def MakeWeight(indir_name, ch):
    print '---Calculating event weight:', indir_name
    exe = join(Path.dir_bin, 'MakeWeight')
    indir = join(Path.dir_2017, indir_name)
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
    cmd_syst = '{exe} {indir}/{{nt}}.root syst'.format(exe=exe, indir=indir)
    cmd = '{exe} {indir}/{{nt}}.root nosyst'.format(exe=exe, indir=indir)

    # Process signal MC
    for sigtype in sig_MC:
        if sigtype[1] != ch:
            continue
        for nt in sig_MC[sigtype]:
            Process(cmd_syst, nt)
    # Process resonant bkg MC
    for nt in bkg_MC['Higgs']:
        Process(cmd_syst, nt)
    # Process non-resonant bkg
    for cat in bkg_MC:
        if cat == 'Higgs':
            continue
        for nt in bkg_MC[cat]:
            if nt == 'QCD_Pt-30to40_MGG-80toInf' and ch == 'lep':
                continue  # No event left in the sample
            Process(cmd, nt)
# End of function MakeWeight


def MakeWeight_data(indir_name):
    print 'Calculating event weight:', indir_name
    exe = join(Path.dir_bin, 'MakeWeight_data')
    indir = join(Path.dir_2017, indir_name)
    if not exists(exe):
        print '[ERROR] Executable does not exist!'
        sys.exit(1)
    if not exists(indir):
        print '[ERROR] Input directory does not exist!'
        sys.exit(1)

    # Command
    cmd = '{exe} {indir}/{{nt}}.root'.format(exe=exe, indir=indir)

    Process(cmd, 'data')
# End of function MakeWeight_data


if __name__ == '__main__':
    # hadronic channel (before b-tag cut)
    #ApplyPileupSF('Presel_had_phID', 'had')
    #ApplyElecSF('Presel_had_phID', 'had')
    #ApplyMuonSF('Presel_had_phID', 'had')
    #GetBtagFactor('Presel_had_phID', 'SFbtag_extrapolation_had', 'had')

    # hadronic channel
    #ApplyPileupSF('Presel_had_phID_btag-L', 'had')
    #ApplyElecSF('Presel_had_phID_btag-L', 'had')
    #ApplyMuonSF('Presel_had_phID_btag-L', 'had')
    #ApplyBtagSF('Presel_had_phID_btag-L', 'SFbtag_extrapolation_had', 'had')
    #MakeWeight('Presel_had_phID_btag-L', 'had')
    #MakeWeight_data('Presel_had_phID_btag-L')

    # leptonic channel
    # Note: No event left in sample "QCD_Pt-30to40_MGG-80toInf" for leptonic channel
    #ApplyPileupSF('Presel_lep_phID', 'lep')
    #ApplyElecSF('Presel_lep_phID', 'lep')
    #ApplyMuonSF('Presel_lep_phID', 'lep')
    #GetBtagFactor('Presel_lep_phID', 'SFbtag_extrapolation_lep', 'lep')
    #ApplyBtagSF('Presel_lep_phID', 'SFbtag_extrapolation_lep', 'lep')
    #MakeWeight('Presel_lep_phID', 'lep')
    MakeWeight_data('Presel_lep_phID')
