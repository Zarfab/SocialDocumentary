import java.util.*;
import java.util.regex.Pattern;
import java.util.regex.Matcher;
import java.text.*;

import java.io.File;
import java.io.FileNotFoundException;
import java.io.PrintWriter;


class SegmentManager {
  
 private ArrayList<Segment> allSegments;
 private ArrayList<ArrayList<Segment> > relevantSegments;
 private Segment currentSeg;
 private ArrayList<Segment> playedSegments;
 private int playedIndex = -1;
 private String srtFilePath;
 private ArrayList<String> lastTags;
 private  ArrayList<Integer> lastIds;

  
 private void init() {
   allSegments = new ArrayList<Segment>();
   playedSegments = new ArrayList<Segment>();
   
   relevantSegments = new ArrayList<ArrayList<Segment>>();
   for(int i = 0; i<4; i++)
     relevantSegments.add(new ArrayList<Segment>());
   
   currentSeg = new Segment();
   
   lastTags = new ArrayList<String>();
   lastIds = new  ArrayList<Integer>();
 }
  
 public SegmentManager() {
    init();
    //addTestSegment();
 }
 
 // init the variables, load all keywords list from xml and create segments from srt file
 public SegmentManager(String p_srtFile) {
   init();
   loadFromFile(p_srtFile);
   System.out.println(allSegments.size() + " segments were loaded\n");
 }
 
// load all keywords list from xml and create segments from srt file 
 public void loadFromFile(String p_srtFile) {
   readSegmentsFromFile(p_srtFile);
   srtFilePath = p_srtFile;
 }
 
 // save current state into another srtFile
 public void saveStateInFile() {
   // create the name of the new file
   Date now = new Date();
   SimpleDateFormat sdf = new SimpleDateFormat("yyyyMMdd_HHmm");
   String savedFilePath = srtFilePath.substring(0, srtFilePath.lastIndexOf('.')) + "_" + sdf.format(now) + ".srt"; 
   // create and open the new file
   try {
     //File file = new File(savedFilePath);
     //file.getParentFile().mkdirs();
     PrintWriter writer = new PrintWriter(savedFilePath);  
     // write in the file
     for(int i=0; i<allSegments.size(); i++) {
       writer.println(allSegments.get(i));
     }
     writer.close();
   }
   catch(Exception e) {
     e.printStackTrace();
   }
 }

// to be called when a new list of tags is chosen by the user
 public void calculateNewSegmentsList(ArrayList<String> p_tags, int[] p_ids) {
   
   // check if the tags are the same than previously and ignore the rest if true
   //if(lastTags.size() == p_tags.size()) {
   //  boolean sameTags = true;
   //  for(int i = 0; i<lastTags.size(); i++)
   //    sameTags &= lastTags.get(i).equals(p_tags.get(i));
   //  if(sameTags) {
   //    System.out.println("the list of word is the same\n");
   //    return;
   //  }  
   //}
   
   // clear the relevant segments array
   for(int i = 0; i<relevantSegments.size(); i++)
     relevantSegments.get(i).clear();
    
   // fill it with all the segment ordered by relevance
   for(int i=0; i<allSegments.size(); i++) {
     Segment seg = allSegments.get(i);
     seg.calculateRelevance(p_tags, p_ids);
     relevantSegments.get(seg.getRelevance()).add(seg);
   }
   
   lastTags = p_tags;
   for(int i=0; i<p_ids.length; i++) {
     lastIds.add(p_ids[i]);
   }
   
   playedSegments.clear();
   playedIndex = -1;
 }
 
