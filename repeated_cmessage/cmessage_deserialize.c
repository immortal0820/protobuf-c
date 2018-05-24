#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "../proto/cmessage.pb-c.h"

#define MAX_MSG_SIZE 4096

static size_t 
read_buffer(unsigned max_length, uint8_t *out)
{
	size_t cur_len = 0;
	size_t nread;

	while ((nread = fread(out + cur_len, 1, max_length - cur_len, stdin)) != 0) {
		cur_len += nread;
		if (cur_len == max_length) {
			fprintf(stderr, "max message length exceeded\n");
			exit(1);
		}
	}

	return cur_len;
}

int main (int argc, const char *argv[])
{
	int i = 0;
	CMessage *msg;
	uint8_t buf[MAX_MSG_SIZE];

	size_t msg_len = read_buffer (MAX_MSG_SIZE, buf);

	msg = cmessage__unpack(NULL, msg_len, buf);
	if (msg == NULL) {
		fprintf(stderr, "error unpacking incoming message\n");
		exit(1);
	}

	for (i = 0; i < msg->n_a; i++) {
		if (i > 0) 
			printf(", ");

		printf("Receved: %d", msg->a[i]);
	}

	printf("\n");

	cmessage__free_unpacked(msg, NULL);

	return 0;
}
