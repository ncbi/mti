#include "M_terms.h"

/* new functions */

void doMediumFiltering();
void Process_MediumFiltering();
int Heuristic2(int pos);
void Heuristic5();
int Heuristic6(int pos);
int Heuristic7(int pos);
void Heuristic8(int pos);
void Heuristic9(int pos);
int isExempt(int pos);
int check_ExcpA(int pos);
int check_ExcpB(int pos);
int check_ExcpC(int pos);
int check_ExcpD(int pos);
int check_ExcpE(int pos);
int check_Excp0();
int checkMMD01_D25();
int checkMMRCD01_D25();
int checkD01_D25_and_D26_D27_and_NothingElse(int pos);
int checkD01_D27_and_NothingElse(int pos);
int check_TrueExceptions(char *Heuristic, int pos, char *treecode);
int check_FalseExceptions(char *Heuristic, int pos, char *treecode);
void checkException1f(int pos);
int checkHeuristic5(int pos);
int CheckString(int pos, int flag);
int BCheckString(int pos, int flag);
int num_words(int pos);
int find_uniq(int unipos);
void doHMNM();

/* Extern definitions needed */

extern TMT *mt_table;
extern long mt_count;
extern int max, medFilterR_set, trigger_pos, trigger_code, trigger_mycode,
           RTM_Debug, MTI_AUTO, MTI_REVIEW;
extern float PW1, PW2, PW4;
extern char muid[SMALL_LINE + 1];
extern FILE *fout;

extern int MhShDups_PassI();
extern int isCT();

int MMRCD01_D25_flag = FALSE;
int flag_Excp0 = FALSE;
int run2, run4, run5, run6, run7, run8, run9, run10;

void doMediumFiltering()
{
   int i, j,k;

/* Partial ON  2,5,6,and 10
   run2 = TRUE;
   run4 = FALSE;
   run5 = TRUE;
   run6 = TRUE;
   run7 = FALSE;
   run8 = FALSE;
   run9 = FALSE;
   run10 = TRUE;
*/

/* FULL ON */
   run2 = TRUE;
   run4 = TRUE;
   run5 = TRUE;
   run6 = TRUE;
   run7 = TRUE;
   run8 = TRUE;
   run9 = TRUE;
   run10 = TRUE;
/* */

   /* Do a word count for each topN item */
 
   for(k = 0; k < max; k++)
      if(mt_table[k].safe)
         mt_table[k].num_words = num_words(k);

   /* Normal Exemption Heuristics */
 
   for(k = 0; k < max; k++)
      if(mt_table[k].safe)
         mt_table[k].isExempt = isExempt(k);
      else
         mt_table[k].isExempt = FALSE;

   /* New Exemption Check 10/11/01 - Do for TopN not already Exempt.

           1) If we have any item in topN that is in range of D1-D25 AND
              NOT in range D26-D27 AND Recommended by both MM AND RC proceed.

           2) If we have an item in the topN that is in the range of D01-D25
              AND in the range of D26-D27 AND not in any other categories
              outside of these Dnn categories AND is recommended by the RC
              path only, then proceed for each item fitting this criteria.

           3) For each item, compile a list of all descendant items in the
              topN  that are in the range of D01-D27 AND NOT IN ANY other
              categoryies including this item and find the highest scoring
              item in this pool - mark it as exempt.
   */

   if(checkMMRCD01_D25()) /* (1) */
   {
      for(k = 0; k < max; k++)
      {
         if(mt_table[k].safe && !mt_table[k].isExempt && 
             mt_table[k].paths[PRC] && !mt_table[k].paths[MMI] &&    /* (2) */
            checkD01_D25_and_D26_D27_and_NothingElse(k))
         {
            if(medFilterR_set)
            {
               fprintf(fout, "%s|_|   -- Exemption #1f term found - %s (%ld)\n",
                      muid, mt_table[k].mh, mt_table[k].score);
            } /* fi */

            checkException1f(k);  /* (3) */
         } /* fi */
      } /* for */
   } /* fi */

   /* Setup the Exception flags for further processing */

   /* Must be done first since others rely on it. */

   flag_Excp0 = check_Excp0();

   for(k = 0; k < max; k++)
   {
      if(!mt_table[k].isExempt)
      {
         mt_table[k].flag_ExcpA = check_ExcpA(k);
         mt_table[k].flag_ExcpB = check_ExcpB(k);
         mt_table[k].flag_ExcpC = check_ExcpC(k);
         mt_table[k].flag_ExcpD = check_ExcpD(k);
         mt_table[k].flag_ExcpE = check_ExcpE(k);
       } /* fi */
   } /* for */

   /* NOTES on EXCEPTIONS:

 strcheck) A more general germ can be a term of which the term to be acted
           upon is a sublist.(e.g., "Life" is more general than "Quality of
           Life" and "Quality of Life" is more specific than "Life").

        0) Treat Surgery in the TopN as being E04 top level term.

        A) When a term is in E03 (Anesthesia and Analgesia), consider the
           term to be more specific than the top-level term in E02
           (Therapeutics).

        B) When a term is in E04 (Surgical Procedures, Operative) and not 
           also in E01 (Diagnosis), consider the term to be more specific
           than the top-level term in E02 (Therapeutics).

        C) When a term is in both E01 (Diagnosis) and E04 (Surgical 
           Procedures, Operative), ignore any and all E04 tree numbers.

        D) When a term is in both E01 (Diagnosis) and E05 (Investigative
           Techniques), ignore any and all E05 tree numbers.

        E) When a term is in H01 (Biological Sciences) or H02 (Health
           Occupations), and also in some other subcategory, ignore any
           and all H01 and H02 tree numbers.
      */

      /* Semantic Type removal before anything else -- Heuristic #2 */

      if(run2)
      {
      for(k = 0; k < max; k++)
      { 
         if(!mt_table[k].forcePrint && !mt_table[k].isExempt)
         {
             if(medFilterR_set)
             {
                if(mt_table[k].num_STs > 0)
                {
                   fprintf(fout, "%s|_|mt_table[%2d]: %s [%s] (%d) -\n",
                           muid, k, 
                           mt_table[k].mh, mt_table[k].ST_info[0].rtm_ST,
                           mt_table[k].num_STs); fflush(fout);

                   for(j = 0; j < mt_table[k].num_STs; j++)
                      fprintf(fout, "%s|_|   -- %s\n", muid, 
                              mt_table[k].ST_info[j].MMI_trigger);
                } /* fi */

                else
                   fprintf(fout, "%s|_|mt_table[%2d]: %s [No ST] (%d) -\n",
                           muid, k, mt_table[k].mh, mt_table[k].num_STs);

                fflush(fout);
             } /* fi medFilterR_set */

             if(!mt_table[k].isExempt && mt_table[k].paths[MMI] && 
                   (mt_table[k].num_STs > 0))
             {
                if(!Heuristic2(k))
                {
                    if(medFilterR_set)
                      fprintf(fout, "%s|_|Removing %s -- Heuristic #2\n", 
                              muid, mt_table[k].mh);
                    mt_table[k].safe = FALSE;
                } /* fi */
             } /* fi */
         } /* fi !forcePrint */
      } /* for */
      } /* fi run2 */

      Process_MediumFiltering();

      /* Now process the last heuristic and see if we remove anymore */

      if(run10)
        doHMNM();  /* Heuristic #10 */
} /* doMediumFiltering */


