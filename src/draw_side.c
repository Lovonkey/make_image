#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/vfs.h>
#include <errno.h>


#define OUTPUTFILE	"side.rgb"

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
	printf("usage: mkimage X Y\n");
}

int mkmid(int cur, int total)
{
        int mid;

        if (cur >= total / 2)
                mid = total / 2 - 1;
        else
                mid = total / 2;
        
        return mid;

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
	struct rgba rgba;
        int mid = 0;
	char outfile[32] = {0};
	char cmd[32] = {0};
	
	if (argc < 3) {
		usage();
		return -1;
	}

        x = atoi(argv[1]);
	y = atoi(argv[2]);

	printf("x = %d y = %d\n",x ,y);

	sprintf(outfile, "./%s", OUTPUTFILE);
	if (!access(outfile, 0)) {
		sprintf(cmd, "rm ./%s", OUTPUTFILE);
		if (system(cmd)) {
			printf("exce %s failed\n", cmd);
		}
	}

	fd = open(outfile, O_RDWR | O_CREAT, 00777);
	if (fd < 0) {
		printf("Open %s failed\n", outfile);
		return -1;
	}

        for (i = 0; i < y; i++) {
                for (j = 0; j < x; j++) {
                        mid = mkmid(i, y);
                        if (abs(i - mid) >= mid) {
                                /* red */
                                mkrgba(&rgba, GREEN);                
                        } else {
                                mid = mkmid(j, x);
                                if (abs(j - mid) >= mid) {
                                        mkrgba(&rgba, GREEN);                
                                        /* red */
                                } else {
                                        mkrgba(&rgba, GREEN);                
                                        /* black */
                                }
                        }

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
