#!/usr/bin/env python2

import Path, Util
import os, sys
from os.path import join, exists
from Samples import sig_MC, bkg_MC

# For the following samples & data-driven QCD (derived from data), run DivideSamples_v4.
# For the other samples, run DivideSamples_v3.
sample_evtidx = [
        'GJet_Pt-20to40_MGG-80toInf',
        'QCD_Pt-30to40_MGG-80toInf',
        'QCD_Pt-30toInf_MGG-40to80',
        'QCD_Pt-40toInf_MGG-80toInf',
        ]


def Process(cmd, nt):
    print 'Processing:', nt
    fp = os.popen(cmd.format(nt=nt))
    print fp.read()
    fp.close()


def DivideSig(indir_name, tree_list, outdir_name_list, ch):
    # Set paths
    exe = join(Path.dir_bin, 'DivideSamples_v3')
    indir = join(Path.dir_2017, indir_name)
    if not exists(exe):
        print '[ERROR] Executable does not exist'
        return
    if not exists(indir):
        print '[ERROR] Input dir does not exist'
        return
    if ch != 'had' and ch != 'lep':
        print '[ERROR] Invalid channel!'
        return

    outdir_list = []
    fout_list = []
    for i in outdir_name_list:
        outdir_list.append( join(Path.dir_2017,i) )
    for i in outdir_list:
        Util.CreateDirs(i)
        fout_list.append( join(i,'{nt}.root') )

    # Command template
    cmd = '{bin} {indir}/{{nt}}.root {tree_list} {fout_list}'.format(
            bin=exe, indir=indir, tree_list=','.join(tree_list), fout_list=','.join(fout_list) )

    # Process signal MC
    for sigtype in sig_MC:
        if sigtype[1] != ch:
            continue
        for nt in sig_MC[sigtype]:
            Process(cmd, nt)
# End of function DivideSig


def DivideResBkg(indir_name, tree_list, outdir_name_list, ch):
    # Set paths
    exe = join(Path.dir_bin, 'DivideSamples_v3')
    indir = join(Path.dir_2017, indir_name)
    if not exists(exe):
        print '[ERROR] Executable does not exist'
        return
    if not exists(indir):
        print '[ERROR] Input dir does not exist'
        return
    if ch != 'had' and ch != 'lep':
        print '[ERROR] Invalid channel!'
        return

    outdir_list = []
    fout_list = []
    for i in outdir_name_list:
        outdir_list.append( join(Path.dir_2017,i) )
    for i in outdir_list:
        Util.CreateDirs(i)
        fout_list.append( join(i,'{nt}.root') )

    # Command template
    cmd = '{bin} {indir}/{{nt}}.root {tree_list} {fout_list}'.format(
            bin=exe, indir=indir, tree_list=','.join(tree_list), fout_list=','.join(fout_list) )

    # Process resonant bkg MC
    for nt in bkg_MC['Higgs']:
        Process(cmd, nt)
# End of function DivideResBkg


def DivideNonres(indir_name, tree_list, outdir_name_list, ch):
    # Set paths
    exe1 = join(Path.dir_bin, 'DivideSamples_v3')
    exe2 = join(Path.dir_bin, 'DivideSamples_v4')
    indir = join(Path.dir_2017, indir_name)
    if not exists(exe1) or not exists(exe2):
        print '[ERROR] Executable does not exist'
        return
    if not exists(indir):
        print '[ERROR] Input dir does not exist'
        return
    if ch != 'had' and ch != 'lep':
        print '[ERROR] Invalid channel!'
        return

    outdir_list = []
    fout_list = []
    for i in outdir_name_list:
        outdir_list.append( join(Path.dir_2017,i) )
    for i in outdir_list:
        Util.CreateDirs(i)
        fout_list.append( join(i,'{nt}.root') )

    # Command template
    cmd1 = '{bin} {indir}/{{nt}}.root {tree_list} {fout_list}'.format(
            bin=exe1, indir=indir, tree_list=','.join(tree_list), fout_list=','.join(fout_list) )
    cmd2 = '{bin} {indir}/{{nt}}.root {tree_list} {fout_list}'.format(
            bin=exe2, indir=indir, tree_list=','.join(tree_list), fout_list=','.join(fout_list) )

    # Process non-resonant bkg MC
    for cat in bkg_MC:
        if cat == 'Higgs':
            continue
        for nt in bkg_MC[cat]:
            if ch=='lep' and nt=='QCD_Pt-30to40_MGG-80toInf':
                continue
            if nt in sample_evtidx:
                Process(cmd2, nt)
            else:
                Process(cmd1, nt)
    # Process data-driven QCD samples
    if ch == 'had':
        Process(cmd1, 'DiPhotonJetsBox_scaled')  # scaled DiPhotonJetsBox
        Process(cmd2, 'Data-driven_QCD')  # data-driven QCD
