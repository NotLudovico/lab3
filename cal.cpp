#include <cmath>

#include "graph.hpp"
#include "latex.hpp"
#include "nums.hpp"

void cal() {
  TCanvas* c1 = new TCanvas();
  gStyle->SetOptFit(0111);
  c1->SetGrid();

  const int N_POINTS = 9;
  const double TACCHETTE_APPREZZABILI = 0.5;

  Double_t mult[N_POINTS] = {76.2,  95.7,  144.1, 195.1, 242.3,
                             292.8, 389.6, 491.4, 586.6};
  Double_t oscill[N_POINTS] = {80, 100, 150, 200, 250, 300, 400, 500, 600};
  Double_t e_mult[N_POINTS] = {};
  Double_t e_oscill[N_POINTS] = {20, 20,  50,  50, 50,
                                 50, 100, 100, 100};  // Riportare VOLT/DIV

  Double_t fondoscala[N_POINTS] = {};
  copy(e_oscill, e_oscill + N_POINTS, fondoscala);

  calc_err_oscill(oscill, e_oscill, N_POINTS);
  calc_err_mult_volt(mult, e_mult, N_POINTS, 0.1);

  vector<const char*> titles = {"Fondo Scala (mV/Div)", "Oscilloscopio (mV)",
                                "Multimetro (mV)"};
  vector<ValErr> data = {ValErr(fondoscala), ValErr(oscill, e_oscill),
                         ValErr(mult, e_mult)};

  gen_latex_table(titles, data, N_POINTS);

  TGraphErrors* gr = new TGraphErrors(N_POINTS, mult, oscill, e_mult, e_oscill);
  cosm(gr, "Retta di Calibrazione; V_{multimetro}(mV); V_{oscilloscopio}(mV)",
       0.6, 4);
  vector<const char*> param_names = {"m", "q"};
  vector<Double_t> param_values = {1., 0.};
  fit(gr, "[0]*x+[1]", param_names, param_values, 0, 650);
  gr->Draw("AP");

  set_box_stats(c1, gr);
}