void Process_MediumFiltering()
{
   int k;
   int dup;
   int foundMMD01_D25;

   /* Heuristic #1
         If the same term has been assigned by both MetaMap & Related 
         Citations, keep the term.

         NOTE: This heuristic is implemented in the isExempt routine.
   */


   /* Heuristic #4
         When a PubMed assigned term meets the following four criteria
         remove it:
             1) It is in subcategory range of D01 - D25
             2) It is also in subcategory range D26-D27
             3) It is NOT in any other subcategories
             4) MetaMap has not assigned anything in subcategory range D01-D25
    */

   if(run4)
   {
   foundMMD01_D25 = checkMMD01_D25();  /* #4 check */

   if(!foundMMD01_D25)
   {
      for(k = 0; k < max; k++)
      {
         if(!mt_table[k].forcePrint)
         {
           if(mt_table[k].safe && !mt_table[k].isExempt)
           {
              if(mt_table[k].paths[PRC] && !mt_table[k].paths[MMI])
              {
                 if(checkD01_D25_and_D26_D27_and_NothingElse(k))
                 {
                    if(medFilterR_set)
                      fprintf(fout, "%s|_|Removing %s -- Heuristic #4\n",
                              muid, mt_table[k].mh);
                    mt_table[k].safe = FALSE;
                 } /* fi !Heuristic */
              } /* fi */
           } /* fi safe */
         } /* fi !forcePrint */
      } /* for */
   } /* fi !foundMMD01-D25 */
   } /* fi run4 */

   /* Heuristic #5
         When, in any instance, MetaMap assigns a term from a first-level
         node, and PubMed assigns no term from that node, remove the term.
    */

   if(run5)
   {
      if(PW4 > 0.0)
        Heuristic5();
   } /* fi */


   /* Heuristic #6
         When in any instance, MetaMap assigns a more general term, and the
         more specific term is assigned by both, remove the more general term.
    */

   if(run6)
   {
   for(k = 0; k < max; k++)
   {
      if(mt_table[k].safe && !mt_table[k].isExempt && !mt_table[k].forcePrint)
         if(mt_table[k].paths[MMI] && !mt_table[k].paths[PRC])
            if(!Heuristic6(k))
            {
               if(medFilterR_set)
               {
                  if(trigger_code > -1)
                  {
                     fprintf(fout, "%s|_|Removing %s -- Heuristic #6 [%s]\n",
                           muid, mt_table[k].mh,
                           mt_table[k].treecodes[trigger_mycode]);
                     fprintf(fout, "%s|_|   -- triggered by %s - %s\n", muid,
                           mt_table[trigger_pos].mh, 
                           mt_table[trigger_pos].treecodes[trigger_code]);
                  } /* fi trigger_code */

                  else
                  {
                      fprintf(fout, "%s|_|Removing %s -- Heuristic #6 (strcheck)\n",
                             muid, mt_table[k].mh);
                      fprintf(fout, "%s|_|  -- trigger - %s\n", muid, 
                                 mt_table[trigger_pos].mh);
                  } /* else */
               } /* fi */
               mt_table[k].safe = FALSE;
            } /* fi !Heuristic */
   } /* for */
   } /* fi run6 */

   /* Heuristic 7
         When PubMed only assigns a term and MetaMap assigns no term which 
         is more general, remove the term.
         This rule must apply to every instance for a term to be removed.
         Note that there are exceptions to this rule -
                see notes in the code in the Heuristic7 function.
    */

   if(run7)
   {
   if(PW1 > 0.0)
   {
     for(k = 0; k < max; k++)
     {
        if(mt_table[k].safe && !mt_table[k].isExempt && !mt_table[k].forcePrint)
        {
           if(mt_table[k].paths[PRC])
           {
              if(!Heuristic7(k))
              {
                 if(medFilterR_set)
                    fprintf(fout, "%s|_|Removing %s -- Heuristic #7\n", 
                           muid, mt_table[k].mh);
                 mt_table[k].safe = FALSE;
              } /* fi !Heuristic */
           } /* fi */
        } /* fi safe */
     } /* for */
   } /* fi */
   } /* fi run7 */

   /* Heuristic #8
         When, in any instance, MetaMap and PubMed assign the same, more 
         general term, and PubMed assigns a more specific term, remove the 
         more specific term.
    */

   if(run8)
   {
      for(k = 0; k < max; k++)
      {
         if(mt_table[k].paths[PRC] && !mt_table[k].paths[MMI] && 
                !mt_table[k].isExempt && mt_table[k].safe &&
                !mt_table[k].forcePrint)
            Heuristic8(k);
      } /* for */
   } /* fi run8 */

   /* Heuristic #9
         When, in any instance, MetaMap only assigns a term and there
         is no RC term from the same category.
    */

   if(run9)
   {
   if(PW4 > 0.0)
   {
      for(k = 0; k < max; k++)
      {
         if(mt_table[k].paths[MMI] && !mt_table[k].paths[PRC] && 
                !mt_table[k].isExempt && mt_table[k].safe &&
                (mt_table[k].num_treecodes > 0) && !mt_table[k].forcePrint)
            Heuristic9(k);
      } /* for */
   } /* fi */
   } /* fi run9 */

   /* Now to setup which ones get printed or not */

   for(k = 0; k < max; k++)
     if(!mt_table[k].safe && !mt_table[k].forcePrint)
        mt_table[k].oktoprint = FALSE;
} /* Process_MediumFiltering */


