#!/usr/bin/env python2

import Path, Util, Samples
import os, sys
from os.path import exists, join

# Fit gaussian pdf to datasets (cut values are specified in macro)
def makeModel_gaus(ch, type):
    if ch!='had' and ch!='lep':
        print '[ERROR] Invalid channel!'
        sys.exit(1)
    if type!='Hut' and type!='Hct' and type!='bkg':
        print '[ERROR] Invalid sample type!'
        sys.exit(1)
    # Macro
    exe = join(Path.dir_tqHGG, 'macro/modeling', 'makeModel_gaus.cc')
    # Set input dir
    indir = join(Path.dir_2017, 'model_%s'%ch, 'output_BDT_v1')
    if not exists(indir):
        print '[ERROR] Input dir does not exist!'
        sys.exit(1)
    # Set output dir
    if type=='Hut':
        outdir = join(indir, 'models/Hut')
    elif type=='Hct':
        outdir = join(indir, 'models/Hct')
    else:
        outdir = join(indir, 'models/resbkg')
    Util.CreateDirs(outdir)

    # Command template
    # Bash will see: root -l -b -q <exe>\(\"<ch>\",\"<type>\"\)
    # Root will see: <exe>("<ch>","<type>")
    cmd = 'root -l -b -q {exe}\(\\"{ch}\\",\\"{type}\\"\)'.format(
            exe=exe, ch=ch, type=type)

    # Run
    print 'Processing: ch={}   type={}'.format(ch, type)
    output = open( join(outdir,'modeling.log'), 'w' )
    fp = os.popen( cmd )
    output.write( fp.read() )
    fp.close()
    output.close()
# End of function makeModel_gaus


if __name__ == '__main__':
    # hadronic modeling
    #makeModel_gaus('had', 'Hut')
    #makeModel_gaus('had', 'Hct')
    #makeModel_gaus('had', 'bkg')

    # leptonic modeling
    #makeModel_gaus('lep', 'Hut')
    #makeModel_gaus('lep', 'Hct')
    #makeModel_gaus('lep', 'bkg')
