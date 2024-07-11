#include "parser.h"

#include <istream>
#include <stdexcept>

#include "tokenizer.h"

using namespace std;
using namespace boost;

Parser::Parser() :
    m_tokenizer(new Tokenizer)
{

}

Parser::~Parser()
{

}

Parser::ItemList Parser::run(const boost::filesystem::path& path)
{
    ifstream ifs(path.string());
    if (!ifs.is_open()) {
        throw std::runtime_error("Failed to open file");
    }

    ItemList result;

    using TokenType = Tokenizer::Token::Type;

    string str;
    bool mlc = false;

    vector<Tokenizer::Token> lineTokens;
    Tokenizer::Token t;

    nextline:
    while (getline(ifs, str)) {
        lineTokens.clear();

        m_tokenizer->setString(str);
        while (m_tokenizer->next(t)) {
            if (t.type == TokenType::CommentBegin) {
                mlc = true;
            } else if (t.type == TokenType::CommendEnd) {
                mlc = false;
            } else if (t.type == TokenType::Comment) {
                if (!mlc) {
                    break;
                }
            } else if (!mlc) {
                if (t.type == TokenType::Unknown) {
                    if (t.value != " ") {
                        goto nextline;
                    }
                } else if (t.type == TokenType::Include) {
                    if (!lineTokens.empty()) {
                        goto nextline;
                    }
                    
                    lineTokens.push_back(t);
                } else if (t.type == TokenType::StringBracketed || 
                           t.type == TokenType::StringQuoted) {
                    if (lineTokens.size() != 1) {
                        goto nextline;
                    }
                    
                    lineTokens.push_back(t);
                }
            }
        }

        if (lineTokens.size() == 2) {
            Item item;

            switch (lineTokens.back().type) {
            case TokenType::StringBracketed:
                item.scope = Item::Scope::Global;
                break;
            case TokenType::StringQuoted:
                item.scope = Item::Scope::Local;
                break;
            default:
                assert(false);
            }

            const auto& path = lineTokens.back().value;
            item.path = path.substr(1, path.size() - 2);
            
            result.push_back(std::move(item));
        }
    }

    return result;
}