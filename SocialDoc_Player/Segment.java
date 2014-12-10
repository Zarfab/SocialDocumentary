 import java.util.*;
 
 
 public class Segment {
   
   // Attributes for each segment
  private int indexInFile, rate;
  private float startTime, endTime;
  private int relevance;
  private ArrayList<String> tags;
  private String[] segmentStrings;
  private ArrayList<Integer> relevantTags;
  
  
  // constructor by default
  public Segment() {
    indexInFile = 0;
    rate = 1;
    startTime = (float)0.0;
    endTime = (float)0.0;
    tags = new ArrayList<String>();
    segmentStrings = new String[3];
    relevantTags = new  ArrayList<Integer>();
  }
  
  // constructor with all fields
  public Segment(int p_index,
                  int p_rate, 
                  double p_start, 
                  double p_end,
                  ArrayList<String> p_tags,
                  String[] segStrings) {
    indexInFile = p_index;
    rate = p_rate;
    startTime = (float)p_start;
    endTime = (float)p_end;
    
    segmentStrings = segStrings;
    
    tags = new ArrayList<String>();
    for(int i=0; i<p_tags.size(); i++)
      tags.add(p_tags.get(i));      
    //System.out.println(this.toString());
    relevantTags = new  ArrayList<Integer>();
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
   
   public  ArrayList<Integer> getRelevantTags() {
     return relevantTags;
   }
   
   public ArrayList<String> getTags() {
     return tags;
   }
   
   // return the relevance of this segment in relation with the list of chosen tags
   public int calculateRelevance(ArrayList<String> p_newTags, int[] p_ids) {
     // TODO : 
     //      calculate and update relevance
     
     // simple version
     relevance = 0;
     relevantTags.clear();
     for(int i=0; i<p_newTags.size(); i++) {
       if(tags.contains(p_newTags.get(i))) {
         relevance++;
         relevantTags.add(p_ids[i]);
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

