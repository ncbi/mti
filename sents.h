#define MAX_PHRASES 40
#define MAX_ELEMENTS 20

#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif

struct reqElemStruct {
  int found;
  int numPhrases;
  long phrasePos;
  int doPrefix;
  int doPostfix;
  int ppVariant;
  char phrases[MAX_PHRASES][125];
};

struct ptStruct {
  char PT[125];
  char UI[25];
};

struct lookStruct {
  int negFlag;
  int numPTs;
  struct ptStruct ptList[5];
  char loc[15];
  int numReq;
  int numFound;
  struct reqElemStruct reqElements[MAX_ELEMENTS];
};

static int numPP = 3;
static char *prePost[] = { "were", "are", "have been" };

/* For two or more PTs --
       { 2, {{ "Randomized Controlled Trial", "D016449" },  { "Clinical Trial", "D016430" }},
*/

static long numLookPTs = 13;
static struct lookStruct lookForPTs[13] = { 

       /* 0) we conducted a multi-centre randomised trial  - no prePost */
       { TRUE, 2, {{ "Randomized Controlled Trial", "D016449" }, {"Multicenter Study", "D016448"}},
              "TIAB", 4, 0,
          { 
             { -1, 11, -1, 0, 0, -1, { "we conducted", "we performed", "we present", "we report",
                                       "was carried out", "was carried-out",
                                       "we carried out", "we carried-out", "this", "our", "a"  }},
             { -1, 2, -1, 0, 0, -1,  { "randomized", "randomised" }},
             { -1, 12, -1, 0, 0, -1, { "multicenter", "multi-center", "multi center", 
                                       "multicentre",  "multi-centre", "multi centre",
                                       "multicentered", "multi-centered", "multi centered",
                                       "multicentred", "multi-centred", "multi centred" }},
             { -1, 2, -1, 0, 0, -1, { "trial", "study" }}
          }
       },

       /* 1) this clinical study was randomized  - no prePost */
       /* our randomized controlled trial  - no prePost */
       /* we conducted a randomized clinical study  - no prePost */
       { TRUE, 1, {{ "Randomized Controlled Trial", "D016449" }},
              "TIAB", 4, 0,
          { 
             { -1, 11, -1, 0, 0, -1, { "we conducted", "we performed", "we present", "we report",
                                       "was carried out", "was carried-out",
                                       "we carried out", "we carried-out", "this", "our", "a"  }},
             { -1, 2, -1, 0, 0, -1,  { "randomized", "randomised" }},
             { -1, 18, -1, 0, 0, -1, { "clinical", "control", "controlled", "prospective",
                                       "split-mouth", "split mouth", "splitmouth", 
                                       "single-blinded", "single blinded", "pilot",
                                       "single-masked", "single masked", "placebo",
                                       "double-blinded", "double blinded", 
                                       "cross-over", "cross over", "crossover" }},
             { -1, 2, -1, 0, 0, -1, { "trial", "study" }}
          }
       },

       /* 2) patients [were randomly] divided */
       { TRUE, 1, {{ "Randomized Controlled Trial", "D016449" }},
              "TIAB", 3, 0,
          { 
             { -1, 1, -1, 1, 0, -1,  { "randomly" }},
             { -1, 10, -1, 0, 0, -1, { "patients", "participants", "subjects", "children", "adults",
                                       "adolescents", "men", "women", "males", "females" }},
             { -1, 3, -1, 0, 0, -1,  { "assigned", "divided", "allocated" }}
          }
       },

       /* 3) [patients were] assigned randomly */
       { TRUE, 1, {{ "Randomized Controlled Trial", "D016449" }},
              "TIAB", 3, 0,
          { 
             { -1, 1, -1, 0, 0, -1,  { "randomly" }},
             { -1, 10, -1, 0, 1, -1, { "patients", "participants", "subjects", "children", "adults",
                                       "adolescents", "men", "women", "males", "females" }},
             { -1, 3, -1, 0, 0, -1,  { "assigned", "divided", "allocated" }}
          }
       },

       /* 4) patients [were assigned] randomly */
       { TRUE, 1, {{ "Randomized Controlled Trial", "D016449" }},
              "TIAB", 3, 0,
          { 
             { -1, 1, -1, 0, 0, -1,  { "randomly" }},
             { -1, 10, -1, 0, 0, -1, { "patients", "participants", "subjects", "children", "adults",
                                       "adolescents", "men", "women", "males", "females" }},
             { -1, 3, -1, 1, 0, -1,  { "assigned", "divided", "allocated" }}
          }
       },

       /* 5) patients [are randomized] */
       { TRUE, 1, {{ "Randomized Controlled Trial", "D016449" }},
              "TIAB", 2, 0,
          { 
             { -1, 2, -1, 1, 0, -1,  { "randomized", "randomised" }},
             { -1, 10, -1, 0, 0, -1, { "patients", "participants", "subjects", "children", "adults",
                                       "adolescents", "men", "women", "males", "females" }}
          }
       },

       /* 6) [women have been] randomized */
       { TRUE, 1, {{ "Randomized Controlled Trial", "D016449" }},
              "TIAB", 2, 0,
          { 
             { -1, 2, -1, 0, 0, -1,  { "randomized", "randomised" }},
             { -1, 10, -1, 0, 1, -1, { "patients", "participants", "subjects", "children", "adults",
                                       "adolescents", "men", "women", "males", "females" }}
          }
       },

