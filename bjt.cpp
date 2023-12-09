#include "graph.hpp"
#include "latex.hpp"
#include "nums.hpp"

using namespace std;

void bjt() {
  TCanvas* c1 = new TCanvas();
  c1->SetGrid();
  gStyle->SetOptFit(0111);

  constexpr int N_POINTS = 27;

  // Data
  Double_t volt[N_POINTS] = {0.06, 0.08, 0.10, 0.12, 0.14, 0.16, 0.18,
                             0.20, 0.24, 0.28, 0.32, 0.36, 0.40, 0.60,
                             0.72, 0.76, 0.80, 0.84, 0.88, 0.92, 0.96,
                             1,    1.5,  2,    2.5,  3,    4};  // Volt
  Double_t fondoscala[N_POINTS] = {0.02, 0.02, 0.02, 0.02, 0.1, 0.1, 0.1,
                                   0.1,  0.1,  0.1,  0.1,  0.1, 0.1, 0.1,
                                   0.2,  0.2,  0.2,  0.2,  0.2, 0.2, 0.2,
                                   0.5,  0.5,  0.5,  0.5,  1,   1};  // Volt/Div
  Double_t curr100[N_POINTS] = {0.90,  1.79,  2.92,  4.34,  5.86,  7.54,  8.91,
                                10.15, 12.09, 13.21, 13.92, 14.25, 14.92, 15.7,
                                15.88, 15.99, 16.04, 16.11, 16.16, 16.21, 16.26,
                                16.4,  16.93, 17.38, 17.67, 18.45, 19.16};
  Double_t curr50[N_POINTS] = {0.36, 0.81, 1.49, 2.47, 3.80, 5.05, 6.17,
                               6.95, 7.83, 8.23, 8.39, 8.47, 8.55, 8.70,
                               8.77, 8.79, 8.81, 8.83, 8.85, 8.87, 8.89,
                               8.92, 9.23, 9.44, 9.62, 9.77, 9.93};

  // Errors (Calculated automatically)
  Double_t curr100_err[N_POINTS] = {};
  Double_t curr50_err[N_POINTS] = {};
  Double_t volt_err[N_POINTS] = {};
  copy(fondoscala, fondoscala + N_POINTS, volt_err);

  calc_err("MULT", curr100, curr100_err, N_POINTS);
  calc_err("MULT", curr50, curr50_err, N_POINTS);
  calc_err("OSCILL", volt, volt_err, N_POINTS);

  // gen_latex_table(fondoscala, volt_err, volt, curr100, curr100_err, curr50,
  //                 curr50_err, N_POINTS);

  TGraphErrors* gr100 =
      new TGraphErrors(N_POINTS, volt, curr100, volt_err, curr100_err);

  vector<const char*> param_names = {"a_{100}", "b_{100}"};
  vector<Double_t> param_values = {1, 1.2};
  fit(gr100, "[0]+[1]*x", param_names, param_values, 1, 4, kBlue);
  cosm(gr100);
  gr100->SetMarkerColor(kBlue);

  TGraphErrors* gr50 =
      new TGraphErrors(N_POINTS, volt, curr50, volt_err, curr50_err);
  param_names = {"a_{50}", "b_{50}"};
  fit(gr50, "[0]+[1]*x", param_names, param_values, 1, 4);
  cosm(gr50);
  gr50->SetMarkerColor(kRed);

  TMultiGraph* mg = new TMultiGraph();
  mg->Add(gr100);
  mg->Add(gr50);
  mg->SetTitle("Caratteristica di uscita del BJT; -V_{CE}; -I_{C}(mA)");
  mg->Draw("AP");

  TLegend* legend = new TLegend(0.15, 0.79, 0.35, 0.89);
  legend->AddEntry(gr100, "I_{b} = -100#muA", "lp");
  legend->AddEntry(gr50, "I_{b} = -50#muA", "lp");
  legend->Draw();

  // Creating Latex Tables
  vector<const char*> titles = {"Fondo Scala (V)", "V(V)", "I(mA) - 50\\mu A",
                                "I(mA) - 100\\mu A"};
  vector<ValErr> data = {ValErr(fondoscala), ValErr(volt, volt_err),
                         ValErr(curr50, curr50_err),
                         ValErr(curr100, curr100_err)};
  gen_latex_table(titles, data, N_POINTS);

  // Statistics Box Cosmetics
  set_box_stats(c1, gr100, 0.65, 0.15, 0.25, 0.1);
  set_box_stats(c1, gr50, 0.65, 0.26, 0.25, 0.1);

  // Calculate Delta I_C
  Double_t delta_I[N_POINTS] = {};
  for (int i = 0; i < N_POINTS; i++) {
    delta_I[i] =
        abs(curr50[i] - curr100[i]) / 0.05;  // Divided by \DeltaI (50microA)
  }

  TCanvas* c2 = new TCanvas();
  c2->SetGrid();

  TGraph* grDelta = new TGraph(N_POINTS, volt, delta_I);
  cosm(grDelta, "Guadagno di corrente; -V_{CE}(V); #DeltaI_{C}/#DeltaI_{B}");
  grDelta->Draw("AP");

  TCanvas* c3 = new TCanvas();
  c3->SetGrid();

  TGraphErrors* gr100_inv =
      new TGraphErrors(N_POINTS, curr100, volt, curr100_err, volt_err);
  param_names = {"a_{100}", "b_{100}"};
  param_values = {-5, 10};
  fit(gr100_inv, "[0]+[1]*x", param_names, param_values, 15, 20);

  TGraphErrors* gr50_inv =
      new TGraphErrors(N_POINTS, curr50, volt, curr50_err, volt_err);
  param_names = {"a_{50}", "b_{50}"};
  fit(gr50_inv, "[0]+[1]*x", param_names, param_values, 7, 20);

  TMultiGraph* mg_inv = new TMultiGraph();
  mg_inv->Add(gr100_inv);
  mg_inv->Add(gr50_inv);
  mg_inv->SetTitle("Inversa;  -I_{C} (mA); -V_{CE} (V)");
  mg_inv->Draw("AP");

  // Statistics Box Cosmetics
  set_box_stats(c3, gr100_inv, 0.13, 0.78, 0.25, 0.1);
  set_box_stats(c3, gr50_inv, 0.13, 0.67, 0.25, 0.1);
}
