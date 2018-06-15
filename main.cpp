
#include <iostream>
#include "RGBAnimator.cpp"

using namespace std;
void paint(int r, int g, int b);
void paint_color( int c, int i);

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
    paint(200, 50, 100);
    getchar();
    return 0;

}
void paint(int r, int g, int b)
{
    cout<<"\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n";
    for(int i = 256-16;i>0; i-=16)
    {
        paint_color(r,i);
        paint_color(g,i);
        paint_color(b,i);
        cout<<"\n";
    }
    cout<<"---   ---   ---   \n R     G     B    ";
}
void paint_color( int c, int i)
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
