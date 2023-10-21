/*
  Copyright (C) 2023 3CS
  Copyright (C) 2023 Janith Cooray
  
  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or (at
  your option) any later version.

  This program is distributed in the hope that it will be useful, but
  WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see https://www.gnu.org/licenses/.
*/

bool d_open_file(char* name){
  FILE *fptr;
  fptr = fopen(name, "r+");
  if(fptr == NULL) 
   {
     fptr = fopen(name, "w+");
     fclose(fptr);
     return false;
   }
  fclose(fptr);
  return true;
}

int d_readfile(char* name){
  FILE *fptr;
  fptr = fopen(name,"r");
  int num;
  if(fptr != NULL){
    fscanf(fptr, "%d", &num);
    fclose(fptr);
     return num;
  }
  
  return 0;
}

// 0 for clear and 1 for queue
bool d_write_file(char* name, int num){
  FILE *fptr;
  fptr = fopen(name,"w");
  if(fptr != NULL){
    fprintf(fptr, "%d\n", num);
    fclose(fptr);
    if (num == 1)
      return true;
  }
  return false;
}

// check a directory for its permission schemes
/**
   This is not needed to run but its to address a vulneralbility.

   if the non-root user is able to modify the script or the config
   which points the script, it will void the entire purpose of this
   program
 **/
int check_permissions(const char *dir_path) {
    DIR *dir;
    struct dirent *entry;
    struct stat file_stat;
    int check_result = 0;
    dir = opendir(dir_path);
    if (dir == NULL) {
        perror("opendir");
        return -1; // Failed to open directory
    }
    while ((entry = readdir(dir)) != NULL) {
        char file_path[1000];

        sprintf(file_path, "%s/%s", dir_path, entry->d_name);

        if (stat(file_path, &file_stat) != 0) {
            perror("stat");
            check_result = -1; // Failed to get file/directory information
            break;
        }

        if ((file_stat.st_mode & S_IFREG) != 0 && (file_stat.st_mode & 0777) != 0755) {
            printf("File '%s' does not have permissions set to 755.\n", file_path);
            check_result = -1; // Permissions check failed
            break;
        }

        if ((file_stat.st_mode & S_IFDIR) != 0) {
            if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0) {
                int sub_check_result = check_permissions(file_path);
                if (sub_check_result != 0) {
                    check_result = -1; // Permissions check failed in a subdirectory
                    break;
                }
            }
        }
    }

    closedir(dir);

    return check_result;
}

char* getParentDirectory(const char* file, char* parentDir) {
  
  int i, lastSlashPosition = -1;
  
    for (i = strlen(file) - 1; i >= 0; i--) {
        if (file[i] == '/') {
            lastSlashPosition = i;
            break;
        }
    }
    
    if (lastSlashPosition != -1) {
        strncpy(parentDir, file, lastSlashPosition + 1);
    } else {
        strcpy(parentDir, ".");
    }
}
