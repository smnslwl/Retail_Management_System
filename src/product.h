#ifndef PRODUCT_H
#define PRODUCT_H

#include <QString>
#include <QList>

class Product
{
public:
    Product(const QString &productName = "", int productCategory = 0, int productPrice = 0, int productStock = 0);
    ~Product();
    int id() const;
    const QString &name() const;
    int category() const;
    int price() const;
    int stock() const;
    void setId(int productId);
    void setName(const QString &productName);
    void setCategory(int productCategory);
    void setPrice(int productPrice);
    void setStock(int productStock);
    void save();
    void remove();

private:
    int m_id;
    QString m_name;
    int m_category;
    int m_price;
    int m_stock;

public:
    static const int columnCount = 5;
    static Product getById(int productId);
    static QList<Product> getAll();
};

#endif // PRODUCT_H
