#include <db.h>
#include "M_terms.h"
#include "myLocal.h"

#define D_S_SUCCESS 0

void initNopen_btrees(int doCatalog);
void initLocals(char *local_MMI, char *local_TRG, char *local_REL);
int query_btrees();
void close_btrees();
void init_db(char *database, int pos);


/*----------------------------
%GLOBAL VARIABLES
----------------------------*/
static DB *db_COT_DUI = NULL;
static DB *db_CUIST = NULL;
static DB *db_MHCUI = NULL;
static DB *db_MHTUI = NULL;
static DB *db_MN = NULL;
static DB *db_MUICUI = NULL;
static DB *db_REL_DUI = NULL;
static DB *db_RTM = NULL;
static DB *db_MHENTRY = NULL;
static DB *db_ISSN = NULL;
static DB *db_SPMODS = NULL;
static DB *db_MHDUI = NULL;
static DB *db_RTM_CAT = NULL;
static DB *db_VMHSH = NULL;
static DB *db_CATREPL = NULL;
static DB *db_HMIIPA = NULL;
static DB *db_GEODB = NULL;
static DB *db_MMIOFR = NULL;
static DB *db_PRCOFR = NULL;
static DB *db_ETDUI = NULL;
static DB *db_DUICUIMH = NULL;
static DB *db_PMID_TI = NULL;
static DB *db_PMID_MH = NULL;
static DB *db_QUI_SH = NULL;
static DB *db_MHDUICUI = NULL;
static DB *db_SH_QUI = NULL;
static DB *db_VOCABDEN = NULL;
static DB *db_VOCABTOP = NULL;
static DB *db_L2R_MHDUICUI = NULL;
static DB *db_MRCOC = NULL;
static DB *db_JINFO = NULL;
static DB *db_DUI_MN = NULL;
static DB *db_SCRCLASS = NULL;

static DB *db_Local_MMI = NULL;
static DB *db_Local_TRG = NULL;
static DB *db_Local_REL = NULL;


