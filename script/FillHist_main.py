#!/usr/bin/env python2

import Path, Util
import sys, os
from os.path import join, isfile, exists


exe = Path.dir_bin+'/FillHist_main'
treedir = Path.dir_2017+'/'+sys.argv[1]
histdir = Path.dir_2017+'/plots/'+sys.argv[1]
normdir = Path.dir_2017+'/normfactor'
ch = sys.argv[2]
Util.CreateDir(histdir)

# Command template
cmd = '{bin} {{fin}} {tree} {{fout}} {channel} {weight}'.format( bin=exe, tree='T', channel=ch, weight='EvtInfo.genweight' )

files = os.listdir( treedir )

for f in files:
	filename = join(treedir, f)
	if not isfile(filename) or not f.endswith('.root'):
		continue
	nt = f.rstrip('.root')
	outname = join(histdir, f)
	print 'Processing: ', nt
	fp = os.popen( cmd.format( fin=filename, fout=outname ) )
	fp.close()

print 'Done!'
