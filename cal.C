#include<cmath>

void cal() {
  TCanvas* c1 = new TCanvas();
  const int N_POINTS = 9;
  const double TACCHETTE_APPREZZABILI = 0.5;

  Float_t mult[N_POINTS] = {76.2, 95.7, 144.1, 195.1, 242.3, 292.8, 389.6, 491.4, 586.6};
  Float_t oscill[N_POINTS] = {80,100,150,200,250,300,400,500,600};
  Float_t e_mult[N_POINTS] = {};
  Float_t e_oscill[N_POINTS] = {20,20,50,50,50,50,100,100,100};  // Riportare VOLT/DIV

  // Calculate error 
  for (int i = 0; i < N_POINTS; i++) {
    // Error on oscilloscope
    e_oscill[i] = sqrt(2. * pow(e_oscill[i] * TACCHETTE_APPREZZABILI / 5., 2) + pow(oscill[i] * 0.03,2));
    // Error on multimeter
    e_mult[i] = static_cast<float>(static_cast<int>(mult[i] * 0.003 * 10. + 0.5)) / 10. + 0.1;
    std::cout << e_mult[i] << std::endl;
  }

  

  TGraphErrors* gr = new TGraphErrors(N_POINTS, mult, oscill, e_mult, e_oscill);
  gr->SetTitle("Retta di Calibrazione");

  gr->SetMarkerStyle(4);
  gr->Draw("AP");
  c1->SetGrid();
  gr->GetXaxis()->SetTitle("Multimetro (mV)");
  gr->GetYaxis()->SetTitle("Oscilloscopio (mV)");

  TF1* f1 = new TF1("f1", "[0]*x+[1]", 70, 650);
  f1->SetParameter(0, 1.);
  f1->SetParName(0, "m");

  f1->SetParameter(1, 0.);
  f1->SetParName(1, "q");

  gr->Fit("f1", "R");
  gStyle->SetOptFit(1111);
}