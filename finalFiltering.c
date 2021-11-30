#include "M_terms.h"
#include "gen_spec_list.h"

#define MAXTREEs 15

extern long numFinalCTs;
extern struct finalStruct finalCTlist[MAX_FINAL_CTs];

extern long numFinalSHs;
extern struct finalStruct finalSHlist[MAX_FINAL_SHs];

extern long numFinalMHs;
extern struct finalStruct finalMHlist[MAX_FINAL_MHs];

extern int RTM_Debug;
extern FILE *fout;
extern int MTI_REVIEW, MTI_AUTO;

extern int amIAnimalCT(char *mh);
extern int foundInText(char *lookFor, int caseSensitive, int TitleOnly);

struct bdStruct {
   char *part;
   char *DUI;
   char *tripMH;
   int numTrees;
   char *tripTrees[MAXTREEs];
};

void filterPreIndex(int isLevelOne);
void filterML(int flag);
void filterFluorescence();
void filterGenMiceRatHamster();
void filterOLDMEDLINE();
void filterMaleFemale();
void filterHeuristicN();
void filterMentalDisorders();
void filterGenSpec();
extern int query_btree();
int isDiseaseSCR(char *name);
void checkCTRemove(char *mh);
void checkFDOC();
int isModelOrganism(char *dui);
int isFDOC_SCR(char *name);

int numBD = 37;
struct bdStruct BodyPartsDis[37] = {
   { "Aorta, Thoracic", "D001013", "Aortic Aneurysm, Thoracic", 2,
            { "C14.907.055.239.125", "C14.907.109.139.125" } },
   { "Cervix Uteri", "D002584", "Uterine Diseases", 1, { "C13.351.500.852" } },
   { "Colon", "D003106", "Colonic Diseases", 1, { "C06.405.469.158" } },
   { "Coronary Vessels", "D003331", "Coronary Artery Disease", 3,
            { "C14.280.647.250.260", "C14.907.137.126.339",
              "C14.907.585.250.260" } },
   { "Coronary Vessels", "D003331", "Coronary Vessel Anomalies", 3,
            { "C14.240.400.210", "C14.280.400.210", "C16.131.240.400.210" } },
   { "Gallbladder", "D005704", "Gallbladder Diseases", 1, { "C06.130.564" } },
   { "Liver", "D008099", "Liver Diseases", 1, { "C06.552" } },
   { "Nervous System", "D009420", "Nervous System Diseases", 1, { "C10" } },
   { "Ovary", "D010053", "Ovarian Diseases", 2,
            { "C13.351.500.056.630", "C19.391.630" } },
   { "Prostate", "D011467", "Prostatic Diseases", 1, { "C12.294.565" } },
   { "Spleen", "D013154", "Splenic Diseases", 1, { "C15.604.744" } },
   { "Testis", "D013737", "Testicular Diseases", 2,
            { "C12.294.829", "C19.391.829" } },
   { "Thyroid Gland", "D013961", "Thyroid Diseases", 1, { "C19.874" } },
   { "Urinary Bladder", "D001743", "Urinary Bladder Diseases", 2,
            { "C12.777.829", "C13.351.968.829" } },
   { "Arteries", "D013961", "Vascular Diseases", 1, { "C14.907" } },
   { "Basal Ganglia", "D013961", "Basal Ganglia Diseases", 1,
            { "C10.228.140.079" } },
   { "Brain Stem", "D001933", "Brain Stem Neoplasms", 3,
            { "C04.588.614.250.195.411.100", "C10.228.140.211.500.100",
              "C10.551.240.250.400.200" } },
   { "Brain Stem", "D001933", "Brain Stem Hemorrhage, Traumatic", 8,
            { "C10.228.140.199.275.200", "C10.228.140.300.535.450.200.500",
              "C10.900.300.087.187.200", "C10.900.300.837.150.300",
              "C14.907.253.573.400.150.200", "C26.260.118.175.150",
              "C26.915.300.200.175.200", "C26.915.300.490.150.200" } },
   { "Esophagus", "D004947", "Esophageal Diseases", 1, { "C06.405.117" } },
   { "Gastrointestinal Tract", "D041981", "Gastrointestinal Diseases", 1,
            { "C06.405" } },
   { "Germ Cells", "D005854", "Neoplasms, Germ Cell and Embryonal", 1,
            { "C04.557.465" } },
   { "Leukocytes", "D007962", "HLA Antigens", 2,
            { "D23.050.301.500.450", "D23.050.705.552.450" } },
   { "Lymph", "D008196", "Lymph Nodes", 2,
            { "A10.549.400", "A15.382.520.604.412" } },
   { "Lymph", "D008196", "Lymph Node Excision", 1, { "E04.446" } },
   { "Macrophages", "D008264", "Macrophage Migration-Inhibitory Factors", 4,
            { "D12.644.276.374.480.625", "D12.776.467.374.480.625",
              "D23.125.477.500", "D23.529.374.480.625" } },
   { "Mouth", "D009055", "Mouth Diseases", 1, { "C07.465" } },
   { "Oropharynx", "D009960", "Pharyngitis", 3,
            { "C07.550.781", "C08.730.561", "C09.775.649" } },
   { "Oropharynx", "D009960", "Oropharyngeal Neoplasms", 4,
            { "C04.588.443.665.710.684", "C07.550.745.671",
              "C09.647.710.685", "C09.775.549.685" } },
   { "Palate", "D010159", "Cleft Palate", 9, 
            { "C05.500.460.185", "C05.660.207.540.460.185", "C07.320.440.185",
              "C07.465.525.185", "C07.650.500.460.185", "C07.650.525.185",
              "C16.131.621.207.540.460.185", "C16.131.850.500.460.185",
              "C16.131.850.525.185" } },
   { "Spinal Cord", "D013116", "Spinal Cord Diseases", 1, { "C10.228.854" } },
   { "Stem Cells", "D013234", "Stem Cell Transplantation", 1,
            { "E04.936.225.687" } },
   { "Stem Cells", "D013234", "Stem Cell Research", 2,
            { "E05.242.950", "H01.770.644.145.512" } },
   { "Stomach", "D013270", "Stomach Diseases", 1, { "C06.405.748" } },
   { "Urinary Tract", "D014551", "Urologic Diseases", 2,
            { "C12.777", "C13.351.968" } },
   { "Uterus", "D014599", "Uterine Diseases", 1, { "C13.351.500.852" } },
   { "Vagina", "D014621", "Vaginal Diseases", 1, { "C13.351.500.894" } },
   { "Vulva", "D014844", "Vulvar Diseases", 1, { "C13.351.500.944" } }
};


/***************************************************************************
*
*  filterBodyPartDisease --
*
*      This 
*
***************************************************************************/

