#!/usr/bin/env python2

import Path, Util
import os, sys
from os.path import join, exists


def MakeDatacard(coupling):
    indir = join(Path.dir_2017, 'Datacard_syst')

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
