#include "frontend/evaluationWrapper.cpp"
#include <iostream>
using namespace std;

#define FAIL "\e[0;31m"
#define SUCCESS "\e[0;32m"
#define DEFAULT "\e[0;37m"

int main()
{
    system("cls");
    string inputBuffer;
    EvaluationWrapper *main_io = new EvaluationWrapper();

    while (true)
    {
        cout << DEFAULT << "DB_Engine : ";
        getline(cin, inputBuffer);
        main_io->handle(inputBuffer);
    }
}
    