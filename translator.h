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
        struct
        {
            const QString BEGIN = "Begin";
            const QString END = "End";
            const QString ANALYSIS = "Анализ";
            const QString SINTEZ = "Синтез";
        } KeyWord;
        struct
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
        } Operator;
        struct
        {
            const QString SIN = "sin";
            const QString COS = "cos";
            const QString LN = "ln";
            const QString TG = "tg";
            const QString CTG = "ctg";
        } Function;

        int strCounter {1};

        /**
         * @brief Equation
         * @param[in] equation
         * @param[in] startPos
         * @param[out] end
         * @return
         * Errors
         */
        int Equation(
            const QString *equation,
            int startPos,
            int &end,
            int &result
        );
        /**
         * @brief RightValue
         * @param[in] rvalue
         * @param[in] startPos
         * @param[out] end
         * @return
         * Errors
         */
        int RightValue(
            const QString *rvalue,
            int startPos,
            int &end,
            int &result
        );
        /**
         * @brief Block
         * @param[in] block
         * @param[in] startPos
         * @param[out] end
         * @return
         * Errors
         */
        int Block(
            const QString *block,
            int startPos,
            int &end,
            int &result
        );
        /**
         * @brief Part
         * @param[in] part
         * @param[in] startPos
         * @param[out] end
         * @return
         * Errors
         */
        int Part(
            const QString *part,
            int startPos,
            int &end,
            int &result
        );
        /**
         * @brief Piece
         * @param[in] piece
         * @param[in] startPos
         * @param[out] end
         * @return
         * Errors
         */
        int Piece(
            const QString *piece,
            int startPos,
            int &end,
            int &result
        );
        /**
         * @brief SmallPart
         * @param[in] spart
         * @param[in] startPos
         * @param[out] end
         * @return
         * Errors
         */
        int SmallPart(
            const QString *spart,
            int startPos,
            int &end,
            int &result
        );
        /**
         * @brief SmallPiece
         * @param[in] spiece
         * @param[in] startPos
         * @param[out] end
         * @return
         * Errors
         */
        int SmallPiece(
            const QString *spiece,
            int startPos,
            int &end,
            int &result
        );
        /**
         * @brief Variety
         * @param[in] variety
         * @param[in] startPos
         * @param[out] end
         * @return
         * Errors
         */
        int Variety(
            const QString *variety,
            int startPos,
            int &end,
            int &result
        );
        /**
         * @brief
         * @param[in] symbol
         * @return
         * is char a letter?
         */
        bool letter(QChar symbol);
        /**
         * @brief figure
         * @param[in] symbol
         * @return
         * is char a figure?
         */
        bool figure(QChar symbol);
    };
}

#endif // TRANSLATOR_H
