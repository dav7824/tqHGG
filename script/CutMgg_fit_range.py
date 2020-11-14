#!/usr/bin/env python2

import Path, Util, Samples
import os, sys
from os.path import join, exists
from Samples import sig_MC_s, bkg_MC_s


def Process(cmd, nt, selection):
    print 'Processing: cut="{}", sample={}'.format(selection, nt)
    fp = os.popen(cmd.format(nt=nt,selection=selection))
    print fp.read()
    fp.close()


def CutMgg(indir_name, outdir_name, ch):
    exe = join(Path.dir_bin, 'Presel_copytree')
    indir = join(Path.dir_2017, indir_name)
    outdir = join(Path.dir_2017, outdir_name)
    if not exists(exe) or not exists(indir):
        print '[ERROR] Required files not exist!'
        sys.exit(1)
    if ch!='had' and ch!='lep':
        print '[ERROR] Invalid channel!'
        sys.exit(1)
    if not exists(outdir):
        Util.CreateDir(outdir)

    # Selection cut
    cut_range = '100<=Mgg && Mgg<=180'
    cut_blind = '(100<=Mgg && Mgg<=120) || (130<=Mgg && Mgg<=180)'
    # Command template
    cmd = '{exe} {indir}/{{nt}}.root {tree} {outdir}/{{nt}}.root "{{selection}}"'.format(
            exe=exe, indir=indir, tree='T', outdir=outdir)
    # Run signal MC
    for sigtype in sig_MC_s:
        if sigtype[1] != ch:
            continue
        for nt in sig_MC_s[sigtype]:
            Process(cmd, nt, cut_range)
    # Run resonant bkg MC
    for nt in bkg_MC_s['Higgs']:
        Process(cmd, nt, cut_range)
    # Run data
    Process(cmd, 'data', cut_blind)

    print 'Complete'
# End of function CutMgg


if __name__ == '__main__':
    # hadronic optimization
    #indir = 'optimization_had/output_BDT_v1'
    #outdir = 'optimization_had/fit_input_BDT_v1'
    #ch = 'had'

    # leptonic optimization
    indir = 'optimization_lep/output_BDT_v1'
    outdir = 'optimization_lep/fit_input_BDT_v1'
    ch = 'lep'

    CutMgg(indir, outdir, ch)
