#include <iostream>

using namespace std;

int main()
{
    volatile int a=0;
    a++;
    a++;
    cout << "Hello World! " << "a=" << a <<endl;
    return 0;
}
