#!/usr/bin/env python2

import Path, Util, Samples
import os, sys
from os.path import join, exists
from Samples import sig_MC_s, bkg_MC_s


def GetFilesSig(dir, ch):
    if not exists(dir):
        print '[ERROR] Directory does not exist'
        sys.exit(1)
    if ch != 'had' and ch != 'lep':
        print '[ERROR] Invalid channel'
        sys.exit(1)

    samples = []
    for sigtype in sig_MC_s:
        if sigtype[1] != ch:
            continue
        for nt in sig_MC_s[sigtype]:
            if not exists( join(dir,nt+'.root') ):
                continue
            samples.append(nt)

    return ','.join(samples)
# End of function GetFilesSig


def GetFilesBkg(dir):
    if not exists(dir):
        print '[ERROR] Directory does not exist'
        sys.exit(1)

    samples = []
    for cat in bkg_MC_s:
        # Test: skip QCD samples
        #if cat == 'QCD':
            #continue
        for nt in bkg_MC_s[cat]:
            if not exists( join(dir,nt+'.root') ):
                continue
            samples.append(nt)

    return ','.join(samples)
# End of function GetFilesBkg


def RunTraining(dir_train_name, dir_test_name, outdir_name, ch):
    if ch != 'had' and ch != 'lep':
        print '[ERROR] Invalid channel'
        sys.exit(1)
    exe = join(Path.dir_bin, 'BDTtrain_{}'.format(ch))
    dir_train = join(Path.dir_2017, dir_train_name)
    dir_test = join(Path.dir_2017, dir_test_name)
    outdir = join(Path.dir_2017, outdir_name)
    if not exists(exe):
        print '[ERROR] Executable does not exist'
        sys.exit(1)
    if not exists(dir_train):
        print '[ERROR] Training dir does not exist'
        sys.exit(1)
    if not exists(dir_test):
        print '[ERROR] Testing dir does not exist'
        sys.exit(1)
    if not exists(outdir):
        Util.CreateDir(outdir)

    # Get files for sig/bkg training/testing samples
    fins_sig_train = GetFilesSig(dir_train, ch)
    fins_sig_test  = GetFilesSig(dir_test, ch)
    fins_bkg_train = GetFilesBkg(dir_train)
    fins_bkg_test  = GetFilesBkg(dir_test)

    # Command template
    cmd = 'cd {outdir}; {bin} {fins_sig_train} {fins_sig_test} {fins_bkg_train} {fins_bkg_test} {dir_train} {dir_test} {outdir}'.format(
            bin=exe, fins_sig_train=fins_sig_train, fins_sig_test=fins_sig_test, fins_bkg_train=fins_bkg_train, fins_bkg_test=fins_bkg_test,
            dir_train=dir_train, dir_test=dir_test, outdir=outdir)

    # Run
    os.system(cmd)

    print 'Complete!'
# End of function RunTraining


if __name__ == '__main__':
    # hadronic BDT
    #dir_train = 'BDT_had/train_BDT'
    #dir_test = 'BDT_had/test_BDT'
    #outdir = 'BDT_had/BDT_v1'
    #outdir = 'BDT_had/BDT_v1_noQCD'
    #outdir = 'BDT_had/BDT_v2'  # ignore neg weights
    #outdir = 'BDT_had/BDT_v2_noQCD'  # ignore neg weights
    #ch = 'had'

    # leptonic BDT
    dir_train = 'BDT_lep/train_BDT'
    dir_test = 'BDT_lep/test_BDT'
    #outdir = 'BDT_lep/BDT_v1'
    #outdir = 'BDT_lep/BDT_v1_noQCD'
    #outdir = 'BDT_lep/BDT_v2'  # ignore neg weights
    outdir = 'BDT_lep/BDT_v2_noQCD'  # ignore neg weights
    ch = 'lep'

    RunTraining(dir_train, dir_test, outdir, ch)
