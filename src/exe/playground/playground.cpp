//
// Created by Alether on 12/6/2023.
//
#include<vector>
#include<iostream>

using namespace std;

class Object {
public:
    Object(int a) : a(a) {}

    Object(Object&) = delete;
    Object& operator= (Object&) noexcept = delete;

    Object(Object&&) = default;
    Object& operator= (Object&&) noexcept = default;

    int a;

    ~Object() {
        cout << "destructor is called for: " << a << endl;
    }
};

int main() {

    vector<Object> objects;
    objects.emplace_back(1);
    objects.emplace_back(2);
    objects.emplace_back(3);

    cout << "RETURN 0;" << endl;
    return 0;
}