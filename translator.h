#ifndef TRANSLATOR_H
#define TRANSLATOR_H

#include <QHash>
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
        int Main(QString *srcCode);
    private:
        struct
        {
            const QChar SPACE = ' ';
            const QChar EOS = '@';
            const QChar SEMICOLON = ';';
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

        int strCounter {1};
        bool endOfFile {false};
        QHash<QString, int> variables;

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
        int SmallPiece(
            QString *spiece,
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
        bool findTheWord(
            QString *main,
            QString check,
            int &counter
        );
        /**
         * @brief findTheOperator
         * @param[in] main
         * @param[in] check
         * @param[in] counter
         * @return
         * found or not
         */
        bool findTheOperator(
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
        bool isVariable(QString word);
    };
}

#endif // TRANSLATOR_H
