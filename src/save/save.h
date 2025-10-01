#ifndef SAVE_H
#define SAVE_H

int buk_save(int argc, char *argv[]);
static int process_path(const char *path, const char *temp_backup_dir, const char *project_root, int *valid_paths_found);

#endif // SAVE_H
