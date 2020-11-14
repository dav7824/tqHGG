#!/usr/bin/env python2

import Path, Util, Samples
import os, sys
from os.path import exists, join
from Samples import sig_MC_s, bkg_MC_s


def Process(cmd, nt):
    print 'Processing:', nt
    fp = os.popen(cmd.format(nt=nt))
    print fp.read()
    fp.close()


def RunSamples(indir_name, ch):
    exe = join(Path.dir_bin, 'MakeWeight')
    indir = join(Path.dir_2017, indir_name)
    if not exists(exe) or not exists(indir):
        print '[ERROR] Required files not exist!'
        sys.exit(1)
    if ch!='had' and ch!='lep':
        print '[ERROR] Invalid channel!'
        sys.exit(1)

    # Command template
    cmd = '{exe} {indir}/{{nt}}.root'.format(exe=exe, indir=indir)

    # Run signal MC
    for sigtype in sig_MC_s:
        if sigtype[1] != ch:
            continue
        for nt in sig_MC_s[sigtype]:
            Process(cmd, nt)
    # Run resonant bkg MC
    for nt in bkg_MC_s['Higgs']:
        Process(cmd, nt)
    # Run data
    Process(cmd, 'data')

    print 'Complete!'
# End of function RunSamples


if __name__ == '__main__':
    # hadronic optimization
    indir = 'optimization_had/output_BDT_v1'
    ch = 'had'
    RunSamples(indir, ch)

    # leptonic optimization
    indir = 'optimization_lep/output_BDT_v1'
    ch = 'lep'
    RunSamples(indir, ch)