void initNopen_btrees(int doCatalog)
{
   char index_file[MAXLINE];
   int errno, ret;

   /* -----------------------   MRCOC/COT_DUI  -------------------------- */

   sprintf(index_file, "%s/cotf_dui_index\0", database_home);
   init_db(index_file, COT_DUI);

   /* -----------------------   MTREE/MN  -------------------------- */

   sprintf(index_file, "%s/cui_mn_index\0", database_home);
   init_db(index_file, MN);

   /* -----------------------   CUIST/CUIST  -------------------------- */

   sprintf(index_file, "%s/cuist_index\0", database_home);
   init_db(index_file, CUIST);

   /* -----------------------   MH_ENTRY_TERMS  ----------------------- */

   sprintf(index_file, "%s/mh_entry_index\0", database_home);
   init_db(index_file, MHENTRY);

   /* -----------------------   MHCUI/MHCUI  -------------------------- */

   sprintf(index_file, "%s/mhcui_index\0", database_home);
   init_db(index_file, MHCUI);

   /* -----------------------   MHTUI/MHTUI  -------------------------- */

   sprintf(index_file, "%s/mhtui_index\0", database_home);
   init_db(index_file, MHTUI);

   /* -----------------------   MRREL/REL  -------------------------- */

   sprintf(index_file, "%s/mrrel_dui_index\0", database_home);
   init_db(index_file, REL_DUI);

   /* -----------------------   MUICUI/MUICUI  -------------------------- */

   sprintf(index_file, "%s/muicui_index\0", database_home);
   init_db(index_file, MUICUI);

   /* ----------------------------   SPMODS  -------------------------- */

   sprintf(index_file, "%s/spmods_index\0", database_home);
   init_db(index_file, SPMODS);

   /* -------------------------   MHDUI_TERMS  -------------------------- */

   sprintf(index_file, "%s/mhdui_index\0", database_home);
   init_db(index_file, MHDUI);

   /* -------------------------   SCR_HMIIPAs  -------------------------- */

   sprintf(index_file, "%s/SCR_HMIIPA_index\0", database_home);
   init_db(index_file, HMIIPA);

   /* -------------------------   GEOs  -------------------------- */

   sprintf(index_file, "%s/GEO_index\0", database_home);
   init_db(index_file, GEODBS);

   /* -----------------------   MMI Only Filtering  ------------------------ */

   sprintf(index_file, "%s/mmiFilter_index\0", database_home);
   init_db(index_file, MMIOFR);

   /* -----------------------   PRC Only Filtering  ------------------------ */

   sprintf(index_file, "%s/prcFilter_index\0", database_home);
   init_db(index_file, PRCOFR);

   /* -----------------------   Entry Term DUI MHs  ------------------------ */

   sprintf(index_file, "%s/ET_DUI_index\0", database_home);
   init_db(index_file, ETDUI);

   /* ---------------------------   DUICUIMHs  ----------------------------- */

   sprintf(index_file, "%s/duicuimh_index\0", database_home);
   init_db(index_file, DUICUIMH);

   /* -----------------------------   QUI|SH  ------------------------------ */

   sprintf(index_file, "%s/qui_sh_index\0", database_home);
   init_db(index_file, QUI_SH);

   /* -----------------------   PMID|TI  -------------------------- */

   sprintf(index_file, "%s/PMID_TI_index\0", database_home);
   init_db(index_file, PMID_TI);

   /* ----------------------------   PMID_MHs  ----------------------------- */

   sprintf(index_file, "%s/PMID_MH_index\0", database_home);
   init_db(index_file, PMID_MH);

   /* -----------------------------   SH|QUI  ------------------------------ */

   sprintf(index_file, "%s/sh_qui_index\0", database_home);
   init_db(index_file, SH_QUI);

   /* -----------------------   Vocabulary Density  ------------------------ */

   sprintf(index_file, "%s/vocabDensity_index\0", database_home);
   init_db(index_file, VOCABDEN);

   /* ----------------------  Top Vocabulary Density  ---------------------- */

   sprintf(index_file, "%s/vocabTop_index\0", database_home);
   init_db(index_file, VOCABTOP);

   /* ---------------------------   MHDUICUIs  ----------------------------- */

   sprintf(index_file, "%s/mhduicui_index\0", database_home);
   init_db(index_file, MHDUICUI);

   /* ---------------------------  L2R_MHDUICUIs  -------------------------- */

   sprintf(index_file, "%s/L2R_mhduicui_index\0", database_home);
   init_db(index_file, L2R_MHDUICUI);

   /* ---------------------------  MRCOC  -------------------------- */

   sprintf(index_file, "%s/mrcoc_index\0", database_home);
   init_db(index_file, MRCOC);

   /* ---------------------------  JINFO  -------------------------- */

   sprintf(index_file, "%s/JINFO_index\0", database_home);
   init_db(index_file, JINFO);

   /* -----------------------   MTREE/MN by DUI -------------------------- */

   sprintf(index_file, "%s/dui_mn_index\0", database_home);
   init_db(index_file, DUI_MN);

   /* -------------------------   SCRCLASS  --------------------------- */

   sprintf(index_file, "%s/SCRclasses_index\0", database_home);
   init_db(index_file, SCRCLASS);

   if(doCatalog)
   {
      /* -------------------------   CATALOG RTM  -------------------------- */

      sprintf(index_file, "%s/umls_rtmCatalog_index\0", database_home);
      init_db(index_file, RTM_CAT);

      /* -------------------------   CATALOG VMHSH  ------------------------ */

      sprintf(index_file, "%s/validMHSH_index\0", database_home);
      init_db(index_file, VMHSH);

      /* ----------------------   CATALOG MH/SH REPL  ---------------------- */

      sprintf(index_file, "%s/CatECs_index\0", database_home);
      init_db(index_file, CATREPL);
   } /* fi */

   else
   {
      /* -----------------------   RESTRMESH/RTM  -------------------------- */

      sprintf(index_file, "%s/umls_rtm_index\0", database_home);
      init_db(index_file, RTM);
   } /* else */

   /* -------------------------   ISSN_TERMS  ------------------------- */
/*
   sprintf(index_file, "%s/issn_index\0", database_home);
   init_db(index_file, ISSN);
*/
} /*** End initNopen_btrees */


