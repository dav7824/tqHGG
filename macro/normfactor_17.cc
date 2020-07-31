// 2017 datasets

void norm_sig()
{
	TString nt_dir = "/wk_cms2/youying/public/tH_FCNC/Era2017_RR-31Mar2018_v2/";
	TString outdir = "/wk_cms2/mc_cheng/public/tqHGG/2017/normfactor/";
	vector<TString> filename = {
		"TT_FCNC-TtoHJ_aThadronic_HToaa_eta_hct-MadGraph5-pythia8.root",
		"TT_FCNC-TtoHJ_aThadronic_HToaa_eta_hut-MadGraph5-pythia8.root",
		"TT_FCNC-aTtoHJ_Thadronic_HToaa_eta_hct-MadGraph5-pythia8.root",
		"TT_FCNC-aTtoHJ_Thadronic_HToaa_eta_hut-MadGraph5-pythia8.root",
		"TT_FCNC-T2HJ_aTleptonic_HToaa_eta_hct-MadGraph5-pythia8.root",
		"TT_FCNC-TtoHJ_aTleptonic_HToaa_eta_hut-MadGraph5-pythia8.root",
		"TT_FCNC-aTtoHJ_Tleptonic_HToaa_eta_hct-MadGraph5-pythia8.root",
		"TT_FCNC-aTtoHJ_Tleptonic_HToaa_eta_hut-MadGraph5-pythia8.root",
		"ST_FCNC-TH_Thadronic_HToaa_eta_hct-MadGraph5-pythia8.root",
		"ST_FCNC-TH_Thadronic_HToaa_eta_hut-MadGraph5-pythia8.root",
		"ST_FCNC-TH_Tleptonic_HToaa_eta_hct-MadGraph5-pythia8.root",
		"ST_FCNC-TH_Tleptonic_HToaa_eta_hut-MadGraph5-pythia8.root",
	};
	vector<TString> ntname = {
		"TT_FCNC-TtoHJ_aThad_hct",
		"TT_FCNC-TtoHJ_aThad_hut",
		"TT_FCNC-aTtoHJ_Thad_hct",
		"TT_FCNC-aTtoHJ_Thad_hut",
		"TT_FCNC-TtoHJ_aTlep_hct",
		"TT_FCNC-TtoHJ_aTlep_hut",
		"TT_FCNC-aTtoHJ_Tlep_hct",
		"TT_FCNC-aTtoHJ_Tlep_hut",
		"ST_FCNC-TH_Thad_hct",
		"ST_FCNC-TH_Thad_hut",
		"ST_FCNC-TH_Tlep_hct",
		"ST_FCNC-TH_Tlep_hut",
	};
	vector<float> xs = {
		0.23, 0.23, 0.23, 0.23,
		0.11, 0.11, 0.11, 0.11,
		0.019, 0.139, 0.009, 0.067,
	};
	float lumi = 41.53;
	float BR_tqH = 0.001;

	cout << "[Normfactor]\n";
	for (int i=0; i<filename.size(); ++i) {
		TFile *fin = new TFile(nt_dir + filename[i]);
		TH1D *hweight = (TH1D*)fin->Get("flashggNtuples/h1_weight");
		double sumweight = hweight->GetBinContent(1);
		fin->Close();

		float normfactor = xs[i] * lumi * BR_tqH * 1000 / sumweight;

		TFile *fout = new TFile(outdir + ntname[i] + ".root", "recreate");
		TH1D *nf = new TH1D("normfactor", "", 1, 0, 1);
		nf->SetBinContent(1, normfactor);
		nf->Write();
		fout->Close();

		cout << ntname[i] << "\t\t" << normfactor << endl;
	}

	cout << "Completed!\n";
}

