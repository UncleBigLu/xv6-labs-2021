#include "kernel/types.h" 
#include "kernel/stat.h" 
#include "user/user.h"
#include "kernel/fs.h"


void find(char *path, char *name) {
    int fd;
    struct stat st;
    char buf[512];
    struct dirent de;
    char *p;
    
    if((fd = open(path, 0)) < 0) {
        fprintf(2, "Can not open %s\n", path);
        return;
    }
    
    if(fstat(fd, &st) < 0) {
        fprintf(2, "Can't stat %s\n", path);
        close(fd);
        return;
    }
    
    switch(st.type) {
        case T_FILE:
            fprintf(2, "%s: Is a file\n", path);
            close(fd);
            return;
        
        case T_DIR:
            if(strlen(path) + 1 + DIRSIZ + 1 > sizeof buf) {
                printf("ls: path too long\n");
                break;
            }
            strcpy(buf, path);
            p = buf+strlen(buf);
            *p++ = '/';
            while(read(fd, &de, sizeof(de)) == sizeof(de)) {
                if(de.inum == 0)
                    continue;
                memmove(p, de.name, DIRSIZ);
                p[DIRSIZ] = 0;
                if(stat(buf, &st) < 0){
                    printf("ls: cannot stat %s\n", buf);
                    continue;
                }
                if(strcmp(name, de.name) == 0) {
                    printf("%s\n", buf);
                }
                if(st.type == T_DIR){
                    if(strcmp(de.name, ".") != 0 && strcmp(de.name, "..") != 0){
                        find(buf, name);
                    }
                }
           
        }
         break;
        close(fd);
    }
    return;
}


int main(int argc, char* argv[]) {
    if(argc < 3) { 
        printf("Usage: find <path> <name>\n");
        exit(0); 
    }
    find(argv[1], argv[2]);
    exit(0);
}

