#!/usr/bin/env python2

import Path
import os, sys
from os.path import join

def Run(nt):
    exe = join(Path.dir_bin, 'SFcalc_btag')
    fin = join(Path.dir_2017, 'Presel_had_phID_btag-L', nt+'.root')
    fout = join(Path.dir_tqHGG, 'test_'+nt+'.root')
    fcsv = join(Path.dir_2017, 'SF', 'DeepCSV_94XSF_V5_B_F.csv')

    os.system('{} {} {} {}'.format(exe, fin, fcsv, fout))
    print 'Complete!'

if __name__ == '__main__':
    #Run('DiPhotonJetsBox')
    Run('TT_FCNC-TtoHJ_aThad_hct')
