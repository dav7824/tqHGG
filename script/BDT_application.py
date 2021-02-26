#!/usr/bin/env python2

import Path, Util
import os, sys
from os.path import join, exists
from Samples import sig_MC, bkg_MC


def Process(cmd, nt):
    print '---Processing:', nt
    fp = os.popen(cmd.format(nt=nt))
    print fp.read()
    fp.close()


def BDT_apply(indir_name, fweight_name, ch):
    exe = join(Path.dir_bin, 'BDT_application')
    indir = join(Path.dir_2017, indir_name)
    fweight = join(Path.dir_2017, 'BDT_model/dataset/weights', fweight_name)

    if ch!='had' and ch!='lep':
        print '[ERROR] Invalid channel'
        sys.exit(1)
    if not exists(indir) or not exists(fweight) or not exists(exe):
        print '[ERROR] Required files not exist!'
        sys.exit(1)

    # Command template
    cmd = '{exe} {indir}/{{nt}}.root {fweight} {ch}'.format(
            exe=exe, indir=indir, fweight=fweight, ch=ch)

    # Process sig MC
    for sigtype in sig_MC:
        if sigtype[1] != ch:
            continue
        for nt in sig_MC[sigtype]:
            Process(cmd, nt)
    # Process bkg MC
    for cat in bkg_MC:
        # For had channel, skip MCs replaced by data-driven QCD
        if ch=='had' and (cat in ['GGJets','GJet','QCD']):
            continue
        for nt in bkg_MC[cat]:
            # Skip samples with no event
            if ch=='lep' and nt=='QCD_Pt-30to40_MGG-80toInf':
                continue
            Process(cmd, nt)
    # Process data-driven QCD
    if ch=='had':
        Process(cmd, 'Data-driven_QCD')
        Process(cmd, 'DiPhotonJetsBox_scaled')
    # Process data
    Process(cmd, 'data')
# End of function BDT_apply


if __name__ == '__main__':
    #BDT_apply('BDT_input_had', 'SRhad_BDT.weights.xml', 'had')
    BDT_apply('BDT_input_lep', 'SRlep_BDT.weights.xml', 'lep')
