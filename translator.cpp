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

    quint64 stringPosS = stringPos;
    QString beg = findTheWord(srcCode, currentPos);
    if (endOfFile || beg.isEmpty() || beg != KeyWord.BEGIN)
        return stringPosS; // begin isn't expected
    do {
        while (skipSpaceAndLine(srcCode, currentPos));
        if (endOfFile)
            return stringPos;
        int equationRes;
        int eqErr = Equation(srcCode, currentPos, endPos, equationRes);
        if (eqErr)
            return eqErr;
        // for next word
        while (skipSpaceAndLine(srcCode, endPos));
        if (endOfFile)
            return stringPos;
        currentPos = endPos;
        quint64 stringPosS = stringPos;
        QString w = findTheWord(srcCode, endPos);
        if (w == KeyWord.ANALYSIS || w == KeyWord.SINTEZ)
        {
            stringPos = stringPosS;
            break;
        }
        if (endOfFile)
            return stringPosS;
        // it's necessary to go to the next Expression
        stringPos = stringPosS;
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
    quint64 stringPosS = stringPos;
    QString label = findTheWord(equation, labelCurPos);
    if (endOfFile || label.isEmpty())
        return stringPosS;
    for (int i = 0; i < label.length(); i++)
        if (!figure(label[i]))
            return stringPosS;
    stringPosS = stringPos;
    if (!findTheSymbol(equation, Operator.COLON, labelCurPos))
        return stringPosS; // colon isn't expected

    int varCurPos = labelCurPos;
    while (skipSpaceAndLine(equation, varCurPos));
    if (endOfFile)
        return stringPos;
    stringPosS = stringPos;
    QString var = findTheWord(equation, varCurPos);
    if (endOfFile || var.isEmpty())
        return stringPosS;
    if (!letter(var[0]))
        return stringPosS;
    // ???
    for (int i = 1; i < var.length(); i++)
        if (!(letter(var[i]) || figure(var[i])))
            return stringPosS;
    //---

    stringPosS = stringPos;
    if(!findTheSymbol(equation, Operator.EQUAL, varCurPos))
        return stringPosS; // equal isn'n expected

    while (skipSpaceAndLine(equation, varCurPos));
    if (endOfFile)
        return stringPos;
    int rvErr = RightValue(equation, varCurPos, end, result);
    if (rvErr)
        return rvErr;

    auto founded = variables.find(var);
    if (founded == variables.end())
        variables.insert(var, result);
    else
        variables[var] = result;

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
            while (skipSpaceAndLine(rvalue, currentPos));
            if (endOfFile)
                return stringPos;
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
            while (skipSpaceAndLine(rvalue, currentPos));
            if (endOfFile)
                return stringPos;
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
        return stringPos;
    stringPos++;
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
    while (skipSpaceAndLine(block, currentPos));
    if (endOfFile)
        return stringPos;
    while (
           block->at(currentPos) == Operator.MULTIPLY ||
           block->at(currentPos) == Operator.DIVIDE
    ) {
        stringPos++;
        int pErr = Part(block, currentPos + 1, endPos, partRes);
        if (pErr)
            return pErr;
        if (block->at(currentPos) == Operator.DIVIDE)
        {
            if (partRes == 0)
                return stringPos; // divide zero
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
    while (skipSpaceAndLine(part, currentPos));
    if (endOfFile)
        return stringPos;
    while (
           part->at(currentPos) == Operator.AND ||
           part->at(currentPos) == Operator.OR
    ) {
        stringPos++;
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
        operNotFounded = true;
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
        while (skipSpaceAndLine(spart, currentPos));
        if (endOfFile)
            return stringPos;
        int prevWordPos = currentPos;
        if ((f = findTheFunction(spart, currentPos)) != NON)
            fStack.append(f);
        else
        {
            bool funcFounded{false};
            if (!fStack.isEmpty())
                funcFounded = true;
            int spErr2 = SmallPiece(
                spart,
                prevWordPos,
                endPos,
                spResult,
                funcFounded
            );
            if (spErr2)
                return spErr2;
            else
                break;
        }
    }
    result = spResult;
    while (!fStack.isEmpty())
    {
        double inRad = (double) result * 0.01745;
        switch (fStack.last())
        {
            case SIN:
                result = round(sin(inRad));
                break;
            case COS:
                result = round(cos(inRad));
                break;
            case TG:
                result = round(tan(inRad));
                break;
            case CTG:
                result = round(1 / tan(inRad));
            case LN:
                result = round(log(inRad));
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
    int &result,
    bool funcFounded
) {
    result = 0;
    QString word;
    int currentPos = startPos;
    quint64 stringPosS = stringPos;
    int strCounterS = strCounter;
    for (; currentPos < spiece->length();)
    {
        if (!(funcFounded || operNotFounded) && !word.isEmpty() && isSeparator(word.back()))
            break;
        if (spiece->at(currentPos) == Spec.SEMICOLON)
            break;
//        auto sym = spiece->at(currentPos);
//        if (
//            sym == Spec.SPACE ||
//            sym == Spec.EOS2 ||
//            sym == Operator.AND ||
//            sym == Operator.OR ||
//            sym == Operator.MULTIPLY ||
//            sym == Operator.PLUS ||
//            sym == Operator.DIVIDE
//        )
//            break;
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
        return stringPosS;
    end = currentPos;

    if (!word.isEmpty() && !isSeparator(word.back()) && isNumber(word))
        result = word.toInt();
    else if (!isVariable(word, result))
    {
        strCounter = strCounterS;
        return stringPosS;
    }
    return 0;
}

int Translator::Variety(
    QString *variety,
    int startPos,
    int &end,
    int &result
) {
    int currentPos = startPos;
    quint64 stringPosS = stringPos;
    QString key = findTheWord(variety, currentPos);
    if (!(key == KeyWord.ANALYSIS || key == KeyWord.SINTEZ))
        return stringPosS;
    while (skipSpaceAndLine(variety, currentPos));
    if (endOfFile)
        return stringPos;
    int varCounter{0};
    while (currentPos < variety->length())
    {
        int strCounterS = strCounter;
        quint64 stringPosS = stringPos;
        QString word = findTheWord(variety, currentPos);
        if (word == KeyWord.END && varCounter == variables.count())
        {
            while (skipSpaceAndLine(variety, currentPos));
            if (!endOfFile)
                return stringPos;
            end = currentPos;
            return 0;
        }
        int varVal{0};
        if (!isVariable(word, varVal))
        {
            strCounter = strCounterS;
            return stringPosS;
        }
        varCounter++;
    }
    return stringPos;
}

bool Translator::letter(QChar symbol)
{
    return (
        (symbol >= 65  && symbol <= 90)  ||
        (symbol >= 97  && symbol <= 122) ||
        (symbol >= 1040 && symbol <= 1103)
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
        stringPos++;
        while(skipSpaceAndLine(main, counter));
        return true;
    }
    return false;
}

Translator::func Translator::findTheFunction(QString *main, int &counter)
{
    quint64 stringPosS = stringPos;
    int strCounterS = strCounter;
    int counterS{-1};
    QString func;
    while (counter < main->length())
    {
        if (main->at(counter) == Spec.EOS2)
            counter++;
        if (main->at(counter) == Spec.EOS)
        {
            counterS = counter;
            main->replace(counter, 1, '?');
            strCounter++;
            stringPos = 1;
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
    if (counterS != -1)
        main->replace(counterS, 1, '@');
    strCounter = strCounterS;
    stringPos = stringPosS;
    return NON;
}

bool Translator::isNumber(QString word)
{
    if (word.isEmpty())
        return false;
    for (int i = 0; i < word.length(); i++)
        if (!figure(word[i]) && word[i] != Operator.MINUS)
            return false;
    return true;
}

bool Translator::isVariable(QString word, int &val)
{
    auto founded = variables.find(word);
    if (founded == variables.end())
        return false;
    val = founded.value();
    return true;
}

bool Translator::skipSpaces(QString *main, int &counter)
{
    quint64 stringPosS = stringPos;
    if (counter >= main->length())
    {
        stringPos = stringPosS;
        endOfFile = true;
        return false;
    }
    int next = counter + 1;
    while (next < main->length() &&
           main->at(counter) == Spec.SPACE &&
           main->at(next) == Spec.SPACE
    ) {
        next = ++counter + 1;
        stringPos++;
    }
    if (counter >= main->length())
    {
        stringPos = stringPosS;
        endOfFile = true;
        return false;
    }
    return true;
}

bool Translator::skipSpaceAndLine(QString *main, int &counter)
{
    if (!skipSpaces(main, counter))
        return false;
    for (int i = 0; i < 2; i++, counter++, stringPos++)
    {
        if (counter >= main->length())
        {
            endOfFile = true;
            return false;
        }
        if (main->at(counter) == Spec.EOS2)
        {
            stringPos = 0; // will be increased soon
            continue;
        }
        if (main->at(counter) == Spec.SPACE)
            continue;
        if (main->at(counter) == Spec.EOS)
        {
            quint64 stringPosS = stringPos;
            strCounter++;
            stringPos = 1;
            main->replace(counter, 1, '?');
            counter++;
            if (counter >= main->length())
            {
                stringPos = stringPosS;
                endOfFile = true;
                strCounter--;
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
        sym == Spec.EOS ||
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

QMap<QString, int> Translator::getVariables()
{
    return variables;
}

int Translator::getErrorString()
{
    return strCounter;
}
