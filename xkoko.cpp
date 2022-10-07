#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xos.h>
#include <stdlib.h>
#include <stdio.h>

Display *disp;
int screen;
Window win;
GC gc;
XSizeHints sizeHints = *XAllocSizeHints();

#define WIN_WIDTH 232
#define WIN_HEIGHT 242

void init_x() {
	unsigned long black, white;

	disp = XOpenDisplay(NULL);
	if (disp == NULL) {
	    fprintf(stderr, "Cannot open display\n");
	    exit(1);
	}
   	screen = DefaultScreen(disp);

   	win = XCreateSimpleWindow(disp, DefaultRootWindow(disp), 0, 0, WIN_WIDTH, WIN_HEIGHT, 5, BlackPixel(disp, screen), WhitePixel(disp, screen));

	XSetStandardProperties(disp, win, "xkoko", "xkoko", None, NULL, 0, NULL);
	sizeHints.flags = PMinSize | PMaxSize | PSize;
	sizeHints.min_width = WIN_WIDTH;
	sizeHints.min_height = WIN_HEIGHT;
	sizeHints.max_width = WIN_WIDTH;
	sizeHints.max_height = WIN_HEIGHT;
	XSetWMNormalHints(disp, win, &sizeHints);

	XSelectInput(disp, win, ExposureMask | ButtonPress | PointerMotionMask);

    gc = XCreateGC(disp, win, 0,0);        

	XClearWindow(disp, win);
	XMapRaised(disp, win);

	Pixmap bitmap;
	unsigned int bmpWidth, bmpHeight;
	XReadBitmapFile(disp, win, "koko.xbm", &bmpWidth, &bmpHeight, &bitmap, NULL, NULL);
	
	// Check for window close/button events
	Atom WM_DELETE_WINDOW = XInternAtom(disp, "WM_DELETE_WINDOW", False); 
	XSetWMProtocols(disp, win, &WM_DELETE_WINDOW, 1);
	XEvent evt;
	while (true) {
		XNextEvent(disp, &evt);
		if (evt.type == Expose) {
			// Draw button
			XSetForeground(disp, gc, BlackPixel(disp, screen));
			XSetBackground(disp, gc, WhitePixel(disp, screen));
			// draw Her
			XCopyPlane(disp, bitmap, win, gc, 0, 0, bmpWidth, bmpHeight, 0, 0, 1);
		}
		if ((evt.type == ClientMessage) && (static_cast<unsigned int>(evt.xclient.data.l[0]) == WM_DELETE_WINDOW)) {
	    		break;
	    }
	}
}

void close_x() {
	XFreeGC(disp, gc);
	XDestroyWindow(disp, win);
	XCloseDisplay(disp);
	exit(0);				
}

int main() {
	init_x();
	close_x();
	return 0;
}

