#!/usr/bin/env python2
# Fill histograms for each sample.

import Path, Util, Samples
import sys, os
from os.path import join, exists


def RunFillHist(cmd, nt):
    print 'Processing:', nt
    fp = os.popen( cmd.format(nt=nt) )
    print fp.read()
    fp.close()


# Function FillHist: Fill histograms for all samples of given selection.
# Parameter "tree_name" is only used for MC processing to specify the trees containing event
# info and scale factors.  For data only an event info tree with name "T" is needed.
def FillHist(exe_name, indir_name, tree_name, outdir_name, SF_flags, ch):
    exe = join(Path.dir_bin, exe_name)
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
    cmd_temp = '{bin} {indir}/{{nt}}.root {trees} {outdir}/{{nt}}.root {SF_flags}'.format(
            bin=exe, indir=indir, trees=tree_name, outdir=outdir, SF_flags=SF_flags)
    cmd_data = '{bin} {indir}/{{nt}}.root {trees} {outdir}/{{nt}}.root {SF_flags}'.format(
            bin=exe, indir=indir, trees='T', outdir=outdir, SF_flags='0000000')

    # Process signal MC
    for sigtype in Samples.sig_MC_s:
        if sigtype[1]!=ch:
            continue
        for nt in Samples.sig_MC_s[sigtype]:
            RunFillHist(cmd_temp, nt)

    # Process bkg MC
    for cat in Samples.bkg_MC_s:
        for nt in Samples.bkg_MC_s[cat]:
            RunFillHist(cmd_temp, nt)

    # Process data
    RunFillHist(cmd_data, 'data')

    print '[INFO] Complete FillHist'
# End of function FillHist


if __name__ == '__main__':
    # Presel_had_phID_btag-L
    #exe_name = 'FillHistV2_Presel_had_phID_btag-L'
    #indir_name = 'Presel_had_phID_btag-L'
    #tree_name = 'T'
    #outdir_name = 'Presel_had_phID_btag-L__hist/hist_ori_samples'
    #SF_flags = '0000000'
    #ch = 'had'

    # Presel_had_phID_btag-L + PU + photon
    #exe_name = 'FillHistV2_Presel_had_phID_btag-L'
    #indir_name = 'Presel_had_phID_btag-L'
    #tree_name = 'T+SF_pileup'
    #outdir_name = 'Presel_had_phID_btag-L__hist/hist_PU-pho_samples'
    #SF_flags = '0100001'
    #ch = 'had'

    # Presel_had_phID_btag-L + PU + photon + btag
    #exe_name = 'FillHistV2_Presel_had_phID_btag-L'
    #indir_name = 'Presel_had_phID_btag-L'
    #tree_name = 'T+SF_pileup+SF_btag'
    #outdir_name = 'Presel_had_phID_btag-L__hist/hist_PU-pho-btag_samples'
    #SF_flags = '1100001'
    #ch = 'had'

    # Presel_lep_phID
    #exe_name = 'FillHistV2_Presel_lep_phID'
    #indir_name = 'Presel_lep_phID'
    #tree_name = 'T'
    #outdir_name = 'Presel_lep_phID__hist/hist_ori_samples'
    #SF_flags = '0000000'
    #ch = 'lep'

    # Presel_lep_phID + PU + eID + eReco + muID + muISO + photon
    #exe_name = 'FillHistV2_Presel_lep_phID'
    #indir_name = 'Presel_lep_phID'
    #tree_name = 'T+SF_pileup+SF_Elec+SF_Muon'
    #outdir_name = 'Presel_lep_phID__hist/hist_PU-e-mu-pho_samples'
    #SF_flags = '0111111'
    #ch = 'lep'

    # Presel_lep_phID + PU + eID + eReco + muID + muISO + photon + btag
    exe_name = 'FillHistV2_Presel_lep_phID'
    indir_name = 'Presel_lep_phID'
    tree_name = 'T+SF_pileup+SF_Elec+SF_Muon+SF_btag'
    outdir_name = 'Presel_lep_phID__hist/hist_PU-e-mu-pho-btag_samples'
    SF_flags = '1111111'
    ch = 'lep'

    # Input by command line
    #exe_name = sys.argv[1]
    #indir_name = sys.argv[2]
    #tree_name = sys.argv[3]
    #outdir_name = sys.argv[4]
    #SF_flags = sys.argv[5]
    #ch = sys.argv[6]

    # Run
    FillHist(exe_name, indir_name, tree_name, outdir_name, SF_flags, ch)
    print 'Complete!'
# End of main function


'''
To run:
./FillHist_Presel_had_phID_btag-L <sample.root,SF.root,...> <T1a+T1b+...,T2a+T2b+...,...> <output.root> <weight_expression>

Samples:
1. hadronic signal MC in had channel
2. bkg MC & data in had channel
'''
