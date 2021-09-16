#include <ctype.h>
#include "M_terms.h"

void find_relations();
void process_rel();
void process_cot();
void compute_score();
int query_btree();

extern TMT *mt_table;
extern long mt_count;
extern FILE *fout;
 
extern int COT_FACTOR;
extern int TREE_FACTOR;
extern int TITLE_FACTOR;
extern int RTM_Debug;

int par=0, chd=0, sib=0, rb=0, rn=0, ro=0;

void find_relations()
{
   int z;
   long i, j, k;
   int n, cot = 0, sum_cot = 0;
   char line1[80];
   char term[80];
   char **rows = NULL;
   int t, l;
   char relation[20];
   float freq = 0;
   int ret_code;
   int validHM;

   for(i = 0; i < (mt_count - 1); i++)
   {
      for(k = (i + 1); k < mt_count; k++)
      {
         sprintf(line1, "%s|%s\0", mt_table[i].dui, mt_table[k].dui);
         query_btree(COT_DUI, line1, &rows, &t);
         freq = 0;

         if(FALSE)
         {
             fprintf(fout, "find_relations - #%s|%s# <-> #%s|%s# -(%d)- Line1: #%s#\n", 
                     mt_table[i].mh, mt_table[i].dui, mt_table[k].mh, mt_table[k].dui, t, line1);
             fflush(fout);
         } /* fi */

         if((rows != NULL) && (t > 0))
         {
            freq = -1.0;
            sscanf(rows[0], "%*[^|]|%*[^|]|%f", &freq);

            if(FALSE)
            {
                fprintf(fout, "find_relations2 - #%s#|#%s#|%f|%s|\n", 
                        mt_table[i].mh, mt_table[k].mh, freq, rows[0]);
                fflush(fout);
            } /* fi */

            if(freq > 0.0)
              process_cot(i, k, freq);
         } /* fi */

         /* Free up the rows array since we are done with it for COT query */

         for(z = 0; z < t; z++)
         {
            if(rows[z] != NULL)
              free(rows[z]);
         } /* for */

         if(rows != NULL)
           free(rows);
         rows = NULL;

         t = 0;
         freq = 0;

         /*************** Possible relationships from the REL table:
            RB  has a broader relationship
            RN  has a narrower relationship
            RO  has relationship other than synonymous, narrower, or broader
            RL  the relationship is similar or "alike"
            PAR has parent relationship in a Metathesaurus source vocabulary
            CHD has child relationship in a Metathesaurus source vocabulary
            SIB has sibling relationship in a Metathesaurus source vocabulary
            AQ  is an allowed qualifier for the first concept in a Metathesaurus
                source vocabulary.
         ****************/

         query_btree(REL_DUI, line1, &rows, &t);

         if((rows != NULL) && (t > 0))
         {
            for(l = 0; l < t; l++) 
            {
               if(rows[l] != NULL)
               {
                   strcpy(relation, "");
                   sscanf(rows[l], "%*[^|]|%*[^|]|%[^|]", relation);

                   if(FALSE)
                   {
                       fprintf(fout, "find_relations - REL (%s): #%s#\n",  relation, rows[l]);
                       fflush(fout);
                   } /* fi */

                   if((strcmp(relation, "PAR") == 0) && (par == 0))
                   {
                       par = 1;
                       process_rel(i, k, "PAR");
                   } /* fi */

                   else if((strcmp(relation, "CHD") == 0) && (chd == 0))
                   {
                       chd = 1;
                       process_rel(i, k, "CHD");
                   } /* else fi */

                   else if((strcmp(relation, "SIB") == 0) && (sib == 0))
                   {
                       sib = 1;
                       process_rel(i, k, "SIB");
                   } /* else fi */

                   else if((strcmp(relation, "RB") == 0) && (rb == 0))
                   {
                       rb = 1;
                       process_rel(i, k, "RB");
                   } /* else fi */

                   else if((strcmp(relation, "RN") == 0) && (rn == 0))  
                   {
                       rn = 1;
                       process_rel(i, k, "RN");
                   } /* else fi */

                   else if((strcmp(relation, "RO") == 0) && (ro == 0))
                   {
                       ro = 1;
                       process_rel(i, k, "RO");
                   } /* else fi */

                   free(rows[l]);
                   strcpy(relation, "");
               } /* fi */
            } /* for l */
         } /* fi */

         else if(rows != NULL)
           free(rows);
         rows = NULL;

         par = chd = sib = rn = rb = ro =  0;
      } /* for k */
    } /* for i */
} /* find_relations */


