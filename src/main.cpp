#include "../includes/ion.hh"

/*
std::function<icu::UnicodeString(icu::UnicodeString x)> i = [](icu::UnicodeString x){
  return x;
};
*/

int main(const int argc, const char** argv) {
  if (argc != 2) {
    throw std::runtime_error("Error: Incorrect number of args given in compiler enterence. Needs 2, got " + std::to_string(argc));
  }

  Compiler<icu::UnicodeString> enterence(Ion::Utils::getSource(argv[1]));

  // const auto compiler = enterence << i;
  // const Compiler<void> compiler = enterence << foo;

  return 0;
}
