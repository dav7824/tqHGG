#!/usr/bin/env python2
#
# Apply corrections, produce systematic trees, and apply basic preselections.
# Presel_Syst_v1 process systematic trees from input files.
# Presel_Syst_v2 process central trees from input files and apply systematic variation.

import Path, Util, Samples, SystUnc
import os, sys
from os.path import join


def Presel_Central():
    # Set Paths
    exe = join(Path.dir_bin, 'Presel_CorrSyst')
    outdir = join(Path.dir_2017, 'Presel_basic')
    Util.CreateDir(outdir)
    # Command template
    cmd_t = 'set -f; {exe} {{indir}}/{{fin}} {outdir}/{{nt}}.root {inTree} {outTree} {systtype}'.format(
            exe=exe, outdir=outdir, inTree='flashggNtuples/flashggStdTree', outTree='T', systtype='Central')

    # Process signal MC
    for sigtype in Samples.sig_MC:
        for idx in range(len(Samples.sig_MC[sigtype])):
            print '---Start running: {}'.format(Samples.sig_MC[sigtype][idx])
            cmd = cmd_t.format( indir=Path.dir_nt_2017_Higgs, fin=Samples.sig_MC_file[sigtype][idx],
                    nt=Samples.sig_MC[sigtype][idx] )
            os.system(cmd)
    # Process resonant bkg MC
    for idx in range(len(Samples.bkg_MC['Higgs'])):
        print '---Start running: {}'.format(Samples.bkg_MC['Higgs'][idx])
        cmd = cmd_t.format( indir=Path.dir_nt_2017_Higgs, fin=Samples.bkg_MC_file['Higgs'][idx],
                nt=Samples.bkg_MC['Higgs'][idx] )
        os.system(cmd)
    # Process non-resonant bkg MC
    for cat in Samples.bkg_MC:
        if cat == 'Higgs':
            continue
        for idx in range(len(Samples.bkg_MC[cat])):
            print '---Start running: {}'.format(Samples.bkg_MC[cat][idx])
            cmd = cmd_t.format( indir=Path.dir_nt_2017, fin=Samples.bkg_MC_file[cat][idx],
                    nt=Samples.bkg_MC[cat][idx] )
            os.system(cmd)
    # Process data
    print '---Start running: {}'.format('data')
    cmd = cmd_t.format( indir=Path.dir_nt_2017, fin='DoubleEG.root', nt='data' )
    os.system(cmd)

    print '---Complete!'
# End of Presel_Central


def Presel_Syst_v1(syst, systdir='Up'):
    if systdir != 'Up' and systdir != 'Down':
        print '[ERROR] Systematic variation has to be "Up" or "Down"!'
        return
    if not syst in SystUnc.Syst_tree_1:
        print '[ERROR] Invalid systematic type!'
        return
    # Set Paths
    exe = join(Path.dir_bin, 'Presel_CorrSyst')
    outdir = join(Path.dir_2017, 'Systematics', 'Presel_basic', syst+systdir)
    Util.CreateDirs(outdir)
    # Tree name
    treename = 'flashggNtuples/flashggStdTree{}{}01sigma'.format( syst, systdir )
    # Command template
    cmd_t = 'set -f; {exe} {indir}/{{fin}} {outdir}/{{nt}}.root {inTree} {outTree} {systtype}'.format(
            exe=exe, indir=Path.dir_nt_2017_Higgs, outdir=outdir, inTree=treename, outTree='T', systtype='Central')

    # Process signal MC
    for sigtype in Samples.sig_MC:
        for idx in range(len(Samples.sig_MC[sigtype])):
            print '---Start running: {}'.format(Samples.sig_MC[sigtype][idx])
            cmd = cmd_t.format( fin=Samples.sig_MC_file[sigtype][idx], nt=Samples.sig_MC[sigtype][idx] )
            os.system(cmd)
    # Process resonant bkg MC
    for idx in range(len(Samples.bkg_MC['Higgs'])):
        print '---Start running: {}'.format(Samples.bkg_MC['Higgs'][idx])
        cmd = cmd_t.format( fin=Samples.bkg_MC_file['Higgs'][idx], nt=Samples.bkg_MC['Higgs'][idx] )
        os.system(cmd)

    print '---Complete!'
# End of Presel_Syst_v1


def Presel_Syst_v2(syst, systdir='Up'):
    if systdir != 'Up' and systdir != 'Down':
        print '[ERROR] Systematic variation has to be "Up" or "Down"!'
        return
    if not syst in SystUnc.Syst_tree_2:
        print '[ERROR] Invalid systematic type!'
        return
    # Set paths
    exe = join(Path.dir_bin, 'Presel_CorrSyst')
    outdir = join(Path.dir_2017, 'Systematics', 'Presel_basic', syst+systdir)
    Util.CreateDirs(outdir)
    # Command template
    cmd_t = 'set -f; {exe} {indir}/{{fin}} {outdir}/{{nt}}.root {inTree} {outTree} {systtype}'.format(
            exe=exe, indir=Path.dir_nt_2017_Higgs, outdir=outdir, inTree='flashggNtuples/flashggStdTree', outTree='T',
            systtype=syst+systdir)

    # Process signal MC
    for sigtype in Samples.sig_MC:
        for idx in range(len(Samples.sig_MC[sigtype])):
            print '---Start running: {}'.format(Samples.sig_MC[sigtype][idx])
            cmd = cmd_t.format( fin=Samples.sig_MC_file[sigtype][idx], nt=Samples.sig_MC[sigtype][idx] )
            os.system(cmd)
    # Process resonant bkg MC
    for idx in range(len(Samples.bkg_MC['Higgs'])):
        print '---Start running: {}'.format(Samples.bkg_MC['Higgs'][idx])
        cmd = cmd_t.format( fin=Samples.bkg_MC_file['Higgs'][idx], nt=Samples.bkg_MC['Higgs'][idx] )
        os.system(cmd)

    print '---Complete!'
