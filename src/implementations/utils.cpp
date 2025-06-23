#include "../includes/utils.hh"

std::string const Utils::readFile(const std::string& fileName) {
  std::ifstream file(fileName);
  if (!file) {
    throw std::runtime_error("Could not open file: " + fileName);
  }

  std::string contents;
  file.seekg(0, std::ios::end);
  contents.resize(file.tellg());
  file.seekg(0, std::ios::beg);
  file.read(&contents[0], contents.size());
  return contents;
}

bool Utils::isAlpha(UChar32 c) {
  return u_isalpha(c);
}

bool Utils::isNumeric(UChar32 c) {
  auto cat = u_charType(c);
  return cat == U_DECIMAL_DIGIT_NUMBER ||
    cat == U_LETTER_NUMBER ||
    cat == U_OTHER_NUMBER;
}

bool Utils::isSymbolic(UChar32 c) {
  auto cat = u_charType(c);
  return cat == U_MATH_SYMBOL ||
    cat == U_CURRENCY_SYMBOL ||
    cat == U_MODIFIER_SYMBOL ||
    cat == U_OTHER_SYMBOL;
}

bool Utils::isId(UChar32 c) {
  return Utils::isAlpha(c) ||
    Utils::isNumeric(c) ||
    Utils::isSymbolic(c);
}

void Testing::OutLexer(const Compiler<std::vector<std::unique_ptr<Token>>>& compiler) {
  const std::vector<std::unique_ptr<Token>>& tokens = compiler.unwrap();

  for (const auto& token : tokens) {
    std::cout << "Token({lexum:";

    for (const auto& ch : token->lexum) {
      std::wcout << (wchar_t)ch;
    }
    
    std::cout << ", state:" << static_cast<unsigned short>(token->state) << "})\n";
  }
  std::cout << std::endl;
}
