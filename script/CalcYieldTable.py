#!/usr/bin/env python2

import Path, Util, Samples
import os, sys
from os.path import join

# Tag of input samples (dir name without prefix 'Presel_')
input = sys.argv[1]
# Channel
ch = sys.argv[2]

if ch!='all' and ch!='had' and ch!='lep':
	print '[ERROR] Invalid channel!'
	sys.exit(1)

# Executables
exe_CalcYield =	join(Path.dir_bin, 'CalcYield')
exe_AddYields =	join(Path.dir_bin, 'AddYields')
exe_PrintYieldSummary =	join(Path.dir_bin, 'PrintYieldSummary')
exe_PrintYieldSummary_sig =	join(Path.dir_bin, 'PrintYieldSummary_sig')
# Input dir
indir = join(Path.dir_2017, 'Presel_'+input)
# Norm dir
normdir = join(Path.dir_2017, 'normfactor')
# Temp dir
tmpdir = join(Path.dir_tqHGG, 'tmp', 'yields_presel_'+input)
Util.CreateDir(tmpdir)
# Output dir
outdir = join(Path.dir_2017, 'stackedplots')
Util.CreateDir(outdir)

# Command templates
cmd_CalcYield = '{bin} {fin} {name} {tree} {norm} {fout} {log}'.format(
		bin=exe_CalcYield, fin=join(indir,'{sample}.root'), name='{sample}', tree='T',
		norm=join(normdir,'{sample}.root'), fout=join(tmpdir,'{sample}.root'), log=join(tmpdir,'CalcYield.log')
)

cmd_AddYields = '{bin} {{cat}} {{fins}} {indir} {fout} {log}'.format(
		bin=exe_AddYields, indir=tmpdir, fout=join(tmpdir,'{cat}.root'), log=join(tmpdir,'AddYields.log')
)
cmd_mv = 'cd {dir}; mv {{oldname}}.root {{newname}}.root'.format(dir=tmpdir)

cmd_PrintYieldSummary = '{bin} {fins} {{samples}} {indir} {fdata} {dataTree} {summary}'.format(
		bin=exe_PrintYieldSummary, fins='{samples}', indir=tmpdir, fdata=join(indir,'data.root'), dataTree='T',
		summary=join(outdir,'Yields_'+input+'.txt')
)
cmd_PrintYieldSummary_sig = '{bin} {fins} {{samples}} {indir} {summary}'.format(
		bin=exe_PrintYieldSummary_sig, fins='{samples}', indir=tmpdir,
		summary=join(outdir,'Yields_'+input+'.txt')
)


# Run CalcYield
print '[INFO] Start running: CalcYield'
# Process sig MC
for nt_type in Samples.sig_MC:
	if ch=='had' and nt_type[1]=='lep':
		continue
	if ch=='lep' and nt_type[1]=='had':
		continue
	for nt in Samples.sig_MC[nt_type]:
		print 'Processing:', nt[0]
		fp = os.popen( cmd_CalcYield.format(sample=nt[0]) )
		fp.close()
# Process bkg MC
for cat in Samples.bkg_MC:
	for nt in Samples.bkg_MC[cat]:
		print 'Processing:', nt[0]
		fp = os.popen( cmd_CalcYield.format(sample=nt[0]) )
		fp.close()


# Run AddYields
print '\n[INFO] Start running: AddYields'
# Process sig MC
for nt_type in Samples.sig_MC:
	if ch=='had' and nt_type[1]=='lep':
		continue
	if ch=='lep' and nt_type[1]=='had':
		continue
	cat_name = ''.join(nt_type)
	print 'Processing:', cat_name
	# If there's only 1 sample in the list, change it's name to category name
	if len( Samples.sig_MC[nt_type] ) == 1:
		os.system( cmd_mv.format(oldname=Samples.sig_MC[nt_type][0][0], newname=cat_name) )
	# If there are more than 1 sample in the list, run AddYields and save result to 'category.root'
	else:
		lsamples = []
		for nt in Samples.sig_MC[nt_type]:
			lsamples.append(nt[0])
		fp = os.popen( cmd_AddYields.format(cat=cat_name, fins=','.join(lsamples)) )
		fp.close()
# Process bkg MC
for cat in Samples.bkg_MC:
	print 'Processing:', cat
	# If there's only 1 sample in the list, change it's name to category name
	if len( Samples.bkg_MC[cat] ) == 1:
		os.system( cmd_mv.format(oldname=Samples.bkg_MC[cat][0][0], newname=cat) )
	# If there are more than 1 sample in the list, run AddYields and save result to 'category.root'
	else:
		lsamples = []
		for nt in Samples.bkg_MC[cat]:
			lsamples.append(nt[0])
		fp = os.popen( cmd_AddYields.format(cat=cat, fins=','.join(lsamples)) )
		fp.close()


# Run PrintYieldSummary_sig
print '\n[INFO] Start running: PrintYieldSummary_sig'
samples_sig = []
for nt_type in Samples.sig_MC:
	if ch=='had' and nt_type[1]=='lep':
		continue
	if ch=='lep' and nt_type[1]=='had':
		continue
	cat_name = ''.join(nt_type)
	samples_sig.append(cat_name)
fp = os.popen( cmd_PrintYieldSummary_sig.format(samples=','.join(samples_sig)) )
fp.close()
# Run PrintYieldSummary
print '[INFO] Start running: PrintYieldSummary'
samples_bkg = []
for cat in Samples.bkg_MC:
	samples_bkg.append(cat)
fp = os.popen( cmd_PrintYieldSummary.format(samples=','.join(samples_bkg)) )
fp.close()

print 'End!'
