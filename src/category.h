#ifndef CATEGORY_H
#define CATEGORY_H

#include <QString>
#include <QList>

class Category
{
public:
    Category(const QString &categoryName = "");
    ~Category();
    int id() const;
    const QString &name() const;
    void setId(int categoryId);
    void setName(const QString &categoryName);
    void save();
    void remove();

private:
    int m_id;
    QString m_name;

public:
    static const int columnCount = 2;
    static Category getById(int categoryId);
    static QList<Category> getAll();
};

#endif // CATEGORY_H
