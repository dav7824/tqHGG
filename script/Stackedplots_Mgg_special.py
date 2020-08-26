#!/usr/bin/env python2
# Usage:
#   ./Stackedplots_Mgg_special.py <tag> <channel>
#
# Make Mgg stacked plot. Data is blinded while MCs are not.
# Since data and MC are in different selections, only Mgg plot makes sense.

import Path, Util, Samples
import os, sys
from os.path import join

# Tag for input samples (dir name)
tag = sys.argv[1]
# Channel of samples
ch = sys.argv[2]

if ch!='had' and ch!='lep':
	print '[ERROR] Invalid channel!'
	sys.exit(1)

# Data hist dir
datadir = join(Path.dir_2017, 'stackedplots', 'hist_'+tag+'_blind')
fdata = join(datadir, 'data.root')

# MC hist dir
mcdir = join(Path.dir_tqHGG, 'tmp', 'hist_'+tag)

# Executable
exe_HistPlotter = join(Path.dir_bin, 'HistPlotter')

# Output dir
outdir = join(Path.dir_2017, 'stackedplots', 'plots_'+tag+'_Mgg')
Util.CreateDir(outdir)

# Bkg files
bkg_list_ = Samples.bkg_MC_stack_order
bkg_list = ','.join(bkg_list_)
# Bkg legends
bkg_leg_ = []
for nt in bkg_list_:
	bkg_leg_.append(Samples.bkg_MC_leg[nt])
bkg_leg = ','.join(bkg_leg_)
# Total bkg file
ftotbkg = join(mcdir, 'totbkg.root')

# Sig files
sig_list = 'TT{ch}hct,ST{ch}hct,TT{ch}hut,ST{ch}hut'.format(ch=ch)
# Sig legends
sig_leg = 'TT_hct,ST_hct,TT_hut,ST_hut'

# Bkg dir
dir_bkg = mcdir
# Sig dir
dir_sig = mcdir

# Options
plot_dom = 1
use_log_y = 1

# Command template
cmd_HistPlotter = '{bin} {bkg_list} {bkg_leg} {ftotbkg} {sig_list} {sig_leg} {fdata} {dir_bkg} {dir_sig} \
		{outdir} {ch} {plot_dom} {use_log_y}'.format( \
		bin=exe_HistPlotter, bkg_list=bkg_list, bkg_leg=bkg_leg, ftotbkg=ftotbkg, \
		sig_list=sig_list, sig_leg=sig_leg, fdata=fdata, dir_bkg=dir_bkg, dir_sig=dir_sig, \
		outdir=outdir, ch=ch, plot_dom=plot_dom, use_log_y=use_log_y \
		)

# Run HistPlotter
print '\n[INFO] Start running: HistPlotter'
fp = os.popen(cmd_HistPlotter)
print fp.read()
fp.close()

print 'End!'
