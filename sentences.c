#include "M_terms.h"
#include "newMain.h"
#include "ageLookFor.h"
#include "sents.h"
#include "PTs.h"

#define TRUE 1
#define FALSE 0


extern int veterinaryJournal;
extern int RTM_Debug;
extern int haveFullText;
extern long fullTextStartPos;
extern long TIStartPos;
extern long ABStartPos;
extern long cit_len;
extern char *citation;
extern char *origCitation;
extern long gTLen;
extern char *globalTitle;
extern long gALen;
extern char *globalAbstract;
extern FILE *fout;
extern char muid[SMALL_LINE + 1];
extern char nlmID[SMALL_LINE + 1];
extern struct sentStruct sentList[MAX_SENTs];
extern long numSents;

extern void process_mterm(int ambigFlag, char *ambigTrigger, char *mterm, int this_path, 
                   float weight, int type, char *textloc, char *entry_term,
                   char *trigger, char *this_dui, char *origPMID,
                   char *positionalInfo, int forcePrint);

extern void addPT(char *DUI, char *loc, char *lookFor, char *MHname,
                  char *globalET, char *globalPI, int flag, int isSpecial);

extern int foundInText(char *lookFor, int caseSensitive, int TitleOnly);

struct sentStruct sentXList[MAX_SENTs];
long numXSents = 0;

void splitCitation();
void checkSentPTs();
int foundInSent(char *haystack, char *needle);
void createLocalXSents();
void checkPT_SentNegation();
void findXSent(char *haystack, char *needle);
void expandNegPTLookFor(char *lookFor);
void checkSentMHs();
void checkSentCTs();
char *checkSentMaleFemalePct(int foundMale, int foundFemale);
void findXBeginSent(char *haystack, char *needle);

long globalPos = 0;


/***************************************************************************
*
*  splitCitation --
*
*      Split Title & Abstract into separate sentences for Age & PT lookups.
*   Breaks: period, question mark, exclamation mark  (removed colon 2/11/2021)
*         Added Colon back in 3/1/2021.
*
***************************************************************************/

void splitCitation()
{
    int endSent;
    long i, pos, runningPos, endPos;
    char tmp[PMAXLINE + 1];

    if(RTM_Debug)
    {
        fprintf(fout, "Entered splitCitation\n");
        fflush(fout);
        /*
        fprintf(fout, "gTLen: %ld\n", gTLen); fflush(fout);
        fprintf(fout, "Title: #%s#\n", globalTitle); fflush(fout);
        fprintf(fout, "gALen: %ld\n", gALen); fflush(fout);
        fprintf(fout, "Abstract: #%s#\n", globalAbstract); fflush(fout);
        */
    } /* fi */

    /* Handle Title first - should already be in lowercase */

    endSent = FALSE;
    pos = runningPos = 0;
    strcpy(tmp, "");
    endPos = gTLen;

    for(i = 0; i < endPos; i++)
    {
        if((globalTitle[i] == ' ') && (pos == 0))
        {
            tmp[pos] = globalTitle[i];
            runningPos++;
        } /* fi */

        else
          tmp[pos++] = globalTitle[i];

/* COLON is debatable - I've taken out for now March 30, 2021
        if((globalTitle[i] == '.') || (globalTitle[i] == '!') || (globalTitle[i] == '?') ||
           (globalTitle[i] == ':'))
*/

        if((globalTitle[i] == '.') || (globalTitle[i] == '!') || (globalTitle[i] == '?'))
        {
            endSent = TRUE;

            /* Check to see if we have a digit after the period. If so, not end of sentence */

            if((globalTitle[i] == '.') && (i < (endPos - 1)))
            {
                if(isdigit(globalTitle[i + 1]))
                  endSent = FALSE;
            } /* fi */

            if(endSent)
            {
                tmp[pos] = '\0';
                if(numSents < MAX_SENTs)
                {
                    sentList[numSents].locPos = runningPos;

                    /* NOTE: +6 for "TI  - " */

                    sentList[numSents].citPos = TIStartPos + runningPos + 6;
                    sentList[numSents].sentLen = pos;
                    sentList[numSents].sentence = strdup(tmp);
                    strcpy(sentList[numSents].loc, "TI");
                    numSents++;
                    runningPos = (i + 1);
                } /* fi */

                else
                {
                    fprintf(stderr, "Skipping TI Sentence - too many: #%s#\n", tmp);
                    fflush(stderr);
                } /* else */

                strcpy(tmp, "");
                pos = 0;
            } /* fi endSent */
        } /* fi sentence break char. */
    } /* for each TI character */

    /* Check to see if we have anything left for a sentence */

    if(pos > 0)
    {
        tmp[pos] = '\0';
        if(numSents < MAX_SENTs)
        {
            sentList[numSents].locPos = runningPos;

            /* NOTE: +6 for "TI  - " */

            sentList[numSents].citPos = TIStartPos + runningPos + 6;
            sentList[numSents].sentLen = pos;
            sentList[numSents].sentence = strdup(tmp);
            strcpy(sentList[numSents].loc, "TI");
            numSents++;
        } /* fi */

        else
        {
            fprintf(stderr, "Skipping TI Sentence - too many: #%s#\n", tmp);
            fflush(stderr);
        } /* else */
    } /* fi pos > 0 */

    if(RTM_Debug)
    {
        fprintf(fout, "Number of Sentences after TI: %ld\n", numSents);
        fflush(fout);
    } /* fi */

    /* Handle Abstract Next - should already be in lowercase */

    endSent = FALSE;
    pos = runningPos = 0;
    strcpy(tmp, "");
    endPos = gALen;
    if(haveFullText)  /* Want to limit sentences to regular Abstract text & not Full Text */
      endPos = (fullTextStartPos - ABStartPos - 6);

    for(i = 0; i < endPos; i++)
    {
        if((globalAbstract[i] == ' ') && (pos == 0))
        {
            tmp[pos] = globalAbstract[i];
            runningPos++;
        } /* fi */

        else
          tmp[pos++] = globalAbstract[i];

/*  COLON is debatable - I've taken out for now March 30, 2021
        if((globalAbstract[i] == '.') || (globalAbstract[i] == '!') ||
           (globalAbstract[i] == '?') ||(globalAbstract[i] == ':'))
*/
        if((globalAbstract[i] == '.') || (globalAbstract[i] == '!') ||
           (globalAbstract[i] == '?'))
        {
            endSent = TRUE;

            /* Check to see if we have a digit after the period. If so, not end of sentence */

            if((globalAbstract[i] == '.') && (i < (endPos - 1)))
            {
                if(isdigit(globalAbstract[i + 1]))
                  endSent = FALSE;
            } /* fi */

            if(endSent)
            {
                tmp[pos] = '\0';
                if(numSents < MAX_SENTs)
                {
                    sentList[numSents].locPos = runningPos;

                    /* NOTE: +6 for "AB  - " */

                    sentList[numSents].citPos = ABStartPos + runningPos + 6;
                    sentList[numSents].sentLen = pos;
                    sentList[numSents].sentence = strdup(tmp);
                    strcpy(sentList[numSents].loc, "AB");
                    numSents++;
                    runningPos = (i + 1);
                } /* fi */

                else
                {
                    fprintf(stderr, "Skipping AB Sentence - too many: #%s#\n", tmp);
                    fflush(stderr);
                } /* else */

                strcpy(tmp, "");
                pos = 0;
            } /* fi endSent */
        } /* fi sentence break char. */
    } /* for each AB character */

    /* Check to see if we have anything left for a sentence */

    if(pos > 0)
    {
        tmp[pos] = '\0';
        if(numSents < MAX_SENTs)
        {
            sentList[numSents].locPos = runningPos;

            /* NOTE: +6 for "AB  - " */

            sentList[numSents].citPos = ABStartPos + runningPos + 6;
            sentList[numSents].sentLen = pos;
            sentList[numSents].sentence = strdup(tmp);
            strcpy(sentList[numSents].loc, "AB");
            numSents++;
        } /* fi */

        else
        {
            fprintf(stderr, "Skipping AB Sentence - too many: #%s#\n", tmp);
            fflush(stderr);
        } /* else */
    } /* fi pos > 0 */

    if(RTM_Debug)
    {
        fprintf(fout, "Number of Sentences after AB: %ld\n", numSents);
        fflush(fout);
    } /* fi */
} /* splitCitation */


