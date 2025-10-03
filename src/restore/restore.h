#ifndef RESTORE_H
#define RESTORE_H

int buk_restore(int argc, char *argv[]);
static int restore_path_from_backup(const char *path, const char *temp_backup_dir, const char *project_root, int *valid_paths_found);

#endif // RESTORE_H
