/* 
    Program: GetPRC_NCat_HTTPS

    Call TexTool with title and abstract from citation, pull PubMed
    citations based on related list of PMIDs and then pull the MHs from
    the citations and return a scored list of MeSH Headings.  The local
    version uses the TexTool located on indsc1 and the remote version uses
    the TexTool located on the NCBI PubMed servers.

    NOTE: This version includes attached subheadings from the Related articles.

    NOTE: Input file is assumed to be in MEDLINE format!!

    Usage: 
      java GetPRC_NCat_HTTPS -N topN [-P PMID] [-M NLMID] [-f filename -T TexToolHost] [ -D ] [ -C ]

       Where:

           -N topN is the number of related citations to use

           -P PMID to generate indexing from

           -M NLMID to generate indexing from

           -f filename is the name of the input file to process

           -T TexToolHost is the name of the host to use for TexTool results -
              should be either indsc1 or skr4.

           -C Cataloging, so use NLMCatalog DB as first check.

           -D Display debugging information.

        NOTE: Either a PMID or a filename must be specified

    If the -P is specified, the program does a lookup to see if there are
    any precomputed Indexed related articles, if there are, it pulls the
    topN highest ranking Indexed articles from that list.  If there are NO
    precomputed Indexed related articles, it looks to see if the user has
    specified a backup filename to use - if so, it does a TexTool review
    for the file.
*/

import java.lang.*;
import java.util.*;
import java.net.*;
import java.io.*;

public class GetPRC_NCat_HTTPS
{
   static String PMIDstr = "0";
   static String TexToolHost = "indsc1";
   static String inputFile = "";
   static String resultsFrom = "No Related Articles Found";
   static boolean DebugDisplay = false;
   static int topnum;
   static boolean errorFound = false;
   static boolean UID_found = false;
   static int numRelFound = 0;
   static String Uid = "0";    /* Default UID */
   static String Title = "";
   static String Abstract = "";
   static Vector<String> PRClist = new Vector<String>();
   // Adding back in for Catalog Related Articles
   static String NLMIDstr = "0";
   static String PMID_list = "";
   static String PMIDarray[] = new String[200];
   static int numPMIDs = 0;
   static String NCBI_IDstr = "0";
   static int overallCtr = 0;
   static Hashtable<String, String> hScoreUID = new Hashtable<String, String>();

   public static void main(String args[])
   {
        boolean Pflag = false;
        boolean fFlag = false;
        boolean Tflag = false;
        boolean Cflag = false;
        boolean Mflag = false;

        int i;

        i = 0;
        while(i < args.length)
        {
            if(args[i].compareTo("-D") == 0)
              DebugDisplay = true;

            else if(args[i].compareTo("-C") == 0)
              Cflag = true;

            else if(args[i].compareTo("-M") == 0)
            {
                i++;
                NLMIDstr = args[i];
                Mflag = true;
            } // else fi

            else if(args[i].compareTo("-N") == 0)
            {
                i++;
                topnum = (Integer.parseInt(args[i]));
            } // else fi

            else if(args[i].compareTo("-P") == 0)
            {
                i++;
                PMIDstr = args[i];
                Pflag = true;
            } // else fi

            else if(args[i].compareTo("-T") == 0)
            {
                i++;
                TexToolHost = args[i];
                Tflag = true;
            } // else fi

            else if(args[i].compareTo("-f") == 0)
            {
                i++;
                inputFile = args[i];
                fFlag = true;
            } // else fi

            i++;
        } // while

        errorFound = false;
        numRelFound = 0;

        if(Pflag)
        {
            if(Cflag && Mflag)  // Try Catalog Neighbors first
            {
                getCatalogNeighbors();
                if(numRelFound > 0)
                {
                    numRelFound = 0;  // Reset to see if we get results or not
                    runCatalogRetrieval();
                } /* fi */
            } /* fi */

            else // Regular PMID
              getNeighbors();

            if((numRelFound < topnum) && fFlag)
            {
               System.err.println("Not enough PRC Related, switch to TexTool");
               getTexToolRelated();
            } // fi
        } // fi

        else if(fFlag)
           getTexToolRelated();

        if(numRelFound == 0)
          resultsFrom = "No Related Articles Found" + " - ctr: " + overallCtr;

        System.out.println("Method Used: " + resultsFrom);
   }  /* main */

   // ************************************************************************

   public static void error(String s)
   {
     System.out.println(s);
     System.exit(1);
   } // error

   // ************************************************************************

