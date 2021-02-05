#!/usr/bin/env python2
# Fill histograms for each sample.

import Path, Util
from Samples import sig_MC, bkg_MC
import sys, os
from os.path import join, exists


def Process(cmd, nt):
    print 'Processing:', nt
    fp = os.popen( cmd.format(nt=nt) )
    #print fp.read()
    fp.close()


def FillHist(exe_name, indir_name, outdir_name, ch):
    exe = join(Path.dir_bin, exe_name)
    indir = join(Path.dir_2017, indir_name)
    outdir = join(Path.dir_2017, outdir_name)
    if ch!='had' and ch!='lep':
        print '[ERROR] Invalid channel!'
        sys.exit(1)
    if not exists(exe):
        print '[ERROR] Executable doesn\'t exist!'
        sys.exit(1)
    if not exists(indir):
        print '[ERROR] Input dir doesn\'t exist!'
        sys.exit(1)
    Util.CreateDirs(outdir)

    # Command template
    cmd = '{exe} {indir}/{{nt}}.root {outdir}/{{nt}}.root'.format(
            exe=exe, indir=indir, outdir=outdir)

    # Process signal MC
    for sigtype in sig_MC:
        if sigtype[1]!=ch:
            continue
        for nt in sig_MC[sigtype]:
            Process(cmd, nt)
    # Process bkg MC
    for cat in bkg_MC:
        for nt in bkg_MC[cat]:
            if ch=='lep' and nt=='QCD_Pt-30to40_MGG-80toInf':
                continue
            Process(cmd, nt)
    # Process data
    Process(cmd, 'data')
# End of function FillHist

'''
def FillHist_BDT(exe_name, indir_name, outdir_name, weight, ch):
    exe = join(Path.dir_bin, exe_name)
    indir = join(Path.dir_2017, indir_name)
    outdir = join(Path.dir_2017, outdir_name)
    if not exists(exe) or not exists(indir):
        print '[ERROR] Required files not exist!'
        sys.exit(1)
    if ch!='had' and ch!='lep':
        print '[ERROR] Invalid channel'
        sys.exit(1)
    if not exists(outdir):
        Util.CreateDir(outdir)

    # Command template
    cmd = '{exe} {indir}/{{nt}}.root {outdir}/{{nt}}.root \'{weight}\''.format(
            exe=exe, indir=indir, outdir=outdir, weight=weight)

    # Process signal MC
    for sigtype in sig_MC_s:
        if sigtype[1] != ch:
            continue
        for nt in sig_MC_s[sigtype]:
            RunFillHist(cmd, nt)
    # Process bkg MC
    for cat in bkg_MC_s:
        for nt in bkg_MC_s[cat]:
            if not exists(join(indir,nt+'.root')):
                continue
            RunFillHist(cmd, nt)
    # Process data
    RunFillHist(cmd ,'data')

# End of function FillHist_BDT
'''

if __name__ == '__main__':
    #FillHist('FillHist_had', 'Presel_had_phID_btag-L', 'Presel_had_phID_btag-L/hist', 'had')
    FillHist('FillHist_lep', 'Presel_lep_phID', 'Presel_lep_phID/hist', 'lep')