void filterBodyPartDisease()
{
    int j, k, l, found, foundTrig;
    long i, foundPos;

    /* Use body part and disease/s list from Susan email to remove the
       body part.
    */

    for(j = 0; j < numBD; j++)
    {
        found = FALSE;
        foundPos = -1;
        for(i = 0; !found && (i < numFinalMHs); i++)
        {
            if(finalMHlist[i].oktoprint && (BodyPartsDis[j].DUI[3] == finalMHlist[i].dui[3]))
            {
                if(strcmp(BodyPartsDis[j].DUI, finalMHlist[i].dui) == 0)
                {
                    foundPos = i;
                    found = TRUE;
                } /* fi */
            } /* fi */
        } /* for */

        if(RTM_Debug)
        {
            fprintf(fout, "fBPD - %s - found: %d\n", BodyPartsDis[j].part,
                    found);
            fflush(fout);
        } /* fi */

        /* If we found the body part, see if we can find any of the triggers */

        if(found && (foundPos > -1))
        {
            foundTrig = FALSE;
            for(i = 0; !foundTrig && (i < numFinalMHs); i++)
            {
                if(finalMHlist[i].oktoprint && (i != foundPos)) /* no sense it looking at ourself */
                {
                    for(k = 0; !foundTrig && 
                               (k < BodyPartsDis[j].numTrees); k++)
                    {
                        for(l = 0; !foundTrig && 
                               (l < finalMHlist[i].num_treecodes); l++)
                        {
                            if(BodyPartsDis[j].tripTrees[k][0] ==
                               finalMHlist[i].treecodes[l][0])
                            {
                                if(strstr(finalMHlist[i].treecodes[l],
                                     BodyPartsDis[j].tripTrees[k]) != NULL)
                                {
                                    foundTrig = TRUE;
                                    if(RTM_Debug)
                                    {
                                        fprintf(fout, "fBPD - Found Match: ");
                                        fprintf(fout, "%s (%s) <-> %s\n",
                                           finalMHlist[i].mh,
                                           finalMHlist[i].treecodes[l],
                                           BodyPartsDis[j].tripTrees[k]);
                                        fprintf(fout, "  -- Turning off: %s\n",
                                           finalMHlist[foundPos].mh);
                                        fflush(fout);
                                    } /* fi */

                                    finalMHlist[foundPos].oktoprint = FALSE;
                                } /* fi */
                            } /* fi */
                        } /* for */
                    } /* for */
                } /* fi */
            } /* for */
        } /* fi */
    } /* for */
} /* filterBodyPartDisease */


/***************************************************************************
*
*  filterML --
*
*      This 
*
***************************************************************************/

void filterML(int flag)
{
    int foundAnimals, foundOtherAnimal;
    long i, AnimalsPos, OtherAnimalPos;

    /* Review CTs to remove ML CTs other than Humans if ML is the only
       triggering factor.

       5/31/2018 - If just Antonio ML recommending Humans, go ahead and
          remove.
    */

    foundAnimals = foundOtherAnimal = FALSE;
    AnimalsPos = OtherAnimalPos = -1;
    for(i = 0; i < numFinalCTs; i++)
    {
        if(strcmp(finalCTlist[i].trigger, "Antonio ML Rule Forced") == 0)
        {
            if(strcmp(finalCTlist[i].mh, "Humans") == 0)
            {
                if(RTM_Debug)
                  fprintf(fout, "filterML - Remove ML CT1: %s (%s)\n",
                          finalCTlist[i].mh, finalCTlist[i].trigger);

                finalCTlist[i].oktoprint = FALSE;
            } /* fi */
        } /* fi */

        if((strcmp(finalCTlist[i].trigger, "Antonio ML Rule Forced") == 0) ||
           (strcmp(finalCTlist[i].trigger, 
                 "Antonio ML Rule Forced; L2R Forced Term Resurrection") == 0))
        {
            if(strcmp(finalCTlist[i].mh, "Humans") != 0)
            {
                if(RTM_Debug)
                  fprintf(fout, "filterML - Remove ML CT2: %s (%s)\n",
                          finalCTlist[i].mh, finalCTlist[i].trigger);

                finalCTlist[i].oktoprint = FALSE;
            } /* fi */
        } /* fi */

        if(strcmp(finalCTlist[i].mh, "Animals") == 0)
        {
            foundAnimals = TRUE;
            AnimalsPos = i;
        } /* fi */

        else if(amIAnimalCT(finalCTlist[i].mh))
        {
            foundOtherAnimal = TRUE;
            OtherAnimalPos = i;
        } /* else fi */
    } /* for */

    /* For preindexing, want to remove Animals if we already have Rats, Mice,
       etc. since that is more specific.
    */

    if(foundAnimals && foundOtherAnimal && (AnimalsPos > -1) && flag)
    {
        finalCTlist[AnimalsPos].oktoprint = FALSE;
        if(RTM_Debug)
        {
            fprintf(fout, "Pre-Index - Remove Animals - more Specific Found");
            if(OtherAnimalPos > -1)
              fprintf(fout, ": Found #%s#", finalCTlist[OtherAnimalPos].mh);
            fprintf(fout, "\n");
            fflush(fout);
        } /* fi */
    } /* fi */
} /* filterML */


/***************************************************************************
*
*  filterPreIndex --
*
*      This 
*
***************************************************************************/

