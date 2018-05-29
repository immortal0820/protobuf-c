#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "../proto/streamer.pb-c.h"

#define Length 20

int main ()
{
	size_t len;
	void *buf = NULL;

	char *username = "admin";
	char *password = "admin";
	char *id = "1d33d332d";
	char *ip = "192.168.1.1";

	printf("============pack===========\n");
	
	CloudServer cl = CLOUD_SERVER__INIT;
	AddStreamerRequest msg = ADD_STREAMER_REQUEST__INIT;

	msg.username = calloc(1, Length);
	strncpy(msg.username, username, Length);

	msg.password = calloc(1, Length);
	strncpy(msg.password, password, Length);

	cl.id = calloc(1, Length);
	strncpy(cl.id, id, Length);

	cl.ip = calloc(1, Length);
	strncpy(cl.ip, ip, Length);

	msg.server = &cl;

	len = add_streamer_request__get_packed_size(&msg);
	buf = calloc(1, len);

	add_streamer_request__pack(&msg, buf);

	free(msg.username);
	free(msg.password);
	free(cl.id);
	free(cl.ip);

	printf("==========upacked===========\n");
	
	CloudServer *cl_out = NULL;
	AddStreamerRequest *msg_out = NULL;

	msg_out = add_streamer_request__unpack(NULL, len, buf);
	
	cl_out = msg_out->server;

	printf("username: %s\n", msg_out->username);
	printf("password: %s\n", msg_out->password);
	printf("id: %s\n", cl_out->id);
	printf("ip: %s\n", cl_out->ip);
	
	add_streamer_request__free_unpacked(msg_out, NULL);

	free(buf);

	return 0;
}
