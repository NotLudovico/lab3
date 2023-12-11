#ifndef GRAPH
#define GRAPH
TF1* fit(TGraph* graph, const char* fit_formula,
         vector<const char*> param_names = {""},
         vector<Double_t> param_values = {0}, double fit_min = 0,
         double fit_max = 10, Color_t color = kRed) {
  TF1* f = new TF1("fit", fit_formula, fit_min, fit_max);
  f->SetLineColor(color);
  for (int i = 0; i < param_names.size(); i++) {
    f->SetParameter(i, param_values[i]);
    f->SetParName(i, param_names[i]);
  }
  graph->Fit("fit", "R");
  return f;
}

void cosm(TGraph* graph, const char* title = "", double marker_size = 0.6,
          int marker_style = 8, Color_t marker_color = kBlack) {
  graph->SetTitle(title);
  graph->SetMarkerSize(marker_size);
  graph->SetMarkerStyle(marker_style);
  graph->SetMarkerColor(marker_color);
}

void set_box_stats(TCanvas* canvas, TGraph* graph, double x = 0.65,
                   double y = 0.15, double width = 0.25, double height = 0.1) {
  canvas->Update();

  TPaveStats* st = (TPaveStats*)graph->FindObject("stats");
  st->SetX1NDC(x);  // new x start position
  st->SetX2NDC(x + width);
  st->SetY1NDC(y);  // new y start position
  st->SetY2NDC(y + height);

  canvas->Modified();
}
#endif