void filterPreIndex(int isLevelOne)
{
    int numForced, numAllowed, k, j, n, ok, found, foundD12,
        foundHM, foundT, foundForced;
    long i, z, l, numGEOsFound, numFDOC;
    char dui[50], tmp[SMALL_LINE + 1];
    char **rows = NULL;

    if(RTM_Debug)
    {
        fprintf(fout, "Entered filterPreIndex(%d)\n", isLevelOne);
        fflush(fout);
    } /* fi */

    /* Remove all SHs, include all CTs, 
       remove all but, Humans if from Antonio's ML, and limit to
       the top 5 recommendations (less CTs).  Also, remove Animals
       if we already have a specific animal recommended (e.g.,Rat, Mice).
       The number may go above 5 if we have more Forced terms.
       Remove Fluorescence if we have a Technique from E05 tree.

       NOTE: 1/21/15 Keep SCR UNLESS it is in D12, then use the HMs
             for the term making sure to mark the HMs as MHs so they
             will print.
    */

    filterFluorescence();

    /* Turn off SHs */

    for(i = 0; i < numFinalSHs; i++)
    {
        if(RTM_Debug)
          fprintf(fout, "Pre-Index - Remove SH: %s\n", finalSHlist[i].mh);

        finalSHlist[i].oktoprint = FALSE;
    } /* for */

    /* Review CTs to remove ML CTs other than Humans if ML is the only
       triggering factor.

       For preindexing, want to remove Animals if we already have Rats, Mice,
       etc. since that is more specific.
    */

    if(isLevelOne)
      filterML(FALSE);
    else
      filterML(TRUE);

    /* ############   RUN 4 Code Here from NMHM_code_022515 ############ */

    /* ############   RUN 3 Code Here from NMHM_code_022515 ############ */

    /* Remove all NM & HMs -- Keep HMs that were Forced */
    /* Keep HMs if NM is a disease/syndrome SCR - 6/14/16 */
    /* 10/4/2017 - added back in keeping if SCR is forced */

    for(i = 0; i < numFinalCTs; i++)
      finalCTlist[i].saveFDOC = FALSE;

    for(i = 0; i < numFinalMHs; i++)
      finalMHlist[i].saveFDOC = FALSE;

    if(MTI_REVIEW)
      checkFDOC();

    for(i = 0; i < numFinalMHs; i++)
    {
        if((finalMHlist[i].datatype == NM) && finalMHlist[i].oktoprint)
        {
            foundForced = FALSE;
            if(finalMHlist[i].wasForced || strstr(finalMHlist[i].trigger, "Forced") != NULL)
              foundForced = TRUE;

            if(!foundForced && finalMHlist[i].saveFDOC)
            {
                if(RTM_Debug)
                  fprintf(fout, "Pre-Index - KEEPING SCR FDOC: %s\n", finalMHlist[i].mh);

                if(finalMHlist[i].trigger != NULL)
                  free(finalMHlist[i].trigger);

                finalMHlist[i].trigger = strdup("Forced Disease, Organism, Chemical");
            } /* fi */

            else if(!foundForced)
            {
                finalMHlist[i].oktoprint = FALSE;
                if(RTM_Debug)
                {
                    fprintf(fout, "Removing NM: #%s|%s# filterPreIndex Rule\n",
                            finalMHlist[i].mh, finalMHlist[i].dui);

                    for(j = 0; j < finalMHlist[i].num_HMs; j++)
                      fprintf(fout, "  --- HM[%ld]: #%s#\n", j,
                              finalMHlist[i].HMs[j]);
                    fflush(fout);
                } /* fi */

                /* Now remove any associated HMs for NM, if not a disease SCR
                   and not a Forced term.
                */

                if(!isDiseaseSCR(finalMHlist[i].mh))
                {
                    for(j = 0; j < finalMHlist[i].num_HMs; j++)
                    {
                        foundHM = FALSE;
                        for(k = 0; !foundHM && (k < numFinalMHs); k++)
                        {
                            if(finalMHlist[k].ETflag && (strlen(finalMHlist[k].mh_orig) > 0))
                            {
                                if(finalMHlist[i].HMs[j][0] == finalMHlist[k].mh_orig[0])
                                {
                                    if(strcmp(finalMHlist[i].HMs[j], finalMHlist[k].mh_orig) == 0)
                                      foundHM = TRUE;
                                } /* fi */
                            } /* fi */

                            else
                            {
                                if(finalMHlist[i].HMs[j][0] == finalMHlist[k].mh[0])
                                {
                                    if(strcmp(finalMHlist[i].HMs[j], finalMHlist[k].mh) == 0)
                                      foundHM = TRUE;
                                } /* fi */
                            } /* else */

                            if(foundHM)
                            {
                                if(strstr(finalMHlist[k].trigger, "Forced") == NULL)
                                {
                                    finalMHlist[k].oktoprint = FALSE;
                                    if(RTM_Debug)
                                    {
                                        fprintf(fout, "Removing associated HM:");
                                        fprintf(fout, " #%s# (%s)",
                                                finalMHlist[k].mh, finalMHlist[k].trigger);
                                        fprintf(fout, " filterPreIndex Rule\n");
                                        fflush(fout);
                                    } /* fi RTM_Debug */
                                } /* fi !Forced */
                            } /* fi foundHM */
                        } /* for numFinalMHs */
                    } /* for num_HMs */
                } /* fi !isDiseaseSCR */
            } /* else fi !foundForced */
        } /* fi NM */
    } /* for */

    /* ################################################################# */

    /* Check to see how many forced terms we have - we only want about 5
       terms out of this.  If we have more forced, they should all be
       provided.
    */

    numForced = numAllowed = numFDOC = 0;
    for(i = 0; i < numFinalMHs; i++)
    {
        if(finalMHlist[i].wasForced && finalMHlist[i].oktoprint)
        {
            if(RTM_Debug)
              fprintf(fout, "Forced: %s\n", finalMHlist[i].mh);

            numForced++;
        } /* fi */

        else if(finalMHlist[i].saveFDOC)
        {
            if(RTM_Debug)
              fprintf(fout, "FDOC: %s\n", finalMHlist[i].mh);

            numFDOC++;
        } /* fi */
    } /* for */

    numAllowed = 5 - numForced;
    if(numAllowed < 0)
      numAllowed = 0;

    if(RTM_Debug)
      fprintf(fout, "Found %ld Forced Terms -- Allowed: %ld  -- FDOC: %ld\n",
              numForced, numAllowed, numFDOC);

    /* Now go through and remove extras */

    for(i = 0; i < numFinalMHs; i++)
    {
        if(!finalMHlist[i].wasForced && finalMHlist[i].oktoprint)
        {
            if(numAllowed > 0)
            {
                if(RTM_Debug)
                  fprintf(fout, "Pre-Index - KEEPING: %s\n", finalMHlist[i].mh);
                numAllowed--;
            } /* fi */

            else if(finalMHlist[i].saveFDOC)
            {
                if(RTM_Debug)
                  fprintf(fout, "Pre-Index - KEEPING FDOC: %s\n", finalMHlist[i].mh);

                if(finalMHlist[i].trigger != NULL)
                  free(finalMHlist[i].trigger);

                finalMHlist[i].trigger = strdup("Forced Disease, Organism, Chemical");
            } /* else */

            else
            {
                if(RTM_Debug)
                  fprintf(fout, "Pre-Index - Remove > 5: %s (%s)\n",
                          finalMHlist[i].mh, finalMHlist[i].trigger);

                checkCTRemove(finalMHlist[i].mh);
                finalMHlist[i].oktoprint = FALSE;
            } /* else */
        } /* else fi */
    } /* for */

    /* ############   RUN 1 Code Here from NMHM_code_022515 ############ */

    /* ############   RUN 2 Code Here from NMHM_code_022515 ############ */

    /* ################################################################# */

    /* Switch all HMs that survived to MHs so they will print */

    for(i = 0; i < numFinalMHs; i++)
    {
        if((finalMHlist[i].datatype == HM) && finalMHlist[i].oktoprint)
          finalMHlist[i].datatype = MH;
    } /* for */

    if(RTM_Debug)
    {
        for(i = 0; i < numFinalMHs; i++)
        {
            if(finalMHlist[i].oktoprint)
              fprintf(fout, "Pre-Index - Final OK: %s\n", finalMHlist[i].mh);
        } /* for */
    
        fflush(fout);
    } /* fi */
} /* filterPreIndex */

/***************************************************************************
*
*  filterIJED --
*
*      This 
*
***************************************************************************/

void filterIJED()
{
    int found, foundSpec;
    long i, EDpos, Bpos, BNpos;
    char trigger[SMALL_LINE + 1];

    /* Int J Eat Disord Rule:
        Eating Disorders is recommended for each citation - unless
        we find a more specific term in the same tree.  This is
        normally Anorexia Nervosa or Bulimia Nervosa).  If we find
        a more specific term, remove Eating Disorders.  Also, while
        we are at it, clean up Bulimia also being recommended when
        Bulimia Nervosa is being recommended.

Eating Disorders|D001068|C0013473
Bulimia|D002032|C0006370
Bulimia Nervosa|D052018|C2267227

    */

    strcpy(trigger, "");
    EDpos = Bpos = BNpos = -1;
    found = foundSpec = FALSE;
    for(i = 0; i < numFinalMHs; i++)
    {
        if(strcmp(finalMHlist[i].dui, "D001068") == 0) /* Eating Disorders */
        {
            found = TRUE;
            EDpos = i;
        } /* fi */

         /* Bulimia */

        else if(strcmp(finalMHlist[i].dui, "D002032") == 0)
          Bpos = i;

         /* Bulimia Nervosa */

        else if(strcmp(finalMHlist[i].dui, "D052018") == 0)
          BNpos = i;

         /* Anorexia Nervosa: D000856
            Binge-Eating Disorder: D056912
            Female Athlete Triad Syndrome: D053716
            Pica: D010842
         */

        else if((strcmp(finalMHlist[i].dui, "D000856") == 0) ||
                (strcmp(finalMHlist[i].dui, "D056912") == 0) ||
                (strcmp(finalMHlist[i].dui, "D053716") == 0) ||
                (strcmp(finalMHlist[i].dui, "D010842") == 0))
        {
            foundSpec = TRUE;
            if(strlen(trigger) > 0)
              strcat(trigger, ";");
            else
              strcpy(trigger, "Removed Eating Disorder, More Specific: ");

            if(finalMHlist[i].ETflag && (strlen(finalMHlist[i].mh_orig) > 0))
              strcat(trigger, finalMHlist[i].mh_orig);
            else
              strcat(trigger, finalMHlist[i].mh);
        } /* else fi */
    } /* for */

    if(found && foundSpec) /* Remove Eating Disorders */
    {
        if(EDpos > -1)
          finalMHlist[EDpos].oktoprint = FALSE;

        if(RTM_Debug)
        {
             fprintf(fout, "filterIJED - %s\n", trigger);
             fflush(fout);
        } /* fi */
    } /* fi */

    if((Bpos > -1) && (BNpos > -1))
    {
        finalMHlist[Bpos].oktoprint = FALSE;

        if(RTM_Debug)
        {
             fprintf(fout,
                     "filterIJED - Removing Bulimia have Bulimia Nervosa\n");
             fflush(fout);
        } /* fi */
    } /* fi */
} /* filterIJED */


