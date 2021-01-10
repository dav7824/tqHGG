#!/usr/bin/env python2

import Path, Util
from Samples import sig_MC
import sys, os
from os.path import join, exists


def Process(cmd, nt):
    print '---Processing: {}'.format(nt)
    fp = os.popen( cmd.format(nt=nt) )
    print fp.read()
    fp.close()


def FindGenPart(indir_name, type, ch):
    exe = join(Path.dir_bin, 'FindGenPart')
    indir = join(Path.dir_2017, indir_name)
    treename = 'T'
    if type != 'TT' and type != 'ST':
        print '[ERROR] Invalid signal production mode'
        return
    if ch != 'had' and ch != 'lep':
        print '[ERROR] Invalid channel'
        return
    if not exists(indir):
        print '[ERROR] Input dir does not exist'
        return

    # Command template
    cmd = '{exe} {indir}/{{nt}}.root {treename} {type} {ch}'.format(
            exe=exe, indir=indir, treename=treename, type=type, ch=ch)

    print '\n\n'
    print '---Start processing: {} {}'.format(type, ch)
    print '---Input dir: {}'.format(indir)

    # Process signal MC of given type & ch
    for sigtype in sig_MC:
        if sigtype[0] != type or sigtype[1] != ch:
            continue
        for nt in sig_MC[sigtype]:
            Process(cmd, nt)
# End of function FindGenPart


if __name__ == '__main__':
    # hadronic TT
    FindGenPart('FindGenPart_had', 'TT', 'had')
    # hadronic ST
    FindGenPart('FindGenPart_had', 'ST', 'had')
    # leptonic TT
    FindGenPart('FindGenPart_lep', 'TT', 'lep')
    # leptonic ST
    FindGenPart('FindGenPart_lep', 'ST', 'lep')
