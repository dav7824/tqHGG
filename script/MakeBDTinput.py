#!/usr/bin/env python2

import Path, Util, Samples
import os, sys
from os.path import join, exists
from Samples import sig_MC_s, bkg_MC_s


def RunSample(indir_name, outdir_name, ch, sample, nscale):
    if ch == 'had':
        exe = join(Path.dir_bin, 'MakeBDTinput_had')
    elif ch == 'lep':
        exe = join(Path.dir_bin, 'MakeBDTinput_lep')
    else:
        print '[ERROR] Invalid channel'
        sys.exit(1)
    filename = sample + '.root'
    fin = join(Path.dir_2017, indir_name, filename)
    outdir = join(Path.dir_2017, outdir_name)
    fout = join(outdir, filename)
    fnorm = join(Path.dir_2017, 'normfactor', filename)

    if not exists(exe):
        print '[ERROR] Executable not exist'
        sys.exit(1)
    # If the input file does not exist, return
    if not exists(fin):
        print '[Warning] Input file not exist: {}\n'.format(filename)
        return
    if not exists(outdir):
        Util.CreateDir(outdir)

    # Command template
    cmd = '{bin} {fin} {fout} {fnorm} {nscale}'.format(
            bin=exe, fin=fin, fout=fout, fnorm=fnorm, nscale=nscale)

    # Run
    print 'Processing:', sample
    fp = os.popen(cmd)
    print fp.read()
    fp.close()
# End of function RunSample


def RunSignal(indir_name, outdir_name, ch):
    for sigtype in sig_MC_s:
        if sigtype[1] != ch:
            continue
        for nt in sig_MC_s[sigtype]:
            RunSample(indir_name, outdir_name, ch, nt, 4)


def RunResonantBkg(indir_name, outdir_name, ch):
    for nt in bkg_MC_s['Higgs']:
        RunSample(indir_name, outdir_name, ch, nt, 3)


def RunNonResonantBkg(indir_name, outdir_name, ch):
    for cat in bkg_MC_s:
        if cat == 'Higgs':
            continue
        for nt in bkg_MC_s[cat]:
            RunSample(indir_name, outdir_name, ch, nt, 2)


if __name__ == '__main__':
    # BDT had training
    #print '\n<BDT had training>'
    #indir = 'BDT_had/train'
    #outdir = 'BDT_had/train_BDT'
    #ch = 'had'
    #RunSignal(indir, outdir, ch)
    #RunResonantBkg(indir, outdir, ch)
    #RunNonResonantBkg(indir, outdir, ch) # No need to run this for optimization / model

    # BDT had testing
    #print '\n<BDT had testing>'
    #indir = 'BDT_had/test'
    #outdir = 'BDT_had/test_BDT'
    #ch = 'had'
    #RunSignal(indir, outdir, ch)
    #RunResonantBkg(indir, outdir, ch)
    #RunNonResonantBkg(indir, outdir, ch) # No need to run this for optimization / model
    #RunSample(indir, outdir, ch, 'data', 1)

    # BDT lep training
    #print '\n<BDT lep training>'
    #indir = 'BDT_lep/train'
    #outdir = 'BDT_lep/train_BDT'
    #ch = 'lep'
    #RunSignal(indir, outdir, ch)
    #RunResonantBkg(indir, outdir, ch)
    #RunNonResonantBkg(indir, outdir, ch) # No need to run this for optimization / model

    # BDT lep testing
    #print '\n<BDT lep testing>'
    indir = 'BDT_lep/test'
    outdir = 'BDT_lep/test_BDT'
    ch = 'lep'
    #RunSignal(indir, outdir, ch)
    #RunResonantBkg(indir, outdir, ch)
    #RunNonResonantBkg(indir, outdir, ch) # No need to run this for optimization / model
    RunSample(indir, outdir, ch, 'data', 1)

    # had optimization
    #print '\n<had optimization>'
    #indir = 'optimization_had/plain'
    #outdir = 'optimization_had/BDT'
    #ch = 'had'
    #RunSignal(indir, outdir, ch)
    #RunResonantBkg(indir, outdir, ch)
    #RunSample(indir, outdir, ch, 'data', 2)

    # lep optimization
    #print '\n<lep optimization>'
    #indir = 'optimization_lep/plain'
    #outdir = 'optimization_lep/BDT'
    #ch = 'lep'
    #RunSignal(indir, outdir, ch)
    #RunResonantBkg(indir, outdir, ch)
    #RunSample(indir, outdir, ch, 'data', 2)

    # had model
    #print '\n<had model>'
    #indir = 'model_had/plain'
    #outdir = 'model_had/BDT'
    #ch = 'had'
    #RunSignal(indir, outdir, ch)
    #RunResonantBkg(indir, outdir, ch)
    #RunSample(indir, outdir, ch, 'data', 2)

    # lep model
    #print '\n<lep model>'
    #indir = 'model_lep/plain'
    #outdir = 'model_lep/BDT'
    #ch = 'lep'
    #RunSignal(indir, outdir, ch)
    #RunResonantBkg(indir, outdir, ch)
    #RunSample(indir, outdir, ch, 'data', 2)
