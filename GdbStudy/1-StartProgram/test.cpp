#include <iostream>
#include <string>

using namespace std;

void showMessage(int i)
{
    cout << i << endl;
}

int main()
{
    cout << "process id = 1" << endl;
    cout << "process id = 2" << endl;
    cout << "process id = 3" << endl;
    cout << "process id = 4" << endl;
    cout << "process id = 5" << endl;

    int i = 50;
    showMessage(i);

    return 0;
}

