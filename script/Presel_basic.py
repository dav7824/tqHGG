#!/usr/bin/env python2

import Path, Util
import os, sys
from Samples import sig_MC, bkg_MC
from os.path import join


exe = join(Path.dir_bin, 'Presel_basic_v2')
indir = Path.dir_nt_2017
outdir = join(Path.dir_2017, 'Presel_basic')
treename = 'flashggNtuples/flashggStdTree'

cmd = 'set -f; %s %s/{fin} %s %s/{fout}.root' % (exe, indir, treename, outdir)


def Process(file, sample):
    print '[INFO] Processing:', sample
    fp = os.popen( cmd.format(fin=file, fout=sample) )
    print fp.read()
    fp.close()


def RunSignal():
    for sigtype in sig_MC:
        for nt in sig_MC[sigtype]:
            Process( nt[1], nt[0] )


def RunBkg():
    for cat in bkg_MC:
        for nt in bkg_MC[cat]:
            Process( nt[1], nt[0] )


if __name__ == '__main__':
    Util.CreateDir(outdir)
    RunSignal()
    RunBkg()
    Process( 'DoubleEG.root', 'data' )
    print 'Complete!'
