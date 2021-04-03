#!/usr/bin/env python2

import Path, Util
from Samples import sig_MC, bkg_MC
from SystUnc import *
import os, sys
from os.path import join, exists


# ProcSig: Process TT & ST signals for given systematic, coupling, and channel
def ProcSig(indir_name, coupling, ch, syst_name, fout_name):
    print '------Processing signal:', coupling
    exe = join(Path.dir_bin, 'CutBDT_syst_signal')
    indir = join(Path.dir_2017, indir_name)
    outdir = join(Path.dir_2017, 'Datacard_syst_weight')

    if not (syst_name in Syst_weight_1 or syst_name in Syst_weight_2 or syst_name in Syst_weight_3):
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

    # If the systematic uncertainty only affects leptonic channel, fill null entry for hadronic channel
    if ch == 'had' and syst_name in Syst_weight_2:
        fout = open( join(outdir,fout_name), 'a' )
        fout.write( '{:>25s}  lnN  {:^11s}  {:^11s}\n'.format(syst_name, '-', '-') )
        fout.close()
        return

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
    print '------Processing resonant background'
    exe = join(Path.dir_bin, 'CutBDT_syst_resbkg')
    indir = join(Path.dir_2017, indir_name)
    outdir = join(Path.dir_2017, 'Datacard_syst_weight')

    if not (syst_name in Syst_weight_1 or syst_name in Syst_weight_2 or syst_name in Syst_weight_3):
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

    # If the systematic uncertainty only affects leptonic channel, fill null entry for hadronic channel
    if ch == 'had' and syst_name in Syst_weight_2:
        fout = open( join(outdir,fout_name), 'a' )
        fout.write( '{:>25s}  lnN  {:^11s}\n'.format(syst_name, '-') )
        fout.close()
        return

    # Command template
    cmd = '{exe} {indir} {ch} {syst_name} {fout}'.format(
            exe=exe, indir=indir, ch=ch,
            syst_name=syst_name, fout=join(outdir,fout_name))

    # Run
    fp = os.popen(cmd)
    fp.close()
# End of function ProcRbkg


# ProcSystEntry: Process all signals & resonant bkgs for given systematic and channel
def ProcSystEntry(indir_name, ch, syst_name):
    if not (syst_name in Syst_weight_1 or syst_name in Syst_weight_2 or syst_name in Syst_weight_3):
        print '[ERROR] Invalid systematic type'
        sys.exit(1)
    if ch!='had' and ch!='lep':
        print '[ERROR] Invalid channel'
        sys.exit(1)

    print '---Processing systematic:', syst_name

    # Process signal MCs
    for coupling in ['hut', 'hct']:
        fout_name = 'sig_{}_{}.txt'.format(coupling, ch)
        ProcSig( indir_name, coupling, ch, syst_name, fout_name )
    # Process resonant bkg MCs
    fout_name = 'resbkg_{}.txt'.format(ch)
    ProcRbkg( indir_name, ch, syst_name, fout_name )
# End of function ProcSystEntry


# ProcChannel: Process all systematics for given channel
def ProcChannel(indir_name, ch):
    if ch!='had' and ch!='lep':
        print '[ERROR] Invalid channel'
        sys.exit(1)

    print '---Channel:', ch

    for syst_name in Syst_weight_1+Syst_weight_2+Syst_weight_3:
        ProcSystEntry(indir_name, ch, syst_name)
# End of function ProcChannel


def MakeDatacard(coupling):
    indir = join(Path.dir_2017, 'Datacard_syst_weight')

    if coupling!='hut' and coupling!='hct':
        print '[ERROR] Invalid coupling'
        sys.exit(1)

    # File paths
    # Input files
    fsig_had_name = join(indir, 'sig_{}_{}.txt'.format(coupling,'had'))
    fsig_lep_name = join(indir, 'sig_{}_{}.txt'.format(coupling,'lep'))
    fresbkg_had_name = join(indir, 'resbkg_{}.txt'.format('had'))
    fresbkg_lep_name = join(indir, 'resbkg_{}.txt'.format('lep'))
    # Output files
    fout_had_name = '{}/Final_{}_had.txt'.format(indir, coupling)
    fout_lep_name = '{}/Final_{}_lep.txt'.format(indir, coupling)
    fout_comb_name = '{}/Final_{}_comb.txt'.format(indir, coupling)

    # Read / write files
    fsig_had = open(fsig_had_name, 'r')
    fsig_lep = open(fsig_lep_name, 'r')
    fresbkg_had = open(fresbkg_had_name, 'r')
    fresbkg_lep = open(fresbkg_lep_name, 'r')
    fout_had = open(fout_had_name, 'w')
    fout_lep = open(fout_lep_name, 'w')
    fout_comb = open(fout_comb_name, 'w')

    for lsig_had in fsig_had:
        lsig_had_ = lsig_had.split()
        lsig_lep_ = fsig_lep.readline().split()
        lresbkg_had_ = fresbkg_had.readline().split()
        lresbkg_lep_ = fresbkg_lep.readline().split()

        fout_had.write( '{:>25s}  lnN  {:^11s}  {:^11s}  {:^11s}  {:^11s}\n'.format(
                lsig_had_[0], lsig_had_[2], lsig_had_[3], lresbkg_had_[2], '-'))
        fout_lep.write( '{:>25s}  lnN  {:^11s}  {:^11s}  {:^11s}  {:^11s}\n'.format(
                lsig_lep_[0], lsig_lep_[2], lsig_lep_[3], lresbkg_lep_[2], '-'))
        fout_comb.write( '{:>25s}  lnN  {:^11s}  {:^11s}  {:^11s}  {:^11s}  {:^11s}  {:^11s}  {:^11s}  {:^11s}\n'.format(
                lsig_had_[0], lsig_had_[2], lsig_had_[3], lresbkg_had_[2], '-',
                lsig_lep_[2], lsig_lep_[3], lresbkg_lep_[2], '-'))

    fsig_had.close()
    fsig_lep.close()
    fresbkg_had.close()
    fresbkg_lep.close()
    fout_had.close()
    fout_lep.close()
    fout_comb.close()
# End of function MakeDatacard


if __name__ == '__main__':
    #ProcChannel('BDT_input_had', 'had')
    #print '\n'
    #ProcChannel('BDT_input_lep', 'lep')

    #MakeDatacard('hut')
    MakeDatacard('hct')
