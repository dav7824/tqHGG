#!/usr/bin/env python2

import Path, Util, Samples
import os, sys
from os.path import join, exists
from Samples import sig_MC_s, bkg_MC_s


def Process(cmd, nt):
    print 'Processing:', nt
    fp = os.popen(cmd.format(nt=nt))
    print fp.read()
    fp.close()


# Divide non-resonant bkg MC as 2 parts
def Divide_nonres_bkg(indir_name, tree, outdir1_name, outdir2_name, ch):
    print 'Start dividing non-resonant bkg tree:', tree
    exe = join(Path.dir_bin, 'DivideSamples_v1')
    indir = join(Path.dir_2017, indir_name)
    outdir1 = join(Path.dir_2017, outdir1_name)
    outdir2 = join(Path.dir_2017, outdir2_name)

    if not exists(exe):
        print '[ERROR] Executable does not exist'
        sys.exit(1)
    if not exists(indir):
        print '[ERROR] Input dir does not exist'
        sys.exit(1)
    if ch != 'had' and ch != 'lep':
        print '[ERROR] Invalid channel!'
        sys.exit(1)
    Util.CreateDir(outdir1)
    Util.CreateDir(outdir2)

    # Command template
    cmd = '{bin} {indir}/{{nt}}.root {tree} {outdir1}/{{nt}}.root {outdir2}/{{nt}}.root'.format(
            bin=exe, indir=indir, tree=tree, outdir1=outdir1, outdir2=outdir2)

    # Process non-resonant bkg MC
    for cat in bkg_MC_s:
        if cat == 'Higgs':
            continue
        for nt in bkg_MC_s[cat]:
            # Skip empty tree
            if ch == 'lep' and nt == 'QCD_Pt-30to40_MGG-80toInf':
                continue
            Process(cmd, nt)

    print 'Complete dividing non-resonant bkg tree:', tree
# End of function Divide_nonres_bkg


# Divide resonant bkg MC as 3 parts
def Divide_res_bkg(indir_name, tree, outdir1_name, outdir2_name, outdir3_name, ch):
    print 'Start dividing resonant bkg tree:', tree
    exe = join(Path.dir_bin, 'DivideSamples_v2')
    indir = join(Path.dir_2017, indir_name)
    outdir1 = join(Path.dir_2017, outdir1_name)
    outdir2 = join(Path.dir_2017, outdir2_name)
    outdir3 = join(Path.dir_2017, outdir3_name)

    if not exists(exe):
        print '[ERROR] Executable does not exist'
        sys.exit(1)
    if not exists(indir):
        print '[ERROR] Input dir does not exist'
        sys.exit(1)
    if ch != 'had' and ch != 'lep':
        print '[ERROR] Invalid channel!'
        sys.exit(1)
    Util.CreateDir(outdir1)
    Util.CreateDir(outdir2)
    Util.CreateDir(outdir3)

    # Command template
    cmd = '{bin} {indir}/{{nt}}.root {tree} {outdir1}/{{nt}}.root {outdir2}/{{nt}}.root {outdir3}/{{nt}}.root'.format(
            bin=exe, indir=indir, tree=tree, outdir1=outdir1, outdir2=outdir2, outdir3=outdir3)

    # Process resonant bkg MC
    for nt in bkg_MC_s['Higgs']:
        Process(cmd, nt)

    print 'Complete dividing resonant bkg tree:', tree
# End of function Divide_res_bkg


# Divide signal MC as 3 parts
def Divide_sig(indir_name, tree, outdir1_name, outdir2_name, outdir3_name, ch):
    print 'Start dividing signal tree:', tree
    exe = join(Path.dir_bin, 'DivideSamples_v2')
    indir = join(Path.dir_2017, indir_name)
    outdir1 = join(Path.dir_2017, outdir1_name)
    outdir2 = join(Path.dir_2017, outdir2_name)
    outdir3 = join(Path.dir_2017, outdir3_name)

    if not exists(exe):
        print '[ERROR] Executable does not exist'
        sys.exit(1)
    if not exists(indir):
        print '[ERROR] Input dir does not exist'
        sys.exit(1)
    if ch != 'had' and ch != 'lep':
        print '[ERROR] Invalid channel!'
        sys.exit(1)
    Util.CreateDir(outdir1)
    Util.CreateDir(outdir2)
    Util.CreateDir(outdir3)

    # Command template
    cmd = '{bin} {indir}/{{nt}}.root {tree} {outdir1}/{{nt}}.root {outdir2}/{{nt}}.root {outdir3}/{{nt}}.root'.format(
            bin=exe, indir=indir, tree=tree, outdir1=outdir1, outdir2=outdir2, outdir3=outdir3)

    # Process signal MC
    for sigtype in sig_MC_s:
        if sigtype[1] != ch:
            continue
        for nt in sig_MC_s[sigtype]:
            Process(cmd, nt)

    print 'Complete dividing signal tree:', tree
# End of function Divide_sig


