#!/usr/bin/env python2

import Path, Util, Samples
import os, sys
from Samples import sig_MC_s, bkg_MC_s
from os.path import join, exists

def RunCalcYield(cmd, nt):
    print 'Processing:', nt
    fp = os.popen(cmd.format(nt=nt))
    fp.close()

def CalcYield(indir_name, outdir_name, trees, ch, SF_flags):
    print '[INFO] Start calculating yields'
    exe = join(Path.dir_bin, 'CalcYield')
    indir = join(Path.dir_2017, indir_name)
    outdir = join(Path.dir_2017, outdir_name)
    normdir = join(Path.dir_2017, 'normfactor')

    if not exists(exe):
        print '[ERROR] Executable doesn\'t exist!'
        sys.exit(1)
    if not exists(indir):
        print '[ERROR] Input dir doesn\'t exist!'
        sys.exit(1)
    if ch!='had' and ch!='lep':
        print '[ERROR] Invalid channel!'
        sys.exit(1)
    Util.CreateDirs(outdir)

    # Command template
    cmd = '{bin} {indir}/{{nt}}.root {trees} {normdir}/{{nt}}.root {outdir}/{{nt}}.root {SF_flags}'.format(
            bin=exe, indir=indir, trees=trees, normdir=normdir, outdir=outdir, SF_flags=SF_flags)

    # Process signal MC
    for sigtype in sig_MC_s:
        if sigtype[1]!=ch:
            continue
        for nt in sig_MC_s[sigtype]:
            RunCalcYield(cmd, nt)

    # Process bkg MC
    for cat in bkg_MC_s:
        for nt in bkg_MC_s[cat]:
            RunCalcYield(cmd, nt)

    print '[INFO] Complete calculating yields'
# End of function RunCalcYield


def AddYields(indir_name, outdir_name, ch):
    print '[INFO] Start adding yields'
    exe = join(Path.dir_bin, 'AddYields')
    indir = join(Path.dir_2017, indir_name)
    outdir = join(Path.dir_2017, outdir_name)

    if not exists(exe):
        print '[ERROR] Executable doesn\'t exist!'
        sys.exit(1)
    if not exists(indir):
        print '[ERROR] Input dir doesn\'t exist!'
        sys.exit(1)
    if ch!='had' and ch!='lep':
        print '[ERROR] Invalid channel!'
        sys.exit(1)
    Util.CreateDirs(outdir)

    # Command template
    cmd_1 = 'cp {indir}/{{ori_name}}.root {outdir}/{{new_name}}.root'.format(indir=indir, outdir=outdir)
    cmd_2 = '{bin} {{fin_list}} {indir} {outdir}/{{cat}}.root'.format(bin=exe, indir=indir, outdir=outdir)

    # Process signal MC
    for sigtype in sig_MC_s:
        if sigtype[1]!=ch:
            continue
        cat = ''.join(sigtype)
        print 'Processing:', cat
        if len( sig_MC_s[sigtype] ) == 1:
            os.system( cmd_1.format(ori_name=sig_MC_s[sigtype][0], new_name=cat) )
        else:
            samples = ','.join(sig_MC_s[sigtype])
            fp = os.popen( cmd_2.format(fin_list=samples, cat=cat) )
            fp.close()

    # Process bkg MC
    for cat in bkg_MC_s:
        print 'Processing:', cat
        if len( bkg_MC_s[cat] ) == 1:
            os.system( cmd_1.format(ori_name=bkg_MC_s[cat][0], new_name=cat) )
        else:
            samples = ','.join(bkg_MC_s[cat])
            fp = os.popen( cmd_2.format(fin_list=samples, cat=cat) )
            fp.close()

    print '[INFO] Complete adding yields'
# End of function AddYields


