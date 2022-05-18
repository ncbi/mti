/* If not found regular, look for special cases
aged >= 60 years
aged 50 years and older
aged 8-12 years
aged 15 to 54 years
mean age of 67.9 years (range 62-74 years)
mean age at the time of surgery was 71.9 (range 46-88)
age ranged from 14 to 61 years old, with an average age of 42.7 +/- 11.93 years
age ranges from 20 to 40 years
age at the time of surgery ranged from 5 to 47 years old
age range 23-83 years; mean age 54.5 +/- 17.6 years
mean age 66 years; range 48-77 years
age: range 62-96 years, mean 78 +/- 12.5 years
were age between 19 and 65 years
aged between 6 and 20 years
young age group (mean age 21 years; range 17-25 years)
median age was 28 years (iqr 23-34 years)
age between 24 and 94 years(mean age: 59.21 +/- 12.06 years)
average age of 55.88 +/- 7.35 years (range, 42-71 years)
between 42 and 78 years of age (with a mean of 56 +/- 4)
at age 40 years
age 18-25
aged 4 to 95  (47.6+/-11.8) years

Infant, Newborn (0 - 1 month)
Infant (1 month - 23 months)
Child, Preschool (2 - 5)
Child (6 - 12)
Adolescent (13 - 18)
Young Adult (19 - 24)
Adult (19 - 44)
Middle Aged (45 - 63)
Aged (64 - 79)
Aged, 80 and over (80+)
*/

static long numAgeLookForRight = 53;
static char *ageTriggersRight[] = {
"ages ranged between",
"between the ages of",
"between the ages",
"between the age",
"average age was",
"average age of",
"median age was",
"age group of",
"aged  ranged",
"aged  ranges",
"aged between",
"ages  ranged",
"ages  ranges",
"ages between",
"mean age was",
"median  aged",
"median  ages",
"average age",
"age groups:",
"age  ranged",
"age  ranges",
"age between",
"aged ranged",
"aged ranges",
"ages  range",
"ages ranged",
"ages ranges",
"mean age of",
"median  age",
"median aged",
"median ages",
"age  range",
"age ranged",
"age ranges",
"aged range",
"ages range",
"mean  aged",
"mean  ages",
"median age",
"age range",
"mean  age",
"mean aged",
"mean ages",
"mean age",
"at age",
"aged:",
"ages:",
"ages,",
"age:",
"age,",
"aged",
"ages",
"age"
};

/* If not found regular, look for special cases
         two-year-old boy|man|male
         two-year-old girl|woman|female

         two year old boy|man|male
         two year old girl|woman|female

         twelve-years-old boy|man|male
         twelve-years-old girl|woman|female

         twelve years old boy|man|male
         twelve years old girl|woman|female

         twenty-two-years-old boy|man|male|gentleman
         twenty-two-years-old girl|woman|female|lady

         twenty-two-yrs-old boy|man|male
         twenty-two-yrs-old girl|woman|female

         twenty two years old boy|man|male
         twenty two years old girl|woman|female

         19-month-old girl|female
         19-month-old boy|male

         19-months-old girl|female
         19-months-old boy|male

         18-30 age group
         18-30 age range
*/

