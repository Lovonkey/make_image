#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/vfs.h>
#include <errno.h>

int main(int argc, char *argv[])
{
        
        int fdi = 0;
        int fdo = 0;
        int len = 1;
        int diff_pix = 10;
        unsigned char buf[4] = {0};
        char diff = 0;
        int i;

	if (argc < 3) {
		printf("mkdim filename diff_pix\n");
		return -1;
	}

        diff_pix = atoi(argv[2]);

        srand(time(NULL));

        fdi = open(argv[1], O_RDONLY);
        if (fdi < 0) {
                printf("Open %s failed\n", argv[1]);
                return -1;
        }
        
        fdo = open("out.rgb", O_RDWR | O_CREAT, 0777);
        if (fdo < 0) {
                printf("Open out.rgb failed\n");
                return -1;
        }

        while (len) {
                len = read(fdi, buf, sizeof(buf));
                if (len) {
                        for (i = 0; i < 3; i++) {
                                diff = rand() % diff_pix;
                                if (buf[i]) {
                                        buf[i] -= diff;
                                } else {
                                        buf[i] += diff;
                                }
                        }

                        write(fdo, buf, sizeof(buf));
                }
        }

        close(fdi);
        close(fdo);

	return 0;
}
