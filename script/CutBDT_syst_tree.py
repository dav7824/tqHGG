#!/usr/bin/env python2

import Path, Util
from Samples import sig_MC, bkg_MC
from SystUnc import *
import os, sys
from os.path import join, exists


# ProcSig: Process TT & ST signals for given systematic, coupling, and channel
def ProcSig(indir_name, coupling, ch, syst_name, fout_name):
    exe = join(Path.dir_bin, 'CutBDT_syst_signal_tree')
    indir = join(Path.dir_2017, 'Systematics', indir_name)
    outdir = join(Path.dir_2017, 'Datacard_syst_tree')

    if not (syst_name in Syst_tree_1 or syst_name in Syst_tree_2):
        print '[ERROR] Invalid systematic type'
        sys.exit(1)
    if not (ch=='had' or ch=='lep'):
        print '[ERROR] Invalid channel'
        sys.exit(1)
    if not (coupling=='hut' or coupling=='hct'):
        print '[ERROR] Invalid coupling'
        sys.exit(1)
    if not exists(exe):
        print '[ERROR] Executable does not exist'
        sys.exit(1)
    if not exists(indir):
        print '[ERROR] Input dir does not exist'
        sys.exit(1)
    if not exists(outdir):
        Util.CreateDir(outdir)

    # Command template
    cmd = '{exe} {indir} {coupling} {ch} {syst_name} {fout}'.format(
            exe=exe, indir=indir, coupling=coupling, ch=ch,
            syst_name=syst_name, fout=join(outdir,fout_name))

    # Run
    fp = os.popen(cmd)
    fp.close()
# End of function ProcSig


# ProcRbkg: Process resonant backgrounds for given systematic and channel
def ProcRbkg(indir_name, ch, syst_name, fout_name):
    exe = join(Path.dir_bin, 'CutBDT_syst_resbkg_tree')
    indir = join(Path.dir_2017, 'Systematics', indir_name)
    outdir = join(Path.dir_2017, 'Datacard_syst_tree')

    if not (syst_name in Syst_tree_1 or syst_name in Syst_tree_2):
        print '[ERROR] Invalid systematic type'
        sys.exit(1)
    if not (ch=='had' or ch=='lep'):
        print '[ERROR] Invalid channel'
        sys.exit(1)
    if not exists(exe):
        print '[ERROR] Executable does not exist'
        sys.exit(1)
    if not exists(indir):
        print '[ERROR] Input dir does not exist'
        sys.exit(1)
    if not exists(outdir):
        Util.CreateDir(outdir)

    # Command template
    cmd = '{exe} {indir} {ch} {syst_name} {fout}'.format(
            exe=exe, indir=indir, ch=ch,
            syst_name=syst_name, fout=join(outdir,fout_name))

    # Run
    fp = os.popen(cmd)
    fp.close()
# End of function ProcRbkg


# ProcSystEntry: Process all signals & resonant bkgs for given systematic and channel
def ProcSystEntry(indir_name, ch, syst_name, fout_tag):
    if not (syst_name in Syst_tree_1 or syst_name in Syst_tree_2):
        print '[ERROR] Invalid systematic type'
        sys.exit(1)
    if ch!='had' and ch!='lep':
        print '[ERROR] Invalid channel'
        sys.exit(1)

    # Process signal MCs
    for coupling in ['hut', 'hct']:
        fout_name = 'sig_{}_{}_{}.txt'.format(coupling, ch, fout_tag)
        ProcSig( indir_name, coupling, ch, syst_name, fout_name )
    # Process resonant bkg MCs
    fout_name = 'resbkg_{}_{}.txt'.format(ch, fout_tag)
    ProcRbkg( indir_name, ch, syst_name, fout_name )
# End of function ProcSystEntry