static long numAgeLookForLeft = 176;
static char *ageTriggersLeft[] = {
"months old gentleman",
"months-old gentleman",
"month old gentleman",
"month-old gentleman",
"months oldgentleman",
"months-oldgentleman",
"monthsold gentleman",
"years old gentleman",
"years-old gentleman",
"month oldgentleman",
"month-oldgentleman",
"monthold gentleman",
"monthsoldgentleman",
"year old gentleman",
"year-old gentleman",
"years oldgentleman",
"years-oldgentleman",
"yearsold gentleman",
"montholdgentleman",
"months old female",
"months-old female",
"year oldgentleman",
"year-oldgentleman",
"yearold gentleman",
"yearsoldgentleman",
"month old female",
"month-old female",
"months oldfemale",
"months-oldfemale",
"monthsold female",
"yearoldgentleman",
"years old female",
"years-old female",
"month oldfemale",
"month-oldfemale",
"monthold female",
"months old lady",
"months old male",
"months-old lady",
"months-old male",
"monthsoldfemale",
"year old female",
"year-old female",
"years old woman",
"years oldfemale",
"years-old woman",
"years-oldfemale",
"yearsold female",
"month old girl",
"month old lady",
"month old male",
"month-old girl",
"month-old lady",
"month-old male",
"montholdfemale",
"months oldlady",
"months oldmale",
"months-oldlady",
"months-oldmale",
"monthsold lady",
"monthsold male",
"year old woman",
"year oldfemale",
"year-old woman",
"year-oldfemale",
"yearold female",
"years old girl",
"years old lady",
"years old male",
"years oldwoman",
"years-old girl",
"years-old lady",
"years-old male",
"years-oldwoman",
"yearsold woman",
"yearsoldfemale",
"month old boy",
"month oldgirl",
"month oldlady",
"month oldmale",
"month-old boy",
"month-oldgirl",
"month-oldlady",
"month-oldmale",
"monthold girl",
"monthold lady",
"monthold male",
"monthsoldlady",
"monthsoldmale",
"year old girl",
"year old male",
"year oldwoman",
"year-old girl",
"year-old male",
"year-oldwoman",
"yearold woman",
"yearoldfemale",
"years old boy",
"years old man",
"years oldgirl",
"years oldlady",
"years oldmale",
"years-old boy",
"years-old man",
"years-oldgirl",
"years-oldlady",
"years-oldmale",
"yearsold girl",
"yearsold lady",
"yearsold male",
"yearsoldwoman",
"month oldboy",
"month-oldboy",
"monthold boy",
"montholdgirl",
"montholdlady",
"montholdmale",
"year old boy",
"year old man",
"year oldgirl",
"year oldmale",
"year-old boy",
"year-old man",
"year-oldgirl",
"year-oldmale",
"yearold girl",
"yearold male",
"yearoldwoman",
"years of age",
"years oldboy",
"years oldman",
"years-of-age",
"years-oldboy",
"years-oldman",
"yearsold boy",
"yearsold man",
"yearsoldgirl",
"yearsoldlady",
"yearsoldmale",
"montholdboy",
"months- old",
"year oldboy",
"year oldman",
"year-oldboy",
"yearold boy",
"yearold man",
"yearoldgirl",
"yearoldmale",
"yearsoldboy",
"yearsoldman",
"month- old",
"months old",
"months-old",
"yearoldboy",
"yearoldman",
"years- old",
"age group",
"age range",
"month old",
"month-old",
"monthsold",
"year- old",
"years old",
"years-old",
"monthold",
"mos- old",
"year old",
"year-old",
"yearsold",
"mos old",
"mos-old",
"yearold",
"yrs old",
"yrs-old",
"mosold",
"yrsold"
};


struct Special_Aged_Struct {
   char *lookFor;
   int numAdds;
   int toADD[10];
};

/* NOTE: List corresponds to the Age_CT_list list in display.c so make
         sure to keep both up-to-date!!

   * 0 * {"Infant, Newborn", "C0021289", "D007231", 0, 0, -1},
   * 1 * {"Infant", "C0021270", "D007223", 0, 0, -1},
   * 2 * {"Child, Preschool", "C0008100", "D002675", 0, 0, -1},
   * 3 * {"Child", "C0008059", "D002648", 0, 0, -1},
   * 4 * {"Adolescent", "C0205653", "D000293", 0, 0, -1},
   * 5 * {"Young Adult", "C0238598", "D055815", 0, 0, -1},
   * 6 * {"Adult", "C0001675", "D000328", 0, 0, -1},
   * 7 * {"Middle Aged", "C0205847", "D008875", 0, 0, -1},
   * 8 * {"Aged", "C0001792", "D000368", 0, 0, -1},
   * 9 * {"Aged, 80 and over", "C0001795", "D000369", 0, 0, -1}

   NOTE: cleanAges has routine to put these into longest to shortest order
      see 00NOTE in that directory.

*/

