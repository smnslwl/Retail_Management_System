#include "category.h"
#include <QtSql>

/**
 * Constructs an category object.
**/
Category::Category(const QString &categoryName)
{
    m_id = 0;
    m_name = categoryName;
}

/**
 * Destructs the category object.
**/
Category::~Category()
{
}

/**
 * Returns the id of the category.
**/
int Category::id() const
{
    return m_id;
}

/**
 * Returns the name of the category.
**/
const QString &Category::name() const
{
    return m_name;
}

/**
 * Sets the id of the category to categoryId.
**/
void Category::setId(int categoryId)
{
    m_id = categoryId;
}

/**
 * Sets the name of the category to categoryName.
**/
void Category::setName(const QString &categoryName)
{
    m_name = categoryName;
}

/**
 * Saves the category to database.
 *
 * If this is a newly created category, an INSERT INTO is used.
 * An category with its id = 0 is assumed to be a new category.
 * Once created, the id of the category is set to the id of the newly inserted category.
 *
 * If this category already existed in the database, an UPDATE statement is used.
 * A category with a nonzero value of ID is assumed to already have existed in the database.
**/
void Category::save()
{
    QSqlQuery q;

    if (m_id == 0) {
        q.prepare(QLatin1String("INSERT INTO categories (name) VALUES(:name)"));
        q.bindValue(":name", m_name);
        q.exec();
        m_id = q.lastInsertId().toInt();
    } else {
        q.prepare(QLatin1String("UPDATE categories SET name = :name WHERE ID = :id"));
        q.bindValue(":name", m_name);
        q.bindValue(":id", m_id);
        q.exec();
    }
}

/**
 * Removes the category from the database.
 *
 * The category can only be deleted if it already existed in the database.
 * A category with a nonzero value of ID is assumed to already have existed in the database.
 * Once the category has been removed from the database, its ID is set to 0.
 * Calling remove() on a category with its ID = 0 will not do anything.
 * However, it is possible to setName() and save() to add a new category into the database.
**/
void Category::remove()
{
    QSqlQuery q;

    if (m_id != 0) {
        q.prepare(QLatin1String("DELETE FROM categories WHERE id = :id"));
        q.bindValue(":id", m_id);
        q.exec();
        m_id = 0;
    }
}

/**
 * Selects the category with the given id from the database.
 * Returns a Category object.
**/
Category Category::getById(int categoryId)
{
    Category category;

    QSqlQuery q;
    q.prepare(QLatin1String("SELECT * FROM categories WHERE id = :id"));
    q.bindValue(":id", categoryId);
    q.exec();

    if (q.next()) {
        category.setId(q.value("id").toInt());
        category.setName(q.value("name").toString());
    };

    return category;
}

/**
 * Selects all categories from the database.
 * Returns a QList of Category objects.
**/
QList<Category> Category::getAll()
{
    QList<Category> categories;

    QSqlQuery q(QLatin1String("SELECT * FROM categories"));
    q.exec();

    while (q.next()) {
        Category category;
        category.setId(q.value("id").toInt());
        category.setName(q.value("name").toString());
        categories.append(category);
    }

    return categories;
}