int Heuristic2(int pos)
{
   int rtn = TRUE;
   int i, j;
   char check[MAXLINE + 1];
   char tmp[MAXLINE + 1], tmp2[MAXLINE + 1];

   rtn = FALSE;
   for(j = 0; !rtn && (j < mt_table[pos].num_STs); j++)
   {
      strcpy(check, "");
      sscanf(mt_table[pos].ST_info[j].MMI_trigger, "%*[^#]#%[^\n]", check);

      if(strstr(mt_table[pos].ST_info[0].rtm_ST, check) != NULL)
         rtn = TRUE;

      /* Else, let's see if we have a comma-separated list of STs */

      else if(strchr(check, ',') != NULL)
      {
         while(!rtn && (strlen(check) > 0))
         {
            strcpy(tmp, "");
            strcpy(tmp2, "");
            sscanf(check, "%[^,],%[^\n]", tmp, tmp2);
            strcpy(check, tmp2);

            if(strstr(mt_table[pos].ST_info[0].rtm_ST, tmp) != NULL)
              rtn = TRUE;
         } /* while */
      } /* else */
   } /* for */

   return(rtn);
} /* Heuristic2 */


void Heuristic5()
{
   int rtn = TRUE;
   int i, j, k;
   int found;
   long lowest_score = 100000;
   long pos_array[30];
   int num_pos = 0;
   int num_saved = 0;
   int num_removed = 0;
   int pos;

   /* Initialize pos_array */

   for(i = 0; i < 30; i++)
     pos_array[i] = -1;

   for(k = 0; k < max; k++)
   {
      if(mt_table[k].safe  && (mt_table[k].datatype != SH) &&
                (mt_table[k].datatype != CT))
      {
         if(mt_table[k].paths[MMI] && !mt_table[k].paths[PRC])
         {
             if(checkHeuristic5(k))
             {
                 if(mt_table[k].score < lowest_score)
                    lowest_score = mt_table[k].score;

                 if(medFilterR_set)
                 {
                    if(num_saved == 0)
                      fprintf(fout, "%s|_|Heuristic #5: Kept term collection\n",
                                     muid);

                    fprintf(fout, "%s|_|      -- %s  score: %ld\n", muid, 
                           mt_table[k].mh, mt_table[k].score);
                 } /* fi */

                 num_saved++;
             } /* fi */
             else
             {
                if(!mt_table[k].isExempt)
                  pos_array[num_pos++] = k;
             } /* else */
         } /* fi */
      } /* fi safe */
   } /* for */

   /* Now check on each of the saved potentially removable items */

   if(medFilterR_set)
   {
       if(num_saved > 0)
          fprintf(fout, "%s|_|Heuristic #5: Lowest score to beat: %ld\n", muid,
                 lowest_score);
       else
          fprintf(fout, "%s|_|Heuristic #5: Nothing in Kept Term Collection\n",
                  muid);
   } /* fi */

   if((num_saved > 0) && (lowest_score < 10000))
   {
      for(i = 0; i < num_pos; i++)
      {
         pos = pos_array[i];
         if(!mt_table[k].forcePrint && (mt_table[pos].score < lowest_score))
         {
             num_removed++;
             mt_table[pos].safe = FALSE;

             if(medFilterR_set)
               fprintf(fout, "%s|_|Removing %s -- Heuristic #5 - [%ld]\n", muid,
                  mt_table[pos].mh, mt_table[pos].score);
         } /* fi */
      } /* for */
   } /* fi */

   else
   {
      if((lowest_score <= 10000) && medFilterR_set)
      {
          fprintf(fout, "%s|_|Heuristic #5: Lowest Score (%ld) >= 10,000\n",
                  muid, lowest_score);
      } /* fi */      
   } /* else */

   if(medFilterR_set && (num_saved > 0))
   {
       fprintf(fout, 
      "%s|_|Heuristic #5: Reviewed %d non-member items and removed %d.\n", muid,
              num_pos, num_removed);
   } /* fi */
} /* Heuristic5 */


int Heuristic6(int pos)
{
   int rtn = TRUE;
   int i, j, k;
   int found;
   int cpos;

   if(mt_table[pos].num_treecodes == 0)
     rtn = TRUE;

   for(i = 0; rtn && (i < mt_table[pos].num_treecodes); i++)
   {
       found = FALSE;
       for(k = 0; !found && (k < max); k++)
       {
           /* Don't want to check ourselves  or use an item we have already
              removed for validation AND we are only checking the topN items -
              not the entire list.
           */

           if((k != pos) && mt_table[k].safe && 
              (mt_table[k].num_treecodes > 0) && mt_table[k].paths[MMI] &&
               mt_table[k].paths[PRC])
           {
              for(j = 0; !found && (j < mt_table[k].num_treecodes); j++)
              {
                 if(strstr(mt_table[k].treecodes[j], 
                     mt_table[pos].treecodes[i]) != NULL)
                 {
                     found = check_TrueExceptions("6", pos,
                                            mt_table[k].treecodes[j]);
                 } /* fi */

                 else
                 {
                    found = check_FalseExceptions("6", pos,
                                            mt_table[k].treecodes[j]);
                 } /* else */

                 if(found)
                 {
                    trigger_pos = k;
                    trigger_code = j;
                    trigger_mycode = i;
                 } /* fi */
              } /* for */
           } /* fi */
       } /* for */

       /* Because if we find the case, we want to remove the general term */

       if(found)
         rtn = FALSE;
   } /* for */

   if(rtn)  /* Then we need to check new heuristic */
   {
      cpos = BCheckString(pos, TRUE);
      if(cpos > -1)
      {
         rtn = FALSE;
         trigger_pos = cpos;
         trigger_code = -1;
      } /* fi */
   } /* fi */

   return(rtn);
} /* Heuristic6 */


int Heuristic7(int pos)
{
   int rtn = FALSE;
   int i, j, k;
   int found, cpos;

   if(mt_table[pos].num_treecodes == 0)
     rtn = FALSE;

   /* Check treecodes first */

   for(i = 0; !rtn && (i < mt_table[pos].num_treecodes); i++)
   {
       found = FALSE;
       for(k = 0; !found && (k < max); k++)
       {
           /* Don't want to check ourselves  or use an item we have already
              removed for validation AND we are only checking the topN items -
              not the entire list.
           */

           if((k != pos) && mt_table[k].safe && 
              (mt_table[k].num_treecodes > 0) && mt_table[k].paths[MMI])
           {
              for(j = 0; !found && (j < mt_table[k].num_treecodes); j++)
              {
                 if((strstr(mt_table[pos].treecodes[i], 
                            mt_table[k].treecodes[j]) != NULL))
                 {
                    found = check_TrueExceptions("7", pos,
                                            mt_table[k].treecodes[j]);

                    if(found && medFilterR_set)
                    {
                      fprintf(fout, "%s|_|Keeping %s [%s] -- Heuristic #7\n",
                         muid, mt_table[pos].mh, mt_table[pos].treecodes[i]); 

                      fprintf(fout, "%s|_|  -- trigger - %s [%s]\n",
                         muid, mt_table[k].mh, mt_table[k].treecodes[j]);
                    } /* fi */
                 } /* fi */

                 else
                 {
                    found = check_FalseExceptions("7", pos,
                                            mt_table[k].treecodes[j]);
                 } /* else */
              } /* for */
           } /* fi */
       } /* for */

       if(found)
         rtn = TRUE;
   } /* for */

   if(!rtn)  /* Then we need to check new heuristic */
   {
      cpos = CheckString(pos, FALSE);
      if(cpos > -1)
        rtn = TRUE;

      if(rtn && medFilterR_set)
      {
         fprintf(fout, "%s|_|Keeping %s -- Heuristic #7 (strcheck)\n",
             muid, mt_table[pos].mh);
         fprintf(fout, "%s|_|  -- trigger - %s\n", muid, mt_table[cpos].mh);
      } /* fi */
   } /* fi */

   return(rtn);
} /* Heuristic7 */


