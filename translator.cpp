#include "translator.h"
using namespace SHK_Translator;

Translator::Translator()
{
}

int Translator::Main(const QString *srcCode)
{
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
        if (!word.compare(KeyWord.BEGIN, Qt::CaseSensitive))
        {
            beginExpected = true;
            word.clear();
        }
    }
    if (!beginExpected)
        return 1;
    int equationRes;
    if (Equation(srcCode, currentPos, endPos, equationRes))
        return 2;
    currentPos = endPos;
    endPos = 0;
    int varietyRes;
    if (Variety(srcCode, currentPos, endPos, varietyRes))
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
        if (!word.compare(KeyWord.END, Qt::CaseSensitive))
        {
            word.clear();
            endExpected = true;
        }
    }
    if (!endExpected)
        return 4;
    return 0;
}

int Translator::Equation(
    const QString *equation,
    int startPos,
    int &end,
    int &result
) {
    bool labelExpected {false};
    bool equalsExpected {false};

    int labelCurPos = startPos;
    while (labelCurPos < equation->length() && !labelExpected)
        if (equation->at(labelCurPos++) == Operator.COLON)
            labelExpected = true;
    if (!labelExpected)
        return 1;
    for (int i = startPos; i < labelCurPos - 1; i++)
        if (!figure(equation->at(i)))
            return 2;

    int varCurPos = labelCurPos;
    while (varCurPos < equation->length() && !equalsExpected)
        if (equation->at(varCurPos++) == Operator.EQUAL)
            equalsExpected = true;
    if (!equalsExpected)
        return 3;
    if (!letter(equation->at(labelCurPos + 1)))
        return 4;
    for (int i = labelCurPos + 1; i < varCurPos - 1; i++)
        if (!letter(equation->at(i)) || !figure(equation->at(i)))
            return 5;

    if (!RightValue(equation, varCurPos, end, result))
        return 6;

    return 0;
}

int Translator::RightValue(
    const QString *rvalue,
    int startPos,
    int &end,
    int &result
) {
    result = 0;


}

int Translator::Block(const QString *block, int startPos, int &end, int &result)
{

}

int Translator::Part(const QString *part, int startPos, int &end, int &result)
{

}

int Translator::Piece(const QString *piece, int startPos, int &end, int &result)
{

}

int Translator::SmallPart(
    const QString *spart,
    int startPos,
    int &end,
    int &result
) {

}

int Translator::SmallPiece(
    const QString *spiece,
    int startPos,
    int &end,
    int &result
) {

}

int Translator::Variety(
    const QString *variety,
    int startPos,
    int &end,
    int &result
) {
    end = startPos;
    return 0;
}

bool Translator::letter(QChar symbol)
{
    return (
        (symbol >= 65  && symbol <= 90)  ||
        (symbol >= 97  && symbol <= 122) ||
        (symbol >= 192 && symbol <= 255)
    );
}

bool Translator::figure(QChar symbol)
{
    return (symbol >= 48 && symbol <= 71);
}
