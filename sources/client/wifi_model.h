#pragma once

#include <QAbstractListModel>
#include <QByteArray>
#include <QHash>
#include <QList>
#include <QPair>
#include <QString>
#include <QVariant>

class WifiNetwork {

    Q_GADGET

    Q_PROPERTY(QString name READ name WRITE setName)
    Q_PROPERTY(ConnectionStatus status READ status WRITE setStatus)

public:
    enum ConnectionStatus {
        Disconnected = 0,
        Connected = 1,
        FailedToConnect = 2
    };
    Q_ENUM(ConnectionStatus)

    // declare class to the QML system.
    static void registerMetatypes();

    WifiNetwork() = default;
    WifiNetwork(const QString& i_name, ConnectionStatus i_status)
        : m_name(i_name), m_status(i_status){}

    QString name() const { return m_name; }
    ConnectionStatus status() const { return m_status; }

    void setName(const QString& i_name) { m_name = i_name; }
    void setStatus(ConnectionStatus i_status) { m_status = i_status; }

private:
    QString m_name;
    ConnectionStatus m_status;
};
Q_DECLARE_METATYPE(WifiNetwork)

class WifiNetworkModel : public QAbstractListModel {

    Q_OBJECT

public:
    enum Roles {
        NameRole = Qt::DisplayRole,  // wsed for showing the network name
        ConnectionRole = Qt::UserRole + 1  // custom role for connection status
    };

    using Item = WifiNetwork;
    using ItemList = QList<Item>;

public:
    explicit WifiNetworkModel(QObject* ip_parent = nullptr);

    int rowCount(const QModelIndex& i_parent = QModelIndex()) const override;
    QVariant data(const QModelIndex& i_index, int i_role) const override;
    QHash<int, QByteArray> roleNames() const override;

    Q_INVOKABLE QVariant get(int index) const;

    void resetItems(const ItemList& i_networks);
    void setConnectionStatus(const QString& i_networkName, WifiNetwork::ConnectionStatus i_connectionStatus);

private:
    ItemList m_networks;
};
