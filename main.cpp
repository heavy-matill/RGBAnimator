
#include <iostream>
#include "RGBAnimator.cpp"

using namespace std;

int main()
{
    cout<<"Hello World\n";

    RGBAnimator animor;

    RGBFadeTask* fdt1 = new RGBFadeTask();
    RGBFlashTask* flt1 = new RGBFlashTask();
    animor.RGBTaskList.push_back(fdt1);  
    animor.RGBTaskList.push_back(flt1);  
    RGBTask* task = *animor.RGBTaskList.begin();
    task->GetAnimation()->Update();
    getchar();
    return 0;

}
