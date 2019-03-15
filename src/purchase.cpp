#include "purchase.h"
#include <QtSql>

/**
 * Constructs an purchase object.
**/
Purchase::Purchase(int purchaseProduct, int purchaseQuantity, int purchaseAmount, const QString &purchaseDate)
{
    m_id = 0;
    m_product = purchaseProduct;
    m_quantity = purchaseQuantity;
    m_amount = purchaseAmount;
    m_date = purchaseDate;
}

/**
 * Destructs the purchase object.
**/
Purchase::~Purchase()
{
}

/**
 * Returns the ID of the purchase.
**/
int Purchase::id() const
{
    return m_id;
}

/**
 * Returns the product ID of the purchase.
**/
int Purchase::product() const
{
    return m_product;
}

/**
 * Returns the quantity of the purchase.
**/
int Purchase::quantity() const
{
    return m_quantity;
}

/**
 * Returns the total cost amount of the purchase.
**/
int Purchase::amount() const
{
    return m_amount;
}

/**
 * Returns the date of the purchase.
**/
const QString &Purchase::date() const
{
    return m_date;
}

/**
 * Sets the ID of the purchase to purchaseId.
**/
void Purchase::setId(int purchaseId)
{
    m_id = purchaseId;
}

/**
 * Sets the product ID of the purchase to purchaseProduct.
**/
void Purchase::setProduct(int purchaseProduct)
{
    m_product = purchaseProduct;
}

/**
 * Sets the quantity of the purchase to purchaseQuantity.
**/
void Purchase::setQuantity(int purchaseQuantity)
{
    m_quantity = purchaseQuantity;
}

/**
 * Sets the total cost amount of the purchase to purchaseAmount.
**/
void Purchase::setAmount(int purchaseAmount)
{
    m_amount = purchaseAmount;
}

/**
 * Sets the date of the purchase to purchaseDate.
**/
void Purchase::setDate(const QString &purchaseDate)
{
    m_date = purchaseDate;
}

/**
 * Saves the purchase to database.
 *
 * If this is a newly created purchase, an INSERT INTO is used.
 * An purchase with its id = 0 is assumed to be a new purchase.
 * Once created, the id of the purchase is set to the id of the newly inserted purchase.
 *
 * If this purchase already existed in the database, an UPDATE statement is used.
 * A purchase with a nonzero value of id is assumed to already have existed in the database.
**/
void Purchase::save()
{
    QSqlQuery q;

    if (m_id == 0) {
        q.prepare(QLatin1String("INSERT INTO purchases (product, quantity, amount, date) VALUES(:product, :quantity, :amount, :date)"));
        q.bindValue(":product", m_product);
        q.bindValue(":quantity", m_quantity);
        q.bindValue(":amount", m_amount);
        q.bindValue(":date", m_date);
        q.exec();
        m_id = q.lastInsertId().toInt();
    } else {
        q.prepare(QLatin1String("UPDATE purchases SET product = :product, quantity = :quantity, amount = :amount, date = :date WHERE ID = :id"));
        q.bindValue(":product", m_product);
        q.bindValue(":quantity", m_quantity);
        q.bindValue(":amount", m_amount);
        q.bindValue(":date", m_date);
        q.bindValue(":id", m_id);
        q.exec();
    }
}


/**
 * Removes the purchase from the database.
 *
 * The purchase can only be deleted if it already existed in the database.
 * A purchase with a nonzero value of id is assumed to already have existed in the database.
 * Once the purchase has been removed from the database, its id is set to 0.
 * Calling remove() on a purchase with its id = 0 will not do anything.
 * However, it is possible to setDate() and save() to add a new purchase into the database.
**/
void Purchase::remove()
{
    QSqlQuery q;

    if (m_id != 0) {
        q.prepare(QLatin1String("DELETE FROM purchases WHERE id = :id"));
        q.bindValue(":id", m_id);
        q.exec();
        m_id = 0;
    }
}

/**
 * Selects the purchase with the given id from the database.
 * Returns a Purchase object.
**/
Purchase Purchase::getById(int purchaseId)
{
    QSqlQuery q;
    q.prepare(QLatin1String("SELECT * FROM purchases WHERE id = :id"));
    q.bindValue(":id", purchaseId);
    q.exec();

    if (q.next()) {
        Purchase purchase;
        purchase.setId(q.value("id").toInt());
        purchase.setProduct(q.value("product").toInt());
        purchase.setQuantity(q.value("quantity").toInt());
        purchase.setAmount(q.value("amount").toInt());
        purchase.setDate(q.value("date").toString());
        return purchase;
    };

    return Purchase();
}

/**
 * Selects all purchases from the database.
 * Returns a QList of Purchase objects.
**/
QList<Purchase> Purchase::getAll()
{
    QList<Purchase> purchases;

    QSqlQuery q(QLatin1String("SELECT * FROM purchases"));
    q.exec();

    while (q.next()) {
        Purchase purchase;
        purchase.setId(q.value("id").toInt());
        purchase.setProduct(q.value("product").toInt());
        purchase.setQuantity(q.value("quantity").toInt());
        purchase.setAmount(q.value("amount").toInt());
        purchase.setDate(q.value("date").toString());
        purchases.append(purchase);
    }

    return purchases;
}
