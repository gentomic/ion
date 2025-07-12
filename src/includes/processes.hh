#ifndef __PROCESSES__
#define __PROCESSES__

#include "./ion.hh"

namespace Ion::Processes {
  class Lexer {
    public:
    const icu::UnicodeString& source;

    static std::function<std::vector<Token>(Compiler<icu::UnicodeString>& last)> run;
    Lexer(icu::UnicodeString& source) : source(source) { };
    ~Lexer() = default;
  };
}

#endif
