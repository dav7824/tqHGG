#!/usr/bin/env python2
# Reconstruct neutrino Pz, given reconstructed leptonic events.
# Usage:
#   ./NuPz.py <train_tag>
#
# The new tree containing nu Pz info will overwrite the original reco tree.

import Path, Util, Samples
import os, sys
from os.path import join
from Samples import sig_MC_s, bkg_MC_s

def RunNuPz( cmd ):
    fp = os.popen(cmd)
    #print fp.read()
    fp.close()

# Training tag
train_tag = sys.argv[1]

# Event tree dirs
# Signal MC samples are selected to only contain test events
evtdir_sig_lep = join(Path.dir_2017, 'Presel_lep_phID__MVAreco-test')
# Other samples are the original ones
evtdir_lep = join(Path.dir_2017, 'Presel_lep_phID')

# Reconstruction tree dirs
recodir_lep = join(Path.dir_2017, 'MVArecoV3_result_lep', train_tag)

# Executable
exe = join(Path.dir_bin, 'NuPz')

# Command template
# ./NuPz <evt_file> <reco_file> <TT|ST>
cmd_t = '%s {evtdir}/{{nt}}.root %s/{{nt}}.root {recotype}' % (exe, recodir_lep)
cmd_TT_sig = cmd_t.format( evtdir=evtdir_sig_lep, recotype='TT' )
cmd_ST_sig = cmd_t.format( evtdir=evtdir_sig_lep, recotype='ST' )
cmd_TT = cmd_t.format( evtdir=evtdir_lep, recotype='TT' )
cmd_ST = cmd_t.format( evtdir=evtdir_lep, recotype='ST' )

# Run signal MC
for sigtype in sig_MC_s:
    if sigtype[1] != 'lep':
        continue
    for nt in sig_MC_s[sigtype]:
        print 'Processing:', nt
        RunNuPz( cmd_TT_sig.format(nt=nt) )
        RunNuPz( cmd_ST_sig.format(nt=nt) )
# Run bkg MC
for cat in bkg_MC_s:
    for nt in bkg_MC_s[cat]:
        # Skip empty samples
        if nt == 'QCD_Pt-30to40_MGG-80toInf':
            continue
        print 'Processing:', nt
        RunNuPz( cmd_TT.format(nt=nt) )
        RunNuPz( cmd_ST.format(nt=nt) )
# Run data
print 'Processing: data'
RunNuPz( cmd_TT.format(nt='data') )
RunNuPz( cmd_ST.format(nt='data') )

print 'Complete!'
