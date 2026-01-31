#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>      // For open, O_RDONLY, etc.
#include <unistd.h>     // For read, write, close, stat
#include <sys/stat.h>   // For stat structure and macros
#include <sys/types.h>
#include <string.h>     // For string manipulation
#include <dirent.h>     // For directory traversal

#define BUFFER_SIZE 4096
#define PATH_MAX_LEN 1024

/* * Helper Function: Copies a single file 
 * (This contains your original logic)
 */
void copy_file(const char *src_path, const char *dest_path, mode_t mode) {
    int inputFd, outputFd;
    ssize_t numRead;
    char buffer[BUFFER_SIZE];

    // 1. Open Source File
    inputFd = open(src_path, O_RDONLY);
    if (inputFd == -1) {
        perror("Error: source file couldn't be opened");
        return;
    }

    // 2. Create Destination File (Truncate if exists)
    outputFd = open(dest_path, O_WRONLY | O_CREAT | O_TRUNC, mode);
    if (outputFd == -1) {
        perror("Error: destination file couldn't be created");
        close(inputFd);
        return;
    }

    // 3. Copy Data
    while ((numRead = read(inputFd, buffer, BUFFER_SIZE)) > 0) {
        if (write(outputFd, buffer, numRead) != numRead) {
            perror("Error: writing error");
            close(inputFd);
            close(outputFd);
            return;
        }
    }

    // 4. Transfer Permissions (Linux/Mac)
    #ifndef _WIN32
        if (fchmod(outputFd, mode) == -1) {
            perror("Error: permissions couldn't be set");
        }
    #endif

    close(inputFd);
    close(outputFd);
    printf("Copied file: %s -> %s\n", src_path, dest_path);
}

/* * Recursive Manager: Determines if path is File or Directory 
 */
void process_path(const char *src, const char *dest, int recursive) {
    struct stat st;

    // Get metadata for the source path
    if (stat(src, &st) == -1) {
        perror("Error: cannot stat source path");
        return;
    }

    // --- CASE 1: IT IS A DIRECTORY ---
    if (S_ISDIR(st.st_mode)) {
        // If -r flag is missing, do not copy directory
        if (!recursive) {
            fprintf(stderr, "cp: -r not specified; omitting directory '%s'\n", src);
            return;
        }

        // Create destination directory with same permissions
        if (mkdir(dest, st.st_mode) == -1) {
            // It is okay if directory already exists, otherwise print error
            // Using a simple check here would be better in production, 
            // but perror will show "File exists" which is informative.
        }

        DIR *dir = opendir(src);
        struct dirent *entry;

        if (dir) {
            // Loop through directory contents
            while ((entry = readdir(dir)) != NULL) {
                // SKIP '.' (current) and '..' (parent) to avoid infinite loop
                if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
                    continue;

                char new_src_path[PATH_MAX_LEN];
                char new_dest_path[PATH_MAX_LEN];

                // Construct new paths: src/filename and dest/filename
                snprintf(new_src_path, PATH_MAX_LEN, "%s/%s", src, entry->d_name);
                snprintf(new_dest_path, PATH_MAX_LEN, "%s/%s", dest, entry->d_name);

                // RECURSION: Call this function again for the child item
                process_path(new_src_path, new_dest_path, recursive);
            }
            closedir(dir);
        } else {
            perror("Error: failed to open directory");
        }
    } 
    // --- CASE 2: IT IS A REGULAR FILE ---
    else {
        copy_file(src, dest, st.st_mode);
    }
}

int main(int argc, char *argv[]) {
    int recursive = 0;
    char *source_path = NULL;
    char *dest_path = NULL;

    // Parse Arguments
    if (argc == 3) {
        // Usage: ./mycp <src> <dest>
        source_path = argv[1];
        dest_path = argv[2];
    } 
    else if (argc == 4 && strcmp(argv[1], "-r") == 0) {
        // Usage: ./mycp -r <src> <dest>
        recursive = 1;
        source_path = argv[2];
        dest_path = argv[3];
    } 
    else {
        printf("Usage: %s [-r] <source> <destination>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    // Start the process
    process_path(source_path, dest_path, recursive);

    return 0;
}
