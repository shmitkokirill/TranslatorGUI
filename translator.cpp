#include "translator.h"
#include <QList>
#include <cmath>
#include <math.h>
using namespace SHK_Translator;

Translator::Translator()
{
}

int Translator::Main(QString *srcCode)
{
    int currentPos {0}, endPos {0};
    while (skipSpaceAndLine(srcCode, currentPos));
    if (endOfFile)
        return stringPos;
    QString beg = findTheWord(srcCode, currentPos);
    if (endOfFile || beg.isEmpty() || beg != KeyWord.BEGIN)
        return 1; // begin isn't expected
    do {
        while (skipSpaceAndLine(srcCode, currentPos));
        if (endOfFile)
            return stringPos;
        int equationRes;
        int eqErr = Equation(srcCode, currentPos, endPos, equationRes);
        if (eqErr)
            return eqErr;
//        int curPosS = endPos;
//        if (!skipSpaceAndLine(srcCode, curPosS))
//            return endPos;
//        endPos = curPosS;
        currentPos = endPos;
        QString w = findTheWord(srcCode, endPos);
        if (w == KeyWord.ANALYSIS || w == KeyWord.SINTEZ)
            break;
    } while (true);
    if (endOfFile)
        return stringPos;
    endPos = 0;
    do {
        while (skipSpaceAndLine(srcCode, currentPos));
        if (endOfFile)
            return stringPos;
        int varietyRes;
        int varietyErr = Variety(srcCode, currentPos, endPos, varietyRes);
        if (varietyErr)
            return varietyErr;
        else
            break;
    } while (true);
    return 0;
}

int Translator::Equation(
    QString *equation,
    int startPos,
    int &end,
    int &result
) {
    int labelCurPos = startPos;
    QString label = findTheWord(equation, labelCurPos);
    if (endOfFile || label.isEmpty())
        return labelCurPos - startPos;
    for (int i = 0; i < label.length(); i++)
        if (!figure(label[i]))
            return labelCurPos - label.length() - startPos;
    if (!findTheSymbol(equation, Operator.COLON, labelCurPos))
        return labelCurPos - startPos; // colon isn't expected

    int varCurPos = labelCurPos;
    while (skipSpaceAndLine(equation, varCurPos));
    if (endOfFile)
        return stringPos;
    QString var = findTheWord(equation, varCurPos);
    if (endOfFile || var.isEmpty())
        return varCurPos - startPos;
    if (!letter(var[0]))
        return varCurPos - var.length();
    // ???
    for (int i = 1; i < var.length(); i++)
        if (!(letter(var[i]) || figure(var[i])))
            return varCurPos - var.length() - startPos;
    //---

    if(!findTheSymbol(equation, Operator.EQUAL, varCurPos))
        return varCurPos - startPos; // equal isn'n expected

    while (skipSpaceAndLine(equation, varCurPos));
    if (endOfFile)
        return varCurPos - startPos;
    int rvErr = RightValue(equation, varCurPos, end, result);
    if (rvErr)
        return rvErr;

    variables.append(QPair<QString, int>(var, result));

    return 0;
}

int Translator::RightValue(
    QString *rvalue,
    int startPos,
    int &end,
    int &result
) {
    result = 0;
    int currentPos = startPos;
    int endPos = currentPos;
    if (rvalue->at(currentPos) == Operator.MINUS)
    {
        currentPos++;
        stringPos++;
        while (skipSpaceAndLine(rvalue, currentPos));
        if (endOfFile)
            return stringPos;
        int blockRes;
        int bErr = Block(rvalue, currentPos, endPos, blockRes);
        if (bErr)
            return bErr;
        result -= blockRes;
    }
    else
    {
        //skip?
        int blockRes;
        int bErr = Block(rvalue, currentPos, endPos, blockRes);
        if (bErr)
            return bErr;
        result += blockRes;
    }
    currentPos = endPos;
    do {
        if (rvalue->at(currentPos) == Operator.MINUS)
        {
            currentPos++;
            stringPos++;
            int curPosS = currentPos;
            while (skipSpaceAndLine(rvalue, curPosS));
            if (endOfFile)
                return stringPos;
            currentPos = curPosS;
            int blockRes;
            int bErr = Block(rvalue, currentPos, endPos, blockRes);
            if (bErr)
                return bErr;
            result -= blockRes;
            currentPos = endPos;
        }
        if (rvalue->at(currentPos) == Operator.PLUS)
        {
            currentPos++;
            stringPos++;
            int curPosS = currentPos;
            while (skipSpaceAndLine(rvalue, curPosS));
            if (endOfFile)
                return stringPos;
            currentPos = curPosS;
            int blockRes;
            int bErr = Block(rvalue, currentPos, endPos, blockRes);
            if (bErr)
                return bErr;
            result += blockRes;
            currentPos = endPos;
        }
    } while (rvalue->at(currentPos) == Operator.PLUS ||
             rvalue->at(currentPos) == Operator.MINUS
    );
    if (!(rvalue->at(currentPos++) == Spec.SEMICOLON))
        return ++stringPos; // OK
    end = currentPos;
    return 0;
}