# End of Presel_Syst_v2


if __name__ == '__main__':
    # Central
    #Presel_Central()

    # Systematic v1
    #Presel_Syst_v1('MvaShift', 'Up')
    #Presel_Syst_v1('MvaShift', 'Down')
    #Presel_Syst_v1('SigmaEOverEShift', 'Up')
    #Presel_Syst_v1('SigmaEOverEShift', 'Down')
    #Presel_Syst_v1('MaterialCentralBarrel', 'Up')
    #Presel_Syst_v1('MaterialCentralBarrel', 'Down')
    #Presel_Syst_v1('MaterialOuterBarrel', 'Up')
    #Presel_Syst_v1('MaterialOuterBarrel', 'Down')
    #Presel_Syst_v1('MaterialForward', 'Up')
    #Presel_Syst_v1('MaterialForward', 'Down')
    #Presel_Syst_v1('FNUFEB', 'Up')
    #Presel_Syst_v1('FNUFEB', 'Down')
    #Presel_Syst_v1('FNUFEE', 'Up')
    #Presel_Syst_v1('FNUFEE', 'Down')
    #Presel_Syst_v1('MCScaleGain6EB', 'Up')
    #Presel_Syst_v1('MCScaleGain6EB', 'Down')
    #Presel_Syst_v1('MCScaleGain1EB', 'Up')
    #Presel_Syst_v1('MCScaleGain1EB', 'Down')
    #Presel_Syst_v1('ShowerShapeHighR9EB', 'Up')
    #Presel_Syst_v1('ShowerShapeHighR9EB', 'Down')
    #Presel_Syst_v1('MCScaleHighR9EB', 'Up')
    #Presel_Syst_v1('MCScaleHighR9EB', 'Down')
    #Presel_Syst_v1('MCSmearHighR9EBRho', 'Up')
    #Presel_Syst_v1('MCSmearHighR9EBRho', 'Down')
    #Presel_Syst_v1('MCSmearHighR9EBPhi', 'Up')
    #Presel_Syst_v1('MCSmearHighR9EBPhi', 'Down')
    #Presel_Syst_v1('ShowerShapeHighR9EE', 'Up')
    #Presel_Syst_v1('ShowerShapeHighR9EE', 'Down')
    #Presel_Syst_v1('MCScaleHighR9EE', 'Up')
    #Presel_Syst_v1('MCScaleHighR9EE', 'Down')
    #Presel_Syst_v1('MCSmearHighR9EERho', 'Up')
    #Presel_Syst_v1('MCSmearHighR9EERho', 'Down')
    #Presel_Syst_v1('MCSmearHighR9EEPhi', 'Up')
    #Presel_Syst_v1('MCSmearHighR9EEPhi', 'Down')
    #Presel_Syst_v1('ShowerShapeLowR9EB', 'Up')
    #Presel_Syst_v1('ShowerShapeLowR9EB', 'Down')
    #Presel_Syst_v1('MCScaleLowR9EB', 'Up')
    #Presel_Syst_v1('MCScaleLowR9EB', 'Down')
    #Presel_Syst_v1('MCSmearLowR9EBRho', 'Up')
    #Presel_Syst_v1('MCSmearLowR9EBRho', 'Down')
    #Presel_Syst_v1('MCSmearLowR9EBPhi', 'Up')
    #Presel_Syst_v1('MCSmearLowR9EBPhi', 'Down')
    #Presel_Syst_v1('ShowerShapeLowR9EE', 'Up')
    #Presel_Syst_v1('ShowerShapeLowR9EE', 'Down')
    #Presel_Syst_v1('MCScaleLowR9EE', 'Up')
    #Presel_Syst_v1('MCScaleLowR9EE', 'Down')
    #Presel_Syst_v1('MCSmearLowR9EERho', 'Up')
    #Presel_Syst_v1('MCSmearLowR9EERho', 'Down')
    #Presel_Syst_v1('MCSmearLowR9EEPhi', 'Up')
    #Presel_Syst_v1('MCSmearLowR9EEPhi', 'Down')

    # Systematic v2
    Presel_Syst_v2('JEC', 'Up')
    Presel_Syst_v2('JEC', 'Down')
    Presel_Syst_v2('JER', 'Up')
    Presel_Syst_v2('JER', 'Down')
    Presel_Syst_v2('METJetEn', 'Up')
    Presel_Syst_v2('METJetEn', 'Down')
    Presel_Syst_v2('METJetRes', 'Up')
    Presel_Syst_v2('METJetRes', 'Down')
    Presel_Syst_v2('METUncEn', 'Up')
    Presel_Syst_v2('METUncEn', 'Down')
    Presel_Syst_v2('METPhoEn', 'Up')
    Presel_Syst_v2('METPhoEn', 'Down')
