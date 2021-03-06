/* Setup defaults */

#define localTrue 1
#define localFalse 0

int strictFilter_set = localFalse;
int medFilter_set = localFalse;
int medFilterR_set = localFalse;
int starMHTI_set = localFalse;
int addCTs = localFalse;
int addGEOs = localFalse;
int addUSA = localFalse;
int doAgedReview = localFalse;
int showAgedReview = localFalse;
int doMHSHDups = localTrue;
int RTM_Debug = localFalse;
int PRC_Debug = localFalse;
int HMIIPA_Debug = localFalse;
int RSfilterTO = localFalse;
int RSfilterALL = localFalse;
int showRSfilter = localFalse;
int remMHs = localFalse;
int showHMs = localFalse;
int showHMsD = localFalse;
int showTUIs = localFalse;
int showETs = localFalse;
int ETsOFF = localFalse;
int showETsD = localFalse;
int showJust = localFalse;
int noCheckRC = localFalse;
int limitTitleOnly = localFalse;
int limitPTs = localFalse;
int writeTables = localFalse;
int writeSpecial = localFalse;
int NORMAL = localTrue;
int CATALOGING = localFalse;
int EOT_FLAG = localFalse;
int SMALL = localFalse;
int LOCAL = localTrue;
int REMOTE = localFalse;
int Title_Only = localFalse;
int Special_PT = -1;
int doLogging = localFalse;
int doSuppChemUpdate = localFalse;
int testSpecials = localFalse;
int showTreecodes = localFalse;
int doTimingFlag = localFalse;
int showDUIs = localFalse;
int showInterimBreakout = localFalse;
int showInterimIIBreakout = localFalse;
int noShowBL = localFalse;
int addAddls = localFalse;
int removeRCOnly = localFalse;
int noAnswer = localFalse;
long numMMILines = 0;
int lTO_II = localFalse;
int try_Elink = localFalse;
int checkNoTerms = localFalse;
int doSAP = localFalse;
int exitNoResult = localFalse;
int useWSD = localFalse;
int useWSDTIOnly = localFalse;
int RTM_logNF = localFalse;
int MMIO_Filter = localFalse;
int haveHMD = localFalse;
int forceNewTerms = localFalse;
int idCarboSeq = localFalse;
int forceMHs = localFalse;
int cutOff15 = localFalse;
int checkForFirstLine = localFalse;
int doAMLFilter = localFalse;
int trackPositional = localFalse;
int doNoAddForced = localFalse;
int firstLineReviewJournal = localFalse;
int forceMTIFL = localFalse;
int forceMTIFLReview = localFalse;
int noMTIFL = localFalse;
int BioASQ = localFalse;
int BioASQII = localFalse;
int showCalc = localFalse;
int showMeSHVersion = localFalse;
int showDisclaimer = localFalse;
int showType = localFalse;
int preIndex = localFalse;
int MeSHonDemand = localFalse;
int MeSHonDemand2 = localFalse;
int MoD_Raw = localFalse;
int MoD_PP = localFalse;
int OLDMEDLINE = localFalse;
int level1Filter = localFalse;
int showRemovals = localFalse;
int JSON = localFalse;
int showScores = localFalse;
int showScoresFull = localFalse;
int showIlyaML = localFalse;
int MTI_AUTO = localFalse;
int doIL2R = localFalse;
int goHigh = localFalse;
int doIL2RHybrid = localFalse;
int forceCHJ = localFalse;
int forceOK_Title = localFalse;
int ACCME = localFalse;
int MTI_REVIEW = localFalse;
int forceCON = localFalse;
int haveFullText = localFalse;
int doLookAtConf = localFalse;
int look4PTs = localFalse;
int foundNonHumanTrigger = localFalse;

float PW1=7.0, PW4=2.0, PW2=0.0;
float DIRECT=1.0, ATX=1.0, GP75=0.90, GC75=0.75, GS75=0.70, OTHER=0.50,
      IM = 1.0, NIM = 0.8, EMP_HSTAR = 0.0;
float mySEC = 1.0;
int COT_FACTOR=10000, TITLE_FACTOR=20, TREE_FACTOR=100;

int top_relcit=10, max=25;
int topn=25;
int cutOffScore = -1;
int rRCCutOffScore = 10;
int secondMM_Cutoff = 75;

char display[51] = "detail";
int hasCON = localFalse;
int hasEFR = localFalse;
int hasPROF = localFalse;
int hasRPF = localFalse;
int hasROF = localFalse;
int hasUOF = localFalse;

/*
int numTTHosts = 5;
char *texToolHosts[] = { "ii-server1", "ii-server4", "ii-server5", "ii-server11", "ii-server13" };

int numTTHosts = 4;
char *texToolHosts[] = { "ii-server4", "ii-server5", "ii-server11", "ii-server13" };
*/
int numTTHosts = 5;
char *texToolHosts[] = { "ii-server1", "ii-server4", "ii-server5", "ii-server11", "ii-server13" };
