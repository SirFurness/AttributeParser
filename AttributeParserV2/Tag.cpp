//
//  Tag.cpp
//  AttributeParserV2
//
//  Created by Eric on 3/5/16.
//  Copyright © 2016 Eric. All rights reserved.
//

#include "Tag.hpp"
#include <string>
#include <iostream>
using namespace std;

//another comment to make it look like i use comments

Tag::Tag() {
    name = "";
    closingTagLine = 0;
    startingTagLine = 0;
}

Tag::Tag(string name) {
    this->name = name;
    closingTagLine = 0;
    startingTagLine = 0;
}


bool Tag::create(string tagLine, int tagLineNumber) {
    int spaceCell = 0;
    int lastSpaceCell = 0;
    
    bool isTagCreated = findTagName(tagLine, spaceCell, tagLineNumber);
    
    if(isTagCreated) {
        //make it check for variable amount of attributes
        bool isThereAnAttribute = true;
        
        while (isThereAnAttribute) {
            if(findAttributeName(tagLine, spaceCell, lastSpaceCell)) {
                findAttributeValue(tagLine, lastSpaceCell, spaceCell);
            }
            else {
                isThereAnAttribute = false;
            }
        }
    }
    
    return isTagCreated;
}

void Tag::findAttributeValue(string tagLine, int lastSpaceCell, int &spaceCell) {
    
    decltype(tagLine.size()) startCell = lastSpaceCell+4;
    
    decltype(tagLine.size()) endCell = tagLine.find(char(34), startCell);
    
    spaceCell = endCell+1;
    
    attributeValue.push_back(tagLine.substr(startCell, endCell-startCell));
}

bool Tag::findTagName(string tagLine, int &spaceCell, int tagLineNumber) {
    if(tagLine.substr(1, 1) != "/") {
        
        startingTagLine = tagLineNumber;
        
        decltype(tagLine.size()) endCell = 0;
        string::iterator it;
        bool foundStart = false;
        
        if(tagLine.find(char(32)) != string::npos) {
            endCell = tagLine.find(char(32));
            spaceCell = endCell;
        }
        else {
            endCell = tagLine.find(">");
        }
        
        for(char c : tagLine) {
            if(isalpha(c) && !foundStart) {
                it = find(tagLine.begin(), tagLine.end(), c);
                foundStart = true;
            }
            
        }
        
        name = tagLine.substr(it-tagLine.begin(), endCell-(it-tagLine.begin()));
        
        return true;
        
    }

    return false;
    
}

string Tag::findClosingTagName(string tagLine) {
    
    string tagName = "";
    
    if(tagLine.substr(1, 1) == "/") {
        
        decltype(tagLine.size()) endCell = 0;
        string::iterator it;
        
        endCell = tagLine.find(">");
        
        tagName = tagLine.substr(2, endCell-2);
        
        
    }

    return tagName;
    
}

bool Tag::findAttributeName(string tagLine, int spaceCell, int &lastSpaceCell) {
    
    decltype(tagLine.size()) startCell = spaceCell+1, endCell;
    
    if(tagLine.find('=') != string::npos) {
        //if(std::find(it, tagLine.end(), char(32)) != tagLine.end()) {
          //  endCell = tagLine.find(char(32));
        //}
        endCell = tagLine.find(' ', startCell);
        if(endCell != string::npos) {
            attributeName.push_back(tagLine.substr(startCell, endCell-startCell));
            lastSpaceCell = endCell;
            return true;
        }
        else {
            return false;
        }
    }
    else {
           return false;
       }
    
    
}

vector<string> Tag::getNested() {
    
    return nested;
    
}

bool Tag::isNested(const vector<Tag> &tags) {
    
    
    for(Tag tag : tags) {
        
        if(tag.startingTagLine < startingTagLine && tag.closingTagLine > closingTagLine) {
            nested.push_back(tag.getName());
        }
        
    }
    
    return true;
}

void Tag::setClosingTagLineNumber(int line) {
    closingTagLine = line;
}

void Tag::setName(string name) {
    this->name = name;
}

string Tag::getName() {
    return name;
}

int Tag::getClosingTagLine() {
    return closingTagLine;
}

int Tag::getStartingTagLine() {
    return startingTagLine;
}

vector<string> Tag::getAttributeName() {
    return attributeName;
}

vector<string> Tag::getAttributeValue() {
    return attributeValue;
}
