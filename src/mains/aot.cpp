#include "../ion.hh"

int main(int argc, char** argv) {
  if (argc != 2) std::cerr << "ERROR: Incorrect number of args passed";

  const std::string source = Utils::readFile(argv[1]);
  const std::string_view ref = source;
  Compiler<std::string_view> compiler(ref);

  // const Compiler<LexerOutput> output = compiler | Lexer::run;

  Compiler<LexerOutput> c = Lexer::run(compiler);

  Testing::OutLexer(c);

  return 0;
}
