#include "parsingargs.h"
#include <list>

ParsingArgs::ParsingArgs() {}

ParsingArgs::~ParsingArgs() {}

bool ParsingArgs::AddArgType(const string shortName, const string longName, KeyType type)
{
    if (longName.empty() && shortName.empty())
    {
        return false;
    }
    Option tmp;
    tmp.longName = longName;
    tmp.shortName = shortName;
    tmp.type = type;
    args.push_back(tmp);
    return true;
}

ParsingArgs::KeyType ParsingArgs::GetKeyType(string& key)
{
    for (int i = 0; i < args.size(); ++i)
    {
        string shortName = "-";
        string longName = "--";
        shortName += args[i].shortName;
        longName += args[i].longName;
        if (0 == key.compare(shortName) ||
            (0 == key.compare(longName))
            )
        {
            RemoveKeyType(key);
            return args[i].type;
        }
    }
    return INVALID;
}

void ParsingArgs::RemoveKeyType(string& word)
{
    if (word.size() >= 2)
    {
        if (word[1] == '-')
        {
            word.erase(1, 1);
        }
        if (word[0] == '-')
        {
            word.erase(0, 1);
        }
    }
}

bool ParsingArgs::GetWord(string& Paras, string& word)
{
    size_t iNotSpacePos = Paras.find_first_not_of(' ', 0);
    if (iNotSpacePos == string::npos)
    {
        Paras.clear();
        word.clear();
        return true;
    }
    int length = Paras.size();
    list<char> specialChar;
    int islashPos = -1;
    for (int i = iNotSpacePos; i < length; i++)
    {
        char cur = Paras[i];
        bool bOk = false;
        switch (cur)
        {
        case ' ':
            if (specialChar.empty())
            {
                if (i != (length - 1))
                {
                    Paras = string(Paras, i + 1, length - i - 1);
                }
                else
                {
                    Paras.clear();
                }
                bOk = true;
            }
            else
            {
                if (specialChar.back() == '\\')
                {
                    specialChar.pop_back();
                }
                word.append(1, cur);
            }
            break;
        case '"':
            if (specialChar.empty())
            {
                specialChar.push_back(cur);
            }
            else if (specialChar.back() == cur)
            {
                specialChar.pop_back();
            }
            else if (specialChar.back() == '\\')
            {
                word.append(1, cur);
                specialChar.pop_back();
            }
            else
            {
                word.clear();
                return false;
            }
            break;
        case '\\':
            if (specialChar.empty())
            {
                specialChar.push_back(cur);
                islashPos = i;
            }
            else if (specialChar.back() == '"')
            {
                if (i < (length - 1))
                {
                    if ('"' == Paras[i + 1] || '\\' == Paras[i + 1])
                    {
                        specialChar.push_back(cur);
                    }
                    else
                    {
                        word.append(1, cur);
                    }
                }
                else
                {
                    word.clear();
                    return false;
                }
            }
            else if ('\\' == specialChar.back())
            {
                word.append(1, cur);
                specialChar.pop_back();
            }
            else
            {
                word.clear();
                return false;
            }
            break;
        default:
            word.append(1, Paras[i]);
            if (i == (length - 1))
            {
                bOk = true;
                Paras.clear();
            }
            break;
        }
        if (bOk)
        {
            return true;
        }
    }
    if (specialChar.empty())
    {
        Paras.clear();
        return true;
    }
    else
    {
        return false;
    }
}

bool ParsingArgs::IsDuplicateKey(const string& key, const map<string, vector<string> >& result)
{
    if (result.find(key) != result.end())
    {
        return true;
    }

    for (int i = 0; i < args.size(); ++i)
    {
        if ((key.compare(args[i].longName) == 0 && result.find(args[i].shortName) != result.end())
            || (key.compare(args[i].shortName) == 0 && result.find(args[i].longName) != result.end())
            )
        {
            return true;
        }
    }
    return false;
}

int ParsingArgs::Parse(const string& Paras, map<string, vector<string> >& result, string& errPos)
{
    string tmpString = Paras;
    KeyType keyType = INVALID;
    string sKey = "";
    bool bFindValue = false;
    while (!tmpString.empty())
    {
        string word = "";

        bool bRet = GetWord(tmpString, word);

        if (bRet == false)
        {
            errPos = tmpString;
            return -4;
        }
        else
        {
            KeyType tmpType = GetKeyType(word);
            if (IsDuplicateKey(word, result))
            {
                errPos = tmpString;
                return -5;
            }
            if (tmpType != INVALID)
            {
                if (tmpType == MUST && keyType == MUST && !bFindValue)
                {
                    errPos = tmpString;
                    return -3;
                }
                keyType = tmpType;
                vector<string> tmp;
                result[word] = tmp;
                sKey = word;
                bFindValue = false;
            }
            else
            {
                switch (keyType)
                {
                case MAYBE:
                case MUST:
                {
                    map<string, vector<string> >::iterator it = result.find(sKey);
                    if (it != result.end())
                    {
                        it->second.push_back(word);
                        bFindValue = true;
                    }
                    else
                    {
                        errPos = tmpString;
                        return -1;
                    }
                }
                break;
                case NO:
                    errPos = tmpString;
                    return -2;
                default:
                    errPos = tmpString;
                    return -1;
                }
            }
        }
    }
    return 0;
}