/***************************************************************************
*
*  checkSentPTs --
*
*      
*
***************************************************************************/

void checkSentPTs()
{
    int done, q, x, y, z, foundReqElem, found;
    long begPos, endPos, endCheck, i, j, p, r, sentLen, pos, citPos, lastSentToCheck;
    char tmp[MAXLINE + 1], globalET[MAXLINE + 1], globalPI[MAXLINE + 1],
         loc[5], locLC[5], PTname[250], PTui[25];

    createLocalXSents();
    checkPT_SentNegation();

    /* NOTE: July 29, 2021 - ignoring last sentence in the abstract since it normally talks
           about future actions.
    */

    if(gALen > 0) /* We have an abstract */
      lastSentToCheck = numSents - 1;
    else
      lastSentToCheck = numSents;

    if(RTM_Debug)
    {
        for(i = 0; i < lastSentToCheck; i++)
          fprintf(fout, "Sentence[%ld]: #%s#\n", i, sentXList[i].sentence);
    } /* fi */

    for(j = 0; j < numLookPTs; j++)
    {
        begPos = endPos = -1;
        done = FALSE;
        strcpy(globalET, "[");
        strcpy(globalPI, "");

        if(RTM_Debug)
        {
            fprintf(fout, "\nStarting LookForPTs[%ld]\n", j);
            fflush(fout);
        } /* fi */

        for(i = 0; i < lastSentToCheck; i++)
        {
             if(strstr(lookForPTs[j].loc, sentList[i].loc) != NULL)
             {
                 foundReqElem = 0;
                 for(x = 0; x < lookForPTs[j].numReq; x++)
                 {
                     found = FALSE;
                     for(y = 0; !found && (y < lookForPTs[j].reqElements[x].numPhrases); y++)
                     {
                         if(lookForPTs[j].reqElements[x].doPrefix ||
                            lookForPTs[j].reqElements[x].doPostfix)
                         {
                             for(q = 0; !found && (q < numPP); q++)
                             {
                                 if(lookForPTs[j].reqElements[x].doPrefix)
                                   sprintf(tmp, "%s %s\0", prePost[q],
                                           lookForPTs[j].reqElements[x].phrases[y]);
                                 else
                                   sprintf(tmp, "%s %s\0",
                                           lookForPTs[j].reqElements[x].phrases[y], prePost[q]);

                                 if(lookForPTs[j].negFlag)
                                   found = foundInSent(sentXList[i].sentence, tmp);
                                 else
                                   found = foundInSent(sentList[i].sentence, tmp);

                                 if(RTM_Debug)
                                 {
                                     fprintf(fout, "%d|CheckingI #%s# Sentence: %ld\n",
                                             found, tmp, i);
                                     fflush(fout);
                                 } /* fi */

                                 if(found)
                                 {
                                     foundReqElem++;
                                     lookForPTs[j].reqElements[x].found = y;
                                     lookForPTs[j].reqElements[x].phrasePos = globalPos;
                                     lookForPTs[j].reqElements[x].ppVariant = q;
                                 } /* fi */
                             } /* for q each numPP */
                         } /* fi */

                         else
                         {
                             if(lookForPTs[j].negFlag)
                               found = foundInSent(sentXList[i].sentence,
                                                   lookForPTs[j].reqElements[x].phrases[y]);
                             else
                               found = foundInSent(sentList[i].sentence,
                                                   lookForPTs[j].reqElements[x].phrases[y]);

                             if(RTM_Debug)
                             {
                                 fprintf(fout, "%d|CheckingE #%s# in Sentence: %ld\n", found,
                                        lookForPTs[j].reqElements[x].phrases[y], i);
                                 fflush(fout);
                             } /* fi */

                             if(found)
                             {
                                 foundReqElem++;
                                 lookForPTs[j].reqElements[x].found = y;
                                 lookForPTs[j].reqElements[x].phrasePos = globalPos;
                             } /* fi */
                         } /* else */
                     } /* for each phrase */            
                 } /* for each reqElement */

                 /* See if we found everything required for this item - if we have
                    and the PT is Controlled Clinical Trial, check to see if "a scoping review"
                    is in the title or not and then stop the add here if it is.
                 */

                 if((foundReqElem == lookForPTs[j].numReq) && 
                    (strcmp(lookForPTs[j].ptList[0].UI, "D018848") == 0))
                 {
                     if(foundInText("a scoping review", FALSE, TRUE))
                     {
                         foundReqElem = 0;

                         if(RTM_Debug)
                         {
                             fprintf(fout,
                          "Sentences: Found Controlled Clinical Trial - ignoring scoping review\n");
                             fflush(fout);
                         } /* fi */
                     } /* fi */
                 } /* fi */

                 /* See if we found everything required for this item */

                 if(foundReqElem == lookForPTs[j].numReq)
                 {
                     lookForPTs[j].numFound = foundReqElem;
                     done = TRUE;
                     sentLen = sentList[i].sentLen;
                     begPos = sentLen;
                     endPos = 0;
                     strcat(globalET, "]");
                     if(strlen(globalPI) > 0)
                       strcat(globalPI, ";");

                     if(RTM_Debug)
                     {
                         fprintf(fout, "\nFound All Required Elements: Sentence [%s]: %ld: #%s#\n",
                                sentList[i].loc, i, sentList[i].sentence);
                         fprintf(fout, "     Elements Found: ");
                     } /* fi */

                     for(x = 0; x < lookForPTs[j].numReq; x++)
                     {
                         pos = lookForPTs[j].reqElements[x].found;
                         if(pos > -1)
                         {
                             if((lookForPTs[j].reqElements[x].doPrefix || 
                                 lookForPTs[j].reqElements[x].doPostfix) &&
                                 (lookForPTs[j].reqElements[x].ppVariant > -1))
                             {
                                 endCheck = lookForPTs[j].reqElements[x].phrasePos + 1 +
                                  (long)strlen(lookForPTs[j].reqElements[x].phrases[pos]) +
                                  (long)strlen(prePost[lookForPTs[j].reqElements[x].ppVariant]);
                             } /* fi */

                             else
                             {
                                 endCheck = lookForPTs[j].reqElements[x].phrasePos + 
                                  (long)strlen(lookForPTs[j].reqElements[x].phrases[pos]);
                             } /* else */

                             if(lookForPTs[j].reqElements[x].phrasePos < begPos)
                               begPos = lookForPTs[j].reqElements[x].phrasePos;

                             if(endCheck > endPos)
                               endPos = endCheck;

                             if(RTM_Debug)
                             {
                                 if(x > 0)
                                   fprintf(fout, "|");

                                 if(lookForPTs[j].reqElements[x].doPrefix && 
                                    (lookForPTs[j].reqElements[x].ppVariant > -1))
                                   fprintf(fout, "%s ",
                                           prePost[lookForPTs[j].reqElements[x].ppVariant]);

                                 fprintf(fout, "%s", lookForPTs[j].reqElements[x].phrases[pos]);

                                 if(lookForPTs[j].reqElements[x].doPostfix && 
                                    (lookForPTs[j].reqElements[x].ppVariant > -1))
                                   fprintf(fout, " %s",
                                           prePost[lookForPTs[j].reqElements[x].ppVariant]);

                                 fprintf(fout, " (%ld)", lookForPTs[j].reqElements[x].phrasePos);
                             } /* fi */
                         } /* fi pos > -1 */
                     } /* for */

                     if(RTM_Debug)
                     {
                         fprintf(fout, "\n");
                         fflush(fout);

                         if((begPos > -1) && (endPos < sentLen))
                         {
                             fprintf(fout, "Full Phrase [%ld-%ld]: ", begPos, endPos);
                             fflush(stdout);
                             for(x = begPos; x < endPos; x++)
                               fprintf(fout, "%c", sentList[i].sentence[x]);
                             fprintf(fout, "\n"); fflush(stdout);
                         } /* fi */
                     } /* fi RTM_Debug */

                     citPos = sentList[i].citPos;
                     p = 0;
                     strcpy(tmp, "");
                     for(x = begPos; x < endPos; x++)
                        tmp[p++] = origCitation[x + citPos];
                     tmp[p] = '\0';

                     if(RTM_Debug)
                     {
                         fprintf(fout, "Full Phrase from Citation [%ld|%ld|%ld-%ld]: %s\n", citPos,
                                 sentList[i].locPos, begPos, endPos, tmp);
                         fflush(fout);
                     } /* fi */

                     strcpy(loc, "");
                     strcpy(locLC, "");
                     if(sentList[i].loc[0] == 'T')
                     {
                         strcpy(loc, "TI");
                         strcpy(locLC, "ti");
                     } /* fi */

                     else
                     {
                         strcpy(loc, "AB");
                         strcpy(locLC, "ab");
                     } /* else */


                     sprintf(globalPI, "%ld^%ld^0;\0", citPos + begPos, endPos - begPos);

                     for(z = 0; z < lookForPTs[j].numPTs; z++)
                     {
                         if(veterinaryJournal &&
                             (strcmp(lookForPTs[j].ptList[z].UI, "D016449") == 0))
                         {
                             strcpy(PTname, "Randomized Controlled Trial, Veterinary");
                             strcpy(PTui, "D000081262");
                         } /* fi */

                         else
                         {
                             strcpy(PTname, lookForPTs[j].ptList[z].PT);
                             strcpy(PTui, lookForPTs[j].ptList[z].UI);
                         } /* else */

                         sprintf(globalET, "[\"%s\"-%s-0-\"%s\"]\0", PTname, locLC, tmp);
                         addPT(PTui, loc, tmp, PTname, globalET, globalPI, TRUE, FALSE);

                         if(RTM_Debug)
                         {
                             fprintf(fout, "Adding PT: #%s:%s#\n", PTname, PTui);
                             fprintf(fout, "Sent globalPI: #%s#  globalET: #%s#\n",
                                     globalPI, globalET);
                             fflush(fout);
                         } /* fi */
                     } /* for */
                 } /* fi found everything */

                 /* Reset for next round */

                 lookForPTs[j].reqElements[x].found = -1;
                 lookForPTs[j].reqElements[x].phrasePos = -1;
                 lookForPTs[j].reqElements[x].ppVariant = -1;
             } /* fi right location (TIAB <-> TI) */
        } /* for each sentence */
    } /* for each PT Rule */

    /* Free up local copy of sentences for next round */

    for(r = 0; r < numXSents; r++)
    {
        free(sentXList[r].sentence);
        strcpy(sentXList[r].loc, "");
    } /* for */
    numXSents = 0;
} /* checkSentPTs */


