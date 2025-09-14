#ifndef W_TOKEN_STREAM_HPP
#define W_TOKEN_STREAM_HPP

#include <deque>

#include <frontend/ast/nodes.hpp>
#include <frontend/lexer.hpp>

namespace W {
    struct TokenStream {
    public:
        TokenStream(Lexer& lexer);
        TokenStream(const TokenStream&) = delete;
        TokenStream(TokenStream&&) noexcept = default;
        ~TokenStream() = default;

        TokenStream& operator=(const TokenStream&) = delete;
        TokenStream& operator=(TokenStream&&) noexcept = default;

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