void Heuristic8(int pos)
{
   int i, j, k, found;
   int cpos;

   found = FALSE;
   for(i = 0; !found && (i < mt_table[pos].num_treecodes); i++)
   {
       for(k = 0; k < max; k++)
       {
           /* Don't want to check ourselves  or use an item we have already
              removed for validation AND we are only checking the topN items -
              not the entire list.
           */

           if((k != pos) && mt_table[k].safe && 
              (mt_table[k].num_treecodes > 0) && mt_table[k].paths[MMI] &&
               mt_table[k].paths[PRC])
           {
              for(j = 0; !found && (j < mt_table[k].num_treecodes); j++)
              {
                 if(strstr(mt_table[pos].treecodes[i], 
                            mt_table[k].treecodes[j]) != NULL)
                 {
                     found = check_TrueExceptions("8", pos,
                                            mt_table[k].treecodes[j]);
                 } /* fi */

                 else
                 {
                    found = check_FalseExceptions("8", pos,
                                            mt_table[k].treecodes[j]);
                 } /* else */

                 if(found)
                 {
                    if(medFilterR_set)
                    {
                       fprintf(fout, "%s|_|Removing %s [%s] -- Heuristic #8\n", 
                             muid, mt_table[pos].mh,
                             mt_table[pos].treecodes[i]);
                       fprintf(fout, "%s|_|   -- subset of %s - %s\n", muid,
                             mt_table[k].mh, mt_table[k].treecodes[j]);
                    } /* fi */

                    mt_table[pos].safe = FALSE;
                    found = TRUE;
                 } /* fi */
              } /* for */
           } /* fi */
       } /* for */
   } /* for */
} /* Heuristic8 */


void Heuristic9(int pos)
{
   int i, j, k, found;
   int cpos;
   int FOUND_H_OR_I = FALSE;

   found = FALSE;

   /* Check to see exceptions first */
   /* Changed "H" to "H01" 04/02/10 - JGM due to MeSH Changes
     2001 - "H" was Physical Sciences
        2002 - "Physical Sciences" became "Natural Sciences"
           2009 - "Natural Sciences" became "Natural Science Disciplines" (H01)
   */

   for(i = 0; !FOUND_H_OR_I && (i < mt_table[pos].num_treecodes); i++)
      if((strncmp(mt_table[pos].treecodes[i], "H01", 3) == 0) ||
         (mt_table[pos].treecodes[i][0] == 'I'))
   {
      FOUND_H_OR_I = TRUE;

      if(medFilterR_set)
      {
         fprintf(fout,
           "%s|_|Not Reviewing %s [%s] -- Heuristic #9 (H01 or I exception)\n", 
           muid, mt_table[pos].mh, mt_table[pos].treecodes[i]);
         fflush(fout);
      } /* fi */
   } /* for */

   if(!FOUND_H_OR_I)
   {
       for(i = 0; !found && (i < mt_table[pos].num_treecodes); i++)
       {
           for(k = 0; k < max; k++)
           {
               /* Don't want to check ourselves  or use an item we have already
                removed for validation AND we are only checking the topN items -
                not the entire list.
               */

               if((k != pos) && mt_table[k].safe && 
                  (mt_table[k].num_treecodes > 0) && mt_table[k].paths[PRC])
               {
                  for(j = 0; !found && (j < mt_table[k].num_treecodes); j++)
                  {
                     if(mt_table[pos].treecodes[i][0] ==
                             mt_table[k].treecodes[j][0])
                       found = TRUE;
                  } /* for */
               } /* fi */
           } /* for */
       } /* for */

       if(!found)
       {
          if(medFilterR_set)
          {
             fprintf(fout, "%s|_|Removing %s -- Heuristic #9\n", 
                     muid, mt_table[pos].mh);
             fflush(fout);
          } /* fi */

          mt_table[pos].safe = FALSE;
       } /* fi */
   } /* fi !found_H_OR_I */
} /* Heuristic9 */


