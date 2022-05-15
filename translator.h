#ifndef TRANSLATOR_H
#define TRANSLATOR_H

#include "qglobal.h"
#include <QMap>
#include <QString>
namespace SHK_Translator
{
    class Translator
    {
    public:
        Translator();
        ~Translator();
        /**
         * @brief Main
         * Enter the program
         * @param[in] srcCode
         * Output from CodeEditor
         * @return
         * Errors
         */
        int Main(QString *srcCode);
        /**
         * @brief getter
         * @return
         * list of collected variables
         */
        QMap<QString, int> getVariables();
        /**
         * @brief getter
         * @return
         * strCounter
         */
        int getErrorString();
        QString getErrorMessage();
    private:
        struct
        {
            const QChar SPACE = ' ';
            const QChar EOS = '@';
            const QChar SEMICOLON = ';';
            const QChar EOS2 = '?';
        } Spec;
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
        enum func
        {
            SIN = 1,
            COS,
            TG,
            CTG,
            LN,
            NON
        };

        quint64 stringPos {1};
        int strCounter {1};
        bool endOfFile {false};
        bool operNotFounded{false};
        bool funcFounded{false};
        QString errorMsg{"Ошибка! %1. Строка %2, позиция %3."};
        QMap<QString, int> variables;

        /**
         * @brief Equation
         * @param[in] equation
         * @param[in] startPos
         * @param[out] end
         * @return
         * Errors
         */
        int Equation(
            QString *equation,
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
            QString *rvalue,
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
            QString *block,
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
            QString *part,
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
            QString *piece,
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
            QString *spart,
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
        int SmallPiece(QString *spiece,
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
            QString *variety,
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
        /**
         * @brief find word in main string
         * @param[in] main
         * @param[in] check
         * what to check (which word)
         * @param[in] counter
         * @return
         * found or not
         */
        QString findTheWord(QString *main, int &count);
        /**
         * @brief findTheOperator
         * @param[in] main
         * @param[in] check
         * @param[in] counter
         * @return
         * found or not
         */
        bool findTheSymbol(
            QString *main,
            QChar check,
            int &counter
        );
        /**
         * @brief findTheFunction
         * @param[in] main
         * @param[in] counter
         * @return
         * enum with founded function
         */
        func findTheFunction(QString *main, int &counter);
        /**
         * @brief isNumber
         * @param[in] word
         * @return
         * true if word is Number
         */
        bool isNumber(QString word);
        /**
         * @brief isVariable
         * @param[in] word
         * @return
         * true if word is Variable
         */
        bool isVariable(QString word, int &val);
        /**
         * @brief removes more than 1 space until sym expected
         * @param[in] main
         * @param[in] counter
         * @return
         * true if 1 space was deleted
         */
        bool skipSpaces(QString *main, int &counter);
        /**
         * @brief sym is separator?
         * @param[in] sym
         * @return
         * true if it is separator
         */
        bool isSeparator(QChar sym);
        /**
         * @brief it is necessary to go to the next sym (word)
         * @param[main] main
         * @param[in] counter
         * increases
         */
        bool skipSpaceAndLine(QString *main, int &counter);
        /**
         * @brief it is necessary to execute operation NOT
         * @param[in] number
         * @return
         * inversed number with sign
         */
        unsigned int inverseNumberWithSign(unsigned int number);
    };
}

#endif // TRANSLATOR_H
