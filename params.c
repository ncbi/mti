#include "M_terms.h"
#include "myLocal.h"

extern int strictFilter_set, medFilter_set, medFilterR_set, starMHTI_set, SMALL,
       addCTs, addGEOs, remMHs, showHMs, showHMsD, writeTables, NORMAL,
       EOT_FLAG, showETs, showETsD, noCheckRC, limitTitleOnly, limitPTs,
       doTimingFlag, writeSpecial, doLogging, showTreecodes,
       doAgedReview, RTM_Debug, showTUIs, RSfilterTO, TTHost_specified,
       doSuppChemUpdate, testSpecials, doMHSHDups, addUSA, showAgedReview,
       showDUIs, RSfilterALL, showRSfilter, showInterimBreakout, noShowBL,
       showInterimIIBreakout, addAddls, cutOffScore, showJust, CATALOGING,
       removeRCOnly, rRCCutOffScore, lTO_II, try_Elink, checkNoTerms,
       doSAP, exitNoResult, PRC_Debug, HMIIPA_Debug, useWSD, useWSDTIOnly,
       RTM_logNF, MMIO_Filter, forceNewTerms, idCarboSeq, doAMLFilter,
       forceMHs, cutOff15, checkForFirstLine, trackPositional,
       doNoAddForced, forceMTIFL, noMTIFL, BioASQ, firstLineReviewJournal,
       showCalc, forceMTIFLReview, BioASQII, showMeSHVersion, showDisclaimer,
       showType, preIndex, MeSHonDemand, MeSHonDemand2, MoD_Raw, OLDMEDLINE,
       level1Filter, showRemovals, JSON, showScores, showScoresFull, showIlyaML,
       MTI_AUTO, doIL2R, goHigh, doIL2RHybrid, MoD_PP, ETsOFF, forceCHJ,
       forceOK_Title, ACCME, MTI_REVIEW, forceCON, haveFullText, doLookAtConf,
       look4PTs;

extern float PW1, PW2, PW4, DIRECT, ATX, GP75, GC75, GS75, OTHER, IM, NIM,
       EMP_HSTAR, mySEC;

extern int COT_FACTOR, TITLE_FACTOR, TREE_FACTOR, top_relcit, max, topn;

extern char mmi_index[128], trg_index[128], rel_index[128], display[32], 
            filename[SMALL_LINE + 1], output_file[SMALL_LINE + 1], 
            TexTool_Host[100], actualCommand[MAXLINE + 1];

extern int read_MMI, read_TRG, read_REL, LOCAL, REMOTE, showCommand;

void param_init(int argc, char *argv[]);
void read_param_from_file(char *paramFilename);
void print_usage();
void reset_vars();
void print_vars();

