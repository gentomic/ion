#ifndef __CONSTRUCTS__
#define __CONSTRUCTS__

namespace Ion::Constructs {
  enum class State : unsigned short {
    START, IDENTIFIER, NUMBER, CHAR, STRING,
    COMMENT_LINE, COMMENT_BLOCK,

    // delimiters
    DELIMITER, SEMICOLON, COMMA, WHITESPACE,

    /*
    // operators
    PLUS, MINUS, MULT, DIV, MOD, CONCAT,
    ASSIGN, ARROW, QUESTION_MARK,
    COLON, DOT, AT, LOGICAL_NOT, BANG,
    BIT_OR, BIT_AND, BIT_NOT, BIT_XOR, BIT_LEFT, BIT_RIGHT,
    GREATER_THAN, LESS_THAN, GREATER_EQUALS, LESS_EQUALS,
    EQUALS, NOT_EQUALS, LOGICAL_AND, LOGICAL_XOR, LOGICAL_OR,
    DIVIDER, MULTIPLIER, POINTER, MEMORY, REF, MUT_REF, RETURN,

    // keywords
    I8, I16, I32, I64, I128,
    U8, U16, U32, U64, U128,
    F8, F16, F32, F64, F128,
    K_CHAR, STRING, EXPR, FN, BOOL,
    ARRAY, SET, TYPE, ENUM, CLASS,
    OBJ, INTERFACE, GENERIC, UNIT,
    NIL, UNDEFINED,
    

    // annotations
    // adjust later
    GENERIC, MAKE_NEW, CONSTRUCTOR, DECONSTRUCTOR,
    GLOBAL, EXPORT, SHOW, EXTERN,
    WARNING, ERROR, INLINE, ENTRY, PUBLIC, PRIVATE,
    PROTECTED, VIRTUAL, OVERRIDE, FRIEND, FINAL, STATIC, RESERVE,
    MACRO, EXTENDS, ASYNC, AWAIT, HALT, RESUME, REFLECT,
    USING, OPERATOR, TEMPLATE, EVAL, EXTENDS, LINE, APPEND,
    CONFIG,
    // make config stuff later
    */

    ERROR //, NUM_STATES
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
// using Runner = Ion::Constructs::Runner;

// #include "./ast.hh"

#endif
