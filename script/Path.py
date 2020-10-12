#!/usr/bin/env python2
import commands

dir_tqHGG = commands.getoutput('echo $DIR_TQHGG')
dir_res = commands.getoutput('echo $DIR_PROD')

dir_2016 = dir_res + '/2016'
dir_2017 = dir_res + '/2017'
dir_2018 = dir_res + '/2018'
dir_run2 = dir_res + '/RunII'

dir_bin = dir_tqHGG + '/bin'
dir_src = dir_tqHGG + '/src'
dir_script = dir_tqHGG + '/script'
dir_list = dir_tqHGG + '/list'

dir_nt_2016 = '/wk_cms2/ykao/public/thFCNC/flashgg_105X/2016'
dir_nt_2017 = '/wk_cms2/youying/public/thFCNC/flashgg_105X/2017'
#dir_nt_2017_sig = '/wk_cms2/youying/public/tH_FCNC/Era2017_RR-31Mar2018_v2'
dir_nt_2018 = '/wk_cms2/youying/public/thFCNC/flashgg_105X/2018'

# New 2017 signal MC with event number & jet flavour
dir_nt_2017_sig = '/wk_cms2/youying/public/2017_94X_3_1_X_and_3_2_0/newFCNC'

dir_norm_2017 = dir_2017 + '/normfactor'
