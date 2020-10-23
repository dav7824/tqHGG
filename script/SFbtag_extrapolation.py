#!/usr/bin/env python2

import Path, Util, Samples
import os, sys
from os.path import join, exists
from Samples import sig_MC_s, bkg_MC_s


def Process(cmd, nt):
    print 'Processing:', nt
    fp = os.popen( cmd.format(nt=nt) )
    print fp.read()
    fp.close()


def CalculateExtrapolation(indir_name, outdir_name, ch):
    print 'Start calculating extrapolation factor'
    exe = join(Path.dir_bin, 'SFbtag_extrapolation')
    fcsv = join(Path.dir_2017, 'SF', 'DeepCSV_94XSF_V5_B_F.csv')
    indir = join(Path.dir_2017, indir_name)
    outdir = join(Path.dir_2017, outdir_name)

    if not exists(exe):
        print '[ERROR] Executable does not exist!'
        sys.exit(1)
    if not exists(fcsv):
        print '[ERROR] b-tagging CSV does not exist!'
        sys.exit(1)
    if not exists(indir):
        print '[ERROR] Input directory does not exist!'
        sys.exit(1)
    Util.CreateDir(outdir)

    # Command template
    cmd = '{bin} {indir}/{{nt}}.root {csv} {outdir}/{{nt}}.root'.format(
            bin=exe, indir=indir, csv=fcsv, outdir=outdir)

    # Process signal MC
    for sigtype in sig_MC_s:
        if sigtype[1] != ch:
            continue
        for nt in sig_MC_s[sigtype]:
            Process(cmd, nt)
    # Process bkg MC
    for cat in bkg_MC_s:
        for nt in bkg_MC_s[cat]:
            Process(cmd, nt)

    print 'Complete calculating extrapolation factor'
# End of function CalculateExtrapolation


def AddExtrapolation(indir_name, exdir_name, ch):
    print 'Start adding extrapolation factor'
    exe = join(Path.dir_bin, 'SFbtag_extrapolation_add')
    indir = join(Path.dir_2017, indir_name)
    exdir = join(Path.dir_2017, exdir_name)

    if not exists(exe):
        print '[ERROR] Executable does not exist!'
        sys.exit(1)
    if not exists(indir):
        print '[ERROR] Input directory does not exist!'
        sys.exit(1)
    if not exists(exdir):
        print '[ERROR] Ex_factor directory does not exist!'
        sys.exit(1)

    # Command template
    cmd = '{bin} {indir}/{{nt}}.root {exdir}/{{nt}}.root'.format(
            bin=exe, indir=indir, exdir=exdir)

    # Process signal MC
    for sigtype in sig_MC_s:
        if sigtype[1] != ch:
            continue
        for nt in sig_MC_s[sigtype]:
            Process(cmd, nt)
    # Process bkg MC
    for cat in bkg_MC_s:
        for nt in bkg_MC_s[cat]:
            Process(cmd, nt)

    print 'Complete adding extrapolation factor'
# End of function AddExtrapolation


if __name__ == '__main__':
    # hadronic
    #indir = 'Presel_had_phID'
    #indir2 = 'Presel_had_phID_btag-L'
    #outdir = 'Presel_had_phID__btag-extrapolation'
    #ch = 'had'

    # leptonic
    indir = 'Presel_lep_phID'
    indir2 = 'Presel_lep_phID'
    outdir = 'Presel_lep_phID__btag-extrapolation'
    ch = 'lep'

    # Run workflow
    #CalculateExtrapolation(indir, outdir, ch)
    AddExtrapolation(indir2, outdir, ch)
