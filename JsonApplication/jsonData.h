#ifndef JSONDATA_H
#define JSONDATA_H
#include <QString>

class JsonData
{
public:
    JsonData();
    QString getName() const;
    void setName(const QString &value);

    QString getSurname() const;
    void setSurname(const QString &value);

    int getAge() const;
    void setAge(int value);

private:
    QString name;
    QString surname;
    int age;
};

#endif // JSONDATA_H
