#!/usr/bin/env python2

import os, sys
from os.path import join, exists
import Path, Util, Samples

# Name of selection (executable name without prefix 'Presel_')
select = sys.argv[1]
# Tag of input samples (dir name without prefix 'Presel_')
input = sys.argv[2]
# Tag of output samples (dir name without prefix 'Presel_')
output = sys.argv[3]
# Channel
ch = sys.argv[4]
# Other options (in a string) to be passed to selection C++ code
opt = sys.argv[5]

if ch!='all' and ch!='had' and ch!='lep':
	print '[ERROR] Invalid channel!'
	sys.exit(1)

# A dash ('-') means no option will be used
if opt == '-':
	opt = ''

# Executable
exe = Path.dir_bin+'/Presel_'+select
# Input dir
indir = Path.dir_2017+'/Presel_'+input
# Output dir
outdir = Path.dir_2017+'/Presel_'+output
Util.CreateDir(outdir)

if not exists(exe):
	print '[ERROR] %s does not exist!' % exe
	sys.exit(1)
if not exists(indir):
	print '[ERROR] %s does not exist!' % indir
	sys.exit(1)

samples = {}
if ch=='had':
	samples = Samples.MC_had
elif ch=='lep':
	samples = Samples.MC_lep
elif ch=='all':
	samples = Samples.MC_all

# Command template
cmd = '{bin} {{fin}} {tree} {{fout}} {{options}}'.format(bin=exe, tree='T')

# Run command on each MC
for cat in samples:
	for nt in samples[cat]:
		print 'Processing:', nt
		filename = nt+'.root'
		fp = os.popen( cmd.format(fin=join(indir,filename), fout=join(outdir,filename), options=opt) )
		fp.close()
# Run command on data
print 'Processing: data'
fp = os.popen( cmd.format(fin=join(indir,'data.root'), fout=join(outdir,'data.root'), options=opt) )
fp.close()

print 'End!'
