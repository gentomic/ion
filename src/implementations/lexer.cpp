#include "../ion.hh"

/*
State Lexer::define(std::string_view lexum) {
  const std::unordered_map<std::string_view, State> reserved = {
    {"nil", State::NIL}, {"undefined", State::UNDEFINED},
  };

  
    // annotations
    // adjust later
    GENERIC, MAKE_NEW, CONSTRUCTOR, DECONSTRUCTOR,
    GLOBAL, EXPORT, SHOW, EXTERN,
    WARNING, ERROR, INLINE, ENTRY, PUBLIC, PRIVATE,
    PROTECTED, VIRTUAL, OVERRIDE, FRIEND, FINAL, STATIC, RESERVE,
    MACRO, EXTENDS, ASYNC, AWAIT, HALT, RESUME, REFLECT,
    USING, OPERATOR, TEMPLATE, EVAL, EXTENDS, LINE, APPEND,
    CONFIG,

  const auto it = reserved.find(lexum);
  return (it != reserved.end()) ? reserved.at(lexum) : State::IDENTIFIER;
}
*/

State Lexer::tStart(const UChar32& c,  Constructs::Runner& runner) {
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

State Lexer::transition(const State last, const UChar32& c, Constructs::Runner& runner) {
  switch(last) {
    case State::START: return Lexer::tStart(c, runner);
    case State::IDENTIFIER: if (Utils::isId(c)) return State::IDENTIFIER;
    case State::NUMBER: if (Utils::isNumeric(c)) return State::NUMBER;
    default: return State::ERROR;
  }
  return State::ERROR;
} // create submachines later "sm"s

std::unique_ptr<Token> Lexer::accumulate(Constructs::Runner& runner) {
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

/*
std::unique_ptr<Token> Lexer::accumulate(icu::StringCharacterIterator& it) {
  icu::UnicodeString lexum;
  UChar32 c = it.next32PostInc();
  State current = State::START;
  State last;

  while (it.hasNext()) {
    last = current;
    current = Lexer::transition(last, c);
    if (current == State::ERROR) break;
    lexum += c;
    c = it.next32PostInc();
  }
}
*/

// EXAMPLE: " ++ -- "
// Start -> Id -> Id -> Error | Start -> Id -> Id -> Error
// N/A -> '+' -> '+' -> N/A | N/A -> '-' -> '-' -> N/A

static const LexerFn Lexer::run = [](const Compiler<std::string_view> compiler){
  const icu::UnicodeString source = icu::UnicodeString::fromUTF8(compiler.unwrap());
  std::vector<std::unique_ptr<Token>> tokens;

  icu::StringCharacterIterator it(source);
  Constructs::Runner runner(it);

  for (runner.it.setToStart(); runner.it.hasNext();) {
    tokens.push_back(Lexer::accumulate(runner));
    Sub::auxStore = ""; // clear global auxiliary Store
  }

  return tokens;
};

// submachines
State Sub::sChar(Constructs::Runner& runner) {
  UChar32 c = runner.forward(); // skip over first '
  if (runner.forward() != '\'') std::runtime_error("ERROR: Missing '\'' at line: " + runner.ln); // skip over second '
  // ^ might have an off by one error; check later

  Sub::auxStore += c; // append to aux

  return State::CHAR;
}

State Sub::string(Constructs::Runner& runner) {
  UChar32 c = runner.forward();

  while (c != '"') {
    Sub::auxStore += c;
    c = runner.forward();
  }

  runner.forward(); // skips second "

  return State::STRING;
}

State Sub::commentLine(Constructs::Runner& runner) {
  while (runner.peek() != '\n') {
    runner.forward();
  }

  return State::COMMENT_LINE;
}

State Sub::commentBlock(Constructs::Runner& runner) {
  UChar32 c = runner.current();

  while (runner.current() != '*' && runner.peek() != '/') {
    runner.forward();
  }
  runner.forward(); // eats final '/'

  return State::COMMENT_BLOCK;
}
