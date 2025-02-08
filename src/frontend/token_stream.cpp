#include <frontend/lexer.hpp>
#include <frontend/token_stream.hpp>

namespace W {
    TokenStream::TokenStream(Lexer& lexer):
        m_lexer(lexer),
        m_index(0)
    {}

    const Token& TokenStream::next() {
        if (m_index == m_tokens.size()) {
            m_tokens.push_back((m_lexer.next()));
        }
        return m_tokens[m_index++];
    }

    const Token& TokenStream::peek(size_t advance) {
        while (m_tokens.size() <= m_index + advance) {
            m_tokens.push_back((m_lexer.next()));
        }
        return m_tokens[m_index + advance];
    }

    void TokenStream::commit() {
        m_tokens.erase(m_tokens.begin(), m_tokens.begin() + m_index);
        m_index = 0;
    }

    void TokenStream::uncommit() {
        m_index = 0;
    }
}
