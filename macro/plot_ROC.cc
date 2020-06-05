void plot_ROC()
{
	TString indir = "/wk_cms2/mc_cheng/public/tqHGG/2017/MVAreco_training/";
	vector<TString> nttype = {"TThad", "SThad", "TTlep", "STlep"};
	vector<TString> tag = {"Fisher", "BDT", "BDTG", "ANN"};
	vector<int> color = {kBlack, kSpring, kBlue, kRed};

	TString outdir = "/wk_cms2/mc_cheng/public/tqHGG/2017/MVAreco_training/ROC/";

	for (int i=0; i<nttype.size(); ++i) {
		vector<TFile*> fin(tag.size());
		vector<TH1D*> hist(tag.size());
		for (int j=0; j<tag.size(); ++j) {
			fin[j] = new TFile(indir+Form("TrainSummary_%s_%s.root", nttype[i].Data(), tag[j].Data()));
			hist[j] = (TH1D*)fin[j]->Get( Form("dataset/Method_%s/%s/MVA_%s_rejBvsS", tag[j].Data(), tag[j].Data(), tag[j].Data()) );
			hist[j]->SetLineColor( color[j] );
			hist[j]->SetLineWidth(2);
		}
		TCanvas *c = new TCanvas();
		c->SetGrid();

		//hist[0]->SetTitle(nttype[i]);
		//hist[0]->SetStats(0);
		THStack *hs = new THStack("hs", nttype[i]);
		for (int j=0; j<tag.size(); ++j) {
			//hist[j]->Draw("lsame");
			hs->Add(hist[j]);
		}

		hs->Draw("l nostack");
		hs->GetXaxis()->SetTitle("signal eff");
		hs->GetYaxis()->SetTitle("bkg rejection");
		c->BuildLegend();
		c->SaveAs(outdir+Form("roc_%s.png", nttype[i].Data()));
		delete hs;
		delete c;
		for (int j=0; j<tag.size(); ++j) fin[j]->Close();
	}

	cout << "Completed!\n";
}
