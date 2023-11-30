#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>

int main() {
  double value = 150;
  double err = 12.3;

  std::stringstream stream;
  stream << std::setprecision(1) << err;
  std::string err_str = stream.str();
  std::cout << err_str << '\n';
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

  std::cout << counter << " , " << power;
  double pow = 1.;
  double approx = 0.5;
  if (value < 0) approx *= -1;

  if (power == 0) {
    for (int i = 0; i < counter; i++) {
      pow *= 10;
    }
    value = static_cast<float>(static_cast<int>(value * pow + approx)) / pow;
    std::cout << "\n\n" << value << " ± " << err_str;
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