void initLocals(char *local_MMI, char *local_TRG, char *local_REL)
{
   char index_file[MAXLINE];
   int errno, ret;

   /* ------------------------   Local MMI  --------------------------- */

   if(strlen(local_MMI) > 0)
   {
      strcpy(index_file, local_MMI);
      init_db(index_file, LMMI);
   } /* fi */

   /* ------------------------   Local TRG  --------------------------- */

   if(strlen(local_TRG) > 0)
   {
      strcpy(index_file, local_TRG);
      init_db(index_file, LTRG);
   } /* fi */

   /* ------------------------   Local REL  --------------------------- */

   if(strlen(local_REL) > 0)
   {
      strcpy(index_file, local_REL);
      init_db(index_file, LREL);
   } /* fi */
} /* initLocals */


void init_db(char *database, int pos)
{
   int errno, ret;

   ret = 0; errno = 0;

   if(pos == COT_DUI)
   {
      if((ret = db_create(&db_COT_DUI, NULL, 0)) == 0)
         errno = db_COT_DUI->open(db_COT_DUI, NULL, database, NULL, DB_BTREE, 
                             DB_RDONLY, 0600);
   } /* fi */

   else if (pos == CUIST)
   {
      if((ret = db_create(&db_CUIST, NULL, 0)) == 0)
         errno = db_CUIST->open(db_CUIST, NULL, database, NULL, DB_BTREE, 
                             DB_RDONLY, 0600);
   } /* else fi */

   else if (pos == MHCUI)
   {
      if((ret = db_create(&db_MHCUI, NULL, 0)) == 0)
         errno = db_MHCUI->open(db_MHCUI, NULL, database, NULL, DB_BTREE, 
                             DB_RDONLY, 0600);
   } /* else fi */

   else if (pos == MHTUI)
   {
      if((ret = db_create(&db_MHTUI, NULL, 0)) == 0)
         errno = db_MHTUI->open(db_MHTUI, NULL, database, NULL, DB_BTREE, 
                             DB_RDONLY, 0600);
   } /* else fi */

   else if (pos == MN)
   {
      if((ret = db_create(&db_MN, NULL, 0)) == 0)
         errno = db_MN->open(db_MN, NULL, database, NULL, DB_BTREE, 
                             DB_RDONLY, 0600);
   } /* else fi */

   else if (pos == MUICUI)
   {
      if((ret = db_create(&db_MUICUI, NULL, 0)) == 0)
         errno = db_MUICUI->open(db_MUICUI, NULL, database, NULL, DB_BTREE, 
                             DB_RDONLY, 0600);
   } /* else fi */

   else if (pos == REL_DUI)
   {
      if((ret = db_create(&db_REL_DUI, NULL, 0)) == 0)
         errno = db_REL_DUI->open(db_REL_DUI, NULL, database, NULL, DB_BTREE, 
                             DB_RDONLY, 0600);
   } /* else fi */

   else if (pos == RTM)
   {
      if((ret = db_create(&db_RTM, NULL, 0)) == 0)
         errno = db_RTM->open(db_RTM, NULL, database, NULL, DB_BTREE, 
                             DB_RDONLY, 0600);
   } /* else fi */

   else if (pos == MHENTRY)
   {
      if((ret = db_create(&db_MHENTRY, NULL, 0)) == 0)
         errno = db_MHENTRY->open(db_MHENTRY, NULL, database, NULL, DB_BTREE, 
                             DB_RDONLY, 0600);
   } /* else fi */

   else if (pos == SPMODS)
   {
      if((ret = db_create(&db_SPMODS, NULL, 0)) == 0)
         errno = db_SPMODS->open(db_SPMODS, NULL, database, NULL, DB_BTREE, 
                             DB_RDONLY, 0600);
   } /* else fi */

   else if (pos == ISSN)
   {
      if((ret = db_create(&db_ISSN, NULL, 0)) == 0)
         errno = db_ISSN->open(db_ISSN, NULL, database, NULL, DB_BTREE, 
                             DB_RDONLY, 0600);
   } /* else fi */

   else if (pos == MHDUI)
   {
      if((ret = db_create(&db_MHDUI, NULL, 0)) == 0)
         errno = db_MHDUI->open(db_MHDUI, NULL, database, NULL, DB_BTREE, 
                             DB_RDONLY, 0600);
   } /* else fi */

   else if (pos == HMIIPA)
   {
      if((ret = db_create(&db_HMIIPA, NULL, 0)) == 0)
         errno = db_HMIIPA->open(db_HMIIPA, NULL, database, NULL, DB_BTREE, 
                             DB_RDONLY, 0600);
   } /* else fi */

   else if (pos == GEODBS)
   {
      if((ret = db_create(&db_GEODB, NULL, 0)) == 0)
         errno = db_GEODB->open(db_GEODB, NULL, database, NULL, DB_BTREE, 
                             DB_RDONLY, 0600);
   } /* else fi */

   else if (pos == MMIOFR)
   {
      if((ret = db_create(&db_MMIOFR, NULL, 0)) == 0)
         errno = db_MMIOFR->open(db_MMIOFR, NULL, database, NULL, DB_BTREE, 
                             DB_RDONLY, 0600);
   } /* else fi */

   else if (pos == PRCOFR)
   {
      if((ret = db_create(&db_PRCOFR, NULL, 0)) == 0)
         errno = db_PRCOFR->open(db_PRCOFR, NULL, database, NULL, DB_BTREE, 
                             DB_RDONLY, 0600);
   } /* else fi */

   else if (pos == ETDUI)
   {
      if((ret = db_create(&db_ETDUI, NULL, 0)) == 0)
         errno = db_ETDUI->open(db_ETDUI, NULL, database, NULL, DB_BTREE, 
                             DB_RDONLY, 0600);
   } /* else fi */

   else if (pos == DUICUIMH)
   {
      if((ret = db_create(&db_DUICUIMH, NULL, 0)) == 0)
         errno = db_DUICUIMH->open(db_DUICUIMH, NULL, database, NULL, DB_BTREE, 
                             DB_RDONLY, 0600);
   } /* else fi */

   else if (pos == QUI_SH)
   {
      if((ret = db_create(&db_QUI_SH, NULL, 0)) == 0)
         errno = db_QUI_SH->open(db_QUI_SH, NULL, database, NULL, DB_BTREE, 
                             DB_RDONLY, 0600);
   } /* else fi */

   else if (pos == PMID_TI)
   {
      if((ret = db_create(&db_PMID_TI, NULL, 0)) == 0)
         errno = db_PMID_TI->open(db_PMID_TI, NULL, database, NULL, DB_BTREE, 
                             DB_RDONLY, 0600);
   } /* else fi */

   else if (pos == PMID_MH)
   {
      if((ret = db_create(&db_PMID_MH, NULL, 0)) == 0)
         errno = db_PMID_MH->open(db_PMID_MH, NULL, database, NULL, DB_BTREE, 
                             DB_RDONLY, 0600);
   } /* else fi */

   else if (pos == MHDUICUI)
   {
      if((ret = db_create(&db_MHDUICUI, NULL, 0)) == 0)
         errno = db_MHDUICUI->open(db_MHDUICUI, NULL, database, NULL, DB_BTREE, 
                             DB_RDONLY, 0600);
   } /* else fi */

   else if (pos == L2R_MHDUICUI)
   {
      if((ret = db_create(&db_L2R_MHDUICUI, NULL, 0)) == 0)
         errno = db_L2R_MHDUICUI->open(db_L2R_MHDUICUI, NULL,
                             database, NULL, DB_BTREE, DB_RDONLY, 0600);
   } /* else fi */

   else if (pos == MRCOC)
   {
      if((ret = db_create(&db_MRCOC, NULL, 0)) == 0)
         errno = db_MRCOC->open(db_MRCOC, NULL,
                             database, NULL, DB_BTREE, DB_RDONLY, 0600);
   } /* else fi */

   else if (pos == JINFO)
   {
      if((ret = db_create(&db_JINFO, NULL, 0)) == 0)
         errno = db_JINFO->open(db_JINFO, NULL,
                             database, NULL, DB_BTREE, DB_RDONLY, 0600);
   } /* else fi */

   else if (pos == DUI_MN)
   {
      if((ret = db_create(&db_DUI_MN, NULL, 0)) == 0)
         errno = db_DUI_MN->open(db_DUI_MN, NULL,
                             database, NULL, DB_BTREE, DB_RDONLY, 0600);
   } /* else fi */

   else if (pos == SCRCLASS)
   {
      if((ret = db_create(&db_SCRCLASS, NULL, 0)) == 0)
         errno = db_SCRCLASS->open(db_SCRCLASS, NULL, database, NULL, DB_BTREE, 
                             DB_RDONLY, 0600);
   } /* else fi */

   else if (pos == SH_QUI)
   {
      if((ret = db_create(&db_SH_QUI, NULL, 0)) == 0)
         errno = db_SH_QUI->open(db_SH_QUI, NULL, database, NULL, DB_BTREE, 
                             DB_RDONLY, 0600);
   } /* else fi */

   else if (pos == VOCABDEN)
   {
      if((ret = db_create(&db_VOCABDEN, NULL, 0)) == 0)
         errno = db_VOCABDEN->open(db_VOCABDEN, NULL, database, NULL, DB_BTREE, 
                             DB_RDONLY, 0600);
   } /* else fi */

   else if (pos == VOCABTOP)
   {
      if((ret = db_create(&db_VOCABTOP, NULL, 0)) == 0)
         errno = db_VOCABTOP->open(db_VOCABTOP, NULL, database, NULL, DB_BTREE, 
                             DB_RDONLY, 0600);
   } /* else fi */

   else if (pos == RTM_CAT)
   {
      if((ret = db_create(&db_RTM_CAT, NULL, 0)) == 0)
         errno = db_RTM_CAT->open(db_RTM_CAT, NULL, database, NULL, DB_BTREE, 
                             DB_RDONLY, 0600);
   } /* else fi */

   else if (pos == VMHSH)
   {
      if((ret = db_create(&db_VMHSH, NULL, 0)) == 0)
         errno = db_VMHSH->open(db_VMHSH, NULL, database, NULL, DB_BTREE, 
                             DB_RDONLY, 0600);
   } /* else fi */

   else if (pos == CATREPL)
   {
      if((ret = db_create(&db_CATREPL, NULL, 0)) == 0)
         errno = db_CATREPL->open(db_CATREPL, NULL, database, NULL, DB_BTREE, 
                             DB_RDONLY, 0600);
   } /* else fi */

   else if (pos == LMMI)
   {
      if((ret = db_create(&db_Local_MMI, NULL, 0)) == 0)
         errno = db_Local_MMI->open(db_Local_MMI, NULL, database, NULL,
                                    DB_BTREE, DB_RDONLY, 0600);
   } /* else fi */

   else if (pos == LTRG)
   {
      if((ret = db_create(&db_Local_TRG, NULL, 0)) == 0)
         errno = db_Local_TRG->open(db_Local_TRG, NULL, database, NULL,
                                    DB_BTREE, DB_RDONLY, 0600);
   } /* else fi */

   else if (pos == LREL)
   {
      if((ret = db_create(&db_Local_REL, NULL, 0)) == 0)
         errno = db_Local_REL->open(db_Local_REL, NULL, database, NULL,
                                    DB_BTREE, DB_RDONLY, 0600);
   } /* else fi */

   if(errno || ret) 
   {
      (void)fprintf(stderr, "db_open problem ---> %s\n", database);
      exit(1);
   } /* fi */
} /* init_db */



