#include <X11/Xlib.h>
#include <X11/Xos.h>
#include <X11/Xutil.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <pthread.h>
#include <memory.h>
#include <time.h>
#include <math.h>
#define WIDTH 800
#define HEIGHT 600
#define wComp 10
#define hComp 10
#define tTrue 1
#define fFalse 0

#include "lib.h"
typedef uint32_t Pixls;

void fill(Pixls *pxls, size_t width, size_t height, size_t stride,
          Pixls pixl)
{
    size_t x = 0;
    while (height-- > 0)
    {
        while (x < width)
        {
            pxls[x] = pixl;
            x++;
        }
        x = 0;
        pxls += stride;
    }
}
struct Stars {
    int zz[4096];
    int xx[4096];
    int yy[4096];
};
//void initt(int i){
//    srand(time(NULL));
//    zz[i] = ((float)RAND_MAX - 10)*20.0f;
//    xx[i]=rand()%800;
//    yy[i]=2*((float)RAND_MAX - 100)*20.0f;
//}


int main() {
    int count=0;
    Pixls pixl1[WIDTH * HEIGHT];
    struct Stars stars;
    //float y = 0;
    int wi=(WIDTH/wComp)-1,hi=(HEIGHT/hComp)-1;
    //printf("wi=%d, hi=%d\n",wi,hi);
    Display *xd= XOpenDisplay(NULL);
    if(xd==NULL){
        perror("ERROD:");
    }
    XWindowAttributes wa = {0};
    XImage *image = NULL;
    Window win = XCreateSimpleWindow(
            xd,
            XDefaultRootWindow(xd),
            0,0,800,600,
            0,0,
            0);
    XGetWindowAttributes(xd, win, &wa);

    fill(pixl1, WIDTH, HEIGHT, WIDTH, 0x000000);
    //fill(&pixl1[0*WIDTH+(0*wComp)], wComp, hComp, WIDTH, 0x005000);//LefUPANGLE
    //fill(&pixl1[(0)*WIDTH+(wi*wComp)], wComp, hComp, WIDTH, 0x005000);//RightUPANGLE
    //fill(&pixl1[(hi*hComp)*WIDTH+(wi*wComp)], wComp, hComp, WIDTH, 0x005000);//RightBottomANGLE
    //fill(&pixl1[(hi*hComp)*WIDTH+(0*wComp)], wComp, hComp, WIDTH, 0x005000);
    srand(time(NULL));

    image = XCreateImage(
            xd,
            wa.visual,
            wa.depth,
            ZPixmap,
            0,
            (char *)pixl1,
            WIDTH,
            HEIGHT,
            32,
            WIDTH * sizeof(Pixls));
    GC gc = XCreateGC(xd, win, 0, NULL);

    Atom wm_delete_window = XInternAtom(xd,"WM_DELETE",False);
    XSetWMProtocols(xd,win,&wm_delete_window,1);
    XSelectInput(xd,win,KeyPressMask|ExposureMask);
    XMapWindow(xd,win);
    XSync(xd,False);
    int quit =0;
    int ii=0,kk=0,iz=0;
    while(!quit){
        XEvent event={0};
        XNextEvent(xd,&event);
        switch(event.type){
            case Expose:{
//                ii=0,kk=0;
//                while (ii < 800) {
//                    while (kk < 600) {
//                        fill(&pixl1[(kk) * 800 + (ii)], 2, 2, WIDTH, 0x109000);
//                        //XPutImage(xd, win, gc, image, 0, 0, 0, 0, WIDTH, HEIGHT);
//                        kk++;
//                    }
//                    ii++;
//                }
                for(int i=0;i<4096;i++) {
                    count++;
                    if ((count % 1200) == 0) { for(int i=0;i<3000;i++) fill(&pixl1[(rand()%600)*800+(rand()%800)], 2, 2, WIDTH, 0x000000);}
                    else {
                        stars.zz[i] -= 100.0f;
                        //printf("%d %ld\n",stars.zz[i],stars.zz[i]);
                        //XPutImage(xd, win, gc, image, 0, 0, 0, 0, WIDTH, HEIGHT);
                        if (stars.zz[i]) {
                            stars.xx[i] = 2.0f*(rand()%(2*790)-790);
                            stars.yy[i] = 2.0f*(rand()%(2*590)-590);
                            stars.zz[i] = stars.zz[i];
                        }
                        int x1 = (int) (stars.xx[i]%(stars.zz[i])+1.01f)+WIDTH/2;
                        int y1 = (int) (stars.yy[i]%(stars.zz[i])+1.01f)+HEIGHT/2;
                        int x2 = (sinf(x1)+1.0f)*(sinf(x1)+1.0f)+WIDTH/2;
                        int y2 = (cosf(y1)+1.0f)*(cosf(y1)+1.0f)+HEIGHT/2;
                        //zz[i] = (x1%y1)+1.0f;
                        if (x1 < 0 || x1 > WIDTH || y1 < 0 || y1 > HEIGHT) {
                            stars.xx[i] = 2.0f*(rand()%(2*790)-790);
                            stars.yy[i] = 2.0f*(rand()%(2*590)-590);
                            stars.zz[i] = stars.zz[i];
                        } else {
                            //zz[i] -= 2.0;
                            //x1++;
                            //y1++;
                            if(stars.zz[i]%800==0)
                            stars.zz[i] = 0;
                            fill(&pixl1[(y1)*WIDTH + (x1)], 2, 2, WIDTH, 0x005000);
                            XPutImage(xd, win, gc, image, 0, 0, 0, 0, WIDTH, HEIGHT);
                            XFlush(xd);
//                            ii=0,kk=0,iz=0;
//                            while(iz<10) {
//                                while (ii < 790) {
//                                    while (kk < 590) {
//                                        fill(&pixl1[(kk) * 790 + (ii)], 2, 2, WIDTH, 0x000000);
//                                        XPutImage(xd, win, gc, image, 0, 0, 0, 0, WIDTH, HEIGHT);
//                                        kk++;
//                                    }
//                                    ii++;
//                                }
//                                iz++;
//                            }

                        }
                    }
                }
            }break;
            case KeyPress:
            {
                if (event.xkey.keycode == 9)
                {
                    perror("WM_DELETE_AND_SAVE_YOURSELF\n");
                    quit = 1;
                    break;
                }
                printf("They key has been pressed! %d\n", event.xkey.keycode);
            }break;
            case ClientMessage:{
                if((Atom)event.xclient.data.l[0]==wm_delete_window) {
                    perror("WM_DELETE\n");
                    quit=1;
                }
            }break;
        }


        memset(&event, 0, sizeof(event));
        event.type = Expose;
        event.xexpose.window = win;
        XSendEvent(xd,win,False,ExposureMask,&event);
        usleep(10);

    }
    XCloseDisplay(xd);
    return 0;
}
//int main(){
//    Pixls pixl1[800*600];
//    srand(time(NULL));
//    for(int i=0;i<4096;i++) {
//        xx[i] = 2.0 * (rand() / (RAND_MAX - 0.5)) * 20.0;
//        yy[i] = 2.0 * (rand() / (RAND_MAX - 0.5)) * 20.0;
//        zz[i] = (rand() / (RAND_MAX + 0.0001)) * 20.0;
//        int xx1=(int)(xx[i]/(zz[i]+0.001));
//        int yy1=(int)yy[i]/(zz[i]+0.001);
//        fill(&pixl1[abs((yy1)+(xx1))], 1, 1, WIDTH, 0x005000);
//        printf("%d %d %d\n", xx[i], yy[i],
//               zz[i]);
//    }
//    return 0;
//}
//xx[i]=2*(rand()/RAND_MAX-0.5)*20;
//yy[i]=2*(rand()/RAND_MAX-0.5)*20;
//zz[i] = (rand()/RAND_MAX+0.0001)*20;