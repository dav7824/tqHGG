#!/usr/bin/env python2

import Path, Util
from Samples import sig_MC, bkg_MC
import os, sys
from os.path import join, exists


# Executable for each type of signal MC
dict_normal = {
        ('TT', 'had'): 'MVAreco_GenPerm_TThad',
        ('ST', 'had'): 'MVAreco_GenPerm_SThad',
        ('TT', 'lep'): 'MVAreco_GenPerm_TTlep',
        ('ST', 'lep'): 'MVAreco_GenPerm_STlep',
        }
# Executable for each type of signal MC, but wrong type
dict_false = {
        ('TT', 'had'): 'MVAreco_GenPerm_SThad',
        ('ST', 'had'): 'MVAreco_GenPerm_TThad',
        ('TT', 'lep'): 'MVAreco_GenPerm_STlep',
        ('ST', 'lep'): 'MVAreco_GenPerm_TTlep',
        }


def Process(cmd, nt):
    print '---Start processing: {}'.format(nt)
    fp = os.popen( cmd.format(nt=nt) )
    print fp.read()
    fp.close()


def GenPerm_sig_train(indir_name, outdir_name, type, ch):
    print '---Start running: GenPerm_sig_train'
    print '---Input dir: {}'.format(indir_name)
    print '---Output dir: {}'.format(outdir_name)
    print '---Signal type/channel: {}/{}'.format(type, ch)
    if type!='TT' and type!='ST':
        print '[ERROR] Invalid signal type'
        return
    if ch!='had' and ch!='lep':
        print '[ERROR] Invalid channel'
        return
    # Set paths
    exe_name = dict_normal[(type, ch)]
    exe = join(Path.dir_bin, exe_name)
    indir = join(Path.dir_2017, indir_name)
    outdir = join(Path.dir_2017, outdir_name)
    if not exists(exe):
        print '[ERROR] Executable not exist'
        return
    if not exists(indir):
        print '[ERROR] Input dir not exist'
        return
    Util.CreateDirs(outdir)

    # Command template
    cmd = '{exe} {indir}/{{nt}}.root {outdir}/{{nt}}.root {mode}'.format(
            exe=exe, indir=indir, outdir=outdir, mode='sig_train')

    # Process signal MC
    for sigtype in sig_MC:
        if sigtype[0]!=type or sigtype[1]!=ch:
            continue
        for nt in sig_MC[sigtype]:
            Process(cmd, nt)

    print '---Complete!\n\n\n'
# End of function GenPerm_sig_train


def GenPerm_sig_test(indir_name, outdir_name, type, ch):
    print '---Start running: GenPerm_sig_test'
    print '---Input dir: {}'.format(indir_name)
    print '---Output dir: {}'.format(outdir_name)
    print '---Signal type/channel: {}/{}'.format(type, ch)
    if type!='TT' and type!='ST':
        print '[ERROR] Invalid signal type'
        return
    if ch!='had' and ch!='lep':
        print '[ERROR] Invalid channel'
        return
    # Set paths
    exe_name = dict_normal[(type, ch)]
    exe = join(Path.dir_bin, exe_name)
    indir = join(Path.dir_2017, indir_name)
    outdir = join(Path.dir_2017, outdir_name)
    if not exists(exe):
        print '[ERROR] Executable not exist'
        return
    if not exists(indir):
        print '[ERROR] Input dir not exist'
        return
    Util.CreateDirs(outdir)

    # Command template
    cmd = '{exe} {indir}/{{nt}}.root {outdir}/{{nt}}.root {mode}'.format(
            exe=exe, indir=indir, outdir=outdir, mode='sig_test')

    # Process signal MC
    for sigtype in sig_MC:
        if sigtype[0]!=type or sigtype[1]!=ch:
            continue
        for nt in sig_MC[sigtype]:
            Process(cmd, nt)

    print '---Complete!\n\n\n'
# End of function GenPerm_sig_test


def GenPerm_sig_false(indir_name, outdir_name, type, ch):
    print '---Start running: GenPerm_sig_false'
    print '---Input dir: {}'.format(indir_name)
    print '---Output dir: {}'.format(outdir_name)
    print '---Signal type/channel: {}/{}'.format(type, ch)
    if type!='TT' and type!='ST':
        print '[ERROR] Invalid signal type'
        return
    if ch!='had' and ch!='lep':
        print '[ERROR] Invalid channel'
        return
    # Set paths
    exe_name = dict_false[(type, ch)]
    exe = join(Path.dir_bin, exe_name)
    indir = join(Path.dir_2017, indir_name)
    outdir = join(Path.dir_2017, outdir_name)
    if not exists(exe):
        print '[ERROR] Executable not exist'
        return
    if not exists(indir):
        print '[ERROR] Input dir not exist'
        return
    Util.CreateDirs(outdir)

    # Command template
    cmd = '{exe} {indir}/{{nt}}.root {outdir}/{{nt}}.root {mode}'.format(
            exe=exe, indir=indir, outdir=outdir, mode='bkg')

    # Process signal MC
    for sigtype in sig_MC:
        if sigtype[0]!=type or sigtype[1]!=ch:
            continue
        for nt in sig_MC[sigtype]:
            Process(cmd, nt)

    print '---Complete!\n\n\n'
