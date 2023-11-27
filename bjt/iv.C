#include <string>

void build_graph(TGraphErrors* graph, Color_t color, int num, double fit_min,
                 double fit_max, double prov_m = 0.4, double prov_q = 3);
void calc_err(std::string instrument, Double_t* data, Double_t* err,
              int N_POINTS);
void set_box_stats(TGraphErrors* gr100, TGraphErrors* gr200,
                   double width = 0.25, double height = 0.1, double x = 0.65,
                   double y = 0.15, double vert_spacing = 0.01);
void gen_latex_table(Int_t* fondoscala, Double_t* volt_err, Double_t* volt,
                     Double_t* curr, Double_t* curr_err, int N_POINTS);
/*

  MACRO

*/
void iv() {
  TCanvas* c1 = new TCanvas();
  c1->SetGrid();
  gStyle->SetOptFit(0111);

  const int N_POINTS = 5;

  // Data
  Double_t volt[N_POINTS] = {0.5, 1, 1.5, 2, 2.5};
  Double_t volt_err[N_POINTS] = {20, 20, 20, 20, 20};  // Volt/Div
  Double_t curr100[N_POINTS] = {0, 1, 1.2, 1.3, 1.4};
  Double_t curr200[N_POINTS] = {0, 0.5, 0.6, 0.7, 0.8};

  // Errors (Calculated automatically)
  Double_t curr100_err[N_POINTS] = {};
  Double_t curr200_err[N_POINTS] = {};

  // Calculating errors
  Int_t fondoscala[N_POINTS];  // Don't touch
  std::copy(volt_err, volt_err + 5, fondoscala);
  calc_err("MULT", curr100, curr100_err, N_POINTS);
  calc_err("MULT", curr200, curr200_err, N_POINTS);
  calc_err("OSCILL", volt, volt_err, N_POINTS);

  gen_latex_table(fondoscala, volt_err, volt, curr100, curr100_err, N_POINTS);
  gen_latex_table(fondoscala, volt_err, volt, curr200, curr200_err, N_POINTS);

  TGraphErrors* gr100 =
      new TGraphErrors(N_POINTS, volt, curr100, volt_err, curr100_err);

  TGraphErrors* gr200 =
      new TGraphErrors(N_POINTS, volt, curr200, volt_err, curr200_err);

  build_graph(gr100, kRed, 100, 1, 4);
  build_graph(gr200, kBlue, 200, 1, 4);

  gr100->Draw("AP");
  gr200->Draw("SAME P");

  // Statistics Box Cosmetics
  c1->Update();
  set_box_stats(gr100, gr200, 0.25, 0.1, 0.65, 0.15, 0.01);
  c1->Modified();

  Double_t delta_I[N_POINTS] = {};
  for (int i = 0; i < N_POINTS; i++) {
    delta_I[i] = abs(curr200[i] - curr100[i]);
  }

  TCanvas* c2 = new TCanvas();
  c2->SetGrid();
  TGraph* grDelta = new TGraph(N_POINTS, volt, delta_I);
  grDelta->SetMarkerStyle(8);
  grDelta->SetMarkerColor(kBlue);
  grDelta->GetXaxis()->SetTitle("-V_{CE} (V)");
  grDelta->GetYaxis()->SetTitle("-#DeltaI_{C} (mA)");
  grDelta->SetTitle("#DeltaI");
  grDelta->Draw("AP");
}
/*

  BUILD GRAPH AND COSMETICS

*/
void build_graph(TGraphErrors* graph, Color_t color, int num, double fit_min,
                 double fit_max, double prov_m = 0.4, double prov_q = 3) {
  graph->SetMarkerStyle(4);
  graph->SetMarkerColor(color);
  graph->GetXaxis()->SetTitle("-V_{CE} (V)");
  graph->GetYaxis()->SetTitle("-I_{C} (mA)");
  graph->SetTitle("Caratteristica di uscita del BJT");

  TF1* f = new TF1("f", "[0]*x+[1]", fit_min, fit_max);
  f->SetLineColor(color);
  f->SetParameter(0, prov_m);
  f->SetParName(0, ("m_{" + std::to_string(num) + "}").c_str());
  f->SetParameter(1, prov_q);
  f->SetParName(1, ("q_{" + std::to_string(num) + "}").c_str());
  graph->Fit("f", "R");
}
/*

  CALCULATE ERROR

*/
void calc_err(std::string instrument, Double_t* data, Double_t* err,
              int N_POINTS) {
  for (int i = 0; i < N_POINTS; i++) {
    if (instrument == "OSCILL") {  // Error on Oscilloscope (Voltage)
      const double TACCHETTE_APPREZZABILI = 0.5;
      err[i] = sqrt(pow(err[i] * TACCHETTE_APPREZZABILI / 5., 2) +
                    pow(5. * TACCHETTE_APPREZZABILI / 5., 2) +
                    pow(data[i] * 0.03, 2));
    } else if (instrument == "MULT") {  // Error on multimeter (Current)
      err[i] =
          static_cast<float>(static_cast<int>(data[i] * 0.015 * 1e4 + 0.5)) /
              1e4 +
          0.02;
    } else {
      std::cout << "****** \n INVALID INSTRUMENT \n ******";
    }
  }
}

/*

  SET STATISTICS BOX POSITION

*/
void set_box_stats(TGraphErrors* gr100, TGraphErrors* gr200,
                   double width = 0.25, double height = 0.1, double x = 0.65,
                   double y = 0.15, double vert_spacing = 0.01) {
  TPaveStats* st = (TPaveStats*)gr100->FindObject("stats");
  st->SetX1NDC(x);  // new x start position
  st->SetX2NDC(x + width);
  st->SetY1NDC(y);  // new y start position
  st->SetY2NDC(y + height);

  TPaveStats* st2 = (TPaveStats*)gr200->FindObject("stats");
  st2->SetX1NDC(x);  // new x start position
  st2->SetX2NDC(x + width);
  st2->SetY1NDC(y + vert_spacing + height);  // new y start position
  st2->SetY2NDC(y + vert_spacing + height * 2);
}

void gen_latex_table(Int_t* fondoscala, Double_t* volt_err, Double_t* volt,
                     Double_t* curr, Double_t* curr_err, int N_POINTS) {
  std::string table =
      "\\begin{table}[] \n\\begin{tabular} {| c | c | c |} "
      "\n\\hline \n"
      "\\textbf{Fondo Scala (mV)} & \\textbf{Tensione V(mV)} & "
      "\\textbf{Corrente I(mA)} \\\\ \\hline \n";

  for (int i = 0; i < N_POINTS; i++) {
    table += "\t" + std::to_string(fondoscala[i]) + " & $" +
             std::to_string(volt[i]) + " \\pm " + std::to_string(volt_err[i]) +
             "$ & $" + std::to_string(curr[i]) + " \\pm " +
             std::to_string(curr_err[i]) + "$ \\\\ \\hline \n";
  }
  table += "\\end{tabular} \n\\end{table}";
  std::cout << "\n\n\n\n" << table << "\n\n\n\n";
}