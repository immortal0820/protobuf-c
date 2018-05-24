#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "../proto/demessage.pb-c.h"

static int 
dmessage_serialize (int argc, const char *argv[])
{
	void *buf = NULL;
	unsigned len = 0;

	DMessage msg = DMESSAGE__INIT;
	Submessage sub1 = SUBMESSAGE__INIT;
	Submessage sub2 = SUBMESSAGE__INIT;

	if (argc != 2 && argc != 3) {
		fprintf(stderr, "Usage: dmessage a[b]\n");
		return -1;
	}

	sub1.value = atoi(argv[1]);
	msg.a = &sub1;

	if (argc == 3) {
		sub2.value = atoi(argv[2]);
		msg.b = &sub2;
	}

	len = dmessage__get_packed_size(&msg);
	buf = calloc(1, len);
	if (buf == NULL) {
		fprintf(stderr, "buf is null\n");
		return -1;
	}

	dmessage__pack(&msg, buf);

	fprintf(stderr, "Writing %d serialize bytes\n", len);
	fwrite(buf, 1, len, stdout);

	free(buf);
	return 0;
}

static int
emessage_serialize (int argc, const char *argv[])
{
	void *buf = NULL;
	unsigned i, len = 0;
	
	Submessage **subs;
	EMessage msg = EMESSAGE__INIT;

	subs = malloc(sizeof(Submessage *) * (argc - 1));

	for (i = 1; i < argc; i++) {
		subs[i-1] = calloc(1, sizeof(Submessage));
		submessage__init(subs[i-1]);
		subs[i-1]->value = atoi(argv[i]);
	}

	msg.n_a = argc - 1;
	msg.a = subs;

	len = emessage__get_packed_size(&msg);
	buf = calloc(1, len);
	if (buf == NULL) {
		fprintf(stderr, "buf is null\n");
		return -1;
	}

	emessage__pack(&msg, buf);

	fprintf(stderr, "Writing %d serialize bytes\n", len);
	fwrite(buf, len, 1, stdout);

	free(buf);
	for (i = 1; i < argc; i++) 
		free(subs[i-1]);
	free(subs);

	return 0;
}

int main (int argc, const char *argv[])
{
	int ret = 0;

#if 1
	ret = dmessage_serialize(argc, argv);
	if (ret < 0) {
		fprintf(stderr, "dmessage serialized error\n");
		return -1;
	}
#else
	ret = emessage_serialize(argc, argv);
	if (ret < 0) {
		fprintf(stderr, "emessage serialized error\n");
		return -1;
	}
#endif

	return 0;
}