   public static void getNeighbors()
   {
      int pagesize = topnum * 5;
      int numFind = topnum * 5;
      double divisor = 2147483647.0;
      double score = 0.0;
      float modScore = 0;

      boolean done = false;
      boolean firstScore = true;
      boolean localDone = false;
      int ctr = 0;
      overallCtr = 0;

      while(!done)
      {
/*
https://eutils.ncbi.nlm.nih.gov/entrez/eutils/elink.fcgi?dbfrom=pubmed&id=22825365&cmd=neighbor_score&term=medline[sb]&tool=Medical_Text_Indexer&email=mork@nlm.nih.gov&api_key=8ed2ad8415029f5f018b4ee5da61542e8308
*/
         done = true;
         try
         {
            String urlString =
                    "https://eutils.ncbi.nlm.nih.gov/entrez/eutils/elink.fcgi";

            // &term=medline[sb] says to limit results to those that have 
            // already been indexed.

            // Removed 3/6/09 - JGM while we wait to have it fixed by NCBI
            // 3/6/09 - JGM changed cmd=neighbor to cmd=neighbor_score
            //   due to unannounced changed from NCBI on 3/5/09.

            // 3/31/09 - Finally have the filtering back so that it's not
            // returning scores of zero.  Added back in &term=medline[sb]
            // and removed the 5X topNum.

            URL newurl = new URL(urlString + "?" + "dbfrom=pubmed&id=" +
              PMIDstr +
              "&cmd=neighbor_score&term=medline[sb]&tool=Medical_Text_Indexer" +
              "&email=mork@nlm.nih.gov" +
              "&api_key=8ed2ad8415029f5f018b4ee5da61542e8308");

            if(DebugDisplay)
              System.out.println("URL: " + urlString + "?" + 
                    "dbfrom=pubmed&id=" + PMIDstr +
              "&cmd=neighbor_score&term=medline[sb]&tool=Medical_Text_Indexer" +
              "&email=mork@nlm.nih.gov" +
              "&api_key=8ed2ad8415029f5f018b4ee5da61542e8308");

            URLConnection conn = newurl.openConnection();
            BufferedReader buffURL = 
                 new BufferedReader(new InputStreamReader(newurl.openStream()));

            String muidLine = "";
            String lPMID = "";
            String lScore = "";

            if(DebugDisplay)
              System.out.println("Before While - topnum: " + topnum);

            while(((muidLine = buffURL.readLine()) != null) &&
                   (numRelFound < numFind))
            {
                if(DebugDisplay)
                  System.out.println("getN - muidLine: #" + muidLine + "#");

                // ONLY interested in Similar Articles, skip rest

                if(muidLine.indexOf("<LinkName>pubmed_pubmed</LinkName>") > -1)
                {
                    localDone = false;
                    while(!localDone && 
                          ((muidLine = buffURL.readLine()) != null) &&
                           (numRelFound < numFind))
                    {
                        if(DebugDisplay)
                          System.out.println("getN loop - muidLine: #" + 
                                             muidLine + "#");

                        int pos = muidLine.indexOf("<Id>");
                        if(pos > -1)
                        {
                            score = -1;
                            modScore = -1;

                            int posEnd = muidLine.indexOf("</Id>");
                            lPMID = muidLine.substring(pos + 4, posEnd);
                            muidLine = buffURL.readLine();
                            pos = muidLine.indexOf("<Score>");
                            if(pos > -1)
                            {
                                posEnd = muidLine.indexOf("</Score>");
                                lScore = muidLine.substring(pos + 7, posEnd);
                                score = Double.parseDouble(lScore);

                                  // Assume 80% match first

                                if(firstScore)
                                  divisor = score / 0.8;
                                firstScore = false;
                                double pcnt = score / divisor;
                                Float tscore = new Float(pcnt);
                                Float tmp = new Float(255);
                                modScore = tscore.floatValue() *
                                           tmp.floatValue();

                                if(DebugDisplay)
                                  System.out.println("score: " + score + 
                                    "|pcnt: " + pcnt + "|tscore: " +
                                    tscore + "|modScore: " + modScore);
                            } // fi

                            // Problem with 0|NaN results.

                            if(!lPMID.equals("0"))
                            {
                                if(!lPMID.equals(PMIDstr))
                                {
                                    if(score > -1)
                                    {
                                        numRelFound++;
                                        String tmp = lPMID + "|" +
                                                 Float.toString(modScore);
                                        PRClist.add(tmp);
                                    } // fi score

                                    if(DebugDisplay)
                                      System.out.println("Neighbor: " + lPMID +
                                         "|" + score + "|" + modScore);
                                } // fi !original PMID

                                // Else original PMID with score - set divisor
                                // to the new score.

                                else if(pos > -1) 
                                {
                                    divisor = score;

                                    if(DebugDisplay)
                                      System.out.println("Divisor: " + lPMID +
                                           "|" + score +  "|" + divisor);
                                } // else fi
                            } // fi
                        } // fi

                        // See if we finished up the Similar Articles

                        if(muidLine.indexOf("</LinkSetDb>") > -1)
                          localDone = true;
                    } // while
                } // fi
            } // while

            // Make sure buffer is empty since we stop after we have enough
            // info.

             while((muidLine = buffURL.readLine()) != null)
             {
                 if(DebugDisplay)
                   System.out.println("Cleanup Loop1: #" + muidLine + "#");
             } // while

            buffURL.close();

            if((numRelFound > 0) && (numRelFound >= topnum))
            {
                resultsFrom = "PubMed Related Articles" + " - ctr: " + ctr;
                for(int i=0; i< PRClist.size(); i++)
                   System.out.println(PRClist.elementAt(i));
            } // fi
         } // try 1

         catch (MalformedURLException ex) {
            error("5) ERROR - Bad URL");
         } catch (UnsupportedEncodingException ex) {
            error("6) ERROR - Unsupported Encoding Specified.");
         } catch (UnknownServiceException ex) {
            error("7) ERROR - UnknownServiceException occured.");
         } catch (IOException ex)
         {
            ctr++;
            if(ctr < 3)
            {
                System.err.println("8) IO Exception found: " + ex);
                done = false;
            } // fi

            else
            {
                System.err.println("Stopping after 3 attempts on getNeighbors");
                error("8) ERROR - IOException occured.");
            } // else
         } // catch

         // See if we have any results and retry up to 4 more times if not

         ctr++;
         overallCtr++;
         if((numRelFound == 0) && (ctr < 5))
         {
             done = false;
             try
             {
                 Thread.sleep(3000);
             } // try

             catch (Exception exT)
               { System.err.println("Sleep Interrupted: " + exT); }

             if(DebugDisplay)
               System.out.println("Retry Check: "+ ctr);
         } // fi 
      } // while !done
   } // getNeighbors