/***************************************************************************
*
*  filterMeSHonDemand --
*
*      This 
*
***************************************************************************/

void filterMeSHonDemand()
{
    long i;

    /* Review CTs to remove ML CTs other than Humans, Male, Female, and Swine
       if ML is the only triggering factor.

       Also, remove all Subheadings since we are going to just throw
       them away later anyway.
    */

    for(i = 0; i < numFinalCTs; i++)
    {
        if(strcmp(finalCTlist[i].trigger, "Antonio ML Rule Forced") == 0)
        {
            if((strcmp(finalCTlist[i].mh, "Humans") != 0) &&
               (strcmp(finalCTlist[i].mh, "Male") != 0) &&
               (strcmp(finalCTlist[i].mh, "Female") != 0) &&
               (strcmp(finalCTlist[i].mh, "Swine") != 0))
            {
                if(RTM_Debug)
                  fprintf(fout, "MoD - Remove ML CT: %s (%s)\n",
                          finalCTlist[i].mh, finalCTlist[i].trigger);

                finalCTlist[i].oktoprint = FALSE;
            } /* fi */
        } /* fi */
    } /* for */

    for(i = 0; i < numFinalSHs; i++)
    {
        if(RTM_Debug)
          fprintf(fout, "MoD - Remove SH: %s\n", finalSHlist[i].mh);

        finalSHlist[i].oktoprint = FALSE;
    } /* for */
} /* filterMeSHonDemand */

/***************************************************************************
*
*  filterNeoplasms --
*
*      This 
*
***************************************************************************/

void filterNeoplasms()
{
    int j, k, l, found, foundT;
    long i, foundPos;
    char DUI[25], treeA[50], treeB[50];

    /* If we have Neoplasms and at least one specific cell line
       (A11.251.210.190) OR Tumor Cells, Cultured (A11.251.860),
       remove Neoplasms.

Neoplasms|D009369|C0027651
Cell Line, Tumor : A11.251.210.190
Tumor Cells, Cultured : A11.251.860
    */

    strcpy(DUI, "D009369");
    strcpy(treeA, "A11.251.210.190");
    strcpy(treeB, "A11.251.860");

    found = FALSE;
    foundPos = -1;
    for(i = 0; !found && (i < numFinalMHs); i++)
    {
        if(DUI[3] == finalMHlist[i].dui[3])
        {
            if(strcmp(DUI, finalMHlist[i].dui) == 0)
            {
                foundPos = i;
                found = TRUE;
            } /* fi */
        } /* fi */
    } /* for */

    if(RTM_Debug)
    {
        fprintf(fout, "filterNeoplasms - found: %d\n", found);
        fflush(fout);
    } /* fi */

        /* If we found the body part, see if we can find any of the triggers */

    if(found && (foundPos > -1))
    {
        foundT = FALSE;
        for(i = 0; !foundT && (i < numFinalMHs); i++)
        {
            if(i != foundPos) /* no sense it looking at ourself */
            {
                for(l = 0; !foundT && (l < finalMHlist[i].num_treecodes); l++)
                {
                     if(finalMHlist[i].treecodes[l][0] == 'A')
                     {
                         if((strstr(finalMHlist[i].treecodes[l],
                                    treeA) != NULL) ||
                            (strstr(finalMHlist[i].treecodes[l],
                                    treeB) != NULL))
                         {
                             foundT = TRUE;
                             if(RTM_Debug)
                             {
                                 fprintf(fout, "fNeoplasms - Found Match: ");
                                 fprintf(fout, "%s (%s)\n",finalMHlist[i].mh,
                                           finalMHlist[i].treecodes[l]);
                                 fprintf(fout, "  -- Turning off: %s\n",
                                           finalMHlist[foundPos].mh);
                                 fflush(fout);
                             } /* fi */

                             finalMHlist[foundPos].oktoprint = FALSE;
                         } /* fi strstr */
                     } /* fi "A" */
                } /* for  each treecode*/
            } /* fi != foundPos */
        } /* for */
    } /* fi */
} /* filterNeoplasms */

/***************************************************************************
*
*  filterCancer -- Different then filterNeoplasms and is used explicitly
*     to remove "Cancer" ET when specific cancer found like "Breast Cancer".
*
***************************************************************************/

void filterCancer()
{
    int j, k, l, found, foundT, done;
    long i, foundPos;
    char DUI[25], treeA[50];

    /* If we have "Cancer" ET and at least one more specific "cancer"
       like "Breast Cancer", remove "Cancer".

Neoplasms|D009369|C0027651
Neoplasms by Site : C04.588
    */

    strcpy(DUI, "D009369");
    strcpy(treeA, "C04.588");

    found = done = FALSE;
    foundPos = -1;
    for(i = 0; !found && !done && (i < numFinalMHs); i++)
    {
        if(finalMHlist[i].oktoprint && (DUI[3] == finalMHlist[i].dui[3]))
        {
            if(finalMHlist[i].ETflag && (strcmp(DUI, finalMHlist[i].dui) == 0))
            {
                done = TRUE;
                if(strcmp(finalMHlist[i].mh, "Cancer") == 0)
                {
                    found = TRUE;
                    foundPos = i;
                } /* fi */
            } /* fi */
        } /* fi */
    } /* for */

    if(RTM_Debug)
    {
        fprintf(fout, "filterCancer - found: %d\n", found);
        fflush(fout);
    } /* fi */

    /* If we found Cancer, see if we have any other ETs */

    if(found && (foundPos > -1))
    {
        foundT = FALSE;
        for(i = 0; !foundT && (i < numFinalMHs); i++)
        {
             /* no sense it looking at ourself, or non ET, or not ok to print */

            if((i != foundPos) && finalMHlist[i].oktoprint &&
               finalMHlist[i].ETflag)
            {
                for(l = 0; !foundT && (l < finalMHlist[i].num_treecodes); l++)
                {
                     if(finalMHlist[i].treecodes[l][0] == treeA[0])
                     {
                         if((strstr(finalMHlist[i].treecodes[l],
                                    treeA) != NULL) &&
                            (strstr(finalMHlist[i].mh, " Cancer") != NULL)
                           )
                         {
                             foundT = TRUE;
                             if(RTM_Debug)
                             {
                                 fprintf(fout, "fCancer - Found Match: ");
                                 fprintf(fout, "%s (%s)\n",finalMHlist[i].mh,
                                           finalMHlist[i].treecodes[l]);
                                 fprintf(fout, "  -- Turning off: %s\n",
                                           finalMHlist[foundPos].mh);
                                 fflush(fout);
                             } /* fi */

                             finalMHlist[foundPos].oktoprint = FALSE;
                         } /* fi strstr */
                     } /* fi "A" */
                } /* for  each treecode*/
            } /* fi != foundPos */
        } /* for */
    } /* fi */
} /* filterCancer */

/***************************************************************************
*
*  filterFluorescence --
*
***************************************************************************/

