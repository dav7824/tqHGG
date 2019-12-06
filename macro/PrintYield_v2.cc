void PrintTex_1(ostream &out, int year)
{
    out << "\\documentclass{article}" << endl;
    out << "\\usepackage[utf8]{inputenc}" << endl;
    out << endl;
    out << "\\begin{document}" << endl;
    out << "\\begin{center}" << endl;
    out << "\\begin{tabular}{ |c|c| }" << endl;
    out << "\\hline" << endl;

    switch (year) {
	case 2016:
	    out << "\\multicolumn{2}{|c|}{2016, 35.9 $fb^{-1}$ (13 TeV)} \\\\" << endl;
	    break;
	case 2017:
	    out << "\\multicolumn{2}{|c|}{2017, 41.5 $fb^{-1}$ (13 TeV)} \\\\" << endl;
	    break;
	default:
	    break;
    }
    out << "\\hline" << endl;
}

void PrintTex_2(ostream &out)
{
    out << "\\end{tabular}" << endl;
    out << "\\end{center}" << endl;
    out << "\\end{document}" << endl;
}

void PrintYield_v2(const char *inDir_, const char *outDir_, int year)
{
    string inDir = inDir_;
    string outDir = outDir_;
    inDir += '/';
    outDir += '/';

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
    vector<string> mc_tex = {
	"$\\gamma\\gamma+jets$",
	"$\\gamma+jets$",
	"QCD",
	"$t+\\gamma jets$",
	"$t\\bar{t}+\\gamma/jets$",
	"Drell Yan",
	"$W+jets$",
	"$V\\gamma$",
	"$VV$",
	"SM Higgs"
    };
    // path to the root file of total MC
    string mc_tot_path = inDir + "mc_tot.root";
    // path to the root file of data
    string data_path = inDir + "DoubleEG.root";
    // path to output text file
    string text_path = outDir + "yield.txt";
    // path to output Latex .tex file
    string tex_path = outDir + "yield.tex";

    ofstream output(text_path.c_str(), ios::out);
    ofstream output_tex(tex_path.c_str(), ios::out);
    output_tex << fixed << setprecision(1);
    PrintTex_1(output_tex, year);

    for (int i=0; i<mc.size(); ++i) {
	TFile *fin = new TFile( (inDir + mc[i] + ".root").c_str() );
	TH1D *h1 = (TH1D*)fin->Get("yield");
	TH1D *h2 = (TH1D*)fin->Get("yield_err2");
	double yield = h1->GetBinContent(1);
	double yield_err = sqrt(h2->GetBinContent(1));
	output << mc[i] << "      " << yield << " +- " << yield_err << endl;
	output_tex << mc_tex[i] << " & $" << yield << "\\pm " << yield_err << "$ \\\\" << endl;
	fin->Close();
    }
    output << endl;
    output_tex << "\\hline" << endl;

    TH1D *h1 = 0;
    TH1D *h2 = 0;
    TFile *fin = 0;
    double data_yield[2];
    double mc_yield[2];

    fin = new TFile(mc_tot_path.c_str());
    h1 = (TH1D*)fin->Get("yield");
    h2 = (TH1D*)fin->Get("yield_err2");
    mc_yield[0] = h1->GetBinContent(1);
    mc_yield[1] = h2->GetBinContent(1);
    output << "MC      " << mc_yield[0] << " +- " << sqrt(mc_yield[1]) << endl;
    output_tex << "MC & $" << mc_yield[0] << "\\pm " << sqrt(mc_yield[1]) << "$ \\\\" << endl;
    fin->Close();

    fin = new TFile(data_path.c_str());
    h1 = (TH1D*)fin->Get("yield");
    data_yield[0] = h1->GetBinContent(1);
    data_yield[1] = h1->GetBinContent(1);
    output << "Data      " << data_yield[0] << " +- " << sqrt(data_yield[1]) << endl;
    output_tex << "data & $" << data_yield[0] << "\\pm " << sqrt(data_yield[1]) << "$ \\\\" << endl;
    output_tex << "\\hline" << endl;
    fin->Close();

    double diff[2];
    diff[0] = mc_yield[0] - data_yield[0];
    diff[1] = mc_yield[1] + data_yield[1];
    double ratio[2];
    ratio[0] = diff[0] / data_yield[0];    // (MC_yield - data_yield) / data_yield
    ratio[1] = (diff[1] / (diff[0]*diff[0]) + data_yield[1] / (data_yield[0]*data_yield[0])) * ratio[0]*ratio[0];
    output << "\n(MC - data) / data = (" <<  ratio[0] * 100. << " +- " << sqrt(ratio[1]) * 100. << ") %" << endl;
    output_tex << "(MC-data)/data & $(" << ratio[0]*100. << "\\pm " << sqrt(ratio[1])*100. << ")\\%$ \\\\" << endl;
    output_tex << "\\hline" << endl;
    output << "MC - data = " << diff[0] << " +- " << sqrt(diff[1]) << endl;

    PrintTex_2(output_tex);

    output.close();
    output_tex.close();
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
