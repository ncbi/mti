#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "newMain.h"
#include "myLocal.h"

#define MAXLINE 9000
#define MED_LINE 40096
#define SMALL_LINE 4096
#define TINY_LINE 1024
#define TRUE 1
#define FALSE 0
#define MAXNUMLINES 100
#define MAXLCHM 350000
#define MAXLCSH 500

#define VMHSH   17
#define CATREPL 18


struct repl_struct {
  char *search_for;
  char *replace_by;
};

struct repl_struct **repl_list;
long num_repls = 0;
long max_repls = 20;

char *qualifiers[90];
int numQuals = 0;

struct LCSHStruct {
   char *LCH;
   char *SH;
};

struct LCSHStruct LCSHs[MAXLCSH];
long numLCSHs = 0;

struct LCMHStruct {
   char *LCH;
   long lenLCH;
   int level;
   char *MH;
};

struct LCMHStruct LCMHs[MAXLCHM];
long numLCMHs = 0;
long firstPos[127][127];

char LCMapLinesSorted[MAXNUMLINES][MED_LINE + 1];
char LClinesSorted[MAXNUMLINES][MED_LINE + 1];
char LClines[MAXNUMLINES][MED_LINE + 1];
int numLCs, maxLCs, numSorted, numLCMapLines;

extern char *globalTitle;
extern char *globalAbstract;
extern char main_uid[SMALL_LINE + 1];
extern char nlmID[SMALL_LINE + 1];
extern int haveHMD;
extern long gTLen, gALen, maxgALen;
extern FILE *fout;
extern int RTM_Debug;

extern void addLCMapElement(char *LCH, char *Map, int level);
extern int query_btree();

void read_Cataloging_Citation (FILE *fpP);
void GetText(FILE *fp);
void Load_Quals();
void Load_QualsII();
void Load_Repls();
void Add_Repls(char *search_for, char *replace_by);
char *Strip_Str(char *insegment);
char *splitContents(char *line);
char *Strip_StrII(char *insegment, char *search_for, char *replace_by);
void processLClines();
int charCompare(const void *p1, const void *p2);
int charCompareII(const void *p1, const void *p2);
char *Strip_DollarH(char *inTitle);
char *Strip_URLs(char *inText);
int isAQual(char *term, char *first, char *firstN, char *firstO,
            int firstMapBad);
void addLCLine(char *LCH, char *Repl, char *normMH, int level);
void addLCTerm(char *LCH, char *term, char *normMH, int level);
void loadLCHtoMH();
long findMapping(char *term);
void checkMHSHCombos(int firstMapBad, char *firstN, char *SH, char *firstO,
                     char *tmpII, char *tmpIII);
void cleanReplList();

/* ------------------------------ MAIN ------------------------------ */

void read_Cataloging_Citation (FILE *fpP)
{
    int i;

    Load_Quals();
    Load_QualsII();
    Load_Repls();
    loadLCHtoMH();

    GetText(fpP);

    /* Cleanup */

    for(i = 0; i < numQuals; i++)
      free(qualifiers[i]);

    cleanReplList();
} /* read_Cataloging_Citation */

/***************************************************************************
*
*  GetText --
*
*      This 
*
***************************************************************************/

