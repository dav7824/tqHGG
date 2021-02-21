#!/usr/bin/env python2

import Path, Util
import sys, os
from Samples import sig_MC, bkg_MC
from os.path import join, exists


def Process(cmd, nt, treetype):
    fp = os.popen( cmd.format(nt=nt, treetype=treetype) )
    print fp.read()
    fp.close()


def MVAreco_apply(indir_name, outdir_name, ch):
    # Executable
    exe = join(Path.dir_bin, 'MVAreco_application')
    # Input permutation tree dir
    indir = join(Path.dir_2017, indir_name)
    # TMVA weight file dir
    weidir = join(Path.dir_2017, 'MVAreco_model/dataset/weights')
    # Output dir
    outdir = join(Path.dir_2017, outdir_name)

    if ch!='had' and ch!='lep':
        print '[ERROR] Invalid channel'
        sys.exit(1)
    if not exists(exe):
        print '[ERROR] Executable not exist'
        sys.exit(1)
    if not exists(indir) or not exists(weidir):
        print '[ERROR] Input dir not exist'
        sys.exit(1)
    Util.CreateDir(outdir)

    # Command template
    cmd = '%s %s/{{nt}}.root {fweight} %s/{{nt}}.root {{treetype}} {recotype} %s' % (exe, indir, outdir, ch)
    if ch == 'had':
        fweight_TT = join(weidir, 'TThad_ANN.weights.xml')
        fweight_ST = join(weidir, 'SThad_ANN.weights.xml')
    else:
        fweight_TT = join(weidir, 'TTlep_ANN.weights.xml')
        fweight_ST = join(weidir, 'STlep_ANN.weights.xml')
    cmdTT = cmd.format( fweight=fweight_TT, recotype='TT' )
    cmdST = cmd.format( fweight=fweight_ST, recotype='ST' )

    # Process signal MC
    for sigtype in sig_MC:
        if sigtype[1] != ch:
            continue
        if sigtype[0] == 'TT':
            (cmdSig, cmdBkg) = (cmdTT, cmdST)
        else:
            (cmdSig, cmdBkg) = (cmdST, cmdTT)
        for nt in sig_MC[sigtype]:
            print '---Processing:', nt
            Process( cmdSig, nt, 'sig' )
            Process( cmdBkg, nt, 'bkg' )
    # Process bkg MC
    for cat in bkg_MC:
        for nt in bkg_MC[cat]:
            if ch=='lep' and nt=='QCD_Pt-30to40_MGG-80toInf':
                continue
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
# End of function MVAreco_apply


if __name__ == '__main__':
    #MVAreco_apply( 'MVAreco_perms_had', 'MVAreco_result_had', 'had' )
    MVAreco_apply( 'MVAreco_perms_lep', 'MVAreco_result_lep', 'lep' )
