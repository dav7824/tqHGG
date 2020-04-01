#!/usr/bin/env python2
# Run preselections on all Run2 datasets.
# Usage:
#   ./PreselRun2.py <binary name> <input dir name> <output dir name>
#
# Note: Only file names are needed. The path to files are added by the script.

import Path
import os
import sys

os.system( '{}/Presel16.py {} {} {}'.format(Path.dir_script, sys.argv[1], sys.argv[2], sys.argv[3]) )
os.system( '{}/Presel17.py {} {} {}'.format(Path.dir_script, sys.argv[1], sys.argv[2], sys.argv[3]) )
os.system( '{}/Presel18.py {} {} {}'.format(Path.dir_script, sys.argv[1], sys.argv[2], sys.argv[3]) )

print '[INFO] End of PreselRun2.py (binary: {}, indir: {}, outdir: {})'.format(sys.argv[1], sys.argv[2], sys.argv[3])
