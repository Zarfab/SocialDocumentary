 import java.util.*;
 
 
 public class Segment {
   
   // Attributes for each segment
  private int indexInFile, rate;
  private float startTime, endTime;
  private int relevance;
  private StringList annotKeywords;
  private String[] segmentStrings;
  private StringList relevantKeywords;
  
  
  // constructor by default
  public Segment() {
    indexInFile = 0;
    rate = 1;
    startTime = (float)0.0;
    endTime = (float)0.0;
    annotKeywords = new StringList();
    segmentStrings = new String[3];
    relevantKeywords = new  StringList();
  }
  
  // constructor with all fields
  public Segment(int p_index,
                  int p_rate, 
                  double p_start, 
                  double p_end,
                  StringList p_annotations,
                  String[] segStrings) {
    indexInFile = p_index;
    rate = p_rate;
    startTime = (float)p_start;
    endTime = (float)p_end;
    
    segmentStrings = segStrings;
    
    annotKeywords = new StringList();
    for(int i=0; i<p_annotations.size(); i++)
      annotKeywords.append(p_annotations.get(i));      
    //System.out.println(this.toString());
    relevantKeywords = new  StringList();
  }
   
   // re-convert the Segment object to a srt-type string for saving
   public String toString() {
     String temp = "";
     for(int i=0; i<segmentStrings.length; i++)
       temp += segmentStrings[i] + "\n";
     
     temp += rate + "\n";
     return temp;
   }
   
   // getters and setters
   public int getIndexInFile() {
     return indexInFile;
   }
   
   public int getRate() {
     return rate;
   }
   
   public float getStartTime() {
     return startTime;
   }
   
   public float getEndTime() {
     return endTime;
   }
   
   public int getRelevance() {
     return relevance;
   }
   
   public  StringList getRelevantKeywords() {
     return relevantKeywords;
   }
   
   public StringList getKeywords() {
     return annotKeywords;
   }
   
   // return the relevance of this segment in relation with the list of chosen tags
   public int calculateRelevance(StringList selectedKeywords) {
     // TODO : 
     //      calculate and update relevance
     
     // simple version
     relevance = 0;
     relevantKeywords.clear();
     for(int i=0; i<selectedKeywords.size(); i++) {
       if(annotKeywords.hasValue(selectedKeywords.get(i))) {
         relevance++;
         relevantKeywords.append(selectedKeywords.get(i));
       }
     }
     if(relevance > 3)
       relevance = 3;
     return relevance;
   }
   
   public void increaseRate() {
     rate++;
   }
   
 }

