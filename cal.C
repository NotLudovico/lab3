void cal() {
  TCanvas* c1 = new TCanvas();
  const int N_POINTS = 9;
  const int TACCHETTE_APPREZZABILI = 10;

  Float_t mult[N_POINTS] = {0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 1.};
  Float_t oscill[N_POINTS] = {0.21, 0.29,  0.41, 0.49, 0.69,
                              0.81, 0.901, 1.2,  1.3};
  Float_t e_mult[N_POINTS] = {};
  Float_t e_oscill[N_POINTS] = {};  // Riportare VOLT/DIV

  // Calculate error on oscilloscope
  for (size_t i = 0; i < N_POINTS; i++) {
    e_oscill[i] = e_oscill[i] * TACCHETTE_APPREZZABILI / 5.;
  }

  TGraphErrors* gr = new TGraphErrors(N_POINTS, mult, oscill, e_mult, e_oscill);
  gr->SetTitle("Retta di Calibrazione");

  gr->SetMarkerStyle(4);
  gr->Draw("AP");
  c1->SetGrid();
  gr->GetXaxis()->SetTitle("Multimetro");
  gr->GetYaxis()->SetTitle("Oscilloscopio");

  TF1* f1 = new TF1("f1", "[0]*x+[1]", 0, 2);
  f1->SetParameter(0, 1.);
  f1->SetParName(0, "m");

  f1->SetParameter(1, 0.);
  f1->SetParName(1, "q");

  gr->Fit("f1", "R");
  gStyle->SetOptFit(1111);
}