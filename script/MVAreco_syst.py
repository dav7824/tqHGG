#!/usr/bin/env python2

import Path, Util
from Samples import sig_MC, bkg_MC
from SystUnc import *
import os, sys
from os.path import join, exists


def Process(cmd, nt):
    fp = os.popen( cmd.format(nt=nt) )
    fp.close()


# GenPerm: Generate TT & ST permutations for samples of given channel, systematic, and variation (Up or Down)
def GenPerm(indir_name, outdir_name, ch, syst, systdir):
    if systdir!='Up' and systdir!='Down':
        print '[ERROR] Systematic variation has to be "Up" or "Down"!'
        sys.exit(1)
    if not (syst in Syst_tree_1 or syst in Syst_tree_2):
        print '[ERROR] Invalid systematic name!'
        sys.exit(1)
    # Check channel
    if ch == 'had':
        exeTT = join(Path.dir_bin, 'MVAreco_GenPerm_TThad')
        exeST = join(Path.dir_bin, 'MVAreco_GenPerm_SThad')
    elif ch == 'lep':
        exeTT = join(Path.dir_bin, 'MVAreco_GenPerm_TTlep')
        exeST = join(Path.dir_bin, 'MVAreco_GenPerm_STlep')
    else:
        print '[ERROR] Invalid channel'
        sys.exit(1)
    # Input dir
    indir = join(Path.dir_2017, 'Systematics', indir_name, syst+systdir)
    if not exists(indir):
        print '[ERROR] Input dir not exist'
        sys.exit(1)
    # Output dir
    outdir = join(Path.dir_2017, 'Systematics', outdir_name, syst+systdir)
    Util.CreateDirs(outdir)

    # Command template
    cmdTT = '{exe} {indir}/{{nt}}.root {outdir}/{{nt}}.root bkg'.format(
            exe=exeTT, indir=indir, outdir=outdir)
    cmdST = '{exe} {indir}/{{nt}}.root {outdir}/{{nt}}.root bkg'.format(
            exe=exeST, indir=indir, outdir=outdir)

    # Process signal MC
    for sigtype in sig_MC:
        if sigtype[1] != ch:
            continue
        for nt in sig_MC[sigtype]:
            Process( cmdTT, nt )
            Process( cmdST, nt )
    # Process resonant bkg MC
    for nt in bkg_MC['Higgs']:
        Process( cmdTT, nt )
        Process( cmdST, nt )
# End of function GenPerm


# MVAreco_apply: Apply trained MVAs for samples of given channel, systematic, and variation (Up or Down)
def MVAreco_apply(indir_name, outdir_name, ch, syst, systdir):
    if systdir!='Up' and systdir!='Down':
        print '[ERROR] Systematic variation has to be "Up" or "Down"!'
        sys.exit(1)
    if not (syst in Syst_tree_1 or syst in Syst_tree_2):
        print '[ERROR] Invalid systematic name!'
        sys.exit(1)
    # Executable
    exe = join(Path.dir_bin, 'MVAreco_application')
    # Input permutation tree dir
    indir = join(Path.dir_2017, 'Systematics', indir_name, syst+systdir)
    # TMVA weight file dir
    weidir = join(Path.dir_2017, 'MVAreco_model/dataset/weights')
    # Output dir
    outdir = join(Path.dir_2017, 'Systematics', outdir_name, syst+systdir)

    if ch!='had' and ch!='lep':
        print '[ERROR] Invalid channel'
        sys.exit(1)
    if not exists(exe):
        print '[ERROR] Executable not exist'
        sys.exit(1)
    if not exists(indir) or not exists(weidir):
        print '[ERROR] Input dir not exist'
        sys.exit(1)
    Util.CreateDirs(outdir)

    # Command template
    cmd = '%s %s/{{nt}}.root {fweight} %s/{{nt}}.root bkg {recotype} %s' % (exe, indir, outdir, ch)
    if ch == 'had':
        fweight_TT = join(weidir, 'TThad_ANN.weights.xml')
        fweight_ST = join(weidir, 'SThad_ANN.weights.xml')
    else:
        fweight_TT = join(weidir, 'TTlep_ANN.weights.xml')
        fweight_ST = join(weidir, 'STlep_ANN.weights.xml')
    cmdTT = cmd.format( fweight=fweight_TT, recotype='TT' )
    cmdST = cmd.format( fweight=fweight_ST, recotype='ST' )

    # Process signal MC
    for sigtype in sig_MC:
        if sigtype[1] != ch:
            continue
        for nt in sig_MC[sigtype]:
            Process( cmdTT, nt )
            Process( cmdST, nt )
    # Process resonant bkg MC
    for nt in bkg_MC['Higgs']:
        Process( cmdTT, nt )
        Process( cmdST, nt )
