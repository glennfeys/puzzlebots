#ifdef RPI
#include <fcntl.h>
#include <linux/fb.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include "sense_led.h"

#define FILESIZE (64 * sizeof(uint16_t))

// function to convert a SPGM_RGBTRIPLE struct to a 16-bit representation of the RGB color
uint16_t convertColor(SPGM_RGBTRIPLE* color) {
    int red = (31 * color->rgbRed) / 255;
    int green = (61 * color->rgbGreen) / 255;
    int blue = (31 * color->rgbBlue) / 255;
    uint16_t converted_color = ((red << 11) | (green << 5) | blue);
    return converted_color;
}

void display_ledgrid(SPGM_RGBTRIPLE* ledgrid, const char* framebuffer) {

    int fb;
    uint16_t* p;
    uint16_t* map;
    struct fb_var_screeninfo screen_info;

    fb = open(framebuffer, O_RDWR);
    if (fb < 0) {
        printf("Could not open file.\n");
        exit(1);
    }

    if (ioctl(fb, FBIOGET_VSCREENINFO, &screen_info) < 0) {
        printf("Error in IOCTL call.\n");
        close(fb);
        exit(1);
    } else {
        map = mmap(NULL, FILESIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fb, 0);

        p = map;
        // clear the LED matrix
        memset(map, 0, FILESIZE);

        // goes over the ledgrid and lights the LED's
        // ledgrid is always a 8x8 matrix so its an array with 64 elements
        for (int i = 0; i < 64; i++) {
            *(p + i) = convertColor(&ledgrid[i]);
        }
        close(fb);
    }
}

void clear_leds(const char* framebuffer) {
    int fb;
    uint16_t* map;
    uint16_t* p;
    struct fb_var_screeninfo var_info;

    // open the frambuffer
    fb = open(framebuffer, O_RDWR);
    if (fb == -1) {
        printf("Could not open file.\n");
        exit(2);
    }

    if (ioctl(fb, FBIOGET_FSCREENINFO, &var_info) == -1) {
        printf("Error in IOCTL call.\n");
        close(fb);
        exit(2);
    }

    // map the framebuffer into memory
    map = mmap(NULL, FILESIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fb, 0);
    // clear the led matrix
    memset(map, 0, FILESIZE);
    // unmap and close the framebuffer
    if (munmap(map, FILESIZE) == -1) {
        printf("Error un-mmapping the file");
    }
    close(fb);
}
#endif
