#include "tokenizer.h"

#include <map>
#include <regex>
#include <sstream>

using namespace std;

namespace {
    using TokenType = Tokenizer::Token::Type;

    const map<TokenType, string> tokenRegexMap {
        {TokenType::Comment, "//"},
        {TokenType::CommentBegin, "/\\*"},
        {TokenType::CommendEnd, "\\*/"},
        {TokenType::Include, "#\\s*include"},
        {TokenType::StringQuoted, "\"[^\"]+\""},
        {TokenType::StringBracketed, "<[^>]+>"},
        {TokenType::Unknown, ".+?"}
    };
}

struct Tokenizer::PImpl
{
    string str;
    regex expr;
    sregex_iterator it;
};

Tokenizer::Tokenizer() : 
    m_pimpl(new PImpl)
{
    stringstream ss;

    for (auto it = tokenRegexMap.cbegin(); it != tokenRegexMap.cend(); ++it) {
        if (it != tokenRegexMap.cbegin()) {
            ss << '|';
        }

        ss << '(' << it->second << ')';
    }

    m_pimpl->expr.assign(ss.str());
}

Tokenizer::~Tokenizer()
{

}

void Tokenizer::setString(const std::string& str)
{
    m_pimpl->str = str;
    m_pimpl->it = sregex_iterator(m_pimpl->str.begin(), m_pimpl->str.end(), m_pimpl->expr);
}

bool Tokenizer::next(Token& token)
{
    if (m_pimpl->it == sregex_iterator()) {
        return false;
    }

    const std::smatch &match = *m_pimpl->it;

    token.value = match.str();

    for (auto it = tokenRegexMap.cbegin(); it != tokenRegexMap.cend(); ++it) {
        const size_t idx = distance(tokenRegexMap.cbegin(), it) + 1;
        if (match[idx].matched) {
            token.type = it->first;
            break;
        }
    }

    ++m_pimpl->it;
    return true;
}
