void iv() {
  TCanvas* c1 = new TCanvas();
  const int N_POINTS = 9;
  const int TACCHETTE_APPREZZABILI = 10;

  Float_t x[N_POINTS] = {400, 450, 500, 550, 600, 650, 700, 720, 760};
  Float_t y[N_POINTS] = {0.005, 0.020, 0.068, 0.198, 0.612,
                         1.783, 4.89,  9.8,   18.4};
  Float_t ex[N_POINTS] = {5, 5, 5, 5, 5, 5, 5, 5, 5};  // VOLTS/DIV
  Float_t ey[N_POINTS] = {};

  // Calculate error on oscilloscope
  for (size_t i = 0; i < N_POINTS; i++) {
    ex[i] = ex[i] * TACCHETTE_APPREZZABILI / 5.;
  }

  TGraphErrors* gr = new TGraphErrors(N_POINTS, x, y, ex, ey);
  gr->SetTitle("Diodo al Si");

  gr->SetMarkerStyle(4);
  gr->Draw("AP");
  c1->SetLogy();
  c1->SetGrid();
  gr->GetXaxis()->SetTitle("Voltage (mV)");
  gr->GetYaxis()->SetTitle("Current (mA)");

  TF1* f1 = new TF1("f1", "[0]*exp(x/[1] - 1.)", 400, 800);
  f1->SetParameter(0, 1E-6);
  f1->SetParName(0, "I0");

  f1->SetParameter(1, 50.);
  f1->SetParName(1, "#eta*VT");

  gr->Fit("f1", "R");
  gStyle->SetOptFit(1111);
}