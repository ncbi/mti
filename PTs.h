
/**************************************************************************

  NOTE: Recompile findHighlightPTs.c in
        /nfsvol/nls/II_Group_WorkArea/Condor_WorkArea/DCMS/MTI_AUTO_2016/src

  NOTE: for Multicenter Study we don't want the plural version of triggers
        hence the ignore entries below.

  NOTE: controlled clinical trials was added since the plural tends to incorrectly
        trigger the singular.

  NOTE: systematic reviews was added since the plural tends to incorrectly
        trigger the singular.

  NOTE: Preferred Reporting Items for Systematic reviews and Meta-Analyses (PRISMA) Guidelines

  MISC.
       "'%s'"
       "\"%s\""
       "makes %s inappropriate"

**************************************************************************/

static long numIgnore = 266;
static char *IgnoreList[] = { "preferred reporting items for systematic reviews and meta-analysis", "preferred reporting items for systematic reviews and meta analysis", "preferred reporting items for systematic reviews and meta analysis", "-two", "a systematic review protocol", "case report form", "case report forms", "case study", "case-report form", "case-report forms", "chart review", "charts review", "controlled clinical trials", "future challenges in randomized", "future directions for randomized", "future group randomized", "future interventional randomized", "future prospective randomized", "future randomized", "in case of", "in case", "insufficient for a systematic review", "multi center studies", "multi center trials", "multi centre studies", "multi centre trials", "multi-center studies", "multi-center trials", "multi-centre studies", "multi-centre trials", "multicenter studies", "multicenter trials", "multicentre studies", "multicentre trials", "non systematic", "non-systematic", "patient advocate", "patient centered", "patient centred", "patient centric", "patient cohort", "patient derived", "patient held", "patient portal", "patient registry", "patient reported", "patient satisfaction", "patient specific", "patient survey", "patient tailored", "patient tracing", "patient-centered", "patient-centred", "patient-centric", "patient-derived", "patient-held", "patient-reported", "patient-specific", "patient-tailored", "phase 1 and phase 2 metabolite", "phase 1 and phase 2 metabolites", "phase 1 conjugate", "phase 1 conjugates", "phase 1 derivative", "phase 1 derivatives", "phase 1 lockdown", "phase 1 metabolism", "phase 1 metabolite", "phase 1 metabolites", "phase 1 post lockdown", "phase 2 conjugate", "phase 2 conjugates", "phase 2 derivative", "phase 2 derivatives", "phase 2 lockdown", "phase 2 metabolite", "phase 2 metabolites", "phase 2 post lockdown", "phase i and phase ii metabolite", "phase i and phase ii metabolites", "phase i conjugate", "phase i conjugates", "phase i derivative", "phase i derivatives", "phase i metabolism", "phase i metabolite", "phase i metabolites", "phase ii conjugate", "phase ii conjugates", "phase ii derivative", "phase ii derivatives", "phase ii metabolism", "phase ii metabolite", "phase ii metabolites", "phase-1 and phase-2 metabolite", "phase-1 and phase-2 metabolites", "phase-1 conjugate", "phase-1 conjugates", "phase-1 derivative", "phase-1 derivatives", "phase-1 lockdown", "phase-1 metabolism", "phase-1 metabolite", "phase-1 metabolites", "phase-1 post-lockdown", "phase-2 conjugate", "phase-2 conjugates", "phase-2 derivative", "phase-2 derivatives", "phase-2 lockdown", "phase-2 metabolite", "phase-2 metabolites", "phase-2 post-lockdown", "phase-i and phase-ii metabolite", "phase-i and phase-ii metabolites", "phase-i conjugate", "phase-i conjugates", "phase-i derivative", "phase-i derivatives", "phase-i metabolism", "phase-i metabolite", "phase-i metabolites", "phase-ii conjugate", "phase-ii conjugates", "phase-ii derivative", "phase-ii derivatives", "phase-ii metabolism", "phase-ii metabolite", "phase-ii metabolites", "preferred reporting items for systematic reviews and meta analyses", "preferred reporting items for systematic reviews and meta-analyses", "preferred reporting items for systematic reviews and metaanalyses", "protocol for a systematic review", "protocol for systematic review", "pseudo-random", "random access assay", "random access memory", "random access", "random agreement", "random agreements", "random alarm", "random amplification", "random amplified", "random attack", "random attacks", "random blood", "random bootstrap sample", "random bootstrap samples", "random bootstrap", "random change", "random changes", "random cluster sampling", "random cluster", "random coil", "random copolymer", "random copolymers", "random dataset", "random datasets", "random detection", "random dynamic", "random dynamics", "random effect", "random effects model", "random effects", "random feature", "random features", "random field", "random fields", "random forest", "random forests", "random genetic drift", "random guess", "random insertion", "random insertions", "random intercept", "random intercepts", "random k fold", "random k-fold", "random kfold", "random location", "random locations", "random machine", "random motion", "random noise", "random number", "random oracle", "random oracles", "random sample", "random sampling", "random signal", "random signals", "random slope", "random slopes", "random survival", "random telephone", "random variable", "random variables", "random variation", "random variations", "random walk", "random walks", "random-effect", "random-effects", "random-forest", "random-forests", "random-intercept", "randomised controlled studies", "randomised pre clinical", "randomised pre-clinical", "randomised preclinical", "randomised-controlled studies", "randomized controlled studies", "randomized forest", "randomized forests", "randomized future conditions", "randomized pre clinical", "randomized pre-clinical", "randomized preclinical", "randomized trials", "randomized-controlled studies", "randomly sampled", "randomness", "rats were randomly", "recent meta analysis", "recent meta-analysis", "recent metaanalysis", "review of chart", "review of charts", "systematic reviews", "time trial", "time trials", "time-trial", "time-trials", "trial & error", "trial and error", "trial registration information", "trial registration isrctn", "trial registration no", "trial registration number", "trial registration numbers", "trial registration url", "trial registration- url", "trial registration:", "trial-and-error", "we reviewed the record", "we reviewed the records", "peer review", "observational learning", "future definitive randomized", "future definitive randomised", "multiple-center nomogram", "multiple center nomogram", "retrospective review", "interview study", "systematic reviews and meta-analysis guidelines", "systematic reviews and meta-analysis guideline", "systematic review and meta-analysis guidelines", "systematic review and meta-analysis guideline", "systematic reviews and meta analysis guidelines", "systematic reviews and meta analysis guideline", "systematic review and meta analysis guidelines", "systematic review and meta analysis guideline", "systematic reviews and metaanalysis guidelines", "systematic reviews and metaanalysis guideline", "systematic review and metaanalysis guidelines", "systematic review and metaanalysis guideline"
};


