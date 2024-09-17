//
// Created by ran on 6/20/24.
// ran wurembrand
#include "copytree.h"

// Function to split the path into directory path and filename
void split_path(const char *original_path, char **new_path, char **filename) {
    char *last_slash;

    // Duplicate the original path to modify it
    *new_path = strdup(original_path);
    if (*new_path == NULL) {
        perror("Failed to allocate memory");
        exit(EXIT_FAILURE);
    }

    // Find the last '/' in the new path
    last_slash = strrchr(*new_path, '/');
    if (last_slash != NULL) {
        // Allocate memory and copy the filename
        *filename = strdup(last_slash + 1);  // +1 to skip the '/'
        if (*filename == NULL) {
            perror("Failed to allocate memory");
            free(*new_path);
            exit(EXIT_FAILURE);
        }

        // Terminate the new path at the last '/'
        *last_slash = '\0';
    } else {
        // If there's no '/', the whole path is the filename
        *filename = strdup(*new_path);
        if (*filename == NULL) {
            perror("Failed to allocate memory");
            free(*new_path);
            exit(EXIT_FAILURE);
        }

        // And now new_path will effectively be empty
        **new_path = '\0';
    }
}

// Function to create a directory if it doesn't exist
int ensure_dir_exists(const char *path) {
    struct stat st = {0};
    if (stat(path, &st) == -1) {
        if (mkdir(path, 0755) == -1) {
            perror("Failed to create directory");
            return -1;
        }
    }
    return 0;
}

// Recursive function to copy directory structure
//int copy_directory_structure(const char *src, const char *dest, int copy_symlinks, int copy_permissions) {
//    DIR *dir;
//    struct dirent *entry;
//
//    // Ensure the destination directory exists
//    if (ensure_dir_exists(dest) != 0) {
//        return -1;
//    }
//
//    // Open the source directory
//    dir = opendir(src);
//    if (dir == NULL) {
//        perror("Failed to open directory");
//        return -1;
//    }
//
//    while ((entry = readdir(dir)) != NULL) {
//        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
//            continue;  // Skip the '.' and '.' directories to prevent infinite recursion
//        }
//
//        char new_src_subpath[PATH_SIZE];
//        char new_dest_subpath[PATH_SIZE];
//
//        // Construct new subpaths for src and dest
//        snprintf(new_src_subpath, sizeof(new_src_subpath), "%s/%s", src, entry->d_name);
//        snprintf(new_dest_subpath, sizeof(new_dest_subpath), "%s/%s", dest, entry->d_name);
//
//        // Recursively copy directory structure if it's a directory
//        if (entry->d_type == DT_DIR) {
//            if (copy_directory_structure(new_src_subpath, new_dest_subpath,copy_symlinks,copy_permissions) != 0) {
//                closedir(dir);
//                return -1; // Stop if the copying fails
//            }
//        } else {
//            // Handle files and symbolic links
//            if (entry->d_type == DT_LNK && copy_symlinks) {
//                char link_target[PATH_SIZE];
//                ssize_t len = readlink(new_src_subpath, link_target, sizeof(link_target) - 1);
//                if (len == -1) {
//                    perror("Failed to read link");
//                    closedir(dir);
//                    return -1;
//                }
//                link_target[len] = '\0';  // Null-terminate the link target
//                if (symlink(link_target, new_dest_subpath) == -1) {
//                    perror("failed creating symlink");
//                    closedir(dir);
//                    return -1;
//                }
//            } else {
//                // Use the copy_file function to handle regular files
//                copy_file(new_src_subpath, new_dest_subpath, copy_symlinks, copy_permissions);
//            }
//        }
//    }
//
//    closedir(dir);
//    return 0;
//}

