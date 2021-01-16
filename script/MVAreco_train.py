#!/usr/bin/env python2

import Path, Util
from Samples import sig_MC, bkg_MC
import os, sys
from os.path import join, exists


def MVAreco_train(indir_train_name, indir_test_name, outdir_name, type, ch):
    print '---MVA reconstruction training: {} {}'.format(type, ch)
    # Set paths
    exe_name = 'MVAreco_train'
    exe = join(Path.dir_bin, exe_name)
    indir_train = join(Path.dir_2017, indir_train_name)
    indir_test = join(Path.dir_2017, indir_test_name)
    outdir = join(Path.dir_2017, outdir_name)
    if type!='TT' and type!='ST':
        print '[ERROR] Invalid signal type'
        return
    if ch!='had' and ch!='lep':
        print '[ERROR] Invalid channel'
        return
    if not exists(exe):
        print '[ERROR] Executable not exist'
        return
    if not exists(indir_train):
        print '[ERROR] Training input dir not exist'
        return
    if not exists(indir_test):
        print '[ERROR] Testing input dir not exist'
        return
    Util.CreateDirs(outdir)

    # Command template
    if type=='TT':
        filename = 'TT_FCNC*'
    else:
        filename = 'ST_FCNC*'
    cmd = 'set -f; cd {outdir};  {exe} {indir_train}/{filename} {indir_test}/{filename} {outdir} {type} {ch}'.format(
            exe=exe, indir_train=indir_train, indir_test=indir_test, filename=filename, outdir=outdir, type=type, ch=ch)

    # Submit job
    jobname = 'MVAreco_{}{}'.format(type, ch)
    Util.submit_job(cmd, jobname)
# End of function MVAreco_train


if __name__ == '__main__':
    MVAreco_train('MVAreco_perm_had/Train_input', 'MVAreco_perm_had/opt_set', 'MVAreco_model', 'TT', 'had')
    MVAreco_train('MVAreco_perm_had/Train_input', 'MVAreco_perm_had/opt_set', 'MVAreco_model', 'ST', 'had')
    MVAreco_train('MVAreco_perm_lep/Train_input', 'MVAreco_perm_lep/opt_set', 'MVAreco_model', 'TT', 'lep')
    MVAreco_train('MVAreco_perm_lep/Train_input', 'MVAreco_perm_lep/opt_set', 'MVAreco_model', 'ST', 'lep')
