#include "stringmanager.h"
using namespace SHK_Translator;

StringManager::StringManager()
{
}
StringManager::~StringManager() = default;

bool StringManager::TrimFile(QString *input, QStringList *output)
{
    if (input->isEmpty())
    {
        emit sendMsgToBar("TrimFile: Нет входных данных");
        return false;
    }
    if (!output)
    {
        emit sendMsgToBar("TrimFile: Не инициализирована выходная переменная");
        return false;
    }

    QRegExp removeSpecial("[@?]+");
    *input = input->replace(removeSpecial, "~");
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
            output->append(raw);
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
