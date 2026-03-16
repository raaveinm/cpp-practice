#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <vector>

struct Document {
    int id = 0;
    double relevance = 0.0;
    int rating = 0;
};

// Сортирует документы по релевантности и рейтингу
void SortDocuments(std::vector<Document>& docs) {
    std::sort(docs.begin(), docs.end(),
        [](const Document& lhs, const Document& rhs){
            if (abs(lhs.relevance - rhs.relevance) < 1e-6) {
                // релевантности примерно равны, упорядочиваем документы по рейтингу
                return lhs.rating > rhs.rating;
            } else {
                return lhs.relevance > rhs.relevance;
            }
        });
}

int main() {
    std::vector<Document> docs{ {1, 0.3, 3}, {2, 0.2, 5} };
    SortDocuments(docs);
    for (const auto& doc : docs) {
        std::cout << doc.id << ',';
    }
}