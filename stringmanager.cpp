#include "stringmanager.h"
using namespace Translator;

StringManager::StringManager()
{

}

bool StringManager::TrimFile(QString *input, QStringList *output)
{
    if (output)
    {
        return false;
    }
    QRegExp removeSpaces("[ ]+");
    *input = input->replace(removeSpaces, " ");
    output = new QStringList();
    int i {input->length() - 1}, j, stNum {1};
    while (input->at(i) == '\n')
    {
        input->remove(i--, 1);
    }
    QString raw;
    for (i = 0; i < input->length();)
    {
        if (input->at(i) == '\n')
        {
            input->remove(i, 1);
            QString number = QString::number(stNum);
            input->insert(i, "@" + number + "@");
            int size {2 + number.length()};
            for (j = 0; j < size; j++)
            {
                raw.append(input->at(i++));
            }
            output->append(raw);
            raw.clear();
            stNum++;
            continue;
        }
        raw.append(input->at(i++));
    }
    return true;
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