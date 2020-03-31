#!/usr/bin/env python2
import Path
import Util
import os

# Output directory
outDir = Path.dir_2018+'/Presel_a'
Util.CreateDir( outDir )

# Executable
exe = Path.dir_bin+'/Presel_a'

# List of bkg MC + data
ls = Util.ReadList(Path.dir_list+'/datasets_2018.txt')
# List of sig MC
#ls_sig = Util.ReadList(Path.dir_list+'/datasets_2018_sig.txt')

i_job = 0
# Submit jobs for bkg MC + data
for dataset in ls:
	print 'Start processing dataset: {}'.format(dataset[0])
	cmd = '"set -o noglob; {} {} flashggNtuples/flashggStdTree {}"'.format(exe, Path.dir_nt_2018+'/'+dataset[1], outDir+'/'+dataset[0]+'.root')
	os.system( Path.dir_tqHGG + '/qSub/submitJOB.py -c {} -N presel18_{:02d}'.format(cmd, i_job) )
	i_job += 1
	print '\n'

# Submit jobs for sigMC
#for dataset in ls_sig:
#	print 'Start processing dataset: {}'.format(dataset[0])
#	cmd = '"set -o noglob; {} {} flashggNtuples/flashggStdTree {}"'.format(exe, Path.dir_nt_2018_sig+'/'+dataset[1], outDir+'/'+dataset[0]+'.root')
#	os.system( Path.dir_tqHGG + '/qSub/submitJOB.py -c {} -N presel18_{:02d}'.format(cmd, i_job) )
#	i_job += 1
#	print '\n'

print '[INFO] Completed submitting {} jobs.'.format(i_job)
