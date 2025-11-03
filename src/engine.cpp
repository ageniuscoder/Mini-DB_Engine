#include <bits/stdc++.h>
using namespace std;

#include "execution_engine.h"
string InputBuffer;


void index_loader(execution_engine *);

int main(int argc, char **argv)
{

    system("title Database Management System");
    system("cls");
    EvaluationWrapper *main_io = new EvaluationWrapper();
    execution_engine *main_exec_engine = new execution_engine();


    thread index_loader_thread(index_loader, main_exec_engine);
    index_loader_thread.detach();

    while (true)
    {
        cout << BLUE << DB_PROMPT << DEFAULT;
        getline(cin, InputBuffer);
        AST_NODE *eval_node = main_io->handle(InputBuffer);
        if (eval_node == nullptr)
            continue;
        main_exec_engine->execute(eval_node);
    }

    return 0;
}

void index_loader(execution_engine *main_exec_engine)
{
    main_exec_engine->main_pager->load_index_files();
}
