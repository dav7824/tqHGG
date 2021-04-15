#!/usr/bin/env python2

import Path, Util
from Samples import sig_MC, bkg_MC
from SystUnc import *
import os, sys
from os.path import join, exists


def Process(cmd, nt):
    fp = os.popen( cmd.format(nt=nt) )
    fp.close()


# MakeBDTinput: Produce BDT tree for given channel, systematic, and variation (Up or Down)
def MakeBDTinput(evtdir_name, recodir_name, outdir_name, ch, syst, systdir):
    if systdir!='Up' and systdir!='Down':
        print '[ERROR] Systematic variation has to be "Up" or "Down"!'
        sys.exit(1)
    if not (syst in Syst_tree_1 or syst in Syst_tree_2):
        print '[ERROR] Invalid systematic name!'
        sys.exit(1)
    if ch == 'had':
        exe = join(Path.dir_bin, 'MakeBDTinput_had')
    elif ch == 'lep':
        exe = join(Path.dir_bin, 'MakeBDTinput_lep')
    else:
        print '[ERROR] Invalid channel'
        sys.exit(1)
    evtdir = join(Path.dir_2017, 'Systematics', evtdir_name, syst+systdir)
    recodir = join(Path.dir_2017, 'Systematics', recodir_name, syst+systdir)
    outdir = join(Path.dir_2017, 'Systematics', outdir_name, syst+systdir)
    normdir = join(Path.dir_2017, 'normfactor')

    if not exists(exe):
        print '[ERROR] Executable not exist'
        sys.exit(1)
    if not exists(evtdir) or not exists(recodir):
        print '[ERROR] Input dir not exist'
        sys.exit(1)
    if not exists(outdir):
        Util.CreateDirs(outdir)

    # Command template
    cmd = '{exe} {evtdir}/{{nt}}.root {recodir}/{{nt}}.root {outdir}/{{nt}}.root {normdir}/{{nt}}.root {syst} 3'.format(
            exe=exe, evtdir=evtdir, recodir=recodir, outdir=outdir, normdir=normdir, syst='nosyst')

    # Process signal MC
    for sigtype in sig_MC:
        if sigtype[1] != ch:
            continue
        for nt in sig_MC[sigtype]:
            Process(cmd, nt)
    # Process resonant bkg
    for nt in bkg_MC['Higgs']:
        Process(cmd, nt)
# End of function MakeBDTinput


# BDT_apply: Apply trained BDT for given channel, systematic, and variation (Up or Down)
def BDT_apply(indir_name, ch, syst, systdir):
    if systdir!='Up' and systdir!='Down':
        print '[ERROR] Systematic variation has to be "Up" or "Down"!'
        sys.exit(1)
    if not (syst in Syst_tree_1 or syst in Syst_tree_2):
        print '[ERROR] Invalid systematic name!'
        sys.exit(1)
    exe = join(Path.dir_bin, 'BDT_application')
    indir = join(Path.dir_2017, 'Systematics', indir_name, syst+systdir)
    weidir = join(Path.dir_2017, 'BDT_model/dataset/weights')
    if ch == 'had':
        fweight = join(weidir, 'SRhad_BDT.weights.xml')
    elif ch == 'lep':
        fweight = join(weidir, 'SRlep_BDT.weights.xml')
    else:
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
    # Process resonant bkg MC
    for nt in bkg_MC['Higgs']:
        Process(cmd, nt)
# End of function BDT_apply


# AddBdtVar: Add variables for given channel, systematic, and variation (Up or Down)
def AddBdtVar(evtdir_name, bdtdir_name, ch, syst, systdir):
    if systdir!='Up' and systdir!='Down':
        print '[ERROR] Systematic variation has to be "Up" or "Down"!'
        sys.exit(1)
    if not (syst in Syst_tree_1 or syst in Syst_tree_2):
        print '[ERROR] Invalid systematic name!'
        sys.exit(1)
    exe = join(Path.dir_bin, 'AddBdtVar')
    evtdir = join(Path.dir_2017, 'Systematics', evtdir_name, syst+systdir)
    bdtdir = join(Path.dir_2017, 'Systematics', bdtdir_name, syst+systdir)
    if ch!='had' and ch!='lep':
        print '[ERROR] Invalid channel'
        sys.exit(1)
    if not exists(exe) or not exists(evtdir) or not exists(bdtdir):
        print '[ERROR] Required files not exist'
        sys.exit(1)

    # Command template
    cmd = '{exe} {evtdir}/{{nt}}.root {bdtdir}/{{nt}}.root'.format(
            exe=exe, evtdir=evtdir, bdtdir=bdtdir)

    # Process signal MC
    for sigtype in sig_MC:
        if sigtype[1] != ch:
            continue
        for nt in sig_MC[sigtype]:
            Process(cmd, nt)
    # Process resonant bkg MC
    for nt in bkg_MC['Higgs']:
        Process(cmd, nt)
# End of function AddBdtVar


# ProcSystBDT: Run all BDT processes for given channel, systematic, and variation (Up or Down)
def ProcSystBDT(ch, syst, systdir):
    if systdir!='Up' and systdir!='Down':
        print '[ERROR] Systematic variation has to be "Up" or "Down"!'
        sys.exit(1)
    if not (syst in Syst_tree_1 or syst in Syst_tree_2):
        print '[ERROR] Invalid systematic name!'
        sys.exit(1)
    if ch == 'had':
        evtdir_name = 'Presel_had_phID_btag-L'
        recodir_name = 'MVAreco_result_had'
        bdtdir_name = 'BDT_input_had'
    elif ch == 'lep':
        evtdir_name = 'Presel_lep_phID'
        recodir_name = 'MVAreco_result_lep'
        bdtdir_name = 'BDT_input_lep'
    else:
        print '[ERROR] Invalid channel'
        sys.exit(1)
    evtdir = join(Path.dir_2017, 'Systematics', evtdir_name, syst+systdir)
    recodir = join(Path.dir_2017, 'Systematics', recodir_name, syst+systdir)
    if not (exists(evtdir) and exists(recodir)):
        print '[ERROR] Input dir not exist'
        sys.exit(1)

    # Run sequence
    MakeBDTinput(evtdir_name, recodir_name, bdtdir_name, ch, syst, systdir)
    BDT_apply(bdtdir_name, ch, syst, systdir)
    AddBdtVar(evtdir_name, bdtdir_name, ch, syst, systdir)
# End of function ProcSystBDT
