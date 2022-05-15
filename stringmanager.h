#ifndef STRINGMANAGER_H
#define STRINGMANAGER_H

#include <QObject>
#include <QString>
#include <QStringList>
#include <QMap>


namespace SHK_Translator
{
    class StringManager : public QObject
    {
        Q_OBJECT
    public:
        StringManager();
        ~StringManager();
        /**
         * @brief TrimThePathFromRight
         * @param[in] str
         * @return
         * trimmed str from right pos
         */
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
        /**
         * @brief
         * @param[in] result
         * @return
         * result from collected variables
         */
        QString getOutputResult(QMap<QString, int> result);
    signals:
        /**
         * @brief sendMsgToBar
         * It's for communication between Ui and This obj
         */
        void sendMsgToBar(QString);

    };
}
#endif // STRINGMANAGER_H
