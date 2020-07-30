#!/usr/bin/env python2
# Fill histograms for each sample.

import Path, Util, Samples
import sys, os
from os.path import join, exists

# Tag for input samples (dir name)
tag = sys.argv[1]
# Channel of samples
ch = sys.argv[2]
# List of trees used (strings seperated by commas)
#treelist = sys.argv[3]
treelist = 'T'
# Weight expression
#weight_expr = sys.argv[4]
weight_expr = 'EvtInfo.genweight'

if ch!='all' and ch!='had' and ch!='lep':
	print '[ERROR] Invalid channel!'
	sys.exit(1)

# Generate FillHist_[tag].cc
Util.GenCode_FillHist(tag)
# Compile FillHist_[tag]
fp = os.popen( 'cd $DIR_TQHGG; make bin/FillHist_{}'.format(tag) )
print fp.read()
fp.close()

# Test if the compilation succeed
exe = join( Path.dir_bin, 'FillHist_'+tag )
if not exists(exe):
	print '[ERROR] Compilation failed!\n'
	sys.exit(1)

# Input dir
indir = join( Path.dir_2017, tag )
# Output dir
outdir = join( Path.dir_2017, 'stackedplots', 'hist_'+tag )
Util.CreateDir(outdir)

# Command template
cmd_FillHist = '{bin} {{fin}} {trees} {{fout}} {ch} {weight}'.format(
		bin=exe, trees=treelist, ch=ch, weight=weight_expr)

# Process sig MC
for nt_type in Samples.sig_MC_s:
	if ch=='had' and nt_type[1]=='lep':
		continue
	if ch=='lep' and nt_type[1]=='had':
		continue
	for nt in Samples.sig_MC_s[nt_type]:
		print 'Processing:', nt
		fp = os.popen( cmd_FillHist.format( fin=join(indir,nt+'.root'), fout=join(outdir,nt+'.root') ) )
		print fp.read()
		fp.close

# Process bkg MC
for cat in Samples.bkg_MC_s:
	for nt in Samples.bkg_MC_s[cat]:
		print 'Processing:', nt
		fp = os.popen( cmd_FillHist.format( fin=join(indir,nt+'.root'), fout=join(outdir,nt+'.root') ) )
		print fp.read()
		fp.close

# Process data
print 'Processing: data'
fp = os.popen( cmd_FillHist.format( fin=join(indir,'data.root'), fout=join(outdir,'data.root') ) )
print fp.read()
fp.close

os.system('rm {}'.format(exe))
print 'End!'
