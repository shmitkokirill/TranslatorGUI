#ifndef STRINGMANAGER_H
#define STRINGMANAGER_H

#include <QString>
#include <QStringList>


namespace Translator
{
    class StringManager
    {
    public:
        StringManager();
        QString TrimThePathFromRight(const QString str);
        /**
         * @brief TrimFile
         * Deletes all spaces between words and
         * inserts numbers of string instead of
         * \\n
         * @param[in] input
         * Where will be deleted
         * @param[out] output
         * Result list of trimmed strings
         * @return
         * true if success
         */
        bool TrimFile(QString *input, QStringList *output);

    };
}
#endif // STRINGMANAGER_H
