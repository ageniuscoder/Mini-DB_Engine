#include <iostream>
#include "Utils/Exceptions.h"
using namespace std;
int main(){
    try{
        throw CustomException("Database connection failed");

    }catch(const CustomException &e){
        cerr<<"Caught CustomException: "<<e.what()<<endl;

    }
}