void GetText(FILE *fp)
{
    char line[MED_LINE + 1];
    char Title[MED_LINE + 1];
    char Contents[MED_LINE + 1];
    char Summary[MED_LINE + 1];
    char tmp[MED_LINE + 1];
    char tmp2[MED_LINE + 1];
    char *foo, *fooII, *fooIII;
    long nlmIDL, PMID, diff, len;
    int before, i, type_950;

    strcpy(Title, "");
    strcpy(Contents, "");
    strcpy(Summary, "");
    numLCs = 0;
    maxLCs = 0;
    nlmIDL = -1;
    PMID = -1;
    while(fgets(line, MED_LINE, fp) != (char *)NULL)
    {
        if(strncmp(line, "001 ", 4) == 0) /* PMID */
        {
            PMID = -1;
            sscanf(line, "001 %ld", &PMID);
        } /* fi */

        else if(strncmp(line, "035 ", 4) == 0) /* nlmID */
        {
            if(strstr(line, "$a") == NULL)
            {
                nlmIDL = -1;
                sscanf(line, "035 %ld", &nlmIDL);
            } /* fi */
        } /* else fi */

        else if(strncmp(line, "245 ", 4) == 0) /* Title */
        {
            strcpy(tmp, "");
            sscanf(line, "%*[^$]$a%[^\n]", tmp);

            /* Secondary testing added 11/2/09 JGM.  Example:
               245 00$6880-01$aJapanese Classification of Esophageal Cancer 
            */
 
            if(strlen(tmp) == 0)
            {
               strcpy(tmp, "");
               sscanf(line, "%*[^$]$%*[^$]$a%[^\n]", tmp);
            } /* fi */

            foo = strstr(tmp, "/$c");
            strcpy(tmp2, tmp);
            if(foo != NULL)
            {
                diff = foo - tmp;
                tmp2[diff] = '\0';
            } /* fi */

            if(strlen(tmp2) > 0)
            {
               if(strlen(Title) > 0)
                 strcat(Title, "; ");
               foo = Strip_Str(tmp2);

               fooII = Strip_DollarH(foo);
               free(foo);
               fooIII = Strip_URLs(fooII);
               free(fooII);
               strcat(Title, fooIII);
               free(fooIII);
            } /* fi */
        } /* else fi */

        /* NOTE: Although we track 505 Contents field, we do not use it
           7/12/07 email explains it causes too much noise.
        */

        else if(strncmp(line, "505 ", 4) == 0) /* Contents */
        {
            strcpy(tmp, "");
            sscanf(line, "%*[^$]$a%[^\n]", tmp);
            if(strstr(line, " / ") != NULL)
            {
                foo = splitContents(tmp);
                strcpy(tmp, foo);
                free(foo);
            } /* fi */

            if(strlen(tmp) > 0)
            {
               foo = Strip_StrII(tmp, " -- ", ". ");
               strcpy(tmp, foo);
               free(foo);
               if(strlen(Contents) > 0)
                 strcat(Contents, "; ");
               strcat(Contents, tmp);
            } /* fi */
        } /* else fi */

        else if(strncmp(line, "520 ", 4) == 0) /* Summary */
        {
            strcpy(tmp, "");
            sscanf(line, "%*[^$]$a%[^\n]", tmp);
            if(strlen(tmp) > 0)
            {
               if(strlen(Summary) > 0)
                 strcat(Summary, "; ");
               foo = Strip_Str(tmp);
               fooII = Strip_URLs(foo);
               free(foo);
               strcat(Summary, fooII);
               free(fooII);
            } /* fi */
        } /* else fi */

        /* Don't want Foreign language (French=6, German=17) text though */

        else if((strncmp(line, "950 ", 4) == 0) && /* Library of Congress */
                (strncmp(line, "950  4", 6) != 0) && /* Spanish & Misc. */
                (strncmp(line, "950 06", 6) != 0) && /* French */
                (strncmp(line, "950  6", 6) != 0) && /* French */
                (strncmp(line, "950  7", 6) != 0) && /* Finnish */
                (strncmp(line, "950 07", 6) != 0) && /* Finnish */
                (strncmp(line, "950 17", 6) != 0)) /* German */
        {
            strcpy(tmp, "");
            type_950 = -1;
            sscanf(line, "950 %d$a%[^\n]", &type_950, tmp);

            if(strncmp(tmp,
               "Video recordings for the hearing impaired", 40) != 0)
              strcpy(LClines[numLCs++], tmp);
        } /* else fi */

        else if(strncmp(line, "992 ", 4) == 0) /* HMD/Catalog designation */
        {
            if(strstr(line, "992 HMD") != NULL)
              haveHMD = TRUE;
        } /* else fi */

        else if(strncmp(line, "999 ", 4) == 0) /* End Of Record */
        {
             if(numLCs > maxLCs)
               maxLCs = numLCs;

             processLClines();

             sprintf(main_uid, "%ld\0", PMID);
             sprintf(nlmID, "%ld\0", nlmIDL);

             free(globalTitle);
             globalTitle = strdup(Title);
             gTLen = (long)strlen(globalTitle);

             if((numSorted > 0) || (strlen(Summary) > 0))
             {
                 len = (long)strlen(Summary);
                 if(len > 0)
                 {
                     foo = Strip_Str(Summary);
                     len = (long)strlen(foo);
                     if((len + gALen + 100) >= maxgALen)
                     {
                         maxgALen += (len + gALen + MED_LINE);
                         globalAbstract = realloc(globalAbstract, maxgALen + 1);
                     } /* fi */

                     sprintf(globalAbstract, "%s; \0", foo);
                     gALen = (long)strlen(globalAbstract);
                     free(foo);
                 } /* fi */

                 if(numSorted > 0)
                 {
                    for(i = 0; i < numSorted; i++)
                    {
                        len = (long)strlen(LClinesSorted[i]);
                        if((len + gALen + 100) >= maxgALen)
                        {
                            maxgALen += (len + gALen + MED_LINE);
                            globalAbstract = realloc(globalAbstract,
                                                              maxgALen + 1);
                        } /* fi */

                        if(i > 0)
                           strcat(globalAbstract, "; ");

                        strcat(globalAbstract, LClinesSorted[i]);
                        gALen = (long)strlen(globalAbstract);
                    } /* for */

                    strcat(globalAbstract, "\n");
                    gALen = (long)strlen(globalAbstract);
                 } /* fi numSorted */
             } /* fi AB info */

             strcpy(Title, "");
             strcpy(Contents, "");
             strcpy(Summary, "");
             nlmIDL = -1;
             PMID = -1;
             for(i = 0; i < numLCs; i++)
             {
                strcpy(LClines[i], "");
                strcpy(LClinesSorted[i], "");
             } /* for */
             numLCs = 0;
        } /* else fi */
    } /* while */
} /* GetText */

/***************************************************************************
*
*  Load_Quals --
*
*      This routine
*
***************************************************************************/

void Load_Quals()
{
    FILE *fp;
    char line[SMALL_LINE + 1], qual[SMALL_LINE + 1];

    fp = fopen(QUALIFIERS, "r");
    while(fgets(line, MED_LINE, fp) != (char *)NULL)
    {
       strcpy(qual, "");
       sscanf(line, "%*[^|]|%[^\n]", qual);
       if(strlen(qual) > 0)
          qualifiers[numQuals++] = (char *)strdup(qual);
       else
       {
           fprintf(stderr, "ERROR: Qualifier bad #%s#\n", line);
           fflush(stderr);
       } /* else */
    } /* while */
    fclose(fp);
} /* Load_Quals */

