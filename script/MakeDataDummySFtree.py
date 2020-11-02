#!/usr/bin/env python2

import Path, Util
import os, sys
from os.path import join, exists


def Process(indir_name, ch):
    exe = join(Path.dir_bin, 'MakeDataDummySFtree')
    fin = join(Path.dir_2017, indir_name, 'data.root')
    if not exists(fin):
        print '[ERROR] Input file does not exist!'
        sys.exit(1)

    # Command template
    cmd = '{exe} {fin} {ch}'.format(
            exe=exe, fin=fin, ch=ch)

    os.system(cmd)
    print 'Complete!'
# End of function Process


if __name__ == '__main__':
    # hadronic
    #indir_name = 'Presel_had_phID_btag-L'
    #ch = 'had'

    # leptonic
    indir_name = 'Presel_lep_phID'
    ch = 'lep'

    Process(indir_name, ch)
