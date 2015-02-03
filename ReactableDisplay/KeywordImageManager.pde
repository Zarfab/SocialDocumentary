import TUIO.*;  

public class KeywordImageManager {
  
  KeywordImage keywordImages[];
  IntList identifiedTags;
  
  public KeywordImageManager(float scale_factor) {
    keywordImages = new KeywordImage[18];
    identifiedTags = new IntList();
    
    String categories[] = {"people", "emotion", "action"};
    for (int i = 0; i < 3; i++ ){
      for(int j=0; j<6; j++) {
        String activeName = categories[i]+"_"+j+"b.png";
        String unactiveName = categories[i]+"_"+j+"n.png";
        keywordImages[i*6+j] = new KeywordImage(activeName, unactiveName, scale_factor);
      }
    }
  }
  
  public void setActiveTags(IntList newActiveTags) {
    for(int i=0; i<18; i++) {
      keywordImages[i].setActive(false);
      if(newActiveTags.hasValue(i))
        keywordImages[i].setActive(true);
    }
  }
  
  public void drawKeywords(PGraphics screen, Vector tuioObjectList, int mode) {
    for (int i=0;i<tuioObjectList.size();i++) {      
      TuioObject tobj = (TuioObject)tuioObjectList.elementAt(i);
      int id = tobj.getSymbolID();
      keywordImages[id].checkLastDraw(); 
      keywordImages[id].pushXYA(tobj.getScreenX(screen.width), tobj.getScreenY(screen.height), tobj.getAngle());       
      switch(mode) {
        case 1:
          keywordImages[id].drawMeanFilter(screen);
          break;
        case 2:
          keywordImages[id].drawMedianFilter(screen);
          break;
        case 3:
          keywordImages[id].drawWithTrace(screen);
          break;
        default:
          keywordImages[id].drawRow(screen);
          break;
      }
    }
  }
  
}
