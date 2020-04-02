#!/usr/bin/env python2
# Run preselections.
# Usage:
#   ./Presel.py -Y year -E exe -I indir -O outdir [-L listfile]

import Path
import Util
import os
import time
import argparse

# Get the command line arguments
parser = argparse.ArgumentParser( description='' )
parser.add_argument(
	'--year', '-Y', type=int,
	help='Year of the datasets. (2016, 2017 or 2018)'
	)
parser.add_argument(
	'--exe', '-E', type=str,
	help='The binary to run on the datasets.'
	)
parser.add_argument(
	'--list', '-L', type=str, default='default_list',
	help='The list file of the datasets.'
	)
parser.add_argument(
	'--indir', '-I', type=str,
	help='The directory of the input datasets.'
	)
parser.add_argument(
	'--outdir', '-O', type=str,
	help='The directory to save the output.'
	)
parser.add_argument(
	'--run_qsub', action='store_true',
	help='Whether send jobs with qsub.'
	)
args = parser.parse_args()

# Check the arguments
if args.year!=2016 and args.year!=2017 and args.year!=2018:
	print '[ERROR] Invalid year for the datasets!'
	exit()

if args.list == 'default_list':
	if args.year == 2016:
		args.list = Path.dir_list + '/datasets_2016.txt'
	if args.year == 2017:
		args.list = Path.dir_list + '/datasets_2017_all.txt'
	if args.year == 2018:
		args.list = Path.dir_list + '/datasets_2018.txt'

if not os.path.exists(args.exe):
	print '[ERROR] Binary doesn\'t exist!'
	exit()

if not os.path.exists(args.list):
	print '[ERROR] List file doesn\'t exist!'
	exit()

if not os.path.exists(args.indir):
	print '[ERROR] Input directory doesn\'t exist!'
	exit()

# Create output directory
Util.CreateDir(args.outdir)

# Read n-tuple list
ls = Util.ReadList(args.list)


# Print the information
print '[INFO] Year of n-tuples: {}'.format(args.year)
print '[INFO] Start running: {}'.format(args.exe)
print '[INFO] Input directory: {}'.format(args.indir)
print '[INFO] Output directory: {}'.format(args.outdir)
print '[INFO] List of n-tuples: {}'.format(args.list)
print '[INFO] N-tuples to be processed:'
for dataset in ls:
	print '\t{}'.format(dataset[0])
print '\n'

i_job = 0
# Submit jobs for datasets
for dataset in ls:
	print 'Start processing dataset: {}'.format(dataset[0])
	cmd = '"set -o noglob; {} {} flashggStdTree {}"'.format(args.exe, args.indir+'/'+dataset[0]+'.root', args.outdir+'/'+dataset[0]+'.root')
	if args.run_qsub:
		t = time.time()
		os.system( Path.dir_tqHGG + '/qSub/submitJOB.py -c {} -N presel{:05d}_{:03d}'.format(cmd, int(t%100000), int(t*1000%1000)) )
	else:
		os.system( cmd.strip('"') )
	i_job += 1
	print ''

print '[INFO] Total job number: {}'.format(i_job)
