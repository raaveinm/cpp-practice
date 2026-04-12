#include <vector>


#define INIT_SETTER(Name, Type, Val) \
ParserParam& Name(const Type Val) { \
        this->Val = Val; \
        return *this; \
    }

using namespace std::literals;


// dummies
enum class Language { DE, FR, IT };
struct City {
    std::string name;
    std::string iso_code;
    std::string phone_code;
    std::string country_name;
    std::string country_iso_code;
    std::string country_time_zone;
    std::vector<Language> languages;
};
struct Country {
    std::string name;
    std::string iso_code;
    std::string phone_code;
    std::string time_zone;
    std::vector<Language> languages;
};

struct ParserParam {
    const std::vector<City> *cities;
    const Json* json;
    const std::string* country_name;
    const std::string* country_iso_code;
    const std::string* country_phone_code;
    const std::string* country_time_zone;
    const std::vector<Language>* languages;

    INIT_SETTER(SetCities, std::vector<City>*, cities)
    INIT_SETTER(SetJson, Json*, json)
    INIT_SETTER(SetCountryName, std::string*, country_name)
    INIT_SETTER(SetCountryIsoCode, std::string*, country_iso_code)
    INIT_SETTER(SetCountryPhoneCode, std::string*, country_phone_code)
    INIT_SETTER(SetCountryTimeZone, std::string*, country_time_zone)
    INIT_SETTER(SetLanguages, std::vector<Language>*, languages)
};

// Дана функция ParseCitySubjson, обрабатывающая JSON-объект со списком городов конкретной страны:
void ParseCitySubjson(ParserParam& param) {
    const auto& [cities, json, country_name, country_iso_code, country_phone_code, country_time_zone, languages] = param;
    for (const auto& city_json : json.AsList()) {
        const auto& city_obj = city_json.AsObject();
        cities.push_back({city_obj["name"s].AsString(), city_obj["iso_code"s].AsString(),
                          country_phone_code + city_obj["phone_code"s].AsString(), country_name, country_iso_code,
                          country_time_zone, languages});
    }
}

// ParseCitySubjson вызывается только из функции ParseCountryJson следующим образом:
void ParseCountryJson(std::vector<Country>& countries, std::vector<City>& cities, const Json& json) {
    for (const auto& country_json : json.AsList()) {
        const auto& country_obj = country_json.AsObject();
        countries.push_back({
            country_obj["name"s].AsString(),
            country_obj["iso_code"s].AsString(),
            country_obj["phone_code"s].AsString(),
            country_obj["time_zone"s].AsString(),
        });
        Country& country = countries.back();
        for (const auto& lang_obj : country_obj["languages"s].AsList()) {
            country.languages.push_back(FromString<Language>(lang_obj.AsString()));
        }
        ParseCitySubjson({
            .cities = &cities,
            .json = &country_obj["cities"s],
            .country_name = &country.name,
            .country_iso_code = &country.iso_code,
            .country_phone_code = &country.phone_code,
            .country_time_zone = &country.time_zone,
            .languages = &country.languages
        });
    }
}