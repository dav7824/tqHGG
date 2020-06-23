#ifndef UTILITY_H
#define UTILITY_H

#include "TString.h"
#include "TFile.h"
#include "TTree.h"
#include "TH1D.h"

#include <iostream>
#include <cstdlib>
#include <vector>
#include <string>
using namespace std;

const int Nyear = 3;
const int Nwp = 3;
const float list_btag[Nyear][Nwp] = {
	// loose  medium  tight
	{0.2217, 0.6321, 0.8953},  // 2016 legacy
	{0.1522, 0.4941, 0.8001},  // 2017
	{0.1241, 0.4184, 0.7527}   // 2018
};

float GetBtagCut(TString &year, TString &wp)
{
	int i_year;
	int i_wp;
	if (year=="2016" || year=="16") i_year = 0;
	else if (year=="2017" || year=="17") i_year = 1;
	else if (year=="2018" || year=="18") i_year = 2;
	else {
		cout << "[ERROR] Invalid year argument!\n";
		exit(1);
	}
	if (wp=="loose" || wp=="L") i_wp = 0;
	else if (wp=="medium" || wp=="M") i_wp = 1;
	else if (wp=="tight" || wp=="T") i_wp = 2;
	else {
		cout << "[ERROR] Invalid WP argument!\n";
		exit(1);
	}
	return list_btag[i_year][i_wp];
}

vector<TH1D*> hists;

TH1D *CreateHist(char *name, char *title, int nbin, double min, double max, TString unit = "")
{
	char *xtitle;
	char *ytitle;
	double binwidth = (max - min) / nbin;
	if (unit == "") {
		xtitle = title;
		ytitle = Form("entries / %.2g", binwidth);
	} else {
		xtitle = Form("%s (%s)", title, unit.Data());
		ytitle = Form("entries / %.2g %s", binwidth, unit.Data());
	}
	TH1D *hist = new TH1D( name, Form(";%s;%s", xtitle, ytitle), nbin, min, max );
	hist->Sumw2();
	hists.push_back( hist );

	return hist;
}

void MakeFillHist(TTree *T, TString treevar, TString &exp_weight, char *hist_name, char *hist_title, int nbin, double min, double max, TString unit = "")
{
	CreateHist(hist_name, hist_title, nbin, min, max, unit);
	T->Draw(Form("%s>>%s", treevar.Data(), hist_name), exp_weight);
}

TH1D *CopyHist(char *old_name, char *new_name, char *new_title)
{
	TH1D *h_old = (TH1D*)gDirectory->Get(old_name);
	TH1D *h_new = (TH1D*)h_old->Clone(new_name);
	h_new->SetXTitle(new_title);
	hists.push_back( h_new );

	return h_new;
}

// Take a pointer based string which contains a list of fields seperated by commas, and convert it to a vector of strings
// For predictable results, provide at least one field and all fields be non-empty
void ParseCStringList(const char *input, vector<string> &output)
{
	int idx = 0;
	char read = '\0';
	char sep = ',';
	string tmp_str;

	do {
		read = input[idx++];
		if (read == sep || read == '\0') {
			output.push_back(tmp_str);
			tmp_str.clear();
			if (read == '\0') break;
		}
		else tmp_str += read;
	} while (true);
}

double GetHistVal(const char *fileName, const char *histName)
{
	TFile *fin = new TFile(fileName);
	TH1D *hist = (TH1D*)fin->Get(histName);
	double val = hist->GetBinContent(1);

	fin->Close();

	return val;
}

double GetNormFactor(const char *fileName)
{
	return GetHistVal(fileName, "normfactor");
}

#endif
