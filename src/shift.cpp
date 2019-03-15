#include "shift.h"
#include "sale.h"
#include <QtSql>

/**
 * Constructs an shift object.
**/
Shift::Shift(int shiftCashier, const QString &shiftStartDate, const QString &shiftEndDate)
{
    m_id = 0;
    m_cashier = shiftCashier;
    m_start_date = shiftStartDate;
    m_end_date = shiftEndDate;
}

/**
 * Destructs the shift object.
**/
Shift::~Shift()
{
}

/**
 * Returns the id of the shift.
**/
int Shift::id() const
{
    return m_id;
}

/**
 * Returns the cashier of the shift.
**/
int Shift::cashier() const
{
    return m_cashier;
}

/**
 * Returns the start date of the shift.
**/
const QString &Shift::startDate() const
{
    return m_start_date;
}

/**
 * Returns the end date of the shift.
**/
const QString &Shift::endDate() const
{
    return m_end_date;
}

/**
 * Sets the id of the shift to shiftId.
**/
void Shift::setId(int shiftId)
{
    m_id = shiftId;
}

/**
 * Sets the cashier of the shift to shiftCashier.
**/
void Shift::setCashier(int shiftCashier)
{
    m_cashier = shiftCashier;
}

/**
 * Sets the start date of the sale to shiftStartDate.
**/
void Shift::setStartDate(const QString &shiftStartDate)
{
    m_start_date = shiftStartDate;
}

/**
 * Sets the end date of the sale to shiftEndDate.
**/
void Shift::setEndDate(const QString &shiftEndDate)
{
    m_end_date = shiftEndDate;
}

/**
 * Saves the shift to database.
 *
 * If this is a newly created shift, an INSERT INTO is used.
 * An shift with its id = 0 is assumed to be a new shift.
 * Once created, the id of the shift is set to the id of the newly inserted shift.
 *
 * If this shift already existed in the database, an UPDATE statement is used.
 * A shift with a nonzero value of ID is assumed to already have existed in the database.
**/
void Shift::save()
{
    QSqlQuery q;

    if (m_id == 0) {
        q.prepare(QLatin1String("INSERT INTO shifts (cashier, start_date, end_date) VALUES(:cashier, :start_date, :end_date)"));
        q.bindValue(":cashier", m_cashier);
        q.bindValue(":start_date", m_start_date);
        q.bindValue(":end_date", m_end_date);
        q.exec();
        m_id = q.lastInsertId().toInt();
    } else {
        q.prepare(QLatin1String("UPDATE shifts SET cashier = :cashier, start_date = :start_date, end_date = :end_date WHERE ID = :id"));
        q.bindValue(":cashier", m_cashier);
        q.bindValue(":start_date", m_start_date);
        q.bindValue(":end_date", m_end_date);
        q.bindValue(":id", m_id);
        q.exec();
    }
}

/**
 * Removes the shift from the database.
 *
 * The shift can only be deleted if it already existed in the database.
 * A shift with a nonzero value of ID is assumed to already have existed in the database.
 * Once the shift has been removed from the database, its ID is set to 0.
 * Calling remove() on a shift with its ID = 0 will not do anything.
 * However, it is possible to setName() and save() to add a new shift into the database.
**/
void Shift::remove()
{
    QSqlQuery q;

    if (m_id != 0) {
        q.prepare(QLatin1String("DELETE FROM shifts WHERE id = :id"));
        q.bindValue(":id", m_id);
        q.exec();
        m_id = 0;
    }
}

/**
 * Selects the shift with the given id from the database.
 * Returns a Shift object.
**/
Shift Shift::getById(int shiftId)
{
    Shift shift;

    QSqlQuery q;
    q.prepare(QLatin1String("SELECT * FROM shifts WHERE id = :id"));
    q.bindValue(":id", shiftId);
    q.exec();

    if (q.next()) {
        shift.setId(q.value("id").toInt());
        shift.setCashier(q.value("cashier").toInt());
        shift.setStartDate(q.value("start_date").toString());
        shift.setEndDate(q.value("end_date").toString());
    };

    return shift;
}

/**
 * Selects all shifts from the database.
 * Returns a QList of Shift objects.
**/
QList<Shift> Shift::getAll()
{
    QList<Shift> shifts;

    QSqlQuery q(QLatin1String("SELECT * FROM shifts"));
    q.exec();

    while (q.next()) {
        Shift shift;
        shift.setId(q.value("id").toInt());
        shift.setCashier(q.value("cashier").toInt());
        shift.setStartDate(q.value("start_date").toString());
        shift.setEndDate(q.value("end_date").toString());
        shifts.append(shift);
    }

    return shifts;
}

/**
 * Selects all shifts by a given cashier from the database.
 * Returns a QList of Shift objects.
**/
QList<Shift> Shift::getAllByCashier(int shiftCashierId)
{
    QList<Shift> shifts;

    QSqlQuery q;
    q.prepare(QLatin1String("SELECT * FROM shifts WHERE cashier = :cashier"));
    q.bindValue(":cashier", shiftCashierId);
    q.exec();

    while (q.next()) {
        Shift shift;
        shift.setId(q.value("id").toInt());
        shift.setCashier(q.value("cashier").toInt());
        shift.setStartDate(q.value("start_date").toString());
        shift.setEndDate(q.value("end_date").toString());
        shifts.append(shift);
    }

    return shifts;
}
