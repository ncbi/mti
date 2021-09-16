static long numBadLeaf = 690;
static char *badLeaves[] = { "a  80915b",
     "a 007",
     "a 0105n",
     "a 033",
     "a 078",
     "a 10",
     "a 10",
     "a 101",
     "a 10255",
     "a 103",
     "a 105972",
     "a 1062",
     "a 10749",
     "a 1079",
     "a 108835",
     "a 108836",
     "a 10947",
     "a 1110u",
     "a 113",
     "a 121",
     "a 1226",
     "a 125800",
     "a 127722",
     "a 130",
     "a 130b",
     "a 130c",
     "a 131701",
     "a 134974",
     "a 1354",
     "a 137",
     "a 144",
     "a 145",
     "a 147627",
     "a 157378.0",
     "a 159",
     "a 159470",
     "a 161906",
     "a 16316 c",
     "a 16686",
     "a 16886b",
     "a 17",
     "a 17",
     "a 17002 c",
     "a 170634",
     "a 172013",
     "a 177430",
     "a 182086",
     "a 185 cpd",
     "a 1899",
     "a 19",
     "a 19",
     "a 192",
     "a 192411.29",
     "a 192558",
     "a 192621",
     "a 195",
     "a 195,773",
     "a 195773",
     "a 201a",
     "a 204",
     "a 20668",
     "a 20812",
     "a 20832",
     "a 21459a",
     "a 21459b",
     "a 215",
     "a 216546",
     "a 2174",
     "a 218",
     "a 2186",
     "a 21960",
     "a 21978c1",
     "a 22700",
     "a 228839",
     "a 23187",
     "a 23189",
     "a 2337",
     "a 23871",
     "a 23887",
     "a 2545",
     "a 25794",
     "a 25822",
     "a 25822b",
     "a 259745",
     "a 26771 a",
     "a 269a",
     "a 272",
     "a 27223",
     "a 274",
     "a 278637",
     "a 285222",
     "a 286,982",
     "a 286982",
     "a 289099",
     "a 300",
     "a 301",
     "a 30312",
     "a 304000",
     "a 304121",
     "a 30641",
     "a 308165",
     "a 31472",
     "a 315675",
     "a 317920",
     "a 3217",
     "a 325100",
     "a 3253",
     "a 3331",
     "a 336",
     "a 338",
     "a 33853",
     "a 350619",
     "a 3665",
     "a 36683",
     "a 366833",
     "a 37536",
     "a 37812",
     "a 381,393",
     "a 381393",
     "a 38503",
     "a 385358",
     "a 39355",
     "a 40926",
     "a 41030",
     "a 41304",
     "a 4166",
     "a 419259",
     "a 42867",
     "a 432411",
     "a 4334",
     "a 435",
     "a 437203",
     "a 43818",
     "a 443654",
     "a 45474",
     "a 45975",
     "a 489",
     "a 49816",
     "a 500359b",
     "a 503",
     "a 503083 a",
     "a 503083 b",
     "a 503083 e",
     "a 503083 f",
     "a 51493a",
     "a 53693",
     "a 53868a",
     "a 53930a",
     "a 53930b",
     "a 53930c",
     "a 54145",
     "a 54741",
     "a 55453",
     "a 5610",
     "a 56234",
     "a 56268",
     "a 56619",
     "a 56620",
     "a 57219",
     "a 57696",
     "a 58365a",
     "a 58365b",
     "a 585",
     "a 585539",
     "a 601",
     "a 602",
     "a 603",
     "a 60586",
     "a 60969",
     "a 61603",
     "a 61827",
     "a 62095",
     "a 62176",
     "a 62198",
     "a 62254",
     "a 62671",
     "a 63075",
     "a 63162",
     "a 63387",
     "a 64077",
     "a 643c",
     "a 64662",
     "a 65186",
     "a 65281",
     "a 65282",
     "a 65317",
     "a 6599",
     "a 66193",
     "a 66359",
     "a 66649",
     "a 68552",
     "a 68567",
     "a 68828",
     "a 68930",
     "a 69024",
     "a 69328",
     "a 69412",
     "a 69992",
     "a 7",
     "a 70104",
     "a 70108",
     "a 70360",
     "a 70450",
     "a 705239",
     "a 70874",
     "a 71378",
     "a 71497",
     "a 71623",
     "a 71915",
     "a 72517",
     "a 73025",
     "a 73209",
     "a 741439",
     "a 74273",
     "a 74283",
     "a 747",
     "a 74704",
     "a 748",
     "a 74932",
     "a 751",
     "a 75179",
     "a 75200",
     "a 753",
     "a 75925",
     "a 75943",
     "a 75962",
     "a 75998",
     "a 76154",
     "a 76889",
     "a 76928",
     "a 769662",
     "a 77003",
     "a 770041",
     "a 771726",
     "a 77636",
     "a 778",
     "a 778317",
     "a 778935",
     "a 779",
     "a 784168",
     "a 78773",
     "a 790742",
     "a 79175",
     "a 792611",
     "a 79285",
     "a 793394",
     "a 794278",
     "a 794282",
     "a 795614",
     "a 80 2715",
     "a 802715",
     "a 803401",
     "a 803467",
     "a 80556",
     "a 80915",
     "a 80915a",
     "a 80987",
     "a 80b",
     "a 81282",
     "a 816719.1",
     "a 81988",
     "a 82516",
     "a 82548a",
     "a 82695",
     "a 827",
     "a 82846a",
     "a 82846b",
     "a 83016a",
     "a 83016f",
     "a 83094a",
     "a 83586 c",
     "a 83586c",
     "a 837093",
     "a 84441",
     "a 844606",
     "a 84538",
     "a 84543",
     "a 850002",
     "a 85226",
     "a 85380",
     "a 854777",
     "a 85653",
     "a 85783.0",
     "a 85917",
     "a 86415",
     "a 86719.1",
     "a 867744",
     "a 86929",
     "a 87049",
     "a 887826",
     "a 8947",
     "a 90289a",
     "a 90289b",
     "a 90720a",
     "a 90931 a",
     "a 90931a",
     "a 91",
     "a 91",
     "a 9145",
     "a 9145c",
     "a 924",
     "a 93176",
     "a 93177",
     "a 93178",
     "a 935142",
     "a 9387",
     "a 94964",
     "a 966492",
     "a 967079",
     "a 987306",
     "a 98881",
     "a dna",
     "a fiber",
     "a form dna",
     "a gene",
     "a thalassemia",
     "abate",
     "ability",
     "absorption",
     "academic training",
     "academy",
     "accent",
     "accounting",
     "acupuncture",
     "adjustment",
     "adoption",
     "advantage",
     "after treatment",
     "aid",
     "ailing",
     "alcoholic",
     "alien",
     "allergy and immunology",
     "ally",
     "ambush",
     "amelia",
     "america",
     "amyloid",
     "antigen",
     "antral",
     "api",
     "aspiration (psychology)",
     "aspirations (psychology)",
     "association",
     "attitude",
     "austin",
     "bailing",
     "binocular",
     "biochemistry",
     "biofeedback (psychology)",
     "biofeedbacks (psychology)",
     "black and white",
     "bonding (psychology)",
     "bondings (psychology)",
     "brace",
     "brand",
     "broom",
     "budget",
     "bylaw",
     "callisto",
     "calm x",
     "cardiac electrophysiology",
     "cardiac surgery",
     "carrying",
     "causality",
     "causation",
     "character",
     "chronology",
     "clinical research",
     "co dependence (psychology)",
     "co dependences (psychology)",
     "co dependencies (psychology)",
     "co dependency (psychology)",
     "codependencies (psychology)",
     "codependency (psychology)",
     "coding",
     "coin",
     "color",
     "comb",
     "commit",
     "companion",
     "compensation",
     "competence",
     "compliance",
     "concentration",
     "conflict",
     "conflict (psychology)",
     "conflicts (psychology)",
     "constitution",
     "contract",
     "cope",
     "counter",
     "counter",
     "countertransference (psychology)",
     "countertransferences (psychology)",
     "critical period",
     "critical period (psychology)",
     "critical periods (psychology)",
     "crowding",
     "cyfra 21-1",
     "cyfra 21.1",
     "cyfra21-1",
     "cyfra21.1",
     "cyp3a4 protein, mouse",
     "cytochrome p450 3a4",
     "cytogenetic",
     "data set",
     "dataset",
     "denial",
     "denial (psychology)",
     "denials (psychology)",
     "dependencies (psychology)",
     "dependency",
     "dependency (psychology)",
     "desensitization (psychology)",
     "desensitizations (psychology)",
     "diane",
     "dislocation",
     "drama",
     "drb",
     "dream",
     "drill",
     "drinking",
     "dual diagnoses (psychiatry)",
     "dual diagnosis (psychiatry)",
     "ear",
     "ect (psychotherapy)",
     "edu",
     "educational personnel",
     "educational technic",
     "educational technique",
     "efficiency",
     "emergency",
     "empowerment",
     "endocrinology",
     "ensure",
     "ensure",
     "epigenetic",
     "europa",
     "exercise",
     "extinction (psychology)",
     "extinctions (psychology)",
     "extraversion (psychology)",
     "extraversions (psychology)",
     "extroversion (psychology)",
     "extroversions (psychology)",
     "familiarity",
     "fatigue",
     "feral",
     "fetishism (psychiatric)",
     "fetishisms (psychiatric)",
     "fever",
     "flexibility",
     "floor",
     "flushing",
     "fly",
     "forecasting",
     "fostering",
     "foundation",
     "ftp",
     "future",
     "future",
     "ganglion",
     "ganymede",
     "gender",
     "generalization",
     "generalization (psychology)",
     "generalizations (psychology)",
     "genome",
     "grant",
     "green or",
     "growth charts as topic",
     "habituation (psychophysiology)",
     "habituations (psychophysiology)",
     "handling",
     "handling (psychology)",
     "handlings (psychology)",
     "harman",
     "health benefit",
     "heart surgery",
     "heat wave",
     "hip",
     "hybrid",
     "hypersensitivity",
     "hyperthermia",
     "icon",
     "id",
     "imageries (psychotherapy)",
     "imagery",
     "imagery (psychotherapy)",
     "immunity",
     "imprinting (psychology)",
     "imprintings (psychology)",
     "in migration",
     "inflation",
     "inhalation",
     "inhaling",
     "inhibition",
     "inhibition (psychology)",
     "inhibitions (psychology)",
     "introversion (psychology)",
     "introversions (psychology)",
     "io",
     "jupiter",
     "kinase",
     "knee",
     "knowledge",
     "knowledge of results (psychology)",
     "korea",
     "lab",
     "lab",
     "lasso",
     "latency period",
     "latency period (psychology)",
     "latency periods (psychology)",
     "legitimacy",
     "let",
     "link",
     "link",
     "longum",
     "lymph",
     "magic",
     "magnetic",
     "manna",
     "matador",
     "mcat",
     "mediating",
     "melting",
     "metabolomic",
     "metabonomic",
     "metagenomic",
     "metric",
     "mimic",
     "mining",
     "minor",
     "multifactorial causality",
     "natural history",
     "nature",
     "neck",
     "nondisjunction",
     "observation",
     "observational study",
     "orange",
     "orf",
     "orientation",
     "origin of life",
     "orphan",
     "orthopedic",
     "p 128",
     "p 230",
     "p128",
     "p230",
     "packaging",
     "paper",
     "patient agent",
     "pedagogy",
     "personal power",
     "pet",
     "ph",
     "phoebe",
     "phototoxicity",
     "place",
     "planum",
     "pontal",
     "power",
     "power (psychology)",
     "powers (psychology)",
     "practice (psychology)",
     "practices (psychology)",
     "pragmatic clinical trial",
     "prebiotic",
     "president",
     "president",
     "productivity",
     "prohibit",
     "proven",
     "proxy",
     "pulse",
     "quad",
     "quadruplet",
     "radio",
     "ramp",
     "recognition (psychology)",
     "recurrence",
     "recycling",
     "redux",
     "refusal",
     "regression",
     "regression (psychology)",
     "regressions (psychology)",
     "rejection",
     "rejection (psychology)",
     "rejections (psychology)",
     "relate",
     "renaissance",
     "report",
     "resource",
     "rest",
     "results knowledge (psychology)",
     "results knowledges (psychology)",
     "retention",
     "retention (psychology)",
     "retentions (psychology)",
     "reticulum",
     "retinal",
     "risk",
     "rodent",
     "roundup",
     "runaway",
     "rupture",
     "saturn",
     "self assessment (psychology)",
     "self assessments (psychology)",
     "senescence",
     "sentinel",
     "sentinel",
     "separated",
     "separation",
     "serology",
     "serum",
     "serum",
     "set",
     "set (psychology)",
     "sets (psychology)",
     "sex",
     "shadowing",
     "sham treatment",
     "signal detection (psychology)",
     "signal detections (psychology)",
     "simplex",
     "social power",
     "song",
     "specialist",
     "specialization",
     "spotting",
     "squad",
     "starvation",
     "stigmata",
     "suffering",
     "sural",
     "survival",
     "symbiotic relation (psychology)",
     "symbiotic relations (psychology)",
     "symbolism (psychology)",
     "symbolisms (psychology)",
     "syndrome",
     "talent",
     "talon",
     "tanning",
     "task force",
     "tax",
     "teaching",
     "teaching method",
     "tear",
     "teeth",
     "tetra",
     "tha",
     "timeline",
     "titan",
     "tooth",
     "track",
     "trainers training",
     "training activity",
     "training technic",
     "training technique",
     "transfer",
     "transfer (psychology)",
     "transfers (psychology)",
     "transient",
     "translating",
     "tree shrew",
     "triplet",
     "trust",
     "ultrafine",
     "unconscious (psychology)",
     "understanding",
     "undertaking",
     "universal",
     "universal",
     "urso",
     "venom",
     "ventilation",
     "vermin",
     "vessel",
     "vision",
     "walker",
     "white",
     "will",
     "woodbine",
     "zona" };

