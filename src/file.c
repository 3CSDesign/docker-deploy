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