# End of function MVAreco_apply


# Reco_nuPz: Reconstruct neutrino Pz for samples of given channel, systematic, and variation (Up or Down)
def Reco_nuPz(evtdir_name, recodir_name, ch, syst, systdir):
    if systdir!='Up' and systdir!='Down':
        print '[ERROR] Systematic variation has to be "Up" or "Down"!'
        sys.exit(1)
    if not (syst in Syst_tree_1 or syst in Syst_tree_2):
        print '[ERROR] Invalid systematic name!'
        sys.exit(1)
    exe = join(Path.dir_bin, 'NuPz')
    evtdir = join(Path.dir_2017, 'Systematics', evtdir_name, syst+systdir)
    recodir = join(Path.dir_2017, 'Systematics', recodir_name, syst+systdir)
    if ch!='had' and ch!='lep':
        print '[ERROR] Invalid channel'
        sys.exit(1)
    if not exists(exe):
        print '[ERROR] Executable not exist'
        sys.exit(1)
    if not exists(evtdir) or not exists(recodir):
        print '[ERROR] Input dir not exist'
        sys.exit(1)

    # Command template
    cmdTT = '{exe} {evtdir}/{{nt}}.root {recodir}/{{nt}}.root {recotype} {ch}'.format(
            exe=exe, evtdir=evtdir, recodir=recodir, recotype='TT', ch=ch)
    cmdST = '{exe} {evtdir}/{{nt}}.root {recodir}/{{nt}}.root {recotype} {ch}'.format(
            exe=exe, evtdir=evtdir, recodir=recodir, recotype='ST', ch=ch)

    # Process signal MC
    for sigtype in sig_MC:
        if sigtype[1] != ch:
            continue
        for nt in sig_MC[sigtype]:
            Process( cmdTT, nt )
            Process( cmdST, nt )
    # Process resonant bkg MC
    for nt in bkg_MC['Higgs']:
        Process( cmdTT, nt )
        Process( cmdST, nt )
# End of function Reco_nuPz


# ProcSystMVAreco: Execute MVA reco & nu Pz reco for samples of given channel, systematic, and variation (Up or Down)
def ProcSystMVAreco(ch, syst, systdir):
    if systdir!='Up' and systdir!='Down':
        print '[ERROR] Systematic variation has to be "Up" or "Down"!'
        sys.exit(1)
    if not (syst in Syst_tree_1 or syst in Syst_tree_2):
        print '[ERROR] Invalid systematic name!'
        sys.exit(1)
    if ch == 'had':
        evtdir_name = 'Presel_had_phID_btag-L'
        permdir_name = 'MVAreco_perms_had'
        recodir_name = 'MVAreco_result_had'
    elif ch == 'lep':
        evtdir_name = 'Presel_lep_phID'
        permdir_name = 'MVAreco_perms_lep'
        recodir_name = 'MVAreco_result_lep'
    else:
        print '[ERROR] Invalid channel'
        sys.exit(1)
    evtdir = join(Path.dir_2017, 'Systematics', evtdir_name, syst+systdir)
    if not exists(evtdir):
        print '[ERROR] Input dir not exist'
        sys.exit(1)

    # Run sequence
    GenPerm(evtdir_name, permdir_name, ch, syst, systdir)
    MVAreco_apply(permdir_name, recodir_name, ch, syst, systdir)
    Reco_nuPz(evtdir_name, recodir_name, ch, syst, systdir)
# End of function ProcSystMVAreco