int query_btree(int which_DB,           /* Input  */
                char  *query,           /* Input  */
                char ***q_results,      /* Output */
                int   *numberOfResults) /* Output */
{

  int return_code = D_S_SUCCESS;
  int i = 0;
  DBT data, key;
  char **results;
  DBC *dbcp;
  int errno, status;
  DB *db = NULL;
  db_recno_t count;
  
  if(which_DB == COT_DUI)
    db = db_COT_DUI;

  else if(which_DB == CUIST)
    db = db_CUIST;

  else if(which_DB == MHCUI)
    db = db_MHCUI;

  else if(which_DB == MHTUI)
    db = db_MHTUI;

  else if(which_DB == MN)
    db = db_MN;

  else if(which_DB == MUICUI)
    db = db_MUICUI;

  else if(which_DB == REL_DUI)
    db = db_REL_DUI;

  else if(which_DB == RTM)
    db = db_RTM;

  else if(which_DB == MHENTRY)
    db = db_MHENTRY;

  else if(which_DB == ISSN)
    db = db_ISSN;

  else if(which_DB == SPMODS)
    db = db_SPMODS;

  else if(which_DB == MHDUI)
    db = db_MHDUI;

  else if(which_DB == HMIIPA)
    db = db_HMIIPA;

  else if(which_DB == GEODBS)
    db = db_GEODB;

  else if(which_DB == MMIOFR)
    db = db_MMIOFR;

  else if(which_DB == PRCOFR)
    db = db_PRCOFR;

  else if(which_DB == ETDUI)
    db = db_ETDUI;

  else if(which_DB == DUICUIMH)
    db = db_DUICUIMH;

  else if(which_DB == QUI_SH)
    db = db_QUI_SH;

  else if(which_DB == PMID_TI)
    db = db_PMID_TI;

  else if(which_DB == PMID_MH)
    db = db_PMID_MH;

  else if(which_DB == MHDUICUI)
    db = db_MHDUICUI;

  else if(which_DB == L2R_MHDUICUI)
    db = db_L2R_MHDUICUI;

  else if(which_DB == MRCOC)
    db = db_MRCOC;

  else if(which_DB == JINFO)
    db = db_JINFO;

  else if(which_DB == DUI_MN)
    db = db_DUI_MN;

  else if(which_DB == SCRCLASS)
    db = db_SCRCLASS;

  else if(which_DB == SH_QUI)
    db = db_SH_QUI;

  else if(which_DB == VOCABDEN)
    db = db_VOCABDEN;

  else if(which_DB == VOCABTOP)
    db = db_VOCABTOP;

  else if(which_DB == RTM_CAT)
    db = db_RTM_CAT;

  else if(which_DB == VMHSH)
    db = db_VMHSH;

  else if(which_DB == CATREPL)
    db = db_CATREPL;

  else if(which_DB == LMMI)
    db = db_Local_MMI;

  else if(which_DB == LTRG)
    db = db_Local_TRG;

  else if(which_DB == LREL)
    db = db_Local_REL;

  else
  {
     fprintf(stderr, "Inappropriate which_DB specified: %d\n", which_DB);
     fflush(stderr);
     exit(1);
  } /* else */
   
  results =(char **)malloc( sizeof( char *) * 1024 );/* assume no more than  */
                                                     /* 1K hits per query    */
           
  memset(&key, 0, sizeof(key));
  memset(&data, 0, sizeof(data));

  /* Acquire a cursor for the database. */

  if((errno = db->cursor(db, NULL, &dbcp, 0)) == 0) 
  {
     key.data  = query ;
     key.size  = sizeof(char) * strlen(query) + 1;

     /* Set the cursor to the position of the keys we have & then
        grab the count of items available to us.  If we have dup
        keys (which we allow) the count can be > 1.
     */

     status = dbcp->get(dbcp, &key, &data, DB_SET);

     if(status == 0)
       dbcp->count(dbcp, &count, 0);
     else
       count = -1;

     i = 0;
     while((status == 0) && (i < 1024) && (i < count))
     {
         results[i] = (char *)malloc(strlen(data.data) + 1);
         strcpy(results[i], data.data);
         i++;

         status = dbcp->get(dbcp, &key, &data, DB_NEXT);
     } /* while */

     dbcp->close(dbcp); 

     *numberOfResults = i;
     *q_results = results;
   } /* fi */
   else
   {
      fprintf(stderr, "ERROR: db->cursor failed: %d\n", errno);
      fflush(stderr);
      exit(1);
   } /* else */

   return (return_code);
} /*** End query_btree */



