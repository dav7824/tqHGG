#!/usr/bin/env python2
# Reconstruct neutrino Pz, given reconstructed leptonic signal events.
#
# Usage:
#   ./NuPz.py <name of MVA method>
#
# The new tree containing nu Pz info will overwrite the original reco tree.

import Path
import Samples
import os
import sys

def PrintRun( cmd ):
	print 'Running: ', cmd
	os.system( cmd )

method = sys.argv[1]
indir0 = Path.dir_2017+'/Presel_lep_btag-L_phID'
indir1 = Path.dir_2017+'/MVAreco_result_lep_btag-L_phID/'+method+'_'
exe = Path.dir_bin + '/NuPz'

cmd_template = exe + ' {indir0}/{{sample}}.root {tree0} {indir1}/{{sample}}.root {tree1} {{nttype}}'.format(
		indir0=indir0, tree0='T', indir1=indir1, tree1='T_MVAreco_'+method
		)

for nt in Samples.sig_TTlep:
	cmd = cmd_template.format( sample=nt, nttype='TT' )
	PrintRun( cmd )
	print ''
for nt in Samples.sig_STlep:
	cmd = cmd_template.format( sample=nt, nttype='ST' )
	PrintRun( cmd )
	print ''
for nt in Samples.bkg_noQCD_data:
	cmd = cmd_template.format( sample=nt, nttype='bkg' )
	PrintRun( cmd )
	print ''

print '[NuPz.py] Completed!'
