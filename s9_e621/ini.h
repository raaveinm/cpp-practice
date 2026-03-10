//
// Created by Kirill "Raaveinm" on 1/23/26.
//

#ifndef TMP_INI_H
#define TMP_INI_H


#include <iostream>
#include <map>
#include <string>
#include <unordered_map>

namespace ini {
using Section = std::unordered_map<std::string, std::string>;
    
class Document {
public:
    Section& AddSection(std::string name);
    [[nodiscard]] const Section& GetSection(const std::string& name) const;
    [[nodiscard]] std::size_t GetSectionCount() const;

private:
    std::unordered_map<std::string, Section> sections_;
};

Document Load(std::istream& input);
} // END OF INI NAMESPACE

#endif //TMP_INI_H