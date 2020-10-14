#!/usr/bin/env python2

import Path, Util
import os, sys
from Samples import sig_MC_s, bkg_MC_s
from os.path import join, exists


def RunSelection(cmd, nt):
    print 'Processing:', nt
    fp = os.popen( cmd.format(nt=nt) )
    print fp.read()
    fp.close()
# End of function RunSelection


def Selection(exe_name, indir_name, outdir_name, ch, option=''):
    exe = join(Path.dir_bin, exe_name)
    indir = join(Path.dir_2017, indir_name)
    outdir = join(Path.dir_2017, outdir_name)

    if not exists(exe):
        print '[ERROR] Executable does not exist!'
        sys.exit(1)
    if not exists(indir):
        print '[ERROR] Input dir does not exist!'
        sys.exit(1)
    if ch != 'had' and ch != 'lep':
        print '[ERROR] Invalid channel!'
        sys.exit(1)
    Util.CreateDir(outdir)

    cmd = '{bin} {indir}/{{nt}}.root {tree} {outdir}/{{nt}}.root {option}'.format(
            bin=exe, indir=indir, tree='T', outdir=outdir, option=option)

    # Process signal MC
    for sigtype in sig_MC_s:
        if sigtype[1] != ch:
            continue
        for nt in sig_MC_s[sigtype]:
            RunSelection(cmd, nt)
    # Process bkg MC
    for cat in bkg_MC_s:
        for nt in bkg_MC_s[cat]:
            RunSelection(cmd, nt)
    # Process data
    RunSelection(cmd, 'data')

    print 'Complete!'
# End of function Selection


if __name__ == '__main__':
    # Hadronic selection
    #Selection('Presel_had', 'Presel_basic', 'Presel_had', 'had')
    # Leptonic selection
    #Selection('Presel_lep', 'Presel_basic', 'Presel_lep', 'lep')
    # Photon IDMVA
    #Selection('Presel_photonIDMVA', 'Presel_had', 'Presel_had_phID', 'had')
    #Selection('Presel_photonIDMVA', 'Presel_lep', 'Presel_lep_phID', 'lep')
    # b-tag requirement
    Selection('Presel_btag', 'Presel_had_phID', 'Presel_had_phID_btag-L', 'had', '2017 L')
