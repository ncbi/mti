#include <ctype.h>
#include "M_terms.h"
#include "newList.h"

#define MAXCTIs 250

/* Extern definitions needed */

extern TCHECKTAG *ct_table;
extern int ct_count;
extern TMT *mt_table;
extern long mt_count;

extern int haveHMD, max, doAgedReview, showAgedReview, RTM_Debug, CATALOGING,
           nursingJournal, veterinaryJournal, dentistryJournal,
           firstLineReviewJournal, removeAged, showJust, doNoAddForced,
           Pregnant_Non_Pregnant_OK, sportsMedicineJournal, botanyJournal,
           geriatricJournal, foundNonHumanTrigger;
extern long avgScore, numBelow;

extern FILE *fout;
extern int remMHs;
extern char *belowLineTmp[];
extern char muid[SMALL_LINE + 1];
extern char nlmID[SMALL_LINE + 1];
extern char *globalTitle;
extern char *globalAbstract;
extern char *citation;
extern char PTs[SMALL_LINE + 1];
extern long cit_len;
extern long fITpos, fITlen;
extern float PW4; /* PRC */
extern long txtStartPos, txtLen, gTLen, gALen;

extern void process_checktags(char *mterm, int this_path, char *this_dui, char *textloc,
                              char *trigger);
extern void process_subheads(char *trigger, char *mterm,  int this_path,
                      char *this_dui, char *textloc, long inscore);
extern int search_index(char *this_dui);
extern long search_indexMH(char *thisMH);
extern int check_special_term();
extern int amIPT(char *term);
extern void process_mterm(int ambigFlag, char *ambigTrigger, char *mterm, int this_path, 
                   float weight, int type, char *textloc, char *entry_term,
                   char *trigger, char *this_dui, char *origPMID,
                   char *positionalInfo, int forcePrint);
extern int foundInText(char *lookFor, int caseSensitive, int TitleOnly);

extern struct MLstruct MLPicks;
extern int amIGeographical(int pos);
extern void modifyBelowLine(char *, char *, char *);
extern long checkAAsLong(char *lookFor, char *longForm);
extern char *getVocabDen(char *nlmID, char *DUI);
extern void updateFinalList(char *mh, char *dui, char *from, int which);
extern int isLookForOK(char *lookFor);
extern char *findMHDUI(char *mh);

extern long numFinalCTs;
extern struct finalStruct finalCTlist[MAX_FINAL_CTs];
extern long numFinalMHs;
extern struct finalStruct finalMHlist[MAX_FINAL_MHs];
extern int isAnimalContext(char *lookFor);
extern int isAnimalRelated();
int isAmbigTrig(char *lookFor);


long fromCTpos[MAXCTIs];
long fromMHList[MAXCTIs];
int CTposFromTI[MAXCTIs];
int CTposFromAB[MAXCTIs];

char positionalInfo[MAXCTIs][1025];
char triggers[MAXCTIs][1025];
long new_CTs, CTIpos, foundCTIpos;

void PCT(int whichCT, int pos, int ppos, char *trigger);
void PSH(int whichSH, int pos, int ppos, char *trigger);
void checkTreecodes(int pos, int ppos);
int check_exclude_MHs(char *term, char *entry_term, int path, char *textloc);
int checkTitle(char *MH_term, char *entry_term);
void setupCTList();
void check_CTs(int flag);
void check_SHs();
int search_indexTopCT(char *this_dui, char *this_mh);
int checkCTifUsed(long pos);
int isCT(char *DUI);
void finalCatalogCheck(long endPos);
void finalDCMSCheck();
void finalCheck();
void checkCoordinationRules();
void checkNewOtherCoordRules();
int checkTerm(long pos, char *str);
void AntonioCheck();
void checkPregnancyMammalOnly();
void doReviewMTIFLCheck();
void doForcedMiceRats();
int isMHExclude(char *term, int flag);
int isMHExcludeLC(char *term, int flag);
void checkTreecodes_CT();
void checkText_Triggers(int okMice, int okRats);
int findCT(char *CheckTag);
void removeFromBelowLine(char *CheckTag);
void blockFromMT_Table(char *CheckTag);
int amIAnimalCT(char *mh);
void clearCTList(char *mh);
void checkDina_RegEx();
char *buildET(char *PI);
void Final_Treecode_CT_Check();
void AddFinalTreeCode_CT(long ctPos, char *mh, char *treecode);


/* validate flags -- if TRUE, then do not check trees */

int notE02810 = TRUE;
int notE01 = TRUE;

/***************************************************************************
*
*  PCT --
*
*      process_checktags short-hand routine 
*
***************************************************************************/

void PCT(int whichCT, int pos, int ppos, char *trigger)
{
    process_checktags(CT_list[whichCT].Name, ppos, CT_list[whichCT].DUI,
                      mt_table[pos].textloc, trigger);
    CT_list[whichCT].already_used = TRUE;
} /* PCT */

/***************************************************************************
*
*  PSH --
*
*      process_subheading short-hand routine 
*
***************************************************************************/

void PSH(int whichSH, int pos, int ppos, char *trigger)
{
    process_subheads(trigger, SH_list[whichSH].Name, ppos, SH_list[whichSH].DUI,
                     mt_table[pos].textloc, 1111);
    SH_list[whichSH].already_used = TRUE;
} /* PSH */

/***************************************************************************
*
*  checkTreecodes --
*
*      This 
*
***************************************************************************/

void checkTreecodes(int pos, int ppos)
{
    int i, j, k, found, ok, ct, sh, docheck, fpos, animalOK;
    char tree_num[256];
    char trigger[MAXLINE + 1];

    /* Setup dual tests first */

    notE01 = TRUE;
    notE02810 = TRUE;
    for(i = 0; i < mt_table[pos].num_treecodes; i++)
    {
        if(mt_table[pos].treecodes[i][0] == 'E')
        {
              /* E01 - Diagnosis */
           if(strncmp(mt_table[pos].treecodes[i], "E01", 3) == 0)
             notE01 = FALSE;

              /* E02.810.530 - Radiosurgery */
              /* E02.810.814 - Whole-Body Irradiation */


              /* E02.815.530 - Radiosurgery 2015 */
              /* E02.815.814 - Whole-Body Irradiation */
           else if(
               (strncmp(mt_table[pos].treecodes[i], "E02.815.530", 11) == 0) || 
               (strncmp(mt_table[pos].treecodes[i], "E02.815.814", 11) == 0))
             notE02810 = FALSE;
        } /* fi */
    } /* for */

    /* Now go through all of the treecodes for this term and add Subheadings
       as appropriate for each treecode we find.
    */

    for(i = 0; i < mt_table[pos].num_treecodes; i++)
    {
        strcpy(tree_num, mt_table[pos].treecodes[i]);

        /* Go from end of array so that we get longer entries first */

        found = FALSE;
        fpos = -1;
        for(j = (numTrSHs - 1); !found && (j >= 0); j--)
        {
            if(tree_num[0] == TreeSHs[j].Treecode[0])
            {
                if(strstr(tree_num, TreeSHs[j].Treecode) != NULL)
                {
                   /* Now, if we found a match, do we need to add it? */
  
                   docheck = FALSE;
                   for(k = 0; !docheck && (k < TreeSHs[j].numSHs); k++)
                   {
                     if(!SH_list[TreeSHs[j].SHs[k]].already_used)
                       docheck = TRUE;
                   } /* for */

                   if(docheck)
                   {
                      found = TRUE;
                      fpos = j;
                   } /* fi */
                } /* fi */
            } /* fi */
        } /* for */

        if(found)
        {
            for(k = 0; k < TreeSHs[fpos].numSHs; k++)
            {
                if(!SH_list[TreeSHs[fpos].SHs[k]].already_used)
                {
                    ok = FALSE;
                    sh = TreeSHs[fpos].SHs[k];
                    if((sh == radiotherapy) && notE02810)
                    {
                       if(notE02810)
                         ok = TRUE;
                    } /* fi */

                    else if(sh == surgery)
                    {
                       if(notE01)
                         ok = TRUE;
                    } /* fi */

                    else
                      ok = TRUE;

                    if(ok)
                    {
                       sprintf(trigger, "%s (%s)\0", tree_num,
                               mt_table[pos].mh);

                       if(RTM_Debug)
                       {
                          fprintf(fout, "Adding Subheading - %s\n", trigger);
                          fflush(fout);
                       } /* fi */

                       PSH(sh, pos, ppos, trigger);
                    } /* ok */
                } /* fi !already used */
            } /* for each SH */
        } /* fi found */
    } /* for each treecode */
} /* checkTreecodes */

/***************************************************************************
*
*  check_exclude_MHs --
*
*      This 
*
***************************************************************************/

int check_exclude_MHs(char *term, char *entry_term, int path, char *textloc)
{
   int rtn = FALSE;

   if((strcmp(term, "TEST") == 0) || (strcmp(term, "Disease") == 0))
      rtn =  TRUE;

   if(strcmp(term, "Role") == 0)
   {
      if(path == PRC)
        rtn = FALSE;
      else
        rtn = TRUE;
   } /* fi */

   return(rtn);
} /* check_exclude_MHs */

/***************************************************************************
*
*  checkTitle --
*
*      This 
*
***************************************************************************/

int checkTitle(char *MH_term, char *entry_term)
{
   int rtn = TRUE; /* TRUE means exclude term, FALSE means keep the term */
   int done = FALSE;
   char tmp[ENTRY_TERM_SIZE + 1];
   char tmp2[ENTRY_TERM_SIZE + 1];
   char tmp3[ENTRY_TERM_SIZE + 1];
   char tmp5[ENTRY_TERM_SIZE + 1];
   char loc[ENTRY_TERM_SIZE + 1];
   char eterm[ENTRY_TERM_SIZE + 1];
   char query[MAXLINE + 1];
   FILE *out, *popen();

   /* Only look if we have an entry term in the title */

   if((strlen(entry_term) > 1) && (strstr(entry_term, "-ti") != NULL))
   {
      /* Loop through each entry term (source term) from MMI and check
         it against the MH_term we are looking to exclude. But, only
         check terms from the title.
      */

      strcpy(tmp, entry_term);
      done = FALSE;
      while(!done && (strlen(tmp) > 0))
      {
         strcpy(tmp2, "");
         strcpy(tmp3, "");
         sscanf(tmp, "%[^:]:%[^\n]", tmp2, tmp3);
         while(!done && (strlen(tmp2) > 0))
         {
            strcpy(eterm, "");
            strcpy(tmp5, "");
            strcpy(loc, "");
            sscanf(tmp2, "%*[^\"]\"%[^\"]\"-%[^,]%[^\n]", eterm, loc, tmp5);

            if((strlen(eterm) > 0) && (strncmp(loc, "ti", 2) == 0))
            {
               if(strcmp(MH_term, eterm) == 0)
               {
                  done = TRUE;
                  rtn = FALSE;
               } /* fi */
            } /* fi eterm */

            strcpy(tmp2, tmp5);
         } /* while tmp2 */

         strcpy(tmp, tmp3);
      } /* while tmp */
   } /* fi */

   return(rtn);
} /* checkTitle */

/***************************************************************************
*
*  setupCTList --
*
*      This routine initializes our list of CheckTags that are already in
*   the mt_table list of concepts from the various paths.
*
***************************************************************************/

void setupCTList()
{
   long i;

   /* Initialize everything */

   for(i = 0; i < num_ct; i++)
   {
      CT_list[i].already_used = FALSE;

      if(search_indexTopCT(CT_list[i].DUI, CT_list[i].Name) >= 0)
        CT_list[i].already_used = TRUE;
   } /* for */
} /* setupCTList */

/***************************************************************************
*
*  check_CTs --
*
*      This 
*
***************************************************************************/

