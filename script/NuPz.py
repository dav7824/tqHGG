#!/usr/bin/env python2
# Reconstruct neutrino Pz, given reconstructed leptonic events.
# Usage:
#   ./NuPz.py <train_tag>
#
# The new tree containing nu Pz info will overwrite the original reco tree.

import Path, Util, Samples
import os, sys
from os.path import join

def RunNuPz( cmd ):
	fp = os.popen(cmd)
	#print fp.read()
	fp.close()

# Training tag
train_tag = sys.argv[1]

# Event tree dirs
# Signal MC samples are selected to only contain test events
evtdir_sig_lep = join(Path.dir_2017, 'Presel_lep_phID__MVAreco')
# Other samples are the original ones
evtdir_lep = join(Path.dir_2017, 'Presel_lep_phID')

# Reconstruction tree dirs
recodir_lep = join(Path.dir_2017, 'MVArecoV2_result_lep', train_tag)

# Executable
exe = join(Path.dir_bin, 'NuPz')

# Command template
# ./NuPz <evt_file> <reco_file> <TT|ST>
cmd_t = '%s {evtdir}/{{nt}}.root %s/{{nt}}.root {recotype}' % (exe, recodir_lep)
cmd_TT_sig = cmd_t.format( evtdir=evtdir_sig_lep, recotype='TT' )
cmd_ST_sig = cmd_t.format( evtdir=evtdir_sig_lep, recotype='ST' )
cmd_TT = cmd_t.format( evtdir=evtdir_lep, recotype='TT' )
cmd_ST = cmd_t.format( evtdir=evtdir_lep, recotype='ST' )

# Run TT for signal samples
print '[INFO] Start running: TTlep_%s' % train_tag
for nt in Samples.sig_MC_expr_v2[('TT','lep')]:
	print 'Processing:', nt
	RunNuPz( cmd_TT_sig.format(nt=nt) )
# Run TT for other samples
for cat in Samples.bkg_MC_s:
	for nt in Samples.bkg_MC_s[cat]:
		print 'Processing:', nt
		RunNuPz( cmd_TT.format(nt=nt) )
print 'Processing: data'
RunNuPz( cmd_TT.format(nt='data') )

# Run ST for signal samples
print '[INFO] Start running: STlep_%s' % train_tag
for nt in Samples.sig_MC_expr_v2[('ST','lep')]:
	print 'Processing:', nt
	RunNuPz( cmd_ST_sig.format(nt=nt) )
# Run ST for other samples
for cat in Samples.bkg_MC_s:
	for nt in Samples.bkg_MC_s[cat]:
		print 'Processing:', nt
		RunNuPz( cmd_ST.format(nt=nt) )
print 'Processing: data'
RunNuPz( cmd_ST.format(nt='data') )

print 'Complete!'