void param_init(int argc, char *argv[])
{
   int i;
   int first_time = TRUE;

   reset_vars();
   strcpy(actualCommand, "");

   for(i = 1; i < (argc - 1); i++)
   {
       if(i > 1)
         strcat(actualCommand, " ");
       strcat(actualCommand, argv[i]);
   } /* for */

   i = 1;
   while(i < argc)
   {
      if(strcmp(argv[i], "-f") == 0)
      {
         i++;
         strcpy(actualCommand, "");
         read_param_from_file(argv[i]);
      } /* fi */

      else if(strcmp(argv[i], "-E") == 0)
         EOT_FLAG = TRUE;

      else if(strcmp(argv[i], "-doTiming") == 0)
         doTimingFlag = TRUE;

      else if(strcmp(argv[i], "-LOCAL") == 0)
      {
         LOCAL = TRUE;
         REMOTE = FALSE;
      } /* else fi */

      else if(strcmp(argv[i], "-REMOTE") == 0)
      {
         REMOTE = TRUE;
         LOCAL = FALSE;
      } /* else fi */

      else if(strcmp(argv[i], "-showDisclaimer") == 0)
         showDisclaimer = TRUE;

      else if(strcmp(argv[i], "-forceOK_Title") == 0)
         forceOK_Title = TRUE;

      else if(strcmp(argv[i], "-showRemovals") == 0)
         showRemovals = TRUE;

      else if(strcmp(argv[i], "-showScores") == 0)
      {
         showScores = TRUE;
         showETs = FALSE;
      } /* else fi */

      else if(strcmp(argv[i], "-showScoresFull") == 0)
      {
         showScoresFull = TRUE;
         showETs = FALSE;
      } /* else fi */

      else if(strcmp(argv[i], "-showMeSHVersion") == 0)
         showMeSHVersion = TRUE;

      else if(strcmp(argv[i], "-preIndex") == 0)
      {
          preIndex = TRUE;
          showETs = FALSE;
      } /* else fi */

      else if(strcmp(argv[i], "-level1Filter") == 0)
      {
          level1Filter = TRUE;
          showETs = FALSE;
      } /* else fi */

      else if(strcmp(argv[i], "-MTI_AUTO") == 0)
      {
          MTI_REVIEW = FALSE;
          MTI_AUTO = TRUE;
          showETs = FALSE;
      } /* else fi */

      else if(strcmp(argv[i], "-MTI_REVIEW") == 0)
      {
          MTI_REVIEW = TRUE;
          MTI_AUTO = TRUE;
          showETs = FALSE;
      } /* else fi */

      else if(strcmp(argv[i], "-forceCON") == 0)
      {
          forceCON = TRUE;
          MTI_AUTO = TRUE;
          doIL2RHybrid = TRUE;
      } /* else fi */

      else if(strcmp(argv[i], "-look4PTs") == 0)
        look4PTs = TRUE;

      else if(strcmp(argv[i], "-haveFullText") == 0)
        haveFullText = TRUE;

      else if(strcmp(argv[i], "-ACCME") == 0)
      {
          MTI_AUTO = TRUE;
          ACCME = TRUE;
          showETs = FALSE;
      } /* else fi */

      else if(strcmp(argv[i], "-showCommand") == 0)
         showCommand = TRUE;

      else if(strcmp(argv[i], "-TexToolHost") == 0)
      {
         i++;
         TTHost_specified = TRUE;
         strcpy(TexTool_Host, argv[i]);
      } /* else fi */

      else if(strcmp(argv[i], "-mindex") == 0)
      {
         i++;
         read_MMI = TRUE;
         strcpy(mmi_index, argv[i]);
      } /* else fi */

      else if(strcmp(argv[i], "-tindex") == 0)
      {
         i++;
         read_TRG = TRUE;
         strcpy(trg_index, argv[i]);
      } /* else fi */

      else if(strcmp(argv[i], "-rindex") == 0)
      {
         i++;
         read_REL = TRUE;
         strcpy(rel_index, argv[i]);
      } /* else fi */

      else if(strcmp(argv[i], "-mmi") == 0)
      {
         i++;
         PW1 = atof(argv[i]);
      } /* else fi */

      else if(strcmp(argv[i], "-trg") == 0)
      {
         i++;
         PW2 = atof(argv[i]);
      } /* else fi */

      else if(strcmp(argv[i], "-pub") == 0)
      {
         i++;
         PW4 = atof(argv[i]);
      } /* else fi */

      else if(strcmp(argv[i], "-cot") == 0)
      {
         i++;
         COT_FACTOR = atoi(argv[i]);
      } /* else fi */

      else if(strcmp(argv[i], "-rel") == 0)
      {
         i++;
         TREE_FACTOR = atoi(argv[i]);
      } /* else fi */

      else if(strcmp(argv[i], "-ti") == 0)
      {
         i++;
         TITLE_FACTOR = atoi(argv[i]);
      } /* else fi */

      else if(strcmp(argv[i], "-topn") == 0)
      {
         i++;
         topn = atoi(argv[i]);
         max = topn;
      } /* else fi */

      else if(strcmp(argv[i], "-cutOffScore") == 0)
      {
         i++;
         cutOffScore = atoi(argv[i]);
      } /* else fi */

      else if(strcmp(argv[i], "-rRCCutOffScore") == 0)
      {
         i++;
         rRCCutOffScore = atoi(argv[i]);
      } /* else fi */

      else if(strcmp(argv[i], "-dir") == 0)
      {
         i++;
         DIRECT = atof(argv[i]);
      } /* else fi */

      else if(strcmp(argv[i], "-atx") == 0)
      {
         i++;
         ATX = atof(argv[i]);
      } /* else fi */

      else if(strcmp(argv[i], "-par") == 0)
      {
         i++;
         GP75 = atof(argv[i]);
      } /* else fi */

      else if(strcmp(argv[i], "-chd") == 0)
      {
         i++;
         GC75 = atof(argv[i]);
      } /* else fi */

      else if(strcmp(argv[i], "-sib") == 0)
      {
         i++;
         GS75 = atof(argv[i]);
      } /* else fi */

      else if(strcmp(argv[i], "-oth") == 0)
      {
         i++;
         OTHER = atof(argv[i]);
      } /* else fi */

      else if(strcmp(argv[i], "-im") == 0)
      {
         i++;
         IM = atof(argv[i]);
      } /* else fi */

      else if(strcmp(argv[i], "-nim") == 0)
      {
         i++;
         NIM = atof(argv[i]);
      } /* else fi */

      else if(strcmp(argv[i], "-emp_hstar") == 0)
      {
         i++;
         EMP_HSTAR = atof(argv[i]);
      } /* else fi */

      else if(strcmp(argv[i], "-sec") == 0)
      {
         i++;
         mySEC = atof(argv[i]);
      } /* else fi */

      else if(strcmp(argv[i], "-cit") == 0)
      {
         i++;
         top_relcit = atoi(argv[i]);
      } /* else fi */

      else if(strcmp(argv[i], "-JSON") == 0)
        JSON = TRUE;

      else if(strcmp(argv[i], "-xml") == 0)
         strcpy(display, "xml");

      else if(strcmp(argv[i], "-justFacts") == 0)
         strcpy(display, "justFacts");

      else if(strcmp(argv[i], "-detail") == 0)
         strcpy(display, "detail");

      else if(strcmp(argv[i], "-detail2") == 0)
         strcpy(display, "detail2");

      else if(strcmp(argv[i], "-debug1") == 0)
         strcpy(display, "debug1");

      else if(strcmp(argv[i], "-display_simple") == 0)
         strcpy(display, "display_simple");

      else if(strcmp(argv[i], "-display_simple2") == 0)
         strcpy(display, "display_simple2");

      else if(strcmp(argv[i], "-display_simple3") == 0)
         strcpy(display, "display_simple3");

      else if(strcmp(argv[i], "-display_full") == 0)
         strcpy(display, "display_full_listing");

      else if(strcmp(argv[i], "-display_fullR") == 0)
         strcpy(display, "display_OK_full_listing");

      else if(strcmp(argv[i], "-detail_list") == 0)
         strcpy(display, "detail_list");

      else if(strcmp(argv[i], "-detail_listII") == 0)
         strcpy(display, "detail_listII");

      else if(strcmp(argv[i], "-SMALL") == 0)
         SMALL = TRUE;

      else if(strcmp(argv[i], "-strictFilter") == 0)
         strictFilter_set = TRUE;

      else if(strcmp(argv[i], "-medFilter") == 0)
         medFilter_set = TRUE;

      else if(strcmp(argv[i], "-medFilterR") == 0)
      {
         medFilter_set = TRUE;
         medFilterR_set = TRUE;
      } /* else fi */

      else if(strcmp(argv[i], "-starMHTI") == 0)
         starMHTI_set = TRUE;

      else if(strcmp(argv[i], "-RTM_Debug") == 0)
         RTM_Debug = TRUE;

      else if(strcmp(argv[i], "-PRC_Debug") == 0)
         PRC_Debug = TRUE;

      else if(strcmp(argv[i], "-HMIIPA_Debug") == 0)
         HMIIPA_Debug = TRUE;

      else if(strcmp(argv[i], "-RSfilterTO") == 0)
         RSfilterTO = TRUE;

      else if(strcmp(argv[i], "-RSfilterALL") == 0)
         RSfilterALL = TRUE;

      else if(strcmp(argv[i], "-showRSfilter") == 0)
         showRSfilter = TRUE;

      else if(strcmp(argv[i], "-addCTs") == 0)
         addCTs = TRUE;

      else if(strcmp(argv[i], "-addGEOs") == 0)
         addGEOs = TRUE;

      else if(strcmp(argv[i], "-addUSA") == 0)
         addUSA = TRUE;

      else if(strcmp(argv[i], "-forceNewTerms") == 0)
         forceNewTerms = TRUE;

      else if(strcmp(argv[i], "-doSAP") == 0)
         doSAP = TRUE;

      else if(strcmp(argv[i], "-addAddls") == 0)
         addAddls = TRUE;

      else if(strcmp(argv[i], "-removeRCOnly") == 0)
         removeRCOnly = TRUE;

      else if(strcmp(argv[i], "-doAgedReview") == 0)
         doAgedReview = TRUE;

      else if(strcmp(argv[i], "-showAgedReview") == 0)
         showAgedReview = TRUE;

      else if(strcmp(argv[i], "-showDUIs") == 0)
         showDUIs = TRUE;

      else if(strcmp(argv[i], "-remMHs") == 0)
         remMHs = TRUE;

      else if(strcmp(argv[i], "-showTUIs") == 0)
         showTUIs = TRUE;

      else if(strcmp(argv[i], "-showHMs") == 0)
         showHMs = TRUE;

      else if(strcmp(argv[i], "-showHMsD") == 0)
         showHMsD = TRUE;

      else if(strcmp(argv[i], "-showETs") == 0)
         showETs = TRUE;

      else if(strcmp(argv[i], "-ETsOFF") == 0)
         showETs = FALSE;

      else if(strcmp(argv[i], "-showCalc") == 0)
         showCalc = TRUE;

      else if(strcmp(argv[i], "-showETsD") == 0)
      {
         showETs = TRUE;
         showETsD = TRUE;
      } /* else fi */

      else if(strcmp(argv[i], "-showJust") == 0)
      {
          showJust = TRUE;
          trackPositional = TRUE;
      } /* else fi */

      else if(strcmp(argv[i], "-trackPositional") == 0)
         trackPositional = TRUE;

      else if(strcmp(argv[i], "-exitNoResult") == 0)
         exitNoResult = TRUE;

      else if(strcmp(argv[i], "-useWSD") == 0)
         useWSD = TRUE;

      else if(strcmp(argv[i], "-useWSDTIOnly") == 0)
         useWSDTIOnly = TRUE;

      else if(strcmp(argv[i], "-RTM_logNF") == 0)
         RTM_logNF = TRUE;

      else if(strcmp(argv[i], "-noCheckRC") == 0)
         noCheckRC = TRUE;

      else if(strcmp(argv[i], "-limitTitleOnly") == 0)
         limitTitleOnly = TRUE;

      else if(strcmp(argv[i], "-cutOff15") == 0)
         cutOff15 = TRUE;

      else if(strcmp(argv[i], "-MMIO_Filter") == 0)
         MMIO_Filter = TRUE;

      else if(strcmp(argv[i], "-limitPTs") == 0)
         limitPTs = TRUE;

      else if(strcmp(argv[i], "-doLogging") == 0)
         doLogging = TRUE;

      else if(strcmp(argv[i], "-doSuppChemUpdate") == 0)
         doSuppChemUpdate = TRUE;

      else if(strcmp(argv[i], "-testSpecials") == 0)
         testSpecials = TRUE;

      else if(strcmp(argv[i], "-showInterimBreakout") == 0)
         showInterimBreakout = TRUE;

      else if(strcmp(argv[i], "-showInterimIIBreakout") == 0)
         showInterimIIBreakout = TRUE;

      else if(strcmp(argv[i], "-noShowBL") == 0)
         noShowBL = TRUE;

      else if(strcmp(argv[i], "-ignoreMHSHDups") == 0)
         doMHSHDups = FALSE;

      else if(strcmp(argv[i], "-writeTables") == 0)
         writeTables = TRUE;

      else if(strcmp(argv[i], "-writeSpecial") == 0)
         writeSpecial = TRUE;

      else if(strcmp(argv[i], "-showTreecodes") == 0)
         showTreecodes = TRUE;

      else if(strcmp(argv[i], "-idCarboSeq") == 0)
         idCarboSeq = TRUE;

      else if(strcmp(argv[i], "-forceMHs") == 0)
         forceMHs = TRUE;

      else if(strcmp(argv[i], "-doNoAddForced") == 0)
         doNoAddForced = TRUE;

      else if(strcmp(argv[i], "-forceMTIFL") == 0)
         forceMTIFL = TRUE;

      else if(strcmp(argv[i], "-forceCHJ") == 0)
         forceCHJ = TRUE;

      else if(strcmp(argv[i], "-forceMTIFLReview") == 0)
         forceMTIFLReview = TRUE;

      else if(strcmp(argv[i], "-noMTIFL") == 0)
         noMTIFL = TRUE;

      else if(strcmp(argv[i], "-doAMLFilter") == 0) /* Antonio ML Filtering */
         doAMLFilter = TRUE;

      else if(strcmp(argv[i], "-doIL2R") == 0) /* Ilya's ML L2R */
         doIL2R = TRUE;

      else if(strcmp(argv[i], "-doIL2RHybrid") == 0)
         doIL2RHybrid = TRUE;

      else if(strcmp(argv[i], "-goHigh") == 0)
         goHigh = TRUE;

      else if(strcmp(argv[i], "-Special_Output") == 0)
      {
         writeSpecial = TRUE;
         SMALL = TRUE;
      } /* else fi */

      else if(strcmp(argv[i], "-default") == 0)
      {
         reset_vars();
         addCTs = TRUE;
         addGEOs = TRUE;
         addUSA = TRUE;
         forceNewTerms = TRUE;
         remMHs = TRUE;
         noCheckRC = TRUE;
         limitTitleOnly = TRUE;
         doSuppChemUpdate = TRUE;
         doAgedReview = TRUE;
         strcpy(display, "detail");
      } /* else fi */

      else if(strcmp(argv[i], "-default_MTI") == 0)  /* No eLink try */
      {
         reset_vars();
         addCTs = TRUE;
         addGEOs = TRUE;
         addUSA = TRUE;
         forceNewTerms = TRUE;
         remMHs = TRUE;
         showHMs = FALSE;
         showETs = FALSE;
         noCheckRC = TRUE;
         lTO_II = TRUE;
         limitTitleOnly = TRUE;
         limitPTs = TRUE;
         starMHTI_set = FALSE;
         RSfilterTO = TRUE;
         RSfilterALL = TRUE;
         showTUIs = FALSE;
         exitNoResult = TRUE;
         useWSD = FALSE;
         useWSDTIOnly = FALSE;
         doSuppChemUpdate = TRUE;
         doAgedReview = TRUE;
         strcpy(display, "justFacts");
         NORMAL = TRUE;
         CATALOGING = FALSE;
         MMIO_Filter = TRUE;
         idCarboSeq = FALSE;
         forceMHs = TRUE;
         checkForFirstLine = FALSE;
         trackPositional = FALSE;
         doAMLFilter = TRUE;
      } /* else fi */

      else if(strcmp(argv[i], "-defaultL_MTI") == 0)  /* Try eLink first */
      {
         reset_vars();
         addCTs = TRUE;
         addGEOs = TRUE;
         addUSA = TRUE;
         forceNewTerms = TRUE;
         remMHs = TRUE;
         showHMs = FALSE;
         showETs = FALSE;
         noCheckRC = TRUE;
         lTO_II = TRUE;
         limitTitleOnly = TRUE;
         limitPTs = TRUE;
         starMHTI_set = FALSE;
         RSfilterTO = TRUE;
         RSfilterALL = TRUE;
         try_Elink = TRUE;
         showTUIs = FALSE;
         exitNoResult = TRUE;
         useWSD = FALSE;
         useWSDTIOnly = FALSE;
         doSuppChemUpdate = TRUE;
         doAgedReview = TRUE;
         strcpy(display, "justFacts");
         NORMAL = TRUE;
         CATALOGING = FALSE;
         MMIO_Filter = TRUE;
         idCarboSeq = FALSE;
         forceMHs = TRUE;
         checkForFirstLine = FALSE;
         trackPositional = FALSE;
         doAMLFilter = TRUE;
         forceOK_Title = TRUE;
      } /* else fi */

      else if(strcmp(argv[i], "-default_DCMS") == 0)  /* No eLink try */
      {
         reset_vars();
         addCTs = TRUE;
         addGEOs = TRUE;
         addUSA = TRUE;
         forceNewTerms = TRUE;
         remMHs = TRUE;
         showHMs = TRUE;
         showETs = TRUE;
         noCheckRC = TRUE;
         lTO_II = TRUE;
         limitTitleOnly = TRUE;
         limitPTs = TRUE;
         starMHTI_set = TRUE;
         RSfilterTO = TRUE;
         RSfilterALL = TRUE;
         showTUIs = TRUE;
         exitNoResult = TRUE;
         useWSD = FALSE;
         useWSDTIOnly = FALSE;
         doSuppChemUpdate = TRUE;
         doAgedReview = TRUE;
         strcpy(display, "detail_list");
         NORMAL = TRUE;
         CATALOGING = FALSE;
         MMIO_Filter = TRUE;
         idCarboSeq = TRUE;
         forceMHs = TRUE;
         checkForFirstLine = TRUE;
         trackPositional = TRUE;
         forceOK_Title = TRUE;
      } /* else fi */

      else if(strcmp(argv[i], "-defaultL_DCMS") == 0)  /* Try eLink first */
      {
         reset_vars();
         addCTs = TRUE;
         addGEOs = TRUE;
         addUSA = TRUE;
         forceNewTerms = TRUE;
         remMHs = TRUE;
         showHMs = TRUE;
         showETs = TRUE;
         noCheckRC = TRUE;
         lTO_II = TRUE;
         limitTitleOnly = TRUE;
         limitPTs = TRUE;
         starMHTI_set = TRUE;
         RSfilterTO = TRUE;
         RSfilterALL = TRUE;
         try_Elink = TRUE;
         showTUIs = TRUE;
         doSuppChemUpdate = TRUE;
         doAgedReview = TRUE;
         exitNoResult = TRUE;
         useWSD = FALSE;
         useWSDTIOnly = FALSE;
         strcpy(display, "detail_list");
         NORMAL = TRUE;
         CATALOGING = FALSE;
         MMIO_Filter = TRUE;
         idCarboSeq = TRUE;
         forceMHs = TRUE;
         checkForFirstLine = TRUE;
         trackPositional = TRUE;
         forceOK_Title = TRUE;
      } /* else fi */

      else if(strcmp(argv[i], "-opt1_DCMS") == 0)  /* No eLink try */
      {
         reset_vars();
         addCTs = TRUE;
         addGEOs = TRUE;
         addUSA = TRUE;
         forceNewTerms = TRUE;
         remMHs = TRUE;
         showHMs = TRUE;
         showETs = TRUE;
         noCheckRC = TRUE;
         lTO_II = TRUE;
         limitTitleOnly = TRUE;
         limitPTs = TRUE;
         starMHTI_set = TRUE;
         RSfilterTO = TRUE;
         RSfilterALL = TRUE;
         exitNoResult = TRUE;
         useWSD = FALSE;
         useWSDTIOnly = FALSE;
         doSuppChemUpdate = TRUE;
         doAgedReview = TRUE;
         strcpy(display, "detail");
         NORMAL = TRUE;
         CATALOGING = FALSE;
         MMIO_Filter = TRUE;
         idCarboSeq = TRUE;
         forceMHs = TRUE;
         checkForFirstLine = TRUE;
         trackPositional = TRUE;
         forceOK_Title = TRUE;
      } /* else fi */

      else if(strcmp(argv[i], "-opt1L_DCMS") == 0)  /* Try eLink first */
      {
         reset_vars();
         addCTs = TRUE;
         addGEOs = TRUE;
         addUSA = TRUE;
         forceNewTerms = TRUE;
         remMHs = TRUE;
         showHMs = TRUE;
         showETs = TRUE;
         noCheckRC = TRUE;
         lTO_II = TRUE;
         limitTitleOnly = TRUE;
         limitPTs = TRUE;
         starMHTI_set = TRUE;
         RSfilterTO = TRUE;
         RSfilterALL = TRUE;
         try_Elink = TRUE;
         doSuppChemUpdate = TRUE;
         doAgedReview = TRUE;
         exitNoResult = TRUE;
         useWSD = FALSE;
         useWSDTIOnly = FALSE;
         strcpy(display, "detail");
         NORMAL = TRUE;
         CATALOGING = FALSE;
         MMIO_Filter = TRUE;
         idCarboSeq = TRUE;
         forceMHs = TRUE;
         checkForFirstLine = TRUE;
         trackPositional = TRUE;
         forceOK_Title = TRUE;
      } /* else fi */

      else if(strcmp(argv[i], "-opt1_OLDMEDLINE") == 0)  /* No eLink try */
      {
         reset_vars();
         addCTs = TRUE;
         addGEOs = TRUE;
         addUSA = TRUE;
         forceNewTerms = TRUE;
         remMHs = TRUE;
         showHMs = TRUE;
         showETs = FALSE;
         noCheckRC = TRUE;
         lTO_II = TRUE;
         limitTitleOnly = TRUE;
         limitPTs = TRUE;
         starMHTI_set = TRUE;
         RSfilterTO = TRUE;
         RSfilterALL = TRUE;
         exitNoResult = TRUE;
         useWSD = FALSE;
         useWSDTIOnly = FALSE;
         doSuppChemUpdate = TRUE;
         doAgedReview = TRUE;
         strcpy(display, "detail");
         NORMAL = TRUE;
         CATALOGING = FALSE;
         MMIO_Filter = TRUE;
         idCarboSeq = TRUE;
         forceMHs = TRUE;
         checkForFirstLine = TRUE;
      } /* else fi */

      else if(strcmp(argv[i], "-BioASQ") == 0)  /* Try eLink first */
      {
         reset_vars();
         BioASQ = TRUE;
         addCTs = TRUE;
         addGEOs = TRUE;
         addUSA = TRUE;
         forceNewTerms = FALSE;
         remMHs = TRUE;
         showHMs = TRUE;
         showETs = FALSE;
         noCheckRC = TRUE;
         lTO_II = TRUE;
         limitTitleOnly = TRUE;
         limitPTs = TRUE;
         starMHTI_set = FALSE;
         RSfilterTO = TRUE;
         RSfilterALL = TRUE;
         try_Elink = TRUE;
         doSuppChemUpdate = TRUE;
         doAgedReview = TRUE;
         exitNoResult = TRUE;
         useWSD = FALSE;
         useWSDTIOnly = FALSE;
         strcpy(display, "BioASQ");
         NORMAL = TRUE;
         CATALOGING = FALSE;
         MMIO_Filter = TRUE;
         idCarboSeq = TRUE;
         forceMHs = TRUE;
         checkForFirstLine = TRUE;
         forceMTIFL = TRUE;
         doAMLFilter = TRUE;
         trackPositional = TRUE;
         doIL2RHybrid = TRUE;
      } /* else fi */

      else if(strcmp(argv[i], "-BioASQII") == 0)  /* Try eLink first */
      {
         reset_vars();
         BioASQ = TRUE;
         addCTs = TRUE;
         addGEOs = TRUE;
         addUSA = TRUE;
         forceNewTerms = TRUE;
         remMHs = TRUE;
         showHMs = TRUE;
         showETs = FALSE;
         noCheckRC = TRUE;
         lTO_II = TRUE;
         limitTitleOnly = TRUE;
         limitPTs = TRUE;
         starMHTI_set = FALSE;
         RSfilterTO = TRUE;
         RSfilterALL = TRUE;
         try_Elink = TRUE;
         doSuppChemUpdate = TRUE;
         doAgedReview = TRUE;
         exitNoResult = TRUE;
         useWSD = FALSE;
         useWSDTIOnly = FALSE;
         strcpy(display, "BioASQ");
         NORMAL = TRUE;
         CATALOGING = FALSE;
         MMIO_Filter = TRUE;
         idCarboSeq = TRUE;
         forceMHs = TRUE;
         checkForFirstLine = FALSE;
         forceMTIFL = FALSE;
         doAMLFilter = TRUE;
         trackPositional = TRUE;
         doIL2RHybrid = TRUE;
      } /* else fi */

      else if(strcmp(argv[i], "-MeSHonDemand") == 0)  /* No eLink try */
      {
         reset_vars();
         addCTs = TRUE;
         addGEOs = TRUE;
         addUSA = TRUE;
         forceNewTerms = FALSE;
         remMHs = TRUE;
         showHMs = TRUE;
         showETs = FALSE;
         noCheckRC = TRUE;
         lTO_II = TRUE;
         limitTitleOnly = TRUE;
         limitPTs = TRUE;
         starMHTI_set = FALSE;
         RSfilterTO = TRUE;
         RSfilterALL = TRUE;
         exitNoResult = TRUE;
         useWSD = FALSE;
         useWSDTIOnly = FALSE;
         doSuppChemUpdate = TRUE;
         doAgedReview = TRUE;
         strcpy(display, "display_simple");
         NORMAL = TRUE;
         CATALOGING = FALSE;
         MMIO_Filter = TRUE;
         idCarboSeq = TRUE;
         forceMHs = TRUE;
         checkForFirstLine = FALSE;
         forceMTIFL = TRUE;
         doAMLFilter = FALSE;
         showMeSHVersion = TRUE;
         showDisclaimer = TRUE;
         showType = TRUE;
         trackPositional = TRUE;
         MeSHonDemand = TRUE;
         MeSHonDemand2 = FALSE;
         MoD_Raw = FALSE;
         MoD_PP = FALSE;
      } /* else fi */

      else if(strcmp(argv[i], "-MoD_Raw") == 0)  /* No eLink try */
      {
         reset_vars();
         addCTs = TRUE;
         addGEOs = TRUE;
         addUSA = TRUE;
         forceNewTerms = FALSE;
         remMHs = TRUE;
         showHMs = TRUE;
         showETs = FALSE;
         noCheckRC = TRUE;
         lTO_II = TRUE;
         limitTitleOnly = TRUE;
         limitPTs = TRUE;
         starMHTI_set = FALSE;
         RSfilterTO = TRUE;
         RSfilterALL = TRUE;
         exitNoResult = TRUE;
         useWSD = FALSE;
         useWSDTIOnly = FALSE;
         doSuppChemUpdate = TRUE;
         doAgedReview = TRUE;
         strcpy(display, "display_simple");
         NORMAL = TRUE;
         CATALOGING = FALSE;
         MMIO_Filter = TRUE;
         idCarboSeq = TRUE;
         forceMHs = TRUE;
         checkForFirstLine = FALSE;
         forceMTIFL = TRUE;
         doAMLFilter = FALSE;
         showMeSHVersion = FALSE;
         showDisclaimer = FALSE;
         showType = TRUE;
         trackPositional = TRUE;
         MeSHonDemand = TRUE;
         MeSHonDemand2 = FALSE;
         MoD_Raw = TRUE;
         MoD_PP = FALSE;
      } /* else fi */

      else if(strcmp(argv[i], "-MeSHonDemand2") == 0)  /* No eLink try */
      {
         reset_vars();
         addCTs = TRUE;
         addGEOs = TRUE;
         addUSA = TRUE;
         forceNewTerms = FALSE;
         remMHs = TRUE;
         showHMs = TRUE;
         showETs = FALSE;
         noCheckRC = TRUE;
         lTO_II = TRUE;
         limitTitleOnly = TRUE;
         limitPTs = TRUE;
         starMHTI_set = FALSE;
         RSfilterTO = TRUE;
         RSfilterALL = TRUE;
         exitNoResult = TRUE;
         useWSD = FALSE;
         useWSDTIOnly = FALSE;
         doSuppChemUpdate = TRUE;
         doAgedReview = TRUE;
         strcpy(display, "display_simple");
         NORMAL = TRUE;
         CATALOGING = FALSE;
         MMIO_Filter = TRUE;
         idCarboSeq = TRUE;
         forceMHs = TRUE;
         checkForFirstLine = FALSE;
         forceMTIFL = TRUE;
         doAMLFilter = FALSE;
         showMeSHVersion = FALSE;
         showDisclaimer = FALSE;
         showType = TRUE;
         trackPositional = TRUE;
         MeSHonDemand = TRUE;
         MeSHonDemand2 = TRUE;
         MoD_Raw = FALSE;
         MoD_PP = FALSE;
      } /* else fi */

      else if(strcmp(argv[i], "-MoD_PP") == 0)  /* Pretty Print */
      {
         reset_vars();
         addCTs = TRUE;
         addGEOs = TRUE;
         addUSA = TRUE;
         forceNewTerms = FALSE;
         remMHs = TRUE;
         showHMs = TRUE;
         showETs = FALSE;
         noCheckRC = TRUE;
         lTO_II = TRUE;
         limitTitleOnly = TRUE;
         limitPTs = TRUE;
         starMHTI_set = FALSE;
         RSfilterTO = TRUE;
         RSfilterALL = TRUE;
         exitNoResult = TRUE;
         useWSD = FALSE;
         useWSDTIOnly = FALSE;
         doSuppChemUpdate = TRUE;
         doAgedReview = TRUE;
         strcpy(display, "justFacts");
         NORMAL = TRUE;
         CATALOGING = FALSE;
         MMIO_Filter = TRUE;
         idCarboSeq = TRUE;
         forceMHs = TRUE;
         checkForFirstLine = FALSE;
         forceMTIFL = TRUE;
         doAMLFilter = FALSE;
         showMeSHVersion = FALSE;
         showDisclaimer = FALSE;
         showType = TRUE;
         trackPositional = FALSE;
         MeSHonDemand = TRUE;
         MeSHonDemand2 = FALSE;
         MoD_Raw = FALSE;
         MoD_PP = TRUE;
         showDUIs = TRUE;
      } /* else fi */

      else if(strcmp(argv[i], "-OLDMEDLINE") == 0)  /* No eLink try */
      {
         reset_vars();
         addCTs = FALSE;
         addGEOs = TRUE;
         addUSA = TRUE;
         forceNewTerms = FALSE;
         remMHs = TRUE;
         showHMs = TRUE;
         showETs = FALSE;
         noCheckRC = TRUE;
         lTO_II = TRUE;
         limitTitleOnly = TRUE;
         limitPTs = TRUE;
         starMHTI_set = FALSE;
         RSfilterTO = TRUE;
         RSfilterALL = TRUE;
         exitNoResult = TRUE;
         useWSD = FALSE;
         useWSDTIOnly = FALSE;
         doSuppChemUpdate = TRUE;
         doAgedReview = TRUE;
         strcpy(display, "detail");
         NORMAL = TRUE;
         CATALOGING = FALSE;
         MMIO_Filter = TRUE;
         idCarboSeq = TRUE;
         forceMHs = TRUE;
         checkForFirstLine = FALSE;
         forceMTIFL = TRUE;
         doAMLFilter = FALSE;
         showMeSHVersion = FALSE;
         showDisclaimer = FALSE;
         showType = FALSE;
         trackPositional = FALSE;
         OLDMEDLINE = TRUE;
         doMHSHDups = FALSE;  /* Turn-off MH to SH mapping */
      } /* else fi */

      else if(strcmp(argv[i], "-opt1L_OLDMEDLINE") == 0)  /* eLink try */
      {
         reset_vars();
         addGEOs = TRUE;
         addUSA = TRUE;
         showETs = FALSE;
         forceNewTerms = TRUE;
         remMHs = TRUE;
         showHMs = TRUE;
         noCheckRC = TRUE;
         lTO_II = TRUE;
         limitTitleOnly = TRUE;
         limitPTs = TRUE;
         RSfilterTO = TRUE;
         RSfilterALL = TRUE;
         exitNoResult = TRUE;
         doSuppChemUpdate = TRUE;
         doAgedReview = TRUE;
         strcpy(display, "detail");
         NORMAL = TRUE;
         MMIO_Filter = TRUE;
         idCarboSeq = TRUE;
         forceMHs = TRUE;
         forceMTIFL = TRUE;
         OLDMEDLINE = TRUE;
         try_Elink = TRUE;
         doMHSHDups = FALSE;  /* Turn-off MH to SH mapping */
      } /* else fi */

      else if(strcmp(argv[i], "-optOLDL_DCMS") == 0)  /* Try eLink first */
      {
         reset_vars();
         addCTs = TRUE;
         addGEOs = TRUE;
         addUSA = TRUE;
         forceNewTerms = TRUE;
         remMHs = TRUE;
         showHMs = TRUE;
         showETs = FALSE;
         noCheckRC = TRUE;
         lTO_II = TRUE;
         limitTitleOnly = TRUE;
         limitPTs = TRUE;
         starMHTI_set = FALSE;
         RSfilterTO = TRUE;
         RSfilterALL = TRUE;
         try_Elink = TRUE;
         doSuppChemUpdate = TRUE;
         doAgedReview = TRUE;
         exitNoResult = TRUE;
         useWSD = FALSE;
         useWSDTIOnly = FALSE;
         strcpy(display, "detail");
         NORMAL = TRUE;
         CATALOGING = FALSE;
         MMIO_Filter = TRUE;
         idCarboSeq = TRUE;
         forceMHs = TRUE;
         checkForFirstLine = FALSE;
         trackPositional = TRUE;
         forceMTIFL = TRUE;
         OLDMEDLINE = TRUE;
      } /* else fi */

      else if(strcmp(argv[i], "-opt1L_ILYA") == 0)  /* Try eLink first */
      {
         reset_vars();
         addCTs = TRUE;
         addGEOs = TRUE;
         addUSA = TRUE;
         forceNewTerms = TRUE;
         remMHs = TRUE;
         showHMs = TRUE;
         showETs = FALSE;
         noCheckRC = TRUE;
         lTO_II = TRUE;
         limitTitleOnly = TRUE;
         limitPTs = TRUE;
         starMHTI_set = FALSE;
         RSfilterTO = TRUE;
         RSfilterALL = TRUE;
         try_Elink = TRUE;
         doSuppChemUpdate = TRUE;
         doAgedReview = TRUE;
         exitNoResult = TRUE;
         useWSD = FALSE;
         useWSDTIOnly = FALSE;
         strcpy(display, "detail");
         NORMAL = TRUE;
         CATALOGING = FALSE;
         MMIO_Filter = TRUE;
         idCarboSeq = TRUE;
         forceMHs = TRUE;
         checkForFirstLine = TRUE;
         trackPositional = TRUE;
         showDUIs = TRUE;
         showIlyaML = TRUE;
         doAMLFilter = TRUE;
      } /* else fi */

      else if(strcmp(argv[i], "-default_Strict") == 0)
      {
         reset_vars();
         addCTs = FALSE;
         addGEOs = FALSE;
         addUSA = FALSE;
         remMHs = TRUE;
         showHMs = FALSE;
         showETs = FALSE;
         noCheckRC = FALSE;
         RSfilterTO = FALSE;
         showTUIs = FALSE;
         doSuppChemUpdate = TRUE;
         doAgedReview = FALSE;
         strictFilter_set = TRUE;
         strcpy(display, "detail_list");
      } /* else fi */

      else if(strcmp(argv[i], "-opt1_Strict") == 0)
      {
         reset_vars();
         addCTs = FALSE;
         addGEOs = FALSE;
         addUSA = FALSE;
         remMHs = TRUE;
         showHMs = FALSE;
         showETs = FALSE;
         noCheckRC = FALSE;
         RSfilterTO = FALSE;
         showTUIs = FALSE;
         doSuppChemUpdate = TRUE;
         doAgedReview = FALSE;
         strictFilter_set = TRUE;
         strcpy(display, "detail");
      } /* else fi */

      else if(strcmp(argv[i], "-default_BackOfBook") == 0)
      {
         reset_vars();
         addCTs = TRUE;
         addGEOs = TRUE;
         addUSA = TRUE;
         showETs = TRUE;
         limitTitleOnly = TRUE;
         limitPTs = TRUE;
         RSfilterTO = TRUE;
         RSfilterALL = TRUE;
         doSuppChemUpdate = TRUE;
         doAgedReview = TRUE;
         addAddls = TRUE;
         removeRCOnly = TRUE;
         rRCCutOffScore = 100000;
         doMHSHDups = FALSE;
         noShowBL = TRUE;
         strcpy(display, "detail");
         useWSD = FALSE;
         MMIO_Filter = TRUE;
      } /* else fi */

      else if((strcmp(argv[i], "-opt1_CATALOG") == 0) ||
              (strcmp(argv[i], "-opt1_Catalog") == 0))
      {
         reset_vars();
         doMHSHDups = FALSE;
         topn = 10;
         addGEOs = TRUE;
         addUSA = TRUE;
         addCTs = TRUE;
         starMHTI_set = FALSE;
         doSuppChemUpdate = TRUE;
         medFilter_set = FALSE;
         strictFilter_set = FALSE;
         strcpy(display, "detail");
         useWSD = FALSE;
         NORMAL = FALSE;
         CATALOGING = TRUE;
         MMIO_Filter = FALSE;
         idCarboSeq = TRUE;
         forceMHs = TRUE;
         forceNewTerms = TRUE;
         try_Elink = FALSE;
         trackPositional = TRUE;
      } /* else fi */

      else if((strcmp(argv[i], "-opt1L_CATALOG") == 0) ||
              (strcmp(argv[i], "-opt1L_Catalog") == 0))
      {
         reset_vars();
         doMHSHDups = FALSE;
         topn = 10;
         addGEOs = TRUE;
         addUSA = TRUE;
         addCTs = TRUE;
         starMHTI_set = FALSE;
         doSuppChemUpdate = TRUE;
         medFilter_set = FALSE;
         strictFilter_set = FALSE;
         strcpy(display, "detail");
         useWSD = FALSE;
         try_Elink = TRUE;
         NORMAL = FALSE;
         CATALOGING = TRUE;
         MMIO_Filter = FALSE;
         idCarboSeq = TRUE;
         forceMHs = TRUE;
         forceNewTerms = TRUE;
         trackPositional = TRUE;
      } /* else fi */

      else if((strcmp(argv[i], "-help") == 0) || (strcmp(argv[i], "-h") == 0))
      {
         print_usage();
         exit(0);
      } /* else fi */

      else if(argv[i][0] != '-')
      {
          if(first_time)
          {
             strcpy(filename, argv[i]);
             first_time = FALSE;
          } /* fi */
          else
             strcpy(output_file, argv[i]);
      } /* else fi */

      else
      {
         fprintf(stderr,
            "Error at argument %d just after \"%s\" (Unknown argument)\n",
            i, argv[i - 1]);
         fflush(stderr);
         exit(2);
      } /* else */

      i++;
   } /* while */
} /* param_init */


