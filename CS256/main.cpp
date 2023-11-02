#include <iostream>
#include <stdlib.h>
#include <vector>
#include <string>
#include <chrono>
#include <fstream>
#include <iomanip>
#include <ctime>
#include <regex> 
#include <sstream>
#include "./base/base.h"
#include "./function/function2.h" 
#include "./function/function3.h" 
#include "./function/function4.h" 
#include "./function/function5.h" 
#include "./function/function6.h" 
using namespace std;

int main(){
    int choose;
    cout <<"select 1 2 3 4 5 6 7" <<endl;
    bool valid = false;
    while(!valid){
        cin >> choose;
        if((choose >= 1) && (choose <= 7)){
            valid = true;
       
        switch(choose) {
            case 1:
                cout <<"1";
                break;
            case 2:
                function2();
                break;
            case 3:
                function3();
                break;
            case 4:
                function4();
                break;
            case 5:
                function5();
                break;
            case 6:
                function6();
                break;
            case 7:
                exit(0);
                cout << "Program closed!";
                break;

        }
        }    
        else 
            cout << "Invalid Input! Please input 1 to 7!" << endl;
    }
}