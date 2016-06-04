#include "procedureresult.h"

ProcedureResult::ProcedureResult(bool outcome, QString message):
    result(outcome), errorMessage(message)
{

}

void ProcedureResult::setMessage(QString message){
    errorMessage = message;
}

bool ProcedureResult::getResult()
{
    return result;
}

QString ProcedureResult::getMessage(){
    return errorMessage;
}

