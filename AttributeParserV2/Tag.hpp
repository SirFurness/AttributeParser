//
//  Tag.hpp
//  AttributeParserV2


#ifndef Tag_hpp
#define Tag_hpp

#include <string>
#include <vector>

using namespace std;

//aaaaaand a final comment here

class Tag {
public:
    Tag();
    Tag(string name);
    Tag(string name, string attributeName);
    Tag(string name, string attributeName, string attributeValue);
    
    
    bool create(string tagLine, int tagLineNumber);
    
    void setName(string name);
    void setClosingTagLineNumber(int line);
    
    string findClosingTagName(string str);
    
    bool isNested(const vector<Tag> &tags);
    
    
    string getName();
    int getClosingTagLine();
    int getStartingTagLine();
    vector<string> getNested();
    vector<string> getAttributeName();
    vector<string> getAttributeValue();
private:
    bool findTagName(string tagLine, int &spaceCell, int tagLineNumber);
    bool findAttributeName(string tagLine, int spaceCell, int &lastSpaceCell);
    void findAttributeValue(string tagLine, int lastSpaceCell, int &spaceCell);
    
    
    int closingTagLine;
    int startingTagLine;
    
    string name;
    vector<string> nested;
    vector<string> attributeName;
    vector<string> attributeValue;
    
};

#endif /* Tag_hpp */
