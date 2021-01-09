#ifndef UTILITY_H
#define UTILITY_H

#include "TString.h"
#include "TFile.h"
#include "TTree.h"
#include "TH1D.h"

#include <iostream>
#include <cstdlib>
#include <vector>
#include <map>
#include <string>
using namespace std;

const int Nyear = 3;
const int Nwp = 3;
// DeepFlavourJet tagger
const float list_btag[Nyear][Nwp] = {
    // loose  medium  tight
    {-1, -1, -1},  // 2016 legacy
    {0.0521, 0.3033, 0.7489},  // 2017
    {-1, -1, -1}   // 2018
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

// Take a pointer based string which contains a list of fields seperated by commas, and convert it to a vector of strings
// For predictable results, provide at least one field and all fields be non-empty
void ParseCStringList(const char *input, vector<string> &output, char sep=',')
{
    int idx = 0;
    char read = '\0';
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
void ParseCStringList(const char *input, vector<TString> &output, char sep=',')
{
    int idx = 0;
    char read = '\0';
    TString tmp_str;

    do {
        read = input[idx++];
        if (read == sep || read == '\0') {
            output.push_back(tmp_str);
            tmp_str.Clear();
            if (read == '\0') break;
        }
        else tmp_str += read;
    } while (true);
}

void CreateHist(map<TString,TH1D*> &hists, char *name, char *title, int nbin, double min, double max, TString unit = "")
{
    char *xtitle;
    char *ytitle;
    double binwidth = (max - min) / nbin;
    if (unit == "") {
        xtitle = title;
        ytitle = Form("events / %.2g", binwidth);
    } else {
        xtitle = Form("%s (%s)", title, unit.Data());
        ytitle = Form("events / %.2g %s", binwidth, unit.Data());
    }
    TH1D *hist = new TH1D( name, Form(";%s;%s", xtitle, ytitle), nbin, min, max );
    hist->Sumw2();

    hists[name] = hist;
}

void FillHist(TTree *T, const char *var, const char *hist, const TString &weight, const TString cut = "")
{
    TString selection = Form("(%s)", weight.Data());
    if (cut != "")  selection += Form(" * (%s)", cut.Data());
    T->Draw( Form("%s>>%s",var,hist), selection );
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

void GetYieldInfo(const char *fileName, float *yield)
{
    TFile *fin = new TFile(fileName);
    yield[0] = ((TH1D*)fin->Get("yield"))->GetBinContent(1);
    yield[1] = ((TH1D*)fin->Get("yield2"))->GetBinContent(1);
    fin->Close();
}

    template <typename T>
T GetMaximum(T *arr, int N)
{
    T maxima = -999;
    for (int i=0; i<N; ++i)
        if (arr[i] > maxima) maxima = arr[i];

    return maxima;
}

#endif
