#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../proto/amessage.pb-c.h"

int main (int argc, const char *argv[])
{
	AMessage msg = AMESSAGE__INIT;
	void *buf = NULL;
	size_t size = 0;

	if (argc != 2 && argc != 3) {
		fprintf(stderr, "Usage: amessage a[b]\n");
		return -1;
	}

	msg.a = atoi(argv[1]);
	if (argc == 3) {
		msg.has_b = 1;
		msg.b = atoi(argv[2]);
	}

	size = amessage__get_packed_size(&msg);
	buf = malloc(size);
	if (buf == NULL) 
		return -1;

	amessage__pack(&msg, buf);

	fprintf(stderr, "Write %d serialized bytes\n", size);
	fwrite(buf, size, 1, stdout);

	free(buf);
	return 0;
}
