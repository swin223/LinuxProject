#include <iostream>
#include <string>
#include <vector>

using namespace std;

void printMsg(int a)
{
    cout << "a = ";
    while(a--){
        cout << " " << a;
    }
    cout << endl;
}

void printMsg(string str1,string str2)
{
    string tempStr1 = str1;
    string tempStr2 = str2;
    cout << tempStr1 << endl;
    cout << tempStr2 << endl;
}

int printNum(int a)
{
    if(a > 50) return 500;
    else return 0;
}

int main()
{
    cout << "----" << endl;
    printMsg(10);
    cout << "----" << endl;
    string aaa(10,'a'),bbb(10,'b');
    printMsg(aaa,bbb);
    cout << "----" << endl;
    cout << "big = " << printNum(100) << " " << "little = " << printNum(30) << endl;
    cout << "----" << endl;
    int a = 50;
    cout << a << endl;

    return 0;
}

