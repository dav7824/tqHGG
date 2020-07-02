#!/usr/bin/env python2
# After running SampleSummary.py, run this to print the results.
# Usage:
#   ./PrintYieldSummary <directory>

import os, sys
import Path, Util
from os.path import join
from Samples import *

# Executable
exe_add = Path.dir_bin + '/AddYields'
exe_print = Path.dir_bin + '/PrintYieldSummary'
# Working dir
tmpdir = Path.dir_tqHGG + '/tmp'
outdir = Path.dir_2017 + '/plots/' + sys.argv[1]
Util.CreateDir(outdir)
# Command template
cmd_add = '{bin} {{list}} {indir} {{fout}} {{category}}'.format( bin=exe_add, indir=tmpdir )
cmd_print = '{bin} {list_file} {list_name} {bkg_dir} {data} {data_tree} {output}'.format( \
			bin=exe_print, \
			list_file=','.join(bkg_cat_file), \
			list_name=','.join(bkg_cat_name), \
			bkg_dir=tmpdir, \
			data=Path.dir_2017+'/'+sys.argv[1]+'/data.root', \
			data_tree='T', \
			output=outdir+'/YieldSummary.txt')

def Run_add( fin_list, cat_name ):
	print 'Processing: ', cat_name
	fin_str = ','.join(fin_list)
	fp = os.popen( cmd_add.format( list=fin_str, fout=join(tmpdir,cat_name+'.root'), category=cat_name ) )
	fp.close()

# Run AddYields
Run_add(bkg_GJet, 'GJet')
Run_add(bkg_QCD, 'QCD')
Run_add(bkg_tW, 'tW')
Run_add(bkg_TTV, 'TTV')
Run_add(bkg_VG, 'VG')
Run_add(bkg_VV, 'VV')
Run_add(bkg_Higgs, 'Higgs')

# Run PrintYieldSummary
print 'Printing results...'
fp = os.popen( cmd_print )
fp.close()

print 'Done!'
