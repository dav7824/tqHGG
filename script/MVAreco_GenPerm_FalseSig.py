#!/usr/bin/env python2
#
# Generate false reconstruction permutations for signal MC.
# For TT signal events, generate ST permutations.  For ST signal events, generate TT permutations.
# In this scenario the samples are processed under "bkg" mode of the GenPerm codes.
#
# Usage:
#   ./MVAreco_GenPerm_FalseSig.py <channel>
#
# Note:
# 1. Make sure that the input event tree of signal MC only contains test events of MVA.
# 2. The leaf "Evt_idx" of output tree will have different values from that of the "test tree", although they corresponds
#    to same test events.  Beware of this difference in future processing.

import Path, Util, Samples
import os, sys
from os.path import join, exists

def Process(cmd):
	fp = os.popen(cmd)
	log.write( fp.read() )
	fp.close()

# Channel
ch = sys.argv[1]

indir_name = ''
outdir_name = ''
reco_exe = {}  # Executable to run for each reco type
if ch=='had':
	indir_name = 'Presel_had_phID_btag-L__MVAreco'
	outdir_name = 'MVArecoV2_Perm_had'
	reco_exe['TT'] = join(Path.dir_bin, 'MVAreco_GenPerm_SThad')
	reco_exe['ST'] = join(Path.dir_bin, 'MVAreco_GenPerm_TThad')
elif ch=='lep':
	indir_name = 'Presel_lep_phID__MVAreco'
	outdir_name = 'MVArecoV3_Perm_lep'
	reco_exe['TT'] = join(Path.dir_bin, 'MVAreco_GenPerm_STlep')
	reco_exe['ST'] = join(Path.dir_bin, 'MVAreco_GenPerm_TTlep')
else:
	print '[ERROR] Invalid channel!'
	sys.exit(1)

# I/O dirs
indir = join(Path.dir_2017, indir_name)
outdir = join(Path.dir_2017, outdir_name)

# Command template
cmd_t = '{bin} %s/{nt}.root %s/{nt}.root bkg' % (indir, outdir)

# Run
log = open( join(outdir,'summary_FalseSig.txt'), 'w' )
for recotype in reco_exe:
	for nt in Samples.sig_MC_expr_v2[(recotype, ch)]:
		print 'Processing:', nt
		log.write('*** Processing: %s' % nt)
		Process( cmd_t.format(bin=reco_exe[recotype], nt=nt) )

log.close()
print 'Complete!'
