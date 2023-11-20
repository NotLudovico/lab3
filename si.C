#include <cmath>

void si() {
  TCanvas* c1 = new TCanvas();
  const int N_POINTS = 16;
  const double TACCHETTE_APPREZZABILI = 0.5;
  const double m_cal = 1.017;
  const double q_cal = 2.635;

  // Oscill (V)
  Float_t oscill[N_POINTS] = {420, 440, 460, 480, 500, 520, 540, 560,
                              580, 600, 620, 640, 660, 680, 700, 720};
  // Mult (A)
  Float_t mult[N_POINTS] = {0.02, 0.03, 0.04, 0.06, 0.08, 0.13, 0.2,  0.3,
                            0.45, 0.67, 1,    1.51, 2.23, 3.3,  4.84, 7.66};
  Float_t e_oscill[N_POINTS] = {
      100, 100, 100, 100, 100, 100, 100, 100,
      100, 100, 100, 100, 100, 100, 200};  // VOLTS/DIV
  Float_t e_mult[N_POINTS] = {};

  // Calculate error
  for (int i = 0; i < N_POINTS; i++) {
    // Error on oscilloscope
    e_oscill[i] = sqrt(pow(e_oscill[i] * TACCHETTE_APPREZZABILI / 5., 2) +
                       pow(5. * TACCHETTE_APPREZZABILI / 5., 2) +
                       pow(oscill[i] * 0.03, 2));
    std::cout << "Errore Oscilloscopio -> Misura " << oscill[i] << " : "
              << e_oscill[i] << std::endl;
    // Error on multimeter
    e_mult[i] =
        static_cast<float>(static_cast<int>(mult[i] * 0.015 * 1e4 + 0.5)) /
            1e4 +
        0.02;
    std::cout << "Errore Multimetro -> Misura: " << mult[i] << " : "
              << e_mult[i] << std::endl
              << std::endl;
  }

  // Fix Oscilloscope measure
  for (size_t i = 0; i < N_POINTS; i++) {
    oscill[i] = (oscill[i] - q_cal) / m_cal;
  }

  TGraphErrors* gr = new TGraphErrors(N_POINTS, oscill, mult, e_oscill, e_mult);
  gr->SetTitle("Diodo al Si");

  gr->SetMarkerStyle(4);
  gr->Draw("AP");
  c1->SetLogy();
  c1->SetGrid();
  gr->GetXaxis()->SetTitle("V(mV)");
  gr->GetYaxis()->SetTitle("I(mA)");

  TF1* f1 = new TF1("f1", "[0]*(exp(x/[1]) - 1.)", 350, 800);
  f1->SetParameter(0, 1E-6);
  f1->SetParName(0, "I0");

  f1->SetParameter(1, 50.);
  f1->SetParName(1, "#etaV_{T}");

  gr->Fit("f1", "R");
  gStyle->SetOptFit(1111);
}
