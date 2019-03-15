#ifndef GROUPSALE_H
#define GROUPSALE_H

#include <QString>
#include <QList>

class GroupSale
{
public:
    GroupSale(const QString &groupSaleName = "", int groupSaleDiscount = 0, int groupSaleTax = 0, int groupSaleShift = 0, const QString &groupSaleDate = "");
    ~GroupSale();
    int id() const;
    const QString &name() const;
    int discount() const;
    int tax() const;
    int shift() const;
    const QString &date() const;
    void setId(int saleId);
    void setName(const QString &groupSaleName);
    void setDiscount(int groupSaleDiscount);
    void setTax(int groupSaleTax);
    void setShift(int groupSaleShift);
    void setDate(const QString &groupSaleDate);
    void save();
    void remove();
    void calculateTotals();
    int subtotal() const;
    int total() const;

private:
    int m_id;
    QString m_name;
    int m_discount;
    int m_tax;
    int m_shift;
    QString m_date;
    int m_subtotal;
    int m_total;

public:
    static const int columnCount = 8;
    static GroupSale getById(int groupSaleId);
    static QList<GroupSale> getAll();
    static QList<GroupSale> getAllByShift(int groupSaleShiftId);
};

#endif // GROUPSALE_H