void read_param_from_file(char *paramFilename)
{
   FILE *fp;
   char *p, *q;
   char strbuf[SMALL_LINE + 1];
   char token[3];
   int len, i, tmpI;
   float tmpF;
   char tmp[SMALL_LINE + 1], tmp2[SMALL_LINE + 1];

   reset_vars();

   fp = fopen(paramFilename, "r");
   if(fp == NULL)
   {
      fprintf(stderr, "ERROR: Unable to open parameter file: %s\n",
              paramFilename);
      fflush(stderr);
      exit(2);
   } /* fi */

   while (fgets(strbuf, SMALL_LINE, fp) != NULL)
   {
     len = (int)strlen(strbuf);
     strbuf[len - 1] = '\0';

     if(strbuf[len - 2] == ' ')
     {
        i = len - 2;
        while((i >= 0) && (strbuf[i] == ' '))
          strbuf[i--] = '\0';
     } /* fi */

     /* Handle special cases */


     if((strncmp(strbuf, "Output|", 7) == 0) ||
        (strncmp(strbuf, "Filtering|", 10) == 0))
     {
         strcpy(token, "|");
         p = strtok(strbuf, token);
         q = strtok (NULL, token);

         if(q != NULL)
           strcpy(strbuf, q);
     } /* fi */

     else
     {
         strcpy(tmp, "");
         strcpy(tmp2, "");
         sscanf(strbuf, "%[^|]|%[^\n]", tmp, tmp2);
         if(strcmp(tmp, tmp2) == 0)
           strcpy(strbuf, tmp);
     } /* else */

     strcpy(token, "|");
     if(strchr(strbuf, '|') != NULL)
     {
         p = strtok(strbuf, token);
         q = strtok (NULL, token);

         if(strcmp(p, "mmi")==0)
         {
            tmpF = atof(q);
            if(tmpF != PW1)
            {
               sprintf(tmp, " -mmi %.2f\0", tmpF);
               strcat(actualCommand, tmp);
            } /* fi */

            PW1 = tmpF;
         } /* else fi */

         else if (strcmp(p, "trg")==0)
         {
            tmpF = atof(q);
            if(tmpF != PW2)
            {
               sprintf(tmp, " -trg %.2f\0", tmpF);
               strcat(actualCommand, tmp);
            } /* fi */

            PW2 = tmpF;
         } /* else fi */

         else if (strcmp(p, "pub")==0)
         {
            tmpF = atof(q);
            if(tmpF != PW4)
            {
               sprintf(tmp, " -pub %.2f\0", tmpF);
               strcat(actualCommand, tmp);
            } /* fi */

            PW4 = tmpF;
         } /* else fi */

         else if (strcmp(p, "dir")==0)
         {
            tmpF = atof(q);
            if(tmpF != DIRECT)
            {
               sprintf(tmp, " -dir %.2f\0", tmpF);
               strcat(actualCommand, tmp);
            } /* fi */

            DIRECT = tmpF;
         } /* else fi */

         else if (strcmp(p, "atx")==0)
         {
            tmpF = atof(q);
            if(tmpF != ATX)
            {
               sprintf(tmp, " -atx %.2f\0", tmpF);
               strcat(actualCommand, tmp);
            } /* fi */

            ATX = tmpF;
         } /* else fi */

         else if (strcmp(p, "par")==0)
         {
            tmpF = atof(q);
            if(tmpF != GP75)
            {
               sprintf(tmp, " -par %.2f\0", tmpF);
               strcat(actualCommand, tmp);
            } /* fi */

            GP75 = tmpF;
         } /* else fi */

         else if (strcmp(p, "chd")==0)
         {
            tmpF = atof(q);
            if(tmpF != GC75)
            {
               sprintf(tmp, " -chd %.2f\0", tmpF);
               strcat(actualCommand, tmp);
            } /* fi */

            GC75 = tmpF;
         } /* else fi */

         else if (strcmp(p, "sib")==0)
         {
            tmpF = atof(q);
            if(tmpF != GS75)
            {
               sprintf(tmp, " -sib %.2f\0", tmpF);
               strcat(actualCommand, tmp);
            } /* fi */

            GS75 = tmpF;
         } /* else fi */

         else if (strcmp(p, "oth")==0)
         {
            tmpF = atof(q);
            if(tmpF != OTHER)
            {
               sprintf(tmp, " -oth %.2f\0", tmpF);
               strcat(actualCommand, tmp);
            } /* fi */

            OTHER = tmpF;
         } /* else fi */

         else if (strcmp(p, "im")==0)
         {
            tmpF = atof(q);
            if(tmpF != IM)
            {
               sprintf(tmp, " -im %.2f\0", tmpF);
               strcat(actualCommand, tmp);
            } /* fi */

            IM = tmpF;
         } /* else fi */

         else if (strcmp(p, "nim")==0)
         {
            tmpF = atof(q);
            if(tmpF != NIM)
            {
               sprintf(tmp, " -nim %.2f\0", tmpF);
               strcat(actualCommand, tmp);
            } /* fi */

            NIM = tmpF;
         } /* else fi */

         else if (strcmp(p, "emp_hstar")==0)
         {
            tmpF = atof(q);
            if(tmpF != EMP_HSTAR)
            {
               sprintf(tmp, " -emp_hstar %.2f\0", tmpF);
               strcat(actualCommand, tmp);
            } /* fi */

            EMP_HSTAR = tmpF;
         } /* else fi */

         else if (strcmp(p, "sec")==0)
         {
            tmpF = atof(q);
            if(tmpF != mySEC)
            {
               sprintf(tmp, " -sec %.2f\0", tmpF);
               strcat(actualCommand, tmp);
            } /* fi */

            mySEC = tmpF;
         } /* else fi */

         else if (strcmp(p, "cit")==0)
         {
            tmpI = atoi(q);
            if(tmpI != top_relcit)
            {
               sprintf(tmp, " -cit %d\0", tmpI);
               strcat(actualCommand, tmp);
            } /* fi */

            top_relcit = tmpI;
         } /* else fi */

         else if (strcmp(p, "cot")==0)
         {
            tmpI = atoi(q);
            if(tmpI != COT_FACTOR)
            {
               sprintf(tmp, " -cot %d\0", tmpI);
               strcat(actualCommand, tmp);
            } /* fi */

            COT_FACTOR = tmpI;
         } /* else fi */

         else if (strcmp(p, "ti")==0)
         {
            tmpI = atoi(q);
            if(tmpI != TITLE_FACTOR)
            {
               sprintf(tmp, " -ti %d\0", tmpI);
               strcat(actualCommand, tmp);
            } /* fi */

            TITLE_FACTOR = tmpI;
         } /* else fi */

         else if (strcmp(p, "rel")==0)
         {
            tmpI = atoi(q);
            if(tmpI != TREE_FACTOR)
            {
               sprintf(tmp, " -rel %d\0", tmpI);
               strcat(actualCommand, tmp);
            } /* fi */

            TREE_FACTOR = tmpI;
         } /* else fi */

         else if (strcmp(p, "topn")==0)
         {
            tmpI = atoi(q);

            if(tmpI != topn)
            {
               sprintf(tmp, " -topn %d\0", tmpI);
               strcat(actualCommand, tmp);
            } /* fi */

            max = tmpI;
            topn = max;
         } /* else fi */

         else if(strcmp(p, "cutOffScore") == 0)
         {
            tmpI = atoi(q);
            if(tmpI != cutOffScore)
            {
               sprintf(tmp, " -cutOffScore %d\0", tmpI);
               strcat(actualCommand, tmp);
            } /* fi */

            cutOffScore = tmpI;
         } /* else fi */

         else if(strcmp(p, "rRCCutOffScore") == 0)
         {
            tmpI = atoi(q);
            if(tmpI != rRCCutOffScore)
            {
               sprintf(tmp, " -rRCCutOffScore %d\0", tmpI);
               strcat(actualCommand, tmp);
            } /* fi */

            rRCCutOffScore = tmpI;
         } /* else fi */

         else if (strcmp(p, "TexToolHost")==0)
         {
            if(strcmp(TexTool_Host, q) != 0)
            {
               sprintf(tmp, " -TexToolHost %s\0", q);
               strcat(actualCommand, tmp);
            } /* fi */

            TTHost_specified = TRUE;
            strcat(TexTool_Host, q);
         } /* else fi */

         else if (strcmp(p, "mindex")==0)
         {
            if(strcmp(mmi_index, q) != 0)
            {
               sprintf(tmp, " -mindex %s\0", q);
               strcat(actualCommand, tmp);
            } /* fi */

            read_MMI = TRUE;
            strcat(mmi_index, q);
         }

         else if (strcmp(p, "tindex")==0)
         {
            if(strcmp(trg_index, q) != 0)
            {
               sprintf(tmp, " -tindex %s\0", q);
               strcat(actualCommand, tmp);
            } /* fi */

            read_TRG = TRUE;
            strcat(trg_index, q);
         }

         else if (strcmp(p, "rindex")==0)
         {
            if(strcmp(rel_index, q) != 0)
            {
               sprintf(tmp, " -rindex %s\0", q);
               strcat(actualCommand, tmp);
            } /* fi */

            read_REL = TRUE;
            strcat(rel_index, q);
         } /* else fi */
     } /* fi we have "|" */

     else if (strcmp(strbuf, "doTiming") == 0)
     {
         doTimingFlag = TRUE;
         strcat(actualCommand, " -doTiming");
     } /* else fi */

     else if (strcmp(strbuf, "LOCAL")==0)
     {
        strcat(actualCommand, " -LOCAL");
        LOCAL = TRUE;
        REMOTE = FALSE;
     } /* else fi */

     else if (strcmp(strbuf, "REMOTE") == 0)
     {
        strcat(actualCommand, " -REMOTE");
        REMOTE = TRUE;
        LOCAL = FALSE;
     } /* else fi */

     else if (strcmp(strbuf, "EOT") == 0)
     {
        strcat(actualCommand, " -EOT");
        EOT_FLAG = TRUE;
     } /* else fi */

     else if (strcmp(strbuf, "JSON") == 0)
        JSON = TRUE;

     else if (strcmp(strbuf, "xml") == 0)
        strcpy(display, "xml");

     else if(strcmp(strbuf, "justFacts") == 0)
         strcpy(display, "justFacts");

     else if (strcmp(strbuf, "detail")==0)
        strcpy(display, "detail");

     else if (strcmp(strbuf, "detail2")==0)
        strcpy(display, "detail2");

     else if (strcmp(strbuf, "debug1")==0)
        strcpy(display, "debug1");

     else if (strcmp(strbuf, "display_simple")==0)
        strcpy(display, "display_simple");

     else if (strcmp(strbuf, "display_simple2")==0)
        strcpy(display, "display_simple2");

     else if (strcmp(strbuf, "display_simple3")==0)
        strcpy(display, "display_simple3");

     else if (strcmp(strbuf, "display_full")==0)
        strcpy(display, "display_full_listing");

     else if(strcmp(strbuf, "display_fullR") == 0)
        strcpy(display, "display_OK_full_listing");

     else if (strcmp(strbuf, "detail_list")==0)
        strcpy(display, "detail_list");

     else if (strcmp(strbuf, "detail_listII")==0)
        strcpy(display, "detail_listII");

     else if (strcmp(strbuf, "SMALL")==0)
     {
        if(!SMALL)
          strcat(actualCommand, " -SMALL");
        SMALL = TRUE;
     } /* else fi */

     else if (strcmp(strbuf, "strictFilter")==0)
     {
        if(!strictFilter_set)
          strcat(actualCommand, " -strictFilter");
        strictFilter_set = TRUE;
     } /* else fi */

     else if (strcmp(strbuf, "medFilter")==0)
     {
        if(!medFilter_set)
          strcat(actualCommand, " -medFilter");
        medFilter_set = TRUE;
     } /* else fi */

     else if (strcmp(strbuf, "starMHTI")==0)
     {
        if(!starMHTI_set)
          strcat(actualCommand, " -starMHTI");
        starMHTI_set = TRUE;
     } /* else fi */

     else if (strcmp(strbuf, "medFilterR")==0)
     {
        if(!medFilterR_set)
          strcat(actualCommand, " -medFilterR");
        medFilter_set = TRUE;
        medFilterR_set = TRUE;
     } /* else fi */

     else if(strstr(strbuf, "RTM_Debug") != NULL)
     {
        if(!RTM_Debug)
          strcat(actualCommand, " -RTM_Debug");
        RTM_Debug = TRUE;
     } /* else fi */

     else if (strcmp(strbuf, "PRC_Debug")==0)
     {
        if(!PRC_Debug)
          strcat(actualCommand, " -PRC_Debug");
        PRC_Debug = TRUE;
     } /* else fi */

     else if (strcmp(strbuf, "HMIIPA_Debug")==0)
     {
        if(!HMIIPA_Debug)
          strcat(actualCommand, " -HMIIPA_Debug");
        HMIIPA_Debug = TRUE;
     } /* else fi */

     else if (strcmp(strbuf, "RSfilterTO")==0)
     {
        if(!RSfilterTO)
          strcat(actualCommand, " -RSfilterTO");
        RSfilterTO = TRUE;
     } /* else fi */

     else if (strcmp(strbuf, "RSfilterALL")==0)
     {
        if(!RSfilterALL)
          strcat(actualCommand, " -RSfilterALL");
        RSfilterALL = TRUE;
     } /* else fi */

     else if (strcmp(strbuf, "showRSfilter")==0)
     {
        if(!showRSfilter)
          strcat(actualCommand, " -showRSfilter");
        showRSfilter = TRUE;
     } /* else fi */

     else if (strcmp(strbuf, "addCTs")==0)
     {
        if(!addCTs)
          strcat(actualCommand, " -addCTs");
        addCTs = TRUE;
     } /* else fi */

     else if (strcmp(strbuf, "addGEOs")==0)
     {
        if(!addGEOs)
          strcat(actualCommand, " -addGEOs");
        addGEOs = TRUE;
     } /* else fi */

     else if (strcmp(strbuf, "addUSA")==0)
     {
        if(!addUSA)
          strcat(actualCommand, " -addUSA");
        addUSA = TRUE;
     } /* else fi */

     else if (strcmp(strbuf, "forceNewTerms")==0)
     {
        if(!forceNewTerms)
          strcat(actualCommand, " -forceNewTerms");
        forceNewTerms = TRUE;
     } /* else fi */

     else if (strcmp(strbuf, "doSAP")==0)
     {
        if(!doSAP)
          strcat(actualCommand, " -doSAP");
        doSAP = TRUE;
     } /* else fi */

     else if (strcmp(strbuf, "addAddls")==0)
     {
        if(!addAddls)
          strcat(actualCommand, " -addAddls");
        addAddls = TRUE;
     } /* else fi */

     else if (strcmp(strbuf, "removeRCOnly")==0)
     {
        if(!removeRCOnly)
          strcat(actualCommand, " -removeRCOnly");
        removeRCOnly = TRUE;
     } /* else fi */

     else if (strcmp(strbuf, "doAgedReview")==0)
     {
        if(!doAgedReview)
          strcat(actualCommand, " -doAgedReview");
        doAgedReview = TRUE;
     } /* else fi */

     else if (strcmp(strbuf, "showAgedReview")==0)
     {
        if(!showAgedReview)
          strcat(actualCommand, " -showAgedReview");
        showAgedReview = TRUE;
     } /* else fi */

     else if (strcmp(strbuf, "showDUIs")==0)
     {
        if(!showDUIs)
          strcat(actualCommand, " -showDUIs");
        showDUIs = TRUE;
     } /* else fi */

     else if (strcmp(strbuf, "remMHs")==0)
     {
        if(!remMHs)
          strcat(actualCommand, " -remMHs");
        remMHs = TRUE;
     } /* else fi */

     else if (strcmp(strbuf, "showTUIs")==0)
     {
        if(!showTUIs)
          strcat(actualCommand, " -showTUIs");
        showTUIs = TRUE;
     } /* else fi */

     else if (strcmp(strbuf, "showHMsD")==0)
     {
        if(!showHMsD)
          strcat(actualCommand, " -showHMsD");
        showHMsD = TRUE;
     } /* else fi */

     else if (strcmp(strbuf, "showHMs")==0)
     {
        if(!showHMs)
          strcat(actualCommand, " -showHMs");
        showHMs = TRUE;
     } /* else fi */

     else if (strcmp(strbuf, "showETs")==0)
     {
        if(!showETs)
          strcat(actualCommand, " -showETs");
        showETs = TRUE;
     } /* else fi */

     else if (strcmp(strbuf, "ETsOFF")==0)
        showETs = FALSE;

     else if (strcmp(strbuf, "showCalc")==0)
     {
        if(!showCalc)
          strcat(actualCommand, " -showCalc");
        showCalc = TRUE;
     } /* else fi */

     else if (strcmp(strbuf, "showETsD")==0)
     {
        if(!showETsD)
          strcat(actualCommand, " -showETsD");
        showETs = TRUE;
        showETsD = TRUE;
     } /* else fi */

     else if (strcmp(strbuf, "showJust")==0)
     {
        if(!showJust)
          strcat(actualCommand, " -showJust");
        showJust = TRUE;
        trackPositional = TRUE;
     } /* else fi */

     else if(strcmp(strbuf, "exitNoResult") == 0)
     {
        if(!exitNoResult)
          strcat(actualCommand, " -exitNoResult");
        exitNoResult = TRUE;
     } /* else fi */

     else if(strcmp(strbuf, "useWSD") == 0)
     {
        if(!useWSD)
          strcat(actualCommand, " -useWSD");
        useWSD = TRUE;
     } /* else fi */

     else if(strcmp(strbuf, "useWSDTIOnly") == 0)
     {
        if(!useWSDTIOnly)
          strcat(actualCommand, " -useWSDTIOnly");
        useWSDTIOnly = TRUE;
     } /* else fi */

     else if(strcmp(strbuf, "RTM_logNF") == 0)
     {
        if(!RTM_logNF)
          strcat(actualCommand, " -RTM_logNF");
        RTM_logNF = TRUE;
     } /* else fi */

     else if (strcmp(strbuf, "noCheckRC")==0)
     {
        if(!noCheckRC)
          strcat(actualCommand, " -noCheckRC");
        noCheckRC = TRUE;
     } /* else fi */

     else if (strcmp(strbuf, "limitTitleOnly")==0)
     {
        if(!limitTitleOnly)
          strcat(actualCommand, " -limitTitleOnly");
        limitTitleOnly = TRUE;
     } /* else fi */

     else if (strcmp(strbuf, "ignoreMHSHDups")==0)
     {
        if(!doMHSHDups)
          strcat(actualCommand, " -ignoreMHSHDups");
        doMHSHDups = FALSE;
     } /* else fi */

     else if (strcmp(strbuf, "limitPTs")==0)
     {
        if(!limitPTs)
          strcat(actualCommand, " -limitPTs");
        limitPTs = TRUE;
     } /* else fi */

     else if (strcmp(strbuf, "doLogging")==0)
     {
        if(!doLogging)
          strcat(actualCommand, " -doLogging");
        doLogging = TRUE;
     } /* else fi */

     else if (strcmp(strbuf, "cutOff15")==0)
     {
        if(!cutOff15)
          strcat(actualCommand, " -cutOff15");
        cutOff15 = TRUE;
     } /* else fi */

     else if (strcmp(strbuf, "MMIO_Filter")==0)
     {
        if(!MMIO_Filter)
          strcat(actualCommand, " -MMIO_Filter");
        MMIO_Filter = TRUE;
     } /* else fi */

     else if (strcmp(strbuf, "doSuppChemUpdate")==0)
     {
        if(!doSuppChemUpdate)
          strcat(actualCommand, " -doSuppChemUpdate");
        doSuppChemUpdate = TRUE;
     } /* else fi */

     else if (strcmp(strbuf, "testSpecials")==0)
     {
        if(!testSpecials)
          strcat(actualCommand, " -testSpecials");
        testSpecials = TRUE;
     } /* else fi */

     else if (strcmp(strbuf, "showInterimBreakout")==0)
     {
        if(!showInterimBreakout)
          strcat(actualCommand, " -showInterimBreakout");
        showInterimBreakout = TRUE;
     } /* else fi */

     else if (strcmp(strbuf, "showInterimIIBreakout")==0)
     {
        if(!showInterimIIBreakout)
          strcat(actualCommand, " -showInterimIIBreakout");
        showInterimIIBreakout = TRUE;
     } /* else fi */

     else if (strcmp(strbuf, "noShowBL")==0)
     {
        if(!noShowBL)
          strcat(actualCommand, " -noShowBL");
        noShowBL = TRUE;
     } /* else fi */

     else if (strcmp(strbuf, "writeTables")==0)
     {
        if(!writeTables)
          strcat(actualCommand, " -writeTables");
        writeTables = TRUE;
     } /* else fi */

     else if (strcmp(strbuf, "writeSpecial")==0)
     {
        if(!writeSpecial)
          strcat(actualCommand, " -writeSpecial");
        writeSpecial = TRUE;
     } /* else fi */

     else if (strcmp(strbuf, "showTreecodes")==0)
     {
        if(!showTreecodes)
          strcat(actualCommand, " -showTreecodes");
        showTreecodes = TRUE;
     } /* else fi */

     else if (strcmp(strbuf, "idCarboSeq")==0)
     {
        if(!idCarboSeq)
          strcat(actualCommand, " -idCarboSeq");
        idCarboSeq = TRUE;
     } /* else fi */

     else if (strcmp(strbuf, "forceMHs")==0)
     {
        if(!forceMHs)
          strcat(actualCommand, " -forceMHs");
        forceMHs = TRUE;
     } /* else fi */

     else if (strcmp(strbuf, "doNoAddForced")==0)
     {
        if(!doNoAddForced)
          strcat(actualCommand, " -doNoAddForced");
        doNoAddForced = TRUE;
     } /* else fi */

     else if (strcmp(strbuf, "forceMTIFL")==0)
     {
        if(!forceMTIFL)
          strcat(actualCommand, " -forceMTIFL");
        forceMTIFL = TRUE;
     } /* else fi */

     else if (strcmp(strbuf, "forceCHJ")==0)
     {
        if(!forceCHJ)
          strcat(actualCommand, " -forceCHJ");
        forceCHJ = TRUE;
     } /* else fi */

     else if (strcmp(strbuf, "forceMTIFLReview")==0)
     {
        if(!forceMTIFLReview)
          strcat(actualCommand, " -forceMTIFLReview");
        forceMTIFLReview = TRUE;
     } /* else fi */

     else if (strcmp(strbuf, "noMTIFL")==0)
     {
        if(!noMTIFL)
          strcat(actualCommand, " -noMTIFL");
        noMTIFL = TRUE;
     } /* else fi */

     else if(strcmp(strbuf, "doAMLFilter") == 0) /* Antonio ML Filtering */
     {
        if(!doAMLFilter)
          strcat(actualCommand, " -doAMLFilter");
        doAMLFilter = TRUE;
     } /* else fi */

     else if(strcmp(strbuf, "doIL2R") == 0) /* Ilya's ML L2R */
     {
        if(!doIL2R)
          strcat(actualCommand, " -doIL2R");
        doIL2R = TRUE;
     } /* else fi */

     else if(strcmp(strbuf, "doIL2RHybrid") == 0)
     {
        if(!doIL2RHybrid)
          strcat(actualCommand, " -doIL2RHybrid");
        doIL2RHybrid = TRUE;
     } /* else fi */

     else if(strcmp(strbuf, "goHigh") == 0)
     {
        if(!goHigh)
          strcat(actualCommand, " -goHigh");
        goHigh = TRUE;
     } /* else fi */

      else if(strcmp(strbuf, "Special_Output") == 0)
      {
        if(!writeSpecial)
          strcat(actualCommand, " -Special_Output");
         writeSpecial = TRUE;
         SMALL = TRUE;
      } /* else fi */

      else if(strcmp(strbuf, "showDisclaimer") == 0)
      {
          strcat(actualCommand, " -showDisclaimer");
          showDisclaimer = TRUE;
      } /* else fi */

      else if(strcmp(strbuf, "showRemovals") == 0)
      {
          strcat(actualCommand, " -showRemovals");
          showRemovals = TRUE;
      } /* else fi */

      else if(strcmp(strbuf, "showScores") == 0)
      {
          strcat(actualCommand, " -showScores");
          showScores = TRUE;
          showETs = FALSE;
      } /* else fi */

      else if(strcmp(strbuf, "showScoresFull") == 0)
      {
          strcat(actualCommand, " -showScoresFull");
          showScoresFull = TRUE;
          showETs = FALSE;
      } /* else fi */

      else if(strcmp(strbuf, "showMeSHVersion") == 0)
      {
          strcat(actualCommand, " -showMeSHVersion");
          showMeSHVersion = TRUE;
      } /* else fi */

      else if(strcmp(strbuf, "preIndex") == 0)
      {
          strcat(actualCommand, " -preIndex");
          preIndex = TRUE;
          showETs = FALSE;
      } /* else fi */

      else if(strcmp(strbuf, "level1Filter") == 0)
      {
          strcat(actualCommand, " -level1Filter");
          level1Filter = TRUE;
          showETs = FALSE;
      } /* else fi */

      else if(strcmp(strbuf, "MTI_AUTO") == 0)
      {
          strcat(actualCommand, " -MTI_AUTO");
          MTI_REVIEW = FALSE;
          MTI_AUTO = TRUE;
          showETs = FALSE;
      } /* else fi */

      else if(strcmp(strbuf, "MTI_REVIEW") == 0)
      {
          strcat(actualCommand, " -MTI_REVIEW");
          MTI_REVIEW = TRUE;
          MTI_AUTO = TRUE;
          showETs = FALSE;
      } /* else fi */

      else if(strcmp(strbuf, "ACCME") == 0)
      {
          strcat(actualCommand, " -ACCME");
          MTI_AUTO = TRUE;
          ACCME = TRUE;
          showETs = FALSE;
      } /* else fi */

      else if(strcmp(strbuf, "trackPositional") == 0)
      {
          strcat(actualCommand, " -trackPositional");
          trackPositional = TRUE;
      } /* else fi */

     else if (strcmp(strbuf, "default")==0)
     {
         strcpy(actualCommand, " -default");
         reset_vars();
         addCTs = TRUE;
         addGEOs = TRUE;
         addUSA = TRUE;
         remMHs = TRUE;
         noCheckRC = TRUE;
         limitTitleOnly = TRUE;
         doSuppChemUpdate = TRUE;
         doAgedReview = TRUE;
         strcpy(display, "detail");
     } /* else fi */

      else if (strcmp(strbuf, "default_MTI") == 0)  /* No eLink try */
      {
         strcpy(actualCommand, " -default_MTI");
         reset_vars();
         addCTs = TRUE;
         addGEOs = TRUE;
         addUSA = TRUE;
         forceNewTerms = TRUE;
         remMHs = TRUE;
         showHMs = FALSE;
         showETs = FALSE;
         noCheckRC = TRUE;
         lTO_II = TRUE;
         limitTitleOnly = TRUE;
         limitPTs = TRUE;
         starMHTI_set = FALSE;
         RSfilterTO = TRUE;
         RSfilterALL = TRUE;
         showTUIs = FALSE;
         exitNoResult = TRUE;
         useWSD = FALSE;
         useWSDTIOnly = FALSE;
         doSuppChemUpdate = TRUE;
         doAgedReview = TRUE;
         strcpy(display, "justFacts");
         NORMAL = TRUE;
         CATALOGING = FALSE;
         MMIO_Filter = TRUE;
         idCarboSeq = FALSE;
         forceMHs = TRUE;
         checkForFirstLine = FALSE;
         trackPositional = FALSE;
         doAMLFilter = TRUE;
      } /* else fi */

      else if (strcmp(strbuf, "defaultL_MTI") == 0)  /* Try eLink first */
      {
         strcpy(actualCommand, " -defaultL_MTI");
         reset_vars();
         addCTs = TRUE;
         addGEOs = TRUE;
         addUSA = TRUE;
         forceNewTerms = TRUE;
         remMHs = TRUE;
         showHMs = FALSE;
         showETs = FALSE;
         noCheckRC = TRUE;
         lTO_II = TRUE;
         limitTitleOnly = TRUE;
         limitPTs = TRUE;
         starMHTI_set = FALSE;
         RSfilterTO = TRUE;
         RSfilterALL = TRUE;
         try_Elink = TRUE;
         showTUIs = FALSE;
         exitNoResult = TRUE;
         useWSD = FALSE;
         useWSDTIOnly = FALSE;
         doSuppChemUpdate = TRUE;
         doAgedReview = TRUE;
         strcpy(display, "justFacts");
         NORMAL = TRUE;
         CATALOGING = FALSE;
         MMIO_Filter = TRUE;
         idCarboSeq = FALSE;
         forceMHs = TRUE;
         checkForFirstLine = FALSE;
         trackPositional = FALSE;
         doAMLFilter = TRUE;
         forceOK_Title = TRUE;
      } /* else fi */

     else if (strcmp(strbuf, "default_DCMS")==0)
     {
        strcpy(actualCommand, " -default_DCMS");
        reset_vars();
         addCTs = TRUE;
         addGEOs = TRUE;
         addUSA = TRUE;
         forceNewTerms = TRUE;
         remMHs = TRUE;
         showHMs = FALSE;
         showETs = FALSE;
         noCheckRC = TRUE;
         lTO_II = TRUE;
         limitTitleOnly = TRUE;
         limitPTs = TRUE;
         starMHTI_set = FALSE;
         RSfilterTO = TRUE;
         RSfilterALL = TRUE;
         try_Elink = TRUE;
         showTUIs = FALSE;
         exitNoResult = TRUE;
         useWSD = FALSE;
         useWSDTIOnly = FALSE;
         doSuppChemUpdate = TRUE;
         doAgedReview = TRUE;
         strcpy(display, "justFacts");
         NORMAL = TRUE;
         CATALOGING = FALSE;
         MMIO_Filter = TRUE;
         idCarboSeq = FALSE;
         forceMHs = TRUE;
         checkForFirstLine = FALSE;
         trackPositional = FALSE;
         doAMLFilter = TRUE;
         forceOK_Title = TRUE;
     } /* else fi */

     else if (strcmp(strbuf, "defaultL_DCMS")==0)
     {
        strcpy(actualCommand, " -defaultL_DCMS");
        reset_vars();
        addCTs = TRUE;
        addGEOs = TRUE;
        addUSA = TRUE;
        remMHs = TRUE;
        showHMs = TRUE;
        showETs = TRUE;
        noCheckRC = TRUE;
        lTO_II = TRUE;
        limitTitleOnly = TRUE;
        limitPTs = TRUE;
        starMHTI_set = TRUE;
        RSfilterTO = TRUE;
        RSfilterALL = TRUE;
        try_Elink = TRUE;
        showTUIs = TRUE;
        doSuppChemUpdate = TRUE;
        doAgedReview = TRUE;
        exitNoResult = TRUE;
        useWSD = FALSE;
        useWSDTIOnly = FALSE;
        strcpy(display, "detail_list");
        MMIO_Filter = TRUE;
        idCarboSeq = TRUE;
        forceMHs = TRUE;
        forceNewTerms = TRUE;
        checkForFirstLine = TRUE;
        trackPositional = TRUE;
        forceOK_Title = TRUE;
     } /* else fi */

     else if (strcmp(strbuf, "opt1_DCMS")==0)
     {
        strcpy(actualCommand, " -opt1_DCMS");
        reset_vars();
        addCTs = TRUE;
        addGEOs = TRUE;
        addUSA = TRUE;
        remMHs = TRUE;
        showHMs = TRUE;
        showETs = TRUE;
        noCheckRC = TRUE;
        lTO_II = TRUE;
        limitTitleOnly = TRUE;
        limitPTs = TRUE;
        starMHTI_set = TRUE;
        RSfilterTO = TRUE;
        RSfilterALL = TRUE;
        doSuppChemUpdate = TRUE;
        doAgedReview = TRUE;
        exitNoResult = TRUE;
        useWSD = FALSE;
        useWSDTIOnly = FALSE;
        strcpy(display, "detail");
        MMIO_Filter = TRUE;
        idCarboSeq = TRUE;
        forceMHs = TRUE;
        forceNewTerms = TRUE;
        checkForFirstLine = TRUE;
        trackPositional = TRUE;
         forceOK_Title = TRUE;
     } /* else fi */

     else if (strcmp(strbuf, "opt1L_DCMS")==0)
     {
        strcpy(actualCommand, " -opt1L_DCMS");
        reset_vars();
        addCTs = TRUE;
        addGEOs = TRUE;
        addUSA = TRUE;
        remMHs = TRUE;
        showHMs = TRUE;
        showETs = TRUE;
        noCheckRC = TRUE;
        lTO_II = TRUE;
        limitTitleOnly = TRUE;
        limitPTs = TRUE;
        starMHTI_set = TRUE;
        RSfilterTO = TRUE;
        RSfilterALL = TRUE;
        try_Elink = TRUE;
        doSuppChemUpdate = TRUE;
        doAgedReview = TRUE;
        exitNoResult = TRUE;
        useWSD = FALSE;
        useWSDTIOnly = FALSE;
        strcpy(display, "detail");
        MMIO_Filter = TRUE;
        idCarboSeq = TRUE;
        forceMHs = TRUE;
        forceNewTerms = TRUE;
        checkForFirstLine = TRUE;
        trackPositional = TRUE;
         forceOK_Title = TRUE;
     } /* else fi */

     else if (strcmp(strbuf, "BioASQ")==0)
     {
        strcpy(actualCommand, " -BioASQ");
        reset_vars();
        BioASQ = TRUE;
        addCTs = TRUE;
        addGEOs = TRUE;
        addUSA = TRUE;
        forceNewTerms = FALSE;
        remMHs = TRUE;
        showHMs = TRUE;
        showETs = FALSE;
        noCheckRC = TRUE;
        lTO_II = TRUE;
        limitTitleOnly = TRUE;
        limitPTs = TRUE;
        starMHTI_set = FALSE;
        RSfilterTO = TRUE;
        RSfilterALL = TRUE;
        try_Elink = TRUE;
        doSuppChemUpdate = TRUE;
        doAgedReview = TRUE;
        exitNoResult = TRUE;
        useWSD = FALSE;
        useWSDTIOnly = FALSE;
        strcpy(display, "BioASQ");
        NORMAL = TRUE;
        CATALOGING = FALSE;
        MMIO_Filter = TRUE;
        idCarboSeq = TRUE;
        forceMHs = TRUE;
        checkForFirstLine = TRUE;
        forceMTIFL = TRUE;
        doAMLFilter = TRUE;
        trackPositional = TRUE;
     } /* else fi */

      else if(strcmp(strbuf, "MeSHonDemand") == 0)  /* No eLink try */
      {
         strcpy(actualCommand, " -MeSHonDemand");
         reset_vars();
         addCTs = TRUE;
         addGEOs = TRUE;
         addUSA = TRUE;
         forceNewTerms = FALSE;
         remMHs = TRUE;
         showHMs = TRUE;
         showETs = FALSE;
         noCheckRC = TRUE;
         lTO_II = TRUE;
         limitTitleOnly = TRUE;
         limitPTs = TRUE;
         starMHTI_set = FALSE;
         RSfilterTO = TRUE;
         RSfilterALL = TRUE;
         exitNoResult = TRUE;
         useWSD = FALSE;
         useWSDTIOnly = FALSE;
         doSuppChemUpdate = TRUE;
         doAgedReview = TRUE;
         strcpy(display, "display_simple");
         NORMAL = TRUE;
         CATALOGING = FALSE;
         MMIO_Filter = TRUE;
         idCarboSeq = TRUE;
         forceMHs = TRUE;
         checkForFirstLine = FALSE;
         forceMTIFL = TRUE;
         doAMLFilter = TRUE;
         showMeSHVersion = TRUE;
         showDisclaimer = TRUE;
         showType = TRUE;
         trackPositional = FALSE;
         MeSHonDemand = TRUE;
         MeSHonDemand2 = FALSE;
         MoD_Raw = FALSE;
         try_Elink = FALSE;
      } /* else fi */

      else if(strcmp(strbuf, "MoD_Raw") == 0)  /* No eLink try */
      {
         strcpy(actualCommand, " -MoD_Raw");
         reset_vars();
         addCTs = TRUE;
         addGEOs = TRUE;
         addUSA = TRUE;
         forceNewTerms = FALSE;
         remMHs = TRUE;
         showHMs = TRUE;
         showETs = FALSE;
         noCheckRC = TRUE;
         lTO_II = TRUE;
         limitTitleOnly = TRUE;
         limitPTs = TRUE;
         starMHTI_set = FALSE;
         RSfilterTO = TRUE;
         RSfilterALL = TRUE;
         exitNoResult = TRUE;
         useWSD = FALSE;
         useWSDTIOnly = FALSE;
         doSuppChemUpdate = TRUE;
         doAgedReview = TRUE;
         strcpy(display, "display_simple");
         NORMAL = TRUE;
         CATALOGING = FALSE;
         MMIO_Filter = TRUE;
         idCarboSeq = TRUE;
         forceMHs = TRUE;
         checkForFirstLine = FALSE;
         forceMTIFL = TRUE;
         doAMLFilter = TRUE;
         showMeSHVersion = TRUE;
         showDisclaimer = TRUE;
         showType = TRUE;
         trackPositional = FALSE;
         MeSHonDemand = TRUE;
         MeSHonDemand2 = FALSE;
         MoD_Raw = TRUE;
         try_Elink = FALSE;
      } /* else fi */

      else if(strcmp(strbuf, "MeSHonDemand2") == 0)  /* No eLink try */
      {
         reset_vars();
         addCTs = TRUE;
         addGEOs = TRUE;
         addUSA = TRUE;
         forceNewTerms = FALSE;
         remMHs = TRUE;
         showHMs = TRUE;
         showETs = FALSE;
         noCheckRC = TRUE;
         lTO_II = TRUE;
         limitTitleOnly = TRUE;
         limitPTs = TRUE;
         starMHTI_set = FALSE;
         RSfilterTO = TRUE;
         RSfilterALL = TRUE;
         exitNoResult = TRUE;
         useWSD = FALSE;
         useWSDTIOnly = FALSE;
         doSuppChemUpdate = TRUE;
         doAgedReview = TRUE;
         strcpy(display, "display_simple");
         NORMAL = TRUE;
         CATALOGING = FALSE;
         MMIO_Filter = TRUE;
         idCarboSeq = TRUE;
         forceMHs = TRUE;
         checkForFirstLine = FALSE;
         forceMTIFL = TRUE;
         doAMLFilter = FALSE;
         showMeSHVersion = FALSE;
         showDisclaimer = FALSE;
         showType = TRUE;
         trackPositional = TRUE;
         MeSHonDemand = TRUE;
         MeSHonDemand2 = TRUE;
         MoD_Raw = FALSE;
         try_Elink = FALSE;
      } /* else fi */

      else if(strcmp(strbuf, "MoD_PP") == 0)  /* Pretty Print */
      {
         strcpy(actualCommand, " -MoD_PP");
         reset_vars();
         addCTs = TRUE;
         addGEOs = TRUE;
         addUSA = TRUE;
         forceNewTerms = FALSE;
         remMHs = TRUE;
         showHMs = TRUE;
         showETs = FALSE;
         noCheckRC = TRUE;
         lTO_II = TRUE;
         limitTitleOnly = TRUE;
         limitPTs = TRUE;
         starMHTI_set = FALSE;
         RSfilterTO = TRUE;
         RSfilterALL = TRUE;
         exitNoResult = TRUE;
         useWSD = FALSE;
         useWSDTIOnly = FALSE;
         doSuppChemUpdate = TRUE;
         doAgedReview = TRUE;
         strcpy(display, "justFacts");
         NORMAL = TRUE;
         CATALOGING = FALSE;
         MMIO_Filter = TRUE;
         idCarboSeq = TRUE;
         forceMHs = TRUE;
         checkForFirstLine = FALSE;
         forceMTIFL = TRUE;
         doAMLFilter = FALSE;
         showMeSHVersion = FALSE;
         showDisclaimer = FALSE;
         showType = TRUE;
         trackPositional = FALSE;
         MeSHonDemand = TRUE;
         MeSHonDemand2 = FALSE;
         MoD_Raw = FALSE;
         MoD_PP = TRUE;
         showDUIs = TRUE;
         try_Elink = FALSE;
      } /* else fi */

      else if(strcmp(strbuf, "default_Strict") == 0)
      {
         strcpy(actualCommand, " -default_Strict");
         reset_vars();
         addCTs = FALSE;
         addGEOs = FALSE;
         addUSA = FALSE;
         remMHs = TRUE;
         showHMs = FALSE;
         showETs = FALSE;
         noCheckRC = FALSE;
         RSfilterTO = FALSE;
         showTUIs = FALSE;
         doSuppChemUpdate = TRUE;
         doAgedReview = FALSE;
         strictFilter_set = TRUE;
         strcpy(display, "detail_list");
      } /* else fi */

      else if(strcmp(strbuf, "opt1_Strict") == 0)
      {
         strcpy(actualCommand, " -opt1_Strict");
         reset_vars();
         addCTs = FALSE;
         addGEOs = FALSE;
         addUSA = FALSE;
         remMHs = TRUE;
         showHMs = FALSE;
         showETs = FALSE;
         noCheckRC = FALSE;
         RSfilterTO = FALSE;
         showTUIs = FALSE;
         doSuppChemUpdate = TRUE;
         doAgedReview = FALSE;
         strictFilter_set = TRUE;
         strcpy(display, "detail");
      } /* else fi */

     else if(strcmp(strbuf, "default_BackOfBook") == 0)
     {
        strcpy(actualCommand, " -default_BackOfBook");
        reset_vars();
        addCTs = TRUE;
        addGEOs = TRUE;
        addUSA = TRUE;
        showETs = TRUE;
        limitTitleOnly = TRUE;
        limitPTs = TRUE;
        RSfilterTO = TRUE;
        RSfilterALL = TRUE;
        doSuppChemUpdate = TRUE;
        doAgedReview = TRUE;
        addAddls = TRUE;
        removeRCOnly = TRUE;
        rRCCutOffScore = 100000;
        doMHSHDups = FALSE;
        noShowBL = TRUE;
        useWSD = FALSE;
        strcpy(display, "detail");
        MMIO_Filter = TRUE;
     } /* else fi */

      else if((strcmp(strbuf, "-opt1_CATALOG") == 0) ||
              (strcmp(strbuf, "-opt1_Catalog") == 0))
      {
         strcpy(actualCommand, " -opt1_CATALOG");
         reset_vars();
         doMHSHDups = FALSE;
         topn = 10;
         addGEOs = TRUE;
         addUSA = TRUE;
         starMHTI_set = FALSE;
         doSuppChemUpdate = TRUE;
         medFilter_set = FALSE;
         strictFilter_set = FALSE;
         strcpy(display, "detail");
         useWSD = FALSE;
         NORMAL = FALSE;
         CATALOGING = TRUE;
         MMIO_Filter = FALSE;
         idCarboSeq = TRUE;
         forceMHs = TRUE;
         forceNewTerms = TRUE;
         trackPositional = TRUE;
      } /* else fi */

      else if((strcmp(strbuf, "-opt1L_CATALOG") == 0) ||
              (strcmp(strbuf, "-opt1L_Catalog") == 0))
      {
         strcpy(actualCommand, " -opt1L_CATALOG");
         reset_vars();
         doMHSHDups = FALSE;
         topn = 10;
         addGEOs = TRUE;
         addUSA = TRUE;
         starMHTI_set = FALSE;
         doSuppChemUpdate = TRUE;
         medFilter_set = FALSE;
         strictFilter_set = FALSE;
         strcpy(display, "detail");
         useWSD = FALSE;
         try_Elink = TRUE;
         NORMAL = FALSE;
         CATALOGING = TRUE;
         MMIO_Filter = FALSE;
         idCarboSeq = TRUE;
         forceMHs = TRUE;
         forceNewTerms = TRUE;
         trackPositional = TRUE;
      } /* else fi */
   } /* while */
   fclose(fp);

   if(strcmp(display, "detail") != 0)
   {
       strcat(actualCommand, " -");
       strcat(actualCommand, display);
   } /* fi */
} /* read_param_from_file */


