#!/usr/bin/env python2

import Path, Util
from Samples import sig_MC, bkg_MC
import os, sys
from os.path import exists, join
from ROOT import TFile, TTree


def Process(fin_name, treename):
    fin = TFile(fin_name)
    T = fin.Get(treename)
    nentries = T.GetEntries()
    fin.Close()
    return nentries


def GetEntries(indir_name, ch):
    if ch!='had' and ch!='lep':
        print '[ERROR] Invalid channel'
        sys.exit(1)
    indir = join(Path.dir_2017, indir_name)
    if not exists(indir):
        print '[ERROR] Input dir not exist'
        sys.exit(1)
    treename = 'T'

    entries = []
    # Process signal MC
    for sigtype in sig_MC:
        if sigtype[1] != ch:
            continue
        for nt in sig_MC[sigtype]:
            fin = join(indir, nt+'.root')
            entries.append( (nt, Process(fin, treename)) )
    # Process bkg MC
    for cat in bkg_MC:
        for nt in bkg_MC[cat]:
            # Skip empty sample
            if ch=='lep' and nt=='QCD_Pt-30to40_MGG-80toInf':
                continue
            fin = join(indir, nt+'.root')
            entries.append( (nt, Process(fin, treename)) )
    # Process data
    fin = join(indir, 'data.root')
    entries.append( ('data', Process(fin, treename)) )

    return entries
# End of function GetEntries


if __name__ == '__main__':
    indir_name = sys.argv[1]
    ch = sys.argv[2]

    entries = GetEntries(indir_name, ch)

    for nt in entries:
        print '{:18s}  {}'.format(nt[0], nt[1])
