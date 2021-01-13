#!/usr/bin/env python2

import Path, Util
from Samples import sig_MC
import os, sys
from os.path import exists, join


def DeepJet_Adjust(indir_name, ch):
    # Set paths
    exe = join(Path.dir_bin, 'DeepJet_Adjust')
    indir = join(Path.dir_2017, indir_name)
    tree = 'T'
    if not exists(exe):
        print '[ERROR] Executable not exist'
        return
    if not exists(indir):
        print '[ERROR] Input dir not exist'
        return
    if ch!='had' and ch!='lep':
        print '[ERROR] Invalid channel'
        return

    # Command template
    cmd = '{exe} {indir}/{{nt}}.root {tree}'.format(exe=exe, indir=indir, tree=tree)

    # Process signal MC
    for sigtype in sig_MC:
        if sigtype[1] != ch:
            continue
        for nt in sig_MC[sigtype]:
            print '---Start processing: {}'.format(nt)
            fp = os.popen( cmd.format(nt=nt) )
            print fp.read()
            fp.close()
# End of DeepJet_Adjust


if __name__ == '__main__':
    # hadronic
    #DeepJet_Adjust('GenPermInput_had/train_set', 'had')
    #DeepJet_Adjust('GenPermInput_had/opt_set', 'had')
    #DeepJet_Adjust('GenPermInput_had/fit_set', 'had')
    # leptonic
    DeepJet_Adjust('GenPermInput_lep/train_set', 'lep')
    DeepJet_Adjust('GenPermInput_lep/opt_set', 'lep')
    DeepJet_Adjust('GenPermInput_lep/fit_set', 'lep')