int copy_directory_structure(const char *src, const char *dest, int copy_symlinks, int copy_permissions) {
    DIR *dir;
    struct dirent *entry;

    // Ensure the destination directory exists
    if (ensure_dir_exists(dest) != 0) {
        return -1;
    }

    // Open the source directory
    dir = opendir(src);
    if (dir == NULL) {
        perror("Failed to open directory");
        return -1;
    }

    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
            continue;  // Skip the '.' and '..' directories to prevent infinite recursion
        }

        char new_src_subpath[PATH_SIZE];
        char new_dest_subpath[PATH_SIZE];

        // Construct new subpaths for src and dest
        snprintf(new_src_subpath, sizeof(new_src_subpath), "%s/%s", src, entry->d_name);
        snprintf(new_dest_subpath, sizeof(new_dest_subpath), "%s/%s", dest, entry->d_name);

        // Recursively copy directory structure if it's a directory
        if (entry->d_type == DT_DIR) {
            if (copy_directory_structure(new_src_subpath, new_dest_subpath, copy_symlinks, copy_permissions) != 0) {
                closedir(dir);
                return -1; // Stop if the copying fails
            }
        } else if (entry->d_type == DT_LNK) {
            if (copy_symlinks) {
                char link_target[PATH_SIZE];
                ssize_t len = readlink(new_src_subpath, link_target, sizeof(link_target) - 1);
                if (len == -1) {
                    perror("Failed to read link");
                    closedir(dir);
                    return -1;
                }
                link_target[len] = '\0';  // Null-terminate the link target
                if (symlink(link_target, new_dest_subpath) == -1) {
                    perror("Failed to create symlink");
                    closedir(dir);
                    return -1;
                }
            }
        } else {
            // Use the copy_file function to handle regular files
            copy_file(new_src_subpath, new_dest_subpath, copy_symlinks, copy_permissions);
        }
    }

    closedir(dir);
    return 0;
}



int helper_copy_directory(const char *src, const char *dest, int copy_symlinks, int copy_permissions) {
    DIR *dir = opendir(src);
    if (dir == NULL) {
        perror("Failed to open dir");
        return -1;
    }
    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) continue;

        char src_path[PATH_SIZE];
        char dest_path[PATH_SIZE];
        snprintf(src_path, PATH_SIZE, "%s/%s", src, entry->d_name);
        snprintf(dest_path, PATH_SIZE, "%s/%s", dest, entry->d_name);

        if (entry->d_type == DT_DIR) {
            if (ensure_dir_exists(dest_path) != 0 || helper_copy_directory(src_path, dest_path, copy_symlinks, copy_permissions) != 0) {
                closedir(dir);
                return -1;
            }
        } else if (entry->d_type == DT_LNK && copy_symlinks) {
            char link_target[PATH_SIZE];
            ssize_t len = readlink(src_path, link_target, sizeof(link_target) - 1);
            if (len == -1) {
                perror("Failed to read link");
                closedir(dir);
                return -1;
            }
            link_target[len] = '\0';  // Null-terminate the link target
            if (symlink(link_target, dest_path) == -1) {
                perror("failed creating symlink");
                closedir(dir);
                return -1;
            }
        } else if (entry->d_type == DT_REG) {
            int fdIn = open(src_path, O_RDONLY);
            if (fdIn < 0) {
                perror("Failed to open src");
                closedir(dir);
                return -1;
            }

            struct stat src_stat;
            if (stat(src_path, &src_stat) != 0) {
                perror("Failed to get file status");
                close(fdIn);
                closedir(dir);
                return -1;
            }

            mode_t file_mode = copy_permissions ? src_stat.st_mode : (S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);
            int fdOut = open(dest_path, O_WRONLY | O_CREAT, file_mode);

            if (fdOut < 0) {
                perror("Failed to open des file");
                close(fdIn);
                closedir(dir);
                return -1;
            }
            char buffer[4096];
            ssize_t bytesRead;
            while ((bytesRead = read(fdIn, buffer, sizeof(buffer))) > 0) {
                if (write(fdOut, buffer, bytesRead) != bytesRead) {
                    perror("Failed to writing to dest");
                    close(fdIn);
                    close(fdOut);
                    closedir(dir);
                    return -1;
                }
            }
            close(fdIn);
            close(fdOut);

        }
    }

    closedir(dir);
    return 0;
}


//void copy_directory(const char *src, const char *dest, int copy_symlinks, int copy_permissions) {
//    if (copy_directory_structure(src, dest,copy_symlinks,copy_permissions) != 0) {
//        fprintf(stderr, "Failed to copy directory structure.\n");
//        exit(EXIT_FAILURE);
//    }
//}