 // to be called when the player is at the end of a segment or when a new tag is chosen
 public void goToNextSegment() {  
   if(playedSegments.size()-1 == playedIndex) {
     // get the first part in relevanceSegments where there are some segments
     int rel=3;
     while(relevantSegments.get(rel).size() <= 0 && rel > 0) {
       rel--;
       //System.out.println(relevantSegments.get(rel).size());
     }   
   
     // if relevant segment list is empty, fill it with the same keywords
     if(rel == 0 && relevantSegments.get(0).size() == 0) {
       ArrayList<String> tempTags = lastTags;
       int[] tempIds = new int[lastIds.size()];
       for(int i=0; i<tempIds.length; i++) {
         tempIds[i] = lastIds.get(i);
       }
       lastTags = new ArrayList<String>();
       calculateNewSegmentsList(tempTags, tempIds);
     }
   
     // select a segment by roulette technic
     // calculate sum of rates in this part
     int sumOfRates = 0;
     for(int i=0; i<relevantSegments.get(rel).size(); i++)
       sumOfRates += relevantSegments.get(rel).get(i).getRate();
       // get a random value between 1 and sumOfRate
     int random = (int)(Math.floor(Math.random() * (sumOfRates))) + 1;
       // get the segment
     int k = random;   
     int i = -1;
     while(i<relevantSegments.get(rel).size() && k > 0) {
       i++;
       //System.out.println("k = "+ k + " i = " +i);
       k -= relevantSegments.get(rel).get(i).getRate();
    
     }
   
     currentSeg = relevantSegments.get(rel).remove(i);
     playedSegments.add(currentSeg);
     playedIndex++;
   }
   else {
     playedIndex++;
     currentSeg = playedSegments.get(playedIndex);
   }
 }
 
 public void goToPreviousSegment() {
   if(playedIndex > 1) {
     playedIndex--;
     currentSeg = playedSegments.get(playedIndex);
   }
 }
 
 // to be called in case of user's attention is going over 2
 public void incrementCurrent() {
   currentSeg.increaseRate();
 }
 
 public Segment getCurrentSegment() {
   return currentSeg;
 }
 
 // create the list of all segments from srt file
 private void readSegmentsFromFile(String p_srtFile) {
   try {
     File segmentFile = new File(p_srtFile);
     Scanner scan = new Scanner(segmentFile);
     int lineInd = 0;
     
     int tempIndex = 0;
     double tempStartTime = 0.0;
     double tempEndTime = 0.0;
     ArrayList<String> tempTags = new ArrayList<String>();
     int tempRate = 1;
     String[] segStrings = new String[3];
     
     String str = new String("");
     Pattern pIndex = Pattern.compile("([0-9]+)");
     Matcher mIndex;
     
     while(scan.hasNextLine()) {
      str = scan.nextLine();
      //System.out.println(str);
      switch(lineInd) {
        case 0: // segment index
          segStrings = new String[3];
          segStrings[0] = str;
          mIndex = pIndex.matcher(str);
          if (mIndex.find()) {
            tempIndex = Integer.parseInt(mIndex.group(0));
          }
          break;
        case 1: // start and end times
          segStrings[1] = str;
          String timeString[] = {"", ""};
          timeString = str.split(" --> ");
          tempStartTime = timeStringToDouble(timeString[0]);
          tempEndTime = timeStringToDouble(timeString[1]);
          break;
        case 2: // keywords
          segStrings[2] = str;
          tempTags.clear();
          tempTags.addAll(extractTagsFromString(str));
          break;
        case 3:
          mIndex = pIndex.matcher(str);
          if (mIndex.find()) {
            tempRate = Integer.parseInt(mIndex.group(0));
          }
          break;
        case 4: // push new segment
          allSegments.add(new Segment(tempIndex, tempRate, tempStartTime, tempEndTime, tempTags, segStrings));
          break;
        default: // new line
          break;
      }
      lineInd = (lineInd+1)%5;
     }
   }
   catch(Exception e) {
     e.printStackTrace();
   }
 }
 
 
 // used to convert the time string from srt file to seconds
 private double timeStringToDouble(String timeString) {
   // replace coma by a point for double parsing
   timeString = timeString.replace(',', '.');
   
   String hms[] = timeString.split(":");
   
   return 3600*Double.parseDouble(hms[0]) + 60*Double.parseDouble(hms[1]) + Double.parseDouble(hms[2]);
 }
 // used to get the tags from srt file for each segment
 private ArrayList<String> extractTagsFromString(String tagsString) {
   ArrayList<String> tags = new ArrayList<String>();
   
   String allTags[] = tagsString.split("-");
   for(int i = 0; i < allTags.length; i++) {
    String classTags[] = allTags[i].split(", ");
    for(int j=0; j<classTags.length; j++) {
     classTags[j].toLowerCase();
     classTags[j] = classTags[j].replaceAll(" ", "");
     tags.add(classTags[j]);
    } 
   }
   return tags;
 }   
}
