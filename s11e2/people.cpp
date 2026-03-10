#include "people.h"

#include <stdexcept>
#include <utility>

using namespace std;

Person::Person(std::string name, const int age, const Gender gender)
: name_(std::move(name)), age_(age), gender_(gender) {}

const std::string & Person::GetName() const { return name_; }
int Person::GetAge() const { return age_; }
Gender Person::GetGender() const { return gender_; }

Programmer::Programmer(const string& name, const int age, const Gender gender)
: Person(name, age, gender) {}

// const string& Programmer::GetName() const {
//     return Person::GetName();
// }
//
// int Programmer::GetAge() const {
//     // Заглушка, реализуйте метод самостоятельно
//     return 0;
// }
//
// Gender Programmer::GetGender() const {
//     // Заглушка, реализуйте метод самостоятельно
//     return Gender::MALE;
// }

void Programmer::AddProgrammingLanguage(ProgrammingLanguage language) { languages_.emplace(language); }
bool Programmer::CanProgram(const ProgrammingLanguage language) const {
    if (!languages_.empty()) {
        return languages_.contains(language);
    }
    return false;
}

Worker::Worker(const string& name, const int age, const Gender gender) : Person(name, age, gender) {}
//
// const string& Worker::GetName() const {
//     // Заглушка, реализуйте метод самостоятельно
//     throw std::logic_error("Not implemented"s);
// }
//
// int Worker::GetAge() const {
//     // Заглушка, реализуйте метод самостоятельно
//     return 0;
// }
//
// Gender Worker::GetGender() const {
//     return Gender::MALE;
// }

void Worker::AddSpeciality(WorkerSpeciality speciality) {
    specialities_.emplace(speciality);
}

bool Worker::HasSpeciality(const WorkerSpeciality speciality) const {
    if (!specialities_.empty()) {
        return specialities_.contains(speciality);
    }
    return false;
}