int Translator::Block(QString *block, int startPos, int &end, int &result)
{
    int blockRes {0};
    int partRes;
    int endPos;
    int pErr = Part(block, startPos, endPos, partRes);
    if (pErr)
        return pErr;
    blockRes = partRes;
    int currentPos = endPos;
    while (
           block->at(currentPos) == Operator.MULTIPLY ||
           block->at(currentPos) == Operator.DIVIDE
    ) {
        int pErr = Part(block, currentPos + 1, endPos, partRes);
        if (pErr)
            return pErr;
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
    result = blockRes;
    end = endPos;
    return 0;
}

int Translator::Part(QString *part, int startPos, int &end, int &result)
{
    int endPos;
    int pieceRes;
    int currentPos = startPos;
    while (skipSpaceAndLine(part, currentPos));
    if (endOfFile)
        return stringPos;
    int pieceErr = Piece(part, currentPos, endPos, pieceRes);
    if (pieceErr)
        return pieceErr;
    result = pieceRes;
    currentPos = endPos;
    while (
           part->at(currentPos) == Operator.AND ||
           part->at(currentPos) == Operator.OR
    ) {
        int pieceErr2 = Piece(part, currentPos + 1, endPos, pieceRes);
        if (pieceErr2)
            return pieceErr2;
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
    int currentPos = startPos;
    while (skipSpaceAndLine(piece, currentPos));
    if (endOfFile)
        return stringPos;
    if (piece->at(currentPos) == Operator.NOT)
    {
        currentPos++;
        stringPos++;
        notExpected = true;
    }
    while (skipSpaceAndLine(piece, currentPos));
    if (endOfFile)
        return stringPos;
    int spErr = SmallPart(piece, currentPos, endPos, sPartRes);
    if (spErr)
        return spErr;
    if (notExpected)
        sPartRes = inverseNumberWithSign((unsigned int) sPartRes);
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
    int spResult {0};
    int endPos;
    QList<func> fStack;
    func f;
    while (currentPos < spart->length())
    {
        int prevWordPos = currentPos;
        while (skipSpaceAndLine(spart, currentPos));
        if (endOfFile)
            return stringPos;
        if ((f = findTheFunction(spart, currentPos)) != NON)
            fStack.append(f);
        else
        {
            int spErr2 = SmallPiece(spart, prevWordPos, endPos, spResult);
            if (spErr2)
                return spErr2;
            else
                break;
        }
    }
    result = spResult;
    while (!fStack.isEmpty())
    {
        result = round((double) result * 0.01745);
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
    end = endPos;
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
        if (spiece->at(currentPos) == Spec.SEMICOLON)
            break;
        auto sym = spiece->at(currentPos);
        if (
            sym == Spec.SPACE ||
            sym == Spec.EOS2 ||
            sym == Operator.AND ||
            sym == Operator.OR ||
            sym == Operator.MULTIPLY ||
            sym == Operator.PLUS ||
            sym == Operator.DIVIDE
        )
            break;
        word.append(spiece->at(currentPos++));
        stringPos++;
        if (isSeparator(spiece->at(currentPos)))
            break;
        while (skipSpaceAndLine(spiece, currentPos));
        if (endOfFile)
            return stringPos;
    }

    while (skipSpaceAndLine(spiece, currentPos));
    if (endOfFile)
        return stringPos;
    end = currentPos;

    if (isNumber(word))
        result = word.toInt();
    else if (!isVariable(word, result))
        return stringPos;
    return 0;
}

int Translator::Variety(
    QString *variety,
    int startPos,
    int &end,
    int &result
) {
    int currentPos = startPos;
    QString key = findTheWord(variety, currentPos);
    if (!(key == KeyWord.ANALYSIS || key == KeyWord.SINTEZ))
        return stringPos;
    while (currentPos < variety->length())
    {
        QString word = findTheWord(variety, currentPos);
        if (word == KeyWord.END)
        {
            end = currentPos;
            return 0;
        }
        int varVal{0};
        if (!isVariable(word, varVal))
            return stringPos;
    }
    return currentPos;
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
    return (symbol >= 48 && symbol <= 57);
}

QString Translator::findTheWord(QString *main, int &counter)
{
    QString word {""};
    for (; counter < main->length();)
    {
        if (skipSpaceAndLine(main, counter))
            return word.trimmed();
        if (endOfFile)
            return word.trimmed();
        if (!letter(main->at(counter)) && !figure(main->at(counter)))
            return word.trimmed();
        word.append(main->at(counter));
        counter++;
        stringPos++;
    }
    endOfFile = true;
    return QString();
}

bool Translator::findTheSymbol(QString *main, QChar check, int &counter)
{
    while(skipSpaceAndLine(main, counter));
    if (main->at(counter) == check)
    {
        counter++;
        while(skipSpaceAndLine(main, counter));
        return true;
    }
    return false;
}

Translator::func Translator::findTheFunction(QString *main, int &counter)
{
    QString func;
    while (counter < main->length())
    {
        if (main->at(counter) == Spec.EOS2)
        {
            counter++;
            stringPos++;
        }
        if (main->at(counter) == Spec.EOS)
        {
            main->replace(counter, 1, '?');
            strCounter++;
            stringPos = 0;
            break;
        }
        if (isSeparator(main->at(counter)))
            break;
        func.append(main->at(counter++));
        stringPos++;
    }
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
        if (!figure(word[i]) && word[i] != Operator.MINUS)
            return false;
    return true;
}

bool Translator::isVariable(QString word, int &val)
{
    for (auto item : variables)
        if (item.first == word)
        {
            val = item.second;
            return true;
        }
    return false;
}

void Translator::skipSpace(QString *main, int &counter)
{
    if (main->at(counter) == Spec.SPACE)
        counter++;
}

bool Translator::skipSpaceAndLine(QString *main, int &counter)
{
    for (int i = 0; i < 2; i++, counter++, stringPos++)
    {
        if (counter >= main->length())
        {
            endOfFile = true;
            return false;
        }
        if (main->at(counter) == Spec.EOS2)
            continue;
        if (main->at(counter) == Spec.SPACE)
            continue;
        if (main->at(counter) == Spec.EOS)
        {
            strCounter++;
            stringPos = 0;
            main->replace(counter, 1, '?');
            counter++;
            stringPos++;
            if (counter >= main->length())
            {
                endOfFile = true;
                return false;
            }
            return true;
        }
        return (i == 0) ? false : true;
    }
    return true;
}

unsigned int Translator::inverseNumberWithSign(unsigned int number)
{
    // количество цифр числа a в системе счисления 2
    unsigned int shift = (unsigned int)std::log2(number) + 1u;
    return (shift == 32) ? ~number : (~number & ((1u << shift) - 1u));
}

bool Translator::isSeparator(QChar sym)
{
    if (
        sym == Spec.SPACE ||
        sym == Spec.EOS2 ||
        sym == Operator.AND ||
        sym == Operator.MINUS ||
        sym == Operator.OR ||
        sym == Operator.MULTIPLY ||
        sym == Operator.PLUS ||
        sym == Operator.DIVIDE
    )
        return true;
    return false;
}

const QList<QPair<QString, int>> Translator::getVariables()
{
    return variables;
}

int Translator::getErrorString()
{
    return strCounter;
}
