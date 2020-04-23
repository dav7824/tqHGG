#!/usr/bin/env python2
# Run preselections.
# Usage:
#   ./Presel.py -Y <year> -E <exe> -I <indir> -O <outdir> [-L <listfile>] [--option <opt>] [--run_qsub]

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
	help='Name of binary.'
	)
parser.add_argument(
	'--list', '-L', type=str, default='default_list',
	help='Name of list file specifying the datasets.'
	)
parser.add_argument(
	'--indir', '-I', type=str,
	help='Name of input directory.'
	)
parser.add_argument(
	'--outdir', '-O', type=str,
	help='Name of output directory.'
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

# Check the arguments
iodir = ''
if args.year==2016:
	iodir = Path.dir_2016
elif args.year==2017:
	iodir = Path.dir_2017
elif args.year==2018:
	iodir = Path.dir_2018
else:
	print '[ERROR] Invalid year for the datasets!'
	exit()
indir = iodir+'/'+args.indir
outdir = iodir+'/'+args.outdir

bin = Path.dir_bin+'/'+args.exe
if not os.path.exists(bin):
	print '[ERROR] Binary doesn\'t exist!'
	exit()

if args.list == 'default_list':
	if args.year == 2016:
		args.list = 'datasets_2016.txt'
	if args.year == 2017:
		args.list = 'datasets_2017_all.txt'
	if args.year == 2018:
		args.list = 'datasets_2018.txt'

flist = Path.dir_list+'/'+args.list
if not os.path.exists(flist):
	print '[ERROR] List file doesn\'t exist!'
	exit()

if not os.path.exists(indir):
	print '[ERROR] Input directory doesn\'t exist!'
	exit()

# Create output directory
Util.CreateDir(outdir)

# Read n-tuple list
ls = Util.ReadList(flist)


# Print the information
print '[INFO] Year of n-tuples: {}'.format(args.year)
print '[INFO] Start running: {}'.format(args.exe)
print '[INFO] Input directory: {}'.format(indir)
print '[INFO] Output directory: {}'.format(outdir)
print '[INFO] List of n-tuples: {}'.format(args.list)
print '[INFO] N-tuples to be processed:'
for nt in ls:
	print '\t{}'.format(nt[0])
print '\n'

i_job = 0
# Submit jobs for datasets
for nt in ls:
	print 'Start processing dataset: {}'.format(nt[0])
	cmd = '"set -o noglob; {} {} flashggStdTree {} {}"'.format(bin, indir+'/'+nt[0]+'.root', outdir+'/'+nt[0]+'.root', args.option)
	if args.run_qsub:
		t = time.time()
		jobname = 'Presel_{}_{}_{}_{:06d}'.format(args.outdir, args.year, nt[0], int(t%1000000))
		os.system( Path.dir_tqHGG + '/qSub/submitJOB.py -c {} -N {}'.format(cmd, jobname)
	else:
		os.system( cmd.strip('"') )
	i_job += 1
	print ''

print '[INFO] Total job number: {}'.format(i_job)
