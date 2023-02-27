#include <errno.h>
#include <fcntl.h>
#include <limits.h>
#include <memory.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
void perror(const char *s);

int verify(char *arg) {

  int fd;
  fd = open("temp", O_WRONLY | O_CREAT, 0644);
  fd = open("temp2", O_WRONLY | O_CREAT, 0644);
  system("./dbx ls > temp");
  char *comanda = "grep ";
  char *comanda2;
  comanda2 = (char *)malloc(7 + strlen(comanda) + strlen(arg) + strlen("temp") +
                            strlen(" > temp2"));
  strcpy(comanda2, comanda);
  strcat(comanda2, arg);
  strcat(comanda2, " ");
  strcat(comanda2, "temp");
  strcat(comanda2, " > temp2");
  system(comanda2);
  FILE *fp;
  char line[1001];
  fp = fopen("temp2", "r");
  if (fp == NULL) {
    printf("Error: Unable to open file\n");
    return 0;
  }

  fgets(line, 1001, fp);
  if (strcmp(line, "") == 0)
	return 0;
  char *f = strtok(line, " ");
  while (f) {
    f++;

    if (strlen(arg) != strlen(f))
      f = strtok(NULL, " /");
    else {
      int i = 0;
      while (i < strlen(arg)) {
        if (arg[i] != f[i])
          f = strtok(NULL, " /");
        i++;
      }
    }
    return 1;
  }
  return 0;
}

void ls_dbx() {
  char *comanda = "./dbx ls";
  system(comanda);
}

void mv_dbx(char *arg1, char *arg2) {
  char *comanda = "./dbx mv ";
  char *comanda2;
  comanda2 = (char *)malloc(4 + strlen(comanda) + strlen(arg1) + strlen(arg2));
  strcpy(comanda2, comanda);
  strcat(comanda2, arg1);
  strcat(comanda2, " ");
  strcat(comanda2, arg2);

  system(comanda2);
}

void mkdir_dbx(char *arg) {
  char *comanda = "./dbx mkdir ";
  char *comanda2;
  comanda2 = (char *)malloc(4 + strlen(comanda) + strlen(arg));
  strcpy(comanda2, comanda);
  strcat(comanda2, arg);
  system(comanda2);
}

void rm_dbx(char *arg) {
  char *comanda = "./dbx rm ";
  char *comanda2;
  comanda2 = (char *)malloc(4 + strlen(comanda) + strlen(arg));
  strcpy(comanda2, comanda);
  strcat(comanda2, arg);
  system(comanda2);
}

void cp_dbx(char *arg1, char *arg2) {
  char *comanda = "./dbx cp ";
  char *comanda2;
  comanda2 = (char *)malloc(4 + strlen(comanda) + strlen(arg1) + strlen(arg2));
  strcpy(comanda2, comanda);
  strcat(comanda2, arg1);
  strcat(comanda2, " ");
  strcat(comanda2, arg2);
  system(comanda2);
}

void touch_dbx(char *arg) {
  char *comanda = "./dbx put ";
  char *comanda2;
  comanda2 = (char *)malloc(4 + strlen(comanda) + strlen(arg));
  strcpy(comanda2, comanda);
  strcat(comanda2, arg);
  system(comanda2);
}

void mv_(int argc, const char *argv[]) {
  if (argc != 2) {
    printf("Numar incorect de argumente\n");
    return 1;
  }

  int f1 = open(argv[1], O_RDONLY);
  if (f1 < 0) {
    perror("Nu exista fiserul\n");
    return 1;
  }
  rename(argv[1], argv[2]);
}

void ls_() {
  if (system("ls") != 0) {
    perror("Error");
    return;
  }
}

int isDirectory(const char *path) {
  struct stat statbuf;
  if (stat(path, &statbuf) != 0)
    return 0;
  return S_ISDIR(statbuf.st_mode);
}

void rmdir_(char *arg) {
  if (isDirectory(arg) != 0) {
    char *comanda1 = "rm";
    char *comanda2;
    char *sl = " -r ";
    comanda2 = (char *)malloc(4 + strlen(comanda1) + strlen(arg));
    strcpy(comanda2, comanda1);
    strcat(comanda2, sl);
    strcat(comanda2, arg);
    system(comanda2);
  } else {
    perror("The object isn't a directory\n");
    return;
  }
}

void cp_(int argvs, const char *argv[]) {
  int fs, fd;
  fs = open(argv[1], O_RDONLY);
  fd = open(argv[2], O_WRONLY | O_CREAT, 0644);
  if (fs < 0) {
    perror("file does not exist");
    return errno;
  }
  if (fd < 0) {
    perror("can't create the file");
    return errno;
  }

  char buff[4096];
  ssize_t nread;
  while (nread = read(fs, buff, 4096)) {
    if (nread < 0) {
      perror("can't read");
      return errno;
    }
    ssize_t nwrite = write(fd, buff, nread);
    if (nwrite < 0) {
      perror("can't write");
      return errno;
    }
  }
  close(fd);
  close(fs);
}

void sync_(char *arg) {
  if (isDirectory(arg) == 0) {
    touch_dbx(arg);
  } else {
    int fd;
    fd = open("temp", O_WRONLY | O_CREAT, 0644);
    mkdir_dbx(arg);
    char *c = "ls  ";
    char *c2 = (char *)malloc(16 + strlen(c) + strlen(arg));
    strcpy(c2, c);
    strcat(c2, arg);
    strcat(c2, " > temp");
    system(c2);
    FILE *t = fopen("temp", "r");
    char line[101];
    while (fgets(line, sizeof(line), t) != NULL) {
      strtok(line, "\n");
      if (strchr(line, '.')) {
	touch_dbx(line);
	cp_dbx(line,arg);
	rm_dbx(line);
      }
	else {mkdir_dbx(line);cp_dbx(line,arg);rm_dbx(line);}
    }
  }
}