int isExempt(int pos)
{
   int rtn = FALSE;
   int i, k, ok;
   char tmp[SMALL_LINE + 1];

   /* Make sure we exempt NMs */

   if(mt_table[pos].datatype == NM)
   {
      rtn = TRUE;
      if(medFilterR_set)
        fprintf(fout, "%s|_|Exempting %s -- Heuristic #1 - [NM]\n",
                muid, mt_table[pos].mh);
   }

   /* Make sure we exempt CTs recommended by MM only */

   if((mt_table[pos].datatype == CT) && mt_table[pos].paths[MMI] &&
            !mt_table[pos].paths[PRC])
   {
      rtn = TRUE;
      if(medFilterR_set)
        fprintf(fout, "%s|_|Exempting %s -- Heuristic #1 - [CT & MM]\n",
                muid, mt_table[pos].mh);
   } /* fi */

   /* Keep items recommended by both paths. */

   else if(mt_table[pos].paths[MMI] && mt_table[pos].paths[PRC])
   {
      rtn = TRUE;
      if(medFilterR_set)
        fprintf(fout, "%s|_|Exempting %s -- Heuristic #1 - [MM;RC]\n",
                muid, mt_table[pos].mh);
   } /* else fi */

   /* We don't want to remove Subheadings from the list. */

   else if(mt_table[pos].datatype == SH)
   {
      rtn = TRUE;
      if(medFilterR_set)
        fprintf(fout, "%s|_|Exempting %s -- Heuristic #1 - [SH]\n",
                muid, mt_table[pos].mh);
   } /* else fi */

   /* Keep items recommended by both paths for 9-10 (93%).   */
   /* Keep items recommended by both paths for 8-10 (88.6%). */
   /* Keep items recommended by both paths for 7-10 (82.9%). */

   else if(mt_table[pos].num_RCs > 6)
   {
       ok = TRUE;

       if(MTI_AUTO)
       {
           if(isCT(mt_table[pos].dui))
             ok = FALSE;
       } /* fi */

       if(ok)
       {
           mt_table[pos].forcePrint = TRUE;
           if(strlen(mt_table[pos].trigger) > 0)
             strcat(mt_table[pos].trigger, ";");
           sprintf(tmp, "Heuristic #N: %ld\0", mt_table[pos].num_RCs);
           strcat(mt_table[pos].trigger, tmp);
           rtn = TRUE;
           if(medFilterR_set)
             fprintf(fout, "%s|_|Exempting %s -- Heuristic #N - num_RCs: %ld\n",
                     muid, mt_table[pos].mh, mt_table[pos].num_RCs);
       } /* fi */

       else if(RTM_Debug)
       {
           fprintf(fout,
                   "CHJ Not Exempting|%s|_|%s -- Heuristic #N - num_RCs: %ld\n",
                   muid, mt_table[pos].mh, mt_table[pos].num_RCs);
           fflush(fout);
       } /* else fi */
   } /* else fi */

   /* Make sure we keep items from the Z tree where MetaMap selected them. */

   else
   {
       for (i = 0; !rtn && (i < mt_table[pos].num_treecodes); i++) 
       {
          if((mt_table[pos].treecodes[i][0] == 'Z') && mt_table[pos].paths[MMI])
          {
              rtn = TRUE;
              if(medFilterR_set)
                fprintf(fout, "%s|_|Exempting %s -- Heuristic #1 - [MM - Z01]\n",
                       muid, mt_table[pos].mh);
          } /* fi */
       } /* for */
   } /* else */

   /* See if the MH is a substitute for a SH either in the list or from
      Susanne's supplemental list of substitutes.
   */

   if(!rtn && (mt_table[pos].datatype == MH) && mt_table[pos].paths[MMI] &&
            !mt_table[pos].paths[PRC])
   {
      if(MhShDups_PassI(pos))
      {
         rtn = TRUE;
         if(medFilterR_set)
           fprintf(fout, "%s|_|Exempting %s -- Heuristic #1 - [SH Substitution]\n",
                  muid, mt_table[pos].mh);
      } /* fi */
   } /* fi */

   return(rtn);
} /* isExempt */


int check_ExcpA(int pos)
{
   int i;
   int found = FALSE;

   for(i = 0; !found && (i < mt_table[pos].num_treecodes); i++)
   {
       if(strncmp(mt_table[pos].treecodes[i], "E03", 3) == 0)
         found = TRUE;
   } /* for */

  return(found);
} /* check_ExcpA */


int check_ExcpB(int pos)
{
   int i;
   int found_E01 = FALSE;
   int found_E04 = FALSE;

   for(i = 0; i < mt_table[pos].num_treecodes; i++)
   {
       if(strncmp(mt_table[pos].treecodes[i], "E01", 3) == 0)
         found_E01 = TRUE;

       else if(strncmp(mt_table[pos].treecodes[i], "E04", 3) == 0)
         found_E04 = TRUE;
   } /* for */

   if(found_E04 && !found_E01)
     return(TRUE);
   else
     return(FALSE);
} /* check_ExcpB */


int check_ExcpC(int pos)
{
   int i;
   int found_E01 = FALSE;
   int found_E04 = FALSE;

   for(i = 0; i < mt_table[pos].num_treecodes; i++)
   {
       if(strncmp(mt_table[pos].treecodes[i], "E01", 3) == 0)
         found_E01 = TRUE;

       else if(strncmp(mt_table[pos].treecodes[i], "E04", 3) == 0)
         found_E04 = TRUE;
   } /* for */

   if(found_E04 && found_E01)
     return(TRUE);
   else
     return(FALSE);
} /* check_ExcpC */


int check_ExcpD(int pos)
{
   int i;
   int rtn = FALSE;
   int found_E01 = FALSE;
   int found_E05 = FALSE;

   for(i = 0; !rtn && (i < mt_table[pos].num_treecodes); i++)
   {
       if(strncmp(mt_table[pos].treecodes[i], "E01", 3) == 0)
         found_E01 = TRUE;

       else if(strncmp(mt_table[pos].treecodes[i], "E05", 3) == 0)
         found_E05 = TRUE;

       if(found_E01 && found_E05)
         rtn = TRUE;
   } /* for */

   return(rtn);
} /* check_ExcpD */


int check_ExcpE(int pos)
{
   int i;
   int rtn = FALSE;
   int found_H01 = FALSE;
   int found_H02 = FALSE;
   int found_Other = FALSE;

   for(i = 0; !rtn && (i < mt_table[pos].num_treecodes); i++)
   {
       if(strncmp(mt_table[pos].treecodes[i], "H01", 3) == 0)
         found_H01 = TRUE;

       else if(strncmp(mt_table[pos].treecodes[i], "H02", 3) == 0)
         found_H02 = TRUE;

       else
         found_Other = TRUE;

       if((found_H01 || found_H02) && found_Other)
         rtn = TRUE;
   } /* for */

   return(rtn);
} /* check_ExcpE */


int check_Excp0()
{
   int k;
   int found = FALSE;

   for(k = 0; !found && (k < max); k++)
   {
      if(mt_table[k].safe)
      {
         if(strcmp(mt_table[k].mh, "Surgery") == 0)
         {
            found = TRUE;
            mt_table[k].num_treecodes++;
            mt_table[k].treecodes = (char **)realloc(mt_table[k].treecodes,
                  (sizeof(char *) * mt_table[k].num_treecodes) + 1);
            mt_table[k].treecodes[mt_table[k].num_treecodes - 1] = 
                           (char *)malloc(strlen("E04") + 1);
            strcpy(mt_table[k].treecodes[mt_table[k].num_treecodes - 1], "E04");

           if(medFilterR_set)
              fprintf(fout, "%s|_|Surgery/E04 Exemption triggering agent found!\n",
                      muid);
         } /* fi */
      } /* fi */
   } /* for */

   return(found);
} /* check_Excp0 */


