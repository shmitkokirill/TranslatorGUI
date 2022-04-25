#include "translator.h"

#include <QList>
#include <math.h>
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
    for (int i = labelCurPos + 1; i < varCurPos - 2 /*!!*/; i++)
        if (!(letter(equation->at(i)) || figure(equation->at(i))))
            return 5;
    variables.insert(
        equation->mid(labelCurPos, varCurPos - 1 - labelCurPos),
        NULL
    );

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
    int endPos = startPos;
    result = 0;
    int currentPos = startPos;
    if (rvalue->at(currentPos) == Operator.MINUS)
    {
        currentPos++;
        int blockRes;
        if (Block(rvalue, currentPos, endPos, blockRes))
            return 1;
        result -= blockRes;
    }
    currentPos = endPos;
    do {
        int blockRes;
        if (Block(rvalue, currentPos, endPos, blockRes))
            return 2;
        result += blockRes;
    } while (rvalue->at(currentPos) == Operator.PLUS ||
             rvalue->at(currentPos) == Operator.MINUS
    );
    if (!(rvalue->at(currentPos) == Spec.SEMICOLON))
        return 2;
    end = endPos;
    return 0;
}

int Translator::Block(QString *block, int startPos, int &end, int &result)
{
    int blockRes {0};
    int partRes;
    int endPos;
    if (Part(block, startPos, endPos, partRes))
        return 1;
    blockRes = partRes;
    int currentPos = endPos;
    while (
           block->at(currentPos) == Operator.MULTIPLY ||
           block->at(currentPos) == Operator.DIVIDE
    ) {
        if (Part(block, currentPos + 1, endPos, partRes))
            return 2;
        if (block->at(currentPos) == Operator.DIVIDE)
        {
            blockRes /= partRes;
            currentPos = endPos;
            continue;
        }
        if (block->at(currentPos) == Operator.MULTIPLY)
        {
            blockRes *= partRes;
            currentPos = endPos;
            continue;
        }
    }
    end = endPos;
    return 0;
}

int Translator::Part(QString *part, int startPos, int &end, int &result)
{
    result = 0;
    int endPos;
    int pieceRes;
    if (Piece(part, startPos, endPos, pieceRes))
        return 1;
    result = pieceRes;
    int currentPos = endPos;
    while (
           part->at(currentPos) == Operator.AND ||
           part->at(currentPos) == Operator.OR
    ) {
        if (Piece(part, currentPos + 1, endPos, pieceRes))
            return 2;
        if (part->at(currentPos) == Operator.AND)
        {
            result &= pieceRes;
            currentPos = endPos;
            continue;
        }
        if (part->at(currentPos) == Operator.OR)
        {
            result |= pieceRes;
            currentPos = endPos;
            continue;
        }
    }
    end = endPos;
    return 0;
}

int Translator::Piece(QString *piece, int startPos, int &end, int &result)
{
    result = 0;
    int sPartRes;
    int endPos;
    bool notExpected {false};
    if (piece->at(startPos) == Operator.NOT)
        notExpected = true;
    if (SmallPart(piece, startPos, endPos, sPartRes))
        return 1;
    if (notExpected)
        result = !sPartRes;
    result = sPartRes;
    end = endPos;
    return 0;
}

int Translator::SmallPart(
    QString *spart,
    int startPos,
    int &end,
    int &result
) {
    int currentPos = startPos;
    result = 0;
    int spResult;
    int endPos;
    QList<func> fStack;
    func f;
    while (currentPos < spart->length())
    {
        int prevWordPos = currentPos;
        if ((f = findTheFunction(spart, currentPos)) != NON)
            fStack.append(f);
        else
            if (SmallPiece(spart, prevWordPos, endPos, spResult))
                return 1;
            else
                break;
    }
    result = spResult;
    while (!fStack.isEmpty())
    {
        switch (fStack.last())
        {
            case SIN:
                result = sin(result);
                break;
            case COS:
                result = cos(result);
                break;
            case TG:
                result = tan(result);
                break;
            case CTG:
                result = 1 / tan(result);
            case LN:
                result = log(result);
                break;
        }
        fStack.pop_back();
    }
    end = currentPos;
    return 0;
}

int Translator::SmallPiece(
    QString *spiece,
    int startPos,
    int &end,
    int &result
) {
    result = 0;
    QString word;
    int currentPos = startPos;
    for (; currentPos < spiece->length();)
    {
        if (spiece->at(currentPos) == Spec.SPACE)
            break;
        word.append(spiece->at(currentPos++));
    }
    currentPos++;
    end = currentPos;
    if (!(isVariable(word) || isNumber(word)))
        return 1;
    return 0;
}

int Translator::Variety(
    QString *variety,
    int startPos,
    int &end,
    int &result
) {
    int currentPos = startPos;
    if (!findTheWord(variety, KeyWord.ANALYSIS, currentPos))
    {
        currentPos = startPos;
        if (!findTheWord(variety, KeyWord.SINTEZ, currentPos))
            return 1;
    }
    while (currentPos < variety->length())
    {
        QString word;
        for (; currentPos < variety->length(); )
        {
            if (variety->at(currentPos) == Spec.EOS)
            {
                variety->remove(currentPos, 1);
                strCounter++;
                break;
            }
            if (variety->at(currentPos) == Spec.SPACE)
                break;
            word.append(variety->at(currentPos++));
        }
        currentPos++;
        if (word == KeyWord.END)
            return 0;
        if (!isVariable(word))
            return 2;
    }
    return 3;
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
    for (; counter < main->length(); )
    {
        if (main->at(counter) == Spec.EOS)
        {
            main->remove(counter, 1);
            strCounter++;
            break;
        }
        if (main->at(counter) == Spec.SPACE)
            break;
        word.append(main->at(counter++));
    }
//    counter++;
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

Translator::func Translator::findTheFunction(QString *main, int &counter)
{
    QString func;
    while (counter < main->length())
    {
        if (main->at(counter) == Spec.EOS)
        {
            main->remove(counter, 1);
            strCounter++;
        }
        if (main->at(counter) == Spec.SPACE)
            break;
        func.append(main->at(counter++));
    }
//    counter++;
    if (func == Function.COS)
        return COS;
    if (func == Function.SIN)
        return SIN;
    if (func == Function.TG)
        return TG;
    if (func == Function.CTG)
        return CTG;
    if (func == Function.LN)
        return LN;
    return NON;
}

bool Translator::isNumber(QString word)
{
    for (int i = 0; i < word.length(); i++)
        if (!figure(word[i]))
            return false;
    return true;
}

bool Translator::isVariable(QString word)
{
    return variables.contains(word);
}

