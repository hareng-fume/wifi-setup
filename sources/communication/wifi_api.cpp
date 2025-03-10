#include "wifi_api.h"

#include <QHash>

namespace Communication::Status {

QString getMessageFrom(int i_statusCode) {
    static const QHash<int, QString> MESSAGE_FROM_CODE = {
        {200, "Ok"},
        {400, "Bad Request"},
        {401, "Unauthorized"},
        {404, "Not Found"},
        {500, "Internal Server Error"}
    };
    return MESSAGE_FROM_CODE.value(i_statusCode, "Unknown Error");
}

} // namespace Communication::Status
