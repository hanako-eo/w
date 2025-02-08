#ifndef W_TOKEN_STREAM_HPP
#define W_TOKEN_STREAM_HPP

#include <deque>

#include <frontend/ast.hpp>
#include <frontend/lexer.hpp>

namespace W {
    class TokenStream {
    public:
        TokenStream(Lexer& lexer);
        ~TokenStream() = default;

        const Token& peek(size_t advance = 0);
        const Token& next();

        void commit();
        void uncommit();
    
    private:
        std::deque<Token> m_tokens;
        Lexer& m_lexer;
        size_t m_index;
    };
}

#endif
