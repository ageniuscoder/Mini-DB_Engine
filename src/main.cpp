#include "frontend/evaluationWrapper.cpp"
#include <iostream>
using namespace std;


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
    