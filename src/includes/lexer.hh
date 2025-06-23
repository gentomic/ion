#ifndef __LEXER__
#define __LEXER__

#include "../ion.hh"

using LexerOutput = std::vector<std::unique_ptr<Token>>;
using LexerFn = std::function<LexerOutput(const Compiler<std::string_view>)>;

namespace Ion::Processes::Lexer {
  State tStart(const UChar32& c,  Runner& runner);
  State transition(const State last, const UChar32& c, Runner& runner);
  std::unique_ptr<Token> accumulate(Runner& runner);
  static const LexerFn run;
}

namespace Ion::Processes::Lexer::Submachines {
  static icu::UnicodeString auxStore = "";
  State string(Runner& runner);
  State sChar(Runner& runner);
  State commentLine(Runner& runner);
  State commentBlock(Runner& runner);
}

namespace Lexer = Ion::Processes::Lexer;
namespace Sub = Ion::Processes::Lexer::Submachines;

#endif
