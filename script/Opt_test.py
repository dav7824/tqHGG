#!/usr/bin/env python2

import Path, Util, Samples
import os, sys
from os.path import join, exists
from Samples import sig_MC_s, bkg_MC_s


def RunTest(indir_name, outdir_name, ch):
    exe = join(Path.dir_bin, 'Opt_test')
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

    # Get signal samples
    ls_sig = []
    for sigtype in sig_MC_s:
        if sigtype[1] != ch:
            continue
        for nt in sig_MC_s[sigtype]:
            ls_sig.append(nt)
    fsig = ','.join(ls_sig)
    # Get resonant bkg
    ls_resbkg = []
    for nt in bkg_MC_s['Higgs']:
        ls_resbkg.append(nt)
    fresbkg = ','.join(ls_resbkg)
    # Get data
    fdata = 'data'

    # Command template
    cmd = '{exe} {indir} {fsig} {fresbkg} {fdata} {outdir}'.format(
            exe=exe, indir=indir, fsig=fsig, fresbkg=fresbkg, fdata=fdata, outdir=outdir)

    # Run
    fp = os.popen(cmd)
    print fp.read()
    fp.close()
    print 'Complete'
# End of function RunTest


if __name__ == '__main__':
    # hadronic optimization
    #indir = 'optimization_had/output_BDT_v1'
    #outdir = 'optimization_had/test_BDT_v1'
    #ch = 'had'

    # leptonic optimization
    indir = 'optimization_lep/output_BDT_v1'
    outdir = 'optimization_lep/test_BDT_v1'
    ch = 'lep'

    # Run
    RunTest(indir, outdir, ch)
