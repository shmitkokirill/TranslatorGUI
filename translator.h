#ifndef TRANSLATOR_H
#define TRANSLATOR_H

#include <QString>
namespace SHK_Translator
{
    class Translator
    {
    public:
        Translator();
        /**
         * @brief Main
         * Enter the program
         * @param[in] srcCode
         * Output from CodeEditor
         * @return
         * Errors
         */
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

        /**
         * @brief Equation
         * @param[in] equation
         * @param[in] startPos
         * @param[out] end
         * @return
         * Errors
         */
        int Equation(const QString *equation, int startPos, int &end);
        /**
         * @brief RightValue
         * @param[in] rvalue
         * @param[in] startPos
         * @param[out] end
         * @return
         * Errors
         */
        int RightValue(const QString *rvalue, int startPos, int &end);
        /**
         * @brief Block
         * @param[in] block
         * @param[in] startPos
         * @param[out] end
         * @return
         * Errors
         */
        int Block(const QString *block, int startPos, int &end);
        /**
         * @brief Part
         * @param[in] part
         * @param[in] startPos
         * @param[out] end
         * @return
         * Errors
         */
        int Part(const QString *part, int startPos, int &end);
        /**
         * @brief Piece
         * @param[in] piece
         * @param[in] startPos
         * @param[out] end
         * @return
         * Errors
         */
        int Piece(const QString *piece, int startPos, int &end);
        /**
         * @brief SmallPart
         * @param[in] spart
         * @param[in] startPos
         * @param[out] end
         * @return
         * Errors
         */
        int SmallPart(const QString *spart, int startPos, int &end);
        /**
         * @brief SmallPiece
         * @param[in] spiece
         * @param[in] startPos
         * @param[out] end
         * @return
         * Errors
         */
        int SmallPiece(const QString *spiece, int startPos, int &end);
        /**
         * @brief Variety
         * @param[in] variety
         * @param[in] startPos
         * @param[out] end
         * @return
         * Errors
         */
        int Variety(const QString *variety, int startPos, int &end);
    };
}

#endif // TRANSLATOR_H
