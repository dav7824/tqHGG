#!/usr/bin/env python2

import Path, Util, Samples
import sys, os
from os.path import join

exe = join(Path.dir_bin, 'FindGenPart')
indir = join(Path.dir_2017, 'MVAreco_GenPerm')

cmd_template = '{bin} {{fin}} T {{type}} {{ch}}'.format(bin=exe)

#os.system(cmd_template.format(fin=join(indir,'TT_FCNC-TtoHJ_aThad_hct.root'), type='TT', ch='had'))

for key in Samples.sig_MC_s:
    for nt in Samples.sig_MC_s[key]:
        print 'Processing:', nt
        fp = os.popen( cmd_template.format(fin=join(indir,nt+'.root'), type=key[0], ch=key[1]) )
        print fp.read()
        fp.close()

print 'End!'
