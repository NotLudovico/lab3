#ifndef LATEX
#define LATEX

#include <iostream>
#include <vector>

#include "nums.hpp"

class ValErr {
  Double_t* val;
  Double_t* err;
  bool has_error = false;

 public:
  ValErr(Double_t* val_arr, Double_t* err_arr)
      : val{val_arr}, err{err_arr}, has_error{true} {};

  ValErr(Double_t* val_arr) : val{val_arr}, has_error{false} {};

  void print(int index) const {
    if (has_error) {
      vector<string> values = approx(val[index], err[index]);
      std::cout << "$ " << values[0] << " \\pm " << values[1] << " $";
    } else {
      std::cout << "$ " << val[index] << " $";
    }
  }
};

void gen_latex_table(vector<const char*> titles, vector<ValErr> data,
                     int N_POINTS) {
  std::cout << "\\begin{table}[] \n\\centering \n\\begin{tabular} {|";
  for (int i = 0; i < titles.size(); i++) {
    std::cout << " c |";
  }
  std::cout << "}\n\\hline \n";

  for (int i = 0; i < titles.size(); i++) {
    std::cout << "\\textbf{" << titles[i] << "} ";

    if (i != titles.size() - 1) {
      std::cout << " & ";
    }
  }

  std::cout << " \\\\ \\hline \n";

  for (int j = 0; j < N_POINTS; j++) {
    std::cout << "\t";
    for (int i = 0; i < titles.size(); i++) {
      data[i].print(j);

      if (i != titles.size() - 1) {
        std::cout << " & ";
      }
    }
    std::cout << " \\\\ \\hline \n";
  }
  std::cout << "\\end{tabular} \n\\caption{} \n\\end{table}\n\n\n\n";
}
#endif