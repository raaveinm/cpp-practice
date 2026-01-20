//
// Created by raaveinm on 1/20/26.
//
#include <iostream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <unordered_set>

using namespace std;

template <typename Hash>
int FindCollisions(const Hash& hasher, istream& text) {
    int collisions = 0;
    unordered_set<string> seen_words;
    unordered_set<size_t> seen_hashes;
    string word;

    while (text >> word) {
        if (seen_words.insert(word).second) {
            size_t h = hasher(word);
            if (!seen_hashes.insert(h).second)
                ++collisions;
        }
    }
    return collisions;
}

struct HasherDummy {
    size_t operator() (const string& str) const {
        size_t res = 0;
        size_t pos = 1;
        for (const char c : str) {
            res += static_cast<size_t>(c) * pos;
            pos *= 37;
        }
        return res;
    }
};

struct DummyHash {
    size_t operator()(const string&) const {
        return 42;
    }
};

int main() {
    DummyHash dummy_hash;
    hash<string> good_hash;

    {
        istringstream stream("I love C++"s);
        cout << FindCollisions(dummy_hash, stream) << endl;
    }
    {
        istringstream stream("I love C++"s);
        cout << FindCollisions(good_hash, stream) << endl;
    }
}