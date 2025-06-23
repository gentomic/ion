#ifndef __LEXER__
#define __LEXER__

#include "../ion.hh"

using LexerOutput = std::vector<std::unique_ptr<Token>>;
using LexerFn = std::function<LexerOutput(const Compiler<std::string_view>)>;

namespace Ion::Processes::Lexer {
  // State define(std::string_view lexum);
  State tStart(const UChar32& c,  Constructs::Runner& runner);
  State transition(const State last, const UChar32& c, Constructs::Runner& runner);
  std::unique_ptr<Token> accumulate(Constructs::Runner& runner);
  static const LexerFn run;
}

namespace Ion::Processes::Lexer::Submachines {
  static icu::UnicodeString auxStore = "";
  State string(Constructs::Runner& runner);
  State sChar(Constructs::Runner& runner);
  State commentLine(Constructs::Runner& runner);
  State commentBlock(Constructs::Runner& runner);
}

namespace Lexer = Ion::Processes::Lexer;
namespace Sub = Ion::Processes::Lexer::Submachines;

#endif
