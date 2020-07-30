#!/usr/bin/env python2

import os, sys
import Path, Util, Samples
from os.path import join

# Tag for input samples (dir name)
tag = sys.argv[1]
# Channel of samples
ch = sys.argv[2]

if ch!='all' and ch!='had' and ch!='lep':
	print '[ERROR] Invalid channel!'
	sys.exit(1)

# Setting directories
exe_HistNorm = join(Path.dir_bin, 'HistNorm')
exe_HistAdd = join(Path.dir_bin, 'HistAdd')
indir = join(Path.dir_2017, "stackedplots", "hist_"+tag)
normdir = join(Path.dir_2017, "normfactor")
tmpdir = join(Path.dir_tqHGG, "tmp", "hist_"+tag)
Util.CreateDir(tmpdir)


# Normalize histograms
print '[INFO] Start running: HistNorm'
cmd_HistNorm = '{bin} {{fin}} {{fnorm}} {{fout}}'.format(bin=exe_HistNorm)
# Process signal MC
for nt_type in Samples.sig_MC_s:
	if ch=='had' and nt_type[1]=='lep':
		continue
	if ch=='lep' and nt_type[1]=='had':
		continue
	for nt in Samples.sig_MC_s[nt_type]:
		print 'Processing:', nt
		fp = os.popen( cmd_HistNorm.format( fin=join(indir,nt+'.root'), fnorm=join(normdir,nt+'.root'), fout=join(tmpdir,nt+'.root') ) )
		fp.close()
# Process bkg MC
for cat in Samples.bkg_MC_s:
	for nt in Samples.bkg_MC_s[cat]:
		print 'Processing:', nt
		fp = os.popen( cmd_HistNorm.format( fin=join(indir,nt+'.root'), fnorm=join(normdir,nt+'.root'), fout=join(tmpdir,nt+'.root') ) )
		fp.close()


# Add histograms
print '[INFO] Start running: HistAdd'
cmd_HistAdd = '{bin} {{finlist}} {indir} {{fout}}'.format(bin=exe_HistAdd, indir=tmpdir)
cmd_mv = 'cd {dir}; mv {{oldname}}.root {{newname}}.root'.format(dir=tmpdir)
# Process signal MC
for nt_type in Samples.sig_MC_s:
	if ch=='had' and nt_type[1]=='lep':
		continue
	if ch=='lep' and nt_type[1]=='had':
		continue
	cat_name = ''.join(nt_type)
	print 'Processing:', cat_name
	if len( Samples.sig_MC_s[nt_type] )==1:
		os.system( cmd_mv.format( oldname=Samples.sig_MC_s[nt_type][0], newname=cat_name ) )
	else:
		fp = os.popen( cmd_HistAdd.format( finlist=','.join(Samples.sig_MC_s[nt_type]), fout=join(tmpdir,cat_name+'.root') ) )
		fp.close()
# Process bkg MC
for cat in Samples.bkg_MC_s:
	print 'Processing:', cat
	if len( Samples.bkg_MC_s[cat] )==1:
		os.system( cmd_mv.format( oldname=Samples.bkg_MC_s[cat][0], newname=cat ) )
	else:
		fp = os.popen( cmd_HistAdd.format( finlist=','.join(Samples.bkg_MC_s[cat]), fout=join(tmpdir,cat+'.root') ) )
		fp.close()
# Add all MC bkg
print 'Processing: total bkg MC'
fp = os.popen( cmd_HistAdd.format( finlist=','.join(Samples.bkg_MC_s.keys()), fout=join(tmpdir,'totbkg.root') ) )
fp.close()

print '[INFO] HistNormAdd.py completed'
