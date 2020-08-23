#!/usr/bin/env python2

import Path, Util, Samples
import sys, os
from os.path import join

def RunDivTree(cmd):
	fp = os.popen(cmd)
	print fp.read()
	fp.close()

# Suffix for output dirs
version = '__MVAreco'

# Preselection names
presel_had = 'Presel_had_phID_btag-L'
presel_lep = 'Presel_lep_phID'

# Input dirs
indir_had = join(Path.dir_2017, presel_had)
indir_lep = join(Path.dir_2017, presel_lep)

# Output dirs
outdir_had = join(Path.dir_2017, presel_had+version)
outdir_lep = join(Path.dir_2017, presel_lep+version)
Util.CreateDir(outdir_had)
Util.CreateDir(outdir_lep)

# Permutaiton dirs
permdir_had = join(Path.dir_2017, 'MVAreco_Perm_had')
permdir_lep = join(Path.dir_2017, 'MVAreco_Perm_lep')

# Executable
bin = join(Path.dir_bin, 'DivideTree')

# Command template
cmd_divtree = '{exe} {fin} {fout} {fperm}'
cmd_divtree_had = cmd_divtree.format(exe=bin, fin=join(indir_had,'{nt}.root'), fout=join(outdir_had,'{nt}.root'), fperm=join(permdir_had,'{nt}.root'))
cmd_divtree_lep = cmd_divtree.format(exe=bin, fin=join(indir_lep,'{nt}.root'), fout=join(outdir_lep,'{nt}.root'), fperm=join(permdir_lep,'{nt}.root'))

# Run
for key in Samples.sig_MC_had:
	for nt in Samples.sig_MC_had[key]:
		print 'Processing:', nt
		RunDivTree( cmd_divtree_had.format(nt=nt) )
for key in Samples.sig_MC_lep:
	for nt in Samples.sig_MC_lep[key]:
		print 'Processing:', nt
		RunDivTree( cmd_divtree_lep.format(nt=nt) )

print 'Complete!'
