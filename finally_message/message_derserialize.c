#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "../proto/demessage.pb-c.h"

#define MAX_MSG_SIZE 4096

static int
read_buffer (int i, char c, uint8_t *buf)
{
	while (fread(&c, 1, 1, stdin) != 0) {
		if (i >= MAX_MSG_SIZE) {
			fprintf(stderr, "message too long for allocated buffer\n");
			exit(1);
		}
		buf[i++] = c;
	}

	return i;
}

static int 
dmessage_derserialize (int i, char c, uint8_t *buf)
{
	DMessage *msg;
	Submessage *sub1, *sub2;

	size_t len = read_buffer(i, c, buf);

	msg = dmessage__unpack(NULL, len, buf);
	if (msg == NULL) {
		fprintf(stderr, "Unpack dmessage error\n");
		return -1;
	}

	sub1 = msg->a;
	sub2 = msg->b;

	printf("Receved: a = %d", sub1->value);
	if (sub2 != NULL) 
		printf(", b = %d", sub2->value);
	printf("\n");

	dmessage__free_unpacked(msg, NULL);

	return 0;
}

static int
emessage_derserialize (int i, char c, uint8_t *buf)
{
	EMessage *msg;
	size_t len = 0;

	len = read_buffer(i, c, buf);

	msg = emessage__unpack(NULL, len, buf);
	if (msg == NULL) {
		fprintf(stderr, "Unpack emessage error\n");
		return -1;
	}

	for (i = 0; i < msg->n_a; i++) 
		printf("%d ", msg->a[i]->value);
	printf("\n");

	emessage__free_unpacked(msg, NULL);

	return 0;
}

int main (int argc, const char *argv[])
{
	int ret;

	char c;
	int i = 0;
	uint8_t buf[MAX_MSG_SIZE] = {0};
	
#if 1
	ret = dmessage_derserialize(i, c, buf);
	if (ret < 0) {
		fprintf(stderr, "dmessage derserialize error\n");
		return -1;
	}
#else 
	ret = emessage_derserialize(i, c, buf);
	if (ret < 0) {
		fprintf(stderr, "emessage derserialize error\n");
		return -1;
	}
#endif

	return 0;
}
