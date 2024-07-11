#pragma once

#include <string>
#include <memory>

class Tokenizer
{
public:
    struct Token
    {
        enum class Type {
            Comment,
            CommentBegin,
            CommendEnd,
            Include,
            StringQuoted,
            StringBracketed,
            Unknown
        };

        Type type;
        std::string value;
    };

    Tokenizer();
    ~Tokenizer();

    void setString(const std::string& str);
    bool next(Token& token);
private:
    struct PImpl;
    std::unique_ptr<PImpl> m_pimpl;
};