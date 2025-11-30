#include "synthesis_table.h"

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

void SynthesisTable::addAttributeToCurrentClass(const string& name, const string& type, const string& meta) {
    if (!currentClass) return; // Erro silencioso ou logar erro de "Atributo fora de classe"

    Attribute attr;
    attr.name = name;
    attr.type = type;
    attr.metaattribute = meta;
    
    currentClass->attributes.push_back(attr);
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

void SynthesisTable::addAttributeToCurrentDataType(const string& name, const string& type) {
    if (!currentDataType) return;

    Attribute attr;
    attr.name = name;
    attr.type = type;
    attr.metaattribute = ""; 

    currentDataType->attributes.push_back(attr);
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
    if (!currentEnum) return;
    currentEnum->literals.push_back(literal);
}

void SynthesisTable::addGeneralization(const string& name, const string& restrictions, const string& parent, const vector<string>& children, int line, int col) {
    if (!currentPackage) return;

    Generalization gen;
    gen.name = name;
    gen.restrictions = restrictions;
    gen.parentClass = parent;
    gen.childClasses = children;

    currentPackage->generalizations.push_back(gen);
    
    // Validação Semântica Básica:
    // Verificar se a classe Pai e as Filhas existem neste pacote
    if (!checkClassExists(parent)) {
        errors.push_back({"Classe pai '" + parent + "' não encontrada no genset '" + name + "'.", "Defina a classe antes de usar.", line, col});
    }
}


void SynthesisTable::addInternalRelationToCurrentClass(const string& target, const string& card, const string& name, const string& stereo) {
    // Segurança: só adiciona se estivermos lendo uma classe
    if (!currentClass) return;

    InternalRelation rel;
    rel.name = name;           
    rel.stereotype = stereo;   
    rel.otherClass = target;   
    rel.cardinality = card;    

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

    // Formatação da cardinalidade para caber na string única
    // Exemplo de formato: "[0..1] -> [1..*]"
    // Se cardSource vier vazio, ajustamos a formatação
    string cSource = cardSource.empty() ? "?" : cardSource;
    string cTarget = cardTarget.empty() ? "?" : cardTarget;
    
    rel.cardinality = "[" + cSource + "] -> [" + cTarget + "]";

    currentPackage->externalRelations.push_back(rel);
}

