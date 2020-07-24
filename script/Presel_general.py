#!/usr/bin/env python2

import os, sys
from os.path import join
from Samples import *
import Path, Util

# Name of selection (executable name without prefix 'Presel_')
select = sys.argv[1]
# Tag of input samples (dir name without prefix 'Presel_')
input = sys.argv[2]
# Tag of output samples (dir name without prefix 'Presel_')
output = sys.argv[3]
# Channel
ch = sys.argv[4]

# Executable
exe = Path.dir_bin+'/Presel_'+select
# Input dir
indir = Path.dir_2017+'/Presel_'+input
# Output dir
outdir = Path.dir_2017+'/Presel_'+output
Util.CreateDir(outdir)

# Command template
cmd = '{bin} {{fin}} {tree} {{fout}}'.format(bin=exe, tree='T')

# List of input sample names
samples = bkg_data[:]
if ch=='had':
	samples.extend(sig_had)
elif ch=='lep':
	samples.extend(sig_lep)
else:
	print '[ERROR] Invalid channel!'
	sys.exit()

for nt in samples:
	print 'Processing:', nt
	filename = nt+'.root'
	fp = os.popen( cmd.format(fin=join(indir,filename), fout=join(outdir,filename)) )
	fp.close()

print 'End!'
