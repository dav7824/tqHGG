TString maindir = "/wk_cms2/mc_cheng/FCNH/CMSSW_8_1_0/src/tqHGG_syst/";

void PlotBrR(TString coupling, TString channel)
{
    if (coupling != "Hut" && coupling != "Hct") {
        cout << "[ERROR] Invalid coupling\n";
        exit(1);
    }
    if (channel != "had" && channel != "lep" && channel != "comb") {
        cout << "[ERROR] Invalid channel\n";
        exit(1);
    }

    // Read input text file
    int i_scan;
    float BR, limit_exp[5], limit_obs;
    vector<float> BRs, limits_exp[5], limits_obs;
    TString fin_name = Form("Limits_%s_%s.txt", channel.Data(), coupling.Data());
    ifstream fin( (maindir+fin_name).Data(), ios::in );
    if (!fin) {
        cerr << "[ERROR] Cannot open input file\n";
        exit(1);
    }
    while (fin >> i_scan >> BR >> limit_exp[0] >> limit_exp[1] >> limit_exp[2] >> limit_exp[3] >> limit_exp[4] >> limit_obs) {
        BRs.push_back(BR);
        for (int i=0; i<5; ++i)  limits_exp[i].push_back(limit_exp[i]);
        limits_obs.push_back(limit_obs);
    }
    fin.close();

    int Nscan = BRs.size();

    // Create limit graphs
    TGraph *gr_obs = new TGraph(Nscan);  // Observed limit
    TGraph *gr_exp = new TGraph(Nscan);  // Medium expected limit
    TGraph *gr_1sigma = new TGraph(Nscan*2);  // Expected limit @ +1/-1 sigma
    TGraph *gr_2sigma = new TGraph(Nscan*2);  // Expected limit @ +2/-2 sigma
    for (int i=0; i<Nscan; ++i) {
        // Set observed point
        gr_obs->SetPoint(i, BRs[i]*100., limits_obs[i]);
        // Set medium point
        gr_exp->SetPoint(i, BRs[i]*100., limits_exp[2][i]);
        // Set 1 sigma points
        gr_1sigma->SetPoint(i, BRs[i]*100., limits_exp[3][i]);  // +1 sigma
        gr_1sigma->SetPoint(2*Nscan-1-i, BRs[i]*100., limits_exp[1][i]);  // -1 sigma
        // Set 2 sigma points
        gr_2sigma->SetPoint(i, BRs[i]*100., limits_exp[4][i]);  // +2 sigma
        gr_2sigma->SetPoint(2*Nscan-1-i, BRs[i]*100., limits_exp[0][i]);  // -2 sigma
    }
    gr_obs->SetLineColor(kBlack);
    gr_obs->SetLineWidth(3);
    gr_obs->SetLineStyle(2);
    gr_exp->SetLineColor(kRed);
    gr_exp->SetLineWidth(3);
    //gr_exp->SetLineStyle(2);
    gr_1sigma->SetFillColor(kGreen);
    gr_2sigma->SetFillColor(kYellow);

    // Create histogram for graph background
    TString htitle = Form(";BR(t #rightarrow %sH) [%%];signal strength r @ 95%% CL", (coupling=="Hut")?"u":"c");
    //TH2D *hh = new TH2D("hh", htitle, 10, 0., 1.1, 10, 6e-2, 90.);
    TH2D *hh = new TH2D("hh", htitle, 10, 0., 2.6, 10, 6e-2, 90.);
    hh->SetStats(false);

    // Set canvas
    TCanvas *c = new TCanvas();
    c->SetGrid();
    c->SetLogy();

    // Paint graphs
    hh->Draw("");
    gr_2sigma->Draw("fsame");
    gr_1sigma->Draw("fsame");
    gr_exp->Draw("same");
    gr_obs->Draw("same");

    // Set legend
    TLegend *leg = new TLegend(0.55, 0.65, 0.85, 0.85);
    leg->AddEntry(gr_obs, "observed", "l");
    leg->AddEntry(gr_exp, "medium expected", "l");
    leg->AddEntry(gr_1sigma, "#pm 1#sigma", "f");
    leg->AddEntry(gr_2sigma, "#pm 2#sigma", "f");
    leg->Draw();

    // LaTex text
    TLatex latex;
    latex.SetTextAlign(10);
    latex.DrawLatexNDC(.1, .91, "CMS #bf{#it{Preliminary}}");
    if (channel == "had")
        latex.DrawLatexNDC(.16, .75, "#scale[1.2]{hadronic}");
    else if (channel == "lep")
        latex.DrawLatexNDC(.16, .75, "#scale[1.2]{leptonic}");
    else
        latex.DrawLatexNDC(.16, .75, "#scale[1.2]{hadronic+leptonic}");
    latex.SetTextAlign(30);
    latex.DrawLatexNDC(.9, .91, "#bf{41.5 fb^{-1} (2017, 13TeV)}");

    // Draw line @ r=1
    TLine lin;
    //lin.SetLineColor(kGray+2);
    //lin.SetLineStyle(7);
    lin.SetLineWidth(2);
    //lin.DrawLine(0., 1., 1.1, 1.);
    lin.DrawLine(0., 1., 2.6, 1.);

    // Save plot
    TString fig_name = Form("BRvsR_%s_%s.png", channel.Data(), coupling.Data());
    c->SaveAs( maindir+fig_name );
}
