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
    QString beg;
    QList<QChar> check = {Spec.SPACE};
    while ((beg = findTheWord(srcCode, check, currentPos)).isEmpty())
        if (endOfFile)
            return 1;
    if (beg != KeyWord.BEGIN)
        return 2;
    do {
        int equationRes;
        if (Equation(srcCode, currentPos, endPos, equationRes))
            return 3;
        skipSpaceAndLine(srcCode, endPos);
        QList<QChar> check = {Spec.SPACE};
        currentPos = endPos;
        QString w = findTheWord(srcCode, check, endPos);
        if (w == KeyWord.ANALYSIS || w == KeyWord.SINTEZ)
            break;
    } while (true);
    if (endOfFile)
        return 4;
    endPos = 0;
    do {
        int varietyRes;
        if (Variety(srcCode, currentPos, endPos, varietyRes))
            return 5;
        else
            break;
    } while (true);
    return 0;
}

const QList<QPair<QString, int>> Translator::getVariables()
{
    return variables;
}

int Translator::Equation(
    QString *equation,
    int startPos,
    int &end,
    int &result
) {
    int labelCurPos = startPos;
    QList<QChar> checkLbl = {Operator.COLON};
    QString label;
    while((label = findTheWord(equation, checkLbl, labelCurPos)).isEmpty())
        if (endOfFile)
            return 1;
    for (int i = 0; i < label.length(); i++)
        if (!figure(label[i]))
            return 2;

    int varCurPos = labelCurPos;
    QList<QChar> checkVar = {Operator.EQUAL};
    QString var;
    while((var = findTheWord(equation, checkVar, varCurPos)).isEmpty())
        if (endOfFile)
            return 3;
    if (!letter(var[0]))
        return 4;
    for (int i = 1; i < var.length(); i++)
        if (!(letter(var[i]) || figure(var[i])))
            return 5;

    if (RightValue(equation, varCurPos, end, result))
        return 6;

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
    skipSpaceAndLine(rvalue, currentPos);
    int endPos = currentPos;
    if (rvalue->at(currentPos) == Operator.MINUS)
    {
        currentPos++;
        int blockRes;
        if (Block(rvalue, currentPos, endPos, blockRes))
            return 1;
        result -= blockRes;
    }
    else
    {
        int blockRes;
        if (Block(rvalue, currentPos, endPos, blockRes))
            return 2;
        result += blockRes;
    }
    currentPos = endPos;
    do {
        if (rvalue->at(currentPos) == Operator.MINUS)
        {
            currentPos++;
            skipSpaceAndLine(rvalue, currentPos);
            int blockRes;
            if (Block(rvalue, currentPos, endPos, blockRes))
                return 2;
            result -= blockRes;
            currentPos = endPos;
        }
        if (rvalue->at(currentPos) == Operator.PLUS)
        {
            currentPos++;
            skipSpaceAndLine(rvalue, currentPos);
            int blockRes;
            if (Block(rvalue, currentPos, endPos, blockRes))
                return 3;
            result += blockRes;
            currentPos = endPos;
        }
    } while (rvalue->at(currentPos) == Operator.PLUS ||
             rvalue->at(currentPos) == Operator.MINUS
    );
    if (!(rvalue->at(currentPos++) == Spec.SEMICOLON))
        return 2;
    end = currentPos;
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
    result = blockRes;
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
    {
        startPos++;
        notExpected = true;
    }
    if (SmallPart(piece, startPos, endPos, sPartRes))
        return 1;
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
    int spResult;
    int endPos;
    QList<func> fStack;
    func f;
    while (currentPos < spart->length())
    {
        int prevWordPos = currentPos;
        skipSpaceAndLine(spart, currentPos);
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
        skipSpaceAndLine(spiece, currentPos);
        if (spiece->at(currentPos) == Spec.SEMICOLON)
            break;
        auto sym = spiece->at(currentPos);
        if (
            sym == Spec.SPACE ||
            sym == Operator.AND ||
            sym == Operator.OR ||
            sym == Operator.MULTIPLY ||
            sym == Operator.PLUS ||
            sym == Operator.DIVIDE
        )
            break;
        word.append(spiece->at(currentPos++));
        if (isSeparator(spiece->at(currentPos)))
            break;
    }
    skipSpaceAndLine(spiece, currentPos);
    end = currentPos;

    if (isNumber(word))
        result = word.toInt();
    else
        return 1;
//    if (isVariable(word))
//        result = variables.value(word);
    return 0;
}

int Translator::Variety(
    QString *variety,
    int startPos,
    int &end,
    int &result
) {
    int currentPos = startPos;
    QList<QChar> check = {Spec.SPACE};
    QString key = findTheWord(variety, check, currentPos);
    if (!(key == KeyWord.ANALYSIS || key == KeyWord.SINTEZ))
        return 1;
    while (currentPos < variety->length())
    {
        QList<QChar> check = {Spec.SPACE};
        QString word = findTheWord(variety, check, currentPos);
        if (word == KeyWord.END)
        {
            end = currentPos;
            return 0;
        }
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

QString Translator::findTheWord(QString *main, QList<QChar> check, int &counter)
{
    int len = main->length();
    QString word {""};
    for (; counter < len; )
    {
        if (main->at(counter) == Spec.EOS)
        {
            counter++;
            strCounter++;
            return word.trimmed();
        }
        for (int i = 0; i < check.count(); i++)
            if (main->at(counter) == check[i])
            {
                counter++;
                return word.trimmed();
            }
        word.append(main->at(counter++));
    }
    if (counter == main->length())
        endOfFile = true;
    return QString();
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
            counter++;
            strCounter++;
            break;
        }
        if (isSeparator(main->at(counter)))
            break;
        func.append(main->at(counter++));
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

bool Translator::isVariable(QString word)
{
    for (auto item : variables)
        if (item.first == word)
            return true;
    return false;
}

void Translator::skipSpace(QString *main, int &counter)
{
//    for (int i = 0; i < 2; i++)
        if (main->at(counter) == Spec.SPACE)
            counter++;
}

void Translator::skipSpaceAndLine(QString *main, int &counter)
{
    for (int i = 0; i < 2; i++)
    {
        if (main->at(counter) == Spec.SPACE)
        {
            strCounter++;
            counter++;
        }
        if (main->at(counter) == Spec.EOS)
            counter++;
    }
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

