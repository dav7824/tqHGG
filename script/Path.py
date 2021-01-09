#!/usr/bin/env python2
import commands

dir_tqHGG = '/wk_cms2/mc_cheng/FCNH/CMSSW_9_4_11/src/tqHGG_DeepJet/tqHGG'
dir_res = commands.getoutput('echo $DIR_PROD')

dir_2016 = dir_res + '/2016'
dir_2017 = dir_res + '/2017_DeepJet'
dir_2018 = dir_res + '/2018'
dir_run2 = dir_res + '/RunII'

dir_bin = dir_tqHGG + '/bin'
dir_src = dir_tqHGG + '/src'
dir_script = dir_tqHGG + '/script'
dir_list = dir_tqHGG + '/list'

#dir_nt_2016 = '/wk_cms2/ykao/public/thFCNC/flashgg_105X/2016'
#dir_nt_2017 = '/wk_cms2/youying/public/thFCNC/flashgg_105X/2017'
#dir_nt_2017_sig = '/wk_cms2/youying/public/tH_FCNC/Era2017_RR-31Mar2018_v2'
#dir_nt_2018 = '/wk_cms2/youying/public/thFCNC/flashgg_105X/2018'

# New 2017 n-tuples with event number & jet flavour
# Data & non-resonant background MC (without systematics)
dir_nt_2017 = '/wk_cms2/youying/public/Era2017_RR-31Mar2018'
# Signal & resonant background MC (with systematics)
dir_nt_2017_Higgs = '/wk_cms/youying/public/Era2017_RR-31Mar2018/syst'

dir_norm_2017 = dir_2017 + '/normfactor'
