#!/usr/bin/env python2
#
# Merge TT & ST trees for output from MVAreco_application. For files containing only TT or ST tree,
# the varaibles not used are filled with values like -999, so that output tree variables are the
# same for all kinds of input files.
#
# Usage:
#   ./MVAreco_MergeResult.py <method_name>
#
#   <method_name>: Name of the MVA method. Used as the name of output tree and output dir.

import Path
import Util
import os
import sys

def PrintRun( cmd ):
	print 'Running: ', cmd
	os.system( cmd )

method = sys.argv[1]

# I/O directories
dir_had_base = Path.dir_2017 + '/MVAreco_result_had_btag-M_phID'
dir_lep_base = Path.dir_2017 + '/MVAreco_result_lep_btag-L_phID'
indir_had = dir_had_base + '/' + method
indir_lep = dir_lep_base + '/' + method
outdir_had = dir_had_base + '/' + method + '_'
outdir_lep = dir_lep_base + '/' + method + '_'
Util.CreateDir(outdir_had)
Util.CreateDir(outdir_lep)
dir_evt_had = Path.dir_2017 + '/Presel_had_btag-M_phID'
dir_evt_lep = Path.dir_2017 + '/Presel_lep_btag-L_phID'

# Executable
exe = Path.dir_bin + '/MVAreco_MergeResult'
# List file for bkg MC & data
flist = Path.dir_list + '/datasets_2017.txt'

# Names of signal samples
nt_sig_TThad = ["TT_FCNC-TtoHJ_aThad_hct", "TT_FCNC-TtoHJ_aThad_hut", "TT_FCNC-aTtoHJ_Thad_hct", "TT_FCNC-aTtoHJ_Thad_hut"]
nt_sig_SThad = ["ST_FCNC-TH_Thad_hct", "ST_FCNC-TH_Thad_hut"]
nt_sig_TTlep = ["TT_FCNC-TtoHJ_aTlep_hct", "TT_FCNC-TtoHJ_aTlep_hut", "TT_FCNC-aTtoHJ_Tlep_hct", "TT_FCNC-aTtoHJ_Tlep_hut"]
nt_sig_STlep = ["ST_FCNC-TH_Tlep_hct", "ST_FCNC-TH_Tlep_hut"]
# Names of bkg are read from the list
nt_bkg = Util.ReadList( flist )

# Command templates
cmd_had_template = exe + " {evtdir}/{{sample}}.root {indir}/{{sample}}.root {outdir}/{{sample}}.root {{nttype}} had {tag}".format( 
		evtdir=dir_evt_had, indir=indir_had, outdir=outdir_had, tag=method )
cmd_lep_template = exe + " {evtdir}/{{sample}}.root {indir}/{{sample}}.root {outdir}/{{sample}}.root {{nttype}} lep {tag}".format( 
		evtdir=dir_evt_lep, indir=indir_lep, outdir=outdir_lep, tag=method )

'''
# Process signal MC
for nt in nt_sig_TThad:
	cmd = cmd_had_template.format( sample=nt, nttype='TT' )
	PrintRun(cmd)
for nt in nt_sig_SThad:
	cmd = cmd_had_template.format( sample=nt, nttype='ST' )
	PrintRun(cmd)
for nt in nt_sig_TTlep:
	cmd = cmd_lep_template.format( sample=nt, nttype='TT' )
	PrintRun(cmd)
for nt in nt_sig_STlep:
	cmd = cmd_lep_template.format( sample=nt, nttype='ST' )
	PrintRun(cmd)
'''

# Process bkg MC & data
for nt in nt_bkg:
	cmd = cmd_had_template.format( sample=nt[0], nttype='bkg' )
	PrintRun(cmd)
	cmd = cmd_lep_template.format( sample=nt[0], nttype='bkg' )
	PrintRun(cmd)

print '[MVAreco_MergeResult.py] Completed!'
