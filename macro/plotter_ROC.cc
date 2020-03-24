void plotter_ROC()
{
    //TString indir = "/wk_cms2/mc_cheng/public/tqHGG/2017/MVAreco_TT-had/training/TT_FCNC-TtoHJ_aThad_hct/";
    // ROC_v2v3v4.png
    /*vector<TString> fin_name = {
	"setting_v2/training_summary.root",
	"setting_v2/training_summary.root",
	"setting_v2/training_summary.root",
	"setting_v3/training_summary.root",
	"setting_v3/training_summary.root",
	"setting_v3/training_summary.root",
	"setting_v4/training_summary.root",
	"setting_v4/training_summary.root",
	"setting_v4/training_summary.root",
    };
    vector<TString> hist_name = {
	Form("dataset/Method_%s/%s/MVA_%s_rejBvsS", "BDTG_a", "BDTG_a", "BDTG_a"),
	Form("dataset/Method_%s/%s/MVA_%s_rejBvsS", "BDTG_b", "BDTG_b", "BDTG_b"),
	Form("dataset/Method_%s/%s/MVA_%s_rejBvsS", "BDTG_c", "BDTG_c", "BDTG_c"),
	Form("dataset/Method_%s/%s/MVA_%s_rejBvsS", "BDTG_d", "BDTG_d", "BDTG_d"),
	Form("dataset/Method_%s/%s/MVA_%s_rejBvsS", "BDTG_e", "BDTG_e", "BDTG_e"),
	Form("dataset/Method_%s/%s/MVA_%s_rejBvsS", "BDTG_f", "BDTG_f", "BDTG_f"),
	Form("dataset/Method_%s/%s/MVA_%s_rejBvsS", "BDTG_g", "BDTG_g", "BDTG_g"),
	Form("dataset/Method_%s/%s/MVA_%s_rejBvsS", "BDTG_h", "BDTG_h", "BDTG_h"),
	Form("dataset/Method_%s/%s/MVA_%s_rejBvsS", "BDTG_i", "BDTG_i", "BDTG_i"),
    };
    vector<int> color = {
	kYellow-7, kOrange+1, kRed, kGreen-9, kSpring-1, kSpring-7, kCyan, kAzure+2, kViolet+10,
    };*/
    TString indir = "/wk_cms2/mc_cheng/public/tqHGG/2017/MVAreco_ST-had/training/All/";
    vector<TString> fin_name = {
	"training_summary_BDT.root",
	"training_summary_BDTG.root",
	"training_summary_ANN.root",
	"training_summary_Cuts.root",
	"training_summary_Fisher.root",
    };
    vector<TString> hist_name = {
	Form("dataset/Method_%s/%s/MVA_%s_rejBvsS", "BDT", "BDT", "BDT"),
	Form("dataset/Method_%s/%s/MVA_%s_rejBvsS", "BDTG", "BDTG", "BDTG"),
	Form("dataset/Method_%s/%s/MVA_%s_rejBvsS", "ANN", "ANN", "ANN"),
	Form("dataset/Method_%s/%s/MVA_%s_rejBvsS", "Cuts", "Cuts", "Cuts"),
	Form("dataset/Method_%s/%s/MVA_%s_rejBvsS", "Fisher", "Fisher", "Fisher"),
    };
    vector<int> color = {
	kViolet, kRed, kBlue, kBlack, kSpring-7,
    };

    int Nhist = hist_name.size();

    vector<TFile*> fin(Nhist);
    vector<TH1D*> hists(Nhist);
    for (int i=0; i<Nhist; ++i) {
	fin[i] = new TFile(indir + fin_name[i]);
	hists[i] = (TH1D*)fin[i]->Get(hist_name[i]);
	hists[i]->SetLineColor(color[i]);
	hists[i]->SetLineWidth(2);
    }

    TCanvas *c = new TCanvas();
    c->SetGrid();
    hists[0]->SetTitle("");
    hists[0]->SetStats(false);
    hists[0]->Draw("l");
    for (int i=1; i<Nhist; ++i) hists[i]->Draw("lsame");
    c->BuildLegend();
    c->SaveAs("ROC_ST-had.png");

    for (int i=0; i<Nhist; ++i) fin[i]->Close();
}