void process_rel(i, k, rel)
   int i, k;
   char *rel;
{

   TREL *new;
   int n;

   if(par || chd || sib)
   {
      new = (TREL *)calloc(1, TREL_SIZE);

      /* Find next location in treerel for i */

      n = mt_table[i].treerel_cnt;
      strcpy(new->mh, mt_table[k].mh);
      strcpy(new->rel, rel);
      new->wt = mt_table[k].weight;
      mt_table[i].treerel[n] = new;
      mt_table[i].treerel_cnt++;

      new = (TREL *)calloc(1, TREL_SIZE);

      /* Find next location in treerel for k */

      n = mt_table[k].treerel_cnt;
      mt_table[k].treerel[n] = new;
      strcpy(mt_table[k].treerel[n]->mh, mt_table[i].mh);
      strcpy(mt_table[k].treerel[n]->rel, rel);
      mt_table[k].treerel[n]->wt = mt_table[i].weight;
      mt_table[k].treerel_cnt++;
   } /* fi PAR/CHD/SIB */

   else if(rn || rb || ro)
   {
      new = (TREL *)calloc(1, TREL_SIZE);

      /* Find next location in othrel for i */

      n = mt_table[i].othrel_cnt;
      strcpy(new->mh, mt_table[k].mh);
      strcpy(new->rel, rel);
      new->wt = mt_table[k].weight;
      mt_table[i].othrel[n] = new;
      mt_table[i].othrel_cnt++;

      new = (TREL *)calloc(1, TREL_SIZE);

      /* Find next location in othrel for k */

      n = mt_table[k].othrel_cnt;
      strcpy(new->mh, mt_table[i].mh);
      strcpy(new->rel, rel);
      new->wt = mt_table[i].weight;
      mt_table[k].othrel[n] = new;
      mt_table[k].othrel_cnt++;
   } /* else fi RN/RB/RO */
}



void process_cot(i, k, freq)
   int i, k ;
   float freq;
{
   TCOT *new;
   int n;
   char line[80];
   char line1[80];
   char term[80];
   char **rows = NULL;
   int t, l, z;
   char freqstring[20];
   float frequency = 0;

   new = (TCOT *)calloc(1, TCOT_SIZE);

   /* Find the next open slot in the cot array for item i */

   n = mt_table[i].cot_cnt;
   mt_table[i].cot[n] = new;
   mt_table[i].cot_cnt++;

   strcpy(mt_table[i].cot[n]->mh, mt_table[k].mh);
   if (freq != 0)
      mt_table[i].cot[n]->cnt = freq;
   else
      mt_table[i].cot[n]->cnt = 0;

   mt_table[i].cot[n]->wt = mt_table[k].weight;

   /* NOW ADD IN THE ENTRY FOR k */

   sprintf(line1, "%s|%s\0", mt_table[k].dui, mt_table[i].dui);
   query_btree(COT_DUI, line1, &rows, &t);
   frequency = 0;

   if((rows != NULL) && (t > 0))
   {
      frequency = -1;
      sscanf(rows[0], "%*[^|]|%*[^|]|%f", &frequency);

      for(z = 0; z < t; z++)
      {
         if(rows[z] != NULL)
           free(rows[z]);
      } /* for */
      free(rows);
      rows = NULL;

      if(frequency > 0.0)
      {
         new = (TCOT *)calloc(1, TCOT_SIZE);

         /* Find the next open slot in the cot array for item k */

         n = mt_table[k].cot_cnt;
         mt_table[k].cot[n] = new;
         mt_table[k].cot_cnt++;

         strcpy(mt_table[k].cot[n]->mh, mt_table[i].mh);
         if (frequency != 0)
            mt_table[k].cot[n]->cnt = frequency;
  
         mt_table[k].cot[n]->wt = mt_table[i].weight;
      } /* fi frequency > 0 */
  } /* fi t > 0 */

  else if(rows != NULL)
    free(rows);
}


void compute_score()
{
   int j, done;
   long i;
   float score;

   for (i= 0; i< mt_count; i++)
   {
      score = 1;

      done = FALSE;
      for (j=0; !done && (j < mt_table[i].cot_cnt); j++)
      {
         if(mt_table[i].cot[j] == (TCOT *)NULL)
            done = TRUE;
         else
            score += (mt_table[i].cot[j]->cnt * COT_FACTOR * 
                                                        mt_table[i].cot[j]->wt);
      } /* for */

      done = FALSE;
      for (j=0; j< mt_table[i].treerel_cnt; j++)
      {
         if(mt_table[i].treerel[j] == (TREL *)NULL)
            done = TRUE;
         else
            score += (mt_table[i].treerel[j]->wt * TREE_FACTOR);
      } /* for */

      done = FALSE;
      for (j=0; j< mt_table[i].othrel_cnt; j++)
      {
         if(mt_table[i].othrel[j] == (TREL *)NULL)
            done = TRUE;
         else
            score += (mt_table[i].othrel[j]->wt * TREE_FACTOR);
      } /* for */

/* modified Sept 19, 00 */
/* updated Sept 8, 05 */

/*******
      if ((strcmp(mt_table[i].path[0], "P001")==0 
               || strcmp(mt_table[i].path[0], "P002")==0) &&
         (mt_table[i].path[1]!=NULL && strcmp(mt_table[i].path[1], "P004")==0))
********/

      if((mt_table[i].paths[MMI] || mt_table[i].paths[TRG]) && 
         mt_table[i].paths[PRC])
        score *=2;

/**********************/
      score *= mt_table[i].weight;
      mt_table[i].score = (long) score;
   } /* for */
}