void filterFluorescence()
{
    int j, k, l, found, foundT;
    long i, foundPos;
    char DUI[25], treeA[50];

    /* If we have "Fluorescence" and at least one technique from E05 Tree,
       remove Fluorescence.

Fluorescence|D005453|C0016315
Investigative Techniques : E05
    */

    strcpy(DUI, "D005453");
    strcpy(treeA, "E05");

    found = FALSE;
    foundPos = -1;
    for(i = 0; !found && (i < numFinalMHs); i++)
    {
        if(finalMHlist[i].oktoprint && (DUI[3] == finalMHlist[i].dui[3]))
        {
            if(strcmp(DUI, finalMHlist[i].dui) == 0)
            {
                found = TRUE;
                foundPos = i;
            } /* fi */
        } /* fi */
    } /* for */

    if(RTM_Debug)
    {
        fprintf(fout, "filterFluorescence - found: %d\n", found);
        fflush(fout);
    } /* fi */

    /* If we found Fluorescence, see if we have any E05 Technique */

    if(found && (foundPos > -1))
    {
        foundT = FALSE;
        for(i = 0; !foundT && (i < numFinalMHs); i++)
        {
             /* no sense it looking at ourself or not ok to print */

            if((i != foundPos) && finalMHlist[i].oktoprint)
            {
                for(l = 0; !foundT && (l < finalMHlist[i].num_treecodes); l++)
                {
                     if(finalMHlist[i].treecodes[l][0] == treeA[0])
                     {
                         if(strstr(finalMHlist[i].treecodes[l], treeA) != NULL)
                         {
                             foundT = TRUE;
                             if(RTM_Debug)
                             {
                                 fprintf(fout, "fFluorescence - Found Match: ");
                                 fprintf(fout, "%s (%s)\n",finalMHlist[i].mh,
                                           finalMHlist[i].treecodes[l]);
                                 fprintf(fout, "  -- Turning off: %s\n",
                                           finalMHlist[foundPos].mh);
                                 fflush(fout);
                             } /* fi */

                             finalMHlist[foundPos].oktoprint = FALSE;
                         } /* fi strstr */
                     } /* fi "A" */
                } /* for  each treecode*/
            } /* fi != foundPos */
        } /* for */
    } /* fi */
} /* filterFluorescence */


/***************************************************************************
*
*  filterGenMiceRatHamster --
*
*      This 
*
***************************************************************************/

void filterGenMiceRatHamster()
{
    int haveMice, haveRat, haveHam, haveMsub, haveRsub, haveHsub, j;
    long i, MicePos, RatPos, HamPos, mTrigPos, rTrigPos, hTrigPos;

    /* Remove Mice, Rats, and/or Cricetinae (Hamsters) if we find anything
       being recommended below them in the tree.
    */

    if(RTM_Debug)
    {
        fprintf(fout, "Entered filterGenMiceRatHamster\n");
        fflush(fout);
    } /* fi */

    MicePos = RatPos = HamPos = mTrigPos = rTrigPos = hTrigPos = -1;
    haveMice = haveRat = haveHam = haveMsub = haveRsub = haveHsub = FALSE;
    for(i = 0; i < numFinalCTs; i++)
    {
        if(RTM_Debug)
        {
            fprintf(fout, "FinalTs[%ld]: %s|%s\n", i, finalCTlist[i].dui,
                    finalCTlist[i].mh);
            fflush(fout);
        } /* fi */

        if(strcmp(finalCTlist[i].dui, "D051379") == 0)  /* Mice */
        {
            haveMice = TRUE;
            MicePos = i;
        } /* fi */

        else if(strcmp(finalCTlist[i].dui, "D051381") == 0)  /* Rats */
        {
            haveRat = TRUE;
            RatPos = i;
        } /* else fi */

        else if(strcmp(finalCTlist[i].dui, "D006224") == 0)  /* Hamster */
        {
            haveHam = TRUE;
            HamPos = i;
        } /* else fi */

        if(RTM_Debug)
        {
            fprintf(fout, "haveMice: %d  haveRats: %d  haveHam: %d\n",
                    haveMice, haveRat, haveHam);
            fflush(fout);
        } /* fi */
    } /* for */

    if(haveMice || haveRat || haveHam)
    {
        for(i = 0; i < numFinalMHs; i++)
        {
            for(j = 0; j < finalMHlist[i].num_treecodes; j++)
            {
                if(finalMHlist[i].treecodes[j][0] = 'B')
                {
                   if(strstr(finalMHlist[i].treecodes[j],         /* Mice */
                             "B01.050.150.900.649.865.635.505.500.") != NULL)
                   {
                       haveMsub = TRUE;
                       mTrigPos = i;
                   } /* fi */

                   else if(strstr(finalMHlist[i].treecodes[j],    /* Rats */
                             "B01.050.150.900.649.865.635.505.700.") != NULL)
                   {
                       haveRsub = TRUE;
                       rTrigPos = i;
                   } /* else fi */

                   else if(strstr(finalMHlist[i].treecodes[j],    /* Hamster */
                             "B01.050.150.900.649.865.635.150.") != NULL)
                   {
                       haveHsub = TRUE;
                       hTrigPos = i;
                   } /* else fi */
                } /* fi */
            } /* for */

            if(RTM_Debug)
            {
                fprintf(fout, "haveMsub: %d  haveRsub: %d  haveHsub: %d\n",
                        haveMsub, haveRsub, haveHsub);
                fflush(fout);
            } /* fi */
        } /* for */
    } /* fi */

    /* Now that we have finished reviewing the CTs, see what we have */

    if(haveMice && haveMsub)
    {
        if(RTM_Debug)
          fprintf(fout, "Indexing Rule - Remove: %s (%s)\n",
                  finalCTlist[MicePos].mh, finalMHlist[mTrigPos].mh);

        finalCTlist[MicePos].oktoprint = FALSE;
    } /* fi */

    if(haveRat && haveRsub)
    {
        if(RTM_Debug)
          fprintf(fout, "Indexing Rule - Remove: %s (%s)\n",
                  finalCTlist[RatPos].mh, finalMHlist[rTrigPos].mh);

        finalCTlist[RatPos].oktoprint = FALSE;
    } /* fi */

    if(haveHam && haveHsub)
    {
        if(RTM_Debug)
          fprintf(fout, "Indexing Rule - Remove: %s (%s)\n",
                  finalCTlist[HamPos].mh, finalMHlist[hTrigPos].mh);

        finalCTlist[HamPos].oktoprint = FALSE;
    } /* fi */
} /* filterGenMiceRatHamster */


/***************************************************************************
*
*  filterOLDMEDLINE --
*
*      This 
*
***************************************************************************/

void filterOLDMEDLINE()
{
    long i;

    /* Turn off all subheadings
       remove all MHs that only come from PRC
       
    */

    /* Turn off SHs */

    for(i = 0; i < numFinalSHs; i++)
    {
        if(RTM_Debug)
          fprintf(fout, "OLDMEDLINE - Remove SH: %s\n", finalSHlist[i].mh);

        finalSHlist[i].oktoprint = FALSE;
    } /* for */

    /* Turn off all PRC only MHs */

    for(i = 0; i < numFinalMHs; i++)
    {
        if(finalMHlist[i].oktoprint && !finalMHlist[i].paths[MMI] &&
           !finalMHlist[i].wasForced)
        {
            if(RTM_Debug)
              fprintf(fout, "OLDMEDLINE - Remove PRC Only: %s\n",
                      finalMHlist[i].mh);

            finalMHlist[i].oktoprint = FALSE;
        } /* fi */
    } /* for */

    /* Review CTs to remove ML CTs other than Humans if ML is the only
       triggering factor.
    */

    filterML(FALSE);
} /* filterOLDMEDLINE */


/***************************************************************************
*
*  filterMaleFemale --
*
*      This 
*
***************************************************************************/

