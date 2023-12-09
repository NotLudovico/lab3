#include "graph.hpp"
#include "latex.hpp"
#include "nums.hpp"

using namespace std;
void si() {
  TCanvas* c1 = new TCanvas();
  gStyle->SetOptFit(0111);
  c1->SetGrid();
  c1->SetLogy();

  const int N_POINTS = 16;
  const double m_cal = 1.017;
  const double q_cal = 2.635;

  // Oscill (V)
  Double_t oscill[N_POINTS] = {420, 440, 460, 480, 500, 520, 540, 560,
                               580, 600, 620, 640, 660, 680, 700, 720};
  // Mult (A)
  Double_t mult[N_POINTS] = {0.02, 0.03, 0.04, 0.06, 0.08, 0.13, 0.2,  0.3,
                             0.45, 0.67, 1,    1.51, 2.23, 3.3,  4.84, 7.66};
  Double_t e_oscill[N_POINTS] = {
      100, 100, 100, 100, 100, 100, 100, 100,
      100, 100, 100, 100, 100, 100, 100, 200};  // VOLTS/DIV
  Double_t e_mult[N_POINTS] = {};

  Double_t fondoscala[N_POINTS] = {};
  copy(e_oscill, e_oscill + N_POINTS, fondoscala);

  // Calc error
  calc_err_oscill(oscill, e_oscill, N_POINTS);
  calc_err_mult_curr(mult, e_mult, N_POINTS, 0.02);

  // Gen Latex Table
  vector<const char*> titles = {"Fondo Scala (mV/Div)", "V(mV)", "I(mA)"};
  vector<ValErr> data = {ValErr(fondoscala), ValErr(oscill, e_oscill),
                         ValErr(mult, e_mult)};
  gen_latex_table(titles, data, N_POINTS);

  // Fix Oscilloscope measure
  for (size_t i = 0; i < N_POINTS; i++) {
    oscill[i] = (oscill[i] - q_cal) / m_cal;
  }

  TGraphErrors* gr = new TGraphErrors(N_POINTS, oscill, mult, e_oscill, e_mult);
  cosm(gr, "Diodo al Si; V(mV); I(mA)", 0.6, 4);
  gr->Draw("AP");

  vector<const char*> param_names = {"I_{0}", "#etaV_{T}"};
  vector<Double_t> param_values = {1E-6, 50.};
  fit(gr, "[0]*exp(x/[1])", param_names, param_values, 400, 750);

  set_box_stats(c1, gr, 0.12, 0.78, 0.25, 0.1);
}