void check_CTs(int flag)
{
   long i, j, k, foundMHpos, lindex, x, index;
   int ok, r, okCheck, ct, pos, found, done, ok2,
       vetOK, Human_found, Animal_found, Aged_found, okMice, okRats;
   char tmp[SMALL_LINE + 1], tmpList[SMALL_LINE + 1], jTmp[SMALL_LINE + 1],
        cui[25], from[SMALL_LINE + 1], *foo, loc[250];

   okMice = okRats = TRUE;
   Human_found = Animal_found = Aged_found = FALSE;

   /* Initialize everything */

   for(i = 0; i < MAXCTIs; i++)
   {
      CTposFromTI[i] = FALSE;
      CTposFromAB[i] = FALSE;
      fromCTpos[i] = -1;
      fromMHList[i] = -1;
      strcpy(triggers[i], "");
      strcpy(positionalInfo[i], "");
   } /* for */
   new_CTs = CTIpos = foundCTIpos = 0;

   /* Do these additions prior to calculationg the final CheckTags to
      make sure whatever they add will be included in the CTs.

     Affects both mt_table & ct_table
   */

   if(flag)
   {
       checkPregnancyMammalOnly();
       checkCoordinationRules();
       checkNewOtherCoordRules();
   } /* fi */

   /* Now, do the CheckTags */

   /* Go through and see if we should allow Mice and/or Rats to be added
      since "Arvicolinae, Peromyscus, Muridae, Murinae" say don't use Mice.
      And, "Dipodomys, Muridae, and Murinae" say don't use Rat.
   */

   for(j = 0; j < mt_count; j++)
   {
      if(mt_table[j].hasPrinted)
      {
          if((strcmp(mt_table[j].mh, "Arvicolinae") == 0) ||
             (strstr(mt_table[j].entry_term, "Arvicolinae") != NULL) ||
             (strcmp(mt_table[j].mh, "Peromyscus") == 0) ||
             (strstr(mt_table[j].entry_term, "Peromyscus") != NULL))
          {
              okMice = FALSE;
              if(RTM_Debug)
              {
                  fprintf(fout, "no MICE: Arvicolinae or Peromyscus found\n");
                  fflush(fout);
              } /* fi */
          } /* fi */

          if((strcmp(mt_table[j].mh, "Muridae") == 0) ||
             (strstr(mt_table[j].entry_term, "Muridae") != NULL) ||
             (strcmp(mt_table[j].mh, "Murinae") == 0) ||
             (strstr(mt_table[j].entry_term, "Murinae") != NULL))
          {
              okMice = FALSE;
              okRats = FALSE;
              if(RTM_Debug)
              {
                  fprintf(fout, "no MICE or RATS: Muridae or Murinae found\n");
                  fflush(fout);
              } /* fi */
          } /* fi */

          if((strcmp(mt_table[j].mh, "Dipodomys") == 0) ||
             (strstr(mt_table[j].entry_term, "Dipodomys") != NULL))
          {
              okRats = FALSE;
              if(RTM_Debug)
              {
                  fprintf(fout, "no RATS: Dipodomys found\n");
                  fflush(fout);
              } /* fi */
          } /* fi */
      } /* fi */
   } /* for */

   /* What does the list of CTs look like before mt_table review */

   if(showAgedReview)
   {
       fprintf(fout, "\nshowAgedReview-1: new_CTs before: %ld\n", new_CTs);
       fprintf(fout, "Any CTs in the mt_table?:\n");
   } /* fi */

   for(j = 0; j < mt_count; j++)
   {
       if((mt_table[j].datatype == CT) && mt_table[j].oktoprint)
       {
           if(showAgedReview || RTM_Debug)
               fprintf(fout, "   CT - mt_table[%ld]: %s (printed?) %d\n", j,
                       mt_table[j].mh, mt_table[j].hasPrinted);

           ct = findCT(mt_table[j].mh);
           if(ct > -1)
           {
               if(strstr(mt_table[j].textloc, "AB") != NULL)
                  CTposFromAB[new_CTs] = TRUE;

               if(strstr(mt_table[j].textloc, "TI") != NULL)
                  CTposFromTI[new_CTs] = TRUE;

               else if(ct == Female)
               {
                   if(foundInText("ovarian cancer", FALSE, TRUE) ||
                      foundInText("ovarian neoplasm", FALSE, TRUE))
                    CTposFromTI[new_CTs] = TRUE;
               } /* else fi */

               fromCTpos[new_CTs] = ct;
               fromMHList[new_CTs] = -1;
               strcpy(triggers[new_CTs], mt_table[j].trigger);
               new_CTs++;
           } /* fi */
       } /* fi */
   } /* for */

   if(showAgedReview)
   {
       fprintf(fout, "\n");
       fprintf(fout, "Any CTs in the ct_table?:\n");
   } /* fi */

   for(j = 0; j < ct_count; j++)
   {
       if(showAgedReview)
           fprintf(fout, "   CT - ct_table[%ld]: %s (printed?) %d\n", j,
              ct_table[j].mh, ct_table[j].hasPrinted);

       ct = findCT(ct_table[j].mh);
       if(ct > -1)
       {
           fromCTpos[new_CTs] = ct;
           fromMHList[new_CTs] = -1;
           strcpy(triggers[new_CTs], ct_table[j].trigger);
           new_CTs++;
       } /* fi */
   } /* for */

   if(showAgedReview)
   {
       fprintf(fout, "\n");
       fprintf(fout, "Any CTs in the belowLineTmp?:\n");

       for(j = 0; j < numBelow; j++)
       {
           if(strstr(belowLineTmp[j], "|CT") != NULL)
              fprintf(fout, "   CT - belowLineTmp[%ld]: %s", j,
                      belowLineTmp[j]);
       } /* for */
       fprintf(fout, "\n");

       fprintf(fout, "\nshowAgedReview-1: new_CTs after: %ld\n", new_CTs);
       for(i = 0; i < new_CTs; i++)
       {
           ct = fromCTpos[i];
           fprintf(fout, "newCT[%ld]: %d", i, fromCTpos[i]);
           if(ct > -1)
             fprintf(fout, " (%s)|%s|\n", CT_list[ct].Name, triggers[i]);

           else
             fprintf(fout, " (N/A)|%s|\n", triggers[i]);
           fflush(fout);
       } /* for */
   } /* fi showAgedReview */

   /* Now go through all of the treecodes for each term and add CheckTags
      as appropriate for each treecode we find.
   */

   checkTreecodes_CT();

   /* What does the list of CTs look like before we do text triggers */

   if(showAgedReview)
   {
       fprintf(fout, "\nshowAgedReview-2: new_CTs: %ld\n", new_CTs);
       for(i = 0; i < new_CTs; i++)
       {
           ct = fromCTpos[i];
           fprintf(fout, "showAgedReview-2[%ld]: %d", i, fromCTpos[i]);
           if(ct > -1)
             fprintf(fout, " (%s)|%s|\n", CT_list[ct].Name, triggers[i]);

           else
             fprintf(fout, " (N/A)|%s|\n", triggers[i]);
           fflush(fout);
       } /* for */
   } /* fi showAgedReview */

   /* Now handle the text lookup list of triggers for CTs */

   checkText_Triggers(okMice, okRats);

   /* Want to make sure Humans, Male, and Female are not recommended for
      Biography PT.  07/12/2013 + added Male & Female 08/15/2013
   */

   if(strstr(PTs, "Biography") != NULL)
   {
       for(i = 0; i < new_CTs; i++)
       {
           ct = fromCTpos[i];
           if((ct == Human) || (ct == Female) || (ct == Male))
           {
               if(showAgedReview || RTM_Debug)
                 fprintf(fout, "- Removing CheckTag: %s -- Biography Rule\n",
                         CT_list[ct].Name); 

               removeFromBelowLine(CT_list[ct].Name);
               blockFromMT_Table(CT_list[ct].Name);
               fromCTpos[i] = -1;
           } /* fi */
       } /* for */

       CT_list[Human].already_used = FALSE;
       CT_list[Female].already_used = FALSE;
       CT_list[Male].already_used = FALSE;
   } /* fi */

   Human_found = CT_list[Human].already_used;
   Animal_found = CT_list[Animals].already_used;
   for(i = 0; i < new_CTs; i++)
   {
       if(fromCTpos[i] == Animals)
         Animal_found = TRUE;

       else if(fromCTpos[i] == Human)
         Human_found = TRUE;
   } /* for */

   if(showAgedReview)
   {
       fprintf(fout, "Humans Already Used: %d  Animals Already Used: %d\n",
               CT_list[Human].already_used, CT_list[Animals].already_used);
       fflush(fout);
   } /* fi */

   /* If Animal_found is not set, go through the list of CTs we are
      recommending and double check to see if we should be recommending
      it.  If we have any of the following, we should also have "Animals":
      Cats, Cattle, Dogs, Cricetinae, Mice, Rabbits, Rats, Sheep, Swine,
      Cercopithecus, ChickEmbryo, Horses, or Bees.
   */

   for(i = 0; !Animal_found && (i < new_CTs); i++)
   {
       ct = fromCTpos[i];
       if(ct > -1)
       {
           if((ct == Cats) || (ct == Cattle) || (ct == Dogs) ||
              (ct == Cricetinae) || (ct == Mice) || (ct == Rabbits) || 
              (ct == Rats) || (ct == Swine) || (ct == Sheep) ||
              (ct == Cercopithecus) || (ct == ChickEmbryo) ||
              (ct == Horses) || (ct == Bees))
           {
                Animal_found = TRUE;
                if(RTM_Debug)
                {
                    fprintf(fout, "+ Adding CT: Animals -- Trigger: %s\n",
                            CT_list[ct].Name);
                    fflush(fout);
                } /* fi */

                modifyBelowLine(CT_list[Animals].Name,
                                "Lookup: Add Animals", "");

                process_checktags(CT_list[Animals].Name, MMI, CT_list[Animals].DUI,
                                  "Lookup: Add Animals", "Lookup: Add Animals");
           } /* fi */
       } /* fi fromCTpos[i] > -1 */
   } /* for */


   /* IF doAgedReview is set do the following review

      Now see what we have and make sure we don't add age related checktags
      if we have Animals set and Humans not set.  If Animals is not set, and we
      have age related checktags recommended, we need to add Humans.

        10 - "Humans", "C0020114"
         3 - "Animals", "C0003062"

         0 - "Adolescent", "C0205653
         1 - "Adult", "C0001675"
         2 - "Aged", "C0001792"
         6 - "Child", "C0008059"
        11 - "Infant", "C0021270"
        12 - "Infant, Newborn", "C0021289"
        26 - "Aged, 80 and over", "C0001795" 
        27 - "Child, Preschool", "C0008100"
        28 - "Middle Aged", "C0205847"
        29 - "Young Adult", "C0238598"
   */

   if(doAgedReview)
   {
       for(i = 0; i < new_CTs; i++)
       {
           ct = fromCTpos[i];
           if(showAgedReview)
           {
               fprintf(fout, "fromCTpos[%ld]: %d", i, fromCTpos[i]);
               if(ct > -1)
                 fprintf(fout, " (%s)\n", CT_list[ct].Name);

               else
                 fprintf(fout, " (N/A)\n");
               fflush(fout);
           } /* fi */

           if(ct > -1)
           {
               if(ct == Human)
                 Human_found = TRUE;

               else if(ct == Animals)
                 Animal_found = TRUE;
           } /* fi */
       } /* for */

       if(showAgedReview)
        fprintf(fout, "Animal_found: %d  - Human_found: %d\n", Animal_found,
              Human_found);

       /* If we have a Veterinary Journal, likely Animals, so remove Humans
          unless we see "human" in the actual text of the article.
       */

       if(Human_found && veterinaryJournal)
       {
           if(!foundInText("human", FALSE, FALSE))
           {
               if(RTM_Debug)
               {
                   fprintf(fout, "Set Humans FALSE Veterinary Journal Rule\n");
                   fflush(fout);
               } /* fi */

               Human_found = FALSE;
               for(i = 0; i < new_CTs; i++)
               {
                   ct = fromCTpos[i];
                   if(ct == Human)
                   {
                       removeFromBelowLine(CT_list[ct].Name);
                       blockFromMT_Table(CT_list[ct].Name);
                       fromCTpos[i] = -1;
                   } /* fi */
               } /* for */
           } /* fi */
       } /* fi */

       /* If we have Animals, but not Humans, remove any Humans related CTs
          NOTE: Make sure to update Age related testing in display.c for the
                belowLineTmp array.
       */

       if(Animal_found && !Human_found)
       {
          for(i = 0; i < new_CTs; i++)
          {
             ct = fromCTpos[i];
             if((ct == Adolescent) || (ct == Adult) || (ct == Aged) || 
                (ct == Child) || (ct == Infant) || (ct == Newborn) ||
                (ct == Aged80) || (ct == ChildPreschool) ||
                (ct == MiddleAged) || (ct == YoungAdult))
             {
                 if(showAgedReview || RTM_Debug)
                    fprintf(fout, "- Removing CheckTag: %s -- AgedReview\n",
                         CT_list[ct].Name);

                 removeFromBelowLine(CT_list[ct].Name);
                 blockFromMT_Table(CT_list[ct].Name);
                 fromCTpos[i] = -1;
             } /* fi */
          } /* for */
       } /* fi Animal && !Human */

       /* If we have Humans but we are in a Geriatrics journal, remove
          any of the earlier age related CTs since they will most likely
          be inappropriate.
       */

       if(Human_found && geriatricJournal)
       {
          for(i = 0; i < new_CTs; i++)
          {
             ct = fromCTpos[i];
             if((ct == Newborn) || (ct == Infant) || (ct == ChildPreschool) ||
                (ct == Child) || (ct == Adolescent) || (ct == YoungAdult) ||
                (ct == Adult) ||  (ct == MiddleAged))
             {
                 if(showAgedReview || RTM_Debug)
                    fprintf(fout, 
                       "- Removing CheckTag: %s -- AgedReview (Geriatrics)\n",
                         CT_list[ct].Name);

                 removeFromBelowLine(CT_list[ct].Name);
                 blockFromMT_Table(CT_list[ct].Name);
                 fromCTpos[i] = -1;
             } /* fi */
          } /* for */
       } /* fi Animal && !Human */
 
       /* Check to see if we have any Aged related terms when 
          Animals not set - if we do, add Humans.
       */

       Aged_found = FALSE;
       for(i = 0; !Aged_found && (i < new_CTs); i++)
       {
           ct = fromCTpos[i];
           if(ct > -1)
           {
               if((ct == Adolescent) || (ct == Adult) || (ct == Aged) || 
                  (ct == Child) || (ct == Infant) || (ct == Newborn) ||
                  (ct == Aged80) || (ct == ChildPreschool) ||
                  (ct == MiddleAged) || (ct == YoungAdult))
               {
                   Aged_found = TRUE;
                   if(!Human_found)
                   {
                      if(showAgedReview || RTM_Debug)
                        fprintf(fout,
                           "+ Adding CheckTag: Humans -- Trigger: Aged Rule\n");

                       modifyBelowLine(CT_list[Human].Name,
                                       "Lookup: Aged Rule", "");
                       process_checktags(CT_list[Human].Name, MMI, CT_list[Human].DUI,
                                         "Lookup: Aged Rule", "Lookup: Aged Rule");
                       Human_found = TRUE;
                   } /* fi */
               } /* fi */
           } /* fi fromCTpos[i] > -1 */
       } /* for */
   } /* fi doAgedReview */

   /* If we have a MTIFL Review journal, make sure we don't add in any
      CheckTags that are not Animals or Humans (9/18/12)

     7/11/13 - Allow if found in Title or Female and Ovarian Cancer in TI
   */

   if(firstLineReviewJournal)
   {
       for(i = 0; i < new_CTs; i++)
       {
           ok = TRUE;
           ct = fromCTpos[i];

           if((ct != Human) && (ct != Animals))
           {
               ok = FALSE;

               if(CTposFromTI[i] || CTposFromAB[i])
                 ok = TRUE;

               /* If I'm not in the Title, maybe one of the instances
                  of this same CT might be.  So, before we remove, check.

                  12/21/17 - adding in allowing if from AB to make consistent
                     with doReviewMTIFLCheck which now allows if found in 
                     either since we just want to make sure there is a textual
                     trigger to the term.
               */

               else
               {
                   for(x = 0; !ok && (x < new_CTs); x++)
                   {
                       if(x != i)
                       {
                           if(fromCTpos[x] == ct) /* Same CT */
                           {
                               if(CTposFromTI[x] || CTposFromAB[x])
                                 ok = TRUE;
                           } /* fi */
                       } /* fi */
                   } /* for */
               } /* else */

               /* Final checks just to make sure */

               if(!ok && (ct == Female))
               {
                   if(foundInText("ovarian cancer", FALSE, TRUE) ||
                      foundInText("ovarian neoplasm", FALSE, TRUE))
                    ok = TRUE;
               } /* fi */

               /* If Male/Female/Pregnancy triggered by something 
                  in TI or AB allow */

               if(!ok &&
                   ((ct == Female) || (ct == Male) || (ct == Pregnancy)) &&
                   CTposFromAB[i])
                 ok = TRUE;

               /* Double check age related CTs even if found in title */

               if(ok && !Human_found)
               {
                   if((ct == Adolescent) || (ct == Adult) || (ct == Aged) || 
                      (ct == Child) || (ct == Infant) || (ct == Newborn) ||
                      (ct == Aged80) || (ct == ChildPreschool) ||
                      (ct == MiddleAged) || (ct == YoungAdult))
                     ok = FALSE;
               } /* fi */

               if(!ok)
               {
                   if(showAgedReview || RTM_Debug)
                     fprintf(fout,
                         "- Removing CheckTag 0: %s (%d:%d) -- MTIFL Review\n",
                         CT_list[ct].Name, CTposFromTI[i], CTposFromAB[i]); 

                   removeFromBelowLine(CT_list[ct].Name);
                   blockFromMT_Table(CT_list[ct].Name);
                   fromCTpos[i] = -1;
               } /* fi */

               else if(RTM_Debug)
               {
                   fprintf(fout,
                       "Allowing MTIFL CheckTag 0: \"%s\" Found in Title\n",
                       CT_list[ct].Name);
                   fflush(fout);
               } /* else fi */
           } /* fi !Humans and !Animals */
       } /* for */
   } /* fi firstLineReviewJournal */

   /* Now add in the CheckTags that survived. */

   for(i = 0; i < new_CTs; i++)
   {
       foundCTIpos = fromCTpos[i];
       ct = fromCTpos[i];
       foundMHpos = fromMHList[i];

       if(strlen(positionalInfo[i]) > 0)
         foo = buildET(positionalInfo[i]);

       else
         foo = strdup("");

       if(foundCTIpos > -1)
       {
           /* For each CheckTag and each Path MH is from */

           if(foundMHpos > -1)
           {
              strcpy(jTmp, mt_table[foundMHpos].mh);
              if(strlen(triggers[i]) > 0)
              {
                  strcat(jTmp, ";");
                  strcat(jTmp, triggers[i]);
              } /* fi */

              for(r = 0; r < NUM_PATHS; r++) 
	      {
	          if(mt_table[foundMHpos].paths[r])
                  {
                      if((ct == Swine) || (ct == Sheep) || (ct == US) ||
                         (ct == Cercopithecus) || (ct == Bees))
                      {
                          process_mterm(FALSE, "", CT_list[ct].Name, MMI,
                                        1000.0, CT, "", foo, jTmp,
                                        CT_list[ct].DUI, "",
                                        positionalInfo[i], TRUE);

                          index = search_index(CT_list[ct].DUI);
                          if((index >= 0) && (mt_table[index].score == 0))
                            mt_table[index].score += 1000;

                          if(showAgedReview || RTM_Debug)
                            fprintf(fout,
                                "- Adding OtherCTA: %s -- Trigger: %s\n",
                                CT_list[ct].Name, jTmp);
                      } /* fi */

                      else
                      {
                          modifyBelowLine(CT_list[ct].Name, jTmp,
                                       positionalInfo[i]);
                          PCT(ct, foundMHpos, r, jTmp);
                          if(showAgedReview || RTM_Debug)
                            fprintf(fout,
                            "- Adding CheckTagA (%ld): %s -- Trigger: %s - PI: #%s#\n",
                                i, CT_list[ct].Name, jTmp, positionalInfo[i]);
                      } /* else */
                  } /* fi */
	      } /* for r */
           } /* fi foundMHpos */

           else if(strlen(triggers[i]) > 0)
           {
               if((ct == Swine) || (ct == Sheep) || (ct == US) ||
                  (ct == Cercopithecus) || (ct == Bees))
               {
                   ok2 = TRUE;
                   lindex = search_indexMH(CT_list[ct].Name);
                   if(lindex > -1)
                   {
                       if(mt_table[lindex].hasPrinted)
                         ok2 = FALSE;
                   } /* fi */

                   if(ok2)
                   {
                       process_mterm(FALSE, "", CT_list[ct].Name, MMI,
                                     1000.0, CT, "", foo, triggers[i],
                                     CT_list[ct].DUI, "",
                                     positionalInfo[i], TRUE);

                       index = search_index(CT_list[ct].DUI);
                       if((index >= 0) && (mt_table[index].score == 0))
                         mt_table[index].score += 1000;

                       if(showAgedReview || RTM_Debug)
                          fprintf(fout,
                                  "- Adding OtherCTB: %s -- Trigger: %s\n",
                                  CT_list[ct].Name, triggers[i]);
                   } /* fi */
               } /* fi */

               else
               {
                   vetOK = TRUE;
                   if((ct == Human) && veterinaryJournal)
                   {
                       vetOK = FALSE;
                       if(foundInText("human", FALSE, FALSE) ||
                          foundInText("children", FALSE, TRUE))
                         vetOK = TRUE;

                       else if(RTM_Debug)
                       {
                           fprintf(fout,
                             "Removing Humans Veterinary Journal Rule\n");
                           fflush(fout);
                       } /* else fi */
                   } /* fi */

                   if(vetOK)
                   {
                       strcpy(loc, "");
                       if(CTposFromTI[i] && CTposFromAB[i])
                         strcpy(loc, "TI;AB");
                       else if(CTposFromTI[i])
                         strcpy(loc, "TI");
                       else if(CTposFromAB[i])
                         strcpy(loc, "AB");

                       modifyBelowLine(CT_list[ct].Name, triggers[i],
                                       positionalInfo[i]);

                       process_mterm(FALSE, "", CT_list[ct].Name, MMI,
                                     1000.0, CT, loc, foo, triggers[i],
                                     CT_list[ct].DUI, "",
                                     positionalInfo[i], TRUE);

                       index = search_index(CT_list[ct].DUI);
                       if((index >= 0) && (mt_table[index].score == 0))
                         mt_table[index].score += 1000;

                       if(showAgedReview || RTM_Debug)
                         fprintf(fout,
                            "- Adding CheckTagB: %s -- Trigger: %s  pos: %s\n",
                            CT_list[ct].Name, triggers[i], positionalInfo[i]);
                   } /* fi */
               } /* else */
           } /* else fi */
       } /* fi CT still valid */

       free(foo);
   } /* for */


   /* If Animal_found is not set, go through the list of CTs we are
      recommending and double check to see if we should be recommending
      it.  If we have any of the following, we should also have "Animals":
      Cats, Cattle, Dogs, Cricetinae, Mice, Rabbits, Rats, Sheep, Swine,
      Cercopithecus, ChickEmbryo, Horses, or Bees.
   */

   if(!CT_list[Animals].already_used)
   {
       strcpy(tmpList, "");
       ok = FALSE;
       ct = -1;
       if(CT_list[Cats].already_used)
       {
           strcpy(tmpList, CT_list[Cats].Name);
           ok = TRUE;
       } /* fi */

       if(CT_list[Cattle].already_used)
       {
           if(strlen(tmpList) > 0)
             strcat(tmpList, ";");
           strcat(tmpList, CT_list[Cattle].Name);
           ok = TRUE;
       } /* fi */

       if(CT_list[Dogs].already_used)
       {
           if(strlen(tmpList) > 0)
             strcat(tmpList, ";");
           strcat(tmpList, CT_list[Dogs].Name);
           ok = TRUE;
       } /* fi */

       if(CT_list[Cricetinae].already_used)
       {
           if(strlen(tmpList) > 0)
             strcat(tmpList, ";");
           strcat(tmpList, CT_list[Cricetinae].Name);
           ok = TRUE;
       } /* fi */

       if(CT_list[Mice].already_used)
       {
           if(strlen(tmpList) > 0)
             strcat(tmpList, ";");
           strcat(tmpList, CT_list[Mice].Name);
           ok = TRUE;
       } /* fi */

       if(CT_list[Rabbits].already_used)
       {
           if(strlen(tmpList) > 0)
             strcat(tmpList, ";");
           strcat(tmpList, CT_list[Rabbits].Name);
           ok = TRUE;
       } /* fi */

       if(CT_list[Rats].already_used)
       {
           if(strlen(tmpList) > 0)
             strcat(tmpList, ";");
           strcat(tmpList, CT_list[Rats].Name);
           ok = TRUE;
       } /* fi */

       if(CT_list[Swine].already_used)
       {
           if(strlen(tmpList) > 0)
             strcat(tmpList, ";");
           strcat(tmpList, CT_list[Swine].Name);
           ok = TRUE;
       } /* fi */

       if(CT_list[Sheep].already_used)
       {
           if(strlen(tmpList) > 0)
             strcat(tmpList, ";");
           strcat(tmpList, CT_list[Sheep].Name);
           ok = TRUE;
       } /* fi */

       if(CT_list[Cercopithecus].already_used)
       {
           if(strlen(tmpList) > 0)
             strcat(tmpList, ";");
           strcat(tmpList, CT_list[Cercopithecus].Name);
           ok = TRUE;
       } /* fi */

       if(CT_list[ChickEmbryo].already_used)
       {
           if(strlen(tmpList) > 0)
             strcat(tmpList, ";");
           strcat(tmpList, CT_list[ChickEmbryo].Name);
           ok = TRUE;
       } /* fi */

       if(CT_list[Horses].already_used)
       {
           if(strlen(tmpList) > 0)
             strcat(tmpList, ";");
           strcat(tmpList, CT_list[Horses].Name);
           ok = TRUE;
       } /* fi */

       if(CT_list[Bees].already_used)
       {
           if(strlen(tmpList) > 0)
             strcat(tmpList, ";");
           strcat(tmpList, CT_list[Bees].Name);
           ok = TRUE;
       } /* fi */

        if(ok)
        {
            if(RTM_Debug)
            {
                fprintf(fout, "Adding CT: Animals -- Trigger: %s\n", tmpList);
                fflush(fout);
            } /* fi */

            sprintf(tmp, "Forced Animals Added: %s\0", tmpList);
            modifyBelowLine(CT_list[Animals].Name, tmp, "");
            process_checktags(CT_list[Animals].Name, MMI, CT_list[Animals].DUI, tmp, tmp);
        } /* fi */
   } /* fi */

   /* Check to see we brought Preganancy back when not supposed too */

   lindex = search_index(CT_list[Pregnancy].DUI);
   if(lindex > -1)
   {
       if(strcmp(mt_table[lindex].mh, "Removed CT") == 0)
         mt_table[lindex].forcePrint = FALSE;
   } /* fi */

   /* Now take a look at things that require CTs to have been added */

   checkPregnancyMammalOnly();

   /* If we have Infant, Child, or Child, Preschool AND "stunting" in the
      title, we want to add Growth Disorders.
   */

   ok = FALSE;
   index = search_index(CT_list[ChildPreschool].DUI);
   if(index >= 0)
     ok = TRUE;

   else
   {
       index = search_index(CT_list[Infant].DUI);
       if(index >= 0)
         ok = TRUE;

       else
       {
           index = search_index(CT_list[Child].DUI);
           if(index >= 0)
             ok = TRUE;
       } /* else */
   } /* else */

   if(ok)
   {
        if(foundInText("stunting", FALSE, TRUE))
        {
            if(RTM_Debug)
            {
                fprintf(fout, "Adding Growth Disorders - in Title\n");
                fflush(fout);
            } /* fi */

            strcpy(from, "CCPI Stunting Forced Rule");
            process_mterm(FALSE, "", "Growth Disorders", MMI, 1000.0,
                          MH, "", "", from, "D006130", "", "", TRUE);

            index = search_index("D006130");
            if(index >= 0)
              mt_table[index].score += 1000;
        } /* fi */
   } /* fi */
} /* check_CTs */

