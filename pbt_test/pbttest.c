#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <fcntl.h>

//#define PROGRAM_USAGE_STRING "usage: %s [0-9]\n"

#define DOTM_MAGIC		0xBC

#define DOTM_SET_RUN		_IOW(DOTM_MAGIC, 9, int)	// JUMPEROID
#define DOTM_SET_OVER		_IOW(DOTM_MAGIC, 10, int)	// GAME OVER
#define DOTM_SET_CLEAR		_IOW(DOTM_MAGIC, 11, int)	// GAME CLEAR

unsigned int commands[] = {
	DOTM_SET_RUN,		// 0
	DOTM_SET_OVER,		// 1
	DOTM_SET_CLEAR,		// 2
};

// pushbutton API
int get_button(int fd, int *button_pushed) {
	unsigned char ret;
	unsigned char bytevalues[9];
	int i;
	int count = 0;

	// get all of the nine button status
	ret = read(fd, bytevalues, 9);
	if (ret < 0) {
		printf("Pushbutton read err\n");
		return -1;
	}

	for (i=0; i<9; i++) {
		// check buttons with value
		if (bytevalues[i]){
			count++;	// more than 2 will terminate user program
			*button_pushed = i;
		}
	}
	return count;
}

int main(int argc, char **argv)
{
	int pbt_fd, dotm_fd, num, i;
	int button_count = 0;
	unsigned int button_num = 0;

	dotm_fd = open("/dev/dotmatrix", O_RDWR); //O_WRONLY
	if (dotm_fd < 0) {
		fprintf(stderr, "error opening dotmatrix device\n");
		return -1;
	}

	pbt_fd = open("/dev/pushbutton", O_RDONLY);
	if (pbt_fd < 0) {
		fprintf(stderr, "error opening pushbutton device\n");
		return -1;
	}
	printf("All fds retrieved (dotm, pbt)\n");

	while (1) {
		button_count = get_button(pbt_fd, &button_num);
		printf("Pushed button count: %d ,Pushed button: %d\n", button_count, button_num);
		if (button_count == 1){
			//ioctl(dotm_fd, commands[button_num], NULL);
			ioctl(dotm_fd, commands[0], NULL);
			ioctl(dotm_fd, commands[1], NULL);
			ioctl(dotm_fd, commands[2], NULL);
			//usleep(500000);
			continue;
		} else {
			printf("Terminating the program: more than 1 button pushed!\n");
			usleep(500000);
			//break;
		}
	}

	close(pbt_fd);
	close(dotm_fd);
	return 0;
}