/***************************************************************************
*
*  Load_QualsII --
*
*      This routine
*
***************************************************************************/

void Load_QualsII()
{
    FILE *fp;
    char line[SMALL_LINE + 1], qual[SMALL_LINE + 1], lch[SMALL_LINE + 1];

    fp = fopen(QUALSII, "r");
    while(fgets(line, MED_LINE, fp) != (char *)NULL)
    {
       strcpy(qual, "");
       strcpy(lch, "");
       sscanf(line, "%[^|]|%[^\n]", lch, qual);
       if((strlen(qual) > 0) && (strlen(lch) > 0))
       {
          if(numLCSHs < MAXLCSH)
          {
             LCSHs[numLCSHs].LCH = (char *)strdup(lch);
             LCSHs[numLCSHs].SH = (char *)strdup(qual);
             numLCSHs++;
          } /* fi */

          else
          {
             fprintf(stderr, "Ran out of QualsII space\n");
             fflush(stderr);
          } /* else */
       } /* fi */
       else
       {
           fprintf(stderr, "ERROR: QualII bad #%s#\n", line);
           fflush(stderr);
       } /* else */
    } /* while */
    fclose(fp);
} /* Load_QualsII */

/***************************************************************************
*
*  Load_Repls --
*
*      This routine
*
***************************************************************************/

void Load_Repls()
{
    FILE *fp;
    char line[SMALL_LINE + 1], search_for[SMALL_LINE + 1], 
         replace_by[SMALL_LINE + 1];

    repl_list = (struct repl_struct **)
             malloc(sizeof(struct repl_struct *) * max_repls);

    Add_Repls("$c", "");
    Add_Repls("$z", "; ");
    Add_Repls("$x", "; ");
    Add_Repls("$v", "; ");
    Add_Repls("$b", " ");
    Add_Repls("$p", " ");
    Add_Repls("$n", " ");

    fp = fopen(REPL_FILE, "r");
    while(fgets(line, MED_LINE, fp) != (char *)NULL)
    {
       strcpy(search_for, "");
       strcpy(replace_by, "");
       sscanf(line, "%[^|]|%[^|]", search_for, replace_by);

       Add_Repls(search_for, replace_by);
    } /* while */
    fclose(fp);

    /* Now realloc the repl_list to only what we need */

    repl_list = (struct repl_struct **)
       realloc(repl_list, sizeof(struct repl_struct *) * num_repls);
} /* Load_Repls */

/***************************************************************************
*
*  Add_Repls --
*
*      This routine
*
***************************************************************************/

void Add_Repls(char *search_for, char *replace_by)
{
    long len_s, len_r;

    if(num_repls >= max_repls)
    {
        max_repls += 10;
        repl_list = (struct repl_struct **)
          realloc(repl_list, sizeof(struct repl_struct *) * max_repls);
    } /* fi */

    repl_list[num_repls] =
          (struct repl_struct *) malloc(sizeof(struct repl_struct));

    len_s = (long)strlen(search_for);
    if(len_s <= 0)
       len_s = 1;

    repl_list[num_repls]->search_for = (char *)malloc(len_s + 1);
    strcpy(repl_list[num_repls]->search_for, search_for);

    len_r = (long)strlen(replace_by);
    if(len_r <= 0)
       len_r = 1;

    repl_list[num_repls]->replace_by = (char *)malloc(len_r + 1);
    strcpy(repl_list[num_repls]->replace_by, replace_by);
    num_repls++;
} /* Add_Repls */

/***************************************************************************
*
*  Strip_Str --
*
*      This routine
*
***************************************************************************/

char *Strip_Str(char *insegment)
{
   long len = ((long)strlen(insegment) * 5);
   long slen, diff, search_end, rlen, tlen;
   int found;
   char *rtn = (char *)malloc(len + 1);
   char *tmp2 = (char *)malloc(len + 1);
   char *tmp3 = (char *)malloc(len + 1);
   char *tmp;
   long i, j, rtn_pos, pos;

   strcpy(tmp3, insegment);

   for(i = 0; i < num_repls; i++)
   {
      found = FALSE;
      strcpy(rtn, "");
      rtn_pos = 0;
      tlen = (long)strlen(tmp3);
      slen = (long)strlen(repl_list[i]->search_for);
      rlen = (long)strlen(repl_list[i]->replace_by);

      strcpy(tmp2, "");
      tmp = strstr(tmp3, repl_list[i]->search_for);
      if(tmp != NULL)
      {
         found = TRUE;

         while(found)
         {
            diff = tmp - tmp3;
            search_end = diff + slen;

            /* Copy up to what we are replacing */

            for(j = 0; j < diff; j++)
              rtn[rtn_pos++] = tmp3[j];
            rtn[rtn_pos] = '\0';

            /* Add in the replace_by information */

            for(j = 0; j < rlen; j++)
              rtn[rtn_pos++] = repl_list[i]->replace_by[j];
            rtn[rtn_pos] = '\0';

            /* Now save off the remainder of the string to process */

            pos = 0;
            for(j = search_end; j < tlen; j++)
              tmp2[pos++] = tmp3[j];
            tmp2[pos] = '\0';

            strcpy(tmp3, tmp2);
            tmp = strstr(tmp3, repl_list[i]->search_for);
            if(tmp != NULL)
              found = TRUE;
            else
            {
               found = FALSE;
               strcat(rtn, tmp3);
            } /* else */
         } /* while */
      } /* fi */
      else
      {
         found = FALSE;
         strcpy(rtn, tmp3);
      } /* else */

      strcpy(tmp3, rtn);
   } /* for */

   free(tmp2); free(tmp3);

   len = (long)strlen(rtn);
   rtn = (char *)realloc(rtn, len + 1);

   return(rtn);
} /* Strip_Str */