/***************************************************************************
*
*  check_SHs --
*
*      This 
*
***************************************************************************/

void check_SHs()
{
   long i, j;

   /* Now go through all of the treecodes for each term and add CheckTags
      as appropriate for each treecode we find.
   */

   for(j = 0; j < mt_count; j++)
   {
      if(mt_table[j].hasPrinted)
      {
         if(RTM_Debug)
         {
            fprintf(fout, "check_SHs - mt_table[%ld].mh: #%s#\n",
                    j, mt_table[j].mh);
            fflush(fout);
         } /* fi */

         /* Add in any subheadings as appropriate */

         checkTreecodes(j, MMI);

      } /* fi */
   } /* for each Term in topN */
} /* check_SHs */

/***************************************************************************
*
*  search_indexTopCT --
*
*      This 
*
***************************************************************************/

int search_indexTopCT(char *this_dui, char *this_mh)
{
   int i;

   for(i = 0; i< max; i++)
   {
      if(strcmp(this_dui, mt_table[i].dui) == 0)
         return i;
   } /* for */


   for(i = 0; i< ct_count; i++)
   {
      if(strcmp(this_mh, ct_table[i].mh) == 0)
         return i;
   } /* for */

   return -1;
} /* search_indexTopCT */

/***************************************************************************
*
*  checkCTifUsed --
*
*      This 
*
***************************************************************************/

int checkCTifUsed(long pos)
{
   int rtn = FALSE;
   int i;

   for(i = 0; !rtn && (i < TermCTs[pos].numCTs); i++)
   {
       if(!CT_list[TermCTs[pos].CTs[i]].already_used)
         rtn = TRUE;
   } /* for */

   return(rtn);
} /* checkCTifUsed */


/***************************************************************************
*
*  isCT --
*
*      This 
*
***************************************************************************/

int isCT(char *DUI)
{
    long i;
    int rtn = FALSE;

    /* Check CT_list list if not found in main */

    for(i = 0; !rtn && (i < num_ct); i++)
    {
        if(DUI[0] == CT_list[i].DUI[0])
        {
            if(strcmp(DUI, CT_list[i].DUI) == 0)
              rtn = TRUE;
        } /* fi */
    } /* for */

    /* Check pseudoCT list if not found in main */

    if(!rtn)
    {
        for(i = 0; !rtn && (i < numPseudoCTs); i++)
        {
            if(DUI[0] == pseudoCTs[i].DUI[0])
            {
                if(strcmp(DUI, pseudoCTs[i].DUI) == 0)
                  rtn = TRUE;
            } /* fi */
        } /* for */
    } /* fi */

    return(rtn);
} /* isCT */


/***************************************************************************
*
*  finalCatalogCheck --
*
*      This 
*
***************************************************************************/

void finalCatalogCheck(long endPos)
{
    int found, done, foundII, z;
    long i, j, x;

    for(i = 0; i < endPos; i++)
    {
        if((mt_table[i].oktoprint) && (mt_table[i].datatype != NM))
        {
            found = FALSE;
            done = FALSE;
            for(j = 0; !found && !done && (j < numCatMHExcls); j++)
            {
                if(strcmp(mt_table[i].dui, CatMHExcls[j].DUI) == 0)
                {
                    done = TRUE;
                    foundII = FALSE;
                    for(x = 0; !foundII && (x < numHMD_OKExcls); x++)
                    {
                        if(strcmp(CatMHExcls[j].DUI, HMD_OKExcls[x].DUI) == 0)
                          foundII = TRUE;
                    } /* for */

                    if(!foundII)
                    {
                       found = TRUE;
                       mt_table[i].oktoprint = FALSE;
                       if(RTM_Debug)
                       {
                           fprintf(fout, "Last Review Removal: #%s#\n",
                                   mt_table[i].mh);
                           fflush(fout);
                       } /* fi */
                    } /* fi */
                } /* fi */
            } /* for */

            if(!found) /* IF not in Catalog Exclude list, look @ Carcinoma */
            {
                if(strcmp(mt_table[i].mh, "Carcinoma") == 0)
                {
                    if((strstr(mt_table[i].entry_term, "Cancer") != NULL) ||
                       (strstr(mt_table[i].entry_term, "cancer") != NULL))
                    {
                        found = TRUE;
                        mt_table[i].oktoprint = FALSE;
                        if(RTM_Debug)
                        {
                            fprintf(fout,
                                   "Removing Carcinoma/Cancer Rule: %s\n",
                                    mt_table[i].mh);
                            fflush(fout);
                        } /* fi */
                    } /* fi */
                } /* fi */
            } /* fi !found */

            if(!found) /* Look @ Breast Feeding */
            {
                if(strcmp(mt_table[i].mh, "Breast Feeding") == 0)
                {
                    if(mt_table[i].paths[MMI] && !mt_table[i].paths[PRC])
                    {
                        found = TRUE;
                        mt_table[i].oktoprint = FALSE;
                        if(RTM_Debug)
                        {
                            fprintf(fout,
                                   "Removing Breast Feeding Rule: %s\n",
                                    mt_table[i].mh);
                            fflush(fout);
                        } /* fi */
                    } /* fi */
                } /* fi */
            } /* fi !found */

            if(!found) /* IF not excluded, look to remove any CTs */
            {
                for(z = 0; !found && (z < num_BadCatCTs); z++)
                {
                    if(strcmp(mt_table[i].dui, BadCatCTs[z].DUI) == 0)
                    {
                        mt_table[i].oktoprint = FALSE;
                        if(RTM_Debug)
                        {
                            fprintf(fout,
                                   "Removing Bad Catalog CT Rule: %s\n",
                                    mt_table[i].mh);
                            fflush(fout);
                        } /* fi */
                    } /* fi */
                } /* for */
            } /* fi !found */
        } /* fi */
    } /* for */

   /* Now go through the CT table and remove ones we don't want, also
      check to see if the CT might already be ready to print in the normal
      mt_table list.
   */

   for(i = 0; i < ct_count; i++)
   {
       found = FALSE;
       for(z = 0; !found && (z < num_BadCatCTs); z++)
       {
          if(strcmp(ct_table[i].dui, BadCatCTs[z].DUI) == 0)
          {
              found = TRUE;
              ct_table[i].oktoprint = FALSE;
              if(RTM_Debug)
              {
                  fprintf(fout, "Removing ct_table Bad Catalog CT Rule: %s\n",
                          ct_table[i].mh);
                  fflush(fout);
              } /* fi */
          } /* fi */
       } /* for */

       foundII = FALSE;
       for(j = 0; !foundII && (j < endPos); j++)
       {
           if(mt_table[i].oktoprint)
           {
              if(strcmp(ct_table[i].dui, mt_table[j].dui) == 0)
              {
                  foundII = TRUE;
                  ct_table[i].oktoprint = FALSE;
                  if(RTM_Debug)
                  {
                      fprintf(fout, "Removing ct_table Dup in MH table: %s\n",
                              ct_table[i].mh);
                      fflush(fout);
                  } /* fi */
              } /* fi */
           } /* fi */
       } /* for each MH */
   } /* for each CT */
} /* finalCatalogCheck */


/***************************************************************************
*
*  finalDCMSCheck --
*
*      This 
*
***************************************************************************/

void finalDCMSCheck()
{
    int found, done, result, removeMe, startedChar, ok;
    char term[SMALL_LINE + 1], tmp[SMALL_LINE + 1], from[SMALL_LINE + 1];
    long i, j, k, x, len;

    /* Remove PTs just to make sure */

    for(i = 0; i < mt_count; i++)
    {
        if(mt_table[i].safe && mt_table[i].oktoprint &&
           (mt_table[i].datatype != NM) && (mt_table[i].datatype != SH))
        {
            if(amIPT(mt_table[i].mh))
            {
                mt_table[i].safe = FALSE;
                mt_table[i].oktoprint = FALSE;
                mt_table[i].score = -1;
                if(RTM_Debug)
                  fprintf(fout, "Removing finalDCMSCheck PT Check: %s\n",
                           mt_table[i].mh);
            } /* fi */

            /* Added check for sequence like "A/California/7/2009
               and A/Puerto Rico/8/34 causing incorrect GEO recommendations.
               16 Oct 2012 JGM
            */

            else
            {
                if(amIGeographical(i))
                {
                    ok = TRUE;

                    /* Want to look for A, B, C, or D before slash based
                       on the WHO nomenclature for Influenza strains.
                    */

                    for(x = 0; ok && (x < 4); x++)
                    {
                        sprintf(tmp, " %c/%s/\0", x + 'A', mt_table[i].mh);
                        len = (long)strlen(tmp);
                        for(k = 0; k < len; k++)
                        {
                            if(isupper(tmp[k]))
                              tmp[k] = tolower(tmp[k]);
                        } /* for */

                        if((strstr(globalTitle, tmp) != NULL) ||
                           (strstr(globalAbstract, tmp) != NULL))
                          ok = FALSE;
                    } /* for */

                    if(!ok)
                    {
                        mt_table[i].safe = FALSE;
                        mt_table[i].oktoprint = FALSE;
                        mt_table[i].score = -1;
                        if(RTM_Debug)
                          fprintf(fout,
                                "Removing finalDCMSCheck /GEO/ Check: %s\n",
                                 mt_table[i].mh);
                    } /* fi */
                } /* fi */
            } /* else */
        } /* fi */
    } /* /* for */

    /* Patient(s) is causing Humans to be recommended even for
       Veterinary Medicine journals.  So, if have Humans in a Vet
       journal, remove unless we have Humans actually in the text.
    */

    if(veterinaryJournal)
    {
        done = FALSE;
        for(i = 0; !done && (i < mt_count); i++)
        {
            if(strcmp(mt_table[i].mh, "Humans") == 0)
            {
                 done = TRUE;
                 if(!foundInText("human", FALSE, FALSE) &&
                    !foundInText("children", FALSE, TRUE))
                 {
                     mt_table[i].safe = FALSE;
                     mt_table[i].oktoprint = FALSE;
                     mt_table[i].score = -1;
                     if(RTM_Debug)
                       fprintf(fout,
                          "Removing Humans Veterinary Journal Rule\n");
                 } /* fi */
            } /* fi */
        } /* /* for */

        done = FALSE;
        for(i = 0; !done && (i < ct_count); i++)
        {
            if(strcmp(ct_table[i].mh, "Humans") == 0)
            {
                 done = TRUE;
                 if(!foundInText("human", FALSE, FALSE) &&
                    !foundInText("children", FALSE, TRUE))
                 {
                     ct_table[i].oktoprint = FALSE;
                     ct_table[i].score = -1;
                     if(RTM_Debug)
                       fprintf(fout,
                          "Removing Humans Veterinary Journal Rule\n");
                 } /* fi */
            } /* fi */
        } /* /* for */
    } /* fi */

    for(i = 0; i < mt_count; i++)
    {
        if(mt_table[i].safe && mt_table[i].oktoprint &&
           !mt_table[i].forcePrint &&
           (mt_table[i].datatype != NM) && (mt_table[i].datatype != SH))
        {
            strcpy(term, mt_table[i].mh);
            if(mt_table[i].ETflag)
            {
                if(strncmp(mt_table[i].trigger,
                           "Entry Term Replacement for ", 27) == 0)
                {
                    strcpy(tmp, "");
                    sscanf(mt_table[i].trigger, "%*[^\"]\"%[^\"]", tmp);
                    if(strlen(tmp) > 0)
                      strcpy(term, tmp);
                } /* fi */
            } /* fi */

            startedChar = FALSE;
            found = FALSE;
            done = FALSE;
            removeMe = TRUE;
            for(j = 0; !done && !found && (j < numMHExcls); j++)
            {
               if(term[0] == MHExcls[j].Term[0])
               {
                  startedChar = TRUE;
                  result = strcmp(term, MHExcls[j].Term);
                  if(result == 0)
                  {
                      found = TRUE;

                      /* We want to remove items in the MH_Exclude list unless:

                           1) It came from a Forced Lookup trigger,

                           2) It has a checkTitle flag and it was found in the
                              Title of the article, and

                           3) If has a checkTitle flag, not found in the Title,
                              BUT, both MM & PRC recommended the term.

                           4) If PRC only, if number of PRC > 5
                      */

                      if(strstr(mt_table[i].trigger, "Forced Lookup:") != NULL)
                        removeMe = FALSE;  /* [1] */

                      else if(MHExcls[j].checkTitle)
                      {
                          if(strstr(mt_table[i].entry_term, "-ti-") != NULL)
                            removeMe = FALSE;  /* [2] */

                          else if(mt_table[i].paths[PRC] &&
                                  mt_table[i].paths[MMI])
                            removeMe = FALSE;  /* [3] */

                          else if(mt_table[i].paths[PRC] &&
                                  !mt_table[i].paths[MMI] &&
                                  (mt_table[i].num_RCs > 5))
                            removeMe = FALSE;  /* [4] */
                      } /* else fi */
                  } /* fi */
               } /* fi 0 == 0 */

               else if(startedChar)
                  done = TRUE;
            } /* for each MH_Exclude */

            if(found && removeMe)
            {
                mt_table[i].safe = FALSE;
                mt_table[i].oktoprint = FALSE;
                mt_table[i].score = -1;
                if(RTM_Debug)
                {
                   fprintf(fout, "\nRemoving MH_Exclude: %s\n", mt_table[i].mh);
                   fprintf(fout, "  -- %s|%s|MMI: %d PRC: %d|%s|%s\n",
                           mt_table[i].mh, mt_table[i].textloc,
                           mt_table[i].paths[MMI],
                           mt_table[i].paths[PRC], mt_table[i].entry_term,
                           mt_table[i].trigger);
                } /* fi */
            } /* fi */

            if(!found)  /* Let's look for Carcinoma then */
            {
                if(strcmp(mt_table[i].mh, "Carcinoma") == 0)
                {
                    if((strstr(mt_table[i].entry_term, "Cancer") != NULL) ||
                       (strstr(mt_table[i].entry_term, "cancer") != NULL))
                    {
                        found = TRUE;
                        mt_table[i].safe = FALSE;
                        mt_table[i].oktoprint = FALSE;
                        mt_table[i].score = -1;
                        if(RTM_Debug)
                          fprintf(fout, "Removing Carcinoma/Cancer Rule: %s\n",
                                  mt_table[i].mh);
                    } /* fi */
                } /* fi */
            } /* fi */

            /* If we have a Nursing journal, it is very likely that
               nursing refers to the profession and not Breast Feeding,
               so remove the Breast Feeding.
            */

            if(!found && nursingJournal)
            {
                if(strcmp(mt_table[i].mh, "Breast Feeding") == 0)
                {
                    if(mt_table[i].paths[MMI] && !mt_table[i].paths[PRC])
                    {
                        found = TRUE;
                        mt_table[i].safe = FALSE;
                        mt_table[i].oktoprint = FALSE;
                        mt_table[i].score = -1;
                        if(RTM_Debug)
                        {
                            fprintf(fout,
                                   "Removing Breast Feeding Rule: %s\n",
                                    mt_table[i].mh);
                            fflush(fout);
                        } /* fi */
                    } /* fi */
                } /* fi */
            } /* fi Nursing */

            /* If we have a Omeprazole, we want to only keep the term
               if it is actually used in the article due to MeSH changes
               mid-December 2012.
            */

            if(!found)
            {
                if(strcmp(mt_table[i].mh, "Omeprazole") == 0)
                {
                    if(!foundInText("omeprazole", FALSE, FALSE) &&
                       !foundInText("prilosec", FALSE, FALSE) &&
                       !foundInText("168", FALSE, FALSE))
                    {
                        found = TRUE;
                        mt_table[i].safe = FALSE;
                        mt_table[i].oktoprint = FALSE;
                        mt_table[i].score = -1;
                        if(RTM_Debug)
                        {
                             fprintf(fout,
                                   "Removing Omeprazole Rule: %s\n",
                                    mt_table[i].mh);
                                fflush(fout);
                        } /* fi */
                    } /* fi */
                } /* fi */
            } /* fi Omeprazole */
        } /* fi */
    } /* for */
} /* finalDCMSCheck */


