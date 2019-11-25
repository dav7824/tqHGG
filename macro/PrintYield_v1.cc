void PrintYield_v1(const char *inDir_, const char *output_path_)
{
    string inDir = inDir_;
    string output_path = output_path_;
    inDir += '/';

    // name of each MC category
    vector<string> mc = {
	"DiPhotonJetsBox",
	"GJet",
	"QCD",
	"TGJets",
	"TTG_Jets",
	"DY",
	"WJets",
	"VG",
	"VV",
	"SM_Higgs"
    };
    // path to the root file of total MC
    string mc_tot_path = inDir + "mc_tot.root";
    // path to the root file of data
    string data_path = inDir + "DoubleEG.root";

    ofstream output(output_path.c_str(), ios::out);

    for (int i=0; i<mc.size(); ++i) {
	TFile *fin = new TFile( (inDir + mc[i] + ".root").c_str() );
	TH1D *h1 = (TH1D*)fin->Get("yield");
	TH1D *h2 = (TH1D*)fin->Get("yield_err2");
	output << mc[i] << "      " << h1->GetBinContent(1) << " +- " << sqrt(h2->GetBinContent(1)) << endl;
	fin->Close();
    }
    output << endl;

    TH1D *h1 = 0;
    TH1D *h2 = 0;
    TFile *fin = 0;
    double data_yield[2];
    double mc_yield[2];

    fin = new TFile(data_path.c_str());
    h1 = (TH1D*)fin->Get("yield");
    data_yield[0] = h1->GetBinContent(1);
    data_yield[1] = h1->GetBinContent(1);
    output << "Data      " << data_yield[0] << " +- " << sqrt(data_yield[1]) << endl;
    fin->Close();

    fin = new TFile(mc_tot_path.c_str());
    h1 = (TH1D*)fin->Get("yield");
    h2 = (TH1D*)fin->Get("yield_err2");
    mc_yield[0] = h1->GetBinContent(1);
    mc_yield[1] = h2->GetBinContent(1);
    output << "MC      " << mc_yield[0] << " +- " << sqrt(mc_yield[1]) << endl;
    fin->Close();

    double diff[2];
    diff[0] = mc_yield[0] - data_yield[0];
    diff[1] = mc_yield[1] + data_yield[1];
    double ratio[2];
    ratio[0] = diff[0] / data_yield[0];    // (MC_yield - data_yield) / data_yield
    ratio[1] = (diff[1] / (diff[0]*diff[0]) + data_yield[1] / (data_yield[0]*data_yield[0])) * ratio[0]*ratio[0];
    output << "\n(MC - data) / data = (" <<  ratio[0] * 100. << " +- " << sqrt(ratio[1]) * 100. << ") %" << endl;
    output << "MC - data = " << diff[0] << " +- " << sqrt(diff[1]) << endl;

    output.close();
}

/*
 * Error propagation
 *
 * f = ax +- by
 * sigma_f^2 = a^2 sigma_x^2 + b^2 sigma_y^2 +- 2ab cov(x,y)
 *
 * f = axy
 * sigma_f^2 / f^2 = sigma_x^2 / x^2 + sigma_y^2 / y^2 + 2 cov(x,y) / (xy)
 *
 * f = ax/y
 * sigma_f^2 / f^2 = sigma_x^2 / x^2 + sigma_y^2 / y^2 - 2 cov(x,y) / (xy)
 */