   // ************************************************************************

   public static void getTexToolRelated()
   {
      String Line = new String();
      String text = new String();
      boolean done = false;
      boolean rtn = false;
      boolean doRead = true;
      boolean tmpP = true;

      try
      { 
         String oFileName = inputFile + ".interim";
         BufferedReader in = new BufferedReader(new FileReader(inputFile));

         // Skip any preceding blank spaces

         while(!rtn && tmpP)
         {
            Line = in.readLine();

            if(Line == null)
              rtn = true;

            else if(Line.length() > 0)
              tmpP = false;

            if(DebugDisplay)
              System.out.println("Top Input Line - rtn: " + rtn +
                           "  tmpP: " + tmpP + "  Line: " + Line);
         } // while

         // Now do the proper reading -- either MEDLINE format or free text

         if(!rtn)
         {
             if(Line.startsWith("UI  - ") || Line.startsWith("PMID- "))
             {
                while(!rtn && !done)
                {
                   doRead = true;

                   if(Line.startsWith("UI  - "))
                     Uid = Line.substring(6);

                   else if(Line.startsWith("PMID- "))
                     Uid = Line.substring(6);

                   else if(Line.startsWith("TI  - "))
                   {
                       Title = Title + Line.substring(6);
                       while(((Line = in.readLine()) != null) && 
                              Line.startsWith("      "))
                          Title = Title + " " + Line.substring(6);

                       doRead = false;
                   } // else if TI

                   else if(Line.startsWith("AB  - "))
                   {
                       Abstract = Abstract + Line.substring(6);
                       while(((Line = in.readLine()) != null) &&
                               Line.startsWith("      "))
                          Abstract = Abstract + " " + Line.substring(6);

                       doRead = false;
                   } // fi AB

                   if(doRead)
                     Line = in.readLine();

                   if(Line == null)
                     rtn = true;

                   else if(Line.length() == 0)
                     done = true;
                } // while

                PMIDstr = Uid;

                /*
                    Running into problems with TextTool and large input text.
                    Setting max title length to 8,000 chars and overall the
                    max length can't exceed 10,000 characters.  Not sure why
                    that is yet, but, seems to be consistent.

                    Want to try and break the title at the end of a word and
                    not in the middle of a word.  Same for the very end.
                */

                if(Title.length() > 8000)
                {
                    int totLen = Title.length();
                    int maxLen = totLen;
                    if(maxLen > 8000)
                      maxLen = 8000;

                    int endTIPos = findPos(Title, maxLen);
                    int endABPos = findPos(Title, totLen);

                    String tmp = Title.substring(0, endTIPos);
                    String tmp2 = Title.substring(endTIPos + 1, endABPos);
                    Title = tmp;

                    if(Abstract.length() > 0)
                      tmp = tmp2 + "  " + Abstract;
                    else
                      tmp = tmp2;

                    Abstract = tmp;
                    if(Abstract.length() > 2000)
                    {
                        endABPos = findPos(Abstract, 2000);
                        tmp = Abstract.substring(0, endABPos);
                        Abstract = tmp;
                    } // fi
                } // fi

                else if((Title.length() + Abstract.length()) > 10000)
                {
                    int maxLen = 10000 - Title.length();
                    int endABPos = findPos(Abstract, maxLen);
                    String tmp = Abstract.substring(0, endABPos);
                    Abstract = tmp;
                } // else fi

            } // fi MEDLINE

            else // Free Text
            {
               text = Line.trim();
               while((Line = in.readLine()) != null)
               {
                   if(DebugDisplay)
                     System.out.println("Free Text Line: " + Line);

                   text = text + " " + Line.trim();
                   Line = in.readLine();
               } // while


                /*
                    Running into problems with TextTool and large input text.
                    Setting max title length to 8,000 chars and overall the
                    max length can't exceed 10,000 characters.  Not sure why
                    that is yet, but, seems to be consistent.

                    Want to try and break the title at the end of a word and
                    not in the middle of a word.  Same for the very end.
                */

                if(text.length() > 8000)
                {
                    int maxLen = text.length();
                    if(maxLen > 10000)
                      maxLen = 10000;

                    int endTIPos = findPos(text, 8000);
                    int endABPos = findPos(text, maxLen);

                    Title = text.substring(0, endTIPos);
                    Abstract = text.substring(endTIPos + 1, endABPos);
                } // fi
 
                else 
                  Title = text;
            } // else freeText
         } // fi rtn
         in.close();

         if(DebugDisplay)
         {
             System.out.println("PMID Working on: " + Uid);
             System.out.println("Title: " + Title);
             System.out.println("Abstract: " + Abstract);
         } // fi

         if((Title.length() > 1) || (Abstract.length() > 1))
         {
            UID_found = false;
            numRelFound = 0;

            runTexTool(oFileName);
         } // fi
      }  catch (FileNotFoundException ex) { 
           error("1) ERROR - File not found");
      }  catch(IOException f) {
           error("2) ERROR - IOException");
      }
   } // getTexToolRelated

