#pragma once

#include <string>
#include <utility>

class Person;

// Наблюдатель за состоянием человека.
class PersonObserver {
public:
    // Этот метод вызывается, когда меняется состояние удовлетворённости человека
    virtual void OnSatisfactionChanged(Person& /*person*/, int /*old_value*/, int /*new_value*/) {}

protected:
    // Класс PersonObserver не предназначен для удаления напрямую
    ~PersonObserver() = default;
};

/*
    Человек.
    При изменении уровня удовлетворённости уведомляет
    связанного с ним наблюдателя
*/
class Person : public PersonObserver {
public:
    virtual ~Person() = default;

    Person(std::string  name, const int age)
    : name_(std::move(name)), age_(age) {}

    [[nodiscard]] int GetSatisfaction() const { return satisfaction_; }
    [[nodiscard]] const std::string& GetName() const { return name_; }

    // «Привязывает» наблюдателя к человеку. Привязанный наблюдатель
    // уведомляется об изменении уровня удовлетворённости человека
    // Новый наблюдатель заменяет собой ранее привязанного
    // Если передать nullptr в качестве наблюдателя, это эквивалентно отсутствию наблюдателя
    void SetObserver(PersonObserver* observer) { observer_ = observer; }
    [[nodiscard]] int GetAge() const { return age_; }

    // Увеличивает на 1 количество походов на танцы
    // Увеличивает удовлетворённость на 1
    virtual  void Dance() {
        dance_count_++;
        ChangeSatisfaction(1);
    }

    [[nodiscard]] int GetDanceCount() const { return dance_count_; }

    virtual void LiveADay() {}

protected:
    void ChangeSatisfaction(const int v) {
        const int old_satisfaction = satisfaction_;
        satisfaction_ += v;
        if (observer_)
            observer_->OnSatisfactionChanged(*this, old_satisfaction, satisfaction_);
    }

    std::string name_;
    PersonObserver* observer_ = nullptr;
    int dance_count_ = 0;
    int satisfaction_ = 100;
    int age_;
};



// Рабочий.
// День рабочего проходит за работой
class Worker : public Person {
public:
    Worker(const std::string& name, const int age) : Person(name, age) {}

    // Рабочий старше 30 лет и младше 40 за танец получает 2 единицы удовлетворённости вместо 1
    // День рабочего проходит за работой
    // Увеличивает счётчик сделанной работы на 1, уменьшает удовлетворённость на 5
    void Work() {
        work_done_ += 1;
        ChangeSatisfaction(-5);
    }

    void LiveADay() override {
        Person::LiveADay();
        Work();
    }

    void Dance() override {
        dance_count_++;
        if (age_ > 30 && age_ < 40) {
            ChangeSatisfaction(2);
        } else {
            ChangeSatisfaction(1);
        }
    }

    // Возвращает значение счётчика сделанной работы
    [[nodiscard]] int GetWorkDone() const { return work_done_; }

private:
    int work_done_ = 0;
};

// Студент.
// День студента проходит за учёбой
class Student : public Person {
public:
    Student(const std::string& name, const int age) : Person(name, age) {
    }

    // День студента проходит за учёбой

    // Учёба увеличивает уровень знаний на 1, уменьшает уровень удовлетворённости на 3
    void Study() {
        knowledge_level_ += 1;
        ChangeSatisfaction(-3);
    }

    void LiveADay() override {
        Person::LiveADay();
        Study();
    }

    // Возвращает уровень знаний
    [[nodiscard]] int GetKnowledgeLevel() const {
        return knowledge_level_;
    }
private:
    int knowledge_level_ = 0;
};
