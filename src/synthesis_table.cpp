#include "synthesis_table.h"

SynthesisTable::SynthesisTable() {
    // Inicializa os ponteiros de contexto como nullptr
    currentPackage = nullptr;
    currentClass = nullptr;
    currentDataType = nullptr;
    currentEnum = nullptr;
    currentGeneralization = nullptr;
}

void SynthesisTable::addImport(const string& importName) {
    imports.push_back(importName);
}

void SynthesisTable::setCurrentPackage(const string& name) {
    // Procura se já existe
    for (auto& pkg : packages) {
        if (pkg.name == name) {
            currentPackage = &pkg;
            // Reseta contextos internos ao mudar de pacote
            currentClass = nullptr;
            currentDataType = nullptr;
            currentEnum = nullptr;
            return;
        }
    }
    // Cria novo
    Package pkg;
    pkg.name = name;
    packages.push_back(pkg);
    currentPackage = &packages.back();
    
    currentClass = nullptr;
    currentDataType = nullptr;
    currentEnum = nullptr;
}

bool SynthesisTable::checkClassExists(const string& name) {
    if (!currentPackage) return false;
    for (const auto& cls : currentPackage->classes) {
        if (cls.name == name) return true;
    }
    return false;
}

void SynthesisTable::addClass(const string& name, const string& stereotype, int line, int col) {
    if (!currentPackage) return; // Sem pacote atual

    if (checkClassExists(name)) {
        errors.push_back({
            "Classe '" + name + "' redefinida no mesmo pacote.",
            "Remova a duplicata ou renomeie.",
            line, col
        });
        return;
    }

    Class cls;
    cls.name = name;
    cls.stereotype = stereotype;
    cls.line = line;
    cls.column = col;

    currentPackage->classes.push_back(cls);
    currentClass = &currentPackage->classes.back();
    currentDataType = nullptr;
    currentEnum = nullptr;
}

void SynthesisTable::addClassWithParents(const string& name, const string& stereotype, const vector<string>& parents, int line, int col) {
    addClass(name, stereotype, line, col);

    if (currentClass) {

        currentClass->parentClasses = parents; 
    }
}

void SynthesisTable::addAttribute(const string& name, const string& type, const string& meta, const int quantity) {
    // Cenário 1: Estamos dentro de uma CLASSE
    if (currentClass != nullptr) {
        Attribute attr;
        attr.name = name;
        attr.type = type;
        attr.metaattribute = meta;
        attr.quantity = quantity;
        currentClass->attributes.push_back(attr);
        return;
    }

    // Cenário 2: Estamos dentro de um DATATYPE
    if (currentDataType != nullptr) {
        Attribute attr;
        attr.name = name;
        attr.type = type;
        attr.metaattribute = meta;
        currentDataType->attributes.push_back(attr);
        return;
    }

    // Cenário 3: Atributo solto (Erro)
    errors.push_back({
        "Atributo '" + name + "' declarado fora de uma Classe ou DataType.",
        "Mova este atributo para dentro de uma estrutura.", 
        0, 0 
    });
}

void SynthesisTable::addDataType(const string& name, int line, int col) {
    if (!currentPackage) return;

    // Verifica duplicidade (opcional, mas recomendado)
    for(const auto& dt : currentPackage->dataTypes) {
        if(dt.name == name) {
            errors.push_back({"DataType '" + name + "' já existe.", "Renomeie.", line, col});
            return;
        }
    }

    DataType dt;
    dt.name = name;
    currentPackage->dataTypes.push_back(dt);

    currentDataType = &currentPackage->dataTypes.back();
    currentClass = nullptr;
    currentEnum = nullptr;
}

void SynthesisTable::addEnumeration(const string& name, int line, int col) {
    if (!currentPackage) return;

    Enumeration en;
    en.name = name;
    currentPackage->enumerations.push_back(en);

    currentEnum = &currentPackage->enumerations.back();
    currentClass = nullptr;
    currentDataType = nullptr;
}

void SynthesisTable::addEnumValueToCurrentEnum(const string& literal) {
    if (currentEnum != nullptr) {
        currentEnum->literals.push_back(literal);
    } else {
        // Opcional: Logar erro se tentar adicionar literal fora de enum
        errors.push_back({"Literal de Enum '" + literal + "' perdido (contexto inválido).", "Verifique a sintaxe.", 0, 0});
    }
}


void SynthesisTable::addInternalRelationToCurrentClass(const string& target, const string& cardTarget, const string& cardSrc,const string& name, const string& stereo) {
    // Segurança: só adiciona se estivermos lendo uma classe
    if (!currentClass) return;

    InternalRelation rel;
    rel.name = name;           
    rel.stereotype = stereo;   
    rel.otherClass = target;   
    rel.cardinalityTarget = cardTarget;    
    rel.cardinalitySource = cardSrc;

    currentClass->relations.push_back(rel);
}


void SynthesisTable::addExternalRelation(const string& name, const string& stereo, 
                                         const string& source, const string& cardSource, 
                                         const string& cardTarget, const string& target, 
                                         int line, int col) {
    
    if (!currentPackage) return;

    ExternalRelation rel;
    rel.name = name;           
    rel.stereotype = stereo;   
    rel.sourceClass = source;  
    rel.targetClass = target;  

    string cSource = cardSource.empty() ? "?" : cardSource;
    string cTarget = cardTarget.empty() ? "?" : cardTarget;
    
    rel.cardinalitySource = cSource;  
    rel.cardinalityTarget = cTarget; 

    currentPackage->externalRelations.push_back(rel);
}

