#ifndef TRANSLATOR_H
#define TRANSLATOR_H

#include <QString>
namespace SHK_Translator
{
    class Translator
    {
    public:
        Translator();
        int Main(const QString *srcCode);
    private:
        struct KeyWord
        {
            const QString BEGIN = "Begin";
            const QString END = "End";
            const QString ANALYSIS = "Анализ";
            const QString SINTEZ = "Синтез";
        };
        struct Operator
        {
            const QChar COLON = ':';
            const QChar EQUAL = '=';
            const QChar MINUS = '-';
            const QChar PLUS = '+';
            const QChar MULTIPLY = '*';
            const QChar DIVIDE = '/';
            const QChar AND = '&';
            const QChar OR = '|';
            const QChar NOT = '^';
        };
        struct Function
        {
            const QString SIN = "sin";
            const QString COS = "cos";
            const QString LN = "ln";
            const QString TG = "tg";
            const QString CTG = "ctg";
        };

        int Equation(const QString *equation, int startPos, int &end);
        int RightValue(const QString *rvalue, int startPos, int &end);
        int Block(const QString *block, int startPos, int &end);
        int Part(const QString *part, int startPos, int &end);
        int Piece(const QString *piece, int startPos, int &end);
        int SmallPart(const QString *spart, int startPos, int &end);
        int SmallPiece(const QString *spiece, int startPos, int &end);
        int Variety(const QString *variety, int startPos, int &end);
    };
}

#endif // TRANSLATOR_H