static int numLeafNoSingular = 219;
static char *noLeafSingles[] = { 
     "feathers",
     "streams",
     "bats",
     "eagles",
     "elephants",
     "extracts",
     "forests",
     "friends",
     "hedgehogs",
     "horses",
     "individuals",
     "languages",
     "meadows",
     "migrants",
     "monkeys",
     "mothers",
     "mushrooms",
     "oranges",
     "orbits",
     "parents",
     "rivers",
     "screws",
     "slaves",
     "snails",
     "snakes",
     "soldiers",
     "spices",
     "starlings",
     "tablets",
     "tigers",
     "nurses",
     "stings",
     "strokes",
     "lungs",
     "blacks",
     "licences",
     "licenses",
     "imaps",
     "bidens",
     "masks",
     "spays",
     "birth defects",
     "yeasts",
     "mails",
     "injectables",
     "gelatinases",
     "asians",
     "apes",
     "moods",
     "linens",
     "fires",
     "cats",
     "central nervous systems",
     "environmental impacts",
     "epithelial tissues",
     "femurs",
     "turkeys",
     "cyclin-dependent kinases",
     "cyclin dependent kinases",
     "longitudinal results",
     "cardiac devices",
     "caterpillars",
     "neurosciences",
     "donor livers",
     "phosphorus diets",
     "power sources",
     "neuronal diseases",
     "geriatrics",
     "hollies",
     "leaves",
     "argas",
     "nomads",
     "dops",
     "larks",
     "pilots",
     "pediatrics",
     "paediatrics",
     "valvular interstitial cells",
     "epidemics",
     "colds",
     "sdgs",
     "masks",
     "seasonal dynamics",
     "access to medicines",
     "species' ranges",
     "species ranges",
     "platelets",
     "paintings",
     "hepatos",
     "guns",
     "gums",
     "onlays",
     "sars",
     "cribs",
     "aardvarks",
     "hips",
     "baths",
     "cigarettes",
     "tsunamis",
     "chills",
     "needles",
     "boats",
     "feelings",
     "nails",
     "violets",
     "robins",
     "alcohols",
     "diureses",
     "rafts",
     "colds",
     "paracrine factors",
     "cosmetics",
     "das",
     "gis",
     "uniforms",
     "minors",
     "falls",
     "steppes",
     "syndromes",
     "alcoholics",
     "capsules",
     "powders",
     "influentials",
     "ointments",
     "antioxidants",
     "adoptions",
     "opioids",
     "allergens",
     "androgens",
     "alders",
     "orphans",
     "colors",
     "hops",
     "endodontics",
     "foundations",
     "natives",
     "pulses",
     "radios",
     "suspensions",
     "translations",
     "ventilations",
     "specialists",
     "shadowings",
     "marines",
     "acyl homoserine lactones",
     "alginates",
     "amyloid fibrils",
     "chlorides",
     "collaborations",
     "esters",
     "shotguns",
     "firearms",
     "rifles",
     "geriatrics",
     "glucocorticoids",
     "honeys",
     "islands",
     "kinetics",
     "ligands",
     "nucleosides",
     "obstetrics",
     "photonics",
     "orthodontics",
     "phospholipids",
     "photons",
     "phytochemicals",
     "proteomics",
     "swallows",
     "conjugate vaccines",
     "workloads",
     "xenobiotics",
     "electrostatics",
     "sisters",
     "saints",
     "prescription drugs",
     "polyphenols",
     "phosphines",
     "neurotoxins",
     "nitrates",
     "mitogens",
     "diphosphates",
     "antithrombins",
     "antipsychotics",
     "antifibrinolytics",
     "peanuts",
     "cathelicidins",
     "corrosives",
     "cheeses",
     "cytostatics",
     "depressions",
     "insignias",
     "emulsions",
     "explosives",
     "flours",
     "ballistics",
     "glucuronides",
     "hypnotics",
     "influenzas",
     "cyclams",
     "lipopolysaccharides",
     "rotations",
     "torques",
     "rights",
     "toness",
     "pediatrics",
     "paediatrics",
     "ppars",
     "salts",
     "16p11.2 deletions",
     "yellow fevers",
     "bulgarians",
     "danes",
     "finns",
     "icelanders",
     "swedes",
     "serbs",
     "black africans",
     "turks",
     "microsporas"
};