/***************************************************************************
*
*  finalCheck --
*
*      This 
*
***************************************************************************/

void finalCheck()
{
    int found, done, result, removeMe, startedChar, foundPlants;
    int foundLOINC;
    char term[SMALL_LINE + 1], tmp[SMALL_LINE + 1], from[SMALL_LINE + 1];
    long i, j, k, index, ctr, indexLOINC;

    /* Check on Plant related terms */

    foundPlants = foundLOINC = FALSE;
    indexLOINC = -1;
    ctr = 0;
    for(i = 0; i < mt_count; i++)
    {
        if(mt_table[i].safe && mt_table[i].oktoprint &&
          (mt_table[i].datatype == MH))
        {
             if(strcmp(mt_table[i].mh,
                  "Logical Observation Identifiers Names and Codes") == 0)
             {
                 foundLOINC = TRUE;
                 indexLOINC = i;
             } /* fi */

             if((ctr < 5) || (mt_table[i].forcePrint))
             {
                 for(k = 0; k < mt_table[i].num_treecodes; k++)
                 {
                     if(strstr(mt_table[i].treecodes[k], "B01.650") != NULL)
                     {
                         foundPlants = TRUE;
                     } /* fi */
                 } /* for */
             } /* fi */
             ctr++;
        } /* fi printed */
    } /* for */

    index = search_index("D018525"); /* Germination */
    if(!foundPlants && !botanyJournal && 
       (index > -1) && mt_table[index].oktoprint &&
       mt_table[index].safe && !foundInText("seed germination", FALSE, FALSE))
    {
        mt_table[index].safe = FALSE;
        mt_table[index].oktoprint = FALSE;
        mt_table[index].score = -1;
        if(RTM_Debug)
          fprintf(fout, "Removing no Plants Rule: %s\n", mt_table[index].mh);
    } /* fi */

    if(foundLOINC)
    {
        if(!foundInText("LOINC", TRUE, FALSE) && 
           !foundInText("Logical Observation Identifiers Names and Codes",
                         FALSE, FALSE))
        {
            mt_table[indexLOINC].safe = FALSE;
            mt_table[indexLOINC].oktoprint = FALSE;
            mt_table[indexLOINC].score = -1;
            if(RTM_Debug)
              fprintf(fout, "finalCheck: Removing LOINC (not found in text)\n");
        } /* fi */
    } /* fi */
} /* finalCheck */

/***************************************************************************
*
*  checkTerm --
*
*      This routine checks to see if the incoming "str" matches the MH
*   pointed to in the mt_table array pointed to by "pos".  If there is
*   no match AND we have an ET (Entry Term), then check the trigger to
*   see if we have the orginal MH listed.  Return value is whether or
*   not we found a match.
*
***************************************************************************/

int checkTerm(long pos, char *str)
{
    int rtn = FALSE;
    char tmp[SMALL_LINE + 1];

     if(mt_table[pos].mh[0] == str[0])
       if(strcmp(mt_table[pos].mh, str) == 0)
         rtn = TRUE;

     if(!rtn)
     {
         sprintf(tmp, "Entry Term Replacement for \"%s\"\0", str);
         if(strstr(mt_table[pos].trigger, tmp) != NULL)
           rtn = TRUE;
     } /* fi */

     return(rtn);
} /* checkTerm */


/***************************************************************************
*
*  checkCoordinationRules --
*
*      This 
*
***************************************************************************/

void checkCoordinationRules()
{
    int foundFemale, foundMale, foundPregnancy, foundViviNon, pos;
    long i, k, index, numFound;
    char from[SMALL_LINE + 1], tmp[SMALL_LINE + 1];

    /* Identify which MHs, and Coordinated terms are already present
       and if any of the three CheckTags we might use are already set.
       If we find a MH, but don't find the coordination term, we will add
       the coordination term.  In some cases, we also need to ensure that
       the appropriate CheckTags are set.
    */

    numFound = 0;
    foundFemale = foundMale = foundPregnancy = foundViviNon = FALSE;
    for(i = 0; i < mt_count; i++)
    {
        if(mt_table[i].hasPrinted)
        {
            if(mt_table[i].datatype == MH)
            {
                if(checkTerm(i, "Viviparity, Nonmammalian"))
                   foundViviNon = TRUE;

                for(k = 0; k < numNeoplasmCoords; k++)
                {
                    if(checkTerm(i, neoplasmCoords[k].mh))
                    {
                        neoplasmCoords[k].foundMH = TRUE;
                        numFound++;
                    } /* fi */

                    else if(checkTerm(i, neoplasmCoords[k].coord))
                    {
                        /* See if from PRC only, and ignore so we can add
                           a note coming from coordination.
                        */

                        pos = search_index(neoplasmCoords[k].DUI);

                        if(pos > -1)
                        {
                            if(mt_table[pos].paths[MMI])
                            {
                                neoplasmCoords[k].foundCoord = TRUE;
                                numFound++;
                            } /* fi */
                        } /* fi */
                    } /* else fi */
                } /* for */
            } /* fi MH */
        } /* fi hasPrinted */

        /* Check to see if we have a CheckTag, if so, see if one of ones
           we are looking for.
        */

        if(mt_table[i].datatype == CT)
        {
           if(mt_table[i].hasPrinted || 
               (mt_table[i].safe && mt_table[i].oktoprint))
           {
               if(!foundPregnancy)
               {
                   if(strcmp(mt_table[i].mh, "Pregnancy") == 0)
                     foundPregnancy = TRUE;
               } /* fi */

               if(!foundMale)
               {
                   if(strcmp(mt_table[i].mh, "Male") == 0)
                     foundMale = TRUE;
               } /* fi */

               if(!foundFemale)
               {
                   if(strcmp(mt_table[i].mh, "Female") == 0)
                     foundFemale = TRUE;
               } /* fi */
           } /* fi ok */
        } /* fi CT */
    } /* for each mh */

    /* If we haven't already found the CTs, check ct_table */

    if(!foundPregnancy || !foundMale || !foundFemale)
    {
        for(i = 0; i < ct_count; i++) 
        {
            if(ct_table[i].oktoprint)
            {
                if(!foundPregnancy)
                {
                    if(strcmp(ct_table[i].mh, "Pregnancy") == 0)
                      foundPregnancy = TRUE;
                } /* fi */

                if(!foundMale)
                {
                    if(strcmp(ct_table[i].mh, "Male") == 0)
                      foundMale = TRUE;
                } /* fi */

                if(!foundFemale)
                {
                    if(strcmp(ct_table[i].mh, "Female") == 0)
                      foundFemale = TRUE;
                } /* fi */
            } /* fi oktoprint */
        } /* for */
    } /* fi */

    if(RTM_Debug)
    {
        /* Let's see where we are with the coordinations */

       fprintf(fout, "\n-------------------------------------------------\n\n");
       fprintf(fout, "foundMale: %d  foundFemale: %d  foundPregnancy: %d\n",
               foundMale, foundFemale, foundPregnancy);

       for(k = 0; k < numNeoplasmCoords; k++)
       {
           fprintf(fout, "%s|%s|%d|%d\n", neoplasmCoords[k].mh,
                   neoplasmCoords[k].coord, neoplasmCoords[k].foundMH,
                   neoplasmCoords[k].foundCoord);
       } /* for */
    } /* fi */

    /* Remove any of the questionable coordinations from the Keep_Check
       list that depend on CTs already being present before we should use.
       If we only have the MH and we don't have the right CT set, then
       turn-off and ignore.

        53  {"Carcinoma, Embryonal",
                 "Testicular Neoplasms", "D013736", "C0039590", 0, 0},

        58  {"Hydatidiform Mole",
                 "Uterine Neoplasms", "D014594", "C0042138", 0, 0},
    */

    if(neoplasmCoords[53].foundMH && !neoplasmCoords[53].foundCoord &&
      !foundMale)
    {
        if(RTM_Debug)
        {
            fprintf(fout, "Removing %s Coordination, no Male CT found\n",
                    neoplasmCoords[53].mh);
            fflush(fout);
        } /* fi */

        neoplasmCoords[53].foundMH = FALSE;
    } /* fi */

    if(neoplasmCoords[58].foundMH && !neoplasmCoords[58].foundCoord &&
      !foundFemale)
    {
        if(RTM_Debug)
        {
            fprintf(fout, "Removing %s Coordination, no Female CT found\n",
                    neoplasmCoords[58].mh);
            fflush(fout);
        } /* fi */

        neoplasmCoords[58].foundMH = FALSE;
    } /* fi */

    /* Now, add in any coordinations that we might be missing */

    for(k = 0; k < numNeoplasmCoords; k++)
    {
        if(neoplasmCoords[k].foundMH && !neoplasmCoords[k].foundCoord)
        {
            sprintf(from, "Neoplasm Coordination Rule: %s\0",
                    neoplasmCoords[k].mh);
            process_mterm(FALSE, "", neoplasmCoords[k].coord,
                          MMI, 1000.0, MH, "", "", from,
                          neoplasmCoords[k].DUI, "", "", TRUE);

            index = search_index(neoplasmCoords[k].DUI);
            if((index >= 0) && (mt_table[index].score == 0))
              mt_table[index].score += 1000;

            updateFinalList(neoplasmCoords[k].coord, neoplasmCoords[k].DUI, from, MH);
        } /* fi */

        /* Coordinate pointers with Validations/base.h file to make sure
           these don't move until we come up with a better plan.
        */

        if(!foundFemale && neoplasmCoords[k].foundMH && !firstLineReviewJournal)
        {
            if((k == 14) || (k == 22) || (k == 26) || (k == 27) || (k == 42) ||
               (k == 44) || (k == 46))
            {
               sprintf(from, "Forced Female Added (Neoplasms): %s\0",
                       neoplasmCoords[k].mh);
               process_checktags(CT_list[Female].Name, MMI, CT_list[Female].DUI, from, from);

               updateFinalList(CT_list[Female].Name, CT_list[Female].DUI, from, CT);
            } /* fi */
        } /* fi */

        if(!foundPregnancy && neoplasmCoords[k].foundMH &&
           !firstLineReviewJournal)
        {
            if((k == 22) || (k == 46))
            {
               sprintf(from, "Forced Pregnancy Added (Neoplasms): %s\0",
                       neoplasmCoords[k].mh);
               process_checktags(CT_list[Pregnancy].Name, MMI, CT_list[Pregnancy].DUI, from, from);

               updateFinalList(CT_list[Pregnancy].Name, CT_list[Pregnancy].DUI, from, CT);
            } /* fi */
        } /* fi */

        /*  2/5/16 - JGM: Turned off Forced Male Added poor perform (0%)
        if(!foundMale && neoplasmCoords[k].foundMH && !firstLineReviewJournal)
        */

        if(FALSE)
        {
            if(k == 45)
            {
               sprintf(from, "Forced Male Added (Neoplasms): %s\0",
                       neoplasmCoords[k].mh);
               process_checktags(CT_list[Male].Name, MMI, CT_list[Male].DUI, from, from);

               updateFinalList(CT_list[Male].Name, CT_list[Male].DUI, from, CT);
            } /* fi */
        } /* fi */
    } /* for */

    /* Now, if we did actually have both Keep_Check MH & Coord, make sure
       their CTs are properly set.
    */

    if(neoplasmCoords[53].foundMH && neoplasmCoords[53].foundCoord &&
      !foundMale && !firstLineReviewJournal)
    {
        sprintf(from, "Forced Male Added (Neoplasms): %s\0",
                neoplasmCoords[53].mh);
        process_checktags(CT_list[Male].Name, MMI, CT_list[Male].DUI, from, from);

        updateFinalList(CT_list[Male].Name, CT_list[Male].DUI, from, CT);
    } /* fi */

    if(neoplasmCoords[58].foundMH && neoplasmCoords[58].foundCoord &&
      !foundFemale && !firstLineReviewJournal)
    {
        if(!foundFemale)
        {
            sprintf(from, "Forced Female Added (Neoplasms): %s\0",
                    neoplasmCoords[58].mh);
            process_checktags(CT_list[Female].Name, MMI, CT_list[Female].DUI, from, from);

            updateFinalList(CT_list[Female].Name, CT_list[Female].DUI, from, CT);
        } /* fi */

        if(!foundPregnancy)
        {
            sprintf(from, "Forced Pregnancy Added (Neoplasms): %s\0",
                    neoplasmCoords[58].mh);
            process_checktags(CT_list[Pregnancy].Name, MMI, CT_list[Pregnancy].DUI, from, from);

            updateFinalList(CT_list[Pregnancy].Name, CT_list[Pregnancy].DUI, from, CT);
        } /* fi */
    } /* fi */

    if(foundPregnancy)
    {
        if(foundInText("viviparous", FALSE, FALSE))
        {
            if(!foundViviNon)
            {
                if((foundInText("lizard", FALSE, FALSE)) ||
                   (foundInText("reptile", FALSE, FALSE)) || 
                   (foundInText("reptilian", FALSE, FALSE)) || 
                   (foundInText("fish", FALSE, FALSE)) || 
                   (foundInText("guppy", FALSE, FALSE)) || 
                   (foundInText("guppies", FALSE, FALSE)))
                {
                    strcpy(from, "Pregnancy & Viviparous Rule");
                    process_mterm(FALSE, "", "Viviparity, Nonmammalian",
                          MMI, 1000.0, MH, "", "", from,
                          "D052286", "", "", TRUE);

                    index = search_index("D052286");
                    if((index >= 0) && (mt_table[index].score == 0))
                      mt_table[index].score += 1000;

                    if(RTM_Debug)
                    {
                        fprintf(fout, "Adding Viviparity, Nonmammalian: %s\n",
                                from);
                        fflush(fout);
                    } /* fi */
                } /* fi */
            } /* fi */
        } /* fi */
    } /* fi */
} /* checkCoordinationRules */


/***************************************************************************
*
*  checkNewOtherCoordRules --
*
*      This 
*
***************************************************************************/

