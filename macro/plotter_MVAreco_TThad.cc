// Making stacked plots for histograms after MVA reconstruction of TT signal in hadronic channel.
//
// input: root files containing data & MC histograms
// output: stacked plots png + (optional) yield text file
//
// arguments:
// 1. MVA version
// 2. MVA method
// 3. MVA score value cut
// 4. (default = 0) whether use log scale for y axis
// 5. (default = 0) whether calculate event yields

void plotter_MVAreco_TThad(TString ver, TString method, TString cut_MVA, bool use_logy = 0, bool calcYield = 0)
{
    // some settings used later
    int year = 2017;
    TString ch = "had";

    // set IO directories
    TString sample = "TT_FCNC-TtoHJ_aThad_hct";
    TString ntdir = Form("/wk_cms2/mc_cheng/public/tqHGG/%d/MVAreco_TT-had/output/%s/setting_%s_%s/", year, sample.Data(), ver.Data(), method.Data());
    TString indir = ntdir + Form("hist_cut%s/", cut_MVA.Data());
    TString outdir = ntdir + Form("plots_cut%s/", cut_MVA.Data());
    TString normdir = Form("/wk_cms2/mc_cheng/public/tqHGG/%d/normfactor/", year);

    // name of each bkg MC
    vector<TString> bkgMC_name = {
	"DiPhotonJetsBox",
	"GJet_Pt-20to40_MGG-80toInf", "GJet_Pt-20toInf_MGG-40to80", "GJet_Pt-40toInf_MGG-80toInf",
	"QCD_Pt-30to40_MGG-80toInf", "QCD_Pt-30toInf_MGG-40to80", "QCD_Pt-40toInf_MGG-80toInf",
	"TGJets", "tW_antitop", "tW_top",
	"TTGG", "TTGJets", "TTJets", "TTWJetsToLNu", "TTZToLLNuNu",
	"DYJetsToLL",
	"WGToLNuG", "ZGToLLG",
	"WW", "WZ", "ZZ",
	"GluGluHToGG", "VBFHToGG", "VHToGG", "ttHJetToGG",
	"THQ_HToGG", "THW_HToGG"
    };
    int n_bkg = bkgMC_name.size();

    // name of each bkg MC category (used in legend)
    vector<TString> bkgMC_legend = {
	"#gamma#gamma+jets", "#gamma+jets", "QCD", "t#gamma+jets", "t+W",
	"tt#gamma#gamma", "tt(#gamma)+jets", "tt+V", "DY", "VG", "VV",
	"SM Higgs"
    };
    // name of each bkg MC category (used as file names)
    vector<TString> bkgMC_cat = {
	"GGJets", "GJets", "QCD", "TGJets", "TW", "TTGG", "TT-G-Jets", "TTV", "DY", "VG", "VV", "SM_Higgs"
    };
    int n_bkgleg = bkgMC_legend.size();

    // open data root file
    cout << "Openning input files...\n";
    cout << "---Openning file: DoubleEG\n";
    TFile *fin_data = new TFile(indir + "DoubleEG.root");
    vector<TFile*> fin_bkgMC(n_bkg);
    for (int i=0; i<n_bkg; ++i) {
	cout << "---Openning file: " << bkgMC_name[i] << endl;
	fin_bkgMC[i] = new TFile(indir + bkgMC_name[i] + ".root");
    }

    // get the histograms in the files
    cout << "Getting input histograms...\n";
    TKey *key = 0;
    TIter nextkey(fin_data->GetListOfKeys());
    vector<TH1D*> hdata;
    vector< vector<TH1D*> > hbkg(n_bkg);
    while (( key = (TKey*)nextkey() )) {
	cout << "---Histogram found: " << key->GetName() << endl;
	hdata.push_back( (TH1D*)key->ReadObj() );
	for (int i=0; i<n_bkg; ++i) {
	    hbkg[i].push_back( (TH1D*)fin_bkgMC[i]->Get( key->GetName() ) );
	}
    }
    int nhist = hdata.size();

    // normalize the histograms to luminosity
    cout << "Normalizing MC histograms...\n";
    for (int i=0; i<n_bkg; ++i) {
	TFile *fin_norm = new TFile(normdir + bkgMC_name[i] + ".root");
	TH1D *h_nf = (TH1D*)fin_norm->Get("normfactor");
	double nf = h_nf->GetBinContent(1);
	fin_norm->Close();

	for (int j=0; j<nhist; ++j) {
	    hbkg[i][j]->Scale(nf);
	}
    }

    // save the plots
    /*cout << "Saving results...\n";
    for (int i=0; i<n_bkg; ++i) {
	TFile *fout = new TFile(outdir + bkgMC_name[i] + ".root", "recreate");
	fout->cd();
	for (int j=0; j<nhist; ++j) {
	    hbkg[i][j]->Write();
	}
	fout->Close();
    }*/

    // add histograms for each MC bkg category
    cout << "Adding histograms of the same categories...\n";
    for (int i=0; i<nhist; ++i) {
	// GJet
	hbkg[1][i]->Add(hbkg[2][i]);
	hbkg[1][i]->Add(hbkg[3][i]);
	// QCD
	hbkg[4][i]->Add(hbkg[5][i]);
	hbkg[4][i]->Add(hbkg[6][i]);
	// t+W
	hbkg[8][i]->Add(hbkg[9][i]);
	// tt(gamma)+jets
	hbkg[11][i]->Add(hbkg[12][i]);
	// tt+V
	hbkg[13][i]->Add(hbkg[14][i]);
	// VG
	hbkg[16][i]->Add(hbkg[17][i]);
	// VV
	hbkg[18][i]->Add(hbkg[19][i]);
	hbkg[18][i]->Add(hbkg[20][i]);
	// SM Higgs
	hbkg[21][i]->Add(hbkg[22][i]);
	hbkg[21][i]->Add(hbkg[23][i]);
	hbkg[21][i]->Add(hbkg[24][i]);
	hbkg[21][i]->Add(hbkg[25][i]);
	hbkg[21][i]->Add(hbkg[26][i]);
    }

    vector< vector<TH1D*>* > hbkg_leg(n_bkgleg);
    hbkg_leg[0] = &(hbkg[0]);
    hbkg_leg[1] = &(hbkg[1]);
    hbkg_leg[2] = &(hbkg[4]);
    hbkg_leg[3] = &(hbkg[7]);
    hbkg_leg[4] = &(hbkg[8]);
    hbkg_leg[5] = &(hbkg[10]);
    hbkg_leg[6] = &(hbkg[11]);
    hbkg_leg[7] = &(hbkg[13]);
    hbkg_leg[8] = &(hbkg[15]);
    hbkg_leg[9] = &(hbkg[16]);
    hbkg_leg[10] = &(hbkg[18]);
    hbkg_leg[11] = &(hbkg[21]);

    // create total MC bkg histograms
    cout << "Creating histograms of total MC bkg...\n";
    vector<TH1D*> hbkg_tot(nhist);
    for (int i=0; i<nhist; ++i) {
	hbkg_tot[i] = (TH1D*)( hbkg_leg[0]->at(i)->Clone() );
	for (int j=1; j<n_bkgleg; ++j)
	    hbkg_tot[i]->Add( hbkg_leg[j]->at(i) );
    }

    // color of each MC category
    vector<int> color = {
	kCyan-10, kCyan+1, kBlue-9, kAzure+1, kBlue-7, kViolet+6, kMagenta-9, kMagenta-10,
	kRed-7, kOrange+1, kOrange-9, kYellow-7, kSpring+9, kGreen-7, kGreen-6,
    };

    // create THStack of MC
    cout << "Creating hist stack of total MC bkg...\n";
    vector<THStack*> hstack(nhist);
    for (int i=0; i<nhist; ++i) {
	hstack[i] = new THStack( hdata[i]->GetName(), "" );
	for (int j=0; j<n_bkgleg; ++j) {
	    hstack[i]->Add( hbkg_leg[j]->at(i) );
	    if (j < color.size()) hbkg_leg[j]->at(i)->SetFillColor(color[j]);
	}
	// set the axis of hist stack
	// draw THStack first to create base histogram
	hstack[i]->Draw();
	hstack[i]->GetXaxis()->SetTitle( hdata[i]->GetXaxis()->GetTitle() );
	hstack[i]->GetYaxis()->SetTitle( hdata[i]->GetYaxis()->GetTitle() );
    }

    /*cout << "Saving results...\n";
    for (int i=0; i<n_bkgleg; ++i) {
	TFile *fout = new TFile(outdir + bkgMC_cat[i] + ".root", "recreate");
	fout->cd();
	for (int j=0; j<nhist; ++j) hbkg_leg[i]->at(j)->Write();
	fout->Close();
    }*/

    // create legend
    cout << "Setting legend...\n";
    TLegend *leg = new TLegend(0.35, 0.7, 0.96, 0.89);
    leg->SetNColumns(3);
    leg->SetLineWidth(0);
    leg->SetFillColorAlpha(kWhite, 0);
    // add entries to legend
    TList *mc_list = hstack[0]->GetHists();
    int i_mc = 0;
    for (TIter it=mc_list->begin(); it!=mc_list->end(); ++it) {
	leg->AddEntry((TH1D*)*it, bkgMC_legend[i_mc], "f");
	++i_mc;
    }
    leg->AddEntry(hdata[0], "data", "p");
    leg->AddEntry(hbkg_tot[0], "MC stat err", "f");

    // create data/MC histograms
    cout << "Creating data/MC histograms...\n";
    vector<TH1D*> hdom(nhist);
    vector<TH1D*> hdom_err(nhist);
    for (int i=0; i<nhist; ++i) {
	hdom[i] = (TH1D*)hdata[i]->Clone();
	hdom_err[i] = (TH1D*)hdata[i]->Clone();
	hdom[i]->Divide(hbkg_tot[i]);
	for (int j=1; j<=hdom[i]->GetNbinsX(); ++j) {
	    hdom[i]->SetBinError( j, hdata[i]->GetBinError(j) / hbkg_tot[i]->GetBinContent(j) );
	    hdom_err[i]->SetBinContent(j, 1);
	    hdom_err[i]->SetBinError( j, hbkg_tot[i]->GetBinError(j) / hbkg_tot[i]->GetBinContent(j) );
	}
    }

    cout << "Setting Latex...\n";
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

    cout << "Setting canvas & pads...\n";
    // prepare drawing plots
    // create canvas and pads
    TCanvas *c = new TCanvas("c", "c", 450., 500.);
    TPad *pad1 = new TPad("pad1", "pad1", 0., 0.23, 1., 1.);
    TPad *pad2 = new TPad("pad2", "pad2", 0., 0., 1., 0.23);
    // configure pad1 (for data + stacked MC plots)
    pad1->SetRightMargin(0.03);
    pad1->SetTopMargin(0.07);
    pad1->SetBottomMargin(0.01);
    pad1->SetGrid();
    pad1->SetTicks();
    if (use_logy) pad1->SetLogy();
    pad1->Draw();
    // configure pad2 (for data/MC plots)
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

    // draw the plot of each histogram
    cout << "Start drawing plots...\n";
    for (int i=0; i<nhist; ++i) {
	cout << "---Drawing: " << hdata[i]->GetName() << endl;
	// configure data hist
	hdata[i]->SetMarkerStyle(20);
	hdata[i]->SetMarkerSize(0.6);
	// configure total bkg MC hist
	hbkg_tot[i]->SetFillColor(kBlack);
	hbkg_tot[i]->SetFillStyle(3245);
	hbkg_tot[i]->SetMarkerStyle(0);
	// configure bkg MC hist stack
	TH1 *hs_b = hstack[i]->GetHistogram();
	hs_b->SetTitleSize(0., "x");
	hs_b->SetLabelSize(0., "x");
	hs_b->GetYaxis()->SetTitleSize(0.03);
	hs_b->GetYaxis()->SetLabelSize(0.028);
	// set y maxima for plot 1
	double ymax = 0.;
	if (!use_logy)  ymax = ((hdata[i]->GetMaximum() > hbkg_tot[i]->GetMaximum()) ? hdata[i]->GetMaximum() : hbkg_tot[i]->GetMaximum()) * 1.4;
	else ymax = ((hdata[i]->GetMaximum() > hbkg_tot[i]->GetMaximum()) ? hdata[i]->GetMaximum() : hbkg_tot[i]->GetMaximum()) * 1000.;
	hstack[i]->SetMaximum(ymax);
	// set y minima for plot 1
	if (!use_logy) hstack[i]->SetMinimum(0);
	else hstack[i]->SetMinimum(0.1);

	// draw plot 1
	pad1->cd();
	hstack[i]->Draw("hist");
	hbkg_tot[i]->Draw("e2 same");
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
	c->SaveAs(outdir + hdata[i]->GetName() + ".png");
    } // end of histogram plotting loop

    if (calcYield) {
	cout << "Calculating event yields...\n";

	// compute the yields
	int idx_hmgg = 0;
	double yield_data = 0;
	double yield_data_err = 0;
	double yield_mc_tot = 0;
	double yield_mc_tot_err = 0;
	vector<double> yield_bkgMC(n_bkgleg);
	vector<double> yield_bkgMC_err(n_bkgleg);

	// yield & error of data
	for (int i=0; i<nhist; ++i)
	    if ( TString(hdata[i]->GetName()) == "dipho_mass" ) {
		idx_hmgg = i;
		yield_data = hdata[i]->Integral();
		yield_data_err = sqrt(yield_data);
		break;
	    }
	// yield of total MC
	yield_mc_tot = hbkg_tot[idx_hmgg]->Integral();
	// yield of each kind of MC
	for (int i=0; i<n_bkgleg; ++i) yield_bkgMC[i] = hbkg_leg[i]->at(idx_hmgg)->Integral();
	// error of total MC
	// error of each kind of MC
	for (int i_bin=1; i_bin<=hbkg_tot[idx_hmgg]->GetNbinsX(); ++i_bin) {
	    yield_mc_tot_err += pow( hbkg_tot[idx_hmgg]->GetBinError(i_bin), 2 );
	    for (int j=0; j<n_bkgleg; ++j) yield_bkgMC_err[j] += pow( hbkg_leg[j]->at(idx_hmgg)->GetBinError(i_bin), 2 );
	}
	yield_mc_tot_err = sqrt(yield_mc_tot_err);
	for (int i=0; i<n_bkgleg; ++i) yield_bkgMC_err[i] = sqrt( yield_bkgMC_err[i] );

	// difference between data & MC
	double diff = yield_mc_tot - yield_data;
	double diff_err2 = pow(yield_mc_tot_err, 2) + pow(yield_data_err, 2);
	double ratio = diff / yield_data;
	double ratio_err2 = ( diff_err2/pow(diff, 2) + pow(yield_data_err, 2)/pow(yield_data, 2) ) * pow(ratio, 2);

	// save the results in txt file
	ofstream out_yield( (outdir + "yields.txt").Data(), ios::out );

	for (int i=0; i<n_bkgleg; ++i) out_yield << bkgMC_cat[i] << "\t\t" << yield_bkgMC[i] << " +- " << yield_bkgMC_err[i] << endl;
	out_yield << endl;
	out_yield << "Total MC\t\t" << yield_mc_tot << " +- " << yield_mc_tot_err << endl;
	out_yield << "DoubleEG\t\t" << yield_data << " +- " << yield_data_err << endl << endl;
	out_yield << "(MC - data) / data\t\t" << ratio * 100 << " +- " << sqrt(ratio_err2) * 100 << " %" << endl;

	out_yield.close();
    }

    // close input files
    fin_data->Close();
    for (int i=0; i<n_bkg; ++i) fin_bkgMC[i]->Close();
}