if __name__ == '__main__':
    # had signal
    #indir_name = 'Presel_had_phID_btag-L__MVAreco-test'
    #indir2_name = 'MVArecoV3_result_had/ANN_opt'
    #outdir1_name = 'BDT_had/train'
    #outdir2_name = 'optimization_had'
    #outdir3_name = 'model_had'
    #ch = 'had'
    #Divide_sig(indir_name, 'T', outdir1_name, outdir2_name, outdir3_name, ch)
    #Divide_sig(indir_name, 'SF_pileup', outdir1_name, outdir2_name, outdir3_name, ch)
    #Divide_sig(indir_name, 'SF_btag', outdir1_name, outdir2_name, outdir3_name, ch)
    #Divide_sig(indir2_name, 'Treco_TT', outdir1_name, outdir2_name, outdir3_name, ch)
    #Divide_sig(indir2_name, 'Treco_ST', outdir1_name, outdir2_name, outdir3_name, ch)

    # lep signal
    #indir_name = 'Presel_lep_phID__MVAreco-test'
    #indir2_name = 'MVArecoV3_result_lep/ANN_opt'
    #outdir1_name = 'BDT_lep/train'
    #outdir2_name = 'optimization_lep'
    #outdir3_name = 'model_lep'
    #ch = 'lep'
    #Divide_sig(indir_name, 'T', outdir1_name, outdir2_name, outdir3_name, ch)
    #Divide_sig(indir_name, 'SF_pileup', outdir1_name, outdir2_name, outdir3_name, ch)
    #Divide_sig(indir_name, 'SF_btag', outdir1_name, outdir2_name, outdir3_name, ch)
    #Divide_sig(indir_name, 'SF_Elec', outdir1_name, outdir2_name, outdir3_name, ch)
    #Divide_sig(indir_name, 'SF_Muon', outdir1_name, outdir2_name, outdir3_name, ch)
    #Divide_sig(indir2_name, 'Treco_TT', outdir1_name, outdir2_name, outdir3_name, ch)
    #Divide_sig(indir2_name, 'Treco_ST', outdir1_name, outdir2_name, outdir3_name, ch)

    # had res bkg
    #indir_name = 'Presel_had_phID_btag-L'
    #indir2_name = 'MVArecoV3_result_had/ANN_opt'
    #outdir1_name = 'BDT_had/train'
    #outdir2_name = 'optimization_had'
    #outdir3_name = 'model_had'
    #ch = 'had'
    #Divide_res_bkg(indir_name, 'T', outdir1_name, outdir2_name, outdir3_name, ch)
    #Divide_res_bkg(indir_name, 'SF_pileup', outdir1_name, outdir2_name, outdir3_name, ch)
    #Divide_res_bkg(indir_name, 'SF_btag', outdir1_name, outdir2_name, outdir3_name, ch)
    #Divide_res_bkg(indir2_name, 'Treco_TT', outdir1_name, outdir2_name, outdir3_name, ch)
    #Divide_res_bkg(indir2_name, 'Treco_ST', outdir1_name, outdir2_name, outdir3_name, ch)

    # lep res bkg
    #indir_name = 'Presel_lep_phID'
    #indir2_name = 'MVArecoV3_result_lep/ANN_opt'
    #outdir1_name = 'BDT_lep/train'
    #outdir2_name = 'optimization_lep'
    #outdir3_name = 'model_lep'
    #ch = 'lep'
    #Divide_res_bkg(indir_name, 'T', outdir1_name, outdir2_name, outdir3_name, ch)
    #Divide_res_bkg(indir_name, 'SF_pileup', outdir1_name, outdir2_name, outdir3_name, ch)
    #Divide_res_bkg(indir_name, 'SF_btag', outdir1_name, outdir2_name, outdir3_name, ch)
    #Divide_res_bkg(indir_name, 'SF_Elec', outdir1_name, outdir2_name, outdir3_name, ch)
    #Divide_res_bkg(indir_name, 'SF_Muon', outdir1_name, outdir2_name, outdir3_name, ch)
    #Divide_res_bkg(indir2_name, 'Treco_TT', outdir1_name, outdir2_name, outdir3_name, ch)
    #Divide_res_bkg(indir2_name, 'Treco_ST', outdir1_name, outdir2_name, outdir3_name, ch)

    # had non-res bkg
    #indir_name = 'Presel_had_phID_btag-L'
    #indir2_name = 'MVArecoV3_result_had/ANN_opt'
    #outdir1_name = 'BDT_had/train'
    #outdir2_name = 'BDT_had/test'
    #ch = 'had'
    #Divide_nonres_bkg(indir_name, 'T', outdir1_name, outdir2_name, ch)
    #Divide_nonres_bkg(indir_name, 'SF_pileup', outdir1_name, outdir2_name, ch)
    #Divide_nonres_bkg(indir_name, 'SF_btag', outdir1_name, outdir2_name, ch)
    #Divide_nonres_bkg(indir2_name, 'Treco_TT', outdir1_name, outdir2_name, ch)
    #Divide_nonres_bkg(indir2_name, 'Treco_ST', outdir1_name, outdir2_name, ch)

    # lep non-res bkg
    indir_name = 'Presel_lep_phID'
    indir2_name = 'MVArecoV3_result_lep/ANN_opt'
    outdir1_name = 'BDT_lep/train'
    outdir2_name = 'BDT_lep/test'
    ch = 'lep'
    Divide_nonres_bkg(indir_name, 'T', outdir1_name, outdir2_name, ch)
    Divide_nonres_bkg(indir_name, 'SF_pileup', outdir1_name, outdir2_name, ch)
    Divide_nonres_bkg(indir_name, 'SF_btag', outdir1_name, outdir2_name, ch)
    Divide_nonres_bkg(indir_name, 'SF_Elec', outdir1_name, outdir2_name, ch)
    Divide_nonres_bkg(indir_name, 'SF_Muon', outdir1_name, outdir2_name, ch)
    Divide_nonres_bkg(indir2_name, 'Treco_TT', outdir1_name, outdir2_name, ch)
    Divide_nonres_bkg(indir2_name, 'Treco_ST', outdir1_name, outdir2_name, ch)