/***************************************************************************
*
*  foundInSent --
*
*      This 
*
***************************************************************************/

int foundInSent(char *haystack, char *needle)
{
   int rtn = FALSE;
   char *base, *foo;
   int okLeft, okRight;
   long i, j, baseLen, lfLen, diff, rPos, pos, x;

   globalPos = -1;
   okLeft = okRight = FALSE;
   base = strdup(haystack);
   baseLen = (long)strlen(haystack);

   lfLen = (long)strlen(needle);
   foo = strstr(base, needle);
   while(!rtn && (foo != NULL))
   {
       okLeft = okRight = FALSE;
       diff = foo - base;
       if(diff == 0)
          okLeft = TRUE;

       else if(diff > 0)
       {
          if(isspace(base[diff - 1]) || ispunct(base[diff - 1]))
            okLeft = TRUE;
       } /* else fi */

       if(okLeft)
       {
          pos = diff + lfLen;

          if((pos >= baseLen) || isspace(base[pos]) || ispunct(base[pos]))
            okRight = TRUE;
       } /* fi okLeft */

       if(okLeft && okRight)
       {
           rtn = TRUE;
           globalPos = diff;
       } /* fi */

       for(j = 0; j < lfLen; j++)
          base[j + diff] = 'X';

       foo = strstr(base, needle);
  } /* while foo */

   free(base);
   return(rtn);
} /* foundInSent */


