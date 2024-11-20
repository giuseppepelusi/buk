#ifndef SAVE_H
#define SAVE_H

int save(int argc, char *argv[]);
static int process_path(const char *path, const char *temp_backup_dir, const char *project_root, int *valid_paths_found);
static int create_backup_zip(const char *temp_backup_dir, const char *project_root);

#endif // SAVE_H