int checkMMD01_D25()  /* Recommended by MM and in D1-D25 */
{
   int j, k;
   int found;
   int rtn = FALSE;

   found = FALSE;
   for(k = 0; !found && (k < max); k++)
   {
       if(mt_table[k].safe && mt_table[k].paths[MMI])
       {
           for(j = 0; !found && (j < mt_table[k].num_treecodes); j++)
           {
             /* We are looking for D01 - D25 -- so if it starts with a "D"
                and isn't within D26 or D27, we're all set.
             */

              if(mt_table[k].treecodes[j][0] == 'D')
              {
                 if((strncmp(mt_table[k].treecodes[j], "D26", 3) != 0) &&
                    (strncmp(mt_table[k].treecodes[j], "D27", 3) != 0))
                 {
                    found = TRUE;
                    rtn = TRUE;
                 } /* fi */
              } /* fi */
           } /* for */
       } /* fi */
   } /* for */

   return(rtn);
} /* checkMMD01_D25 */


int checkMMRCD01_D25()
{  /* Recommended by MM AND RC AND in D1-D25 AND !D26-D27 */
   int j, k;
   int rtn;
   int foundD01_D25 = FALSE;
   int foundD26_D27 = FALSE;

   rtn = FALSE;
   for(k = 0; !rtn && (k < max); k++)
   {
       foundD01_D25 = FALSE;
       foundD26_D27 = FALSE;
       if(mt_table[k].safe && mt_table[k].paths[MMI] && mt_table[k].paths[PRC])
       {
           for(j = 0; j < mt_table[k].num_treecodes; j++)
           {
              if(mt_table[k].treecodes[j][0] == 'D')
              {
                 if((strncmp(mt_table[k].treecodes[j], "D26", 3) == 0) ||
                         (strncmp(mt_table[k].treecodes[j], "D27", 3) == 0))
                   foundD26_D27 = TRUE;
                 else
                   foundD01_D25 = TRUE;
              } /* fi */
           } /* for */
       } /* fi */

       if(foundD01_D25 && !foundD26_D27)  /* We have a match */
       {
          rtn = TRUE;
          if(medFilterR_set)
            fprintf(fout, "%s|_|Exemption #1f triggering agent found - [%s]\n", muid,
                    mt_table[k].mh);
       } /* fi */
   } /* for */

   return(rtn);
} /* checkMMRCD01_D25 */


int checkD01_D25_and_D26_D27_and_NothingElse(int pos)
{
   int done;
   int i;
   int foundNonD = FALSE;
   int foundD01_D25 = FALSE;
   int foundD26_D27 = FALSE;
   int rtn = FALSE;

   done = FALSE;

   if(mt_table[pos].num_treecodes > 0) 
   {
      for(i = 0; !done && (i < mt_table[pos].num_treecodes); i++) 
      {
          if(mt_table[pos].treecodes[i][0] == 'D')
          {
             if((strncmp(mt_table[pos].treecodes[i], "D26", 3) == 0) ||
                         (strncmp(mt_table[pos].treecodes[i], "D27", 3) == 0))
               foundD26_D27 = TRUE;
             else
               foundD01_D25 = TRUE;
          } /* fi */
          else
          {
              foundNonD = TRUE;
              done = TRUE;
          } /* else */
      } /* for */
   } /* fi */

   if(!foundNonD && foundD01_D25 && foundD26_D27)
     rtn = TRUE;

   return(rtn);
} /* checkD01_D25_and_D26_D27_and_NothingElse */


int checkD01_D27_and_NothingElse(int pos)
{
   int done;
   int i;
   int foundNonD = FALSE;
   int foundD01_D27 = FALSE;
   int rtn = FALSE;

   done = FALSE;

   if(mt_table[pos].num_treecodes > 0) 
   {
      for(i = 0; !done && (i < mt_table[pos].num_treecodes); i++) 
      {
          if(mt_table[pos].treecodes[i][0] == 'D')
            foundD01_D27 = TRUE;
          else
          {
             foundNonD = TRUE;
             done = TRUE;
          } /* else */
      } /* for */
   } /* fi */

   if(!foundNonD && foundD01_D27)
     rtn = TRUE;

   return(rtn);
} /* checkD01_D27_and_NothingElse */


int check_TrueExceptions(char *Heuristic, int pos, char *treecode)
{
   int rtn = TRUE;

   if(mt_table[pos].flag_ExcpC)
   {
      if(strncmp(treecode, "E04", 3) == 0)
      {
         rtn = FALSE;
         if(medFilterR_set)
         {
            fprintf(fout, "%s|_|Ignoring Treecode Result %s --",
                    muid, mt_table[pos].mh);

            fprintf(fout, " #%s: Exception C - [%s]\n", Heuristic, treecode);
         } /* fi */
      } /* fi */
   } /* fi */

   else if(mt_table[pos].flag_ExcpD)
   {
      if(strncmp(treecode, "E05", 3) == 0)
      {
         rtn = FALSE;
         if(medFilterR_set)
         {
            fprintf(fout, "%s|_|Ignoring Treecode Result %s --",
                   muid, mt_table[pos].mh);

            fprintf(fout, " #%s: Exception D - [%s]\n", Heuristic, treecode);
         } /* fi */
      } /* fi */
   } /* else fi */

   else if(mt_table[pos].flag_ExcpE)
   {
      if((strncmp(treecode, "H01", 3) == 0) ||
         (strncmp(treecode, "H02", 3) == 0))
      {
         rtn = FALSE;
         if(medFilterR_set)
         {
             fprintf(fout, "%s|_|Ignoring Treecode Result %s --",
                    muid, mt_table[pos].mh);

             fprintf(fout, " #%s: Exception E - [%s]\n", Heuristic, treecode);
         } /* fi */
      } /* fi */
   } /* else fi */

   return(rtn);
} /* check_TrueExceptions */


int check_FalseExceptions(char *Heuristic, int pos, char *treecode)
{
   int rtn = FALSE;

   if(mt_table[pos].flag_ExcpA || mt_table[pos].flag_ExcpB)
   {
       if(strcmp(treecode, "E02") == 0)
       {
           rtn = TRUE;

           if(medFilterR_set)
           {
              if(mt_table[pos].flag_ExcpA)
                fprintf(fout, "%s|_|Keeping %s -- #%s: Exception A - [E02]\n",
                      muid, mt_table[pos].mh, Heuristic);

              else if(mt_table[pos].flag_ExcpB)
                fprintf(fout, "%s|_|Keeping %s -- #%s: Exception B - [E02]\n",
                      muid, mt_table[pos].mh, Heuristic);
           } /* fi */
       } /* fi */
   } /* fi */

   return(rtn);
} /* check_FalseExceptions */


