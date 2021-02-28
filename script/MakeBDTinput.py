#!/usr/bin/env python2

import Path, Util
import os, sys
from os.path import join, exists
from Samples import sig_MC, bkg_MC


def Process(cmd, nt, scale):
    print '---Processing:', nt
    fp = os.popen( cmd.format(nt=nt, scale=scale) )
    print fp.read()
    fp.close()


def MakeBDTinput(evtdir_name, recodir_name, outdir_name, ch):
    # Note:
    # For data & data-driven_QCD, add normfactor histograms with value 1
    if ch == 'had':
        exe = join(Path.dir_bin, 'MakeBDTinput_had')
    elif ch == 'lep':
        exe = join(Path.dir_bin, 'MakeBDTinput_lep')
    else:
        print '[ERROR] Invalid channel'
        sys.exit(1)
    evtdir = join(Path.dir_2017, evtdir_name)
    recodir = join(Path.dir_2017, recodir_name)
    outdir = join(Path.dir_2017, outdir_name)
    normdir = join(Path.dir_2017, 'normfactor')

    if not exists(exe):
        print '[ERROR] Executable not exist'
        sys.exit(1)
    if not exists(evtdir) or not exists(recodir):
        print '[ERROR] Input dir not exist'
        sys.exit(1)
    if not exists(outdir):
        Util.CreateDir(outdir)

    # Command template
    cmd_syst = '{exe} {evtdir}/{{nt}}.root {recodir}/{{nt}}.root {outdir}/{{nt}}.root {normdir}/{{nt}}.root {syst} {{scale}}'.format(
            exe=exe, evtdir=evtdir, recodir=recodir, outdir=outdir, normdir=normdir, syst='syst')
    cmd_nosyst = '{exe} {evtdir}/{{nt}}.root {recodir}/{{nt}}.root {outdir}/{{nt}}.root {normdir}/{{nt}}.root {syst} {{scale}}'.format(
            exe=exe, evtdir=evtdir, recodir=recodir, outdir=outdir, normdir=normdir, syst='nosyst')

    # Process signal MC
    for sigtype in sig_MC:
        if sigtype[1] != ch:
            continue
        for nt in sig_MC[sigtype]:
            Process(cmd_syst, nt, 3)
    # Process resonant bkg
    for nt in bkg_MC['Higgs']:
        Process(cmd_syst, nt, 3)
    # Process non-res bkg
    for cat in bkg_MC:
        if cat == 'Higgs':
            continue
        for nt in bkg_MC[cat]:
            if ch=='lep' and nt=='QCD_Pt-30to40_MGG-80toInf':
                continue
            Process(cmd_nosyst, nt, 2)
    # Process data-driven QCD
    if ch == 'had':
        # Data-driven QCD
        Process(cmd_nosyst, 'Data-driven_QCD', 2)
        # Scaled DiPhotonJetsBox
        cmd_sc = '''{exe} {evtdir}/DiPhotonJetsBox_scaled.root {recodir}/DiPhotonJetsBox.root \
                {outdir}/DiPhotonJetsBox_scaled.root {normdir}/DiPhotonJetsBox.root nosyst 2'''.format(
                exe=exe, evtdir=evtdir, recodir=recodir, outdir=outdir, normdir=normdir)
        print '---Processing: DiPhotonJetsBox_scaled'
        fp = os.popen(cmd_sc)
        print fp.read()
        fp.close()
    # Process data
    Process(cmd_nosyst, 'data', 1)
# End of function MakeBDTinput


def Proc(cmd, nt):
    print '---Processing:', nt
    fp = os.popen( cmd.format(nt=nt) )
    print fp.read()
    fp.close()


def AddBdtVar(evtdir_name, bdtdir_name, ch):
    exe = join(Path.dir_bin, 'AddBdtVar')
    evtdir = join(Path.dir_2017, evtdir_name)
    bdtdir = join(Path.dir_2017, bdtdir_name)
    if ch!='had' and ch!='lep':
        print '[ERROR] Invalid channel'
        sys.exit(1)
    if not exists(exe) or not exists(evtdir) or not exists(bdtdir):
        print '[ERROR] Required files not exist'
        sys.exit(1)

    # Command template
    cmd = '{exe} {evtdir}/{{nt}}.root {bdtdir}/{{nt}}.root'.format(
            exe=exe, evtdir=evtdir, bdtdir=bdtdir)

    # Process signal MC
    for sigtype in sig_MC:
        if sigtype[1] != ch:
            continue
        for nt in sig_MC[sigtype]:
            Proc(cmd, nt)
    # Process bkg MC
    for cat in bkg_MC:
        # For had channel, skip MCs replaced by data-driven QCD
        if ch=='had' and (cat in ['GGJets','GJet','QCD']):
            continue
        for nt in bkg_MC[cat]:
            # Skip empty sample
            if ch=='lep' and nt=='QCD_Pt-30to40_MGG-80toInf':
                continue
            Proc(cmd, nt)
    # Process data-driven
    if ch=='had':
        Proc(cmd, 'Data-driven_QCD')
        Proc(cmd, 'DiPhotonJetsBox_scaled')
    # Process data
    Proc(cmd, 'data')
# End of function AddBdtVar


if __name__ == '__main__':
    #MakeBDTinput('Presel_had_phID_btag-L', 'MVAreco_result_had', 'BDT_input_had', 'had')
    #MakeBDTinput('Presel_lep_phID', 'MVAreco_result_lep', 'BDT_input_lep', 'lep')
    #AddBdtVar('Presel_had_phID_btag-L', 'BDT_input_had', 'had')
    AddBdtVar('Presel_lep_phID', 'BDT_input_lep', 'lep')