char *splitContents(char *line)
{
    long len = (long)strlen(line);
    char *rtn = (char *)malloc((len * 2) + 1);
    char *tmp, *foo;
    char tmp2[MED_LINE + 1];
    char *tokptr;
    long i, pos, lenT, diff;
    int firstTime = TRUE;

    strcpy(rtn, "");
    tmp = strdup(line);
    pos = 0;
    while((tokptr = strtok(tmp, "/")) != NULL)
    {
        lenT = (long)strlen(tokptr);
        if(tokptr[lenT - 1] == ' ')
          tokptr[lenT - 1] = '\0';

        if(strstr(tokptr, "--") != NULL)
        {
           strcpy(tmp2, "");
           sscanf(tokptr, "%*[^-]-- %[^\n]", tmp2);
           if(strlen(tmp2) > 0)
           {
               if(strlen(rtn) > 0)
                 strcat(rtn, " -- ");
               strcat(rtn, tmp2);
           } /* fi */
        } /* fi */

        else if(firstTime)
        {
            firstTime = FALSE;
            if(strlen(rtn) > 0)
              strcat(rtn, " -- ");
            strcat(rtn, tokptr);
        } /* else */

        tmp = NULL;
    } /* while */

    return(rtn);
} /* splitContents */

/***************************************************************************
*
*  Strip_StrII --
*
*      This routine
*
***************************************************************************/

char *Strip_StrII(char *insegment, char *search_for, char *replace_by)
{
   long len = ((long)strlen(insegment) * 5);
   long slen, diff, search_end, rlen, tlen;
   int found;
   char *rtn = (char *)malloc(len + 1);
   char *tmp2 = (char *)malloc(len + 1);
   char *tmp3 = (char *)malloc(len + 1);
   char *tmp;
   long i, j, rtn_pos, pos;

   strcpy(tmp3, insegment);

   found = FALSE;
   strcpy(rtn, "");
   rtn_pos = 0;
   tlen = (long)strlen(tmp3);
   slen = (long)strlen(search_for);
   rlen = (long)strlen(replace_by);

   strcpy(tmp2, "");
   tmp = strstr(tmp3, search_for);
   if(tmp != NULL)
   {
      found = TRUE;

      while(found)
      {
         diff = tmp - tmp3;
         search_end = diff + slen;

         /* Copy up to what we are replacing */

         for(j = 0; j < diff; j++)
           rtn[rtn_pos++] = tmp3[j];
         rtn[rtn_pos] = '\0';

         /* Add in the replace_by information */

         for(j = 0; j < rlen; j++)
           rtn[rtn_pos++] = replace_by[j];
         rtn[rtn_pos] = '\0';

         /* Now save off the remainder of the string to process */

         pos = 0;
         for(j = search_end; j < tlen; j++)
           tmp2[pos++] = tmp3[j];
         tmp2[pos] = '\0';

         strcpy(tmp3, tmp2);
         tmp = strstr(tmp3, search_for);
         if(tmp != NULL)
           found = TRUE;
         else
         {
            found = FALSE;
            strcat(rtn, tmp3);
         } /* else */
      } /* while */
   } /* fi */
   else
   {
      found = FALSE;
      strcpy(rtn, tmp3);
   } /* else */

   strcpy(tmp3, rtn);

   free(tmp2); free(tmp3);

   len = (long)strlen(rtn);
   rtn = (char *)realloc(rtn, len + 1);

   return(rtn);
} /* Strip_StrII */


/***************************************************************************
*
*  charCompare --
*
*      This 
*
***************************************************************************/
int charCompare(const void *p1, const void *p2)
{
   int status;

   status = strcmp(p1, p2);

   return(status);
} /* charCompare */


/***************************************************************************
*
*  charCompareII --
*
*      This 
*
***************************************************************************/
int charCompareII(const void *p1, const void *p2)
{
   int status;
   char a[MAXLINE + 1], b[MAXLINE + 1];

   strcpy(a, "");
   strcpy(b, "");
   sscanf(p1, "%*[^|]|%[^|]", a);
   sscanf(p2, "%*[^|]|%[^|]", b);

   status = strcmp(a, b);

   return(status);
} /* charCompareII */


/***************************************************************************
*
*  Strip_DollarH --
*
*      This 
*
***************************************************************************/
char *Strip_DollarH(char *inTitle)
{
   long len = (long)strlen(inTitle);
   char *rtn = (char *)malloc(len + 1);
   long i, diff, pos;
   char *foo;

   foo = strstr(inTitle, "$h[");
   if(foo == NULL)
     strcpy(rtn, inTitle);
   else
   {
       diff = foo - inTitle;
       pos = 0;
       strcpy(rtn, "");
       for(i = 0; (i < diff) && (i < len); i++)
         rtn[pos++] = inTitle[i];

       while((inTitle[diff] != ']') && (diff < len))
         diff++;

       diff++;
       for(i = diff; i < len; i++)
         rtn[pos++] = inTitle[i];
       rtn[pos] = '\0';
   } /* else */

   return(rtn);
} /* Strip_DollarH */