static long agedPos = 273;
static long numSpecial_Aged = 274;
static struct Special_Aged_Struct Special_Aged_Lookups[] = {
     {"children older than eight months", 1, { 1 }},   /* Infant */
     {"children older than seven months", 1, { 1 }},   /* Infant */
     {"children older than three months", 1, { 1 }},   /* Infant */
     {"children older than eight years", 1, { 3 }},   /* Child */
     {"children older than five months", 1, { 1 }},   /* Infant */
     {"children older than four months", 1, { 1 }},   /* Infant */
     {"children older than nine months", 1, { 1 }},   /* Infant */
     {"children older than seven years", 1, { 3 }},   /* Child */
     {"children older than three years", 2, { 2, 3 }},   /* Child, Preschool, Child */
     {"children older than five years", 2, { 2, 3 }},   /* Child, Preschool, Child */
     {"children older than four years", 2, { 2, 3 }},   /* Child, Preschool, Child */
     {"children older than nine years", 1, { 3 }},   /* Child */
     {"children older than six months", 1, { 1 }},   /* Infant */
     {"children older than ten months", 1, { 1 }},   /* Infant */
     {"children older than two months", 1, { 1 }},   /* Infant */
     {"children older than 10 months", 1, { 1 }},   /* Infant */
     {"children older than 11 months", 1, { 1 }},   /* Infant */
     {"children older than 12 months", 1, { 1 }},   /* Infant */
     {"children older than 13 months", 1, { 1 }},   /* Infant */
     {"children older than 14 months", 1, { 1 }},   /* Infant */
     {"children older than 15 months", 1, { 1 }},   /* Infant */
     {"children older than 16 months", 1, { 1 }},   /* Infant */
     {"children older than 17 months", 1, { 1 }},   /* Infant */
     {"children older than 18 months", 2, { 1, 2 }},   /* Infant, Child, Preschool */
     {"children older than one month", 1, { 1 }},   /* Infant */
     {"children older than six years", 1, { 3 }},   /* Child */
     {"children older than ten years", 1, { 3 }},   /* Child */
     {"children older than two years", 2, { 2, 3 }},   /* Child, Preschool, Child */
     {"children older than 10 years", 1, { 3 }},   /* Child */
     {"children older than 11 years", 1, { 3 }},   /* Child */
     {"children older than 2 months", 1, { 1 }},   /* Infant */
     {"children older than 3 months", 1, { 1 }},   /* Infant */
     {"children older than 4 months", 1, { 1 }},   /* Infant */
     {"children older than 5 months", 1, { 1 }},   /* Infant */
     {"children older than 6 months", 1, { 1 }},   /* Infant */
     {"children older than 7 months", 1, { 1 }},   /* Infant */
     {"children older than 8 months", 1, { 1 }},   /* Infant */
     {"children older than 9 months", 1, { 1 }},   /* Infant */
     {"children older than one year", 2, { 2, 3 }},   /* Child, Preschool, Child */
     {"children older than 1 month", 1, { 1 }},   /* Infant */
     {"children older than 2 years", 2, { 2, 3 }},   /* Child, Preschool, Child */
     {"children older than 3 years", 2, { 2, 3 }},   /* Child, Preschool, Child */
     {"children older than 4 years", 2, { 2, 3 }},   /* Child, Preschool, Child */
     {"children older than 5 years", 2, { 2, 3 }},   /* Child, Preschool, Child */
     {"children older than 6 years", 1, { 3 }},   /* Child */
     {"children older than 7 years", 1, { 3 }},   /* Child */
     {"children older than 8 years", 1, { 3 }},   /* Child */
     {"children older than 9 years", 1, { 3 }},   /* Child */
     {"children older than 1 year", 2, { 2, 3 }},   /* Child, Preschool, Child */
     {"children under three years", 1, { 2 }},   /* Child, Preschool */
     {"child aged 0 to 12 months", 1, { 0 }},   /* Infant, Newborn */
     {"18 years of age and older", 2, { 4, 6 }},   /* Adolescent, Adult */
     {"children under five years", 1, { 2 }},   /* Child, Preschool */
     {"children under four years", 1, { 2 }},   /* Child, Preschool */
     {"18 years of age or older", 2, { 4, 6 }},   /* Adolescent, Adult */
     {"child aged 0 - 12 months", 1, { 0 }},   /* Infant, Newborn */
     {"children under six years", 1, { 2 }},   /* Child, Preschool */
     {"children under two years", 1, { 1 }},   /* Infant */
     {"aged 40 years and older", 2, { 6, 7 }},   /* Adult, Middle Aged */
     {"aged 50 years and older", 2, { 7, 8 }},   /* Middle Aged, Aged */
     {"aged 60 years and older", 2, { 7, 8 }},   /* Middle Aged, Aged */
     {"aged 70 years and older", 2, { 8, 9 }},   /* Aged, Aged, 80 and over */
     {"children under 18 years", 2, { 3, 4 }},   /* Child, Adolescent */
     {"aged 18 years and more", 2, { 4, 6 }},   /* Adolescent, Adult */
     {"aged 18 years and over", 2, { 4, 6 }},   /* Adolescent, Adult */
     {"aged 40 years and more", 2, { 7, 8 }},   /* Middle Aged, Aged */
     {"aged 40 years and over", 2, { 6, 7 }},   /* Adult, Middle Aged */
     {"aged 50 years and more", 2, { 7, 8 }},   /* Middle Aged, Aged */
     {"aged 50 years and over", 2, { 7, 8 }},   /* Middle Aged, Aged */
     {"aged 60 years and more", 2, { 7, 8 }},   /* Middle Aged, Aged */
     {"aged 60 years and over", 2, { 7, 8 }},   /* Middle Aged, Aged */
     {"aged 70 years and more", 2, { 8, 9 }},   /* Aged, Aged, 80 and over */
     {"aged 70 years and over", 2, { 8, 9 }},   /* Aged, Aged, 80 and over */
     {"aged at least 18 years", 2, { 4, 6 }},   /* Adolescent, Adult */
     {"child aged 0-12 months", 1, { 0 }},   /* Infant, Newborn */
     {"childhood to adulthood", 3, { 3, 4, 5 }},   /* Child, Adolescent, Young Adult */
     {"children under 2 years", 1, { 1 }},   /* Infant */
     {"children under 3 years", 1, { 2 }},   /* Child, Preschool */
     {"children under 4 years", 1, { 2 }},   /* Child, Preschool */
     {"children under 5 years", 1, { 2 }},   /* Child, Preschool */
     {"children under 6 years", 1, { 2 }},   /* Child, Preschool */
     {"older aged adolescents", 1, { 4 }},   /* Adolescent */
     {"older-aged adolescents", 1, { 4 }},   /* Adolescent */
     {"18 years old and over", 2, { 4, 6 }},   /* Adolescent, Adult */
     {"18 years old or older", 2, { 4, 6 }},   /* Adolescent, Adult */
     {"middle aged and older", 2, { 7, 8 }},   /* Middle Aged, Aged */
     {"middle-aged and older", 2, { 7, 8 }},   /* Middle Aged, Aged */
     {"older japanese adults", 1, { 8 }},   /* Aged */
     {"child under 18 years", 2, { 3, 4 }},   /* Child, Adolescent */
     {"older chinese adults", 2, { 7, 8 }},   /* Middle Aged, Aged */
     {"older japanese adult", 1, { 8 }},   /* Aged */
     {"over 18 years of age", 2, { 4, 6 }},   /* Adolescent, Adult */
     {"over 20 years of age", 1, { 6 }},     /* Adult */
     {"over 30 years of age", 1, { 6 }},     /* Adult */
     {"over 40 years of age", 2, { 6, 7 }},     /* Adult, Middle Aged */
     {"over 50 years of age", 1, { 7 }},     /* Middle Aged */
     {"over 60 years of age", 2, { 7, 8 }},   /* Middle Aged, Aged */
     {"over 70 years of age", 2, { 8, 9 }},   /* Aged, Aged, 80 and over */
     {"over 80 years of age", 2, { 8, 9 }},   /* Aged, Aged, 80 and over */
     {"over 90 years of age", 1, { 9 }},   /* Aged, 80 and over */
     {"2 year old children", 1, { 2 }},   /* Child, Preschool */
     {"2-year-old children", 1, { 2 }},   /* Child, Preschool */
     {"3 year old children", 1, { 2 }},   /* Child, Preschool */
     {"3-year-old children", 1, { 2 }},   /* Child, Preschool */
     {"4 year old children", 1, { 2 }},   /* Child, Preschool */
     {"4-year-old children", 1, { 2 }},   /* Child, Preschool */
     {"5 year old children", 1, { 2 }},   /* Child, Preschool */
     {"5-year-old children", 1, { 2 }},   /* Child, Preschool */
     {"6 year old children", 1, { 3 }},   /* Child */
     {"6-year-old children", 1, { 3 }},   /* Child */
     {"older chinese adult", 3, { 6, 7, 8 }},   /* Adult, Middle Aged, Aged */
     {"older aged children", 2, { 3, 4 }},   /* Child, Adolescent */
     {"older-aged children", 2, { 3, 4 }},   /* Child, Adolescent */
     {"aged over 18 years", 2, { 6, 7 }},   /* Adult, Middle Aged */
     {"elderly population", 1, { 8 }},   /* Aged */
     {"older irish adults", 3, { 7, 8, 9 }},   /* Middle Aged, Aged, Aged, 80 and over */
     {"over the age of 18", 2, { 6, 7 }},   /* Adult, Middle Aged */
     {"18 years and over", 2, { 4, 6 }},   /* Adolescent, Adult */
     {"aged 40 and older", 2, { 6, 7 }},   /* Adult, Middle Aged */
     {"aged 50 and older", 2, { 7, 8 }},   /* Middle Aged, Aged */
     {"aged 60 and older", 2, { 7, 8 }},   /* Middle Aged, Aged */
     {"aged 70 and older", 2, { 8, 9 }},   /* Aged, Aged, 80 and over */
     {"aged 80 and older", 1, { 9 }},   /* Aged, 80 and over */
     {"extremely elderly", 2, { 8, 9 }},   /* Aged, Aged, 80 and over */
     {"in older children", 1, { 3 }},   /* Child */
     {"older irish adult", 2, { 8, 9 }},   /* Aged, Aged, 80 and over */
     {"older u.s. adults", 3, { 6, 7, 8 }},   /* Adult, Middle Aged, Aged */
     {"older adolescents", 1, { 4 }},   /* Adolescent */
     {"older aged adults", 2, { 6, 7 }},   /* Adult, Middle Aged */
     {"older-aged adults", 2, { 6, 7 }},   /* Adult, Middle Aged */
     {"reproductive aged", 1, { 6 }},   /* Adult */
     {"reproductive-aged", 1, { 6 }},   /* Adult */
     {"2 year old child", 1, { 2 }},   /* Child, Preschool */
     {"2-year-old child", 1, { 2 }},   /* Child, Preschool */
     {"3 year old child", 1, { 2 }},   /* Child, Preschool */
     {"3-year-old child", 1, { 2 }},   /* Child, Preschool */
     {"4 year old child", 1, { 2 }},   /* Child, Preschool */
     {"4-year-old child", 1, { 2 }},   /* Child, Preschool */
     {"5 year old child", 1, { 2 }},   /* Child, Preschool */
     {"5-year-old child", 1, { 2 }},   /* Child, Preschool */
     {"6 year old child", 1, { 3 }},   /* Child */
     {"6-year-old child", 1, { 3 }},   /* Child */
     {"aged 18 and more", 2, { 4, 6 }},   /* Adolescent, Adult */
     {"aged 18 and over", 2, { 4, 6 }},   /* Adolescent, Adult */
     {"aged 20 and more", 1, { 6 }},   /* Adult */
     {"aged 20 and over", 1, { 6 }},   /* Adult */
     {"aged 30 and more", 1, { 6 }},   /* Adult */
     {"aged 30 and over", 1, { 6 }},   /* Adult */
     {"aged 40 and more", 2, { 7, 8 }},   /* Middle Aged, Aged */
     {"aged 40 and over", 2, { 6, 7 }},   /* Adult, Middle Aged */
     {"aged 50 and more", 2, { 7, 8 }},   /* Middle Aged, Aged */
     {"aged 50 and over", 2, { 7, 8 }},   /* Middle Aged, Aged */
     {"aged 60 and more", 2, { 7, 8 }},   /* Middle Aged, Aged */
     {"aged 60 and over", 2, { 7, 8 }},   /* Middle Aged, Aged */
     {"aged 70 and more", 2, { 8, 9 }},   /* Aged, Aged, 80 and over */
     {"aged 70 and over", 2, { 8, 9 }},   /* Aged, Aged, 80 and over */
     {"aged 80 and over", 1, { 9 }},   /* Aged, 80 and over */
     {"aging population", 1, { 8 }},   /* Aged */
     {"elderly children", 2, { 3, 4 }},   /* Child, Adolescent */
     {"newborn children", 1, { 0 }},   /* Infant, Newborn */
     {"older age groups", 1, { 8 }},   /* Aged */
     {"older u.s. adult", 3, { 6, 7, 8 }},   /* Adult, Middle Aged, Aged */
     {"over 18 y of age", 2, { 6, 7 }},   /* Adult, Middle Aged */
     {"aged >=18 years", 2, { 4, 6 }},   /* Adolescent, Adult */
     {"in older adults", 1, { 8 }},   /* Aged */
     {"older age group", 1, { 8 }},   /* Aged */
     {"older adulthood", 1, { 8 }},   /* Aged */
     {"older americans", 1, { 8 }},   /* Aged */
     {"older canadians", 1, { 8 }},   /* Aged */
     {"older us adults", 3, { 6, 7, 8 }},   /* Adult, Middle Aged, Aged */
     {"pre-school aged", 1, { 2 }},   /* Child, Preschool */
     {"pre-school-aged", 1, { 2 }},   /* Child, Preschool */
     {"school children", 1, { 3 }},   /* Child */
     {"school-children", 1, { 3 }},   /* Child */
     {"young adulthood", 2, { 5, 6 }},   /* Young Adult, Adult */
     {"elderly people", 1, { 8 }},   /* Aged */
     {"mid child hood", 1, { 3 }},   /* Child */
     {"mid-child hood", 1, { 3 }},   /* Child */
     {"older children", 2, { 3, 4 }},   /* Child, Adolescent */
     {"older us adult", 3, { 6, 7, 8 }},   /* Adult, Middle Aged, Aged */
     {"preschool aged", 1, { 2 }},   /* Child, Preschool */
     {"preschool-aged", 1, { 2 }},   /* Child, Preschool */
     {"schoolchildren", 1, { 3 }},   /* Child */
     {"under 18 years", 1, { 4 }},   /* Adolescent */
     {"young children", 1, { 2 }},   /* Child, Preschool */
     {"elderly child", 2, { 3, 4 }},   /* Child, Adolescent */
     {"mid childhood", 1, { 3 }},   /* Child */
     {"mid-childhood", 1, { 3 }},   /* Child */
     {"newborn child", 1, { 0 }},   /* Infant, Newborn */
     {"nonagenarians", 1, { 9 }},   /* Aged, 80 and over */
     {"octogenarians", 1, { 9 }},   /* Aged, 80 and over */
     {"older persons", 2, { 8, 9 }},   /* Aged, Aged, 80 and over */
     {"young elderly", 2, { 6, 7 }},   /* Adult, Middle Aged */
     {"centenarians", 1, { 9 }},   /* Aged, 80 and over */
     {"college aged", 1, { 5 }},   /* Young Adult */
     {"college-aged", 1, { 5 }},   /* Young Adult */
     {"nonagenarian", 1, { 9 }},   /* Aged, 80 and over */
     {"octogenarian", 1, { 9 }},   /* Aged, 80 and over */
     {"older adults", 1, { 8 }},   /* Aged */
     {"older people", 1, { 8 }},   /* Aged */
     {"older person", 1, { 8 }},   /* Aged */
     {"young people", 1, { 4 }},   /* Adolescent */
     {"young person", 1, { 4 }},   /* Adolescent */
     {"young-people", 1, { 4 }},   /* Adolescent */
     {"adolescents", 1, { 4 }},   /* Adolescent */
     {"aged adults", 1, { 7 }},   /* Middle Aged */
     {"aged-adults", 1, { 7 }},   /* Middle Aged */
     {"centenarian", 1, { 9 }},   /* Aged, 80 and over */
     {"in an adult", 1, { 6 }},   /* Adult */
     {"middle aged", 1, { 7 }},   /* Middle Aged */
     {"middle-aged", 1, { 7 }},   /* Middle Aged */
     {"non elderly", 1, { 7 }},   /* Middle Aged */
     {"non-elderly", 1, { 7 }},   /* Middle Aged */
     {"older adult", 1, { 8 }},   /* Aged */
     {"older child", 2, { 3, 4 }},   /* Child, Adolescent */
     {"older teens", 1, { 4 }},   /* Adolescent */
     {"older woman", 1, { 8 }},   /* Aged */
     {"older women", 1, { 8 }},   /* Aged */
     {"school aged", 1, { 3 }},   /* Child */
     {"school-aged", 1, { 3 }},   /* Child */
     {"2 year olds", 1, { 2 }},   /* Child, Preschool */
     {"2-year-olds", 1, { 2 }},   /* Child, Preschool */
     {"3 year olds", 1, { 2 }},   /* Child, Preschool */
     {"3-year-olds", 1, { 2 }},   /* Child, Preschool */
     {"4 year olds", 1, { 2 }},   /* Child, Preschool */
     {"4-year-olds", 1, { 2 }},   /* Child, Preschool */
     {"5 year olds", 1, { 2 }},   /* Child, Preschool */
     {"5-year-olds", 1, { 2 }},   /* Child, Preschool */
     {"6 year olds", 1, { 3 }},   /* Child */
     {"6-year-olds", 1, { 3 }},   /* Child */
     {"adolescent", 1, { 4 }},   /* Adolescent */
     {"aged 18-25", 3, { 4, 5, 6 }},   /* Adolescent, Young Adult, Adult */
     {"ages 18-25", 3, { 4, 5, 6 }},   /* Adolescent, Young Adult, Adult */
     {"child hood", 1, { 3 }},   /* Child */
     {"child-hood", 1, { 3 }},   /* Child */
     {"long liver", 2, { 8, 9 }},   /* Aged, Aged, 80 and over */
     {"long-liver", 2, { 8, 9 }},   /* Aged, Aged, 80 and over */
     {"older aged", 2, { 7, 8 }},   /* Middle Aged, Aged */
     {"older-aged", 2, { 7, 8 }},   /* Middle Aged, Aged */
     {"older ages", 2, { 7, 8 }},   /* Middle Aged, Aged */
     {"older-ages", 2, { 7, 8 }},   /* Middle Aged, Aged */
     {"oldest old", 1, { 9 }},   /* Aged, 80 and over */
     {"oldest-old", 1, { 9 }},   /* Aged, 80 and over */
     {"adulthood", 1, { 6 }},   /* Adult */
     {"age 18-25", 3, { 4, 5, 6 }},   /* Adolescent, Young Adult, Adult */
     {"childhood", 1, { 3 }},   /* Child */
     {"geriatric", 1, { 8 }},   /* Aged */
     {"older age", 1, { 8 }},   /* Aged */
     {"older-age", 1, { 8 }},   /* Aged */
     {"older men", 1, { 8 }},   /* Aged */
     {"teenagers", 1, { 4 }},   /* Adolescent */
     {"age > 18", 2, { 6, 7 }},   /* Adult, Middle Aged */
     {"in older", 1, { 8 }},   /* Aged */
     {"mid aged", 1, { 7 }},   /* Middle Aged */
     {"mid-aged", 1, { 7 }},   /* Middle Aged */
     {"old aged", 2, { 7, 8 }},   /* Middle Aged, Aged */
     {"old-aged", 2, { 7, 8 }},   /* Middle Aged, Aged */
     {"over 60s", 1, { 8 }},   /* Aged */
     {"over-60s", 1, { 8 }},   /* Aged */
     {"over 70s", 1, { 8 }},   /* Aged */
     {"over-70s", 1, { 8 }},   /* Aged */
     {"pre-teen", 1, { 3 }},   /* Child */
     {"teenager", 1, { 4 }},   /* Adolescent */
     {"teenaged", 1, { 4 }},   /* Adolescent */
     {"preteen", 1, { 3 }},   /* Child */
     {"teenage", 1, { 4 }},   /* Adolescent */
     {"elderly", 1, { 8 }},   /* Aged */
     {"elders", 1, { 8 }},   /* Aged */
     {"youths", 1, { 4 }},   /* Adolescent */
     {"elder", 1, { 8 }},   /* Aged */
     {"teens", 1, { 4 }},   /* Adolescent */
     {"youth", 1, { 4 }},   /* Adolescent */
     {"teen", 1, { 4 }},   /* Adolescent */
     {"aged", 1, { 8 }}   /* NOTE: MUST BE LAST ENTRY - Aged */
};

