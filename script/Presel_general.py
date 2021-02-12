#!/usr/bin/env python2

import Path, Util, SystUnc
import os, sys
from Samples import sig_MC, bkg_MC
from os.path import join, exists


def RunSelection(cmd, nt):
    print 'Processing:', nt
    fp = os.popen( cmd.format(nt=nt) )
    print fp.read()
    fp.close()
# End of function RunSelection


def Selection(exe_name, indir_name, outdir_name, ch, option=''):
    # Set paths
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

    # Command template
    cmd = '{bin} {indir}/{{nt}}.root {tree} {outdir}/{{nt}}.root {option}'.format(
            bin=exe, indir=indir, tree='T', outdir=outdir, option=option)

    # Process signal MC
    for sigtype in sig_MC:
        if sigtype[1] != ch:
            continue
        for nt in sig_MC[sigtype]:
            RunSelection(cmd, nt)
    # Process bkg MC
    for cat in bkg_MC:
        for nt in bkg_MC[cat]:
            RunSelection(cmd, nt)
    # Process data
    RunSelection(cmd, 'data')

    print 'Complete!'
# End of function Selection


def Selection_syst(exe_name, indir_name, outdir_name, syst, systdir, ch, option=''):
    if systdir!='Up' and systdir!='Down':
        print '[ERROR] Systematic variation has to be "Up" or "Down"!'
        return
    if not (syst in SystUnc.Syst_tree_1 or syst in SystUnc.Syst_tree_2 or
            syst in SystUnc.Syst_weight_1 or syst in SystUnc.Syst_weight_2 or syst in SystUnc.Syst_weight_3):
        print '[ERROR] Invalid systematic name!'
        return
    # Set paths
    exe = join(Path.dir_bin, exe_name)
    indir = join(Path.dir_2017, 'Systematics', indir_name, syst+systdir)
    outdir = join(Path.dir_2017, 'Systematics', outdir_name, syst+systdir)
    if not exists(exe):
        print '[ERROR] Executable does not exist!'
        return
    if not exists(indir):
        print '[ERROR] Input dir does not exist!'
        return
    if ch != 'had' and ch != 'lep':
        print '[ERROR] Invalid channel!'
        return
    Util.CreateDirs(outdir)

    # Command template
    cmd = '{bin} {indir}/{{nt}}.root {tree} {outdir}/{{nt}}.root {option}'.format(
            bin=exe, indir=indir, tree='T', outdir=outdir, option=option)

    # Process signal MC
    for sigtype in sig_MC:
        if sigtype[1] != ch:
            continue
        for nt in sig_MC[sigtype]:
            RunSelection(cmd, nt)
    # Process resonant bkg MC
    for nt in bkg_MC['Higgs']:
        RunSelection(cmd, nt)

    print 'Complete!'
# End of function Selection_syst


# Process each systematic for given selection
def ProcSysts(exe_name, indir_name, outdir_name, ch, option=''):
    # Set paths
    exe = join(Path.dir_bin, exe_name)
    indir = join(Path.dir_2017, 'Systematics', indir_name)
    if not exists(exe):
        print '[ERROR] Executable does not exist!'
        return
    if not exists(indir):
        print '[ERROR] Input dir does not exist!'
        return
    if ch != 'had' and ch != 'lep':
        print '[ERROR] Invalid channel!'
        return

    # Process each systematics
    systs = SystUnc.Syst_tree_1 + SystUnc.Syst_tree_2
    for syst in systs:
        for dir in ['Up', 'Down']:
            Selection_syst(exe_name, indir_name, outdir_name, syst, dir, ch, option)
            print '\n\n'
# End of function ProcSysts


if __name__ == '__main__':
    ## Central samples
    # Hadronic selection
    #Selection('Presel_had', 'Presel_basic', 'Presel_had', 'had')
    # Leptonic selection
    #Selection('Presel_lep', 'Presel_basic', 'Presel_lep', 'lep')
    # Photon IDMVA
    #Selection('Presel_photonIDMVA', 'Presel_had', 'Presel_had_phID', 'had')
    #Selection('Presel_photonIDMVA', 'Presel_lep', 'Presel_lep_phID', 'lep')
    # b-tag requirement
    #Selection('Presel_btag', 'Presel_had_phID', 'Presel_had_phID_btag-L', 'had', '2017 L')

    ## Systematic samples
    # Hadronic selection
    #ProcSysts('Presel_had', 'Presel_basic', 'Presel_had', 'had')
    # Leptonic selection
    #ProcSysts('Presel_lep', 'Presel_basic', 'Presel_lep', 'lep')
    # Photon IDMVA
    #ProcSysts('Presel_photonIDMVA', 'Presel_had', 'Presel_had_phID', 'had')
    #ProcSysts('Presel_photonIDMVA', 'Presel_lep', 'Presel_lep_phID', 'lep')
    # b-tag requirement
    #ProcSysts('Presel_btag', 'Presel_had_phID', 'Presel_had_phID_btag-L', 'had', '2017 L')

    ## Photon MVA ID sideband (central)
    Selection('Presel_had_phIDSB', 'Presel_had', 'Presel_had_phIDSB', 'had')