void print_usage()
{
   printf("Usage:  ind <infile> [<outfile>] [<options>]\n\n");

   printf("  <infile> contains text\n");
   printf("  <outfile> is a file for results (default is stdout)\n\n");
   printf("  ind options:\n");
   printf("     -f <parameter file>  Read parameters from a file\n");
   printf("     -h                   Help - This usage message\n");
   printf("     -E                   Add EOT Marker at end of processing.\n");
   printf("     -mmi <float>         Set MetaMap weight, default=7\n");
   printf("     -trg <float>         Set Trigram Method weight, default=0\n");
   printf("     -pub <float>         Set related citation weight, default=2\n");
   printf("     -cot <int>           Set co-occuring factor, default=10000\n");
   printf("     -rel <int>           Set relations factor, default=100\n");
   printf("     -ti  <int>           Set title factor, default=20\n");
   printf("     -dir <float>         Set direct factor, default=1.0\n");
   printf("     -atx <float>         Set ATX factor, default=1.0\n");
   printf("     -par <float>         Set parent factor, default=0.9\n");
   printf("     -chd <float>         Set child factor, default=0.75\n");
   printf("     -sib <float>         Set sibling factor, default=0.7\n");
   printf("     -oth <float>         Set other factor, default=0.5\n");
   printf("     -im  <float>         Set IM factor, default=1.0\n");
   printf("     -nim <float>         Set NIM factor, default=0.8\n");
   printf("     -emp_hstar <float>   Set Emphasize HSTAR factor, default=0.0\n");
   printf("     -sec <float>         Set Emphasize document section factor, default=1.0\n");
   printf("     -cit <int>           Set number of related citations, default=10\n");
   printf("     -topn <int>          Set display of top n terms, default=25\n");
   printf("     -mindex <file>       Use preprocessed mmi index file\n");
   printf("     -tindex <file>       Use preprocessed trigram index file\n\n");
   printf("     -rindex <file>       Use preprocessed rel. cit. index file\n\n");
   printf("     -xml                 Display output in XML format\n");
   printf("     -detail              Display Path information after score.\n");
   printf("     -detail2             Display Expanded information after score.\n");
   printf("     -debug1              Display detail info plus cot, treerel, othrel arrays.\n");
   printf("     -display_full        Display Path information after score and ENTIRE list of items.\n");
   printf("     -display_simple      Display Simple output version with only MHs.\n");
   printf("     -display_simple2     Display Simple output with labels - Internal Use.\n");
   printf("     -display_simple3     Display Simple output with labels & ids - Internal Use.\n");
   printf("     -detail_list         Display recommendations in bar separated list.\n");
   printf("     -strictFilter              Remove recommendations not from all paths.\n");
   printf("     -medFilter              Susanne's tree structure based review.\n");
   printf("     -medFilterR             Susanne's tree structure based review with Removal information.\n");
   printf("     -starMHTI              Add stars (*) to MH when found in Title section of citation.\n");
   printf("     -useWSD              Turn on Word Sense Disambiguation.\n");
   printf("     -useWSDTIOnly        Turn on Word Sense Disambiguation for Title Only Citations.\n");
   printf("     -addCTs              Add CheckTags found in text.\n");
   printf("     -addGEOs             Add Geographics found in text.\n");
   printf("     -addUSA              Add United States of America if triggers found in text or recommendations.\n");
   printf("     -forceNewTerms       Force new terms to be included in recommendations list if found in text.\n");
   printf("     -addAddls            Add Misc. list of lookup terms.\n");
   printf("     -doAgedReview        Sonya's Aged/Human rules review.\n");
   printf("     -showAgedReview      Debug  of Aged/Human rules review.\n");
   printf("     -remMHs              Remove \"Do Not Index with\" MHs.\n");
   printf("     -showHMs             Display \"HM\" instead of \"MH\" for Headings Mapped to\n");
   printf("     -showHMsD             Display \"HM\" instead of \"MH\" for Headings Mapped to Debug Mode\n");
   printf("     -showETs             Display Entry Terms instead of \"MH\"\n");
   printf("     -showETsD            Display Entry Terms instead of \"MH\" - Debug mode\n");
   printf("     -nocheckRC           Don't run PubMed Related Citations result through MH_exclude - default is FALSE\n");
   printf("     -limitTitleOnly      Limit number of recommendations for Title only citations\n");
   printf("     -limitPTs            Limit number of recommendations for specific Publication Types\n");
   printf("     -cutOff15            Force cutoff after 15 terms less CTs and Forced\n");
   printf("     -MMIO_Filter         Filter MMI Only Recommendations\n");
   printf("     -SMALL               Used to run only a single path.\n");
   printf("     -writeTables         Used to store path results.\n");
   printf("     -writeSpecial        Internal Use.\n");
   printf("     -showTreecodes       Show MeSH Treecode(s) for each recommended term.\n");
   printf("     -showDUIs            Show MeSH Unique Identifier for each recommended term.\n");
   printf("     -showTUIs            Display TermUIs instead of MeSH Headings\n");
   printf("     -showInterimBreakout  Show raw break out of how each path contributed to overall weight of each list item.\n");
   printf("     -showInterimIIBreakout Show scoring after clustering & scoring.\n");
   printf("     -noShowBL            Don't show \"Below the Line\" items.\n\n");
   printf("     -doAMLFilter         Antonio's Machine Learning Filtering\n");
   printf("     -doIL2R              Ilya's ML Learning 2 Rank\n");
   printf("     -doNoAddForced       No Filtering or Forced Adds\n");
   printf("     -forceMTIFL          Force MTIFL filtering\n");
   printf("     -forceMTIFLReview    Force MTIFL Review Journal filtering\n");
   printf("--------------------------------------------------------------\n\n");
   printf("     -Special_Output      SMALL writeSpecial\n");
   printf("     -default             remMHs, addCTs, addGEOs, addUSA, noCheckRC, limitTitleOnly doSuppChemUpdate doAgedReview detail - default filtering and display\n");
   printf("     -default_DCMS        remMHs, addCTs, addGEOs, addUSA, showHMs, showETs, noCheckRC, showTUIs, doSuppChemUpdate, doAgedReview\n");
   printf("                          limitTitleOnly, limitPTs, starMHTI,\n");
   printf("                          detail_list, RSfilterTO, and RSfilterALL\n");
   printf("     -opt1_DCMS           default_DCMS (less showTUIs) w/ detail\n");
   printf("     -LOCAL               Use new local version of TexTool - DEFAULT\n");
   printf("     -REMOTE              Use original version of TexTool\n");
   printf("     -doTiming            Echo timing information along with results.\n");
   printf("     -doLogging           Debug logging information.\n");
   printf("     -doSuppChemUpdate    Update recommendations with latest Supplemental Concepts.\n");
   printf("     -testSpecials        Turn off Special cases filtering to see if still working.\n");
   printf("     -RTM_Debug           Debug information for RTM.\n");
   printf("     -RTM_logNF           Log Debug information for RTM.\n");
   printf("     -PRC_Debug           Debug information for PRC path.\n");
   printf("     -HMIIPA_Debug        Debug information for HMIIPA Lookup.\n");
   printf("     -TexToolHost <host>  Which TexTool host to use.\n");
   printf("     -RSfilterTO          Turn on Rank Score Filtering for Title Only items.\n");
   printf("     -RSfilterALL         Turn on Rank Score Filtering for Title and Abstract items.\n");
   printf("     -showRSfilter        Show debug when Rank Score Filtering used.\n");
   printf("     -doSAP               Subheading attachment.\n");
   printf("     -ignoreMHSHDups      Do NOT map MHs to SHs when matched.\n");
   printf("     -idCarboSeq          Identify Carbohydrate Sequences\n");
   printf("     -forceMHs            Force recommendation of specific MHs\n");
} /* print_usage */