void rm_(char *arg) {
  if (isDirectory(arg) != 0) {
    perror("The object isn't a file\n");
    return;
  } else {
    if (remove(arg) != 0) {
      perror("Can't delete the file.\n");
      return;
    }
    printf("File %s was succesfuly deleted!\n", arg);
  }
}

char *pwd_() {

  char *abs_path;

  abs_path = realpath("./", NULL);

  return abs_path;
}
void mkdir_(char *nume_fisier) {
  char *abs_path = pwd_();
  char *new_path;
  char *sl = "/";
  new_path = (char *)malloc(2 + strlen(abs_path) + strlen(nume_fisier));
  strcpy(new_path, abs_path);
  strcat(new_path, "/");
  strcat(new_path, nume_fisier);
  mkdir(new_path, 0777);
}

void touch_(char *arg) {
  int fd;
  fd = open(arg, O_WRONLY | O_CREAT, 0644);
}

void cpd_(char *arg1, char *arg2) {
  char *c = "cp ";
  char *c2;
  c2 = (char *)malloc(200);
  strcpy(c2, c);
  strcat(c2, arg1);
  strcat(c2, " ");
  strcat(c2, arg2);
  system(c2);
}

void help() {
  printf("For exit press 1\n");
  printf("For creating a directory press 2\n");
  printf("For creating a file press 3\n");
  printf("For deleting a file  press 4\n");
  printf("For deleting a directory press 5\nDirectory will be deleted "
         "regardless if it is empty or not\n");
  printf("For finding path press 6\n");
  printf("For copying a file press 7\n");
  printf("For visualising directoy press 8\n");
  printf("For renaming a file press 9\n");
  printf("For syncing the file with dbx press 10\n");
  printf("For copying a file to a directory press 11\n");
}
int main() {
  int comanda;
  printf("Wellcome Dropbox\nFor help press 0\nPlease input your command: ");

  int a = 1;
  char temp[100];
  char temp2[100];
  char buff[101], buff1[101];
  char *pt;
  char *pt1;
  while (a == 1) {
    printf("->");
    if (fgets(buff, 101, stdin) == NULL)
      if (feof(stdin))
        break;
    strtok(buff, "\n");
    comanda = atoi(buff);
    switch (comanda) {
    case 0:
      help();
      break;
    case 1:
      a = 0;
      break;
    case 2:
      printf("Input directory name: ");
      fgets(buff, 101, stdin);
      strtok(buff, "\n");
      mkdir_(buff);
      break;
    case 3:
      printf("Input file name: ");
      fgets(buff, 101, stdin);
      strtok(buff, "\n");
      touch_(buff);
      break;
    case 4:
      printf("Input file name: ");
      fgets(buff, 101, stdin);
      strtok(buff, "\n");
      if (isDirectory(buff) != 0)
        break;
      if (verify(buff) == 1) // sincronizat cu dbx
       { rm_dbx(buff);
      rm_(buff);}
      else{printf("nu e"); rm_(buff);}
      break;
    case 5:
      printf("Input directory name: ");
      fgets(buff, 101, stdin);
      strtok(buff, "\n");
      if (isDirectory(buff) == 0)
        break;
	if(verify(buff) == 1)
	{rm_dbx(buff);rmdir_(buff);}
	else
		rmdir_(buff);
        break;

      break;
    case 6:
      printf("Path: %s\n", pwd_());
      break;
    case 11:
      printf("Input one existing source and destination\n");
      fgets(buff, 101, stdin);
      strtok(buff, "\n");
      fgets(buff1, 101, stdin);
      strtok(buff1, "\n");
      const char *v[5];
      v[1] = buff;
      v[2] = buff1;
      if (verify(buff) == 1 && verify(buff1) == 1) {
        cp_dbx(buff, buff1);
        cpd_(buff, buff1);
      } else
        cpd_(buff, buff1);
      break;
    case 8:
      ls_();
      printf("\n");
      ls_dbx();
      break;
    case 9:
      printf("Input one existing source and destination for renaming\n");
      fgets(buff, 101, stdin);
      strtok(buff, "\n");
      fgets(buff1, 101, stdin);
      strtok(buff1, "\n");
      const char *v2[5];
      v2[1] = buff;
      v2[2] = buff1;
      if (verify(buff) == 1) {
        mv_(2, v2);
        mv_dbx(buff, buff1);
      } else
        mv_(2, v2);
      break;
    case 10:
      printf("Input file name to sync\n");
      fgets(buff, 101, stdin);
      strtok(buff, "\n");
      sync_(buff);
      break;
    case 7:
      printf("Input one existing source and destination\n");
      fgets(buff, 101, stdin);
      strtok(buff, "\n");
      fgets(buff1, 101, stdin);
      strtok(buff1, "\n");
      const char *v3[5];
      v3[1] = buff;
      v3[2] = buff1;
      cp_(2, v3);
      break;
    default:
      printf("Input a valid command!");
      printf("->");
      fgets(buff, 101, stdin);
      strtok(buff1, "\n");
      comanda = atoi(buff);
    }
  }
  return 0;
}