#!/usr/bin/env python2

import Path, Util, Samples
import sys, os
from os.path import join

def RunProcess(nt, nttype):
	print 'Processing:', nt
	fp = os.popen( cmd_t.format(nt=nt, nttype=nttype) )
	fp.close()

# Dir names
indir_name = 'MVArecoV2_Perm_had'
outdir_name = 'MVArecoV3_Perm_had'

# I/O dirs
indir = join(Path.dir_2017, indir_name)
outdir = join(Path.dir_2017, outdir_name)
Util.CreateDir(outdir)

# Executable
exe = join(Path.dir_bin, 'MVAreco_AdjustPermTree')

# Command tmeplate
cmd_t = '{bin} {indir}/{{nt}}.root {outdir}/{{nt}}.root {{nttype}}'.format(bin=exe, indir=indir, outdir=outdir)

# Process TT had signal
for nt in Samples.sig_MC_expr_v2[('TT', 'had')]:
	RunProcess(nt, 'sig')
# Process ST had signal
for nt in Samples.sig_MC_expr_v2[('ST', 'had')]:
	RunProcess(nt, 'sig')
# Process bkg
for cat in Samples.bkg_MC_s:
	for nt in Samples.bkg_MC_s[cat]:
		RunProcess(nt, 'bkg')
# Process data
RunProcess('data', 'bkg')

print 'Complete!'