void reset_vars()
{
   strictFilter_set = FALSE;
   medFilter_set = FALSE;
   medFilterR_set = FALSE;
   starMHTI_set = FALSE;
   RTM_Debug = FALSE;
   PRC_Debug = FALSE;
   HMIIPA_Debug = FALSE;
   RSfilterTO = FALSE;
   RSfilterALL = FALSE;
   showRSfilter = FALSE;
   addCTs = FALSE;
   addGEOs = FALSE;
   addUSA = FALSE;
   forceNewTerms = FALSE;
   addAddls = FALSE;
   removeRCOnly = FALSE;
   doAgedReview = FALSE;
   showAgedReview = FALSE;
   doMHSHDups = TRUE;
   remMHs = FALSE;
   showTUIs = FALSE;
   showHMs = FALSE;
   showHMsD = FALSE;
   showETs = FALSE;
   showETsD = FALSE;
   showJust = FALSE;
   trackPositional = FALSE;
   noCheckRC = FALSE;
   limitTitleOnly = FALSE;
   limitPTs = FALSE;
   doLogging = FALSE;
   writeTables = FALSE;
   writeSpecial = FALSE;
   showTreecodes = FALSE;
   showDUIs = FALSE;
   noShowBL = FALSE;
   SMALL = FALSE;
   NORMAL = TRUE;
   CATALOGING = FALSE;
   EOT_FLAG = FALSE;
   cutOff15 = FALSE;
   MMIO_Filter = FALSE;
   forceMTIFL = FALSE;
   forceMTIFLReview = FALSE;
   doNoAddForced = FALSE;
   checkForFirstLine = FALSE;
   firstLineReviewJournal = FALSE;
   noMTIFL = FALSE;
   BioASQ = FALSE;
   showCalc = FALSE;

   PW1=7;
   PW2=0;
   PW4=2;
   DIRECT=1.0;
   ATX=1.0;
   GP75=0.90;
   GC75=0.75;
   GS75=0.70;
   OTHER=0.50;
   IM = 1.0;
   NIM = 0.8;
   EMP_HSTAR = 0.0;
   mySEC = 1.0;
   COT_FACTOR=10000;
   TITLE_FACTOR=20;
   TREE_FACTOR=100;
   top_relcit=10;
   max=25;
   topn=25;
   cutOffScore = 0;
   rRCCutOffScore = 10;
   read_MMI = FALSE;
   read_TRG = FALSE;
   read_REL = FALSE;
   LOCAL = TRUE;
   REMOTE = FALSE;
   doTimingFlag = FALSE;
   TTHost_specified = FALSE;
   doSuppChemUpdate = FALSE;
   testSpecials = FALSE;
   showInterimBreakout = FALSE;
   showInterimIIBreakout = FALSE;
   doSAP = FALSE;
   exitNoResult = FALSE;
   useWSD = FALSE;
   useWSDTIOnly = FALSE;
   RTM_logNF = FALSE;
   try_Elink = FALSE;
   idCarboSeq = FALSE;
   forceMHs = FALSE;
   preIndex = FALSE;
   level1Filter = FALSE;
   JSON = FALSE;

   strcpy(mmi_index, "");
   strcpy(rel_index, "");
   strcpy(trg_index, "");
   strcpy(display, "detail");
   strcpy(TexTool_Host, default_TexToolHost);
} /* reset_vars */


