#include "product.h"
#include <QtSql>

/**
 * Constructs an product object.
**/
Product::Product(const QString &productName, int productCategory, int productPrice, int productStock)
{
    m_id = 0;
    m_name = productName;
    m_category = productCategory;
    m_price = productPrice;
    m_stock = productStock;
}

/**
 * Destructs the product object.
**/
Product::~Product()
{
}

/**
 * Returns the ID of the product.
**/
int Product::id() const
{
    return m_id;
}

/**
 * Returns the name of the product.
**/
const QString &Product::name() const
{
    return m_name;
}

/**
 * Returns the category id of the product.
**/
int Product::category() const
{
    return m_category;
}

/**
 * Returns the price of the product.
**/
int Product::price() const
{
    return m_price;
}

/**
 * Returns the number of available stock of the product.
**/
int Product::stock() const
{
    return m_stock;
}

/**
 * Sets the ID of the product to productId.
**/
void Product::setId(int productId)
{
    m_id = productId;
}

/**
 * Sets the name of the product to productName.
**/
void Product::setName(const QString &productName)
{
    m_name = productName;
}

/**
 * Sets the category of the product to productCategory.
**/
void Product::setCategory(int productCategory)
{
    m_category = productCategory;
}

/**
 * Sets the price of the product to productPrice.
**/
void Product::setPrice(int productPrice)
{
    m_price = productPrice;
}

/**
 * Sets the units available in stock of the product to productStock.
**/
void Product::setStock(int productStock)
{
    m_stock = productStock;
}

/**
 * Saves the product to database.
 *
 * If this is a newly created product, an INSERT INTO is used.
 * An product with its id = 0 is assumed to be a new product.
 * Once created, the id of the product is set to the id of the newly inserted product.
 *
 * If this product already existed in the database, an UPDATE statement is used.
 * A product with a nonzero value of id is assumed to already have existed in the database.
**/
void Product::save()
{
    QSqlQuery q;

    if (m_id == 0) {
        q.prepare(QLatin1String("INSERT INTO products (name, category, price, stock) VALUES(:name, :category, :price, :stock)"));
        q.bindValue(":name", m_name);
        q.bindValue(":category", m_category);
        q.bindValue(":price", m_price);
        q.bindValue(":stock", m_stock);
        q.exec();
        m_id = q.lastInsertId().toInt();
    } else {
        q.prepare(QLatin1String("UPDATE products SET name = :name, category = :category, price = :price, stock = :stock WHERE ID = :id"));
        q.bindValue(":name", m_name);
        q.bindValue(":category", m_category);
        q.bindValue(":price", m_price);
        q.bindValue(":stock", m_stock);
        q.bindValue(":id", m_id);
        q.exec();
    }
}


/**
 * Removes the product from the database.
 *
 * The product can only be deleted if it already existed in the database.
 * A product with a nonzero value of id is assumed to already have existed in the database.
 * Once the product has been removed from the database, its id is set to 0.
 * Calling remove() on a product with its id = 0 will not do anything.
 * However, it is possible to setName() and save() to add a new product into the database.
**/
void Product::remove()
{
    QSqlQuery q;

    if (m_id != 0) {
        q.prepare(QLatin1String("DELETE FROM products WHERE id = :id"));
        q.bindValue(":id", m_id);
        q.exec();
        m_id = 0;
    }
}

/**
 * Selects the product with the given id from the database.
 * Returns a Product object.
**/
Product Product::getById(int productId)
{
    QSqlQuery q;
    q.prepare(QLatin1String("SELECT * FROM products WHERE id = :id"));
    q.bindValue(":id", productId);
    q.exec();

    if (q.next()) {
        Product product;
        product.setId(q.value("id").toInt());
        product.setName(q.value("name").toString());
        product.setCategory(q.value("category").toInt());
        product.setPrice(q.value("price").toInt());
        product.setStock(q.value("stock").toInt());
        return product;
    };

    return Product();
}

/**
 * Selects all products from the database.
 * Returns a QList of Product objects.
**/
QList<Product> Product::getAll()
{
    QList<Product> products;

    QSqlQuery q(QLatin1String("SELECT * FROM products"));
    q.exec();

    while (q.next()) {
        Product product;
        product.setId(q.value("id").toInt());
        product.setName(q.value("name").toString());
        product.setCategory(q.value("category").toInt());
        product.setPrice(q.value("price").toInt());
        product.setStock(q.value("stock").toInt());
        products.append(product);
    }

    return products;
}
