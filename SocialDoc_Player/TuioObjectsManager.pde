// java imports
import java.util.*;

// Processing import
import TUIO.*;

class TuioObjectsManager {
 
 private ArrayList<Vector> objectsList;
 private IntList tagsIds;
 private int maxLength;
 private int lastEventTime;
 private int timeFilterMs;
 
 private boolean realEvent;
 private IntList lastTagsIDs;
 
 // initialize tweets list from tweetFile (txt) 
  public TuioObjectsManager() {
    objectsList = new ArrayList<Vector>();
    maxLength = 12;
    
    lastEventTime = millis();
    timeFilterMs = 1500;
    
    realEvent = false;
    tagsIds = new IntList();
    lastTagsIDs = new IntList();
  }
  
  public void pushObjects(Vector objects) {
    if(millis()-lastEventTime > timeFilterMs) {
      objectsList.clear();
    }
    objectsList.add(objects);
    if(objectsList.size() > maxLength) {
      objectsList.remove(0);
    }
    
    
    int[] tagIdsNumb = new int[180];
    for(int k = 0; k<180; k++)
      tagIdsNumb[k] = 0;
    for(int i = 0; i<objectsList.size(); i++) {
      Vector obj = objectsList.get(i);
      for (int j=0;j<obj.size();j++) {
        TuioObject o = (TuioObject)obj.elementAt(j);
        int id = o.getSymbolID();
        tagIdsNumb[id]++;
      }
    }
    
    // get the most common tags in the array
    lastTagsIDs.clear();
    for(int i = 0; i < tagsIds.size(); i++) {
      lastTagsIDs.append(tagsIds.get(i));
    }
    tagsIds.clear();
    for (int i= 0; i< 3 ; i++) {
      int val = 0; 
      int id = 0;
      for(int j=0; j<tagIdsNumb.length; j++) {
        if(tagIdsNumb[j] > val) {
          val = tagIdsNumb[j];
          id = j;
        }
      }
      if(val > 0) {
        tagsIds.append(id);
        tagIdsNumb[id] = 0;
      }
    }
    
    realEvent = !sameIntLists(tagsIds, lastTagsIDs);
    //if(realEvent)
    //  print("it is a real event!\n");
  }
  
  public int[] getTagsIDs() {
    return tagsIds.array();
  }
  
  public boolean isRealEvent(){
    return realEvent;
  }
  
  
  private boolean sameIntLists(IntList l1, IntList l2) {
    //print(l1 + " "+ l2 +"\n");
    if(l1.size() != l2.size())
      return false;
    IntList tl1 = l1;
    IntList tl2 = l2;
    tl1.sort();
    tl2.sort();
    boolean same = true;
    for(int i = 0; i<tl1.size(); i++) {
      same &= tl1.get(i) == tl2.get(i);
    }
    return same;
  }
}