void print_vars()
{
   printf("Variables are set accordingly:\n");
   printf("-mmi %.2f  -trg %.2f  -pub %.2f  -topn %d  -dir %.2f  -atx %.2f\n",
         PW1, PW2, PW4, topn, DIRECT, ATX);

   printf("-par %.2f  -chd %.2f  -sib %.2f  -oth %.2f  -emp_hstar %.2f\n", GP75,
         GC75, GS75, OTHER, EMP_HSTAR);

   printf("-sec %.2f  -cit %d  -im %.2f  -nim %.2f  -cot %d  -ti %d  -rel %d\n",
         mySEC, top_relcit, IM, NIM, COT_FACTOR, TITLE_FACTOR, TREE_FACTOR);

   printf("-doTiming %d\n", doTimingFlag);
   printf("Filters: ");
   if(strictFilter_set)
     printf("-strictFilter ");

   if(medFilter_set)
     printf("-medFilter ");

   if(medFilterR_set)
     printf("-medFilterR ");

   if(starMHTI_set)
     printf("-starMHTI ");

   if(addCTs)
     printf("-addCTs ");

   if(remMHs)
     printf("-remMHs ");

   if(showHMs)
     printf("-showHMs ");

   if(showHMsD)
     printf("-showHMsD ");

   if(showETs)
     printf("-showETs ");

   if(addGEOs)
     printf("-addGEOs ");

   if(addUSA)
     printf("-addUSA ");

   if(addAddls)
     printf("-addAddls ");

   if(doAgedReview)
     printf("-doAgedReview ");

   if(showAgedReview)
     printf("-showAgedReview ");

   if(noCheckRC)
     printf("-noCheckRC ");

   if(limitTitleOnly)
     printf("-limitTitleOnly ");

   if(limitPTs)
     printf("-limitPTs ");

   if(RSfilterTO)
     printf("-RSfilterTO ");

   if(RSfilterALL)
     printf("-RSfilterALL ");

   if(showRSfilter)
     printf("-showRSfilter ");

   if(doSuppChemUpdate)
     printf("-doSuppChemUpdate ");

   if(testSpecials)
     printf("-testSpecials ");

   if(showTreecodes)
     printf("-showTreecodes ");

   if(showDUIs)
     printf("-showDUIs ");

   if(showTUIs)
     printf("-showTUIs ");

   if(showInterimBreakout)
     printf("-showInterimBreakout ");

   if(showInterimIIBreakout)
     printf("-showInterimIIBreakout ");

   if(noShowBL)
     printf("-noShowBL ");

   if(doSAP)
     printf("-doSAP ");

   if(useWSD)
     printf("-useWSD ");

   if(useWSDTIOnly)
     printf("-useWSDTIOnly ");

   if(RTM_logNF)
     printf("-RTM_logNF ");

   if(cutOff15)
     printf("-cutOff15 ");

   if(MMIO_Filter)
     printf("-MMIO_Filter ");

   if(idCarboSeq)
     printf("-idCarboSeq ");

   if(forceMHs)
     printf("-forceMHs ");

   printf("\n");

   printf("TexTool Host: %s\n", TexTool_Host);

   printf("Display: %s\n", display); fflush(stdout);
} /* print_vars */
