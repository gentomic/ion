#ifndef __CONSTRUCTS__
#define __CONSTRUCTS__

#include "./ion.hh"

namespace Ion::Constructs {
  enum class State {

  };

  struct Token {
    const State type;
    const icu::UnicodeString lexum;

    Token(State type, icu::UnicodeString lexum) : type(type), lexum(lexum) { };
    ~Token() = default;
  };

  template <typename T>
  class CompilerMonad {
    public:
    const T data;

    template <typename M>
    CompilerMonad<M> const operator<<(std::function<M(T)> f) {
      return CompilerMonad(f(data));
    }

    /*
    template <typename M>
    CompilerMonad<M> const operator>>=(std::function<CompilerMonad<M>(T)> f) {
      return f(data);
    }
    */

    CompilerMonad(T data) : data(data) { };
    ~CompilerMonad() = default;
  };
}

template <typename T>
using Compiler = Ion::Constructs::CompilerMonad<T>;
using State = Ion::Constructs::State;
using Token = Ion::Constructs::Token;

#endif
