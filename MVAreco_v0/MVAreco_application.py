#!/usr/bin/env python2
# Usage:
#   ./MVAreco_application.py <MVA method of weight files> <MVA name tag>
#
# <MVA method of weight files> specifies the weight files to be used, while <MVA name tag> doesn't affect the actual process
# and is for display purpose. For clearity it's better to let <MVA name tag> the same as when running MVAreco_training.

import Path
import Util
import os
import sys

def PrintRun( cmd ):
	print 'Running: ', cmd
	os.system( cmd )


# MVA method of weight files
method = sys.argv[1]
# MVA name tag
train_tag = sys.argv[2]
# Names of i/o directories
indir_had_name = "MVAreco_perm_had_btag-M_phID"
indir_lep_name = "MVAreco_perm_lep_btag-L_phID"
outdir_had_name = "MVAreco_result_had_btag-M_phID/"+method
outdir_lep_name = "MVAreco_result_lep_btag-L_phID/"+method


# Names of weight files
template_fweight = "{{}}_{}.weights.xml".format( method )
fweight_TThad = template_fweight.format( "TThad" )
fweight_SThad = template_fweight.format( "SThad" )
fweight_TTlep = template_fweight.format( "TTlep" )
fweight_STlep = template_fweight.format( "STlep" )

# Paths to executable, list, i/o dir of had/lep channel
exe = Path.dir_bin+"/MVAreco_application"
flist = Path.dir_list+"/datasets_2017.txt"
dir_weight = Path.dir_2017+"/MVAreco_training/dataset/weights"
indir_had = Path.dir_2017+"/"+indir_had_name
indir_lep = Path.dir_2017+"/"+indir_lep_name
outdir_had = Path.dir_2017+"/"+outdir_had_name
outdir_lep = Path.dir_2017+"/"+outdir_lep_name

# Create output dir
Util.CreateDirs( outdir_had )
Util.CreateDirs( outdir_lep )

# Names of samples
nt_sig_TThad = ["TT_FCNC-TtoHJ_aThad_hct", "TT_FCNC-TtoHJ_aThad_hut", "TT_FCNC-aTtoHJ_Thad_hct", "TT_FCNC-aTtoHJ_Thad_hut"]
nt_sig_SThad = ["ST_FCNC-TH_Thad_hct", "ST_FCNC-TH_Thad_hut"]
nt_sig_TTlep = ["TT_FCNC-TtoHJ_aTlep_hct", "TT_FCNC-TtoHJ_aTlep_hut", "TT_FCNC-aTtoHJ_Tlep_hct", "TT_FCNC-aTtoHJ_Tlep_hut"]
nt_sig_STlep = ["ST_FCNC-TH_Tlep_hct", "ST_FCNC-TH_Tlep_hut"]
# Names of bkg are read from the list
nt_bkg = Util.ReadList( flist )

# Command templates
cmd_had_template = exe + " {indir}/{{sample}}.root {weightdir}/{{fweight}} {outdir}/{{sample}}.root {{nttype}} {{recotype}} had {tag}".format( 
		indir=indir_had, weightdir=dir_weight, outdir=outdir_had, tag=train_tag )
cmd_lep_template = exe + " {indir}/{{sample}}.root {weightdir}/{{fweight}} {outdir}/{{sample}}.root {{nttype}} {{recotype}} lep {tag}".format( 
		indir=indir_lep, weightdir=dir_weight, outdir=outdir_lep, tag=train_tag )

# Run TThad signal
for nt in nt_sig_TThad:
	cmd = cmd_had_template.format( sample=nt, fweight=fweight_TThad, nttype='sig', recotype='TT' )
	PrintRun( cmd )
# Run SThad signal
for nt in nt_sig_SThad:
	cmd = cmd_had_template.format( sample=nt, fweight=fweight_SThad, nttype='sig', recotype='ST' )
	PrintRun( cmd )
# Run TTlep signal
for nt in nt_sig_TTlep:
	cmd = cmd_lep_template.format( sample=nt, fweight=fweight_TTlep, nttype='sig', recotype='TT' )
	PrintRun( cmd )
# Run STlep signal
for nt in nt_sig_STlep:
	cmd = cmd_lep_template.format( sample=nt, fweight=fweight_STlep, nttype='sig', recotype='ST' )
	PrintRun( cmd )

# Run had bkg
for nt in nt_bkg:
	cmd_TT = cmd_had_template.format( sample=nt[0], fweight=fweight_TThad, nttype='bkg', recotype='TT' )
	cmd_ST = cmd_had_template.format( sample=nt[0], fweight=fweight_SThad, nttype='bkg', recotype='ST' )
	PrintRun( cmd_TT )
	PrintRun( cmd_ST )
# Run lep bkg
for nt in nt_bkg:
	cmd_TT = cmd_lep_template.format( sample=nt[0], fweight=fweight_TTlep, nttype='bkg', recotype='TT' )
	cmd_ST = cmd_lep_template.format( sample=nt[0], fweight=fweight_STlep, nttype='bkg', recotype='ST' )
	PrintRun( cmd_TT )
	PrintRun( cmd_ST )

print '\n[MVAreco_application.py] Completed!'
