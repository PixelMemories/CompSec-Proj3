/* sample code to read an environment variable */

#include <stdio.h>
#include <stdlib.h>

int
main(int argc, char **argv) {
	char *h = getenv("HIDDEN");
	if (h)
		printf("HIDDEN=\"%s\"\n", h);
	else
		printf("HIDDEN not set\n");
}

