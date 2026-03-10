#include "json.h"

using namespace std;

namespace json {

namespace {

Node LoadNode(istream& input);

void SkipWhitespace(istream& input) {
    while (std::isspace(input.peek())) {
        input.get();
    }
}

Node LoadArray(istream& input) {
    Array result;
    char c;
    while (input >> c && c != ']') {
        if (c != ',') {
            input.putback(c);
        }
        result.push_back(LoadNode(input));
    }

    if (c != ']') throw ParsingError("Unclosed array");

    return Node(std::move(result));
}

Node LoadNum(istream& input) {
    std::string parsed_num;

    while (std::isdigit(input.peek())
        || input.peek() == '.' || input.peek() == '-'
        || input.peek() == 'e' || input.peek() == 'E'
        || input.peek() == '+') {
        parsed_num += static_cast<char>(input.get());
        }

    try {
        if (parsed_num.find_first_of(".eE"s) != std::string::npos)
            return {std::stod(parsed_num)};

        return {std::stoi(parsed_num)};
    } catch (...) {
        throw ParsingError("Failed to convert number: " + parsed_num);
    }
}

Node LoadString(istream& input) {
    string s;
    char c;
    while (input.get(c)) {
        if (c == '"') {
            return Node(std::move(s));
        }
        if (c == '\\') {
            if (!input.get(c)) {
                throw ParsingError("Unexpected end of input after \\");
            }
            switch (c) {
                case 'n':
                    s += '\n';
                    break;
                case 'r':
                    s += '\r';
                    break;
                case 't':
                    s += '\t';
                    break;
                case '"':
                    s += '"';
                    break;
                case '\\':
                    s += '\\';
                    break;
                default:
                    s += c;
            }
        } else {
            s += c;
        }
    }
    throw ParsingError("String parsing error");
}


Node LoadDict(istream& input) {
    Dict result;
    char c;
    while (input >> c && c != '}') {
        if (c == ',')
            input >> c;

        string key = LoadString(input).AsString();
        input >> c;
        if (c != ':')
            throw ParsingError("Expected ':' in dict");

        result.insert({std::move(key), LoadNode(input)});
    }

    if (c != '}') throw ParsingError("Unclosed dict");
    return Node{std::move(result)};
}

Node LoadBoolOrNull(istream& input) {
        std::string parsed;

        while (std::isalpha(input.peek())) {
            parsed += static_cast<char>(input.get());
        }

        if (parsed == "true") return Node(true);
        if (parsed == "false") return Node(false);
        if (parsed == "null") return Node(nullptr);

        throw ParsingError("Failed to parse bool or null: " + parsed);
    }


Node LoadNode(istream& input) {
    SkipWhitespace(input);

    char c;
    if (!(input >> c))
        throw ParsingError("Unexpected end of input");

    if (c == 'n' || c == 't' || c == 'f') {
        input.putback(c);
        return LoadBoolOrNull(input);
    }

    if (c == '[' ) return LoadArray(input);
    if (c == '{') return LoadDict(input);
    if (c == '"') return LoadString(input);

    if (c == '-' || std::isdigit(c)) {
        input.putback(c);
        return LoadNum(input);
    }

    throw ParsingError("Unexpected character: " + string(1, c));
}

}  // namespace

