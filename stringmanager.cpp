#include "stringmanager.h"
using namespace SHK_Translator;

StringManager::StringManager()
{
}
StringManager::~StringManager() = default;

int StringManager::getErrPos()
{
    return errPos;
}

bool StringManager::TrimFile(QString *input, QString &output)
{
    if (input->isEmpty())
    {
        emit sendMsgToBar("TrimFile: Нет входных данных");
        return false;
    }

    int strCount = 1;
    for (int i = 0; i < input->length(); i++)
    {
        QChar sym = input->at(i);
        if (sym == '\n')
        {
            strCount++;
            continue;
        }
        if (
            !(sym >= 65 && sym <= 90) &&
            !(sym >= 97  && sym <= 122) &&
            !(sym >= 1040 && sym <= 1103) &&
            !(
                 sym == '=' ||
                 sym == '+' ||
                 sym == '-' ||
                 sym == ';' ||
                 sym == '/' ||
                 sym == '*' ||
                 sym == ':' ||
                 sym == '&' ||
                 sym == '|' ||
                 sym == '^'
            ) &&
            !(sym >= 48 && sym <= 57) &&
            sym != ' '
        ) {
            errPos = i;
            QString sN = QString::number(strCount);
            output = errorMsg.arg("Встречен неизвестный терминал", sN);
            return false;
        }
    }
    int i {input->length() - 1};
    while (input->at(i) == '\n')
    {
        input->remove(i--, 1);
    }
    input->append('\n');
    QString raw;
    for (i = 0; i < input->length();)
    {
        if (input->at(i) == '\n')
        {
            input->replace(i, 1, '@');
            raw.append('@');
            i++;
            raw.clear();
            continue;
        }
        raw.append(input->at(i++));
    }
    return true;
}

QString StringManager::getOutputResult(QMap<QString, int> result)
{
    QString res;
    for(auto key : result.keys())
    {
        res.append(key + " = " + QString::number(result[key]) + "\n");
    }
    return res;
}

QString StringManager::TrimThePathFromRight(const QString str)
{
    QString result = str;
    int i;
    int len = str.length();
    for (i = str.length() - 1; i >= 0; i--)
    {
        if (str[i] == '/')
        {
            break;
        }
    }
    return result.remove(i + 1, len - i);
}
