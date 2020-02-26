#ifndef SBIF_PARSING_H
#define SBIF_PARSING_H

#include <map>
#include <vector>
#include <string>

using namespace std;

//according to https://blog.csdn.net/fly542/article/details/19486185

class ParsingArgs
{
public:
    ParsingArgs();
    ~ParsingArgs();

    enum KeyType { INVALID = -1, NO, MAYBE, MUST };

    bool AddArgType(const string shortName, const string longName = NULL, KeyType type = NO);

    int Parse(const string& paras, map<string, vector<string> >& result, string& errPos);

private:

    KeyType GetKeyType(string& key);

    void RemoveKeyType(string& paras);

    bool GetWord(string& Paras, string& word);

    bool IsDuplicateKey(const string& key, const map<string, vector<string> >& result);

    struct Option
    {
        string longName;
        string shortName;
        KeyType type;
    };

    vector<Option> args;
};

#endif  //SBIF_PARSING_H

