#!/usr/bin/env python2
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
