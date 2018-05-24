#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "../proto/amessage.pb-c.h"

#define MAX_MSG_SIZE 1024

static size_t
read_buffer (unsigned max_length, uint8_t *out)
{
	size_t nread;
	size_t cur_read = 0;

	while ((nread = fread(out + cur_read, 1, max_length - cur_read, stdin)) != 0) {
		cur_read += nread;
		if (cur_read == max_length) {
			fprintf(stderr, "max message length exceeded\n");
			exit(1);
		}
	}

	return cur_read;
}

int main (int argc, const char *argv[])
{
	AMessage *msg;
	uint8_t buf[MAX_MSG_SIZE];

	size_t msg_size = read_buffer (MAX_MSG_SIZE, buf);

	msg = amessage__unpack(NULL, msg_size, buf);
	if (msg == NULL) {
		fprintf(stderr, "error unpacking incoming message\n");
		return -1;
	}

	printf("Received: a = %d", msg->a);
	if (msg->has_b)
		printf(", b = %d", msg->b);

	printf("\n");

	amessage__free_unpacked(msg, NULL);

	return 0;
}
