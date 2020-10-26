#!/usr/bin/env python2

import Path, Util, Samples
import os, sys
from os.path import join, exists

def CountEvents(indir_name, tree):
    exe = join(Path.dir_bin, 'CountEvents')
    indir = join(Path.dir_2017, indir_name)
    log_path = join(indir, 'nentries.log')

    if not exists(exe):
        print '[ERROR] Executable does not exist!'
        sys.exit(1)
    if not exists(indir):
        print '[ERROR] Input dir does not exist!'
        sys.exit(1)

    # Command template
    cmd = '{bin} {indir}/{{nt}}.root {{nt}} {tree}'.format(
            bin=exe, indir=indir, tree=tree)

    files = os.listdir(indir)

    log = open(log_path, 'w')

    log.write( '[INFO] Dir: {}\n'.format(indir_name) )
    # Run
    for file in files:
        if not file.endswith('.root'):
            continue
        nt = os.path.splitext(file)[0]
        fp = os.popen( cmd.format(nt=nt) )
        log.write( fp.read() )
        fp.close()
    log.close()
# End of function CountEvents


if __name__ == '__main__':
    CountEvents('BDT_had/train', 'T')
    CountEvents('BDT_had/test', 'T')
    CountEvents('BDT_lep/train', 'T')
    CountEvents('BDT_lep/test', 'T')

    CountEvents('optimization_had', 'T')
    CountEvents('optimization_lep', 'T')

    #CountEvents('model_had', 'T')
    #CountEvents('model_lep', 'T')
