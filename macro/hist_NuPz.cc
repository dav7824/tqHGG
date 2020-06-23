/*
 * Make histograms of neutrino Pz info of events reconstructed.
 */

void hist_NuPz( TString method = "ANN" )
{
	// Set the directories
	TString dir_base = "/wk_cms2/mc_cheng/public/tqHGG/2017/";
	TString dir_reco = dir_base+"MVAreco_result_lep_btag-L_phID/";
	TString dir_evt = dir_base+"Presel_lep_btag-L_phID/";
	TString dir_norm = dir_base+"normfactor/";
	gSystem->cd( dir_reco );
	TString dir_in = method+"_/";
	TString dir_out = method+"_NuPz/";
	gSystem->mkdir( dir_out );

	// Leptonic signal MC
	vector<TString> samples = {
		"TT_FCNC-TtoHJ_aTlep_hct",
		"TT_FCNC-TtoHJ_aTlep_hut",
		"TT_FCNC-aTtoHJ_Tlep_hct",
		"TT_FCNC-aTtoHJ_Tlep_hut",
		"ST_FCNC-TH_Tlep_hct",
		"ST_FCNC-TH_Tlep_hut",
	};

	// Branch variables
	// Tree 1
	float genweight = 0;
	float met_Px = 0;
	float met_Py = 0;
	int Gen_size = 0;
	vector<int> *Gen_ID = 0;
	vector<int> *Gen_MomID = 0;
	vector<bool> *Gen_fromHardProcessFinalState = 0;
	vector<float> *Gen_Pt = 0;
	vector<float> *Gen_Eta = 0;
	vector<float> *Gen_Phi = 0;
	vector<float> *Gen_Mass = 0;
	// Tree 2
	float reco_score = 0;
	float Nu_Pz[2] = {0};
	int Nu_Pz_real = 0;

	TLorentzVector gen_nu;
	TLorentzVector reco_nu[2];
	double theta[2];

	// Start sample loop
	for (int i=0; i<samples.size(); ++i) {
		cout << "Processing: " << samples[i] << endl;
		TFile *fin_evt = new TFile( dir_evt+samples[i]+".root" );
		TFile *fin_reco = new TFile( dir_in+samples[i]+".root" );
		TTree *Tevt = (TTree*)fin_evt->Get("T");
		TTree *Treco = (TTree*)fin_reco->Get( TString("T_MVAreco_")+method );
		TFile *fout = new TFile( dir_out+samples[i]+".root", "recreate" );

		// Histograms to be filled
		vector<TH1D*> hists;
		double x_max = 400;
		double x_min = -400;
		int nbin = 40;
		char *title = Form( ";#nu Pz (GeV);events / %.2e GeV", (x_max-x_min)/nbin );
		TH1D *hnuPz_gen = new TH1D("hnuPz_gen", title, nbin, x_min, x_max); hists.push_back(hnuPz_gen);
		TH1D *hnuPz_0 = new TH1D("hnuPz_0", title, nbin, x_min, x_max); hists.push_back(hnuPz_0);
		TH1D *hnuPz_1 = new TH1D("hnuPz_1", title, nbin, x_min, x_max); hists.push_back(hnuPz_1);
		TH1D *hnuPz_2 = new TH1D("hnuPz_2", title, nbin, x_min, x_max); hists.push_back(hnuPz_2);
		TH1D *hnuPz_3 = new TH1D("hnuPz_3", title, nbin, x_min, x_max); hists.push_back(hnuPz_3);
		TH1D *hnuPz_4 = new TH1D("hnuPz_4", title, nbin, x_min, x_max); hists.push_back(hnuPz_4);
		double th_max = 3.2;
		double th_min = 0;
		int th_nbin = 40;
		char *title2 = Form( ";#theta_{gen-reco};events / %.2e", (th_max-th_min)/th_nbin );
		TH1D *htheta_0 = new TH1D("htheta_0", title2, th_nbin, th_min, th_max); hists.push_back(htheta_0);
		TH1D *htheta_1 = new TH1D("htheta_1", title2, th_nbin, th_min, th_max); hists.push_back(htheta_1);
		TH1D *htheta_2 = new TH1D("htheta_2", title2, th_nbin, th_min, th_max); hists.push_back(htheta_2);
		TH1D *htheta_3 = new TH1D("htheta_3", title2, th_nbin, th_min, th_max); hists.push_back(htheta_3);
		TH1D *htheta_4 = new TH1D("htheta_4", title2, th_nbin, th_min, th_max); hists.push_back(htheta_4);
		for (int j=0; j<hists.size(); ++j) hists[j]->Sumw2();

		// Set tree branches
		// Tree 1
		Tevt->SetBranchAddress("EvtInfo.genweight", &genweight);
		Tevt->SetBranchAddress("MetInfo.Px", &met_Px);
		Tevt->SetBranchAddress("MetInfo.Py", &met_Py);
		Tevt->SetBranchAddress("GenPartInfo.size", &Gen_size);
		Tevt->SetBranchAddress("GenPartInfo.PdgID", &Gen_ID);
		Tevt->SetBranchAddress("GenPartInfo.MomPdgID", &Gen_MomID);
		Tevt->SetBranchAddress("GenPartInfo.fromHardProcessFinalState", &Gen_fromHardProcessFinalState);
		Tevt->SetBranchAddress("GenPartInfo.Pt", &Gen_Pt);
		Tevt->SetBranchAddress("GenPartInfo.Eta", &Gen_Eta);
		Tevt->SetBranchAddress("GenPartInfo.Phi", &Gen_Phi);
		Tevt->SetBranchAddress("GenPartInfo.Mass", &Gen_Mass);
		// Tree 2
		// If it's a TT sample
		if (i<=3) {
			Treco->SetBranchAddress("TT_score", &reco_score);
			Treco->SetBranchAddress("TT_Nu_Pz_0", &Nu_Pz[0]);
			Treco->SetBranchAddress("TT_Nu_Pz_1", &Nu_Pz[1]);
			Treco->SetBranchAddress("TT_Nu_Pz_real", &Nu_Pz_real);
		}
		// If it's a ST sample
		else {
			Treco->SetBranchAddress("ST_score", &reco_score);
			Treco->SetBranchAddress("ST_Nu_Pz_0", &Nu_Pz[0]);
			Treco->SetBranchAddress("ST_Nu_Pz_1", &Nu_Pz[1]);
			Treco->SetBranchAddress("ST_Nu_Pz_real", &Nu_Pz_real);
		}

		// Get normfactor
		TFile *fnorm = new TFile( dir_norm+samples[i]+".root" );
		float nf = ((TH1D*)fnorm->Get("normfactor"))->GetBinContent(1);
		fnorm->Close();

		float Nmatch[5] = {0};
		float tot_weight = 0;

		// Start event loop
		for (int evt=0; evt<Tevt->GetEntries(); ++evt)
		{
			Tevt->GetEntry(evt);
			Treco->GetEntry(evt);
			bool gen_nu_exist = false;

			if (reco_score < -5) continue;

			tot_weight += genweight;

			// Start gen particle loop
			for (int i=0; i<Gen_size; ++i) {
				if ( (fabs(Gen_ID->at(i))==12 || fabs(Gen_ID->at(i))==14) && fabs(Gen_MomID->at(i))==24 && Gen_fromHardProcessFinalState->at(i) ) {
					gen_nu_exist = true;
					gen_nu.SetPtEtaPhiM( Gen_Pt->at(i), Gen_Eta->at(i), Gen_Phi->at(i), Gen_Mass->at(i) );
					break;
				}
			} // End of gen particle loop
			if (!gen_nu_exist) {
				cout << "[ERROR] No gen nu in the event!\n";
				return;
			}
			reco_nu[0].SetXYZM( met_Px, met_Py, Nu_Pz[0], 0 ); theta[0] = gen_nu.DeltaR(reco_nu[0]);
			reco_nu[1].SetXYZM( met_Px, met_Py, Nu_Pz[1], 0 ); theta[1] = gen_nu.DeltaR(reco_nu[1]);
			bool match_0 = theta[0] < 0.4;
			bool match_1 = theta[1] < 0.4;

			// Fill histograms
			hnuPz_gen->Fill( gen_nu.Pz(), genweight );
			if (Nu_Pz_real == 1) { // If Pz solutions are real
				hnuPz_0->Fill( Nu_Pz[0], genweight );
				htheta_0->Fill( theta[0], genweight );
				if (match_0) Nmatch[0] += genweight;
				hnuPz_1->Fill( Nu_Pz[1], genweight );
				htheta_1->Fill( theta[1], genweight );
				if (match_1) Nmatch[1] += genweight;
				if ( fabs(Nu_Pz[0]) < fabs(Nu_Pz[1]) ) {
					hnuPz_3->Fill( Nu_Pz[0], genweight );
					htheta_3->Fill( theta[0], genweight );
					if (match_0) Nmatch[3] += genweight;
					hnuPz_4->Fill( Nu_Pz[1], genweight );
					htheta_4->Fill( theta[1], genweight );
					if (match_1) Nmatch[4] += genweight;
				} else {
					hnuPz_3->Fill( Nu_Pz[1], genweight );
					htheta_3->Fill( theta[1], genweight );
					if (match_1) Nmatch[3] += genweight;
					hnuPz_4->Fill( Nu_Pz[0], genweight );
					htheta_4->Fill( theta[0], genweight );
					if (match_0) Nmatch[4] += genweight;
				}
			} else { // If Pz solutions are not real
				hnuPz_2->Fill( Nu_Pz[0], genweight );
				htheta_2->Fill( theta[0], genweight );
				if (match_0) Nmatch[2] += genweight;
			}
		} // End of event loop

		for (int j=0; j<hists.size(); ++j) hists[j]->Scale(nf);

		tot_weight *= nf;
		for (int j=0; j<5; ++j) {
			Nmatch[j] *= nf;
		}
		cout << "Match: (total = " << tot_weight << ")\n";
		cout << "Pz large: " << Nmatch[0]+Nmatch[2] << endl;
		cout << "Pz small: " << Nmatch[1]+Nmatch[2] << endl;
		cout << "Pz small abs: " << Nmatch[3]+Nmatch[2] << endl;
		cout << "Pz large abs: " << Nmatch[4]+Nmatch[2] << endl;
		cout << endl;

		fin_evt->Close();
		fin_reco->Close();
		fout->cd();
		for (int j=0; j<hists.size(); ++j) hists[j]->Write();
		fout->Close();
	} // End of sample loop

	cout << "Completed!\n";
}
