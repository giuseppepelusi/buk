#include <stdio.h>		// For printf, fprintf, perror
#include "help.h"
#include "../utils/globals.h"

void buk_help()
{
	printf("usage: %s [-v | --version] [-h | --help] <command> [<args>]\n", NAME);
	printf("\n");
	printf("These are the available %s commands:\n", NAME);
	printf("\n");
	printf("   init        Initialize backup tracking in the current directory\n");
	printf("   config      Set the backup directory path\n");
	printf("   save        Save files or directories to the backup\n");
	printf("   delete      Remove files or directories from the backup\n");
	printf("   restore     Restore files or directories from the backup\n");
	printf("\n");
	printf("Use \'man buk\' for a complete guide and examples.\n");
}

void buk_usage(const char *cmd, const char *args)
{
    fprintf(stderr, "error: invalid arguments\n");
    fprintf(stderr, "\n");
    fprintf(stderr, "usage: %s %s %s\n", NAME, cmd, args ? args : "");
    fprintf(stderr, "Use '%s --help' for more information\n", NAME);
}
