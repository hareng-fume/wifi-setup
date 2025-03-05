#include "settings.h"

#include <QFile>

namespace Communication {

//-----------------------------------------------------------------------------
ISettings::~ISettings() {}

//-----------------------------------------------------------------------------
bool JsonSettings::ReadFromFile(const QString &i_filePath)
{
    m_jsonDocument = QJsonDocument();

    QFile file(i_filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "Failed to open " << i_filePath;
        return false;
    }

    auto jsonData = file.readAll();
    file.close();

    auto jsonDoc = QJsonDocument::fromJson(jsonData);
    if (jsonDoc.isNull() || !jsonDoc.isObject()) {
        qDebug() << "Invalid JSON format in " << i_filePath;
        return false;
    }
    m_jsonDocument = std::move(jsonDoc);
    return true;
}

//-----------------------------------------------------------------------------
QVariant JsonSettings::Get(const QString &i_name) const
{
    Q_UNUSED(i_name);
    return {};
}

} // namespace Communication
