#!/usr/bin/env python2

import Path, Util, Samples
import os, sys
from os.path import join, exists
from Samples import sig_MC_s, bkg_MC_s


def Process(cmd, nt, log):
    print 'Processing:', nt
    fp = os.popen(cmd.format(nt=nt))
    log.write(fp.read())
    fp.close()
    log.write('\n\n')


def RunBDTapp(indir_name, fweight_name, outdir_name, ch):
    exe = join(Path.dir_bin, 'BDTapplication')
    indir = join(Path.dir_2017, indir_name)
    fweight = join(Path.dir_2017, fweight_name)
    outdir = join(Path.dir_2017, outdir_name)

    if not exists(indir) or not exists(fweight) or not exists(exe):
        print '[ERROR] Required files not exist!'
        sys.exit(1)
    if not exists(outdir):
        Util.CreateDir(outdir)

    # Command template
    cmd = '{exe} {indir}/{{nt}}.root {fweight} {outdir}/{{nt}}.root {ch}'.format(
            exe=exe, indir=indir, fweight=fweight, outdir=outdir, ch=ch)

    log = open(join(outdir,'BDTapplication.log'), 'w')
    # Process sig MC
    for sigtype in sig_MC_s:
        if sigtype[1] != ch:
            continue
        for nt in sig_MC_s[sigtype]:
            Process(cmd, nt, log)
    # Process bkg MC
    for cat in bkg_MC_s:
        for nt in bkg_MC_s[cat]:
            if not exists(join(indir,nt+'.root')):
                continue
            Process(cmd, nt, log)
    # Process data
    Process(cmd, 'data', log)

    log.close()
    print 'Complete!'
# End of function RunBDTapp


if __name__ == '__main__':
    # hadronic BDT test
    #indir = 'BDT_had/test_BDT'
    #fweight = 'BDT_had/BDT_v1/dataset/weights/hadronic_SR_BDT.weights.xml'
    #outdir = 'BDT_had/output_BDT_v1'
    #ch = 'had'

    # leptonic BDT test
    #indir = 'BDT_lep/test_BDT'
    #fweight = 'BDT_lep/BDT_v1/dataset/weights/leptonic_SR_BDT.weights.xml'
    #outdir = 'BDT_lep/output_BDT_v1'
    #ch = 'lep'

    # hadronic optimization
    #indir = 'optimization_had/BDT'
    #fweight = 'BDT_had/BDT_v1/dataset/weights/hadronic_SR_BDT.weights.xml'
    #outdir = 'optimization_had/output_BDT_v1'
    #ch = 'had'

    # leptonic optimization
    #indir = 'optimization_lep/BDT'
    #fweight = 'BDT_lep/BDT_v1/dataset/weights/leptonic_SR_BDT.weights.xml'
    #outdir = 'optimization_lep/output_BDT_v1'
    #ch = 'lep'

    # hadronic modeling
    #indir = 'model_had/BDT'
    #fweight = 'BDT_had/BDT_v1/dataset/weights/hadronic_SR_BDT.weights.xml'
    #outdir = 'model_had/output_BDT_v1'
    #ch = 'had'

    # leptonic modeling
    indir = 'model_lep/BDT'
    fweight = 'BDT_lep/BDT_v1/dataset/weights/leptonic_SR_BDT.weights.xml'
    outdir = 'model_lep/output_BDT_v1'
    ch = 'lep'

    RunBDTapp(indir, fweight, outdir, ch)
