#!/usr/bin/env python2

import Path, Util
from Samples import sig_MC, bkg_MC
import os, sys
from os.path import exists, join


def Process(cmd, nt, mode):
    fp = os.popen( cmd.format(nt=nt,mode=mode) )
    print fp.read()
    fp.close()


def GenPerm(indir_name, outdir_name, ch):
    # Check channel
    if ch == 'had':
        exeTT = join(Path.dir_bin, 'MVAreco_GenPerm_TThad')
        exeST = join(Path.dir_bin, 'MVAreco_GenPerm_SThad')
    elif ch == 'lep':
        exeTT = join(Path.dir_bin, 'MVAreco_GenPerm_TTlep')
        exeST = join(Path.dir_bin, 'MVAreco_GenPerm_STlep')
    else:
        print '[ERROR] Invalid channel'
        sys.exit(1)
    # Input dir
    indir = join(Path.dir_2017, indir_name)
    if not exists(indir):
        print '[ERROR] Input dir not exist'
        sys.exit(1)
    # Output dir
    outdir = join(Path.dir_2017, outdir_name)
    Util.CreateDir(outdir)

    # Command template
    cmdTT = '{exe} {indir}/{{nt}}.root {outdir}/{{nt}}.root {{mode}}'.format(
            exe=exeTT, indir=indir, outdir=outdir)
    cmdST = '{exe} {indir}/{{nt}}.root {outdir}/{{nt}}.root {{mode}}'.format(
            exe=exeST, indir=indir, outdir=outdir)

    # Process signal MC
    for sigtype in sig_MC:
        if sigtype[1] != ch:
            continue
        if sigtype[0] == 'TT':
            cmdSig = cmdTT
            cmdBkg = cmdST
        else:    # sigtype[0] == 'ST'
            cmdSig = cmdST
            cmdBkg = cmdTT
        for nt in sig_MC[sigtype]:
            # Run
            print '---Processing:', nt
            Process( cmdSig, nt, 'sig' )
            Process( cmdBkg, nt ,'bkg' )
    # Process bkg MC
    for cat in bkg_MC:
        for nt in bkg_MC[cat]:
            if ch=='lep' and nt=='QCD_Pt-30to40_MGG-80toInf':
                continue
            # Run
            print '---Processing:', nt
            Process( cmdTT, nt, 'bkg' )
            Process( cmdST, nt, 'bkg' )
    # Process data-driven QCD
    if ch == 'had':
        print '---Processing:', 'Data-driven_QCD'
        Process( cmdTT, 'Data-driven_QCD', 'bkg' )
        Process( cmdST, 'Data-driven_QCD', 'bkg' )
    # Process data
    print '---Processing:', 'data'
    Process( cmdTT, 'data', 'bkg' )
    Process( cmdST, 'data', 'bkg' )
# End of function GenPerm


if __name__ == '__main__':
    #GenPerm('Presel_had_phID_btag-L', 'MVAreco_perms_had', 'had')
    GenPerm('Presel_lep_phID', 'MVAreco_perms_lep', 'lep')
