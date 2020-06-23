/*
 * Plot the histograms produced by macro hist_NuPz.cc.
 */

vector<vector<TH1D*>> hists;

TH1D *AddHist_TT(int idx, TString name)
{
	TH1D *res = (TH1D*)hists[0][idx]->Clone( name );
	for (int i=1; i<=3; ++i) res->Add( hists[i][idx] );
	return res;
}

TH1D *AddHist_ST(int idx, TString name)
{
	TH1D *res = (TH1D*)hists[4][idx]->Clone( name );
	res->Add( hists[5][idx] );
	return res;
}

void plotter_NuPz(TString method = "ANN")
{
	// Set the directories
	TString dir_base = "/wk_cms2/mc_cheng/public/tqHGG/2017/";
	TString dir_wk = dir_base+"MVAreco_result_lep_btag-L_phID/"+method+"_NuPz/";
	gSystem->cd( dir_wk );

	// Leptonic signal MC
	vector<TString> samples = {
		"TT_FCNC-TtoHJ_aTlep_hct",
		"TT_FCNC-TtoHJ_aTlep_hut",
		"TT_FCNC-aTtoHJ_Tlep_hct",
		"TT_FCNC-aTtoHJ_Tlep_hut",
		"ST_FCNC-TH_Tlep_hct",
		"ST_FCNC-TH_Tlep_hut",
	};

	// Read files
	cout << "Reading files...\n";
	vector<TFile*> fin(samples.size());
	for (int i=0; i<samples.size(); ++i) fin[i] = new TFile(samples[i]+".root");

	// Get histograms
	vector<TString> histname = {
		"hnuPz_gen",
		"hnuPz_0",
		"hnuPz_1",
		"hnuPz_2",
		"hnuPz_3",
		"hnuPz_4",
		"htheta_0",
		"htheta_1",
		"htheta_2",
		"htheta_3",
		"htheta_4",
	};
	for (int i=0; i<samples.size(); ++i) {
		vector<TH1D*> hh;
		for (int j=0; j<histname.size(); ++j) {
			hh.push_back((TH1D*)fin[i]->Get(histname[j]));
		}
		hists.push_back(hh);
	}

	// Arrange histograms
	cout << "Arranging histograms...\n";
	TH1D *hPz_gen_TT = AddHist_TT( 0, "Pz_gen_TT" );
	TH1D *hPz_L_TT = AddHist_TT( 1, "Pz_L_TT" );
	TH1D *hPz_S_TT = AddHist_TT( 2, "Pz_S_TT" );
	TH1D *hPz_im_TT = AddHist_TT( 3, "Pz_im_TT" );
	TH1D *hPz_Sa_TT = AddHist_TT( 4, "Pz_Sa_TT" );
	TH1D *hPz_La_TT = AddHist_TT( 5, "Pz_La_TT" );
	TH1D *htheta_L_TT = AddHist_TT( 6, "theta_L_TT" );
	TH1D *htheta_S_TT = AddHist_TT( 7, "theta_S_TT" );
	TH1D *htheta_im_TT = AddHist_TT( 8, "theta_im_TT" );
	TH1D *htheta_Sa_TT = AddHist_TT( 9, "theta_Sa_TT" );
	TH1D *htheta_La_TT = AddHist_TT( 10, "theta_La_TT" );
	TH1D *hPz_gen_ST = AddHist_ST( 0, "Pz_gen_ST" );
	TH1D *hPz_L_ST = AddHist_ST( 1, "Pz_L_ST" );
	TH1D *hPz_S_ST = AddHist_ST( 2, "Pz_S_ST" );
	TH1D *hPz_im_ST = AddHist_ST( 3, "Pz_im_ST" );
	TH1D *hPz_Sa_ST = AddHist_ST( 4, "Pz_Sa_ST" );
	TH1D *hPz_La_ST = AddHist_ST( 5, "Pz_La_ST" );
	TH1D *htheta_L_ST = AddHist_ST( 6, "theta_L_ST" );
	TH1D *htheta_S_ST = AddHist_ST( 7, "theta_S_ST" );
	TH1D *htheta_im_ST = AddHist_ST( 8, "theta_im_ST" );
	TH1D *htheta_Sa_ST = AddHist_ST( 9, "theta_Sa_ST" );
	TH1D *htheta_La_ST = AddHist_ST( 10, "theta_La_ST" );

	hPz_L_TT->Add( hPz_im_TT );
	hPz_S_TT->Add( hPz_im_TT );
	hPz_Sa_TT->Add( hPz_im_TT );
	hPz_La_TT->Add( hPz_im_TT );
	htheta_L_TT->Add( htheta_im_TT );
	htheta_S_TT->Add( htheta_im_TT );
	htheta_Sa_TT->Add( htheta_im_TT );
	htheta_La_TT->Add( htheta_im_TT );
	hPz_L_ST->Add( hPz_im_ST );
	hPz_S_ST->Add( hPz_im_ST );
	hPz_Sa_ST->Add( hPz_im_ST );
	hPz_La_ST->Add( hPz_im_ST );
	htheta_L_ST->Add( htheta_im_ST );
	htheta_S_ST->Add( htheta_im_ST );
	htheta_Sa_ST->Add( htheta_im_ST );
	htheta_La_ST->Add( htheta_im_ST );

	// Start drawing
	cout << "Start drawing...\n";
	TCanvas *c = new TCanvas("c", "");
	c->SetGrid();

	hPz_gen_TT->SetLineColor( kBlack ); hPz_gen_TT->SetLineWidth(2);
	hPz_L_TT->SetLineColor( kOrange ); hPz_L_TT->SetLineWidth(2);
	hPz_S_TT->SetLineColor( kViolet ); hPz_S_TT->SetLineWidth(2);
	hPz_Sa_TT->SetLineColor( kRed ); hPz_Sa_TT->SetLineWidth(2);
	hPz_La_TT->SetLineColor( kBlue ); hPz_La_TT->SetLineWidth(2);
	hPz_gen_TT->SetStats(false);
	hPz_gen_TT->Draw("hist");
	hPz_L_TT->Draw("histsame");
	hPz_S_TT->Draw("histsame");
	c->BuildLegend();
	c->SaveAs("Pz_TT_v1.png");
	hPz_Sa_TT->SetStats(false);
	hPz_Sa_TT->Draw("hist");
	hPz_gen_TT->Draw("histsame");
	hPz_La_TT->Draw("histsame");
	c->BuildLegend();
	c->SaveAs("Pz_TT_v2.png");

	hPz_gen_ST->SetLineColor( kBlack ); hPz_gen_ST->SetLineWidth(2);
	hPz_L_ST->SetLineColor( kOrange ); hPz_L_ST->SetLineWidth(2);
	hPz_S_ST->SetLineColor( kViolet ); hPz_S_ST->SetLineWidth(2);
	hPz_Sa_ST->SetLineColor( kRed ); hPz_Sa_ST->SetLineWidth(2);
	hPz_La_ST->SetLineColor( kBlue ); hPz_La_ST->SetLineWidth(2);
	hPz_gen_ST->SetStats(false);
	hPz_gen_ST->Draw("hist");
	hPz_L_ST->Draw("histsame");
	hPz_S_ST->Draw("histsame");
	c->BuildLegend();
	c->SaveAs("Pz_ST_v1.png");
	hPz_Sa_ST->SetStats(false);
	hPz_Sa_ST->Draw("hist");
	hPz_gen_ST->Draw("histsame");
	hPz_La_ST->Draw("histsame");
	c->BuildLegend();
	c->SaveAs("Pz_ST_v2.png");

	htheta_L_TT->SetLineColor( kOrange ); htheta_L_TT->SetLineWidth(2);
	htheta_S_TT->SetLineColor( kViolet ); htheta_S_TT->SetLineWidth(2);
	htheta_Sa_TT->SetLineColor( kRed ); htheta_Sa_TT->SetLineWidth(2);
	htheta_La_TT->SetLineColor( kBlue ); htheta_La_TT->SetLineWidth(2);
	htheta_Sa_TT->SetStats(false);
	htheta_Sa_TT->Draw("hist");
	htheta_L_TT->Draw("histsame");
	htheta_S_TT->Draw("histsame");
	htheta_La_TT->Draw("histsame");
	c->BuildLegend();
	c->SaveAs("theta_TT.png");

	htheta_L_ST->SetLineColor( kOrange ); htheta_L_ST->SetLineWidth(2);
	htheta_S_ST->SetLineColor( kViolet ); htheta_S_ST->SetLineWidth(2);
	htheta_Sa_ST->SetLineColor( kRed ); htheta_Sa_ST->SetLineWidth(2);
	htheta_La_ST->SetLineColor( kBlue ); htheta_La_ST->SetLineWidth(2);
	htheta_Sa_ST->SetStats(false);
	htheta_Sa_ST->Draw("hist");
	htheta_L_ST->Draw("histsame");
	htheta_S_ST->Draw("histsame");
	htheta_La_ST->Draw("histsame");
	c->BuildLegend();
	c->SaveAs("theta_ST.png");

	// Close files
	for (int i=0; i<samples.size(); ++i) fin[i]->Close();

	cout << "Completed!\n";
}
