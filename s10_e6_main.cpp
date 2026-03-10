#include <cstdint>
#include <iostream>

using namespace std;

int main() {
    int64_t a;
    int64_t b;
    if (cin >> a >> b) {
        bool overflow = false;
        if (a > 0 && b > 0) {
            if (a > INT64_MAX - b) {
                overflow = true;
            }
        } else if (a < 0 && b < 0) {
            if (a < INT64_MIN - b) {
                overflow = true;
            }
        }

        if (overflow) {
            cout << "Overflow!" << endl;
        } else {
            cout << a + b << endl;
        }
    }
    return 0;
}