/***************************************************************************
*
*  createLocalXSents --
*
*      
*
***************************************************************************/

void createLocalXSents()
{
    long i;

    numXSents = 0;
    for(i = 0; i < numSents; i++)
    {
        sentXList[i].locPos = sentList[i].locPos;
        sentXList[i].citPos = sentList[i].citPos;
        sentXList[i].sentLen = sentList[i].sentLen;
        sentXList[i].sentence = strdup(sentList[i].sentence);
        strcpy(sentXList[i].loc, sentList[i].loc);
        numXSents++;
    } /* for */
} /* createLocalXSents */


/***************************************************************************
*
*  checkPT_Negation --
*
*      
*
***************************************************************************/

void checkPT_SentNegation()
{
    int x, y, q;
    long i, j;
    char tmp[MAXLINE + 1];

    /* Go through ignore list first */

    for(i = 0; i < numSents; i++)
    {
        for(j = 0; j < numIgnore; j++)
        {
           findXSent(sentList[i].sentence, IgnoreList[j]);
           findXSent(sentXList[i].sentence, IgnoreList[j]);
        } /* for */
    } /* for each Sentence */

    if(RTM_Debug)
      fprintf(fout, "numLookPTs: %ld\n", numLookPTs);

    for(j = 0; j < numLookPTs; j++)
    {
        if(RTM_Debug)
          fprintf(fout, "\nLook[%ld]\n", j);
        for(x = 0; x < lookForPTs[j].numReq; x++)
        {
             if(RTM_Debug)
               fprintf(fout, "    Reqmnt[%d]  numPhrases: %d\n", x,
                       lookForPTs[j].reqElements[x].numPhrases);
            for(y = 0; y < lookForPTs[j].reqElements[x].numPhrases; y++)
            {
                if(lookForPTs[j].reqElements[x].doPrefix ||
                   lookForPTs[j].reqElements[x].doPostfix)
                {
                    for(q = 0; q < numPP; q++)
                    {
                        if(lookForPTs[j].reqElements[x].doPrefix)
                          sprintf(tmp, "%s %s\0", prePost[q],
                                  lookForPTs[j].reqElements[x].phrases[y]);
                        else
                          sprintf(tmp, "%s %s\0",
                                  lookForPTs[j].reqElements[x].phrases[y], prePost[q]);

                        if(RTM_Debug)
                          fprintf(fout, "        lookFor[y:%d  q:%d]: #%s#\n", y, q, tmp);

                        if(strlen(tmp) > 4)
                          expandNegPTLookFor(tmp);
                    } /* for q each numPP */
                } /* fi */

                else
                {
                    if(RTM_Debug)
                      fprintf(fout, "        lookFor[%d]: #%s#\n", y,
                              lookForPTs[j].reqElements[x].phrases[y]);

                    if(strlen(lookForPTs[j].reqElements[x].phrases[y]) > 4)
                      expandNegPTLookFor(lookForPTs[j].reqElements[x].phrases[y]);
                } /* else */
            } /* for each phrase */
        } /* for each reqElement */
    } /* for each PT Rule */
} /* checkPT_SentNegation */


