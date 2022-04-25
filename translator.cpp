#include "translator.h"
using namespace SHK_Translator;

Translator::Translator()
{
}

int Translator::Main(QString *srcCode)
{
    int currentPos {0}, endPos {0};
    QString word;
    bool beginExpected {false};
    bool endExpected {false};

    beginExpected = findTheWord(srcCode, KeyWord.BEGIN, currentPos);
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
    endExpected = findTheWord(srcCode, KeyWord.END, currentPos);
    if (!endExpected)
        return 4;
    return 0;
}

int Translator::Equation(
    QString *equation,
    int startPos,
    int &end,
    int &result
) {
    bool labelExpected {false};
    bool equalsExpected {false};

    int labelCurPos = startPos;
    labelExpected = findTheOperator(equation, Operator.COLON, labelCurPos);
    if (!labelExpected)
        return 1;
    for (int i = startPos; i < labelCurPos - 1; i++)
        if (!figure(equation->at(i)))
            return 2;

    int varCurPos = labelCurPos;
    equalsExpected = findTheOperator(equation, Operator.EQUAL, varCurPos);
    if (!equalsExpected)
        return 3;
    if (!letter(equation->at(labelCurPos + 1)))
        return 4;
    for (int i = labelCurPos + 1; i < varCurPos - 1; i++)
        if (!(letter(equation->at(i)) || figure(equation->at(i))))
            return 5;

    if (!RightValue(equation, varCurPos, end, result))
        return 6;

    return 0;
}

int Translator::RightValue(
    QString *rvalue,
    int startPos,
    int &end,
    int &result
) {
    result = 0;



}

int Translator::Block(QString *block, int startPos, int &end, int &result)
{

}

int Translator::Part(QString *part, int startPos, int &end, int &result)
{

}

int Translator::Piece(QString *piece, int startPos, int &end, int &result)
{

}

int Translator::SmallPart(
    QString *spart,
    int startPos,
    int &end,
    int &result
) {

}

int Translator::SmallPiece(
    QString *spiece,
    int startPos,
    int &end,
    int &result
) {

}

int Translator::Variety(
    QString *variety,
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

bool Translator::findTheWord(QString *main, QString check, int &counter)
{
    QString word;
    for (; counter < main->length(); counter++)
    {
        if (main->at(counter) == Spec.EOS)
        {
            main->remove(counter, 1);
            strCounter++;
            break;
        }
        if (main->at(counter) == Spec.SPACE)
            break;
        word.append(main->at(counter));
    }
    counter++;
    if (!word.compare(check, Qt::CaseSensitive))
        return true;
    if (counter >= main->length())
        endOfFile = true;
    return false;
}

bool Translator::findTheOperator(QString *main, QChar check, int &counter)
{
    while (counter < main->length())
    {
        if (main->at(counter) == Spec.EOS)
        {
            main->remove(counter, 1);
            strCounter++;
        }
        if (main->at(counter++) == check)
            return true;
    }
    if (counter >= main->length())
        endOfFile = true;
    return false;
}