   // ************************************************************************

   public static void runTexTool(String oFileName)
   {
      File f = new File(oFileName);
      boolean UID_found = false;
      int beforeNumRelFound = numRelFound;

      try{
      f.createNewFile();
      } catch(IOException ioE) {
         System.out.println("ERROR - IOException, Could not create Output file:" + oFileName);
         System.exit(-1);
      } // try ... catch
      PrintStream outFile = null;

      try{
         FileOutputStream fos = new FileOutputStream(f);
         outFile = new PrintStream(fos, true);
      } catch(FileNotFoundException fnfe) {
         System.out.println("ERROR - IOException, Could not find Output file:" + oFileName);
         System.exit(-1);
      } // try ... catch

      // Enforce a maximum of 10,000 related articles

      int pagesize, srvndoc;
      pagesize = topnum * 5;
      if(pagesize < 10000)
        srvndoc = pagesize;
      else
      {
          srvndoc = 10000;
          pagesize = 10000;
      } // else

      String n1 = (Title.replace('&', ' ')).replaceAll("  ", " ");
      String n2 = (Abstract.replace('&', ' ')).replaceAll("  ", " ");
      String n3 = "pmid=&title=" + n1 + "&abs=" + n2 +
       "&srvndoc=" + srvndoc + "&pagesize=" + pagesize +
       "&pagecnt=1&host=" + TexToolHost;


      if(DebugDisplay)
      {
          System.out.println();
          System.out.println("oFileName: " + oFileName);
          System.out.println("Call to TexTool: " + n3);
      } // fi

      outFile.println(n3.length());
      outFile.println(n3);
      outFile.close();

      String cmdString =
             "/nfsvol/nls/tools/MTI_resources/bin/TextQueryNlm " +
             oFileName;

      try
      {
         Runtime runtime = Runtime.getRuntime(); 
         Process p = runtime.exec(cmdString.trim());

         BufferedReader buff1URL = 
               new BufferedReader(new InputStreamReader(p.getInputStream()));

         String muidLine;

         while((muidLine = buff1URL.readLine()) != null)
         {
             if(DebugDisplay)
                System.out.println("runTexTool muidLineJ: " + muidLine);

             if(muidLine.startsWith("There are no related documents found"))
                UID_found = true;

             else if(!muidLine.startsWith("The neighbours"))
             {
                int pos = muidLine.indexOf("|");
                String oneUID = muidLine.substring(0, pos);
                String score = muidLine.substring(pos +1);

                if(!oneUID.equals(PMIDstr))
                {
                   if((score.length() > 0) && (numRelFound <= pagesize))
                   {
                       numRelFound++;
                       String tmp = oneUID + "|" + score;
                       PRClist.add(tmp);
                   } // fi score
                } // fi not input PMID

                else
                  UID_found = true;
             } // else fi
         } // while

         buff1URL.close();
         f.delete();  // Now delete the temporary file


         if(numRelFound > 0)
         {
             if(beforeNumRelFound > 5)
                resultsFrom = "PubMed Related Articles" +
                   " - ctr: " + overallCtr + "  - Prev: " + beforeNumRelFound;
             else
                resultsFrom = "TexTool Related Articles" + 
                   " - ctr: " + overallCtr + "  - Prev: " + beforeNumRelFound;

             for(int i=0; i< PRClist.size(); i++)
                System.out.println(PRClist.elementAt(i));
         } // fi
      } // try

      catch (MalformedURLException ex) {
         error("5T) ERROR - Bad URL");
      } catch (UnsupportedEncodingException ex) {
         error("6T) ERROR - Unsupported Encoding Specified.");
      } catch (UnknownServiceException ex) {
         error("7T) ERROR - UnknownServiceException occured.");
      } catch (IOException ex) {
         error("8T) ERROR - IOException occured.");
      }
   } // runTexTool