/* Prefix Negation phrases for PTs - Make sure you have "%s<space>" or "%s<dash>"
     "%s context"
 */

static long numPrefix_PTNegs = 41;
static char *PT_NegPrefix[] = { " context", " could not", "-free", " free", "-inspired", " inspired", " is needed", " is required", " is still needed", " is still required", " is unfeasible", " must be carried out", " must be carried-out", " must be conducted", " must be performed", " must still be carried out", " must still be carried-out", " must still be conducted", " must still be performed", " needs to be carried out", " needs to be carried-out", " needs to be conducted", " needs to be performed", " principle", " principles", " was not done", " was not performed", " was not possible", " was not", " was unfeasible", " was not conducted", " was not carried out", " was not carried-out", " was not feasible", " was not required", " are needed", " are still needed", " are required", " are unfeasible", " are warranted", " are unwarranted"};


/* Postfix Negation phrases for PTs - Make sure you have "<space>%s" or "<dash>%s"
     "anti %s"
 */

static long numPostfix_PTNegs = 240;
static char *PT_NegPostfix[] = { 
"anti ",
"anti-",
"could not attempt ",
"could not attempt a ",
"could not attempt an ",
"could not attempt the ",
"couldn't attempt ",
"couldn't attempt a ",
"couldn't attempt an ",
"couldn't attempt the ",
"did not attempt ",
"did not attempt a ",
"did not attempt an ",
"did not attempt the ",
"did not carry out ",
"did not carry out an ",
"did not carry out the ",
"did not carry-out ",
"did not carry-out a ",
"did not carry-out an ",
"did not carry-out the ",
"did not conduct ",
"did not conduct a ",
"did not conduct an ",
"did not conduct the ",
"did not perform ",
"did not perform a ",
"did not perform an ",
"did not perform the ",
"did not permit ",
"did not permit a ",
"did not permit an ",
"did not permit the ",
"didn't attempt ",
"didn't attempt a ",
"didn't attempt an " ,
"didn't attempt the ",
"didn't carry out ",
"didn't carry out a ",
"didn't carry out an ",
"didn't carry out the ",
"didn't carry-out ",
"didn't carry-out a ",
"didn't carry-out an ",
"didn't carry-out the ",
"didn't conduct ",
"didn't conduct a ",
"didn't conduct an ",
"didn't conduct the ",
"didn't perform ",
"didn't perform a ",
"didn't perform an ",
"didn't perform the ",
"excluded ",
"false ",
"false-",
"future studies with ",
"future studies with a ",
"future studies with an ",
"future studies with the ",
"future trials with ",
"future trials with a ",
"future trials with an ",
"future trials with the ",
"in future ",
"in the future ",
"instead of ",
"lack of  ",
"lack of ",
"lack of a  ",
"lack of a ",
"lack of an  ",
"lack of an ",
"lack of the ",
"meta analysis of ",
"meta analysis of a ",
"meta analysis of an ",
"meta analysis of the ",
"meta-analysis of ",
"meta-analysis of a ",
"meta-analysis of an ",
"meta-analysis of the ",
"metaanalysis of ",
"metaanalysis of a ",
"metaanalysis of an ",
"metaanalysis of the ",
"no ",
"no-",
"non ",
"non-",
"none of the studies are suitable for ",
"none of the studies are suitable for a ",
"none of the studies are suitable for an ",
"none of the studies are suitable for the ",
"none of the studies were suitable for ",
"none of the studies were suitable for a ",
"none of the studies were suitable for an ",
"none of the studies were suitable for the ",
"none of the trials are suitable for ",
"none of the trials are suitable for a ",
"none of the trials are suitable for an ",
"none of the trials are suitable for the ",
"none of the trials were suitable for ",
"none of the trials were suitable for a ",
"none of the trials were suitable for an ",
"none of the trials were suitable for the ",
"not ",
"not a ",
"not amenable to ",
"not pooled for ",
"not possible to carry out ",
"not possible to carry out a ",
"not possible to carry out an ",
"not possible to carry out the ",
"not possible to conduct ",
"not possible to conduct a ",
"not possible to conduct an ",
"not possible to conduct the ",
"not possible to perform ",
"not possible to perform a ",
"not possible to perform an ",
"not possible to perform the ",
"not-",
"not-amenable to ",
"preclude ",
"preclude a ",
"preclude an ",
"preclude the ",
"precluded ",
"precluded a ",
"precluded an ",
"precluded the ",
"precluded meaningful pooling and ",
"precluded meaningful pooling or ",
"precludes ",
"precludes a ",
"precludes an ",
"precludes meaningful pooling and ",
"precludes meaningful pooling or ",
"precludes the ",
"prevented ",
"prevented a ",
"prevented an ",
"prevented the ",
"previous ",
"rather than ",
"rather than a ",
"rather than an ",
"rather than the ",
"recent ",
"recently carried out ",
"recently carried out a ",
"recently carried out an ",
"recently carried out the ",
"recently carried-out ",
"recently carried-out a ",
"recently carried-out an ",
"recently carried-out the ",
"recently conducted ",
"recently conducted a ",
"recently conducted an ",
"recently conducted the ",
"recently performed ",
"recently performed a ",
"recently performed an ",
"recently performed the ",
"recently published ",
"recently published comprehensive ",
"study was not suitable for ",
"study was not suitable for a ",
"study was not suitable for an ",
"study was not suitable for the ",
"systematic review of ",
"systematic review of a ",
"systematic review of an ",
"systematic review of the ",
"the studies are not suitable for ",
"the studies are not suitable for a ",
"the studies are not suitable for an ",
"the studies are not suitable for the ",
"the studies were not suitable for ",
"the studies were not suitable for a ",
"the studies were not suitable for an ",
"the studies were not suitable for the ",
"the trials are not suitable for ",
"the trials are not suitable for a ",
"the trials are not suitable for an ",
"the trials are not suitable for the ",
"the trials were not suitable for ",
"the trials were not suitable for a ",
"the trials were not suitable for an ",
"the trials were not suitable for the ",
"there is a need for ",
"there is a need for a ",
"there is a need for an ",
"there is a need for the ",
"these results must be validated in ",
"these results must be validated in a ",
"these results must be validated in an ",
"these results must be validated in the ",
"these results need to be validated in ",
"these results need to be validated in a ",
"these results need to be validated in an ",
"these results need to be validated in the ",
"these results should be validated in ",
"these results should be validated in a ",
"these results should be validated in an ",
"these results should be validated in the ",
"trial was not suitable for ",
"trial was not suitable for a ",
"trial was not suitable for an ",
"trial was not suitable for the ",
"unable to carry out ",
"unable to carry out a ",
"unable to carry out an ",
"unable to carry out the ",
"unable to do ",
"unable to do a ",
"unable to do an ",
"unable to do the ",
"unable to peform ",
"unable to peform a ",
"unable to peform an ",
"unable to peform the ",
"without ",
"without a ",
"without an ",
"without the ",
"needs to be confirmed in ",
"needs to be confirmed in a ",
"needs to be confirmed in an ",
"needs to be confirmed in the ",
"needs to be confirmed by ",
"needs to be confirmed by a ",
"needs to be confirmed by an ",
"needs to be confirmed by the ",
"needs to be confirmed with ",
"needs to be confirmed with a ",
"needs to be confirmed with an ",
"needs to be confirmed with the "
};


/* Forced PTs for a given journal based on whether or not it is Title Only or
   has both Title & Abstract.

   TIO, TIAB, ANY
*/

struct ptLocalStruct {
  char PT[125];
  char UI[25];
};

struct forceStruct {
  char NlmID[50];
  int numPTs;
  struct ptLocalStruct ptList[5];
  char checkLoc[10];
};

/* Check to see if this is a journal that we want to go ahead and just force a PT (or more)
       based only in the NlmID.

       BMJ Case Rep|101526291 - Trigger Case Reports with any configuration
       Handb Clin Neurol|0166161 - Trigger Review with any configuration
*/
static long numForcedPTs = 2;
static struct forceStruct forcePTs[2] = { 
       { "101526291", 1, {{ "Case Reports", "D002363" }}, "ANY" },
       { "0166161", 1, {{ "Review", "D016454" }}, "ANY" },
};
