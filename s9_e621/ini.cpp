//
// Created by Kirill "Raaveinm" on 1/23/26.
//

#include "ini.h"

ini::Section & ini::Document::AddSection(std::string name) {
    return sections_[std::move(name)];
}

const ini::Section& ini::Document::GetSection(const std::string& name) const {
    auto it = sections_.find(name);
    if (it != sections_.end()) {
        return it->second;
    }
    static const ini::Section empty;
    return empty;
}

std::size_t ini::Document::GetSectionCount() const {
    return sections_.size();
}

ini::Document ini::Load(std::istream& input) {
    ini::Document doc;
    ini::Section* current_section = nullptr;
    std::string line;

    while (std::getline(input, line)) {
        const size_t start_pos = line.find_first_not_of(" \t");

        if (start_pos == std::string::npos)
            continue;

        if (line[start_pos] == '[') {
            if (const size_t end_pos = line.find(']', start_pos); end_pos != std::string::npos) {
                std::string section_name = line.substr(start_pos + 1, end_pos - start_pos - 1);
                current_section = &doc.AddSection(std::move(section_name));
            }
        }
        else {
            if (const size_t eq_pos = line.find('=', start_pos); eq_pos != std::string::npos) {
                std::string key = line.substr(start_pos, eq_pos - start_pos);
                if (const size_t key_end = key.find_last_not_of(" \t"); key_end != std::string::npos) {
                    key = key.substr(0, key_end + 1);
                } else {
                    key.clear();
                }
                std::string value = line.substr(eq_pos + 1);
                const size_t val_start = value.find_first_not_of(" \t");
                if (val_start != std::string::npos) {
                    value = value.substr(val_start);
                    const size_t val_end = value.find_last_not_of(" \t");
                    value = value.substr(0, val_end + 1);
                } else {
                    value.clear();
                }

                if (current_section && !key.empty())
                    (*current_section)[key] = value;
            }
        }
    }
    return doc;
}