   // ************************************************************************

   public static void getCatalogNeighbors()
   {
      int pagesize = (topnum * 10);
      double divisor = 2147483647.0;
      double score = 0.0;
      float modScore = 0;

      int maxFound = (topnum * 10);

      if(pagesize > 200)
        pagesize = 200;

      if(maxFound > 200)
        maxFound = 200;

/*
    Step 01: Retrieve the PMID from the citation which is the BIBUI number
             that Cataloging actually uses for referencing.  [ -P ]

    Step 02: Retrieve the NLMID from the citation which just seems to be a key
             into the NCBI database.  [ -M ]

    Step 03: Do an esearch of the NLMID and grab the "ID" line which is the
             internal NCBI number.
 
https://eutils.ncbi.nlm.nih.gov/entrez/eutils/esearch.fcgi?db=nlmcatalog&term=101468148[nlmid]

    Step 04: Now we can retrieve the related articles link information
             (getCatalogNeighbors).

        https://eutils.ncbi.nlm.nih.gov/entrez/eutils/elink.fcgi?dbfrom=nlmcatalog&id=101468148&cmd=neighbor_score&db=nlmcatalog&tool=Medical_Text_Indexer&email=mork@nlm.nih.gov&api_key=8ed2ad8415029f5f018b4ee5da61542e8308

    Step 05: Once we have all of the related article links, we can then pull
             down the actual records associated with the related articles.
             This is in xml format and we'll need to extract the MH/SH terms.
             (runCatalogRetrieval)

https://eutils.ncbi.nlm.nih.gov/entrez/eutils/efetch.fcgi?db=nlmcatalog&id=546987&retmode=xml&api_key=8ed2ad8415029f5f018b4ee5da61542e8308

*/

      getNCBI_ID();

      boolean done = false;
      int ctr = 0;
      boolean firstScore = true;
      while(!done)
      {
         try
         {
            String urlString =
                 "https://eutils.ncbi.nlm.nih.gov/entrez/eutils/elink.fcgi?";

            URL newurl = new URL(urlString + "dbfrom=nlmcatalog&id=" +
                 NCBI_IDstr + "&cmd=neighbor_score" +
                 "&db=nlmcatalog&tool=Medical_Text_Indexer" +
                 "&email=mork@nlm.nih.gov" + 
                 "&api_key=8ed2ad8415029f5f018b4ee5da61542e8308");

            if(DebugDisplay)
              System.out.println("getNC - newurl: #" + newurl + "#");

            URLConnection conn = newurl.openConnection();
            BufferedReader buffURL = 
                new BufferedReader(new InputStreamReader(newurl.openStream()));

            String muidLine = "";
            String lPMID = "";
            String lScore = "";
            done = true;

            while(((muidLine = buffURL.readLine()) != null) &&
                   (numRelFound < maxFound))
            {
                if(DebugDisplay)
                  System.out.println("getNC - muidLine: #" + muidLine + "#");

                int pos = muidLine.indexOf("<Id>");
                if(pos > -1)
                {
                    score = -1;
                    modScore = -1;

                    int posEnd = muidLine.indexOf("</Id>");
                    lPMID = muidLine.substring(pos + 4, posEnd);
                    muidLine = buffURL.readLine();
                    pos = muidLine.indexOf("<Score>");
                    if(pos > -1)
                    {
                       posEnd = muidLine.indexOf("</Score>");
                       lScore = muidLine.substring(pos + 7, posEnd);
                       score = Double.parseDouble(lScore);
                       if(firstScore)
                         divisor = score / 0.8;  // Assume 80% match first
                       firstScore = false;
                       double pcnt = score / divisor;
                       Float tscore = new Float(pcnt);
                       Float jgm = new Float(255);
                       modScore = tscore.floatValue() * jgm.floatValue();

                       if(DebugDisplay)
                         System.out.println("score: " + score + "|pcnt: " +
                                            pcnt + "|tscore: " + tscore +
                                            "|modScore: " + modScore);
                    } // fi

                    if(!lPMID.equals(NLMIDstr) && !lPMID.equals(NCBI_IDstr))
                    {
                        if(score > -1)
                        {
                           if(PMID_list.length() > 0)
                             PMID_list = PMID_list + "," + lPMID;
                           else 
                             PMID_list = lPMID;

                           if(numPMIDs < 200)
                           {
                               PMIDarray[numPMIDs] = lPMID;
                               numPMIDs++;
                           } // fi

                           String newScore = Float.toString(modScore);
                           hScoreUID.put(lPMID, newScore);
                           numRelFound++;
                        } // fi score

                        if(DebugDisplay)
                          System.out.println("Neighbor: " + lPMID + "|" +
                                             score + "|" + modScore);
                    } // fi !original

                    else if(pos > -1) // Original PMID with score - set divisor
                                      // to the new score.
                    {
                        divisor = score;

                        if(DebugDisplay)
                          System.out.println("Divisor: " + lPMID + "|" +
                                             score + "|" + divisor);
                    } /* else fi */
                } // fi
            } // while

            // Make sure buffer is empty since we stop after we have enough
            // info.

             while((muidLine = buffURL.readLine()) != null)
             {
                 if(DebugDisplay)
                   System.out.println("Cleanup Loop2: #" + muidLine + "#");
             } // while

            buffURL.close();

            if(numRelFound > 0)
              resultsFrom = "NLM Catalog Related Records";
         } // try

         catch (MalformedURLException ex) {
            error("5C) ERROR - Bad URL");
         } catch (UnsupportedEncodingException ex) {
            error("6C) ERROR - Unsupported Encoding Specified.");
         } catch (UnknownServiceException ex) {
            error("7C) ERROR - UnknownServiceException occured.");
         } catch (IOException ex)
         {
            ctr++;
            if(ctr < 2)
            {
                System.err.println("8C) IO Exception found: " + ex);
                done = false;
                try
                {
                    System.err.println("Sleeping for 3 seconds ...");
                    Thread.sleep(3000);
                    System.err.println("Let's try once more ...");
                } // try

                catch (Exception exT)
                { System.err.println("Sleep Interrupted: " + exT); }
            } // fi

            else
            {
                System.err.println("Stopping after 3 attempts" +
                                   " on getCatalogNeighbors");
                error("8C) ERROR - IOException occured.");
            } // else
         } // catch
      } // while !done
   } // getCatalogNeighbors