/***************************************************************************
*
*  expandNegPTLookFor --
*
*      This 
*
***************************************************************************/

void expandNegPTLookFor(char *lookFor)
{
   long i, j;
   char tmp[SMALL_LINE + 1];

    for(j = 0; j < numSents; j++)
    {
        for(i = 0; i < numPrefix_PTNegs; i++)
        {
            sprintf(tmp, "%s%s\0", lookFor, PT_NegPrefix[i]);
            findXSent(sentXList[j].sentence, tmp);
        } /* for Prefix */

        for(i = 0; i < numPostfix_PTNegs; i++)
        {
            sprintf(tmp, "%s%s\0", PT_NegPostfix[i], lookFor);
            findXSent(sentXList[j].sentence, tmp);
        } /* for Postfix */

        /* Misc. */

        sprintf(tmp, "\"%s\"\0", lookFor);
        findXSent(sentXList[j].sentence, tmp);

        sprintf(tmp, "'%s'\0", lookFor);
        findXSent(sentXList[j].sentence, tmp);

        sprintf(tmp, "makes %s inappropriate\0", lookFor);
        findXSent(sentXList[j].sentence, tmp);
    } /* for each sentence */
} /* expandNegPTLookFor */


/***************************************************************************
*
*  findXSent --
*
*      This 
*
***************************************************************************/

void findXSent(char *haystack, char *needle)
{
   int rtn = FALSE;
   char *base, *foo;
   int okLeft, okRight;
   long i, j, baseLen, lfLen, diff, rPos, pos, x;

   globalPos = -1;
   okLeft = okRight = FALSE;
   base = strdup(haystack);
   baseLen = (long)strlen(haystack);

   lfLen = (long)strlen(needle);
   foo = strstr(base, needle);
   while(foo != NULL)
   {
       okLeft = okRight = FALSE;
       diff = foo - base;
       if(diff == 0)
          okLeft = TRUE;

       else if(diff > 0)
       {
          if(isspace(base[diff - 1]) || ispunct(base[diff - 1]))
            okLeft = TRUE;
       } /* else fi */

       if(okLeft)
       {
          pos = diff + lfLen;

          if((pos >= baseLen) || isspace(base[pos]) || ispunct(base[pos]))
            okRight = TRUE;
       } /* fi okLeft */

       if(okLeft && okRight)
       {
           rtn = TRUE;
           globalPos = diff;
       } /* fi */

       for(j = 0; j < lfLen; j++)
       {
          base[j + diff] = 'x';
          haystack[j + diff] = 'x';
       } /* for */

       foo = strstr(base, needle);
  } /* while foo */

   free(base);
} /* findXSent */


/***************************************************************************
*
*  checkSentMHs --
*
*      
*
***************************************************************************/