void filterMaleFemale()
{
    int haveMale, haveFemale, haveMaleSpecific, haveFemaleSpecific,
        onlyMaleML, onlyFemaleML, onlyMalePRC, onlyFemalePRC;
    long i, MalePos, FemalePos;

    /* Remove either Male or Female if they are only here because of
       either Machine Learning or PRC AND there is solid evidence that
       the article is talking about the opposite sex.  For example if
       "case-control study in middle-aged men" is in the text,
       there is no point in recommending Female unless something else
       in the text supports it.  Same for if we have a Male or Female
       specific disease and no supporting evidence for the other.
    */

    if(RTM_Debug)
    {
        fprintf(fout, "Entered filterMaleFemale\n");
        fflush(fout);
    } /* fi */

    MalePos = FemalePos = -1;
    haveMale = haveFemale = haveMaleSpecific = haveFemaleSpecific = FALSE;
    onlyMaleML = onlyFemaleML = onlyMalePRC = onlyFemalePRC = FALSE;
    for(i = 0; i < numFinalCTs; i++)
    {
        if(strcmp(finalCTlist[i].dui, "D005260") == 0)  /* Female */
        {
            haveFemale = TRUE;
            FemalePos = i;
            if((strstr(finalCTlist[i].trigger, "CT Treecode Lookup") != NULL)||
               (strstr(finalCTlist[i].trigger, "CT Text Lookup") != NULL))
              haveFemaleSpecific = TRUE;

            else if(strcmp(finalCTlist[i].trigger,
                           "Antonio ML Rule Forced") == 0)
              onlyFemaleML = TRUE;

            else if((strchr(finalCTlist[i].trigger, ';') == NULL) &&
                    (strstr(finalCTlist[i].trigger, "Heuristic #N:") != NULL))
              onlyFemalePRC = TRUE;
        } /* fi */

        else if(strcmp(finalCTlist[i].dui, "D008297") == 0)  /* Male */
        {
            haveMale = TRUE;
            MalePos = i;
            if((strstr(finalCTlist[i].trigger, "CT Treecode Lookup") != NULL)||
               (strstr(finalCTlist[i].trigger, "CT Text Lookup") != NULL))
              haveMaleSpecific = TRUE;

            else if(strcmp(finalCTlist[i].trigger,
                           "Antonio ML Rule Forced") == 0)
              onlyMaleML = TRUE;

            else if((strchr(finalCTlist[i].trigger, ';') == NULL) &&
                    (strstr(finalCTlist[i].trigger, "Heuristic #N:") != NULL))
              onlyMalePRC = TRUE;
        } /* else fi */
    } /* for */

    if(RTM_Debug)
    {
        fprintf(fout, "haveMale: %d  haveFemale: %d\n", haveMale, haveFemale);
        fprintf(fout, "haveMaleSpecific: %d  haveFemaleSpecific: %d\n",
                haveMaleSpecific, haveFemaleSpecific);
        fprintf(fout, "onlyMaleML: %d  onlyFemaleML: %d\n",
                onlyMaleML, onlyFemaleML);
        fprintf(fout, "onlyMalePRC: %d  onlyFemalePRC: %d\n",
                onlyMalePRC, onlyFemalePRC);
        fflush(fout);
    } /* fi */

    /* See if we need to remove anything. */

    if(haveMale && haveFemale)
    {
        if(haveMaleSpecific && !haveFemaleSpecific)
        {
            if(onlyFemaleML || onlyFemalePRC)
            {
                if(RTM_Debug)
                  fprintf(fout, "Male/Female Rule - Remove: Female\n");

                finalCTlist[FemalePos].oktoprint = FALSE;
            } /* fi */
        } /* fi MALE */

        else if(!haveMaleSpecific && haveFemaleSpecific)
        {
            if(onlyMaleML || onlyMalePRC)
            {
                if(RTM_Debug)
                  fprintf(fout, "Male/Female Rule - Remove: Male\n");

                finalCTlist[MalePos].oktoprint = FALSE;
            } /* fi */
        } /* fi FEMALE */
    } /* fi */
} /* filterMaleFemale */


/***************************************************************************
*
*  filterHeuristicN --
*
*      This 
*
***************************************************************************/

void filterHeuristicN()
{
    long i;

    /* Remove ALL CheckTags if they are only here because of
       the Heuristic #N PRC Only rule.
    */

    if(RTM_Debug)
    {
        fprintf(fout, "Entered filterHeuristicN\n");
        fflush(fout);
    } /* fi */

    for(i = 0; i < numFinalCTs; i++)
    {
        if((strchr(finalCTlist[i].trigger, ';') == NULL) &&
           (strstr(finalCTlist[i].trigger, "Heuristic #N:") != NULL))
        {
            if(RTM_Debug)
             fprintf(fout, "Heuristic #N CHJ - Remove: %s|%s\n",
                     finalCTlist[i].mh, finalCTlist[i].trigger);
            finalCTlist[i].oktoprint = FALSE;
        } /* fi */
    } /* for */
} /* filterHeuristicN */

/***************************************************************************
*
*  filterMentalDisorders --
*
*      This 
*
***************************************************************************/

void filterMentalDisorders()
{
    int found, foundSpec, done;
    long i, l, MDpos, Spos;
    char trigger[SMALL_LINE + 1];

    /* If we are recommending Mental Disorders, remove it if we are also
       recommending something more specific within the Mental Disorders tree.

Mental Disorders|D001523|C0004936|F3
    */

    strcpy(trigger, "");
    MDpos = Spos = -1;
    found = foundSpec = FALSE;
    for(i = 0; i < numFinalMHs; i++)
    {
        if(strcmp(finalMHlist[i].dui, "D001523") == 0) /* Mental Disorders */
        {
            found = TRUE;
            MDpos = i;
        } /* fi */

        else
        {
            for(l = 0; !foundSpec && (l < finalMHlist[i].num_treecodes); l++)
            {
                if(finalMHlist[i].treecodes[l][0] == 'F')
                {
                    if(strstr(finalMHlist[i].treecodes[l], "F03.") != NULL)
                    {
                        foundSpec = TRUE;
                        Spos = i;
                    } /* fi */
                } /* fi */
            } /* for */
        } /* else */
    } /* for */

    /* See if this is coming from Mental Illness since that trigger is usually good */

    if(found && foundSpec && (MDpos > -1) && (Spos > -1))
    {
        if(strstr(finalMHlist[MDpos].trigger, "Forced Non-Leaf Node Lookup") != NULL)
        {
            found = FALSE;
            MDpos = -1;
        } /* fi */
    } /* fi */

    /* If we should still remove then do so.  Keep track though if what we remove had been forced,
       transfer that flag onto the more specific term.
    */

    if(found && foundSpec && (MDpos > -1) && (Spos > -1))
    {
        strcpy(trigger, "Removed Mental Disorder, More Specific: ");

        if(finalMHlist[Spos].ETflag &&
           (strlen(finalMHlist[Spos].mh_orig) > 0))
          strcat(trigger, finalMHlist[Spos].mh_orig);
        else
          strcat(trigger, finalMHlist[Spos].mh);

        finalMHlist[MDpos].oktoprint = FALSE;
        if(finalMHlist[MDpos].wasForced)
           finalMHlist[Spos].wasForced = TRUE;

        if(RTM_Debug)
        {
            fprintf(fout, "filterMentalDisorders - %s\n", trigger);
            fflush(fout);
        } /* fi */
    } /* fi */
} /* filterMentalDisorders */

/***************************************************************************
*
*  filterGenSpec --
*
*      This 
*
***************************************************************************/

