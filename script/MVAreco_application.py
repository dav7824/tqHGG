#!/usr/bin/env python2
#
# Apply trained MVA to event permutations.
#
# Usage:
#   ./MVAreco_application <train_tag>
#
# <train_tag>: Name tag for MVA method

import Path, Util, Samples
import sys, os
from os.path import join, exists

def RunApp(cmd, log):
	out = open(log, 'w')
	fp = os.popen( cmd )
	out.write( fp.read() )
	out.close()
	fp.close()

# Name tag for the MVA (used to specify the weight file and as part of the output directory names)
# Should be the same as the "method name" specified when booking MVA in training code, so this script
# can find the correct weight files.
train_tag = sys.argv[1]

# Executable
exe = join(Path.dir_bin, 'MVAreco_application')

# Input dir
indir_had = join(Path.dir_2017, 'MVAreco_Perm_had_v2')
indir_lep = join(Path.dir_2017, 'MVAreco_Perm_lep_v2')
# Output dir
outdir_had = join(Path.dir_2017, 'MVAreco_result_had_v2', train_tag)
outdir_lep = join(Path.dir_2017, 'MVAreco_result_lep_v2', train_tag)
logdir_had = join(outdir_had, 'log')
logdir_lep = join(outdir_lep, 'log')
# Log dir
Util.CreateDir(outdir_had)
Util.CreateDir(outdir_lep)
Util.CreateDir(logdir_had)
Util.CreateDir(logdir_lep)
# Weight file dir
weidir = join(Path.dir_2017, 'MVAreco_train_v2/dataset/weights')

# Basic templates
fweight_t = '{reco_type}{ch}_%s.weights.xml' % train_tag
#./MVAreco_application <fin> <fweight> <fout> <nt_type=sig|bkg> <reco_type=TT|ST> <channel=had|lep>
cmd_t = '%s {indir}/{{nt}}.root %s/{fweight} {outdir}/{{nt}}.root {nt_type} {reco_type} {ch}' % (exe, weidir)
logpath_t = '{dir}/{{nt}}__{reco_type}.log'

# Weight file name template
fweight_TThad = fweight_t.format(reco_type='TT', ch='had')
fweight_SThad = fweight_t.format(reco_type='ST', ch='had')
fweight_TTlep = fweight_t.format(reco_type='TT', ch='lep')
fweight_STlep = fweight_t.format(reco_type='ST', ch='lep')
# Command template
cmd_TThad_sig = cmd_t.format(indir=indir_had, fweight=fweight_TThad, outdir=outdir_had, nt_type='sig', reco_type='TT', ch='had')
cmd_SThad_sig = cmd_t.format(indir=indir_had, fweight=fweight_SThad, outdir=outdir_had, nt_type='sig', reco_type='ST', ch='had')
cmd_TTlep_sig = cmd_t.format(indir=indir_lep, fweight=fweight_TTlep, outdir=outdir_lep, nt_type='sig', reco_type='TT', ch='lep')
cmd_STlep_sig = cmd_t.format(indir=indir_lep, fweight=fweight_STlep, outdir=outdir_lep, nt_type='sig', reco_type='ST', ch='lep')
cmd_TThad_bkg = cmd_t.format(indir=indir_had, fweight=fweight_TThad, outdir=outdir_had, nt_type='bkg', reco_type='TT', ch='had')
cmd_SThad_bkg = cmd_t.format(indir=indir_had, fweight=fweight_SThad, outdir=outdir_had, nt_type='bkg', reco_type='ST', ch='had')
cmd_TTlep_bkg = cmd_t.format(indir=indir_lep, fweight=fweight_TTlep, outdir=outdir_lep, nt_type='bkg', reco_type='TT', ch='lep')
cmd_STlep_bkg = cmd_t.format(indir=indir_lep, fweight=fweight_STlep, outdir=outdir_lep, nt_type='bkg', reco_type='ST', ch='lep')
# Log file path template
log_TThad = logpath_t.format(dir=logdir_had, reco_type='TT')
log_SThad = logpath_t.format(dir=logdir_had, reco_type='ST')
log_TTlep = logpath_t.format(dir=logdir_lep, reco_type='TT')
log_STlep = logpath_t.format(dir=logdir_lep, reco_type='ST')

# Run TThad reconstruction
print '[INFO] Apply MVA TThad_%s' % train_tag
for nt in Samples.sig_MC_expr_v2[('TT','had')]:
	print 'Processing:', nt
	RunApp(cmd_TThad_sig.format(nt=nt), log_TThad.format(nt=nt))
for cat in Samples.bkg_MC_s:
	for nt in Samples.bkg_MC_s[cat]:
		print 'Processing:', nt
		RunApp(cmd_TThad_bkg.format(nt=nt), log_TThad.format(nt=nt))
print 'Processing: Data'
RunApp(cmd_TThad_bkg.format(nt='data'), log_TThad.format(nt='data'))

# Run SThad reconstruction
print '[INFO] Apply MVA SThad_%s' % train_tag
for nt in Samples.sig_MC_expr_v2[('ST','had')]:
	print 'Processing:', nt
	RunApp(cmd_SThad_sig.format(nt=nt), log_SThad.format(nt=nt))
for cat in Samples.bkg_MC_s:
	for nt in Samples.bkg_MC_s[cat]:
		print 'Processing:', nt
		RunApp(cmd_SThad_bkg.format(nt=nt), log_SThad.format(nt=nt))
print 'Processing: Data'
RunApp(cmd_SThad_bkg.format(nt='data'), log_SThad.format(nt='data'))

# Run TTlep reconstruction
print '[INFO] Apply MVA TTlep_%s' % train_tag
for nt in Samples.sig_MC_expr_v2[('TT','lep')]:
	print 'Processing:', nt
	RunApp(cmd_TTlep_sig.format(nt=nt), log_TTlep.format(nt=nt))
for cat in Samples.bkg_MC_s:
	for nt in Samples.bkg_MC_s[cat]:
		print 'Processing:', nt
		RunApp(cmd_TTlep_bkg.format(nt=nt), log_TTlep.format(nt=nt))
print 'Processing: Data'
RunApp(cmd_TTlep_bkg.format(nt='data'), log_TTlep.format(nt='data'))

# Run STlep reconstruction
print '[INFO] Apply MVA STlep_%s' % train_tag
for nt in Samples.sig_MC_expr_v2[('ST','lep')]:
	print 'Processing:', nt
	RunApp(cmd_STlep_sig.format(nt=nt), log_STlep.format(nt=nt))
for cat in Samples.bkg_MC_s:
	for nt in Samples.bkg_MC_s[cat]:
		print 'Processing:', nt
		RunApp(cmd_STlep_bkg.format(nt=nt), log_STlep.format(nt=nt))
print 'Processing: Data'
RunApp(cmd_STlep_bkg.format(nt='data'), log_STlep.format(nt='data'))

print 'End!'
