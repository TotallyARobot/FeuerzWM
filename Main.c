#include<X11/Xlib.h>
#include<X11/Xutil.h>
#include<stdlib.h> 
#include<stdio.h>
#include<string.h>
#include<unistd.h>

XWindowAttributes attr;
XButtonEvent start;
XEvent event;
FILE *fp;
#define MAX(a, b) ((a) > (b) ? (a) : (b))

int main(int argc,char *argv[]){
    char args[255];
    strcpy(args,argv[1]);
    for(int i = 2; i < argc; i++){
        strcat(args," ");
        strcat(args,argv[i]);
    }
    Display *dpy = XOpenDisplay(0);
    Window root = RootWindow(dpy,DefaultScreen(dpy));
    KeyCode exit = XKeysymToKeycode(dpy, XStringToKeysym("b"));
    fp = popen(args,"r");


    KeyCode spawnterm = XKeysymToKeycode(dpy, XK_Return);
    KeyCode closekey = XKeysymToKeycode(dpy, XStringToKeysym("I"));
    KeyCode progstart = XKeysymToKeycode(dpy, XStringToKeysym("h"));
    KeyCode fullscreen = XKeysymToKeycode(dpy, XStringToKeysym("f"));
    
    //XSelectInput(dpy,root,KeyPressMask);
    XGrabKey(dpy, NULL, Mod4Mask, root, True, GrabModeAsync, GrabModeAsync);
    //XGrabKey(dpy, spawnterm,Mod4Mask,root(dpy),True,GrabModeAsync,GrabModeAsync);
    //XGrabKey(dpy, closekey,Mod4Mask,root(dpy),True,GrabModeAsync,GrabModeAsync);
    //XGrabKey(dpy, progstart,Mod4Mask,root(dpy),True,GrabModeAsync,GrabModeAsync);
    //XGrabKey(dpy, fullscreen,Mod4Mask,root(dpy),True,GrabModeAsync,GrabModeAsync);
    XGrabButton(dpy, 1, Mod4Mask, root, True,ButtonPressMask|ButtonReleaseMask|PointerMotionMask, GrabModeAsync, GrabModeAsync, None, None);
    XGrabButton(dpy, 3, Mod4Mask, root, True,ButtonPressMask|ButtonReleaseMask|PointerMotionMask, GrabModeAsync, GrabModeAsync, None, None);
    for(;;){
        if(event.type == KeyPress && event.xkey.keycode == exit)
            break;
        else if(event.type == KeyPress && event.xkey.keycode == spawnterm)
            fp = popen("st","r");
        else if(event.type == KeyPress && event.xkey.keycode == closekey)
            XKillClient(dpy,event.xbutton.subwindow);
        else if(event.type == KeyPress && event.xkey.keycode == progstart)
            fp = popen("dmenu_run","r");
        else if(event.type == KeyPress && event.xkey.keycode == fullscreen)
            XMoveResizeWindow(dpy,event.xbutton.subwindow,0,0,XDisplayWidth(dpy, 0),XDisplayHeight(dpy,0));
        else if(event.type == ButtonPress && event.xbutton.subwindow != None)
        {
            XRaiseWindow(dpy,event.xbutton.subwindow);
            XGetWindowAttributes(dpy, event.xbutton.subwindow, &attr);
            start = event.xbutton;
            XSetInputFocus(dpy,event.xbutton.subwindow,RevertToParent,CurrentTime);
        }
        else if(event.type == MotionNotify && start.subwindow != None)
        {
            int xdiff = event.xbutton.x_root - start.x_root;
            int ydiff = event.xbutton.y_root - start.y_root;
            XMoveResizeWindow(dpy, start.subwindow,
                attr.x + (start.button==1 ? xdiff : 0),
                attr.y + (start.button==1 ? ydiff : 0),
                MAX(1, attr.width + (start.button==3 ? xdiff : 0)),
                MAX(1, attr.height + (start.button==3 ? ydiff : 0)));
        }
        else if(event.type == ButtonRelease)
            start.subwindow = None;
        XNextEvent(dpy,&event);
    }
}