void SynthesisTable::addGeneralizationFull(const string& name, const string& restrictions, const string& parent, const vector<string>& children, int line, int col) {
    if (!currentPackage) return;
    
    Generalization gen;
    gen.name = name;
    gen.restrictions = restrictions;
    gen.parentClass = parent;
    gen.childClasses = children;

    currentPackage->generalizations.push_back(gen);
}

void SynthesisTable::startGeneralizationBlock(const string& name, const string& restrictions, int line, int col) {
    if (!currentPackage) return;

    Generalization gen;
    gen.name = name;
    gen.restrictions = restrictions;
    
    currentPackage->generalizations.push_back(gen);
    
    currentGeneralization = &currentPackage->generalizations.back();
    
    currentClass = nullptr;
    currentDataType = nullptr;
    currentEnum = nullptr;
}

void SynthesisTable::setGeneralizationParent(const string& parentName) {
    if (currentGeneralization) {
        currentGeneralization->parentClass = parentName;
    }
}

void SynthesisTable::addGeneralizationChildren(const vector<string>& children) {
    if (currentGeneralization) {
        currentGeneralization->childClasses = children;
    }
}


size_t SynthesisTable::getNumberOfClasses() const {
    size_t count = 0;
    for (const auto& pkg : packages) {
        count += pkg.classes.size();
    }
    return count;
}

size_t SynthesisTable::getNumberOfDataTypes() const {
    size_t count = 0;
    for (const auto& pkg : packages) {
        count += pkg.dataTypes.size();
    }
    return count;
}

size_t SynthesisTable::getNumberOfEnumerations() const {
    size_t count = 0;
    for (const auto& pkg : packages) {
        count += pkg.enumerations.size();
    }
    return count;
}

size_t SynthesisTable::getNumberOfGeneralizations() const {
    size_t count = 0;
    for (const auto& pkg : packages) {
        count += pkg.generalizations.size();
    }
    return count;
}

size_t SynthesisTable::getNumberOfExternalRelations() const {
    size_t count = 0;
    for (const auto& pkg : packages) {
        count += pkg.externalRelations.size();
    }
    return count;
}

size_t SynthesisTable::getNumberOfInternalRelations() const {
    size_t count = 0;
    for (const auto& pkg : packages) {
        for (const auto& cls : pkg.classes) {
            count += cls.relations.size();
        }
    }
    return count;
}

void SynthesisTable::validateStructure() {
    for (const auto& pkg : packages) {
        
        // --- 1. Validar Heranças e Relações das Classes ---
        for (const auto& cls : pkg.classes) {
            
            // A. Validar Pais (Herança)
            for (const string& parentName : cls.parentClasses) {
                bool parentFound = false;
                for (const auto& target : pkg.classes) {
                    if (target.name == parentName) {
                        parentFound = true;
                        break;
                    }
                }
                if (!parentFound) {
                    errors.push_back({
                        "Classe pai '" + parentName + "' não encontrada para a classe '" + cls.name + "'.",
                        "Verifique se o nome está correto ou declare a classe.",
                        cls.line, cls.column
                    });
                }
            }

            // B. Validar Relações Internas (Target)
            for (const auto& rel : cls.relations) {
                bool targetFound = false;
                for (const auto& target : pkg.classes) {
                    if (target.name == rel.otherClass) { // rel.otherClass é o destino
                        targetFound = true;
                        break;
                    }
                }
                if (!targetFound) {
                    errors.push_back({
                        "A relação '" + rel.name + "' aponta para uma classe inexistente: '" + rel.otherClass + "'.",
                        "Declare a classe destino.",
                        cls.line, cls.column 
                    });
                }
            }
        }

        // --- 2. Validar Generalizações (Gensets) ---
        for (const auto& gen : pkg.generalizations) {
            // Validar se o Pai existe
            bool parentFound = false;
            for (const auto& target : pkg.classes) {
                if (target.name == gen.parentClass) {
                    parentFound = true;
                    break;
                }
            }
            if (!parentFound) {
                errors.push_back({
                    "Genset '" + gen.name + "' refere-se a uma classe pai inexistente: '" + gen.parentClass + "'.",
                    "Corrija o nome da classe general.",
                    0, 0 
                });
            }

            // Validar se os Filhos existem
            for (const string& childName : gen.childClasses) {
                bool childFound = false;
                for (const auto& target : pkg.classes) {
                    if (target.name == childName) {
                        childFound = true;
                        break;
                    }
                }
                if (!childFound) {
                    errors.push_back({
                        "Genset '" + gen.name + "' refere-se a uma classe filha inexistente: '" + childName + "'.",
                        "Corrija o nome da classe specific.",
                        0, 0
                    });
                }
            }
        }

        // --- 3. Validar Relações Externas ---
        for (const auto& rel : pkg.externalRelations) {
            
            // A. Validar Classe de Origem (Source)
            bool sourceFound = false;
            for (const auto& cls : pkg.classes) {
                if (cls.name == rel.sourceClass) { 
                    sourceFound = true; 
                    break; 
                }
            }
            if (!sourceFound) {
                errors.push_back({
                    "Relação Externa '" + rel.name + "' tem origem inválida: '" + rel.sourceClass + "'.",
                    "A classe de origem deve ser declarada neste pacote.",
                    0, 0 
                });
            }

            // B. Validar Classe de Destino (Target)
            bool targetFound = false;
            for (const auto& cls : pkg.classes) {
                if (cls.name == rel.targetClass) { 
                    targetFound = true; 
                    break; 
                }
            }
            if (!targetFound) {
                errors.push_back({
                    "Relação Externa '" + rel.name + "' tem destino inválido: '" + rel.targetClass + "'.",
                    "A classe de destino deve ser declarada neste pacote.",
                    0, 0
                });
            }
        }
    }
}

