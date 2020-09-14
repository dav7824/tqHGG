#!/usr/bin/env python2

import Path, Util, Samples
import os, sys
from os.path import join

# I/O directories
indir_sig = join(Path.dir_2017, 'MVAreco_GenPerm')
indir_hadbkg = join(Path.dir_2017, 'Presel_had_phID_btag-L')
indir_lepbkg = join(Path.dir_2017, 'Presel_lep_phID')
outdir_had = join(Path.dir_2017, 'MVArecoV2_Perm_had')
Util.CreateDir(outdir_had)
outdir_lep = join(Path.dir_2017, 'MVArecoV3_Perm_lep')
Util.CreateDir(outdir_lep)

# Sample names
nt_sig_TThad = ['TT_FCNC-TtoHJ_aThad_hct', 'TT_FCNC-aTtoHJ_Thad_hct', 'TT_FCNC-TtoHJ_aThad_hut', 'TT_FCNC-aTtoHJ_Thad_hut']
nt_sig_SThad = ['ST_FCNC-TH_Thad_hct', 'ST_FCNC-TH_Thad_hut']
nt_sig_TTlep = ['TT_FCNC-TtoHJ_aTlep_hct', 'TT_FCNC-aTtoHJ_Tlep_hct', 'TT_FCNC-TtoHJ_aTlep_hut', 'TT_FCNC-aTtoHJ_Tlep_hut']
nt_sig_STlep = ['ST_FCNC-TH_Tlep_hct', 'ST_FCNC-TH_Tlep_hut']
nt_bkg = Samples.bkg_MC_s

# Executables
exe_TThad = join(Path.dir_bin, 'MVAreco_GenPerm_TThad')
exe_SThad = join(Path.dir_bin, 'MVAreco_GenPerm_SThad')
exe_TTlep = join(Path.dir_bin, 'MVAreco_GenPerm_TTlep')
exe_STlep = join(Path.dir_bin, 'MVAreco_GenPerm_STlep')

# Command template
cmd_TThad_sig = '{bin} {indir}/{{nt}}.root {outdir}/{{nt}}.root sig'.format(bin=exe_TThad, indir=indir_sig, outdir=outdir_had)
cmd_SThad_sig = '{bin} {indir}/{{nt}}.root {outdir}/{{nt}}.root sig'.format(bin=exe_SThad, indir=indir_sig, outdir=outdir_had)
cmd_TTlep_sig = '{bin} {indir}/{{nt}}.root {outdir}/{{nt}}.root sig'.format(bin=exe_TTlep, indir=indir_sig, outdir=outdir_lep)
cmd_STlep_sig = '{bin} {indir}/{{nt}}.root {outdir}/{{nt}}.root sig'.format(bin=exe_STlep, indir=indir_sig, outdir=outdir_lep)
cmd_TThad_bkg = '{bin} {indir}/{{nt}}.root {outdir}/{{nt}}.root bkg'.format(bin=exe_TThad, indir=indir_hadbkg, outdir=outdir_had)
cmd_SThad_bkg = '{bin} {indir}/{{nt}}.root {outdir}/{{nt}}.root bkg'.format(bin=exe_SThad, indir=indir_hadbkg, outdir=outdir_had)
cmd_TTlep_bkg = '{bin} {indir}/{{nt}}.root {outdir}/{{nt}}.root bkg'.format(bin=exe_TTlep, indir=indir_lepbkg, outdir=outdir_lep)
cmd_STlep_bkg = '{bin} {indir}/{{nt}}.root {outdir}/{{nt}}.root bkg'.format(bin=exe_STlep, indir=indir_lepbkg, outdir=outdir_lep)

mesg_sample = '---Start running: {}'
'''
# Run TT hadronic GenPerm
print 'Start processing: TThad'
output = open( join(outdir_had,'summary_TT.txt'), 'w' )

for sample in nt_sig_TThad:
	print mesg_sample.format(sample)
	output.write( '\n'+mesg_sample.format(sample)+'\n' )
	fp = os.popen( cmd_TThad_sig.format(nt=sample) )
	output.write( fp.read() )
	fp.close()
for cat in nt_bkg:
	for sample in nt_bkg[cat]:
		print mesg_sample.format(sample)
		output.write( '\n'+mesg_sample.format(sample)+'\n' )
		fp = os.popen( cmd_TThad_bkg.format(nt=sample) )
		output.write( fp.read() )
		fp.close()
print mesg_sample.format('data')
output.write( '\n'+mesg_sample.format('data')+'\n' )
fp = os.popen( cmd_TThad_bkg.format(nt='data') )
output.write( fp.read() )
fp.close()

output.close()

# Run ST hadronic GenPerm
print 'Start processing: SThad'
output = open( join(outdir_had,'summary_ST.txt'), 'w' )

for sample in nt_sig_SThad:
	print mesg_sample.format(sample)
	output.write( '\n'+mesg_sample.format(sample)+'\n' )
	fp = os.popen( cmd_SThad_sig.format(nt=sample) )
	output.write( fp.read() )
	fp.close()
for cat in nt_bkg:
	for sample in nt_bkg[cat]:
		print mesg_sample.format(sample)
		output.write( '\n'+mesg_sample.format(sample)+'\n' )
		fp = os.popen( cmd_SThad_bkg.format(nt=sample) )
		output.write( fp.read() )
		fp.close()
print mesg_sample.format('data')
output.write( '\n'+mesg_sample.format('data')+'\n' )
fp = os.popen( cmd_SThad_bkg.format(nt='data') )
output.write( fp.read() )
fp.close()

output.close()
'''
# Run TT leptonic GenPerm
print 'Start processing: TTlep'
output = open( join(outdir_lep,'summary_TT.txt'), 'w' )

for sample in nt_sig_TTlep:
	print mesg_sample.format(sample)
	output.write( '\n'+mesg_sample.format(sample)+'\n' )
	fp = os.popen( cmd_TTlep_sig.format(nt=sample) )
	output.write( fp.read() )
	fp.close()
for cat in nt_bkg:
	for sample in nt_bkg[cat]:
		print mesg_sample.format(sample)
		output.write( '\n'+mesg_sample.format(sample)+'\n' )
		fp = os.popen( cmd_TTlep_bkg.format(nt=sample) )
		output.write( fp.read() )
		fp.close()
print mesg_sample.format('data')
output.write( '\n'+mesg_sample.format('data')+'\n' )
fp = os.popen( cmd_TTlep_bkg.format(nt='data') )
output.write( fp.read() )
fp.close()

output.close()

# Run ST leptonic GenPerm
print 'Start processing: STlep'
output = open( join(outdir_lep,'summary_ST.txt'), 'w' )

for sample in nt_sig_STlep:
	print mesg_sample.format(sample)
	output.write( '\n'+mesg_sample.format(sample)+'\n' )
	fp = os.popen( cmd_STlep_sig.format(nt=sample) )
	output.write( fp.read() )
	fp.close()
for cat in nt_bkg:
	for sample in nt_bkg[cat]:
		print mesg_sample.format(sample)
		output.write( '\n'+mesg_sample.format(sample)+'\n' )
		fp = os.popen( cmd_STlep_bkg.format(nt=sample) )
		output.write( fp.read() )
		fp.close()
print mesg_sample.format('data')
output.write( '\n'+mesg_sample.format('data')+'\n' )
fp = os.popen( cmd_STlep_bkg.format(nt='data') )
output.write( fp.read() )
fp.close()

output.close()

print 'End!'