   // ************************************************************************

   public static void runCatalogRetrieval()
   {
      boolean done, doRead;

      /*
        Step 05: Once we have all of the related article links, we can then pull
                down the actual records associated with the related articles.
                This is in xml format and we'll need to extract the MH/SH terms.

        https://eutils.ncbi.nlm.nih.gov/entrez/eutils/efetch.fcgi?db=nlmcatalog&id=546987&report=xml&mode=text

      */

      checkCatalogPMID_list();

      boolean outerDone = false;
      int ctr = 0;
      numRelFound = 0;

      if(PMID_list.length() == 0)
        outerDone = true;

      while(!outerDone)
      {
         try
         {
             // Sample: https://eutils.ncbi.nlm.nih.gov/entrez/eutils/efetch.fcgi?db=nlmcatalog&id=546987&report=xml&mode=text&tool=Medical_Text_Indexer&email=mork@nlm.nih.gov&api_key=8ed2ad8415029f5f018b4ee5da61542e8308

             String urlString =
                    "https://eutils.ncbi.nlm.nih.gov/entrez/eutils/efetch.fcgi";
             URL newurl = new URL(urlString + "?" + "db=nlmcatalog&id=" +
                     PMID_list + "&retmode=xml" + 
                     "&tool=Medical_Text_Indexer&email=mork@nlm.nih.gov" +
                     "&api_key=8ed2ad8415029f5f018b4ee5da61542e8308");

             URLConnection conn = newurl.openConnection();
             BufferedReader buffURL = 
                new BufferedReader(new InputStreamReader(newurl.openStream()));

             String Line = "";
             String UID = "";
             String lPMID = "";
             String lNCBIUI = "";
             int PMIDptr = 0;
             done = false;
             boolean foundData = false;
             outerDone = true;
             boolean foundDescript = false;
             int descriptCtr = 0;

             while(!done)
             {
                if(DebugDisplay)
                   System.out.println("runCatalogRetrieval Line: " + Line);

                doRead = true;

                if(Line.startsWith("pmfetch need params:"))
                    System.exit(0);

                else if(Line.indexOf("Error occured: Document internals") > -1)
                   errorFound = true;

                else if(Line.indexOf("Error occurred: PubMed article server") > -1)
                   errorFound = true;

                else if(Line.indexOf("<NlmUniqueID>") > -1)
                {
                    foundDescript = false;
                    int pos = Line.indexOf("<NlmUniqueID>");
                    int posEnd = Line.indexOf("</NlmUniqueID>");
                    lPMID = Line.substring(pos + 13, posEnd);
                    foundData = true;
                    if(PMIDptr < numPMIDs)
                    {
                       lNCBIUI = PMIDarray[PMIDptr];
                       PMIDptr++;
                    } // fi
                } // else fi PMID

                else if(Line.indexOf("</NLMCatalogRecord>") > -1)
                {
                    if(foundDescript)
                    {
                       descriptCtr++;
                       numRelFound++;
                    } // fi

                    if(descriptCtr >= topnum)
                      done = true;
                    foundDescript = false;
                } /* else fi */

                else if(Line.indexOf("<MeshHeadingList>") > -1)
                {
                    long ctrMH = 0;
                    String tmp = PMIDstr + "|" + hScoreUID.get(lNCBIUI) + 
                                 "|" + lNCBIUI + "|" + lPMID + "|";
                    while(Line.indexOf("</MeshHeadingList>") < 0)
                    {
                        Line = buffURL.readLine();
                        if(DebugDisplay)
                          System.out.println("runCatalogRetrieval MHList: " +
                                             Line);
                        if(Line.indexOf("<MeshHeading>") > -1)
                        {
                            String isIM = "NIM";
                            String MeSH_Term = "";
                            String SH_TermList = "";
                            int ctrSH = 0;
                            while(Line.indexOf("</MeshHeading>") < 0)
                            {
                                Line = buffURL.readLine();
                                if(DebugDisplay)
                                  System.out.println("runCatalogRetrieval" +
                                         " MHList2: " + Line);

                                if(Line.indexOf("MajorTopicYN=\"Y\"") > -1)
                                   isIM = "IM";

                                if(Line.indexOf("<DescriptorName") > -1)
                                {
                                    foundDescript = true;
                                    int pos = Line.indexOf(">");
                                    int posEnd =
                                           Line.indexOf("</DescriptorName>");
                                    MeSH_Term = Line.substring(pos + 1, posEnd);
                                } /* fi */

                                else if(Line.indexOf("<QualifierName") > -1)
                                {
                                    int pos = Line.indexOf(">");
                                    int posEnd =
                                             Line.indexOf("</QualifierName>");
                                    String SH_Term = 
                                              Line.substring(pos + 1, posEnd);

                                    String t = SH_Term.replace("&amp;", "&");
                                    if(ctrSH > 0)
                                     SH_TermList = SH_TermList + "/";
                                    SH_TermList = SH_TermList + t;
                                    ctrSH++;
                                } /* else fi */
                            } /* while */

                            if(ctrMH > 0)
                              tmp = tmp + ":";
                            tmp = tmp + MeSH_Term + "^" + isIM;
                            if(ctrSH > 0)
                              tmp = tmp + "^" + SH_TermList;
                            ctrMH++;
                        } /* fi MeshHeading */
                    } /* while */

                    if(ctrMH > 0)
                      System.out.println(tmp);
                } /* else fi MH List */

                if(doRead)
                  Line = buffURL.readLine();

                if(Line == null)
                  done = true;
             } // while 

             // Make sure buffer is empty since we stop after we have enough
             // info.

             while((Line = buffURL.readLine()) != null)
             {
                 if(DebugDisplay)
                   System.out.println("Cleanup Loop4: #" + Line + "#");
             } // while

             buffURL.close();
             System.out.flush();
         } // try

         catch (MalformedURLException ex) {
            error("9C) ERROR - Bad URL");
         } catch (UnsupportedEncodingException ex) {
            error("10C) ERROR - Unsupported Encoding Specified.");
         } catch (UnknownServiceException ex) {
            error("11C) ERROR - UnknownServiceException occured.");
         } catch (IOException ex)
         {
            ctr++;
            if(ctr < 2)
            {
                System.err.println("12C) IO Exception found: " + ex);
                outerDone = false;
                try
                {
                    System.err.println("Sleeping for 3 seconds ...");
                    Thread.sleep(3000);
                    System.err.println("Let's try once more ...");
                } // try

                catch (Exception exT)
                { System.err.println("Sleep Interrupted: " + exT); }
            } // fi

            else
            {
                System.err.println("Stopping after 3 attempts" +
                                   " on runCatalogRetrieval");
                error("12C) ERROR - IOException occured: " + PMIDstr);
            } // else
         } // catch
      } // while !outerDone
   } // runCatalogRetrieval

