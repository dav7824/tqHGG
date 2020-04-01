#!/usr/bin/env python2
# Run preselections on 2017 datasets.
# Usage:
#   ./Presel17.py <binary name> <input dir name> <output dir name>
#
# Note: Only file names are needed. The path to files are added by the script.

import Path
import os
import sys

exe = Path.dir_bin + '/' + sys.argv[1]
indir = Path.dir_2017 + '/' + sys.argv[2]
outdir = Path.dir_2017 + '/' + sys.argv[3]

os.system( '{}/Presel.py --year 2017 --exe {} --indir {} --outdir {}'.format(Path.dir_script, exe, indir, outdir) )

print '[INFO] End of Presel17.py (binary: {}, indir: {}, outdir: {})'.format(sys.argv[1], sys.argv[2], sys.argv[3])
