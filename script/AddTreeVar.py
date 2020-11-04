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


def AddTreeVar(indir1_name, indir2_name, ch):
    exe = join(Path.dir_bin, 'AddTreeVar')
    indir1 = join(Path.dir_2017, indir1_name)
    indir2 = join(Path.dir_2017, indir2_name)

    if not exists(exe) or not exists(indir1) or not exists(indir2):
        print '[ERROR] Required files not exist!'
        sys.exit(1)
    if ch!='had' and ch!='lep':
        print '[ERROR] Invalid channel!'
        sys.exit(1)

    # Command template
    cmd = '{exe} {indir1}/{{nt}}.root {indir2}/{{nt}}.root'.format(
            exe=exe, indir1=indir1, indir2=indir2)

    # Process signal MC
    for sigtype in sig_MC_s:
        if sigtype[1] != ch:
            continue
        for nt in sig_MC_s[sigtype]:
            Process(cmd, nt)
    # Process bkg MC (resonant)
    for nt in bkg_MC_s['Higgs']:
        Process(cmd, nt)
    # Process data
    Process(cmd, 'data')

    print 'Complete!'
# End of function AddTreeVar


if __name__ == '__main__':
    # hadronic optimization
    #indir1 = 'optimization_had/plain'
    #indir2 = 'optimization_had/BDT'
    #ch = 'had'

    # leptonic optimization
    #indir1 = 'optimization_lep/plain'
    #indir2 = 'optimization_lep/BDT'
    #ch = 'lep'

    # hadronic modeling
    #indir1 = 'model_had/plain'
    #indir2 = 'model_had/BDT'
    #ch = 'had'

    # leptonic modeling
    indir1 = 'model_lep/plain'
    indir2 = 'model_lep/BDT'
    ch = 'lep'

    AddTreeVar(indir1, indir2, ch)
