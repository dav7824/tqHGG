#!/usr/bin/env python2

import Path, Util
from Samples import *
import os
from os.path import join

exe = Path.dir_bin+'/Presel_basic_v2'
indir = Path.dir_nt_2017
outdir = Path.dir_2017+'/Presel_basic'
Util.CreateDir(outdir)

cmd = 'set -o noglob; {bin} {{fin}} {tree} {{fout}}'.format(bin=exe, tree='flashggNtuples/flashggStdTree')

for nt in bkg_file_multi:
	print 'Processing:', nt[0]
	fp = os.popen( cmd.format(fin=join(indir,nt[1]), fout=join(outdir,nt[0]+'.root')) )
	fp.close()

print 'Done!'