static long numCheckForAnimals = 29;
static char *checkForAnimals[] = {
"bee", "bees", "cat", "cats", "cattle", "dog",  "dogs", "drosophila melanogaster", "guinea pig", "guinea pigs", "horse", "horses", "marmoset",  "marmosets", "mice", "mouse", "monkey", "monkeys", "pinus tabuliformis", "primate", "primates", "rabbit", "rabbits", "rat", "rats", "rodent", "rodents", "sheep", "swine"
};

static long numAgeIgnores = 95;
static char *ageIgnores[] = {"across the ages", "bronze ages", "cultivation ages", "dark ages", "different ages", "during the ages", "for the ages", "groundwater ages", "ice ages", "industry ages", "iron ages", "middle ages", "next 10 months", "next 10 years", "next 2 months", "next 2 years", "next 3 months", "next 3 years", "next 4 months", "next 4 years", "next 5 months", "next 5 years", "next 6 months", "next 6 years", "next 7 months", "next 7 years", "next 8 months", "next 8 years", "next 9 months", "next 9 years", "next eight months", "next eight years", "next five months", "next five years", "next four months", "next four years", "next month", "next nine months", "next nine years", "next seven months", "next seven years", "next six months", "next six years", "next ten months", "next ten years", "next three months", "next three years", "next two months", "next two years", "next year", "past 10 months", "past 10 years", "past 2 months", "past 2 years", "past 3 months", "past 3 years", "past 4 months", "past 4 years", "past 5 months", "past 5 years", "past 6 months", "past 6 years", "past 7 months", "past 7 years", "past 8 months", "past 8 years", "past 9 months", "past 9 years", "past eight months", "past eight years", "past five months", "past five years", "past four months", "past four years", "past month", "past nine months", "past nine years", "past seven months", "past seven years", "past six months", "past six years", "past ten months", "past ten years", "past three months", "past three years", "past two months", "past two years", "past year", "through the ages", "throughout the ages", "translational ages", "two ages", "year period", "years after", "years before"
};

