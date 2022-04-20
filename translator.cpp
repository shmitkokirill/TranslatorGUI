#include "translator.h"
using namespace SHK_Translator;

Translator::Translator()
{
}

int Translator::Main(const QString *srcCode)
{
    KeyWord key;
    int currentPos {0}, endPos {0};
    QString word;
    bool beginExpected {false};
    bool endExpected {false};
    while (!beginExpected && currentPos < srcCode->length())
    {
        for (; currentPos < srcCode->length(); currentPos++)
        {
            if (srcCode->at(currentPos) == '@')
            {
                strCounter++;
                break;
            }
            word.append(srcCode->at(currentPos));
        }
        currentPos++;
        if (!word.compare(key.BEGIN, Qt::CaseSensitive))
        {
            beginExpected = true;
            word.clear();
        }
    }
    if (!beginExpected)
        return 1;
    if (Equation(srcCode, currentPos, endPos))
        return 2;
    currentPos = endPos;
    endPos = 0;
    if (Variety(srcCode, currentPos, endPos))
        return 3;
    currentPos = endPos;
    endPos = 0;
    while (!endExpected && currentPos < srcCode->length())
    {
        for (; currentPos < srcCode->length(); currentPos++)
        {
            if (srcCode->at(currentPos) == '@')
            {
                strCounter++;
                break;
            }
            word.append(srcCode->at(currentPos));
        }
        currentPos++;
        if (!word.compare(key.END, Qt::CaseSensitive))
        {
            word.clear();
            endExpected = true;
        }
    }
    if (!endExpected)
        return 4;
    return 0;
}

int Translator::Equation(const QString *equation, int startPos, int &end)
{
    end = startPos;
    return 0;
}

int Translator::RightValue(const QString *rvalue, int startPos, int &end)
{

}

int Translator::Block(const QString *block, int startPos, int &end)
{

}

int Translator::Part(const QString *part, int startPos, int &end)
{

}

int Translator::Piece(const QString *piece, int startPos, int &end)
{

}

int Translator::SmallPart(const QString *spart, int startPos, int &end)
{

}

int Translator::SmallPiece(const QString *spiece, int startPos, int &end)
{

}

int Translator::Variety(const QString *variety, int startPos, int &end)
{
    end = startPos;
    return 0;
}
