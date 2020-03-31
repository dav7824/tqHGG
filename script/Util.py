#!/usr/bin/env python2
from Path import dir_tqHGG
import os
import re

def CreateDir(dir):
	if not os.path.exists(dir):
		os.mkdir(dir)
	else:
		print '[INFO] The directory "{}" already exists.'.format(dir)


def CreateDirs(dirpath):
	if not os.path.exists(dirpath):
		os.makedirs(dirpath)
	else:
		print '[INFO] The dirpath "{}" already exists.'.format(dirpath)


#def SubmitJob(cmd, name):
#	os.system( '{}/qSub/submitJOB.py -c {} -N {}'.format(dir_tqHGG, cmd, name) )


def ReadList(path):
	fin = open(path, 'r')
	ls = []
	for line in fin:
		if line.strip() == '':
			continue
		res = re.match(r'#', line.lstrip())
		if res:
			continue
		ls.append( line.split() )

	fin.close()
	return ls