static long numVLBW = 152;
static char *vlbwTriggers[] = {
"1250 g birth weight",
"1250 g birth-weight",
"1250 g birthweight",
"1500 g birth weight",
"1500 g birth-weight",
"1500 g birthweight",
"1250g birth weight",
"1250g birth-weight",
"1250g birthweight",
"1500g birth weight",
"1500g birth-weight",
"1500g birthweight",
"<1250 g birth weight",
"<1250 g birth-weight",
"<1250 g birthweight",
"< 1250 g birth weight",
"< 1250 g birth-weight",
"< 1250 g birthweight",
"<1250g birth weight",
"<1250g birth-weight",
"<1250g birthweight",
"< 1250g birth weight",
"< 1250g birth-weight",
"< 1250g birthweight",
"<1500 g birth weight",
"<1500 g birth-weight",
"<1500 g birthweight",
"< 1500 g birth weight",
"< 1500 g birth-weight",
"< 1500 g birthweight",
"<1500g birth weight",
"<1500g birth-weight",
"<1500g birthweight",
"< 1500g birth weight",
"< 1500g birth-weight",
"< 1500g birthweight",
"1,500 g birth-weight",
"1,500 g birthweight",
"1,250g birth weight",
"1,250g birth-weight",
"1,250g birthweight",
"1,500g birth weight",
"1,500g birth-weight",
"1,500g birthweight",
"<1,250 g birth weight",
"<1,250 g birth-weight",
"<1,250 g birthweight",
"< 1,250 g birth weight",
"< 1,250 g birth-weight",
"< 1,250 g birthweight",
"<1,250g birth weight",
"<1,250g birth-weight",
"<1,250g birthweight",
"< 1,250g birth weight",
"< 1,250g birth-weight",
"< 1,250g birthweight",
"<1,500 g birth weight",
"<1,500 g birth-weight",
"<1,500 g birthweight",
"< 1,500 g birth weight",
"< 1,500 g birth-weight",
"< 1,500 g birthweight",
"<1,500g birth weight",
"<1,500g birth-weight",
"<1,500g birthweight",
"< 1,500g birth weight",
"< 1,500g birth-weight",
"< 1,500g birthweight",
"birth weight (BW) < 1500",
"birth-weight (BW) < 1500",
"birthweight (BW) < 1500",
"birth weight (BW) <1500",
"birth-weight (BW) <1500",
"birthweight (BW) <1500",
"birth weight (BW) < 1,500",
"birth-weight (BW) < 1,500",
"birthweight (BW) < 1,500",
"birth weight (BW) <1,500",
"birth-weight (BW) <1,500",
"birthweight (BW) <1,500",
"birth weight 1250",
"birth-weight 1250",
"birthweight 1250",
"birth weight 1,250",
"birth-weight 1,250",
"birthweight 1,250",
"birth weight 1500",
"birth-weight 1500",
"birthweight 1500",
"birth weight 1,500",
"birth-weight 1,500",
"birthweight 1,500",
"birth weight < 1500",
"birth-weight < 1500",
"birthweight < 1500",
"birth weight < 1,500",
"birth-weight < 1,500",
"birthweight < 1,500",
"birth weight <1500",
"birth-weight <1500",
"birthweight <1500",
"birth weight <1,500",
"birth-weight <1,500",
"birthweight <1,500",
"birth weight < 1250",
"birth-weight < 1250",
"birthweight < 1250",
"birth weight < 1,250",
"birth-weight < 1,250",
"birthweight < 1,250",
"birth weight <1250",
"birth-weight <1250",
"birthweight <1250",
"birth weight <1,250",
"birth-weight <1,250",
"birthweight <1,250",
"birth weight below 1500",
"birth-weight below 1500",
"birthweight below 1500",
"birth weight below 1250",
"birth-weight below 1250",
"birthweight below 1250",
"birth weight below 1,500",
"birth-weight below 1,500",
"birthweight below 1,500",
"birth weight below 1,250",
"birth-weight below 1,250",
"birthweight below 1,250",
"birth weight less than 1250",
"birth-weight less than 1250",
"birthweight less than 1250",
"birth weight less than 1251",
"birth-weight less than 1251",
"birthweight less than 1251",
"birth weight less than 1500",
"birth-weight less than 1500",
"birthweight less than 1500",
"birth weight less than 1501",
"birth-weight less than 1501",
"birthweight less than 1501",
"birth weight less than 1,250",
"birth-weight less than 1,250",
"birthweight less than 1,250",
"birth weight less than 1,251",
"birth-weight less than 1,251",
"birthweight less than 1,251",
"birth weight less than 1,500",
"birth-weight less than 1,500",
"birthweight less than 1,500",
"birth weight less than 1,501",
"birth-weight less than 1,501",
"birthweight less than 1,501"
};
