#include "semantic_analyser.h"
#include <iostream>

SemanticAnalyzer::SemanticAnalyzer(const SynthesisTable& table) : synthesisTable(table) {}

void SemanticAnalyzer::analyze() {
    const auto& packages = synthesisTable.getPackages();
    for (const auto& pkg : packages) {
        checkSubkindPattern(pkg);
        checkRolePattern(pkg);
        checkPhasePattern(pkg);
        checkRelatorPattern(pkg);
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

void SemanticAnalyzer::checkRolePattern(const Package& pkg) {
    map<string, vector<string>> parentToRolesMap;

    // Agrupar roles pelos seus pais
    for (const auto& cls : pkg.classes) {
        if (cls.stereotype == "role") {
            for (const auto& parentName : cls.parentClasses) {
                // filtrar parentes que são roleMixins
                string parentStereo = "";
                for (const auto& p : pkg.classes) {
                    if (p.name == parentName) {
                        parentStereo = p.stereotype;
                        break;
                    }
                }
                if (parentStereo == "roleMixin") continue; 

                parentToRolesMap[parentName].push_back(cls.name);
            }
        }
    }

    //Analisar cada agrupamento
    for (const auto& entry : parentToRolesMap) {
        string parentName = entry.first;
        vector<string> roles = entry.second;

        PatternResult result;
        result.patternName = "Role Pattern";
        result.participants["general"].push_back(parentName);
        result.participants["roles"] = roles;

        if (roles.size() == 1) {
            result.status = "INCOMPLETE";
            result.description = "A classe '" + parentName + "' possui apenas um role ('" + roles[0] + "'). O padrão Role requer pelo menos dois roles";
            results.push_back(result);
            continue;
        }

        bool foundGenset = false;
        string gensetName = "";
        string gensetType = "";

        for (const auto& gen : pkg.generalizations) {
            if (gen.parentClass == parentName) {
                // Verifica interseção entre filhos do genset e roles encontrados
                for (const auto& child : gen.childClasses) {
                    for (const auto& r : roles) {
                        if (child == r) {
                            foundGenset = true;
                            gensetName = gen.name;
                            gensetType = gen.restrictions;
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
            result.description = "A classe '" + parentName + "' possui " + to_string(roles.size()) + 
                                 " roles agrupados corretamente no genset '" + gensetName + "' e retrição '" + gensetType + "'.";
        } else {
            // --- MUDANÇA 2: Sem Genset também é INCOMPLETE ---
            result.status = "INCOMPLETE";
            result.description = "A classe '" + parentName + "' possui " + to_string(roles.size()) + 
                                 " roles, mas não há um 'genset' declarado (obrigatório para o padrão Role).";
        }

        results.push_back(result);

    }
}


void SemanticAnalyzer::checkPhasePattern(const Package& pkg) {
    map<string, vector<string>> parentToPhasesMap;

    // Agrupar phases pelos seus pais
    for (const auto& cls : pkg.classes) {
        if (cls.stereotype == "phase") {
            for (const auto& parentName : cls.parentClasses) {
                parentToPhasesMap[parentName].push_back(cls.name);
            }
        }
    }

    // analisa cada agrupamento
    for (const auto& entry : parentToPhasesMap) {
        string parentName = entry.first;
        vector<string> phases = entry.second;
        PatternResult result;
        result.patternName = "Phase Pattern";
        result.participants["general"].push_back(parentName);
        result.participants["phases"] = phases;

        // não pode ter só uma fase
        if (phases.size() == 1) {
            result.status = "INCOMPLETE";
            result.description = "A classe '" + parentName + "' possui apenas uma phase ('" + phases[0] + "'). O padrão Phase requer pelo menos duas phases e um genset.";
            results.push_back(result);
            continue;
        }

        // --- Verificar existência do Genset (Obrigatório) ---
        bool foundGenset = false;
        string gensetName = "";
        string gensetType = "";

        for (const auto& gen : pkg.generalizations) {
            if (gen.parentClass == parentName) {
                // Verifica interseção entre filhos do genset e roles encontrados
                for (const auto& child : gen.childClasses) {
                    for (const auto& r : phases) {
                        if (child == r) {
                            foundGenset = true;
                            gensetName = gen.name;
                            gensetType = gen.restrictions;
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
            result.description = "A classe '" + parentName + "' possui " + to_string(phases.size()) + 
                                 " phases agrupados corretamente no genset '" + gensetName + "' e retrição '" + gensetType + "'.";
        } else {
            result.status = "INCOMPLETE";
            result.description = "A classe '" + parentName + "' possui " + to_string(phases.size()) + 
                                 " phases, mas não há um 'genset' declarado (obrigatório para o padrão Phase).";
        }
        results.push_back(result);
    }
}

void SemanticAnalyzer::checkRelatorPattern(const Package& pkg) {
    // Procurar as classes que são Relatores
    for(const auto& cls : pkg.classes) {
        if(cls.stereotype != "relator") continue;

        PatternResult result;
        result.patternName = "Relator Pattern";
        result.participants["relator"].push_back(cls.name);

        // Coletar os mediados (Targets das relações internas de mediação)
        vector<string> mediatedClasses;

        for (const auto& rel : cls.relations) {
            if (rel.stereotype == "mediation") {
                mediatedClasses.push_back(rel.otherClass);
            }
        }

        result.participants["mediated"] = mediatedClasses;

        if (mediatedClasses.size() < 2) {
            result.status = "INCOMPLETE";
            result.description = "O relator '" + cls.name + "' possui menos de 2 mediações (encontradas: " + to_string(mediatedClasses.size()) + ").";
            results.push_back(result);
            continue;
        }

        // Verificar relações materiais entre os pares de mediados
        bool allPairsConnected = true;
        string missingConnectionMsg = "";

        // Loop duplo para verificar cada par único
        for (size_t i = 0; i < mediatedClasses.size(); ++i) {
            for (size_t j = i + 1; j < mediatedClasses.size(); ++j) {
                string classA = mediatedClasses[i];
                string classB = mediatedClasses[j];
                
                bool linkFound = false;

                // Procura nas relações EXTERNAS do pacote
                for (const auto& extRel : pkg.externalRelations) {
                    if (extRel.stereotype == "material") {
                        // Verifica conexão bidirecional (A->B ou B->A)
                        bool matchDirect = (extRel.sourceClass == classA && extRel.targetClass == classB);
                        bool matchInverse = (extRel.sourceClass == classB && extRel.targetClass == classA);
                        
                        if (matchDirect || matchInverse) {
                            linkFound = true;
                            break;
                        }
                    }
                }

                if (!linkFound) {
                    allPairsConnected = false;
                    missingConnectionMsg = "Falta relação @material entre '" + classA + "' e '" + classB + "'.";
                    break; // Pode parar no primeiro erro ou continuar para listar todos
                }
            }
            if (!allPairsConnected) break;
        }

        if (allPairsConnected) {
            result.status = "COMPLETE";
            result.description = "O relator '" + cls.name + "' media as classes " + 
                                 to_string(mediatedClasses.size()) + " classes, e todas possuem relações materiais entre si.";
        } else {
            result.status = "INCOMPLETE";
            result.description = "O relator '" + cls.name + "' está incompleto. " + missingConnectionMsg;
        }

        results.push_back(result);
    }
}