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
        checkModePattern(pkg);
        checkRoleMixinPattern(pkg);
    }
}

void SemanticAnalyzer::checkSubkindPattern(const Package& pkg) {
    map<string, vector<string>> parentToSubkindsMap;

    for (const auto& cls : pkg.classes) {
        if (cls.stereotype == "subkind") {

            // Validação: Subkind sem pai
            if (cls.parentClasses.empty()) {
                PatternIssue issue;
                issue.patternName = "Subkind Pattern";
                issue.status = "Hierarquia Inválida: Erro de Coersão"; 
                issue.issueDescription = "A classe '" + cls.name + "' é declarada como 'subkind' mas não especializa nenhuma outra classe. Todo subkind deve obrigatoriamente herdar de um Kind (ou outro tipo rígido).";
                
                // Contexto: Mostra quem é o órfão
                issue.participants["specifics"].push_back(cls.name);
                // "general" fica vazio ou nem é adicionado, indicando a falta do pai
                
                issues.push_back(issue);
                continue; // Pula para a próxima classe, não tem como mapear sem pai
            }
            
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
            
            PatternIssue issue;
            issue.patternName = "Subkind Pattern";
            issue.status = "Declaração Incompleta";
            issue.issueDescription = "A classe '" + parentName + "' possui especializações do tipo 'subkind', mas não há um genset que as agrupe. Considere adicionar um genset para melhor modelagem.";
            issue.participants = result.participants;
            issues.push_back(issue);

            continue;
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
            PatternIssue issue;
            issue.patternName = "Role Pattern";
            issue.status = "Declaração Incompleta";
            issue.issueDescription = "A classe '" + parentName + "' possui apenas um role ('" + roles[0] + "'). O padrão Role requer pelo menos dois roles";
            issue.participants = result.participants;
            
            issues.push_back(issue);
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
            if(gensetType.find("disjoint") != string::npos) {
                PatternIssue issue;
                issue.patternName = "Role Pattern";
                issue.status = "Violação de Anti-Rigidez";

                issue.issueDescription = "O genset '" + gensetName + "' que agrupa os roles da classe '" + parentName + "' possui a restrição 'disjoint', o que viola a característica de anti-rigidez dos roles.";
                issue.participants = result.participants;
                issues.push_back(issue);
            } else {
                result.status = "COMPLETE";
                result.description = "A classe '" + parentName + "' possui " + to_string(roles.size()) + 
                                     " roles agrupados corretamente no genset '" + gensetName + "' (sem disjoint).";
                
                results.push_back(result);
            }
        } else {
            PatternIssue issue;
            issue.patternName = "Role Pattern";
            issue.status = "Declaração Incompleta";
            issue.issueDescription = "A classe '" + parentName + "' possui " + to_string(roles.size()) + 
                                 " roles, mas não há um 'genset' declarado (obrigatório para o padrão Role).";
            issue.participants = result.participants;
            
            issues.push_back(issue);
        }
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

void SemanticAnalyzer::checkModePattern(const Package& pkg) {
    for (const auto& cls : pkg.classes) {
        // Filtra apenas classes Mode
        if (cls.stereotype != "mode") continue;

        PatternResult result;
        result.patternName = "Mode Pattern";
        result.participants["mode"].push_back(cls.name);

        bool hasCharacterization = false;
        vector<string> characterizedClasses;
        vector<string> dependencyClasses;

        // Analisa as relações internas
        for (const auto& rel : cls.relations) {
            if (rel.stereotype == "characterization") {
                hasCharacterization = true;
                characterizedClasses.push_back(rel.otherClass);
            } else if (rel.stereotype == "externalDependence") {
                dependencyClasses.push_back(rel.otherClass);
            } else {
                // Caso existam outras relações (ex: derivation), também guardamos como dependência/outros
                dependencyClasses.push_back(rel.otherClass);
            }
        }

        result.participants["characterized"] = characterizedClasses;
        result.participants["dependencies"] = dependencyClasses;

        // Validação
        if (hasCharacterization) {
            result.status = "COMPLETE";
            string desc = "O mode '" + cls.name + "' caracteriza a(s) classe(s): ";
            for (size_t i = 0; i < characterizedClasses.size(); ++i) {
                desc += (i > 0 ? ", " : "") + characterizedClasses[i];
            }
            desc += ".";
            
            if (!dependencyClasses.empty()) {
                desc += " Também possui " + to_string(dependencyClasses.size()) + " dependência(s) externa(s).";
            }
            result.description = desc;
        } else {
            result.status = "INCOMPLETE";
            result.description = "A classe mode '" + cls.name + "' foi declarada, mas não possui uma relação interna de @characterization (obrigatória para modes).";
        }

        results.push_back(result);
    }
}

void SemanticAnalyzer::checkRoleMixinPattern(const Package& pkg) {
    for (const auto& cls : pkg.classes) {
        // Filtra apenas classes RoleMixin
        if (cls.stereotype != "roleMixin") continue;

        string parentName = cls.name;
        
        // 2. Encontrar os filhos dessa RoleMixin
        vector<const Class*> childrenClasses;
        for (const auto& potentialChild : pkg.classes) {
            for (const auto& p : potentialChild.parentClasses) {
                if (p == parentName) {
                    childrenClasses.push_back(&potentialChild);
                    break;
                }
            }
        }

        PatternResult result;
        result.patternName = "Role Mixin Pattern";
        result.participants["general"].push_back(parentName);

        vector<string> specializedRoles;
        bool allChildrenAreRoles = true;

        for (const auto* child : childrenClasses) {
            specializedRoles.push_back(child->name);
            if (child->stereotype != "role") {
                allChildrenAreRoles = false;
            }
        }
        result.participants["specifics"] = specializedRoles;

        // Validação 1: Quantidade de filhos
        if (specializedRoles.size() < 2) {
            result.status = "INCOMPLETE";
            result.description = "O roleMixin '" + parentName + "' possui menos de 2 especializações (encontradas: " + to_string(childrenClasses.size()) + "). Mixins devem abstrair propriedades de múltiplos tipos.";
            results.push_back(result);
            continue;
        }

        // Validação 2: Tipos dos filhos
        if (!allChildrenAreRoles) {
            result.status = "INCOMPLETE";
            result.description = "O roleMixin '" + parentName + "' possui filhos que não são 'role'. Pelo padrão definido, todos devem ser roles.";
            results.push_back(result);
            continue;
        }

        // Validação 3: Genset com Disjoint
        bool foundGenset = false;
        bool isDisjoint = false;
        string gensetName = "";

        for (const auto& gen : pkg.generalizations) {
            if (gen.parentClass == parentName) {
                // Verifica se cobre os filhos encontrados
                bool coversChildren = false;
                for (const auto& childName : gen.childClasses) {
                    for (const auto& cName : specializedRoles) {
                        if (childName == cName) {
                            coversChildren = true;
                            break;
                        }
                    }
                }

                if (coversChildren) {
                    foundGenset = true;
                    gensetName = gen.name;
                    
                    // Verifica a restrição "disjoint" na string restrictions
                    // (Ex: "disjoint complete" ou só "disjoint")
                    if (gen.restrictions.find("disjoint") != string::npos) {
                        isDisjoint = true;
                    }
                    break; 
                }
            }
        }

        if (foundGenset) {
            if (isDisjoint) {
                result.status = "COMPLETE";
                result.description = "O roleMixin '" + parentName + "' é especializado por roles agrupados no genset disjoint '" + gensetName + "'.";
            } else {
                result.status = "INCOMPLETE";
                result.description = "O genset '" + gensetName + "' foi encontrado, mas falta a restrição 'disjoint' obrigatória para RoleMixin.";
            }
        } else {
            result.status = "INCOMPLETE";
            result.description = "O roleMixin '" + parentName + "' tem filhos, mas não há um 'genset' declarado.";
        }

        results.push_back(result);
    }
}