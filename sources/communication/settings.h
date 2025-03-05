#ifndef SETTINGS_H
#define SETTINGS_H

#include <QJsonDocument>
#include <QVariant>

namespace Communication {

class ISettings
{
public:
    virtual ~ISettings() = 0;
    virtual bool ReadFromFile(const QString &i_filePath) = 0;
    virtual QVariant Get(const QString &i_name) const = 0;
};

class JsonSettings final : public ISettings
{
public:
    virtual bool ReadFromFile(const QString &i_filePath) override;
    virtual QVariant Get(const QString &i_name) const override;

private:
    QJsonDocument m_jsonDocument;
};

} // namespace Communication

#endif // SETTINGS_H
