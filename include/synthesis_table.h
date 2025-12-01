#ifndef SYNTHESIS_TABLE_H
#define SYNTHESIS_TABLE_H

#include <string>
#include <vector>
#include <iostream>

using namespace std;

struct Attribute {
    string name;
    string type;
    string metaattribute;
};

struct DataType {
    string name;
    vector<Attribute> attributes;
};

struct InternalRelation {
    string name;
    string stereotype;
    string otherClass;
    string cardinalityTarget;
    string cardinalitySource;
};

struct ExternalRelation {
    string name;
    string stereotype;
    string sourceClass;
    string targetClass;
    string cardinalityTarget;
    string cardinalitySource;
};

struct Generalization {
    string restrictions;
    string name;
    string parentClass;
    vector<string> childClasses;
};

struct Enumeration {
    string name;
    vector<string> literals;
};

struct Class {
    string name;
    string stereotype;
    vector<string> parentClasses;
    vector<Attribute> attributes;
    vector<InternalRelation> relations;
    int line;
    int column;
};

struct Package {
    string name;
    vector<Class> classes;
    vector<Generalization> generalizations;
    vector<Enumeration> enumerations;
    vector<DataType> dataTypes;
    vector<Enumeration> enums;
    vector<ExternalRelation> externalRelations;
};

struct SyntaxError {
    string message;
    string suggestion;
    int line;
    int col;
};

class SynthesisTable {
private:
    vector<string> imports;
    vector<Package> packages;
    vector<SyntaxError> errors;

    // Ponteiros de Contexto (Onde o parser está agora?)
    Package* currentPackage = nullptr;
    Class* currentClass = nullptr;
    DataType* currentDataType = nullptr;
    Enumeration* currentEnum = nullptr;
    Generalization* currentGeneralization = nullptr;

public:
    SynthesisTable();

    // Métodos de Controle de Contexto
    void setCurrentPackage(const string& name);

    // Métodos para Pacotes
    void addImport(const string& importName);
    
    // Métodos para Classes
    void addClass(const string& name, const string& stereotype, int line, int col);
    void addClassWithParents(const string& name, const string& stereotype, const vector<string>& parents, int line, int col);
    bool checkClassExists(const string& name);
    void addInternalRelationToCurrentClass(const string& target, const string& cardTarget, const string& cardSrc,const string& name, const string& stereo);

    void addAttribute(const string& name, const string& type, const string& meta);

    // Métodos para DataTypes
    void addDataType(const string& name, int line, int col);

    // Métodos para Enums
    void addEnumeration(const string& name, int line, int col);
    void addEnumValueToCurrentEnum(const string& literal);

    // Métodos para Relações Externas e Generalizações
    void addExternalRelation(const string& name, const string& stereo, const string& source, const string& cardSource, const string& cardTarget, const string& target, int line, int col);
    
    // Generalization: Para Inline (Cria tudo de uma vez)
    void addGeneralizationFull(const string& name, const string& restrictions, const string& parent, const vector<string>& children, int line, int col);

    // Generalization: Para Block (Passo a passo)
    void startGeneralizationBlock(const string& name, const string& restrictions, int line, int col);
    void setGeneralizationParent(const string& parentName);
    void addGeneralizationChildren(const vector<string>& children);

    // Getters
    const vector<Package>& getPackages() const { return packages; }
    const vector<string>& getImports() const { return imports; }
    const vector<SyntaxError>& getErrors() const { return errors; }

};

#endif // SYNTHESIS_TABLE_H
