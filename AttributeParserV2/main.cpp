//
//  main.cpp
//  AttributeParserV2
//
//  Created by Eric on 2/25/16.
//  Copyright © 2016 Eric. All rights reserved.
//

#include <cmath>
#include <cctype>
#include <cstdio>
#include <sstream>
#include <vector>
#include <iostream>
#include <algorithm>
#include <iostream>
#include "Tag.hpp"
//a comment cause i didnt put any anywhere else

using namespace std;

void createTags(const vector<string> &tagLines, vector<Tag> &tags) {
    int tagLineNumber = 0;
    
    vector<string> names;
    
    for(string str : tagLines) {
        Tag tag;
        
        if(tag.create(str, tagLineNumber)) {
            tags.push_back(tag);
            names.push_back(tag.getName());
        }
        else {
            
            for(Tag &name : tags) {
                
                if(name.getName() == tag.findClosingTagName(str)) {
                    name.setClosingTagLineNumber(tagLineNumber);
                }
                
            }
            
        }
        
        ++tagLineNumber;
    }
    
    for(Tag &tempTag : tags) {
        
        tempTag.isNested(tags);
        
    }
    
}

string getWantedTagName(string query) {
    
    string::size_type endCell = query.find('~');
    
    return query.substr(0, endCell);

}

string getWantedAttributeName(string query) {
    
    string::size_type startCell = query.find('~')+1;
    
    return query.substr(startCell);
    
}

Tag getWantedTag(const vector<Tag> &tags, string wantedTagName) {
    
    for(Tag tempTag : tags) {
        if(tempTag.getName() == wantedTagName) {
            return tempTag;
        }
    }
    
    return Tag();
    
}

bool tagHasAttribute(const vector<string> &attributeNames, string wantedAttributeName) {
    
    for(string attributeName : attributeNames) {
        
        if(attributeName == wantedAttributeName) {
            return true;
        }
        
    }
    
    return false;
    
}

string getTagAttributeValue(Tag tag, string wantedAttributeName) {
    
    int location = 0;
    
    for(int i = 0; i < tag.getAttributeName().size(); i++) {
        if(tag.getAttributeName()[i] == wantedAttributeName) {
            location = i;
        }
    }
    
    return tag.getAttributeValue()[location];
    
}

string processQuery(string query, const vector<Tag> &tags, bool isNested) {
    
    string wantedTagName = getWantedTagName(query);
    string wantedAttributeName = getWantedAttributeName(query);
    Tag tag = getWantedTag(tags, wantedTagName);
    
    if(tagHasAttribute(tag.getAttributeName(), wantedAttributeName)) {
        
        string attributeValue = getTagAttributeValue(tag, wantedAttributeName);
        
        vector<string> nested = tag.getNested();
        
        if((nested.size() == 0 && !isNested) || (nested.size() > 0 && isNested)) {
            return attributeValue;
        }
        else {
            return "Not Found!";
        }
        
    }
    else {
        return "Not Found!";
    }
    
    
    
}

vector<int> getPeriods(string query) {
    
    vector<int> periods;
    
    for(int i = 0; i < query.size(); i++) {
        if(query[i] == '.') {
            periods.push_back(i);
        }
    }
    
    return periods;
    
}

vector<string> getNestedTagNames(string query, const vector<int> &periods) {
    
    vector<string> names;
    
    int tildaLoc = query.find('~');
    
    names.push_back(query.substr(0, periods[0]));
    
    for(int i = 0; i < periods.size()-1; i++) {
        
        names.push_back(query.substr(periods[i]+1, (periods[i+1])-(periods[i]+1)));
        
    }
    
    names.push_back(query.substr(periods.back()+1, tildaLoc-(periods.back()+1)));
    
    return names;
    
    
}

int getTagLocation(string name, vector<Tag> &tags) {
    
    int location = 0;
    
    for(int i = 0; i < tags.size(); i++) {
        if(tags[i].getName() == name) {
            location = i;
        }
    }
    
    return location;
    
}

string processNestedQuery(string query, vector<Tag> &tags) {
    
    vector<int> periods = getPeriods(query);
    
    vector<string> names = getNestedTagNames(query, periods);
    
    string nestedQuery = query.substr(periods.back()+1);
    
    string output = processQuery(nestedQuery, tags, true);
    
    bool areNested = true;
    
    bool isFirstTag = true;
    
    int i = 0;
    int tagLoc;
    Tag tag;
    vector<string> nestedIn;
    vector<string> firstNestIn;
    
    for(int j = 0; j < names.size(); j++) {
        
        i = (names.size()-1)-j;
        
        tagLoc = getTagLocation(names[i], tags);
        
        if(isFirstTag) {
            firstNestIn = tags[tagLoc].getNested();
            isFirstTag = false;
        }
        nestedIn = tags[tagLoc].getNested();
        
        if(firstNestIn.size() == names.size()-1) {
        
            for(string str : nestedIn) {
                if(!(find(names.begin(), names.end(), str) != names.end())) {
                    areNested = false;
                }
            }
        
        }
        else {
            areNested = false;
        }
        
    }
    
    if(areNested) {
        return output;
    }
    
    return "Not Found!";
    
    
}

void processQueries(const vector<string> &queryLines, vector<Tag> &tags) {
    
    for(string query : queryLines) {
    
        if(query.find('.') != string::npos) {
            cout << processNestedQuery(query, tags) << endl;
        }
        else {
            cout << processQuery(query, tags, false) << endl;
        }
        
    }
    
}

void getNumberOfLinesAndQueries(int &numberOfTagLines, int &numberOfQueryLines) {
    string lineNumbers;
    
    getline(cin, lineNumbers);
    
    istringstream iss(lineNumbers);
    
    
    iss >> numberOfTagLines >> numberOfQueryLines;
    
}

void getLines(int numberOfLines, int numberOfQueries, vector<string> &tagLines, vector<string> &queryLines) {
    
    getNumberOfLinesAndQueries(numberOfLines, numberOfQueries);
    
    string input;
    
    for(int i = 0; i < numberOfLines; i++) {
        getline(cin, input);
        tagLines.push_back(input);
    }
    
    for(int i = 0; i < numberOfQueries; i++) {
        getline(cin, input);
        queryLines.push_back(input);
    }
    
}

void getInput(vector<Tag> &tags) {
    
    int numberOfTagLines, numberOfQueryLines;
    
    vector<string> tagLines, queryLines;
    
    getLines(numberOfTagLines, numberOfQueryLines, tagLines, queryLines);
    
    createTags(tagLines, tags);
    processQueries(queryLines, tags);
    
}

int main(int argc, const char * argv[]) {

    vector<Tag> tags;
    
    getInput(tags);

    return 0;
}
