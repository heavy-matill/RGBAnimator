
#include <iostream>
#include "RGBAnimator.cpp"
#include <unistd.h>
#define PAINT_RESOLUTION 16

using namespace std;
void paint_rgb(int r, int g, int b);
void paint_c( int c, int i);
void print_col(color_t col);
void paint_col(color_t col);

int main()
{
    cout<<"Hello World\n";

    RGBAnimator animor;

    RGBFadeTask* fdt1 = new RGBFadeTask(color_t(0,255,0), color_t(255,0,100),1255,4,false);
    RGBFlashTask* flt1 = new RGBFlashTask(color_t(0,255,0), color_t(255,0,100),50,50,4,false);
    print_col(fdt1->color_from);
    print_col(fdt1->color_to);
    //animor.RGBTaskList.push_back(fdt1);  
    animor.RGBTaskList.push_back(flt1);  
    RGBTask* task = *animor.RGBTaskList.begin();
    RGBAnimation* anim = task->GetAnimation();
    while(anim->Update(20))
    {
        paint_col(anim->color_current);    
        usleep(200000);
    }
    //paint_rgb(200, 50, 100);
    getchar();
    return 0;

}
void print_col(color_t col)
{
    printf("R: %u G: %u B: %u\n", col.R, col.G, col.B);
}
void paint_col(color_t col)
{
    paint_rgb(col.R, col.G, col.B);
}
void paint_rgb(int r, int g, int b)
{
    cout<<"\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n";
    for(int i = 256-PAINT_RESOLUTION;i>0; i-=PAINT_RESOLUTION)
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