   // ************************************************************************

   public static void checkCatalogPMID_list()
   {
      // Go through PMID_list and identify which PMIDs are no longer
      // valid PMIDs (have been deleted, etc.), then rebuild the
      // PMID_list variable with what we find.

      try
      {
          if(DebugDisplay)
             System.out.println("checkCatalogPMID_list - PMID_list: " +
                                PMID_list);

          // Sample: https://eutils.ncbi.nlm.nih.gov/entrez/eutils/esummary.fcgi?db=pubmed&id=112170

          // Sample: https://eutils.ncbi.nlm.nih.gov/entrez/eutils/esummary.fcgi?db=pubmed&id=16772037

          String urlString =
                 "https://eutils.ncbi.nlm.nih.gov/entrez/eutils/esummary.fcgi";
          URL newurl = new URL(urlString + "?" + "db=nlmcatalog&id=" +
                  PMID_list + "&rettype=medline&retmode=text" + 
                  "&tool=Medical_Text_Indexer&email=mork@nlm.nih.gov" +
                  "&api_key=8ed2ad8415029f5f018b4ee5da61542e8308");

          URLConnection conn = newurl.openConnection();
          BufferedReader buffURL = 
              new BufferedReader(new InputStreamReader(newurl.openStream()));

          String lPMID = "";
          String Line = "";
          PMID_list = "";
          while((Line = buffURL.readLine()) != null)
          {
             if(DebugDisplay)
                System.out.println("checkCatalogPMID_list Line: " + Line);
             int pos = Line.indexOf("<Id>");
             int posEnd = Line.indexOf("</Id>");
             if(pos > -1)
             {
                 lPMID = Line.substring(pos + 4, posEnd);
                 if(DebugDisplay)
                   System.out.println("checkCatalogPMID_list - lPMID: #" +
                                      lPMID + "#");
                 if(PMID_list.length() > 0)
                   PMID_list = PMID_list + "," + lPMID;
                 else 
                   PMID_list = lPMID;
             } // fi
          } // while 

          buffURL.close();
      } // try

      catch (MalformedURLException ex) {
            error("9) ERROR - Bad URL");
      } catch (UnsupportedEncodingException ex) {
            error("10) ERROR - Unsupported Encoding Specified.");
      } catch (UnknownServiceException ex) {
            error("11) ERROR - UnknownServiceException occured.");
      } catch (IOException ex)
      {
         error("12) ERROR - IOException occured: " + PMID_list);
      } // catch
   } // checkCatalogPMID_list

