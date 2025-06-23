#ifndef __CONSTRUCTS__
#define __CONSTRUCTS__

namespace Ion::Constructs {
  enum class State : unsigned short {
    START, IDENTIFIER, NUMBER, CHAR, STRING,
    COMMENT_LINE, COMMENT_BLOCK,

    // delimiters
    DELIMITER, SEMICOLON, COMMA, WHITESPACE,

    ERROR
  };

  
  class Runner {
    public:
    icu::StringCharacterIterator& it;
    size_t pos = 0;
    size_t ln = 1;

    UChar32 current() { return it.current32(); }

    char forward() {
      const UChar32 c = it.next32PostInc();
      if (c == '\n') {
        pos = 0;
        ln++;
        forward();
      } else {
        pos++;
      }
      
      return c;
    }

    UChar32 peek() {
      const int32_t saved = it.getIndex();

      if (it.hasNext()) {
        const UChar32 c = it.next32PostInc();
        it.setIndex(saved);
        return c;
      }

      return U_SENTINEL; // U'\0'
    }

    Runner(icu::StringCharacterIterator& it) : it(it) { };
    ~Runner() = default;
  };
  
  
  struct Token {
    public:
    State state;
    std::u32string lexum;

    Token(State state, std::u32string lexum) : state(state), lexum(lexum) { };
    Token() = default;
    ~Token() = default;
  };

  template<typename T = void>
  class CompilerMonad {
    private:
    std::optional<T> data;

    public:

    std::optional<T>& show() const {
      return data;
    }

    T& unwrap() const {
      if (!data.has_value()) {
        throw std::runtime_error("ERROR: Attempted to unwrap a std::nullopt");
      }
      return *data;
    }

    template <typename U>
    CompilerMonad<U> operator|(std::function<U(T)> f) {
      if (!data.has_value()) {
        return CompilerMonad<U>(std::nullopt);
      }

      return CompilerMonad<T>(f(*data));
    }

    template <typename U>
    CompilerMonad<U> operator>>=(std::function<CompilerMonad<U>(T)> f) {
      if (!data.has_value()) {
        return CompilerMonad<U>(std::nullopt);
      }

      return f(*data);
    }

    CompilerMonad(T data) : data(std::make_optional<T>(data)) { };
    CompilerMonad(std::optional<T> data = nullopt) : data(data) { };
    ~CompilerMonad() = default;
  };
}

template <typename T>
using Compiler = Ion::Constructs::CompilerMonad<T>;
using Token = Ion::Constructs::Token;
using State = Ion::Constructs::State;
using Runner = Ion::Constructs::Runner;

#include "./ast.hh"

#endif
