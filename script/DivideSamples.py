#!/usr/bin/env python2

import Path, Util
import os, sys
from os.path import join, exists
from Samples import sig_MC, bkg_MC


def Process(cmd, nt):
    print 'Processing:', nt
    fp = os.popen(cmd.format(nt=nt))
    print fp.read()
    fp.close()


# Divide signal MC trees
def Divide_sig(indir_name, tree_list, outdir_name_list, ch):
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

    print '---Start processing:'
    print '---Input dir: {}'.format(indir_name)
    print '---Output dirs:'
    for i in outdir_name_list:
        print '\t{}'.format(i)
    print '---Trees:'
    for i in tree_list:
        print '\t{}'.format(i)
    print '---Channel: {}'.format(ch)
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
# End of function Divide_sig


if __name__ == '__main__':
    # had signal
    #indir_name = 'FindGenPart_had'
    #outdir_name_list = [
            #'GenPermInput_had/train_set',
            #'GenPermInput_had/opt_set',
            #'GenPermInput_had/fit_set',
            #]
    #ch = 'had'

    # lep signal
    indir_name = 'FindGenPart_lep'
    outdir_name_list = [
            'GenPermInput_lep/train_set',
            'GenPermInput_lep/opt_set',
            'GenPermInput_lep/fit_set',
            ]
    ch = 'lep'

    # Run
    tree_list = ['T', 'TGenPart']
    Divide_sig(indir_name, tree_list, outdir_name_list, ch)
