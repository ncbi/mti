#include <ctype.h>
#include <time.h>
#include "M_terms.h"
#include "newMain.h"
#include "ageLookFor.h"

#define TRUE 1
#define FALSE 0
#define RSfilterTO_limit 190
#define RSfilterTO_rank 9

#define RSfilterALL_limit 203
#define RSfilterALL_rank 13

#define HUMAN_TOP_SCR_FILE "/nfsvol/nls/MEDLINE_Baseline_Repository/MeSH/2021/SCR_Triggers/Human_Proteins/topCombined"
#define HUMAN_TOP_MICRORNA_FILE "/nfsvol/nls/MEDLINE_Baseline_Repository/MeSH/2021/SCR_Triggers/Human_Proteins/microRNA/topCombined"

#define MOUSE_TOP_SCR_FILE "/nfsvol/nls/MEDLINE_Baseline_Repository/MeSH/2021/SCR_Triggers/Mouse_Proteins/topCombined"
#define MOUSE_TOP_MICRORNA_FILE "/nfsvol/nls/MEDLINE_Baseline_Repository/MeSH/2021/SCR_Triggers/Mouse_Proteins/microRNA/topCombined"

#define RAT_TOP_SCR_FILE "/nfsvol/nls/MEDLINE_Baseline_Repository/MeSH/2021/SCR_Triggers/Rat_Proteins/topCombined"
#define RAT_TOP_MICRORNA_FILE "/nfsvol/nls/MEDLINE_Baseline_Repository/MeSH/2021/SCR_Triggers/Rat_Proteins/microRNA/topCombined"

#define ARABIDOPSIS_TOP_SCR_FILE "/nfsvol/nls/MEDLINE_Baseline_Repository/MeSH/2021/SCR_Triggers/Arabidopsis_Proteins/topCombined"
#define ARABIDOPSIS_TOP_MICRORNA_FILE "/nfsvol/nls/MEDLINE_Baseline_Repository/MeSH/2021/SCR_Triggers/Arabidopsis_Proteins/microRNA/topCombined"

#define DROSOPHILA_TOP_SCR_FILE "/nfsvol/nls/MEDLINE_Baseline_Repository/MeSH/2021/SCR_Triggers/Drosophila_Proteins/topCombined"
#define DROSOPHILA_TOP_MICRORNA_FILE "/nfsvol/nls/MEDLINE_Baseline_Repository/MeSH/2021/SCR_Triggers/Drosophila_Proteins/microRNA/topCombined"

#define SCEREVISIAE_TOP_SCR_FILE "/nfsvol/nls/MEDLINE_Baseline_Repository/MeSH/2021/SCR_Triggers/Scerevisiae_Proteins/topCombined"

#define CELEGANS_TOP_SCR_FILE "/nfsvol/nls/MEDLINE_Baseline_Repository/MeSH/2021/SCR_Triggers/Celegans_Proteins/topCombined"
#define CELEGANS_TOP_MICRORNA_FILE "/nfsvol/nls/MEDLINE_Baseline_Repository/MeSH/2021/SCR_Triggers/Celegans_Proteins/microRNA/topCombined"

#define ECOLI_TOP_SCR_FILE "/nfsvol/nls/MEDLINE_Baseline_Repository/MeSH/2021/SCR_Triggers/Ecoli_Proteins/topCombined"

#define ZEBRAFISH_TOP_SCR_FILE "/nfsvol/nls/MEDLINE_Baseline_Repository/MeSH/2021/SCR_Triggers/Zebrafish_Proteins/topCombined"
#define ZEBRAFISH_TOP_MICRORNA_FILE "/nfsvol/nls/MEDLINE_Baseline_Repository/MeSH/2021/SCR_Triggers/Zebrafish_Proteins/microRNA/topCombined"

#define XENOPUS_TOP_SCR_FILE "/nfsvol/nls/MEDLINE_Baseline_Repository/MeSH/2021/SCR_Triggers/Xenopus_Proteins/topCombined"
#define XENOPUS_TOP_MICRORNA_FILE "/nfsvol/nls/MEDLINE_Baseline_Repository/MeSH/2021/SCR_Triggers/Xenopus_Proteins/microRNA/topCombined"

#define SPOMBE_TOP_SCR_FILE "/nfsvol/nls/MEDLINE_Baseline_Repository/MeSH/2021/SCR_Triggers/Spombe_Proteins/topCombined"

#define MAX_BELOW 250
#define IntJEatDisord "8111226"
#define MAX_L2R_RESULTS 1000

#define SHOW_L2R 0
#define L2R_PATH "/nfsvol/nls/II_Group_WorkArea/MTI/Resources/Ilya"
#define RNDL2R_FILE "/nfsvol/nls/II_Group_WorkArea/MTI/Resources/L2R.txt"

#define REPL_FILE_4_AGES "/nfsvol/nls/II_Group_WorkArea/MTI/Resources/convertNumbers.txt"

#define DEFAULT_L2R_HOST "Server9"
/*
int numIMLhosts = 4;
char *IMLHosts[] = { "Server9", "Server10", "Server12", "Server13" };
int numIMLhosts = 3;
char *IMLHosts[] = { "Server9", "Server10", "Server13" };
*/
int numIMLhosts = 4;
char *IMLHosts[] = { "Server9", "Server10", "Server12", "Server13" };

struct ctDlist {
   char name[35];
   char dui[25];
};
struct ctDlist CT_DUI_list[] = {
     {"Adolescent", "D000293"}, {"Adult", "D000328"}, {"Aged", "D000368"},
     {"Aged, 80 and over", "D000369"}, {"Animals", "D000818"},
     {"Bees", "D001516"}, {"Cats", "D002415"}, {"Cattle", "D002417"},
     {"Chlorocebus aethiops", "D002522"}, {"Chick Embryo", "D002642"},
     {"Child", "D002648"}, {"Child, Preschool", "D002675"},
     {"Cricetinae", "D006224"}, {"Dogs", "D004285"}, {"Female", "D005260"},
     {"Guinea Pigs", "D006168"}, {"History of Medicine", "D006666"},
     {"History, 15th Century", "D049668"}, {"History, 16th Century", "D049669"},
     {"History, 17th Century", "D049670"}, {"History, 18th Century", "D049671"},
     {"History, 19th Century", "D049672"}, {"History, 20th Century", "D049673"},
     {"History, 21st Century", "D049674"}, {"History, Ancient", "D049690"},
     {"History, Medieval", "D049691"}, {"Horses", "D006736"},
     {"Humans", "D006801"}, {"Infant", "D007223"},
     {"Infant, Newborn", "D007231"}, {"Male", "D008297"}, {"Mice", "D051379"},
     {"Middle Aged", "D008875"}, {"Pregnancy", "D011247"},
     {"Rabbits", "D011817"}, {"Rats", "D051381"}, {"Sheep", "D012756"},
     {"Swine", "D013552"}, {"United States", "D014481"},
     {"Young Adult", "D055815"}
};
int num_ctdui = 40;

int numForceChecks = 11;
char *ForceChecks[] = { 
   "Forced Dashed Lookup:",         /*  2 */
   "Forced Leaf Node Lookup:",      /*  3 */
   "Forced Lookup:",                /*  4 */
   "Forced New Lookup:",            /*  5 */
   "CT Text Lookup:",               /*  9 */
   "Forced Mice/Rats:",             /* 12 */
   "Medical Tourism Lookup:",       /* 17 */
   "Forced Non-Leaf Node Lookup:",  /* 19 */
   "Forced SCR Leaf Node Lookup:",  /* 21 */
   "Forced Isotope Lookup:",        /* 31 */
   "AMBIG/METAPHOR"
};

extern float PW1;
extern float PW4;
extern int noAnswer;
extern long numMMILines;
extern pid_t mypid;

extern TMT *mt_table;
extern long mt_count;
extern long fITpos, fITlen;

extern TCHECKTAG *ct_table;
extern int ct_count; 

extern TCHECKTAG *sh_table;
extern int sh_count;

extern int max;
extern long cit_len;
extern char *citation;
extern char *L2R_Citation;
extern long gTLen;
extern char *globalTitle;
extern long gALen;
extern char *globalAbstract;
extern FILE *fout;
extern char muid[SMALL_LINE + 1];
extern char nlmID[SMALL_LINE + 1];

extern char *PRC_PMIDs[MAX_PRC];
extern float PRC_Scores[MAX_PRC];
extern long numPRC;

extern int haveFullText;
extern long fullTextStartPos;
extern long fullTextTableStartPos;

extern struct nlStruct nonLeafs[1000];
extern long numNL;

extern struct ftsStruct FT_Snippets[MAX_SNIPPETS];
extern long numFTS;

extern int showHMs, showETs, showETsD, limitTitleOnly, Title_Only, showDUIs,
           Special_PT, showTUIs, RSfilterTO, TitleNumWords, showTreecodes,
           RSfilterALL, showRSfilter, addAddls, cutOffScore, showRemovals,
           RTM_Debug, addCTs, lTO_II, doLogging, CATALOGING, exitNoResult,
           nursingJournal, veterinaryJournal, dentistryJournal, hasCON, hasEFR,
           hasPROF, hasRPF, hasROF, hasUOF, doNoAddForced, showType,
           firstLineJournal, preIndex, MeSHonDemand, MeSHonDemand2, MoD_Raw,
           MoD_PP, OLDMEDLINE, level1Filter, JSON, showScores,
           showScoresFull, trackPositional, showIlyaML, MTI_AUTO, doIL2R,
           goHigh, doIL2RHybrid, ACCME, MTI_REVIEW, foundNonHumanTrigger,
           haveFreeText, insectsJournal, doLookAtConf, geriatricJournal,
           botanyJournal, zootaxaJournal;

extern char *fITXALLbase;
extern long fITXALLlen;

/* Publication Type extern definitions */

extern char PTs[SMALL_LINE + 1];
extern long numPTsFound;
extern struct finalStruct foundPTsList[MAX_FINAL_MHs];
extern char *ExistPTs[MAX_PTs];
extern int numExistingPTs;

extern void printAddls(int which, int show_id, int out_ctr);
extern int find_uniq(int unipos);
extern int search_index(char *this_dui);
extern void filterPreIndex(int isLevelOne);
extern void filterIJED();
extern void filterBodyPartDisease();
extern void filterMeSHonDemand();
extern void filterNeoplasms();
extern void filterCancer();
extern void filterOLDMEDLINE();
extern void filterGenMiceRatHamster();
extern void filterHeuristicN();
extern void filterMentalDisorders();
extern void filterGenSpec();
extern void process_mterm(int ambigFlag, char *ambigTrigger, char *mterm, int this_path, 
                   float weight, int type, char *textloc, char *entry_term,
                   char *trigger, char *this_dui, char *origPMID,
                   char *positionalInfo, int forcePrint);
extern int foundInText(char *lookFor, int caseSensitive, int TitleOnly);
extern int foundInText_Star(char *lookFor, int caseSensitive, int TitleOnly);
extern char *findMHDUI(char *mh);
extern int query_btree();
extern void validate_topn(long numPrinted);
extern void check_CTs(int flag);
extern int foundInALLTextXdOut(char *lookFor);
extern long isMHExcludeLC(char *term, int flag);
extern long isLookForOK(char *lookFor);
extern char *getVocabDen(char *nlmID, char *DUI);
extern int binsearchNewCheck(char *word, char *dui);
extern void Final_Treecode_CT_Check();
extern char *Strip_Str(char *insegment);
extern void Load_Repls();
extern void cleanReplList();
extern int isAnimalContext(char *lookFor);
extern void checkNewOtherCoordRules();
extern void process_PTterm(char *mterm, char *textloc,
                           char *entry_term, char *trigger, char *this_dui,
                           char *positionalInfo);
extern char *checkSentMaleFemalePct(int foundMale, int foundFemale);

long highScore = 1000;

struct XML_sub_struct
{
   char Orig;
   char Sub[6];
};

struct XML_sub_struct XML_sub[] = {
    { '"', "&#34;" }, { '&', "&#38;" }, { '<', "&#60;" }, { '>', "&#62;" }
};


struct repl_age_struct {
  char *search_for;
  char *replace_by;
};

struct rAStruct {
   char *checkFirst;
   int numSearch;
   struct repl_age_struct repls[25];
};

struct rAStruct repl_age_list[20];
int num_age_repls = 0;
long ageCitLen = 0;
long begSpan, endSpan, usedEnd;
float begYear, endYear;

struct ctAgelist {
   char Name[35];
   char DUI[25];
   int found;
   int inListAlready;
   long listPos;
};

/* NOTE: List corresponds to the Special_Aged_Lookups list in ageLookFor.h
         so make sure to keep both up-to-date!!
*/

int numAgeCTs = 10;
struct ctAgelist Age_CT_list[] = {
   /* 0 */ {"Infant, Newborn", "D007231", 0, 0, -1},
   /* 1 */ {"Infant", "D007223", 0, 0, -1},
   /* 2 */ {"Child, Preschool", "D002675", 0, 0, -1},
   /* 3 */ {"Child", "D002648", 0, 0, -1},
   /* 4 */ {"Adolescent", "D000293", 0, 0, -1},
   /* 5 */ {"Young Adult", "D055815", 0, 0, -1},
   /* 6 */ {"Adult", "D000328", 0, 0, -1},
   /* 7 */ {"Middle Aged", "D008875", 0, 0, -1},
   /* 8 */ {"Aged", "D000368", 0, 0, -1},
   /* 9 */ {"Aged, 80 and over", "D000369", 0, 0, -1}
};

long triggerLen = 0;
long pSL_start = 0;
char *foundAgeTrigger;
int entered_checkForAgeYears = FALSE;

int num_XML_sub = 4;
long out_ctr = 0;
int printPMID = TRUE;
char *belowLineTmp[MAX_BELOW];
long numBelow = 0;
int haveHumansBelowLine = FALSE;

long numBioASQPrinted = 0;
long numOverallPrinted = 0;

long numFinalCTs = 0;
struct finalStruct finalCTlist[MAX_FINAL_CTs];

long numFinalSHs = 0;
struct finalStruct finalSHlist[MAX_FINAL_SHs];

long numFinalMHs = 0;
struct finalStruct finalMHlist[MAX_FINAL_MHs];

long numFinalOVFs = 0;
struct finalStruct finalOVFlist[MAX_FINAL_MHs];

void display_output(int strictFilter_set, int medFilter_set, 
                    int medFilterR_set, int starMHTI_set, int expanded_flag,
                    int justFacts, int BioASQ);
void display_output_list(int strictFilter_set, int medFilter_set,
                         int starMHTI_set, int showNoTerms);
void display_simple(int strictFilter_set, int show_labels, int show_id);
void display_full_listing(int strictFilter_set, int medFilter_set, int OKonly);
int validDisplay(int pos);
int amIGeographical(int pos);
int amIGeo_Final(int pos);
char *whatLocs(int mt_pos);
int getTitleOnlyLimit();
int MMRCset(int mt_pos);
int lookFor(char *DUI, long endPos);
void combined_display(int strictFilter_set, int medFilter_set, 
     int medFilterR_set, int starMHTI_set, int expanded_flag, int which,
     int showNoTerms, int show_labels, int show_id, int justFacts,
     int BioASQ, int OKonly);
void printCTs(int strictFilter_set, int which);
void printSHs(int strictFilter_set, int which);
int check_special_term();
long lookForCT(char *DUI);
void modifyBelowLine(char *MHname, char *trig, char *poInfo);
char *show_MH_HM_ET(int datatype, int isET);
void savePrinted(long pos, int whichArray, int pPos);
void printFinal(int which, int starMHTI_set, int justFacts, int show_id,
                int show_labels, int OKonly);
void printFinalJustInfo(char *PRC_from, int useWSD, int useWSDTIOnly);
char *pullTUI(char *inMH);
void pullTREEs(long pos);
void pullTREEs_CT(long pos);
void pullTREEs_OVF(long pos);
void addSCR(long uniq_ID);
int findSAPMH(char *mh);
void cleanFinalList();
void saveOverFlow(long pos, int whichArray);
void printNonLeaf();
void printRemovals();
char *cleanET(char *inET);
void print_MoD_Raw();
void print_JSON();
char *findTI(char *PMID);
void updateFinalList(char *mh, char *dui, char *from, int which);
void print_IlyaML();
void doLearning2Rank(int BioASQ);
int GetNextIMLRand();
void checkZika_2016();
void forcedKeepAfterL2R();
void forcedKeepAfterL2R_MTI_AUTO();
int shouldKeepL2R(char *inL2R, int haveCOVID19);
int shouldKeepL2R_MTI_AUTO(char *inL2R, int haveCOVID19);
int find_SCR_HM(char *mh);
void save_SCR_HM(char *mh);
int isCTDUI(char *dui);
void checkForAgeYears();
char *hideStdDev(char *snippet);
long isTriggerInText(char *lookFor, char *text);
char *pullSnippet(long startPos, char *text);
char *pullSnippetLeft(long startPos, char *text, long strLen);
void Load_Age_Repls();
char *stringEditByWord(char *insegment);
char *pullYears(char *snippet, long len, int hasRange, long snippetOffset);
void parseYears(char *years, char *snippet);
float determineYear(char *str);
void checkBeforeFinalPrint();
int checkFinalMH(char *mh);
long checkFinalMHPos(char *mh);
long checkFinalCT(char *mh);
int isBadL2R(char *lineL2R, char *pmidL2R);
char *updateL2R(char *lineL2R);
int checkAddCT(char *citSpan, int flag);
void check_Species_Protein_SCRs(char *filename, char *targetSpecies);
void check_HeadingMappedTo(char *inSCR, char *triggerMsg);
void check_Species_microRNA_SCRs(char *filename, char *targetSpecies);
char *SwapAnd(char *insegment);
void checkMaleFemalePct();
void AddSpecialAgedTerms(int agePos, char *snippet, long begSpan);
long lookForAgedTermsRight(char *haystack);
long lookForAgedTermsLeft(char *haystack);
char *XoutTriggerInText(char *lookFor, char *text);
void lookAtConfidence();
void checkAddHumans(int foundHumanNoPrint, long humanPos, char *trigger);
long findSnippet(long startPos);
void lookForAnimalDiseaseAdds();
void addAnimalDiseaseTerm(char *mh, char *dui);
void check_PTs_Protocol();
void check_PTs_Reviews();
void makePTasTopic(long ptPos, char *reason);
void addFemalePregnancy(long pregnancyPos);
char *verifyAmbig_Metaphor(char *triggerList);
int countForced(char *trigger);
char *XoutPercentages(char *text);

/***************************************************************************
*                                                                          *
***************************************************************************/

void combined_display(int strictFilter_set, int medFilter_set, 
     int medFilterR_set, int starMHTI_set, int expanded_flag, int which,
     int showNoTerms, int show_labels, int show_id, int justFacts,
     int BioASQ, int OKonly)
{
   int j, k, okCT, t, t2, max_title, done, ok, added, tctr, found, foundT,
       foundOther, foundFullText, foundFullText_Table, ok3, foundHuman,
       foundHumanNoPrint, foundHumanTrigger, foundReviewPT, foundSysReviewPT,
       foundPT;
   long i, l, m, x, start, index, ctPos, numUsed, numValidCT, numValidMH, jgm,
        pos, humanPos, FTSpos, reviewPos;
   char tmp[MED_LINE + 1], trigger[MAXLINE + 1], extras[MAXLINE + 1], *foo,
        tmp2[SMALL_LINE + 1], mh_orig[251], *entryTerm,
        posInfo[SMALL_LINE + 1], humanTrigger[SMALL_LINE + 1];

   /* Calculate whatever the highest score is right now */

   highScore = 0;
   for(i = 0; i < mt_count; i++)
   {
       if(mt_table[i].score > highScore)
         highScore = mt_table[i].score;
   } /* for */

   if(highScore == 0)
     highScore = 1000;

   /* Verify that we have everything that we need */

   strcpy(tmp, "");
   strcpy(trigger, "");
   strcpy(extras, "");
   strcpy(tmp2, "");
   numBelow = 0;
   printPMID = TRUE;

   out_ctr = 0;
   max_title = max + 100;
   if(limitTitleOnly && Title_Only)
     max_title = getTitleOnlyLimit();

   /* For now, if we have a special Publication Type, it supercedes any
      of the other limits for recommendations.
   */

   if(Special_PT > -1)
     max_title = special_PTs[Special_PT].rec_limit;

   /* Add all items that we are "forcing" to display first */

   for(i = 0; i < mt_count; i++)
   {
      if(mt_table[i].oktoprint && (mt_table[i].datatype != CT) &&
         mt_table[i].forcePrint)
      {
          /* If we are "forcing" an HM term, add the SCR above it */

          if(mt_table[i].datatype == HM)
            addSCR(mt_table[i].uniq_ID);

          mt_table[i].hasPrinted = TRUE;
          savePrinted(i, 0, 0);
      } /* fi */
   } /* for */

   i=0;
   done = FALSE;
   strcpy(extras, "");
   tctr = 0;

   /* Add main recommendations here with cut-off for Title Only */

   while(!done && (i < max) && (mt_count > 0) && (tctr < max_title))
   {
      if(!strictFilter_set || validDisplay(i))
      {
          if(mt_table[i].oktoprint && !mt_table[i].hasPrinted &&
             ((mt_table[i].datatype == MH) || (mt_table[i].datatype == NM) ||
              (mt_table[i].datatype == HM)))
          {
             ok = TRUE;

             if(!doNoAddForced && (mt_table[i].score < cutOffScore))
             {
                done = TRUE;
                ok = FALSE;
             } /* fi */

             /* 081307 - lTO_II
                Positions 1 & 2 ok - as before
                Positions 3 & 4 - only if also recommended by MMI
                Ignore everything below this level and if the score dips
                below 1 - even if before this position.
             */

             if(lTO_II && Title_Only && !doNoAddForced)
             {
                 if((out_ctr > 3) || mt_table[i].score < 1)
                 {
                    done = TRUE;
                    ok = FALSE;
                 } /* fi */

                 else if((out_ctr == 2) || (out_ctr == 3))  /* Pos 3 or 4 */
                 {
                    if(!mt_table[i].paths[MMI])
                    {
                       ok = FALSE;
                       out_ctr++;   /* False print so we don't lower items */
                    } /* fi */
                 } /* else fi */
             } /* fi */

             /* For TitleOnly abstracts, stop printing when below 10 on list,
                score < 190 and item isn't recommended by both MM and RC.
                ONLY if RSfilterTO and Title_Only are set do we check.
             */

             if(ok && (RSfilterTO && Title_Only) && !doNoAddForced)
             {
                if((out_ctr > RSfilterTO_rank) && !MMRCset(i) &&
                        (mt_table[i].score < RSfilterTO_limit))
                {
                   if(showRSfilter)
                    fprintf(fout, 
                       "\nRSfilterTO: Stopping on %s (%ld) [rank: %d]\n",
                       mt_table[i].mh, mt_table[i].score, out_ctr);
                   done = TRUE;
                   ok = FALSE;
                } /* fi */
             } /* fi */

             /* For Title and Abstract only citations, stop printing when
                below 13 on list, score < 203 without regard to paths.
                ONLY if RSfilterALL and !Title_Only do we check.
             */

             if(ok && (RSfilterALL && !Title_Only) && !doNoAddForced)
             {
                if((out_ctr > RSfilterALL_rank) &&
                        (mt_table[i].score < RSfilterALL_limit))
                {
                   if(showRSfilter)
                    fprintf(fout, 
                       "\nRSfilterALL: Stopping on %s (%ld) [rank: %d]\n",
                       mt_table[i].mh, mt_table[i].score, out_ctr);
                   done = TRUE;
                   ok = FALSE;
                } /* fi */
             } /* fi */

             if(ok)
             {
                out_ctr++;
                mt_table[i].hasPrinted = TRUE;
                savePrinted(i, 0, 1);
                tctr++;
             } /* fi ok */
          } /* fi oktoprint */

          else if(mt_table[i].treatLikePrinted)
             out_ctr++;
      } /* fi validDisplay */

      i++;
   } /* while max */

   /* Before we determine the final list of CTs, we need to go through
      remainder of the MH, NM, and CT lists to see what else will be added
      to make sure.
   */

   if(!noAnswer)
   {
      /* Print out any HMs that were added due to an NM that has printed,
         but hasn't printed yet because it didn't score high-enough.
      */

      for(i = 0; i < mt_count; i++)
      {
         if(mt_table[i].hasPrinted && (mt_table[i].datatype == NM))
         {
             for(k = 0; k < mt_table[i].num_HMs; k++)
             {
                ok = FALSE;
                index = find_uniq(mt_table[i].HMs[k]);

                if(index > -1)
                {
                    if(!mt_table[index].hasPrinted &&
                       (mt_table[index].score > 0) && mt_table[index].safe)
                      ok = TRUE;
                } /* fi */

                if(ok)
                {
                   out_ctr++;
                   mt_table[index].hasPrinted = TRUE;
                   savePrinted(index, 0, 3);
                } /* fi ok */
             } /* for each HM for this NM */
          } /* fi NM && hasPrinted */
      } /* for each MTI */

      /* Non Tree Top Geographic Term and Epidemiologic Studies Terms */

      for(i = 0; i < mt_count; i++)
      {
         if(!mt_table[i].hasPrinted && 
            ((mt_table[i].datatype == MH) || (mt_table[i].datatype == HM)) &&
            check_special_term(i))
         {
             if(mt_table[i].oktoprint)
             {
                out_ctr++;
                mt_table[i].hasPrinted = TRUE;

                strcpy(extras, "");
                if(mt_table[i].datatype == MH)
                {
                     if(mt_table[i].ETflag)
                       strcpy(extras, "ET");
                     else
                       strcpy(extras, "MH");
                } /* fi */
                else if(mt_table[i].datatype == CT)
                  strcpy(extras, "CT");
                else if(mt_table[i].datatype == NM)
                  strcpy(extras, "NM");
                else if(mt_table[i].datatype == HM)
                  strcpy(extras, "HM");
                else if(mt_table[i].datatype == SH)
                  strcpy(extras, "SH");

                if(strlen(mt_table[i].entry_term) > 0)
                  entryTerm = strdup(mt_table[i].entry_term);
                else
                  entryTerm = strdup(" ");

                if(mt_table[i].numPIs > 0)
                {
                    strcpy(posInfo, "");
                    for(k = 0; k < mt_table[i].numPIs; k++)
                    {
                        if(k > 0)
                          strcat(posInfo, ";");
                        sprintf(tmp, "%ld^%ld^%d\0",
                                mt_table[i].PIs[k].startPos,
                                mt_table[i].PIs[k].len,
                                mt_table[i].PIs[k].hasBreak);
                        strcat(posInfo, tmp);
                    } /* for */
                } /* fi */
                else
                  strcpy(posInfo, " ");

                strcpy(tmp, "");
                foo = whatLocs(i);
                strcpy(trigger, mt_table[i].trigger);
                if(strlen(trigger) == 0)
                  strcpy(trigger, " ");

                strcpy(tmp2, mt_table[i].textloc);
                if(strlen(tmp2) == 0)
                  strcpy(tmp2, " ");

                if(strlen(mt_table[i].mh_orig) > 0)
                  strcpy(mh_orig, mt_table[i].mh_orig);
                else
                  strcpy(mh_orig, " ");

                sprintf(tmp, "%s|%s|%s|%s|%d|%s|%s|%ld|%s|%s|%s|%s|\0",
                        trigger, posInfo, muid, mt_table[i].mh,
                        mt_table[i].ETflag, extras,
                        mt_table[i].dui, mt_table[i].score, foo, 
                        tmp2, mh_orig, entryTerm);
                free(foo);
                free(entryTerm);

                if(numBelow < MAX_BELOW)
                {
                    if(RTM_Debug)
                    {
                        fprintf(fout, "Adding #%s# to belowLineTmp[%ld]\n",
                                tmp, numBelow);
                        fflush(fout);
                    } /* fi */

                    belowLineTmp[numBelow++] = strdup(tmp);
                } /* fi */
             } /* fi oktoprint */
         } /* fi check_special_term */
      } /* for */

      /* CTs within regular list */

      for (i = 0; i < mt_count; i++) 
      {
         if((mt_table[i].datatype == CT) && !mt_table[i].hasPrinted)
         {
             if(mt_table[i].oktoprint &&
                (!strictFilter_set || validDisplay(i) || addCTs))
             {
                 /* Check to see if CT is also in the CT table.  If
                    it is, see which has bigger explanation and use
                    that.
                 */

                 okCT = TRUE;
                 ctPos = lookForCT(mt_table[i].dui);
                 if(ctPos > -1)
                 {
                     if(strlen(ct_table[ctPos].trigger) >
                        strlen(mt_table[i].trigger))
                       okCT = FALSE;
                 } /* fi */

                 if(okCT)
                 {
                    out_ctr++;
                    mt_table[i].hasPrinted = TRUE;

                    strcpy(extras, "");
                    if(mt_table[i].datatype == MH)
                    {
                         if(mt_table[i].ETflag)
                           strcpy(extras, "ET");
                         else
                           strcpy(extras, "MH");
                    } /* fi */
                    else if(mt_table[i].datatype == CT)
                      strcpy(extras, "CT");
                    else if(mt_table[i].datatype == NM)
                      strcpy(extras, "NM");
                    else if(mt_table[i].datatype == HM)
                      strcpy(extras, "HM");
                    else if(mt_table[i].datatype == SH)
                      strcpy(extras, "SH");

                    if(strlen(mt_table[i].entry_term) > 0)
                      entryTerm = strdup(mt_table[i].entry_term);
                    else
                      entryTerm = strdup(" ");

                    if(mt_table[i].numPIs > 0)
                    {
                        strcpy(posInfo, "");
                        for(k = 0; k < mt_table[i].numPIs; k++)
                        {
                            if(k > 0)
                              strcat(posInfo, ";");
                            sprintf(tmp, "%ld^%ld^%d\0",
                                    mt_table[i].PIs[k].startPos,
                                    mt_table[i].PIs[k].len,
                                    mt_table[i].PIs[k].hasBreak);
                            strcat(posInfo, tmp);
                        } /* for */
                    } /* fi */
                    else
                      strcpy(posInfo, " ");

                    strcpy(tmp, "");
                    foo = whatLocs(i);
                    strcpy(trigger, mt_table[i].trigger);
                    if(strlen(trigger) == 0)
                      strcpy(trigger, " ");

                    strcpy(tmp2, mt_table[i].textloc);
                    if(strlen(tmp2) == 0)
                      strcpy(tmp2, " ");

                    if(strlen(mt_table[i].mh_orig) > 0)
                      strcpy(mh_orig, mt_table[i].mh_orig);
                    else
                      strcpy(mh_orig, " ");

                    sprintf(tmp, "%s|%s|%s|%s|%d|%s|%s|%ld|%s|%s|%s|%s|\0",
                            trigger, posInfo, muid, mt_table[i].mh,
                            mt_table[i].ETflag, extras,
                            mt_table[i].dui, mt_table[i].score, foo, tmp2,
                            mh_orig, entryTerm);
                    free(foo);
                    free(entryTerm);

                    if(numBelow < MAX_BELOW)
                    {
                        if(RTM_Debug)
                        {
                            fprintf(fout, 
                                   "II Adding #%s# to belowLineTmp[%ld]\n",
                                    tmp, numBelow);
                            fflush(fout);
                        } /* fi */

                        belowLineTmp[numBelow++] = strdup(tmp);
                    } /* fi */
                 } /* fi okCT */
             } /* fi oktoprint */
         } /* fi */
      } /* for */
   } /* fi */

   if(!noAnswer)
   {
      if(!doNoAddForced)
        check_SHs();

      if(!doNoAddForced && addCTs)
      {
          if(RTM_Debug)
          {
              fprintf(fout, "Calling check_CTs ...\n");
              fflush(fout);
          } /* fi */
          check_CTs(TRUE);

          /* Print out any items that we added via check_CTs. */

         added = FALSE;
         for(i = 0; i < mt_count; i++)
         {
            if(!mt_table[i].hasPrinted && mt_table[i].forcePrint &&
               mt_table[i].oktoprint)
            {
                out_ctr++;
                added = TRUE;
                mt_table[i].hasPrinted = TRUE;
                savePrinted(i, 0, 6);
            } /* fi */
         } /* for */

         /* Now, if we added anything, rerun the checkCTs */

         if(added)
          check_CTs(FALSE);
      } /* fi addCTs */
   } /* fi !noAnswer */

   /* Go through mt_table make sure we got everything before adding non-leaf */

   for(i = 0; i < mt_count; i++)
   {
       if(!mt_table[i].hasPrinted && mt_table[i].forcePrint &&
           mt_table[i].oktoprint)
       {
           out_ctr++;
           mt_table[i].hasPrinted = TRUE;
           savePrinted(i, 0, 6);
       } /* fi */
   } /* for */

   /* Look at Non-Leaf Forced Terms here */

   printNonLeaf();

   /* Go through mt_table one last time to make sure we got everything */

   for(i = 0; i < mt_count; i++)
   {
       if(!mt_table[i].hasPrinted && mt_table[i].forcePrint &&
           mt_table[i].oktoprint)
       {
           ok3 = TRUE;
           if(isCTDUI(mt_table[i].dui))
           {
              pos = checkFinalCT(mt_table[i].mh);
              if(pos > -1)
              {
                  if(!finalCTlist[jgm].oktoprint)
                    ok3 = FALSE;
              } /* fi */
           } /* fi */

           if(ok3)
           {
              out_ctr++;
              mt_table[i].hasPrinted = TRUE;
              savePrinted(i, 0, 6);
           } /* fi */
       } /* fi */

       /* Want to check and see if trigger has changed with extra info */

       else if(mt_table[i].hasPrinted && isCTDUI(mt_table[i].dui) &&
               mt_table[i].oktoprint)
       {
           found = FALSE;
           for(jgm = 0; !found && (jgm < numFinalCTs); jgm++)
           {
               if(mt_table[i].dui == finalCTlist[jgm].dui)
               {
                   found = TRUE;
                   if(strlen(mt_table[i].trigger) >
                      strlen(finalCTlist[jgm].trigger))
                   {
                       free(finalCTlist[jgm].trigger);
                       finalCTlist[jgm].trigger = strdup(mt_table[i].trigger);
                   } /* fi */
               } /* fi */
           } /* for */

           if(!found) /* Make sure we add it at this point! */
             savePrinted(i, 0, 6);
       } /* else fi */
   } /* for */

   /* Final check of the CTs after Non-Leaf have printed */

   if(!doNoAddForced && addCTs)
     check_CTs(FALSE);

   checkNewOtherCoordRules();

   haveHumansBelowLine = FALSE;
   if((numBelow > 0)  && !doNoAddForced)
   {
       for(i = 0; !haveHumansBelowLine && (i < numBelow); i++)
       {
           if(strstr(belowLineTmp[i], "|Humans|") != NULL)
              haveHumansBelowLine = TRUE;
       } /* for */

       for(i = 0; i < numBelow; i++)
       {
            if(strlen(belowLineTmp[i]) > 0)
              savePrinted(i, 4, 12);
       } /* for */
   } /* fi */

   /* If we want the full list, we need to add information into the OVF list */

   if(which == FULL_DISPLAY)
   {
       for(i = 0; i < mt_count; i++)
       {
           if(!mt_table[i].hasPrinted)
             saveOverFlow(i, 0);
       } /* for each mt_count entry */
   } /* fi */

   /*************************** BELOW THE LINE *******************************/

   /* CTs List */

   printCTs(strictFilter_set, which);

   /* modified Sept 25, 00 to print subheadings according to rank */

   for (i = 0; (i < max) && (mt_count > 0); i++)
   {
         if((mt_table[i].datatype == SH) && (mt_table[i].score > 200))
         {
            if(mt_table[i].oktoprint && (!strictFilter_set || validDisplay(i)))
            {
               out_ctr++;
               mt_table[i].hasPrinted = TRUE;
               savePrinted(i, 0, 7);
            } /* fi validDisplay */
         } /* fi */
   } /* for */

   /* SH List */

   printSHs(strictFilter_set, which);

   for(i = 0; i < numBelow; i++)
     free(belowLineTmp[i]);

   if(!doNoAddForced)
   {
       filterNeoplasms();
       filterCancer();

       filterMentalDisorders();
       filterGenSpec();

       if(strcmp(nlmID, IntJEatDisord) == 0)
         filterIJED();

       if(preIndex)
         filterPreIndex(FALSE);

       else if(MTI_AUTO || ACCME)
       {
           filterPreIndex(TRUE);  /* Includes filterML(FALSE) */
           filterHeuristicN();
       } /* fi */

       else if(level1Filter)
         filterPreIndex(TRUE);

       /* Added filtering 4/4/14 */

       if(firstLineJournal || preIndex || level1Filter || OLDMEDLINE)
         filterBodyPartDisease();

       if(MeSHonDemand)
         filterMeSHonDemand();

       if(OLDMEDLINE)
         filterOLDMEDLINE();

       /* Handle Zika on it's own */

       checkZika_2016();
   } /* fi */

   /* Check on what we are returning */

   numValidCT = numValidMH = 0;
   for(i = 0; i < numFinalCTs; i++)
   {
       if(finalCTlist[i].oktoprint)
         numValidCT++;
   } /* for */

   /* Remove all of the subheadings if that is all we have, or if we
      only have CTs and SHs.
   */

   if(numValidMH == 0)
   {
       for(i = 0; i < numFinalSHs; i++)
       {
           if(RTM_Debug)
             fprintf(fout, "Only SH, Remove SH: %s\n", finalSHlist[i].mh);

           finalSHlist[i].oktoprint = FALSE;
       } /* for */
   } /* fi */

   for(i = 0; i < numFinalMHs; i++)
   {
       if(finalMHlist[i].oktoprint)
         numValidMH++;
   } /* for */

   /* MTIFL (only MTIFL AND Chinese Journals and NOT Preliminary MeSH requires
      at least one result and the default result is Humans.

      24717340|Humans|C0086418|1000|CT|Antonio ML Rule Forced||MM

   */

   if((firstLineJournal || MTI_AUTO || MTI_REVIEW) && !BioASQ)
   {
       /* If we don't have any CTs or MHs, add Humans */

       if((numValidCT + numValidMH) == 0)
       {
           finalCTlist[0].ETflag = FALSE;
           finalCTlist[0].datatype = CT;
           finalCTlist[0].oktoprint = TRUE;
           finalCTlist[0].wasForced = TRUE;
           finalCTlist[0].score = 1000;
           finalCTlist[0].num_RCs = 0;
           finalCTlist[0].VocabDenFactor = 0.0;
           finalCTlist[0].muid = strdup(muid);
           finalCTlist[0].mh = strdup("Humans");
           finalCTlist[0].mh_orig = strdup("");
           finalCTlist[0].dui = strdup("D006801");
           finalCTlist[0].entry_term = strdup("");
           finalCTlist[0].RC_PMIDs = strdup("");
           finalCTlist[0].trigger = strdup("Forced MTIFL Default Humans");
           finalCTlist[0].textloc = strdup("");
           finalCTlist[0].numPIs = 0;
           finalCTlist[0].paths[MMI] = TRUE;
           finalCTlist[0].num_treecodes = 0;
           pullTREEs_CT(0);
           finalCTlist[0].num_STs = 0;
           numFinalCTs = 1;
/*
        fprintf(fout, 
          "%s|Humans|C0086418|1000|CT|Forced MTIFL Default Humans||MM\n", muid);
*/

       } /* fi */
   } /* fi */

   /* Make sure all of the CTs have some score

      See if we need to add Humans here triggered by something we are
      recommending.  07/09/19
   */

   foundHuman = foundHumanNoPrint = foundHumanTrigger = FALSE;
   humanPos = -1;
   strcpy(humanTrigger, "");
   for(i = 0; i < numFinalCTs; i++)
   {
       if(strcmp(finalCTlist[i].mh, "Humans") == 0)
       {
           humanPos = i;
           if(finalCTlist[i].oktoprint)
             foundHuman = TRUE;
           else
             foundHumanNoPrint = TRUE;
       } /* fi */

       finalCTlist[i].score = highScore + 1000;
   } /* for */

   /* If we have any MHs marked as HMs that have additional pathways, modify
      at this point.  6/9/16

      See if we need to add Humans here triggered by something we are
      recommending.  07/09/19
   */

   foundHumanTrigger = FALSE;
   for(i = 0; !foundHumanTrigger && (i < numFinalMHs); i++)
   {
       if((finalMHlist[i].datatype == MH) && finalMHlist[i].oktoprint)
       {
           for(l = 0; (l < finalMHlist[i].num_treecodes); l++)
           {
               /* Persons MeSH Tree - trigger Humans */

               if(finalMHlist[i].treecodes[l][0] == 'M')
               {
                   if(strstr(finalMHlist[i].treecodes[l], "M01") != NULL)
                   {
                       foundHumanTrigger = TRUE;
                       strcpy(humanTrigger, finalMHlist[i].mh);
                   } /* fi */
               } /* fi M */
           } /* for */
       } /* fi */

       if((finalMHlist[i].datatype == HM) && finalMHlist[i].oktoprint)
       {
           if(strstr(finalMHlist[i].trigger, "Forced") != NULL)
             finalMHlist[i].datatype = MH;
       } /* fi */
   } /* for */

   if(!doNoAddForced)
   {
       if(foundHumanTrigger && !foundHuman)
         checkAddHumans(foundHumanNoPrint, humanPos, humanTrigger);

       if(doIL2R || doIL2RHybrid)
         doLearning2Rank(BioASQ);

       if(!veterinaryJournal)
         checkForAgeYears();

       checkMaleFemalePct();

       if(doLookAtConf)
         lookAtConfidence();

       Final_Treecode_CT_Check();

       checkBeforeFinalPrint();

       if(veterinaryJournal)
         lookForAnimalDiseaseAdds();

       /* Only keep GEOs if they are found in the title of the article
          for the Zootaxa (101179386) journal.  Per Susan's email 11/15/19.
       */

       if(strcmp(nlmID, "101179386") == 0)
       {
           for(i = 0; i < numFinalMHs; i++)
           {
                if(finalMHlist[i].oktoprint)
                {
                   if(amIGeo_Final(i))
                   {
                       if(strstr(finalMHlist[i].textloc, "TI") == NULL)
                       {
                           if(RTM_Debug)
                           {
                               fprintf(fout,
                                  "Remove GEOs from Zootax AB Only: #%s#\n",
                                  finalMHlist[i].mh);
                               fflush(fout);
                           } /* fi */

                           finalMHlist[i].oktoprint = FALSE;
                           finalMHlist[i].score = -1;
                       } /* fi */
                   } /* fi */
                } /* fi */
           } /* for */
       } /* fi */
   } /* fi !doNoAddForced */

   /* Go through and see which terms are only coming from full text or
      the full text tables.
   */

   if(RTM_Debug)
   {
       fprintf(fout, "Entering Full Text Clarification: %d %ld\n",
               haveFullText, fullTextStartPos);
       fflush(fout);
   } /* fi */

   if(haveFullText && (fullTextStartPos > -1))
   {
       for(i = 0; i < numFinalCTs; i++)
       {
            if(finalCTlist[i].oktoprint)
            {
                FTSpos = -1;
                foundOther = foundFullText = foundFullText_Table = FALSE;
                if(finalCTlist[i].numPIs > 0)
                {
                   for(k = 0; k < finalCTlist[i].numPIs; k++)
                   {
                       if(finalCTlist[i].PIs[k].startPos < fullTextStartPos)
                         foundOther = TRUE;

                       else if(finalCTlist[i].PIs[k].startPos <
                                                      fullTextTableStartPos)
                       {
                           foundFullText = TRUE;
                           FTSpos = findSnippet(finalCTlist[i].PIs[k].startPos);
                       } /* else fi */

                       else if(finalCTlist[i].PIs[k].startPos >=
                                                        fullTextStartPos)
                       {
                           foundFullText_Table = TRUE;
                           FTSpos = findSnippet(finalCTlist[i].PIs[k].startPos);
                       } /* else fi */
                   } /* for */

                   if(!foundOther && (foundFullText || foundFullText_Table))
                   {
                       if(FTSpos > -1)
                       {
                           if(strcmp(FT_Snippets[FTSpos].snippetType,
                                     "SECTION") == 0)
                           {
                             sprintf(trigger,
                               "Forced Full Text: %s found in Section: %s\0",
                               finalCTlist[i].trigger,
                               FT_Snippets[FTSpos].snippetName);
                           } /* fi */

                           else /* TABLE */
                           {
                             sprintf(trigger,
                              "Forced Full Text: %s found in Table: %s (%s)\0",
                              finalCTlist[i].trigger,
                              FT_Snippets[FTSpos].snippetName,
                              FT_Snippets[FTSpos].snippetCaption);
                           } /* else */
                       } /* fi */

                       else
                         sprintf(trigger, "Forced Full Text: %s\0",
                                 finalCTlist[i].trigger);
                       free(finalCTlist[i].trigger);
                       finalCTlist[i].trigger = strdup(trigger);

                       free(finalCTlist[i].textloc);
                       finalCTlist[i].textloc = strdup("FT");
                   } /* fi */
                } /* fi */
            } /* fi */
       } /* for */

       for(i = 0; i < numFinalMHs; i++)
       {
            if(finalMHlist[i].oktoprint)
            {
                FTSpos = -1;
                foundOther = foundFullText = foundFullText_Table = FALSE;
                if(finalMHlist[i].numPIs > 0)
                {
                   for(k = 0; k < finalMHlist[i].numPIs; k++)
                   {
                       if(finalMHlist[i].PIs[k].startPos < fullTextStartPos)
                         foundOther = TRUE;

                       else if(finalMHlist[i].PIs[k].startPos <
                                                          fullTextTableStartPos)
                       {
                           foundFullText = TRUE;
                           FTSpos = findSnippet(finalMHlist[i].PIs[k].startPos);
                       } /* else fi */

                       else if(finalMHlist[i].PIs[k].startPos >=
                                                          fullTextStartPos)
                       {
                           foundFullText_Table = TRUE;
                           FTSpos = findSnippet(finalMHlist[i].PIs[k].startPos);
                       } /* else fi */
                   } /* for */

                   if(!foundOther && (foundFullText || foundFullText_Table))
                   {
                       /* If Geographical only from Full Text, remove
                          per Susan email 1/18/19.
                       */

                       if(amIGeo_Final(i))
                       {
                           if(RTM_Debug)
                           {
                               fprintf(fout,
                                  "Remove GEOs from FullText Only: #%s#\n",
                                  finalMHlist[i].mh);
                               fflush(fout);
                           } /* fi */

                           finalMHlist[i].oktoprint = FALSE;
                           finalMHlist[i].score = -1;
                       } /* fi */

                       else
                       {
                           if(RTM_Debug)
                           {
                               fprintf(fout, "Add Full Text Prior: #%s#\n",
                                        finalMHlist[i].trigger);
                               fflush(fout);
                           } /* fi */

                           if(FTSpos > -1)
                           {
                               if(strcmp(FT_Snippets[FTSpos].snippetType,
                                         "SECTION") == 0)
                               {
                                 sprintf(trigger,
                                 "Forced Full Text: %s found in Section: %s\0",
                                   finalMHlist[i].trigger,
                                   FT_Snippets[FTSpos].snippetName);
                               } /* fi */

                               else /* TABLE */
                               {
                                 sprintf(trigger,
                              "Forced Full Text: %s found in Table: %s (%s)\0",
                                  finalMHlist[i].trigger,
                                  FT_Snippets[FTSpos].snippetName,
                                  FT_Snippets[FTSpos].snippetCaption);
                               } /* else */
                           } /* fi */

                           else
                             sprintf(trigger, "Forced Full Text - %s\0",
                                     finalMHlist[i].trigger);

                           free(finalMHlist[i].trigger);
                           finalMHlist[i].trigger = strdup(trigger);

                           free(finalMHlist[i].textloc);
                           finalMHlist[i].textloc = strdup("FT");

                           if(RTM_Debug)
                           {
                               fprintf(fout, "Add Full Text After: #%s#\n",
                                        finalMHlist[i].trigger);
                               fflush(fout);
                           } /* fi */
                       } /* else ok */
                   } /* fi */
                } /* fi */
            } /* fi */
       } /* for */
   } /* fi haveFullText */

   if(RTM_Debug)
   {
       fprintf(fout, "Exit Full Text Clarification\n");
       fflush(fout);
   } /* fi */


   /* If ACCME, remove CheckTags and anything that is not tied
      directly to the actual text we are processing (no PRC Only,
      no purely machine learning).  Also, remove any geographics.
   */

   if(ACCME)
   {
       if(RTM_Debug)
       {
           fprintf(fout, "ACCME Filtering:\n");
           fflush(fout);
       } /* fi */

       /* Turn off all CheckTags */

       for(i = 0; i < numFinalCTs; i++)
       {
           finalCTlist[i].oktoprint = FALSE;
           if(RTM_Debug)
           {
               fprintf(fout, "ACCME: Remove CT: #%s#\n", finalCTlist[i].mh);
               fflush(fout);
           } /* fi */
       } /* for */

       /* Remove Geographics and anything not tied directly to the text */

       for(i = 0; i < numFinalMHs; i++)
       {
           if((finalMHlist[i].datatype == MH) && finalMHlist[i].oktoprint)
           {
                foundT = FALSE;
                for(l = 0; !foundT && (l < finalMHlist[i].num_treecodes); l++)
                {
                     if(finalMHlist[i].treecodes[l][0] == 'Z')
                     {
                         foundT = TRUE;
                         finalMHlist[i].oktoprint = FALSE;
                         if(RTM_Debug)
                         {
                             fprintf(fout, "ACCME: Remove GEO: #%s#\n",
                                     finalMHlist[i].mh);
                             fflush(fout);
                         } /* fi */
                     } /* fi "Z" */
                } /* for  each treecode */

                /* Make sure something from text */

                if(!finalMHlist[i].paths[MMI])
                {
                    finalMHlist[i].oktoprint = FALSE;
                    if(RTM_Debug)
                    {
                        fprintf(fout, "ACCME: Remove MH (not in text): #%s#\n",
                                finalMHlist[i].mh);
                        fflush(fout);
                    } /* fi */
                } /* fi */
           } /* fi */
       } /* for */
   } /* fi ACCME */

   /* Check to make sure we are not recommending both the PT and it's "as Topic" term.  
      If we have the "as Topic" term, remove the PT from indexing.
   */

   if(numPTsFound > 0)
   {
       for(i = 0; i < numPTsFound; i++)
       {
           if(foundPTsList[i].oktoprint)
           {
               foundPT = FALSE;
               for(x = 0; !foundPT && (x < numCheckPTs); x++)
               {
                   if(foundPTsList[i].mh[0] == casTPTlist[x].PT[0])
                   {
                       if(strcmp(foundPTsList[i].mh, casTPTlist[x].PT) == 0)
                       {
                           foundPT = TRUE;
                           if(checkFinalMH(casTPTlist[x].asTopic))
                           {
                               foundPTsList[i].oktoprint = FALSE;
                               if(RTM_Debug)
                               {
                                   fprintf(fout, 
                                     "PT List: Remove '%s' PT Found as Topic: '%s'\n",
                                      foundPTsList[i].mh, casTPTlist[x].asTopic);
                                   fflush(fout);
                               } /* fi */
                           } /* fi */
                       } /* fi */
                   } /* fi */
               } /* for */
           } /* fi oktoprint */
       } /* for */

       /* Check for Review & Systematic Review being indexed by MTIA */

       foundReviewPT = foundSysReviewPT = FALSE;
       reviewPos = -1;
       for(i = 0; i < numPTsFound; i++)
       {
           if(foundPTsList[i].oktoprint)
           {
               if(strcmp(foundPTsList[i].mh, "Review") == 0)
               {
                   /* Want to keep special case where Systematic Review also added */

                   if(strstr(foundPTsList[i].trigger, "Forced Pub Type Lookup Special") == NULL)
                   {
                       foundReviewPT = TRUE;
                       reviewPos = i;
                   } /* fi */
               } /* fi */

               else if(strcmp(foundPTsList[i].mh, "Systematic Review") == 0)
                 foundSysReviewPT = TRUE;
           } /* fi oktoprint */
       } /* for */

       if(foundReviewPT && foundSysReviewPT && (reviewPos > -1))
       {
           foundPTsList[reviewPos].oktoprint = FALSE;
           if(RTM_Debug)
           {
               fprintf(fout, 
                "PT List: Remove Review PT since Found Systematic Review PT\n");
               fflush(fout);
           } /* fi */
       } /* fi */

       /* Look at Protocol Rules - "protocol" in Title */

       if(foundInText("protocol", FALSE, TRUE))
         check_PTs_Protocol();

       /* Make sure we don't have Review, Systematic Review, and Meta-analysis with any of
          the Clinical Trials or RCTs.
       */

       check_PTs_Reviews();
   } /* fi numPTsFound > 0 */

   /* Final check here to see if we have anything to print out or not.
   */

   if((firstLineJournal || MTI_AUTO || MTI_REVIEW) && !BioASQ)
   {
       numValidCT = numValidMH = 0;
       for(i = 0; i < numFinalCTs; i++)
       {
           if(finalCTlist[i].oktoprint)
             numValidCT++;
       } /* for */

       for(i = 0; i < numFinalMHs; i++)
       {
           if(finalMHlist[i].oktoprint)
             numValidMH++;
       } /* for */

       if((numValidCT + numValidMH) == 0)
       {
           finalCTlist[0].ETflag = FALSE;
           finalCTlist[0].datatype = CT;
           finalCTlist[0].oktoprint = TRUE;
           finalCTlist[0].wasForced = TRUE;
           finalCTlist[0].score = 1000;
           finalCTlist[0].num_RCs = 0;
           finalCTlist[0].VocabDenFactor = 0.0;
           finalCTlist[0].muid = strdup(muid);
           finalCTlist[0].mh = strdup("Humans");
           finalCTlist[0].mh_orig = strdup("");
           finalCTlist[0].dui = strdup("D006801");
           finalCTlist[0].entry_term = strdup("");
           finalCTlist[0].RC_PMIDs = strdup("");
           finalCTlist[0].trigger = strdup("Forced MTIFL Default Humans");
           finalCTlist[0].textloc = strdup("");
           finalCTlist[0].numPIs = 0;
           finalCTlist[0].paths[MMI] = TRUE;
           finalCTlist[0].num_treecodes = 0;
           pullTREEs_CT(0);
           finalCTlist[0].num_STs = 0;
           numFinalCTs = 1;
       } /* fi */
   } /* fi */

    /* Go through and look at items we marked with AMBIG/METAPHOR.  We are looking here to see 
       if any other trigger actually triggered the same term but wasn't ambiguous.  If so, we
       want to remove the AMBIG/METAPHOR tag since we have collaborating evidence it is ok from
       something that didn't trigger the tag.
    */

    for(i = 0; i < numFinalCTs; i++)
    {
        if(finalCTlist[i].oktoprint)
        {
            if(strstr(finalCTlist[i].trigger, "AMBIG/METAPHOR") != NULL)
            {
                foo = verifyAmbig_Metaphor(finalCTlist[i].trigger);
                if(strlen(foo) > 0)
                {
                    free(finalCTlist[i].trigger);
                    finalCTlist[i].trigger = strdup(foo);
                } /* fi */

                free(foo);
            } /* fi */
        } /* fi */
    } /* for each CT */

    for(i = 0; i < numFinalMHs; i++)
    {
        if(finalMHlist[i].oktoprint)
        {
            if(strstr(finalMHlist[i].trigger, "AMBIG/METAPHOR") != NULL)
            {
                foo = verifyAmbig_Metaphor(finalMHlist[i].trigger);
                if(strlen(foo) > 0)
                {
                    free(finalMHlist[i].trigger);
                    finalMHlist[i].trigger = strdup(foo);
                } /* fi */

                free(foo);
            } /* fi */
        } /* fi */
    } /* for each MH */

   if(BioASQ)
     printFinal(BioASQ_DISPLAY, starMHTI_set, justFacts, show_id,
                show_labels, OKonly);

   else if(MoD_Raw)
     print_MoD_Raw();

   else if(JSON)
     print_JSON();

   else if(showIlyaML)
     print_IlyaML();

   else
   {
       if(MeSHonDemand && !MeSHonDemand2)
       {
           if(MoD_PP)
             fprintf(fout, "%s|", muid);

           fprintf(fout, "PRC|");
           numUsed = 0;
           for(i = 0; i < numPRC; i++)
           {
               if(PRC_Scores[i] > -1)
               {
                   if(numUsed > 0)
                     fprintf(fout, ";");
                   fprintf(fout, "%s", PRC_PMIDs[i]);
                   numUsed++;
               } /* fi */
           } /* for */
           fprintf(fout, "|\n");
       } /* fi */

       printFinal(which, starMHTI_set, justFacts, show_id, show_labels, OKonly);
   } /* else */

   if(numOverallPrinted == 0)
   {
       if(showNoTerms)
         fprintf(fout, "NO_TERMS\n");

       else if((which == DEFAULT_DISPLAY) && !BioASQ && !showIlyaML)
         fprintf(fout, "%s|||||||\n", muid);
   } /* fi */

   if(showRemovals || showScoresFull)
     printRemovals();
} /* combined_display */

/***************************************************************************
*                                                                          *
***************************************************************************/

void printCTs(int strictFilter_set, int which)
{
   int MMI_flag, REL_flag, TRG_flag, found;
   long i;
   char dLabel[10];
   char *foo;

   strcpy(dLabel, "");

   for(i = 0; i < ct_count; i++) 
   {
      found = lookFor(ct_table[i].dui, mt_count);
      if(!found && ct_table[i].oktoprint)  /* JGM 081311 - added oktoprint */
      {
         MMI_flag = ct_table[i].paths[MMI];
         TRG_flag = ct_table[i].paths[TRG];
         REL_flag = ct_table[i].paths[PRC];

         if(!strictFilter_set || (MMI_flag && REL_flag) || addCTs)
         {
            out_ctr++;
            ct_table[i].hasPrinted = TRUE;
            savePrinted(i, 1, 9);
         } /* fi ctr */
      } /* fi !found */

      /* If FULL_DISPLAY, we can add anything not a dup or printed */

      if(which == FULL_DISPLAY)
      {
          if(!found && !ct_table[i].hasPrinted)
            saveOverFlow(i, 1);
      } /* fi */
   } /* for */
} /* printCTs */

/***************************************************************************
*                                                                          *
***************************************************************************/

void printSHs(int strictFilter_set, int which)
{
   int MMI_flag, REL_flag, TRG_flag, found;
   long i;
   char dLabel[10];
   char *foo;

   strcpy(dLabel, "");
   for(i = 0; i< sh_count; i++)
   {
      found = lookFor(sh_table[i].dui, mt_count);
      if(!found)
      {
         MMI_flag = sh_table[i].paths[MMI];
         TRG_flag = sh_table[i].paths[TRG];
         REL_flag = sh_table[i].paths[PRC];

         if(sh_table[i].oktoprint && 
                (!strictFilter_set || (MMI_flag && REL_flag)))
         {
            out_ctr++;
            sh_table[i].hasPrinted = TRUE;
            savePrinted(i, 3, 10);
         } /* fi ok */
      } /* fi !found */

      /* If FULL_DISPLAY, we can add anything not a dup or printed */

      if(which == FULL_DISPLAY)
      {
          if(!found && !sh_table[i].hasPrinted)
            saveOverFlow(i, 3);
      } /* fi */
   } /* for */
} /* printSHs */

/***************************************************************************
*                                                                          *
***************************************************************************/

void display_output(int strictFilter_set, int medFilter_set, 
       int medFilterR_set, int starMHTI_set, int expanded_flag,
       int justFacts, int BioASQ)
{
   combined_display(strictFilter_set, medFilter_set, medFilterR_set, 
           starMHTI_set, expanded_flag, DEFAULT_DISPLAY, FALSE, FALSE,
           FALSE, justFacts, BioASQ, FALSE);

   if(numBioASQPrinted > 0)
     fprintf(fout, "], \"pmid\":%s},\n", muid);
} /* display_output */

/***************************************************************************
*                                                                          *
***************************************************************************/

void display_output_list(int strictFilter_set, int medFilter_set,
                         int starMHTI_set, int showNoTerms)
{
    combined_display(strictFilter_set, medFilter_set, FALSE, starMHTI_set,
                     FALSE, LIST_DISPLAY, showNoTerms, FALSE, FALSE, FALSE,
                     FALSE, FALSE);
} /* display_output_list */

/***************************************************************************
*                                                                          *
***************************************************************************/

int validDisplay(int pos)
{
   int rtn = FALSE;

   if(mt_table[pos].paths[MMI] && mt_table[pos].paths[PRC])
     rtn = TRUE;

   return(rtn);
} /* validDisplay */

/***************************************************************************
*                                                                          *
***************************************************************************/

int amIGeographical(int pos)
{
   int rtn = FALSE;
   int i;
   int found;

   if (mt_table[pos].num_treecodes > 0) 
   {
      found = FALSE;
      for (i = 0; !found && (i < mt_table[pos].num_treecodes); i++) 
      {
          if(mt_table[pos].treecodes[i][0] == 'Z')
          {
             found = TRUE;
             rtn = TRUE;
          } /* fi */
      } /* for */
   } /* fi */

   return(rtn);
} /* amIGeographical */

/***************************************************************************
*                                                                          *
***************************************************************************/

int amIGeo_Final(int pos)
{
   int rtn = FALSE;
   int i;
   int found;

   if (finalMHlist[pos].num_treecodes > 0) 
   {
      found = FALSE;
      for (i = 0; !found && (i < finalMHlist[pos].num_treecodes); i++) 
      {
          if(finalMHlist[pos].treecodes[i][0] == 'Z')
          {
             found = TRUE;
             rtn = TRUE;
          } /* fi */
      } /* for */
   } /* fi */

   return(rtn);
} /* amIGeo_Final */


char *whatLocs(int mt_pos)
{
   char *rtn = (char *)malloc(4096);

   strcpy(rtn, "");

   if(mt_table[mt_pos].paths[MMI])
    strcpy(rtn, "MM");

   if(mt_table[mt_pos].paths[TRG])
   {
      if(strlen(rtn) > 0)
        strcat(rtn, ";");
      strcat(rtn, "TG");
   } /* fi */

   if(mt_table[mt_pos].paths[PRC])
   {
      if(strlen(rtn) > 0)
        strcat(rtn, ";");
      strcat(rtn, "RC");
   } /* fi */
   
   if(strlen(rtn) == 0)
    strcpy(rtn, "N/A");

   return(rtn);
} /* whatLocs */


int getTitleOnlyLimit()
{
   int rtn = 7;

   switch(TitleNumWords)
   {
       case 0:
       case 1:
       case 2:
          rtn = 7;
       break;

       case 3:
       case 4:
          rtn = 8;
       break;

       case 5:
       case 6:
          rtn = 10;
       break;

       case 7:
       case 8:
       case 9:
       case 10:
          rtn = 11;
       break;

       case 11:
       case 12:
       case 13:
       case 14:
          rtn = 12;
       break;

       case 15:
       case 16:
       case 17:
       case 18:
          rtn = 13;
       break;

       case 19:
       case 20:
       case 21:
          rtn = 14;
       break;

       default: /* > 21 */
          rtn = 13;
   } /* switch */

   return(rtn);
} /* getTitleOnlyLimit */


int MMRCset(int mt_pos)
{
   int rtn = FALSE;

   if(mt_table[mt_pos].paths[MMI] && mt_table[mt_pos].paths[PRC])
     rtn = TRUE;

   return(rtn);
} /* MMRCset */


int lookFor(char *DUI, long endPos)
{
    int rtn = FALSE;
    long j;

    for(j = 0; !rtn && (j < endPos); j++)
    {
        if(mt_table[j].hasPrinted)
        {
            if(strcmp(DUI, mt_table[j].dui) == 0)
              rtn = TRUE;
        } /* fi */
    } /* for */

    return(rtn);
} /* lookFor */

/* Geographics (Z01), Epidemiologic Studies (E05 & N05 & N06) */

int check_special_term(int index)
{
   int i, j, n, k, ok;
   char tree_num[256];
   int special_tree_count = 4;
   char *special_tree[] = { "Z01", "E05.318.760.500", "N05.715.360.775.175",
                            "N06.850.520.450.500" };
   char *mh, *et;
   long z, lenMH, lenET;

   if((mt_table[index].num_treecodes != 0) && mt_table[index].safe)
   {
      for(i = 0; i < mt_table[index].num_treecodes; i++)
      {
          strcpy(tree_num, mt_table[index].treecodes[i]);

          /* If not a general category heading */

          if((strcmp(tree_num, "Z01") != 0) && 
             (strcmp(tree_num, "Z01.433") != 0) &&  /* Cities */
             (strcmp(tree_num, "Z01.586") != 0))    /* Hist Geog Locations */
          {
             for(j = 0; j < special_tree_count; j++) 
             {
                if((strstr(tree_num, special_tree[j]) != NULL) &&
                    mt_table[index].paths[MMI])
                  return(TRUE);
             } /* for */
          } /* fi */
      } /* for */
   } /* fi */

   return(FALSE);
} /* check_special_term */

/***************************************************************************
*
*  lookForCT --
*
*      This 
*
***************************************************************************/

long lookForCT(char *DUI)
{
    long rtn = -1;
    long j;
    int found;

    found = FALSE;
    for(j = 0; !found && (j < ct_count); j++)
    {
        if(strcmp(DUI, ct_table[j].dui) == 0)
        {
            rtn = j;
            found = TRUE;
        } /* fi */
    } /* for */

    return(rtn);
} /* lookForCT */

/***************************************************************************
*
*  modifyBelowLine --
*
*      This routine simply adds the trigger to the trigger information for
*  the MH if it is not already there.  These needed to be put into a list
*  prior to doing the final review and simply need to be updated prior to
* displaying.
*
***************************************************************************/

void modifyBelowLine(char *MHname, char *trig, char *posInfo)
{
    long i, j, len;
    int found;
    char lookFor[SMALL_LINE + 1], trigger[MED_LINE + 1],
         positional[MED_LINE + 1], *rest, *tmp;

    sprintf(lookFor, "|%s|\0", MHname);
    found = FALSE;
    for(i = 0; !found && (i < numBelow); i++)
    {
        if((strstr(belowLineTmp[i], "|CT|") != NULL) &&
           (strstr(belowLineTmp[i], lookFor) != NULL))
        {
            if(RTM_Debug)
            {
                fprintf(fout, "modifyBelowLine - Adding: #%s# to #%s#\n",
                        trig, belowLineTmp[i]);
                fflush(fout);
            } /* fi */

            found = TRUE;
            len = (long)strlen(belowLineTmp[i]);
            rest = (char *)malloc(len + 100);
            strcpy(rest, "");
            strcpy(trigger, "");
            strcpy(positional, "");
            sscanf(belowLineTmp[i], "%[^|]|%[^|]|%[^\n]", trigger,
                   positional, rest);

            /* 2/28/14 - added trimmed to 1024 to limit the trigger len */

            if(strlen(trig) > 0)
            {
                if((strstr(trigger, trig) == NULL) && (len < 1024))
                {
                    if(trigger[0] == ' ') /* Empty */
                      strcpy(trigger, trig);
                    else
                    {
                        strcat(trigger, ";");
                        strcat(trigger, trig);
                    } /* else */
                } /* fi */
            } /* fi */

            if(strlen(posInfo) > 0)
            {
                if(strstr(positional, posInfo) == NULL)
                {
                    if(positional[0] == ' ') /* Empty */
                      strcpy(positional, posInfo);
                    else
                    {
                        strcat(positional, ";");
                        strcat(positional, posInfo);
                    } /* else */
                } /* fi */
            } /* fi */

            tmp = (char *)malloc(strlen(trigger) + strlen(positional) + strlen(rest) + 100);
            strcpy(tmp, "");

            sprintf(tmp, "%s|%s|%s\0", trigger, positional, rest);
            free(belowLineTmp[i]);
            belowLineTmp[i] = strdup(tmp);

            free(tmp);
            free(rest);
        } /* fi */
    } /* for */
} /* modifyBelowLine */

/***************************************************************************/

char *show_MH_HM_ET(int datatype, int isET)
{
   char *rtn = (char *)malloc(5);

   strcpy(rtn, "MH");

   if(datatype == MH)
     strcpy(rtn, "MH");

   else if(datatype == NM)
     strcpy(rtn, "NM");

   else if(datatype == CT)
     strcpy(rtn, "CT");

   if(showHMs && (datatype == HM))
     strcpy(rtn, "HM");

   else if(datatype == HM)
     strcpy(rtn, "MH");

   /* No ET for CheckTags or SCRs (NM) */

   if(showETs && isET && ((strcmp(rtn, "MH") == 0) || (strcmp(rtn, "HM") == 0)))
     strcpy(rtn, "ET");

   return(rtn);
} /* show_MH_HM */

/***************************************************************************/

void savePrinted(long pos, int whichArray, int pPos)
{
   int ETflag, ok, found, hasBreak, haveHumans, index, posFound;
   long i, p, score, j, startPos, len, numPI;
   char uid[SMALL_LINE + 1], mh[SMALL_LINE + 1], dt[25], dui[50],
        locs[25], textLoc[MAXLINE + 1], trigger[MAXLINE + 1],
        mh_orig[SMALL_LINE + 1], posInfo[SMALL_LINE + 1], tmp[SMALL_LINE + 1],
        rest[SMALL_LINE + 1], entryTerm[MAXLINE + 1], treecode[SMALL_LINE + 1];

   /* whichArray: 0 - mt_table, 1 - ct_table, 3 - sh_table
        4 - belowLineTmp */

   if(RTM_Debug)
   {
       fprintf(fout, "savePrinted(%ld, %d, %d)\n", pos, whichArray, pPos);
       fflush(fout);
   } /* fi */

   if(whichArray == 0) /* mt_table */
   {
       /* Make sure we don't add duplicates, need to check all three lists */

       ok = TRUE;
       found = FALSE;
       for(j = 0; !found && (j < numFinalCTs); j++)
       {
           if(mt_table[pos].dui[4] == finalCTlist[j].dui[4])
           {
               if(strcmp(mt_table[pos].dui, finalCTlist[j].dui) == 0)
               {
                   found = TRUE;
                   ok = FALSE;
               } /* fi */
           } /* fi */
       } /* for */

       /* Add exception for "Removed CT" - make sure we don't add */

       if(mt_table[pos].mh[0] == 'R')
       {
           if(strcmp(mt_table[pos].mh, "Removed CT") == 0)
           {
               found = TRUE;
               ok = FALSE;
           } /* fi */
       } /* fi */

       /* Check to make sure table entry is oktoprint */

       if(ok)
       {
           if(!mt_table[pos].oktoprint || (mt_table[pos].score < 0))
           {
               if(RTM_Debug)
               {
                   fprintf(fout, "ERR - trying to add blocked mt_table");
                   fprintf(fout, " (%s): #%s# [%d] (%ld)\n", muid,
                           mt_table[pos].mh, mt_table[pos].oktoprint,
                           mt_table[pos].score);
                   fflush(fout);
               } /* fi */

               found = TRUE;
               ok = FALSE;
           } /* fi */
       } /* fi */

       if(ok)
       {
           found = FALSE;
           for(j = 0; !found && (j < numFinalSHs); j++)
           {
               if(mt_table[pos].dui[4] == finalSHlist[j].dui[4])
               {
                   if(strcmp(mt_table[pos].dui, finalSHlist[j].dui) == 0)
                   {
                       found = TRUE;
                       ok = FALSE;
                   } /* fi */
               } /* fi */
           } /* for */
       } /* fi OK */

       if(ok)
       {
           found = FALSE;
           for(j = 0; !found && (j < numFinalMHs); j++)
           {
               if(mt_table[pos].dui[4] == finalMHlist[j].dui[4])
               {
                   if(strcmp(mt_table[pos].dui, finalMHlist[j].dui) == 0)
                   {
                       found = TRUE;
                       ok = FALSE;
                   } /* fi */
               } /* fi */
           } /* for */
       } /* fi OK */

       if(isCTDUI(mt_table[pos].dui))
         mt_table[pos].datatype = CT;

       if(ok && (mt_table[pos].datatype == CT) && (numFinalCTs < MAX_FINAL_CTs))
       {
           p = numFinalCTs;
           finalCTlist[p].ETflag = mt_table[pos].ETflag;
           finalCTlist[p].datatype = mt_table[pos].datatype;
           finalCTlist[p].oktoprint = TRUE;
           finalCTlist[p].wasForced = mt_table[pos].forcePrint;
           finalCTlist[p].score = mt_table[pos].score;
           finalCTlist[p].num_RCs = mt_table[pos].num_RCs;
           finalCTlist[p].VocabDenFactor = mt_table[pos].VocabDenFactor;
           finalCTlist[p].origPos = pos;

           finalCTlist[p].muid = strdup(muid);
           finalCTlist[p].mh = strdup(mt_table[pos].mh);
           finalCTlist[p].mh_orig = strdup(mt_table[pos].mh_orig);
           finalCTlist[p].dui = strdup(mt_table[pos].dui);
           finalCTlist[p].entry_term = strdup(mt_table[pos].entry_term);
           finalCTlist[p].RC_PMIDs = strdup(mt_table[pos].RC_PMIDs);
           finalCTlist[p].trigger = strdup(mt_table[pos].trigger);
           finalCTlist[p].textloc = strdup(mt_table[pos].textloc);

           /* Positional Information */

           finalCTlist[p].numPIs = mt_table[pos].numPIs;
           for(i = 0; i < mt_table[pos].numPIs; i++)
           {
              finalCTlist[p].PIs[i].isStart = mt_table[pos].PIs[i].isStart;
              finalCTlist[p].PIs[i].startPos = mt_table[pos].PIs[i].startPos;
              finalCTlist[p].PIs[i].endPos = mt_table[pos].PIs[i].endPos;
              finalCTlist[p].PIs[i].len = mt_table[pos].PIs[i].len;
              finalCTlist[p].PIs[i].hasBreak = mt_table[pos].PIs[i].hasBreak;
           } /* for */

           /* Paths */

           for(i = 0; i < NUM_PATHS; i++)
             finalCTlist[p].paths[i] = mt_table[pos].paths[i];

           /* Treecodes - only keep first 25 (shouldn't be a problem */

           finalCTlist[p].num_treecodes = mt_table[pos].num_treecodes;
           if(finalCTlist[p].num_treecodes > 25)
             finalCTlist[p].num_treecodes = 25;
           for(i = 0; (i < 25) && (i < mt_table[pos].num_treecodes); i++)
              finalCTlist[p].treecodes[i] = strdup(mt_table[pos].treecodes[i]);

           /* Semantic Types - max 25 */

           finalCTlist[p].num_STs = mt_table[pos].num_STs;
           for(i = 0; i < mt_table[pos].num_STs; i++)
           {
               finalCTlist[p].ST_info[i].MMI_trigger =
                                   strdup(mt_table[pos].ST_info[i].MMI_trigger);
               finalCTlist[p].ST_info[i].rtm_ST =
                                   strdup(mt_table[pos].ST_info[i].rtm_ST);
           } /* for */

           numFinalCTs++;
       } /* fi mt_table CT */

       else if(ok && (mt_table[pos].datatype == SH) &&
                     (numFinalSHs < MAX_FINAL_SHs))
       {
           p = numFinalSHs;
           finalSHlist[p].ETflag = mt_table[pos].ETflag;
           finalSHlist[p].datatype = mt_table[pos].datatype;
           finalSHlist[p].oktoprint = TRUE;
           finalSHlist[p].score = mt_table[pos].score;
           finalSHlist[p].num_RCs = mt_table[pos].num_RCs;
           finalSHlist[p].VocabDenFactor = mt_table[pos].VocabDenFactor;
           finalSHlist[p].origPos = pos;

           finalSHlist[p].muid = strdup(muid);
           finalSHlist[p].mh = strdup(mt_table[pos].mh);
           finalSHlist[p].mh_orig = strdup(mt_table[pos].mh_orig);
           finalSHlist[p].dui = strdup(mt_table[pos].dui);
           finalSHlist[p].entry_term = strdup(mt_table[pos].entry_term);
           finalSHlist[p].RC_PMIDs = strdup(mt_table[pos].RC_PMIDs);
           finalSHlist[p].trigger = strdup(mt_table[pos].trigger);
           finalSHlist[p].textloc = strdup(mt_table[pos].textloc);

           /* Positional Information */

           finalSHlist[p].numPIs = mt_table[pos].numPIs;
           for(i = 0; i < mt_table[pos].numPIs; i++)
           {
              finalSHlist[p].PIs[i].isStart = mt_table[pos].PIs[i].isStart;
              finalSHlist[p].PIs[i].startPos = mt_table[pos].PIs[i].startPos;
              finalSHlist[p].PIs[i].endPos = mt_table[pos].PIs[i].endPos;
              finalSHlist[p].PIs[i].len = mt_table[pos].PIs[i].len;
              finalSHlist[p].PIs[i].hasBreak = mt_table[pos].PIs[i].hasBreak;
           } /* for */

           /* Paths */

           for(i = 0; i < NUM_PATHS; i++)
             finalSHlist[p].paths[i] = mt_table[pos].paths[i];

           /* Treecodes - only keep first 25 (shouldn't be a problem */

           finalSHlist[p].num_treecodes = mt_table[pos].num_treecodes;
           if(finalSHlist[p].num_treecodes > 25)
             finalSHlist[p].num_treecodes = 25;
           for(i = 0; (i < 25) && (i < mt_table[pos].num_treecodes); i++)
              finalSHlist[p].treecodes[i] = strdup(mt_table[pos].treecodes[i]);

           /* Semantic Types - max 25 */

           finalSHlist[p].num_STs = mt_table[pos].num_STs;
           for(i = 0; i < mt_table[pos].num_STs; i++)
           {
               finalSHlist[p].ST_info[i].MMI_trigger =
                                   strdup(mt_table[pos].ST_info[i].MMI_trigger);
               finalSHlist[p].ST_info[i].rtm_ST =
                                   strdup(mt_table[pos].ST_info[i].rtm_ST);
           } /* for */

           numFinalSHs++;
       } /* else fi mt_table SH */

       else if(ok && (mt_table[pos].datatype != SH) && 
               (mt_table[pos].datatype != CT) && (numFinalMHs < MAX_FINAL_MHs))
       {
           p = numFinalMHs;
           finalMHlist[p].ETflag = mt_table[pos].ETflag;
           finalMHlist[p].datatype = mt_table[pos].datatype;
           finalMHlist[p].oktoprint = TRUE;
           finalMHlist[p].wasForced = mt_table[pos].forcePrint;
           finalMHlist[p].score = mt_table[pos].score;
           finalMHlist[p].num_RCs = mt_table[pos].num_RCs;
           finalMHlist[p].VocabDenFactor = mt_table[pos].VocabDenFactor;
           finalMHlist[p].origPos = pos;

           finalMHlist[p].muid = strdup(muid);
           finalMHlist[p].mh = strdup(mt_table[pos].mh);
           finalMHlist[p].mh_orig = strdup(mt_table[pos].mh_orig);
           finalMHlist[p].dui = strdup(mt_table[pos].dui);
           finalMHlist[p].entry_term = strdup(mt_table[pos].entry_term);
           finalMHlist[p].RC_PMIDs = strdup(mt_table[pos].RC_PMIDs);
           finalMHlist[p].trigger = strdup(mt_table[pos].trigger);
           finalMHlist[p].textloc = strdup(mt_table[pos].textloc);

           /* Positional Information */

           finalMHlist[p].numPIs = mt_table[pos].numPIs;
           for(i = 0; i < mt_table[pos].numPIs; i++)
           {
              finalMHlist[p].PIs[i].isStart = mt_table[pos].PIs[i].isStart;
              finalMHlist[p].PIs[i].startPos = mt_table[pos].PIs[i].startPos;
              finalMHlist[p].PIs[i].endPos = mt_table[pos].PIs[i].endPos;
              finalMHlist[p].PIs[i].len = mt_table[pos].PIs[i].len;
              finalMHlist[p].PIs[i].hasBreak = mt_table[pos].PIs[i].hasBreak;
           } /* for */

           /* Paths */

           for(i = 0; i < NUM_PATHS; i++)
             finalMHlist[p].paths[i] = mt_table[pos].paths[i];

           /* HMs for each SCR - only keep first 25 (shouldn't be a problem) */

           finalMHlist[p].num_HMs = mt_table[pos].num_HMs;
           if(finalMHlist[p].num_HMs > 25)
             finalMHlist[p].num_HMs = 25;
           for(i = 0; (i < 25) && (i < mt_table[pos].num_HMs); i++)
           {
               index = find_uniq(mt_table[pos].HMs[i]);
               finalMHlist[p].HMs[i] = strdup(mt_table[index].mh);
           } /* for */

           /* Treecodes - only keep first 25 (shouldn't be a problem) */

           finalMHlist[p].num_treecodes = mt_table[pos].num_treecodes;
           if(finalMHlist[p].num_treecodes > 25)
             finalMHlist[p].num_treecodes = 25;
           for(i = 0; (i < 25) && (i < mt_table[pos].num_treecodes); i++)
              finalMHlist[p].treecodes[i] = strdup(mt_table[pos].treecodes[i]);

           /* If we don't have any treecodes, add them here */

           if(finalMHlist[p].num_treecodes == 0)
             pullTREEs(p);

           /* Semantic Types - max 25 */

           finalMHlist[p].num_STs = mt_table[pos].num_STs;
           for(i = 0; i < mt_table[pos].num_STs; i++)
           {
               finalMHlist[p].ST_info[i].MMI_trigger =
                                   strdup(mt_table[pos].ST_info[i].MMI_trigger);
               finalMHlist[p].ST_info[i].rtm_ST =
                                   strdup(mt_table[pos].ST_info[i].rtm_ST);
           } /* for */

           numFinalMHs++;
       } /* else fi mt_table MH/NM/HM */

       else if(ok) /* Error */
       {
           fprintf(stderr, "ERROR: Too Many of something -\n");
           fprintf(stderr, "  -- CT: %ld of %ld\n", numFinalCTs, MAX_FINAL_CTs);
           fprintf(stderr, "  -- SH: %ld of %ld\n", numFinalSHs, MAX_FINAL_SHs);
           fprintf(stderr, "  -- MH: %ld of %ld\n", numFinalMHs, MAX_FINAL_MHs);
           fflush(stderr);
       } /* else */

       if(RTM_Debug)
       {
           if(strlen(mt_table[pos].mh_orig) > 0)
             fprintf(fout, 
                "mt_table - Pos: %ld  MH: #%s#  ",
                 pos, mt_table[pos].mh_orig);
           else
             fprintf(fout, 
                 "mt_table - Pos: %ld  MH: #%s#  ",
                 pos, mt_table[pos].mh);

           fprintf(fout, "Which: %d  pPos: %d  ok: %d  type: %d\n",
                 whichArray, pPos, ok, mt_table[pos].datatype);
       } /* fi RTM_Debug */
   } /* fi mt_table */

   else if(whichArray == 1) /* CheckTag Table */
   {
       /* Make sure we don't add duplicates */

       ok = TRUE;
       found = FALSE;
       for(j = 0; !found && (j < numFinalCTs); j++)
       {
           if(ct_table[pos].mh[0] == finalCTlist[j].mh[0])
           {
               if(strcmp(ct_table[pos].mh, finalCTlist[j].mh) == 0)
               {
                   found = TRUE;
                   ok = FALSE;
               } /* fi */
           } /* fi */
       } /* for */

       /* Add exception for "Removed CT" - make sure we don't add */

       if(ct_table[pos].mh[0] == 'R')
       {
           if(strcmp(ct_table[pos].mh, "Removed CT") == 0)
           {
               found = TRUE;
               ok = FALSE;
           } /* fi */
       } /* fi */

       /* Check to make sure table entry is oktoprint */

       if(ok)
       {
           if(!ct_table[pos].oktoprint || (ct_table[pos].score < 0))
           {
               if(RTM_Debug)
               {
                   fprintf(fout, "ERR - trying to add blocked ct_table");
                   fprintf(fout, " (%s): #%s# [%d] (%ld)\n", muid,
                           ct_table[pos].mh, ct_table[pos].oktoprint,
                           ct_table[pos].score);
                   fflush(fout);
               } /* fi */
               found = TRUE;
               ok = FALSE;
           } /* fi */
       } /* fi */

       if(ok && (numFinalCTs < MAX_FINAL_CTs))
       {
           p = numFinalCTs;
           finalCTlist[p].ETflag = FALSE;
           finalCTlist[p].datatype = CT;
           finalCTlist[p].oktoprint = TRUE;
           finalCTlist[p].score = ct_table[pos].score;
           finalCTlist[p].num_RCs = 0;
           finalCTlist[p].VocabDenFactor = 0;
           finalCTlist[p].origPos = pos + 1000;

           finalCTlist[p].muid = strdup(muid);
           finalCTlist[p].mh = strdup(ct_table[pos].mh);
           finalCTlist[p].mh_orig = strdup("");
           finalCTlist[p].dui = strdup(ct_table[pos].dui);
           finalCTlist[p].entry_term = strdup("");
           finalCTlist[p].RC_PMIDs = strdup("");
           finalCTlist[p].trigger = strdup(ct_table[pos].trigger);
           finalCTlist[p].textloc = strdup(ct_table[pos].textloc);

           /* Positional Information */

           finalCTlist[p].numPIs = 0;

           /* Paths */

           for(i = 0; i < NUM_PATHS; i++)
             finalCTlist[p].paths[i] = ct_table[pos].paths[i];

           /* Treecodes - only keep first 25 (shouldn't be a problem */

           finalCTlist[p].num_treecodes = 0;
           pullTREEs_CT(p);

           /* Semantic Types - max 25 */

           finalCTlist[p].num_STs = 0;

           numFinalCTs++;
       } /* fi */

       else if(numFinalCTs >= MAX_FINAL_CTs) /* Error */
       {
           fprintf(stderr, "ERROR: Too Many CTs -\n");
           fprintf(stderr, "  -- CT: %ld of %ld\n", numFinalCTs, MAX_FINAL_CTs);
           fflush(stderr);
       } /* else */

       if(RTM_Debug)
       {
           fprintf(fout, "ct_table - Pos: %ld  MH: #%s#  Which: %d  pPos: %d\n",
                   pos, ct_table[pos].mh, whichArray, pPos);
           fflush(fout);
       } /* fi */
   } /* else fi ct_table */

   else if(whichArray == 3) /* Subheading Table */
   {
       /* Make sure we don't add duplicates */

       ok = TRUE;
       found = FALSE;
       for(j = 0; !found && (j < numFinalSHs); j++)
       {
           if(sh_table[pos].mh[0] == finalSHlist[j].mh[0])
           {
               if(strcmp(sh_table[pos].mh, finalSHlist[j].mh) == 0)
               {
                   found = TRUE;
                   ok = FALSE;
               } /* fi */
           } /* fi */
       } /* for */

       if(ok && (numFinalSHs < MAX_FINAL_SHs))
       {
           p = numFinalSHs;
           finalSHlist[p].ETflag = FALSE;
           finalSHlist[p].datatype = SH;
           finalSHlist[p].oktoprint = TRUE;
           finalSHlist[p].score = sh_table[pos].score;
           finalSHlist[p].num_RCs = 0;
           finalSHlist[p].VocabDenFactor = 0;
           finalSHlist[p].origPos = pos + 3000;

           finalSHlist[p].muid = strdup(muid);
           finalSHlist[p].mh = strdup(sh_table[pos].mh);
           finalSHlist[p].mh_orig = strdup("");
           finalSHlist[p].dui = strdup(sh_table[pos].dui);
           finalSHlist[p].entry_term = strdup("");
           finalSHlist[p].RC_PMIDs = strdup("");
           finalSHlist[p].trigger = strdup(sh_table[pos].trigger);
           finalSHlist[p].textloc = strdup(sh_table[pos].textloc);

           /* Positional Information */

           finalSHlist[p].numPIs = 0;

           /* Paths */

           for(i = 0; i < NUM_PATHS; i++)
             finalSHlist[p].paths[i] = sh_table[pos].paths[i];

           /* Treecodes - only keep first 25 (shouldn't be a problem */

           finalSHlist[p].num_treecodes = 0;

           /* Semantic Types - max 25 */

           finalSHlist[p].num_STs = 0;

           numFinalSHs++;
       } /* fi */

       else if(numFinalSHs >= MAX_FINAL_SHs) /* Error */
       {
           fprintf(stderr, "ERROR: Too Many SHs -\n");
           fprintf(stderr, "  -- SH: %ld of %ld\n", numFinalSHs, MAX_FINAL_SHs);
           fflush(stderr);
       } /* else */

       if(RTM_Debug)
       {
           fprintf(fout, "sh_table - Pos: %ld  MH: #%s#  Which: %d  pPos: %d\n",
                   pos, sh_table[pos].mh, whichArray, pPos);
           fflush(fout);
       } /* fi */
   } /* else fi sh_table */

   else if(whichArray == 4) /* belowLineTmp */
   {
       ETflag = score = -1;
       strcpy(uid, "");
       strcpy(mh, "");
       strcpy(dt, "");
       strcpy(dui, "");
       strcpy(locs, "");
       strcpy(textLoc, "");
       strcpy(trigger, "");
       strcpy(mh_orig, "");
       strcpy(posInfo, "");
       strcpy(entryTerm, "");

       sscanf(belowLineTmp[pos],
     "%[^|]|%[^|]|%[^|]|%[^|]|%d|%[^|]|%[^|]|%ld|%[^|]|%[^|]|%[^|]|%[^|]",
          trigger, posInfo, uid, mh, &ETflag, dt, dui, &score, locs,
          textLoc, mh_orig, entryTerm);

       /* Fudge since some scores are showing up really negative */

       if(score < -1)
         score = 1000;

       /* mh_orig may be empty if not an ET */

       if((strlen(uid) > 0) && (strlen(mh) > 0) && (ETflag > -1) &&
          (strlen(dt) > 0) && (score > -1) && (strlen(locs) > 0) &&
          (strlen(textLoc) > 0) && (strlen(trigger) > 0) && 
          (strlen(posInfo) > 0))
       {
            if(strcmp(textLoc, " ") == 0)
              strcpy(textLoc, "");

            if(strcmp(trigger, " ") == 0)
              strcpy(trigger, "");

            if(strcmp(posInfo, " ") == 0)
              strcpy(posInfo, "");

            if(strcmp(mh_orig, " ") == 0)
              strcpy(mh_orig, "");

            if(RTM_Debug)
            {
                fprintf(fout,
                  "Parsed:%s|%s|%d|%s|%s|%ld|%s|%s|%s|%s|%s|%s|\n",
                  uid, mh, ETflag, dt, dui, score, locs, textLoc, trigger,
                  mh_orig, posInfo, entryTerm);
                fflush(fout);
            } /* fi */

            if(strcmp(dt, "CT") == 0) /* Check Tag */
            {
                /* Make sure we don't add duplicates */

                haveHumans = FALSE;
                ok = TRUE;
                found = FALSE;
                for(j = 0; j < numFinalCTs; j++)
                {
                    if(mh[0] == finalCTlist[j].mh[0])
                    {
                        if(strcmp(mh, finalCTlist[j].mh) == 0)
                        {
                            found = TRUE;
                            ok = FALSE;

                            /* See if we should expand positional info we
                               have for this CT.  Make sure we don't add
                               duplicate positional info.
                            */

                            numPI = finalCTlist[j].numPIs;
                            strcpy(tmp, posInfo);
                            while(strlen(tmp) > 0)
                            {
                                strcpy(rest, "");
                                hasBreak = startPos = len = -1;
                                sscanf(tmp, "%ld^%ld^%d;%[^\n]",
                                       &startPos, &len, &hasBreak, rest);

                                strcpy(tmp, rest);

                                if((startPos > -1) && (len > -1))
                                {
                                    posFound = FALSE;
                                    for(p = 0; !posFound && (p < numPI); p++)
                                    {
                                        if(finalCTlist[j].PIs[p].startPos ==
                                              startPos)
                                          posFound = TRUE;
                                    } /* for */

                                    if(!posFound)
                                    {
                                        finalCTlist[j].PIs[numPI].isStart =
                                                                TRUE;
                                        finalCTlist[j].PIs[numPI].startPos =
                                                                 startPos;
                                        finalCTlist[j].PIs[numPI].endPos = 
                                                                 startPos + len;
                                        finalCTlist[j].PIs[numPI].len = len;
                                        finalCTlist[j].PIs[numPI].hasBreak =
                                                                 hasBreak;
                                        numPI++;
                                    } /* fi */
                                } /* fi */
                            } /* while */

                            finalCTlist[j].numPIs = numPI;
                        } /* fi */
                    } /* fi */

                    if(strcmp(finalCTlist[j].mh, "Humans") == 0)
                      haveHumans = TRUE;
                } /* for */

                /* Now, double check to see if we are adding in any Age
                   related CTs without Humans being specified and remove
                   them if we are!
                */

                if(ok && !haveHumans && !haveHumansBelowLine)
                {
                    if((strcmp(mh, "Adolescent") == 0) ||
                       (strcmp(mh, "Adult") == 0) ||
                       (strcmp(mh, "Aged") == 0) ||
                       (strcmp(mh, "Child") == 0) ||
                       (strcmp(mh, "Infant") == 0) ||
                       (strcmp(mh, "Infant, Newborn") == 0) ||
                       (strcmp(mh, "Aged, 80 and over") == 0) ||
                       (strcmp(mh, "Child, Preschool") == 0) ||
                       (strcmp(mh, "Middle Aged") == 0) ||
                       (strcmp(mh, "Young Adult") == 0))
                    {
                        if(RTM_Debug)
                        {
                           fprintf(fout,
                              "Removing belowLineTmp Aged CT|%s| - no Humans\n",
                              mh);
                           fflush(fout);
                        } /* fi */

                        ok = FALSE;
                    } /* fi */
                } /* fi */

                if(ok && (numFinalCTs < MAX_FINAL_CTs))
                {
                    p = numFinalCTs;
                    finalCTlist[p].ETflag = ETflag;
                    finalCTlist[p].datatype = CT;
                    finalCTlist[p].oktoprint = TRUE;
                    finalCTlist[p].score = score;
                    finalCTlist[p].num_RCs = 0;
                    finalCTlist[p].VocabDenFactor = 0;
                    finalCTlist[p].origPos = pos + 4000;

                    finalCTlist[p].muid = strdup(uid);
                    finalCTlist[p].mh = strdup(mh);
                    finalCTlist[p].mh_orig = strdup(mh_orig);
                    finalCTlist[p].dui = strdup(dui);
                    finalCTlist[p].entry_term = strdup(entryTerm);
                    finalCTlist[p].RC_PMIDs = strdup("");
                    finalCTlist[p].trigger = strdup(trigger);
                    finalCTlist[p].textloc = strdup(textLoc);

                    /* Positional Information */

                    finalCTlist[p].numPIs = 0;
                    if(strlen(posInfo) > 0)
                    {
                        numPI = 0;
                        strcpy(tmp, posInfo);
                        while(strlen(tmp) > 0)
                        {
                            strcpy(rest, "");
                            hasBreak = startPos = len = -1;
                            sscanf(tmp, "%ld^%ld^%d;%[^\n]", &startPos, &len,
                                   &hasBreak, rest);

                            strcpy(tmp, rest);
                            if((startPos > -1) && (len > -1))
                            {
                                finalCTlist[p].PIs[numPI].isStart = TRUE;
                                finalCTlist[p].PIs[numPI].startPos = startPos;
                                finalCTlist[p].PIs[numPI].endPos = 
                                                                 startPos + len;
                                finalCTlist[p].PIs[numPI].len = len;
                                finalCTlist[p].PIs[numPI].hasBreak = hasBreak;
                                numPI++;
                            } /* fi */
                        } /* while */

                        finalCTlist[p].numPIs = numPI;
                    } /* fi */

                    /* Paths */

                    finalCTlist[p].paths[MMI] = TRUE;

                    /* Treecodes - only keep first 25 (shouldn't be a problem */

                    finalCTlist[p].num_treecodes = 0;
                    pullTREEs_CT(p);

                    /* Semantic Types - max 25 */

                    finalCTlist[p].num_STs = 0;

                    numFinalCTs++;
                } /* fi */

                else if(numFinalCTs >= MAX_FINAL_CTs) /* Error */
                {
                    fprintf(stderr, "ERROR: Too Many CTs -\n");
                    fprintf(stderr, "  -- CT: %ld of %ld\n", numFinalCTs,
                            MAX_FINAL_CTs);
                    fflush(stderr);
                } /* else */
            } /* fi CT */

            else /* MH/NM/HM */
            {
                /* Make sure we don't add duplicates */

                ok = TRUE;
                found = FALSE;
                for(j = 0; !found && (j < numFinalMHs); j++)
                {
                    if(mh[0] == finalMHlist[j].mh[0])
                    {
                       if(strcmp(mh, finalMHlist[j].mh) == 0)
                       {
                           found = TRUE;
                           ok = FALSE;
                       } /* fi */
                    } /* fi */

                    if(!found && (mh[0] == finalMHlist[j].mh_orig[0]))
                    {
                        if(strcmp(mh, finalMHlist[j].mh_orig) == 0)
                        {
                            found = TRUE;
                            ok = FALSE;
                        } /* fi */
                    } /* fi */

                    if(!found && ((strlen(mh_orig) > 0) &&
                            (mh_orig[0] == finalMHlist[j].mh_orig[0])))
                    {
                        if(strcmp(mh_orig, finalMHlist[j].mh_orig) == 0)
                        {
                            found = TRUE;
                            ok = FALSE;
                        } /* fi */
                    } /* fi */

                    if(found)
                    {

                        /* See if we should expand positional info we
                           have for this MH.  Make sure we don't add
                           duplicate positional info.
                        */

                        numPI = finalMHlist[j].numPIs;
                        strcpy(tmp, posInfo);
                        while(strlen(tmp) > 0)
                        {
                            strcpy(rest, "");
                            hasBreak = startPos = len = -1;
                            sscanf(tmp, "%ld^%ld^%d;%[^\n]",
                                   &startPos, &len, &hasBreak, rest);

                            strcpy(tmp, rest);

                            if((startPos > -1) && (len > -1))
                            {
                                posFound = FALSE;
                                for(p = 0; !posFound && (p < numPI); p++)
                                {
                                    if(finalMHlist[j].PIs[p].startPos ==
                                          startPos)
                                      posFound = TRUE;
                                } /* for */

                                if(!posFound)
                                {
                                    finalMHlist[j].PIs[numPI].isStart =
                                                                TRUE;
                                    finalMHlist[j].PIs[numPI].startPos =
                                                                 startPos;
                                    finalMHlist[j].PIs[numPI].endPos = 
                                                                 startPos + len;
                                    finalMHlist[j].PIs[numPI].len = len;
                                    finalMHlist[j].PIs[numPI].hasBreak =
                                                                 hasBreak;
                                    numPI++;
                                } /* fi */
                            } /* fi */
                        } /* while */

                        finalMHlist[j].numPIs = numPI;
                    } /* fi */
                } /* for */

                if(ok && (numFinalMHs < MAX_FINAL_MHs))
                {
                    p = numFinalMHs;
                    finalMHlist[p].ETflag = ETflag;

                    if((strcmp(dt, "MH") == 0) || (strcmp(dt, "MH") == 0))
                      finalMHlist[p].datatype = MH;

                    else if(strcmp(dt, "NM") == 0)
                      finalMHlist[p].datatype = NM;

                    else if(strcmp(dt, "HM") == 0)
                      finalMHlist[p].datatype = HM;

                    else
                      finalMHlist[p].datatype = -1;

                    finalMHlist[p].oktoprint = TRUE;
                    finalMHlist[p].score = score;
                    finalMHlist[p].num_RCs = 0;
                    finalMHlist[p].VocabDenFactor = 0;
                    finalMHlist[p].origPos = pos + 4000;

                    finalMHlist[p].muid = strdup(uid);
                    finalMHlist[p].mh = strdup(mh);
                    finalMHlist[p].mh_orig = strdup(mh_orig);
                    finalMHlist[p].dui = strdup(dui);
                    finalMHlist[p].entry_term = strdup(entryTerm);
                    finalMHlist[p].RC_PMIDs = strdup("");
                    finalMHlist[p].trigger = strdup(trigger);
                    finalMHlist[p].textloc = strdup(textLoc);

                    /* Positional Information */

                    finalMHlist[p].numPIs = 0;
                    if(strlen(posInfo) > 0)
                    {
                        numPI = 0;
                        strcpy(tmp, posInfo);
                        while(strlen(tmp) > 0)
                        {
                            strcpy(rest, "");
                            hasBreak = startPos = len = -1;
                            sscanf(tmp, "%ld^%ld^%d;%[^\n]", &startPos, &len,
                                   &hasBreak, rest);
                            strcpy(tmp, rest);

                            if((startPos > -1) && (len > -1))
                            {
                                finalMHlist[p].PIs[numPI].isStart = TRUE;
                                finalMHlist[p].PIs[numPI].startPos = startPos;
                                finalMHlist[p].PIs[numPI].endPos = 
                                                                 startPos + len;
                                finalMHlist[p].PIs[numPI].len = len;
                                finalMHlist[p].PIs[numPI].hasBreak = hasBreak;
                                numPI++;
                            } /* fi */
                        } /* while */

                        finalMHlist[p].numPIs = numPI;
                    } /* fi */

                    /* Paths */

                    finalMHlist[p].paths[MMI] = TRUE;

                    /* Treecodes */

                    finalMHlist[p].num_treecodes = 0;
                    pullTREEs(p);

                    /* Semantic Types - max 25 */

                    finalMHlist[p].num_STs = 0;

                    numFinalMHs++;
                } /* fi */

                else if(numFinalMHs >= MAX_FINAL_MHs) /* Error */
                {
                    fprintf(stderr, "ERROR: Too Many MHs -\n");
                    fprintf(stderr, "  -- MHs: %ld of %ld\n", numFinalMHs,
                            MAX_FINAL_MHs);
                    fflush(stderr);
                } /* else */
            } /* else */
       } /* fi */

       else
       {
           fprintf(stderr, "ERROR: Bad belowLineTmp Parse (%s): #%s#\n",
                   muid, belowLineTmp[pos]);
           fflush(stderr);
       } /* else */

       if(RTM_Debug)
       {
           fprintf(fout, 
               "\nbelowLineTmp - Pos: %ld  line: #%s#  Which: %d  pPos: %d\n",
               pos, belowLineTmp[pos], whichArray, pPos);
           fflush(fout);
       } /* fi */
   } /* else fi belowLineTmp */

   else
    printf("Unknown Type: %ld - %d - %d\n", pos, whichArray, pPos);
} /* savePrinted */

/***************************************************************************
*
*  printFinal --
*
*      This 
*
***************************************************************************/

void printFinal(int which, int starMHTI_set, int justFacts, int show_id,
                int show_labels, int OKonly)
{
    long i, j, k, pos, pCtr;
    char tmp[MAXLINE + 1], *foo, *foo2, DUI[50], TUI[50],
         treeCodes[MAXLINE + 1], tStar[3];

    if(which == LIST_DISPLAY)
      fprintf(fout, "%s|", muid);

    /* Check Tags (CTs) -- Never Star */

    if((which == SIMPLE_DISPLAY) && show_labels)
      fprintf(fout, "\nSuggested CheckTags:\n");

    fflush(fout);
    for(i = 0; i < numFinalCTs; i++)
    {
        if(finalCTlist[i].oktoprint)
        {
            numOverallPrinted++;
            strcpy(DUI, "");
            strcpy(TUI, "");
            strcpy(treeCodes, "");
            if(showDUIs || (which == BioASQ_DISPLAY))
            {
                if(which == BioASQ_DISPLAY)
                  strcpy(DUI, finalCTlist[i].dui);
                else
                  sprintf(DUI, "|%s\0", finalCTlist[i].dui);
            } /* fi */

            if(showTUIs || (which == LIST_DISPLAY))
            {
                foo = pullTUI(finalCTlist[i].mh);
                if(which == LIST_DISPLAY)
                  strcpy(TUI, foo);
                else
                  sprintf(TUI, "|%s\0", foo);
                free(foo);
            } /* fi */

            if(showTreecodes)
            {
                if(finalCTlist[i].num_treecodes > 0)
                  strcat(treeCodes, "|");

                for(k = 0; k < finalCTlist[i].num_treecodes; k++)
                {
                    if(k > 0)
                      strcat(treeCodes, ";");
                    strcat(treeCodes, finalCTlist[i].treecodes[k]);
                } /* for */
            } /* fi */

            if(which == DEFAULT_DISPLAY)
            {
                /* Identify Paths */

                strcpy(tmp, "");
                if(finalCTlist[i].paths[MMI] && finalCTlist[i].paths[PRC])
                  strcpy(tmp, "MM;RC");

                else if(finalCTlist[i].paths[MMI])
                  strcpy(tmp, "MM");

                else if(finalCTlist[i].paths[PRC])
                  strcpy(tmp, "RC");

                else
                  strcpy(tmp, "NA");

                if(showScores || showScoresFull)
                  fprintf(fout, "%s|%s|%ld|CT", finalCTlist[i].muid,
                          finalCTlist[i].mh, finalCTlist[i].score);

                else if(justFacts)
                  fprintf(fout, "%s|%s%s%s%s|%ld",
                          finalCTlist[i].muid, finalCTlist[i].mh, DUI, TUI,
                          treeCodes, finalCTlist[i].score);

                else
                  fprintf(fout, "%s|%s|%s|%ld|CT|%s|%s|%s%s%s",
                          finalCTlist[i].muid, finalCTlist[i].mh,
                          finalCTlist[i].dui, finalCTlist[i].score,
                          finalCTlist[i].trigger, finalCTlist[i].textloc, tmp, 
                          TUI, treeCodes);

                if(trackPositional && (finalCTlist[i].numPIs > 0))
                {
                    fprintf(fout, "|");
                    for(k = 0; k < finalCTlist[i].numPIs; k++)
                    {
                        if(k > 0)
                          fprintf(fout, ";");
                        fprintf(fout, "%ld^%ld^%d",
                                finalCTlist[i].PIs[k].startPos,
                                finalCTlist[i].PIs[k].len,
                                finalCTlist[i].PIs[k].hasBreak);
                    } /* for */
                } /* fi */

                fprintf(fout,"\n");
            } /* fi Default Display */

            else if(which == LIST_DISPLAY)
            {
                if(showTUIs)
                  fprintf(fout, "%s^%s^:CT|", finalCTlist[i].mh, TUI);
                else
                  fprintf(fout, "%s:CT|", finalCTlist[i].mh);
            } /* else fi */

            else if(which == BioASQ_DISPLAY)
            {
                if(numBioASQPrinted == 0)
                  fprintf(fout, "{\"labels\":[");
            
                 if(numBioASQPrinted > 0)
                   fprintf(fout, ", ");

                 fprintf(fout, "\"%s\"", DUI);
                 numBioASQPrinted++;
            } /* else fi */

            else if(which == SIMPLE_DISPLAY)
            {
                if(MeSHonDemand2 || MoD_PP)
                  fprintf(fout, "%s|", finalCTlist[i].muid);

                if(MeSHonDemand)
                {
                    fprintf(fout, "%s|CT|", finalCTlist[i].mh);

                    if(!MoD_PP)
                    {
                        for(k = 0; k < finalCTlist[i].numPIs; k++)
                        {
                            if(k > 0)
                             fprintf(fout, ";");
                            fprintf(fout, "%ld^%ld",
                                finalCTlist[i].PIs[k].startPos,
                                finalCTlist[i].PIs[k].len);
                        } /* for */

                        if(finalCTlist[i].numPIs == 0)
                          fprintf(fout, "N/A");

                        fprintf(fout, "|");
                    } /* fi */

                    fprintf(fout, "\n");
                } /* fi */

                else
                {
                    if(show_id)
                      fprintf(fout, "%s|", finalCTlist[i].muid);

                    if(showType)
                      fprintf(fout, "CT: %s\n", finalCTlist[i].mh);

                    else
                      fprintf(fout, "   -- %s%s%s%s\n", finalCTlist[i].mh, DUI,
                              TUI, treeCodes);
                } /* else */
            } /* else fi */
        } /* fi oktoprint */

        if(which == FULL_DISPLAY)
        {
            if(finalCTlist[i].oktoprint || !OKonly)
            {
                foo2 = show_MH_HM_ET(finalCTlist[i].datatype,
                                     finalCTlist[i].ETflag);

                strcpy(tmp, "");
                if(finalCTlist[i].paths[MMI] && finalCTlist[i].paths[PRC])
                  strcpy(tmp, "MM;RC");

                else if(finalCTlist[i].paths[MMI])
                  strcpy(tmp, "MM");

                else if(finalCTlist[i].paths[PRC])
                  strcpy(tmp, "RC");

                else
                  strcpy(tmp, "NA");

                fprintf(fout, "%s|%ld:%ld|%s|%s|%ld|%s|%s|%s|%s\n",
                        finalCTlist[i].muid, pCtr, finalCTlist[i].origPos,
                        finalCTlist[i].mh, finalCTlist[i].dui,
                        finalCTlist[i].score, foo2, finalCTlist[i].trigger,
                        finalCTlist[i].textloc, tmp);
                pCtr++;
                free(foo2);
            } /* fi */
        } /* fi */
    } /* for each CT */
    fflush(fout);

    /* MH/NM/HM */
    /* NOTE: NO NMs or SHs for BioASQ */

    if((which == SIMPLE_DISPLAY) && show_labels)
      fprintf(fout, "Suggested MeSH Headings:\n");
    pCtr = 1;

    for(i = 0; i < numFinalMHs; i++)
    {
        strcpy(tStar, "");
        if(finalMHlist[i].oktoprint)
        {
            numOverallPrinted++;
            strcpy(DUI, "");
            strcpy(TUI, "");
            strcpy(treeCodes, "");

            /* Starring No GEOs or CTs, rest only if found in Title */
            /* For Level 1 Indexing, we want to star only the top MH */

            if(level1Filter)
            {
                if((i == 0) && (finalMHlist[i].num_treecodes > 0) &&
                   (finalMHlist[i].treecodes[0][0] != 'Z'))
                  strcpy(tStar, "*");
            } /* fi */

            else
            {
                if(starMHTI_set && 
                   (strstr(finalMHlist[i].textloc, "TI") != NULL))
                {
                    if((finalMHlist[i].num_treecodes > 0) &&
                       (finalMHlist[i].treecodes[0][0] != 'Z'))
                      strcpy(tStar, "*");
                } /* fi */
            } /* else */

            if(showDUIs || (which == BioASQ_DISPLAY))
            {
                if(which == BioASQ_DISPLAY)
                  strcpy(DUI, finalMHlist[i].dui);
                else
                  sprintf(DUI, "|%s\0", finalMHlist[i].dui);
            } /* fi */

            if(showTUIs || (which == LIST_DISPLAY))
            {
                foo = pullTUI(finalMHlist[i].mh);
                if(which == LIST_DISPLAY)
                  strcpy(TUI, foo);
                else
                  sprintf(TUI, "|%s\0", foo);
                free(foo);
            } /* fi */

            if(showTreecodes)
            {
                if(finalMHlist[i].num_treecodes > 0)
                  strcat(treeCodes, "|");

                for(k = 0; k < finalMHlist[i].num_treecodes; k++)
                {
                    if(k > 0)
                      strcat(treeCodes, ";");
                    strcat(treeCodes, finalMHlist[i].treecodes[k]);
                } /* for */
            } /* fi */

            foo2 = show_MH_HM_ET(finalMHlist[i].datatype,
                                 finalMHlist[i].ETflag);
            if(which == DEFAULT_DISPLAY)
            {
                /* Identify Paths */

                strcpy(tmp, "");
                if(finalMHlist[i].paths[MMI] && finalMHlist[i].paths[PRC])
                  strcpy(tmp, "MM;RC");

                else if(finalMHlist[i].paths[MMI])
                  strcpy(tmp, "MM");

                else if(finalMHlist[i].paths[PRC])
                  strcpy(tmp, "RC");

                else
                  strcpy(tmp, "NA");

                if(showScores || showScoresFull)
                  fprintf(fout, "%s|%s|%ld|%s", finalMHlist[i].muid,
                          finalMHlist[i].mh, finalMHlist[i].score, foo2);

                else if(justFacts)
                  fprintf(fout, "%s|%s%s%s%s|%ld",
                          finalMHlist[i].muid, finalMHlist[i].mh, DUI, TUI,
                          treeCodes, finalMHlist[i].score);

                else
                  fprintf(fout, "%s|%s%s|%s|%ld|%s|%s|%s|%s%s%s",
                          finalMHlist[i].muid, tStar, finalMHlist[i].mh,
                          finalMHlist[i].dui, finalMHlist[i].score, foo2,
                          finalMHlist[i].trigger, finalMHlist[i].textloc, tmp, 
                          TUI, treeCodes);

                if(trackPositional && (finalMHlist[i].numPIs > 0))
                {
                    fprintf(fout, "|");
                    for(k = 0; k < finalMHlist[i].numPIs; k++)
                    {
                        if(k > 0)
                          fprintf(fout, ";");
                        fprintf(fout, "%ld^%ld^%d",
                                finalMHlist[i].PIs[k].startPos,
                                finalMHlist[i].PIs[k].len,
                                finalMHlist[i].PIs[k].hasBreak);
                    } /* for */
                } /* fi */

                fprintf(fout, "\n");
            } /* fi Default Display */

            else if(which == LIST_DISPLAY)
            {
                fprintf(fout, "%s", tStar);
                if(showTUIs)
                  fprintf(fout, "%s^%s^:%s|", finalMHlist[i].mh, TUI, foo2);
                else
                  fprintf(fout, "%s:%s|", finalMHlist[i].mh, foo2);
            } /* else fi */

            else if((which == BioASQ_DISPLAY) &&
                    (finalMHlist[i].datatype != NM))
            {
                if(numBioASQPrinted == 0)
                  fprintf(fout, "{\"labels\":[");
            
                 if(numBioASQPrinted > 0)
                   fprintf(fout, ", ");

                 fprintf(fout, "\"%s\"", DUI);
                 numBioASQPrinted++;
            } /* else fi */

            else if(which == SIMPLE_DISPLAY)
            {
                if(MeSHonDemand2 || MoD_PP)
                  fprintf(fout, "%s|", finalMHlist[i].muid);

                if(MeSHonDemand)
                {
                    fprintf(fout, "%s|%s|", finalMHlist[i].mh, foo2);

                    if(!MoD_PP)
                    {
                        for(k = 0; k < finalMHlist[i].numPIs; k++)
                        {
                            if(k > 0)
                              fprintf(fout, ";");
                            fprintf(fout, "%ld^%ld",
                                    finalMHlist[i].PIs[k].startPos,
                                    finalMHlist[i].PIs[k].len);
                        } /* for */

                        if(finalMHlist[i].numPIs == 0)
                          fprintf(fout, "N/A");

                        fprintf(fout, "|");
                    } /* fi */

                    fprintf(fout, "\n");
                } /* fi */

                else
                {
                    if(show_id)
                      fprintf(fout, "%s|", finalMHlist[i].muid);

                    if(showType)
                      fprintf(fout, "%s: %s\n", foo2, finalMHlist[i].mh);

                    else
                    {
                        if(showETs && finalMHlist[i].ETflag && 
                                 (strlen(finalMHlist[i].mh_orig) > 0))
                          fprintf(fout, "   -- %s (Entry Term for: %s)%s%s%s\n",
                                  finalMHlist[i].mh, finalMHlist[i].mh_orig,
                                  DUI, TUI, treeCodes);

                        else if(showHMs && (finalMHlist[i].datatype == HM))
                          fprintf(fout, "   -- %s (Heading Mapped to)%s%s%s\n",
                                  finalMHlist[i].mh, DUI, TUI, treeCodes);

                        else
                          fprintf(fout, "   -- %s%s%s%s\n", finalMHlist[i].mh,
                                  DUI, TUI, treeCodes);
                    } /* else */
                } /* else !MeSHonDemand */
            } /* else fi */

            free(foo2);
        } /* fi oktoprint */

        if(which == FULL_DISPLAY)
        {
            if(finalMHlist[i].oktoprint || !OKonly)
            {
                foo2 = show_MH_HM_ET(finalMHlist[i].datatype,
                                     finalMHlist[i].ETflag);

                strcpy(tmp, "");
                if(finalMHlist[i].paths[MMI] && finalMHlist[i].paths[PRC])
                  strcpy(tmp, "MM;RC");

                else if(finalMHlist[i].paths[MMI])
                  strcpy(tmp, "MM");

                else if(finalMHlist[i].paths[PRC])
                  strcpy(tmp, "RC");

                else
                  strcpy(tmp, "NA");

                fprintf(fout, "%s|%ld:%ld|%s|%s|%ld|%s|%s|%s|%s\n",
                        finalMHlist[i].muid, pCtr, finalMHlist[i].origPos,
                        finalMHlist[i].mh, finalMHlist[i].dui,
                        finalMHlist[i].score, foo2, finalMHlist[i].trigger,
                        finalMHlist[i].textloc, tmp);
                pCtr++;
                free(foo2);
            } /* fi */
        } /* fi */
    } /* for each MH */
    fflush(fout);

    /* Publication Types (PTs) -- Never Star */

    for(i = 0; i < numPTsFound; i++)
    {
        if(foundPTsList[i].oktoprint)
        {
            strcpy(DUI, "");
            strcpy(TUI, "");
            strcpy(treeCodes, "");

            if(showTUIs || (which == LIST_DISPLAY))
            {
                foo = pullTUI(foundPTsList[i].mh);
                if(which == LIST_DISPLAY)
                  strcpy(TUI, foo);
                else
                  sprintf(TUI, "|%s\0", foo);
                free(foo);
            } /* fi */

            if(showTreecodes)
            {
                if(foundPTsList[i].num_treecodes > 0)
                  strcat(treeCodes, "|");

                for(k = 0; k < foundPTsList[i].num_treecodes; k++)
                {
                    if(k > 0)
                      strcat(treeCodes, ";");
                    strcat(treeCodes, foundPTsList[i].treecodes[k]);
                } /* for */
            } /* fi */

            if(which == DEFAULT_DISPLAY)
            {
                /* Identify Paths */

                strcpy(tmp, "MM");

                if(showScores || showScoresFull)
                  fprintf(fout, "%s|%s|%ld|CT", foundPTsList[i].muid,
                          foundPTsList[i].mh, foundPTsList[i].score);

                else if(justFacts)
                  fprintf(fout, "%s|%s%s%s%s|%ld",
                          foundPTsList[i].muid, foundPTsList[i].mh, DUI, TUI,
                          treeCodes, foundPTsList[i].score);

                else
                  fprintf(fout, "%s|%s|%s|%ld|PT|%s|%s|%s%s%s",
                          foundPTsList[i].muid, foundPTsList[i].mh,
                          foundPTsList[i].dui, foundPTsList[i].score,
                          foundPTsList[i].trigger,
                          foundPTsList[i].textloc, tmp, TUI, treeCodes);

                if(trackPositional && (foundPTsList[i].numPIs > 0))
                {
                    fprintf(fout, "|");
                    for(k = 0; k < foundPTsList[i].numPIs; k++)
                    {
                        if(k > 0)
                          fprintf(fout, ";");
                        fprintf(fout, "%ld^%ld",
                                foundPTsList[i].PIs[k].startPos,
                                foundPTsList[i].PIs[k].len);
                    } /* for */
                } /* fi */

                fprintf(fout,"\n");
            } /* fi Default Display */

            else if(which == LIST_DISPLAY)
            {
                if(showTUIs)
                  fprintf(fout, "%s^%s^:PT|", foundPTsList[i].mh, TUI);
                else
                  fprintf(fout, "%s:PT|", foundPTsList[i].mh);
            } /* else fi */
        } /* fi oktoprint */
    } /* for each PT */
    fflush(fout);

    if((which == SIMPLE_DISPLAY) && (numFinalMHs == 0) && show_labels)
    {
        if(show_id)
          fprintf(fout, "%s|", muid);

        if(showType)
          fprintf(fout, "No MeSH Headings Recommended\n");

        else
          fprintf(fout, "   -- No MeSH Headings Recommended\n");
    } /* fi */
    fflush(fout);

    if((which == SIMPLE_DISPLAY) && (numFinalCTs == 0) && show_labels)
    {
        if(show_id)
          fprintf(fout, "%s|", muid);

        if(showType)
          fprintf(fout, "No CheckTags Recommended\n");

        else
          fprintf(fout, "   -- No CheckTags Recommended\n");
    } /* fi */
    fflush(fout);

    /* Last, SHs */

    if((which == SIMPLE_DISPLAY) && show_labels)
      fprintf(fout, "\nSuggested SubHeadings:\n");
    fflush(fout);

    for(i = 0; i < numFinalSHs; i++)
    {
        if(finalSHlist[i].oktoprint)
        {
            numOverallPrinted++;
            strcpy(TUI, "");
            if(showTUIs || (which == LIST_DISPLAY))
            {
                foo = pullTUI(finalSHlist[i].mh);
                if(which == LIST_DISPLAY)
                  strcpy(TUI, foo);
                else
                  sprintf(TUI, "|%s\0", foo);
                free(foo);
            } /* fi */

            if(which == DEFAULT_DISPLAY)
            {
                /* Identify Paths */

                strcpy(tmp, "");
                if(finalSHlist[i].paths[MMI] && finalSHlist[i].paths[PRC])
                  strcpy(tmp, "MM;RC");

                else if(finalSHlist[i].paths[MMI])
                  strcpy(tmp, "MM");

                else if(finalSHlist[i].paths[PRC])
                  strcpy(tmp, "RC");

                else
                  strcpy(tmp, "NA");

                if(!showScores && !showScoresFull)
                {
                    if(justFacts)
                      fprintf(fout, "%s|%s|%s|%ld\n", finalSHlist[i].muid,
                              finalSHlist[i].mh, finalSHlist[i].dui,
                              finalSHlist[i].score);

                    else
                      fprintf(fout, "%s|%s|%s|%ld|SH|%s|%s|%s%s\n",
                              finalSHlist[i].muid, finalSHlist[i].mh,
                              finalSHlist[i].dui, finalSHlist[i].score,
                              finalSHlist[i].trigger, finalSHlist[i].textloc,
                              tmp, TUI);
                } /* fi */
            } /* fi Default Display */

            else if(which == LIST_DISPLAY)
            {
                if(showTUIs)
                  fprintf(fout, "%s^%s^:SH|", finalSHlist[i].mh, TUI);
                else
                  fprintf(fout, "%s:SH|", finalSHlist[i].mh);
            } /* else fi */

            else if(which == SIMPLE_DISPLAY)
            {
                if(show_id)
                  fprintf(fout, "%s|", finalSHlist[i].muid);

                if(showType)
                  fprintf(fout, "SH: %s\n", finalSHlist[i].mh);

                else
                  fprintf(fout, "   -- %s\n", finalSHlist[i].mh);
            } /* else fi */
        } /* fi oktoprint */

        if(which == FULL_DISPLAY)
        {
            if(finalSHlist[i].oktoprint || !OKonly)
            {
                foo2 = show_MH_HM_ET(finalSHlist[i].datatype,
                                     finalSHlist[i].ETflag);

                strcpy(tmp, "");
                if(finalSHlist[i].paths[MMI] && finalSHlist[i].paths[PRC])
                  strcpy(tmp, "MM;RC");

                else if(finalSHlist[i].paths[MMI])
                  strcpy(tmp, "MM");

                else if(finalSHlist[i].paths[PRC])
                  strcpy(tmp, "RC");

                else
                  strcpy(tmp, "NA");

                fprintf(fout, "%s|%ld:%ld|%s|%s|%ld|%s|%s|%s|%s\n",
                        finalSHlist[i].muid, pCtr, finalSHlist[i].origPos,
                        finalSHlist[i].mh, finalSHlist[i].dui,
                        finalSHlist[i].score, foo2, finalSHlist[i].trigger,
                        finalSHlist[i].textloc, tmp);
                pCtr++;
                free(foo2);
            } /* fi */
        } /* fi */
    } /* for each SH */
    fflush(fout);

    if((which == FULL_DISPLAY) && (numFinalOVFs > 0))
    {
        for(i = 0; i < numFinalOVFs; i++)
        {
            if(finalOVFlist[i].oktoprint || !OKonly)
            {
                foo2 = show_MH_HM_ET(finalOVFlist[i].datatype,
                                     finalOVFlist[i].ETflag);

                strcpy(tmp, "");
                if(finalOVFlist[i].paths[MMI] && finalOVFlist[i].paths[PRC])
                  strcpy(tmp, "MM;RC");

                else if(finalOVFlist[i].paths[MMI])
                  strcpy(tmp, "MM");

                else if(finalOVFlist[i].paths[PRC])
                  strcpy(tmp, "RC");

                else
                  strcpy(tmp, "NA");

                fprintf(fout, "%s|%ld:%ld|%s|%s|%ld|%s|%s|%s|%s\n",
                        finalOVFlist[i].muid, pCtr, finalOVFlist[i].origPos,
                        finalOVFlist[i].mh, finalOVFlist[i].dui,
                        finalOVFlist[i].score, foo2, finalOVFlist[i].trigger,
                        finalOVFlist[i].textloc, tmp);
                pCtr++;
                free(foo2);
            } /* fi */
        } /* for */
    } /* fi */
    fflush(fout);

    if(which == LIST_DISPLAY)
      fprintf(fout, "\n");

    else if((which == SIMPLE_DISPLAY) && (numFinalSHs == 0) && show_labels)
    {
        if(show_id)
          fprintf(fout, "%s|", muid);

        if(showType)
          fprintf(fout, "No SubHeadings Recommended\n");

        else
          fprintf(fout, "   -- No SubHeadings Recommended\n");
    } /* else fi */

    else if((which == FULL_DISPLAY) && (pCtr == 0))
       fprintf(fout, "%s||||||||\n", muid);
    fflush(fout);

    if((which == FULL_DISPLAY) && ((numFinalCTs > 0) || (numFinalMHs > 0)))
      fprintf(fout, "%s|END OF RECORD MARKER\n", muid);
    fflush(fout);
} /* printFinal */

/***************************************************************************
*
*  printFinalJustInfo --
*
*      This 
*
***************************************************************************/

void printFinalJustInfo(char *PRC_from, int useWSD, int useWSDTIOnly)
{
    int isSpecialJournal;
    long i, j;
    char tmp[MAXLINE + 1], *foo, *foo2, *foo3, *foo4, *foo5, *newET;

    /*
JUST: 0|PMID|which PRC|Title Only Flag|firstLineJournal Flag|WSD usage|v121008A
JUST: 2|VocabFactor|Check Tag MH|CT|Triggering information (Note: mt_table CT)
JUST: 4|VocabFactor|MeSH Heading|Type|From|MMI Entry Terms|Triggers|# RC|PRC List|Score|Where|Entry Term Info|Positional Information
    Type: NM = Chemical
    Type: MH = MeSH Heading
    Type: ET = Entry Term
    From: MM (MetaMap), PRC (PubMed Related Citations)
    PRC List: Colon sep list (PMID^Rank^Score:PMID^Rank^Score)
        23185024^2^190.02449:19153231^4^149.2675
    Where: TI - Title, AB - Abstract
    Entry Term Info: semi-colon sep list (MH^textLoc^textWord^Positional Info)
        STAT1^ab-6^STAT1^964:5;STAT1^ab-7^STAT1^1080:5
JUST: 5|MeSH Subheading|SH
JUST: 6|PMID|CM| (Note: Comment/Correction citation)
JUST: 7|0.0000|Publication Type|PT|From|Entry Term|Triggers|Score|Where|Positional Information

       firstLineJournal Flag:
        0 - No
        1 - MTIFL
        2 - Chinese Journal
        3 - MTI-AUTO
        4 - Russian Journal
        5 - MTI-AUTO Comment On
        6 - MTI-Review
    */

    isSpecialJournal = 0;

    if(firstLineJournal && MTI_AUTO && hasCON)
      isSpecialJournal = 5;

    else if(firstLineJournal && MTI_AUTO && MTI_REVIEW)
      isSpecialJournal = 6;

    else if(firstLineJournal && MTI_AUTO)
      isSpecialJournal = 3;

    else if(firstLineJournal)
      isSpecialJournal = 1;

    if(strlen(PRC_from) < 2)
      strcpy(PRC_from, "No Related Articles Used");

    fprintf(fout, "JUST: 0|%s|%s|%d|%d|%s|v121008A|VocabDen\n",
            muid, PRC_from, Title_Only, isSpecialJournal,
            (useWSD || useWSDTIOnly) ? "WSD" : "no WSD");

    /* Check Tags (CTs) -- Never Star */

    for(i = 0; i < numFinalCTs; i++)
    {
        if(finalCTlist[i].oktoprint)
        {
            if(strlen(finalCTlist[i].entry_term) > 0)
              foo = strdup(finalCTlist[i].entry_term);
            else
              foo = strdup(" ");

            /* Clean up entry term info in move from MM13 to MM14 */

            newET = cleanET(foo);
            free(foo);
            foo = strdup(newET);
            free(newET);

            if(strlen(finalCTlist[i].trigger) > 0)
              foo2 = strdup(finalCTlist[i].trigger);
            else
              foo2 = strdup(" ");

            fprintf(fout, "JUST: 2|%0.4f|%s|CT|%s|%s|%s",
                    finalCTlist[i].VocabDenFactor, finalCTlist[i].mh,
                    foo, foo2, finalCTlist[i].textloc);

            free(foo);
            free(foo2);

            if(strlen(finalCTlist[i].textloc) == 0)
              fprintf(fout, " ");

            fprintf(fout, "|");

            if(finalCTlist[i].numPIs > 0)
            {
                for(j = 0; j < finalCTlist[i].numPIs; j++)
                {
                    if(j > 0)
                      fprintf(fout, ";");
                    fprintf(fout, "%ld^%ld^%d", finalCTlist[i].PIs[j].startPos,
                            finalCTlist[i].PIs[j].len,
                            finalCTlist[i].PIs[j].hasBreak);
                } /* for */
            } /* fi */

            else
              fprintf(fout, " ");

            fprintf(fout, "|\n");
        } /* fi oktoprint */
    } /* for */

    /* MH/NM/HM */

    for(i = 0; i < numFinalMHs; i++)
    {
        if(finalMHlist[i].oktoprint)
        {
            foo3 = show_MH_HM_ET(finalMHlist[i].datatype,
                                 finalMHlist[i].ETflag);

           strcpy(tmp, "");
            if(finalMHlist[i].paths[MMI] && finalMHlist[i].paths[PRC])
              strcpy(tmp, "MM;RC");

            else if(finalMHlist[i].paths[MMI])
              strcpy(tmp, "MM");

            else if(finalMHlist[i].paths[PRC])
              strcpy(tmp, "RC");

            else
              strcpy(tmp, "NA");

            if(strlen(finalMHlist[i].entry_term) > 0)
              foo = strdup(finalMHlist[i].entry_term);
            else
              foo = strdup(" ");

            /* Clean up entry term info in move from MM13 to MM14 */

            newET = cleanET(foo);
            free(foo);
            foo = strdup(newET);
            free(newET);

            if(strlen(finalMHlist[i].trigger) > 0)
              foo2 = strdup(finalMHlist[i].trigger);
            else
              foo2 = strdup(" ");

            if(strlen(finalMHlist[i].RC_PMIDs) > 0)
              foo4 = strdup(finalMHlist[i].RC_PMIDs);
            else
              foo4 = strdup(" ");

            if(strlen(finalMHlist[i].textloc) > 0)
              foo5 = strdup(finalMHlist[i].textloc);
            else
              foo5 = strdup(" ");

            fprintf(fout, "JUST: 4|%0.4f|%s|%s|%s|%s|%s|%ld|%s|%ld|%s|",
                    finalMHlist[i].VocabDenFactor, finalMHlist[i].mh, foo3, tmp,
                    foo, foo2, finalMHlist[i].num_RCs, foo4,
                    finalMHlist[i].score, foo5);

            free(foo);
            free(foo2);
            free(foo3);
            free(foo4);
            free(foo5);

            if(finalMHlist[i].numPIs > 0)
            {
                for(j = 0; j < finalMHlist[i].numPIs; j++)
                {
                    if(j > 0)
                      fprintf(fout, ";");
                    fprintf(fout, "%ld^%ld^%d", finalMHlist[i].PIs[j].startPos,
                            finalMHlist[i].PIs[j].len,
                            finalMHlist[i].PIs[j].hasBreak);
                } /* for */
            } /* fi */

            else
              fprintf(fout, " ");

            fprintf(fout, "|\n");
        } /* fi oktoprint */
    } /* for */

    /* SHs */

    for(i = 0; i < numFinalSHs; i++)
    {
        if(finalSHlist[i].oktoprint)
          fprintf(fout, "JUST: 5|%s|SH|\n",finalSHlist[i].mh);
    } /* for each SH */

    /* Publication Types (if appropriate) -
     JUST: 7|Publication Type|PT|From|Entry Term|Triggers|Score|Where|PI
    */

    for(i = 0; i < numPTsFound; i++)
    {
        if(foundPTsList[i].oktoprint)
        {
            fprintf(fout, "JUST: 7|0.0000|%s|PT|MM|%s|%s|%ld|%s|",
                    foundPTsList[i].mh, foundPTsList[i].entry_term,
                    foundPTsList[i].trigger, foundPTsList[i].score,
                    foundPTsList[i].textloc);

            if(foundPTsList[i].numPIs > 0)
            {
                for(j = 0; j < foundPTsList[i].numPIs; j++)
                {
                    if(j > 0)
                      fprintf(fout, ";");
                    fprintf(fout, "%ld^%ld^0", foundPTsList[i].PIs[j].startPos,
                            foundPTsList[i].PIs[j].len);
                } /* for */
            } /* fi */

            fprintf(fout, "|\n");
        } /* fi oktoprint */
    } /* for each PT */
} /* printFinalJustInfo */

/***************************************************************************
*                                                                          *
***************************************************************************/

char *pullTUI(char *inMH)
{
   char *rtn = (char *)malloc(51);
   char TUI[51], term[MAXLINE + 1], lcMH[MAXLINE + 1];
   char **rows;
   long len = (long)strlen(inMH);
   long i;
   int n, found;

   strcpy(rtn, "");

   strcpy(lcMH, inMH);
   for(i = 0; i < len; i++)
     if(isupper(lcMH[i]))
       lcMH[i] = tolower(lcMH[i]);

   query_btree(MHTUI, lcMH, &rows, &n);
   if(n > 0)
   {
        found = FALSE;
        for(i = 0; i < n; i++)
        {
            if(!found)
            {
               strcpy(TUI, "");
               strcpy(term, "");
               sscanf(rows[i], "%[^|]|%[^\n]", term, TUI);

               if(strcmp(term, inMH) == 0)
               {
                   found = TRUE;
                   strcpy(rtn, TUI);
               } /* fi */
            } /* fi */
        } /* for */

        /* If no direct match, use first one returned */

        if(!found)
        {
             strcpy(TUI, "");
             sscanf(rows[0], "%*[^|]|%[^\n]", TUI);
             strcpy(rtn, TUI);
        } /* fi */

        for(i = 0; i < n; i++)
           free(rows[i]);
   } /* fi n > 0 */

   if(rows != NULL)
     free(rows);

   return(rtn);
} /* pullTUI */

/***************************************************************************
*                                                                          *
***************************************************************************/

void pullTREEs(long pos)
{
   char tree_num[501];
   char **rows;
   int i, n, j;

   j = 0;
   query_btree(DUI_MN, finalMHlist[pos].dui, &rows, &n);
   for(i = 0; i < n; i++)
   {
       strcpy(tree_num, "");
       sscanf(rows[i], "%*[^|]|%*[^|]|%[^|]", tree_num);
       if(strlen(tree_num) > 0)
       {
          if(j < 25)
          {
              finalMHlist[pos].treecodes[j] = (char *)strdup(tree_num);
              j++;
          } /* fi */
       } /* fi */

       free(rows[i]);
   } /* for */
   finalMHlist[pos].num_treecodes = j;

   if(rows != NULL)
     free(rows);
} /* pullTREEs */

/***************************************************************************
*                                                                          *
***************************************************************************/

void pullTREEs_CT(long pos)
{
   char tree_num[501];
   char **rows;
   int i, n, j;

   j = 0;
   query_btree(DUI_MN, finalCTlist[pos].dui, &rows, &n);
   for(i = 0; i < n; i++)
   {
       strcpy(tree_num, "");
       sscanf(rows[i], "%*[^|]|%*[^|]|%[^|]", tree_num);
       if(strlen(tree_num) > 0)
       {
          if(j < 25)
          {
              finalCTlist[pos].treecodes[j] = (char *)strdup(tree_num);
              j++;
          } /* fi */
       } /* fi */

       free(rows[i]);
   } /* for */
   finalCTlist[pos].num_treecodes = j;

   if(rows != NULL)
     free(rows);
} /* pullTREEs_CT */

/***************************************************************************
*                                                                          *
***************************************************************************/

void pullTREEs_OVF(long pos)
{
   char tree_num[501];
   char **rows;
   int i, n, j;

   j = 0;
   query_btree(DUI_MN, finalOVFlist[pos].dui, &rows, &n);
   for(i = 0; i < n; i++)
   {
       strcpy(tree_num, "");
       sscanf(rows[i], "%*[^|]|%*[^|]|%[^|]", tree_num);
       if(strlen(tree_num) > 0)
       {
          if(j < 25)
          {
              finalOVFlist[pos].treecodes[j] = (char *)strdup(tree_num);
              j++;
          } /* fi */
       } /* fi */

       free(rows[i]);
   } /* for */
   finalOVFlist[pos].num_treecodes = j;

   if(rows != NULL)
     free(rows);
} /* pullTREEs_OVF */

/***************************************************************************
*                                                                          *
***************************************************************************/

void display_simple(int strictFilter_set, int show_labels, int show_id)
{
   if(show_id)
     fprintf(fout, "%s|", muid);

   if(!show_labels)
   {
       if(showType && !MeSHonDemand2 && !MoD_Raw && !MoD_PP)
         fprintf(fout, "MeSH on Demand Results:\n");
       else if(!MeSHonDemand2 && !MoD_Raw && !MoD_PP)
         fprintf(fout, "Results:\n");
   } /* fi */

   combined_display(strictFilter_set, FALSE, FALSE, FALSE, FALSE, 
             SIMPLE_DISPLAY, FALSE, show_labels, show_id, FALSE, FALSE, FALSE);
} /* display_simple */

/***************************************************************************
*                                                                          *
***************************************************************************/

void display_full_listing(int strictFilter_set, int medFilter_set, int OKonly)
{
   combined_display(strictFilter_set, medFilter_set, FALSE, FALSE, FALSE,
                    FULL_DISPLAY, FALSE, FALSE, FALSE, FALSE, FALSE, OKonly);
} /* display_full_listing */

/***************************************************************************
*
*  addSCR --
*
*      This 
*
***************************************************************************/

void addSCR(long uniq_ID)
{
    int found;
    long i, j;

    for(i = 0; i < mt_count; i++)
    {
        /* Check all SCRs that have nto printed yet and see if they are
           the NM for this HM.
        */

        if(mt_table[i].oktoprint && (mt_table[i].datatype == NM) &&
           !mt_table[i].hasPrinted)
        {
            found = FALSE;
            for(j = 0; !found && (j < mt_table[i].num_HMs); j++)
            {
                if(uniq_ID == mt_table[i].HMs[j])
                {
                    found = TRUE;
                    mt_table[i].hasPrinted = TRUE;
                    savePrinted(i, 0, 20);
                } /* fi */
            } /* for */
        } /* fi */
    } /* for */
} /* addSCR */

/***************************************************************************
*
*  findSAPMH --
*
*      This 
*
***************************************************************************/

int findSAPMH(char *mh)
{
    int rtn, found;
    long i;

    /* -1 Not Found, 1 - Found & Printed, 2 - Found & !Printed */

    rtn = -1;
    found = FALSE;
    for(i = 0; !found && (i < numFinalMHs); i++)
    {
         if(mh[0] == finalMHlist[i].mh[0])
         {
             if(strcmp(mh, finalMHlist[i].mh) == 0)
             {
                 found = TRUE;
                 if(finalMHlist[i].oktoprint)
                   rtn = 1;
                 else
                    rtn = 2;
             } /* fi */
         } /* fi */

         if(!found && finalMHlist[i].ETflag)
         {
             if(mh[0] == finalMHlist[i].mh_orig[0])
             {
                 if(strcmp(mh, finalMHlist[i].mh_orig) == 0)
                 {
                     found = TRUE;
                     if(finalMHlist[i].oktoprint)
                       rtn = 1;
                     else
                       rtn = 2;
                 } /* fi */
             } /* fi */
         } /* fi */
    } /* for */

    return(rtn);
} /* findSAPMH */

/***************************************************************************
*
*  cleanFinalList --
*
*      This 
*
***************************************************************************/

void cleanFinalList()
{
    int i;
    long j;

    /* Free up CTs */

    for(j = 0; j < numFinalCTs; j++)
    {
        free(finalCTlist[j].muid);
        free(finalCTlist[j].mh);
        free(finalCTlist[j].mh_orig);
        free(finalCTlist[j].dui);
        free(finalCTlist[j].entry_term);
        free(finalCTlist[j].RC_PMIDs);
        free(finalCTlist[j].trigger);
        free(finalCTlist[j].textloc);

        for(i = 0; i < finalCTlist[j].num_HMs; i++)
          free(finalCTlist[j].HMs[i]);

        for(i = 0; i < finalCTlist[j].num_treecodes; i++)
          free(finalCTlist[j].treecodes[i]);

        for(i = 0; i < finalCTlist[j].num_STs; i++)
        {
            free(finalCTlist[j].ST_info[i].MMI_trigger);
            free(finalCTlist[j].ST_info[i].rtm_ST);
        } /* for */
    } /* for */

    /* Free up SHs */

    for(j = 0; j < numFinalSHs; j++)
    {
        free(finalSHlist[j].muid);
        free(finalSHlist[j].mh);
        free(finalSHlist[j].mh_orig);
        free(finalSHlist[j].dui);
        free(finalSHlist[j].entry_term);
        free(finalSHlist[j].RC_PMIDs);
        free(finalSHlist[j].trigger);
        free(finalSHlist[j].textloc);

        for(i = 0; i < finalSHlist[j].num_HMs; i++)
          free(finalSHlist[j].HMs[i]);

        for(i = 0; i < finalSHlist[j].num_treecodes; i++)
          free(finalSHlist[j].treecodes[i]);

        for(i = 0; i < finalSHlist[j].num_STs; i++)
        {
            free(finalSHlist[j].ST_info[i].MMI_trigger);
            free(finalSHlist[j].ST_info[i].rtm_ST);
        } /* for */
    } /* for */

    /* Free up MHs */

    for(j = 0; j < numFinalMHs; j++)
    {
        free(finalMHlist[j].muid);
        free(finalMHlist[j].mh);
        free(finalMHlist[j].mh_orig);
        free(finalMHlist[j].dui);
        free(finalMHlist[j].entry_term);
        free(finalMHlist[j].RC_PMIDs);
        free(finalMHlist[j].trigger);
        free(finalMHlist[j].textloc);

        for(i = 0; i < finalMHlist[j].num_HMs; i++)
          free(finalMHlist[j].HMs[i]);

        for(i = 0; i < finalMHlist[j].num_treecodes; i++)
            free(finalMHlist[j].treecodes[i]);

        for(i = 0; i < finalMHlist[j].num_STs; i++)
        {
            free(finalMHlist[j].ST_info[i].MMI_trigger);
            free(finalMHlist[j].ST_info[i].rtm_ST);
        } /* for */
    } /* for */

    /* Free up OVFs */

    for(j = 0; j < numFinalOVFs; j++)
    {
        free(finalOVFlist[j].muid);
        free(finalOVFlist[j].mh);
        free(finalOVFlist[j].mh_orig);
        free(finalOVFlist[j].dui);
        free(finalOVFlist[j].entry_term);
        free(finalOVFlist[j].RC_PMIDs);
        free(finalOVFlist[j].trigger);
        free(finalOVFlist[j].textloc);

        for(i = 0; i < finalOVFlist[j].num_treecodes; i++)
          free(finalOVFlist[j].treecodes[i]);

        for(i = 0; i < finalOVFlist[j].num_STs; i++)
        {
            free(finalOVFlist[j].ST_info[i].MMI_trigger);
            free(finalOVFlist[j].ST_info[i].rtm_ST);
        } /* for */
    } /* for */
} /* cleanFinalList */

/***************************************************************************
*
*  saveOverFlow --
*
*      This 
*
***************************************************************************/

void saveOverFlow(long pos, int whichArray)
{
   int ETflag, ok, found;
   long i, p, score, j;
   char uid[SMALL_LINE + 1], mh[SMALL_LINE + 1], dt[25], dui[50],
        locs[25], textLoc[MAXLINE + 1], trigger[MAXLINE + 1],
        mh_orig[SMALL_LINE + 1];

   /* whichArray: 0 - mt_tble, 1 - ct_table, 3 - sh_table

      Original Position (origPos):
        CheckTag Table: pos + 1000
        Other Table: pos + 2000
        Subheading Table: pos + 3000
        BelowLine Adds: pos + 4000
   */

   if(whichArray == 0) /* mt_table */
   {
       if(numFinalOVFs < MAX_FINAL_MHs)
       {
           p = numFinalOVFs;
           finalOVFlist[p].ETflag = mt_table[pos].ETflag;
           finalOVFlist[p].datatype = mt_table[pos].datatype;
           finalOVFlist[p].oktoprint = mt_table[pos].oktoprint;
           finalOVFlist[p].wasForced = mt_table[pos].forcePrint;
           finalOVFlist[p].score = mt_table[pos].score;
           finalOVFlist[p].num_RCs = mt_table[pos].num_RCs;
           finalOVFlist[p].VocabDenFactor = mt_table[pos].VocabDenFactor;
           finalOVFlist[p].origPos = pos;

           finalOVFlist[p].muid = strdup(muid);
           finalOVFlist[p].mh = strdup(mt_table[pos].mh);
           finalOVFlist[p].mh_orig = strdup(mt_table[pos].mh_orig);
           finalOVFlist[p].dui = strdup(mt_table[pos].dui);
           finalOVFlist[p].entry_term = strdup(mt_table[pos].entry_term);
           finalOVFlist[p].RC_PMIDs = strdup(mt_table[pos].RC_PMIDs);
           finalOVFlist[p].trigger = strdup(mt_table[pos].trigger);
           finalOVFlist[p].textloc = strdup(mt_table[pos].textloc);

           /* Positional Information */

           finalOVFlist[p].numPIs = mt_table[pos].numPIs;
           for(i = 0; i < mt_table[pos].numPIs; i++)
           {
              finalOVFlist[p].PIs[i].isStart = mt_table[pos].PIs[i].isStart;
              finalOVFlist[p].PIs[i].startPos = mt_table[pos].PIs[i].startPos;
              finalOVFlist[p].PIs[i].endPos = mt_table[pos].PIs[i].endPos;
              finalOVFlist[p].PIs[i].len = mt_table[pos].PIs[i].len;
              finalOVFlist[p].PIs[i].hasBreak = mt_table[pos].PIs[i].hasBreak;
           } /* for */

           /* Paths */

           for(i = 0; i < NUM_PATHS; i++)
             finalOVFlist[p].paths[i] = mt_table[pos].paths[i];

           /* Treecodes - only keep first 25 (shouldn't be a problem */

           finalOVFlist[p].num_treecodes = mt_table[pos].num_treecodes;
           if(finalOVFlist[p].num_treecodes > 25)
             finalOVFlist[p].num_treecodes = 25;
           for(i = 0; (i < 25) && (i < mt_table[pos].num_treecodes); i++)
              finalOVFlist[p].treecodes[i] = strdup(mt_table[pos].treecodes[i]);

           /* Semantic Types - max 25 */

           finalOVFlist[p].num_STs = mt_table[pos].num_STs;
           for(i = 0; i < mt_table[pos].num_STs; i++)
           {
               finalOVFlist[p].ST_info[i].MMI_trigger =
                                   strdup(mt_table[pos].ST_info[i].MMI_trigger);
               finalOVFlist[p].ST_info[i].rtm_ST =
                                   strdup(mt_table[pos].ST_info[i].rtm_ST);
           } /* for */

           numFinalOVFs++;
       } /* fi mt_table */

       else /* Error */
       {
           fprintf(stderr, "ERROR: Too Many OVG: %ld of %ld\n",
                   numFinalOVFs, MAX_FINAL_MHs);
           fflush(stderr);
       } /* else */

       if(RTM_Debug)
       {
           if(strlen(mt_table[pos].mh_orig) > 0)
             fprintf(fout, 
                "mt_table - Pos: %ld  MH: #%s#  Which: %d\n",
                 pos, mt_table[pos].mh_orig, whichArray);
           else
             fprintf(fout, 
                 "mt_table - Pos: %ld  MH: #%s#  Which: %d\n",
                 pos, mt_table[pos].mh, whichArray);
       } /* fi RTM_Debug */
   } /* fi */

   else if(whichArray == 1) /* CheckTag Table */
   {
       if(numFinalOVFs < MAX_FINAL_MHs)
       {
           p = numFinalOVFs;
           finalOVFlist[p].ETflag = FALSE;
           finalOVFlist[p].datatype = CT;
           finalOVFlist[p].oktoprint = ct_table[pos].oktoprint;
           finalOVFlist[p].score = ct_table[pos].score;
           finalOVFlist[p].num_RCs = 0;
           finalOVFlist[p].VocabDenFactor = 0;
           finalOVFlist[p].origPos = pos + 1000;

           finalOVFlist[p].muid = strdup(muid);
           finalOVFlist[p].mh = strdup(ct_table[pos].mh);
           finalOVFlist[p].mh_orig = strdup("");
           finalOVFlist[p].dui = strdup(ct_table[pos].dui);
           finalOVFlist[p].entry_term = strdup("");
           finalOVFlist[p].RC_PMIDs = strdup("");
           finalOVFlist[p].trigger = strdup(ct_table[pos].trigger);
           finalOVFlist[p].textloc = strdup(ct_table[pos].textloc);

           /* Positional Information */

           finalOVFlist[p].numPIs = 0;

           /* Paths */

           for(i = 0; i < NUM_PATHS; i++)
             finalOVFlist[p].paths[i] = ct_table[pos].paths[i];

           /* Treecodes - only keep first 25 (shouldn't be a problem */

           finalOVFlist[p].num_treecodes = 0;
           pullTREEs_OVF(p);

           /* Semantic Types - max 25 */

           finalOVFlist[p].num_STs = 0;

           numFinalOVFs++;
       } /* fi */

       else /* Error */
       {
           fprintf(stderr, "ERROR: Too Many OVF: %ld of %ld\n",
                   numFinalOVFs, MAX_FINAL_MHs);
           fflush(stderr);
       } /* else */

       if(RTM_Debug)
       {
           fprintf(fout, "ct_table - Pos: %ld  MH: #%s#  Which: %d\n",
                   pos, ct_table[pos].mh, whichArray);
           fflush(fout);
       } /* fi */
   } /* else fi */

   else if(whichArray == 3) /* Subheading Table */
   {
       if(numFinalOVFs < MAX_FINAL_MHs)
       {
           p = numFinalOVFs;
           finalOVFlist[p].ETflag = FALSE;
           finalOVFlist[p].datatype = SH;
           finalOVFlist[p].oktoprint = sh_table[pos].oktoprint;
           finalOVFlist[p].score = sh_table[pos].score;
           finalOVFlist[p].num_RCs = 0;
           finalOVFlist[p].VocabDenFactor = 0;
           finalOVFlist[p].origPos = pos + 3000;

           finalOVFlist[p].muid = strdup(muid);
           finalOVFlist[p].mh = strdup(sh_table[pos].mh);
           finalOVFlist[p].mh_orig = strdup("");
           finalOVFlist[p].dui = strdup(sh_table[pos].dui);
           finalOVFlist[p].entry_term = strdup("");
           finalOVFlist[p].RC_PMIDs = strdup("");
           finalOVFlist[p].trigger = strdup(sh_table[pos].trigger);
           finalOVFlist[p].textloc = strdup(sh_table[pos].textloc);

           /* Positional Information */

           finalOVFlist[p].numPIs = 0;

           /* Paths */

           for(i = 0; i < NUM_PATHS; i++)
             finalOVFlist[p].paths[i] = sh_table[pos].paths[i];

           /* Treecodes - only keep first 25 (shouldn't be a problem */

           finalOVFlist[p].num_treecodes = 0;
           pullTREEs_OVF(p);

           /* Semantic Types - max 25 */

           finalOVFlist[p].num_STs = 0;

           numFinalOVFs++;
       } /* fi */

       else /* Error */
       {
           fprintf(stderr, "ERROR: Too Many OVG: %ld of %ld\n",
                   numFinalOVFs, MAX_FINAL_MHs);
           fflush(stderr);
       } /* else */

       if(RTM_Debug)
       {
           fprintf(fout, "sh_table - Pos: %ld  MH: #%s#  Which: %d\n",
                   pos, sh_table[pos].mh, whichArray);
           fflush(fout);
       } /* fi */
   } /* else fi */

   else
    printf("Unknown Type: %ld - %d\n", pos, whichArray);
} /* saveOverFlow */


/***************************************************************************
*
*  PrintNonLeaf --
*
*      This 
*
***************************************************************************/

void printNonLeaf()
{
   int done, found, posFound, hasBreak, hasSH;
   long i, j, k, p, index, startPos, len, numPI;
   char tmp[SMALL_LINE + 1], treecode[SMALL_LINE + 1], rest[SMALL_LINE + 1],
        SHlist[SMALL_LINE + 1];

   for(i = 0; i < numNL; i++)
   {
       hasSH = FALSE;
       if(strstr(nonLeafs[i].treecodes, "|SH:") != NULL)
         hasSH = TRUE;

       if(RTM_Debug)
       {
           fprintf(fout, "\nChecking Non-Leaf (%ld): #%s|%s|%s|%s|%s#\n", i,
                   nonLeafs[i].trigger, nonLeafs[i].MHname, nonLeafs[i].DUI,
                   nonLeafs[i].treecodes, nonLeafs[i].PI);
           fflush(fout);
       } /* fi */

       done = found = FALSE;

       /* Check to see if we already have the term as MH */

       for(j = 0; !found && (j < numFinalMHs); j++)
       {
           if(nonLeafs[i].DUI[4] == finalMHlist[j].dui[4])
           {
               if(strcmp(nonLeafs[i].DUI, finalMHlist[j].dui) == 0)
               {
                   found = TRUE;
                   finalMHlist[j].oktoprint = TRUE;
                   finalMHlist[j].wasForced = TRUE;
                   finalMHlist[j].paths[MMI] = TRUE;

                   if(hasSH)
                   {
                       strcpy(SHlist, "");
                       sscanf(nonLeafs[i].treecodes, "%*[^|]|%[^\n]", SHlist);
                   } /* fi */

                   if(strstr(finalMHlist[j].trigger, "Forced") == NULL)
                   {
                       if(strlen(finalMHlist[j].trigger) > 0)
                       {
                           if(hasSH)
                              sprintf(tmp, "%s;Forced Non-Leaf Node Lookup:%s - %s\0",
                                      finalMHlist[j].trigger, nonLeafs[i].trigger, SHlist);
                           else
                              sprintf(tmp, "%s;Forced Non-Leaf Node Lookup:%s\0",
                                      finalMHlist[j].trigger, nonLeafs[i].trigger);
                       } /* fi */

                       else
                       {
                           if(hasSH)
                             sprintf(tmp, "Forced Non-Leaf Node Lookup:%s - %s\0",
                                     nonLeafs[i].trigger, SHlist);
                           else
                             sprintf(tmp, "Forced Non-Leaf Node Lookup:%s\0", nonLeafs[i].trigger);
                       } /* else */

                       free(finalMHlist[j].trigger);
                       finalMHlist[j].trigger = strdup(tmp);
                   } /* fi */

                   /* See if we should expand positional info we
                      have for this MH.  Make sure we don't add
                      duplicate positional info.
                   */

                   numPI = finalMHlist[j].numPIs;
                   strcpy(tmp, nonLeafs[i].PI);
                   while(strlen(tmp) > 0)
                   {
                       strcpy(rest, "");
                       hasBreak = startPos = len = -1;
                       sscanf(tmp, "%ld^%ld^%d;%[^\n]",
                              &startPos, &len, &hasBreak, rest);

                       strcpy(tmp, rest);

                       /* Need to compensate for the added text we put in
                          to get the freeText to work properly.
                       */

                       if(haveFreeText)
                         startPos -= 21;

                       if((startPos > -1) && (len > -1))
                       {
                           posFound = FALSE;
                           for(p = 0; !posFound && (p < numPI); p++)
                           {
                               if(finalMHlist[j].PIs[p].startPos == startPos)
                                 posFound = TRUE;
                           } /* for */

                           if(!posFound)
                           {
                               finalMHlist[j].PIs[numPI].isStart = TRUE;
                               finalMHlist[j].PIs[numPI].startPos = startPos;
                               finalMHlist[j].PIs[numPI].endPos = 
                                                         startPos + len;
                               finalMHlist[j].PIs[numPI].len = len;
                               finalMHlist[j].PIs[numPI].hasBreak = hasBreak;
                               numPI++;
                           } /* fi */
                       } /* fi */
                   } /* while */

                   finalMHlist[j].numPIs = numPI;

                   if(RTM_Debug)
                   {
                       fprintf(fout, "Ignoring Non-Leaf #%s#",
                               nonLeafs[i].MHname);
                       fprintf(fout, " already in final MH list\n");
                       fflush(fout);
                   } /* fi */
               } /* fi */
           } /* fi */
       } /* for */

       /* Check to see if we already have the term as CT */

       for(j = 0; !found && (j < numFinalCTs); j++)
       {
           if(nonLeafs[i].DUI[4] == finalCTlist[j].dui[4])
           {
               if(strcmp(nonLeafs[i].DUI, finalCTlist[j].dui) == 0)
               {
                   found = TRUE;
                   finalCTlist[j].oktoprint = TRUE;
                   finalCTlist[j].wasForced = TRUE;
                   finalCTlist[j].paths[MMI] = TRUE;

                   /* See if we should expand positional info we
                      have for this CT.  Make sure we don't add
                      duplicate positional info.
                   */

                   numPI = finalCTlist[j].numPIs;
                   strcpy(tmp, nonLeafs[i].PI);
                   while(strlen(tmp) > 0)
                   {
                       strcpy(rest, "");
                       hasBreak = startPos = len = -1;
                       sscanf(tmp, "%ld^%ld^%d;%[^\n]",
                              &startPos, &len, &hasBreak, rest);

                       strcpy(tmp, rest);

                       /* Need to compensate for the added text we put in
                          to get the freeText to work properly.
                       */

                       if(haveFreeText)
                         startPos -= 21;

                       if((startPos > -1) && (len > -1))
                       {
                           posFound = FALSE;
                           for(p = 0; !posFound && (p < numPI); p++)
                           {
                               if(finalCTlist[j].PIs[p].startPos == startPos)
                                 posFound = TRUE;
                           } /* for */

                           if(!posFound)
                           {
                               finalCTlist[j].PIs[numPI].isStart = TRUE;
                               finalCTlist[j].PIs[numPI].startPos = startPos;
                               finalCTlist[j].PIs[numPI].endPos = 
                                                         startPos + len;
                               finalCTlist[j].PIs[numPI].len = len;
                               finalCTlist[j].PIs[numPI].hasBreak = hasBreak;
                               numPI++;
                           } /* fi */
                       } /* fi */
                   } /* while */

                   finalCTlist[j].numPIs = numPI;

                   if(RTM_Debug)
                   {
                       fprintf(fout, "Ignoring Non-Leaf #%s#",
                               nonLeafs[i].MHname);
                       fprintf(fout, " already in final CT list\n");
                       fflush(fout);
                   } /* fi */
               } /* fi */
           } /* fi */
       } /* for */

       /* Check to see if we already have the term as SH */

       for(j = 0; !found && (j < numFinalSHs); j++)
       {
           if(strcasecmp(nonLeafs[i].MHname, finalSHlist[j].mh) == 0)
           {
               found = TRUE;
               if(RTM_Debug)
               {
                   fprintf(fout, "Ignoring Non-Leaf #%s#",
                           nonLeafs[i].MHname);
                   fprintf(fout, " already in final SH list\n");
                   fflush(fout);
               } /* fi */
           } /* fi */
       } /* for */

       for(j = 0; !found && (j < num_MH_SH); j++)
       {
           if(strcasecmp(nonLeafs[i].MHname, MH_SH_list[j].MH_concept) == 0)
           {
               found = TRUE;
               if(RTM_Debug)
               {
                   fprintf(fout, "Ignoring Non-Leaf #%s#",
                           nonLeafs[i].MHname);
                   fprintf(fout, " should be mapped to SH\n");
                   fflush(fout);
               } /* fi */
           } /* fi */
       } /* for */

       /* Check to see if we had the term, but, didn't transfer it over
          to the final set of terms.
       */

       if(!found)
       {
          index = search_index(nonLeafs[i].DUI);
          if(index > -1)
          {
               if((mt_table[index].paths[MMI] && !mt_table[index].paths[PRC]) ||
                  (mt_table[index].paths[MMI] && mt_table[index].paths[PRC]))
               {
                   if(mt_table[index].score < 1)
                   {
                       found = TRUE;
                       if(RTM_Debug)
                       {
                           fprintf(fout,
                              "Ignoring Non-Leaf #%s# - #%s# %d|%d|%ld|%d",
                               nonLeafs[i].MHname, mt_table[index].trigger,
                               mt_table[index].paths[MMI],
                               mt_table[index].paths[PRC],
                               mt_table[index].score,
                               mt_table[index].oktoprint);
                           fprintf(fout, " already eliminated\n");
                           fflush(fout);
                       } /* fi */
                   } /* fi score < 1 */
               } /* fi */
          } /* fi */
       } /* fi */

       /* If ok, then see if we have a more specific term already printed */

       strcpy(tmp, nonLeafs[i].treecodes);
       while(!found && (strlen(tmp) > 0))
       {
           strcpy(treecode, "");
           strcpy(rest, "");
           sscanf(tmp, "%[^;];%[^\n]", treecode, rest);
           strcpy(tmp, rest);

           if(strlen(treecode) > 0)
           {
               for(j = 0; !found && (j < numFinalMHs); j++)
               {
                   for(k = 0; !found && (k < finalMHlist[j].num_treecodes); k++)
                   {
                       if(treecode[0] == finalMHlist[j].treecodes[k][0])
                       {
                           if(strcmp(treecode, 
                                     finalMHlist[j].treecodes[k]) != 0)
                           {
                               if(strstr(finalMHlist[j].treecodes[k],
                                         treecode) != NULL)
                               {
                                   /* Mark the more specific term as forced since this was
                                      a leafnode find.
                                   */

                                   found = TRUE;
                                   finalMHlist[j].wasForced = TRUE;
                                   if(RTM_Debug)
                                   {
                                       fprintf(fout,
                                         "hNL Found Specific: %s|%s (%s) -> %s|%s\n",
                                         nonLeafs[i].MHname, treecode, nonLeafs[i].trigger,
                                         finalMHlist[j].mh,
                                         finalMHlist[j].treecodes[k]);
                                      fflush(fout);
                                   } /* fi RTM_Debug */
                               } /* fi */
                           } /* fi */
                       } /* fi */
                   } /* for each treecode */
               } /* for each MH */
           } /* fi */
       } /* while */

       /* If we did not find a more specific term, then go ahead and add
          the nonLeaf.
       */

       if(!found)
       {
           if(RTM_Debug)
           {
               fprintf(fout, "Adding Non-Leaf Node: %s (%s)\n",
                       nonLeafs[i].MHname, nonLeafs[i].trigger);
               fflush(fout);
           } /* fi */

           if(hasSH)
           {
               strcpy(SHlist, "");
               sscanf(nonLeafs[i].treecodes, "%*[^|]|%[^\n]", SHlist);
               sprintf(tmp, "Forced Non-Leaf Node Lookup:%s - %s\0",
                   nonLeafs[i].trigger, SHlist);
           } /* fi */

           else
             sprintf(tmp, "Forced Non-Leaf Node Lookup:%s\0", nonLeafs[i].trigger);

           if(isCTDUI(nonLeafs[i].DUI))
             process_mterm(FALSE, "", nonLeafs[i].MHname, MMI, 1000, CT,
                  "", nonLeafs[i].ET, tmp, nonLeafs[i].DUI,
                  "", nonLeafs[i].PI, TRUE);

           else if(nonLeafs[i].DUI[0] == 'C')
             process_mterm(FALSE, "", nonLeafs[i].MHname, MMI, 1000, NM,
                  "", nonLeafs[i].ET, tmp, nonLeafs[i].DUI,
                  "", nonLeafs[i].PI, TRUE);
           else
             process_mterm(FALSE, "", nonLeafs[i].MHname, MMI, 1000, MH,
                   "", nonLeafs[i].ET, tmp, nonLeafs[i].DUI,
                    "", nonLeafs[i].PI, TRUE);

           index = search_index(nonLeafs[i].DUI);
           if((index >= 0) && (mt_table[index].score <= 0))
             mt_table[index].score += 1000;
       } /* fi !done */

       free(nonLeafs[i].trigger);
       free(nonLeafs[i].MHname);
       free(nonLeafs[i].DUI);
       free(nonLeafs[i].treecodes);
       free(nonLeafs[i].ET);
       free(nonLeafs[i].PI);
   } /* for */

   numNL = 0;
} /* printNonLeaf */


/***************************************************************************
*
*  printRemovals --
*
*      This 
*
***************************************************************************/

void printRemovals()
{
   int found;
   long i, score, j;
   char *foo2, mh[SMALL_LINE + 1], *dui;

   for(i = 0; i < numFinalCTs; i++)
   {
      if(!finalCTlist[i].oktoprint)
      {
          if(showScoresFull)
          {
              score = finalCTlist[i].score;
              if(score < 0)
                score = 0;
              fprintf(fout, "MTI|%s|%ld|%s|CT|Removed\n", finalCTlist[i].dui,
                      score, finalCTlist[i].mh);
          } /* fi */

          else
          {
              fprintf(fout, "REMOVED|%s|", muid);
              fprintf(fout, "%s|", finalCTlist[i].mh);

              fprintf(fout, "MM: %d|PR: %d|Final CT Table|%ld\n",
                 finalCTlist[i].paths[MMI], finalCTlist[i].paths[PRC], i);
          } /* else */
      } /* fi */
   } /* for */

   for(i = 0; i < numFinalMHs; i++)
   {
      if(!finalMHlist[i].oktoprint && (finalMHlist[i].datatype != NM) &&
          (finalMHlist[i].datatype != SH))
      {
          strcpy(mh, "");
          if(strlen(finalMHlist[i].mh_orig) > 0)
            strcpy(mh, finalMHlist[i].mh_orig);
          else
            strcpy(mh, finalMHlist[i].mh);

          if(showScoresFull)
          {
              score = finalMHlist[i].score;
              if(score < 0)
                score = 0;
              fprintf(fout, "MTI|%s|%ld|%s|MH|Removed\n", finalMHlist[i].dui,
                      score, mh);
          } /* fi */

          else
          {
              fprintf(fout, "REMOVED|%s|", muid);
              fprintf(fout, "%s|", mh);

              fprintf(fout, "MM: %d|PR: %d|Final MH Table|%ld\n",
                 finalMHlist[i].paths[MMI], finalMHlist[i].paths[PRC], i);
          } /* else */
      } /* fi */
   } /* for */

   for(i = 0; i < mt_count; i++)
   {
      if(!mt_table[i].oktoprint || !mt_table[i].hasPrinted)
      {
          strcpy(mh, "");
          if(strlen(mt_table[i].mh_orig) > 0)
            strcpy(mh, mt_table[i].mh_orig);
          else
            strcpy(mh, mt_table[i].mh);

          /* Check to make sure we didn't actually print term even
             though it says we didn't.  Some of the final filtering
             may add terms.  Need to check both finalCTlist & finalMHlist.
          */

          found = FALSE;
          for(j = 0; !found && (j < numFinalMHs); j++)
          {
              if(finalMHlist[j].oktoprint && 
                 (mt_table[i].dui[4] == finalMHlist[j].dui[4]))
              {
                  if(strcmp(mt_table[i].dui, finalMHlist[j].dui) == 0)
                    found = TRUE;
              } /* fi */
          } /* for */

          for(j = 0; !found && (j < numFinalCTs); j++)
          {
              if(finalCTlist[j].oktoprint && 
                 (mt_table[i].dui[4] == finalCTlist[j].dui[4]))
              {
                  if(strcmp(mt_table[i].dui, finalCTlist[j].dui) == 0)
                    found = TRUE;
              } /* fi */
          } /* for */

          if(!found)
          {
              if(showScoresFull)
              {
                  if(mt_table[i].datatype != SH)
                  {
                      foo2 = show_MH_HM_ET(mt_table[i].datatype,
                                           mt_table[i].ETflag);

                      score = mt_table[i].score;
                      if(score < 0)
                        score = 0;
                      fprintf(fout, "MTI|%s|%ld|%s|%s|Removed\n",
                              mt_table[i].dui, score, mh, foo2);
                      free(foo2);
                   } /* fi */
              } /* fi */

              else
              {
                  fprintf(fout, "REMOVED|%s|", muid);
                  fprintf(fout, "%s|", mh);

                  fprintf(fout, "MM: %d|PR: %d:%ld|Vocab: %0.4f|mt_table|%ld\n",
                     mt_table[i].paths[MMI], mt_table[i].paths[PRC], 
                     mt_table[i].num_RCs, mt_table[i].VocabDenFactor, i);
               } /* else */
          } /* fi !found */
      } /* fi */
   } /* for */

   for(i = 0; i < ct_count; i++)
   {
      /* Check to make sure we didn't actually print term even
         though it says we didn't.  Some of the final filtering
         may add terms.
      */

      if(!ct_table[i].oktoprint || !ct_table[i].hasPrinted)
      {
          found = FALSE;
          for(j = 0; !found && (j < numFinalCTs); j++)
          {
              if(finalCTlist[j].oktoprint && 
                 (ct_table[i].dui[4] == finalCTlist[j].dui[4]))
              {
                  if(strcmp(ct_table[i].dui, finalCTlist[j].dui) == 0)
                    found = TRUE;
              } /* fi */
          } /* for */

          if(!found)
          {
              if(showScoresFull)
              {
                  score = ct_table[i].score;
                  if(score < 0)
                    score = 0;

                  dui = findMHDUI(ct_table[i].mh);
                  fprintf(fout, "MTI|%s|%ld|%s|CT|Removed\n", dui,
                          score, ct_table[i].mh);
                  free(dui);
              } /* fi */

              else
              {
                  fprintf(fout, "REMOVED|%s|", muid);
                  fprintf(fout, "%s|", ct_table[i].mh);

                  fprintf(fout, "MM: %d|PR: %d|ct_table|%ld\n",
                     ct_table[i].paths[MMI], ct_table[i].paths[PRC], i);
              } /* else */
          } /* fi !found */
      } /* fi */
   } /* for */

   fprintf(fout, "\n");
} /* printRemovals */


/***************************************************************************
*
*  cleanET --
*
*      This 
*
***************************************************************************/

char *cleanET(char *inET)
{
   long len = (long)strlen(inET);
   long diff, tLen, i, pos;
   char *rtn = (char *)malloc(len + 1000);
   char *tmp, *tmpc, *rest, *cRest, *term, *t1, *t2, *t3, *t4, *tr, *foo;

   strcpy(rtn, "");
   if(strchr(inET, '-') == NULL)
     strcpy(rtn, inET);

   /* We want to remove the new Part-of-Speech and Negation designations
      so downstream programs don't need to be changed.
   */

   else
   {
       tmpc = strdup(inET);
       tmp = (char *)malloc(len + 1);
       rest = (char *)malloc(len + 1);
       cRest = (char *)malloc(len + 1);
       term = (char *)malloc(len + 1);
       t1 = (char *)malloc(len + 1);
       t2 = (char *)malloc(len + 1);
       t3 = (char *)malloc(len + 1);
       t4 = (char *)malloc(len + 1);
       tr = (char *)malloc(len + 1);
       strcpy(rest, "");
       strcpy(cRest, "");
       strcpy(term, "");
       strcpy(t1, "");
       strcpy(t2, "");
       strcpy(t3, "");
       strcpy(t4, "");
       strcpy(tr, "");
       strcpy(tmp, "");

       if(RTM_Debug)
         fprintf(fout, "\ntmpc: #%s#\n", tmpc);

       while(strlen(tmpc) > 0)
       {
           strcpy(tmp, "");
           strcpy(cRest, "");
           sscanf(tmpc, "%[^:]:%[^\n]", tmp, cRest);

           while(strlen(tmp) > 0)
           {
               strcpy(term, "");
               strcpy(rest, "");

               foo = strstr(tmp, ",\"");
               tLen = (long)strlen(tmp);

               if(foo == NULL)
                 diff = tLen;

               else
                 diff = foo - tmp;

               for(i = 0; i < diff; i++)
                 term[i] = tmp[i];
               term[diff] = '\0';

               pos = 0;
               for(i = (diff + 1); i < tLen; i++)
                 rest[pos++] = tmp[i];
               rest[pos] = '\0';

               if(RTM_Debug)
                 fprintf(fout, "term: #%s#\nrest: #%s#\n", term, rest);

               strcpy(tmp, rest);
               if(strlen(term) > 0)
               {
                   if(strchr(term, '-') == NULL)
                     strcat(rtn, term);

                   else
                   {
                       strcpy(t1, "");
                       strcpy(t2, "");
                       strcpy(t3, "");
                       strcpy(t4, "");
                       strcpy(tr, "");
                       if(term[0] == '[')
                       {
                          strcat(rtn, "[");
                          sscanf(term,
                          "[\"%[^\"]\"-%[^-]-%[^-]-\"%[^\"]\"-%*[^-]-%*d%[^\n]",
                          t1, t2, t3, t4, tr);
                       } /* fi */

                       else
                       sscanf(term, 
                          "\"%[^\"]\"-%[^-]-%[^-]-\"%[^\"]\"-%*[^-]-%*d%[^\n]",
                              t1, t2, t3, t4, tr);

                        if(RTM_Debug)
                        {
                            fprintf(fout, "  t1: #%s#\n", t1);
                            fprintf(fout, "  t2: #%s#\n", t2);
                            fprintf(fout, "  t3: #%s#\n", t3);
                            fprintf(fout, "  t4: #%s#\n", t4);
                            fprintf(fout, "  tr: #%s#\n", tr);
                       } /* fi */

                       strcat(rtn, "\"");
                       strcat(rtn, t1);
                       strcat(rtn, "\"");

                       if(strlen(t2) > 0)
                         strcat(rtn, "-");
                       strcat(rtn, t2);

                       if(strlen(t3) > 0)
                         strcat(rtn, "-");
                       strcat(rtn, t3);

                       if(strlen(t4) > 0)
                         strcat(rtn, "-\"");
                       strcat(rtn, t4);
                       if(strlen(t4) > 0)
                         strcat(rtn, "\"");

                       strcat(rtn, tr);

                       if(strlen(tmp) > 0)
                         strcat(rtn, ",");
                   } /* else */
               } /* fi */
           } /* inner while */

           strcpy(tmpc, cRest);
           if(strlen(tmpc) > 0)
             strcat(rtn, ":");
       } /* while */

       free(tmp); free(rest); free(term); free(tmpc); free(cRest);
       free(t1); free(t2); free(t3); free(tr); free(t4);
   } /* else */

   /* Verify that the last thing is a close bracket */

   tLen = (long)strlen(rtn);
   if(tLen > 2)
   {
       if(rtn[tLen - 1] != ']')
         strcat(rtn, "]");
   } /* fi */

   if(RTM_Debug)
     fprintf(fout, "rtn: #%s#\n", rtn);

   return(rtn);
} /* cleanET */


/***************************************************************************
*
*  print_MoD_Raw --
*
*      This 
*
***************************************************************************/

void print_MoD_Raw()
{
   long i, k, numUsed, numPrinted, x, len, pos;
   char tmp[25], *foo, *foo2;

   numUsed = 0;
   fprintf(fout, "{ \"MoD_Raw\": {\n");
   fprintf(fout, "   \"PRC_List\": [\n");
   for(i = 0; i < numPRC; i++)
   {
       if(PRC_Scores[i] > -1)
       {
           foo = findTI(PRC_PMIDs[i]);
           len = (long)strlen(foo);
           foo2 = (char *)malloc((len * 2) + 1);
           strcpy(foo2, "");
           pos = 0;
           for(x = 0; x < len; x++)
           {
               if(foo[x] == '"')
                 foo2[pos++] = '\\';
               foo2[pos++] = foo[x];
           } /* for */
           foo2[pos] = '\0';

           if(numUsed > 0)
             fprintf(fout, ",\n");
           fprintf(fout, "      { \"PMID\":%s, \"Title\":", PRC_PMIDs[i]);
           fprintf(fout, "\"%s\" }", foo2);
           numUsed++;

           free(foo); free(foo2);
       } /* fi */
   } /* for */
   

   numPrinted = 0;
   if((numFinalCTs + numFinalMHs) > 0)
   {
       fprintf(fout, "\n ],\n");
       fprintf(fout, "   \"Term_List\": [\n");

       for(i = 0; i < numFinalCTs; i++)
       {
           if(finalCTlist[i].oktoprint)
           {
                if(numPrinted > 0)
                  fprintf(fout, ",\n");
                numPrinted++;
                fprintf(fout, "      { \"Term\":");
                fprintf(fout, "\"%s\", \"Type\":\"CT\"", finalCTlist[i].mh);
                if(finalCTlist[i].numPIs > 0)
                {
                    fprintf(fout, ", \"Pos_List\": [\n");
                    for(k = 0; k < finalCTlist[i].numPIs; k++)
                    {
                        if(k > 0)
                         fprintf(fout, ",\n");
                        fprintf(fout, "           { \"start\":%ld",
                                finalCTlist[i].PIs[k].startPos);
                        fprintf(fout, ", \"len\":%ld }",
                                finalCTlist[i].PIs[k].len);
                    } /* for */
                    fprintf(fout, "\n           ]\n");
                } /* fi */

                fprintf(fout, "      }");
           } /* fi */
       } /* for */

       for(i = 0; i < numFinalMHs; i++)
       {
           if(finalMHlist[i].oktoprint)
           {
                if(numPrinted > 0)
                  fprintf(fout, ",\n");
                numPrinted++;

                strcpy(tmp, "MH");
                if(finalMHlist[i].datatype == NM)
                  strcpy(tmp, "NM");

                else if(finalMHlist[i].datatype == HM)
                  strcpy(tmp, "HM");

                fprintf(fout, "      { \"Term\":");
                fprintf(fout, "\"%s\", \"Type\":\"%s\"",finalMHlist[i].mh, tmp);
                if(finalMHlist[i].numPIs > 0)
                {
                    fprintf(fout, ", \"Pos_List\": [\n");
                    for(k = 0; k < finalMHlist[i].numPIs; k++)
                    {
                        if(k > 0)
                         fprintf(fout, ",\n");
                        fprintf(fout, "           { \"start\":%ld",
                                finalMHlist[i].PIs[k].startPos);
                        fprintf(fout, ", \"len\":%ld }",
                                finalMHlist[i].PIs[k].len);
                    } /* for */
                    fprintf(fout, "\n           ]\n");
                } /* fi */

                fprintf(fout, "      }");
           } /* fi */
       } /* for */

       fprintf(fout, "\n   ]\n");
   } /* fi */

   else
     fprintf(fout, "\n ]\n");

   /* Close out results */

   fprintf(fout, "  }\n}\n");
} /* print_MoD_Raw */

/***************************************************************************
*
*  findTI --
*
*      This routine pulls the Title from the PRC citations from our own
*  version of PubMed so we don't have to call them again.
*
***************************************************************************/

char *findTI(char *PMID)
{
    char *rtn;
    char **rows;
    int n, k;

    rtn = (char *)malloc(250);
    strcpy(rtn, "");

    query_btree(PMID_TI, PMID, &rows, &n);

    if((rows != NULL) && (n > 0))
    {
        free(rtn);
        rtn = strdup(rows[0]);
        strcpy(rtn, "");
        sscanf(rows[0], "%*[^|]|%*d|%[^|]", rtn);
        for(k = 0; k < n; k++)
           free(rows[k]);
        free(rows);
    } /* fi */
    
    else if(rows != NULL)
      free(rows);

    if(strlen(rtn) == 0)
    {
        fprintf(stderr, "PMID Not found: #%s#\n", PMID);
        fflush(stderr);
    } /* fi */

    return(rtn);
} /* findTI */


/***************************************************************************
*
*  updateFinalList --
*
*      This 
*
***************************************************************************/

void updateFinalList(char *mh, char *dui, char *from, int which)
{
   int found;
   long j;
   char tmp[MAXLINE + 1];

   if(which == MH)
   {
       found = FALSE;
       for(j = 0; !found && (j < numFinalMHs); j++)
       {
           if(strcmp(dui, finalMHlist[j].dui) == 0)
           {
               found = TRUE;
               finalMHlist[j].wasForced = TRUE;

               /* If don't already have this note, add it */

               if(strstr(finalMHlist[j].trigger, from) == NULL)
               {
                   finalMHlist[j].paths[MMI] = TRUE;
                   strcpy(tmp, finalMHlist[j].trigger);
                   if(strlen(tmp) > 0)
                     strcat(tmp, "; ");
                   strcat(tmp, from);
                   free(finalMHlist[j].trigger);
                   finalMHlist[j].trigger = strdup(tmp);
               } /* fi */
           } /* fi */
       } /* for */
   } /* fi MH */

   else if(which == CT)
   {
       found = FALSE;
       for(j = 0; !found && (j < numFinalCTs); j++)
       {
           if(strcmp(dui, finalCTlist[j].dui) == 0)
           {
               found = TRUE;
               finalMHlist[j].wasForced = TRUE;

               /* If don't already have this note, add it */

               if(strstr(finalCTlist[j].trigger, from) == NULL)
               {
                   finalCTlist[j].paths[MMI] = TRUE;
                   strcpy(tmp, finalCTlist[j].trigger);
                   if(strlen(tmp) > 0)
                     strcat(tmp, "; ");
                   strcat(tmp, from);
                   free(finalCTlist[j].trigger);
                   finalCTlist[j].trigger = strdup(tmp);
               } /* fi */
           } /* fi */
       } /* for */
   } /* fi CT */
} /* updateFinalList */


/***************************************************************************
*
*  print_JSON --
*
*      This 
*
***************************************************************************/

void print_JSON()
{
   long i, numPrinted, x, len;
   char tmp[25];

   fprintf(fout, "{ \"MTI_Result\": {\n");
   fprintf(fout, "   \"PMID\":\"%s\"", muid);

   numPrinted = 0;
   if((numFinalCTs + numFinalMHs) > 0)
   {
       fprintf(fout, ",\n   \"Term_List\": [\n");

       for(i = 0; i < numFinalCTs; i++)
       {
           if(finalCTlist[i].oktoprint)
           {
                if(numPrinted > 0)
                  fprintf(fout, ",\n");
                numPrinted++;
                fprintf(fout, "      { \"Term\":\"%s\", ", finalCTlist[i].mh);
                fprintf(fout, "\"DUI\":\"%s\", ", finalCTlist[i].dui);
                fprintf(fout, "\"Score\":%ld, ", finalCTlist[i].score);
                fprintf(fout, "\"Type\":\"CT\", ");

                len = (long)strlen(finalCTlist[i].trigger);
                if(len > 0)
                {
                    fprintf(fout, "\"Trigger\":\"");

                    for(x = 0; x < len; x++)
                    {
                        if(finalCTlist[i].trigger[x] == '"')
                          fprintf(fout, "\\");
                          fprintf(fout, "%c", finalCTlist[i].trigger[x]);
                    } /* for */

                    fprintf(fout, "\", ");
                } /* fi */

                if(strlen(finalCTlist[i].textloc) > 0)
                  fprintf(fout, "\"Location\":\"%s\", ",
                          finalCTlist[i].textloc);

                fprintf(fout, "\"Path\":");
                if(finalCTlist[i].paths[MMI] && finalCTlist[i].paths[PRC])
                  fprintf(fout, "\"MM;RC\"");

                else if(finalCTlist[i].paths[MMI])
                  fprintf(fout, "\"MM\"");

                else if(finalCTlist[i].paths[PRC])
                  fprintf(fout, "\"RC\"");

                else
                  fprintf(fout, "\"NA\"");
                fprintf(fout, " }");

                numOverallPrinted++;
           } /* fi */
       } /* for */

       for(i = 0; i < numFinalMHs; i++)
       {
           if(finalMHlist[i].oktoprint)
           {
                if(numPrinted > 0)
                  fprintf(fout, ",\n");
                numPrinted++;

                strcpy(tmp, "MH");
                if(finalMHlist[i].datatype == NM)
                  strcpy(tmp, "NM");

                else if(finalMHlist[i].datatype == HM)
                  strcpy(tmp, "HM");

                fprintf(fout, "      { \"Term\":\"%s\", ", finalMHlist[i].mh);
                fprintf(fout, "\"DUI\":\"%s\", ", finalMHlist[i].dui);
                fprintf(fout, "\"Score\":%ld, ", finalMHlist[i].score);
                fprintf(fout, "\"Type\":\"%s\", ", tmp);

                len = (long)strlen(finalMHlist[i].trigger);
                if(len > 0)
                {
                    fprintf(fout, "\"Trigger\":\"");

                    for(x = 0; x < len; x++)
                    {
                        if(finalMHlist[i].trigger[x] == '"')
                          fprintf(fout, "\\");
                          fprintf(fout, "%c", finalMHlist[i].trigger[x]);
                    } /* for */

                    fprintf(fout, "\", ");
                } /* fi */

                if(strlen(finalMHlist[i].textloc) > 0)
                  fprintf(fout, "\"Location\":\"%s\", ",
                          finalMHlist[i].textloc);
                fprintf(fout, "\"Path\":");
                if(finalMHlist[i].paths[MMI] && finalMHlist[i].paths[PRC])
                  fprintf(fout, "\"MM;RC\"");

                else if(finalMHlist[i].paths[MMI])
                  fprintf(fout, "\"MM\"");

                else if(finalMHlist[i].paths[PRC])
                  fprintf(fout, "\"RC\"");

                else
                  fprintf(fout, "\"NA\"");
                fprintf(fout, " }");

                numOverallPrinted++;
           } /* fi */
       } /* for */

       fprintf(fout, "\n   ]\n");
   } /* fi */
   else
     fprintf(fout, "\n");

   /* Close out results */

   fprintf(fout, "  }\n}\n");
} /* print_JSON */


/***************************************************************************
*
*  print_IlyaML --
*
*      This 
*
***************************************************************************/

void print_IlyaML()
{
   long i, x, len;
   char tmp[25], *foo;

   fprintf(fout, "%s\n", L2R_Citation);
   fprintf(fout, "JID - %s\n", nlmID);

   if((numFinalCTs + numFinalMHs) > 0)
   {
       for(i = 0; i < numFinalCTs; i++)
       {
           if(finalCTlist[i].oktoprint)
           {
               fprintf(fout, "MTI|%s|%ld|%s|CT\n", finalCTlist[i].dui,
                       finalCTlist[i].score, finalCTlist[i].mh);
           } /* fi */
       } /* for */

       for(i = 0; i < numFinalMHs; i++)
       {
           if(finalMHlist[i].oktoprint)
           {
                if(finalMHlist[i].datatype != NM)
                {
                    fprintf(fout, "MTI|%s|%ld|%s|MH\n", finalMHlist[i].dui,
                            finalMHlist[i].score, finalMHlist[i].mh);
                } /* fi */
           } /* fi */
       } /* for */
   } /* fi */
   else
     fprintf(fout, "MTI|NO RESULTS\n");
} /* print_IlyaML */


/***************************************************************************
*
*  doLearning2Rank --
*
*      This 
*
***************************************************************************/

void doLearning2Rank(int BioASQ)
{
   int found, n, k, ok, foundQ, okCT, isCT_L2R, foundERR, numRepeat,
       foundDiseases, foundAnimalDiseases, foundAnimals, ok_noCON,
       haveCOVID19, isAmbig;
   long i, j, l, x, p, Q, len, seedval, rtn_random, numResults, nxt_random,
        index, numLines, diseasePos;
   float score;
   char tmpFile[SMALL_LINE + 1], line[SMALL_LINE + 1], cmd[SMALL_LINE + 1],
        IL2R_host[SMALL_LINE + 1], *L2R_lines[MAX_L2R_RESULTS + 1], ambigTrigger[SMALL_LINE + 1],
        L2R_DUI[250], L2R_PMID[SMALL_LINE + 1], L2R_MH[SMALL_LINE + 1],
        **rows, dui[250], mh[SMALL_LINE + 1], *foo, tmp[MAXLINE + 1],
        L2R_score[500];
   FILE *fpo, *fp;

   /* No point in doing if we don't have MTI results -- 
      updated 4/7/2017 to skip if only Humans from MTI
   */

   ok = TRUE;

   if((numFinalCTs + numFinalMHs) > 1)
     ok = TRUE;

   else if((numFinalCTs + numFinalMHs) == 1)
   {
       if(numFinalCTs == 1)
       {
           if(strcmp(finalCTlist[0].mh, "Humans") == 0)
             ok = FALSE;
       } /* fi */
   } /* else fi */

  else if((numFinalCTs + numFinalMHs) == 0)
     ok = FALSE;

   if(ok)
   {
       /* Build input file with text + MTI recommendations */

       sprintf(tmpFile, "/tmp/tl2r_%d\0", mypid);
       fpo = fopen(tmpFile, "w");

       fprintf(fpo, "%s\n", L2R_Citation);
       fprintf(fpo, "JID - %s\n", nlmID);
       if(SHOW_L2R)
         fprintf(fout, "L2R from MTI|%s|%s\nJID - %s\n", 
                 muid, L2R_Citation, nlmID);

       foundAnimals = FALSE;
       for(i = 0; i < numFinalCTs; i++)
       {
           if(strcmp(finalCTlist[i].mh, "Animals") == 0)
             foundAnimals = TRUE;

           finalCTlist[i].foundInL2R = FALSE;
           if(finalCTlist[i].oktoprint)
           {
               fprintf(fpo, "MTI|%s|%ld|%s|CT\n", finalCTlist[i].dui,
                       finalCTlist[i].score, finalCTlist[i].mh);

               if(SHOW_L2R)
                 fprintf(fout, "L2R from MTI|%s|MTI|%s|%ld|%s|CT\n", muid,
                         finalCTlist[i].dui, finalCTlist[i].score,
                         finalCTlist[i].mh);
           } /* fi */
       } /* for */

       foundDiseases = foundAnimalDiseases = haveCOVID19 = FALSE;
       diseasePos = -1;
       for(i = 0; i < numFinalMHs; i++)
       {
           finalMHlist[i].foundInL2R = FALSE;
           if(finalMHlist[i].oktoprint)
           {
                if(finalMHlist[i].datatype != NM)
                {
                    if((strstr(finalMHlist[i].trigger, "COVID-19") != NULL) || 
                       (strstr(finalMHlist[i].mh, "COVID-19") != NULL) ||
                       (strstr(finalMHlist[i].mh, "SARS-CoV-2") != NULL))
                      haveCOVID19 = TRUE;

                    for(l = 0; (l < finalMHlist[i].num_treecodes); l++)
                    {
                        if(finalMHlist[i].treecodes[l][0] == 'C')
                        {
                           if(strstr(finalMHlist[i].treecodes[l],"C22") != NULL)
                           {
                               if(strstr(finalMHlist[i].treecodes[l],"C22.674.710") == NULL)
                                 foundAnimalDiseases = TRUE;
                           } /* fi */

                           else
                             foundDiseases = TRUE;
                        } /* fi */

                       /* For purpose of adding/keeping Humans,
                          F03 considered diseases as well as 
                          the "C" tree.  9/13/2018.
                       */

                       else if(finalMHlist[i].treecodes[l][0] == 'F')
                       {
                           if(strstr(finalMHlist[i].treecodes[l],"F03") != NULL)
                             foundDiseases = TRUE;
                       } /* else fi */
                    } /* for each treecode */

                    if(finalMHlist[i].ETflag)
                    {
                       fprintf(fpo, "MTI|%s|%ld|%s|MH\n", finalMHlist[i].dui,
                               finalMHlist[i].score, finalMHlist[i].mh_orig);

                       if(SHOW_L2R || RTM_Debug)
                         fprintf(fout, "L2R from MTI|%s|MTI|%s|%ld|%s|MH\n",
                                 muid, finalMHlist[i].dui,
                                 finalMHlist[i].score, finalMHlist[i].mh_orig);
                    } /* fi */

                    else
                    {
                       fprintf(fpo, "MTI|%s|%ld|%s|MH\n", finalMHlist[i].dui,
                                finalMHlist[i].score, finalMHlist[i].mh);

                       if(SHOW_L2R || RTM_Debug)
                         fprintf(fout, "L2R from MTI|%s|MTI|%s|%ld|%s|MH\n",
                                 muid, finalMHlist[i].dui,
                                 finalMHlist[i].score, finalMHlist[i].mh);
                    } /* else */
                } /* fi !NM */
           } /* fi oktoprint */

           if((diseasePos == -1) && foundDiseases)
             diseasePos = i;
       } /* for */
       fclose(fpo);

       /* Loop up to 5 times in the event we see an error from L2R and
          change to a different random server after each attempt.  If we
          are successful, then just skip out of the loop.
       */

       numRepeat = 0;
       while(numRepeat < 1)
       {
           /* Identify which of the servers to use */

           strcpy(IL2R_host, DEFAULT_L2R_HOST);
           #ifdef RNDL2R_FILE
              fp = fopen(RNDL2R_FILE, "r+");
              if(fp != NULL)
              {
                  fgets(line, 250, fp);
                  rtn_random = -1;
                  nxt_random = 0;
                  sscanf(line, "%ld", &rtn_random);
                  if((rtn_random < 0) || (rtn_random >= numIMLhosts))
                    rtn_random = 0;

                  strcpy(IL2R_host, IMLHosts[rtn_random]);
                  nxt_random = rtn_random + 1;
                  if(nxt_random >= numIMLhosts)
                    nxt_random = 0;

                  rewind(fp);
                  fprintf(fp, "%ld\n", nxt_random);
                  fclose(fp);
              } /* fi */

              else
              {
                  fprintf(stderr, "ERROR - Unable to open RND L2R file: #%s#\n",
                          RNDL2R_FILE);
                  fflush(stderr);
              } /* else */

           #else

                  /* Randomize which L2R Host we will call */

                  seedval = time ((long int *) 0);
                  srand48(seedval);
                  rtn_random = GetNextIMLRand();

                  if(numIMLhosts > 1)
                  {
                      if((rtn_random > -1) && (rtn_random < numIMLhosts))
                        strcpy(IL2R_host, IMLHosts[rtn_random]);
                      else
                        strcpy(IL2R_host, DEFAULT_L2R_HOST);
                  } /* fi */
           #endif

           /* Now call Ilya's Learning 2 Rank server with the file */

           numResults = numLines = 0;
           sprintf(cmd, "%s/%s/runRClient.sh < %s\0",
                   L2R_PATH, IL2R_host, tmpFile);
           fp = popen(cmd, "r");
           if(fp != NULL)
           {
               while(fgets(line, SMALL_LINE, fp) != (char *)NULL)
               {
                   numLines++;
                   if(RTM_Debug)
                   {
                       fprintf(fout, "Line From L2R: #%s#\n", line);
                       fflush(fout);
                   } /* fi */

                   /* Handle any last year this year changes in MH */

                   foo = updateL2R(line);
                   strcpy(line, foo);
                   free(foo);

                   if(RTM_Debug)
                   {
                       fprintf(fout, 
                        "Line From L2R After Update: #%s#\n", line);
                       fflush(fout);
                   } /* fi */

                   ok = TRUE;

                   /* See if removing L2R scores with a minus sign helps */

                   strcpy(L2R_score, "");
                   sscanf(line, "%*[^|]|%*[^|]|%*[^|]|%[^\n]", L2R_score);

                   if(L2R_score[0] == '-')
                   {
                       ok = FALSE;
                       if(RTM_Debug)
                       {
                           fprintf(fout,
                               "Removing Line From L2R - neg score: #%s#\n",
                               L2R_score);
                           fflush(fout);
                       } /* fi */
                   } /* fi */

                   if(ok)
                   {
                       if(doIL2RHybrid)
                       {
                           if(MTI_AUTO)
                           {
                               if(shouldKeepL2R_MTI_AUTO(line, haveCOVID19))
                                L2R_lines[numResults++] = strdup(line);
                           } /* fi */

                           else
                           {
                               if(shouldKeepL2R(line, haveCOVID19))
                                 L2R_lines[numResults++] = strdup(line);
                           } /* else */
                       } /* fi doIL2RHybrid */

                       else
                         L2R_lines[numResults++] = strdup(line);
                   } /* fi ok */
               } /* while */
               pclose(fp);
           } /* fi */

           /* If we had lines back from L2R, but, ended up with no results
              because we filtered everything out, then no need to repeat
              the L2R request five times.
           */

           if((numLines > 0) && (numResults == 0))
             numRepeat = 10;

           /* Check that we actually received a result or just an error */

           if(numResults > 0)
           {
               foundERR = FALSE;
               for(i = 0; !foundERR && (i < numResults); i++)
               {
                   if((strstr(L2R_lines[i], 
                       "Error occurred during initialization of VM") != NULL) ||
                      (strstr(L2R_lines[i], 
                       "There is insufficient memory for the Java") != NULL) ||
                      (strstr(L2R_lines[i], 
                       "java.lang.OutOfMemoryError") != NULL))
                     foundERR = TRUE;
               } /* for */

               /* If we have an error, remove results, sleep, try again */

               if(foundERR)
               {
                   if(RTM_Debug)
                   {
                       fprintf(fout, "Found L2R error, removing results:\n");
                       fflush(fout);
                   } /* fi */

                   for(i = 0; i < numResults; i++)
                   {
                       if(RTM_Debug)
                       {
                           fprintf(fout, "L2R Removed: #%s#\n", L2R_lines[i]);
                           fflush(fout);
                       } /* fi */
                       free(L2R_lines[i]);
                   } /* for */

                   numResults = 0;
                   sleep(5);
               } /* fi foundERR */

               else
                 numRepeat = 10;
           } /* fi */

           numRepeat++;
       } /* while */

       unlink(tmpFile);

       if(numResults > 0)
       {
           for(i = 0; i < numResults; i++)
           {
               if(!BioASQ && TRUE)
               {
                   fprintf(fout, "L2R (%s): %s", IL2R_host, L2R_lines[i]);
                   fflush(fout);
               } /* fi */

               strcpy(L2R_DUI, "");
               strcpy(L2R_PMID, "");
               strcpy(L2R_MH, "");
               sscanf(L2R_lines[i], "%[^|]|%[^|]|%[^|]", 
                      L2R_PMID, L2R_MH, L2R_DUI);

               if(RTM_Debug)
               {
                   fprintf(fout,
                      "L2R_MH: #%s# - L2R_PMID: #%s# - L2R_DUI: #%s#\n",
                       L2R_MH, L2R_PMID, L2R_DUI);
                   fflush(fout);
               } /* fi */

               ok = TRUE;
               if(isBadL2R(L2R_MH, L2R_PMID))
                 ok = FALSE;

               if(ok && (strlen(L2R_DUI) > 0) && (strlen(L2R_PMID) > 0) &&
                  (strcmp(L2R_PMID, muid) == 0) && (strlen(L2R_MH) > 0))
               {
                   /* First things first, check to see if we need to replace
                      the DUI due to a MeSH change since we won't retrain
                      L2R until after the change-over.
                   */
/* */
                   foundQ = FALSE;
                   for(Q = 0; !foundQ && (Q < num_YEP_Replace); Q++)
                   {
                       if(L2R_DUI[4] == Replacements_YEP[Q].oldDUI[4])
                       {
                           if(strcmp(L2R_DUI, Replacements_YEP[Q].oldDUI) == 0)
                           {
                               foundQ = TRUE;
                               strcpy(L2R_DUI, Replacements_YEP[Q].newDUI);
                               if(RTM_Debug)
                               {
                                   fprintf(fout,
                                       "YEP L2R Replacement: #%s# to #%s#\n",
                                       Replacements_YEP[Q].oldDUI,
                                       Replacements_YEP[Q].newDUI);
                                   fflush(fout);
                               }
                           }
                       }
                   }

                   /* Let's see if we can find this L2R term

                      4/14/16 - If we find and term is turned off via
                          earlier filtering, make sure we turn it back on.

                      4/20/17 -
                      Do NOT resurrect any reg MHs for L2R with a position
                      higher than 0 - 8.  Still allow CTs to be resurrected.
                   */

                   found = FALSE;
                   for(j = 0; !found && (j < numFinalCTs); j++)
                   {
                       if(RTM_Debug)
                       {
                           fprintf(fout, "L2R CT Check: (%s|%s) <-> (%s|%s)\n",
                              L2R_DUI, L2R_MH, finalCTlist[j].dui,
                              finalCTlist[j].mh);
                           fflush(fout);
                       } /* fi */

                       if(strlen(finalCTlist[j].dui) == 0)
                       {
                           foo = findMHDUI(finalCTlist[j].mh);
                           if(finalCTlist[j].dui)
                             free(finalCTlist[j].dui);
                           finalCTlist[j].dui = strdup(foo);
                           free(foo);
                       } /* fi */

                       if(strcmp(L2R_DUI, finalCTlist[j].dui) == 0)
                       {
                           if(RTM_Debug)
                           {
                               fprintf(fout, " --- FOUND Match L2R\n");
                               fflush(fout);
                           } /* fi */

                           found = TRUE;

                           /* See if we have a CT only recommended by
                              Antonio's ML - don't want to resurrect it.
                           */
                           okCT = TRUE;
                           if(MTI_AUTO || ACCME || level1Filter)
                           {
                               if(!finalCTlist[j].oktoprint)
                               {
                                   if(strcmp(finalCTlist[j].trigger,
                                             "Antonio ML Rule Forced") == 0)
                                   {
                                       okCT = FALSE;
                                       if(RTM_Debug)
                                       {
                                           fprintf(fout, "Ignoring L2R");
                                           fprintf(fout, " only Antonio ML");
                                           fprintf(fout, ": #%s#\n",
                                               finalCTlist[j].mh);
                                           fflush(fout);
                                       } /* fi */
                                   } /* fi */
                               } /* fi */
                           } /* fi */

                           if(okCT)
                           {
                               finalCTlist[j].foundInL2R = TRUE;
                               if(!finalCTlist[j].oktoprint)
                               {
                                   finalCTlist[j].paths[MMI] = TRUE;
                                   finalCTlist[j].oktoprint = TRUE;
                                   strcpy(tmp, finalCTlist[j].trigger);
                                   if(strlen(tmp) > 0)
                                     strcat(tmp, "; ");
                                   strcat(tmp, "L2R Forced Term Resurrection");
                                   free(finalCTlist[j].trigger);
                                   finalCTlist[j].trigger = strdup(tmp);
                               } /* fi */
                           } /* fi okCT */
                       } /* fi */
                   } /* for */

                   if(RTM_Debug)
                   {
                       fprintf(fout, "L2R Found Status after CTs: %d\n", found);
                       fflush(fout);
                   } /* fi */

                   for(j = 0; !found && (j < numFinalMHs); j++)
                   {
                       if(RTM_Debug)
                       {
                           fprintf(fout, "L2R MH Check: (%s|%s) <-> (%s|%s)\n",
                              L2R_DUI, L2R_MH, finalMHlist[j].dui,
                              finalMHlist[j].mh);
                           fflush(fout);
                       } /* fi */

                       if(strlen(finalMHlist[j].dui) == 0)
                       {
                           if(finalMHlist[j].ETflag &&
                               (strlen(finalMHlist[j].mh_orig) > 0))
                             foo = findMHDUI(finalMHlist[j].mh_orig);
                           else
                             foo = findMHDUI(finalMHlist[j].mh);

                           if(finalMHlist[j].dui)
                             free(finalMHlist[j].dui);
                           finalMHlist[j].dui = strdup(foo);
                           free(foo);
                       } /* fi */

                       if(strcmp(L2R_DUI, finalMHlist[j].dui) == 0)
                       {
                           if(RTM_Debug)
                           {
                               fprintf(fout, " --- FOUND Match L2R\n");
                               fflush(fout);
                           } /* fi */

                           found = TRUE;
                           finalMHlist[j].foundInL2R = TRUE;
                           if(!finalMHlist[j].oktoprint && (i < 9))
                           {
                               finalMHlist[j].paths[MMI] = TRUE;
                               finalMHlist[j].oktoprint = TRUE;
                               strcpy(tmp, finalMHlist[j].trigger);
                               if(strlen(tmp) > 0)
                                 strcat(tmp, "; ");
                               strcat(tmp, "L2R Forced Term Resurrection");
                               free(finalMHlist[j].trigger);
                               finalMHlist[j].trigger = strdup(tmp);
                               if(finalMHlist[j].datatype == HM)
                                 finalMHlist[j].datatype = MH;
                           } /* fi */
                       } /* fi */
                   } /* for */

                   if(RTM_Debug)
                   {
                       fprintf(fout, "L2R Found Status after MHs: %d\n", found);
                       fflush(fout);
                   } /* fi */

                   /* If term is humans and !found, we want to ignore
                      since performance has been so bad to date. Starting
                      out just eliminating from MTI-AUTO citations.

                      April 24, 2017
                   */

                   if(!found && MTI_AUTO && (strcmp(L2R_MH, "humans") == 0))
                   {
                       found = TRUE;
                       if(RTM_Debug)
                       {
                           fprintf(fout, "Ignore L2R Humans: #%s#\n",
                                   L2R_lines[i]);
                           fflush(fout);
                       } /* !ok */
                   } /* fi */

                   isCT_L2R = isCTDUI(L2R_DUI);

                   /* Limit L2R Adds to the first 9 positions (0 - 8)

                      None L2R Only for Comment On per Susan (11/26)
                   */

                   ok_noCON = TRUE;
                   if(hasCON)
                     ok_noCON = FALSE;

                   if(ok_noCON && !found && ((i < 9) || isCT_L2R))
                   {
                       if(RTM_Debug)
                       {
                           fprintf(fout, "New L2R Term: #%s#", L2R_lines[i]);
                           fflush(fout);
                       } /* fi */

                       isAmbig = FALSE;
                       strcpy(ambigTrigger, "");
                       if((strcmp(L2R_MH, "accidental falls") == 0) || 
                          (strcmp(L2R_MH, "cross-sectional studies") == 0) ||
                          (strcmp(L2R_MH, "chickens") == 0) ||
                          (strcmp(L2R_MH, "seasons") == 0))
                       {
                           isAmbig = TRUE;
                           sprintf(ambigTrigger, "L2R Forced Term;AMBIG/METAPHOR30:%s\0", L2R_MH);
                       } /* fi */

                       n = 0;
                       rows = NULL;
                       query_btree(L2R_MHDUICUI, L2R_MH, &rows, &n);

                       if((rows != NULL) && (n > 0))
                       {
                           if(rows[0] != NULL)
                           {
                               strcpy(mh, "");
                               strcpy(dui, "");
                               sscanf(rows[0], "%[^|]|%[^|]", mh, dui);

                               if((strlen(mh) > 0) && (strlen(dui) > 0))
                               {
                                   if(RTM_Debug)
                                   {
                                       fprintf(fout, "Adding: #%s|%s#\n", mh, dui);
                                       fflush(fout);
                                   } /* fi */

                                   if(isCTDUI(dui))
                                   {
                                       p = numFinalCTs;
                                       finalCTlist[p].foundInL2R = TRUE;
                                       finalCTlist[p].wasForced = TRUE;
                                       finalCTlist[p].ETflag = FALSE;
                                       finalCTlist[p].datatype = CT;
                                       finalCTlist[p].oktoprint = TRUE;
                                       finalCTlist[p].score = 1000;
                                       finalCTlist[p].num_RCs = 0;
                                       finalCTlist[p].VocabDenFactor = 0;
                                       finalCTlist[p].origPos = 6000;

                                       finalCTlist[p].muid = strdup(muid);
                                       finalCTlist[p].mh = strdup(mh);
                                       finalCTlist[p].mh_orig = strdup("");
                                       finalCTlist[p].dui = strdup(dui);
                                       finalCTlist[p].entry_term = strdup("");
                                       finalCTlist[p].RC_PMIDs = strdup("");
                                       if(isAmbig)
                                         finalCTlist[p].trigger = strdup(ambigTrigger);
                                       else
                                         finalCTlist[p].trigger = strdup("L2R Forced Term");
                                       finalCTlist[p].textloc = strdup("");

                                       /* Positional Information */

                                       finalCTlist[p].numPIs = 0;

                                       /* Paths */

                                       finalCTlist[p].paths[MMI] = TRUE;

                                       /* Treecodes */

                                       finalCTlist[p].num_treecodes = 0;
                                       pullTREEs_CT(p);

                                       /* Semantic Types - max 25 */

                                       finalCTlist[p].num_STs = 0;

                                       numFinalCTs++;
                                   } /* fi CT */

                                   else
                                   {
                                       p = numFinalMHs;
                                       finalMHlist[p].foundInL2R = TRUE;
                                       finalMHlist[p].wasForced = TRUE;
                                       finalMHlist[p].ETflag = FALSE;
                                       finalMHlist[p].datatype = MH;
                                       finalMHlist[p].oktoprint = TRUE;
                                       finalMHlist[p].score = 1000;
                                       finalMHlist[p].num_RCs = 0;
                                       finalMHlist[p].VocabDenFactor = 0;
                                       finalMHlist[p].origPos = 6000;

                                       finalMHlist[p].muid = strdup(muid);
                                       finalMHlist[p].mh = strdup(mh);
                                       finalMHlist[p].mh_orig = strdup("");
                                       finalMHlist[p].dui = strdup(dui);
                                       finalMHlist[p].entry_term = strdup("");
                                       finalMHlist[p].RC_PMIDs = strdup("");
                                       if(isAmbig)
                                         finalMHlist[p].trigger = strdup(ambigTrigger);
                                       else
                                         finalMHlist[p].trigger = strdup("L2R Forced Term");
                                       finalMHlist[p].textloc = strdup("");

                                       /* Positional Information */

                                       finalMHlist[p].numPIs = 0;

                                       /* Paths */

                                       finalMHlist[p].paths[MMI] = TRUE;

                                       /* Treecodes */

                                       finalMHlist[p].num_treecodes = 0;
                                       pullTREEs(p);

                                       /* Semantic Types - max 25 */

                                       finalMHlist[p].num_STs = 0;

                                       numFinalMHs++;
                                   } /* else */
                               } /* fi */

                               else
                               {
                                   fprintf(stderr, "ERR - Bad MHDUICUI: #%s#\n",
                                           L2R_MH);
                                   fflush(stderr);
                               } /* else */
                           } /* fi */

                           /* Cleanup the rows */

                           for(k = 0; k < n; k++)
                           {
                               if(rows[k] != NULL )
                                 free(rows[k]);
                           } /* for */

                           if(rows != NULL)
                             free(rows);
                       } /* fi rows && n */
                   } /* fi */
               } /* fi */

               else if(ok)
               {
                   fprintf(stderr, "ERR - Bad L2R Line: #%s#\n", L2R_lines[i]);
                   fprintf(fout, "L2R_PMID: #%s# <-> muid #%s#\n",
                           L2R_PMID, muid);
                   fprintf(fout, "L2R_DUI: #%s#\n", L2R_DUI);
                   fflush(stderr);
               } /* else fi */

               /* Clean up storage */

               free(L2R_lines[i]);
           } /* for */

           /* Save any terms we want to force program to keep like 
              NM, HM, certain forced terms, etc.
           */

           if(doIL2RHybrid)
           {
               if(MTI_AUTO)
                 forcedKeepAfterL2R_MTI_AUTO();
               else
                 forcedKeepAfterL2R();
           } /* fi */

/* Force just ADDs for MHs 011117 */
           for(i = 0; i < numFinalMHs; i++)
              finalMHlist[i].foundInL2R = TRUE;
/* ----- End of ADDs change */

           /* Second chance for CheckTags where we used actual text to
              identify the term.

                   if(finalCTlist[i].wasForced &&
                       ((strstr(finalCTlist[i].textloc, "TI") != NULL) ||
                        (strstr(finalCTlist[i].trigger,
                                "CT Text Lookup") != NULL)))

               *** NOTE: CT Text Lookup removed since causing too many
                     false positives.  314/1101 5/3/2018

                   if(finalCTlist[i].wasForced &&
                       (strstr(finalCTlist[i].textloc, "TI") != NULL))

              ----------------------------------------------------------
   ** Removed again on 6/17/19 since while it does improve recall by a good
      amount (0.35), it also reduced precision by a greater amount (0.51).

                   if(finalCTlist[i].wasForced &&
                       ((strcmp(finalCTlist[i].mh, "Female") == 0) || 
                       (strcmp(finalCTlist[i].mh, "Male") == 0))) 
                   {
         *** Added 6/14/19 - we were missing a lot of these

                       if(strstr(finalCTlist[i].trigger,
                               "Antonio ML Rule Forced") != NULL)
                       {
                           finalCTlist[i].foundInL2R = TRUE;
                           if(RTM_Debug)
                           {
                               fprintf(fout,
                                   "L2R Saving CT #%s|%s|%s|%d# AML Found\n",
                                   finalCTlist[i].mh, finalCTlist[i].trigger,
                                   finalCTlist[i].textloc,
                                   finalCTlist[i].wasForced);
                               fflush(fout);
                           }
                       }
                   } 
           */

           for(i = 0; i < numFinalCTs; i++)
           {
               if(!finalCTlist[i].foundInL2R)
               {
                   if(finalCTlist[i].wasForced &&
                       (strstr(finalCTlist[i].textloc, "TI") != NULL))
                   {
                       finalCTlist[i].foundInL2R = TRUE;
                       if(RTM_Debug)
                       {
                           fprintf(fout,
                               "L2R Saving CT #%s|%s|%s|%d# TI Found\n",
                               finalCTlist[i].mh, finalCTlist[i].trigger,
                               finalCTlist[i].textloc,
                               finalCTlist[i].wasForced);
                           fflush(fout);
                       } /* fi */
                   } /* fi TI */

                   else if(finalCTlist[i].wasForced && foundDiseases && 
                       !veterinaryJournal && !foundAnimalDiseases &&
                       !foundAnimals &&
                       (strstr(finalCTlist[i].mh, "Humans") != NULL))
                   {
                       finalCTlist[i].foundInL2R = TRUE;
                       finalCTlist[i].oktoprint = TRUE;
                       if(RTM_Debug)
                       {
                           fprintf(fout,
                             "L2R Saving CT #%s|%s|%s|%d# Disease (%s) Found\n",
                             finalCTlist[i].mh, finalCTlist[i].trigger,
                             finalCTlist[i].textloc, finalCTlist[i].wasForced,
                             finalMHlist[diseasePos].mh);
                           fflush(fout);
                       } /* fi */
                   } /* else fi TI */

                   else if(finalCTlist[i].wasForced && !foundNonHumanTrigger &&
                        (strstr(finalCTlist[i].mh, "Humans") != NULL) &&
                        (strstr(finalCTlist[i].trigger, "CT Text Lookup") != NULL))
                   {
                       finalCTlist[i].foundInL2R = TRUE;
                       finalCTlist[i].oktoprint = TRUE;
                       if(RTM_Debug)
                       {
                           fprintf(fout,
                             "L2R Saving CT #%s|%s|%s|%d# Humans Found\n",
                             finalCTlist[i].mh, finalCTlist[i].trigger,
                             finalCTlist[i].textloc, finalCTlist[i].wasForced);
                           fflush(fout);
                       } /* fi */
                   } /* else fi TI */
               } /* fi */
           } /* for */

           /* Now, go through and turn off ones we didn't find.  Initially
              this will include the NM terms, but, for now that is ok.

              Added numResults > 1 check December 2017 after seeing a lot
              of L2R results being only Humans.
           */

           if(numResults > 1)
           {
               if(!goHigh)
               {
                   for(i = 0; i < numFinalCTs; i++)
                   {
                       if(!finalCTlist[i].foundInL2R)
                       {
                           finalCTlist[i].oktoprint = FALSE;
                           if(!BioASQ && RTM_Debug)
                           {
                               fprintf(fout,
                                "Removing CT #%s|%s|%s|%d#, not found in L2R\n",
                                finalCTlist[i].mh, finalCTlist[i].trigger,
                                finalCTlist[i].textloc,
                                finalCTlist[i].wasForced);
                               fflush(fout);
                           } /* fi */
                       } /* fi */
                   } /* for */
               } /* fi goHigh */

               /* Old In-Process -- keep MTI terms and just add L2R */

               if(MTI_AUTO)
               {
                   for(i = 0; i < numFinalMHs; i++)
                      finalMHlist[i].foundInL2R = TRUE;
               } /* fi MTI_AUTO */

               else
               {
                   for(i = 0; i < numFinalMHs; i++)
                   {
                       if(!finalMHlist[i].foundInL2R)
                       {
                           finalMHlist[i].oktoprint = FALSE;
                           if(!BioASQ && TRUE)
                           {
                               fprintf(fout,
                                   "Removing MH #%s#, not found in L2R\n",
                                   finalMHlist[i].mh);
                               fflush(fout);
                           } /* fi */
                       } /* fi */
                   } /* for */
               } /* else */
           } /* fi numResults > 1 */

       } /* fi numResults > 0 */
   } /* fi have MTI recommendations */
} /* doLearning2Rank */

/***************************************************************************
*
*  GetNextIMLRand --
*
*      This routine 
*
***************************************************************************/

int GetNextIMLRand()
{
   int rtn = -1;
   long pos;

   while(rtn == -1)
   {
       pos = lrand48() % numIMLhosts;
       if(pos >= numIMLhosts)
         rtn = -1;
       else
         rtn = pos;
   } /* while */

   return(rtn);
} /* GetNextIMLRand */


/***************************************************************************
*
*  checkZika_2016 --
*
*      This 
*
***************************************************************************/

void checkZika_2016()
{
   int foundZika, foundZVI;
   long i, p, len;
   char *tmp;

   /* 
      Zika Virus Infection|D000071243|C0276289
      Zika Virus|D000071244|C0318793
   */

   if(foundInText("zika", FALSE, FALSE) ||
      foundInText("zikv", FALSE, FALSE))
   {
       if(RTM_Debug)
       {
           fprintf(fout, "\nForced Zika Terms Check\n");
           fflush(fout);
       } /* fi RTM_Debug */

       foundZika = foundZVI = FALSE;

       for(i = 0; i < numFinalMHs; i++)
       {
           if(strcmp(finalMHlist[i].dui, "D000071243") == 0)
           {
               foundZVI = TRUE;
               if(!finalMHlist[i].oktoprint)
               {
                   if(RTM_Debug)
                   {
                       fprintf(fout, "Reinstating Zika Virus Infection\n");
                       fflush(fout);
                   } /* fi RTM_Debug */
                   finalMHlist[i].oktoprint = TRUE;
                   len = (long)strlen(finalMHlist[i].trigger);
                   tmp = (char *)malloc(len + 1000);
                   if(len > 0)
                     sprintf(tmp, "%s; Zika Lookup\0", finalMHlist[i].trigger);
                   else
                     strcpy(tmp, "Zika Lookup");
                   free(finalMHlist[i].trigger);
                   finalMHlist[i].trigger = strdup(tmp);
                   free(tmp);
               } /* fi */
           } /* fi */

           else if(strcmp(finalMHlist[i].dui, "D000071244") == 0)
           {
               foundZika = TRUE;
               if(!finalMHlist[i].oktoprint)
               {
                   if(RTM_Debug)
                   {
                       fprintf(fout, "Reinstating Zika Virus\n");
                       fflush(fout);
                   } /* fi RTM_Debug */
                   finalMHlist[i].oktoprint = TRUE;
                   len = (long)strlen(finalMHlist[i].trigger);
                   tmp = (char *)malloc(len + 1000);
                   if(len > 0)
                     sprintf(tmp, "%s; Zika Lookup\0", finalMHlist[i].trigger);
                   else
                     strcpy(tmp, "Zika Lookup");
                   free(finalMHlist[i].trigger);
                   finalMHlist[i].trigger = strdup(tmp);
                   free(tmp);
               } /* fi */
           } /* else fi */
       } /* for */

       /* If we don't find one or the other term, add them */

       if(!foundZika)
       {
           if(RTM_Debug)
           {
               fprintf(fout, "Adding Zika Virus\n");
               fflush(fout);
           } /* fi RTM_Debug */
           p = numFinalMHs;
           finalMHlist[p].foundInL2R = TRUE;
           finalMHlist[p].ETflag = FALSE;
           finalMHlist[p].datatype = MH;
           finalMHlist[p].oktoprint = TRUE;
           finalMHlist[p].score = 1000;
           finalMHlist[p].num_RCs = 0;
           finalMHlist[p].VocabDenFactor = 0;
           finalMHlist[p].origPos = 8000;

           finalMHlist[p].muid = strdup(muid);
           finalMHlist[p].mh = strdup("Zika Virus");
           finalMHlist[p].mh_orig = strdup("");
           finalMHlist[p].dui = strdup("D000071244");
           finalMHlist[p].entry_term = strdup("");
           finalMHlist[p].RC_PMIDs = strdup("");
           finalMHlist[p].trigger = strdup("Zika Lookup");
           finalMHlist[p].textloc = strdup("");

           /* Positional Information */

           finalMHlist[p].numPIs = 0;

           /* Paths */

           finalMHlist[p].paths[MMI] = TRUE;

           /* Treecodes */

           finalMHlist[p].num_treecodes = 0;
           pullTREEs(p);

           /* Semantic Types - max 25 */

           finalMHlist[p].num_STs = 0;

           numFinalMHs++;
       } /* fi */

       if(!foundZVI)
       {
           if(RTM_Debug)
           {
               fprintf(fout, "Adding Zika Virus Infection\n");
               fflush(fout);
           } /* fi RTM_Debug */
           p = numFinalMHs;
           finalMHlist[p].foundInL2R = TRUE;
           finalMHlist[p].ETflag = FALSE;
           finalMHlist[p].datatype = MH;
           finalMHlist[p].oktoprint = TRUE;
           finalMHlist[p].score = 1000;
           finalMHlist[p].num_RCs = 0;
           finalMHlist[p].VocabDenFactor = 0;
           finalMHlist[p].origPos = 8000;

           finalMHlist[p].muid = strdup(muid);
           finalMHlist[p].mh = strdup("Zika Virus Infection");
           finalMHlist[p].mh_orig = strdup("");
           finalMHlist[p].dui = strdup("D000071243");
           finalMHlist[p].entry_term = strdup("");
           finalMHlist[p].RC_PMIDs = strdup("");
           finalMHlist[p].trigger = strdup("Zika Lookup");
           finalMHlist[p].textloc = strdup("");

           /* Positional Information */

           finalMHlist[p].numPIs = 0;

           /* Paths */

           finalMHlist[p].paths[MMI] = TRUE;

           /* Treecodes */

           finalMHlist[p].num_treecodes = 0;
           pullTREEs(p);

           /* Semantic Types - max 25 */

           finalMHlist[p].num_STs = 0;

           numFinalMHs++;
       } /* fi */
   } /* fi found "zika" */
} /* checkZika_2016 */

/***************************************************************************
*
*  forcedKeepAfterL2R --
*
*      This routine 
*
***************************************************************************/

void forcedKeepAfterL2R()
{
   int found_L2R_HM;
   long i, j;

   for(i = 0; i < numFinalMHs; i++)
   {
       if(finalMHlist[i].oktoprint)
       {

           if(!finalMHlist[i].foundInL2R)
           {
               /* Want to make sure we keep the NM/HM terms where
                  L2R has kept one or more of the HMs.  If came from
                  the Forced SCR Leaf Node Lookup keep as well (94.78%).
               */

               if(goHigh)
               {
                   if((finalMHlist[i].datatype == NM) ||
                      (finalMHlist[i].datatype == HM))
                     finalMHlist[i].foundInL2R = TRUE;
               } /* fi */

               else
               {
                   if(finalMHlist[i].datatype == NM)
                   {
                       found_L2R_HM = FALSE;
                       if(strstr(finalMHlist[i].trigger,
                             "Forced SCR Leaf Node Lookup") != NULL)
                         found_L2R_HM = TRUE;

                       for(j = 0; !found_L2R_HM &&
                                 (j < finalMHlist[i].num_HMs); j++)
                       {
                           if(find_SCR_HM(finalMHlist[i].HMs[j]))
                             found_L2R_HM = TRUE;
                       } /* for */

                       if(found_L2R_HM)
                       {
                           finalMHlist[i].foundInL2R = TRUE;
                           for(j = 0; j < finalMHlist[i].num_HMs; j++)
                             save_SCR_HM(finalMHlist[i].HMs[j]);
                       } /* fi */
                   } /* fi */
               } /* else */

               /* Keep Randomized Controlled Trials as Topic,
                  Patents as Topic, Advertising as Topic -- any forced
                  as Topic terms we are confident about.
               */

               if(strstr(finalMHlist[i].trigger, "Forced as Topic") != NULL)
                 finalMHlist[i].foundInL2R = TRUE;

               /* Save certain Forced rules */

               else if((strstr(finalMHlist[i].trigger,
                          "Forced GENBANK SI Entry") != NULL) ||
                       (strstr(finalMHlist[i].trigger,
                          "Forced Surgical Wound Infection Entry") != NULL) ||
                       (strstr(finalMHlist[i].trigger,
                          "Forced Leaf Node Lookup") != NULL) ||
                       (strstr(finalMHlist[i].trigger,
                          "Forced Lookup") != NULL) ||
                       (strstr(finalMHlist[i].trigger,
                          "Forced New Lookup") != NULL) ||
                       (strstr(finalMHlist[i].trigger,
                          "Forced Animals Added") != NULL) ||
                       (strstr(finalMHlist[i].trigger,
                          "Forced Female Added (Neoplasms)") != NULL) ||
                       (strstr(finalMHlist[i].trigger,
                          "Forced Pregnancy Added (Neoplasms)") != NULL) ||
                       (strstr(finalMHlist[i].trigger,
                          "Spores Specifics") != NULL) ||
                       (strstr(finalMHlist[i].trigger,
                          "Medical Tourism Lookup") != NULL) ||
                       (strstr(finalMHlist[i].trigger,
                          "Forced SCR Leaf Node Lookup") != NULL) ||
                       (strstr(finalMHlist[i].trigger,
                          "Forced - Special Journal Add") != NULL) ||
                       (strstr(finalMHlist[i].trigger,
                          "Heuristic #N") != NULL))
                 finalMHlist[i].foundInL2R = TRUE;
           } /* !foundInL2R */
       } /* fi */
   } /* for */

   /* Want to SAVE the following list of CTs since  the Baseline better

	          Cats D002415
	          Cattle	D002417
                  Chlorocebus aethiops D002522
	          Cricetinae D006224
                  Dogs D004285
	          Guinea Pigs D006168
                  Horses D006736
                  Mice|D051379
	          Pregnancy D011247
	          Rabbits	D011817
	          Rats D051381
	          Sheep D012756
	          Swine D013552
                  Young Adult D055815
   */

   for(i = 0; i < numFinalCTs; i++)
   {
       if(finalCTlist[i].oktoprint)
       {
           if(!finalCTlist[i].foundInL2R)
           {
               if((strcmp(finalCTlist[i].dui, "D002415") == 0) ||
                  (strcmp(finalCTlist[i].dui, "D002417") == 0) ||
                  (strcmp(finalCTlist[i].dui, "D002522") == 0) ||
                  (strcmp(finalCTlist[i].dui, "D006224") == 0) ||
                  (strcmp(finalCTlist[i].dui, "D004285") == 0) ||
                  (strcmp(finalCTlist[i].dui, "D006168") == 0) ||
                  (strcmp(finalCTlist[i].dui, "D006736") == 0) ||
                  (strcmp(finalCTlist[i].dui, "D051379") == 0) ||
                  (strcmp(finalCTlist[i].dui, "D011247") == 0) ||
                  (strcmp(finalCTlist[i].dui, "D011817") == 0) ||
                  (strcmp(finalCTlist[i].dui, "D051381") == 0) ||
                  (strcmp(finalCTlist[i].dui, "D012756") == 0) ||
                  (strcmp(finalCTlist[i].dui, "D013552") == 0) ||
                  (strcmp(finalCTlist[i].dui, "D055815") == 0))
                 finalCTlist[i].foundInL2R = TRUE;

               /* Keep History, 20th Century from baseline */

               else if(strcmp(finalCTlist[i].dui, "D049673") == 0)
                 finalCTlist[i].foundInL2R = TRUE;
           } /* fi */
       } /* fi */
   } /* for */

   /* Want to SAVE Female if we have something from the text. */

   for(i = 0; i < numFinalCTs; i++)
   {
       if(finalCTlist[i].oktoprint)
       {
           if(finalCTlist[i].mh[0] == 'F')
           {
               if((strcmp(finalCTlist[i].mh, "Female") == 0) &&
                  (strstr(finalCTlist[i].trigger, "CT Text Lookup:") != NULL))
                 finalCTlist[i].foundInL2R = TRUE;
           } /* fi */
       } /* fi */
   } /* for */
} /* forcedKeepAfterL2R */

/***************************************************************************
*
*  forcedKeepAfterL2R_MTI_AUTO --
*
*      This routine 
*
***************************************************************************/

void forcedKeepAfterL2R_MTI_AUTO()
{
   int found_L2R_HM;
   long i, j;

   for(i = 0; i < numFinalMHs; i++)
   {
       if(finalMHlist[i].oktoprint)
       {

           if(!finalMHlist[i].foundInL2R)
           {
               /* Want to make sure we keep the NM/HM terms where
                  L2R has kept one or more of the HMs.  If came from
                  the Forced SCR Leaf Node Lookup keep as well (94.78%).
               */

               if(goHigh)
               {
                   if((finalMHlist[i].datatype == NM) ||
                      (finalMHlist[i].datatype == HM))
                     finalMHlist[i].foundInL2R = TRUE;
               } /* fi */

               else
               {
                   if(finalMHlist[i].datatype == NM)
                   {
                       found_L2R_HM = FALSE;
                       if(strstr(finalMHlist[i].trigger,
                             "Forced SCR Leaf Node Lookup") != NULL)
                         found_L2R_HM = TRUE;

                       for(j = 0; !found_L2R_HM &&
                                 (j < finalMHlist[i].num_HMs); j++)
                       {
                           if(find_SCR_HM(finalMHlist[i].HMs[j]))
                             found_L2R_HM = TRUE;
                       } /* for */

                       if(found_L2R_HM)
                       {
                           finalMHlist[i].foundInL2R = TRUE;
                           for(j = 0; j < finalMHlist[i].num_HMs; j++)
                             save_SCR_HM(finalMHlist[i].HMs[j]);
                       } /* fi */
                   } /* fi */
               } /* else */

               /* Keep Randomized Controlled Trials as Topic,
                  Patents as Topic, Advertising as Topic -- any forced
                  as Topic terms we are confident about.
               */

               if(strstr(finalMHlist[i].trigger, "Forced as Topic") != NULL)
                 finalMHlist[i].foundInL2R = TRUE;

               /* Save certain Forced rules */

               else if((strstr(finalMHlist[i].trigger,
                          "Forced GENBANK SI Entry") != NULL) ||
                       (strstr(finalMHlist[i].trigger,
                          "Forced Surgical Wound Infection Entry") != NULL) ||
                       (strstr(finalMHlist[i].trigger,
                          "Forced Leaf Node Lookup") != NULL) ||
                       (strstr(finalMHlist[i].trigger,
                          "Forced Lookup") != NULL) ||
                       (strstr(finalMHlist[i].trigger,
                          "Forced New Lookup") != NULL) ||
                       (strstr(finalMHlist[i].trigger,
                          "Forced Animals Added") != NULL) ||
                       (strstr(finalMHlist[i].trigger,
                          "Forced Female Added (Neoplasms)") != NULL) ||
                       (strstr(finalMHlist[i].trigger,
                          "Forced Pregnancy Added (Neoplasms)") != NULL) ||
                       (strstr(finalMHlist[i].trigger,
                          "Spores Specifics") != NULL) ||
                       (strstr(finalMHlist[i].trigger,
                          "Medical Tourism Lookup") != NULL) ||
                       (strstr(finalMHlist[i].trigger,
                          "Forced SCR Leaf Node Lookup") != NULL) ||
                       (strstr(finalMHlist[i].trigger,
                          "Forced - Special Journal Add") != NULL) ||
                       (strstr(finalMHlist[i].trigger,
                          "Heuristic #N") != NULL))
                 finalMHlist[i].foundInL2R = TRUE;
           } /* !foundInL2R */
       } /* fi */
   } /* for */

   /* Want to SAVE all of the CTs for MTI-AUTO that we are not relying
      on L2R for.
   */

   for(i = 0; i < numFinalCTs; i++)
   {
       if(finalCTlist[i].oktoprint)
       {
           if((finalCTlist[i].mh[0] == 'B') || (finalCTlist[i].mh[0] == 'H') ||
              (finalCTlist[i].mh[0] == 'I') || (finalCTlist[i].mh[0] == 'U'))
           {
               if((strcmp(finalCTlist[i].mh, "Bees") != 0) &&
                  (strncmp(finalCTlist[i].mh, "History, ", 9) != 0) &&
                  (strcmp(finalCTlist[i].mh, "Infant") != 0) &&
                  (strcmp(finalCTlist[i].mh, "United States") != 0))
                 finalCTlist[i].foundInL2R = TRUE;
           } /* fi */

           else
             finalCTlist[i].foundInL2R = TRUE;
       } /* fi */
   } /* for */

   /* Want to SAVE Female if we have something from the text. */

   for(i = 0; i < numFinalCTs; i++)
   {
       if(finalCTlist[i].oktoprint)
       {
           if(finalCTlist[i].mh[0] == 'F')
           {
               if((strcmp(finalCTlist[i].mh, "Female") == 0) &&
                  (strstr(finalCTlist[i].trigger, "CT Text Lookup:") != NULL))
                 finalCTlist[i].foundInL2R = TRUE;
           } /* fi */
       } /* fi */
   } /* for */
} /* forcedKeepAfterL2R_MTI_AUTO */

/***************************************************************************
*
*  shouldKeepL2R --
*
*      This routine 
*
***************************************************************************/

int shouldKeepL2R(char *inL2R, int haveCOVID19)
{
   int rtn;
   char term[SMALL_LINE + 1], dui[250];

   rtn = TRUE;

   strcpy(term, "");
   strcpy(dui, "");
   sscanf(inL2R, "%*[^|]|%[^|]|%[^|]", term, dui);

   /* Want to use certain as Topic terms, but, not others */

   if(strstr(inL2R, "as topic") != NULL)
   {
       rtn = FALSE;

       if((strcmp(term, "anecdotes as topic") == 0) ||
          (strcmp(term, "legislation as topic") == 0))
        rtn = FALSE;
   } /* fi */

   else if(isCTDUI(dui))
   {
       /* Want to remove the following list of CTs since the Baseline better

        Cats D002415
        Cattle	D002417
        Chlorocebus aethiops D002522
        Cricetinae D006224
        Dogs D004285
        Guinea Pigs D006168
        Horses D006736
        Pregnancy D011247
        Rabbits	D011817
        Rats D051381
        Sheep D012756
        Swine D013552
        Young Adult D055815
      */

      if((strcmp(dui, "D002415") == 0) ||
         (strcmp(dui, "D002417") == 0) ||
         (strcmp(dui, "D002522") == 0) ||
         (strcmp(dui, "D006224") == 0) ||
         (strcmp(dui, "D004285") == 0) ||
         (strcmp(dui, "D006168") == 0) ||
         (strcmp(dui, "D006736") == 0) ||
         (strcmp(dui, "D011247") == 0) ||
         (strcmp(dui, "D011817") == 0) ||
         (strcmp(dui, "D051381") == 0) ||
         (strcmp(dui, "D012756") == 0) ||
         (strcmp(dui, "D013552") == 0) ||
         (strcmp(dui, "D055815") == 0))
        rtn = FALSE;

        /* Want to keep the following list of CTs since the L2R is better

          Bees
          History, 15th Century
          History, 16th Century
          History, 17th Century
          History, 18th Century
          History, 19th Century
          History, 20th Century
          History, 21st Century
          History, Ancient
          History, Medieval
          Infant
          United States
       */

       if((strcmp(term, "bees") == 0) ||
          (strncmp(term, "history, ", 9) == 0) ||
          (strcmp(term, "infant") == 0) ||
          (strcmp(term, "united states") == 0))
        rtn = TRUE;

   } /* else */

   /* Some terms we don't want L2R for - */

   else if((strcmp(dui, "D004772") == 0) ||
           (strcmp(dui, "D004532") == 0) ||
           (strcmp(dui, "D010361") == 0) ||
           (strcmp(dui, "D014930") == 0))
     rtn= FALSE;

   if(goHigh)
    rtn = TRUE;

    /*    2021 - If we are recommending COVID-19 terms then if any of these terms are only
                 from PRC, remove them.
          Pneumonia, Viral|D011024
          Coronavirus Infections|D018352
          Betacoronavirus|D000073640
          Pandemics|D058873
    */

   if(haveCOVID19)
   {
       if((strcmp(dui, "D011024") == 0) ||
           (strcmp(dui, "D018352") == 0) ||
           (strcmp(dui, "D000073640") == 0) ||
           (strcmp(dui, "D058873") == 0))
        rtn= FALSE;
   } /* fi */

   if(!rtn && RTM_Debug)
   {
       fprintf(fout, "L2R - Ignoring L2R Line: #%s|%s#\n", term, dui);
       fflush(fout);
   } /* fi */

   return(rtn);
} /* shouldKeepL2R */

/***************************************************************************
*
*  shouldKeepL2R_MTI_AUTO --
*
*      This routine 
*
***************************************************************************/

int shouldKeepL2R_MTI_AUTO(char *inL2R, int haveCOVID19)
{
   int rtn;
   char term[SMALL_LINE + 1], dui[250];

   rtn = TRUE;

   strcpy(term, "");
   strcpy(dui, "");
   sscanf(inL2R, "%*[^|]|%[^|]|%[^|]", term, dui);

   /* Want to use certain as Topic terms, but, not others */

   if(strstr(inL2R, "as topic") != NULL)
   {
       rtn = FALSE;

       if((strcmp(term, "atlases as topic") == 0) ||
          (strcmp(term, "bibliography as topic") == 0) ||
          (strcmp(term, "clinical trials, phase iii as topic") == 0) ||
          (strcmp(term, "controlled clinical trials as topic") == 0) ||
          (strcmp(term, "cookbooks as topic") == 0) ||
          (strcmp(term, "correspondence as topic") == 0) ||
          (strcmp(term, "databases as topic") == 0) ||
          (strcmp(term, "dictionaries as topic") == 0) ||
          (strcmp(term, "dissertations, academic as topic") == 0) ||
          (strcmp(term, "manuscripts as topic") == 0) ||
          (strcmp(term, "motion pictures as topic") == 0) ||
          (strcmp(term, "newspapers as topic") == 0) ||
          (strcmp(term, "portraits as topic") == 0) ||
          (strcmp(term, "textbooks as topic") == 0) ||
          (strcmp(term, "webcasts as topic") == 0) ||
          (strcmp(term, "congresses as topic") == 0) ||
          (strcmp(term, "clinical trials, phase ii as topic") == 0) ||
          (strcmp(term, "patents as topic") == 0) ||
          (strcmp(term, "poetry as topic") == 0) ||
          (strcmp(term, "meta-analysis as topic") == 0) ||
          (strcmp(term, "periodicals as topic") == 0) ||
          (strcmp(term, "wit and humor as topic") == 0) ||
          (strcmp(term, "terminology as topic") == 0) ||
          (strcmp(term, "practice guidelines as topic") == 0) ||
          (strcmp(term, "clinical trials, phase i as topic") == 0) ||
          (strcmp(term, "clinical trials as topic") == 0) ||
          (strcmp(term, "review literature as topic") == 0) ||
          (strcmp(term, "statistics as topic") == 0) ||
          (strcmp(term, "interviews as topic") == 0) ||
          (strcmp(term, "advertising as topic") == 0) ||
          (strcmp(term, "patient education as topic") == 0))
        rtn = TRUE;
   } /* fi */

   else if(isCTDUI(dui))
   {
       rtn = FALSE;

        /* Want to keep the following list of CTs since the L2R is better

          Bees
          History, 15th Century
          History, 16th Century
          History, 17th Century
          History, 18th Century
          History, 19th Century
          History, 20th Century
          History, 21st Century
          History, Ancient
          History, Medieval
          Infant
          United States
       */

       if((strcmp(term, "bees") == 0) ||
          (strncmp(term, "history, ", 9) == 0) ||
          (strcmp(term, "infant") == 0) ||
          (strcmp(term, "united states") == 0))
        rtn = TRUE;
   } /* else */

    /*    2021 - If we are recommending COVID-19 terms then if any of these terms are only
                 from PRC, remove them.
          Pneumonia, Viral|D011024
          Coronavirus Infections|D018352
          Betacoronavirus|D000073640
          Pandemics|D058873
    */

   if(haveCOVID19)
   {
       if((strcmp(dui, "D011024") == 0) ||
           (strcmp(dui, "D018352") == 0) ||
           (strcmp(dui, "D000073640") == 0) ||
           (strcmp(dui, "D058873") == 0))
        rtn= FALSE;
   } /* fi */

   if(!rtn && RTM_Debug)
   {
       fprintf(fout, "L2R - Ignoring L2R Line: #%s|%s#\n", term, dui);
       fflush(fout);
   } /* fi */

   return(rtn);
} /* shouldKeepL2R_MTI_AUTO */

/***************************************************************************
*
*  find_SCR_HM --
*
*      This 
*
***************************************************************************/

int find_SCR_HM(char *mh)
{
    int rtn, found;
    long i;

    rtn = FALSE;
    found = FALSE;
    for(i = 0; !found && (i < numFinalMHs); i++)
    {
         if(mh[0] == finalMHlist[i].mh[0])
         {
             if(strcmp(mh, finalMHlist[i].mh) == 0)
             {
                 found = TRUE;
                 if(finalMHlist[i].oktoprint && finalMHlist[i].foundInL2R)
                   rtn = TRUE;
             } /* fi */
         } /* fi */

         if(!found && finalMHlist[i].ETflag)
         {
             if(mh[0] == finalMHlist[i].mh_orig[0])
             {
                 if(strcmp(mh, finalMHlist[i].mh_orig) == 0)
                 {
                     found = TRUE;
                     if(finalMHlist[i].oktoprint && finalMHlist[i].foundInL2R)
                       rtn = TRUE;
                 } /* fi */
             } /* fi */
         } /* fi */
    } /* for */

    return(rtn);
} /* find_SCR_HM */

/***************************************************************************
*
*  save_SCR_HM --
*
*      This 
*
***************************************************************************/

void save_SCR_HM(char *mh)
{
    int found;
    long i;

    found = FALSE;
    for(i = 0; !found && (i < numFinalMHs); i++)
    {
         if(mh[0] == finalMHlist[i].mh[0])
         {
             if(strcmp(mh, finalMHlist[i].mh) == 0)
             {
                 found = TRUE;
                 finalMHlist[i].foundInL2R = TRUE;
             } /* fi */
         } /* fi */

         if(!found && finalMHlist[i].ETflag)
         {
             if(mh[0] == finalMHlist[i].mh_orig[0])
             {
                 if(strcmp(mh, finalMHlist[i].mh_orig) == 0)
                 {
                     found = TRUE;
                     finalMHlist[i].foundInL2R = TRUE;
                 } /* fi */
             } /* fi */
         } /* fi */
    } /* for */
} /* save_SCR_HM */

/***************************************************************************
*
*  isCTDUI --
*
*      This 
*
***************************************************************************/

int isCTDUI(char *dui)
{
    int rtn;
    long i;

    rtn = FALSE;
    for(i = 0; !rtn && (i < num_ctdui); i++)
    {
         if(dui[3] == CT_DUI_list[i].dui[3])
         {
             if(strcmp(dui, CT_DUI_list[i].dui) == 0)
               rtn = TRUE;
         } /* fi */
    } /* for */

    return(rtn);
} /* isCTDUI */


/***************************************************************************
*
*  checkForAgeYears --
*
*      This 
*
***************************************************************************/

void checkForAgeYears()
{
   int foundHumans, foundRange, found, okFoundAges, ok, done, loopDone, foundAge_CTs;
   long i, j, x, p, tPos, foundPos, diff, diffR, diffR2, len, lenT, pos,
        strLen, tLen, bornYear, ageDiff, currYear, foundLen, posJ;
   float k;
   char *lcCit, *snippet, *foo, *fooR, *fooR2, *yearStr, *fooB,
        from[SMALL_LINE + 1], *tmp, *citSpan, *snippSD, *origSnippet,
        inParens[SMALL_LINE + 1],  betweenAnd[SMALL_LINE + 1],
        tmpCheck[SMALL_LINE + 1];
   struct tm *tmstruct;
   time_t now;

   Load_Repls();
   entered_checkForAgeYears = TRUE;
   begYear = endYear = -1.0;
   begSpan = endSpan = -1;
   foundHumans = okFoundAges =foundRange = foundAge_CTs = FALSE;
   strcpy(inParens, "");
   strcpy(betweenAnd, "");
   for(i = 0; i < numAgeCTs; i++)
      Age_CT_list[i].found = FALSE;

   for(i = 0; i < numFinalCTs; i++)
   {
       if(finalCTlist[i].oktoprint)
       {
           if(strcmp(finalCTlist[i].mh, "Humans") == 0)
              foundHumans = TRUE;

           found = FALSE;
           for(j = 0; !found && (j < numAgeCTs); j++)
           {
               if(finalCTlist[i].mh[0] == Age_CT_list[j].Name[0])
               {
                   if(strcmp(finalCTlist[i].mh, Age_CT_list[j].Name) == 0)
                   {
                       if(RTM_Debug)
                       {
                           fprintf(fout, "Age CT already in list: #%s#\n",
                                   Age_CT_list[j].Name);
                           fflush(fout);
                       } /* fi */
                       found = foundAge_CTs = TRUE;
                       Age_CT_list[j].inListAlready = TRUE;
                       Age_CT_list[j].listPos = i;
                   } /* fi */
               } /* fi */
           } /* for */
       } /* fi valid CT */
   } /* for */

   /* IF we have any of the age related terms but Humans was turned off for whatever reason,
      we need to go back through and turn off the age related terms.
   */

   if(RTM_Debug)
   {
       fprintf(fout, "Found Humans: %d -- Found Age CTs?: %d\n", foundHumans, foundAge_CTs);
       fflush(fout);
   } /* fi */

   if(!foundHumans && foundAge_CTs)
   {
       for(i = 0; i < numFinalCTs; i++)
       {
           if(finalCTlist[i].oktoprint)
           {
               found = FALSE;
               for(j = 0; !found && (j < numAgeCTs); j++)
               {
                   if(finalCTlist[i].mh[0] == Age_CT_list[j].Name[0])
                   {
                       if(strcmp(finalCTlist[i].mh, Age_CT_list[j].Name) == 0)
                       {
                           if(RTM_Debug)
                           {
                               fprintf(fout, "Removing Age CT - no Humans: #%s#\n",
                                       Age_CT_list[j].Name);
                               fflush(fout);
                           } /* fi */
                           found = TRUE;
                           finalCTlist[i].oktoprint = FALSE;
                       } /* fi */
                   } /* fi */
               } /* for */
           } /* fi valid CT */
       } /* for */
   } /* fi */

   /* Going to ignore if not already set to show humans.

      If we have Humans - make sure to remove certain bad triggers for Ages
      before we begin so that we don't have any spurious ages triggered.
        AGEs is the most frequent
   */

   if(foundHumans)
   {
       lcCit = (char *)strdup(citation);
       foo = XoutTriggerInText("AGES", lcCit);
       free(lcCit);
       lcCit = strdup(foo);
       free(foo);

       foo = XoutTriggerInText("AGEs", lcCit);
       free(lcCit);
       lcCit = strdup(foo);
       free(foo);

       for(i = 0; i < cit_len; i++)
       {
           if(isupper(lcCit[i]))
             lcCit[i] = tolower(lcCit[i]);
       } /* for */
       ageCitLen = cit_len;

       /* List of Age Ignores in agelookFor.h */

       for(i = 0; i < numAgeIgnores; i++)
       {
           foo = XoutTriggerInText(ageIgnores[i], lcCit);
           free(lcCit);
           lcCit = strdup(foo);
           free(foo);
       } /* for */

       /* Go through and Xout possible age/animal related triggers -
          want to get rid of things like "elderly cats".
       */

       for(i = 0; i < numCheckForAnimals; i++)
       {
           for(j = 0; j < numSpecial_Aged; j++)
           {
              sprintf(tmpCheck, "%s %s\0", Special_Aged_Lookups[j].lookFor,
                      checkForAnimals[i]);

              foo = XoutTriggerInText(tmpCheck, lcCit);
              free(lcCit);
              lcCit = strdup(foo);
              free(foo);
           } /* for */
       } /* for */

       /* Xout non-adult & non adult */

       sprintf(tmpCheck, "non-adult");
       foo = XoutTriggerInText(tmpCheck, lcCit);
       free(lcCit);
       lcCit = strdup(foo);
       free(foo);

       sprintf(tmpCheck, "non adult");
       foo = XoutTriggerInText(tmpCheck, lcCit);
       free(lcCit);
       lcCit = strdup(foo);
       free(foo);

       /* Remove any percentages prior to looking for ages (age groups: 7.7%) */

       foo = XoutPercentages(lcCit);
       free(lcCit);
       lcCit = strdup(foo);
       free(foo);

       if(RTM_Debug)
       {
           fprintf(fout, "Age Range (Humans Found): %s\n", muid);
           fflush(fout);
       } /* fi */

       Load_Age_Repls();

       if(RTM_Debug)
       {
           fprintf(fout, "Loaded %ld Age Repls\n", num_age_repls);
           fflush(fout);
       } /* fi */

       /********************** born in handling here **********************/

           foundPos = isTriggerInText("born in", lcCit);
           if(foundPos > -1)
           {
               begSpan = foundPos;
               endSpan = begSpan + triggerLen + 5; /* Space + year */
               citSpan = (char *)malloc(triggerLen + 1000);
               strcpy(citSpan, "");
               tPos = 0;
               for(i = begSpan; i < endSpan; i++)
                 citSpan[tPos++] = citation[i];
               citSpan[tPos] = '\0';

               if(RTM_Debug)
               {
                   fprintf(fout, "Born In Span: #%s#\n", citSpan);
                   fflush(fout);
               } /* fi */

               /* Make sure we start with 1900 or 2000  and then
                  make sure we have a four digit year.  Not going to
                  use if it says "born in 21" since we don't know if
                  that is 1921 or 2021.
               */

               if(((strstr(citSpan, "born in 19") != NULL) ||
                  (strstr(citSpan, "born in 20") != NULL) ||
                  (strstr(citSpan, "born in 21") != NULL)) && 
                    isdigit(citSpan[10]) && isdigit(citSpan[11]))
               {
                   time(&now);
                   tmstruct = localtime(&now);
                   currYear = tmstruct->tm_year + 1900;
                   bornYear = -1;
                   sscanf(citSpan, "born in %ld", &bornYear);
                   ageDiff = currYear - bornYear;

                   if(RTM_Debug)
                   {
                       fprintf(fout, "Born In - Current Year: %ld  ", currYear);
                       fprintf(fout, "Born Year: %ld  Diff: %ld\n",
                               bornYear, ageDiff);
                       fflush(fout);
                   } /* fi */

                   if(ageDiff > 0)
                   {
                      begYear = ageDiff;
                      endYear = ageDiff;

                      if((begYear > -1) && (endYear > -1))
                        checkAddCT(citSpan, TRUE);
                   } /* fi */
               } /* fi */

               free(citSpan);
           } /* fi foundPos */

       /****************** Special Age related handling here ******************/

       /* Update list in ageLookFor.h (Special_Aged_Lookups) */

       for(j = 0; j < numSpecial_Aged; j++)
       {
           foundPos = isTriggerInText(Special_Aged_Lookups[j].lookFor, lcCit);

           /* Check to see if animal related trigger or not */

           if(foundPos > -1)
           {
               if(isAnimalContext(Special_Aged_Lookups[j].lookFor))
                 foundPos = -1;
           } /* fi */

           /* Need to make sure it is not "aged between" or "aged 1-3" or something
              like that.

              IMPORTANT NOTE: Also add to lookForAgedTermsRight
           */

           if((foundPos > -1) && (strcmp(Special_Aged_Lookups[j].lookFor, "aged") == 0))
           {
               if((strstr(lcCit, "aged between") != NULL) ||
                  (strstr(lcCit, "aged 1") != NULL) || (strstr(lcCit, "aged 2") != NULL) ||
                  (strstr(lcCit, "aged 3") != NULL) || (strstr(lcCit, "aged 4") != NULL) ||
                  (strstr(lcCit, "aged 5") != NULL) || (strstr(lcCit, "aged 6") != NULL) ||
                  (strstr(lcCit, "aged 7") != NULL) || (strstr(lcCit, "aged 8") != NULL) ||
                  (strstr(lcCit, "aged 9") != NULL) || (strstr(lcCit, "aged 0") != NULL) ||
                  (strstr(lcCit, "aged one") != NULL) || (strstr(lcCit, "aged two") != NULL) ||
                  (strstr(lcCit, "aged three") != NULL) || (strstr(lcCit, "aged four") != NULL) ||
                  (strstr(lcCit, "aged five") != NULL) || (strstr(lcCit, "aged six") != NULL) ||
                  (strstr(lcCit, "aged seven") != NULL) || (strstr(lcCit, "aged eight") != NULL) ||
                  (strstr(lcCit, "aged nine") != NULL) || (strstr(lcCit, "aged ten") != NULL) ||
                  (strstr(lcCit, "aged at least") != NULL) ||
                  (strstr(lcCit, "aged from") != NULL) || 
                  (strstr(lcCit, "aged  ranged") != NULL) ||
                  (strstr(lcCit, "aged  ranges") != NULL) ||
                  (strstr(lcCit, "aged ranged") != NULL) ||
                  (strstr(lcCit, "aged ranges") != NULL) ||
                  (strstr(lcCit, "aged range") != NULL) ||
                  (strstr(lcCit, "aged less") != NULL) ||
                  (strstr(lcCit, "aged more") != NULL) ||
                  (strstr(lcCit, "females aged") != NULL) || 
                  (strstr(lcCit, "female aged") != NULL) ||
                  (strstr(lcCit, "women aged") != NULL) || (strstr(lcCit, "woman aged") != NULL) ||
                  (strstr(lcCit, "males aged") != NULL) || (strstr(lcCit, "male aged") != NULL) ||
                  (strstr(lcCit, "men aged") != NULL) || (strstr(lcCit, "man aged") != NULL) ||
                  (strstr(lcCit, "adults aged") != NULL) ||
                  (strstr(lcCit, "adults aged") != NULL) ||
                  (strstr(lcCit, "adolescents aged") != NULL) ||
                  (strstr(lcCit, "adolescent aged") != NULL) ||
                  (strstr(lcCit, "infants aged") != NULL) ||
                  (strstr(lcCit, "infant aged") != NULL) ||
                  (strstr(lcCit, "toddlers aged") != NULL) ||
                  (strstr(lcCit, "toddler aged") != NULL) ||
                  (strstr(lcCit, "children aged") != NULL) ||
                  (strstr(lcCit, "child aged") != NULL) ||
                  (strstr(lcCit, "students aged") != NULL) ||
                  (strstr(lcCit, "student aged") != NULL) ||
                  (strstr(lcCit, "middle aged") != NULL) ||
                  (strstr(lcCit, "middle-aged") != NULL) ||
                  (strstr(lcCit, "school aged") != NULL) ||
                  (strstr(lcCit, "school-aged") != NULL) ||
                  (strstr(lcCit, "preschool aged") != NULL) ||
                  (strstr(lcCit, "preschool-aged") != NULL) ||
                  (strstr(lcCit, "college aged") != NULL) ||
                  (strstr(lcCit, "college-aged") != NULL) ||
                  (strstr(lcCit, "reproductive aged") != NULL) ||
                  (strstr(lcCit, "reproductive-aged") != NULL) ||
                  (strstr(lcCit, "median aged") != NULL) ||
                  (strstr(lcCit, "mean aged") != NULL) ||
                  (strstr(lcCit, "aged (") != NULL) || (strstr(lcCit, "aged(") != NULL) ||
                  (strstr(lcCit, "aged:") != NULL) || (strstr(lcCit, "aged :") != NULL) ||
                  (strstr(lcCit, "aged<") != NULL) || (strstr(lcCit, "aged <") != NULL) ||
                  (strstr(lcCit, "aged>") != NULL) || (strstr(lcCit, "aged >") != NULL) ||
                  (strstr(lcCit, "aged ranged") != NULL) ||
                  (strstr(lcCit, "aged ranges") != NULL) ||
                  (strstr(lcCit, "aged range") != NULL))
                 foundPos = -1;
           } /* fi */

           /* ADULT: Need to make sure it is not "older adults" or something like that. */

           if((foundPos > -1) && ((strcmp(Special_Aged_Lookups[j].lookFor, "adults") == 0) ||
                                  (strcmp(Special_Aged_Lookups[j].lookFor, "adult") == 0)))
           {
               if((strstr(lcCit, "older adults") != NULL) ||
                  (strstr(lcCit, "older adult") != NULL) ||
                  (strstr(lcCit, "young adult") != NULL) ||
                  (strstr(lcCit, "spiralis adult") != NULL) ||
                  (strstr(lcCit, "adult extract") != NULL) ||
                  (strstr(lcCit, "pre adult") != NULL) ||
                  (strstr(lcCit, "pre-adult") != NULL))
                 foundPos = -1;
           } /* fi */

           /* ADOLESCENT: Need to make sure it is not "14-tean" or something like that. */

           if((foundPos > -1) && (strcmp(Special_Aged_Lookups[j].lookFor, "teen") == 0))
           {
               if(
                  (strstr(lcCit, "one-teen") != NULL) ||  (strstr(lcCit, "one teen") != NULL) ||
                  (strstr(lcCit, "two-teen") != NULL) ||  (strstr(lcCit, "two teen") != NULL) ||
                  (strstr(lcCit, "three-teen") != NULL) ||  (strstr(lcCit, "three teen") != NULL) ||
                  (strstr(lcCit, "four-teen") != NULL) ||  (strstr(lcCit, "four teen") != NULL) ||
                  (strstr(lcCit, "five-teen") != NULL) ||  (strstr(lcCit, "five teen") != NULL) ||
                  (strstr(lcCit, "six-teen") != NULL) ||  (strstr(lcCit, "six teen") != NULL) ||
                  (strstr(lcCit, "seven-teen") != NULL) ||  (strstr(lcCit, "seven teen") != NULL) ||
                  (strstr(lcCit, "eight-teen") != NULL) ||  (strstr(lcCit, "eight teen") != NULL) ||
                  (strstr(lcCit, "nine-teen") != NULL) ||  (strstr(lcCit, "nine teen") != NULL) ||
                  (strstr(lcCit, "ten-teen") != NULL) ||  (strstr(lcCit, "ten teen") != NULL))
                 foundPos = -1;
           } /* fi */

           /* CHILD: Need to make sure it is not "childhood history" or something like that. */

           if((foundPos > -1) && (strcmp(Special_Aged_Lookups[j].lookFor, "childhood") == 0))
           {
               if((strstr(lcCit, "beyond childhood") != NULL) ||
                  (strstr(lcCit, "in childhood") != NULL) ||
                  (strstr(lcCit, "of childhood") != NULL) ||
                  (strstr(lcCit, "during childhood") != NULL) ||
                  (strstr(lcCit, "childhood history") != NULL) ||
                  (strstr(lcCit, "childhood experiences") != NULL) ||
                  (strstr(lcCit, "childhood experience") != NULL) ||
                  (strstr(lcCit, "childhood vaccination") != NULL) ||
                  (strstr(lcCit, "childhood misfortune") != NULL) ||
                  (strstr(lcCit, "childhood adversity") != NULL) ||
                  (strstr(lcCit, "childhood maltreatment") != NULL) ||
                  (strstr(lcCit, "childhood trauma") != NULL) ||
                  (strstr(lcCit, "childhood abuse") != NULL) ||
                  (strstr(lcCit, "childhood memories") != NULL) ||
                  (strstr(lcCit, "childhood memory") != NULL) ||
                  (strstr(lcCit, "childhood epigenetics") != NULL) ||
                  (strstr(lcCit, "early childhood") != NULL) ||
                  (strstr(lcCit, "wounds of childhood") != NULL))
                 foundPos = -1;
           } /* fi */

           /* CHILD: Need to make sure it is not "newborn children" or something like that. */

           if((foundPos > -1) && ((strcmp(Special_Aged_Lookups[j].lookFor, "children") == 0) ||
                                  (strcmp(Special_Aged_Lookups[j].lookFor, "child") == 0)))
           {
               if((strstr(lcCit, "5-year-old children") != NULL) ||
                  (strstr(lcCit, "4-year-old children") != NULL) ||
                  (strstr(lcCit, "3-year-old children") != NULL) ||
                  (strstr(lcCit, "2-year-old children") != NULL) ||
                  (strstr(lcCit, "5 year old children") != NULL) ||
                  (strstr(lcCit, "4 year old children") != NULL) ||
                  (strstr(lcCit, "3 year old children") != NULL) ||
                  (strstr(lcCit, "2 year old children") != NULL) ||
                  (strstr(lcCit, "5-year-old child") != NULL) ||
                  (strstr(lcCit, "4-year-old child") != NULL) ||
                  (strstr(lcCit, "3-year-old child") != NULL) ||
                  (strstr(lcCit, "2-year-old child") != NULL) ||
                  (strstr(lcCit, "5 year old child") != NULL) ||
                  (strstr(lcCit, "4 year old child") != NULL) ||
                  (strstr(lcCit, "3 year old child") != NULL) ||
                  (strstr(lcCit, "2 year old child") != NULL) ||
                  (strstr(lcCit, "newborn children") != NULL) ||
                  (strstr(lcCit, "newborn child") != NULL) ||
                  (strstr(lcCit, "pre-school child") != NULL) ||
                  (strstr(lcCit, "preschool child") != NULL) ||
                  (strstr(lcCit, "pre-school children") != NULL) ||
                  (strstr(lcCit, "toddler") != NULL) ||
                  (strstr(lcCit, "child-pugh") != NULL) ||
                  (strstr(lcCit, "child pugh") != NULL) ||
                  (strstr(lcCit, "preschool children") != NULL))
                 foundPos = -1;
           } /* fi */

           if(foundPos > -1)
           {
               begSpan = foundPos;
               endSpan = begSpan + triggerLen;
               citSpan = (char *)malloc(triggerLen + 1000);
               strcpy(citSpan, "");
               tPos = 0;
               for(i = begSpan; i < endSpan; i++)
                 citSpan[tPos++] = citation[i];
               citSpan[tPos] = '\0';

               if(RTM_Debug)
               {
                   fprintf(fout, "Special_Aged Span: #%s#\n", citSpan);
                   fflush(fout);
               } /* fi */

               for(x = 0; x < Special_Aged_Lookups[j].numAdds; x++)
                  AddSpecialAgedTerms(Special_Aged_Lookups[j].toADD[x],
                                      citSpan, begSpan);
               free(citSpan);

               /* X-out the special aged lookup we found */

               foo = XoutTriggerInText(Special_Aged_Lookups[j].lookFor, lcCit);
               free(lcCit);
               lcCit = strdup(foo);
               free(foo);
           } /* fi foundPos */
       } /* for */

       /****************** Regular related handling here ******************/

       if(foundNonHumanTrigger)
       {
          if(RTM_Debug)
          {
              fprintf(fout, "Skipping Regular Age Related checks due to animal found in text\n");
              fflush(fout);
          } /* fi */
          loopDone = TRUE;
       } /* fi */

       else
         loopDone = FALSE;

       while(!loopDone)
       {
           foundPos = lookForAgedTermsRight(lcCit);

           if(RTM_Debug)
           {
               fprintf(fout, "foundPos lookForAgedTermsRight: %ld\n", foundPos);
               fflush(fout);
           } /* fi */

           if(foundPos > -1)
           {
               /****************** Look for easy dates right after trigger ******************/

               begSpan = foundPos;
               snippet = pullSnippet(foundPos, lcCit);
               origSnippet = strdup(snippet);
               lenT = (long)strlen(origSnippet);

               if(RTM_Debug)
               {
                   fprintf(fout, "Initial Snippet Right: #%s#\n", snippet);
                   fflush(fout);
               } /* fi */

               /* Maybe we have an assumed years in the text - e.g., "age: 18-65".
                  aged 15
                  ages 15 - 34
                  ages 15 to 34
                  ages 15 and 34
                  age, 16-71
                  ages, 16-71
                  average age was 52.27+/-12.18;
               */

               foundLen = 0;
               if((strstr(snippet, "range ") == NULL) && (strstr(snippet, "ranged ") == NULL) &&
                  (strstr(snippet, "between ") == NULL))
               {
                   endSpan = foundPos + triggerLen + 1;
                   ok = TRUE;
                   i = endSpan;
                   while(ok && (i < (foundPos + lenT)))
                   {
                       if(isdigit(lcCit[i]) || (lcCit[i] == '-') ||
                          (lcCit[i] == ' ') || (lcCit[i] == '=') ||
                          (lcCit[i] == 'a') || (lcCit[i] == 'n') ||
                          (lcCit[i] == 'd') || (lcCit[i] == '.') ||
                          (lcCit[i] == '+') || (lcCit[i] == '-') ||
                          (lcCit[i] == '/') || (lcCit[i] == ',') ||
                          (lcCit[i] == 't') || (lcCit[i] == 'o'))
                         foundLen++;

                       else
                         ok = FALSE;
                       i++;
                   } /* while */

                   if(foundLen > 2)
                   {
                       /* Check here to see if we have "years", "yrs", "months", "mos", "weeks",
                          or "wks" after the date range and include in the snippet if we do.
                       */

                       endSpan += foundLen;
                       if(endSpan < (foundPos + lenT))
                       {
                           if((lcCit[endSpan] == 'y') || (lcCit[endSpan] == 'm') ||
                              (lcCit[endSpan] == 'w'))
                           {
                               posJ = 0;
                               strcpy(tmpCheck, "");
                               ok = FALSE;
                               i = endSpan;
                               while(!ok && (i < (foundPos + lenT)))
                               {
                                   if(!isalnum(lcCit[i]))
                                     ok = TRUE;
                                   else
                                     tmpCheck[posJ++] = lcCit[i];
                                   i++;
                               } /* while */
                               tmpCheck[posJ] = '\0';

                               if(RTM_Debug)
                               {
                                   fprintf(fout, "tmpCheck: #%s#\n", tmpCheck); 
                                   fflush(fout);
                               } /* fi */

                               if(posJ > 2)
                               {
                                   if((strcmp(tmpCheck, "years") == 0) ||
                                      (strcmp(tmpCheck, "yrs") == 0) ||
                                      (strcmp(tmpCheck, "months") == 0) ||
                                      (strcmp(tmpCheck, "weeks") == 0) ||
                                      (strcmp(tmpCheck, "mos") == 0) ||
                                      (strcmp(tmpCheck, "wks") == 0))
                                     endSpan += posJ;
                               } /* fi */
                           } /* fi y, m, w */
                       } /* fi endSpan */
                   } /* fi foundLen > 2 */
               } /* fi foundPos > -1 */

               /****************** Look for harder to find dates ******************/

               if(foundLen < 3)
               {
                   tLen = 5;
                   foo = strstr(snippet, "years");
                   if(foo == NULL)
                   {
                       tLen = 3;
                       foo = strstr(snippet, "yrs");
                   } /* fi */

                   if(foo == NULL)
                   {
                       tLen = 6;
                       foo = strstr(snippet, "months");
                   } /* fi */

                   if(foo == NULL)
                   {
                       tLen = 5;
                       foo = strstr(snippet, "weeks");
                   } /* fi */

                   if(foo == NULL)
                   {
                       tLen = 3;
                       foo = strstr(snippet, "wks");
                   } /* fi */

                   if(foo == NULL)
                   {
                       tLen = 3;
                       if((strstr(snippet, "mos0") == NULL) &&
                          (strstr(snippet, "mos1") == NULL) &&
                          (strstr(snippet, "mos2") == NULL) &&
                          (strstr(snippet, "mos3") == NULL) &&
                          (strstr(snippet, "mos4") == NULL) &&
                          (strstr(snippet, "mos5") == NULL) &&
                          (strstr(snippet, "mos6") == NULL) &&
                          (strstr(snippet, "mos7") == NULL) &&
                          (strstr(snippet, "mos8") == NULL) &&
                          (strstr(snippet, "mos9") == NULL) &&
                          (strstr(snippet, "mosa") == NULL) &&
                          (strstr(snippet, "mosb") == NULL) &&
                          (strstr(snippet, "mosc") == NULL) &&
                          (strstr(snippet, "mosd") == NULL) &&
                          (strstr(snippet, "mose") == NULL) &&
                          (strstr(snippet, "mosf") == NULL) &&
                          (strstr(snippet, "mosg") == NULL) &&
                          (strstr(snippet, "mosh") == NULL) &&
                          (strstr(snippet, "mosi") == NULL) &&
                          (strstr(snippet, "mosj") == NULL) &&
                          (strstr(snippet, "mosk") == NULL) &&
                          (strstr(snippet, "mosl") == NULL) &&
                          (strstr(snippet, "mosm") == NULL) &&
                          (strstr(snippet, "mosn") == NULL) &&
                          (strstr(snippet, "moso") == NULL) &&
                          (strstr(snippet, "mosp") == NULL) &&
                          (strstr(snippet, "mosq") == NULL) &&
                          (strstr(snippet, "mosr") == NULL) &&
                          (strstr(snippet, "moss") == NULL) &&
                          (strstr(snippet, "most") == NULL) &&
                          (strstr(snippet, "mosu") == NULL) &&
                          (strstr(snippet, "mosv") == NULL) &&
                          (strstr(snippet, "mosw") == NULL) &&
                          (strstr(snippet, "mosx") == NULL) &&
                          (strstr(snippet, "mosy") == NULL) &&
                          (strstr(snippet, "mosz") == NULL))
                         foo = strstr(snippet, "mos");
                       else
                         foo = NULL;
                   } /* fi == NULL */

                   if(foo != NULL)
                   {
                       diff = foo - snippet;
                       endSpan = (begSpan + diff + tLen); /* years = 5/yrs = 3 */
                       if(strstr(snippet, "range") != NULL)
                       {
                           fooR = strstr(snippet, "range");
                           diffR = fooR - snippet;
                           if((diffR - diff) < 16)
                             endSpan = begSpan + (long)strlen(snippet);

                           else if((strstr(fooR, "years") != NULL) || 
                                   (strstr(fooR, "months") != NULL) ||
                                   (strstr(fooR, "weeks") != NULL) ||
                                   (strstr(fooR, "mos") != NULL) ||
                                   (strstr(fooR, "wks") != NULL) ||
                                   (strstr(fooR, "yrs") != NULL))
                             endSpan = begSpan + (long)strlen(snippet);
                       } /* fi range */

                       else if(strstr(snippet, "between") != NULL)
                       {
                           fooR = strstr(snippet, "between");
                           diffR = fooR - snippet;
                           if((diffR - diff) < 16)
                             endSpan = begSpan + (long)strlen(snippet);

                           else if((strstr(fooR, "years") != NULL) || 
                                   (strstr(fooR, "months") != NULL) ||
                                   (strstr(fooR, "weeks") != NULL) ||
                                   (strstr(fooR, "mos") != NULL) ||
                                   (strstr(fooR, "wks") != NULL) ||
                                   (strstr(fooR, "yrs") != NULL))
                           endSpan = begSpan + (long)strlen(snippet);
                       } /* else fi */

                       else if(strstr(snippet, "iqr") != NULL)
                       {
                           fooR = strstr(snippet, "iqr");
                           diffR = fooR - snippet;
                           if((diffR - diff) < 16)
                             endSpan = begSpan + (long)strlen(snippet);
                       } /* else fi */

                       /* See if we have "years (##-##)" */

                       else
                       {
                           fooR = strstr(snippet, "years (");

                           if(fooR == NULL)
                             fooR = strstr(snippet, "yrs (");

                           if(fooR == NULL)
                             fooR = strstr(snippet, "months (");

                           if(fooR == NULL)
                             fooR = strstr(snippet, "mos (");

                           if(fooR != NULL)
                           {
                               diffR = fooR - snippet;
                               if(diffR == diff)
                               {
                                   ok = TRUE;
                                   done = FALSE;
                                   len = (long)strlen(snippet);
                                   pos = 0;
                                   for(i = (diffR + 6); ok && !done && (i < len); i++)
                                   {
                                       if(snippet[i] == ')')
                                       {
                                           inParens[pos++] = snippet[i];
                                           done = TRUE;
                                           endSpan = begSpan + (i + 1);
                                       } /* fi */

                                       else if((snippet[i] == '(') ||  (snippet[i] == '-') ||
                                               (snippet[i] == ' ') || isdigit(snippet[i]))
                                       {
                                           ok = TRUE;
                                           inParens[pos++] = snippet[i];
                                       } /* else fi */

                                       else
                                         ok = FALSE;
                                   } /* for */

                                   if(ok)
                                     inParens[pos] = '\0';
                                   else
                                     strcpy(inParens, "");
                               } /* fi diffR == diff */
                           } /* fi fooR != NULL */
                       } /* else */
                   } /* fi foo != NULL */

                   /* If we don't find "years" or "yrs", maybe we have an assumed
                      years in the text - e.g., "age: 18-65".
                   */

                   else
                   {
                       endSpan = foundPos + triggerLen + 1;
                       ok = TRUE;
                       i = endSpan;

                       /* Want to be able to identify various ranges:
                          aged 15
                          ages 15 - 34
                          ages 15 to 34
                          ages 15 and 34
                          age, 16-71
                          ages, 16-71
                          average age was 52.27+/-12.18;
                       */

                       while(ok && (i < (foundPos + lenT)))
                       {
                           if(isdigit(lcCit[i]) || (lcCit[i] == '-') ||
                              (lcCit[i] == ' ') || (lcCit[i] == '=') ||
                              (lcCit[i] == 'a') || (lcCit[i] == 'n') ||
                              (lcCit[i] == 'd') || (lcCit[i] == '.') ||
                              (lcCit[i] == '+') || (lcCit[i] == '-') ||
                              (lcCit[i] == '/') || (lcCit[i] == ',') ||
                              (lcCit[i] == '%') ||
                              (lcCit[i] == 't') || (lcCit[i] == 'o'))
                             endSpan++;

                           else
                             ok = FALSE;
                           i++;
                       } /* while */
                   } /* else foo == NULL */
               } /* fi foundLen  < 3 */

           /* select just the span we identified to move forward.  No
              point in doing a conversion of the entire citation when
              we just need to look at the span we have.
           */

           if((begSpan > -1) && (endSpan > -1) && (endSpan <= cit_len))
           {
               tPos = 0;
               tmp = (char *)malloc((endSpan - begSpan) + 1000);
               citSpan = (char *)malloc((endSpan - begSpan) + 1000);
               strcpy(tmp, "");
               strcpy(citSpan, "");

               for(i = begSpan; i < endSpan; i++)
               {
                  tmp[tPos] = lcCit[i];
                  if(citation[i] == '\n')
                    citSpan[tPos++] = ' ';
                  else
                    citSpan[tPos++] = citation[i];
               } /* for */
               tmp[tPos] = '\0';
               citSpan[tPos] = '\0';

               /* Try to remove any ##% that we find in the text so that
                  the number is not used for Ages */

               if(strchr(tmp, '%') != NULL)
               {
                   for(i = 0; i < tPos; i++)
                   {
                       if(tmp[i] == '%')
                       {
                           p = i - 1;
                           while((p > -1) && isdigit(tmp[p]))
                           {
                               tmp[p] = 'x';
                               citSpan[p] = 'x';
                               p--;
                           } /* while */
                       } /* fi */
                   } /* for */
               } /* fi */
           } /* fi */

           else
           {
               tmp = strdup(snippet);
               citSpan = strdup(snippet);
           } /* else */
           free(snippet);

           if(RTM_Debug)
           {
               fprintf(fout, "Snippet Before Edit: #%s#\n", tmp);
               fprintf(fout, "CitSpan Before Edit: #%s#\n", citSpan);
               fflush(fout);
           } /* fi */

           /*  Now do the substitutions for numbers, etc. */

           foo = stringEditByWord(tmp);
           snippet = strdup(foo);
           free(tmp);
           free(foo);

           if(RTM_Debug)
           {
               fprintf(fout, "Snippet: #%s#\n", snippet);
               fprintf(fout, "CitSpan: #%s#\n", citSpan);
               fflush(fout);
           } /* fi */

           snippSD = hideStdDev(snippet);

           if(RTM_Debug)
           {
               fprintf(fout, "after StdDev Snippet: #%s#\n", snippSD);
               fflush(fout);
           } /* fi */

           free(snippet);
           snippet = strdup(snippSD);
           free(snippSD);

           yearStr = (char *)NULL;

           if(strlen(snippet) > 0)
           {
               foundRange = FALSE;

               /* Dither about where exactly to pull from */

               foo = strstr(snippet, "years");

               if(foo == NULL)
                 foo = strstr(snippet, "year");

               if(foo == NULL)
                 foo = strstr(snippet, "yrs");

               if(foo == NULL)
                 foo = strstr(snippet, "months");

               if(foo == NULL)
                 foo = strstr(snippet, "month");

               if(foo == NULL)
                 foo = strstr(snippet, "mos");

               if(foo != NULL)
               {
                   diff = foo - snippet;
                   fooR = strstr(snippet, "range");
                   if(fooR != NULL)
                   {
                       diffR = fooR - snippet;
                       if((diffR - diff) < 16)
                         foundRange = TRUE;

                       else if((strstr(fooR, "years") != NULL) ||
                               (strstr(fooR, "year") != NULL) ||
                               (strstr(fooR, "yrs") != NULL) ||
                               (strstr(fooR, "months") != NULL) ||
                               (strstr(fooR, "month") != NULL) ||
                               (strstr(fooR, "mos") != NULL))
                       {
                           fooR2 = strstr(fooR, "years");

                           if(fooR2 == NULL)
                             fooR2 = strstr(fooR, "year");

                           if(fooR2 == NULL)
                             fooR2 = strstr(fooR, "yrs");

                           if(fooR2 == NULL)
                             fooR2 = strstr(fooR, "months");

                           if(fooR2 == NULL)
                             fooR2 = strstr(fooR, "month");

                           if(fooR2 == NULL)
                             fooR2 = strstr(fooR, "mos");

                           diffR2 = fooR2 - fooR;
                           foundRange = TRUE;
                       } /* else fi */
                   } /* fi range */

                   else if(strstr(snippet, "iqr") != NULL)
                   {
                       fooR = strstr(snippet, "iqr");
                       if(fooR != NULL)
                       {
                           diffR = fooR - snippet;
                           if((diffR - diff) < 16)
                             foundRange = TRUE;
                       } /* fi iqr */
                   } /* else fi */

                   else if(strstr(snippet, "between") != NULL)
                   {
                       pos = 0;
                       strcpy(betweenAnd, "");
                       for(i = 0; i < diff; i++)
                         betweenAnd[pos++] = snippet[i];
                       betweenAnd[pos] = '\0';
                       fooB = SwapAnd(betweenAnd);
                       strcpy(betweenAnd, fooB);
                       free(fooB);
                   } /* else fi */

                   if(foundRange)
                   {
                       len = (long)strlen(fooR);
                       yearStr = pullYears(fooR, len, TRUE, diffR);
                   } /* fi foundRange */

                   else if(strlen(inParens) > 0)
                   {
                       len = (long)strlen(inParens);
                       yearStr = pullYears(inParens, len, TRUE, 0);
                   } /* else fi */

                   else if(strlen(betweenAnd) > 0)
                   {
                       len = (long)strlen(betweenAnd);
                       yearStr = pullYears(betweenAnd, len, TRUE, 0);
                   } /* else fi */

                   else
                     yearStr = pullYears(snippet, diff, FALSE, 0);
               } /* fi found years */

               else
               {
                   diff = (long)strlen(snippet);
                   if(strchr(snippet, '-') == NULL)
                     yearStr = pullYears(snippet, diff, TRUE, 0);
                   else
                     yearStr = pullYears(snippet, diff, FALSE, 0);
               } /* else */
           } /* fi have snippet */

           if(yearStr != NULL)
           {
               if(RTM_Debug)
               {
                   fprintf(fout, "Years1: #%s#  citSpan: #%s#\n", 
                           yearStr, citSpan);
                   fflush(fout);
               } /* fi */

               parseYears(yearStr, snippet);

               if((begYear > -1) && (endYear > -1))
                 checkAddCT(citSpan, FALSE);
           } /* fi yearStr */

           if(yearStr != NULL)
             free(yearStr);
       } /* fi lookRight foundPos > -1 */

       /* If not found regular, look for left-side
             two-year-old boy|man|male
             two-year-old girl|woman|female
       */

       else
       {
           foundPos = lookForAgedTermsLeft(lcCit);

           strLen = triggerLen;
           if(foundPos > -1)
           {
              snippet = pullSnippetLeft(foundPos, lcCit, strLen);
              origSnippet = strdup(snippet);
              lenT = (long)strlen(origSnippet);
              begSpan = pSL_start;
              endSpan = foundPos + strLen;

              if(RTM_Debug)
              {
                  fprintf(fout, "Initial Snippet Left: #%s#\n", snippet);
                  fflush(fout);
              } /* fi */

              /* select just the span we identified to move forward.  No
                 point in doing a conversion of the entire citation when
                 we just need to look at the span we have.
              */

              if((begSpan > -1) && (endSpan > -1) && (endSpan <= cit_len))
              {
                  tPos = 0;
                  tmp = (char *)malloc((endSpan - begSpan) + 1000);
                  citSpan = (char *)malloc((endSpan - begSpan) + 1000);
                  strcpy(tmp, "");
                  strcpy(citSpan, "");

                  for(i = begSpan; i < endSpan; i++)
                  {
                      tmp[tPos] = lcCit[i];
                      citSpan[tPos++] = citation[i];
                  } /* for */
                  tmp[tPos] = '\0';
                  citSpan[tPos] = '\0';
              } /* fi begSpan */

              else
              {
                  tmp = strdup(snippet);
                  citSpan = strdup(snippet);
              } /* else */

              if(RTM_Debug)
              {
                  fprintf(fout, "Snippet Before Edit: #%s#\n", tmp);
                  fprintf(fout, "CitSpan Before Edit: #%s#\n", citSpan);
                  fflush(fout);
              } /* fi */

              /*  Now do the substitutions for numbers, etc. */

              free(snippet);
              foo = stringEditByWord(tmp);
              snippet = strdup(foo);
              free(tmp);
              free(foo);

              if(RTM_Debug)
              {
                  fprintf(fout, "Snippet: #%s#\n", snippet);
                  fprintf(fout, "CitSpan: #%s#\n", citSpan);
                  fflush(fout);
              } /* fi */

              yearStr = (char *)NULL;

              if((strlen(snippet) > 0) && (strstr(snippet, "year") != NULL))
              {
                  foundRange = FALSE;

                  /* See about where exactly to pull from */

                  foo = strstr(snippet, "year");
                  if(foo != NULL)
                  {
                      diff = foo - snippet;
                      yearStr = pullYears(snippet, diff, FALSE, 0);
                  } /* fi */

                  if(yearStr != NULL)
                  {
                      if(RTM_Debug)
                      {
                          fprintf(fout, "Years2: #%s#  citSpan: #%s#\n",
                                  yearStr, citSpan);
                          fflush(fout);
                      } /* fi */

                      parseYears(yearStr, snippet);

                      if(RTM_Debug)
                      {
                          fprintf(fout, "begYear: %f  endYear: %f\n",
                                  begYear, endYear);
                          fflush(fout);
                      } /* fi */

                      /* Don't need to trim off the end (usedEnd)
                         here since we specified the entire lookup
                         that we wanted.
                      */

                      if((begYear > -1) && (endYear > -1))
                        checkAddCT(citSpan, TRUE);
                  } /* fi yearStr */

                  else
                  {
                      if(RTM_Debug)
                      {
                          fprintf(fout, "yearStr Empty (%s)\n", snippet);
                          fflush(fout);
                      } /* fi */
                  } /* else */

                  if(yearStr != NULL)
                    free(yearStr);
              } /* fi year snippet */

              else if((strlen(snippet) > 0) &&
                      ((strstr(snippet, "month") != NULL) ||
                       (strstr(snippet, "mos") != NULL)))
              {
                  foundRange = FALSE;

                  /* See about where exactly to pull from */

                  foo = strstr(snippet, "month");
                  if(foo == NULL)
                    foo = strstr(snippet, "mos");
                  if(foo != NULL)
                  {
                      diff = foo - snippet;
                      yearStr = pullYears(snippet, diff, FALSE, 0);
                  } /* fi */

                  if(yearStr != NULL)
                  {
                      if(RTM_Debug)
                      {
                          fprintf(fout, "Months: #%s#  citSpan: #%s#\n",
                                  yearStr, citSpan);
                          fflush(fout);
                      } /* fi */

                      parseYears(yearStr, snippet);

                      /* Don't need to trim off the end (usedEnd)
                         here since we specified the entire lookup
                         that we wanted.
                      */

                      if((begYear > -1) && (endYear > -1))
                        checkAddCT(citSpan, TRUE);
                  } /* fi yearStr */

                  else
                  {
                      if(RTM_Debug)
                      {
                          fprintf(fout, "yearStr Months Empty (%s)\n", snippet);
                          fflush(fout);
                      } /* fi */
                  } /* else */

                  if(yearStr != NULL)
                    free(yearStr);
              } /* else fi month snippet */

              else if((strlen(snippet) > 0) &&
                      ((strstr(snippet, "age group") != NULL) ||
                       (strstr(snippet, "age range") != NULL)))
              {
                  foundRange = TRUE;

                  /* See about where exactly to pull from */

                  foo = strstr(snippet, "age");
                  if(foo != NULL)
                  {
                      diff = foo - snippet;
                      yearStr = pullYears(snippet, diff, FALSE, 0);
                  } /* fi */

                  if(yearStr != NULL)
                  {
                      if(RTM_Debug)
                      {
                          fprintf(fout, "Age Group/Range: #%s#  citSpan: #%s#\n",
                                  yearStr, citSpan);
                          fflush(fout);
                      } /* fi */

                      parseYears(yearStr, snippet);

                      /* Don't need to trim off the end (usedEnd)
                         here since we specified the entire lookup
                         that we wanted.
                      */

                      if((begYear > -1) && (endYear > -1))
                        checkAddCT(citSpan, TRUE);
                  } /* fi yearStr */

                  else
                  {
                      if(RTM_Debug)
                      {
                          fprintf(fout, "yearStr Age Group/Range Empty (%s)\n", snippet);
                          fflush(fout);
                      } /* fi */
                  } /* else */

                  if(yearStr != NULL)
                    free(yearStr);
              } /* else fi age group/range snippet */

                  free(origSnippet);
                  free(snippet);
                  free(citSpan);
               } /* fi foundPos > -1 */
           } /* else */

           if(foundPos < 0)
             loopDone = TRUE;

           else
           {
               /* "aged" is a special case being in two different lists so we need to
                  see if maybe it was found all on its own and if so, still add it
                  as "aged".
               */

               if((strcmp(citSpan, "aged") == 0) || (strcmp(citSpan, "aged ") == 0))
               {
                   for(i = 0; i < Special_Aged_Lookups[agedPos].numAdds; i++)
                      AddSpecialAgedTerms(Special_Aged_Lookups[agedPos].toADD[i],
                                          citSpan, begSpan);
               } /* fi */

               for(i = begSpan; i < endSpan; i++)
                 lcCit[i] = 'X';
           } /* else */

           if(RTM_Debug)
           {
               fprintf(fout, "AFTER foundPos2: %ld\n", foundPos);
               fflush(fout);
           } /* fi */

           begYear = endYear = -1.0;
           begSpan = endSpan = -1;

           for(i = 0; i < numAgeCTs; i++)
             Age_CT_list[i].found = FALSE;
       } /* while !loopDone */

       free(lcCit);
   } /* fi Humans */

/* Removed 12/23/2020 - causing occasional core dump
   cleanReplList();
*/

   if(RTM_Debug)
   {
       fprintf(fout, "Leaving checkForAges\n");
       fflush(fout);
   } /* fi */
} /* checkForAgeYears */


/***************************************************************************
*
*  hideStdDev --
*
*      This routine 
*
***************************************************************************/

char *hideStdDev(char *snippet)
{
    int done;
    long diff, i, j, len, pos, snipLen, posT, fooLen;
    char *rtn, *foo, tmp[120];

    snipLen = (long)strlen(snippet);
    len = 0;
    rtn = strdup(snippet);
    foo = strstr(snippet, "sd=");
    len = 3;

    if(foo == NULL)
    {
        foo = strstr(snippet, "s.d. = ");
        len = 7;
    } /* fi */

    if(foo == NULL)
    {
        foo = strstr(snippet, "s.d.=");
        len = 5;
    } /* fi */

    if(foo == NULL)
    {
        foo = strstr(snippet, "sd = ");
        len = 5;
    } /* fi */

    if(foo == NULL)
    {
        foo = strstr(snippet, "s.d = ");
        len = 6;
    } /* fi */

    if(foo == NULL)
    {
        foo = strstr(snippet, "s.d=");
        len = 4;
    } /* fi */

    if(foo == NULL)
    {
        foo = strstr(snippet, "standard deviation = ");
        len = 21;
    } /* fi */

    if(foo == NULL)
    {
        foo = strstr(snippet, "standard deviation of ");
        len = 22;
    } /* fi */

    if(foo == NULL)
    {
        foo = strstr(snippet, "standard deviation=");
        len = 19;
    } /* fi */

    if(foo == NULL)
    {
        foo = strstr(snippet, "(sd ");
        len = 4;
    } /* fi */

    if(foo == NULL)
    {
        foo = strstr(snippet, "(sd,");
        len = 4;
    } /* fi */

    if(foo == NULL)
    {
        foo = strstr(snippet, "(sd: ");
        len = 5;
    } /* fi */

    if(foo == NULL)
    {
        foo = strstr(snippet, "[sd ");
        len = 4;
    } /* fi */

    if(foo == NULL)
    {
        foo = strstr(snippet, "[sd: ");
        len = 5;
    } /* fi */

    if(foo == NULL)
    {
        foo = strstr(snippet, "[sd, ");
        len = 5;
    } /* fi */

    if(foo == NULL)
    {
        foo = strstr(snippet, "sd ");
        len = 5;
    } /* fi */

    if(foo == NULL)
    {
        foo = strstr(snippet, "standard deviation (sd)");
        len = 23;
    } /* fi */

    if(foo == NULL)
    {
        foo = strstr(snippet, "standard deviation [sd]");
        len = 23;
    } /* fi */

    if(foo == NULL)
    {
        foo = strstr(snippet, "standard deviation,");
        len = 19;
    } /* fi */

    if(foo == NULL)
    {
        foo = strstr(snippet, "standard deviation ");
        len = 19;
    } /* fi */

    if(foo == NULL)
    {
        foo = strstr(snippet, "follow-up time of ");
        len = 18;
    } /* fi */

    if(foo == NULL)
    {
        foo = strstr(snippet, "follow-up of ");
        len = 13;
    } /* fi */

    if(foo == NULL)
    {
        foo = strstr(snippet, "mean follow-up ");
        len = 15;
    } /* fi */

    if(foo == NULL)
    {
        foo = strstr(snippet, "median follow-up ");
        len = 17;
    } /* fi */

    if(foo == NULL)
    {
        foo = strstr(snippet, "duration of ");
        len = 12;
    } /* fi */

    /* Blank out everything after trigger and a number even if we have 99 SD +/- 8.1 years it
       should come out the other end as 99 XX XXX XXX years.
    */

    if(foo != NULL)
    {
        fooLen = (long)strlen(foo);
        if(RTM_Debug)
        {
            fprintf(fout, "Std Dev Found [len: %ld]: #%s#\n", fooLen, foo);
            fflush(fout);
        } /* fi */

        diff = foo - snippet;
        pos = diff;
        done = FALSE;
        for(i = 0; !done && (i < fooLen); i++)
        {
            if(i < len)
              rtn[pos++] = 'X';

            else if(isdigit(rtn[pos]) || (rtn[pos] == '.') || (rtn[pos] == '+') ||
                    (rtn[pos] == '-') || (rtn[pos] == '/'))
              rtn[pos++] = 'X';

            else if(rtn[pos] == ' ')
              rtn[pos++] = ' ';

            else
              done = TRUE;
        } /* for */

        if(RTM_Debug)
        {
            fprintf(fout, "Return after Std Dev removed: #%s#\n", rtn);
            fflush(fout);
        } /* fi */
    } /* fi */

    return(rtn);
} /* hideStdDev */

/***************************************************************************
*
*  isTriggerInText --
*
*      This 
*
***************************************************************************/

long isTriggerInText(char *lookFor, char *text)
{
   long rtn = -1;
   char *base, *foo, *baseLookFor;
   int okLeft, okRight, done;
   long i, j, baseLen, lfLen, diff, rPos, pos;

   okLeft = okRight = FALSE;
   lfLen = (long)strlen(lookFor);
   triggerLen = lfLen;
   base = strdup(text);
   baseLen = (long)strlen(text);
   baseLookFor = strdup(lookFor);

   done = FALSE;
   foo = strstr(base, baseLookFor);
   while(!done && (foo != NULL))
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

          /* Only actual lookFor, no plural allowed */

          if((pos >= baseLen) || isspace(base[pos]) || ispunct(base[pos]))
            okRight = TRUE;
       } /* fi okLeft */

       if(okLeft && okRight)
       {
           done = TRUE;
           rtn = diff;
       } /* fi */
 
       for(j = 0; j < lfLen; j++)
          base[j + diff] = 'X';

       foo = strstr(base, baseLookFor);
  } /* while foo */

   free(base); free(baseLookFor);
   return(rtn);
} /* isTriggerInText */


/***************************************************************************
*
*  pullSnippet --
*
*      This 
*
***************************************************************************/

char *pullSnippet(long startPos, char *text)
{
    int done;
    long i, p, endPos, sLen, sPos, abStart;
    char *snippet, *foo;

    done = FALSE;
    i = endPos = startPos;
    while(!done && (i < ageCitLen))
    {
        if((i + 1) < ageCitLen)
        {
            if((text[i + 1] == ' ') || (text[i + 1] == '<'))
            {
                if((text[i] == '?') || (text[i] == '.'))
                  done = TRUE;
            } /* fi */

            /* Check to see if we are in a table and have an end cell span */

            else if(text[i] == '<')
            {
               if(text[i + 1] == '/')
                 done = TRUE;
            } /* fi */
        } /* fi */

        i++;
        endPos++;
    } /* while */

    /* Make sure snippet doesn't extend from TI to AB -
       PMID: 31026078 triggered this.
     */

    foo = strstr(text, "ab  - ");
    if(foo != NULL)
    {
        abStart = foo - text;
        if((startPos < abStart) && (endPos > abStart))
          endPos = abStart - 1;
    } /* fi */

    sLen = (endPos - startPos);
    snippet = (char *)malloc(sLen + 1000);
    strcpy(snippet, "");
    sPos = 0;

    /* Make sure to not include newline characters in snippet */

    for(i = startPos; i < endPos; i++)
    {
       if(text[i] == '\n')
         snippet[sPos++] = ' ';
       else
         snippet[sPos++] = text[i];
    } /* for */
    snippet[sPos] = '\0';

    /* Try to remove any ##% that we find in the text so that the number is not used for Ages */

    if(strchr(snippet, '%') != NULL)
    {
       for(i = 0; i < sPos; i++)
       {
           if(snippet[i] == '%')
           {
               p = i - 1;
               while((p > -1) && isdigit(snippet[p]))
                 snippet[p--] = 'x';
           } /* fi */
       } /* for */
    } /* fi */

    return(snippet);
} /* pullSnippet */


/***************************************************************************
*
*  pullSnippetLeft --
*
*      This 
*
***************************************************************************/

char *pullSnippetLeft(long startPos, char *text, long strLen)
{
    int done, ctr;
    long i, p, endPos, sLen, sPos;
    char *snippet;

    endPos = startPos + strLen;
    done = FALSE;
    ctr = 0;
    i = startPos;
    while(!done && (i > 0))
    {
        if((text[i] == ' ') || (text[i] == '-'))
        {
            ctr++;
            if(ctr > 4)
              done = TRUE;
        } /* fi */

        /* 14-year-old man, 15-year-old woman and 20-year-old man */

        else if((text[i] == ',') || (text[i] == ';') || (text[i] == '\n'))
          done = TRUE;

        i--;
    } /* while */

    if(done)
      startPos = i + 2;
    else
      startPos = 0;

    /* Remove any preceding blanks if we have any */

    while(text[startPos] == ' ')
      startPos++;

    sLen = (endPos - startPos);
    snippet = (char *)malloc(sLen + 1000);
    strcpy(snippet, "");
    sPos = 0;
    for(i = startPos; i < endPos; i++)
      snippet[sPos++] = text[i];
    snippet[sPos] = '\0';

    /* Try to remove any ##% that we find in the text so that the number is not used for Ages */

    if(strchr(snippet, '%') != NULL)
    {
       for(i = 0; i < sPos; i++)
       {
           if(snippet[i] == '%')
           {
               p = i - 1;
               while((p > -1) && isdigit(snippet[p]))
                 snippet[p--] = 'x';
           } /* fi */
       } /* for */
    } /* fi */

    pSL_start = startPos;

    return(snippet);
} /* pullSnippetLeft */


/***************************************************************************
*
*  Load_Age_Repls --
*
*      This routine
*
***************************************************************************/

void Load_Age_Repls()
{
    FILE *fp;
    char line[SMALL_LINE + 1], search_for[SMALL_LINE + 1], 
         replace_by[SMALL_LINE + 1];
    int pos, numFound;
    long len_s, len_r;

    numFound = 0;
    fp = fopen(REPL_FILE_4_AGES, "r");
    while(fgets(line, SMALL_LINE, fp) != (char *)NULL)
    {
       if(strchr(line, '|') == NULL)
       {
           if(numFound > 0)
             num_age_repls++;

           strcpy(search_for, "");
           sscanf(line, "%[^\n]", search_for);
           repl_age_list[num_age_repls].numSearch = 0;
           repl_age_list[num_age_repls].checkFirst = strdup(search_for);
       } /* fi */

       else
       {
           numFound++;
           strcpy(search_for, "");
           strcpy(replace_by, "");
           sscanf(line, "%[^|]|%[^\n]", search_for, replace_by);

           pos = repl_age_list[num_age_repls].numSearch;
           repl_age_list[num_age_repls].repls[pos].search_for =
                         strdup(search_for);
           repl_age_list[num_age_repls].repls[pos].replace_by =
                         strdup(replace_by);
           repl_age_list[num_age_repls].numSearch++;
       } /* else */
    } /* while */

    if(numFound > 0)
      num_age_repls++;
} /* Load_Age_Repls */

/***************************************************************************
*
*  stringEditByWord --
*
*      This routine checks to see if a distinct word prior to converting
*
***************************************************************************/

char *stringEditByWord(char *insegment)
{
   long len = ((long)strlen(insegment) * 5);
   long slen, diff, search_end, rlen, tlen;
   int found, okLeft, okRight;
   char *rtn = (char *)malloc(len + 1);
   char *tmp2 = (char *)malloc(len + 1);
   char *tmp3 = (char *)malloc(len + 1);
   char *tmp;
   long i, j, x, rtn_pos, pos;

   strcpy(tmp3, insegment);
   for(i = 0; i < num_age_repls; i++)
   {
      if((strstr(tmp3, repl_age_list[i].checkFirst) != NULL) ||
         (strlen(repl_age_list[i].checkFirst) == 0))
      {
          for(x = 0; x < repl_age_list[i].numSearch; x++)
          {
              found = okLeft = okRight = FALSE;
              strcpy(rtn, "");
              rtn_pos = 0;
              tlen = (long)strlen(tmp3);
              slen = (long)strlen(repl_age_list[i].repls[x].search_for);
              rlen = (long)strlen(repl_age_list[i].repls[x].replace_by);

              strcpy(tmp2, "");
              tmp = strstr(tmp3, repl_age_list[i].repls[x].search_for);
              if(tmp != NULL)
              {
                  found = TRUE;
                  while(found)
                  {
                      okLeft = okRight = FALSE;
                      diff = tmp - tmp3;
                      search_end = diff + slen;

                      /* Copy up to what we are replacing */

                      for(j = 0; j < diff; j++)
                        rtn[rtn_pos++] = tmp3[j];
                      rtn[rtn_pos] = '\0';

                      if(diff == 0)
                        okLeft = TRUE;

                      else if(diff > 0)
                      {
                         if(isspace(tmp3[diff - 1]) || ispunct(tmp3[diff - 1]))
                           okLeft = TRUE;
                      } /* else fi */

                      if(okLeft)
                      {
                          /* Only actual lookFor, no plural allowed */

                          if((search_end >= tlen) || 
                             isspace(tmp3[search_end]) ||
                             ispunct(tmp3[search_end]))
                            okRight = TRUE;
                      } /* fi */

                      if(okLeft && okRight)  /* Add in the replace_by info */
                      {
                          for(j = 0; j < rlen; j++)
                            rtn[rtn_pos++] =
                                  repl_age_list[i].repls[x].replace_by[j];
                      } /* fi */

                      else /* Not a match, X out so we can skip */
                      {
                          for(j = 0; j < slen; j++)
                            rtn[rtn_pos++] = 'X';
                      } /* else */
                      rtn[rtn_pos] = '\0';

                      /* Now save off the remainder of the string to process */

                      pos = 0;
                      for(j = search_end; j < tlen; j++)
                        tmp2[pos++] = tmp3[j];
                      tmp2[pos] = '\0';

                      strcpy(tmp3, tmp2);
                      tmp = strstr(tmp3, repl_age_list[i].repls[x].search_for);
                      if(tmp != NULL)
                        found = TRUE;
                      else
                      {
                          found = FALSE;
                          strcat(rtn, tmp3);
                      } /* else */
                  } /* while found */
              } /* fi tmp */

              else
              {
                  found = FALSE;
                  strcpy(rtn, tmp3);
              } /* else */

              strcpy(tmp3, rtn);
          } /* for each search for */
      } /* if checkFirst found */
   } /* for */

   free(tmp2); free(tmp3);

   len = (long)strlen(rtn);
   rtn = (char *)realloc(rtn, len + 1);

   return(rtn);
} /* stringEditByWord */


/***************************************************************************
*
*  pullYears --
*
*      This routine 
*
***************************************************************************/

char *pullYears(char *snippet, long len, int hasRange, long snippetOffset)
{
   int done, foundDash, ok, foundPlus, foundSlash;
   long yPos, i, yearEnd, yearBeg, sEndPos;
   char *rtn, *foo, *tmp;

   if(RTM_Debug)
   {
       fprintf(fout, "pullYears('%s', %ld, %ld) - hasRange: %d\n", snippet,
               len, snippetOffset, hasRange);
       fflush(fout);
   } /* fi */

   if((strchr(snippet, '&') != NULL) || (strchr(snippet, '#') != NULL) ||
       (strchr(snippet, ';') != NULL))
      tmp = Strip_Str(snippet);
   else
      tmp = strdup(snippet);

   if(RTM_Debug)
   {
       fprintf(fout, "pullYears - TMP: #%s#\n", tmp);
       fflush(fout);
   } /* fi */

   yearEnd = len - 1;
   yearBeg = sEndPos = 0;
   rtn = (char *)malloc((len * 3) + 1);
   strcpy(rtn, "");

   /* Find start/end of years.  If years found work right to left, if years
      not found, work left to right.  For range/iqr, force left to right.
    */

   if(hasRange)
     foo = NULL;
   else
   {
       foo = strstr(tmp, "year");
       if(foo == NULL)
         foo = strstr(tmp, "yrs");
   } /* else */

   if(foo != NULL)
   {
       sEndPos = len;
       yearEnd = foo - tmp;
       i = yearEnd;
       done = FALSE;
       while(!done && (i >= 0))
       {
           if(isdigit(tmp[i]))
           {
               yearEnd = i + 1;
               while(!done && (i >= 0))
               {
                   i--;
                   if(isdigit(tmp[i]) || (tmp[i] == ' ') ||
                      (tmp[i] == '+') || (tmp[i] == '/') ||
                      (tmp[i] == '(') || (tmp[i] == ',') ||
                      (tmp[i] == '-') || (tmp[i] == '.'))
                     done = FALSE;
                   else
                     done = TRUE;
               } /* while */

               yearBeg = i + 1;
           } /* fi */
           i--;
       } /* while */

       if(RTM_Debug)
       {
           fprintf(fout, "Interim 1 pullYears: #");
           for(i = yearBeg; i < yearEnd; i++)
             fprintf(fout, "%c", tmp[i]);
           fprintf(fout, "#\n");
       } /* fi */

       /* Remove any beginning spaces want to keep leading dash so we can tell negative number  */

       while(!isdigit(tmp[yearBeg]) && (tmp[yearBeg] != '-') && (yearBeg < len))
         yearBeg++;

       if(RTM_Debug)
       {
           fprintf(fout, "Interim 1 after clean pullYears: #");
           for(i = yearBeg; i < yearEnd; i++)
             fprintf(fout, "%c", tmp[i]);
           fprintf(fout, "#\n");
       } /* fi */
   } /* fi */

   else
   {
       i = 0;
       done = FALSE;
       while(!done && (i < len))
       {
           if(isdigit(tmp[i]))
           {
               yearBeg = i;
               while(!done && (i < len))
               {
                   i++;
                   sEndPos++;
                   if(isdigit(tmp[i]) || (tmp[i] == ' ') ||  (tmp[i] == ',') ||
                      (tmp[i] == '+') || (tmp[i] == '/') ||
                      (tmp[i] == '-') || (tmp[i] == '.'))
                     done = FALSE;
                   else
                     done = TRUE;
               } /* while */
               yearEnd = i;
           } /* fi */
           i++;
           sEndPos++;
       } /* while */

       if(RTM_Debug)
       {
           fprintf(fout, "Interim 2 pullYears: #");
           for(i = yearBeg; i < yearEnd; i++)
             fprintf(fout, "%c", tmp[i]);
           fprintf(fout, "#\n");
       } /* fi */

       /* Remove any end spaces */

       while(!isdigit(tmp[yearEnd]) && (yearEnd > 0))
         yearEnd--;

       if(RTM_Debug)
       {
           fprintf(fout, "Interim 2 after clean pullYears: #");
           for(i = yearBeg; i < yearEnd; i++)
             fprintf(fout, "%c", tmp[i]);
           fprintf(fout, "#\n");
       } /* fi */

       /* For the "for" loop, one past where we want to end */

       yearEnd++;
   } /* else */

   if(RTM_Debug)
   {
       fprintf(fout, "Interim pullYears: #");
       for(i = yearBeg; i < yearEnd; i++)
         fprintf(fout, "%c", tmp[i]);
       fprintf(fout, "#\n");
   } /* fi */

   /* Pull the year info, removing any embedded spaces & extra dashes */

   yPos = 0;
   foundDash = foundPlus = foundSlash = FALSE;
   for(i = yearBeg; i < yearEnd; i++)
   {
       if(tmp[i] != ' ')
       {
           ok = TRUE;
           if((tmp[i] == '-') && foundDash)
             ok = FALSE;

           if((tmp[i] == '+') && foundPlus)
             ok = FALSE;

           if((tmp[i] == '/') && foundSlash)
             ok = FALSE;

           if(ok)
           {
               if(tmp[i] == ',')
                 rtn[yPos++] = '.';  /* Convert comma to period european notation for decimal */
               else
                 rtn[yPos++] = tmp[i];
           } /* fi */
       } /* fi */

       if(tmp[i] == '-')
         foundDash = TRUE;

       if(tmp[i] == '+')
         foundPlus = TRUE;

       if(tmp[i] == '/')
         foundSlash = TRUE;
   } /* for */
   rtn[yPos] = '\0';

   /* Want to make sure we return nothing if we are not confident we have
      a valid result.
   */

   if(hasRange && !foundDash)
   {
       if(RTM_Debug)
       {
           fprintf(fout, "RANGE, NO DASH: #%s# (%s)\n", tmp, rtn);
           fflush(fout);
       } /* fi */
/*
       strcpy(rtn, "");
*/
   } /* fi */

   usedEnd = snippetOffset + sEndPos;

   if(RTM_Debug)
   {
       fprintf(fout, "offset: %ld  length: %ld\n", snippetOffset, sEndPos);
       fprintf(fout, "rtn end: #%s#\n", rtn);
       fflush(fout);
   } /* fi */

   free(tmp);

   return(rtn);
} /* pullYears */

/***************************************************************************
*
*  parseYears --
*
*      This 
*
***************************************************************************/

void parseYears(char *years, char *snippet)
{
    char left[SMALL_LINE + 1], right[SMALL_LINE + 1], *foo, tmp[SMALL_LINE + 1];
    int havePlusMinus;
    long foundPos, diff, diff2;
    float floatLeft, floatRight, floatPlusMinus;

    floatLeft = floatRight = floatPlusMinus = 0.0;
    havePlusMinus = FALSE;
    strcpy(left, "");
    strcpy(right, "");

    if(strstr(years, "+/-") != NULL)                   /* 25.6+/-6.5 */
    {
        sscanf(years, "%[^+]+/-%[^\n]", left, right);
        havePlusMinus = TRUE;
    } /* fi */

    else if(strstr(years, "+/") != NULL)               /* 31.7+/5.4 */
    {
        sscanf(years, "%[^+]+/%[^\n]", left, right);
        havePlusMinus = TRUE;
    } /* else fi */

    else if(strstr(years, "+-") != NULL)               /* 27.3+-3.7 */
    {
        sscanf(years, "%[^+]+-%[^\n]", left, right);
        havePlusMinus = TRUE;
    } /* else fi */

    else if(strchr(years, '+') != NULL)                /* 6.34+3.75 */
    {
        sscanf(years, "%[^+]+%[^\n]", left, right);
        havePlusMinus = TRUE;
    } /* else fi */

    else if(strchr(years, '-') != NULL)                /* 18-34 */
      sscanf(years, "%[^-]-%[^\n]", left, right);

    else                                               /* normal number */
      sscanf(years, "%[^\n]", left);

    if(RTM_Debug)
    {
        fprintf(fout, "Left: #%s#  Right: #%s#\n", left, right);
        fflush(fout);
    } /* fi */

    /* Handle case like "aged 4 to 95  (47.6+/-11.8) years old." where we
       are erroneously picking up the +/- years.
       Added: May 20, 2020 - JGM
    */

    if(strlen(left) > 0)
    {
        foo = strchr(left, '(');
        if(foo != NULL) /* 4-95(47.6 */
        {
            diff = foo - left;
            foo = strchr(left, '-');
            if(foo != NULL)
            {
                diff2 = foo - left;
                if(diff2 < diff)
                {
                    strcpy(tmp, left);
                    strcpy(left, "");
                    strcpy(right, "");
                    sscanf(tmp, "%[^-]-%[^(]", left, right);
                    havePlusMinus = FALSE;

                    if(RTM_Debug)
                    {
                        fprintf(fout, "Left2: #%s#  Right2: #%s#\n",
                                left, right);
                        fflush(fout);
                    } /* fi */
                } /* fi */
            } /* fi */
        } /* fi */
    } /* fi */


    if(strlen(left) > 0)
    {
        floatLeft = determineYear(left);
        if(RTM_Debug)
        {
            fprintf(fout, "number Left: %f\n", floatLeft);
            fflush(fout);
        } /* fi */

        if(strlen(right) > 0)
        {
            if(havePlusMinus)
              floatPlusMinus = determineYear(right);

            else
             floatRight = determineYear(right);

            if(RTM_Debug)
            {
                fprintf(fout, "number Right: %f  +/-: %f\n", floatRight,
                        floatPlusMinus);
                fflush(fout);
            } /* fi */
        } /* fi */

        if(havePlusMinus)
        {
            begYear = floatLeft - floatPlusMinus;
            endYear = floatLeft + floatPlusMinus;
        } /* fi */

        else
        {
            begYear = floatLeft;
            if(floatRight > 0.0)
              endYear = floatRight;
            else
              endYear = begYear;
        } /* else */

        if(begYear < 0)
          begYear = floatLeft;

        if(endYear < 0.0)
          endYear = 0.0;

        if(RTM_Debug)
        {
            fprintf(fout, "begYear: %f  endYear: %f\n", begYear, endYear);
            fflush(fout);
        } /* fi */

        if(begYear > endYear)
        {
            if(RTM_Debug)
            {
                fprintf(fout, "ERROR: begYear > endYear\n");
                fflush(fout);
            } /* fi */
            begYear = endYear = -1.0;
        } /* fi */

        else if((begYear > 150.0) || (endYear > 150.0))
        {
            if(RTM_Debug)
            {
                fprintf(fout, "ERROR: begYear or endYear > 150\n");
                fflush(fout);
            } /* fi */
            begYear = endYear = -1.0;
        } /* else fi */

        if(RTM_Debug)
        {
            fprintf(fout, 
                 "parseYears - begYear: %f  endYear: %f  snippet: #%s#\n", 
                 begYear, endYear, snippet);
            fflush(fout);
        } /* fi */

        /* If these are months so we need to divide by 12 before sending on. */

        foundPos = isTriggerInText("months", snippet);
        if(foundPos == -1)
          foundPos = isTriggerInText("month", snippet);
        if(foundPos == -1)
          foundPos = isTriggerInText("mos", snippet);

        if(foundPos > -1)
        {
            if(begYear > -1)
              begYear = begYear / 12.0;

            if(endYear > -1)
              endYear = endYear / 12.0;

            if(RTM_Debug)
            {
                fprintf(fout, "begMonth: %f  endMonth: %f\n", begYear, endYear);
                fflush(fout);
            } /* fi */
        } /* fi month/months */

        /* If these are weeks so we need to divide by 52 before sending on. */

        foundPos = isTriggerInText("weeks", snippet);
        if(foundPos == -1)
          foundPos = isTriggerInText("wks", snippet);

        if(foundPos > -1)
        {
            if(begYear > -1)
              begYear = begYear / 52.0;

            if(endYear > -1)
              endYear = endYear / 22.0;

            if(RTM_Debug)
            {
                fprintf(fout, "begWeek: %f  endWeek: %f\n", begYear, endYear);
                fflush(fout);
            } /* fi */
        } /* fi weeks/wks */
    } /* fi */

    else if(RTM_Debug)
    {
        fprintf(fout, "ERR - Bad years: #%s#\n", years);
        fflush(fout);
    } /* else */


    if(RTM_Debug)
    {
        fprintf(fout, 
                "parseYears End - begYear: %f  endYear: %f  snippet: #%s#\n", 
                begYear, endYear, snippet);
            fflush(fout);
    } /* fi */
} /* parseYears */

/***************************************************************************
*
*  determineYear --
*
*      This 
*
***************************************************************************/

float determineYear(char *str)
{
    float rtn;

    rtn = -1.0;
    sscanf(str, "%f", &rtn);

    return(rtn);
} /* determineYear */



/***************************************************************************
*
*  checkBeforeFinalPrint --
*
*      This 
*
***************************************************************************/

void checkBeforeFinalPrint()
{
   int foundHumans, foundAnimals, foundInBreeding, foundConsanguinity,
       foundParents, foundPreliminaryData, done, foundUS, foundMEDICARE,
       foundMice, foundRats, foundArabidopsis, foundDrosophila,
       foundScerevisiae, foundCelegans, foundEcoli, foundZebrafish,
       foundXenopus, foundSpombe, foundAD, foundND, foundDementia,
       foundDiseases, foundAnimalDiseases, foundAgeRelated, ok, foundMaleFemale,
       foundPlants, foundPlantBreeding, foundLegendary, foundCongo, foundDCR,
       foundRuminant, foundBreeding, foundFemale, foundProstateCancer,
       foundNeoplasms, foundTumorMicroEnv, foundPancreatitis, foundPancChld,
       foundDiabetesM, foundDiabetesChild, foundDecompression, foundDecompSurg,
       haveSinging, foundVocalAnimal, foundAging, proceed, foundPregnancy,
       foundCOVID_TREAT, foundCOVID_Drug, found_OtherCOVID, foundCOVID_Testing,
       foundPregnancy_Infectious, foundCOVID, foundHyperthermia, foundHyperthermiaLocal,
       foundFatalOutcome, foundCR, foundBreastNeoplasm, foundMale, yesAdd,
       foundPuncture, foundSpinalPuncture, foundPedigree;

   long i, j, l, p, inBreedingPos, consanguinityPos, parentsPos, prelimPos,
        NDpos, DemPos, plantBreedingPos, legendaryPos, congoPos, ruminantPos,
        breedingPos, pos, femalePos, posMicroEnv, pancreatitisPos, decompPos, malePos,
        singingPos, vocAnimalPos, DiabetesMPos, agingPos, topLimit, numFound,
        panChldPos, covidDrugPos, hyperthermiaPos, fatalOutcomePos, pregnancyPos,
        puncturePos, spinalPuncturePos, pedigreePos;

   char *tmp, tmpMH[SMALL_LINE+ 1], mh[SMALL_LINE + 1], from[SMALL_LINE + 1];

   foundHumans = foundAnimals = foundUS = foundMice = foundRats = FALSE;
   foundArabidopsis = foundDrosophila = foundScerevisiae = FALSE;
   foundCelegans = foundEcoli = foundZebrafish = FALSE;
   foundXenopus = foundSpombe = foundCongo = foundDCR = FALSE;
   foundInBreeding = foundConsanguinity = foundRuminant = FALSE;
   foundParents = foundPreliminaryData = foundMEDICARE = FALSE;
   foundAD = foundND = foundDementia = foundMaleFemale = FALSE;
   foundDiseases = foundAnimalDiseases = foundAgeRelated = FALSE;
   foundPlants = foundPlantBreeding = foundLegendary = FALSE;
   foundBreeding = foundFemale = foundProstateCancer = foundNeoplasms = FALSE;
   foundTumorMicroEnv = foundPancreatitis = foundPancChld = FALSE;
   foundDiabetesM = foundDiabetesChild = foundDecompression = FALSE;
   foundDecompSurg = haveSinging = foundVocalAnimal = foundAging = FALSE;
   foundCOVID_TREAT = foundCOVID_Drug = found_OtherCOVID = foundCOVID = FALSE;
   foundPregnancy = foundCOVID_Testing = foundPregnancy_Infectious = FALSE;
   foundHyperthermia = foundHyperthermiaLocal = foundFatalOutcome = FALSE;
   foundBreastNeoplasm = foundMale = foundPuncture = foundSpinalPuncture = FALSE;
   foundPedigree = FALSE;

   inBreedingPos = consanguinityPos = parentsPos = prelimPos = -1;
   NDpos = DemPos = plantBreedingPos = legendaryPos = congoPos = -1;
   breedingPos = femalePos = posMicroEnv = pancreatitisPos = malePos = -1;
   decompPos = singingPos = vocAnimalPos = DiabetesMPos = agingPos = -1;
   panChldPos = covidDrugPos = hyperthermiaPos = fatalOutcomePos = pregnancyPos = -1;
   puncturePos = spinalPuncturePos = pedigreePos = -1;

   for(i = 0; i < numFinalCTs; i++)
   {
       if(finalCTlist[i].oktoprint)
       {
            if(finalCTlist[i].mh[0] == 'A')
            {
                if(strcmp(finalCTlist[i].mh, "Animals") == 0)
                  foundAnimals = TRUE;

                else if((strcmp(finalCTlist[i].mh, "Adolescent") == 0) ||
                        (strcmp(finalCTlist[i].mh, "Adult") == 0) ||
                        (strcmp(finalCTlist[i].mh, "Aged") == 0) ||
                        (strcmp(finalCTlist[i].mh, "Aged, 80 and over") == 0))
                  foundAgeRelated = TRUE;
            } /* fi 'A' */

            else if(finalCTlist[i].mh[0] == 'C')
            {
                if((strcmp(finalCTlist[i].mh, "Child") == 0) ||
                   (strcmp(finalCTlist[i].mh, "Child, Preschool") == 0))
                  foundAgeRelated = TRUE;
            } /* else fi 'C' */

            else if(finalCTlist[i].mh[0] == 'F')
            {
                if(strcmp(finalCTlist[i].mh, "Female") == 0)
                {
                    foundMaleFemale = TRUE;
                    femalePos = i;
                    foundFemale = TRUE;
                } /* fi */
            } /* else fi 'F' */

            else if(finalCTlist[i].mh[0] == 'H')
            {
                if(strcmp(finalCTlist[i].mh, "Humans") == 0)
                  foundHumans = TRUE;
            } /* else fi 'H' */

            else if(finalCTlist[i].mh[0] == 'I')
            {
                if((strcmp(finalCTlist[i].mh, "Infant") == 0) ||
                   (strcmp(finalCTlist[i].mh, "Infant, Newborn") == 0))
                  foundAgeRelated = TRUE;
            } /* else fi 'I' */

            else if(finalCTlist[i].mh[0] == 'M')
            {
                if(strcmp(finalCTlist[i].mh, "Mice") == 0)
                  foundMice = TRUE;

                else if(strcmp(finalCTlist[i].mh, "Middle Aged") == 0)
                  foundAgeRelated = TRUE;

                else if(strcmp(finalCTlist[i].mh, "Male") == 0)
                {
                    foundMaleFemale = TRUE;
                    foundMale = TRUE;
                    malePos = i;
                } /* else fi */
            } /* else fi 'M' */

            else if(finalCTlist[i].mh[0] == 'P')
            {
                if(strcmp(finalCTlist[i].mh, "Pregnancy") == 0)
                {
                     foundPregnancy = TRUE;
                     pregnancyPos = i;
                } /* fi */
            } /* else fi 'R' */

            else if(finalCTlist[i].mh[0] == 'R')
            {
                if(strcmp(finalCTlist[i].mh, "Rats") == 0)
                  foundRats = TRUE;
            } /* else fi 'R' */

            else if(finalCTlist[i].mh[0] == 'U')
            {
                if(strcmp(finalCTlist[i].mh, "United States") == 0)
                  foundUS = TRUE;
            } /* else fi 'U' */

            else if(finalCTlist[i].mh[0] == 'Y')
            {
                if(strcmp(finalCTlist[i].mh, "Young Adult") == 0)
                  foundAgeRelated = TRUE;
            } /* else fi 'U' */
       } /* fi oktoprint */
   } /* for */

   /* If we didn't find all 11 of the Model Organisms, we need to go through the
      MeSH Heading recommendations since not required to add if specific
      term used in some cases (Mice/Rats).  In other cases, we don't care where
      in the tre the term comes from as long as it is at or below the species
      level (e.g., Celegans or treed below Celegans).

      Mice       : B01.050.150.900.649.313.992.635.505.500
      Rats       : B01.050.150.900.649.313.992.635.505.700
      Arabidopsis: B01.650.940.800.575.912.250.157.100
      Drosophila : B01.050.500.131.617.720.500.500.750.310.250
      Scerevisiae: B01.300.107.795.785.800; B01.300.930.705.655
      Celegans   : B01.050.500.500.294.400.875.660.250.250
      Ecoli      : B03.440.450.425.325.300; B03.660.250.150.180.100
      Zebrafish  : B01.050.150.900.493.200.244.828
      Xenopus    : B01.050.150.900.090.180.610.500
      Spombe     : B01.300.107.797; B01.300.930.720
   */

   for(i = 0; i < numFinalMHs; i++)
   {
       if(strstr(finalMHlist[i].trigger, "Entry Term Replacement") != NULL)
         strcpy(mh, finalMHlist[i].mh_orig);

       else
         strcpy(mh, finalMHlist[i].mh);

       if((finalMHlist[i].datatype == NM) && finalMHlist[i].oktoprint)
       {
            if((strstr(mh, "COVID") != NULL) ||
               (strstr(mh, "SARS-CoV-2") != NULL) ||
               (strstr(mh, "coronavirus 2") != NULL) ||
               (strstr(mh, "nucleocapsid protein, Coronavirus") != NULL))
              found_OtherCOVID = TRUE;

            if(mh[0] == 'C')
            {
                if(strcmp(mh, "COVID-19 drug treatment") == 0)
                  foundCOVID_TREAT = TRUE;

                else if(strcmp(mh, "COVID-19 diagnostic testing") == 0)
                  foundCOVID_Testing = TRUE;

                else if(strcmp(mh, "COVID-19") == 0)
                  foundCOVID = TRUE;
            } /* fi */

            if(mh[0] == 'r')
            {
                if(strcmp(mh, "remdesivir") == 0)
                {
                   foundCOVID_Drug = TRUE;
                   covidDrugPos = i;
                } /* else fi */
            } /* fi */
       } /* fi valid NMs */


       if(((finalMHlist[i].datatype == MH) || (finalMHlist[i].datatype == HM)) &&
           finalMHlist[i].oktoprint)
       {
            if(mh[0] == 'A')
            {
                if(strcmp(mh, "Alzheimer Disease") == 0)
                  foundAD = TRUE;

                else if(strcmp(mh, "Aging") == 0)
                {
                    foundAging = TRUE;
                    agingPos = i;
                } /* else fi */

                else if(strcmp(mh, "Alanine") == 0)
                {
                   foundCOVID_Drug = TRUE;
                   covidDrugPos = i;
                } /* else fi */

                else if(strcmp(mh, "Azithromycin") == 0)
                {
                   foundCOVID_Drug = TRUE;
                   covidDrugPos = i;
                } /* else fi */
            } /* fi 'A' */

            else if(mh[0] == 'B')
            {
                if(strcmp(mh, "Breeding") == 0)
                {
                    foundBreeding = TRUE;
                    breedingPos = i;
                } /* fi */

                else if(strcmp(mh, "Breast Neoplasms") == 0)
                  foundBreastNeoplasm = TRUE;
            } /* else fi 'B' */

            else if(mh[0] == 'C')
            {
                if(strcmp(mh, "Congo") == 0)
                {
                    foundCongo = TRUE;
                    congoPos = i;
                } /* fi */

                else if(strcmp(mh, "Chloroquine") == 0)
                {
                   foundCOVID_Drug = TRUE;
                   covidDrugPos = i;
                } /* else fi */
            } /* else fi 'C' */

            else if(mh[0] == 'D')
            {
                if(strcmp(mh, "Dementia") == 0)
                {
                    foundDementia = TRUE;
                    DemPos = i;
                } /* fi */

                else if(strcmp(mh, "Democratic Republic of the Congo") == 0)
                  foundDCR = TRUE;

                else if(strcmp(mh, "Diabetes Mellitus") == 0)
                {
                    foundDiabetesM = TRUE;
                    DiabetesMPos = i;
                } /* else fi */

                else if(strcmp(mh, "Decompression") == 0)
                {
                    foundDecompression = TRUE;
                    decompPos = i;
                } /* fi */

                else if(strcmp(mh, "Drugs, Chinese Herbal") == 0)
                {
                   foundCOVID_Drug = TRUE;
                   covidDrugPos = i;
                } /* else fi */

                else if(strcmp(mh, "Dexamethasone") == 0)
                {
                   foundCOVID_Drug = TRUE;
                   covidDrugPos = i;
                } /* else fi */

                else if(strcmp(mh, "Decompression, Surgical") == 0)
                  foundDecompSurg = TRUE;
            } /* else fi 'D' */

            else if(mh[0] == 'F')
            {
                if(strcmp(mh, "Fever") == 0)
                {
                   foundHyperthermia = TRUE;
                   hyperthermiaPos = i;
                } /* fi */

                else if(strcmp(mh, "Fatal Outcome") == 0)
                {
                   foundFatalOutcome = TRUE;
                   fatalOutcomePos = i;
                } /* else fi */
            } /* else fi 'F' */

            else if(mh[0] == 'H')
            {
                if(strcmp(mh, "Hydroxychloroquine") == 0)
                {
                   foundCOVID_Drug = TRUE;
                   covidDrugPos = i;
                } /* fi */

                else if(strcmp(mh, "Hyperthermia, Induced") == 0)
                  foundHyperthermiaLocal = TRUE;
            } /* else fi 'H' */

            else if(mh[0] == 'I')
            {
                if(strcmp(mh, "Inbreeding") == 0)
                {
                    foundInBreeding = TRUE;
                    inBreedingPos = i;
                } /* fi */
            } /* else fi 'I' */

            else if(mh[0] == 'L')
            {
                if(strcmp(mh, "Legendary Creatures") == 0)
                {
                    foundLegendary = TRUE;
                    legendaryPos = i;
                } /* fi */

                else if(strcmp(mh, "Lopinavir") == 0)
                {
                   foundCOVID_Drug = TRUE;
                   covidDrugPos = i;
                } /* else fi */
            } /* else fi 'L' */

            else if(mh[0] == 'N')
            {
                if(strcmp(mh, "Neurodegenerative Diseases") == 0)
                {
                    foundND = TRUE;
                    NDpos = i;
                } /* fi */
            } /* else fi 'N' */

            else if(mh[0] == 'P')
            {
                if(strcmp(mh, "Prostatic Neoplasms") == 0)
                  foundProstateCancer = TRUE;

                else if(strcmp(mh, "Plant Breeding") == 0)
                {
                    foundPlantBreeding = TRUE;
                    plantBreedingPos = i;
                } /* else fi */

                else if(strcmp(mh, "Punctures") == 0)
                {
                    foundPuncture = TRUE;
                    puncturePos = i;
                } /* else fi */

                else if(strcmp(mh, "Pancreatitis") == 0)
                {
                    foundPancreatitis = TRUE;
                    pancreatitisPos = i;
                } /* else fi */

                else if(strcmp(mh, "Pedigree") == 0)
                {
                    foundPedigree = TRUE;
                    pedigreePos = i;
                } /* else fi */

                else if(strcmp(mh, "Pregnancy Complications, Infectious") == 0)
                  foundPregnancy_Infectious = TRUE;

                else if(strstr(mh, "Pancreatitis,") != NULL)
                {
                    foundPancChld = TRUE;
                    panChldPos = i;
                } /* else fi */
            } /* else fi 'P' */

            else if(mh[0] == 'R')
            {
                if(strcmp(mh, "Rumination, Cognitive") == 0)
                {
                    foundRuminant = TRUE;
                    ruminantPos = i;
                } /* fi */

                else if(strcmp(mh, "Ritonavir") == 0)
                {
                   foundCOVID_Drug = TRUE;
                   covidDrugPos = i;
                } /* else fi */
            } /* else fi 'R' */

            else if(mh[0] == 'S')
            {
                if(strcmp(mh, "Singing") == 0)
                {
                    haveSinging = TRUE;
                    singingPos = i;
                } /* fi */

                else if(strcmp(mh, "Spinal Puncture") == 0)
                {
                    foundSpinalPuncture = TRUE;
                    spinalPuncturePos = i;
                } /* fi */
            } /* else fi 'S' */

            else if(mh[0] == 'T')
            {
                if(strcmp(mh, "Tumor Microenvironment") == 0)
                {
                    foundTumorMicroEnv = TRUE;
                    posMicroEnv = i;
                } /* fi */
            } /* else fi 'T' */

            else if(mh[0] == 'V')
            {
                if(strcmp(mh, "Vocalization, Animal") == 0)
                {
                    foundVocalAnimal = TRUE;
                    vocAnimalPos = i;
                } /* fi */
            } /* else fi 'V' */

           for(l = 0; (l < finalMHlist[i].num_treecodes); l++)
           {
               if(finalMHlist[i].treecodes[l][0] == 'A')
               {
                   if(strstr(finalMHlist[i].treecodes[l], "A18") != NULL)
                     foundPlants = TRUE;
               } /* fi A */

               else if(finalMHlist[i].treecodes[l][0] == 'B')
               {
                   if(strstr(finalMHlist[i].treecodes[l],
                         "B01.050.150.900.649.313.992.635.505.500") != NULL)
                     foundMice = TRUE;

                   else if(strstr(finalMHlist[i].treecodes[l],
                         "B01.050.150.900.649.313.992.635.505.700") != NULL)
                     foundRats = TRUE;

                   else if(strstr(finalMHlist[i].treecodes[l],
                         "B01.650.940.800.575.912.250.157.100") != NULL)
                     foundArabidopsis = TRUE;

                   else if(strstr(finalMHlist[i].treecodes[l],
                         "B01.050.500.131.617.720.500.500.750.310.250") != NULL)
                     foundDrosophila = TRUE;

                   else if((strstr(finalMHlist[i].treecodes[l],
                               "B01.300.107.795.785.800") != NULL) ||
                           (strstr(finalMHlist[i].treecodes[l],
                               "B01.300.930.705.655") != NULL))
                     foundScerevisiae = TRUE;

                   else if(strstr(finalMHlist[i].treecodes[l],
                         "B01.050.500.500.294.400.875.660.250.250") != NULL)
                     foundCelegans = TRUE;

                   else if((strstr(finalMHlist[i].treecodes[l],
                               "B03.440.450.425.325.300") != NULL) ||
                           (strstr(finalMHlist[i].treecodes[l],
                               "B03.660.250.150.180.100") != NULL))
                     foundEcoli = TRUE;

                   else if(strstr(finalMHlist[i].treecodes[l],
                         "B01.050.150.900.493.200.244.828") != NULL)
                     foundZebrafish = TRUE;

                   else if(strstr(finalMHlist[i].treecodes[l],
                         "B01.050.150.900.090.180.610.500") != NULL)
                     foundXenopus = TRUE;

                   else if((strstr(finalMHlist[i].treecodes[l],
                               "B01.300.107.797") != NULL) ||
                           (strstr(finalMHlist[i].treecodes[l],
                               "B01.300.930.720") != NULL))
                     foundSpombe = TRUE;

                   else if(strstr(finalMHlist[i].treecodes[l], "B01.650") != NULL)
                     foundPlants = TRUE;
               } /* else fi "B" */

               else if(finalMHlist[i].treecodes[l][0] == 'C')
               {
                   if(strstr(finalMHlist[i].treecodes[l], "C22") != NULL)
                   {
                       if(strstr(finalMHlist[i].treecodes[l], "C22.674.710") == NULL)
                         foundAnimalDiseases = TRUE;
                   } /* fi */

                   else
                   {
                       if(RTM_Debug)
                       {
                           fprintf(fout, "foundDiseases: %s (%s)\n", finalMHlist[i].mh,
                                   finalMHlist[i].treecodes[l]);
                           fflush(fout);
                       } /* fi */
                       foundDiseases = TRUE;
                   } /* else */

                   if(strstr(finalMHlist[i].treecodes[l], "C04") != NULL)
                     foundNeoplasms = TRUE;

                   if(strstr(finalMHlist[i].treecodes[l],
                             "C18.452.394.750.") != NULL)
                     foundDiabetesChild = TRUE;
               } /* else fi */

               /* For purpose of adding Humans, F03 considered diseases
                  as well as the "C" tree.  6/20/2018.
               */

               else if(finalMHlist[i].treecodes[l][0] == 'F')
               {
                   if(strstr(finalMHlist[i].treecodes[l], "F03") != NULL)
                   {
                       if(RTM_Debug)
                       {
                           fprintf(fout, "foundDiseases: %s (%s)\n", finalMHlist[i].mh,
                                   finalMHlist[i].treecodes[l]);
                           fflush(fout);
                       } /* fi */
                       foundDiseases = TRUE;
                   } /* else */
               } /* else fi */
           } /* for  each treecode */
       } /* fi */
   } /* for */

   /* See Inbreeding vs Plant Breeding file for code that goes here if we
      need it back.

       Inbreeding_vs_Plant_Breeding
   */

   if(!foundHumans)
   {
       parentsPos = -1;
       foundParents = FALSE;
       for(i = 0; i < numFinalMHs; i++)
       {
            if(finalMHlist[i].oktoprint)
            {
                 if(finalMHlist[i].ETflag)
                 {
                     if(finalMHlist[i].mh_orig[0] == 'P')
                     {
                         if(strcmp(finalMHlist[i].mh_orig, "Parents") == 0)
                         {
                             foundParents = TRUE;
                             parentsPos = i;
                         } /* fi */
                     } /* fi */
                 } /* fi ET */

                 else
                 {
                     if(finalMHlist[i].mh[0] == 'P')
                     {
                         if(strcmp(finalMHlist[i].mh, "Parents") == 0)
                         {
                             foundParents = TRUE;
                             parentsPos = i;
                         } /* fi */
                     } /* fi */
                 } /* else */
            } /* fi oktoprint */
       } /* for */

       /* If no Humans and we have Parents, turn it off */

       if(!foundHumans && foundParents)
         finalMHlist[parentsPos].oktoprint = FALSE;
   } /* fi !foundHumans */

   /* If we are processing MTIA (or anything with PTs AND we have Fatal Outcome indexed,
      make sure Case Reports is indexed.  If not, remove Fatal Outcome.
   */

   if(MTI_AUTO && foundFatalOutcome && (fatalOutcomePos > -1))
   {
       foundCR = FALSE;
       for(i = 0; !foundCR && (i < numPTsFound); i++)
       {
           if(foundPTsList[i].mh[0] == 'C')
           {
               if(strcmp(foundPTsList[i].mh, "Case Reports") == 0)
                 foundCR = TRUE;
           } /* fi */
       } /* for */

       if(!foundCR)  /* Remove Fatal Outcome */
       {
           if(RTM_Debug)
           {
               fprintf(fout, "Removing Fatal Outcome - Case Reports not found\n");
               fflush(fout);
           } /* fi */

           finalMHlist[fatalOutcomePos].oktoprint = FALSE;
           finalMHlist[fatalOutcomePos].score = 0;
       } /* fi */
   } /* fi */

   /* If we have Preliminary Data - 2018 Introduced it should be removed
      since it should not be recommended very often per Olga.
   */

   if(foundPreliminaryData && (prelimPos > -1))
     finalMHlist[prelimPos].oktoprint = FALSE;

   /* If we have both Fever (Hyperthermia) and Hyperthermia, Local remove Hyperthermia
   */

   if(foundHyperthermia && foundHyperthermiaLocal && (hyperthermiaPos > -1))
     finalMHlist[hyperthermiaPos].oktoprint = FALSE;

   /* If we have Legendary Creatures - remove it.  1/10/19 */

   if(foundLegendary && (legendaryPos > -1))
     finalMHlist[legendaryPos].oktoprint = FALSE;

    /* If we have Pancreatitis and have any of the more specific children
       MeSH Headings, we want to remove Pancreatitis.
    */

    if(foundPancreatitis && foundPancChld && (pancreatitisPos > -1))
    {
        if(RTM_Debug)
        {
            fprintf(fout, "Removing Pancreatitis - more specific (%s)\n",
                     finalMHlist[panChldPos].mh);
            fflush(fout);
        } /* fi */

        finalMHlist[pancreatitisPos].oktoprint = FALSE;
    } /* fi foundPancreatitis */

    /* If we have Decompression and Decompression, Surgical, 
       remove Decompression.
    */

    if(foundDecompression && foundDecompSurg && (decompPos > -1))
    {
        if(RTM_Debug)
        {
            fprintf(fout, "Removing Decompression - more specific\n");
            fflush(fout);
        } /* fi */

        finalMHlist[decompPos].oktoprint = FALSE;
    } /* fi foundPancreatitis */

   /* If we have Animals and Rumination, Cognitive, remove the term */

   if(foundAnimals && foundRuminant && (ruminantPos > -1))
     finalMHlist[ruminantPos].oktoprint = FALSE;

   /* Check to see if we are recommending Alzheimer Disease and we also
      have Dementia and/or Neurodegenerative Diseases - if so, remove them
      IF they are not being forced by something in text.
   */

   if(foundAD && (foundND || foundDementia))
   {
       if(foundND && (NDpos > -1))
       {
           if(!finalMHlist[NDpos].wasForced)
             finalMHlist[NDpos].oktoprint = FALSE;
       } /* fi */

       if(foundDementia && (DemPos > -1))
       {
           if(!finalMHlist[DemPos].wasForced)
             finalMHlist[DemPos].oktoprint = FALSE;
       } /* fi */
   } /* fi */

   /* If we have Breast Neoplasms and no indication of Male, then force Female CT */

   if(foundBreastNeoplasm && !foundMale && !foundFemale)
   {
       foundFemale = TRUE;
       p = numFinalCTs;
       finalCTlist[p].foundInL2R = FALSE;
       finalCTlist[p].ETflag = FALSE;
       finalCTlist[p].datatype = CT;
       finalCTlist[p].oktoprint = TRUE;
       finalCTlist[p].score = 1000;
       finalCTlist[p].num_RCs = 0;
       finalCTlist[p].VocabDenFactor = 0.0;
       finalCTlist[p].muid = strdup(muid);
       finalCTlist[p].mh = strdup("Female");
       finalCTlist[p].mh_orig = strdup("");
       finalCTlist[p].dui = strdup("D005260");
       finalCTlist[p].entry_term = strdup("");
       finalCTlist[p].RC_PMIDs = strdup("");
       finalCTlist[p].textloc = strdup("TI");
       finalCTlist[p].trigger = strdup("Forced Breast Neoplasms");

       /* Positional Information */

       finalCTlist[p].numPIs = 0;

       /* Paths */

       finalCTlist[p].paths[MMI] = TRUE;
       finalCTlist[p].paths[PRC] = FALSE;

       /* Treecodes */

       finalCTlist[p].num_treecodes = 0;

       /* Semantic Types - max 25 */

       finalCTlist[p].num_STs = 0;

       numFinalCTs++;
   } /* fi */

   /* Check to see if we are recommending Female on an article about
      Prostate Cancer and if so, remove it.
   */

   if(foundProstateCancer && foundFemale)
   {
       if(foundFemale && (femalePos > -1))
         finalCTlist[femalePos].oktoprint = FALSE;

       /* If Male is not already recommended, add it. */

       if(!foundMale)
       {
           foundMale = TRUE;
           p = numFinalCTs;
           finalCTlist[p].foundInL2R = FALSE;
           finalCTlist[p].ETflag = FALSE;
           finalCTlist[p].datatype = CT;
           finalCTlist[p].oktoprint = TRUE;
           finalCTlist[p].score = 1000;
           finalCTlist[p].num_RCs = 0;
           finalCTlist[p].VocabDenFactor = 0.0;
           finalCTlist[p].muid = strdup(muid);
           finalCTlist[p].mh = strdup("Male");
           finalCTlist[p].mh_orig = strdup("");
           finalCTlist[p].dui = strdup("D008297");
           finalCTlist[p].entry_term = strdup("");
           finalCTlist[p].RC_PMIDs = strdup("");
           finalCTlist[p].textloc = strdup("TI");
           finalCTlist[p].trigger = strdup("Forced Prostate Cancer");

           /* Positional Information */

           finalCTlist[p].numPIs = 0;

           /* Paths */

           finalCTlist[p].paths[MMI] = TRUE;
           finalCTlist[p].paths[PRC] = FALSE;

           /* Treecodes */

           finalCTlist[p].num_treecodes = 0;

           /* Semantic Types - max 25 */

           finalCTlist[p].num_STs = 0;

           numFinalCTs++;
       } /* fi !Male */
   } /* fi */

   /* Check to see if we are recommending both Congo and 
      Democratic Republic of the Congo and remove Congo if so.
   */

   if(foundDCR && (foundCongo && (congoPos > -1)))
     finalMHlist[congoPos].oktoprint = FALSE;

   /* Check to see if Aging should be recommended - only if Animals or Humans */

   if(!foundHumans && !foundAnimals && foundAging && (agingPos > -1))
     finalMHlist[agingPos].oktoprint = FALSE;

   /* Check to see if Individuals is the only trigger for Humans and that
      we have Animals -- if so, remove Humans.
   */

   if(foundHumans && foundAnimals)
   {
       done = FALSE;
       for(i = 0; i < numFinalCTs; i++)
       {
           if(finalCTlist[i].oktoprint && 
              (strcmp(finalCTlist[i].mh, "Humans") == 0))
           {
               done = TRUE;
               if(strcmp(finalCTlist[i].trigger,
                         "CT Text Lookup: individuals") == 0)
               {
                   finalCTlist[i].oktoprint = FALSE;
                   foundHumans = FALSE;
                   if(RTM_Debug)
                   {
                       fprintf(fout,
                           "Removing Humans - only trigger Individuals\n");
                       fflush(fout);
                   } /* fi */
               } /* fi */
           } /* fi */
       } /* for */
   } /* fi */

   /* If we have any of the Medicare terms - want to add United States
      since about 90% of the time that is what it will need.
   */
      
   if(foundMEDICARE && !foundUS)
   {
       p = numFinalCTs;
       finalCTlist[p].foundInL2R = FALSE;
       finalCTlist[p].ETflag = FALSE;
       finalCTlist[p].datatype = CT;
       finalCTlist[p].oktoprint = TRUE;
       finalCTlist[p].score = 1000;
       finalCTlist[p].num_RCs = 0;
       finalCTlist[p].VocabDenFactor = 0.0;
       finalCTlist[p].origPos = 10000;

       finalCTlist[p].muid = strdup(muid);
       finalCTlist[p].mh = strdup("United States");
       finalCTlist[p].mh_orig = strdup("");
       finalCTlist[p].dui = strdup("D014481");
       finalCTlist[p].entry_term = strdup("");
       finalCTlist[p].RC_PMIDs = strdup("");
       finalCTlist[p].textloc = strdup("TI");
       finalCTlist[p].trigger = strdup("Forced Medicare Found");

       /* Positional Information */

       finalCTlist[p].numPIs = 0;

       /* Paths */

       finalCTlist[p].paths[MMI] = TRUE;
       finalCTlist[p].paths[PRC] = FALSE;

       /* Treecodes */

       finalCTlist[p].num_treecodes = 0;
       pullTREEs_CT(p);

       /* Semantic Types - max 25 */

       finalCTlist[p].num_STs = 0;

       numFinalCTs++;
   } /* fi */

    /* If have Puncture being recommended but have "spinal" or "lumbar" in Title
       then change to Spinal Puncture.
       August 18, 2021.
    */
      
   if(foundPuncture && (puncturePos > -1))
   {
        if((strstr(globalTitle, "spinal") != NULL) || (strstr(globalTitle, "spine") != NULL) || 
           (strstr(globalTitle, "lumbar") != NULL))
        {
            if(foundSpinalPuncture && (spinalPuncturePos > -1))
            {
                if(RTM_Debug)
                {
                    fprintf(fout, "Removing Puncture keeping Spinal Puncture\n");
                    fflush(fout);
                } /* fi */
                finalMHlist[puncturePos].oktoprint = FALSE;
                finalMHlist[spinalPuncturePos].score += finalMHlist[puncturePos].score;
                if(finalMHlist[puncturePos].wasForced)
                  finalMHlist[spinalPuncturePos].wasForced = TRUE;
            } /* fi */

            else
            {
                finalMHlist[puncturePos].oktoprint = FALSE;
                if(RTM_Debug)
                {
                    fprintf(fout, "Replacing Puncture with Spinal Puncture\n");
                    fflush(fout);
                } /* fi */

                p = numFinalMHs;
                finalMHlist[p].foundInL2R = FALSE;
                finalMHlist[p].ETflag = FALSE;
                finalMHlist[p].datatype = MH;
                finalMHlist[p].oktoprint = TRUE;
                finalMHlist[p].score = 1000 + finalMHlist[puncturePos].score;
                finalMHlist[p].num_RCs = 0;
                finalMHlist[p].VocabDenFactor = 0.0;
                finalMHlist[p].origPos = 10000;
                if(finalMHlist[puncturePos].wasForced)
                  finalMHlist[p].wasForced = TRUE;

                finalMHlist[p].muid = strdup(muid);
                finalMHlist[p].mh = strdup("Spinal Puncture");
                finalMHlist[p].mh_orig = strdup("");
                finalMHlist[p].dui = strdup("D013129");
                finalMHlist[p].entry_term = strdup("");
                finalMHlist[p].RC_PMIDs = strdup("");
                finalMHlist[p].textloc = strdup("TI");
                finalMHlist[p].trigger = strdup("Replacing Puncture with Spinal Puncture");

                /* Positional Information */

                finalMHlist[p].numPIs = 0;

                /* Paths */

                finalMHlist[p].paths[MMI] = TRUE;
                finalMHlist[p].paths[PRC] = FALSE;

                /* Treecodes */

                finalMHlist[p].num_treecodes = 0;
                pullTREEs(p);

                /* Semantic Types - max 25 */

                finalMHlist[p].num_STs = 0;

                numFinalMHs++;
            } /* else */
        } /* fi in title */
   } /* fi */

   /* If we have diabetic or diabetes in the title and have not included
      any of the terms treed under Diabetes Mellitus, we want to add
      Diabetes Mellitus.  Check to see if we just need to turn back on
      or not.
   */

   if(!foundDiabetesM && !foundDiabetesChild)
   {
       if(foundInText("diabetic", FALSE, TRUE) ||
          foundInText("diabetes", FALSE, TRUE))
       {
           /* We don't already have Diabetes Mellitus */

           p = numFinalMHs;
           finalMHlist[p].foundInL2R = FALSE;
           finalMHlist[p].ETflag = FALSE;
           finalMHlist[p].datatype = MH;
           finalMHlist[p].oktoprint = TRUE;
           finalMHlist[p].score = 1000;
           finalMHlist[p].num_RCs = 0;
           finalMHlist[p].VocabDenFactor = 0.0;
           finalMHlist[p].origPos = 10000;

           finalMHlist[p].muid = strdup(muid);
           finalMHlist[p].mh = strdup("Diabetes Mellitus");
           finalMHlist[p].mh_orig = strdup("");
           finalMHlist[p].dui = strdup("D003920");
           finalMHlist[p].entry_term = strdup("");
           finalMHlist[p].RC_PMIDs = strdup("");
           finalMHlist[p].textloc = strdup("TI");
           finalMHlist[p].trigger = strdup("Forced Diabetes Found");

           /* Positional Information */

           finalMHlist[p].numPIs = 0;

           /* Paths */

           finalMHlist[p].paths[MMI] = TRUE;
           finalMHlist[p].paths[PRC] = FALSE;

           /* Treecodes */

           finalMHlist[p].num_treecodes = 0;
           pullTREEs(p);

           /* Semantic Types - max 25 */

           finalMHlist[p].num_STs = 0;

           numFinalMHs++;
       } /* fi diabetic || diabetes */
   } /* fi */

   /* If we have "covid" in the text and have one of the set drugs, make sure that
      COVID-19 Drug Treatment is indexed.

      Alanine
      Azithromycin
      Chloroquine
      Drugs, Chinese Herbal
      Hydroxychloroquine
      Lopinavir
      Ritonavir

      Dexamethasone
      remdesivir (SCR)
   */

   if(found_OtherCOVID && !foundCOVID_TREAT && foundCOVID_Drug)
   {
       p = numFinalMHs;
       finalMHlist[p].foundInL2R = FALSE;
       finalMHlist[p].ETflag = FALSE;
       finalMHlist[p].datatype = NM;
       finalMHlist[p].oktoprint = TRUE;
       finalMHlist[p].score = 1000;
       finalMHlist[p].num_RCs = 0;
       finalMHlist[p].VocabDenFactor = 0.0;
       finalMHlist[p].origPos = 10000;

       finalMHlist[p].muid = strdup(muid);
       finalMHlist[p].mh = strdup("COVID-19 drug treatment");
       finalMHlist[p].mh_orig = strdup("");
       finalMHlist[p].dui = strdup("C000705127");
       finalMHlist[p].entry_term = strdup("");
       finalMHlist[p].RC_PMIDs = strdup("");
       finalMHlist[p].textloc = strdup("TI");
       if(covidDrugPos > -1)
         sprintf(tmpMH, "Forced COVID Testing - COVID+Drug Found: %s\0",
                 finalMHlist[covidDrugPos].mh);
       else
         strcpy(tmpMH, "Forced COVID Testing - COVID+Drug Found");
       finalMHlist[p].trigger = strdup(tmpMH);

       /* Positional Information */

       finalMHlist[p].numPIs = 0;

       /* Paths */

       finalMHlist[p].paths[MMI] = TRUE;
       finalMHlist[p].paths[PRC] = FALSE;

       /* Treecodes */

       finalMHlist[p].num_treecodes = 0;
       pullTREEs(p);

       /* Semantic Types - max 25 */

       finalMHlist[p].num_STs = 0;

       numFinalMHs++;
   } /* fi */


   /* If we are indexing COVID term and we find one of the testing phrases in the title, we want
      to make sure that COVID-19 diagnostic testing is indexed.
   */

   if(foundCOVID && !foundCOVID_Testing)
   {
       if(foundInText("PCR", TRUE, TRUE) ||
          foundInText("antibody test", FALSE, TRUE) ||
          foundInText("antibody testing", FALSE, TRUE) ||
          foundInText("clinical testing", FALSE, TRUE) ||
          foundInText("covid 19 diagnostic testing", FALSE, TRUE) ||
          foundInText("covid 19 testing", FALSE, TRUE) ||
          foundInText("covid-19 diagnostic testing", FALSE, TRUE) ||
          foundInText("covid-19 testing", FALSE, TRUE) ||
          foundInText("covid19 diagnostic testing", FALSE, TRUE) ||
          foundInText("covid19 testing", FALSE, TRUE) ||
          foundInText("diagnostic test", FALSE, TRUE) ||
          foundInText("diagnostic testing", FALSE, TRUE) ||
          foundInText("diagnostic tool", FALSE, TRUE) ||
          foundInText("laboratory testing", FALSE, TRUE) ||
          foundInText("mass testing", FALSE, TRUE) ||
          foundInText("polymerase chain reaction", FALSE, TRUE) ||
          foundInText("serologic test", FALSE, TRUE) ||
          foundInText("serologic testing", FALSE, TRUE) ||
          foundInText("serological test", FALSE, TRUE) ||
          foundInText("serological testing", FALSE, TRUE) ||
          foundInText("serological-based covid-19 testing", FALSE, TRUE) ||
          foundInText("serology assay", FALSE, TRUE) ||
          foundInText("test for covid 19", FALSE, TRUE) ||
          foundInText("test for covid-19", FALSE, TRUE) ||
          foundInText("test for covid19", FALSE, TRUE) ||
          foundInText("testing for coronavirus", FALSE, TRUE) ||
          foundInText("testing for covid 19", FALSE, TRUE) ||
          foundInText("testing for covid-19", FALSE, TRUE) ||
          foundInText("testing for covid19", FALSE, TRUE) ||
          foundInText("testing of coronavirus", FALSE, TRUE) ||
          foundInText("testing of covid 19", FALSE, TRUE) ||
          foundInText("testing of covid-19", FALSE, TRUE) ||
          foundInText("testing of covid19", FALSE, TRUE) ||
          foundInText("testing patients for covid 19", FALSE, TRUE) ||
          foundInText("testing patients for covid-19", FALSE, TRUE) ||
          foundInText("testing patients for covid19", FALSE, TRUE) ||
          foundInText("testing platform", FALSE, TRUE) ||
          foundInText("testing policy", FALSE, TRUE) ||
          foundInText("testing rate", FALSE, TRUE) ||
          foundInText("tests for covid 19", FALSE, TRUE) ||
          foundInText("tests for covid-19", FALSE, TRUE) ||
          foundInText("tests for covid19", FALSE, TRUE) ||
          foundInText("universal screening", FALSE, TRUE) ||
          foundInText("diagnosing", FALSE, TRUE) ||
          foundInText("diagnosis", FALSE, TRUE) ||
          foundInText("diagnostic", FALSE, TRUE) ||
          foundInText("serologic", FALSE, TRUE))
       {
           p = numFinalMHs;
           finalMHlist[p].foundInL2R = FALSE;
           finalMHlist[p].ETflag = FALSE;
           finalMHlist[p].datatype = NM;
           finalMHlist[p].oktoprint = TRUE;
           finalMHlist[p].score = 1000;
           finalMHlist[p].num_RCs = 0;
           finalMHlist[p].VocabDenFactor = 0.0;
           finalMHlist[p].origPos = 10000;

           finalMHlist[p].muid = strdup(muid);
           finalMHlist[p].mh = strdup("COVID-19 diagnostic testing");
           finalMHlist[p].mh_orig = strdup("");
           finalMHlist[p].dui = strdup("C000657964");
           finalMHlist[p].entry_term = strdup("");
           finalMHlist[p].RC_PMIDs = strdup("");
           finalMHlist[p].textloc = strdup("TI");
           strcpy(tmpMH, "Forced COVID Testing - COVID+Testing Phrase Found");
           finalMHlist[p].trigger = strdup(tmpMH);

           /* Positional Information */

           finalMHlist[p].numPIs = 0;

           /* Paths */

           finalMHlist[p].paths[MMI] = TRUE;
           finalMHlist[p].paths[PRC] = FALSE;

           /* Treecodes */

           finalMHlist[p].num_treecodes = 0;
           pullTREEs(p);

           /* Semantic Types - max 25 */

           finalMHlist[p].num_STs = 0;

           numFinalMHs++;
       } /* fi */
   } /* fi */

   /* See if we have Pregnancy but not Female for some reason - not sure why but occassionally
      this seems to happen.
   */

   if(foundPregnancy && !foundFemale)
      addFemalePregnancy(pregnancyPos);

   /* If we have COVID & Pregnancy, make sure we have Pregnancy Complications, Infectious */

   if(found_OtherCOVID && foundPregnancy && !foundPregnancy_Infectious)
   {
       p = numFinalMHs;
       finalMHlist[p].foundInL2R = FALSE;
       finalMHlist[p].ETflag = FALSE;
       finalMHlist[p].datatype = MH;
       finalMHlist[p].oktoprint = TRUE;
       finalMHlist[p].score = 1000;
       finalMHlist[p].num_RCs = 0;
       finalMHlist[p].VocabDenFactor = 0.0;
       finalMHlist[p].origPos = 10000;

       finalMHlist[p].muid = strdup(muid);
       finalMHlist[p].mh = strdup("Pregnancy Complications, Infectious");
       finalMHlist[p].mh_orig = strdup("");
       finalMHlist[p].dui = strdup("D011251");
       finalMHlist[p].entry_term = strdup("");
       finalMHlist[p].RC_PMIDs = strdup("");
       finalMHlist[p].textloc = strdup("TI");
       strcpy(tmpMH, "Forced COVID Pregnancy Complications, Infectious");
       finalMHlist[p].trigger = strdup(tmpMH);

       /* Positional Information */

       finalMHlist[p].numPIs = 0;

       /* Paths */

       finalMHlist[p].paths[MMI] = TRUE;
       finalMHlist[p].paths[PRC] = FALSE;

       /* Treecodes */

       finalMHlist[p].num_treecodes = 0;
       pullTREEs(p);

       /* Semantic Types - max 25 */

       finalMHlist[p].num_STs = 0;

       numFinalMHs++;
   } /* fi */

   /* If we have any Neoplasms term being recommended and if we have
      microenvironment, micro-environment, or micro environment in the
      text, we want to recommend Tumor Microenvironment if it is not already
      in there.   If it is there, make sure it prints.
   */
      
   if(foundNeoplasms)
   {
       if(foundInText("microenvironment", FALSE, FALSE) ||
          foundInText("micro-environment", FALSE, FALSE) ||
          foundInText("micro environment", FALSE, FALSE))
       {
           /* If we already have it, then just make sure it prints */

           if(foundTumorMicroEnv && (posMicroEnv > -1))
             finalMHlist[posMicroEnv].oktoprint = TRUE;

           else /* Add it */
           {
               p = numFinalMHs;
               finalMHlist[p].foundInL2R = FALSE;
               finalMHlist[p].ETflag = FALSE;
               finalMHlist[p].datatype = MH;
               finalMHlist[p].oktoprint = TRUE;
               finalMHlist[p].score = 1000;
               finalMHlist[p].num_RCs = 0;
               finalMHlist[p].VocabDenFactor = 0.0;
               finalMHlist[p].origPos = 10000;

               finalMHlist[p].muid = strdup(muid);
               finalMHlist[p].mh = strdup("Tumor Microenvironment");
               finalMHlist[p].mh_orig = strdup("");
               finalMHlist[p].dui = strdup("D059016");
               finalMHlist[p].entry_term = strdup("");
               finalMHlist[p].RC_PMIDs = strdup("");
               finalMHlist[p].textloc = strdup("TI");
               finalMHlist[p].trigger = strdup("Forced MicroEnv Found");

               /* Positional Information */

               finalMHlist[p].numPIs = 0;

               /* Paths */

               finalMHlist[p].paths[MMI] = TRUE;
               finalMHlist[p].paths[PRC] = FALSE;

               /* Treecodes */

               finalMHlist[p].num_treecodes = 0;
               pullTREEs(p);

               /* Semantic Types - max 25 */

               finalMHlist[p].num_STs = 0;

               numFinalMHs++;
           } /* else add */
       } /* fi trigger found */
   } /* fi found Neoplasms */

   /* If we don't find any Neoplasms being indexed, check to see if "cancer" in title
      and add in Neoplasms if found.
   */

   else /* Did not find any Neoplasms */
   {
       if(foundInText("cancer", FALSE, TRUE) ||
          foundInText("neoplasm", FALSE, TRUE))
       {
           p = numFinalMHs;
           finalMHlist[p].foundInL2R = FALSE;
           finalMHlist[p].ETflag = FALSE;
           finalMHlist[p].datatype = MH;
           finalMHlist[p].oktoprint = TRUE;
           finalMHlist[p].score = 1000;
           finalMHlist[p].num_RCs = 0;
           finalMHlist[p].VocabDenFactor = 0.0;
           finalMHlist[p].origPos = 10000;

           finalMHlist[p].muid = strdup(muid);
           finalMHlist[p].mh = strdup("Neoplasms");
           finalMHlist[p].mh_orig = strdup("");
           finalMHlist[p].dui = strdup("D009369");
           finalMHlist[p].entry_term = strdup("");
           finalMHlist[p].RC_PMIDs = strdup("");
           finalMHlist[p].textloc = strdup("TI");
           finalMHlist[p].trigger = strdup("Forced Cancer Found");

           /* Positional Information */

           finalMHlist[p].numPIs = 0;

           /* Paths */

           finalMHlist[p].paths[MMI] = TRUE;
           finalMHlist[p].paths[PRC] = FALSE;

           /* Treecodes */

           finalMHlist[p].num_treecodes = 0;
           pullTREEs(p);

           /* Semantic Types - max 25 */

           finalMHlist[p].num_STs = 0;

           numFinalMHs++;
       } /* add */
   } /* else */


    /* If have Singing and Animals, change to Vocalization, Animal
       If we already have Vocalization, Animal just make sure it prints.
       December 16, 2019.
    */
      
   if(foundAnimals && haveSinging && (singingPos > -1))
   {
       finalMHlist[singingPos].oktoprint = TRUE;

       if(foundVocalAnimal && (vocAnimalPos > -1))
       {
           finalMHlist[vocAnimalPos].oktoprint = TRUE;
           finalMHlist[vocAnimalPos].score += finalMHlist[singingPos].score;
       } /* fi */

       else /* Add it */
       {
           p = numFinalMHs;
           finalMHlist[p].foundInL2R = FALSE;
           finalMHlist[p].ETflag = FALSE;
           finalMHlist[p].datatype = MH;
           finalMHlist[p].oktoprint = TRUE;
           finalMHlist[p].score = 1000;
           finalMHlist[p].num_RCs = 0;
           finalMHlist[p].VocabDenFactor = 0.0;
           finalMHlist[p].origPos = 10000;

           finalMHlist[p].muid = strdup(muid);
           finalMHlist[p].mh = strdup("Vocalization, Animal");
           finalMHlist[p].mh_orig = strdup("");
           finalMHlist[p].dui = strdup("D014828");
           finalMHlist[p].entry_term = strdup("");
           finalMHlist[p].RC_PMIDs = strdup("");
           finalMHlist[p].textloc = strdup("TI");
           finalMHlist[p].trigger = strdup("Replacement of Singing - Animals");

           /* Positional Information */

           finalMHlist[p].numPIs = 0;

           /* Paths */

           finalMHlist[p].paths[MMI] = TRUE;
           finalMHlist[p].paths[PRC] = FALSE;

           /* Treecodes */

           finalMHlist[p].num_treecodes = 0;
           pullTREEs(p);

           /* Semantic Types - max 25 */

           finalMHlist[p].num_STs = 0;
           numFinalMHs++;
       } /* else add */

       finalMHlist[singingPos].score = 0;
   } /* fi */

   /* If we have plants, remove things like Male, Female, Pedigree
   */

   if(botanyJournal || foundPlants || foundPlantBreeding)
   {
       if(foundFemale && (femalePos > -1))
       {
            if(RTM_Debug)
            {
                fprintf(fout, "Removing Female - Plants Found\n");
                fflush(fout);
            } /* fi */
            finalCTlist[femalePos].oktoprint = FALSE;
            finalCTlist[femalePos].wasForced = FALSE;
       } /* if Female */

       if(foundMale && (malePos > -1))
       {
            if(RTM_Debug)
            {
                fprintf(fout, "Removing Male - Plants Found\n");
                fflush(fout);
            } /* fi */
            finalCTlist[malePos].oktoprint = FALSE;
            finalCTlist[malePos].wasForced = FALSE;
       } /* if Male */

       if(foundPedigree && (pedigreePos > -1))
       {
            if(RTM_Debug)
            {
                fprintf(fout, "Removing Pedigree - Plants Found\n");
                fflush(fout);
            } /* fi */
            finalMHlist[pedigreePos].oktoprint = FALSE;
            finalMHlist[pedigreePos].wasForced = FALSE;
       } /* if Male */
   } /* fi Plants */

   /* For Comments/MTI-AUTO
      If we have any disease (except C22) specified, add Humans
      If only have C22 Animals Diseases, add Animals.
      Don't add Humans if Veterinary Journal
      Added also insectsJournal botanyJournal and zootaxaJournal to exclude
   */

   if(hasCON || MTI_AUTO || MTI_REVIEW)
   {
       ok = FALSE;

       if(foundAnimalDiseases || veterinaryJournal || foundAnimals || 
          insectsJournal || zootaxaJournal || botanyJournal || foundPlants || foundPlantBreeding)
         ok = FALSE;
/*
       else if(!foundHumans && 
               (foundDiseases || foundAgeRelated || foundMaleFemale))
*/
       else if(!foundHumans && (foundDiseases || foundAgeRelated))
         ok = TRUE;

       /* First check to see if we can just turn it back on */

       if(ok)
       {
           pos = checkFinalCT("Humans");
           if(pos > -1)  /* Have it, but make sure oktoprint */
           {
               finalCTlist[pos].oktoprint = TRUE;
               foundHumans = TRUE;
               ok = FALSE;
               tmp = (char *)malloc(strlen(finalCTlist[pos].trigger) + 1000);
               if(strlen(finalCTlist[pos].trigger) > 0)
                 sprintf(tmp, "%s; Forced Turn Back On - CON/AUTO/REVIEW\0",
                         finalCTlist[pos].trigger);
               else
                 strcpy(tmp, "Forced Turn Back On - CON/AUTO/REVIEW");

               free(finalCTlist[pos].trigger);
               finalCTlist[pos].trigger = strdup(tmp);
               free(tmp);

               /* If we already checked for age years but left due to Humans not
                  being recommended - now recheck since we added Humans.
               */

               if(entered_checkForAgeYears)
                 checkForAgeYears();
           } /* fi */
       } /* fi */

       if(ok)
       {
           foundHumans = TRUE;
           p = numFinalCTs;
           finalCTlist[p].foundInL2R = FALSE;
           finalCTlist[p].ETflag = FALSE;
           finalCTlist[p].datatype = CT;
           finalCTlist[p].oktoprint = TRUE;
           finalCTlist[p].score = 1000;
           finalCTlist[p].num_RCs = 0;
           finalCTlist[p].VocabDenFactor = 0.0;
           finalCTlist[p].muid = strdup(muid);
           finalCTlist[p].mh = strdup("Humans");
           finalCTlist[p].mh_orig = strdup("");
           finalCTlist[p].dui = strdup("D006801");
           finalCTlist[p].entry_term = strdup("");
           finalCTlist[p].RC_PMIDs = strdup("");
           finalCTlist[p].textloc = strdup("TI");
           finalCTlist[p].trigger = strdup("Forced Humans CON/AUTO/REVIEW");

           /* Positional Information */

           finalCTlist[p].numPIs = 0;

           /* Paths */

           finalCTlist[p].paths[MMI] = TRUE;
           finalCTlist[p].paths[PRC] = FALSE;

           /* Treecodes */

           finalCTlist[p].num_treecodes = 0;
           pullTREEs_CT(p);

           /* Semantic Types - max 25 */

           finalCTlist[p].num_STs = 0;

           numFinalCTs++;

           /* If we already checked for age years but left due to Humans not
              being recommended - now recheck since we added Humans.
           */

           if(entered_checkForAgeYears)
             checkForAgeYears();
       } /* fi */

       else if(!foundAnimals && foundAnimalDiseases)
       {
           foundAnimals = TRUE;
           p = numFinalCTs;
           finalCTlist[p].foundInL2R = FALSE;
           finalCTlist[p].ETflag = FALSE;
           finalCTlist[p].datatype = CT;
           finalCTlist[p].oktoprint = TRUE;
           finalCTlist[p].score = 1000;
           finalCTlist[p].num_RCs = 0;
           finalCTlist[p].VocabDenFactor = 0.0;
           finalCTlist[p].muid = strdup(muid);
           finalCTlist[p].mh = strdup("Animals");
           finalCTlist[p].mh_orig = strdup("");
           finalCTlist[p].dui = strdup("D000818");
           finalCTlist[p].entry_term = strdup("");
           finalCTlist[p].RC_PMIDs = strdup("");
           finalCTlist[p].textloc = strdup("TI");
           finalCTlist[p].trigger = strdup("Forced Animals CON");

           /* Positional Information */

           finalCTlist[p].numPIs = 0;

           /* Paths */

           finalCTlist[p].paths[MMI] = TRUE;
           finalCTlist[p].paths[PRC] = FALSE;

           /* Treecodes */

           finalCTlist[p].num_treecodes = 0;
           pullTREEs_CT(p);

           /* Semantic Types - max 25 */

           finalCTlist[p].num_STs = 0;

           numFinalCTs++;
       } /* else fi */
   } /* fi */

   if(foundHumans)
   {
       check_Species_Protein_SCRs(HUMAN_TOP_SCR_FILE, "Human");
       check_Species_microRNA_SCRs(HUMAN_TOP_MICRORNA_FILE, "Human");
   } /* fi */

   if(foundMice)
   {
       check_Species_Protein_SCRs(MOUSE_TOP_SCR_FILE, "Mouse");
       check_Species_microRNA_SCRs(MOUSE_TOP_MICRORNA_FILE, "Mouse");
   } /* fi */

   if(foundRats)
   {
       check_Species_Protein_SCRs(RAT_TOP_SCR_FILE, "Rat");
       check_Species_microRNA_SCRs(RAT_TOP_MICRORNA_FILE, "Rat");
   } /* fi */

   if(foundArabidopsis)
   {
       check_Species_Protein_SCRs(ARABIDOPSIS_TOP_SCR_FILE, "Arabidopsis");
       check_Species_microRNA_SCRs(ARABIDOPSIS_TOP_MICRORNA_FILE,
                                   "Arabidopsis");
   } /* fi */

   if(foundDrosophila)
   {
       check_Species_Protein_SCRs(DROSOPHILA_TOP_SCR_FILE, "Drosophila");
       check_Species_microRNA_SCRs(DROSOPHILA_TOP_MICRORNA_FILE,
                                   "Drosophila");
   } /* fi */

   if(foundScerevisiae)
     check_Species_Protein_SCRs(SCEREVISIAE_TOP_SCR_FILE, "Scerevisiae");

   if(foundCelegans)
   {
       check_Species_Protein_SCRs(CELEGANS_TOP_SCR_FILE, "Celegans");
       check_Species_microRNA_SCRs(CELEGANS_TOP_MICRORNA_FILE, "Celegans");
   } /* fi */

   if(foundEcoli)
     check_Species_Protein_SCRs(ECOLI_TOP_SCR_FILE, "Ecoli");

   if(foundZebrafish)
   {
       check_Species_Protein_SCRs(ZEBRAFISH_TOP_SCR_FILE, "Zebrafish");
       check_Species_microRNA_SCRs(ZEBRAFISH_TOP_MICRORNA_FILE, "Zebrafish");
   } /* fi */

   if(foundXenopus)
   {
       check_Species_Protein_SCRs(XENOPUS_TOP_SCR_FILE, "Xenopus");
       check_Species_microRNA_SCRs(XENOPUS_TOP_MICRORNA_FILE, "Xenopus");
   } /* fi */

   if(foundSpombe)
     check_Species_Protein_SCRs(SPOMBE_TOP_SCR_FILE, "Spombe");

   /* Now go through and trim list for each type of citations we have
      REG  : CTs + 18
      MTIFL: CTs + 15
      MTIR : CTs + 12
      MTIA : OK for now
   */

   proceed = TRUE;
   topLimit = 18;
   numFound = 0;

   if(MTI_REVIEW)
   {
       proceed = TRUE;
       topLimit = 12;
   } /* else fi MTIR */

   else if(firstLineJournal)
   {
       proceed = TRUE;
       topLimit = 15;
   } /* else fi MTIFL */

   else if(!MTI_AUTO)
   {
       proceed = TRUE;
       topLimit = 18;
   } /* else fi REG */

  if(proceed)
  {
      numFound = 0;
      for(i = 0; i < numFinalMHs; i++)
      {
          if(finalMHlist[i].oktoprint)
          {
              if(!finalMHlist[i].wasForced && (numFound >= topLimit) && 
                 (strcmp(finalMHlist[i].trigger, "Forced Animal Diseases") != 0))
              {
                  if(RTM_Debug)
                  {
                      fprintf(fout, "checkBeforeFinalPrint - Removing");
                      fprintf(fout, " (#: %ld  Top: %ld: %s [%ld] - trigger: #%s#  forced: %d)\n",
                              numFound, topLimit, finalMHlist[i].mh,
                              finalMHlist[i].datatype, finalMHlist[i].trigger,
                              finalMHlist[i].wasForced);
                      fflush(fout);
                  } /* fi */

                  finalMHlist[i].oktoprint = FALSE;
              } /* fi */

              numFound++;
          } /* fi */
      } /* for */
  } /* fi */

  /* If we have The Veterinary record (Vet Rec) - 0031164 and are recommending
     nothing or just Animals, add in Veterinary Medicine per Susan's email
     6/1/2020.
  */

  if(strcmp(nlmID, "0031164") == 0)
  {
      numFound = 0;
      for(i = 0; i < numFinalMHs; i++)
      {
          if(finalMHlist[i].oktoprint)
            numFound++;
      } /* for */

      if(numFound == 0)
      {
          p = numFinalMHs;
          finalMHlist[p].foundInL2R = FALSE;
          finalMHlist[p].ETflag = FALSE;
          finalMHlist[p].datatype = MH;
          finalMHlist[p].oktoprint = TRUE;
          finalMHlist[p].score = 1000;
          finalMHlist[p].num_RCs = 0;
          finalMHlist[p].VocabDenFactor = 0.0;
          finalMHlist[p].origPos = 10000;

          finalMHlist[p].muid = strdup(muid);
          finalMHlist[p].mh = strdup("Veterinary Medicine");
          finalMHlist[p].mh_orig = strdup("");
          finalMHlist[p].dui = strdup("D014730");
          finalMHlist[p].entry_term = strdup("");
          finalMHlist[p].RC_PMIDs = strdup("");
          finalMHlist[p].textloc = strdup("TI");
          finalMHlist[p].trigger = strdup("Forced Vet Rec");

          /* Positional Information */

          finalMHlist[p].numPIs = 0;

          /* Paths */

          finalMHlist[p].paths[MMI] = TRUE;
          finalMHlist[p].paths[PRC] = FALSE;

          /* Treecodes */

          finalMHlist[p].num_treecodes = 0;
          pullTREEs(p);

          /* Semantic Types - max 25 */

          finalMHlist[p].num_STs = 0;

          numFinalMHs++;
      } /* add */

      /* Check CheckTags next for Animals */

      numFound = 0;
      for(i = 0; i < numFinalCTs; i++)
      {
          if(finalCTlist[i].oktoprint)
          {
              if(strcmp(finalCTlist[i].mh, "Animals") == 0)
              {
                  numFound++;
                  foundAnimals = TRUE;
              } /* fi */
          } /* fi */
      } /* for */

      if(numFound == 0)
      {
          foundAnimals = TRUE;
          p = numFinalCTs;
          finalCTlist[p].foundInL2R = FALSE;
          finalCTlist[p].ETflag = FALSE;
          finalCTlist[p].datatype = CT;
          finalCTlist[p].oktoprint = TRUE;
          finalCTlist[p].score = 1000;
          finalCTlist[p].num_RCs = 0;
          finalCTlist[p].VocabDenFactor = 0.0;
          finalCTlist[p].origPos = 10000;

          finalCTlist[p].muid = strdup(muid);
          finalCTlist[p].mh = strdup("Animals");
          finalCTlist[p].mh_orig = strdup("");
          finalCTlist[p].dui = strdup("D000818");
          finalCTlist[p].entry_term = strdup("");
          finalCTlist[p].RC_PMIDs = strdup("");
          finalCTlist[p].textloc = strdup("TI");
          finalCTlist[p].trigger = strdup("Forced Vet Rec");

          /* Positional Information */

          finalCTlist[p].numPIs = 0;

          /* Paths */

          finalCTlist[p].paths[MMI] = TRUE;
          finalCTlist[p].paths[PRC] = FALSE;

          /* Treecodes */

          finalCTlist[p].num_treecodes = 0;
          pullTREEs_CT(p);

          /* Semantic Types - max 25 */

          finalCTlist[p].num_STs = 0;

          numFinalCTs++;
      } /* add */
  } /* fi The Veterinary Record */

  /* Check to make sure if we are recommending any Term that would automatically add Humans
     via the IMS - make sure to add Humans.
  */

  if(!foundHumans)
  {
      yesAdd = FALSE;
      for(i = 0; !yesAdd && (i < numFinalMHs); i++)
      {
          if(finalMHlist[i].oktoprint)
          {
              for(j = 0; !yesAdd && (j < num_AddHumansList); j++)
              {
                  if(addHumans_duis[j][4] == finalMHlist[i].dui[4])
                  {
                      if(strcmp(addHumans_duis[j], finalMHlist[i].dui) == 0)
                      {
                          yesAdd = TRUE;
                          foundHumans = TRUE;
                          p = numFinalCTs;
                          finalCTlist[p].foundInL2R = FALSE;
                          finalCTlist[p].ETflag = FALSE;
                          finalCTlist[p].datatype = CT;
                          finalCTlist[p].oktoprint = TRUE;
                          finalCTlist[p].score = 1000;
                          finalCTlist[p].num_RCs = 0;
                          finalCTlist[p].VocabDenFactor = 0.0;
                          finalCTlist[p].origPos = 10000;

                          sprintf(from, "IMS Forced Humans: %s\0", finalMHlist[i].mh);
                          finalCTlist[p].muid = strdup(muid);
                          finalCTlist[p].mh = strdup("Humans");
                          finalCTlist[p].mh_orig = strdup("");
                          finalCTlist[p].dui = strdup("D006801");
                          finalCTlist[p].entry_term = strdup("");
                          finalCTlist[p].RC_PMIDs = strdup("");
                          finalCTlist[p].textloc = strdup("TI");
                          finalCTlist[p].trigger = strdup(from);

                          /* Positional Information */

                          finalCTlist[p].numPIs = 0;

                          /* Paths */

                          finalCTlist[p].paths[MMI] = TRUE;
                          finalCTlist[p].paths[PRC] = FALSE;

                          /* Treecodes */

                          finalCTlist[p].num_treecodes = 0;
                          pullTREEs_CT(p);

                          /* Semantic Types - max 25 */

                          finalCTlist[p].num_STs = 0;

                          numFinalCTs++;
                      } /* fi */
                  } /* fi */
              } /* for */
          } /* fi */
      } /* for */
  } /* fi !foundHumans */

  /* Check to make sure if we are recommending any Term that would automatically add Animals
     via the IMS - make sure to add Animals.
  */

  if(!foundAnimals)
  {
      yesAdd = FALSE;
      for(i = 0; !yesAdd && (i < numFinalMHs); i++)
      {
          if(finalMHlist[i].oktoprint)
          {
              for(j = 0; !yesAdd && (j < num_AddAnimalsList); j++)
              {
                  if(addAnimals_duis[j][4] == finalMHlist[i].dui[4])
                  {
                      if(strcmp(addAnimals_duis[j], finalMHlist[i].dui) == 0)
                      {
                          yesAdd = TRUE;
                          foundAnimals = TRUE;
                          p = numFinalCTs;
                          finalCTlist[p].foundInL2R = FALSE;
                          finalCTlist[p].ETflag = FALSE;
                          finalCTlist[p].datatype = CT;
                          finalCTlist[p].oktoprint = TRUE;
                          finalCTlist[p].score = 1000;
                          finalCTlist[p].num_RCs = 0;
                          finalCTlist[p].VocabDenFactor = 0.0;
                          finalCTlist[p].origPos = 10000;

                          sprintf(from, "IMS Forced Animals: %s\0", finalMHlist[i].mh);
                          finalCTlist[p].muid = strdup(muid);
                          finalCTlist[p].mh = strdup("Animals");
                          finalCTlist[p].mh_orig = strdup("");
                          finalCTlist[p].dui = strdup("D000818");
                          finalCTlist[p].entry_term = strdup("");
                          finalCTlist[p].RC_PMIDs = strdup("");
                          finalCTlist[p].textloc = strdup("TI");
                          finalCTlist[p].trigger = strdup(from);

                          /* Positional Information */

                          finalCTlist[p].numPIs = 0;

                          /* Paths */

                          finalCTlist[p].paths[MMI] = TRUE;
                          finalCTlist[p].paths[PRC] = FALSE;

                          /* Treecodes */

                          finalCTlist[p].num_treecodes = 0;
                          pullTREEs_CT(p);

                          /* Semantic Types - max 25 */

                          finalCTlist[p].num_STs = 0;

                          numFinalCTs++;
                      } /* fi */
                  } /* fi */
              } /* for */
          } /* fi */
      } /* for */
  } /* fi !foundAnimals */

  /* If we have Male/Female without Humans or Animals AND do have Plant reference -
     For right now, if it is not a Plant based article, we will add Humans.  If it is
     a Plant based article, leave Male/Female along.  The original goal was to actually
     remove Male/Female but examples like PMID: 32918714 made us hold up on that.
  */

  if(!foundHumans && !foundAnimals && (foundFemale || foundMale))
  {
      if(botanyJournal || foundPlants || foundPlantBreeding)
      {
/* Do nothing for the moment for Plant based articles with Male/Female June 23, 2021
          if(foundFemale && (femalePos > -1))
          {
               finalCTlist[femalePos].oktoprint = FALSE;
               if(RTM_Debug)
               {
                   fprintf(fout, "Removing Female - Plants Only\n");
                   fflush(fout);
               }
          }

          if(foundMale && (malePos > -1))
          {
               finalCTlist[malePos].oktoprint = FALSE;
               if(RTM_Debug)
               {
                   fprintf(fout, "Removing Male - Plants Only\n");
                   fflush(fout);
               } 
          }
*/
      } /* fi Plant-based article */

      else /* not plant-based, then add Humans */
      {
           /* First check to see if we can just turn it back on */

           pos = checkFinalCT("Humans");
           if(pos > -1)  /* Have it, but make sure oktoprint */
           {
               finalCTlist[pos].oktoprint = TRUE;
               foundHumans = TRUE;
               tmp = (char *)malloc(strlen(finalCTlist[pos].trigger) + 1000);
               if(strlen(finalCTlist[pos].trigger) > 0)
                 sprintf(tmp, "%s; Forced Turn Back On - Male/Female Rule\0",
                         finalCTlist[pos].trigger);
               else
                 strcpy(tmp, "Forced Turn Back On - Male/Female Rule");

               free(finalCTlist[pos].trigger);
               finalCTlist[pos].trigger = strdup(tmp);
               free(tmp);
           } /* fi */

           else /* We need to add it */
           {
               foundHumans = TRUE;
               p = numFinalCTs;
               finalCTlist[p].ETflag = FALSE;
               finalCTlist[p].datatype = CT;
               finalCTlist[p].oktoprint = TRUE;
               finalCTlist[p].score = 1000;
               finalCTlist[p].num_RCs = 0;
               finalCTlist[p].VocabDenFactor = 0.0;
               finalCTlist[p].muid = strdup(muid);
               finalCTlist[p].mh = strdup("Humans");
               finalCTlist[p].mh_orig = strdup("");
               finalCTlist[p].dui = strdup("D006801");
               finalCTlist[p].entry_term = strdup("");
               finalCTlist[p].RC_PMIDs = strdup("");
               finalCTlist[p].textloc = strdup("TI");
               finalCTlist[p].trigger = strdup("Forced Humans Male/Female Rule");

               /* Positional Information */

               finalCTlist[p].numPIs = 0;

               /* Paths */

               finalCTlist[p].paths[MMI] = TRUE;
               finalCTlist[p].paths[PRC] = FALSE;

               /* Treecodes */

               finalCTlist[p].num_treecodes = 0;
               pullTREEs_CT(p);

               /* Semantic Types - max 25 */

               finalCTlist[p].num_STs = 0;

               numFinalCTs++;
           } /* else need to add */


           /* If we already checked for age years but left due to Humans not
              being recommended - now recheck since we added Humans.
           */

           if(foundHumans && entered_checkForAgeYears)
             checkForAgeYears();
      } /* else not Plant based */
  } /* !foundHumans && !foundAnimals && (foundFemale || foundMale */
} /* checkBeforeFinalPrint */


/***************************************************************************
*
*  checkFinalMH --
*
*      This 
*
***************************************************************************/

int checkFinalMH(char *mh)
{
   int found;
   long i;

   found = FALSE;
   for(i = 0; !found && (i < numFinalMHs); i++)
   {
       if(finalMHlist[i].oktoprint)
       {
           if(finalMHlist[i].ETflag)
           {
               if(finalMHlist[i].mh_orig[0] == mh[0])
               {
                   if(strcmp(finalMHlist[i].mh_orig, mh) == 0)
                     found = TRUE;
               } /* fi */
           } /* fi ET */

           else
           {
               if(finalMHlist[i].mh[0] == mh[0])
               {
                   if(strcmp(finalMHlist[i].mh, mh) == 0)
                     found = TRUE;
               } /* fi */
           } /* fi !ET */
       } /* fi oktoprint */
   } /* for */

   return(found);
} /* checkFinalMH */


/***************************************************************************
*
*  checkFinalMHPos --
*
*      This 
*
***************************************************************************/

long checkFinalMHPos(char *mh)
{
   int found;
   long i, rtn;

   rtn = -1;
   found = FALSE;
   for(i = 0; !found && (i < numFinalMHs); i++)
   {
       if(finalMHlist[i].ETflag)
       {
           if(finalMHlist[i].mh_orig[0] == mh[0])
           {
               if(strcmp(finalMHlist[i].mh_orig, mh) == 0)
               {
                   found = TRUE;
                   rtn = i;
               } /* fi */
           } /* fi */
       } /* fi ET */

       else
       {
           if(finalMHlist[i].mh[0] == mh[0])
           {
               if(strcmp(finalMHlist[i].mh, mh) == 0)
               {
                   found = TRUE;
                   rtn = i;
               } /* fi */
           } /* fi */
       } /* fi !ET */
   } /* for */

   return(rtn);
} /* checkFinalMHPos */


/***************************************************************************
*
*  checkFinalCT --
*
*      This 
*
***************************************************************************/

long checkFinalCT(char *mh)
{
   int found;
   long i, rtn;

   rtn = -1;
   found = FALSE;
   for(i = 0; !found && (i < numFinalCTs); i++)
   {
       if(finalCTlist[i].mh[0] == mh[0])
       {
           if(strcmp(finalCTlist[i].mh, mh) == 0)
           {
               found = TRUE;
               rtn = i;
           } /* fi */
       } /* fi */
   } /* for */

   return(rtn);
} /* checkFinalCT */


/***************************************************************************
*
*  updateL2R --
*
*      NOTE: Updates are chronologically, so go through all of them.
*
***************************************************************************/

char *updateL2R(char *lineL2R)
{
   int chgMade;
   long i;
   char *rtn, tmp[SMALL_LINE + 1], field1[SMALL_LINE + 1], DUI[250],
        newDUI[250], rest[SMALL_LINE + 1], tmp2[SMALL_LINE + 1];

   rtn = strdup(lineL2R);
   strcpy(tmp, "");
   strcpy(field1, "");
   strcpy(DUI, "");
   strcpy(rest, "");
   sscanf(lineL2R, "%[^|]|%[^|]|%[^|]|%[^\n]", field1, tmp, DUI, rest);

   if(RTM_Debug)
   {
       fprintf(fout, "updateL2R: #%s# -> #%s#\n", lineL2R, tmp);
       fflush(fout);
   } /* fi */

   if((strlen(tmp) > 0) && (strlen(field1) > 0) && (strlen(DUI) > 0) &&
      (strlen(rest) > 0))
   {
       chgMade = FALSE;
       for(i = 0; i < num_L2R_Replace; i++)
       {
           if(tmp[0] == Replacements_L2R[i].oldMH[0])
           {
               if(strcmp(tmp, Replacements_L2R[i].oldMH) == 0)
               {
                   strcpy(tmp, Replacements_L2R[i].newMH);
                   strcpy(newDUI, Replacements_L2R[i].newDUI);
                   chgMade = TRUE;
                   if(RTM_Debug)
                   {
                       fprintf(fout, "L2R Update: #%s# to #%s#\n", 
                           Replacements_L2R[i].oldMH, tmp);
                       fflush(fout);
                   } /* fi */
               } /* fi */
           } /* fi */
       } /* for */

       if(chgMade)
       {
           free(rtn);
           sprintf(tmp2, "%s|%s|%s|%s\n\0", field1, tmp, newDUI, rest);
           rtn = strdup(tmp2);
       } /* fi */
   } /* fi */

   else
   {
        fprintf(stderr, "ERR - bad L2R Line: #%s#\n", lineL2R);
        fflush(stderr);
   } /* else */

   return(rtn);
} /* updateL2R */


/***************************************************************************
*
*  isBadL2R --
*
*      NOTE:
*
***************************************************************************/

int isBadL2R(char *mhL2R, char *pmidL2R)
{
   int rtn, done;
   long i;
   char tmp[SMALL_LINE + 1];

   rtn = FALSE;
   if(RTM_Debug)
   {
       fprintf(fout, "Checking L2R term: #%s#\n", mhL2R);
       fflush(fout);
   } /* fi */

   /* Look through low performing list of L2R terms -
      Performance < 46% right and at least 5 occurrences
      Updated list on 4/25/2019.
   */

   done = FALSE;
   for(i = 0; !rtn && !done && (i < numBad_L2R); i++)
   {
        if(mhL2R[0] == bad_L2R_Terms[i][0])
        {
            if(strcmp(mhL2R, bad_L2R_Terms[i]) == 0)
            {
                rtn = TRUE;
                if(RTM_Debug)
                {
                     fprintf(fout, "Ignore L2R (low perform): #%s#\n", mhL2R);
                     fflush(fout);
                } /* fi */
            } /* fi */
        } /* fi */

        else if(mhL2R[0] < bad_L2R_Terms[i][0])
          done = TRUE;

        if(FALSE)
        {
             fprintf(fout, "L2R Checking: [%d|%d] #%s# vs #%s#\n",
                     rtn, done, mhL2R, bad_L2R_Terms[i]);
             fflush(fout);
        } /* fi */
   } /* for */

   /* Check against the MH Exclusion list */

   if(!rtn)
   {
       if(isMHExcludeLC(mhL2R, TRUE))
       {
           rtn = TRUE;
           if(RTM_Debug)
           {
                fprintf(fout, "Ignore L2R (MHExcluded): #%s#\n", mhL2R);
                fflush(fout);
           } /* fi */
       } /* fi */
   } /* fi */

   /* If we have a Veterinary Medicine journal, make sure L2R doesn't add
      any Age Related Check Tags.

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

   if(!rtn && veterinaryJournal)
   {
       if((strcmp(mhL2R, "adolescent") == 0) ||
          (strcmp(mhL2R, "adult") == 0) ||
          (strcmp(mhL2R, "aged") == 0) ||
          (strcmp(mhL2R, "aged, 80 and over") == 0) ||
          (strcmp(mhL2R, "child") == 0) ||
          (strcmp(mhL2R, "child, preschool") == 0) ||
          (strcmp(mhL2R, "infant") == 0) ||
          (strcmp(mhL2R, "infant, newborn") == 0) ||
          (strcmp(mhL2R, "middle aged") == 0) ||
          (strcmp(mhL2R, "young adult") == 0))
       {
           rtn = TRUE;
           if(RTM_Debug)
           {
                fprintf(fout, "Ignore L2R (VET Aged): #%s#\n", mhL2R);
                fflush(fout);
           } /* fi */
       } /* fi */
   } /* fi */

   /* If we didn't find it in the above low performers or MHexclude list,
      look for other cases we have put together.
   */

   if(!rtn)
   {
       if(mhL2R[0] == 'a')
       {
           if((strcmp(mhL2R, "art") == 0) ||
              (strcmp(mhL2R, "achievement") == 0) ||
              (strcmp(mhL2R, "absorption") == 0))
             rtn = TRUE;

           /* Added fall/fallen/fell/falls -> accidental falls
              filter 9/12/16
            */

           else if(strcmp(mhL2R, "accidental falls") == 0)
           {
               if(!isLookForOK("falls"))
               {
                   rtn = TRUE;
                   for(i = 0; i < numFinalMHs; i++)
                   {
                       if(finalMHlist[i].mh[0] == 'A')
                       {
                           if(strcmp(finalMHlist[i].dui, "D000058") == 0)
                             finalMHlist[i].oktoprint = FALSE;
                       } /* fi */
                   } /* for */
               } /* fi */
           } /* else fi */

           /* Added parsing triggering Software and Algorithms
                  filter 4/18/17

              NOTE: Also Software
           */

           else if(strcmp(mhL2R, "algorithms") == 0)
           {
               if(foundInText("parsing", FALSE, FALSE) ||
                  foundInText("parsed", FALSE, FALSE))
                 rtn = TRUE;
           } /* else fi */

           else if(strcmp(mhL2R, "aggression") == 0)
           {
               if(foundInText("fighting", FALSE, FALSE))
                 rtn = TRUE;
           } /* else fi */
       } /* fi 'a' */

       else if(mhL2R[0] == 'c')
       {
           if(strcmp(mhL2R, "cognition disorders") == 0)
           {
               if(foundInText("cognitive decline", FALSE, TRUE))
                 rtn = TRUE;
           } /* fi */

           else if(strcmp(mhL2R, "chick embryo") == 0)
           {
               if(!foundInText("chick embryo", FALSE, FALSE))
                 rtn = TRUE;
           } /* else fi */

           else if(strcmp(mhL2R, "cross-sectional studies") == 0)
           {
               if(!isLookForOK("cross-sectional"))
                 rtn = TRUE;
           } /* else fi */

           else if(strcmp(mhL2R, "chickens") == 0)
           {
               if(!isLookForOK("chickens"))
                 rtn = TRUE;
           } /* else fi */
       } /* else fi 'c' */

       else if(mhL2R[0] == 'd')
       {
           /* Added 7/17/17 rare type 1 & type 2 are together */

           if(strcmp(mhL2R, "diabetes mellitus, type 1") == 0)
           {
               if(checkFinalMH("Diabetes Mellitus, Type 2"))
                 rtn = TRUE;
           } /* fi */

           /* Added 7/17/17 rare type 1 & type 2 are together */

           else if(strcmp(mhL2R, "diabetes mellitus, type 2") == 0)
           {
               if(checkFinalMH("Diabetes Mellitus, Type 1"))
                 rtn = TRUE;

               else if(foundInText("accord", FALSE, TRUE) &&
                      !foundInText("ACCORD", TRUE, TRUE))
                 rtn = TRUE;
           } /* else fi */

           /* NOTE: Also Eczema */

           else if(strcmp(mhL2R, "dermatitis, atopic") == 0)
           {
               if(foundInText("skin damage", FALSE, FALSE))
                 rtn = TRUE;
           } /* else fi */

           else if(strcmp(mhL2R, "diptera") == 0)
           {
               if(foundInText("time flies", FALSE, FALSE) ||
                  foundInText("flies with time", FALSE, FALSE))
                 rtn = TRUE;
           } /* else fi */
       } /* else fi 'd' */

       else if(mhL2R[0] == 'e')
       {
           /* NOTE: Also dermatitis, atopic */

           if(strcmp(mhL2R, "eczema") == 0)
           {
               if(foundInText("skin damage", FALSE, FALSE))
                 rtn = TRUE;
           } /* fi */

           else if(strcmp(mhL2R, "entomology") == 0)
              rtn = TRUE;
       } /* else fi 'e' */

       else if(mhL2R[0] == 'f')
       {
           /* Added foster triggering Foster Home Care filter 12/15/17
              NOTE: Also Special Filtering (Special_FOSTER).
           */

           if(strcmp(mhL2R, "foster home care") == 0)
           {
               if(foundInText("foster care", FALSE, FALSE) ||
                  foundInText("foster carer", FALSE, FALSE) ||
                  foundInText("foster caregiver", FALSE, FALSE) ||
                  foundInText("foster home", FALSE, FALSE) ||
                  foundInText("foster-parents", FALSE, FALSE) ||
                  foundInText("foster-parent", FALSE, FALSE) ||
                  foundInText("foster parents", FALSE, FALSE) ||
                  foundInText("foster parent", FALSE, FALSE) ||
                  foundInText("foster children", FALSE, FALSE) ||
                  foundInText("foster child", FALSE, FALSE) ||
                  foundInText("foster youth", FALSE, FALSE) ||
                  foundInText("foster mother", FALSE, FALSE) ||
                  foundInText("foster father", FALSE, FALSE) ||
                  foundInText("foster family", FALSE, FALSE))
                 rtn = FALSE;
               else
                 rtn = TRUE;
           } /* fi */
       } /* else fi 'f' */

       else if(mhL2R[0] == 'g')
       {
           if(strcmp(mhL2R, "goats") == 0)
           {
              if(foundInText("kid", FALSE, FALSE))
              {
                  if(!foundInText("goat", FALSE, FALSE))
                    rtn = TRUE;
              } /* fi */
           } /* fi */
       } /* else fi 'g' */

       else if(mhL2R[0] == 'h')
       {
           /* Don't want Hepatitis A showing up if article is about Hepatitis E for example. */

           if(strcmp(mhL2R, "hepatitis a") == 0)
           {
               if(!foundInText("hepatitis a", FALSE, FALSE))
               {
                   rtn = TRUE;
                   if(RTM_Debug)
                   {
                       fprintf(fout, "Removing L2R MHs (%s)\n", mhL2R);
                       fflush(fout);
                   } /* fi */
               } /* fi */
           } /* fi */

           /* Don't want Hepatitis B, Chronic showing up if article is about 
              Hepatitis E for example.
           */

           else if((strcmp(mhL2R, "hepatitis b, chronic") == 0) ||
              (strcmp(mhL2R, "hepatitis b") == 0))
           {
               if(!foundInText("hepatitis b", FALSE, FALSE))
               {
                   rtn = TRUE;
                   if(RTM_Debug)
                   {
                       fprintf(fout, "Removing L2R MHs (%s)\n", mhL2R);
                       fflush(fout);
                   } /* fi */
               } /* fi */
           } /* else fi */

           /* Don't want Hepatitis C, Chronic showing up if article is about
              Hepatitis E for example.
           */

           else if((strcmp(mhL2R, "hepatitis c, chronic") == 0) ||
              (strcmp(mhL2R, "hepatitis c") == 0))
           {
               if(!foundInText("hepatitis c", FALSE, FALSE))
               {
                   rtn = TRUE;
                   if(RTM_Debug)
                   {
                       fprintf(fout, "Removing L2R MHs (%s)\n", mhL2R);
                       fflush(fout);
                   } /* fi */
               } /* fi */
           } /* else fi */

           /* Don't want Hepatitis D, Chronic showing up if article is about
              Hepatitis E for example.
           */

           else if((strcmp(mhL2R, "hepatitis d, chronic") == 0) ||
              (strcmp(mhL2R, "hepatitis d") == 0))
           {
               if(!foundInText("hepatitis d", FALSE, FALSE))
               {
                   rtn = TRUE;
                   if(RTM_Debug)
                   {
                       fprintf(fout, "Removing L2R MHs (%s)\n", mhL2R);
                       fflush(fout);
                   } /* fi */
               } /* fi */
           } /* else fi */

           /* Don't want Hepatitis A showing up if article is about Hepatitis E for example. */

           else if(strcmp(mhL2R, "hepatitis e") == 0)
           {
               if(!foundInText("hepatitis e", FALSE, FALSE))
               {
                   rtn = TRUE;
                   if(RTM_Debug)
                   {
                       fprintf(fout, "Removing L2R MHs (%s)\n", mhL2R);
                       fflush(fout);
                   } /* fi */
               } /* fi */
           } /* else fi */
       } /* else fi 'h' */

       else if(mhL2R[0] == 'i')
       {
           if(strcmp(mhL2R, "iraq war, 2003-2011") == 0)
           {
              if(foundInText("battle", FALSE, TRUE) ||
                 foundInText("war", FALSE, TRUE))
                rtn = TRUE;
           } /* fi */
       } /* else fi 'i' */

       else if(mhL2R[0] == 'l')
       {
           if(strcmp(mhL2R, "long qt syndrome") == 0)
           {
               if(foundInText("short qt syndrome", FALSE, FALSE) &&
                  !foundInText("long qt syndrome", FALSE, FALSE))
                 rtn = TRUE;
           } /* fi */
       } /* else fi 'l' */

       else if(mhL2R[0] == 'm')
       {
           if(strcmp(mhL2R, "molecular sequence data") == 0)
             rtn = TRUE;

           else if(strcmp(mhL2R, "micrornas") == 0)
           {
              if(foundInText("let", FALSE, TRUE) &&
                 (strstr(globalTitle, "let-") == NULL))
                rtn = TRUE;
           } /* else fi */

           else if(strcmp(mhL2R, "military personnel") == 0)
           {
              if(foundInText("battle", FALSE, TRUE) ||
                 foundInText("war", FALSE, TRUE))
                rtn = TRUE;
           } /* else fi */
       } /* else fi 'm' */

       else if(mhL2R[0] == 'n')
       {
           if(strcmp(mhL2R, "noise") == 0)
             rtn = TRUE;
       } /* else fi 'n' */

       else if(mhL2R[0] == 'o')
       {
           if(strcmp(mhL2R, "out-of-hospital cardiac arrest") == 0)
           {
               if((foundInText("in-hospital cardiac arrest", FALSE, FALSE) ||
                   foundInText("in hospital cardiac arrest", FALSE, FALSE)) &&
                   !foundInText("out-of-hospital cardiac arrest", FALSE, FALSE))
                 rtn = TRUE;
           } /* fi */
       } /* else fi 'o' */

       else if(mhL2R[0] == 'p')
       {
           if(strcmp(mhL2R, "pediatrics") == 0)
           {
               if((!foundInText("paediatrics", FALSE, FALSE) ||
                   !foundInText("pediatrics", FALSE, FALSE)) || 
                   (foundInText("pediatric case", FALSE, FALSE) ||
                    foundInText("pediatric cases", FALSE, FALSE) ||
                    foundInText("paediatric case", FALSE, FALSE) ||
                    foundInText("paediatric cases", FALSE, FALSE)))
                  rtn = TRUE;
           } /* fi */
       } /* else fi 'p' */

       else if(mhL2R[0] == 'r')
       {
           if(strcmp(mhL2R, "reality testing") == 0)
           {
               if(foundInText("reality", FALSE, TRUE))
                 rtn = TRUE;
           } /* fi */

           /* 12/9/2019 - all low performers */

           else if(strstr(mhL2R, "rats, ") != NULL)
              rtn = TRUE;

           else if(strcmp(mhL2R, "risk factors") == 0)
           {
               if(foundInText("accord", FALSE, TRUE) &&
                  !foundInText("ACCORD", TRUE, TRUE))
                 rtn = TRUE;
           } /* fi */

           else if(strcmp(mhL2R, "radioactive fallout") == 0)
           {
               if(foundInText("chernobyl", FALSE, TRUE) ||
                  foundInText("nagasaki", FALSE, TRUE) ||
                  foundInText("hiroshima", FALSE, TRUE) ||
                  foundInText("radiation", FALSE, TRUE) ||
                  foundInText("radioactive", FALSE, TRUE) ||
                  foundInText("radionuclides", FALSE, TRUE) ||
                  foundInText("radiocesium", FALSE, TRUE) ||
                  foundInText("radiolabeled", FALSE, TRUE) ||
                  foundInText("plutonium", FALSE, TRUE) ||
                  foundInText("atomic", FALSE, TRUE) ||
                  foundInText("nuclear", FALSE, TRUE))
                 rtn = FALSE;
               else
                 rtn = TRUE;
           } /* fi */
       } /* else fi 'r' */

       else if(mhL2R[0] == 's')
       {
           /* Added fall/fallen/fell/falls -> Seasons filter 9/12/16 */

           if(strcmp(mhL2R, "seasons") == 0)
           {
               if(!isLookForOK("seasons"))
               {
                    rtn = TRUE;
                    for(i = 0; i < numFinalMHs; i++)
                    {
                       if(finalMHlist[i].mh[0] == 'S')
                       {
                           if((strcmp(finalMHlist[i].mh, "Seasons") == 0) ||
                              (strcmp(finalMHlist[i].mh, "Season") == 0))
                             finalMHlist[i].oktoprint = FALSE;
                       } /* fi */
                    } /* for */
               } /* fi */
           } /* fi */

           /* NOTE: Also Algorithm */

           else if(strcmp(mhL2R, "software") == 0)
           {
               if(foundInText("parsing", FALSE, FALSE) ||
                  foundInText("parsed", FALSE, FALSE))
                 rtn = TRUE;
           } /* else fi */
      } /* else fi 's' */

       else if(mhL2R[0] == 'w')
       {
           if(strcmp(mhL2R, "weight loss") == 0)
           {
               if(foundInText("loss", FALSE, TRUE) &&
                  (!foundInText("weight loss", FALSE, TRUE) &&
                   !foundInText("weight-loss", FALSE, TRUE)))
                 rtn = TRUE;
           } /* else fi */
       } /* else fi 'w' */

       else if(mhL2R[0] == 'v')
       {
           if(strcmp(mhL2R, "violence") == 0)
           {
               if(foundInText("fighting", FALSE, TRUE))
                 rtn = TRUE;
           } /* else fi */
       } /* else fi 'w' */

       if(rtn && RTM_Debug)
       {
           fprintf(fout, "Ignore L2R (Alpha Check): #%s#\n", mhL2R);
           fflush(fout);
       } /* fi */
   } /* fi */

   /* isLookForOk returns false if not ok */

   if(!rtn)
   {
       if((strcmp(mhL2R, "male") != 0) && !isLookForOK(mhL2R))
       {
           rtn = TRUE;
           if(RTM_Debug)
           {
               fprintf(fout, "Ignore L2R (isLookForOk): #%s#\n", mhL2R);
               fflush(fout);
           } /* fi */
       } /* fi */
   } /* fi still ok */

   if(rtn && RTM_Debug)
   {
       fprintf(fout, "Ignore L2R: #%s# (%s)\n", mhL2R, pmidL2R);
       fflush(fout);
   } /* !ok */

   return(rtn);
} /* isBadL2R */


/***************************************************************************
*
*  checkAddCT --
*
*      NOTE:
*
***************************************************************************/

int checkAddCT(char *citSpan, int flag)
{
   long i, p, len, pos;
   float k;
   char *tmp;

   for(k = begYear; k <= endYear; (k += 0.1))
   {
       /* Infant, Newborn (0 - 1 month) */

       if((k >= 0) && (k < 0.08))
         Age_CT_list[0].found = TRUE;

       /* Infant (1 month - 23 months) */

       if((k > 0.08) && (k < 2.0))
         Age_CT_list[1].found = TRUE;

       /* Child, Preschool (2 - 5) */

       if((k > 1.9) && (k < 6.0))
         Age_CT_list[2].found = TRUE;

       /* Child (6 - 12) */

       if((k > 5.9) && (k < 13.0))
         Age_CT_list[3].found = TRUE; 

       /* Adolescent (13 - 18) */

       if((k > 12.9) && (k < 19.0))
         Age_CT_list[4].found = TRUE;

       /* Young Adult (19 - 24) */

       if((k > 18.9) && (k < 25.0))
         Age_CT_list[5].found = TRUE;

       /* Adult (19 - 44) */

       if((k > 18.9) && (k < 45.0))
         Age_CT_list[6].found = TRUE;

       /* Middle Aged (45 - 64) */

       if((k > 44.9) && (k < 65.0))
         Age_CT_list[7].found = TRUE;

       /* Aged (65 - 79) */

       if((k > 64.9) && (k < 80.0))
         Age_CT_list[8].found = TRUE;

       /* Aged, 80 and over (80+) */

       if(k > 79.9)
         Age_CT_list[9].found = TRUE;
   } /* for */

   /* If we are in a Geriatrics journal we don't want to have anything
      younger than Aged being added since it would be wrong in the vast
      majority of cases.

      Aged = position 8, so just up to that look at.
   */

   if(geriatricJournal)
   {
       for(i = 0; i < 8; i++)
       {
           if(Age_CT_list[i].found)
           {
               Age_CT_list[i].found = FALSE;
               if(RTM_Debug)
               {
                   fprintf(fout,
                           "Age Range - Age Group Removed (Geriatrics): %s\n",
                           Age_CT_list[i].Name);
                   fflush(fout);
               } /* fi */
           } /* fi */
       } /* for */
   } /* fi */

   /* Go through and see if we need to add anything that was missed. */

   for(i = 0; i < numAgeCTs; i++)
   {
       if(Age_CT_list[i].found)
       {
           if(RTM_Debug)
           {
               fprintf(fout, "Age Range - Age Group Identified: %s\n",
                       Age_CT_list[i].Name);
               fflush(fout);
           } /* fi */

           /* If turned off for some reason, turn it back on */

           if(Age_CT_list[i].inListAlready)
           {
               p = Age_CT_list[i].listPos;
               if(!finalCTlist[p].oktoprint)
               {
                   if(RTM_Debug)
                   {
                       fprintf(fout,
                           "Age Range Turn Back On Forced Age Group: %s\n",
                           finalCTlist[p].mh);
                       fflush(fout);
                   } /* fi */

                   finalCTlist[p].oktoprint = TRUE;
                   tmp = (char *)malloc(strlen(citSpan) + 1000);
                   strcpy(tmp, "Forced Turn Back On - Age Group: ");
                   strcat(tmp, citSpan);
                   free(finalCTlist[p].trigger);
                   finalCTlist[p].trigger = strdup(tmp);
                   free(tmp);
               } /* fi !oktoprint */

               else /* add to exising trigger */
               {
                   len = (long)strlen(finalCTlist[p].trigger);
                   tmp = (char *)malloc(strlen(citSpan) + len + 1000);
                   if(len > 0)
                     sprintf(tmp, "%s; Forced Age Group: %s\0",
                             finalCTlist[p].trigger, citSpan);
                   else
                     sprintf(tmp, "Forced Age Group: %s\0", citSpan);
                   free(finalCTlist[p].trigger);
                   finalCTlist[p].trigger = strdup(tmp);
                   free(tmp);
               } /* else */

               pos = finalCTlist[p].numPIs;
               finalCTlist[p].PIs[pos].isStart = TRUE;
               finalCTlist[p].PIs[pos].startPos = begSpan;
               finalCTlist[p].PIs[pos].endPos = begSpan + strlen(citSpan);
               finalCTlist[p].PIs[pos].len = strlen(citSpan);
               finalCTlist[p].PIs[pos].hasBreak = FALSE;
               finalCTlist[p].numPIs++;
           } /* fi inListAlready */

           else /* Need to add it */
           {
               if(RTM_Debug)
               {
                   fprintf(fout,
                      "Age Range Adding Forced Age Group: %s Span: #%s#\n",
                      Age_CT_list[i].Name, citSpan);
                   fflush(fout);
               } /* fi */

               p = numFinalCTs;
               Age_CT_list[i].listPos = p;
               Age_CT_list[i].inListAlready = TRUE;
               finalCTlist[p].foundInL2R = FALSE;
               finalCTlist[p].ETflag = FALSE;
               finalCTlist[p].datatype = MH;
               finalCTlist[p].oktoprint = TRUE;
               finalCTlist[p].score = 1000;
               finalCTlist[p].num_RCs = 0;
               finalCTlist[p].VocabDenFactor = 1.9;
               finalCTlist[p].origPos = 10000;

               finalCTlist[p].muid = strdup(muid);
               finalCTlist[p].mh = strdup(Age_CT_list[i].Name);
               finalCTlist[p].mh_orig = strdup("");
               finalCTlist[p].dui = strdup(Age_CT_list[i].DUI);
               finalCTlist[p].entry_term = strdup("");
               finalCTlist[p].RC_PMIDs = strdup("");
               tmp = (char *)malloc(strlen(citSpan) + 1000);
               sprintf(tmp, "Forced Age Group: %s\0", citSpan);
               finalCTlist[p].trigger = strdup(tmp);
               free(tmp);
               finalCTlist[p].textloc = strdup("");

               /* Positional Information */

               finalCTlist[p].PIs[0].isStart = TRUE;
               finalCTlist[p].PIs[0].startPos = begSpan;
               finalCTlist[p].PIs[0].endPos = endSpan;
               finalCTlist[p].PIs[0].len = strlen(citSpan);
               finalCTlist[p].PIs[0].hasBreak = FALSE;
               finalCTlist[p].numPIs = 1;

               /* Paths */

               finalCTlist[p].paths[MMI] = TRUE;

               /* Treecodes */

               finalCTlist[p].num_treecodes = 0;
               pullTREEs_CT(p);

               /* Semantic Types - max 25 */

               finalCTlist[p].num_STs = 0;

               numFinalCTs++;
           } /* else need to add */
       } /* fi Age Range Found */
   } /* for */


   /* See if there are any ages we should turn off - If we have
              one or more ages that we found in the text, compare against
              what we already have set to print (lookup & ML & L2R & Vocab
              Density) and make sure we don't remove the triggers from the
              actual text CT Text Lookup.  Right now this has been removed
              since it is almost a 50/50 toss up on helping and hurting
              Check Tags.

              with removal: Recall: 74.45%  Precision: 81.87%  F1: 77.99%
              without:      Recall: 74.94%  Precision: 81.60%  F1: 78.12%
   */

   if(FALSE)
   {
       for(i = 0; i < numAgeCTs; i++)
       {
           if(!Age_CT_list[i].found && Age_CT_list[i].inListAlready)
           {
               p = Age_CT_list[i].listPos;
               if((p > -1) && (strstr(finalCTlist[p].trigger,
                                      "CT Text Lookup") == NULL))
               {
                   finalCTlist[p].oktoprint = FALSE;
                   fprintf(fout, 
                          "Age Range: Turning off %s [%s] Age not found (%s)\n",
                          finalCTlist[p].mh, finalCTlist[p].trigger,
                          citSpan);
               } /* fi */
           } /* fi */
       } /* for */
   } /* fi flag */
} /* checkAddCT */

/***************************************************************************
*
*  check_Species_Protein_SCRs --
*
*      This 
*
***************************************************************************/

void check_Species_Protein_SCRs(char *filename, char *targetSpecies)
{
   int ok, checkTIAB;
   long i, l, p, len, startPos, foundLen;
   char line[SMALL_LINE + 1], lookFor[SMALL_LINE + 1], DUI[250],
        SCR[SMALL_LINE + 1], tmp[SMALL_LINE + 1];
   FILE *fp;

   if(RTM_Debug)
   {
       fprintf(fout, "check_Species_Protein_SCRs(%s, %s)\n", filename, targetSpecies);
       fflush(fout);
   } /* fi */

   fp = fopen(filename, "r");
   if(fp != NULL)
   {
       while(fgets(line, SMALL_LINE, fp) != (char *)NULL)
       {
           strcpy(lookFor, "");
           strcpy(DUI, "");
           strcpy(SCR, "");
           checkTIAB = -1;
           sscanf(line, "%[^|]|%*[^|]|%[^|]|%[^|]|%d", lookFor, DUI,
                  SCR, &checkTIAB);
           len = (long)strlen(lookFor);
           if((len > 0) && (strlen(DUI) > 0) && (strlen(SCR) > 0) && (checkTIAB > -1))
           {
               for(l = 0; l < len; l++)
               {
                   if(isupper(lookFor[l]))
                     lookFor[l] = tolower(lookFor[l]);
               } /* for */

               ok = FALSE;
               fITpos = fITlen = -1;
               if(checkTIAB)
                ok = foundInText(lookFor, FALSE, FALSE);
               else
                ok = foundInText(lookFor, FALSE, TRUE);

               /* Have to keep these since we do further findInText to rule out some terms */

               startPos = fITpos;
               foundLen = fITlen;
               if(RTM_Debug)
               {
                   fprintf(fout, 
                   "check_Species_Protein_SCRs: ok: %d  fITpos: %ld  fITlen: %ld  lookFor: #%s#\n",
                           ok, fITpos, fITlen, lookFor);
                   fflush(fout);
               } /* fi */

               /* Add any special filtering here */

               /* Specific Low Back Pain (SLBP)
                  If we see "back pain", turn off the protein.
                */

               if(strcmp(lookFor, "slbp") == 0)
               {
                   if(foundInText("back pain", FALSE, FALSE))
                     ok = FALSE;
               } /* fi */

               if(strcmp(lookFor, "mapt") == 0)
               {
                   if(foundInText("mapt study", FALSE, FALSE))
                     ok = FALSE;
               } /* fi */

               /* copper transport vs choline tma lyase */

               if(strcmp(lookFor, "cutc") == 0)
               {
                   if(foundInText("choline", FALSE, FALSE))
                     ok = FALSE;
               } /* fi */

               /* eGFR */

               if(strcmp(lookFor, "egfr") == 0)
               {
                   if(!foundInText("EGFR", TRUE, FALSE))
                     ok = FALSE;
               } /* fi */

               /* functional and therapeutic outcomes (FTO) */

               if(strcmp(lookFor, "fto") == 0)
               {
                   if(foundInText("fluorine-doped tin oxide", FALSE, FALSE) ||
                      foundInText("fluorine doped tin oxide", FALSE, FALSE) || 
                      foundInText("functional and therapeutic outcomes", FALSE, FALSE) || 
                      foundInText("functional and therapeutic outcome", FALSE, FALSE))
                     ok = FALSE;
               } /* fi */

               if(strcmp(lookFor, "tert") == 0)
               {
                   if(!foundInText("TERT", TRUE, FALSE))
                     ok = FALSE;
               } /* fi */

               if(strcmp(lookFor, "htt") == 0)
               {
                   if(foundInText("5-htt", FALSE, FALSE) ||
                      foundInText("5 htt", FALSE, FALSE) ||
                      foundInText("5htt", FALSE, FALSE))
                     ok = FALSE;
               } /* fi */

               if(strcmp(lookFor, "tspo") == 0)
               {
                   if(!foundInText("TSPO", TRUE, FALSE))
                     ok = FALSE;
               } /* fi */

               if(ok)
               {
                   if(RTM_Debug)
                   {
                       fprintf(fout, "check_Species_Protein_SCRs: ");
                       fprintf(fout, "Forced SCR %s Protein Found - #%s#  trigger: #%s#\n",
                               targetSpecies, SCR, lookFor);
                       fflush(fout);
                   } /* fi */
                   if(!checkFinalMH(SCR))
                   {
                       p = numFinalMHs;
                       finalMHlist[p].foundInL2R = FALSE;
                       finalMHlist[p].ETflag = FALSE;
                       finalMHlist[p].datatype = NM;
                       finalMHlist[p].oktoprint = TRUE;
                       finalMHlist[p].wasForced = TRUE;
                       finalMHlist[p].score = 1000;
                       finalMHlist[p].num_RCs = 0;
                       finalMHlist[p].VocabDenFactor = 0.0;
                       finalMHlist[p].origPos = 10000;

                       finalMHlist[p].muid = strdup(muid);
                       finalMHlist[p].mh = strdup(SCR);
                       finalMHlist[p].mh_orig = strdup("");
                       finalMHlist[p].dui = strdup(DUI);
                       finalMHlist[p].entry_term = strdup("");
                       finalMHlist[p].RC_PMIDs = strdup("");
                       finalMHlist[p].textloc = strdup("TI");

                       strcpy(tmp, targetSpecies);
                       if(checkTIAB)
                         sprintf(tmp, "Forced SCR TIAB %s Protein Found: %s\0",
                                 targetSpecies, lookFor);
                       else
                         sprintf(tmp, "Forced SCR %s Protein Found: %s\0",
                                 targetSpecies, lookFor);

                       finalMHlist[p].trigger = strdup(tmp);

                       /* Positional Information */

                       finalMHlist[p].PIs[0].isStart = TRUE;
                       finalMHlist[p].PIs[0].startPos = startPos;
                       finalMHlist[p].PIs[0].endPos = startPos + foundLen;
                       finalMHlist[p].PIs[0].len = foundLen;
                       finalMHlist[p].PIs[0].hasBreak = FALSE;
                       finalMHlist[p].numPIs = 1;

                       /* Paths */

                       finalMHlist[p].paths[MMI] = TRUE;
                       finalMHlist[p].paths[PRC] = FALSE;

                       /* Treecodes */

                       finalMHlist[p].num_treecodes = 0;
                       pullTREEs(p);

                       /* Semantic Types - max 25 */

                       finalMHlist[p].num_STs = 0;

                       numFinalMHs++;

                       strcpy(tmp, targetSpecies);
                       if(checkTIAB)
                         sprintf(tmp, "Forced HM TIAB %s Protein SCR Found\0",
                                 targetSpecies);
                       else
                         sprintf(tmp, "Forced HM %s Protein SCR Found\0",
                                 targetSpecies);

                       check_HeadingMappedTo(SCR, tmp);
                   } /* fi */
               } /* fi */
           } /* fi */

           else
           {
               fprintf(stderr, 
                   "ERR: Bad Ts Protein Line: #%s#\n", targetSpecies, line);
               fflush(stderr);
           } /* else */
       } /* while */
   } /* fi */
} /* check_Species_Protein_SCRs */

/***************************************************************************
*
*  check_Species_microRNA_SCRs --
*
*      This 
*
***************************************************************************/

void check_Species_microRNA_SCRs(char *filename, char *targetSpecies)
{
   int ok, checkTIAB;
   long i, l, p, len, startPos, foundLen;
   char line[SMALL_LINE + 1], lookFor[SMALL_LINE + 1], DUI[250],
        SCR[SMALL_LINE + 1], tmp[SMALL_LINE + 1];
   FILE *fp;

   fp = fopen(filename, "r");
   if(fp != NULL)
   {
       while(fgets(line, SMALL_LINE, fp) != (char *)NULL)
       {
           strcpy(lookFor, "");
           strcpy(DUI, "");
           strcpy(SCR, "");
           checkTIAB = -1;
           sscanf(line, "%[^|]|%*[^|]|%[^|]|%[^|]|%d", lookFor, DUI, SCR, &checkTIAB);
           len = (long)strlen(lookFor);
           if((len > 0) && (strlen(DUI) > 0) && (strlen(SCR) > 0) && (checkTIAB > -1))
           {
               for(l = 0; l < len; l++)
               {
                   if(isupper(lookFor[l]))
                     lookFor[l] = tolower(lookFor[l]);
               } /* for */

               ok = FALSE;
               fITpos = fITlen = -1;

               if(strchr(lookFor, '*') == NULL)
               {
                   if(checkTIAB)
                    ok = foundInText(lookFor, FALSE, FALSE);
                   else
                    ok = foundInText(lookFor, FALSE, TRUE);
               } /* fi */

               else /* Look for trigger at beginning of word */
               {
                   lookFor[len -1] = '\0';
                   if(checkTIAB)
                    ok = foundInText_Star(lookFor, FALSE, FALSE);
                   else
                    ok = foundInText_Star(lookFor, FALSE, TRUE);
               } /* fi */

               /* Have to keep these since we do further findInText to rule out some terms */

               startPos = fITpos;
               foundLen = fITlen;
               if(RTM_Debug)
               {
                   fprintf(fout, 
                   "check_Species_microRNA_SCRs: ok: %d  fITpos: %ld  fITlen: %ld  lookFor: #%s#\n",
                           ok, fITpos, fITlen, lookFor);
                   fflush(fout);
               } /* fi */

               if(ok)
               {
                   if(!checkFinalMH(SCR))
                   {
                       p = numFinalMHs;
                       finalMHlist[p].foundInL2R = FALSE;
                       finalMHlist[p].ETflag = FALSE;
                       finalMHlist[p].datatype = NM;
                       finalMHlist[p].oktoprint = TRUE;
                       finalMHlist[p].wasForced = TRUE;
                       finalMHlist[p].score = 1000;
                       finalMHlist[p].num_RCs = 0;
                       finalMHlist[p].VocabDenFactor = 0.0;
                       finalMHlist[p].origPos = 10000;

                       finalMHlist[p].muid = strdup(muid);
                       finalMHlist[p].mh = strdup(SCR);
                       finalMHlist[p].mh_orig = strdup("");
                       finalMHlist[p].dui = strdup(DUI);
                       finalMHlist[p].entry_term = strdup("");
                       finalMHlist[p].RC_PMIDs = strdup("");
                       finalMHlist[p].textloc = strdup("TI");

                       strcpy(tmp, targetSpecies);
                       if(checkTIAB)
                         sprintf(tmp, "Forced SCR TIAB %s microRNA Found: %s\0",
                                 targetSpecies, lookFor);
                       else
                         sprintf(tmp, "Forced SCR %s microRNA Found: %s\0",
                                 targetSpecies, lookFor);

                       finalMHlist[p].trigger = strdup(tmp);

                       /* Positional Information */

                       finalMHlist[p].PIs[0].isStart = TRUE;
                       finalMHlist[p].PIs[0].startPos = startPos;
                       finalMHlist[p].PIs[0].endPos = startPos + foundLen;
                       finalMHlist[p].PIs[0].len = foundLen;
                       finalMHlist[p].PIs[0].hasBreak = FALSE;
                       finalMHlist[p].numPIs = 1;

                       /* Paths */

                       finalMHlist[p].paths[MMI] = TRUE;
                       finalMHlist[p].paths[PRC] = FALSE;

                       /* Treecodes */

                       finalMHlist[p].num_treecodes = 0;
                       pullTREEs(p);

                       /* Semantic Types - max 25 */

                       finalMHlist[p].num_STs = 0;

                       numFinalMHs++;

                       strcpy(tmp, targetSpecies);
                       if(checkTIAB)
                         sprintf(tmp, "Forced HM TIAB %s microRNA SCR Found\0",
                                 targetSpecies);
                       else
                         sprintf(tmp, "Forced HM %s microRNA SCR Found\0",
                                 targetSpecies);

                       check_HeadingMappedTo(SCR, tmp);
                   } /* fi */
               } /* fi */
           } /* fi */

           else
           {
               fprintf(stderr, 
                   "ERR: Bad Ts microRNA Line: #%s#\n", targetSpecies, line);
               fflush(stderr);
           } /* else */
       } /* while */
   } /* fi */
} /* check_Species_microRNA_SCRs */


/***************************************************************************
*
*  check_HeadingMappedTo --
*
*      This 
*
***************************************************************************/

void check_HeadingMappedTo(char *inSCR, char *triggerMsg)
{
   long i, uniq_ID, p;
   char **rows = NULL;
   long SCR_score, HM_score, high_score;
   char term[SMALL_LINE + 1], tmp[SMALL_LINE + 1], dui[50];
   int n, k, starred, pos;

   query_btree(HMIIPA, inSCR, &rows, &n);
   if((rows != NULL) && (n > 0))
   {
       for(k = 0; k < n; k++)
       {
           if(rows[k] != NULL)
           {
               if(strstr(rows[k], "|HM|") != NULL)
               {
                   strcpy(term, "");
                   strcpy(dui, "");
                   starred = FALSE;
                   sscanf(rows[k], "%*[^|]|%*[^|]|%[^|]|%d|%[^|]|",
                          dui, &starred, term);
                   if(strlen(dui) > 0)
                   {
                       if(!checkFinalMH(term))
                       {
                           p = numFinalMHs;
                           finalMHlist[p].foundInL2R = FALSE;
                           finalMHlist[p].ETflag = FALSE;
                           finalMHlist[p].datatype = MH;
                           finalMHlist[p].oktoprint = TRUE;
                           finalMHlist[p].score = 1000;
                           finalMHlist[p].num_RCs = 0;
                           finalMHlist[p].VocabDenFactor = 0.0;
                           finalMHlist[p].origPos = 10000;

                           finalMHlist[p].muid = strdup(muid);
                           finalMHlist[p].mh = strdup(term);
                           finalMHlist[p].mh_orig = strdup("");
                           finalMHlist[p].dui = strdup(dui);
                           finalMHlist[p].entry_term = strdup("");
                           finalMHlist[p].RC_PMIDs = strdup("");
                           finalMHlist[p].textloc = strdup("TI");
                           finalMHlist[p].trigger = strdup(triggerMsg);

                           /* Positional Information */

                           finalMHlist[p].numPIs = 0;

                           /* Paths */

                           finalMHlist[p].paths[MMI] = TRUE;
                           finalMHlist[p].paths[PRC] = FALSE;

                           /* Treecodes */

                           finalMHlist[p].num_treecodes = 0;
                           pullTREEs(p);

                           /* Semantic Types - max 25 */

                           finalMHlist[p].num_STs = 0;

                           numFinalMHs++;
                       } /* fi */
                   } /* fi */

                   free(rows[k]);
               } /* fi */
           } /* fi */
       } /* for each row in HMIIPA */
   } /* fi have row */

   if(rows != NULL)
     free(rows);
} /* check_HeadingMappedTo */

/***************************************************************************
*
*  SwapAnd --
*
*      This routine replaces "and" with "-" in cases like between 23 and 50
*
***************************************************************************/

char *SwapAnd(char *insegment)
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
   slen = (long)strlen("and");
   rlen = (long)strlen("-");

   strcpy(tmp2, "");
   tmp = strstr(tmp3, "and");
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

         rtn[rtn_pos++] = '-';
         rtn[rtn_pos] = '\0';

         /* Now save off the remainder of the string to process */

         pos = 0;
         for(j = search_end; j < tlen; j++)
           tmp2[pos++] = tmp3[j];
         tmp2[pos] = '\0';

         strcpy(tmp3, tmp2);
         tmp = strstr(tmp3, "and");
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

   free(tmp2); free(tmp3);

   len = (long)strlen(rtn);
   rtn = (char *)realloc(rtn, len + 1);

   return(rtn);
} /* SwapAnd */


/***************************************************************************
*
*  checkMaleFemalePct --
*
*      This 
*
***************************************************************************/

void checkMaleFemalePct()
{
   int foundMale, foundFemale, ok, lookForMale, lookForFemale, found, done;
   long i, p, pos, strLen, foundPos;
   char *lcCit, *snippet, from[SMALL_LINE + 1], *citSpan, *foo,
        term[250], dui[250], *tmp, loc[25];

   if(RTM_Debug)
   {
      fprintf(fout, "Entered checkMaleFemalePct\n");
      fflush(fout);
   } /* fi */

   begSpan = endSpan = -1;
   foundMale = foundFemale = FALSE;
   for(i = 0; i < numFinalCTs; i++)
   {
       if(finalCTlist[i].oktoprint)
       {
           if(strcmp(finalCTlist[i].mh, "Male") == 0)
              foundMale = TRUE;

           else if(strcmp(finalCTlist[i].mh, "Female") == 0)
              foundFemale = TRUE;
       } /* fi valid CT */
   } /* for */

   /* Going to ignore if already have both, or neither */

   ok = TRUE;
   if(foundMale && foundFemale)
     ok = FALSE;

   else if(!foundMale && !foundFemale)
     ok = FALSE;

   if(ok)
   {
       foo = checkSentMaleFemalePct(foundMale, foundFemale);

       /* foo will be empty if none of the triggers found */

       if(strlen(foo) > 0)
       {
           foundPos = -1;
           strcpy(term, "");
           strcpy(loc, "");
           sscanf(foo, "%*ld|%ld|%[^|]|%[^\n]", &foundPos, loc, term);

           strLen = (long)strlen(term);
           lcCit = (char *)strdup(citation);
           for(i = 0; i < cit_len; i++)
           {
               if(isupper(lcCit[i]))
                 lcCit[i] = tolower(lcCit[i]);
           } /* for */
           snippet = pullSnippetLeft(foundPos, lcCit, strLen);

           if(RTM_Debug)
           {
               fprintf(fout, "Sentence Male/Female Pct Snippet: #%s#\n", snippet);
               fflush(fout);
           } /* fi */

           if(foundFemale)
           {
               strcpy(term, "Male");
               strcpy(dui, "D008297");
           } /* fi Add Male */

           else
           {
               strcpy(term, "Female");
               strcpy(dui, "D005260");
           } /* else Add Female */

           /* First check to see if we can just turn it back on */

           pos = checkFinalCT(term);
           if(pos > -1)  /* Have it, but make sure oktoprint */
           {
               finalCTlist[pos].oktoprint = TRUE;
               tmp = (char *)malloc(strlen(finalCTlist[pos].trigger) + 1000);
               if(strlen(finalCTlist[pos].trigger) > 0)
                 sprintf(tmp, "%s; Forced Turn Back On - Male/Female Pct: %s\0",
                         finalCTlist[pos].trigger, snippet);
               else
                 sprintf(tmp, "Forced Turn Back On - Male/Female Pct: %s\0", snippet);

               free(finalCTlist[pos].trigger);
               finalCTlist[pos].trigger = strdup(tmp);
               free(tmp);
           } /* fi found - turn back on */

           else /* Didn't find, so need to add it */
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
               finalCTlist[p].mh = strdup(term);
               finalCTlist[p].mh_orig = strdup("");
               finalCTlist[p].dui = strdup(dui);
               finalCTlist[p].entry_term = strdup("");
               finalCTlist[p].RC_PMIDs = strdup("");
               tmp = (char *)malloc(strlen(snippet) + 1000);
               sprintf(tmp, "Forced Male/Female Pct: %s\0", snippet);
               finalCTlist[p].trigger = strdup(tmp);
               free(tmp);
               finalCTlist[p].textloc = strdup(loc);

               /* Positional Information */

               finalCTlist[p].PIs[0].isStart = TRUE;
               finalCTlist[p].PIs[0].startPos = begSpan;
               finalCTlist[p].PIs[0].endPos = endSpan;
               finalCTlist[p].PIs[0].len = strlen(snippet);
               finalCTlist[p].PIs[0].hasBreak = FALSE;
               finalCTlist[p].numPIs = 1;

               /* Paths */

               finalCTlist[p].paths[MMI] = TRUE;

               /* Treecodes */

               finalCTlist[p].num_treecodes = 0;
               pullTREEs_CT(p);

               /* Semantic Types - max 25 */

               finalCTlist[p].num_STs = 0;
               
               numFinalCTs++;
           } /* else !found */

           free(snippet);
           free(lcCit);
       } /* fi checkSentMaleFemalePct returned anything */
       free(foo);
   } /* fi ok */

   if(RTM_Debug)
   {
       fprintf(fout, "Leaving checkMaleFemalePct\n");
       fflush(fout);
   } /* fi */
} /* checkMaleFemalePct */


/***************************************************************************
*
*  AddSpecialAgedTerms --
*
*      This 
*
***************************************************************************/

void AddSpecialAgedTerms(int agePos, char *snippet, long begSpan)
{
   int addHumans, foundHumans;
   long p, pos, len, snippetLen;
   char *tmp;

   addHumans = foundHumans = FALSE;
   snippetLen = (long)strlen(snippet);

   /* See if we should reinstate it */

   if(Age_CT_list[agePos].inListAlready)
   {
       p = Age_CT_list[agePos].listPos;
       Age_CT_list[agePos].inListAlready = TRUE;

       if(!finalCTlist[p].oktoprint)
       {
           addHumans = TRUE;
           finalCTlist[p].oktoprint = TRUE;
           tmp = (char *)malloc(snippetLen + 1000);
           strcpy(tmp, "Forced Turn Back On - Age Group: ");
           strcat(tmp, snippet);
           free(finalCTlist[p].trigger);
           finalCTlist[p].trigger = strdup(tmp);
           free(tmp);
       } /* fi !oktoprint */

       else /* add to exising trigger */
       {
           addHumans = TRUE;
           len = (long)strlen(finalCTlist[p].trigger);
           tmp = (char *)malloc(snippetLen + len + 1000);
           if(len > 0)
             sprintf(tmp, "%s; Forced Age Group: %s\0",
                     finalCTlist[p].trigger, snippet);
           else
             sprintf(tmp, "Forced Age Group: %s\0", snippet);
           free(finalCTlist[p].trigger);
           finalCTlist[p].trigger = strdup(tmp);
           free(tmp);
       } /* else */

       pos = finalCTlist[p].numPIs;
       finalCTlist[p].PIs[pos].isStart = TRUE;
       finalCTlist[p].PIs[pos].startPos = begSpan;
       finalCTlist[p].PIs[pos].endPos = begSpan + snippetLen;
       finalCTlist[p].PIs[pos].len = strlen(snippet);
       finalCTlist[p].PIs[pos].hasBreak = FALSE;
       finalCTlist[p].numPIs++;
   } /* fi */

   else /* Add new */
   {
       addHumans = TRUE;
       p = numFinalCTs;
       Age_CT_list[agePos].inListAlready = TRUE;
       Age_CT_list[agePos].listPos = p;
       finalCTlist[p].foundInL2R = FALSE;
       finalCTlist[p].ETflag = FALSE;
       finalCTlist[p].datatype = MH;
       finalCTlist[p].oktoprint = TRUE;
       finalCTlist[p].score = 1000;
       finalCTlist[p].num_RCs = 0;
       finalCTlist[p].VocabDenFactor = 1.9;
       finalCTlist[p].origPos = 10000;

       finalCTlist[p].muid = strdup(muid);
       finalCTlist[p].mh = strdup(Age_CT_list[agePos].Name);
       finalCTlist[p].mh_orig = strdup("");
       finalCTlist[p].dui = strdup(Age_CT_list[agePos].DUI);
       finalCTlist[p].entry_term = strdup("");
       finalCTlist[p].RC_PMIDs = strdup("");
       tmp = (char *)malloc(snippetLen + 1000);
       sprintf(tmp, "Forced Age Group: %s\0", snippet);
       finalCTlist[p].trigger = strdup(tmp);
       free(tmp);
       finalCTlist[p].textloc = strdup("");

       /* Positional Information */

       finalCTlist[p].PIs[0].isStart = TRUE;
       finalCTlist[p].PIs[0].startPos = begSpan;
       finalCTlist[p].PIs[0].endPos = begSpan + snippetLen;
       finalCTlist[p].PIs[0].len = strlen(snippet);
       finalCTlist[p].PIs[0].hasBreak = FALSE;
       finalCTlist[p].numPIs = 1;

       /* Paths */

       finalCTlist[p].paths[MMI] = TRUE;

       /* Treecodes */

       finalCTlist[p].num_treecodes = 0;
       pullTREEs_CT(p);

       /* Semantic Types - max 25 */

       finalCTlist[p].num_STs = 0;

       numFinalCTs++;
   } /* else */

   /* If we added this, we need to make sure Humans is also being indexed */

   if(addHumans)
   {
       pos = checkFinalCT("Humans");
       if(pos > -1)  /* Have it, but make sure oktoprint */
       {
           if(!finalCTlist[pos].oktoprint)
           {
               finalCTlist[pos].oktoprint = TRUE;
               tmp = (char *)malloc(snippetLen + 1000);
               strcpy(tmp, "Forced Turn Back On - Age Group: ");
               strcat(tmp, snippet);
               free(finalCTlist[pos].trigger);
               finalCTlist[pos].trigger = strdup(tmp);
               free(tmp);
           } /* fi */
       } /* fi */

       else /* Add Humans */
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
           finalCTlist[p].mh = strdup("Humans");
           finalCTlist[p].mh_orig = strdup("");
           finalCTlist[p].dui = strdup("D006801");
           finalCTlist[p].entry_term = strdup("");
           finalCTlist[p].RC_PMIDs = strdup("");
           tmp = (char *)malloc(snippetLen + 1000);
           sprintf(tmp, "Forced Age Group Added: %s\0", snippet);
           finalCTlist[p].trigger = strdup(tmp);
           free(tmp);
           finalCTlist[p].textloc = strdup("");

           /* Positional Information */

           finalCTlist[p].PIs[0].isStart = TRUE;
           finalCTlist[p].PIs[0].startPos = begSpan;
           finalCTlist[p].PIs[0].endPos = begSpan + snippetLen;
           finalCTlist[p].PIs[0].len = strlen(snippet);
           finalCTlist[p].PIs[0].hasBreak = FALSE;
           finalCTlist[p].numPIs = 1;

           /* Paths */

           finalCTlist[p].paths[MMI] = TRUE;

           /* Treecodes */

           finalCTlist[p].num_treecodes = 0;
           pullTREEs_CT(p);

           /* Semantic Types - max 25 */

           finalCTlist[p].num_STs = 0;

           numFinalCTs++;
       } /* else */
   } /* fi */
} /* AddSpecialAgedTerms */


/***************************************************************************
*
*  lookForAgedTermsRight --
*
*      This 
*
***************************************************************************/

long lookForAgedTermsRight(char *haystack)
{
   int found, ok;
   long foundPos, i;

   foundPos = -1;
   found = FALSE;
   for(i = 0; !found && (i < numAgeLookForRight); i++)
   {
       ok = TRUE;
       if(strcmp(ageTriggersRight[i], "aged") == 0)
       {
           if((strstr(haystack, "aged between") != NULL) ||
              (strstr(haystack, "aged 1") != NULL) || (strstr(haystack, "aged 2") != NULL) ||
              (strstr(haystack, "aged 3") != NULL) || (strstr(haystack, "aged 4") != NULL) ||
              (strstr(haystack, "aged 5") != NULL) || (strstr(haystack, "aged 6") != NULL) ||
              (strstr(haystack, "aged 7") != NULL) || (strstr(haystack, "aged 8") != NULL) ||
              (strstr(haystack, "aged 9") != NULL) || (strstr(haystack, "aged 0") != NULL) ||
              (strstr(haystack, "aged one") != NULL) || (strstr(haystack, "aged two") != NULL) ||
              (strstr(haystack, "aged three") != NULL) || (strstr(haystack, "aged four") != NULL) ||
              (strstr(haystack, "aged five") != NULL) ||
              (strstr(haystack, "aged six") != NULL) ||
              (strstr(haystack, "aged seven") != NULL) ||
              (strstr(haystack, "aged eight") != NULL) ||
              (strstr(haystack, "aged nine") != NULL) ||
              (strstr(haystack, "aged ten") != NULL) ||
              (strstr(haystack, "aged at least") != NULL) ||
              (strstr(haystack, "aged from") != NULL) || 
              (strstr(haystack, "aged  ranged") != NULL) ||
              (strstr(haystack, "aged  ranges") != NULL) ||
              (strstr(haystack, "aged ranged") != NULL) ||
              (strstr(haystack, "aged ranges") != NULL) ||
              (strstr(haystack, "aged range") != NULL) ||
              (strstr(haystack, "aged less") != NULL) ||
              (strstr(haystack, "aged more") != NULL) ||
              (strstr(haystack, "females aged") != NULL) || 
              (strstr(haystack, "female aged") != NULL) ||
              (strstr(haystack, "women aged") != NULL) ||
              (strstr(haystack, "woman aged") != NULL) ||
              (strstr(haystack, "males aged") != NULL) ||
              (strstr(haystack, "male aged") != NULL) ||
              (strstr(haystack, "men aged") != NULL) ||
              (strstr(haystack, "man aged") != NULL) ||
              (strstr(haystack, "adults aged") != NULL) ||
              (strstr(haystack, "adults aged") != NULL) ||
              (strstr(haystack, "adolescents aged") != NULL) ||
              (strstr(haystack, "adolescent aged") != NULL) ||
              (strstr(haystack, "infants aged") != NULL) ||
              (strstr(haystack, "infant aged") != NULL) ||
              (strstr(haystack, "toddlers aged") != NULL) ||
              (strstr(haystack, "toddler aged") != NULL) ||
              (strstr(haystack, "children aged") != NULL) ||
              (strstr(haystack, "child aged") != NULL) ||
              (strstr(haystack, "students aged") != NULL) ||
              (strstr(haystack, "student aged") != NULL) ||
              (strstr(haystack, "middle aged") != NULL) ||
              (strstr(haystack, "middle-aged") != NULL) ||
              (strstr(haystack, "school aged") != NULL) ||
              (strstr(haystack, "school-aged") != NULL) ||
              (strstr(haystack, "preschool aged") != NULL) ||
              (strstr(haystack, "preschool-aged") != NULL) ||
              (strstr(haystack, "college aged") != NULL) ||
              (strstr(haystack, "college-aged") != NULL) ||
              (strstr(haystack, "reproductive aged") != NULL) ||
              (strstr(haystack, "reproductive-aged") != NULL) ||
              (strstr(haystack, "median aged") != NULL) ||
              (strstr(haystack, "mean aged") != NULL) ||
              (strstr(haystack, "aged (") != NULL) || (strstr(haystack, "aged(") != NULL) ||
              (strstr(haystack, "aged:") != NULL) || (strstr(haystack, "aged :") != NULL) ||
              (strstr(haystack, "aged<") != NULL) || (strstr(haystack, "aged <") != NULL) ||
              (strstr(haystack, "aged>") != NULL) || (strstr(haystack, "aged >") != NULL) ||
              (strstr(haystack, "aged ranged") != NULL) ||
              (strstr(haystack, "aged ranges") != NULL) ||
              (strstr(haystack, "aged range") != NULL))
            ok = FALSE;
       } /* fi */

       if(ok)
         foundPos = isTriggerInText(ageTriggersRight[i], haystack);
       else
         foundPos = -1;

       if(foundPos > -1)
       {
           found = TRUE;
           foundAgeTrigger = strdup(ageTriggersRight[i]);
       } /* fi */
   } /* for */

   return(foundPos);
} /* lookForAgedTermsRight */


/***************************************************************************
*
*  lookForAgedTermsLeft --
*
*      This 
*
***************************************************************************/

long lookForAgedTermsLeft(char *haystack)
{
   int found;
   long foundPos, i;

   foundPos = -1;
   found = FALSE;
   for(i = 0; !found && (i < numAgeLookForLeft); i++)
   {
       foundPos = isTriggerInText(ageTriggersLeft[i], haystack);
       if(foundPos > -1)
       {
           found = TRUE;
           foundAgeTrigger = strdup(ageTriggersLeft[i]);
       } /* fi */
   } /* for */

   return(foundPos);
} /* lookForAgedTermsLeft */

/***************************************************************************
*
*  XoutTriggerInText --
*
*      This 
*
***************************************************************************/

char *XoutTriggerInText(char *lookFor, char *text)
{
   char *base, *foo, *baseLookFor;
   int okLeft, okRight;
   long i, j, baseLen, lfLen, diff, rPos, pos;

   okLeft = okRight = FALSE;
   lfLen = (long)strlen(lookFor);
   triggerLen = lfLen;
   base = strdup(text);
   baseLen = (long)strlen(text);
   baseLookFor = strdup(lookFor);

   foo = strstr(base, baseLookFor);
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

          /* Only actual lookFor, no plural allowed */

          if((pos >= baseLen) || isspace(base[pos]) || ispunct(base[pos]))
            okRight = TRUE;
       } /* fi okLeft */

       for(j = 0; j < lfLen; j++)
          base[j + diff] = 'X';

       foo = strstr(base, baseLookFor);
  } /* while foo */

   free(baseLookFor);
   return(base);
} /* XoutTriggerInText */



/***************************************************************************
*
*  lookAtConfidence --
*
*      This 
*
***************************************************************************/

void lookAtConfidence()
{
   int newTerm;
   long i;
   char *tmp;

   /* What journal?
      Vocabulary Density?
      Title Only Article?
      Found in L2R?
      What paths? MM/Trigger/PRC?
      
   */

   /* Look at CTs first */

   for(i = 0; i < numFinalCTs; i++)
   {
       if(finalCTlist[i].oktoprint)
       {
           tmp = getVocabDen(nlmID, finalCTlist[i].dui);
           fprintf(fout, "Confidence [CT]: #%s# - Vocab: #%s#\n",
                   finalCTlist[i].mh, tmp);
           fflush(fout);
           free(tmp);
       } /* fi */
   } /* for */


   /* Look at MHs */

   for(i = 0; i < numFinalMHs; i++)
   {
       if(finalMHlist[i].oktoprint)
       {
           tmp = getVocabDen(nlmID, finalMHlist[i].dui);
           fprintf(fout, "Confidence [MH]: #%s# - Vocab: #%s#\n",
                   finalMHlist[i].mh, tmp);
           fflush(fout);
           free(tmp);
       } /* fi */
   } /* for */
} /* lookAtConfidence */


/***************************************************************************
*
*  checkAddHumans --
*
*      This 
*
***************************************************************************/

void checkAddHumans(int foundHumanNoPrint, long humanPos, char *trigger)
{
   int ok;
   long i, p;
   char *tmp;

   if(RTM_Debug)
   {
       fprintf(fout, "Entered checkAddHumans\n");
       fflush(fout);
   } /* fi */

   /* Found a trigger, so either turn Humans back on or add */

   if(foundHumanNoPrint && (humanPos > -1))
   {
       if(RTM_Debug)
       {
           fprintf(fout, "Humans Turn Back  On trigger: %s\n", trigger);
           fflush(fout);
       } /* fi */

       finalCTlist[humanPos].oktoprint = TRUE;
       tmp = (char *)malloc(strlen(finalCTlist[humanPos].trigger) + 1000);
       if(strlen(finalCTlist[humanPos].trigger) > 0)
         sprintf(tmp, "%s; Forced Human Turn Back On: %s\0",
                 finalCTlist[humanPos].trigger, trigger);
       else
         sprintf(tmp, "Forced Human Turn Back On: %s\0",
                 finalCTlist[humanPos].trigger, trigger);
       free(finalCTlist[humanPos].trigger);
       finalCTlist[humanPos].trigger = strdup(tmp);
       free(tmp);
   } /* fi have in list */

   else /* We need to add it */
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
       finalCTlist[p].mh = strdup("Humans");
       finalCTlist[p].mh_orig = strdup("");
       finalCTlist[p].dui = strdup("D006801");
       finalCTlist[p].entry_term = strdup("");
       finalCTlist[p].RC_PMIDs = strdup("");
       tmp = (char *)malloc(strlen(trigger) + 1000);
       sprintf(tmp, "Forced Humans:D %s\0", trigger);
       finalCTlist[p].trigger = strdup(tmp);
       free(tmp);
       finalCTlist[p].textloc = strdup("");

       /* Positional Information */

       finalCTlist[p].numPIs = 0;

       /* Paths */

       finalCTlist[p].paths[MMI] = TRUE;

       /* Treecodes */

       finalCTlist[p].num_treecodes = 0;
       pullTREEs_CT(p);

       /* Semantic Types - max 25 */

       finalCTlist[p].num_STs = 0;
               
       numFinalCTs++;
   } /* else */

   if(RTM_Debug)
   {
       fprintf(fout, "Leaving checkAddHumans\n");
       fflush(fout);
   } /* fi */
} /* checkAddHumans */


/***************************************************************************
*
*  findSnippet --
*
*      This 
*
***************************************************************************/

long findSnippet(long startPos)
{
    int found;
    long i, rtn;

    rtn = -1;
    found = FALSE;
    for(i = 0; !found && (i < numFTS); i++)
    {
        if(startPos >= FT_Snippets[i].startPos)
          rtn = i;

        else if(rtn > -1)
           found = TRUE;
    } /* for */

    return(rtn);
} /* findSnippet */



/***************************************************************************
*
*  lookForAnimalDiseaseAdds --
*
*      This 
*
***************************************************************************/

void lookForAnimalDiseaseAdds()
{
   int foundAnimals, foundBirds, foundCats, foundCattle, foundDogs, foundFish, foundGoats,
       foundHorses, foundMonkeys, foundRodents, foundSheep, foundSwine, foundDiseases,
       foundCatDisease, foundCattleDisease, foundDogDisease, foundHorseDisease, foundRodentDisease,
       foundSheepDisease, foundSwineDisease, foundGoatDisease;
   long i, l;
   char mh[SMALL_LINE + 1];

   foundAnimals = foundBirds = foundCats = foundCattle = foundDogs = foundFish = foundGoats = FALSE;
   foundHorses = foundMonkeys = foundRodents = foundSheep = foundSwine = foundDiseases = FALSE;
   foundCatDisease = foundCattleDisease = foundDogDisease = foundHorseDisease = FALSE;
   foundRodentDisease = foundSheepDisease = foundSwineDisease = FALSE;
   foundGoatDisease = FALSE;

   for(i = 0; i < numFinalCTs; i++)
   {
       if(finalCTlist[i].oktoprint)
       {
            if(finalCTlist[i].mh[0] == 'A')
            {
                if(strcmp(finalCTlist[i].mh, "Animals") == 0)
                  foundAnimals = TRUE;
            } /* fi 'A' */

            else if(finalCTlist[i].mh[0] == 'C')
            {
                if(strcmp(finalCTlist[i].mh, "Cats") == 0)
                  foundCats = TRUE;

                else if(strcmp(finalCTlist[i].mh, "Cattle") == 0)
                  foundCattle = TRUE;

                else if(strcmp(finalCTlist[i].mh, "Cricetinae") == 0)
                  foundRodents = TRUE;
            } /* else fi 'C' */

            else if(finalCTlist[i].mh[0] == 'D')
            {
                if(strcmp(finalCTlist[i].mh, "Dogs") == 0)
                  foundDogs = TRUE;
            } /* else fi 'D' */

            else if(finalCTlist[i].mh[0] == 'H')
            {
                if(strcmp(finalCTlist[i].mh, "Horses") == 0)
                  foundHorses = TRUE;
            } /* else fi 'H' */

            else if(finalCTlist[i].mh[0] == 'M')
            {
                if(strcmp(finalCTlist[i].mh, "Mice") == 0)
                  foundRodents = TRUE;
            } /* else fi 'M' */

            else if(finalCTlist[i].mh[0] == 'R')
            {
                if(strcmp(finalCTlist[i].mh, "Rats") == 0)
                  foundRodents = TRUE;
            } /* else fi 'R' */

            else if(finalCTlist[i].mh[0] == 'S')
            {
                if(strcmp(finalCTlist[i].mh, "Sheep") == 0)
                  foundSheep = TRUE;

                else if(strcmp(finalCTlist[i].mh, "Swine") == 0)
                  foundSwine = TRUE;
            } /* else fi 'C' */
       } /* fi oktoprint */
   } /* for each CT */

   /* Only proceed if we at least have Animals found */

   if(foundAnimals)
   {
       for(i = 0; i < numFinalMHs; i++)
       {
           if(strstr(finalMHlist[i].trigger, "Entry Term Replacement") != NULL)
             strcpy(mh, finalMHlist[i].mh_orig);

           else
             strcpy(mh, finalMHlist[i].mh);

           if(mh[0] == 'C')
           {
               if(strcmp(mh, "Cat Diseases") == 0)
                 foundCatDisease = TRUE;

               else if(strcmp(mh, "Cattle Diseases") == 0)
                 foundCattleDisease = TRUE;
           } /* fi */

           else if(mh[0] == 'D')
           {
               if(strcmp(mh, "Dog Diseases") == 0)
                 foundDogDisease = TRUE;
           } /* else fi */

           else if(mh[0] == 'G')
           {

               if(strcmp(mh, "Goats") == 0)
                 foundGoats = TRUE;

               else if(strcmp(mh, "Goat Diseases") == 0)
                 foundGoatDisease = TRUE;
           } /* else fi */

           else if(mh[0] == 'H')
           {
               if(strcmp(mh, "Horse Diseases") == 0)
                 foundHorseDisease = TRUE;
           } /* else fi */

           else if(mh[0] == 'R')
           {
               if(strcmp(mh, "Rodent Diseases") == 0)
                 foundRodentDisease = TRUE;
           } /* else fi */

           else if(mh[0] == 'S')
           {
               if(strcmp(mh, "Sheep Diseases") == 0)
                 foundSheepDisease = TRUE;

               else if(strcmp(mh, "Swine Diseases") == 0)
                 foundSwineDisease = TRUE;
           } /* else fi */

           if((finalMHlist[i].datatype == MH) && finalMHlist[i].oktoprint)
           {
               for(l = 0; (l < finalMHlist[i].num_treecodes); l++)
               {
                   if(finalMHlist[i].treecodes[l][0] == 'B')
                   {
                       if(strstr(finalMHlist[i].treecodes[l],    /* Rodentia treecode */
                                 "B01.050.150.900.649.313.992") != NULL)
                         foundRodents = TRUE;
                   } /* fi B */

                   /* NOTE: Do not want just symptoms trigger - December 29, 2020 */

                   else if((finalMHlist[i].treecodes[l][0] == 'C') &&
                          (strstr(finalMHlist[i].treecodes[l], "C23.888") == NULL))
                      foundDiseases = TRUE;
               } /* for each treecode */
           } /* fi valid MH */
       } /* for each MH */

       /* Now, see if we have a disease and any one of the types of Animals we are looking for */

       if(foundDiseases && (foundCats || foundCattle || foundDogs || foundHorses || 
                            foundRodents || foundSheep || foundSwine || foundGoats))
       {
           if(foundCats && !foundCatDisease)
             addAnimalDiseaseTerm("Cat Diseases", "D002371");

           if(foundCattle && !foundCattleDisease)
             addAnimalDiseaseTerm("Cattle Diseases", "D002418");

           if(foundDogs && !foundDogDisease)
             addAnimalDiseaseTerm("Dog Diseases", "D004283");

           if(foundGoats && !foundGoatDisease)
             addAnimalDiseaseTerm("Goat Diseases", "D015511");

           if(foundHorses && !foundHorseDisease)
             addAnimalDiseaseTerm("Horse Diseases", "D006734");

           if(foundRodents && !foundRodentDisease)
             addAnimalDiseaseTerm("Rodent Diseases", "D012376");

           if(foundSheep && !foundSheepDisease)
             addAnimalDiseaseTerm("Sheep Diseases", "D012757");

           if(foundSwine && !foundSwineDisease)
             addAnimalDiseaseTerm("Swine Diseases", "D013553");
       } /* fi found other term */
   } /* fi foundAnimals */
} /* lookForAnimalDiseaseAdds */



/***************************************************************************
*
*  addAnimalDiseaseTerm --
*
*      This 
*
***************************************************************************/

void addAnimalDiseaseTerm(char *mh, char *dui)
{
   long p;

   p = numFinalMHs;
   finalMHlist[p].foundInL2R = FALSE;
   finalMHlist[p].ETflag = FALSE;
   finalMHlist[p].datatype = MH;
   finalMHlist[p].oktoprint = TRUE;
   finalMHlist[p].wasForced = TRUE;
   finalMHlist[p].score = 1000;
   finalMHlist[p].num_RCs = 0;
   finalMHlist[p].VocabDenFactor = 0.0;
   finalMHlist[p].origPos = 10000;

   finalMHlist[p].muid = strdup(muid);
   finalMHlist[p].mh = strdup(mh);
   finalMHlist[p].mh_orig = strdup("");
   finalMHlist[p].dui = strdup(dui);
   finalMHlist[p].entry_term = strdup("");
   finalMHlist[p].RC_PMIDs = strdup("");
   finalMHlist[p].textloc = strdup("TI");
   finalMHlist[p].trigger = strdup("Forced Animal Diseases");

   /* Positional Information */

   finalMHlist[p].numPIs = 0;

   /* Paths */

   finalMHlist[p].paths[MMI] = TRUE;
   finalMHlist[p].paths[PRC] = FALSE;

   /* Treecodes */

   finalMHlist[p].num_treecodes = 0;
   pullTREEs(p);

   /* Semantic Types - max 25 */

   finalMHlist[p].num_STs = 0;

   numFinalMHs++;
} /* addAnimalDiseaseTerm */



/***************************************************************************
*
*  check_PTs_Protocol --
*
*      This 
*
***************************************************************************/

void check_PTs_Protocol()
{
   long i;
   char from[SMALL_LINE + 1];

   for(i = 0; i < numPTsFound; i++)
   {
       if(foundPTsList[i].oktoprint)
       {
           if((strcmp(foundPTsList[i].mh, "Clinical Trial, Phase I") == 0) ||
              (strcmp(foundPTsList[i].mh, "Clinical Trial, Phase II") == 0) ||
              (strcmp(foundPTsList[i].mh, "Clinical Trial, Phase III") == 0) ||
              (strcmp(foundPTsList[i].mh, "Clinical Trial, Phase IV") == 0))
           {
               if((strstr(foundPTsList[i].textloc, "TI") != NULL) ||
                  (strstr(foundPTsList[i].textloc, "AB") != NULL))
               {
                   foundPTsList[i].oktoprint = FALSE;
                   sprintf(from, "PT Protocol Rule: %s\0", foundPTsList[i].mh);
                   process_PTterm("Clinical Trial Protocol", "TI", "", from, "D000078325", "");
                   if(RTM_Debug)
                   {
                       fprintf(fout, "Protocol Rule - change '%s' to Clinical Trial Protocol\n",
                               foundPTsList[i].mh);
                       fflush(fout);
                   } /* fi */

                   /* Now switch triggering PT to the as Topic version */

                   makePTasTopic(i, "PT Protocol Rule");
               } /* fi in Title */
           } /* fi */

           else if((strcmp(foundPTsList[i].mh, "Randomized Controlled Trial") == 0) ||
              (strcmp(foundPTsList[i].mh, "Randomized Controlled Trial, Veterinary") == 0))
           {
               if(!foundInText("protocol for systematic review and meta-analysis", FALSE, TRUE) &&
                  !foundInText("protocol for meta-analysis and systematic review", FALSE, TRUE))
               {
                   if((strstr(foundPTsList[i].textloc, "TI") != NULL) ||
                      (strstr(foundPTsList[i].textloc, "AB") != NULL))
                   {
                       foundPTsList[i].oktoprint = FALSE;
                       sprintf(from, "PT Protocol Rule: %s\0", foundPTsList[i].mh);
                       process_PTterm("Clinical Trial Protocol", "TI", "", from, "D000078325", "");
                       if(RTM_Debug)
                       {
                           fprintf(fout, "Protocol Rule - change '%s' to Clinical Trial Protocol\n",
                                   foundPTsList[i].mh);
                           fflush(fout);
                       } /* fi */

                       /* Now switch triggering PT to the as Topic version */

                       makePTasTopic(i, "PT Protocol Rule");
                   } /* fi in Title */
               } /* if not protocol for SR & MA */
           } /* else fi */

           else if((strcmp(foundPTsList[i].mh, "Multicenter Study") == 0) ||
              (strcmp(foundPTsList[i].mh, "Observational Study") == 0) ||
              (strcmp(foundPTsList[i].mh, "Observational Study, Veterinary") == 0) ||
              (strcmp(foundPTsList[i].mh, "Meta-Analysis") == 0) ||
              (strcmp(foundPTsList[i].mh, "Review") == 0) ||
              (strcmp(foundPTsList[i].mh, "Systematic Review") == 0))
           {
               if((strstr(foundPTsList[i].textloc, "TI") != NULL) ||
                  (strstr(foundPTsList[i].textloc, "AB") != NULL))
               {
                   foundPTsList[i].oktoprint = FALSE;
                   if(RTM_Debug)
                   {
                       fprintf(fout, "Protocol Rule - change '%s' to as Topic Version\n",
                               foundPTsList[i].mh);
                       fflush(fout);
                   } /* fi */

                   /* Now switch triggering PT to the as Topic version */

                   makePTasTopic(i, "PT Protocol Rule");
               } /* fi in Title */
           } /* fi */
       } /* fi oktoprint */
   } /* for each PT */
} /* check_PTs_Protocol */



/***************************************************************************
*
*  check_PTs_Reviews --
*
*      This 
*
***************************************************************************/

void check_PTs_Reviews()
{
   int foundTrigger;
   long i;

   /* 
       Review, Systematic Review, and Meta-Analysis PT cannot co-occur with the following:
            Phase I Clinical Trial
            Phase II Clinical Trial
            Phase III Clinical Trial
            Randomized Controlled Trial
            Randomized Controlled Trial, Veterinary
            Multicenter Study
            Observational Study

So if MTI has trigger to index Review, Systematic Review, or Meta-analysis and has trigger to index any of those other PTS listed above, keep Review, Systematic Review, and Meta-analysis and convert the other PT  to the as topic version.  For example: MTI has triggers to index both RCT PT and Systematic Review PT ? keep Systematic Review PT and index RCT as topic.
   */
   foundTrigger = FALSE;
   for(i = 0; !foundTrigger && (i < numPTsFound); i++)
   {
       if(foundPTsList[i].oktoprint)
       {
           if((strcmp(foundPTsList[i].mh, "Meta-Analysis") == 0) ||
              (strcmp(foundPTsList[i].mh, "Review") == 0) ||
              (strcmp(foundPTsList[i].mh, "Systematic Review") == 0))
             foundTrigger = TRUE;
       } /* fi oktoprint */
   } /* for each PT */

   /* Make sure to check the Existing Publisher Supplied list of PTs as well */

   if(!foundTrigger)
   {
       for(i = 0; !foundTrigger && (i < numExistingPTs); i++)
       {
           if((strcmp(ExistPTs[i], "Meta-Analysis") == 0) ||
              (strcmp(ExistPTs[i], "Review") == 0) ||
              (strcmp(ExistPTs[i], "Systematic Review") == 0))
             foundTrigger = TRUE;
       } /* for each PT */
   } /* fi */

   if(foundTrigger)
   {
       for(i = 0; i < numPTsFound; i++)
       {
           if(foundPTsList[i].oktoprint)
           {
               if((strcmp(foundPTsList[i].mh, "Clinical Trial, Phase I") == 0) ||
                  (strcmp(foundPTsList[i].mh, "Clinical Trial, Phase II") == 0) ||
                  (strcmp(foundPTsList[i].mh, "Clinical Trial, Phase III") == 0) ||
                  (strcmp(foundPTsList[i].mh, "Clinical Trial, Phase IV") == 0) ||
                  (strcmp(foundPTsList[i].mh, "Randomized Controlled Trial") == 0) ||
                  (strcmp(foundPTsList[i].mh, "Randomized Controlled Trial, Veterinary") == 0) ||
                  (strcmp(foundPTsList[i].mh, "Multicenter Study") == 0) ||
                  (strcmp(foundPTsList[i].mh, "Observational Study") == 0) ||
                  (strcmp(foundPTsList[i].mh, "Observational Study, Veterinary") == 0))
               {
                   foundPTsList[i].oktoprint = FALSE;
                   if(RTM_Debug)
                   {
                       fprintf(fout, "PT Reviews Rule - change '%s' to as Topic Version\n",
                               foundPTsList[i].mh);
                       fflush(fout);
                   } /* fi */

                   /* Now switch triggering PT to the as Topic version */

                   makePTasTopic(i, "PT Reviews Rule");
               } /* fi select term */
           } /* fi oktoprint */
       } /* for each PT */
   } /* fi foundTrigger */
} /* check_PTs_Reviews */



/***************************************************************************
*
*  makePTasTopic --
*
*      This 
*
***************************************************************************/

void makePTasTopic(long ptPos, char *reason)
{
   int foundPT;
   long i, p, x, asTopicPos;
   char from[SMALL_LINE + 1];

   /* Figure out where in our PT/as Topic table this term is located. */

   foundPT = FALSE;
   asTopicPos = -1;
   for(x = 0; !foundPT && (x < numCheckPTs); x++)
   {
       if(foundPTsList[ptPos].mh[0] == casTPTlist[x].PT[0])
       {
           if(strcmp(foundPTsList[ptPos].mh, casTPTlist[x].PT) == 0)
           {
               foundPT = TRUE;
               asTopicPos = x;
           } /* fi */
       } /* fi */
   } /* for */

   if(foundPT)
   {
       if(numFinalMHs < MAX_FINAL_MHs)
       {
           p = numFinalMHs;
           finalMHlist[p].ETflag = foundPTsList[ptPos].ETflag;
           finalMHlist[p].datatype = foundPTsList[ptPos].datatype;
           finalMHlist[p].oktoprint = TRUE;
           finalMHlist[p].wasForced = TRUE;
           finalMHlist[p].score = foundPTsList[ptPos].score;
           finalMHlist[p].num_RCs = foundPTsList[ptPos].num_RCs;
           finalMHlist[p].VocabDenFactor = foundPTsList[ptPos].VocabDenFactor;
           finalMHlist[p].origPos = ptPos;

           finalMHlist[p].muid = strdup(muid);

           if(strlen(foundPTsList[ptPos].trigger) > 0)
              sprintf(from, "%s: Forced change to asTopic; %s\0", 
                      reason, foundPTsList[ptPos].trigger);
           else
              strcpy(from, foundPTsList[ptPos].trigger);

           finalMHlist[p].mh = strdup(casTPTlist[asTopicPos].asTopic);
           finalMHlist[p].mh_orig = strdup(casTPTlist[asTopicPos].asTopic);
           finalMHlist[p].dui = strdup(casTPTlist[asTopicPos].asTopic_UI);
           finalMHlist[p].trigger = strdup(from);

           finalMHlist[p].entry_term = strdup(foundPTsList[ptPos].entry_term);
           finalMHlist[p].RC_PMIDs = strdup(foundPTsList[ptPos].RC_PMIDs);
           finalMHlist[p].textloc = strdup(foundPTsList[ptPos].textloc);

           /* Positional Information */

           finalMHlist[p].numPIs = foundPTsList[ptPos].numPIs;
           for(i = 0; i < foundPTsList[ptPos].numPIs; i++)
           {
              finalMHlist[p].PIs[i].isStart = foundPTsList[ptPos].PIs[i].isStart;
              finalMHlist[p].PIs[i].startPos = foundPTsList[ptPos].PIs[i].startPos;
              finalMHlist[p].PIs[i].endPos = foundPTsList[ptPos].PIs[i].endPos;
              finalMHlist[p].PIs[i].len = foundPTsList[ptPos].PIs[i].len;
              finalMHlist[p].PIs[i].hasBreak = foundPTsList[ptPos].PIs[i].hasBreak;
           } /* for */

           /* Paths */

           for(i = 0; i < NUM_PATHS; i++)
             finalMHlist[p].paths[i] = foundPTsList[ptPos].paths[i];

           /* HMs for each SCR - shouldn't need here */

           finalMHlist[p].num_HMs = 0;

           /* Treecodes - only keep first 25 (shouldn't be a problem) */

           finalMHlist[p].num_treecodes = foundPTsList[ptPos].num_treecodes;
           if(finalMHlist[p].num_treecodes > 25)
             finalMHlist[p].num_treecodes = 25;
           for(i = 0; (i < 25) && (i < foundPTsList[ptPos].num_treecodes); i++)
              finalMHlist[p].treecodes[i] = strdup(foundPTsList[ptPos].treecodes[i]);

           /* If we don't have any treecodes, add them here */

           if(finalMHlist[p].num_treecodes == 0)
             pullTREEs(p);

           /* Semantic Types - max 25 */

           finalMHlist[p].num_STs = foundPTsList[ptPos].num_STs;
           for(i = 0; i < foundPTsList[ptPos].num_STs; i++)
           {
               finalMHlist[p].ST_info[i].MMI_trigger =
                                   strdup(foundPTsList[ptPos].ST_info[i].MMI_trigger);
               finalMHlist[p].ST_info[i].rtm_ST =
                                   strdup(foundPTsList[ptPos].ST_info[i].rtm_ST);
           } /* for */

           numFinalMHs++;
       } /* fi ok to add */
   } /* fi */

   else
   {
       fprintf(stderr, "ERR: Unable to identify PT (%s) in casTPTlist\n", foundPTsList[ptPos].mh);
       fflush(stderr);
   } /* else */
} /* makePTasTopic */



/***************************************************************************
*
*  addFemalePregnancy --
*
*      This 
*
***************************************************************************/

void addFemalePregnancy(long pregnancyPos)
{
   int found;
   long i, p;
   char tmp[SMALL_LINE + 1];

   /* We are indexing Pregnancy but do not have Female showing up.  Force Female to be
      indexed as well.  Most likely scenario is that it was turned off for some reason
      so we want to just turn it back on.  If it is completely not in the CheckTag list
      we want to add it.
   */

   /* See if Female was just turned off */

   found = FALSE;
   for(i = 0; !found && (i < numFinalCTs); i++)
   {
       if(!finalCTlist[i].oktoprint)
       {
           if(strcmp(finalCTlist[i].mh, "Female") == 0)
           {
               if(RTM_Debug)
               {
                   fprintf(fout, "Forced Female Turn Back On Pregnancy Found\n");
                   fflush(fout);
               } /* fi */
               found = TRUE;
               finalCTlist[i].oktoprint = TRUE;

               if(strlen(finalCTlist[i].trigger) > 0)
                 sprintf(tmp, "%s; Forced Female Turn Back On Pregnancy Found\0",
                         finalCTlist[i].trigger);
               else
                 strcpy(tmp, "Forced Female Turn Back On Pregnancy Found");

               free(finalCTlist[i].trigger);
               finalCTlist[i].trigger = strdup(tmp);
               strcpy(tmp, "");
           } /* fi match */
       } /* fi valid CT */
   } /* for */

   /* Didn't find, so need to add */

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
       finalCTlist[p].mh = strdup("Female");
       finalCTlist[p].mh_orig = strdup("");
       finalCTlist[p].dui = strdup("D005260");
       finalCTlist[p].entry_term = strdup("");
       finalCTlist[p].RC_PMIDs = strdup("");
       strcpy(tmp, "Forced Female Turn Back On Pregnancy Found");
       finalCTlist[p].trigger = strdup(tmp);
       finalCTlist[p].textloc = strdup("");

       /* Positional Information */

       if(finalCTlist[pregnancyPos].numPIs > 0)
       {
           finalCTlist[p].PIs[0].isStart = TRUE;
           finalCTlist[p].PIs[0].startPos = finalCTlist[pregnancyPos].PIs[0].startPos;
           finalCTlist[p].PIs[0].endPos = finalCTlist[pregnancyPos].PIs[0].endPos;
           finalCTlist[p].PIs[0].len = finalCTlist[pregnancyPos].PIs[0].len;
           finalCTlist[p].PIs[0].hasBreak = FALSE;
           finalCTlist[p].numPIs = 1;
       } /* fi */

       else
         finalCTlist[p].numPIs = 0;

       /* Paths */

       finalCTlist[p].paths[MMI] = TRUE;

       /* Treecodes - no treecodes for Male/Female */

       finalCTlist[p].num_treecodes = 0;

       /* Semantic Types - max 25 */

       finalCTlist[p].num_STs = 0;
               
       numFinalCTs++;
   } /* fi !found */
} /* addFemalePregnancy */


/***************************************************************************
*
*  verifyAmbig_Metaphor --
*
*      This 
*
***************************************************************************/

char *verifyAmbig_Metaphor(char *triggerList)
{
    int found, numFound, okAmbig;
    long i, j, pos, diff, numReg, numAmb, numFull;
    char tmp[SMALL_LINE + 1], item[SMALL_LINE + 1], rest[SMALL_LINE + 1],
         trigger[SMALL_LINE + 1], *foo, *rtn, tmp2[SMALL_LINE + 1], *regList[250], *ambList[250],
         *fullList[250];

    rtn = strdup(triggerList);

    numReg = numAmb = numFull = 0;
    strcpy(tmp, triggerList);
    while(strlen(tmp) > 0)
    {
        strcpy(item, "");
        strcpy(rest, "");
        sscanf(tmp, "%[^;];%[^\n]", item, rest);

        if(strstr(item, "Forced Full Text: ") != NULL)
        {
            strcpy(tmp2, "");
            sscanf(item, "Forced Full Text: %[^\n]", tmp2);
            strcpy(item, tmp2);
        } /* fi */

        /* CHO CeRtM via:  */

        if(strstr(item, "RtM via: ") != NULL)
        {
            foo = strstr(item, "RtM via: ");
            diff = foo - item;
            if(diff != 0)
            {
                strcpy(item, "");
                strcpy(rest, "");
            } /* fi */
        } /* fi */

        /* babAntonio ML Rule Forced */

        if(strstr(item, "Antonio ML") != NULL)
        {
            foo = strstr(item, "Antonio ML");
            diff = foo - item;
            if(diff != 0)
            {
                strcpy(item, "");
                strcpy(rest, "");
            } /* fi */
        } /* fi */

        /* autForced - Vocab Density */

        if(strstr(item, "Forced - Vocab") != NULL)
        {
            foo = strstr(item, "Forced - Vocab");
            diff = foo - item;
            if(diff != 0)
            {
                strcpy(item, "");
                strcpy(rest, "");
            } /* fi */
        } /* fi */

        /* Forced Leaf NoRtM via: Mus */

        if((strstr(item, "Forced Leaf ") != NULL) && 
           (strstr(item, "Forced Leaf Node Lookup:") == NULL))
        {
            strcpy(item, "");
            strcpy(rest, "");
        } /* fi */

        /* CT TeRtM via: Mus;CT Text Lookup: mouse */

        if((strstr(item, "CT Te") != NULL) && (strstr(item, "CT Text Lookup: ") == NULL))
        {
            strcpy(item, "");
            strcpy(rest, "");
        } /* fi */

        /* CT Text LookuRtM via: Mus;CT Text Lookup: mouse */

        if((strstr(item, "CT Text Look") != NULL) && (strstr(item, "CT Text Lookup: ") == NULL))
        {
            strcpy(item, "");
            strcpy(rest, "");
        } /* fi */

        /* CT Text Lookup: CT Text Lookup: choriocarcinoma */
        /* CT Text Lookup:CT Text Lookup: choriocarcinoma */

        if((strstr(item, "CT Text Lookup: CT Text ") != NULL) ||
           (strstr(item, "CT Text Lookup:CT Text ") != NULL))
        {
            strcpy(item, "");
            strcpy(rest, "");
        } /* fi */

        /* Check on count of forced terms: CT Text Lookup: humanCT Text Lookup: mouse */

        if(strlen(item) > 0)
        {
            numFound = countForced(item);
            if(numFound > 1)
            {
                strcpy(item, "");
                strcpy(rest, "");
            } /* fi */
        } /* fi */

        found = FALSE;
        numFound = 0;
        if(strlen(item) > 0)
        {
            for(i = 0; i < numForceChecks; i++)
            {
                foo = strstr(item, ForceChecks[i]);
                if(foo != NULL)
                {
                    numFound++;
                    diff = foo - item;
                    if(diff == 0)
                      found = TRUE;
                    else
                    {
                        strcpy(item, "");
                        strcpy(rest, "");
                    } /* else */
                } /* fi */
            } /* for */

            if(numFound > 1)
            {
                found = FALSE;
                strcpy(item, "");
                strcpy(rest, "");
            } /* fi */
        } /* fi */

        /* Now see if we have a trigger */

        if(found && (strlen(item) > 0))
        {
            foo = strstr(item, " found in ");
            if(foo != NULL)
            {
                diff = foo - item;
                item[diff] = '\0';
            } /* fi */

            strcpy(trigger, "");
            if(strstr(item, ": ") == NULL)
               sscanf(item, "%*[^:]:%[^\n]", trigger);
            else
               sscanf(item, "%*[^:]: %[^\n]", trigger);

            /* For whatever reason, some triggers contain a ":" separated field */

            if(strchr(trigger, ':') != NULL)
            {
                strcpy(tmp2, trigger);
                strcpy(trigger, "");
                if(strstr(tmp2, ": ") == NULL)
                   sscanf(tmp2, "%*[^:]:%[^\n]", trigger);
                else
                   sscanf(tmp2, "%*[^:]: %[^\n]", trigger);
            } /* fi */

            fullList[numFull++] = strdup(item);
            if(strstr(item, "AMBIG/METAPHOR") == NULL)
              regList[numReg++] = strdup(trigger);

            else if(islower(trigger[0]))  /* AMBIG/METAPHOR + lowercase initial character */
              ambList[numAmb++] = strdup(trigger);
        } /* fi found trigger */

        else if(strlen(item) > 0)
          fullList[numFull++] = strdup(item);

        strcpy(tmp, rest);
    } /* while */

    okAmbig = TRUE;
    for(i = 0; okAmbig && (i < numReg); i++)
    {
        found = FALSE;
        for(j = 0; !found && (j < numAmb); j++)
        {
            if(regList[i][0] == ambList[j][0])
            {
                if(strcmp(regList[i], ambList[j]) == 0)
                  found = TRUE;
            } /* fi */
        } /* for */

        if(!found)
          okAmbig = FALSE;
    } /* for */

    pos = 0;
    strcpy(tmp2, "");
    for(i = 0; i < numFull; i++)
    {
        if(strstr(fullList[i], "AMBIG/METAPHOR") == NULL)
        {
            if(pos > 0)
              strcat(tmp2, ";");
            strcat(tmp2, fullList[i]);
            pos++;
        } /* fi */

        else if(okAmbig)
        {
            if(pos > 0)
              strcat(tmp2, ";");
            strcat(tmp2, fullList[i]);
            pos++;
        } /* else fi */

        free(fullList[i]);
    } /* for */

    if(strlen(tmp2) > 0)
    {
        free(rtn);
        rtn = strdup(tmp2);
    } /* fi */

    for(i = 0; i < numReg; i++)
      free(regList[i]);

    for(i = 0; i < numAmb; i++)
      free(ambList[i]);

    return(rtn);
} /* verifyAmbig_Metaphor */

/***************************************************************************
*
*  countForced --
*
*      This 
*
***************************************************************************/

int countForced(char *trigger)
{
    int rtn;
    long i, j, len, lenCheck, diff;
    char *tmp, *foo;

    tmp = strdup(trigger);
    len = (long)strlen(tmp);
    rtn = 0;

    for(i = 0; i < numForceChecks; i++)
    {
        foo = strstr(tmp, ForceChecks[i]);
        while(foo != NULL)
        {
            rtn++;
            diff = foo - tmp;
            lenCheck = (long)strlen(ForceChecks[i]);
            for(j = 0; j < lenCheck; j++)
              tmp[diff + j] = 'x';

            foo = strstr(tmp, ForceChecks[i]);
        } /* while */
    } /* for */

    return(rtn);
} /* countForced */

/***************************************************************************
*
*  XoutPercentages --
*
*      This 
*
***************************************************************************/

char *XoutPercentages(char *text)
{
   char *base, *foo, *baseLookFor;
   int okLeft, okRight;
   long i, j, baseLen, lfLen, diff, rPos, pos;

   okLeft = okRight = FALSE;
   lfLen = (long)strlen("%");
   triggerLen = lfLen;
   base = strdup(text);
   baseLen = (long)strlen(text);
   baseLookFor = strdup("%");

   foo = strstr(base, baseLookFor);
   while(foo != NULL)
   {
       diff = foo - base;
       j = diff;
       while(isdigit(base[j]) || (base[j] == '%'))
         base[j--] = 'X';

       foo = strstr(base, baseLookFor);
  } /* while foo */

   free(baseLookFor);
   return(base);
} /* XoutPercentages */
