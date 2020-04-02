#include <stdio.h>
#include <iostream>
using namespace std;

void address(int *d)
{
    cout << "Address X: " << d << " Value X: " << *d << endl;
}

void address2(int d)
{
    cout << "Address X: " << &d << " Value X: " << d;
}

int main() {

int *p;
int x = 25;
p = &x;

address(p);
address2(x);

return 0;
}

