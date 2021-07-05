#include <iostream>
#include <string>
#include <string.h>

using namespace std;

void showMessage(int i)
{
    cout << i << endl;
}

int main(int argc, char* argv[])
{
    cout << "extern message" << endl;
    printf("%s\n",argv[1]);
    printf("%s\n",argv[2]);

    cout << "process id = 1" << endl;
    cout << "process id = 2" << endl;
    cout << "process id = 3" << endl;
    cout << "process id = 4" << endl;
    cout << "process id = 5" << endl;

    int i = 50;
    showMessage(i);

    return 0;
}

