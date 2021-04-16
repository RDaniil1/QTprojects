#include "jsonData.h"

JsonData::JsonData()
{
    name = "";
    surname = "";
    age = 0;
}

QString JsonData::getName() const
{
    return name;
}

void JsonData::setName(const QString &value)
{
    name = value;
}

QString JsonData::getSurname() const
{
    return surname;
}

void JsonData::setSurname(const QString &value)
{
    surname = value;
}

int JsonData::getAge() const
{
    return age;
}

void JsonData::setAge(int value)
{
    age = value;
}
