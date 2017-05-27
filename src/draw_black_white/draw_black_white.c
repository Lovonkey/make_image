#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <errno.h>
#include <pthread.h>

enum colours {
        RED,
        BLUE,
        GREEN,
        BLACK,
        WHITE,
};

struct rgba {
	unsigned char r;
	unsigned char g;
	unsigned char b;
	unsigned char a;

};

void usage(void)
{
	printf("usage: mkimage X Y [interval pixel] [start colour 0/1]\n");
}

void mkrgba(struct rgba *rgba, enum colours colour)
{
        memset(rgba, 0, sizeof(*rgba));
        rgba->a = 0xFF;
        switch (colour) {
        case RED:
                rgba->r = 0xFF;
                break;
        case BLUE:
                rgba->b = 0xFF;
                break;
        case GREEN:
                rgba->g = 0xFF;
                break;
        case BLACK:
                break;
        case WHITE:
                rgba->r = 0xFF;
                rgba->g = 0xFF;
                rgba->b = 0xFF;
                break;
        default:
                break;
        }
}

int main(int argc, char *argv[])
{
	int x;
	int y;
	int i, j, fd;
	int len;
        int interval_pixel = 0;
        int draw_colour = WHITE;
        struct rgba rgba;
	char outfile[32] = {0};
	char cmd[32] = {0};       
	
	if (argc < 5) {
		usage();
		return -1;
	}

        x = atoi(argv[1]);
	y = atoi(argv[2]);
        interval_pixel = atoi(argv[3]);
        if (atoi(argv[4]) == 0)
                draw_colour = WHITE;
        else if (atoi(argv[4]) == 1)
                draw_colour = BLACK;

	printf("x = %d y = %d\n",x ,y);
    
	sprintf(outfile, "./black_white_%s_%d.rgb",
                draw_colour == WHITE ? "black" : "white",
                interval_pixel);

	if (!access(outfile, 0)) {
		sprintf(cmd, "rm ./%s", outfile);
		if (system(cmd)) {
			printf("exec %s failed\n", cmd);
		}
	}

	fd = open(outfile, O_RDWR | O_CREAT, 00777);
	if (fd < 0) {
		printf("Open %s failed\n", outfile);
		return -1;
	}   
        
	for (i = 0; i < y; i++) {
                if (!interval_pixel) {
                        draw_colour = BLACK;
                } else {
                        if ((i % (interval_pixel)) == 0) {
                                if (draw_colour == BLACK) {
                                        draw_colour = WHITE;
                                } else {
                                        draw_colour = BLACK;
                                }
                        } else {
                                /* nop */
                        }
                }

                for (j = 0; j < x; j++){
                        
                        mkrgba(&rgba, draw_colour);
                        
                        len = write(fd, &rgba, sizeof(rgba));
             		if (len != sizeof(rgba)) {
             			printf("Write failed\n");
             			return -1;
             		}
                }
	}

	close(fd);

	return 0;
} 

