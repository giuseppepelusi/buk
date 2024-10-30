#include <stdio.h>		// For printf, fprintf, perror
#include "help.h"
#include "../utils/globals.h"

void help()
{
	printf("usage: %s <command> [<args>]\n\n", NAME);
	printf("These are the available %s commands:\n", NAME);
	printf("  help:\t\tShow the help page\n");
	printf("  init:\t\tInitialize the current directory\n");
	printf("  config:\tConfigure the path of the backup directory\n");
	printf("  save:\t\tSave the passed files or directories to the backup directory\n");
	printf("  delete:\tDelete the passed files or directories from the backup directory\n");
	printf("  restore:\tRestore the passed files or directories from the backup\n\t\tdirectory to the local directory\n");
}

void help_init()
{
	printf("%s: malformed expression\n\n", NAME);
	printf("usage: %s init\n", NAME);
	printf("Use \"%s help\" for additional information\n", NAME);
}

void help_config()
{
	printf("%s: malformed expression\n\n", NAME);
	printf("usage: %s config <path>\n", NAME);
	printf("Use \"%s help\" for additional information\n", NAME);
}

void help_save()
{
	printf("%s: malformed expression\n\n", NAME);
	printf("usage: %s save <file|dir> [<file|dir>...]\n", NAME);
	printf("Use \"%s help\" for additional information\n", NAME);
}

void help_delete()
{
	printf("%s: malformed expression\n\n", NAME);
	printf("usage: %s delete <file|dir> [<file|dir>...]\n", NAME);
	printf("Use \"%s help\" for additional information\n", NAME);
}

void help_restore()
{
	printf("%s: malformed expression\n\n", NAME);
	printf("usage: %s restore <file|dir> [<file|dir>...]\n", NAME);
	printf("Use \"%s help\" for additional information\n", NAME);
}
