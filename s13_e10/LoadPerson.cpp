#include <sstream>
#include <vector>

#define INIT_SETTER(Name, Type, Val) \
LoadPersonsParams& Name(const Type Val) { \
this->Val = Val; \
return *this; \
}

// dummies
struct DBLogLevel;
class DBConnector;
class DBHandler;
class DBQuery;

using namespace std::literals;

struct LoadPersonsParams { // In release, I would move this to separate file
    int db_connection_timeout;
    bool db_allow_exceptions;
    DBLogLevel db_log_level;
    int min_age;
    int max_age;
    std::string name_filter;

    INIT_SETTER(SetConnectionTimeOut, int, db_connection_timeout)
    INIT_SETTER(SetAllowExceptions, bool, db_allow_exceptions)
    INIT_SETTER(SetLogLevel, DBLogLevel, db_log_level)
    INIT_SETTER(SetMinAge, int, min_age)
    INIT_SETTER(SetMaxAge, int, max_age)
    INIT_SETTER(SetNameFilter, std::string, name_filter)

#undef INIT_SETTER
};


struct Person {
    std::string name;
    int age;
};

std::vector<Person> LoadPersons(std::string_view db_name, const LoadPersonsParams& params) {
    const auto& [db_connection_timeout, db_allow_exceptions, db_log_level, min_age, max_age, name_filter] = params;
    DBConnector connector(db_allow_exceptions, db_log_level);
    DBHandler db;
    if (db_name.starts_with("tmp."s)) {
        db = connector.ConnectTmp(db_name, db_connection_timeout);
    } else {
        db = connector.Connect(db_name, db_connection_timeout);
    }
    if (!db_allow_exceptions && !db.IsOK()) {
        return {};
    }

    std::ostringstream query_str;
    query_str << "from Persons "s
              << "select Name, Age "s
              << "where Age between "s << min_age << " and "s << max_age << " "s
              << "and Name like '%"s << db.Quote(name_filter) << "%'"s;
    DBQuery query(query_str.str());

    std::vector<Person> persons;
    for (auto [name, age] : db.LoadRows<std::string, int>(query)) {
        persons.push_back({std::move(name), age});
    }
    return persons;
}
