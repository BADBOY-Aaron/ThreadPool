#include<iostream>
#include "Executor.h"
using namespace utils::threading;
int main(){
    std::shared_ptr<Executor> executor = std::make_shared<Executor>();
    //Executor executor;
    auto f = executor->submit([=]{
        std::cout << "++++++++++++++" << std::endl;
         std::this_thread::sleep_for(std::chrono::milliseconds(5000));
    });
    f.wait();
    //std::cout << f.get() << std::endl;
    //executor->waitForSubmittedTasks();

    return 0;
}