void close_btrees()
{
   if (db_COT_DUI != NULL) 
     db_COT_DUI->close(db_COT_DUI, 0);

   if (db_CUIST != NULL) 
     db_CUIST->close(db_CUIST, 0);

   if (db_MHCUI != NULL) 
     db_MHCUI->close(db_MHCUI, 0);

   if (db_MHTUI != NULL) 
     db_MHTUI->close(db_MHTUI, 0);

   if (db_MN != NULL) 
     db_MN->close(db_MN, 0);

   if (db_MUICUI != NULL) 
     db_MUICUI->close(db_MUICUI, 0);

   if (db_REL_DUI != NULL) 
     db_REL_DUI->close(db_REL_DUI, 0);

   if (db_RTM != NULL) 
     db_RTM->close(db_RTM, 0);

   if (db_MHENTRY != NULL) 
     db_MHENTRY->close(db_MHENTRY, 0);

   if (db_ISSN != NULL) 
     db_ISSN->close(db_ISSN, 0);

   if (db_SPMODS != NULL) 
     db_SPMODS->close(db_SPMODS, 0);

   if (db_MHDUI != NULL) 
     db_MHDUI->close(db_MHDUI, 0);

   if (db_HMIIPA != NULL) 
     db_HMIIPA->close(db_HMIIPA, 0);

   if (db_GEODB != NULL) 
     db_GEODB->close(db_GEODB, 0);

   if (db_MMIOFR != NULL) 
     db_MMIOFR->close(db_MMIOFR, 0);

   if (db_PRCOFR != NULL) 
     db_PRCOFR->close(db_PRCOFR, 0);

   if (db_ETDUI != NULL) 
     db_ETDUI->close(db_ETDUI, 0);

   if (db_DUICUIMH != NULL) 
     db_DUICUIMH->close(db_DUICUIMH, 0);

   if (db_QUI_SH != NULL) 
     db_QUI_SH->close(db_QUI_SH, 0);

   if (db_PMID_TI != NULL) 
     db_PMID_TI->close(db_PMID_TI, 0);

   if (db_PMID_MH != NULL) 
     db_PMID_MH->close(db_PMID_MH, 0);

   if (db_MHDUICUI != NULL) 
     db_MHDUICUI->close(db_MHDUICUI, 0);

   if (db_L2R_MHDUICUI != NULL) 
     db_L2R_MHDUICUI->close(db_L2R_MHDUICUI, 0);

   if (db_MRCOC != NULL) 
     db_MRCOC->close(db_MRCOC, 0);

   if (db_JINFO != NULL) 
     db_JINFO->close(db_JINFO, 0);

   if (db_DUI_MN != NULL) 
     db_DUI_MN->close(db_DUI_MN, 0);

   if (db_SCRCLASS != NULL) 
     db_SCRCLASS->close(db_SCRCLASS, 0);

   if (db_SH_QUI != NULL) 
     db_SH_QUI->close(db_SH_QUI, 0);

   if (db_VOCABDEN != NULL) 
     db_VOCABDEN->close(db_VOCABDEN, 0);

   if (db_VOCABTOP != NULL) 
     db_VOCABTOP->close(db_VOCABTOP, 0);

   if (db_RTM_CAT != NULL) 
     db_RTM_CAT->close(db_RTM_CAT, 0);

   if (db_VMHSH != NULL) 
     db_VMHSH->close(db_VMHSH, 0);

   if (db_CATREPL != NULL) 
     db_CATREPL->close(db_CATREPL, 0);

   if (db_Local_MMI != NULL) 
     db_Local_MMI->close(db_Local_MMI, 0);

   if (db_Local_TRG != NULL) 
     db_Local_TRG->close(db_Local_TRG, 0);

   if (db_Local_REL != NULL) 
     db_Local_REL->close(db_Local_REL, 0);
} /*** End close_btrees */