void norm_bkg()
{
	TString nt_dir = "/wk_cms2/youying/public/thFCNC/flashgg_105X/2017/";
	TString outdir = "/wk_cms2/mc_cheng/public/tqHGG/2017/normfactor/";
	vector<TString> filename = {
		"GJet_Pt-20to40_DoubleEMEnriched_MGG-80toInf_TuneCP5_13TeV_Pythia8.root",
		"GJet_Pt-20toInf_DoubleEMEnriched_MGG-40to80_TuneCP5_13TeV_Pythia8.root",
		"GJet_Pt-40toInf_DoubleEMEnriched_MGG-80toInf_TuneCP5_13TeV_Pythia8.root",
		"QCD_Pt-30to40_DoubleEMEnriched_MGG-80toInf_TuneCP5_13TeV_Pythia8.root",
		"QCD_Pt-30toInf_DoubleEMEnriched_MGG-40to80_TuneCP5_13TeV_Pythia8.root",
		"QCD_Pt-40toInf_DoubleEMEnriched_MGG-80toInf_TuneCP5_13TeV_Pythia8.root",
		"TGJets_TuneCP5_13TeV_amcatnlo_madspin_pythia8.root",
		"ST_tW_antitop_5f_inclusiveDecays_TuneCP5_13TeV-powheg-pythia8.root",
		"ST_tW_top_5f_inclusiveDecays_TuneCP5_13TeV-powheg-pythia8.root",
		"TTGG_0Jets_TuneCP5_13TeV_amcatnlo_madspin_pythia8.root",
		"TTGJets_TuneCP5_13TeV-amcatnloFXFX-madspin-pythia8.root",
		"TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8.root",
		"TTWJetsToLNu_TuneCP5_13TeV-amcatnloFXFX-madspin-pythia8.root",
		"TTZToLLNuNu_M-10_TuneCP5_13TeV-amcatnlo-pythia8.root",
		"DYJetsToLL_M-50_TuneCP5_13TeV-amcatnloFXFX-pythia8.root",
		"WGToLNuG_01J_5f_TuneCP5_13TeV-amcatnloFXFX-pythia8.root",
		"ZGToLLG_01J_5f_TuneCP5_13TeV-amcatnloFXFX-pythia8.root",
		"WW_TuneCP5_13TeV-pythia8.root",
		"WZ_TuneCP5_13TeV-pythia8.root",
		"ZZ_TuneCP5_13TeV-pythia8.root",
	};
	vector<TString> filename_res = {
		"GluGluHToGG_M125_13TeV_amcatnloFXFX_pythia8.root",
		"VBFHToGG_M125_13TeV_amcatnlo_pythia8.root",
		"VHToGG_M125_13TeV_amcatnloFXFX_madspin_pythia8.root",
		"ttHJetToGG_M125_13TeV_amcatnloFXFX_madspin_pythia8.root",
		"THQ_ctcvcp_HToGG_M125_13TeV-madgraph-pythia8_TuneCP5.root",
		"THW_ctcvcp_HToGG_M125_13TeV-madgraph-pythia8_TuneCP5.root",
	};
	vector<TString> ntname = {
		"GJet_Pt-20to40_MGG-80toInf",
		"GJet_Pt-20toInf_MGG-40to80",
		"GJet_Pt-40toInf_MGG-80toInf",
		"QCD_Pt-30to40_MGG-80toInf",
		"QCD_Pt-30toInf_MGG-40to80",
		"QCD_Pt-40toInf_MGG-80toInf",
		"TGJets",
		"tW_antitop",
		"tW_top",
		"TTGG",
		"TTGJets",
		"TTJets",
		"TTWJetsToLNu",
		"TTZToLLNuNu",
		"DYJetsToLL",
		"WGToLNuG",
		"ZGToLLG",
		"WW",
		"WZ",
		"ZZ",
	};
	vector<TString> ntname_res = {
		"GluGluHToGG",
		"VBFHToGG",
		"VHToGG",
		"ttHJetToGG",
		"THQ_HToGG",
		"THW_HToGG",
	};
	vector<float> xs = {
		232.8, 3164, 872.8,
		24750, 242700, 117400,
		3.055, 34.97, 34.91,
		0.01687, 4.078, 722.8, 0.2149, 0.2432,
		6529,
		489, 117.864,
		75.8, 27.6, 12.14,
	};
	vector<float> xs_res = {
		33.4, 3.992, 2.449, 0.5101, 0.7301, 0.1464
	};

	float BR_HGG = 0.0023;
	float lumi = 41.53;

	ofstream out("norm_bkg.txt", ios::out);

	for (int cat=0; cat<2; ++cat)
	{
		bool is_resonant = false;
		vector<TString> *filename_ = 0;
		vector<TString> *ntname_ = 0;
		vector<float> *xs_ = 0;
		switch(cat) {
		case 0:
			filename_ = &filename;
			ntname_ = &ntname;
			xs_ = &xs;
			break;
		case 1:
			filename_ = &filename_res;
			ntname_ = &ntname_res;
			xs_ = &xs_res;
			is_resonant = true;
			break;
		}

		int Nnt = filename_->size();

		for (int i=0; i<Nnt; ++i) {
			cout << "Processing: " << ntname_->at(i) << endl;
			TFile *fin = new TFile(nt_dir + filename_->at(i));
			TH1D *hweight = (TH1D*)fin->Get("flashggNtuples/h1_weight");
			double sumweight = hweight->GetBinContent(1);
			fin->Close();

			float normfactor = xs_->at(i) * lumi * 1000 / sumweight;
			if (is_resonant) normfactor *= BR_HGG;

			TFile *fout = new TFile(outdir + ntname_->at(i) + ".root", "recreate");
			TH1D *nf = new TH1D("normfactor", "", 1, 0, 1);
			nf->SetBinContent(1, normfactor);
			nf->Write();
			fout->Close();

			out << ntname_->at(i) << "\t\t" << normfactor << endl;
		}
	}

	out.close();
	cout << "Completed!\n";
}

void norm_DiPhotonJetsBox()
{
	TString nt_dir = "/wk_cms2/youying/public/thFCNC/flashgg_105X/2017/DiPhotonJetsBox_MGG-80toInf_13TeV-Sherpa/";
	TString outdir = "/wk_cms2/mc_cheng/public/tqHGG/2017/normfactor/";
	TString ntname = "DiPhotonJetsBox";

	float lumi = 41.53;
	float xs = 88.36;
	double tot_weight = 0;

	int Nfiles = 518;
	for (int i=0; i<Nfiles; ++i) {
		char filename[256];
		sprintf(filename, "output%d.root", i);
		TFile *fin = new TFile(nt_dir + filename);
		TH1D *hh = (TH1D*)fin->Get("flashggNtuples/h1_weight");
		tot_weight += hh->GetBinContent(1);
		fin->Close();
	}

	float nf = xs * lumi * 1000 / tot_weight;
	TFile *fout = new TFile(outdir + ntname + ".root", "recreate");
	TH1D *hnf = new TH1D("normfactor", "", 1, 0, 1);
	hnf->SetBinContent(1, nf);
	hnf->Write();
	fout->Close();

	ofstream out("norm_DiPhotonJetsBox.txt", ios::out);
	out << ntname << "\t\t" << nf << endl;
}
