#include "M_terms.h"

#define MAX_WORDS 10000

extern int RTM_Debug;
extern FILE *fout;

int idWithinContext(char *text, long len, char *lookFor, char *refine,
                    int windowSize);
int provideContext(char *text, char *lookFor, char *refine, int windowSize);
int checkHaystack(char *needle, char *haystack);


/***************************************************************************
*
*  idWithinContext --
*
*      This
*
***************************************************************************/

int idWithinContext(char *text, long len, char *lookFor, char *refine,
                    int windowSize)
{
    int done, c, rtn;
    long i, diff, start, right, left, fLen, p;
    char *tmpLC, *foo;

    rtn = FALSE;

    tmpLC = strdup(text);
    for(i = 0; i < len; i++)
    {
        if(isupper(tmpLC[i]))
          tmpLC[i] = tolower(tmpLC[i]);
    } /* for */

    /* check for Potential */

    foo = strstr(tmpLC, lookFor);
    if(foo != NULL)
    {
        c = 0;
        while(foo != NULL)
        {
            left = right = -1;
            diff = foo - tmpLC;
            done = FALSE;

            /* Find right-hand side of sentence */

            for(i = diff; !done && (i < len); i++)
            {
                if((tmpLC[i] == '\n') || (tmpLC[i] == '.'))
                {
                   done = TRUE;
                   right = i;
                } /* fi */
            } /* for */

            /* Look for beginning of sentence */

            done = FALSE;
            for(i = diff; !done && (i > -1); i--)
            {
               if((i == 0) || (tmpLC[i] == '.') || (tmpLC[i] == '\n'))
               {
                   done = TRUE;
                   left = i;
               } /* for */
            } /* for */

            if((left > -1) && (right > -1))
            {
                if(tmpLC[left] == '.')
                  left++;

                fLen = (right - left) + 1;
                foo = (char *) malloc(fLen + 25);
                strcpy(foo, "");
                p = 0;
                for(i = left; i < right; i++)
                   foo[p++] = tmpLC[i];
                foo[p] = '\0';

                if(checkHaystack(refine, foo))
                {
                    rtn = provideContext(foo, lookFor, refine, windowSize);

                    if(rtn)
                      strcpy(tmpLC, "");
                } /* fi */

                free(foo);
            } /* fi */

            for(i = 0; i < 8; i++)
              tmpLC[i + diff] = 'x';

            foo = strstr(tmpLC, lookFor);
        } /* while */
    } /* fi */

    free(tmpLC);
    return(rtn);
} /* idWithinContext */

/***************************************************************************
*
*  provideContext --
*
*      This 
*
***************************************************************************/

int provideContext(char *text, char *lookFor, char *refine, int windowSize)
{
    int rtn;
    long i, j, len, numW, left, right;
    char *tmp, *rest, word[SMALL_LINE + 1], context[SMALL_LINE + 1],
         *words[MAX_WORDS];

    rtn = FALSE;
    numW = 0;
    len = (long)strlen(text);
    rest = (char *)malloc(len + 1);
    strcpy(rest, "");
    tmp = strdup(text);

    while(strlen(tmp) > 0)
    {
        strcpy(word, "");
        strcpy(rest, "");
        sscanf(tmp, "%s %[^\n]", word, rest);
        strcpy(tmp, rest);
        if(strlen(word) > 0)
        {
            if(numW < MAX_WORDS)
              words[numW++] = strdup(word);
        } /* fi */
    } /* while */

    /* Find context */

    for(i = 0; i < numW; i++)
    {
        if(checkHaystack(refine, words[i]))
        {
            strcpy(context, "");
            left = i - windowSize;
            if(left < 0)
              left = 0;

            right = i + windowSize + 1;
            if(right > numW)
              right = numW;

            for(j = left; j < right; j++)
            {
                if(strlen(context) > 0)
                  strcat(context, " ");
                strcat(context, words[j]);
            } /* for */

            if(RTM_Debug)
            {
                fprintf(fout, "Context: #%s#\n", context);
                fflush(fout);
            } /* fi */

            if(checkHaystack(lookFor, context))
              rtn = TRUE;
        } /* fi */
    } /* for */
    
    /* Clean Up */

    for(i = 0; i < numW; i++)
      free(words[i]);
    free(tmp);
    free(rest);

    return(rtn);
} /* provideContext */

/***************************************************************************
*
*  checkHaystack --
*
*      This 
*
***************************************************************************/

int checkHaystack(char *needle, char *haystack)
{
   int rtn = FALSE;
   char *base, *foo;
   int okLeft, okRight, rOK, addPlural;
   long i, j, baseLen, lfLen, diff, rPos, pos;

   okLeft = okRight = addPlural = FALSE;
   lfLen = (long)strlen(needle);
   base = strdup(haystack);
   baseLen = (long)strlen(haystack);

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

          if((pos >= baseLen) || isspace(base[pos]) || 
             ispunct(base[pos]) || (base[pos] == 's'))
          {
              rOK = TRUE;

              /* Need to check if really plural or just part of word -
                 E.g., don't want "mus" hitting on "mussels". 010411 - JGM

                 ALSO, don't want case like "needles" -> "needless" 091415
              */

              if(base[pos] == 's')
              {
                  rOK = FALSE;
                  if(base[pos - 1] != 's') /* Don't want plural "ss" */
                  {
                      rPos = pos + 1;
                      if(rPos >= baseLen) /* Reached end */
                        rOK = TRUE;
                      else
                      {
                          if(isspace(base[rPos]) || ispunct(base[rPos]))
                            rOK = TRUE;
                      } /* else */

                      if(rOK)
                        addPlural = TRUE;
                  } /* fi */
              } /* fi */                

              if(rOK)
                okRight = TRUE;
          } /* fi diff, space, or punct right */
       } /* fi okLeft */

       if(okLeft && okRight)
         rtn = TRUE;
 
       for(j = 0; j < lfLen; j++)
          base[j + diff] = 'X';

       foo = strstr(base, needle);
  } /* while foo */

   free(base);
   return(rtn);
} /* checkHaystack */
