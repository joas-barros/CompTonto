#include "semantic_analyser.h"
#include <iostream>

SemanticAnalyzer::SemanticAnalyzer(const SynthesisTable& table) : synthesisTable(table) {}

void SemanticAnalyzer::analyze() {
    const auto& packages = synthesisTable.getPackages();
    for (const auto& pkg : packages) {
        checkSubkindPattern(pkg);
    }
}

void SemanticAnalyzer::checkSubkindPattern(const Package& pkg) {
    map<string, vector<string>> parentToSubkindsMap;

    for (const auto& cls : pkg.classes) {
        if (cls.stereotype == "subkind") {
            for (const auto& parentName : cls.parentClasses) {
                parentToSubkindsMap[parentName].push_back(cls.name);
            }
        }
    }

    for (const auto& entry : parentToSubkindsMap) {
        string parentName = entry.first;
        const vector<string>& subkinds = entry.second;

        PatternResult result;
        result.patternName = "Subkind Pattern";
        result.participants["general"].push_back(parentName);
        result.participants["specifics"] = subkinds;

        if (subkinds.size() == 1) {
            result.status = "COMPLETE";
            result.description = "A classe '" + parentName + "' possui uma única especialização ('" + subkinds[0] + "'). O uso de genset não é obrigatório.";
            results.push_back(result);
            continue;
        }

        bool foundGenset = false;
        string gensetName = "";
        string gensetType = "";

        // Procura um genset que tenha esse Pai
        for (const auto& gen : pkg.generalizations) {
            if (gen.parentClass == parentName) {
                for (const auto& child : gen.childClasses) {
                    for (const auto& subkind : subkinds) {
                        if (child == subkind) {
                            foundGenset = true;
                            gensetType = gen.restrictions;
                            gensetName = gen.name;
                            break;
                        }
                    }
                    if (foundGenset) break;
                }
            }
            if (foundGenset) break;
        }

        if (foundGenset) {
            result.status = "COMPLETE";
            result.description = "A classe '" + parentName + "' possui especializações do tipo 'subkind' e um genset ('" + gensetType + "') de nome '" + gensetName + "' que as agrupa.";
        } else {
            result.status = "INCOMPLETE";
            result.description = "A classe '" + parentName + "' possui especializações do tipo 'subkind', mas não há um genset que as agrupe. Considere adicionar um genset para melhor modelagem.";
        }

        results.push_back(result);
    }
}