# End of function GenPerm_sig_false


#def GenPerm_bkg(indir_name, outdir_name, type, ch):
# End of function GenPerm_bkg


if __name__ == '__main__':
    ### Hadronic signal
    ## Input for MVA training
    #GenPerm_sig_train('GenPermInput_had/train_set', 'MVAreco_perm_had/Train_input', 'TT', 'had')
    #GenPerm_sig_train('GenPermInput_had/train_set', 'MVAreco_perm_had/Train_input', 'ST', 'had')
    ## Training set
    #GenPerm_sig_test('GenPermInput_had/train_set', 'MVAreco_perm_had/train_set', 'TT', 'had')
    #GenPerm_sig_test('GenPermInput_had/train_set', 'MVAreco_perm_had/train_set', 'ST', 'had')
    #GenPerm_sig_false('GenPermInput_had/train_set', 'MVAreco_perm_had/train_set', 'TT', 'had')
    #GenPerm_sig_false('GenPermInput_had/train_set', 'MVAreco_perm_had/train_set', 'ST', 'had')
    ## Optimization set
    #GenPerm_sig_test('GenPermInput_had/opt_set', 'MVAreco_perm_had/opt_set', 'TT', 'had')
    #GenPerm_sig_test('GenPermInput_had/opt_set', 'MVAreco_perm_had/opt_set', 'ST', 'had')
    #GenPerm_sig_false('GenPermInput_had/opt_set', 'MVAreco_perm_had/opt_set', 'TT', 'had')
    #GenPerm_sig_false('GenPermInput_had/opt_set', 'MVAreco_perm_had/opt_set', 'ST', 'had')
    ## Fitting set
    #GenPerm_sig_test('GenPermInput_had/fit_set', 'MVAreco_perm_had/fit_set', 'TT', 'had')
    #GenPerm_sig_test('GenPermInput_had/fit_set', 'MVAreco_perm_had/fit_set', 'ST', 'had')
    #GenPerm_sig_false('GenPermInput_had/fit_set', 'MVAreco_perm_had/fit_set', 'TT', 'had')
    #GenPerm_sig_false('GenPermInput_had/fit_set', 'MVAreco_perm_had/fit_set', 'ST', 'had')

    ## Leptonic signal
    # Input for MVA training
    GenPerm_sig_train('GenPermInput_lep/train_set', 'MVAreco_perm_lep/Train_input', 'TT', 'lep')
    GenPerm_sig_train('GenPermInput_lep/train_set', 'MVAreco_perm_lep/Train_input', 'ST', 'lep')
    # Training set
    GenPerm_sig_test('GenPermInput_lep/train_set', 'MVAreco_perm_lep/train_set', 'TT', 'lep')
    GenPerm_sig_test('GenPermInput_lep/train_set', 'MVAreco_perm_lep/train_set', 'ST', 'lep')
    GenPerm_sig_false('GenPermInput_lep/train_set', 'MVAreco_perm_lep/train_set', 'TT', 'lep')
    GenPerm_sig_false('GenPermInput_lep/train_set', 'MVAreco_perm_lep/train_set', 'ST', 'lep')
    # Optimization set
    GenPerm_sig_test('GenPermInput_lep/opt_set', 'MVAreco_perm_lep/opt_set', 'TT', 'lep')
    GenPerm_sig_test('GenPermInput_lep/opt_set', 'MVAreco_perm_lep/opt_set', 'ST', 'lep')
    GenPerm_sig_false('GenPermInput_lep/opt_set', 'MVAreco_perm_lep/opt_set', 'TT', 'lep')
    GenPerm_sig_false('GenPermInput_lep/opt_set', 'MVAreco_perm_lep/opt_set', 'ST', 'lep')
    # Fitting set
    GenPerm_sig_test('GenPermInput_lep/fit_set', 'MVAreco_perm_lep/fit_set', 'TT', 'lep')
    GenPerm_sig_test('GenPermInput_lep/fit_set', 'MVAreco_perm_lep/fit_set', 'ST', 'lep')
    GenPerm_sig_false('GenPermInput_lep/fit_set', 'MVAreco_perm_lep/fit_set', 'TT', 'lep')
    GenPerm_sig_false('GenPermInput_lep/fit_set', 'MVAreco_perm_lep/fit_set', 'ST', 'lep')
