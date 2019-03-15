#ifndef SHIFT_H
#define SHIFT_H

#include <QString>
#include <QList>

class Shift
{
public:
    Shift(int cashier = 0, const QString &shiftStartDate = "", const QString &endStartDate = "");
    ~Shift();
    int id() const;
    int cashier() const;
    const QString &startDate() const;
    const QString &endDate() const;
    void setId(int shiftId);
    void setCashier(int shiftCashier);
    void setStartDate(const QString &shiftStartDate);
    void setEndDate(const QString &shiftEndDate);
    void save();
    void remove();

private:
    int m_id;
    int m_cashier;
    QString m_start_date;
    QString m_end_date;

public:
    static const int columnCount = 4;
    static Shift getById(int shiftId);
    static QList<Shift> getAll();
    static QList<Shift> getAllByCashier(int shiftCashierId);
};

#endif // SHIFT_H
