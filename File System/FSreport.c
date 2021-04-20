#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <dirent.h>
#include <unistd.h>
#include <sys/stat.h>
#include <time.h>

int inodeNum;
char *name;

typedef struct counter
{
  size_t dirs;
  size_t files;
} countNum;

typedef struct entry
{
  char *name;
  int directory;
  struct entry *nextLink;
} treeIenter;

void printFileDates(struct stat stats);
int tree(const char *directory, const char *prefix, countNum *counter);
int inode(const char *directory, const char *prefix, countNum *counter);
void stroked(const char *name)
{

  char *localstr = malloc(sizeof(char *) * 500);
  char *tmp;
  strcpy(localstr, name);

  tmp = strtok(localstr, "/");

  tmp = strtok(NULL, "/");
  printf("\n%s  ", tmp);
}

int inode(const char *directory, const char *prefix, countNum *counter)
{

  char *path;
  char *segObj;
  char *dirPoint;
  char *linker;

  treeIenter *head = NULL;
  treeIenter *current;
  treeIenter *iter;
  size_t size = 0, index;

  struct dirent *directFile;

  DIR *handler;
  struct stat stats;
  struct tm dt;
  struct dirent *entryDirectory = NULL;

  handler = opendir(directory);
  if (!handler)
  {
    fprintf(stderr, "Cannot open directory \"%s\"\n", directory);
    return -1;
  }

  counter->dirs++;

  while ((directFile = readdir(handler)) != NULL)
  {

    // printf("\n%ld\n", directFile->d_ino);
    if (directFile->d_name[0] == '.')
    {
      continue;
    }

    current = malloc(sizeof(treeIenter) * 50);
    current->directory = directFile->d_type == DT_DIR;
    current->nextLink = NULL;
    current->name = strcpy(malloc(strlen(directFile->d_name)), directFile->d_name);

    inodeNum = directFile->d_ino;

    if (head == NULL)
    {
      head = current;
    }
    else if (strcmp(current->name, head->name) < 0)
    {
      current->nextLink = head;
      head = current;
    }
    else
    {
      for (iter = head; iter->nextLink && strcmp(current->name, iter->nextLink->name) > 0; iter = iter->nextLink)
        ;

      current->nextLink = iter->nextLink;
      iter->nextLink = current;
    }

    size++;
  }

  closedir(handler);
  if (!head)
  {
    return 0;
  }

  for (index = 0; index < size; index++)
  {
    int buffer = size - 1;
    if (buffer == index)
    {
      dirPoint = "";
      segObj = "    ";
    }
    else
    {
      dirPoint = "";
      segObj = "   ";
    }

    printf("\n%d\t%s\t\t%ld\n", inodeNum, head->name, stats.st_size);

    if (head->directory)
    {

      path = malloc(strlen(directory) + strlen(head->name) + 2);
      sprintf(path, "%s/%s", directory, head->name);

      linker = malloc(strlen(prefix) + strlen(segObj) + 1);
      sprintf(linker, "%s%s", prefix, segObj);

      tree(path, linker, counter);

      free(path);
      free(linker);
    }
    else
    {
      counter->files++;
    }

    current = head;
    head = head->nextLink;

    free(current->name);
    free(current);
  }

  return 0;
}

int tree(const char *directory, const char *prefix, countNum *counter)
{
  char *path;
  char *segObj;
  char *dirPoint;
  char *linker;

  treeIenter *head = NULL;
  treeIenter *current;
  treeIenter *iter;
  size_t size = 0, index;

  struct dirent *directFile;

  DIR *handler;
  struct stat stats;
  struct tm dt;
  struct dirent *entryDirectory = NULL;

  char delim[] = "/";

  char *ptr;
  char *ptr2;

  handler = opendir(directory);
  if (!handler)
  {
    fprintf(stderr, "no directory");
    return -1;
  }

  counter->dirs++;

  while ((directFile = readdir(handler)) != NULL)
  {

    if (directFile->d_name[0] == '.')
    {
      continue;
    }

    current = malloc(sizeof(treeIenter));
    current->name = strcpy(malloc(strlen(directFile->d_name)), directFile->d_name);
    current->directory = directFile->d_type == DT_DIR;
    current->nextLink = NULL;

    inodeNum = directFile->d_ino;

    if (head == NULL)
    {
      head = current;
    }
    else if (strcmp(current->name, head->name) < 0)
    {
      current->nextLink = head;
      head = current;
    }
    else
    {
      for (iter = head; iter->nextLink && strcmp(current->name, iter->nextLink->name) > 0; iter = iter->nextLink)
        ;

      current->nextLink = iter->nextLink;
      iter->nextLink = current;
    }

    size++;
  }

  closedir(handler);
  if (!head)
  {
    return 0;
  }

  for (index = 0; index < size; index++)
  {
    int buffer = size - 1;

    if (buffer == index)
    {
      dirPoint = "";
      segObj = "    ";
    }
    else
    {
      dirPoint = "";
      segObj = "   ";
    }

    stroked(directory);
    printf("%s\t%d\t\t%ld\n", head->name, inodeNum, stats.st_size);
    printFileDates(stats);

    if (head->directory)
    {
      //printf

      path = malloc(strlen(directory) + strlen(head->name) + 2);
      sprintf(path, "%s/%s", directory, head->name);

      linker = malloc(strlen(prefix) + strlen(segObj) + 1);
      sprintf(linker, "%s%s", prefix, segObj);

      tree(path, linker, counter);

      free(path);
      free(linker);
    }
    else
    {
      counter->files++;
    }
    //set current to head

    current = head;
    head = head->nextLink;

    //free

    free(current->name);
    free(current);
  }

  return 0;
}
void printFileDates(struct stat stats)
{
  struct tm dt;

  dt = *(gmtime(&stats.st_ctime));

  printf("\n\t%d-%d-%d %d:%d:%d", dt.tm_mday, dt.tm_mon, dt.tm_year,
         dt.tm_hour, dt.tm_min, dt.tm_sec);

  dt = *(gmtime(&stats.st_mtime));
  printf("  %d-%d-%d %d:%d:%d\n", dt.tm_mday, dt.tm_mon, dt.tm_year,
         dt.tm_hour, dt.tm_min, dt.tm_sec);
}

int main(int argc, char const *argv[])
{
  char *dir;
  struct stat stats;

  for (int i = 2; i < argc; i++)
  {

    if (strcmp(argv[1], "-tree") == 0)
    {
      countNum counter = {0, 0};

      printf("Level 1 tree: %s\n", argv[2]);
      tree(argv[2], "", &counter);

      return 0;
    }
    if (strcmp(argv[1], "-inode") == 0)
    {
      countNum counter = {0, 0};

      printf("Level 1 inodes: %s\n", argv[2]);

      inode(argv[2], "", &counter);

      return 0;
    }
    else
    {
      printf("invalid input\n");
      return 0;
    }
  }

  return 0;
}
