#include "frontend/evaluationWrapper.cpp"
#include <iostream>
using namespace std;

int main()
{
    string inputBuffer;  // may be we will make it a class of its own
    EvaluationWrapper* main_io = new EvaluationWrapper();
    while (true)
    {
        cout << "DB_Engine : "; 
        getline(cin, inputBuffer);
        main_io->handle(inputBuffer);

    }
}
 