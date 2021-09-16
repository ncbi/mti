#include <stdio.h>
#include <db.h>
#include <string.h>
#include <stdlib.h>

#define MAXJGMLINE 400096
#define SMALL_LINE 4096

void closeNclean(int ret, DB *dbp);

int main(int argc, char *argv[])
{
  char database_home[SMALL_LINE + 1];
  char input_file[SMALL_LINE + 1];
  char index_file[SMALL_LINE + 1];
  char database[SMALL_LINE + 1];
  char line[MAXJGMLINE + 1];
  char term[MAXJGMLINE + 1];
  FILE *fp;
  int i;

  DB *dbp;
  DBT key, data;
  int ret;

  /* Setup */

  if(argc != 3)
  {
     fprintf(stderr, "Usage: createBtree filename indexname\n");
     fflush(stderr);
     exit(1);
  } /* fi */

  strcpy(database_home, ".");
  strcpy(input_file, argv[1]);
  strcpy(index_file, argv[2]);
  sprintf(database,"%s/%s", database_home,index_file );

  /* Create the database first */

  if((ret = db_create(&dbp, NULL, 0)) != 0)
  {
     fprintf(stderr, "db_create failed: %s\n", db_strerror(ret));
     exit(1);
  } /* fi */

  dbp->set_flags(dbp, DB_DUP);
  dbp->set_pagesize(dbp, (32 * 1024));

  if((ret = dbp->open(dbp, NULL, index_file, NULL, 
                      DB_BTREE, DB_CREATE, 0644)) != 0)
  {
     dbp->err(dbp, ret, "%s", index_file);
     closeNclean(ret, dbp);
  } /* fi */

  /* Now open the input file and read & load the data */

  fp = fopen(input_file,"r");

  if(fp == NULL)
  {
    fprintf(stderr, "fopen error: %s\n", input_file);
    exit(1);
  } /* fi */

  /* Read the file and load in the keys */

  while(fgets(line, MAXJGMLINE, fp) != NULL)
  {
     if((int) strlen(line) > 0)
     {
        line[strlen(line) -1] = '\0';  /* Remove the newline char */
        strcpy(term, "");
        sscanf(line, "%[^|]", term);

        memset(&key, 0, sizeof(key));
        memset(&data, 0, sizeof(data));
      
        key.data =  term ;
        key.size = sizeof(char) * strlen(term) + 1;
      
        data.data = line; 
        data.size = sizeof(char) * strlen(line) + 1;
      
        ret = dbp->put(dbp, NULL, &key, &data, 0);
        if(ret != 0)
        {
           dbp->err(dbp, ret, "DB->put error");
           closeNclean(ret, dbp);
        } /* fi */
     } /* fi */
  } /* while */

  fclose(fp);
  closeNclean(0, dbp);

  return(0);
} /*** End main */


void closeNclean(int ret, DB *dbp)
{
   dbp->close(dbp, 0);
   exit(ret);
} /* closeNclean */
