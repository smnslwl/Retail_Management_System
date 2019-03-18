#include "groupsale.h"
#include "sale.h"
#include <QtSql>

/**
 * Constructs an groupSale object.
**/
GroupSale::GroupSale(const QString &groupSaleName, int groupSaleDiscount, int groupSaleTax, int groupSaleShift, const QString &groupSaleDate)
{
    m_id = 0;
    m_name = groupSaleName;
    m_discount = groupSaleDiscount;
    m_tax = groupSaleTax;
    m_shift = groupSaleShift;
    m_date = groupSaleDate;
}

/**
 * Destructs the groupSale object.
**/
GroupSale::~GroupSale()
{
}

/**
 * Returns the id of the groupSale.
**/
int GroupSale::id() const
{
    return m_id;
}

/**
 * Returns the name of the groupSale (the customer name).
**/
const QString &GroupSale::name() const
{
    return m_name;
}

/**
 * Returns the discount of the groupSale.
**/
int GroupSale::discount() const
{
    return m_discount;
}

/**
 * Returns the tax of the groupSale.
**/
int GroupSale::tax() const
{
    return m_tax;
}

/**
 * Returns the shift of the groupSale.
**/
int GroupSale::shift() const
{
    return m_shift;
}

/**
 * Returns the date of the groupSale.
**/
const QString &GroupSale::date() const
{
    return m_date;
}

/**
 * Sets the id of the groupSale to groupSaleId.
**/
void GroupSale::setId(int groupSaleId)
{
    m_id = groupSaleId;
}

/**
 * Sets the name of the groupSale to groupSaleName.
**/
void GroupSale::setName(const QString &groupSaleName)
{
    m_name = groupSaleName;
}

/**
 * Sets the discount of the groupSale to groupSaleDiscount.
**/
void GroupSale::setDiscount(int groupSaleDiscount)
{
    m_discount = groupSaleDiscount;
}

/**
 * Sets the tax of the groupSale to groupSaleTax.
**/
void GroupSale::setTax(int groupSaleTax)
{
    m_tax = groupSaleTax;
}

/**
 * Sets the shift of the groupSale to groupSaleShift.
**/
void GroupSale::setShift(int groupSaleShift)
{
    m_shift = groupSaleShift;
}

/**
 * Sets the date of the sale to saleDate.
**/
void GroupSale::setDate(const QString &groupSaleDate)
{
    m_date = groupSaleDate;
}

/**
 * Saves the groupSale to database.
 *
 * If this is a newly created groupSale, an INSERT INTO is used.
 * An groupSale with its id = 0 is assumed to be a new groupSale.
 * Once created, the id of the groupSale is set to the id of the newly inserted groupSale.
 *
 * If this groupSale already existed in the database, an UPDATE statement is used.
 * A groupSale with a nonzero value of ID is assumed to already have existed in the database.
**/
void GroupSale::save()
{
    QSqlQuery q;

    if (m_id == 0) {
        q.prepare(QLatin1String("INSERT INTO groupsales (name, discount, tax, shift, date) VALUES(:name, :discount, :tax, :shift, :date)"));
        q.bindValue(":name", m_name);
        q.bindValue(":discount", m_discount);
        q.bindValue(":tax", m_tax);
        q.bindValue(":shift", m_shift);
        q.bindValue(":date", m_date);
        q.exec();
        m_id = q.lastInsertId().toInt();
    } else {
        q.prepare(QLatin1String("UPDATE groupsales SET name = :name, discount = :discount, tax = :tax, shift = :shift, date = :date WHERE ID = :id"));
        q.bindValue(":name", m_name);
        q.bindValue(":discount", m_discount);
        q.bindValue(":tax", m_tax);
        q.bindValue(":shift", m_shift);
        q.bindValue(":date", m_date);
        q.bindValue(":id", m_id);
        q.exec();
    }
}