void checkSentMHs()
{
    int done, q, x, y, z, foundReqElem, found;
    long begPos, endPos, endCheck, i, j, p, r, sentLen, pos, citPos;
    char tmp[MAXLINE + 1], globalET[MAXLINE + 1], globalPI[MAXLINE + 1],
         loc[5], locLC[5], MHname[250], MHui[25], from[MAXLINE + 1];

    createLocalXSents();

    if(RTM_Debug)
    {
        for(i = 0; i < numSents; i++)
          fprintf(fout, "Sentence[%ld]: #%s#\n", i, sentXList[i].sentence);
    } /* fi */

    for(j = 0; j < numLookMHs; j++)
    {
        begPos = endPos = -1;
        done = FALSE;
        strcpy(globalET, "[");
        strcpy(globalPI, "");

        for(i = 0; i < numSents; i++)
        {
             if(strstr(lookForMHs[j].loc, sentList[i].loc) != NULL)
             {
                 foundReqElem = 0;
                 for(x = 0; x < lookForMHs[j].numReq; x++)
                 {
                     found = FALSE;
                     for(y = 0; !found && (y < lookForMHs[j].reqElements[x].numPhrases); y++)
                     {
                         if(lookForMHs[j].negFlag)
                           found = foundInSent(sentXList[i].sentence,
                                               lookForMHs[j].reqElements[x].phrases[y]);
                         else
                           found = foundInSent(sentList[i].sentence,
                                               lookForMHs[j].reqElements[x].phrases[y]);

                         if(RTM_Debug)
                         {
                             fprintf(fout, "MH: %d|Checking #%s# in Sentence: %ld\n", found,
                                    lookForMHs[j].reqElements[x].phrases[y], i);
                             fflush(fout);
                         } /* fi */

                         if(found)
                         {
                             foundReqElem++;
                             lookForMHs[j].reqElements[x].found = y;
                             lookForMHs[j].reqElements[x].phrasePos = globalPos;
                         } /* fi */
                     } /* for each phrase */            
                 } /* for each reqElement */

                 /* See if we found everything required for this item */

                 if(foundReqElem == lookForMHs[j].numReq)
                 {
                     lookForMHs[j].numFound = foundReqElem;
                     done = TRUE;
                     sentLen = sentList[i].sentLen;
                     begPos = sentLen;
                     endPos = 0;
                     strcat(globalET, "]");
                     if(strlen(globalPI) > 0)
                       strcat(globalPI, ";");

                     if(RTM_Debug)
                     {
                         fprintf(fout,
                                "\nFound All Required Elements (MH): Sentence [%s]: %ld: #%s#\n",
                                sentList[i].loc, i, sentList[i].sentence);
                         fprintf(fout, "     Elements Found: ");
                     } /* fi */

                     for(x = 0; x < lookForMHs[j].numReq; x++)
                     {
                         pos = lookForMHs[j].reqElements[x].found;
                         if(pos > -1)
                         {
                             endCheck = lookForMHs[j].reqElements[x].phrasePos + 
                                   (long)strlen(lookForMHs[j].reqElements[x].phrases[pos]);

                             if(lookForMHs[j].reqElements[x].phrasePos < begPos)
                               begPos = lookForMHs[j].reqElements[x].phrasePos;

                             if(endCheck > endPos)
                               endPos = endCheck;

                             if(RTM_Debug)
                             {
                                 if(x > 0)
                                   fprintf(fout, "|");
                                 fprintf(fout, "%s", lookForMHs[j].reqElements[x].phrases[pos]);
                                 fprintf(fout, " (%ld)", lookForMHs[j].reqElements[x].phrasePos);
                             } /* fi */
                         } /* fi pos > -1 */
                     } /* for */

                     if(RTM_Debug)
                     {
                         fprintf(fout, "\n");
                         fflush(fout);

                         if((begPos > -1) && (endPos < sentLen))
                         {
                             fprintf(fout, "MH: Full Phrase [%ld-%ld]: ", begPos, endPos);
                             fflush(stdout);
                             for(x = begPos; x < endPos; x++)
                               fprintf(fout, "%c", sentList[i].sentence[x]);
                             fprintf(fout, "\n"); fflush(stdout);
                         } /* fi */
                     } /* fi RTM_Debug */

                     citPos = sentList[i].citPos;
                     p = 0;
                     strcpy(tmp, "");
                     for(x = begPos; x < endPos; x++)
                        tmp[p++] = origCitation[x + citPos];
                     tmp[p] = '\0';

                     if(RTM_Debug)
                     {
                         fprintf(fout,
                                 "MH: Full Phrase from Citation [%ld|%ld|%ld-%ld]: %s\n", citPos,
                                 sentList[i].locPos, begPos, endPos, tmp);
                         fflush(fout);
                     } /* fi */

                     strcpy(loc, "");
                     strcpy(locLC, "");
                     if(sentList[i].loc[0] == 'T')
                     {
                         strcpy(loc, "TI");
                         strcpy(locLC, "ti");
                     } /* fi */

                     else
                     {
                         strcpy(loc, "AB");
                         strcpy(locLC, "ab");
                     } /* else */


                     sprintf(globalPI, "%ld^%ld^0;\0", citPos + begPos, endPos - begPos);

                     for(z = 0; z < lookForMHs[j].numPTs; z++)
                     {
                         strcpy(MHname, lookForMHs[j].ptList[z].PT);
                         strcpy(MHui, lookForMHs[j].ptList[z].UI);

                         sprintf(globalET, "[\"%s\"-%s-0-\"%s\"]\0", MHname, locLC, tmp);
                         sprintf(from, "Sentence MH Check: %s\0", tmp);
                         process_mterm(FALSE, "", MHname, MMI, 1000.0, MH, loc, globalET, from, MHui, "",
                                  globalPI, TRUE);

                         if(RTM_Debug)
                         {
                             fprintf(fout, "Adding Sentence MH: #%s:%s#\n", MHname, MHui);
                             fprintf(fout, "Sent globalPI: #%s#  globalET: #%s#\n",
                                     globalPI, globalET);
                             fflush(fout);
                         } /* fi */
                     } /* for */
                 } /* fi found everything */

                 /* Reset for next round */

                 lookForMHs[j].reqElements[x].found = -1;
                 lookForMHs[j].reqElements[x].phrasePos = -1;
                 lookForMHs[j].reqElements[x].ppVariant = -1;
             } /* fi right location (TIAB <-> TI) */
        } /* for each sentence */
    } /* for each MH Rule */

    /* Free up local copy of sentences for next round */

    for(r = 0; r < numXSents; r++)
    {
        free(sentXList[r].sentence);
        strcpy(sentXList[r].loc, "");
    } /* for */
    numXSents = 0;
} /* checkSentMHs */


/***************************************************************************
*
*  checkSentCTs --
*
*      
*
***************************************************************************/

