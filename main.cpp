#include "SymbolTable.h"
#include "SymbolTable.cpp"
using namespace std;

void test(string filename)
{
    SymbolTable* st = new SymbolTable();
    try
    {
        st->run(filename);
    }
    catch (exception& e)
    {
        cout << e.what();
    }
    delete st;
}

int main(int argc, char** argv)
{
    string address = "C://Users//Admin//Downloads//Compressed//initial//assignment1//testcase//test1.txt";
    test(address);
    return 0;
}