/**
 * Removes the groupSale from the database.
 *
 * The groupSale can only be deleted if it already existed in the database.
 * A groupSale with a nonzero value of ID is assumed to already have existed in the database.
 * Once the groupSale has been removed from the database, its ID is set to 0.
 * Calling remove() on a groupSale with its ID = 0 will not do anything.
 * However, it is possible to setName() and save() to add a new groupSale into the database.
**/
void GroupSale::remove()
{
    QSqlQuery q;

    if (m_id != 0) {
        q.prepare(QLatin1String("DELETE FROM groupsales WHERE id = :id"));
        q.bindValue(":id", m_id);
        q.exec();
        m_id = 0;
    }
}

/**
 * Calculates subtotal and total for this sale.
 * The subtotal is calculated by adding the amount of all sales belonging to this group.
 * The total is calculated by first applying discount and then the tax on the subtotal.
**/
void GroupSale::calculateTotals()
{
    m_subtotal = 0;
    QList<Sale> sales = Sale::getAllByGroup(m_id);
    for(int i = 0; i < sales.size(); i++) {
        m_subtotal += sales[i].amount();
    }

    double net = m_subtotal;
    if (m_discount && m_subtotal) {
        net = (m_subtotal - m_discount * (double) m_subtotal / 100);
    }

    double total = net;
    if (m_tax && net > 0) {
        total = net + m_tax * net / 100;
    }

    m_total = std::round(total);
}

/**
 * Returns the subtotal of the groupSale.
**/
int GroupSale::subtotal() const
{
    return m_subtotal;
}

/**
 * Returns the final amount of the groupSale.
**/
int GroupSale::total() const
{
    return m_total;
}

/**
 * Selects the groupSale with the given id from the database.
 * Returns a GroupSale object.
**/
GroupSale GroupSale::getById(int groupSaleId)
{
    GroupSale groupSale;

    QSqlQuery q;
    q.prepare(QLatin1String("SELECT * FROM groupsales WHERE id = :id"));
    q.bindValue(":id", groupSaleId);
    q.exec();

    if (q.next()) {
        groupSale.setId(q.value("id").toInt());
        groupSale.setName(q.value("name").toString());
        groupSale.setDiscount(q.value("discount").toInt());
        groupSale.setTax(q.value("tax").toInt());
        groupSale.setShift(q.value("shift").toInt());
        groupSale.setDate(q.value("date").toString());
        groupSale.calculateTotals();
    };

    return groupSale;
}

/**
 * Selects all groupSales from the database.
 * Returns a QList of GroupSale objects.
**/
QList<GroupSale> GroupSale::getAll()
{
    QList<GroupSale> groupSales;

    QSqlQuery q(QLatin1String("SELECT * FROM groupsales"));
    q.exec();

    while (q.next()) {
        GroupSale groupSale;
        groupSale.setId(q.value("id").toInt());
        groupSale.setName(q.value("name").toString());
        groupSale.setDiscount(q.value("discount").toInt());
        groupSale.setTax(q.value("tax").toInt());
        groupSale.setShift(q.value("shift").toInt());
        groupSale.setDate(q.value("date").toString());
        groupSales.append(groupSale);
        groupSale.calculateTotals();
    }

    return groupSales;
}

/**
 * Selects all groupSales by a given cashier from the database.
 * Returns a QList of GroupSale objects.
**/
QList<GroupSale> GroupSale::getAllByShift(int groupSaleShiftId)
{
    QList<GroupSale> groupSales;

    QSqlQuery q;
    q.prepare(QLatin1String("SELECT * FROM groupsales WHERE shift = :shift"));
    q.bindValue(":shift", groupSaleShiftId);
    q.exec();

    while (q.next()) {
        GroupSale groupSale;
        groupSale.setId(q.value("id").toInt());
        groupSale.setName(q.value("name").toString());
        groupSale.setDiscount(q.value("discount").toInt());
        groupSale.setTax(q.value("tax").toInt());
        groupSale.setShift(q.value("shift").toInt());
        groupSale.setDate(q.value("date").toString());
        groupSale.calculateTotals();
        groupSales.append(groupSale);
    }

    return groupSales;
}