/***************************************************************************
*
*  Strip_URLs --
*
*      This 
*
***************************************************************************/
char *Strip_URLs(char *inText)
{
   long len = (long)strlen(inText);
   char *tmp = (char *)malloc(len + 1);
   char *tmpII = (char *)malloc(len + 1);
   char *rtn;
   int foundURL;
   long i, diff, pos;
   char *foo;

   strcpy(tmp, inText);
   foundURL = TRUE;
   while(foundURL)
   {
      foo = strstr(tmp, "http:");

      if(foo == NULL)
        foo = strstr(tmp, "https:");

      if(foo == NULL)
        foo = strstr(tmp, "www");

      if(foo == NULL)
       foundURL = FALSE;

      else
      {
          diff = foo - tmp;
          pos = 0;
          strcpy(tmpII, "");
          for(i = 0; (i < diff) && (i < len); i++)
            tmpII[pos++] = tmp[i];

          while((tmp[diff] != ' ') && (diff < len))
            diff++;

          diff++;
          for(i = diff; i < len; i++)
            tmpII[pos++] = tmp[i];
          tmpII[pos] = '\0';

          strcpy(tmp, tmpII);
          len = (long)strlen(tmp);
      } /* else */
   } /* while */

   rtn = (char *)strdup(tmp);
   free(tmp); free(tmpII);

   return(rtn);
} /* Strip_URLs */


/***************************************************************************
*
*  processLClines --
*
*      This 
*
***************************************************************************/
void processLClines()
{
    int i, level, firstMapBad, firstFree;
    long j, len, mapPos;
    char first[SMALL_LINE + 1], firstN[SMALL_LINE + 1], 
         firstO[SMALL_LINE + 1];
    char tmp[SMALL_LINE + 1], tmpII[SMALL_LINE + 1], 
         element[SMALL_LINE + 1], rest[SMALL_LINE + 1];
    char tmpJ[TINY_LINE + 1], tmpK[TINY_LINE + 1], tmpL[TINY_LINE + 1];
    char marker, nextMarker;
    char *foo;

    numSorted = 0;
    numLCMapLines = 0;
    for(i = 0; i < numLCs; i++)
    {
        strcpy(first, "");
        strcpy(firstN, "");
        strcpy(firstO, "");
        firstMapBad = FALSE;
        marker = 'a';

        if(strchr(LClines[i], '$') != NULL)
        {
            strcpy(tmp, LClines[i]);
            while(strlen(tmp) > 0)
            {
               strcpy(rest, "");
               strcpy(element, "");
               sscanf(tmp, "%[^$]%[^\n]", element, rest);

               len = (long)strlen(rest);
               if(len > 0)
               {
                  nextMarker = rest[1];
                  strcpy(tmp, rest + 2);
               } /* fi */

               else
                 strcpy(tmp, rest);

               len = (long)strlen(element);
               if(len > 0)
               {
                   if(element[len - 1] == '.')
                     element[len - 1] = '\0';

                   foo = Strip_Str(element);
                   if(!isAQual(foo, first, firstN, firstO, firstMapBad))
                   {
                       mapPos = findMapping(foo);
                       if(mapPos > -1)
                       {
                           if(strlen(first) == 0)
                             firstFree = TRUE;
                           else
                             firstFree = FALSE;

                           strcpy(tmpJ, LCMHs[mapPos].MH);
                           while(strlen(tmpJ) > 0)
                           {
                               strcpy(tmpK, "");
                               strcpy(tmpL, "");
                               sscanf(tmpJ, "%[^:]:%[^\n]", tmpK, tmpL);
                               strcpy(tmpJ, tmpL);

                               if(strlen(tmpK) > 0)
                               {
                                  if(firstFree)
                                  {
                                     if(strlen(first) > 0)
                                       strcat(first, ":");

                                     if(strlen(firstO) > 0)
                                       strcat(firstO, ":");

                                     if(strlen(firstN) > 0)
                                       strcat(firstN, ":");
                                  } /* fi */

                                  if(strcmp(tmpK, foo) == 0)
                                  {
                                      addLCLine(foo, "", foo, 150);
                                      if(firstFree)
                                      {
                                         strcat(first, foo);
                                         strcat(firstO, foo);
                                         strcat(firstN, foo);
                                      } /* fi */
                                  } /* fi */

                                  else
                                  {
                                      sprintf(tmpII, "%s [%s]\0",tmpK, foo);
                                      if(firstFree)
                                      {
                                         strcat(first, tmpII);
                                         strcat(firstO, foo);
                                         strcat(firstN, tmpK);
                                      } /* fi */

                                      addLCLine(foo, tmpII, tmpK,
                                                LCMHs[mapPos].level);
                                  } /* else */
                               } /* fi tmpK */
                           } /* while */
                       } /* fi */

                       else if(strlen(foo) > 0)
                       {
                           if((marker != 'd') && (marker != 'x') &&
                                  (strstr(foo, "(DNLM)D") == NULL))
                           {
                               if(marker == 'a')
                                 fprintf(stderr, "LCSH not found|%c|[TOPIC] %s", marker, foo);

                               else if(marker == 'z')
                                 fprintf(stderr, "LCSH not found|%c|[GEO] %s", marker, foo);

                               else if(marker == 'v')
                                 fprintf(stderr, "LCSH not found|%c|[FORM] %s", marker, foo);

                               else if(marker == 'y')
                                 fprintf(stderr, "LCSH not found|%c|[CHRON] %s", marker, foo);

                               else
                                 fprintf(stderr, "LCSH not found|%c|[UNKN] %s", marker, foo);

                               fprintf(stderr, "\n");
                               fflush(stderr);
                           } /* fi */

                          addLCLine(foo, "", "NOT FOUND", 150);
                          if(strlen(first) == 0)
                            firstMapBad = TRUE;
                       } /* else */
                   } /* fi !isAQual */

                   free(foo);
               } /* fi len > 0 */

               marker = nextMarker;
            } /* while */
        } /* fi */

        else  /* No subparts to deal with */
        {
             strcpy(element, LClines[i]);
             len = (long)strlen(element);
             if(element[len - 1] == '.')
               element[len - 1] = '\0';

             foo = Strip_Str(element);
             mapPos = findMapping(foo);
             if(mapPos > -1)
             {
                 strcpy(tmpJ, LCMHs[mapPos].MH);
                 while(strlen(tmpJ) > 0)
                 {
                     strcpy(tmpK, "");
                     strcpy(tmpL, "");
                     sscanf(tmpJ, "%[^:]:%[^\n]", tmpK, tmpL);
                     strcpy(tmpJ, tmpL);

                     if(strlen(tmpK) > 0)
                     {
                         if(strcmp(tmpK, foo) == 0)
                           addLCLine(foo, "", foo, 150);

                         else
                         {
                            sprintf(tmpII, "%s [%s]\0", tmpK, foo);
                            addLCLine(foo, tmpII, tmpK, LCMHs[mapPos].level);
                         } /* else */
                     } /* fi tmpK */
                 } /* while MH */
             } /* fi */

             else if(strlen(foo) > 0)
             {
                 if((marker != 'd') && (marker != 'x') && (strstr(foo, "(DNLM)D") == NULL))
                 {
                     if(marker == 'a')
                       fprintf(stderr, "LCSH not found2|%c|[TOPIC] %s", marker, foo);

                     else if(marker == 'z')
                       fprintf(stderr, "LCSH not found2|%c|[GEO] %s", marker, foo);

                     else if(marker == 'v')
                       fprintf(stderr, "LCSH not found2|%c|[FORM] %s", marker, foo);

                     else if(marker == 'y')
                       fprintf(stderr, "LCSH not found2|%c|[CHRON] %s", marker, foo);

                     else
                       fprintf(stderr, "LCSH not found2|%c|[UNKN] %s", marker, foo);

                     fprintf(stderr, "\n");
                     fflush(stderr);
                 } /* fi */

                 addLCLine(foo, "", "NOT FOUND", 150);
             } /* else */

             free(foo);
        } /* else */
    } /* for */

    qsort(LClinesSorted, numSorted, MED_LINE + 1, charCompare);
    qsort(LCMapLinesSorted, numLCMapLines, MED_LINE + 1, charCompareII);
} /* processLClines */