    ///////////////////////////////////////////////
    /// Node
    ///////////////////////////////////////////////

Node::Node(Array array)
    : value_(std::move(array)) {
}

Node::Node(Dict map)
    : value_(std::move(map)) {
}

Node::Node(int value)
    : value_(value) {
}

Node::Node(string value)
    : value_(std::move(value)) {
}

Node::Node(std::nullptr_t) {
    value_ = nullptr;
}

Node::Node(bool value) {
    value_ = value;
}

Node::Node(double value) {
    value_ = value;
}

bool Node::operator==(const Node &rhs) const {
    return value_ == rhs.value_;
}

bool Node::operator!=(const Node &rhs) const {
    return !(*this == rhs);
}

const Array& Node::AsArray() const {
    if (IsArray()) return std::get<Array>(value_);
    throw std::logic_error("not an array");
}

const Dict& Node::AsMap() const {
    if (IsMap()) return std::get<Dict>(value_);
    throw std::logic_error("not a map");
}

int Node::AsInt() const {
    if (IsInt()) return std::get<int>(value_);
    throw std::logic_error("not an int");
}

const string& Node::AsString() const {
    if (IsString()) return std::get<std::string>(value_);
    throw std::logic_error("not a string");
}

bool Node::AsBool() const {
    if (IsBool()) return std::get<bool>(value_);
    throw std::logic_error("not a bool");
}

double Node::AsDouble() const {
    if (IsPureDouble()) return std::get<double>(value_);
    if (IsInt()) return std::get<int>(value_);
    throw std::logic_error("not a double");
}

double Node::AsPureDouble() const {
    if (IsPureDouble()) return std::get<double>(value_);
    throw std::logic_error("not a pure double");
}

bool Node::IsArray() const {
    return std::holds_alternative<Array>(value_);
}

bool Node::IsMap() const {
    return std::holds_alternative<Dict>(value_);
}

bool Node::IsInt() const {
    return std::holds_alternative<int>(value_);
}

bool Node::IsString() const {
    return std::holds_alternative<string>(value_);
}

bool Node::IsNull() const {
    return std::holds_alternative<std::nullptr_t>(value_);
}

bool Node::IsBool() const {
    return std::holds_alternative<bool>(value_);
}

bool Node::IsDouble() const {
    return std::holds_alternative<double>(value_) ||
        std::holds_alternative<int>(value_);
}

bool Node::IsPureDouble() const {
    return std::holds_alternative<double>(value_);
}

Document::Document(Node root)
    : root_(std::move(root)) {
}

const Node& Document::GetRoot() const {
    return root_;
}

bool Document::operator==(const Document &rhs) const {
    return root_ == rhs.root_;
}

bool Document::operator!=(const Document &rhs) const {
    return !(*this == rhs);
}

Document Load(istream& input) {
    return Document{LoadNode(input)};
}

    ///////////////////////////////////////////////
    /// Print
    ///////////////////////////////////////////////

void PrintNode(const Node& node, std::ostream& out);

std::string EscapeString(const std::string& str) {
    std::string result;
    result.reserve(str.size());
    for (const char c : str) {
        switch (c) {
            case '\n': result += "\\n"; break;
            case '\r': result += "\\r"; break;
            case '\t': result += "\\t"; break;
            case '"':  result += "\\\""; break;
            case '\\': result += "\\\\"; break;
            default:   result += c;
        }
    }
    return result;
}

struct PrintVisitor {
    std::ostream& out;

    void operator()(std::nullptr_t) const {
        out << "null";
    }

    void operator()(const bool value) const {
        out << (value ? "true" : "false");
    }

    void operator()(const int value) const {
        out << value;
    }

    void operator()(const double value) const {
        out << value;
    }

    void operator()(const std::string& value) const {
        out << "\"" << EscapeString(value) << "\"";
    }

    void operator()(const Array& value) const {
        out << "[\n";
        bool first = true;
        for (const Node& node : value) {
            if (!first) out << ",\n";
            first = false;
            PrintNode(node, out);
        }
        out << "\n]";
    }

    void operator()(const Dict& dict) const {
        out << "{\n";
        bool first = true;
        for (const auto& [key, node] : dict) {
            if (!first) out << ",\n";
            first = false;
            out << "\"" << EscapeString(key) << "\": ";
            PrintNode(node, out);
        }
        out << "\n}";
    }
};

void PrintNode(const Node& node, std::ostream& out) {
    std::visit(PrintVisitor{out}, node.GetValue());
}


void Print(const Document& doc, std::ostream& output) {
    (void) &doc;
    (void) &output;
    PrintNode(doc.GetRoot(), output);
}

}  // namespace json