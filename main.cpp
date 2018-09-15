
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
    animor.b_running = true;
    animor.time_delta_next = 0;
    animor.rgb_animation = 0;

    RGBFadeTask* fdt1 = new RGBFadeTask(color_t(0,255,0), color_t(255,0,100),200,2,true);
    RGBFlashTask* flt1 = new RGBFlashTask(color_t(0,255,0), color_t(255,0,100),200,50,2,true);
    print_col(fdt1->color_from);
    print_col(fdt1->color_to);
    animor.queue_task(flt1); 
    //animor.queue_task(fdt1);  
    animor.animate(20);
    paint_col(animor.color_current()); 
    //printf("%u", animor.rgb_animation);
    //animor.rgb_animation = animor.pop_task_virt()->GetAnimation();   
    //animor.RGBTaskList.push_back(flt1);  
    //RGBTask* task = *animor.RGBTaskList.begin();
    //RGBAnimation* anim = task->GetAnimation();
    while(1)
    {
        animor.animate(20);
        paint_col(animor.color_current());    
        printf("%u", animor.RGBTaskList.size());
        //printf("%f, %u", anim->fac_progress_, anim->num_rep_progress);
        usleep(20000);
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
