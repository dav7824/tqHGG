void MakePlot(TString fin_name, TString tag)
{
    TFile *fin = new TFile( fin_name );
    TH2F *hsig = (TH2F*)fin->Get("hcorr_sig");
    TH2F *hbkg = (TH2F*)fin->Get("hcorr_bkg");
    hsig->SetLabelSize(0.025, "x");
    hsig->SetLabelSize(0.025, "y");
    hbkg->SetLabelSize(0.025, "x");
    hbkg->SetLabelSize(0.025, "y");

    TLatex tex;
    tex.SetTextAlign(30);

    TCanvas c("c", "c", 1000, 1000);
    c.SetLeftMargin(0.15);
    c.SetRightMargin(0.15);
    c.SetBottomMargin(0.15);
    hsig->Draw("colz");
    tex.DrawLatexNDC(0.9, .91, "#bf{#scale[0.6]{#color[13]{Correlation coef. #times 100}}}");
    c.SaveAs( tag+"_sig.png" );
    hbkg->Draw("colz");
    tex.DrawLatexNDC(0.9, .91, "#bf{#scale[0.6]{#color[13]{Correlation coef. #times 100}}}");
    c.SaveAs( tag+"_bkg.png" );

    fin->Close();
}

void BDTcorr_plotter()
{
    //MakePlot( "BDTcorr_had.root", "BDTcorr_had" );
    //MakePlot( "BDTcorr_had_NjetHigh.root", "BDTcorr_had_NjetHigh" );
    //MakePlot( "BDTcorr_lep.root", "BDTcorr_lep" );
    MakePlot( "BDTcorr_lep_NjetHigh.root", "BDTcorr_lep_NjetHigh" );
}
