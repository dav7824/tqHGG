#!/usr/bin/env python2
#
# Run bin/SampleSummary to compute event yields and entries.
#
# Usage:
#   ./SampleSummary.py <directory_name>

import Path
import sys, os
from os.path import isfile, join, exists

# Executable
exe = Path.dir_bin + '/SampleSummary'
# Sample directory (log is saved there)
rootdir = Path.dir_2017 + '/' + sys.argv[1]
# Directory for output files
tmpdir = Path.dir_tqHGG + '/tmp'

# Get list of input samples
files = os.listdir(rootdir)

# Command template
cmd = '{bin} {{fin}} {{sample}} {tree} {{norm}} {{fout}}'.format( bin=exe, tree='T' )

# Create log file
output = open(rootdir + '/SampleSummary.txt', 'w')

# Run the code
for f in files:
	filename = join(rootdir, f)
	norm_file = join(Path.dir_norm_2017, f)
	if not isfile(filename) or not f.endswith('.root') or not exists(norm_file):
		continue
	print 'Processing: ', f
	nt = f.rstrip('.root')
	fp = os.popen( cmd.format( fin=filename, sample=nt, norm=norm_file, fout=join(tmpdir,f) ) )
	output.write( fp.read() )
	fp.close()

output.close()
print 'Done!'
