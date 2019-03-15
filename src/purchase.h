#ifndef PURCHASE_H
#define PURCHASE_H

#include <QString>
#include <QList>

class Purchase
{
public:
    Purchase(int purchaseProduct = 0, int purchaseQuantity = 0, int purchaseAmount = 0, const QString &date = 0);
    ~Purchase();
    int id() const;
    int product() const;
    int quantity() const;
    int amount() const;
    const QString &date() const;
    void setId(int purchaseId);
    void setProduct(int purchaseProduct);
    void setQuantity(int purchaseQuantity);
    void setAmount(int purchaseAmount);
    void setDate(const QString &productDate);
    void save();
    void remove();

private:
    int m_id;
    int m_product;
    int m_quantity;
    int m_amount;
    QString m_date;

public:
    static const int columnCount = 5;
    static Purchase getById(int purchaseId);
    static QList<Purchase> getAll();
};

#endif // PURCHASE_H