void checkSentCTs()
{
    int done, q, x, y, z, foundReqElem, found;
    long begPos, endPos, endCheck, i, j, p, r, sentLen, pos, citPos;
    char tmp[MAXLINE + 1], globalET[MAXLINE + 1], globalPI[MAXLINE + 1],
         loc[5], locLC[5], MHname[250], MHui[25], from[MAXLINE + 1];

    createLocalXSents();

    if(RTM_Debug)
    {
        for(i = 0; i < numSents; i++)
          fprintf(fout, "Sentence[%ld]: #%s#\n", i, sentXList[i].sentence);
    } /* fi */

    for(j = 0; j < numLookCTs; j++)
    {
        begPos = endPos = -1;
        done = FALSE;
        strcpy(globalET, "[");
        strcpy(globalPI, "");

        for(i = 0; i < numSents; i++)
        {
             if(strstr(lookForCTs[j].loc, sentList[i].loc) != NULL)
             {
                 foundReqElem = 0;
                 for(x = 0; x < lookForCTs[j].numReq; x++)
                 {
                     found = FALSE;
                     for(y = 0; !found && (y < lookForCTs[j].reqElements[x].numPhrases); y++)
                     {
                         if(lookForCTs[j].negFlag)
                           found = foundInSent(sentXList[i].sentence,
                                               lookForCTs[j].reqElements[x].phrases[y]);
                         else
                           found = foundInSent(sentList[i].sentence,
                                               lookForCTs[j].reqElements[x].phrases[y]);

                         if(RTM_Debug)
                         {
                             fprintf(fout, "CT: %d|Checking #%s# in Sentence: %ld\n", found,
                                    lookForCTs[j].reqElements[x].phrases[y], i);
                             fflush(fout);
                         } /* fi */

                         if(found)
                         {
                             foundReqElem++;
                             lookForCTs[j].reqElements[x].found = y;
                             lookForCTs[j].reqElements[x].phrasePos = globalPos;
                         } /* fi */
                     } /* for each phrase */            
                 } /* for each reqElement */

                 /* See if we found everything required for this item */

                 if(foundReqElem == lookForCTs[j].numReq)
                 {
                     lookForCTs[j].numFound = foundReqElem;
                     done = TRUE;
                     sentLen = sentList[i].sentLen;
                     begPos = sentLen;
                     endPos = 0;
                     strcat(globalET, "]");
                     if(strlen(globalPI) > 0)
                       strcat(globalPI, ";");

                     if(RTM_Debug)
                     {
                         fprintf(fout,
                                "\nFound All Required Elements (CT): Sentence [%s]: %ld: #%s#\n",
                                sentList[i].loc, i, sentList[i].sentence);
                         fprintf(fout, "     Elements Found: ");
                     } /* fi */

                     for(x = 0; x < lookForCTs[j].numReq; x++)
                     {
                         pos = lookForCTs[j].reqElements[x].found;
                         if(pos > -1)
                         {
                             endCheck = lookForCTs[j].reqElements[x].phrasePos + 
                                   (long)strlen(lookForCTs[j].reqElements[x].phrases[pos]);

                             if(lookForCTs[j].reqElements[x].phrasePos < begPos)
                               begPos = lookForCTs[j].reqElements[x].phrasePos;

                             if(endCheck > endPos)
                               endPos = endCheck;

                             if(RTM_Debug)
                             {
                                 if(x > 0)
                                   fprintf(fout, "|");
                                 fprintf(fout, "%s", lookForCTs[j].reqElements[x].phrases[pos]);
                                 fprintf(fout, " (%ld)", lookForCTs[j].reqElements[x].phrasePos);
                             } /* fi */
                         } /* fi pos > -1 */
                     } /* for */

                     if(RTM_Debug)
                     {
                         fprintf(fout, "\n");
                         fflush(fout);

                         if((begPos > -1) && (endPos < sentLen))
                         {
                             fprintf(fout, "CT: Full Phrase [%ld-%ld]: ", begPos, endPos);
                             fflush(stdout);
                             for(x = begPos; x < endPos; x++)
                               fprintf(fout, "%c", sentList[i].sentence[x]);
                             fprintf(fout, "\n"); fflush(stdout);
                         } /* fi */
                     } /* fi RTM_Debug */

                     citPos = sentList[i].citPos;
                     p = 0;
                     strcpy(tmp, "");
                     for(x = begPos; x < endPos; x++)
                        tmp[p++] = origCitation[x + citPos];
                     tmp[p] = '\0';

                     if(RTM_Debug)
                     {
                         fprintf(fout,
                                 "CT: Full Phrase from Citation [%ld|%ld|%ld-%ld]: %s\n", citPos,
                                 sentList[i].locPos, begPos, endPos, tmp);
                         fflush(fout);
                     } /* fi */

                     strcpy(loc, "");
                     strcpy(locLC, "");
                     if(sentList[i].loc[0] == 'T')
                     {
                         strcpy(loc, "TI");
                         strcpy(locLC, "ti");
                     } /* fi */

                     else
                     {
                         strcpy(loc, "AB");
                         strcpy(locLC, "ab");
                     } /* else */


                     sprintf(globalPI, "%ld^%ld^0;\0", citPos + begPos, endPos - begPos);

                     for(z = 0; z < lookForCTs[j].numPTs; z++)
                     {
                         strcpy(MHname, lookForCTs[j].ptList[z].PT);
                         strcpy(MHui, lookForCTs[j].ptList[z].UI);

                         sprintf(globalET, "[\"%s\"-%s-0-\"%s\"]\0", MHname, locLC, tmp);
                         sprintf(from, "Sentence CT Check: %s\0", tmp);
                         process_mterm(FALSE, "", MHname, MMI, 1000.0, CT, loc, globalET, from, MHui, "",
                                  globalPI, TRUE);

                         if(RTM_Debug)
                         {
                             fprintf(fout, "Adding Sentence CT: #%s:%s#\n", MHname, MHui);
                             fprintf(fout, "Sent globalPI: #%s#  globalET: #%s#\n",
                                     globalPI, globalET);
                             fflush(fout);
                         } /* fi */
                     } /* for */
                 } /* fi found everything */

                 /* Reset for next round */

                 lookForCTs[j].reqElements[x].found = -1;
                 lookForCTs[j].reqElements[x].phrasePos = -1;
                 lookForCTs[j].reqElements[x].ppVariant = -1;
             } /* fi right location (TIAB <-> TI) */
        } /* for each sentence */
    } /* for each CT Rule */

    /* Free up local copy of sentences for next round */

    for(r = 0; r < numXSents; r++)
    {
        free(sentXList[r].sentence);
        strcpy(sentXList[r].loc, "");
    } /* for */
    numXSents = 0;
} /* checkSentCTs */


