#!/usr/bin/env python2

import Path, Util
from SystUnc import *
from Samples import sig_MC, bkg_MC
import os, sys
from os.path import join, exists

# Systematic processing modules
import AddScaleFactors_syst
import MVAreco_syst
import BDT_syst
import CutBDT_syst_tree


lsys_1 = [
        'JEC',
        'JER',
        'METJetEn',
        'METJetRes',
        'METUncEn',
        'METPhoEn',
        'MvaShift',
        'SigmaEOverEShift',
        'MaterialCentralBarrel',
        'MaterialOuterBarrel',
        'MaterialForward',
        'FNUFEB',
        'FNUFEE',
        'MCScaleGain6EB',
        'MCScaleGain1EB',
        ]

lsys_2 = [
        'ShowerShapeHighR9EB',
        'MCScaleHighR9EB',
        'MCSmearHighR9EBRho',
        'MCSmearHighR9EBPhi',
        'ShowerShapeHighR9EE',
        'MCScaleHighR9EE',
        'MCSmearHighR9EERho',
        'MCSmearHighR9EEPhi',
        'ShowerShapeLowR9EB',
        'MCScaleLowR9EB',
        'MCSmearLowR9EBRho',
        'MCSmearLowR9EBPhi',
        'ShowerShapeLowR9EE',
        'MCScaleLowR9EE',
        'MCSmearLowR9EERho',
        'MCSmearLowR9EEPhi',
        ]


# ProcSystEntry: Process given systematic (both up & down) in given channel, a tag is given to output text files
def ProcSystEntry(ch, syst, tag):
    if ch == 'had':
        evtdir_name = 'Presel_had_phID_btag-L'
        permdir_name = 'MVAreco_perms_had'
        recodir_name = 'MVAreco_result_had'
        bdtdir_name = 'BDT_input_had'
    elif ch == 'lep':
        evtdir_name = 'Presel_lep_phID'
        permdir_name = 'MVAreco_perms_lep'
        recodir_name = 'MVAreco_result_lep'
        bdtdir_name = 'BDT_input_lep'
    else:
        print '[ERROR] Invalid channel'
        sys.exit(1)
    if not (syst in Syst_tree_1 or syst in Syst_tree_2):
        print '[ERROR] Invalid systematic type'
        sys.exit(1)
    # Run
    print '---Processing:', syst
    for systdir in ['Up','Down']:
        AddScaleFactors_syst.ProcSystSF(ch, syst, systdir)
        MVAreco_syst.ProcSystMVAreco(ch, syst, systdir)
        BDT_syst.ProcSystBDT(ch, syst, systdir)
    CutBDT_syst_tree.ProcSystEntry(bdtdir_name, ch, syst, tag)
    # Delete files to save disk space
    cmd_rm = 'rm -r {}/Systematics/{{dirname}}/{{syst}}{{systdir}}'.format(Path.dir_2017)
    for systdir in ['Up','Down']:
        os.system( cmd_rm.format(dirname=bdtdir_name,syst=syst,systdir=systdir) )
        os.system( cmd_rm.format(dirname=recodir_name,syst=syst,systdir=systdir) )
        os.system( cmd_rm.format(dirname=permdir_name,syst=syst,systdir=systdir) )
        os.system( cmd_rm.format(dirname=evtdir_name,syst=syst,systdir=systdir) )
# End of function ProcSystEntry


# ProcSystBatch: Process all systematics in given list for given channel
def ProcSystBatch(ch, lsys, tag):
    for syst in lsys:
        ProcSystEntry(ch, syst, tag)
# End of function ProcSystBatch


if __name__ == '__main__':
    #ProcSystBatch( 'had', lsys_1, 'pt1' )
    #ProcSystBatch( 'had', lsys_2, 'pt2' )
    #ProcSystBatch( 'lep', lsys_1, 'pt1' )
    ProcSystBatch( 'lep', lsys_2, 'pt2' )
