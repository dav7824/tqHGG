#!/usr/bin/env python2
# Run compiled C++ codes on datasets of 3 years.
# Usage:
#   ./Runner_runII.py -E <exe> -I <indir> [-i <in_tag>] -O <outdir> [-o <out_tag>] [--option <opt>] [--run_qsub]
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
	'--in_tag', '-i', type=str, default='',
	help='Tag of input root files.'
	)
parser.add_argument(
	'--outdir', '-O', type=str,
	help='Name of output directory.'
	)
parser.add_argument(
	'--out_tag', '-o', type=str, default='',
	help='Tag of output root files.'
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

in_tag = '"'+args.in_tag+'"'
out_tag = '"'+args.out_tag+'"'
option = '"'+args.option+'"'

qsub = ''
if args.run_qsub:
	qsub = '--run_qsub'

# Run run II datasets
runner = '{}/Runner.py'.format(Path.dir_script)
for yr in [2016, 2017, 2018]:
	os.system( '{} -Y {} -E {} -I {} -i {} -O {} -o {} --option {} {}'.format(runner, yr, args.exe, args.indir, in_tag, args.outdir, out_tag, option, qsub) )

print '[INFO] End of Runner_runII.py (binary: {}, indir: {}, in_tag: {}, outdir: {}, out_tag: {})'.format(args.exe, args.indir, args.in_tag, args.outdir, args.out_tag)
