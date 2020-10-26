#!/usr/bin/env python2

import Path, Util, Samples
import os, sys
from os.path import join, exists
from Samples import sig_MC_s, bkg_MC_s


def Process(cmd, nt):
    print 'Processing:', nt
    fp = os.popen(cmd.format(nt=nt))
    print fp.read()
    fp.close()


def RunSamples(exe_name, indir_name, outdir_name, recodir_name, ch):
    exe = join(Path.dir_bin, exe_name)
    indir = join(Path.dir_2017, indir_name)
    outdir = join(Path.dir_2017, outdir_name)
    recodir = join(Path.dir_2017, recodir_name)

    if not exists(exe):
        print '[ERROR] Executable does not exist!'
        sys.exit(1)
    if not exists(indir):
        print '[ERROR] Input dir does not exist!'
        sys.exit(1)
    if not exists(recodir):
        print '[ERROR] Reco dir does not exist!'
        sys.exit(1)
    if ch != 'had' and ch != 'lep':
        print '[ERROR] Invalid channel!'
        sys.exit(1)
    Util.CreateDir(outdir)

    # Command template
    cmd = '{bin} {indir}/{{nt}}.root {outdir}/{{nt}}.root {recodir}/{{nt}}.root'.format(
            bin=exe, indir=indir, outdir=outdir, recodir=recodir)

    # Process signal MC
    for sigtype in sig_MC_s:
        if sigtype[1] != ch:
            continue
        for nt in sig_MC_s[sigtype]:
            Process(cmd, nt)

    print 'Complete!'
# End of function RunSamples


if __name__ == '__main__':
    # hadronic
    #exe_name = 'DivideTreeV2_had'
    #indir_name = 'Presel_had_phID_btag-L'
    #outdir_name = 'Presel_had_phID_btag-L__MVAreco-test'
    #recodir_name = 'MVArecoV2_Perm_had'
    #ch = 'had'

    # leptonic
    exe_name = 'DivideTreeV2_lep'
    indir_name = 'Presel_lep_phID'
    outdir_name = 'Presel_lep_phID__MVAreco-test'
    recodir_name = 'MVArecoV3_Perm_lep'
    ch = 'lep'

    RunSamples(exe_name, indir_name, outdir_name, recodir_name, ch)
