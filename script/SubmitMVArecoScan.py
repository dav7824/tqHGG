#!/usr/bin/env python2
# Usage:
#   ./SubmitMVArecoScan.py <input.txt>
#
# Each line in the input file submits a job with specified DNN hyperparameters.  The job will train & apply NN
# with the signal samples correspond to specified <recotype> & <channel>.  The matching rate of each sample will
# be calculated and appended to the end of MatchRate_<recotype><ch>.log.
#
# *** Fields of input file:
# <Job Name> <Recotype=TT|ST> <Channel=had|lep> <Tag> <Option:Error> <Option:Layout> <Option:Training_Strategy>
#
# *** Ways to specify comments in input file:
# (Comments are always whole lines!)
# 1. '#'
#    A line with a '#' as the first non-whitespace character is a comment.
# 2. "'''"
#    All lines btw 2 different lines of "'''" only are comments.

import Path, Util
import os, sys, re
from os.path import join

dbg_print = False

script_submit = join(Path.dir_tqHGG, 'qSub/SubmitMVArecoScan.sh')

jobCmd_template = '''export recotype={recotype}
export ch={ch}
export tag="{tag}"
export opt_err="{opt_err}"
export opt_layout="{opt_layout}"
export opt_strat="{opt_strat}"

qsub {script} -N {name}
'''

def Scan(input):
	# Read input file
	flag_comment = False

	fin = open(input)
	# Start input file line loop
	for line in fin:
		line = line.strip()
		if line=="'''":
			flag_comment = not flag_comment
			continue
		if flag_comment or re.match(r'#', line) or not line:
			continue
		if dbg_print:
			print line
			continue
		# If the line is not a comment, process it and submit job
		jobdesc = line.split()
		jobname = jobdesc[0]
		recotype = jobdesc[1]
		ch = jobdesc[2]

		if recotype!='TT' and recotype!='ST':
			print '[ERROR] Job: {}: Invalid recotype!'.format(jobname)
			continue
		if ch!='had' and ch!='lep':
			print '[ERROR] Job: {}: Invalid channel!'.format(jobname)
			continue

		# Create job directory
		dir_job = join(Path.dir_2017, 'MVAreco_scan', jobname)
		os.system('rm -r {}'.format(dir_job))
		Util.CreateDir(dir_job)
		# Submit the job at job dir, so the stdout & stderr will be automatically saved there.
		os.chdir(dir_job)

		# Submit job
		jobCmd = jobCmd_template.format( recotype=recotype, ch=ch, tag=jobdesc[3], opt_err=jobdesc[4], opt_layout=jobdesc[5], opt_strat=jobdesc[6],
				script=script_submit, name=jobname )
		fp = os.popen(jobCmd)
		print 'Job: {} submitted successfully.  (ID={})'.format(jobname, fp.read().strip())
		fp.close()
	# End input file line loop

	fin.close()
# End of function "Scan"


if __name__ == '__main__':
	fin_name = sys.argv[1]
	Scan(fin_name)
	print 'All jobs submitted!'
