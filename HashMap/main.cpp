#include "hashmap.h"

using namespace std;
#include "test_settings.h"
#include <map>


void test();

int student_main() {
    cout << "This is student main. You can try using HashMap as a client" << endl;
    cout << "and call map.debug() to see the internal state of the hash table." << endl;
    cout << "This is a good way to debug if you like a good visual of the linked lists." << endl;
    cout << endl;
    cout << "To turn on the test harness, go to test_settings.cpp and change RUN_TEST_HARNESS to 1." << endl;
    cout << endl << endl;
    test();
    return 0;
}

template <typename... Ts>
void ensure_at_return_is_const(Ts...) {
    cout << "True";
};

template <typename T>
auto ensure_at_return_is_const(const T& map) -> decltype((void) (map.at(0) = 3), void()) {
    // ASSERT_TRUE(false);
    cout << "false";
    throw domain_error("Error.");

    // If you run into an error here, this means your return value of const
    // at is a non-const reference. Why is that wrong?

    // Details about this error (it's a hacky sfinae technique):
    // we tried running the following code segment (in the decltype line)
    //      const T& map;
    //      map.at(0) = 3;
    // if the segment compiles, then this function is run (which fails)
    // if the segment does not compile, the variadic template before runs (which passes)
}

void test() {
    HashMap<string, int> map(5);
    cout << "Hello from your past and current lecturers!" << endl;
    auto [anna_iter, anna_found] = map.insert({"Anna", 2});
    map.insert({"Avery", 3});
    map.insert({"Nikhil", 4});
    map.insert({"Ethan", 5});
    map.debug();
    map.rehash(2);
    map.debug();

    if (anna_found) map.erase(anna_iter);
    auto ethan_iter = map.find("Ethan");
    ethan_iter->second = 100;
    map.debug();

    // auto iter = ethan_iter;
    // ++iter;
    // iter->second = 200;
    // map.debug();

    HashMap<int, int> map3;
    // std::map<int, int> map3;
    map3.insert({0, 0});
    map3.insert({1, 1});
    ensure_at_return_is_const(map3); // confirm return value is a const reference
}


int main() {
    std::cout << "RUNNING STUDENT MAIN" << std::endl;
    student_main();
    std::cout << "SUCCESSFULLY COMPLETED STUDENT MAIN" << std::endl;
    
    return 0;
}
