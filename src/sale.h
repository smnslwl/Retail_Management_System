#ifndef SALE_H
#define SALE_H

#include <QString>
#include <QList>

class Sale
{
public:
    Sale(int saleProduct = 0, int saleQuantity = 0, int saleAmount = 0, int saleGroup = 0, int saleIndex = 0);
    ~Sale();
    int id() const;
    int product() const;
    int quantity() const;
    int amount() const;
    int group() const;
    int index() const;
    void setId(int saleId);
    void setProduct(int saleProduct);
    void setQuantity(int saleQuantity);
    void setAmount(int saleAmount);
    void setGroup(int saleGroup);
    void setIndex(int saleIndex);
    void save();
    void remove();

private:
    int m_id;
    int m_product;
    int m_quantity;
    int m_amount;
    int m_group;
    int m_index;

public:
    static const int columnCount = 6;
    static Sale getById(int saleId);
    static QList<Sale> getAll();
    static QList<Sale> getAllByGroup(int groupId);
};

#endif // SALE_H
