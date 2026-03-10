#pragma once
#include <string>
#include <unordered_set>

enum class ProgrammingLanguage { CPP, JAVA, PYTHON, PHP };
enum class WorkerSpeciality { BLACKSMITH, CARPENTER, WOOD_CHOPPER, ENGINEER, PLUMBER };
enum class Gender { MALE, FEMALE };

class Person {
public:
    Person(std::string  name, int age, Gender gender);

    [[nodiscard]] const std::string& GetName() const;
    [[nodiscard]] int GetAge() const;
    [[nodiscard]] Gender GetGender() const;

private:
    std::string name_;
    int age_;
    Gender gender_;
};

// Программист. Знает несколько языков программирования
class Programmer : public Person {
public:
    Programmer(const std::string& name, int age, Gender gender);

    // Добавляет программисту знание языка программирования language
    // После вызова этого метода программист может программировать на этом языке
    // и на ранее добавленных
    void AddProgrammingLanguage(ProgrammingLanguage language);

    // Сообщает, может ли программист программировать на языке прогарммирования language
    // Изначально программист не владеет ни одним языком программирования
    [[nodiscard]] bool CanProgram(ProgrammingLanguage language) const;

private:
    std::unordered_set<ProgrammingLanguage> languages_;
};

// Рабочий. Владеет несколькими специальностями


class Worker : public Person {
public:
    Worker(const std::string& name, int age, Gender gender);

    // Добавляет рабочему способность работать по специальности speciality
    void AddSpeciality(WorkerSpeciality speciality);

    // Сообщает, может ли рабочий работать по указанной специальности.
    // Сразу после своего создания рабочий не владеет никакими специальностями
    [[nodiscard]] bool HasSpeciality(WorkerSpeciality speciality) const;

private:
    std::unordered_set<WorkerSpeciality> specialities_;
};
