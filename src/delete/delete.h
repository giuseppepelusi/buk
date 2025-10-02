#ifndef DELETE_H
#define DELETE_H

int buk_delete(int argc, char *argv[]);
static int delete_path_from_backup(const char *path, const char *temp_backup_dir, const char *project_root, int *valid_paths_found);

#endif // DELETE_H
