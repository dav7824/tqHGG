#!/usr/bin/env python2
# Run AddVar_xxx.cc
# Usage:
#   ./AddVarRun2.py -E <exe_name> -I <indir_name> [--option <opt>] [--run_qsub]
#
# Note: Only file names are needed. The path to files are added by the script.

import Path
import os
import argparse

# Get the command line arguments
parser = argparse.ArgumentParser( description='' )
parser.add_argument(
	'--exe', '-E', type=str,
	help='Name of binary.'
	)
parser.add_argument(
	'--indir', '-I', type=str,
	help='Name of input directory.'
	)
parser.add_argument(
	'--option', type=str, default='',
	help='Other options for the binary.'
	)
parser.add_argument(
	'--run_qsub', action='store_true',
	help='Whether send jobs with qsub.'
	)
args = parser.parse_args()

option = '"'+args.option+'"'

qsub = ''
if args.run_qsub:
	qsub = ' --run_qsub'

os.system( '{}/AddVar.py -Y 2016 -E {} -I {} --option {}'.format(Path.dir_script, args.exe, args.indir, option) + qsub )
os.system( '{}/AddVar.py -Y 2017 -E {} -I {} --option {}'.format(Path.dir_script, args.exe, args.indir, option) + qsub )
os.system( '{}/AddVar.py -Y 2018 -E {} -I {} --option {}'.format(Path.dir_script, args.exe, args.indir, option) + qsub )

print '[INFO] End of AddVarRun2.py (binary: {}, indir: {})'.format(args.exe, args.indir)
