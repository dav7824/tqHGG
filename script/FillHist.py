#!/usr/bin/env python2
# Fill histograms for each sample.

import Path, Util
from Samples import sig_MC, bkg_MC
import sys, os
from os.path import join, exists


def Process(cmd, nt):
    print 'Processing:', nt
    fp = os.popen( cmd.format(nt=nt) )
    #print fp.read()
    fp.close()


def FillHist(exe_name, indir_name, outdir_name, ch):
    exe = join(Path.dir_bin, exe_name)
    indir = join(Path.dir_2017, indir_name)
    outdir = join(Path.dir_2017, outdir_name)
    if ch!='had' and ch!='lep':
        print '[ERROR] Invalid channel!'
        sys.exit(1)
    if not exists(exe):
        print '[ERROR] Executable doesn\'t exist!'
        sys.exit(1)
    if not exists(indir):
        print '[ERROR] Input dir doesn\'t exist!'
        sys.exit(1)
    Util.CreateDirs(outdir)

    # Command template
    cmd = '{exe} {indir}/{{nt}}.root {outdir}/{{nt}}.root'.format(
            exe=exe, indir=indir, outdir=outdir)

    # Process signal MC
    for sigtype in sig_MC:
        if sigtype[1]!=ch:
            continue
        for nt in sig_MC[sigtype]:
            Process(cmd, nt)
    # Process bkg MC
    for cat in bkg_MC:
        for nt in bkg_MC[cat]:
            if ch=='lep' and nt=='QCD_Pt-30to40_MGG-80toInf':
                continue
            Process(cmd, nt)
    # Process data
    Process(cmd, 'data')
# End of function FillHist


def Proc(cmd, nt, cut):
    print '---Processing:', nt
    fp = os.popen( cmd.format(nt=nt,cut=cut) )
    print fp.read()
    fp.close()


def FillHist_BDT(exe_name, indir_name, outdir_name, ch):
    exe = join(Path.dir_bin, exe_name)
    indir = join(Path.dir_2017, indir_name)
    outdir = join(Path.dir_2017, outdir_name)
    if not exists(exe) or not exists(indir):
        print '[ERROR] Required files not exist!'
        sys.exit(1)
    if ch!='had' and ch!='lep':
        print '[ERROR] Invalid channel'
        sys.exit(1)
    if not exists(outdir):
        Util.CreateDir(outdir)

    # Command template
    # Note: event weights are scaled by number of divided parts of the sample
    #       signal & res-bkg = 3, non-res bkg = 2, data = 1
    cmd = 'set -f; {exe} {indir}/{{nt}}.root {outdir}/{{nt}}.root "{weight}" "{{cut}}"'.format(
            exe=exe, indir=indir, outdir=outdir, weight='evtwei_central*evtwei_scale')

    # Process signal MC
    for sigtype in sig_MC:
        if sigtype[1] != ch:
            continue
        for nt in sig_MC[sigtype]:
            Proc(cmd, nt, 'evt_no%3==1')
    # Process resonant bkg
    for nt in bkg_MC['Higgs']:
        Proc(cmd, nt, 'evt_no%3==1')
    # Process bkg MC
    for cat in bkg_MC:
        if cat == 'Higgs':
            continue
        for nt in bkg_MC[cat]:
            if ch=='lep' and nt=='QCD_Pt-30to40_MGG-80toInf':
                continue
            Proc(cmd, nt, 'evt_no%2==1')
    # Process data-driven QCD
    if ch == 'had':
        # Data-driven QCD
        Proc(cmd, 'Data-driven_QCD', 'evt_no%2==1')
        # Scaled DiPhotonJetsBox
        Proc(cmd, 'DiPhotonJetsBox_scaled', 'evt_no%2==1')
    # Process data
    Proc(cmd ,'data', '1')
# End of function FillHist_BDT


if __name__ == '__main__':
    #FillHist('FillHist_had', 'Presel_had_phID_btag-L', 'Presel_had_phID_btag-L/hist', 'had')
    #FillHist('FillHist_lep', 'Presel_lep_phID', 'Presel_lep_phID/hist', 'lep')
    #FillHist_BDT('FillHist_BDT_had', 'BDT_input_had', 'BDT_input_had/hist', 'had')
    FillHist_BDT('FillHist_BDT_lep', 'BDT_input_lep', 'BDT_input_lep/hist', 'lep')
