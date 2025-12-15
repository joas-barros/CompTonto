#ifndef SEMANTIC_ANALYZER_H
#define SEMANTIC_ANALYZER_H

#include "synthesis_table.h"
#include <vector>
#include <string>
#include <map>

using namespace std;

struct PatternResult {
    string patternName;   
    string status;        
    string description;   
    map<string, vector<string>> participants;
};

struct PatternIssue {
    string patternName;
    string status;
    string issueDescription;
    map<string, vector<string>> participants;
};

class SemanticAnalyzer {
private:
    const SynthesisTable& synthesisTable;
    vector<PatternResult> results;
    vector<PatternIssue> issues;

    void checkSubkindPattern(const Package& pkg);
    void checkRolePattern(const Package& pkg);
    void checkPhasePattern(const Package& pkg);
    void checkRelatorPattern(const Package& pkg);
    void checkModePattern(const Package& pkg);
    void checkRoleMixinPattern(const Package& pkg);
    
public:
    SemanticAnalyzer(const SynthesisTable& table);
    void analyze();

    // getter para os resultados
    const vector<PatternResult>& getPatterns() const { return results; }
};

#endif