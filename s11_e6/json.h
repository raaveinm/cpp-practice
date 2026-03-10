#pragma once

#include <cstddef>
#include <iostream>
#include <map>
#include <string>
#include <variant>
#include <vector>

namespace json {

class Node;
using Dict = std::map<std::string, Node>;
using Array = std::vector<Node>;

class ParsingError : public std::runtime_error {
public:
    using runtime_error::runtime_error;
};

class Node {
public:
    Node() : value_(nullptr) {}
    Node(Array array);
    Node(Dict map);
    Node(int value);
    Node(std::string value);
    Node(std::nullptr_t value);
    Node(bool value);
    Node(double value);

    bool operator==(const Node &) const;
    bool operator!=(const Node &rhs) const;

    const Array& AsArray() const;
    const Dict& AsMap() const;
    int AsInt() const;
    const std::string& AsString() const;
    bool AsBool() const;
    double AsDouble() const;
    double AsPureDouble() const;

    bool IsArray() const;
    bool IsMap() const;
    bool IsInt() const;
    bool IsString() const;
    bool IsNull() const;
    bool IsBool() const;
    bool IsDouble() const;
    bool IsPureDouble() const;

    const std::variant<Array, Dict, int, std::string, std::nullptr_t, bool, double>& GetValue() const {
        return value_;
    }

private:
    std::variant<Array, Dict, int, std::string, std::nullptr_t, bool, double> value_;
};

class Document {
public:
    explicit Document(Node root);

    const Node& GetRoot() const;

    bool operator==(const Document& rhs) const;
    bool operator!=(const Document& rhs) const;

private:
    Node root_;
};

Document Load(std::istream& input);

void Print(const Document& doc, std::ostream& output);

}  // namespace json