void checkException1f(int pos)
{
   int i, j, k;
   int high_pos;
   long high_score;

   high_pos = pos;
   high_score = mt_table[pos].score;

   for(i = 0; i < mt_table[pos].num_treecodes; i++) 
   {
       for(k = 0; k < max; k++)
       {
          if((k != pos) && checkD01_D27_and_NothingElse(k))  /* (4) */
          {
             for(j = 0; j < mt_table[k].num_treecodes; j++)
             {
                if(strstr(mt_table[k].treecodes[j], 
                        mt_table[pos].treecodes[i]) != NULL)
                {
                   if(medFilterR_set)
                      fprintf(fout,
                       "%s|_|   -- Comparing with descendant: %s (%ld)\n",
                       muid, mt_table[k].mh, mt_table[k].score);

                   if(mt_table[k].score > high_score)
                   {
                      high_pos = k;
                      high_score = mt_table[k].score;
                   } /* fi */
                } /* fi */
             } /* for */
          } /* fi */
       } /* for */
   } /* for */

   mt_table[high_pos].isExempt = TRUE;
   mt_table[high_pos].flag_H1f = TRUE;
   if(medFilterR_set)
      fprintf(fout, "%s|_|Exempting %s -- Heuristic #1f\n", muid,
             mt_table[high_pos].mh);
} /* checkException1f */


int checkHeuristic5(int pos)
{
   int i, j, k;
   int rtn = FALSE;
   int found = FALSE;
   int cpos;

   for(i = 0; !found && (i < mt_table[pos].num_treecodes); i++) 
   {
       for(k = 0; k < max; k++)
       {
          if(k != pos) /* Don't want to check ourselves */
          {
             for(j = 0; !found && (j < mt_table[k].num_treecodes); j++)
             {
                /* First check more specific either path */

                if(strstr(mt_table[k].treecodes[j], 
                        mt_table[pos].treecodes[i]) != NULL)
                {
                   found = check_TrueExceptions("5", pos,
                                            mt_table[k].treecodes[j]);
                   rtn = found;
                } /* fi */

                /* Next check more general AND MMI path */

                if(!found && mt_table[k].paths[MMI] &&
                      (strstr(mt_table[pos].treecodes[i], 
                              mt_table[k].treecodes[j]) != NULL))
                {
                   found = TRUE;
                   rtn = TRUE;
                } /* fi */

                if(!found)
                {
                   found = check_FalseExceptions("5", pos,
                                         mt_table[k].treecodes[j]);
                   rtn = found;
                } /* fi */
             } /* for */
          } /* fi */
       } /* for */
   } /* for */


   if(!rtn)  /* Then we need to check new heuristic */
   {
      cpos = CheckString(pos, FALSE);
      if(cpos > -1)
        rtn = TRUE;

      if(rtn && medFilterR_set)
      {
         fprintf(fout, "%s|_|Adding %s to Kept Term List -- Heuristic #5 (strcheck)\n",
             muid, mt_table[pos].mh);
         fprintf(fout, "%s|_|  -- trigger - %s\n", muid, mt_table[cpos].mh);
      } /* fi */
   } /* fi */


   if(!rtn)  /* Then we need to check OR RC heuristic */
   {
      cpos = BCheckString(pos, FALSE);
      if(cpos > -1)
        rtn = TRUE;

      if(rtn && medFilterR_set)
      {
         fprintf(fout, "%s|_|Adding %s to Kept Term List -- Heuristic #5 (strcheckB)\n",
             muid, mt_table[pos].mh);
         fprintf(fout, "%s|_|  -- trigger - %s\n", muid, mt_table[cpos].mh);
      } /* fi */
   } /* fi */

   return(rtn);
} /* checkHeuristic5 */


int CheckString(int pos, int flag)
{
   int rtn = -1;
   int k;
   int found, check;
   int len, diff;
   int lent = (int)strlen(mt_table[pos].mh);
   char *strC;
   char *tmp;
   char *tmp3 = (char *)malloc(lent + 1);

   found = FALSE;
   strcpy(tmp3, "");
   strcpy(tmp3, mt_table[pos].mh);

   for(k = 0; !found && (k < max); k++)
   {
      if(mt_table[pos].num_words > mt_table[k].num_words)
        diff = mt_table[pos].num_words - mt_table[k].num_words;
      else
        diff = mt_table[k].num_words - mt_table[pos].num_words;

      if(flag)  /* MMI AND RC */
        check = mt_table[k].paths[MMI] && mt_table[k].paths[PRC];
      else  /* MMI Only */
        check = mt_table[k].paths[MMI];

      if((pos != k) && mt_table[k].safe && check && (diff <= 4))
      {
         len = (int)strlen(mt_table[k].mh);
         strC = (char *)malloc(len + 25);
         strcpy(strC, "");

         sprintf(strC, " %s \0", mt_table[k].mh);
         if(strstr(tmp3, strC) != NULL)
            found = TRUE;

         if(!found)
         {
            sprintf(strC, " %s\0", mt_table[k].mh);
            tmp = strstr(tmp3, strC);
            if(tmp != NULL)
            {
               if(((tmp - tmp3) + len + 1) >= strlen(tmp3))
               found = TRUE;
             } /* fi */
         } /* fi */

         if(!found)
         {
            sprintf(strC, " %s)\0", mt_table[k].mh);
            tmp = strstr(tmp3, strC);
            if(tmp != NULL)
            {
               if(((tmp - tmp3) + len + 1) >= strlen(tmp3))
               found = TRUE;
             } /* fi */
         } /* fi */

         if(!found)
         {
            sprintf(strC, "-%s \0", mt_table[k].mh);
            if(strstr(tmp3, strC) != NULL)
               found = TRUE;
         } /* fi */

         if(!found)
         {
            sprintf(strC, "(%s \0", mt_table[k].mh);
            if(strstr(tmp3, strC) != NULL)
               found = TRUE;
         } /* fi */

         if(!found)
         {
            sprintf(strC, " %s-\0", mt_table[k].mh);
            if(strstr(tmp3, strC) != NULL)
               found = TRUE;
         } /* fi */

         if(!found)
         {
            sprintf(strC, "-%s\0", mt_table[k].mh);
            tmp = strstr(tmp3, strC);
            if(tmp != NULL)
            {
               if(((tmp - tmp3) + len + 1) >= strlen(tmp3))
               found = TRUE;
             } /* fi */
         } /* fi */

         if(!found)
         {
            sprintf(strC, "%s,\0", mt_table[k].mh);
            if(strstr(tmp3, strC) != NULL)
               found = TRUE;
         } /* fi */

         if(!found)
         {
            sprintf(strC, "%s \0", mt_table[k].mh);
            tmp = strstr(tmp3, strC);
            if((tmp != NULL) && ((tmp - tmp3) == 0))
               found = TRUE;
         } /* fi */

         if(!found)
         {
            sprintf(strC, "%s-\0", mt_table[k].mh);
            tmp = strstr(tmp3, strC);
            if((tmp != NULL) && ((tmp - tmp3) == 0))
               found = TRUE;
         } /* fi */

         if(found)
           rtn = k;

         free(strC);
      } /* fi */
   } /* for */

   free(tmp3);
   return(rtn);
} /* CheckString */