int isAQual(char *term, char *first, char *firstN, char *firstO,
            int firstMapBad)
{
    int rtn, i, pos;
    long len, lenT;
    char tmpII[SMALL_LINE + 1];
    char tmpIII[SMALL_LINE + 1];
    char tmpA[TINY_LINE + 1], tmpB[TINY_LINE + 1], tmpC[TINY_LINE + 1];
    char tmpD[TINY_LINE + 1], tmpE[TINY_LINE + 1], tmpF[TINY_LINE + 1];
    char tmpJ[TINY_LINE + 1], tmpK[TINY_LINE + 1], tmpL[TINY_LINE + 1];

    rtn = FALSE;
    if(strlen(first) > 0)
    {
       pos = -1;
       for(i = 0; !rtn && (i < numQuals); i++)
       {
           if(strcasecmp(qualifiers[i], term) == 0)
           {
              pos = i;
              rtn = TRUE;
           } /* fi */
       } /* for */

       /* If we don't find it, let's look for all but the last character
          match so we pick up the typos like "classificatio".
       */

       if(!rtn)
       {
           len = (long)strlen(term);

           for(i = 0; !rtn && (i < numQuals); i++)
           {
               lenT = (long)strlen(qualifiers[i]);
               if(lenT == (len + 1))
               {
                  if(strncasecmp(qualifiers[i], term, len) == 0)
                  {
                     pos = i;
                     rtn = TRUE;
                  } /* fi */
               } /* fi lenT */
           } /* for */
       } /* fi */

       /* To compensate for the possibility of multiple terms in first and 
          firstN, we loop through using the ":" as a term separater.
       */

       strcpy(tmpA, first);
       strcpy(tmpD, firstO);
       strcpy(tmpJ, firstN);

       while(strlen(tmpA) > 0)
       {
          strcpy(tmpB, "");  /* first */
          strcpy(tmpC, "");
          sscanf(tmpA, "%[^:]:%[^\n]", tmpB, tmpC);
          strcpy(tmpA, tmpC);

          strcpy(tmpE, "");  /* firstO */
          strcpy(tmpF, "");
          sscanf(tmpD, "%[^:]:%[^\n]", tmpE, tmpF);
          strcpy(tmpD, tmpF);

          strcpy(tmpK, "");  /* firstN */
          strcpy(tmpL, "");
          sscanf(tmpJ, "%[^:]:%[^\n]", tmpK, tmpL);
          strcpy(tmpJ, tmpL);

          if((strlen(tmpK) > 0) && (strlen(tmpB) > 0) && (strlen(tmpE) > 0))
          {
             if(rtn && (pos > -1))
             {
                sprintf(tmpII, "%s/%s\0", tmpB, qualifiers[pos]);
                sprintf(tmpIII, "%s/%s\0", tmpK, qualifiers[pos]);

                checkMHSHCombos(firstMapBad, tmpK, qualifiers[pos], tmpE,
                                tmpII, tmpIII);
             } /* fi */

             /* Now search through the QualsII list to see if we need to add any
                additional entries.  Could have more then one entry, so search
                all. NOTE that this list does not contain any entries from
                above list.
             */

             for(i = 0; i < numLCSHs; i++)
             {
                if(strcmp(term, LCSHs[i].LCH) == 0)
                {
                    rtn = TRUE;
                    sprintf(tmpII, "%s/%s\0", tmpB, LCSHs[i].SH);
                    sprintf(tmpIII, "%s/%s\0", tmpK, LCSHs[i].SH);

                    checkMHSHCombos(firstMapBad, tmpK, LCSHs[i].SH, tmpE,
                                    tmpII, tmpIII);
                } /* fi */
            } /* for */
          } /* fi tmpK && tmpB */
       } /* while we have first terms */
    } /* fi first */

    return(rtn);
} /* isAQual */


