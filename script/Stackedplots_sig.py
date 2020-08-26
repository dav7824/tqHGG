#!/usr/bin/env python2

import os, sys
import Path, Util, Samples
from os.path import join, exists

# Tag for input samples (dir name)
tag = sys.argv[1]
# Channel of samples
ch = sys.argv[2]

if ch!='had' and ch!='lep':
	print '[ERROR] Invalid channel!'
	sys.exit(1)


# Run HistNormAdd.py (If you've already run HistNormAdd.py and just want to make plot using HistPlotter,
# just comment this part out.)
fp = os.popen( '{py} {tag} {ch}'.format(py=join(Path.dir_script,'HistNormAdd.py'), tag=tag, ch=ch) )
print fp.read()
fp.close()


# Set paths
exe_HistPlotter = join(Path.dir_bin, 'HistPlotter')
indir = join(Path.dir_2017, 'stackedplots', 'hist_'+tag)
tmpdir = join(Path.dir_tqHGG, 'tmp', 'hist_'+tag)
if not exists(tmpdir):
	print '[ERROR] Cannot find histograms!'
	sys.exit(1)
outdir = join(Path.dir_2017, 'stackedplots', 'plots_'+tag+'_sig')
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
ftotbkg = join(tmpdir, 'totbkg.root')

# Sig files
sig_list = 'TT{ch}hct,ST{ch}hct,TT{ch}hut,ST{ch}hut'.format(ch=ch)
# Sig legends
sig_leg = 'TT_hct,ST_hct,TT_hut,ST_hut'

# Data file
fdata = join(indir, 'data.root')

# Bkg dir
dir_bkg = tmpdir
# Sig dir
dir_sig = tmpdir

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