void checkNewOtherCoordRules()
{
    int foundWord, j, foundMH, foundCoord;
    long i, k, index, coordPos, len;
    char from[SMALL_LINE + 1], tmp[SMALL_LINE + 1];

    if(RTM_Debug)
    {
        fprintf(fout, "Entered checkNewOtherCoordRules\n");
        fflush(fout);
    } /* fi */

    for(k = 0; k < numNewOthCoords; k++)
    {
        if(RTM_Debug)
        {
            fprintf(fout, "NewOtherCoord check: #%s#\n", NewothCoords[k].mh);
            fflush(fout);
        } /* fi */

        foundMH = foundCoord = FALSE;
        for(i = 0; i < mt_count; i++)
        {
            if(mt_table[i].hasPrinted)
            {
                if(mt_table[i].datatype == MH)
                {
                    if(!foundMH)
                      foundMH = checkTerm(i, NewothCoords[k].mh);

                    if(!foundCoord)
                      foundCoord = checkTerm(i, NewothCoords[k].coord);
                } /* fi MH */
            } /* fi hasPrinted */
        } /* for each mh */

        if(RTM_Debug)
        {
            fprintf(fout, "NewOtherCoord found:MH: %d  foundCoord: %d\n",
                    foundMH, foundCoord);
            fflush(fout);
        } /* fi */

        if(foundMH && foundCoord)
        {
            sprintf(from, "Other Coordination Rule: %s\0", NewothCoords[k].mh);
            updateFinalList(NewothCoords[k].coord, NewothCoords[k].dui, from, MH);
        } /* fi */

        if(foundMH && !foundCoord)
        {
            if(NewothCoords[k].numWords == 0)
            {
                sprintf(from, "Other Coordination Rule: %s\0",
                        NewothCoords[k].mh);

                index = search_index(NewothCoords[k].dui);
                if(index > -1)
                {
                    coordPos = search_indexMH(NewothCoords[k].mh);
                    mt_table[index].hasPrinted = FALSE;
                    mt_table[index].oktoprint = TRUE;
                    if(coordPos > -1)
                      mt_table[index].score = (mt_table[coordPos].score + 1);
                    else
                      mt_table[index].score = 1000;
                    mt_table[index].forcePrint = TRUE;
                    mt_table[index].paths[MMI] = TRUE;

                    len = (long)strlen(mt_table[index].trigger);
                    if((len + strlen(from)) < 4090)
                    {
                        strcpy(tmp, "");
                        strcpy(tmp, mt_table[index].trigger);
                        if(len > 0)
                          strcat(tmp, "; ");
                        strcat(tmp, from);
                        strcpy(mt_table[index].trigger, tmp);
                    } /* fi */
                } /* fi */

                else
                {
                    process_mterm(FALSE, "", NewothCoords[k].coord, MMI,
                       1000.0, MH, "", "", from, NewothCoords[k].dui, "", "", TRUE);

                    index = search_index(NewothCoords[k].dui);
                    if((index >= 0) && (mt_table[index].score == 0))
                      mt_table[index].score += 1000;
                } /* else */

                updateFinalList(NewothCoords[k].coord, NewothCoords[k].dui, from, MH);

                if(RTM_Debug)
                {
                    fprintf(fout, "Adding New Other Coord: %s\n", from);
                    fflush(fout);
                } /* fi */
            } /* fi just MH, add */

            else /* we need to look for word in text */
            {
                foundWord = FALSE;
                for(j = 0; !foundWord && (j < NewothCoords[k].numWords); j++)
                {
                   if(foundInText(NewothCoords[k].words[j], FALSE, FALSE))
                   {
                       foundWord = TRUE;
                       sprintf(from, "Other Coordination Rule: %s (%s)\0",
                               NewothCoords[k].mh, NewothCoords[k].words[j]);

                       process_mterm(FALSE, "", NewothCoords[k].coord, MMI,
                                     1000.0, MH, "", "", from,
                                     NewothCoords[k].dui, "", "", TRUE);

                       index = search_index(NewothCoords[k].dui);
                       if((index >= 0) && (mt_table[index].score == 0))
                         mt_table[index].score += 1000;

                       updateFinalList(NewothCoords[k].coord, NewothCoords[k].dui, from, MH);

                       if(RTM_Debug)
                       {
                           fprintf(fout, "Adding Other Coord: %s\n", from);
                           fflush(fout);
                       } /* fi */
                   } /* fi */
                } /* for */
            } /* else */
        } /* fi foundMH && !foundCoord */
    } /* for each coordination rule */
} /* checkNewOtherCoordRules */


/***************************************************************************
*
*  AntonioCheck --
*
*      This 
*
***************************************************************************/

void AntonioCheck()
{
    int found, n, Humans_found, Animals_found, Aged_Related_Added,
        Animal_Added, ok;
    long i, j, MHpos, index;
    char **rows = NULL;
    char dui[25], *foo;

    Humans_found = CT_list[Human].already_used;
    Animals_found = CT_list[Animals].already_used;

    Aged_Related_Added = Animal_Added = FALSE;

    /* Go through the list from Antonio and see what is already in mt_table */

    for(i = 0; i < NUM_ANTONIO_MHs; i++)
    {
        if(MLPicks.MHstatus[i])  /* 1 */
        {
            /* Since Antonio's ML for Humans is so accurate, take that
               over the rest of our rules.
            */

            ok = TRUE;
            if(strcmp(AntonioMHs[i], "Humans") == 0)
            {
                if(veterinaryJournal && !foundInText("human", FALSE, FALSE))
                {
                    ok = FALSE;
                    if(RTM_Debug)
                    {
                        fprintf(fout, 
                           "Ignoring Humans - Antonio ML Vet Journal\n");
                        fflush(fout);
                    } /* fi */
                } /* fi */

                else
                  Humans_found = TRUE;
            } /* fi */

            if(ok)
            {
                MHpos = search_indexMH(AntonioMHs[i]);
                if(MHpos > -1)
                {
                    MLPicks.found[i] = TRUE;
                    mt_table[MHpos].safe = TRUE;
                    mt_table[MHpos].oktoprint = TRUE;
                    mt_table[MHpos].forcePrint = TRUE;
                    mt_table[MHpos].datatype = CT;

                    if(strlen(mt_table[MHpos].trigger) == 0)
                     strcpy(mt_table[MHpos].trigger, "Antonio ML Rule Forced");

                    if(RTM_Debug)
                    {
                        fprintf(fout, "Forcing MH: \"%s\" Antonio ML Rules\n",
                                mt_table[MHpos].mh);
                        fflush(fout);
                    } /* fi */
                } /* fi */
            } /* fi ok */
        } /* fi picked */
    } /* for */

    /* If something from Antonio wasn't identified above, the performance
       goes way down.  Let's look here before we add Animals and Humans
       or any of the Age related terms.
       Aged and Animals seem to be fine, the rest we need to filter.  We'll
       see if Humans cleans up with cleaning up the Age related terms.
       Added Humans into filter as well.
       Added: 1/13/14
    */

    for(i = 0; i < NUM_ANTONIO_MHs; i++)
    {
        if(MLPicks.MHstatus[i] && !MLPicks.found[i])
        {
            if((strcmp(AntonioMHs[i], "Adolescent") == 0) ||
               (strcmp(AntonioMHs[i], "Adult") == 0) ||
               (strcmp(AntonioMHs[i], "Aged, 80 and over") == 0) ||
               (strcmp(AntonioMHs[i], "Child, Preschool") == 0) ||
               (strcmp(AntonioMHs[i], "Infant") == 0) ||
               (strcmp(AntonioMHs[i], "Middle Aged") == 0) ||
               (strcmp(AntonioMHs[i], "Young Adult") == 0) ||
               (strcmp(AntonioMHs[i], "Humans") == 0) ||
               (strcmp(AntonioMHs[i], "Male") == 0) ||
               (strcmp(AntonioMHs[i], "Female") == 0) ||
               (strcmp(AntonioMHs[i], "Swine") == 0))
            {
                MHpos = search_indexMH(AntonioMHs[i]);
                if(RTM_Debug && (MHpos > -1))
                {
                    fprintf(fout, "Ignoring Antonio ML MH: \"%s\" NF1\n",
                                mt_table[MHpos].mh);
                    fflush(fout);
                } /* fi */
                MLPicks.MHstatus[i] = FALSE;
                MLPicks.found[i] = TRUE;
            } /* fi */
        } /* fi */
    } /* for */

    /* Check on what we are going to add and see if we need to
       add Animals or add Humans, or filter out the Aged Related
       Terms.  (see check_CTs doAgedReview section
    */

    for(i = 0; i < NUM_ANTONIO_MHs; i++)
    {
        if(MLPicks.MHstatus[i] && !MLPicks.found[i])
        {
            if(strcmp(AntonioMHs[i], "Swine") == 0)
              Animal_Added = TRUE;
        } /* fi */
    } /* for */

    /* If we have a veterinary journal, add Animals */

    if(veterinaryJournal && !Animals_found)
      Animal_Added = TRUE;

    if(Animal_Added && !Animals_found)
    {
        process_checktags(CT_list[Animals].Name, MMI, CT_list[Animals].DUI,
                          "Forced - Antonio ML Rules", "Forced - Antonio ML Rules");

        if(RTM_Debug)
        {
            fprintf(fout, "Forcing Animals - Antonio ML Rules\n");
            fflush(fout);
        } /* fi */
    } /* fi */

    /* Now see what we have and make sure we don't add age related checktags
      if we have Animals set and Humans not set.
    */

    if((Animal_Added || Animals_found) && !Humans_found)
    {
        for(i = 0; i < NUM_ANTONIO_MHs; i++)
        {
            if(MLPicks.MHstatus[i] && !MLPicks.found[i])
            {
                if((strcmp(AntonioMHs[i], "Adolescent") == 0) ||
                   (strcmp(AntonioMHs[i], "Adult") == 0) ||
                   (strcmp(AntonioMHs[i], "Aged") == 0) ||
                   (strcmp(AntonioMHs[i], "Aged, 80 and over") == 0) ||
                   (strcmp(AntonioMHs[i], "Child, Preschool") == 0) ||
                   (strcmp(AntonioMHs[i], "Infant") == 0) ||
                   (strcmp(AntonioMHs[i], "Middle Aged") == 0) ||
                   (strcmp(AntonioMHs[i], "Young Adult") == 0))
                {
                    MHpos = search_indexMH(AntonioMHs[i]);
                    if(RTM_Debug && (MHpos > -1))
                    {
                        fprintf(fout,
                                "Ignoring Antonio ML MH: \"%s\" doAgedReview\n",
                                mt_table[MHpos].mh);
                        fflush(fout);
                    } /* fi */
                    MLPicks.MHstatus[i] = FALSE;
                    MLPicks.found[i] = TRUE;
                } /* fi */
            } /* fi */
        } /* for */
    } /* fi */

    /* If Animals is not set, and we have age related checktags
       recommended, we need to add Humans.
    */

    else if(!Humans_found) /* See if we need to add Humans */
    {
        for(i = 0; i < NUM_ANTONIO_MHs; i++)
        {
            if(MLPicks.MHstatus[i] && !MLPicks.found[i])
            {
                if((strcmp(AntonioMHs[i], "Adolescent") == 0) ||
                   (strcmp(AntonioMHs[i], "Adult") == 0) ||
                   (strcmp(AntonioMHs[i], "Aged") == 0) ||
                   (strcmp(AntonioMHs[i], "Aged, 80 and over") == 0) ||
                   (strcmp(AntonioMHs[i], "Child, Preschool") == 0) ||
                   (strcmp(AntonioMHs[i], "Infant") == 0) ||
                   (strcmp(AntonioMHs[i], "Middle Aged") == 0) ||
                   (strcmp(AntonioMHs[i], "Young Adult") == 0))
                  Aged_Related_Added = TRUE;
            } /* fi */
        } /* for */

        if(Aged_Related_Added)
        {
            process_checktags(CT_list[Human].Name, MMI, CT_list[Human].DUI,
                              "Forced - Antonio ML Rules", "Forced - Antonio ML Rules");

            if(RTM_Debug)
            {
                fprintf(fout, "Forcing Humans - Antonio ML Rules\n");
                fflush(fout);
            } /* fi */
        } /* fi */
    } /* else */

    /* ***************************************************************
       Need to add logic to NOT recommend Male/Female when Plant related
       article is being indexed.  Indexer Feedback: 2955
    */

    /* Now, see what we have left to add */

    for(i = 0; i < NUM_ANTONIO_MHs; i++)
    {
/* JGM - 2/5/16: Added removal of Aged from this insertion point (14.31% right)
        if(MLPicks.MHstatus[i] && !MLPicks.found[i])
*/
        if(MLPicks.MHstatus[i] && !MLPicks.found[i] &&
              (strcmp(AntonioMHs[i], "Aged") != 0))
        {
            /* Get the DUI for this MH */

            strcpy(dui, "");
            foo = findMHDUI(AntonioMHs[i]);
            strcpy(dui, foo);
            free(foo);

            /* See if journal typically uses CT */

            if(strlen(dui) > 0)
            {
                if(RTM_Debug)
                {
                    fprintf(fout, "VocabDen - Checking AML: ");
                    fprintf(fout, "%s|%s|%s|\n", nlmID, dui, AntonioMHs[i]);
                    fflush(fout);
                } /* fi */

                foo = getVocabDen(nlmID, dui);
                if(strlen(foo) == 0)
                {
                    strcpy(dui, "");
                    if(RTM_Debug)
                    {
                        fprintf(fout, "Removing AML MH '%s'", AntonioMHs[i]);
                        fprintf(fout, " Vocab Density\n");
                        fflush(fout);
                    } /* fi */

                    if(RTM_Debug)
                      fprintf(fout, "VOCAB AML|%s|%s|\n", muid, AntonioMHs[i]);
                } /* fi */

                free(foo);
            } /* fi */

            /* If we still have a DUI, we can add the term */

            if(strlen(dui) > 0)
            {
                if(RTM_Debug)
                {
                    fprintf(fout, "Adding: \"%s\" -- Antonio ML Rules\n", AntonioMHs[i]);
                    fflush(fout);
                } /* fi */

                if(isCT(dui))
                  process_checktags(AntonioMHs[i], MMI, dui,
                                   "Forced - Antonio ML Rules", "Forced - Antonio ML Rules");
                else
                {
                    process_mterm(FALSE, "", AntonioMHs[i], MMI, 1000.0, MH, "", "",
                                  "Forced - Antonio ML Rules", dui, "", "", TRUE);

                    index = search_index(dui);
                    if((index >= 0) && (mt_table[index].score == 0))
                      mt_table[index].score += 1000;
                } /* else */
            } /* fi */

            strcpy(dui, "");
            n = 0;
        } /* fi */
    } /* for */
} /* AntonioCheck */


/***************************************************************************
*
*  checkPregnancyMammalOnly --
*
*      This 
*
***************************************************************************/

void checkPregnancyMammalOnly()
{
    int foundPregnancy, foundNonMammal, foundMammal;
    long i, k, mtPregPos, ctPregPos;
    char trigger[SMALL_LINE + 1];

    /* Check for "Pregnancy" CheckTag

    */

    strcpy(trigger, "");
    foundPregnancy = FALSE;
    mtPregPos = ctPregPos = -1;
    for(i = 0; !foundPregnancy && (i < mt_count); i++)
    {
        if(mt_table[i].mh[0] == 'P')
        {
            if(strcmp(mt_table[i].mh, "Pregnancy") == 0)
            {
                mtPregPos = i;
                foundPregnancy = TRUE;
            } /* fi */
        } /* fi */
    } /* for */

    if(!foundPregnancy)
    {
        for(i = 0; !foundPregnancy && (i < ct_count); i++)
        {
            if(ct_table[i].mh[0] == 'P')
            {
                if(strcmp(ct_table[i].mh, "Pregnancy") == 0)
                {
                    ctPregPos = i;
                    foundPregnancy = TRUE;
                } /* fi */
            } /* fi */
        } /* for */
    } /* fi */

    if(foundPregnancy)
    {
        foundNonMammal = foundMammal = FALSE;
        for(i = 0; i < mt_count; i++)
        {
            if(mt_table[i].hasPrinted)
            {
                if((mt_table[i].datatype == MH) || (mt_table[i].datatype == CT))
                {
                    for(k = 0; k < mt_table[i].num_treecodes; k++)
                    {
                        if(mt_table[i].treecodes[k][0] == 'B')
                        {
                            if((strstr(mt_table[i].treecodes[k],
/* Amphibians */                       "B01.050.150.900.090") != NULL) ||
                               (strstr(mt_table[i].treecodes[k],
/* Bird */                             "B01.050.150.900.248") != NULL) ||
                               (strstr(mt_table[i].treecodes[k],
/* Fishes */                           "B01.050.150.900.493") != NULL) ||
                               (strstr(mt_table[i].treecodes[k],
/* Reptiles */                         "B01.050.150.900.833") != NULL) ||
                               (strstr(mt_table[i].treecodes[k],
/* Invertebrates */                    "B01.050.500") != NULL) ||
                               (strstr(mt_table[i].treecodes[k],
/* Plants */                           "B01.650") != NULL) ||
                               (strstr(mt_table[i].treecodes[k],
/* Fungi */                            "B01.300") != NULL))
                            {
                                strcpy(trigger, mt_table[i].treecodes[k]);
                                foundNonMammal = TRUE;
                            } /* fi */

                            else if(strstr(mt_table[i].treecodes[k],
/* Mammals */                       "B01.050.150.900.649") != NULL)
                              foundMammal = TRUE;
                        } /* fi */
                    } /* for */
                } /* fi MH */
            } /* fi printed */
        } /* for each mt_table */

        if(foundNonMammal && !foundMammal && (mtPregPos > -1))
        {
            strcpy(mt_table[mtPregPos].mh, "Removed CT");
            mt_table[mtPregPos].safe = FALSE;
            mt_table[mtPregPos].oktoprint = FALSE;
            mt_table[mtPregPos].score = -1;
            mt_table[mtPregPos].forcePrint = FALSE;
            if(RTM_Debug)
            {
                fprintf(fout,
                  "\nRemoving Pregnancy CT - NonMammal Found: %s\n", trigger);
                fflush(fout);
            } /* fi */
        } /* fi */

        else if(foundNonMammal && !foundMammal && (ctPregPos > -1))
        {
            strcpy(ct_table[ctPregPos].mh, "Removed CT");
            ct_table[ctPregPos].oktoprint = FALSE;
            ct_table[ctPregPos].score = -1;
            if(RTM_Debug)
            {
                fprintf(fout,
                  "\nRemoving Pregnancy CT - NonMammal Found 2: %s\n", trigger);
                fflush(fout);
            } /* fi */
        } /* fi */
    } /* fi foundPregnancy */
} /* checkPregnancyMammalOnly */



/***************************************************************************
*
*  doReviewMTIFLCheck --
*
*      This 
*
***************************************************************************/