# End of function DivideNonres


# Scale event weight after dividing samples
# Note: For leptonic channel, 'QCD_Pt-30toInf_MGG-40to80' sample is not used anymore after
# dividing samples for too few events. You can remove them manually.

def ScaleWeight(indir_name, ch):
    exe = join(Path.dir_bin, 'ScaleWeight')
    indir = join(Path.dir_2017, indir_name)
    if ch!='had' and ch!='lep':
        print '[ERROR] Invalid channel'
        return
    if not exists(exe):
        print '[ERROR] Executable not exist'
        return
    if not exists(indir):
        print '[ERROR] Input dir not exist'
        return

    # Command template (process each divided set)
    # signal & resonant bkg
    cmd_3p = '''{exe} {indir}/train_set/{{nt}}.root {SF} {syst};
    {exe} {indir}/opt_set/{{nt}}.root {SF} {syst};
    {exe} {indir}/fit_set/{{nt}}.root {SF} {syst}'''.format(
            exe=exe, indir=indir, SF='3', syst='syst')
    # non-resonant bkg
    cmd_2p = '''{exe} {indir}/train_set/{{nt}}.root {SF} {syst};
    {exe} {indir}/opt_set/{{nt}}.root {SF} {syst}'''.format(
            exe=exe, indir=indir, SF='2', syst='nosyst')

    # Process signal
    for sigtype in sig_MC:
        if ch != sigtype[1]:
            continue
        for nt in sig_MC[sigtype]:
            Process(cmd_3p, nt)
    # Process resonant bkg
    for nt in bkg_MC['Higgs']:
        Process(cmd_3p, nt)
    # Process non-resonant bkg
    for cat in bkg_MC:
        if cat == 'Higgs':
            continue
        for nt in bkg_MC[cat]:
            if ch=='lep' and (nt=='QCD_Pt-30to40_MGG-80toInf' or nt=='QCD_Pt-30toInf_MGG-40to80'):
                continue
            Process(cmd_2p, nt)
    # Process data-driven QCD samples
    if ch == 'had':
        Process(cmd_2p, 'DiPhotonJetsBox_scaled')
        Process(cmd_2p, 'Data-driven_QCD')
# End of function ScaleWeight


if __name__ == '__main__':
    ## hadronic channel
    ## hadronic signal
    #DivideSig(
            #indir_name = 'Presel_had_phID_btag-L',
            #tree_list = ['T', 'TGenPart'],
            #outdir_name_list = [
                #'Presel_had_phID_btag-L/train_set',
                #'Presel_had_phID_btag-L/opt_set',
                #'Presel_had_phID_btag-L/fit_set',
                #],
            #ch = 'had')
    ## hadronic res-bkg
    #DivideResBkg(
            #indir_name = 'Presel_had_phID_btag-L',
            #tree_list = ['T'],
            #outdir_name_list = [
                #'Presel_had_phID_btag-L/train_set',
                #'Presel_had_phID_btag-L/opt_set',
                #'Presel_had_phID_btag-L/fit_set',
                #],
            #ch = 'had')
    ## hadronic non-res bkg
    #DivideNonres(
            #indir_name = 'Presel_had_phID_btag-L',
            #tree_list = ['T'],
            #outdir_name_list = [
                #'Presel_had_phID_btag-L/train_set',
                #'Presel_had_phID_btag-L/opt_set',
                #],
            #ch = 'had')

    ## leptonic channel
    ## leptonic signal
    #DivideSig(
            #indir_name = 'Presel_lep_phID',
            #tree_list = ['T', 'TGenPart'],
            #outdir_name_list = [
                #'Presel_lep_phID/train_set',
                #'Presel_lep_phID/opt_set',
                #'Presel_lep_phID/fit_set',
                #],
            #ch = 'lep')
    ## leptonic res-bkg
    #DivideResBkg(
            #indir_name = 'Presel_lep_phID',
            #tree_list = ['T'],
            #outdir_name_list = [
                #'Presel_lep_phID/train_set',
                #'Presel_lep_phID/opt_set',
                #'Presel_lep_phID/fit_set',
                #],
            #ch = 'lep')
    ## leptonic non-res bkg
    #DivideNonres(
            #indir_name = 'Presel_lep_phID',
            #tree_list = ['T'],
            #outdir_name_list = [
                #'Presel_lep_phID/train_set',
                #'Presel_lep_phID/opt_set',
                #],
            #ch = 'lep')


    # Scale event weights
    # hadronic channel
    #ScaleWeight('Presel_had_phID_btag-L', 'had')
    # leptonic channel
    ScaleWeight('Presel_lep_phID', 'lep')