       /* 7) nonrandomised interventional trial */
       { FALSE, 1, {{ "Clinical Trial", "D016430" }},
              "TIAB", 3, 0,
          { 
             { -1, 6, -1, 0, 0, -1,  { "non-randomized", "non randomized", "nonrandomized",
                                       "non-randomised", "non randomised", "nonrandomised" }},
             { -1, 8, -1, 0, 0, -1, { "before and after", "before & after", "before-and-after",
                                      "clinical", "interventional", "intervention", "cluster",
                                      "exploratory" }},
             { -1, 2, -1, 0, 0, -1, { "trial", "study" }}
          }
       },

       /* 8) non-randomised interventional controlled trial */
       { FALSE, 1, {{ "Controlled Clinical Trial", "D018848" }},
              "TIAB", 3, 0,
          { 
             { -1, 6, -1, 0, 0, -1,  { "non-randomized", "non randomized", "nonrandomized",
                                       "non-randomised", "non randomised", "nonrandomised" }},
             { -1, 1, -1, 0, 0, -1, { "controlled" }},
             { -1, 2, -1, 0, 0, -1, { "trial", "study" }}
          }
       },

       /* 9) this prospective, observational, multicenter study */
       { FALSE, 2, {{ "Observational Study", "D064888" }, {"Multicenter Study", "D016448"}},
              "TIAB", 5, 0,
          { 
             { -1, 2, -1, 0, 0, -1, { "this", "our" }},
             { -1, 3, -1, 0, 0, -1, { "prospective", "retrospective", "pilot" }},
             { -1, 1, -1, 0, 0, -1, { "observational" }},
             { -1, 15, -1, 0, 0, -1, { "multicenter", "multi-center", "multi center", 
                                       "multicentre",  "multi-centre", "multi centre",
                                       "multicentric", "multi-centric", "multi centric",
                                       "multicentered", "multi-centered", "multi centered",
                                       "multicentred", "multi-centred", "multi centred" }},
             { -1, 3, -1, 0, 0, -1, { "trial", "study", "analysis" }}
          }
       },

       /* 10) This multicentric double-blind preliminary study */
       { FALSE, 1, {{ "Multicenter Study", "D016448" }},
              "TIAB", 2, 0,
          { 
             { -1, 15, -1, 0, 0, -1, { 
                            "this multicenter", "this multi-center", "this multi center", 
                            "this multicentre",  "this multi-centre", "this multi centre",
                            "this multicentric", "this multi-centric", "this multi centric",
                            "this multicentered", "this multi-centered", "this multi centered",
                            "this multicentred", "this multi-centred", "this multi centred" }},
             { -1, 3, -1, 0, 0, -1, { "trial", "study", "analysis" }}
          }
       },

       /* 11) Observational, Pilot Trial */
       { FALSE, 1, {{ "Observational Study", "D064888" }},
              "TI", 3, 0,
          { 
             { -1, 1, -1, 0, 0, -1, { "observational" }},
             { -1, 4, -1, 0, 0, -1, { "prospective", "retrospective", "pilot", "longitudinal"  }},
             { -1, 3, -1, 0, 0, -1, { "trial", "study", "analysis" }}
          }
       },

       /* 12) Observational, Pilot Trial */
       { FALSE, 1, {{ "Observational Study", "D064888" }},
              "TIAB", 3, 0,
          { 
             { -1, 2, -1, 0, 0, -1, { "this", "our" }},
             { -1, 1, -1, 0, 0, -1, { "observational" }},
             { -1, 3, -1, 0, 0, -1, { "trial", "study", "analysis" }}
          }
       }
};


static long numLookMHs = 4;
static struct lookStruct lookForMHs[4] = { 

       { FALSE, 1, {{ "Patient Selection", "D018579" }}, "TI", 2, 0,
          { 
             { -1, 1, -1, 0, 0, -1, { "increasing recruitment"  }},
             { -1, 4, -1, 0, 0, -1, { "trials", "trial", "studies", "study" }}
          }
       },

       { FALSE, 1, {{ "Community Health Services", "D003153" }}, "TI", 2, 0,
          { 
             { -1, 2, -1, 0, 0, -1,  { "community-based", "community based" }},
             { -1, 2, -1, 0, 0, -1, { "services", "service" }}
          }
       },

       { FALSE, 1, {{ "Occupational Injuries", "D060051" }}, "TI", 2, 0,
          { 
             { -1, 2, -1, 0, 0, -1,  { "occupational-related", "occupational related" }},
             { -1, 2, -1, 0, 0, -1, { "injuries", "injury" }}
          }
       },

       { FALSE, 1, {{ "Orbital Neoplasms", "D009918" }}, "TI", 2, 0,
          { 
             { -1, 3, -1, 0, 0, -1,  { "orbitally", "orbital", "orbit" }},
             { -1, 4, -1, 0, 0, -1, { "metastatic", "carcinoma", "cancer", "neoplasm" }}
          }
       }
};


static long numLookCTs = 2;
static struct lookStruct lookForCTs[2] = { 

       { FALSE, 1, {{ "Male", "D008297" }}, "TIAB", 2, 0,
          { 
             { -1, 1, -1, 0, 0, -1, { "majority"  }},
             { -1, 4, -1, 0, 0, -1, { "were female", "were women", "are female", "are women" }}
          }
       },

       { FALSE, 1, {{ "Female", "D005260" }}, "TIAB", 2, 0,
          { 
             { -1, 1, -1, 0, 0, -1, { "majority"  }},
             { -1, 4, -1, 0, 0, -1, { "were male", "were men", "are male", "are men" }}
          }
       }
};
