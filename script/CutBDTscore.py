#!/usr/bin/env python2

import Path, Util
from Samples import sig_MC, bkg_MC
import sys, os
from os.path import join, exists


def Process(cmd, nt):
    print 'Processing:', nt
    fp = os.popen( cmd.format(nt=nt) )
    print fp.read()
    fp.close()


def CutBDTscore(indir_name, outdir_name, ch):
    exe = join(Path.dir_bin, 'CutBDTscore')
    indir = join(Path.dir_2017, indir_name)
    outdir = join(Path.dir_2017, outdir_name)
    if ch!='had' and ch!='lep':
        print '[ERROR] Invalid channel!'
        sys.exit(1)
    if not exists(exe) or not exists(indir):
        print '[ERROR] Required files not exist!'
        sys.exit(1)
    if not exists(outdir):
        Util.CreateDirs(outdir)

    # Command template
    cmd = '{exe} {indir}/{{nt}}.root {outdir}/{{nt}}.root {ch} {isdata}'.format(
            exe=exe, indir=indir, outdir=outdir, ch=ch, isdata='MC')
    cmd_data = '{exe} {indir}/{{nt}}.root {outdir}/{{nt}}.root {ch} {isdata}'.format(
            exe=exe, indir=indir, outdir=outdir, ch=ch, isdata='data')

    # Process signal MC
    for sigtype in sig_MC:
        if sigtype[1] != ch:
            continue
        for nt in sig_MC[sigtype]:
            Process(cmd , nt)
    # Process resonant bkg MC
    for nt in bkg_MC['Higgs']:
        Process(cmd, nt)
    # Process data
    Process(cmd_data, 'data')
# End of function CutBDTscore


if __name__ == '__main__':
    #CutBDTscore('BDT_input_had', 'model_had/trees', 'had')
    CutBDTscore('BDT_input_lep', 'model_lep/trees', 'lep')