/***************************************************************************
*
*  checkSentMaleFemalePct --
*
*      
*
***************************************************************************/

char *checkSentMaleFemalePct(int foundMale, int foundFemale)
{
    int found;
    long i, j, diff;
    char tmp[MAXLINE + 1], *rtn, *foo;

    rtn = (char *)malloc(25);
    strcpy(rtn, "");

    createLocalXSents();

    if(RTM_Debug)
    {
        for(i = 0; i < numSents; i++)
          fprintf(fout, "Male/Female Sentence[%ld]: #%s#\n", i, sentXList[i].sentence);
    } /* fi */

    /* Setup the ignores - want to ignore all of these at the beginning of a sentence since
       that would not indicate a percentage of Male or Female e.g., "are women attending"
    */

    for(i = 0; i < numSents; i++)
    {
        for(j = 0; j < numMFIgnore; j++)
          findXBeginSent(sentXList[i].sentence, mfIgnoreList[j]);
    } /* for each Sentence */

    if(RTM_Debug)
    {
        for(i = 0; i < numSents; i++)
          fprintf(fout, "After Ignore Male/Female Sentence[%ld]: #%s#\n", i, sentXList[i].sentence);
    } /* fi */

    /* Now look for the appropriate triggers */

    if(foundMale) /* Look only for Male triggers */
    {
        found = FALSE;
        for(i = 0; !found && (i < numSents); i++)
        {
            for(j = 0; !found && (j < numMFLookFor); j++)
            {
                if(!mfLookForList[j].isFemale)
                {
                    foo = strstr(sentXList[i].sentence, mfLookForList[j].lookFor);
                    if(foo != NULL)
                    {
                        diff = foo - sentXList[i].sentence;
                        found = TRUE;
                        sprintf(tmp, "%ld|%ld|%s|%s\0", i, sentXList[i].citPos + diff,
                                sentXList[i].loc, mfLookForList[j].lookFor);
                        free(rtn);
                        rtn = strdup(tmp);
                    } /* fi */
                } /* fi */
            } /* for */
        } /* for each Sentence */
    } /* fi */

    else /* Look only for Female triggers */
    {
        found = FALSE;
        for(i = 0; !found && (i < numSents); i++)
        {
            for(j = 0; !found && (j < numMFLookFor); j++)
            {
                if(mfLookForList[j].isFemale)
                {
                    foo = strstr(sentXList[i].sentence, mfLookForList[j].lookFor);
                    if(foo != NULL)
                    {
                        diff = foo - sentXList[i].sentence;
                        found = TRUE;
                        sprintf(tmp, "%ld|%ld|%s|%s\0", i, sentXList[i].citPos + diff,
                                sentXList[i].loc, mfLookForList[j].lookFor);
                        free(rtn);
                        rtn = strdup(tmp);
                    } /* fi */
                } /* fi */
            } /* for */
        } /* for each Sentence */
    } /* else */

    /* Free up local copy of sentences for next round */

    for(i = 0; i < numXSents; i++)
    {
        free(sentXList[i].sentence);
        strcpy(sentXList[i].loc, "");
    } /* for */
    numXSents = 0;

    return(rtn);
} /* checkSentMaleFemalePct */


/***************************************************************************
*
*  findXBeginSent --
*
*      This 
*
***************************************************************************/

void findXBeginSent(char *haystack, char *needle)
{
   int rtn = FALSE;
   char *base, *foo;
   int okLeft, okRight;
   long i, j, baseLen, lfLen, diff, rPos, pos, x;

   globalPos = -1;
   okLeft = okRight = FALSE;
   base = strdup(haystack);
   baseLen = (long)strlen(haystack);

   lfLen = (long)strlen(needle);
   foo = strstr(base, needle);
   while(foo != NULL)
   {
       okLeft = okRight = FALSE;
       diff = foo - base;
       if(diff == 0)  /* Only want from beginning of sentence for this */
          okLeft = TRUE;

       if(okLeft)
       {
          pos = diff + lfLen;

          if((pos >= baseLen) || isspace(base[pos]) || ispunct(base[pos]))
            okRight = TRUE;
       } /* fi okLeft */

       if(okLeft && okRight)
          rtn = TRUE;

       for(j = 0; j < lfLen; j++)
       {
          base[j + diff] = 'x';
          haystack[j + diff] = 'x';
       } /* for */

       foo = strstr(base, needle);
  } /* while foo */

   free(base);
} /* findXBeginSent */
