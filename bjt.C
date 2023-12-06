
#include <iomanip>
#include <sstream>
#include <string>

void build_graph(TGraphErrors* graph, Color_t color, int num, double fit_min,
                 double fit_max, double prov_m = 0.4, double prov_q = 0);
void calc_err(std::string instrument, Double_t* data, Double_t* err,
              int N_POINTS);
void set_box_stats(TGraphErrors* gr100, TGraphErrors* gr200,
                   double width = 0.25, double height = 0.1, double x = 0.65,
                   double y = 0.15, double vert_spacing = 0.01);
void gen_latex_table(Double_t* fondoscala, Double_t* volt_err, Double_t* volt,
                     Double_t* curr, Double_t* curr_err, Double_t* curr22,
                     Double_t* curr2_err, int N_POINTS);
void approx(double value, double err);
/*

  MACRO

*/
void bjt() {
  TCanvas* c1 = new TCanvas();
  c1->SetGrid();
  gStyle->SetOptFit(0111);

  const int N_POINTS = 27;

  // Data
  Double_t volt[N_POINTS] = {0.06, 0.08, 0.10, 0.12, 0.14, 0.16, 0.18,
                             0.20, 0.24, 0.28, 0.32, 0.36, 0.40, 0.60,
                             0.72, 0.76, 0.80, 0.84, 0.88, 0.92, 0.96,
                             1,    1.5,  2,    2.5,  3,    4};  // Volt
  Double_t volt_err[N_POINTS] = {0.02, 0.02, 0.02, 0.02, 0.1, 0.1, 0.1,
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

  // Calculating errors
  Double_t fondoscala[N_POINTS];  // Don't touch
  std::copy(volt_err, volt_err + N_POINTS, fondoscala);
  calc_err("MULT", curr100, curr100_err, N_POINTS);
  calc_err("MULT", curr50, curr50_err, N_POINTS);
  calc_err("OSCILL", volt, volt_err, N_POINTS);

  gen_latex_table(fondoscala, volt_err, volt, curr100, curr100_err, curr50,
                  curr50_err, N_POINTS);

  TGraphErrors* gr100 =
      new TGraphErrors(N_POINTS, volt, curr100, volt_err, curr100_err);

  TGraphErrors* gr200 =
      new TGraphErrors(N_POINTS, volt, curr50, volt_err, curr50_err);

  // Graph - Color - Current Value - Fit Min - Fit Max - Fit M (Optional) - Fit
  // Q (Optional)
  build_graph(gr100, kRed, 100, 0.5, 4);
  build_graph(gr200, kBlue, 50, 0.5, 4);

  TMultiGraph* mg = new TMultiGraph();
  mg->Add(gr100);
  mg->Add(gr200);
  mg->SetTitle("Caratteristica di uscita del BJT; -V_{CE} (V); -I_{C} (mA)");
  // mg->GetXaxis()->SetTitle("-V_{CE} (V)");
  // mg->GetYaxis()->SetTitle("-I_{C} (mA)");
  mg->Draw("AP");

  TLegend* legend = new TLegend(0.15, 0.79, 0.35, 0.89);
  legend->AddEntry(gr100, "I_{b} = -100#muA", "lp");
  legend->AddEntry(gr200, "I_{b} = -50#muA", "lp");
  legend->Draw();

  // Statistics Box Cosmetics
  c1->Update();
  set_box_stats(gr100, gr200, 0.25, 0.1, 0.65, 0.15, 0.01);
  c1->Modified();

  // Calculate Delta I_C
  Double_t delta_I[N_POINTS] = {};
  for (int i = 0; i < N_POINTS; i++) {
    delta_I[i] =
        abs(curr50[i] - curr100[i]) / 0.05;  // Divided by \DeltaI (50microA)
  }

  TCanvas* c2 = new TCanvas();
  c2->SetGrid();
  // Plot Delta I_C
  TGraph* grDelta = new TGraph(N_POINTS, volt, delta_I);
  grDelta->SetMarkerStyle(8);
  grDelta->SetMarkerColor(kBlue);
  grDelta->GetXaxis()->SetTitle("-V_{CE} (V)");
  grDelta->GetYaxis()->SetTitle("#DeltaI_{C}/#DeltaI_{B}");
  grDelta->SetTitle("Guadagno di corrente");
  grDelta->Draw("AP");

  TCanvas* c3 = new TCanvas();
  TGraphErrors* gr100_inv =
      new TGraphErrors(N_POINTS, curr100, volt, curr100_err, volt_err);

  TGraphErrors* gr50_inv =
      new TGraphErrors(N_POINTS, curr50, volt, curr50_err, volt_err);

  // Graph - Color - Current Value - Fit Min - Fit Max - Fit M (Optional) - Fit
  // Q (Optional)
  build_graph(gr100_inv, kRed, 100, 15.5, 20);
  build_graph(gr50_inv, kBlue, 50, 8.6, 10);

  TMultiGraph* mg_inv = new TMultiGraph();
  mg_inv->Add(gr100_inv);
  mg_inv->Add(gr50_inv);
  mg_inv->SetTitle("Inversa;  -I_{C} (mA); -V_{CE} (V)");
  mg_inv->Draw("AP");
}
/*

  BUILD GRAPH AND COSMETICS

*/
void build_graph(TGraphErrors* graph, Color_t color, int num, double fit_min,
                 double fit_max, double prov_m = 0.4, double prov_q = 3) {
  graph->SetMarkerStyle(8);
  graph->SetMarkerColor(color);

  TF1* f = new TF1(("f" + std::to_string(num)).c_str(), "[0]*x+[1]", fit_min,
                   fit_max);
  f->SetLineColor(color);
  f->SetParameter(0, prov_m);
  f->SetParName(0, ("b_{" + std::to_string(num) + "}").c_str());
  f->SetParameter(1, prov_q);
  f->SetParName(1, ("a_{" + std::to_string(num) + "}").c_str());
  graph->Fit(("f" + std::to_string(num)).c_str(), "R");
}
/*

  CALCULATE ERROR
  unit 1 -> Volt
  unit 0.001 -> mVolt
*/
void calc_err(std::string instrument, Double_t* data, Double_t* err,
              int N_POINTS) {
  for (int i = 0; i < N_POINTS; i++) {
    if (instrument == "OSCILL") {  // Error on Oscilloscope (Voltage)
      const double TACCHETTE_APPREZZABILI = 0.5;
      err[i] = sqrt(pow(err[i] * TACCHETTE_APPREZZABILI / 5., 2) +
                    pow(0.005 * TACCHETTE_APPREZZABILI / 5., 2) +
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

/*

  GENERATE LATEX TABLE WITH DATA AND ERRORS

*/
void gen_latex_table(Double_t* fondoscala, Double_t* volt_err, Double_t* volt,
                     Double_t* curr, Double_t* curr_err, Double_t* curr2,
                     Double_t* curr2_err, int N_POINTS) {
  std::cout
      << "\\begin{table}[] \n\\centering \n\\begin{tabular} {| c | c | c | c "
         "|} "
         "\n\\hline \n"
         "\\textbf{Fondo Scala (V)} & \\textbf{Tensione V(V)} & "
         "\\textbf{Corrente I(mA) $-50\\mu A$}  & \\textbf{Corrente I(mA) "
         "$-100\\mu A$}\\\\ \\hline \n";

  for (int i = 0; i < N_POINTS; i++) {
    std::cout << "\t" << std::fixed << std::setprecision(3) << fondoscala[i]
              << " & $";
    approx(volt[i], volt_err[i]);
    std::cout << "$ & $";
    approx(curr2[i], curr2_err[i]);
    std::cout << "$ & $";
    approx(curr[i], curr_err[i]);
    std::cout << "$  \\\\ \\hline \n";
  }
  std::cout << "\\end{tabular} \n\\end{table}\n\n\n\n";
}

/*

  APPROXIMATION

*/
void approx(double value, double err) {
  std::stringstream stream;
  stream << std::setprecision(1) << err;
  std::string err_str = stream.str();
  stream.str("");

  int counter = 0;
  int power = 0;
  bool has_found_period = false;

  for (int i = 0; i < err_str.length(); i++) {
    if (err_str[i] == '.') {
      has_found_period = true;
      continue;
    }
    if (err_str[i] == 'e') {
      power = std::stoi(err_str.substr(i + 1, err_str.length()));
      break;
    }
    if (has_found_period) {
      counter += 1;
    }
  }

  double pow = 1.;
  double approx = 0.5;
  if (value < 0) approx *= -1;

  if (power == 0) {
    for (int i = 0; i < counter; i++) {
      pow *= 10;
    }
    std::cout << std::fixed << std::setprecision(counter)
              << static_cast<float>(static_cast<int>(value * pow + approx)) /
                     pow
              << " \\pm " << err_str;
  } else {
    for (int i = 0; i < counter + 1; i++) {
      pow /= 10.;
    }
    value = static_cast<float>(static_cast<int>(value * pow + approx));
    std::string pow_str = "";
    if (power > 1) {
      pow_str = "^{" + std::to_string(power) + "}";
    }
    std::cout << "\n\n"
              << "(" << value << " ± "
              << err_str.substr(0, err_str.length() - 4) << ") \\cdot 10";
  }
}