void addLCLine(char *LCH, char *Repl, char *normMH, int level)
{
    int i, found;
    long len;
    char term[SMALL_LINE + 1];
    char tmp[SMALL_LINE + 1];
    char tmpII[SMALL_LINE + 1];
    char rest[SMALL_LINE + 1];
    char tmpMH[SMALL_LINE + 1];
    char tmpIIMH[SMALL_LINE + 1];
    char restMH[SMALL_LINE + 1];

    if(RTM_Debug)
    {
        fprintf(fout, "addLCLine - LCH: #%s#  Repl: #%s  normMH: #%s#\n",
                LCH, Repl, normMH);
        fflush(fout);
    } /* fi */

    if((strchr(Repl, ':') != NULL) || (strchr(normMH, ':') != NULL))
    {
        strcpy(tmp, Repl);
        strcpy(tmpMH, normMH);
        while(strlen(tmp) > 0)
        {
            strcpy(tmpII, "");
            strcpy(rest, "");
            sscanf(tmp, "%[^:]:%[^\n]", tmpII, rest);
            strcpy(tmp, rest);

            strcpy(tmpIIMH, "");
            strcpy(restMH, "");
            sscanf(tmpMH, "%[^:]:%[^\n]", tmpIIMH, restMH);
            strcpy(tmpMH, restMH);

            if((strlen(tmpII) > 0) && (strlen(tmpIIMH) > 0))
              addLCTerm(LCH, tmpII, tmpIIMH, level);

            else
            {
                fprintf(stderr,
                    "ERROR - Mis-Match LCH: Repl #%s#  normMH #%s#\n",
                    Repl, normMH);
                fflush(stderr);
            } /* else */
        } /* while */
    } /* fi */

    else
    {
       len = (long)strlen(Repl);
       if(len > 0)
         strcpy(term, Repl);
       else
         strcpy(term, LCH);

       addLCTerm(LCH, term, normMH, level);
    } /* else */
} /* addLCLine */


void addLCTerm(char *LCH, char *term, char *normMH, int level)
{
    int i, found;

    found = FALSE;
    for(i = 0; !found && (i < numSorted); i++)
    {
        if(strcmp(term, LClinesSorted[i]) == 0)
          found = TRUE;
    } /* for */

    if(!found)
    {
       strcpy(LClinesSorted[numSorted++], term);
       if(strcmp(normMH, "NOT FOUND") != 0)
       {
           sprintf(LCMapLinesSorted[numLCMapLines], "%s|%s|%d\0", LCH,
                   normMH, level);

           if(RTM_Debug)
           {
               fprintf(fout, "addLCTerm - Line: #%s#\n",
                       LCMapLinesSorted[numLCMapLines]);
               fflush(fout);
           } /* fi */
           numLCMapLines++;
       } /* fi */
    } /* fi */
} /* addLCLine */


/***************************************************************************
*
*  loadLCHtoMH --
*
*      This 
*
***************************************************************************/

void loadLCHtoMH()
{
    char line[MED_LINE + 1];
    char LCH[MED_LINE + 1];
    char MH[MED_LINE + 1];
    long len, lineCtr;
    int level, pos, posII, i, j;
    FILE *fp;

    for(i = 0; i < 127; i++)
    {
       for(j = 0; j < 127; j++)
         firstPos[i][j] = -1;
    } /* for */

    fp = fopen(LCSH2MSH_FILE, "r");

    lineCtr = 0;
    while(fgets(line, MED_LINE, fp) != (char *)NULL)
    {
        lineCtr++;
        strcpy(LCH, "");
        strcpy(MH, "");
        level = -1;
        sscanf(line, "%[^|]|%d|%[^|]", LCH, &level, MH);

        len = (long)strlen(LCH);
        if((len > 0) && (strlen(MH) > 0) && (level > -1))
        {
            if(numLCMHs < MAXLCHM)
            {
               pos = LCH[0];
               posII = LCH[1];
               if((pos > -1) && (posII > -1))
               {
                  if(firstPos[pos][posII] == -1)
                    firstPos[pos][posII] = numLCMHs;
               } /* fi */

               LCMHs[numLCMHs].LCH = (char *)strdup(LCH);
               LCMHs[numLCMHs].lenLCH = len;
               LCMHs[numLCMHs].MH = (char *)strdup(MH);
               LCMHs[numLCMHs].level = level;
               numLCMHs++;
            } /* fi */

            else
            {
                fprintf(stderr, "ERROR: Too many LCHs\n");
                fflush(stderr);
            } /* else */
        } /* fi */

        else
        {
            fprintf(stderr, "ERROR: Bad Line [%ld] - #%s#\n", lineCtr, line);
            fflush(stderr);
        } /* else */
    } /* while */

    fclose(fp);
} /* loadLCHtoMH */