void doReviewMTIFLCheck()
{
    int ok, Human_found, found_AgeRelated, okToContinue;
    long i, j;

    /* The first thing we want to do is go through and remove all of the
       checktags that have been added that are NOT Humans or Animals since
       this is a MTIFL Review article.

       9/18/12 emails

       7/11/13 - Decided on minor change that will allow other CheckTags
                to remain if they are in the Title of the article.

                Also, if "ovarian cancer" is found in the Title, we can
                leave Female.

       4/20/17 - Allowing if we have trigger only in abstract.  The
                 goal is to remove CTs without visible triggers.

       12/21/17 - Make sure we don't allow Age related checktags when 
                Humans was not being recommended.
    */


    /* Check the ct_table first */

    Human_found = FALSE;
    for(i = 0; i < ct_count; i++)
    {
        if(strcmp(ct_table[i].mh, CT_list[Human].Name) == 0)
        {
            if(ct_table[i].oktoprint)
              Human_found = TRUE;
        } /* fi */
    } /* for each CT */

    /* If we don't find Humans there, then check mt_table */

    if(!Human_found)
    {
        for(i = 0; i < mt_count; i++)
        {
            if(mt_table[i].datatype == CT)
            {
                if(strcmp(mt_table[i].mh, CT_list[Human].Name) == 0)
                {
                    if(mt_table[i].oktoprint)
                      Human_found = TRUE;
                } /* fi */
            } /* fi CT */
        } /* for each MH */
    } /* fi */

    /* Go through the mt_table */

    for(i = 0; i < mt_count; i++)
    {
        if(mt_table[i].datatype == CT)
        {
            ok = TRUE;

            if((strcmp(mt_table[i].mh, CT_list[Human].Name) != 0) &&
               (strcmp(mt_table[i].mh, CT_list[Animals].Name) != 0))
            {
                 ok = FALSE;
                 okToContinue = TRUE;

                 /* Check for Age related and not Humans */

                 if(!Human_found)
                 {
                     found_AgeRelated = FALSE;
                     if((strcmp(mt_table[i].mh,
                                         CT_list[Adolescent].Name) == 0) ||
                        (strcmp(mt_table[i].mh, CT_list[Adult].Name) == 0) ||
                        (strcmp(mt_table[i].mh, CT_list[Aged].Name) == 0) ||
                        (strcmp(mt_table[i].mh, CT_list[Child].Name) == 0) ||
                        (strcmp(mt_table[i].mh, CT_list[Infant].Name) == 0) ||
                        (strcmp(mt_table[i].mh, CT_list[Newborn].Name) == 0) ||
                        (strcmp(mt_table[i].mh, CT_list[Aged80].Name) == 0) ||
                        (strcmp(mt_table[i].mh, 
                                         CT_list[ChildPreschool].Name) == 0) ||
                        (strcmp(mt_table[i].mh,
                                         CT_list[MiddleAged].Name) == 0) ||
                        (strcmp(mt_table[i].mh, CT_list[YoungAdult].Name) == 0))
                       found_AgeRelated = TRUE;

                     if(found_AgeRelated)
                       okToContinue = FALSE;
                 } /* fi !Human */

                 if(okToContinue)
                 {
                     if(strstr(mt_table[i].textloc, "TI") != NULL)
                     {
                         ok = TRUE;

                         if(RTM_Debug)
                         {
                             fprintf(fout,
                            "Allowing MTIFL CheckTag: \"%s\" Found in Title\n",
                                mt_table[i].mh);
                             fflush(fout);
                         } /* fi */
                     } /* fi */

                     else if(strstr(mt_table[i].textloc, "AB") != NULL)
                     {
                         ok = TRUE;

                         if(RTM_Debug)
                         {
                             fprintf(fout,
                          "Allowing MTIFL CheckTag: \"%s\" Found in Abstract\n",
                             mt_table[i].mh);
                             fflush(fout);
                         } /* fi */
                     } /* else fi */

                     else if(strcmp(mt_table[i].mh, "Female") == 0)
                     {
                         if(foundInText("ovarian cancer", FALSE, TRUE) ||
                            foundInText("ovarian neoplasm", FALSE, TRUE) ||
                            foundInText("gynecologic", FALSE, FALSE))
                         {
                             ok = TRUE;

                             if(RTM_Debug)
                             {
                                 fprintf(fout,
                                         "Allowing MTIFL CheckTag: \"%s\" ",
                                         mt_table[i].mh);
                                 fprintf(fout,
                                         "Ovarian Cancer found in Title ");
                                 fprintf(fout,
                                         "or gynecologic found in article\n");
                                 fflush(fout);
                             } /* fi */
                         } /* fi */
                     } /* else fi */
                 } /* fi okToContinue */
            } /* fi */

            if(!ok)
            {
                mt_table[i].safe = FALSE;
                mt_table[i].oktoprint = FALSE;
                mt_table[i].forcePrint = FALSE;

                if(RTM_Debug)
                {
                    fprintf(fout,
                    "Removing CheckTag 1: \"%s\" (%s) (%s) MTIFL Review Rule\n",
                            mt_table[i].mh, mt_table[i].textloc,
                            mt_table[i].trigger);
                    fflush(fout);
                } /* fi */
            } /* fi */
        } /* fi CT */
    } /* for each recommendation */

    /* Check ct_table */

    for(i = 0; i < ct_count; i++)
    {
        if((strcmp(ct_table[i].mh, CT_list[Human].Name) != 0) &&
           (strcmp(ct_table[i].mh, CT_list[Animals].Name) != 0))
        {
            ct_table[i].oktoprint = FALSE;

            if(RTM_Debug)
            {
                fprintf(fout, "Removing CheckTag 2: \"%s\" MTIFL Review Rule\n",
                        ct_table[i].mh);
                fflush(fout);
            } /* fi */
        } /* fi */
    } /* for each CT */
} /* doReviewMTIFLCheck */


/***************************************************************************
*
*  doForcedMiceRats --
*
*      This 
*
***************************************************************************/

void doForcedMiceRats()
{
    char from[SMALL_LINE + 1], lookFor[SMALL_LINE + 1], tmp[SMALL_LINE + 1],
         rest[SMALL_LINE + 1];
    int okMice, okRats, ok;
    long AApos, i, index;

    okMice = foundInText("mice", FALSE, FALSE);
    if(!okMice)
      okMice = foundInText("mouse", FALSE, FALSE);

    okRats = foundInText("rats", FALSE, FALSE);
    if(!okRats)
      okRats = foundInText("rat", FALSE, FALSE);

    /* Checks that don't need X'd out text to look in the text */

    if(okMice || okRats)
    {
        for(i = 0; i < numMR; i++)
        {
           strcpy(tmp, MiceRats[i].AAlist);
           if(((MiceRats[i].whichType[0] == 'r') && okRats) ||
              ((MiceRats[i].whichType[0] == 'm') && okMice))
           {
               while(strlen(tmp) > 0)
               {
                    strcpy(lookFor, "");
                    strcpy(rest, "");
                    sscanf(tmp, "%[^;];%[^\n]", lookFor, rest);
                    strcpy(tmp, rest);

                    /* See if lookFor is a AA or not, remove if is */

                    AApos = -1;
                    AApos = checkAAsLong(lookFor, MiceRats[i].longForm);
                    if(AApos != -1)
                    {
                        if(RTM_Debug)
                        {
                            fprintf(fout,"Ignoring Rat/Mice Trigger - AA: %s\n",
                                    lookFor);
                            fflush(fout);
                        } /* fi */
                        strcpy(lookFor, "");
                    } /* fi AApos */

                    /* If lookFor is wistar, make sure we don't
                       have Wistar Furth or Wistar Kyoto.

                       Also, Long-Evans (Long-Evans Cinnamon
                       and Otsuka-Long-Evans-Tokushima Fatty
                    */

                    if((strlen(lookFor) > 0) &&
                       (strcmp(lookFor, "Wistar") == 0))
                    {
                        if(foundInText("WF", TRUE, FALSE) ||
                           foundInText("WKY", TRUE, FALSE) ||
                           foundInText("Wistar Kyoto", TRUE, FALSE) ||
                           foundInText("Wistar-Kyoto", TRUE, FALSE) ||
                           foundInText("Wistar Furth", TRUE, FALSE) ||
                           foundInText("Wistar-Furth", TRUE, FALSE))
                        {
                            if(RTM_Debug)
                            {
                                fprintf(fout, "Ignoring Rat/Mice");
                                fprintf(fout, " Trigger - WF/WKY: %s\n",
                                        lookFor);
                                fflush(fout);
                            } /* fi */
                            strcpy(lookFor, "");
                        } /* fi WF or WKY */
                    } /* fi Wistar */

                    /* If lookFor is Long-Evans (Long-Evans Cinnamon
                       and Otsuka-Long-Evans-Tokushima Fatty
                    */

                    if((strlen(lookFor) > 0) &&
                       ((strcmp(lookFor, "Long-Evans") == 0) ||
                        (strcmp(lookFor, "Long Evans") == 0)))
                    {
                        if(foundInText("LEC", TRUE, FALSE) ||
                           foundInText("OLETF", TRUE, FALSE) ||
                           foundInText("Long-Evans Cinnamon", TRUE, FALSE) ||
                           foundInText("Long Evans Cinnamon", TRUE, FALSE) ||
                           foundInText("Otsuka-Long-Evans-Tokushima",
                                       TRUE, FALSE) ||
                           foundInText("Otsuka Long-Evans Tokushima",
                                       TRUE, FALSE) ||
                           foundInText("Otsuka Long Evans Tokushima",
                                       TRUE, FALSE))
                        {
                            if(RTM_Debug)
                            {
                                fprintf(fout, "Ignoring Rat/Mice");
                                fprintf(fout, " Trigger - LEC/OLETF: %s\n",
                                        lookFor);
                                fflush(fout);
                            } /* fi */
                            strcpy(lookFor, "");
                        } /* fi WF or WKY */
                    } /* fi Long-Evans */

                    /* If lookFor is BB */

                    if((strlen(lookFor) > 0) &&
                       (strcmp(lookFor, "BB") == 0))
                    {
                        if(foundInText("PDGF-BB", TRUE, FALSE) ||
                           foundInText("platelet-derived growth factor-bb",
                                       FALSE, FALSE) ||
                           foundInText("platelet-derived growth factor bb",
                                       FALSE, FALSE) ||
                           foundInText("platelet derived growth factor bb",
                                       FALSE, FALSE) ||
                           foundInText("platelet derived growth factor-bb",
                                       FALSE, FALSE))
                        {
                            if(RTM_Debug)
                            {
                                fprintf(fout, "Ignoring Rat/Mice");
                                fprintf(fout, " Trigger - BB: %s\n", lookFor);
                                fflush(fout);
                            } /* fi */
                            strcpy(lookFor, "");
                        } /* fi BB */
                    } /* fi Rats, Inbred BB */

                    /* If lookFor is SD */

                    if((strlen(lookFor) > 0) &&
                       (strcmp(lookFor, "SD") == 0))
                    {
                        if(foundInText("sd rat", FALSE, FALSE) ||
                           foundInText("sd rats",FALSE, FALSE))
                          ok = TRUE;

                        else
                        {
                            if(RTM_Debug)
                            {
                                fprintf(fout, "Ignoring Rat/Mice");
                                fprintf(fout, " Trigger - SD: %s\n", lookFor);
                                fflush(fout);
                            } /* fi */
                            strcpy(lookFor, "");
                        } /* fi SD */
                    } /* fi Rats, Sprague-Dawley */

                    /* If lookFor is NOD */

                    if((strlen(lookFor) > 0) &&
                       (strcmp(lookFor, "NOD") == 0))
                    {
                        if(foundInText("nod-like", FALSE, FALSE) ||
                           foundInText("nod like", FALSE, FALSE) ||
                           foundInText("(nod)-like", FALSE, FALSE) ||
                           foundInText("(nod) like", FALSE, FALSE) ||
                           foundInText("nod-1", FALSE, FALSE) ||
                           foundInText("nod 1", FALSE, FALSE) ||
                           foundInText("nod-2", FALSE, FALSE) ||
                           foundInText("nod 2", FALSE, FALSE) ||
                           foundInText("nod scid", FALSE, FALSE) ||
                           foundInText("nod-agonist", FALSE, FALSE) ||
                           foundInText("nod agonist", FALSE, FALSE) ||
                           foundInText("nod-factor", FALSE, FALSE) ||
                           foundInText("nod factor", FALSE, FALSE) ||
                           foundInText("nod-mediated", FALSE, FALSE) ||
                           foundInText("nod mediated", FALSE, FALSE) ||
                           foundInText("nod-dependent", FALSE, FALSE) ||
                           foundInText("nod dependent", FALSE, FALSE) ||
                           foundInText("nod-independent", FALSE, FALSE) ||
                           foundInText("nod independent", FALSE, FALSE) ||
                           foundInText("nod-signaling", FALSE, FALSE) ||
                           foundInText("nod signaling", FALSE, FALSE) ||
                           foundInText("oligomerization", FALSE, FALSE) ||
                           foundInText("nitric oxide", FALSE, FALSE) ||
                           foundInText("nod-rip2", FALSE, FALSE) ||
                           !foundInText("NOD", TRUE, FALSE))
                        {
                            if(RTM_Debug)
                            {
                                fprintf(fout, "Ignoring Rat/Mice");
                                fprintf(fout, " Trigger - NOD: %s\n", lookFor);
                                fflush(fout);
                            } /* fi */
                            strcpy(lookFor, "");
                        } /* fi NOD */
                    } /* fi Mice, Inbred NOD */

                    /* Survived filter, now, check to see if we have it */

                    if(strlen(lookFor) > 0)
                    {
                        if(foundInText(lookFor, TRUE, FALSE))
                        {
                            strcpy(tmp, "");  /* Don't need rest */
                            if(RTM_Debug)
                            {
                                fprintf(fout,
                                        "Adding Forced Mice/Rat: %s (%s)\n",
                                        MiceRats[i].mh, lookFor);
                                fflush(fout);
                            } /* fi */

                            sprintf(from, "Forced Mice/Rats:%s\0", lookFor);

                            if(MiceRats[i].DUI[0] == 'D')
                              process_mterm(FALSE, "", MiceRats[i].mh,
                                     MMI, 1000.0, MH, "TI", "", from,
                                     MiceRats[i].DUI, "", "", TRUE);

                            else /* CDNF protein, rat */
                              process_mterm(FALSE, "", MiceRats[i].mh,
                                       MMI, 1000.0, NM, "TI", "", from,
                                       MiceRats[i].DUI, "", "", TRUE);

                            index = search_index(MiceRats[i].DUI);
                            if((index >= 0) && (mt_table[index].score == 0))
                              mt_table[index].score += 1000;

                            if(MiceRats[i].whichType[0] == 'm')
                              process_checktags(CT_list[Mice].Name, MMI, CT_list[Mice].DUI,
                                                from, from);

                            else
                              process_checktags(CT_list[Rats].Name, MMI, CT_list[Rats].DUI,
                                                from, from);
                        } /* fi foundInText lookFor */
                    } /* fi strlen(lookFor) */
               } /* while */
           } /* fi foundInText */
        } /* for */
    } /* fi mice/rats article */
} /* doForcedMiceRats */


/***************************************************************************
*
*  isMHExclude --
*
*      This 
*
***************************************************************************/

int isMHExclude(char *term, int flag)
{
   int found, rtn;
   long j;

   found = rtn = FALSE;
   for(j = 0; !found && (j < numMHExcls); j++)
   {
       if(term[0] == MHExcls[j].Term[0])
       {
           if(strcmp(term, MHExcls[j].Term) == 0)
           {
               found = TRUE;
               if(flag)
               {
                   if(MHExcls[j].checkTitle)
                     rtn = FALSE;
                   else
                     rtn = TRUE;
               } /* fi */

               else
                 rtn = TRUE;
           } /* fi */
       } /* fi */
   } /* for */

   return(rtn);
} /* isMHExclude */


/***************************************************************************
*
*  isMHExcludeLC --
*
*      This is for Learning to Rank since it has MHs in lowercase
*
***************************************************************************/

int isMHExcludeLC(char *term, int flag)
{
   int found, rtn;
   long j;

   found = rtn = FALSE;
   for(j = 0; !found && (j < numMHExcls); j++)
   {
       if(strcasecmp(term, MHExcls[j].Term) == 0)
       {
           found = TRUE;
           if(flag)
           {
               if(MHExcls[j].checkTitle)
                 rtn = FALSE;
               else
                 rtn = TRUE;
           } /* fi */

           else
             rtn = TRUE;
       } /* fi */
   } /* for */

   return(rtn);
} /* isMHExcludeLC */


/***************************************************************************
*
*  checkTreecodes_CT --
*
*      This 
*
***************************************************************************/

