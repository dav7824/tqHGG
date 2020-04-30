#!/usr/bin/env python2

import Path
import Util
import os

# Option for running with qSub
run_qSub = True

# Binary to run
exe = Path.dir_bin+'/Presel_basic'

if not os.path.exists(exe):
	print '[ERROR] Binary file doesn\'t exist!'
	exit()

# List of input n-tuples
flists = ['datasets_2016.txt', 'datasets_2017.txt', 'datasets_2017_sig.txt', 'datasets_2018.txt']

# Directory of input n-tuples
indirs = [Path.dir_nt_2016 , Path.dir_nt_2017 , Path.dir_nt_2017_sig , Path.dir_nt_2018]

# Directory of output n-tuples
outdirs = [Path.dir_2016, Path.dir_2017, Path.dir_2017, Path.dir_2018]

jobnames = ['Presel_basic_2016_{}', 'Presel_basic_2017_{}', 'Presel_basic_2017_sig_{}', 'Presel_basic_2018_{}']

for i in range(len(indirs)):
	print '[INFO] Start processing list: {}'.format(flists[i])

	flist = Path.dir_list+'/'+flists[i]
	if not os.path.exists(flist):
		print '[ERROR] List file doesn\'t exist!'
		exit()

	indir = indirs[i]
	if not os.path.exists(indir):
		print '[ERROR] Input directory doesn\'t exist!'
		exit()

	outdir = outdirs[i]+'/Presel_basic'
	Util.CreateDir(outdir)

	ls = Util.ReadList(flist)

	n_job = 0
	for nt in ls:
		fin = indir+'/'+nt[1]
		fout = outdir+'/'+nt[0]+'.root'
		# Command to run
		cmd = '"set -o noglob; {} {} flashggNtuples/flashggStdTree {}"'.format(exe, fin, fout)
		if run_qSub:
			jobname = jobnames[i].format(nt[0])
			os.system( Path.dir_tqHGG + '/qSub/submitJOB.py -c {} -N {}'.format(cmd, jobname) )
		else:
			os.system( cmd.strip('"') )
		n_job += 1

	print '[INFO] Total job number: {}'.format(n_job)
	print ''
