import TUIO.*;  

public class KeywordVisualManager {
  
  ArrayList<KeywordVisual> keywordVisuals;
  Vector tuioObjects;
  IntList identifiedTags;
  
  public KeywordVisualManager(PApplet parent, String keywordsFile, float scale_factor) {
    keywordVisuals = new ArrayList<KeywordVisual>();
    identifiedTags = new IntList();
    
    String classesString[] = {"people", "action", "emotion"}; 
    XML xml = loadXML(keywordsFile);
    
    for(int c = 0; c < classesString.length; c++) {
      XML currentClass = xml.getChild(classesString[c]);
      //get color for the class
      color classColor = unhex(currentClass.getString("color"));
      
      XML[] classKeywords = currentClass.getChildren("keyword");
      for(int ck = 0; ck < classKeywords.length; ck++) {
        //get id
        int id = classKeywords[ck].getInt("tag");
        //get word
        String word = classKeywords[ck].getContent();
        //create the visual
        println("creating new keyword visual : "+word+", id="+id+", color="+hex(classColor, 6));
        keywordVisuals.add(new KeywordVisual(parent, classColor, word, id, scale_factor));
      }
    }
  }
  
  private int getIndexOfKeywordVisualByTagId(int tagId) {
    for(int i = 0; i < keywordVisuals.size(); i++) {
      if(keywordVisuals.get(i).getTagId() == tagId)
        return i;
    }
    return -1;
  }
  
  public void updateTUIOList(Vector tuioObjectList) {
    tuioObjects = tuioObjectList;
    identifiedTags.clear();
    for (int i=0;i<tuioObjects.size();i++) {      
      TuioObject tobj = (TuioObject)tuioObjects.elementAt(i);
      identifiedTags.append(tobj.getSymbolID());
    }
  }
  
  public StringList getIdentifiedKeywords() {
    StringList identifiedKeywords = new StringList();
    for (int i=0;i<identifiedTags.size();i++) {      
      int index = getIndexOfKeywordVisualByTagId(identifiedTags.get(i));
      if(index >= 0) {
        identifiedKeywords.append(keywordVisuals.get(index).getKeyword()); 
      }
    }
    return identifiedKeywords;
  }
  
  public void setActiveTags(IntList newActiveTags) {
    for(int i=0; i<keywordVisuals.size(); i++) {
      keywordVisuals.get(i).setActive(false);
      if(newActiveTags.hasValue(keywordVisuals.get(i).getTagId()))
        keywordVisuals.get(i).setActive(true);
    }
  }
  
  public void drawKeywords(PGraphics screen, boolean applyFilter) {
    for (int i=0;i<tuioObjects.size();i++) {      
      TuioObject tobj = (TuioObject)tuioObjects.elementAt(i);     
      int index = getIndexOfKeywordVisualByTagId(tobj.getSymbolID());
      if(index >= 0) {
        keywordVisuals.get(index).checkLastDraw(); 
        keywordVisuals.get(index).pushXYA(tobj.getScreenX(screen.width), tobj.getScreenY(screen.height), tobj.getAngle(), screen);
        if(applyFilter)      
          keywordVisuals.get(index).drawFiltered(screen);
        else
          keywordVisuals.get(index).drawRaw(screen);
      }
    }
  }
  
}