void filterGenSpec()
{
    int found, done, x, y, foundCoOccur, doneCoOccur, ok;
    long i, j, k, len;
    char mhGen[SMALL_LINE + 1], mhSpec[SMALL_LINE + 1], tmp[SMALL_LINE + 1];

    found = FALSE;
    for(i = 0; i < numFinalMHs; i++)
    {
        if(finalMHlist[i].oktoprint)
        {
            done = FALSE;

            /* Handle Special cases here --
                If Cardiovascular Diseases in Title, allow regardless
            */

            if(strstr(finalMHlist[i].textloc, "TI") != NULL)
            {
                if(finalMHlist[i].ETflag && 
                              (strlen(finalMHlist[i].mh_orig) > 0))
                  strcpy(tmp, finalMHlist[i].mh_orig);
                else
                  strcpy(tmp, finalMHlist[i].mh);

                if(strcmp(tmp, "Cardiovascular Diseases") == 0)
                  done = TRUE;
            } /* fi found in TI */


            /* 25 March 2016 - Ignoring cases where Specific term only
                   came from PRC and allowing the General term helps
                   Recall +0.14, but, really hurts Precision: -0.32.

                   -----------------------------------------------------

                   If in TI and ET or MH exactly found in Title
                   Slightly better than above, but, still really drops Prec.
                   Recall: +0.12  Precision: -0.25
            */

            /* Tested & Removed 7/17/17 -
              If General term is forced and specific term is not forced,
              keep the general term. 
              Removed since it helps Recall a little (0.04), but drops
              precision a lot (0.12) more.
            */

            for(x = 0; !done && (x < finalMHlist[i].num_treecodes); x++)
            {
                for(j = 0; !done && (j < numFinalMHs); j++)
                {
                   ok = TRUE;

                   if(ok && finalMHlist[j].oktoprint && (i != j))
                   {
                       found = FALSE;
                       for(y = 0; !found &&
                                  (y < finalMHlist[j].num_treecodes); y++)
                       {
                           if(strstr(finalMHlist[j].treecodes[y],
                                     finalMHlist[i].treecodes[x]) != NULL)
                           {
                               strcpy(tmp, "");
                               strcpy(mhGen, "");
                               strcpy(mhSpec, "");

                               found = TRUE;
                               if(finalMHlist[i].ETflag &&
                                   (strlen(finalMHlist[i].mh_orig) > 0))
                                 strcpy(mhGen, finalMHlist[i].mh_orig);
                               else
                                 strcpy(mhGen, finalMHlist[i].mh);

                               if(finalMHlist[j].ETflag &&
                                   (strlen(finalMHlist[j].mh_orig) > 0))
                                 strcpy(mhSpec, finalMHlist[j].mh_orig);
                               else
                                 strcpy(mhSpec, finalMHlist[j].mh);

                               sprintf(tmp, "%s|%s\0", mhGen, mhSpec);

                               foundCoOccur = doneCoOccur = FALSE;
                               for(k = 0; !foundCoOccur && !doneCoOccur &&
                                          (k < numBadHierCoOccur); k++)
                               {
                                    if(tmp[0] == mhHierBadCoOccur[k][0])
                                    {
                                        if(strcmp(tmp, mhHierBadCoOccur[k]) == 0)
                                        {
                                            foundCoOccur = TRUE;
                                            done = TRUE;
                                            if(RTM_Debug)
                                            {
                                                fprintf(fout, "Remove Gen/Spec - %s\n", tmp);
                                                fflush(fout);
                                            } /* fi */
                                            finalMHlist[i].oktoprint = FALSE;

                                            /* Set the reason for removal to forced */

                                            finalMHlist[j].wasForced = TRUE;
                                        } /* fi */
                                    } /* fi */

                                    else if(tmp[0] < mhHierBadCoOccur[k][0])
                                       doneCoOccur = TRUE;
                               } /* for k */
                           } /* fi */
                       } /* for y */
                   } /* fi ok */
                } /* for all other MHs */
            } /* for each treecode */
        } /* fi oktoprint */
    } /* for each MH */
} /* filterGenSpec */

/***************************************************************************
*
*  isDiseaseSCR --
*
*      This 
*
***************************************************************************/

int isDiseaseSCR(char *name)
{
    char **rows;
    int n, k, rtn, class;

    /*
   1 - Regular Chemical,  Drug, or Substance
   2 - Protocol
   3 - Disease
   4 - Organism

   For 1, we want to try and break it down a bit more
      D08, D12 HMs -- Protein
      D26 HMs -- Drug
      D01, D02 HMs -- Chemical
      D09 -- Carbohydrates
      Other

   New Classes:

      40 - Protein
      50 - Drug
      60 - Chemical
      70 - Carbohydrates
      80 - Others
    */
    rtn = FALSE;
    query_btree(SCRCLASS, name, &rows, &n);
    if((rows != NULL) && (n > 0))
    {
        class = -1;
        sscanf(rows[0], "%*[^|]|%d", &class);
        if(class == 3)
          rtn = TRUE;

        for(k = 0; k < n; k++)
          free(rows[k]);
        free(rows);
    } /* fi */

    else if(rows != NULL)
      free(rows);

    return(rtn);
} /* isDiseaseSCR */


/***************************************************************************
*
*  checkCTRemove --
*
*      This 
*
***************************************************************************/

void checkCTRemove(char *mh)
{
    int done;
    long i, len;
    char *tmp, *check, *rest;

    /* Review CTs to remove CT if only trigger is for a term we are removing.
    */

    if(RTM_Debug)
    {
        fprintf(fout, "Entered checkCTRemove(%s)\n", mh);
        fflush(fout);
    } /* fi */

    for(i = 0; i < numFinalCTs; i++)
    {
        if(finalCTlist[i].oktoprint && (strstr(finalCTlist[i].trigger, mh) != NULL))
        {
            if(RTM_Debug)
            {
                fprintf(fout, "  -- Found in trigger|%s|%s|\n", mh, finalCTlist[i].trigger);
                fflush(fout);
            } /* fi */
            tmp = strdup(finalCTlist[i].trigger);
            len = (long)strlen(finalCTlist[i].trigger);
            check = (char *)malloc(len + 1);
            strcpy(check, "");
            rest = (char *)malloc(len + 1);
            strcpy(rest, "");
            done = FALSE;
            while(!done && (strlen(tmp) > 0))
            {
                strcpy(check, "");
                strcpy(rest, "");
                sscanf(tmp, "%[^;]; %[^\n]", check, rest);

                if(strstr(check, mh) == NULL)
                  done = TRUE;

                strcpy(tmp, rest);
                if(strlen(tmp) > 0)
                {
                    if(strstr(tmp, mh) == NULL)
                      done = TRUE;
                } /* fi */
            } /* while */

            /* If mh is the only thing trigger, then we should remove the CT */

            if(!done)
            {
                if(RTM_Debug)
                {
                    fprintf(fout, "Removing CheckTag: %s due to only trigger: %s\n",
                            finalCTlist[i].mh, mh);
                    fflush(fout);
                } /* fi */

                finalCTlist[i].oktoprint = FALSE;                  
            } /* fi */

            free(tmp);
            free(rest);
            free(check);
        } /* fi */
    } /* for */
} /* checkCTRemove */


/***************************************************************************
*
*  checkFDOC --
*
*      This 
*
***************************************************************************/

