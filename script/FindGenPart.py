#!/usr/bin/env python2

import Path, Util
from Samples import sig_MC
import sys, os
from os.path import join, exists


def FindGenPart(indir_name, ch):
    exe = join(Path.dir_bin, 'FindGenPart')
    indir = join(Path.dir_2017, indir_name)
    if ch!='had' and ch!='lep':
        print '[ERROR] Invalid channel'
        sys.exit(1)
    if not exists(exe):
        print '[ERROR] Executable not exist!'
        sys.exit(1)
    if not exists(indir):
        print '[ERROR] Input dir not exist!'
        sys.exit(1)

    # Command template
    cmd = '{exe} {indir}/{{nt}}.root {tree} {{type}} {ch}'.format(
            exe=exe, indir=indir, tree='T', ch=ch)

    # Process signal MC
    for sigtype in sig_MC:
        if ch != sigtype[1]:
            continue
        for nt in sig_MC[sigtype]:
            print '---Processing:', nt
            fp = os.popen( cmd.format(nt=nt, type=sigtype[0]) )
            print fp.read()
            fp.close()
# End of function FindGenPart


if __name__ == '__main__':
    #FindGenPart( 'Presel_had_phID_btag-L', 'had' )
    FindGenPart( 'Presel_lep_phID', 'lep' )
