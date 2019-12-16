// function printing the beginning of .tex file
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

// function printing the end of .tex file
void PrintTex_2(ostream &out)
{
    out << "\\end{tabular}" << endl;
    out << "\\end{center}" << endl;
    out << "\\end{document}" << endl;
}

// function calculating yields of n-tuples
// Take a root file containing Mgg distribution hist named "dipho_mass", and calculate the yield and stat err from it
void CalcYield(const char *filename, double &yield, double &err2)
{
    TFile *fin = new TFile(filename);

    err2 = 0.;
    TH1D *hmgg = (TH1D*)fin->Get("dipho_mass");
    yield = hmgg->Integral();
    for (int i=1; i<=hmgg->GetNbinsX(); ++i) {
	err2 += hmgg->GetBinError(i)*hmgg->GetBinError(i);
    }

    fin->Close();
}

// The main part
void PrintYield_v3(const char *inDir_, const char *outDir_, int year)
{
    string inDir = inDir_;
    string outDir = outDir_;
    inDir += '/';
    outDir += '/';

    // name of each MC to be printed in .txt summary
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
    // name of each MC to be printed in .tex in Latex syntax
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

    // create output stream
    // .txt file
    ofstream output(text_path.c_str(), ios::out);
    // .tex file
    ofstream output_tex(tex_path.c_str(), ios::out);
    output_tex << fixed << setprecision(1);
    // print the begining part of .tex file
    PrintTex_1(output_tex, year);

    // create yield & error variables
    vector<double> yield;
    vector<double> yield_err;
    double mc_yield[2];
    double data_yield[2];
    yield.resize(mc.size());
    yield_err.resize(mc.size());
    // get the yields and errors from each MC histogram root files
    for (int i=0; i<mc.size(); ++i) {
	string in_file = inDir + mc[i] + ".root";
	CalcYield(in_file.c_str(), yield[i], yield_err[i]);
    }
    // get the yields and errors from total MC hist root file
    CalcYield(mc_tot_path.c_str(), mc_yield[0], mc_yield[1]);
    // get the yields and errors from data hist root file
    CalcYield(data_path.c_str(), data_yield[0], data_yield[1]);

    // print yield for each MC
    for (int i=0; i<mc.size(); ++i) {
	output << mc[i] << "    " << yield[i] << " +- " << sqrt(yield_err[i]) << endl;
	output_tex << mc_tex[i] << " & $" << yield[i] << "\\pm " << sqrt(yield_err[i]) << "$ \\\\" << endl;
    }
    output << endl;
    output_tex << "\\hline" << endl;

    // print yield for total MC
    output << "MC      " << mc_yield[0] << " +- " << sqrt(mc_yield[1]) << endl;
    output_tex << "MC & $" << mc_yield[0] << "\\pm " << sqrt(mc_yield[1]) << "$ \\\\" << endl;

    // print yield for data
    output << "Data      " << data_yield[0] << " +- " << sqrt(data_yield[1]) << endl;
    output_tex << "data & $" << data_yield[0] << "\\pm " << sqrt(data_yield[1]) << "$ \\\\" << endl;
    output_tex << "\\hline" << endl;

    // calculate & print (MC-data)/data
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

    // print the end of .tex file
    PrintTex_2(output_tex);

    // close output streams
    output.close();
    output_tex.close();
}
