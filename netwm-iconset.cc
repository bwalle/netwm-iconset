/* compile: fltk-config --use-images --compile  netwm-iconset.cc */
/* usage: ./netwm-iconset 0x3000006 icon.png */

#include <stdio.h>
#include <string.h>
#include <inttypes.h>
#include <stdlib.h>
#include <unistd.h>

#include <X11/Xlib.h>
#include <X11/Xatom.h>

#include <FL/Fl.H>
#include <FL/Fl_PNG_Image.H>


/* -------------------------------------------------------------------------- */
long *imageToNetIcon(const Fl_Image *image)
{
    int pixel_delta = image->d ();
    int line_delta = image->ld ();
    int w = image->w ();
    int h = image->h ();
    if (!line_delta) line_delta = pixel_delta * w;
    uint8_t *pixels = (uint8_t *) * image->data ();
    long *net_icon = new long[w * h + 2];
    uint8_t *p, *row;
    long argb = 0, *icon_pixel = &net_icon[2];
    int x, y;
    net_icon[0] = w;
    net_icon[1] = h;
    row = pixels;
    for (y = 0, row = pixels; y < h; y++, row += line_delta) {
        for (x = 0, p = row; x < w; x++, p += pixel_delta) {
            switch (abs (pixel_delta)) {
                case 1:
                    argb = ((long) p[0] << 16) | ((long) p[0] << 8) | (long) p[0]
                        | 0xff000000;
                    break;
                case 3:
                    argb = ((long) p[0] << 16) | ((long) p[1] << 8) | (long) p[2]
                        | 0xff000000;
                    break;
                case 4:
                    argb = ((long) p[0] << 16) | ((long) p[1] << 8) | (long) p[2]
                        | ((long) p[3] << 24);
                    break;
            }
            *icon_pixel++ = argb;
        }
    }

    return net_icon;
}

/* -------------------------------------------------------------------------- */
int main(int argc, char **argv) 
{
    int         err;
    Display     *display;
    Window      windowid;

    if (argc != 3) {
        fprintf(stderr, "Not enough arguments\n");
        return 1;
    }

    windowid = (Window)strtod(argv[1], NULL);

    if ( (display = XOpenDisplay(NULL)) == NULL )  {
        fprintf(stderr, "Could not open window\n");
        return 1;
    }

    Fl_PNG_Image *image = new Fl_PNG_Image(argv[2]);
    long *net_icon = imageToNetIcon(image);
    err = XChangeProperty (display, windowid,
            XInternAtom (display, "_NET_WM_ICON", 0), XA_CARDINAL, 32,
            PropModeReplace, (unsigned char *) net_icon,
            net_icon[0] * net_icon[1] + 2);
    XFlush(display);
    delete image;
    delete[] net_icon;


    return 0;
} 

/* vim: set ts=4 sw=4 et: */
