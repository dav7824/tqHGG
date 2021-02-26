#!/usr/bin/env python2

import Path, Util
import os, sys
from os.path import join, exists
from Samples import sig_MC, bkg_MC


def BDT_train(indir_name, outdir_name, ch):
    exe = join(Path.dir_bin, 'BDT_train')
    indir = join(Path.dir_2017, indir_name)
    outdir = join(Path.dir_2017, outdir_name)
    if ch!='had' and ch!='lep':
        print '[ERROR] Invalid channel'
        sys.exit(1)
    if not exists(exe) or not exists(indir):
        print '[ERROR] Required files not exist'
        sys.exit(1)
    if not exists(outdir):
        Util.CreateDir(outdir)

    # Command template
    cmd = 'cd {outdir}; {exe} {indir} {outdir} {ch}'.format(exe=exe,
            indir=indir, outdir=outdir, ch=ch)

    # Run training
    fp = os.popen( cmd )
    print fp.read()
    fp.close()
# End of function BDT_train


if __name__ == '__main__':
    BDT_train('BDT_input_had', 'BDT_model', 'had')
    #BDT_train('BDT_input_lep', 'BDT_model', 'lep')