void checkTreecodes_CT()
{
   int PRC_Only, found, done, ct, q, ok;
   long i, j, k;

   for(j = 0; j < mt_count; j++)
   {
      if(mt_table[j].hasPrinted && (mt_table[j].datatype != CT))
      {
/*  JGM 3/25/10 test to see if removing CT recommendations for PRC Only
    recommendations might help.  Taken out after test since it only helped
    slightly.

         if(mt_table[j].paths[PRC] && !mt_table[j].paths[MMI] &&
            !mt_table[j].forcePrint)
           PRC_Only = TRUE;
         else

     Adding back in - 080311 JGM with slight limitation treating these
     like CTs.
*/
         PRC_Only = FALSE;
         if(mt_table[j].paths[PRC] && !mt_table[j].paths[MMI] &&
            !mt_table[j].forcePrint)
         {
             if((strcmp(mt_table[j].mh, "Swine") == 0) ||
                (strcmp(mt_table[j].mh, "Horses") == 0) ||
                (strcmp(mt_table[j].mh, "Sheep") == 0))
               PRC_Only = TRUE;
         } /* fi */

         if(RTM_Debug)
         {
            fprintf(fout, "check_TCTs - mt_table[%ld].mh: #%s#  PRCOnly: %d\n",
                    j, mt_table[j].mh, PRC_Only);
            fflush(fout);
         } /* fi */

         /* Now check on the CTs */

         for(i = 0; !PRC_Only && (i < mt_table[j].num_treecodes); i++)
         {
             if(RTM_Debug)
             {
                fprintf(fout, "   %s\n", mt_table[j].treecodes[i]);
                fflush(fout);
             } /* fi */

             CTIpos = CTIcharStartPos[mt_table[j].treecodes[i][0] - 'A'];

             /* Don't want to include the following treecodes:

                Introduced Species since it can be Plants or Animals
             */

             if((strcmp(mt_table[j].treecodes[i], "B01.050.050.580") == 0) ||
                (strstr(mt_table[j].treecodes[i], "C22.674.710") != NULL) ||
                (strcmp(mt_table[j].treecodes[i], "G16.500.275.157.049.400") == 0) ||
                (strcmp(mt_table[j].treecodes[i], "N06.230.124.049.400") == 0))
               CTIpos = -1;


             if(CTIpos == -1)
             {
                if(RTM_Debug)
                {
                    fprintf(fout, "   -- No Treecodes to Check\n");
                    fflush(fout);
                } /* fi */
             } /* fi */

             else
             {
                foundCTIpos = -1;
                found = FALSE;
                done = FALSE;

                for(k = CTIpos; !done && (k < numCTIs); k++)
                {
/*
                    if(CTInfo[k].Treecode[0] != mt_table[j].treecodes[i][0])

                    if(strcmp(CTInfo[k].Treecode, "M01.060") == 0)
                      done = TRUE;

*/
                    if(CTInfo[k].Treecode[0] != mt_table[j].treecodes[i][0])
                      done = TRUE;

                    else
                    {
                        ok = FALSE;
                        if(CTInfo[k].expand)
                        {
                            if(strstr(mt_table[j].treecodes[i],
                                      CTInfo[k].Treecode) != NULL)
                            {
                                ok = TRUE;
                                if(RTM_Debug)
                                  fprintf(fout, "   -- Have Expand Match %s\n",
                                          CTInfo[k].Treecode);
                            } /* fi */
                        } /* fi */

                        else
                        {
                            if(strcmp(mt_table[j].treecodes[i],
                                      CTInfo[k].Treecode) == 0)
                            {
                                ok = TRUE;
                                if(RTM_Debug)
                                  fprintf(fout, "   -- Have Exact Match %s\n",
                                          CTInfo[k].Treecode);
                            } /* fi */
                        } /* else */

                        if(ok)
                        {
                           found = TRUE;
                           foundCTIpos = k;
                               if(RTM_Debug)
                                 fprintf(fout,"      -- Add following CT(s): ");

                           for(q = 0; q < CTInfo[k].numCTs; q++)
                           {
                               if(RTM_Debug && (q > 0))
                                 fprintf(fout, ";");
                               ct = CTInfo[k].CTs[q];
                               if(RTM_Debug)
                                  fprintf(fout, "%s", CT_list[ct].Name);

                               if(new_CTs < MAXCTIs)
                               {
                                  if(strstr(mt_table[j].textloc, "TI") != NULL)
                                    CTposFromTI[new_CTs] = TRUE;

                                  else if(strstr(mt_table[j].textloc,
                                                 "AB") != NULL)
                                    CTposFromAB[new_CTs] = TRUE;

                                  if(mt_table[j].forcePrint)
                                    CTposFromTI[new_CTs] = TRUE;

                                  fromCTpos[new_CTs] = ct;
                                  fromMHList[new_CTs] = j;
                                  sprintf(triggers[new_CTs], 
                                         "CT Treecode Lookup: %s (%s)\0",
                                         CTInfo[k].Treecode, mt_table[j].mh);
                                  new_CTs++;
                               } /* fi */

                               else
                               {
                                  fprintf(stderr, "Too many CTs!\n");
                                  fflush(stderr);
                               } /* else */
                           } /* for */

                           if(RTM_Debug)
                           {
                              fprintf(fout, "\n");
                              fflush(fout);
                           } /* fi */
                        } /* fi ok */
                    } /* else */
                } /* for each CTI treecode k */

                if(!found && RTM_Debug)
                {
                   fprintf(fout, "   -- No Treecode Match Found\n");
                   fflush(fout);
                } /* fi */
             } /* else */
         } /* for each treecode from term */
      } /* fi */
   } /* for each Term in topN */
} /* checkTreecodes_CT */


/***************************************************************************
*
*  checkText_Triggers --
*
*      This 
*
***************************************************************************/

void checkText_Triggers(int okMice, int okRats)
{
   int n, r, ct, ok, hasNewborn, isAmbig, ageRelatedFound, animalRelatedStatus;
   long q, localFoundPos, localFoundLen, index, textStartPos;
   char PIinfo[1000], ambigTrigger[SMALL_LINE + 1];

   animalRelatedStatus = isAnimalRelated();
   for(q = 0; q < numTCTs; q++)
   {
       isAmbig = ageRelatedFound = hasNewborn = FALSE;
       strcpy(ambigTrigger, "");
       strcpy(PIinfo, "");
       localFoundPos = localFoundLen = txtStartPos = txtLen = textStartPos = -1;
       if(foundInText(TermCTs[q].lcTerm, FALSE, FALSE))
       {
           if((txtStartPos > -1) && (txtLen > -1))
           {
               sprintf(PIinfo, "%ld^%ld^0\0", txtStartPos, txtLen);
               textStartPos = txtStartPos;
           } /* fi */

           localFoundPos = fITpos;
           localFoundLen = fITlen;
           hasNewborn = ageRelatedFound = FALSE;
           for(n = 0; n < TermCTs[q].numCTs; n++)
           {
               ct = TermCTs[q].CTs[n];
               if((ct == Adolescent) || (ct == Adult) || (ct == Aged) || 
                  (ct == Child) || (ct == Infant) || (ct == Newborn) ||
                  (ct == Aged80) || (ct == ChildPreschool) ||
                  (ct == MiddleAged) || (ct == YoungAdult))
                 ageRelatedFound = TRUE;

               if(ct == Newborn)
                 hasNewborn = TRUE;
           } /* for */

           /* NOTE: Make sure to update MTI.c Special_FELIS with any changes */
           /* (cat) might be embedded in a chemical, so check strstr */

           ok = TRUE;

           /* One unified check for terms */

           if(RTM_Debug)
           {
               fprintf(fout, "checkText_Triggers - #%s# ageRelatedFound: %d  isAnimalContext: %d\n",
                       TermCTs[q].lcTerm, ageRelatedFound, animalRelatedStatus);
               fflush(fout);
           } /* fi */

           if(ageRelatedFound && animalRelatedStatus)
             ok = FALSE;

           else
             ok = isLookForOK(TermCTs[q].lcTerm);

           if(RTM_Debug)
           {
               fprintf(fout, "checkText_Triggers - LookFor: #%s#  OK: %d\n",
                       TermCTs[q].lcTerm, ok);
               fflush(fout);
           } /* fi */

           if(ok && hasNewborn)
           {
               if(CT_list[Animals].already_used ||
                 (foundInText("mice", FALSE, FALSE)) ||
                  (foundInText("rat", FALSE, FALSE)) ||
                  (foundInText("rabbit", FALSE, FALSE)) ||
                  (foundInText("marsupial", FALSE, FALSE)))
               {
                   ok = FALSE;
                   if(RTM_Debug)
                   {
                       fprintf(fout,
                         "   -- Ignored 'Infant, Newborn' CT Animal Already\n");
                       fflush(fout);
                   } /* fi */
               } /* fi */
           } /* fi */

           if(ok && (ct == Human) && foundNonHumanTrigger)
           {
               ok = FALSE;
               if(RTM_Debug)
               {
                   fprintf(fout, "   -- Ignored 'Humans' CT Animal Trigger Found\n");
                   fflush(fout);
               } /* fi */
           } /* fi */

           if(ok && (!okMice || !okRats))
           {
               for(r = 0; r < TermCTs[q].numCTs; r++)
               {
                   ct = TermCTs[q].CTs[r];

                   if((ct == Mice) || (ct == Rats))
                   {
                       if((ct == Mice) && (!okMice))
                         ok = FALSE;

                       if((ct == Rats) && (!okRats))
                         ok = FALSE;
                   } /* fi */
               } /* for */

               if(!ok)
               {
                   if(RTM_Debug)
                   {
                       for(r = 0; r < TermCTs[q].numCTs; r++)
                       {
                           ct = TermCTs[q].CTs[r];
                           fprintf(fout,
                            "   -- NOT Adding CT Text Lookup (Mice/Rats): %s\n",
                            CT_list[ct].Name);
                       } /* for */
                       fflush(fout);
                   } /* fi */
               } /* fi */
           } /* fi */

           /* Check for non-pregnant versus pregnant */

           if(ok)
           {
               for(r = 0; ok && (r < TermCTs[q].numCTs); r++)
               {
                   ct = TermCTs[q].CTs[r];
                   if(ct == Pregnancy)
                   {
/*
                       if((foundInText("non-pregnant", FALSE, FALSE)) ||
                          (foundInText("non pregnant", FALSE, FALSE)) ||
                          (foundInText("not pregnant", FALSE, FALSE)))
*/
                       if(!Pregnant_Non_Pregnant_OK ||
                          foundInText("not pregnant", FALSE, FALSE))
                       {
                           ok = FALSE;
                           if(RTM_Debug)
                           {
                               fprintf(fout, "   -- Ignored 'Pregnancy'");
                               fprintf(fout, " - non/not-pregnant\n");
                               fflush(fout);
                           } /* fi */
                       } /* fi */
                   } /* fi */
               } /* for */
           } /* fi */

           /* See if PRC has this potential CT, if not, remove --
              only check Age related CTs -- changed 5/18/15
                   if((ct == Adolescent) || (ct == Adult) || (ct == Aged) || 
                      (ct == Child) || (ct == Infant) || (ct == Newborn) ||
                      (ct == Aged80) || (ct == ChildPreschool) ||
                      (ct == MiddleAged) || (ct == YoungAdult))
              NOTE: Removed this loosening 5/20 after discovering that
                 it is really helping.  Loosening added 3k wrong to 
                 600 correct.
           */

           if(ok && (PW4 > 0.0))
           {
               for(r = 0; r < TermCTs[q].numCTs; r++)
               {
                   ct = TermCTs[q].CTs[r];

                   isAmbig = isAmbigTrig(TermCTs[q].lcTerm);
                   if(isAmbig)
                     sprintf(ambigTrigger, "AMBIG/METAPHOR20:%s\0", TermCTs[q].lcTerm);

                   /* Don't need to check some terms for PRC inclusion -
                      added 4/13/16.

                     If ct was found in title, don't check: 5/17/16
                       -- REMOVED 32.56% correct
                   */

                   if(ct == Mice)
                     ok = TRUE;

                   /* 1/6/17 - If Rabbits and Rabbit(s) in Title, ok) */

                   else if(ct == Rabbits)
                   {
                       if(!isLookForOK("rabbit"))
                        ok = FALSE;

                       else if(foundInText("rabbit", FALSE, TRUE) ||
                               foundInText("rabbits", FALSE, TRUE))
                          ok = TRUE;
                   } /* else fi */

                   /* 7/28/17 - If Chick Embryo in Title, ok) */

                   else if((ct == ChickEmbryo) && 
                           (foundInText("chicken embryo", FALSE, TRUE) ||
                            foundInText("chicken embryos", FALSE, TRUE) ||
                            foundInText("chick embryo", FALSE, TRUE) ||
                            foundInText("chick embryos", FALSE, TRUE)))
                     ok = TRUE;

                   /* 7/28/17 - If Guinea Pigs in Title, ok) */

                   else if((ct == GuineaPigs) && 
                           (foundInText("guinea pig", FALSE, TRUE) ||
                            foundInText("guinea pigs", FALSE, TRUE)))
                     ok = TRUE;

                   /* 7/28/17 - If Sheep in Title, ok) */

                   else if(ct == Sheep)
                   {
                       if(!isLookForOK("sheep"))
                         ok = FALSE;

                       else if(foundInText("sheep", FALSE, TRUE))
                         ok = TRUE;
                   } /* else fi */

                   /* 7/28/17 - If Cattle in Title, ok) */

                   else if(ct == Cattle)
                   {
                       if(!isLookForOK("cattle"))
                        ok = FALSE;

                       else if(foundInText("cattle", FALSE, TRUE))
                        ok = TRUE;
                   } /* else fi */

                   /* 8/14/17 - If Child in Title, ok)
                      10/30/17 - modified to exclude pre-school children
                   */

                   else if(ct == Child)
                   {
                       if(!isLookForOK("child"))
                        ok = FALSE;

                       else if(foundInText("child", FALSE, TRUE) ||
                               foundInText("children", FALSE, TRUE))
                        ok = TRUE;
                   } /* else fi */

                   /* 8/14/17 - If Newborn in Title/AB, ok)
                   */

                   else if(ct == Newborn)
                   {
                       if(!isLookForOK("newborn"))
                        ok = FALSE;

                       else if(foundInText("newborn infant", FALSE, FALSE) ||
                          foundInText("newborn babies", FALSE, FALSE) ||
                          foundInText("newborn baby", FALSE, FALSE) ||
                          foundInText("neonate", FALSE, FALSE) ||
                          foundInText("preterm", FALSE, FALSE) ||
                          foundInText("newborn", FALSE, FALSE))
                        ok = TRUE;
                   } /* else fi */

                   else if(ct == Human)
                   {
                       if(foundNonHumanTrigger)
                       {
                            ok = FALSE;
                            if(RTM_Debug)
                            {
                                fprintf(fout, "   -- Ignored 'Humans' CT Animal Trigger Found2\n");
                                fflush(fout);
                            } /* fi */
                       } /* fi */
                   } /* fi */


                   /* 7/28/17 - If Dogs in Title, ok) */

                   else if(ct == Dogs)
                   {
                       if(!isLookForOK("dogs"))
                        ok = FALSE;

                       else if(foundInText("dog", FALSE, FALSE) || 
                               foundInText("dogs", FALSE, FALSE))
                        ok = TRUE;
                   } /* else fi */

                   else
                   {
                       index = search_indexMH(CT_list[ct].Name);
                       if(index == -1)
                       {
                           if(RTM_Debug)
                           {
                               fprintf(fout, "Ignoring %s - no PRC found\n",
                                       CT_list[ct].Name);
                               fflush(fout);
                           } /* fi */

                           ok = FALSE;
                       } /* fi */

                       else
                       {
                           if(RTM_Debug)
                           {
                               fprintf(fout, "(%s) Found %ld PRC ok?: %d\n",
                                     CT_list[ct].Name, mt_table[index].num_RCs,
                                     mt_table[index].oktoprint);
                               fflush(fout);
                           } /* fi */

                           if(mt_table[index].num_RCs < 1)
                           {
                               if(RTM_Debug)
                               {
                                   fprintf(fout,
                                           "Ignoring %s - zero PRC found\n",
                                           CT_list[ct].Name);
                                   fflush(fout);
                               } /* fi */

                               ok = FALSE;
                           } /* fi */
                       } /* else */
                   } /* else - not excused ct */
               } /* for */
           } /* fi */

           if(ok && (new_CTs < MAXCTIs))
           {
               for(r = 0; r < TermCTs[q].numCTs; r++)
               {
                   ct = TermCTs[q].CTs[r];
                   if(RTM_Debug)
                   {
                       fprintf(fout, "   -- Added CT Text Lookup: %s\n",
                               CT_list[ct].Name);
                       fprintf(fout, "Position: %ld  PI: #%s#\n",
                               new_CTs, PIinfo);
                       fflush(fout);
                   } /* fi */

                   /* See if in Title so we can save later if necessary */

                   if(foundInText(TermCTs[q].lcTerm, FALSE, TRUE))
                     CTposFromTI[new_CTs] = TRUE;
                   else
                   {
                       CTposFromAB[new_CTs] = TRUE;
                       if(ct == Female)
                       {
                           if(foundInText("ovarian cancer", FALSE, TRUE) ||
                              foundInText("ovarian neoplasm", FALSE, TRUE))
                            CTposFromTI[new_CTs] = TRUE;
                       } /* fi */
                   } /* else */

                   fromCTpos[new_CTs] = ct;
                   fromMHList[new_CTs] = -1;
                   if(isAmbig)
                     sprintf(triggers[new_CTs], "CT Text Lookup: %s;%s\0",
                            TermCTs[q].lcTerm, ambigTrigger);
                   else
                     sprintf(triggers[new_CTs], "CT Text Lookup: %s\0", TermCTs[q].lcTerm);
                   strcpy(positionalInfo[new_CTs], PIinfo);
                   new_CTs++;
               } /* for */
           } /* fi */

           else if(ok)
           {
               fprintf(stderr, "Too many CTs!\n");
               fflush(stderr);
           } /* else */
       } /* fi */
   } /* for */
} /* checkText_Triggers */


/***************************************************************************
*
*  findCT --
*
*      This 
*
***************************************************************************/

int findCT(char *CheckTag)
{
   int i, found, rtn;

   rtn = -1;
   found = FALSE;
   for(i = 0; !found && (i < num_ct); i++)
   {
       if(CheckTag[0] == CT_list[i].Name[0])
       {
           if(strcmp(CheckTag, CT_list[i].Name) == 0)
           {
               found = TRUE;
               rtn = i;
           } /* fi */
       } /* fi */
   } /* for */

   return(rtn);
} /* findCT */


/***************************************************************************
*
*  removeFromBelowLine --
*
*      This 
*
***************************************************************************/

