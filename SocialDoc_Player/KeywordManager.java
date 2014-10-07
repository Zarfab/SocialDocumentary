import java.util.*;
import javax.xml.parsers.DocumentBuilderFactory;
import javax.xml.parsers.DocumentBuilder;
import org.w3c.dom.Document;
import org.w3c.dom.NodeList;
import org.w3c.dom.Node;
import org.w3c.dom.Element;

import java.io.File;
import java.io.FileNotFoundException;

class KeywordManager {
   private ArrayList<String> allKeywords;
   private ArrayList<String> selectedKeywords;
   private Hashtable tagKeywordMapping;

   
   public KeywordManager(String p_keywordsXML) {
     init();
     readKeywordsFromFile(p_keywordsXML);
     //setMapping();
   }
   
   private void init() {
     allKeywords = new ArrayList<String>();
     selectedKeywords = new ArrayList<String>();
     tagKeywordMapping = new Hashtable();
   }
    
   private void setMapping() {
    for(int i=0; i<allKeywords.size(); i++) {
     tagKeywordMapping.put(i, allKeywords.get(i));
     System.out.println("Tag ID: " + (i+1) + ", keyword: " + allKeywords.get(i));
    }
   }
  
   
 // create the list of all keywords from the xml file
 public void readKeywordsFromFile(String p_keywordsXMLFile) {
   try {
    File fXmlFile = new File(p_keywordsXMLFile);
    DocumentBuilderFactory dbFactory = DocumentBuilderFactory.newInstance();
    DocumentBuilder dBuilder = dbFactory.newDocumentBuilder();
    Document doc = dBuilder.parse(fXmlFile);
    //optional, but recommended
    doc.getDocumentElement().normalize();
    
    NodeList nList = doc.getElementsByTagName("keyword");
    for (int temp = 0; temp < nList.getLength(); temp++) {
      Node nNode = nList.item(temp);
      if (nNode.getNodeType() == Node.ELEMENT_NODE) {
        Element eElement = (Element) nNode;
        String word = (String)(eElement.getTextContent());
        int tagID = Integer.parseInt(eElement.getAttribute("tag"));
        allKeywords.add(word);
        tagKeywordMapping.put(tagID, word);
        System.out.println("Tag ID: " + tagID + ", keyword: " + word);
      }
    }
  }
  catch (Exception e) {
    e.printStackTrace();
  }
  // Debug print
  //String dispKeywords = "keywords:";
  //for(int i = 0; i < allKeywords.size(); i++)
  //  dispKeywords += (" '" + allKeywords.get(i) + "'");
  //System.out.println(dispKeywords);
 }
  
 
 public void setSelectedKeywordsFromTagIDs(int[] tagNumb) {
   selectedKeywords.clear();
   for(int i=0; i<tagNumb.length; i++) {
     selectedKeywords.add((String)tagKeywordMapping.get(tagNumb[i]));
   }  
 }
 
 public ArrayList<String> getSelectedKeywords() {
   return selectedKeywords;
 }
  
}
