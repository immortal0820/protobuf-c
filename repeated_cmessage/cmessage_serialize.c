#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "../proto/cmessage.pb-c.h"

int main (int argc, const char *argv[])
{
	CMessage msg = CMESSAGE__INIT;
	void *buf;
	unsigned len, i;

	msg.n_a = argc -1;
	msg.a = malloc(sizeof(int) * msg.n_a);
	if (msg.a == NULL)
		exit(1);

	for (i = 0; i < msg.n_a; i++) {
		msg.a[i] = atoi(argv[i+1]);
	}

	len = cmessage__get_packed_size(&msg);
	buf = malloc(len);
	if (buf == NULL) 
		exit(1);

	cmessage__pack(&msg, buf);

	fprintf(stderr, "Writing %d serialized bytes\n", len);
	fwrite(buf, len, 1, stdout);

	free(msg.a);
	free(buf);
	
	return 0;
}
