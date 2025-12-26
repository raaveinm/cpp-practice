#include <cstddef>
#include <iostream>
#include <list>
#include <string>

class Editor {
public:
    Editor() = default;
    // сдвинуть курсор влево
    void Left() {
        if (it_ != text_.begin()) {
            --it_;
        }
    }
    // сдвинуть курсор вправо
    void Right() {
        if (it_ != text_.end()) {
            ++it_;
        }
    }
    // вставить символ token
    void Insert(const char token) {
        text_.insert(it_, token);
    }
    // вырезать не более tokens символов, начиная с текущей позиции курсора
    void Cut(size_t tokens = 1) {
        buffer_.clear();
        for (size_t i = 0; i < tokens && it_ != text_.end(); ++i) {
            buffer_.push_back(*it_);
            it_ = text_.erase(it_);
        }
    }

    // cкопировать не более tokens символов, начиная с текущей позиции курсора
    void Copy(const size_t tokens = 1) {
        buffer_.clear();
        auto temp_it = it_;
        for (size_t i = 0; i < tokens && temp_it != text_.end(); ++i) {
            buffer_.push_back(*temp_it);
            ++temp_it;
        }
    }
    // вставить содержимое буфера в текущую позицию курсора
    void Paste() {
        text_.insert(it_, buffer_.begin(), buffer_.end());
    }
    // получить текущее содержимое текстового редактора
    [[nodiscard]] std::string GetText() const {
        return {text_.begin(), text_.end()};
    }

private:
    std::list<char> buffer_;
    std::list<char>::iterator buffer_it_ = buffer_.begin();
    std::list<char>::iterator buffer_end_it_ = buffer_.end();

    std::list<char> text_;
    std::list<char>::iterator it_ = text_.begin();
    std::list<char>::iterator end_it_ = text_.end();
};


int main() {
    Editor editor;
    const std::pmr::string text = "hello, world";
    for (const char c : text) { editor.Insert(c); }
    for (size_t i = 0; i < text.size(); ++i) { editor.Left(); }
    editor.Cut(7);
    for (size_t i = 0; i < 5; ++i) { editor.Right(); }
    editor.Insert(',');
    editor.Insert(' ');
    editor.Paste();
    editor.Left();
    editor.Left();
    editor.Cut(3);
    std::cout << editor.GetText();
    return 0;
}
