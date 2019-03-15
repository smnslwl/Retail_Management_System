#include "sale.h"
#include <QtSql>

/**
 * Constructs an sale object.
**/
Sale::Sale(int saleProduct, int saleQuantity, int saleAmount, int saleGroup, int saleIndex)
{
    m_id = 0;
    m_product = saleProduct;
    m_quantity = saleQuantity;
    m_amount = saleAmount;
    m_group = saleGroup;
    m_index = saleIndex;
}

/**
 * Destructs the sale object.
**/
Sale::~Sale()
{
}

/**
 * Returns the ID of the sale.
**/
int Sale::id() const
{
    return m_id;
}

/**
 * Returns the product ID of the sale.
**/
int Sale::product() const
{
    return m_product;
}

/**
 * Returns the quantity of the sale.
**/
int Sale::quantity() const
{
    return m_quantity;
}

/**
 * Returns the total cost amount of the sale.
**/
int Sale::amount() const
{
    return m_amount;
}

/**
 * Returns the group of the sale.
**/
int Sale::group() const
{
    return m_group;
}

/**
 * Returns the index of the sale.
**/
int Sale::index() const
{
    return m_index;
}

/**
 * Sets the ID of the sale to saleId.
**/
void Sale::setId(int saleId)
{
    m_id = saleId;
}

/**
 * Sets the product ID of the sale to saleProduct.
**/
void Sale::setProduct(int saleProduct)
{
    m_product = saleProduct;
}

/**
 * Sets the quantity of the sale to saleQuantity.
**/
void Sale::setQuantity(int saleQuantity)
{
    m_quantity = saleQuantity;
}

/**
 * Sets the total cost amount of the sale to saleAmount.
**/
void Sale::setAmount(int saleAmount)
{
    m_amount = saleAmount;
}

/**
 * Sets the group of the sale to saleGroup.
**/
void Sale::setGroup(int saleGroup)
{
    m_group = saleGroup;
}

/**
 * Sets the index of the sale to saleIndex.
**/
void Sale::setIndex(int saleIndex)
{
    m_index = saleIndex;
}

/**
 * Saves the sale to database.
 *
 * If this is a newly created sale, an INSERT INTO is used.
 * An sale with its id = 0 is assumed to be a new sale.
 * Once created, the id of the sale is set to the id of the newly inserted sale.
 *
 * If this sale already existed in the database, an UPDATE statement is used.
 * A sale with a nonzero value of id is assumed to already have existed in the database.
**/
void Sale::save()
{
    QSqlQuery q;

    if (m_id == 0) {
        q.prepare(QLatin1String("INSERT INTO sales (product, quantity, amount, groupsale, saleindex) VALUES(:product, :quantity, :amount, :groupsale, :saleindex)"));
        q.bindValue(":product", m_product);
        q.bindValue(":quantity", m_quantity);
        q.bindValue(":amount", m_amount);
        q.bindValue(":groupsale", m_group);
        q.bindValue(":saleindex", m_index);
        q.exec();
        m_id = q.lastInsertId().toInt();
    } else {
        q.prepare(QLatin1String("UPDATE sales SET product = :product, quantity = :quantity, amount = :amount, groupsale = :groupsale, saleindex = :saleindex WHERE ID = :id"));
        q.bindValue(":product", m_product);
        q.bindValue(":quantity", m_quantity);
        q.bindValue(":amount", m_amount);
        q.bindValue(":groupsale", m_group);
        q.bindValue(":saleindex", m_index);
        q.bindValue(":id", m_id);
        q.exec();
    }
}


/**
 * Removes the sale from the database.
 *
 * The sale can only be deleted if it already existed in the database.
 * A sale with a nonzero value of id is assumed to already have existed in the database.
 * Once the sale has been removed from the database, its id is set to 0.
 * Calling remove() on a sale with its id = 0 will not do anything.
 * However, it is possible to setDate() and save() to add a new sale into the database.
**/
void Sale::remove()
{
    QSqlQuery q;

    if (m_id != 0) {
        q.prepare(QLatin1String("DELETE FROM sales WHERE id = :id"));
        q.bindValue(":id", m_id);
        q.exec();
        m_id = 0;
    }
}

/**
 * Selects the sale with the given id from the database.
 * Returns a Sale object.
**/
Sale Sale::getById(int saleId)
{
    QSqlQuery q;
    q.prepare(QLatin1String("SELECT * FROM sales WHERE id = :id"));
    q.bindValue(":id", saleId);
    q.exec();

    if (q.next()) {
        Sale sale;
        sale.setId(q.value("id").toInt());
        sale.setProduct(q.value("product").toInt());
        sale.setQuantity(q.value("quantity").toInt());
        sale.setAmount(q.value("amount").toInt());
        sale.setGroup(q.value("groupsale").toInt());
        sale.setIndex(q.value("saleindex").toInt());
        return sale;
    };

    return Sale();
}

/**
 * Selects all sales from the database.
 * Returns a QList of Sale objects.
**/
QList<Sale> Sale::getAll()
{
    QList<Sale> sales;

    QSqlQuery q(QLatin1String("SELECT * FROM sales"));
    q.exec();

    while (q.next()) {
        Sale sale;
        sale.setId(q.value("id").toInt());
        sale.setProduct(q.value("product").toInt());
        sale.setQuantity(q.value("quantity").toInt());
        sale.setAmount(q.value("amount").toInt());
        sale.setGroup(q.value("groupsale").toInt());
        sale.setIndex(q.value("saleindex").toInt());
        sales.append(sale);
    }

    return sales;
}

/**
 * Selects all sales belonging to the given group from the database.
 * Returns a QList of Sale objects.
**/
QList<Sale> Sale::getAllByGroup(int groupId)
{
    QList<Sale> sales;

    QSqlQuery q;
    q.prepare(QLatin1String("SELECT * FROM sales WHERE groupsale = :groupsale"));
    q.bindValue(":groupsale", groupId);
    q.exec();

    while (q.next()) {
        Sale sale;
        sale.setId(q.value("id").toInt());
        sale.setProduct(q.value("product").toInt());
        sale.setQuantity(q.value("quantity").toInt());
        sale.setAmount(q.value("amount").toInt());
        sale.setGroup(q.value("groupsale").toInt());
        sale.setIndex(q.value("saleindex").toInt());
        sales.append(sale);
    }

    return sales;
}