void copy_directory(const char *src, const char *dest, int copy_symlinks, int copy_permissions) {
    if (copy_directory_structure(src, dest, copy_symlinks, copy_permissions) != 0) {
        fprintf(stderr, "Failed to copy directory structure.\n");
        exit(EXIT_FAILURE);
    }
}



// Function to copy a file from source to destination, handling symbolic links and permissions
//void copy_file(const char *src, const char *dest, int copy_symlinks, int copy_permissions) {
//    struct stat src_stat;
//    if (stat(src, &src_stat) == -1) {
//        perror("stat failed");
//        return;
//    }
//
//    // Check if the source is a symbolic link
//    if (S_ISLNK(src_stat.st_mode) && copy_symlinks) {
//        char link_target[PATH_SIZE];
//        ssize_t len = readlink(src, link_target, sizeof(link_target) - 1);
//        if (len == -1) {
//            perror("readlink failed");
//            return;
//        }
//        link_target[len] = '\0'; // Null-terminate the link target
//
//        // Create the symbolic link in the destination
//        if (symlink(link_target, dest) == -1) {
//            perror("symlink failed");
//            return;
//        }
//    } else if (S_ISREG(src_stat.st_mode) || (!S_ISLNK(src_stat.st_mode) && !copy_symlinks)) {
//        // Open source file
//        int fdSrc = open(src, O_RDONLY);
//        if (fdSrc < 0) {
//            perror("open src failed");
//            return;
//        }
//
//        // Determine the mode to open the destination file
//        mode_t mode = copy_permissions ? src_stat.st_mode : (S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
//        int fdDest = open(dest, O_WRONLY | O_CREAT | O_TRUNC, mode);
//        if (fdDest < 0) {
//            perror("open dest failed");
//            close(fdSrc);
//            return;
//        }
//
//        // Copy data from source to destination
//        char buffer[BLOCK_SIZE];
//        ssize_t bytesRead;
//        while ((bytesRead = read(fdSrc, buffer, sizeof(buffer))) > 0) {
//            if (write(fdDest, buffer, bytesRead) != bytesRead) {
//                perror("write failed");
//                close(fdSrc);
//                close(fdDest);
//                return;
//            }
//        }
//
//        // Close file descriptors
//        close(fdSrc);
//        close(fdDest);
//    }
//}
//
//
//
//
// Function to copy a file from source to destination, handling symbolic links and permissions
void copy_file(const char *src, const char *dest, int copy_symlinks, int copy_permissions) {
    struct stat src_stat;
    if (stat(src, &src_stat) == -1) {
        perror("Stat failed");
        return;
    }

    // Check if the source is a symbolic link
    if (S_ISLNK(src_stat.st_mode) && copy_symlinks) {
        char link_target[PATH_SIZE];
        ssize_t len = readlink(src, link_target, sizeof(link_target) - 1);
        if (len == -1) {
            perror("Readlink failed");
            return;
        }
        link_target[len] = '\0'; // Null-terminate the link target

        // Create the symbolic link in the destination
        if (symlink(link_target, dest) == -1) {
            perror("Symlink failed");
            return;
        }
    } else if (!S_ISLNK(src_stat.st_mode) || (!copy_symlinks && S_ISREG(src_stat.st_mode))) {
        // Open source file
        int fdSrc = open(src, O_RDONLY);
        if (fdSrc < 0) {
            perror("Open src failed");
            return;
        }

        // Determine the mode to open the destination file
        mode_t mode = copy_permissions ? src_stat.st_mode : (S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
        int fdDest = open(dest, O_WRONLY | O_CREAT | O_TRUNC, mode);
        if (fdDest < 0) {
            perror("Open dest failed");
            close(fdSrc);
            return;
        }

        // Copy data from source to destination
        char buffer[BLOCK_SIZE];
        ssize_t bytesRead;
        while ((bytesRead = read(fdSrc, buffer, sizeof(buffer))) > 0) {
            if (write(fdDest, buffer, bytesRead) != bytesRead) {
                perror("Write failed");
                close(fdSrc);
                close(fdDest);
                return;
            }
        }

        // Close file descriptors
        close(fdSrc);
        close(fdDest);
    }
}