void checkFDOC()
{
    int isOrganism, isDisease, isChemical, isOKSCR, isModel;
    long i, l, numOrganisms;

    /* Check on number of "Model" Organisms we have ready to print in both CT & MH lists */

    numOrganisms = 0;
    for(i = 0; i < numFinalCTs; i++)
    {
        if(finalCTlist[i].oktoprint && finalCTlist[i].wasForced)
        {
            if(isModelOrganism(finalCTlist[i].dui))
               numOrganisms++;
        } /* fi */
    } /* for each CT */

    for(i = 0; i < numFinalMHs; i++)
    {
        if(finalMHlist[i].oktoprint && finalMHlist[i].wasForced)
        {
            if(isModelOrganism(finalMHlist[i].dui))
               numOrganisms++;
        } /* fi */
    } /* for each MH */

    if(RTM_Debug)
    {
        fprintf(fout, "numOrganisms Found: %ld\n", numOrganisms);
        fflush(fout);
    } /* fi */

    for(i = 0; i < numFinalMHs; i++)
    {
        if(RTM_Debug)
        {
            fprintf(fout, "checkFDOC[%ld] - %s|%d|%d|", i, finalMHlist[i].mh,
                    finalMHlist[i].wasForced, finalMHlist[i].oktoprint);
            fflush(fout);

            if(finalMHlist[i].datatype == MH)
              fprintf(fout, "MH|");

            else if(finalMHlist[i].datatype == NM)
              fprintf(fout, "NM|");

            else if(finalMHlist[i].datatype == CT)
              fprintf(fout, "CT|");

            else if(finalMHlist[i].datatype == HM)
              fprintf(fout, "HM|");

            else
              fprintf(fout, "UNK|");

            fprintf(fout, "%s|%ld|\n", finalMHlist[i].textloc, finalMHlist[i].numPIs);
            fflush(fout);
        } /* fi RTM_Debug */

        /* Now, go through terms that may not be indexed.  If they meet any of our criteria,
           we want to save them.

           1) Found in Title with 1 or more occurrences
           2) Found in Abstract with 2 or more occurrences
           3) If no Model Organisms found, we will allow a Model Organism if it shows up
              in the Abstract with only a single occurrence.

           4) If one of the above is found, then we check to see if an Organism,
              Disease, or Chemical and allow if one of these three.  Or, if Chemical or Disease SCR.
        */

        if(finalMHlist[i].oktoprint && !finalMHlist[i].wasForced)
        {
             if(RTM_Debug)
               fprintf(fout, "   --- Term to FDOC check\n");

             isOrganism = isDisease = isChemical = isOKSCR = isModel = FALSE;
             if(finalMHlist[i].datatype == MH)
             {
                 for(l = 0; l < finalMHlist[i].num_treecodes; l++)
                 {
                     if(finalMHlist[i].treecodes[l][0] == 'B')  /* Organism Tree */
                     {
                         isOrganism = TRUE;
                         isModel = isModelOrganism(finalMHlist[i].dui);
                     } /* fi Organism */

                     else if(finalMHlist[i].treecodes[l][0] == 'C')  /* Disease Tree */
                       isDisease = TRUE;

                     else if(finalMHlist[i].treecodes[l][0] == 'D')  /* Chemical Tree */
                       isDisease = TRUE;
                 } /* for */
             } /* fi MH */

             else if(finalMHlist[i].datatype == NM)
             {
                 if(isFDOC_SCR(finalMHlist[i].mh))
                   isOKSCR = TRUE;
             } /* else */

             if(RTM_Debug)
               fprintf(fout, "O: %d|D: %d|C: %d|S: %d\n", isOrganism, isDisease,
                       isChemical, isOKSCR);

             if(isOrganism || isDisease || isChemical ||isOKSCR)
             {
                 /* 1) Found in Title with 1 or more occurrences */

                 if((strstr(finalMHlist[i].textloc, "TI") != NULL) && (finalMHlist[i].numPIs > 0))
                 {
                     finalMHlist[i].saveFDOC = TRUE;
                     if(RTM_Debug)
                     {
                         fprintf(fout, "FDOC Save Rule 1: %s\n", finalMHlist[i].mh);
                         fflush(fout);
                     } /* fi */
                 } /* fi TI Rule 1 */

                 /* 2) Found in Abstract with 2 or more occurrences */

                 else if((strstr(finalMHlist[i].textloc, "AB") != NULL) &&
                        (finalMHlist[i].numPIs > 1))
                 {
                     finalMHlist[i].saveFDOC = TRUE;
                     if(RTM_Debug)
                     {
                         fprintf(fout, "FDOC Save Rule 2: %s\n", finalMHlist[i].mh);
                         fflush(fout);
                     } /* fi */
                 } /* fi AB Rule 2 */

                 /* 3) In the event that we have no organisms indexed, if we find a MODEL organism
                       in the abstract but it only has one occurrence, ok to let it go through.
                       AND then IF AND ONLY IF it is one of the model organisms.

                       UPDATE: If any model organism if even only once in Abstract still add
                            November 19, 2021.
                 if((numOrganisms == 0) && isModel)
                 */

                 if(isModel)
                 {
                     if((strstr(finalMHlist[i].textloc, "AB") != NULL) &&
                        (finalMHlist[i].numPIs > 0))
                     {
                         finalMHlist[i].saveFDOC = TRUE;
                         if(RTM_Debug)
                         {
                             fprintf(fout, "FDOC Save Rule 3: %s\n", finalMHlist[i].mh);
                             fflush(fout);
                         } /* fi */
                     } /* fi AB Rule 3 */
                 } /* fi No Organisms & model */
             } /* fi found target type */
        } /* fi */
    } /* for */
} /* checkFDOC */

/***************************************************************************
*
*  isModelOrganism --
*
*      This 
*
***************************************************************************/

int isModelOrganism(char *dui)
{
    int rtn;

    /* Return true if we match dui to any of the model organisms -
         Humans|D006801
         Mice|D051379
         Rats|D051381
         Arabidopsis|D017360
         Drosophila|D004330
         Saccharomyces cerevisiae|D012441
         Caenorhabditis elegans|D017173
         Escherichia coli|D004926
         Zebrafish|D015027
         Xenopus|D014981
         Schizosaccharomyces|D012568

         Cats|D002415
         Cattle|D002417
         Dogs|D004285
         Cricetinae|D006224
         Rabbits|D011817
         Sheep|D012756
         Swine|D013552
         Chlorocebus aethiops|D002522
         Horses|D006736
         Bees|D001516
         Guinea Pigs|D006168
    */

    rtn = FALSE;
    if((strcmp(dui, "D006801") == 0) || (strcmp(dui, "D051379") == 0) ||
       (strcmp(dui, "D051381") == 0) || (strcmp(dui, "D017360") == 0) ||
       (strcmp(dui, "D004330") == 0) || (strcmp(dui, "D012441") == 0) ||
       (strcmp(dui, "D017173") == 0) || (strcmp(dui, "D004926") == 0) ||
       (strcmp(dui, "D015027") == 0) || (strcmp(dui, "D014981") == 0) ||
       (strcmp(dui, "D012568") == 0) ||

       /* Additional allowed as Model Organisms */

       (strcmp(dui, "D002415") == 0) || (strcmp(dui, "D002417") == 0) ||
       (strcmp(dui, "D004285") == 0) || (strcmp(dui, "D006224") == 0) ||
       (strcmp(dui, "D011817") == 0) || (strcmp(dui, "D006736") == 0) ||
       (strcmp(dui, "D012756") == 0) || (strcmp(dui, "D001516") == 0) ||
       (strcmp(dui, "D013552") == 0) || (strcmp(dui, "D006168") == 0) ||
       (strcmp(dui, "D002522") == 0))
      rtn = TRUE;

    return(rtn);
} /* isModelOrganism */


/***************************************************************************
*
*  isFDOC_SCR --
*
*      This 
*
***************************************************************************/

int isFDOC_SCR(char *name)
{
    char **rows;
    int n, k, rtn, class;

    /*
   1 - Regular Chemical,  Drug, or Substance
   2 - Protocol
   3 - Disease
   4 - Organism

   For 1, we want to try and break it down a bit more
      D08, D12 HMs -- Protein
      D26 HMs -- Drug
      D01, D02 HMs -- Chemical
      D09 -- Carbohydrates
      Other

   New Classes:

      40 - Protein
      50 - Drug
      60 - Chemical
      70 - Carbohydrates
      80 - Others

      For FDOC we want 1, 3, 4
    */

    rtn = FALSE;
    query_btree(SCRCLASS, name, &rows, &n);
    if((rows != NULL) && (n > 0))
    {
        class = -1;
        sscanf(rows[0], "%*[^|]|%d", &class);
        if((class == 1) || (class == 3) || (class == 4))
          rtn = TRUE;

        for(k = 0; k < n; k++)
          free(rows[k]);
        free(rows);
    } /* fi */

    else if(rows != NULL)
      free(rows);

    return(rtn);
} /* isFDOC_SCR */
