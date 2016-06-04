#ifndef PROCEDURERESULT_H
#define PROCEDURERESULT_H

#include <QString>

enum EntityType{ FROM, TO, ANIM };

class ProcedureResult
{
public:
    ProcedureResult(bool outcome, QString message);
    bool getResult();
    QString getMessage();
    void setMessage(QString message);

private:
    bool result;
    QString errorMessage;
};

#endif // PROCEDURERESULT_H
