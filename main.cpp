
#include <iostream>
#include "RGBAnimator.cpp"

using namespace std;
void paint_rgb(int r, int g, int b);
void paint_c( int c, int i);

int main()
{
    cout<<"Hello World\n";

    RGBAnimator animor;

    RGBFadeTask* fdt1 = new RGBFadeTask();
    RGBFlashTask* flt1 = new RGBFlashTask();
    animor.RGBTaskList.push_back(fdt1);  
    animor.RGBTaskList.push_back(flt1);  
    RGBTask* task = *animor.RGBTaskList.begin();
    //task->GetAnimation()->Update(10);
    paint_rgb(200, 50, 100);
    getchar();
    return 0;

}
void paint_rgb(int r, int g, int b)
{
    cout<<"\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n";
    for(int i = 256-16;i>0; i-=16)
    {
        paint_c(r,i);
        paint_c(g,i);
        paint_c(b,i);
        cout<<"\n";
    }
    cout<<"---   ---   ---   \n R     G     B    ";
}
void paint_c( int c, int i)
{
    if (c>i)
    {
        cout<<"###   ";
    }
    else
    {
        cout<<"      ";
    }
}