int BCheckString(int pos, int flag)
{
   int rtn = -1;
   int k, found, len, check, diff;
   char *strB;
   char *tmp;
   char *tmp3;

   found = FALSE;
   len = (int)strlen(mt_table[pos].mh);
   strB = (char *)malloc(len + 25);

   for(k = 0; !found && (k < max); k++)
   {
      if(mt_table[pos].num_words > mt_table[k].num_words)
        diff = mt_table[pos].num_words - mt_table[k].num_words;
      else
        diff = mt_table[k].num_words - mt_table[pos].num_words;

      if(flag)  /* MMI AND RC */
        check = mt_table[k].paths[MMI] && mt_table[k].paths[PRC];
      else  /* REL */
        check = mt_table[k].paths[PRC];

      strcpy(strB, "");
      if((pos != k) && mt_table[k].safe && check && (diff <= 4))
      {
         sprintf(strB, " %s \0", mt_table[pos].mh);
         if(strstr(mt_table[k].mh, strB) != NULL)
            found = TRUE;

         if(!found)
         {
            sprintf(strB, "-%s \0", mt_table[pos].mh);
            if(strstr(mt_table[k].mh, strB) != NULL)
               found = TRUE;
         } /* fi */

         if(!found)
         {
            sprintf(strB, "(%s \0", mt_table[pos].mh);
            if(strstr(mt_table[k].mh, strB) != NULL)
               found = TRUE;
         } /* fi */

         if(!found)
         {
            sprintf(strB, " %s-\0", mt_table[pos].mh);
            if(strstr(mt_table[k].mh, strB) != NULL)
               found = TRUE;
         } /* fi */

         if(!found)
         {
            sprintf(strB, " %s)\0", mt_table[pos].mh);
            if(strstr(mt_table[k].mh, strB) != NULL)
               found = TRUE;
         } /* fi */

         if(!found)
         {
            tmp3 = (char *)malloc(strlen(mt_table[k].mh) + 1);
            strcpy(tmp3, mt_table[k].mh);
            sprintf(strB, " %s\0", mt_table[pos].mh);
            tmp = strstr(tmp3, strB);
            if(tmp != NULL)
            {
               if(((tmp - tmp3) + len + 1) >= strlen(tmp3))
               found = TRUE;
            } /* fi */
            free(tmp3);
         } /* fi */

         if(!found)
         {
            tmp3 = (char *)malloc(strlen(mt_table[k].mh) + 1);
            strcpy(tmp3, mt_table[k].mh);
            sprintf(strB, "-%s\0", mt_table[pos].mh);
            tmp = strstr(tmp3, strB);
            if(tmp != NULL)
            {
               if(((tmp - tmp3) + len + 1) >= strlen(tmp3))
               found = TRUE;
            } /* fi */
            free(tmp3);
         } /* fi */

         if(!found)
         {
            sprintf(strB, " %s\0", mt_table[pos].mh);
            if(strstr(mt_table[k].mh, strB) != NULL)
               found = TRUE;
         } /* fi */

         if(!found)
         {
            sprintf(strB, "%s,\0", mt_table[pos].mh);
            if(strstr(mt_table[k].mh, strB) != NULL)
               found = TRUE;
         } /* fi */

         if(!found)
         {
            sprintf(strB, "%s \0", mt_table[pos].mh);
            if(strstr(mt_table[k].mh, strB) != NULL)
               found = TRUE;
         } /* fi */

         if(!found)
         {
            tmp3 = (char *)malloc(strlen(mt_table[k].mh) + 1);
            strcpy(tmp3, mt_table[k].mh);
            sprintf(strB, "%s \0", mt_table[pos].mh);
            tmp = strstr(tmp3, strB);
            if((tmp != NULL) && ((tmp - tmp3) == 0))
               found = TRUE;
            free(tmp3);
         } /* fi */

         if(!found)
         {
            tmp3 = (char *)malloc(strlen(mt_table[k].mh) + 1);
            strcpy(tmp3, mt_table[k].mh);
            sprintf(strB, "%s-\0", mt_table[pos].mh);
            tmp = strstr(tmp3, strB);
            if((tmp != NULL) && ((tmp - tmp3) == 0))
               found = TRUE;
            free(tmp3);
         } /* fi */

         if(found)
           rtn = k;
      } /* fi */
   } /* for */

   free(strB);
   return(rtn);
} /* BCheckString */


int num_words(int pos)
{
   int rtn = 0;
   int i;
   int len = (int)strlen(mt_table[pos].mh);
   
   for(i = 0; i < len; i++)
     if((mt_table[pos].mh[i] == ' ') || (mt_table[pos].mh[i] == '-'))
       rtn++;

   rtn++;  /* End of string */

   return(rtn);
} /* num_words */


int find_uniq(int unipos)
{
   int rtn = -1;
   int found;
   long i;

   found = FALSE;
   for(i = 0; !found && (i < mt_count); i++)
   {
      if(mt_table[i].uniq_ID == unipos)
      {
         found = TRUE;
         rtn = i;
      } /* fi */
   } /* for */

   return(rtn);
} /* find_uniq */


void doHMNM()
{
   int j, k, found, pos;

   for(k = 0; k < max; k++)
   {
      if(mt_table[k].safe && !mt_table[k].forcePrint &&
         (mt_table[k].datatype == NM))
      {
         found = FALSE;
         for(j = 0; !found &&(j < mt_table[k].num_HMs); j++)
         {
             pos = find_uniq(mt_table[k].HMs[j]);

             if((pos > -1) && (pos < max) && mt_table[pos].safe)
             {
                found = TRUE;
                if(medFilterR_set)
                {
                    fprintf(fout, "%s|_|Keeping %s -- Heuristic #10\n",
                           muid, mt_table[k].mh);
                    fprintf(fout, "%s|_|  -- trigger - %s\n", muid, mt_table[pos].mh);
                } /* fi */
             } /* fi */
         } /* for */

         if(!found)
         {
            mt_table[k].safe = FALSE;
            mt_table[k].oktoprint = FALSE;
            if(medFilterR_set)
               fprintf(fout, "%s|_|Removing %s -- Heuristic #10\n", 
                             muid, mt_table[k].mh);
         } /* fi */
      } /* fi */
   } /* for */
} /* doHMNM */