void removeFromBelowLine(char *CheckTag)
{
    long j;
    char tmp[SMALL_LINE + 1];

    for(j = 0; j < numBelow; j++)
    {
        if(strstr(belowLineTmp[j], "|CT") != NULL)
        {
            strcpy(tmp, "");
            sscanf(belowLineTmp[j], "%*[^|]|%*[^|]|%*[^|]|%[^|]", tmp);
            if(strcmp(CheckTag, tmp) == 0)
            {
                strcpy(belowLineTmp[j], "");
                if(RTM_Debug)
                {
                    fprintf(fout, "removeFromBelowLine(%s) removed: %ld\n",
                            CheckTag, j);
                    fflush(fout);
                } /* fi */
            } /* fi */
        } /* fi */
    } /* for */
} /* removeFromBelowLine */


/***************************************************************************
*
*  blockFromMT_Table --
*
*      This 
*
***************************************************************************/

void blockFromMT_Table(char *CheckTag)
{
    long lindex;

    lindex = search_indexMH(CheckTag);
    if(lindex > -1)
    {
        if(mt_table[lindex].oktoprint)
        {
            mt_table[lindex].oktoprint = FALSE;
            if(RTM_Debug)
            {
                fprintf(fout, "blockFromMT_Table(%s) Found - %ld\n", CheckTag, lindex);
                fflush(fout);
            } /* fi */
        } /* fi */
    } /* fi */
} /* blockFromMT_Table */

/***************************************************************************
*
*  amIAnimalCT --
*
*      This 
*
***************************************************************************/

int amIAnimalCT(char *mh)
{
    int rtn;

    rtn = FALSE;

    if((strcmp(mh, CT_list[Animals].Name) == 0) ||
       (strcmp(mh, CT_list[Cats].Name) == 0) ||
       (strcmp(mh, CT_list[Cattle].Name) == 0) ||
       (strcmp(mh, CT_list[Dogs].Name) == 0) ||
       (strcmp(mh, CT_list[Cricetinae].Name) == 0) ||
       (strcmp(mh, CT_list[Mice].Name) == 0) ||
       (strcmp(mh, CT_list[Rabbits].Name) == 0) ||
       (strcmp(mh, CT_list[Rats].Name) == 0) ||
       (strcmp(mh, CT_list[Sheep].Name) == 0) ||
       (strcmp(mh, CT_list[Swine].Name) == 0) ||
       (strcmp(mh, CT_list[ChickEmbryo].Name) == 0) ||
       (strcmp(mh, CT_list[Horses].Name) == 0) ||
       (strcmp(mh, CT_list[Bees].Name) == 0) ||
       (strcmp(mh, CT_list[GuineaPigs].Name) == 0))
      rtn = TRUE;

    return(rtn);
} /* amIAnimalCT */


/***************************************************************************
*
*  clearCTList --
*
*      clear CT_list[].already_used when removing a CT 
*
***************************************************************************/

void clearCTList(char *mh)
{
    int found, okKeep;
    long i;

    found = FALSE;
    for(i = 0; !found && (i < num_ct); i++)
    {
        if(mh[0] == CT_list[i].Name[0])
        {
            if(strcmp(mh, CT_list[i].Name) == 0)
            {
                found = TRUE;
                CT_list[i].already_used = FALSE;

                if(RTM_Debug)
                {
                    fprintf(fout, "Turning off CT_list used: %s\n", mh);
                    fflush(fout);
                } /* fi */
            } /* fi */
        } /* fi */
    } /* for */

    if(amIAnimalCT(mh))
    {
        /* See if any Animals are still marked as used.  If not, turn off */

        okKeep = FALSE;
        for(i = 0; !okKeep && (i < num_ct); i++)
        {
            if((i != Animals) && CT_list[i].already_used)
            {
                if(amIAnimalCT(CT_list[i].Name))
                  okKeep = TRUE;
            } /* fi */
        } /* for */

        if(!okKeep)
        {
            if(RTM_Debug)
            {
                fprintf(fout, "Also Turning off CT_list used: Animals\n");
                fflush(fout);
            } /* fi */
            CT_list[Animals].already_used = FALSE;
        } /* fi */
    } /* fi */
} /* clearCTList */

/***************************************************************************
*
*  checkDina_RegEx --
*
*      This 
*
***************************************************************************/

void checkDina_RegEx()
{
    int ok;
    long i;
    char from[SMALL_LINE + 1];

    if(search_indexMH("Bees") == -1)
    {
        ok = FALSE;
        if(foundInText("honeybees", FALSE, FALSE) ||
           foundInText("bumblebees", FALSE, FALSE))
        {
            ok = TRUE;
            strcpy(from, "Forced Bees RegExp: honey/bumble");
        } /* fi */

        else
        {
            if(foundInText("APIs", TRUE, FALSE) ||
               foundInText("APIS", TRUE, FALSE))
              ok = FALSE;

            else /* Check for Apis something */
            {
                if(foundInText("apis andreniformis", FALSE, FALSE) ||
                   foundInText("apis cerana", FALSE, FALSE) ||
                   foundInText("apis dorsata", FALSE, FALSE) ||
                   foundInText("apis florea", FALSE, FALSE) ||
                   foundInText("apis koschevnikovi", FALSE, FALSE) ||
                   foundInText("apis mellifera", FALSE, FALSE) ||
                   foundInText("apis nearctica", FALSE, FALSE) ||
                   foundInText("apis nigrocincta", FALSE, FALSE))
                {
                    ok = TRUE;
                    strcpy(from, "Forced Bees RegExp: apis");
                } /* fi */
            } /* else */
        } /* else */

        if(ok)
        {
            process_checktags(CT_list[Bees].Name, MMI, CT_list[Bees].DUI, from, from);
        } /* fi */
    } /* fi !Bees */

    if(search_indexMH("Chick Embryo") == -1)
    {
        ok = FALSE;
        if(foundInText("chick", FALSE, FALSE) &&
           (foundInText("embryo", FALSE, FALSE) ||
            foundInText("embryos", FALSE, FALSE)))
        {
            ok = TRUE;
            strcpy(from, "Forced Chick Embryo RegExp");
        } /* fi */

        if(ok)
        {
            process_checktags(CT_list[ChickEmbryo].Name, MMI, CT_list[ChickEmbryo].DUI,
                              from, from);
        } /* fi */
    } /* fi !ChickEmbryo */
} /* checkDina_RegEx */

/***************************************************************************
*
*  buildET --
*
*      This 
*
***************************************************************************/

char *buildET(char *PI)
{
    long i, startPos, len, pos;
    char *rtn, tmp[SMALL_LINE + 1], rest[SMALL_LINE + 1], 
         txtWord[SMALL_LINE + 1], tmp2[SMALL_LINE + 1];

    rtn = (char *)malloc(SMALL_LINE + 1);
    strcpy(rtn, "");

    strcpy(tmp, PI);
    while(strlen(tmp) > 0)
    {
        strcpy(rest, "");
        startPos = len = -1;
        sscanf(tmp, "%ld^%ld^%*d;%[^\n]", &startPos, &len, rest);
        strcpy(tmp, rest);

        pos = 0;
        strcpy(txtWord, "");
        for(i = 0; i < len; i++)
           txtWord[pos++] = citation[i + startPos];
        txtWord[pos] = '\0';

        if(startPos > gTLen)
          sprintf(tmp2, "\"%s\"-ab-0-\"%s\"\0", txtWord, txtWord);
         else
          sprintf(tmp2, "\"%s\"-ti-0-\"%s\"\0", txtWord, txtWord);

         if(strlen(rtn) > 0)
           strcat(rtn, ",");
         strcat(rtn, tmp2);
    } /* while */

    return(rtn);
} /* buildET */


/***************************************************************************
*
*  Final_Treecode_CT_Check --
*
*      This 
*
***************************************************************************/

void Final_Treecode_CT_Check()
{
   int PRC_Only, found, done, ct, p, q, ok, foundPregnancy,
       foundHumans, foundAnimals;
   long i, j, k;
   char *tmp;

   /* Make sure we zero out what CTs are used and then go back through to
      identify them.
   */

   if(RTM_Debug)
   {
       fprintf(fout, "Final_Treecode_CT_Check entered\n");
       fflush(fout);
   } /* fi */

   for(i = 0; i < num_ct; i++)
     CT_list[i].already_used = FALSE;

   foundPregnancy = foundHumans = foundAnimals = FALSE;
   for(i = 0; i < numFinalCTs; i++)
   {
      found = FALSE;
      for(j = 0; !found && (j < num_ct); j++)
      {
          if(finalCTlist[i].oktoprint)
          {
              if(finalCTlist[i].dui[3] == CT_list[j].DUI[3])
              {
                  if(strcmp(finalCTlist[i].dui, CT_list[j].DUI) == 0)
                  {
                      found = TRUE;
                      CT_list[j].already_used = TRUE;
                      if(RTM_Debug)
                      {
                           fprintf(fout, "FOUND %s CT\n", finalCTlist[i].mh);
                           fflush(fout);
                      } /* fi */
                  } /* fi */
              } /* fi */
          } /* fi oktoprint */
      } /* for j */
   } /* for i */

   for(j = 0; j < numFinalMHs; j++)
   {
      if(finalMHlist[j].oktoprint)
      {
         if(RTM_Debug)
         {
             fprintf(fout, "Final_Treecode_CT_Check - MH: %s\n",
                     finalMHlist[j].mh);
            fflush(fout);
         } /* fi */

         /* Now check on the CTs */

         for(i = 0; i < finalMHlist[j].num_treecodes; i++)
         {
             if(RTM_Debug)
             {
                fprintf(fout, "   %s\n", finalMHlist[j].treecodes[i]);
                fflush(fout);
             } /* fi */

             CTIpos = CTIcharStartPos[finalMHlist[j].treecodes[i][0] - 'A'];

             /* Don't want to include the following treecodes:

                Introduced Species since it can be Plants or Animals
             */

             if((strcmp(finalMHlist[j].treecodes[i], "B01.050.050.580") == 0) ||
                (strstr(finalMHlist[j].treecodes[i], "C22.674.710") != NULL) ||
                (strcmp(finalMHlist[j].treecodes[i], "G16.500.275.157.049.400") == 0) ||
                (strcmp(finalMHlist[j].treecodes[i], "N06.230.124.049.400") == 0))
               CTIpos = -1;

             if(CTIpos == -1)
             {
                if(RTM_Debug)
                {
                    fprintf(fout, "   -- No Treecodes to Check\n");
                    fflush(fout);
                } /* fi */
             } /* fi */

             else
             {
                foundCTIpos = -1;
                found = FALSE;
                done = FALSE;

                for(k = CTIpos; !done && (k < numCTIs); k++)
                {
                    if(CTInfo[k].Treecode[0] != finalMHlist[j].treecodes[i][0])
                      done = TRUE;

                    else
                    {
                        ok = FALSE;
                        if(CTInfo[k].expand)
                        {
                            if(strstr(finalMHlist[j].treecodes[i],
                                      CTInfo[k].Treecode) != NULL)
                            {
                                ok = TRUE;
                                if(RTM_Debug)
                                  fprintf(fout, "   -- Have Expand Match %s\n",
                                          CTInfo[k].Treecode);
                            } /* fi */
                        } /* fi */

                        else
                        {
                            if(strcmp(finalMHlist[j].treecodes[i],
                                      CTInfo[k].Treecode) == 0)
                            {
                                ok = TRUE;
                                if(RTM_Debug)
                                  fprintf(fout, "   -- Have Exact Match %s\n",
                                          CTInfo[k].Treecode);
                            } /* fi */
                        } /* else */

                        if(ok)
                        {
                           found = TRUE;
                           foundCTIpos = k;

                           p = 0;
                           for(q = 0; q < CTInfo[k].numCTs; q++)
                           {
                               ct = CTInfo[k].CTs[q];
                               if(veterinaryJournal && (ct == Human))
                               {
                                   if(RTM_Debug)
                                   {
                                       fprintf(fout,
                                         "Skipping adding CT Humans for Veterinary Journal\n");
                                       fflush(fout);
                                   } /* fi */
                               } /* fi */

                               else
                               {
                                   if(!CT_list[ct].already_used)
                                   {
                                       if(RTM_Debug && (p == 0))
                                         fprintf(fout,
                                           "      -- Add following CT(s): ");

                                       if(RTM_Debug && (p > 0))
                                         fprintf(fout, ";");
                                       p++;
                                       CT_list[ct].already_used = TRUE;
                                       AddFinalTreeCode_CT(ct, finalMHlist[j].mh,
                                                           finalMHlist[j].treecodes[i]);
                                       if(RTM_Debug)
                                          fprintf(fout, "%s (From: %s)",
                                       CT_list[ct].Name,
                                       finalMHlist[j].mh);
                                   } /* fi ct !used */
                               } /* else !vet or !Humans */
                           } /* for */

                           if(RTM_Debug)
                           {
                              fprintf(fout, "\n");
                              fflush(fout);
                           } /* fi */
                        } /* fi ok */
                    } /* else */
                } /* for each CTI treecode k */

                if(!found && RTM_Debug)
                {
                   fprintf(fout, "   -- No Treecode Match Found\n");
                   fflush(fout);
                } /* fi */
             } /* else */
         } /* for each treecode from term */
      } /* fi */
   } /* for each Term in topN */

   /* If we have Pregnancy, we need either Humans or Animals assigned
         If we do find this - look to see if we can just mark oktoprint
         for an existing CT to turn it back on.  If not, add Humans for now.
    */

   foundPregnancy = foundHumans = foundAnimals = FALSE;
   if(CT_list[Human].already_used)
     foundHumans = TRUE;

   if(CT_list[Pregnancy].already_used)
     foundPregnancy = TRUE;

   if(CT_list[Animals].already_used)
     foundAnimals = TRUE;

   if(foundPregnancy && !foundHumans && !foundAnimals)
   {
       if(RTM_Debug)
       {
           fprintf(fout, "Pregnancy Found - no Humans/Animals\n");
           fflush(fout);
       } /* fi */

      found = FALSE;
      for(i = 0; !found && (i < numFinalCTs); i++)
      {
          if(!finalCTlist[i].oktoprint)
          {
              if((strcmp(finalCTlist[i].mh, "Humans") == 0) ||
                 (strcmp(finalCTlist[i].mh, "Animals") == 0))
              {
                  found = TRUE;
                  finalCTlist[i].oktoprint = TRUE;
                  tmp = (char *)malloc(strlen(finalCTlist[i].trigger) + 1000);
                  if(strlen(finalCTlist[i].trigger) > 0)
                    sprintf(tmp, "%s; Forced Pregnancy Turn Back On\0",
                            finalCTlist[i].trigger);
                  free(finalCTlist[i].trigger);
                  finalCTlist[i].trigger = strdup(tmp);
                  free(tmp);

                  if(RTM_Debug)
                  {
                      fprintf(fout, "Adding back in CT for Pregnancy: %s\n",
                              finalCTlist[i].mh);
                      fflush(fout);
                  } /* fi */
              } /* fi */
          } /* fi */
      } /* for */

      if(!found)
        AddFinalTreeCode_CT(Human, "Pregnancy", "G08.686.784.769");
   } /* fi foundPregnancy */
} /* Final_Treecode_CT_Check */


/***************************************************************************
*
*  AddFinalTreeCode_CT --
*
*      This 
*
***************************************************************************/

void AddFinalTreeCode_CT(long ctPos, char *mh, char *treecode)
{
   int found;
   long i, p;
   char *tmp;

   /* If CT is already in the Final CT list, but just turned off then
      turn it back on and add this reason into the trigger information.
   */

   found = FALSE;
   for(i = 0; !found && (i < numFinalCTs); i++)
   {
       if(strcmp(finalCTlist[i].mh, CT_list[ctPos].Name) == 0)
       {
           found = TRUE;
           if(!finalCTlist[i].oktoprint)
           {
               finalCTlist[i].oktoprint = TRUE;
               tmp = (char *)malloc(strlen(finalCTlist[i].trigger) + 1000);
               if(strlen(finalCTlist[i].trigger) > 0)
                 sprintf(tmp, "%s; Forced Treecode Turn Back On: %s (%s)\0",
                         finalCTlist[i].trigger, treecode, mh);
               else
                 sprintf(tmp, "Forced Treecode Turn Back On: %s (%s)\0",
                         finalCTlist[i].trigger, treecode, mh);
               free(finalCTlist[i].trigger);
               finalCTlist[i].trigger = strdup(tmp);
               free(tmp);
           } /* fi */
       } /* fi */
   } /* for */

   /* Else, we need to add it into the list */

   if(!found)
   {
       p = numFinalCTs;
       finalCTlist[p].foundInL2R = FALSE;
       finalCTlist[p].ETflag = FALSE;
       finalCTlist[p].datatype = MH;
       finalCTlist[p].oktoprint = TRUE;
       finalCTlist[p].score = 1000;
       finalCTlist[p].num_RCs = 0;
       finalCTlist[p].VocabDenFactor = 1.9;
       finalCTlist[p].origPos = 10000;

       finalCTlist[p].muid = strdup(muid);
       finalCTlist[p].mh = strdup(CT_list[ctPos].Name);
       finalCTlist[p].mh_orig = strdup("");
       finalCTlist[p].dui = strdup(CT_list[ctPos].DUI);
       finalCTlist[p].entry_term = strdup("");
       finalCTlist[p].RC_PMIDs = strdup("");
       tmp = (char *)malloc(3000);
       sprintf(tmp, "Forced Treecode Turn Back On: %s (%s)\0", treecode, mh);
       finalCTlist[p].trigger = strdup(tmp);
       free(tmp);
       finalCTlist[p].textloc = strdup("");

       /* Positional Information */

       finalCTlist[p].numPIs = 0;

       /* Paths */

       finalCTlist[p].paths[MMI] = TRUE;

       /* Treecodes */

       finalCTlist[p].num_treecodes = 0;

       /* Semantic Types - max 25 */

       finalCTlist[p].num_STs = 0;
               
       numFinalCTs++;
   } /* fi !found */
} /* AddFinalTreeCode_CT */