def PrintYieldSummary(indir_name, ntdir_name):
    print '[INFO] Print yield summary'
    exe = join(Path.dir_bin, 'PrintYieldSummary')
    indir = join(Path.dir_2017, indir_name)
    fdata = join(Path.dir_2017, ntdir_name, 'data.root')

    if not exists(exe):
        print '[ERROR] Executable doesn\'t exist!'
        sys.exit(1)
    if not exists(indir):
        print '[ERROR] Input dir doesn\'t exist!'
        sys.exit(1)

    # Files to be read
    ls_samples = []
    for cat in bkg_MC_s:
        ls_samples.append(cat)
    samples = ','.join(ls_samples)

    # Command template
    cmd = '{bin} {fin_list} {name_list} {indir} {fdata}'.format(
            bin=exe, fin_list=samples, name_list=samples, indir=indir, fdata=fdata)

    # Run & write stdout to log file
    fout = open(join(indir,'yield.txt'), 'w')
    fp = os.popen(cmd)
    fout.write(fp.read())
    fp.close()
    fout.close()
# End of function PrintYieldSummary


def PrintYieldSummary_sig(indir_name, ch):
    print '[INFO] Print yield summary of signal'
    exe = join(Path.dir_bin, 'PrintYieldSummary_sig')
    indir = join(Path.dir_2017, indir_name)

    if not exists(exe):
        print '[ERROR] Executable doesn\'t exist!'
        sys.exit(1)
    if not exists(indir):
        print '[ERROR] Input dir doesn\'t exist!'
        sys.exit(1)
    if ch!='had' and ch!='lep':
        print '[ERROR] Invalid channel!'
        sys.exit(1)

    # Files to be read
    ls_samples = []
    for sigtype in sig_MC_s:
        if sigtype[1] != ch:
            continue
        ls_samples.append(''.join(sigtype))
    samples = ','.join(ls_samples)

    # Command template
    cmd = '{bin} {fin_list} {name_list} {indir}'.format(
            bin=exe, fin_list=samples, name_list=samples, indir=indir)

    # Run & write stdout to log
    fout = open(join(indir,'yield.txt'), 'a')
    fp = os.popen(cmd)
    fout.write(fp.read())
    fp.close()
    fout.close()
# End of function PrintYieldSummary_sig


if __name__ == '__main__':
    # Presel_had_phID_btag-L
    #indir_name = 'Presel_had_phID_btag-L'
    #outdir_name = 'Presel_had_phID_btag-L__hist/yield_ori_samples'
    #outdir2_name = 'Presel_had_phID_btag-L__hist/yield_ori_cat'
    #trees = 'T'
    #ch = 'had'
    #SF_flags = '000000'

    # Presel_had_phID_btag-L + PU + photon
    #indir_name = 'Presel_had_phID_btag-L'
    #outdir_name = 'Presel_had_phID_btag-L__hist/yield_PU-pho_samples'
    #outdir2_name = 'Presel_had_phID_btag-L__hist/yield_PU-pho_cat'
    #trees = 'T+SF_pileup'
    #ch = 'had'
    #SF_flags = '100001'

    # Presel_lep_phID
    #indir_name = 'Presel_lep_phID'
    #outdir_name = 'Presel_lep_phID__hist/yield_ori_samples'
    #outdir2_name = 'Presel_lep_phID__hist/yield_ori_cat'
    #trees = 'T'
    #ch = 'lep'
    #SF_flags = '000000'

    # Presel_lep_phID + PU + ElecID + ElecReco + MuonID + MuonISO + photon
    indir_name = 'Presel_lep_phID'
    outdir_name = 'Presel_lep_phID__hist/yield_PU-e-mu-pho_samples'
    outdir2_name = 'Presel_lep_phID__hist/yield_PU-e-mu-pho_cat'
    trees = 'T+SF_pileup+SF_Elec+SF_Muon'
    ch = 'lep'
    SF_flags = '111111'

    # Run the work flow
    CalcYield(indir_name, outdir_name, trees, ch, SF_flags)
    AddYields(outdir_name, outdir2_name, ch)
    PrintYieldSummary(outdir2_name, indir_name)
    PrintYieldSummary_sig(outdir2_name, ch)
