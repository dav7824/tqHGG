void plotter_v1(TString inDir, TString outDir, int year = 0, TString ch = "", int use_logy = 0)
{
    // read input files
    inDir += '/';
    TFile *fdata = new TFile(inDir + "DoubleEG.root");
    TFile *fmc = new TFile(inDir + "mc_tot.root");
    TFile *fmc_stacked = new TFile(inDir + "mc_stacked.root");

    // set output directory
    outDir += '/';

    // get input histograms
    TKey *key = 0;
    TIter nextkey(fdata->GetListOfKeys());
    vector<TH1D*> hdata;
    vector<TH1D*> hmc;
    vector<THStack*> hmc_st;
    while ((key = (TKey*)nextkey())) {
	hdata.push_back( (TH1D*)key->ReadObj() );
	hmc.push_back( (TH1D*)fmc->Get(key->GetName()) );
	hmc_st.push_back( (THStack*)fmc_stacked->Get(key->GetName()) );
    }
    int nhist = hdata.size();

    // create data/mc histograms
    vector<TH1D*> hdom;
    vector<TH1D*> hdom_err;
    hdom.resize(nhist);
    hdom_err.resize(nhist);
    for (int i=0; i<nhist; ++i) {
	hdom[i] = (TH1D*)hdata[i]->Clone();
	hdom[i]->Divide(hmc[i]);
	hdom_err[i] = (TH1D*)hdata[i]->Clone();
	for (int i_bin=1; i_bin<=hdata[i]->GetNbinsX(); ++i_bin) {
	    hdom[i]->SetBinError(i_bin, hdata[i]->GetBinError(i_bin) / hmc[i]->GetBinContent(i_bin));
	    hdom_err[i]->SetBinContent(i_bin, 1.);
	    hdom_err[i]->SetBinError(i_bin, hmc[i]->GetBinError(i_bin) / hmc[i]->GetBinContent(i_bin));
	}
    }

    // MC category names shown in the legend
    vector<TString> mc_name = {
	"#gamma#gamma+jets",
	"#gamma+jet",
	"QCD",
	"t#gamma+jets",
	"tt+#gamma/jets",
	"DY",
	"W+jets",
	"V#gamma",
	"VV",
	"SM Higgs"
    };

    // set legend
    TLegend *leg = new TLegend(0.35, 0.75, 0.96, 0.92);
    leg->SetNColumns(3);
    leg->SetLineWidth(0);
    // add entries to legend
    TList *mc_list = hmc_st[0]->GetHists();
    int i_mc = 0;
    for (TIter it=mc_list->begin(); it!=mc_list->end(); ++it) {
	leg->AddEntry((TH1D*)*it, mc_name[i_mc], "f");
	++i_mc;
    }
    leg->AddEntry(hdata[0], "data", "p");
    leg->AddEntry(hmc[0], "MC stat err", "f");

    // Latex text displays luminosity, year, and sqrt(s)
    TLatex *tex_info = new TLatex();
    tex_info->SetTextAlign(31);
    tex_info->SetTextSize(0.04);
    // Latex text for cms
    TLatex *tex_cms = new TLatex();
    tex_cms->SetTextAlign(11);
    tex_cms->SetTextSize(0.04);
    // Latex text for channel
    TLatex *tex_ch = new TLatex();
    tex_ch->SetTextAlign(11);
    tex_ch->SetTextSize(0.05);

    // prepare drawing plots
    // create canvas and pads
    TCanvas *c = new TCanvas("c", "c", 450., 500.);
    TPad *pad1 = new TPad("pad1", "pad1", 0., 0.23, 1., 1.);
    TPad *pad2 = new TPad("pad2", "pad2", 0., 0., 1., 0.23);
    // configure pad1
    pad1->SetRightMargin(0.03);
    pad1->SetTopMargin(0.07);
    pad1->SetBottomMargin(0.01);
    pad1->SetGrid();
    pad1->SetTicks();
    if (use_logy) pad1->SetLogy();
    pad1->Draw();
    // configure pad2
    pad2->SetRightMargin(0.03);
    pad2->SetTopMargin(0.01);
    pad2->SetBottomMargin(0.23);
    pad2->SetGrid();
    pad2->SetTicks();
    if (use_logy) pad2->SetLogy();
    pad2->Draw();

    // declare TLine object
    TLine *lin = new TLine();
    lin->SetLineWidth(1);
    lin->SetLineColor(kBlack);

    for (int i=0; i<nhist; ++i) {
	// configure data hist
	hdata[i]->SetMarkerStyle(20);
	hdata[i]->SetMarkerSize(0.6);
	// configure total mc hist
	hmc[i]->SetFillColor(kBlack);
	hmc[i]->SetFillStyle(3245);
	hmc[i]->SetMarkerStyle(0);
	// configure mc stacked hist
	// draw the THStack first to create its base histogram
	pad1->cd();
	hmc_st[i]->Draw();
	TH1 *hs_b = hmc_st[i]->GetHistogram();
	hs_b->SetTitleSize(0., "x");
	hs_b->SetLabelSize(0., "x");
	hs_b->GetYaxis()->SetTitleSize(0.03);
	hs_b->GetYaxis()->SetLabelSize(0.028);
	double ymax = 0.;
	if (!use_logy)  ymax = ((hdata[i]->GetMaximum() > hmc[i]->GetMaximum()) ? hdata[i]->GetMaximum() : hmc[i]->GetMaximum()) * 1.4;
	else ymax = ((hdata[i]->GetMaximum() > hmc[i]->GetMaximum()) ? hdata[i]->GetMaximum() : hmc[i]->GetMaximum()) * 1000.;
	hmc_st[i]->SetMaximum(ymax);
	if (!use_logy) hmc_st[i]->SetMinimum(0);
	else hmc_st[i]->SetMinimum(0.1);

	// draw stacked plot
	pad1->cd();
	hmc_st[i]->Draw("hist");
	hmc[i]->Draw("e2 same");
	hdata[i]->Draw("p same");
	leg->Draw();
	// draw Latex text
	TString tex_info_;
	switch (year) {
	    case 2016:
		tex_info_ = "35.9 fb^{-1} (2016, 13TeV)";
		break;
	    case 2017:
		tex_info_ = "41.5 fb^{-1} (2017, 13TeV)";
		break;
	    default:
		break;
	}
	TString tex_ch_;
	if (ch == "had") tex_ch_ = "hadronic";
	else if (ch == "lep") tex_ch_ = "leptonic";
	tex_info->DrawLatexNDC(0.97, 0.94, tex_info_);
	tex_cms->DrawLatexNDC(0.16, 0.94, "CMS preliminary");
	tex_ch->DrawLatexNDC(0.15, 0.8, tex_ch_);

	// configure data/mc hist
	hdom[i]->SetMarkerStyle(20);
	hdom[i]->SetMarkerSize(0.6);
	// configure data/mc error hist
	hdom_err[i]->SetFillColor(kBlack);
	hdom_err[i]->SetFillStyle(3245);
	hdom_err[i]->SetMarkerStyle(0);
	if (!use_logy) hdom_err[i]->GetYaxis()->SetRangeUser(0.1, 1.9);
	else hdom_err[i]->GetYaxis()->SetRangeUser(0.1, 10.);
	hdom_err[i]->SetYTitle("data / MC");
	hdom_err[i]->SetStats(false);
	hdom_err[i]->SetTitleSize(0.12, "x");
	hdom_err[i]->SetTitleOffset(0.8, "x");
	hdom_err[i]->SetLabelSize(0.09, "x");
	hdom_err[i]->SetLabelOffset(0.01, "x");
	hdom_err[i]->SetTitleSize(0.1, "y");
	hdom_err[i]->SetTitleOffset(0.5, "y");
	hdom_err[i]->SetLabelSize(0.07, "y");

	// draw data/mc plot
	pad2->cd();
	hdom_err[i]->Draw("e2");
	lin->DrawLine(hdom_err[i]->GetXaxis()->GetXmin(), 1., hdom_err[i]->GetXaxis()->GetXmax(), 1.);
	hdom[i]->Draw("p same");

	// save the canvas
	c->SaveAs(outDir + hdata[i]->GetName() + ".png");
    }

    fdata->Close();
    fmc->Close();
    fmc_stacked->Close();
}
