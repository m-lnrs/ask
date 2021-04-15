#include <stdio.h>
#include <termios.h>

char getch() {
	char buffer = 0;
	struct termios config = {0};

	//get current terminal io settings
	if(tcgetattr(0, &config) < 0) {
		perror("tcgetattr()");
	}
	
	//change some terminal io settings
	config.c_lflag &= ~ICANON; //disable buff io
	config.c_lflag &= ~ECHO;   //set no echo mode
	config.c_cc[VTIME] = 0;
	config.c_cc[VMIN] = 1;
	
	//apply this terminal io settings now
	if(tcsetattr(0, TCSANOW, &config) < 0) {
		perror("tcsetattr ICANON");
	}
	
	buffer = getchar();
	
	//restore changes made on terminal io settings?
	config.c_lflag |= ICANON;
	config.c_lflag |= ECHO;
	
	if(tcsetattr(0, TCSADRAIN, &config) < 0) {
		perror("tcsetattr ~ICANON");
	}
	
	return buffer;
}

int main() {
	
	char *ptr = "Computador, você sabe me dizer";
	char answer[128];
	char c;

	int i = 0;
	
	do {
		c = getch();
		putchar(*(ptr++));
		if (c != ' ') {
			answer[i++] = c;
		}
	} while (c != ' ');

	while (getchar() != '\n');

	printf("\n%s\n", answer);

	return 0;
}
