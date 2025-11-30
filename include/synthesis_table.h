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
    string cardinality;
};

struct ExternalRelation {
    string name;
    string stereotype;
    string sourceClass;
    string targetClass;
    string cardinality;
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
    vector<Package> packages;
    vector<SyntaxError> errors;

    // Ponteiros de Contexto (Onde o parser está agora?)
    Package* currentPackage = nullptr;
    Class* currentClass = nullptr;
    DataType* currentDataType = nullptr;
    Enumeration* currentEnum = nullptr;

public:
    SymbolTable();

    // Métodos de Controle de Contexto
    void setCurrentPackage(const string& name);
    
    // Métodos para Classes
    void addClass(const string& name, const string& stereotype, int line, int col);
    bool checkClassExists(const string& name);
    void addAttributeToCurrentClass(const string& name, const string& type, const string& meta);
    void addInternalRelationToCurrentClass(const string& target, const string& card, const string& name, const string& stereo);

    // Métodos para DataTypes
    void addDataType(const string& name, int line, int col);
    void addAttributeToCurrentDataType(const string& name, const string& type);

    // Métodos para Enums
    void addEnumeration(const string& name, int line, int col);
    void addEnumValueToCurrentEnum(const string& literal);

    // Métodos para Relações Externas e Generalizações
    void addExternalRelation(const string& name, const string& stereo, const string& source, const string& cardSource, const string& cardTarget, const string& target, int line, int col);
    
    // Adiciona um GenSet completo (usado quando a regra do parser fecha o bloco genset)
    void addGeneralization(const string& name, const string& restrictions, const string& parent, const vector<string>& children, int line, int col);

    // Getters
    const vector<Package>& getPackages() const { return packages; }
    const vector<SyntaxError>& getErrors() const { return errors; }

};

#endif // SYNTHESIS_TABLE_H