   // ************************************************************************

   public static void getNCBI_ID()
   {
      /*
        Step 03: Do an esearch of the NLMID and grab the "ID" line which is the
                 internal NCBI number.
 
https://eutils.ncbi.nlm.nih.gov/entrez/eutils/esearch.fcgi?db=nlmcatalog&term=14311410R[nlmid]&tool=Medical_Text_Indexer&email=mork@nlm.nih.gov&api_key=8ed2ad8415029f5f018b4ee5da61542e8308
      */

      boolean done = false;
      int ctr = 0;
      while(!done)
      {
         try
         {
            String urlString =
                "https://eutils.ncbi.nlm.nih.gov/entrez/eutils/esearch.fcgi?";

            URL newurl = new URL(urlString + "db=nlmcatalog&term=" +
                    NLMIDstr + "[nlmid]&tool=Medical_Text_Indexer" +
                    "&email=mork@nlm.nih.gov" +
                    "&api_key=8ed2ad8415029f5f018b4ee5da61542e8308");

            if(DebugDisplay)
              System.out.println("getNCBI_ID: URL#" + newurl + "#");

            URLConnection conn = newurl.openConnection();
            BufferedReader buffURL = 
                new BufferedReader(new InputStreamReader(newurl.openStream()));

            String muidLine = "";
            done = true;

            while((muidLine = buffURL.readLine()) != null)
            {
                if(DebugDisplay)
                  System.out.println("getNCBI_ID - muidLine: #" +
                                     muidLine + "#");
                int pos = muidLine.indexOf("<Id>");
                if(pos > -1)
                {
                    int posEnd = muidLine.indexOf("</Id>");
                    NCBI_IDstr = muidLine.substring(pos + 4, posEnd);
                    if(DebugDisplay)
                      System.out.println("getNCBI_ID - NCBI_IDstr: #" +
                                         NCBI_IDstr + "#");
                } // fi
            } // while

            buffURL.close();
         } // try

        catch (MalformedURLException ex) {
            error("5N) ERROR - Bad URL");
         } catch (UnsupportedEncodingException ex) {
            error("6N) ERROR - Unsupported Encoding Specified.");
         } catch (UnknownServiceException ex) {
            error("7N) ERROR - UnknownServiceException occured.");
         } catch (IOException ex) 
         {
            ctr++;
            if(ctr < 2)
            {
                System.err.println("8N) IO Exception found: " + ex);
                done = false;
                try
                {
                    System.err.println("Sleeping for 3 seconds ...");
                    Thread.sleep(3000);
                    System.err.println("Let's try once more ...");
                } // try

                catch (Exception exT)
                { System.err.println("Sleep Interrupted: " + exT); }
            } // fi

            else
            {
                System.err.println("Stopping after 3 attempts on getNCBI_ID");
                error("8N) ERROR - IOException occured.");
            } // else
         } // catch
      } // while !done
   } // getNCBI_ID

   public static int findPos(String text, int sPos)
   {
      int rtn;
      boolean done = false;

      rtn = sPos - 1;
      while(!done && (rtn > 0))
      {
          char aChar = text.charAt(rtn);

          if((aChar == ' ') || (aChar == '.') || (aChar == ';') ||
             (aChar == ':') || (aChar == '?') || (aChar == '!'))
            done = true;

          if(!done)
            rtn--;
      } // while

      return(rtn);
   } // findPos
} // class GetPRC_NCat_HTTPS
