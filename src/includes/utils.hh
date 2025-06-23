#ifndef __UTILS__
#define __UTILS__

#include "../ion.hh"

namespace Ion::Utils {
  std::string const readFile(const std::string& fileName);
  bool isAlpha(UChar32 c);
  bool isNumeric(UChar32 c);
  bool isSymbolic(UChar32 c);
  bool isId(UChar32 c);
}

namespace Ion::Utils::Testing {
  void OutLexer(const Compiler<std::vector<std::unique_ptr<Token>>>& compiler);
}

namespace Utils = Ion::Utils;
namespace Testing = Ion::Utils::Testing;

#endif
