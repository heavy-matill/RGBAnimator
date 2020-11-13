
#include <iostream>
#include "../RGBAnimator.cpp"
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

    animor.add_fade(color_t(255,0,0), color_t(0,255,255),20,10,3,true); // fade ist einer zu wenig flash einer zu viel
    animor.add_flash(color_t(0,255,0), color_t(255,0,100),10,10,4,true);
    animor.add_flash(color_t(0,0,255), color_t(0,0,50),10,10,2,false);
    animor.set_speed(64);
    animor.start();
    animor.process(2);
    //animor.rgb_animation = fdt1->GetAnimation();
    paint_col(animor.get_color_current()); 
    printf("%u", animor.task_list.size());
    //printf("%u", animor.rgb_animation);0,
    //animor.rgb_animation = animor.pop_task_virt()->GetAnimation();   
    //animor.RGBTaskList.push_back(flt1);  
    //RGBTask* task = *animor.RGBTaskList.begin();
    //RGBAnimation* anim = task->GetAnimation();
    while(1)
    {
        animor.process(1);
        paint_col(animor.get_color_current());    
        printf("%u", animor.task_list.size());
        //printf("%f, %u", anim->fac_progress_, anim->num_rep_progress);
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