/***************************************************************************
*
*  findMapping --
*
*      This 
*
***************************************************************************/

long findMapping(char *term)
{
    long rtn = -1;
    long j, pos, posII, startPos;
    int found, done, status;

    pos = term[0];
    posII = term[1];
    startPos = 0;
    if((pos > -1) && (posII > -1))
    {
       if(firstPos[pos][posII] > -1)
         startPos = firstPos[pos][posII];
    } /* fi */

    found = FALSE;
    done = FALSE;
    for(j = startPos; !done && !found && (j < numLCMHs); j++)
    {
        status = strcmp(term, LCMHs[j].LCH);
        if(status == 0)
        {
            found = TRUE;
            rtn = j;
        } /* fi */

        else if(status < 0)
           done = TRUE;
    } /* for */

    /* Look at mixed case equivalent if no direct match found */

    if(!found)
    {
       found = FALSE;
       for(j = 0; !found && (j < numLCMHs); j++)
       {
           status = strcasecmp(term, LCMHs[j].LCH);
           if(status == 0)
           {
               found = TRUE;
               rtn = j;
           } /* fi */
       } /* for */
    } /* fi */

    return(rtn);
} /* findMapping */

/***************************************************************************
*
*  checkMHSHCombos --
*
*      This routine validates the MH/SH combinations coming from our mapping
*   of LCSH to MeSH.
*
***************************************************************************/

void checkMHSHCombos(int firstMapBad, char *firstN, char *SH, char *firstO,
                     char *tmpII, char *tmpIII)
{
    int n, z, ok, found;
    long i, len;
    char tmp[MAXLINE + 1], tmp2[MAXLINE + 1], tmp3[MAXLINE + 1];
    char **rows = NULL;

    if(RTM_Debug)
    {
        fprintf(fout, "checkMHSH - (%d, %s, %s, %s, %s, %s)\n", firstMapBad,
                firstN, SH, firstO, tmpII, tmpIII);
        fflush(fout);
    } /* fi */

    if(firstMapBad)
      addLCLine(firstO, tmpII, "NOT FOUND", 150);

    else
    {
        /* Create a lowercase version of MH so we can search the validMHSH
           table and verify this combination.
        */

        strcpy(tmp, firstN);
        len = (long)strlen(tmp);
        for(i = 0; i < len; i++)
        {
            if(isupper(tmp[i]))
              tmp[i] = tolower(tmp[i]);
        } /* for */

        ok = FALSE;
        query_btree(VMHSH, tmp, &rows, &n);
        if(n > 0)
        {
           sprintf(tmp2, "|%s|\0", SH);
           if(strstr(rows[0], tmp2) != NULL)
             ok = TRUE;

           for(z = 0; z < n; z++)
              free(rows[z]);
        } /* fi */

        if(rows != NULL)
          free(rows);

        if(ok)
          addLCLine(firstO, tmpII, tmpIII, 150);

        else /* see if we have a legitimate replacement or not */
        {
            sprintf(tmp2, "%s/%s\0", firstN, SH);
            len = (long)strlen(tmp2);
            for(i = 0; i < len; i++)
            {
               if(isupper(tmp2[i]))
                 tmp2[i] = tolower(tmp2[i]);
            } /* for */

            ok = FALSE;
            query_btree(CATREPL, tmp2, &rows, &n);
            if(n > 0)
            {
               strcpy(tmp3, "");
               sscanf(rows[0], "%*[^|]|%[^|]", tmp3);
               if(strlen(tmp3) > 0)
                 ok = TRUE;
               for(z = 0; z < n; z++)
                  free(rows[z]);
               sprintf(tmp, "%s [%s]\0", tmpIII, tmp3);
               addLCLine(tmpII, tmp, tmp3, 150);
               addLCLine(tmp3, "", "NOT FOUND", 150);
            } /* fi */

            else
              addLCLine(firstO, tmpII, "NOT FOUND", 150);

            if(rows != NULL)
              free(rows);
        } /* else */
    } /* else */
} /* checkMHSHCombos */

/***************************************************************************
*
*  cleanReplList --
*
*      This 
*
***************************************************************************/

void cleanReplList()
{
    long i;

    for(i = 0; i < num_repls; i++)
    {
        if(repl_list[i]->search_for != NULL)
          free(repl_list[i]->search_for);

        if(repl_list[i]->replace_by != NULL)
          free(repl_list[i]->replace_by);
        free(repl_list[i]);
    } /* for */

    free(repl_list);
} /* cleanReplList */
