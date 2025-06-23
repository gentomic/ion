#include "../ion.hh"

State Lexer::tStart(const UChar32& c, Runner& runner) {
  switch (c) {
    case U'\'': return Sub::sChar(runner);
    case U'"': return Sub::string(runner);
    case U'/':
      if (runner.peek() == '/') return Sub::commentLine(runner);
      if (runner.peek() == '*') return Sub::commentBlock(runner);
      return State::IDENTIFIER;
    case U' ': case U'\t': case U'\n': case U'\r':
      return State::WHITESPACE;
    case U'(': case U')': case U'[':
    case U']': case U'{': case U'}':
      return State::DELIMITER;
    case U',': return State::COMMA;
    case U';': return State::SEMICOLON;
    default:
      if (Utils::isNumeric(c)) return State::NUMBER;
      if (Utils::isAlpha(c) || Utils::isSymbolic(c)) return State::IDENTIFIER;
      return State::ERROR;
  }
}

State Lexer::transition(const State last, const UChar32& c, Runner& runner) {
  switch(last) {
    case State::START: return Lexer::tStart(c, runner);
    case State::IDENTIFIER: if (Utils::isId(c)) return State::IDENTIFIER;
    case State::NUMBER: if (Utils::isNumeric(c)) return State::NUMBER;
    default: return State::ERROR;
  }
  return State::ERROR;
}

std::unique_ptr<Token> Lexer::accumulate(Runner& runner) {
  icu::UnicodeString lexum;
  UChar32 c = runner.forward();
  State current = State::START;
  State last;

  while (runner.it.hasNext()) {
    last = current;
    current = Lexer::transition(last, c, runner);
    if (current == State::ERROR) break;
    lexum += c;
    c = runner.forward();
  }

  return std::make_unique<Token>(last, (Sub::auxStore.isEmpty()) ? lexum : Sub::auxStore);
}

// EXAMPLE: " ++ -- "
// Start -> Id -> Id -> Error | Start -> Id -> Id -> Error
// N/A -> '+' -> '+' -> N/A | N/A -> '-' -> '-' -> N/A

static const LexerFn Lexer::run = [](const Compiler<std::string_view> compiler){
  const icu::UnicodeString source = icu::UnicodeString::fromUTF8(compiler.unwrap());
  std::vector<std::unique_ptr<Token>> tokens;

  icu::StringCharacterIterator it(source);
  Runner runner(it);

  for (runner.it.setToStart(); runner.it.hasNext();) {
    tokens.push_back(Lexer::accumulate(runner));
    Sub::auxStore = ""; // clear global auxiliary Store
  }

  return tokens;
};

// submachines
State Sub::sChar(Runner& runner) {
  UChar32 c = runner.forward(); // skip over first '
  if (runner.forward() != '\'') std::runtime_error("ERROR: Missing '\'' at line: " + runner.ln); // skip over second '
  // ^ might have an off by one error; check later

  Sub::auxStore += c; // append to aux

  return State::CHAR;
}

State Sub::string(Runner& runner) {
  UChar32 c = runner.forward();

  while (c != '"') {
    Sub::auxStore += c;
    c = runner.forward();
  }

  runner.forward(); // skips second "

  return State::STRING;
}

State Sub::commentLine(Runner& runner) {
  while (runner.peek() != '\n') {
    runner.forward();
  }

  return State::COMMENT_LINE;
}

State Sub::commentBlock(Runner& runner) {
  UChar32 c = runner.current();

  while (runner.current() != '*' && runner.peek() != '/') {
    runner.forward();
  }
  runner.forward(); // eats final '/'

  return State::COMMENT_BLOCK;
}
