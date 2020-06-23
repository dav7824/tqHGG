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

	cout << "[Normfactor]\n";
	for (int i=0; i<filename.size(); ++i) {
		TFile *fin = new TFile(nt_dir + filename[i]);
		TH1D *hweight = (TH1D*)fin->Get("flashggNtuples/h1_weight");
		double sumweight = hweight->GetBinContent(1);
		fin->Close();

		float normfactor = xs[i] * lumi * 1000 / sumweight;

		TFile *fout = new TFile(outdir + ntname[i] + ".root", "recreate");
		TH1D *nf = new TH1D("normfactor", "", 1, 0, 1);
		nf->SetBinContent(1, normfactor);
		nf->Write();
		fout->Close();

		cout << ntname[i] << "\t\t" << normfactor << endl;
	}

	cout << "Completed!\n";
}
