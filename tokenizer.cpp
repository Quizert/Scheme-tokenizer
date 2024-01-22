#include <tokenizer.h>

Tokenizer::Tokenizer(std::istream* in) : input_(in) {
    Next();
}

void Tokenizer::DeleteSpaces() {
    char c;
    while (input_->get(c)) {
        if (!std::isspace(c)) {
            input_->putback(c);
            break;
        }
    }
}

Token Tokenizer::ReadToken() {
    DeleteSpaces();
    char c = input_->peek();
    if (std::isdigit(c)) {
        return GetConstant(false);
    } else if (c == '-') {
        input_->get(c);
        if (std::isdigit(input_->peek())) {
            return GetConstant(true);
        } else {
            input_->putback('-');
            return GetSymbol();
        }
    } else if (c == '(' || c == ')') {
        return GetBracket();
    } else if (std::isalpha(c) || c == '+' || c == '<' || c == '=' || c == '>' || c == '*' ||
               c == '/' || c == '#' || c == '?') {
        return GetSymbol();
    } else if (c == '\'') {
        return GetQuote();
    } else if (c == '.') {
        return GetDot();
    } else if (input_->eof()) {
        return Token();
    }
}

ConstantToken Tokenizer::GetConstant(bool is_negative) {
    std::string num_str;
    char c;
    while (input_->get(c) && std::isdigit(c)) {
        num_str += c;
    }
    input_->putback(c);
    int num = std::stoi(num_str);
    if (is_negative) {
        return ConstantToken{-num};
    }
    return ConstantToken{num};
}

BracketToken Tokenizer::GetBracket() {
    char c = input_->get();
    if (c == '(') {
        return BracketToken::OPEN;
    }
    return BracketToken::CLOSE;
}

SymbolToken Tokenizer::GetSymbol() {
    std::string str;
    char c;
    while (input_->get(c) &&
           (std::isalnum(c) || c == '-' || c == '+' || c == '?' || c == '!' || c == '<' ||
            c == '=' || c == '>' || c == '*' || c == '/' || c == '#')) {
        str += c;
    }
    input_->putback(c);
    return SymbolToken{str};
}

QuoteToken Tokenizer::GetQuote() {
    char c;
    input_->get(c);
    return {};
}
DotToken Tokenizer::GetDot() {
    char c;
    input_->get(c);
    return {};
}
bool Tokenizer::IsEnd() {
    return input_->eof();
}
void Tokenizer::Next() {
    if (!IsEnd()) {
        current_token_ = ReadToken();
    }
}
Token Tokenizer::GetToken() {
    if (current_token_) {
        return *current_token_;
    }
}
bool SymbolToken::operator==(const SymbolToken& other) const {
    return this->name == other.name;
}
bool QuoteToken::operator==(const QuoteToken&) const {
    return true;
}
bool DotToken::operator==(const DotToken&) const {
    return true;
}
bool ConstantToken::operator==(const ConstantToken& other) const {
    return this->value == other.value;
}
