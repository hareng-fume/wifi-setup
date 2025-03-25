#include "wifi_model.h"

//-----------------------------------------------------------------------------
void WifiNetwork::registerMetatypes() {
  qRegisterMetaType<WifiNetwork>("WifiNetwork");
}

//-----------------------------------------------------------------------------
WifiNetworkModel::WifiNetworkModel(QObject *ip_parent /*= nullptr*/)
    : QAbstractListModel(ip_parent) {}

//-----------------------------------------------------------------------------
int WifiNetworkModel::rowCount(
    const QModelIndex &i_parent /*= QModelIndex()*/) const {
  Q_UNUSED(i_parent);
  return m_networks.size();
}

//-----------------------------------------------------------------------------
QVariant WifiNetworkModel::data(const QModelIndex &i_index, int i_role) const {
  if (!i_index.isValid() || i_index.row() >= m_networks.size())
    return QVariant();

  const auto &network = m_networks.at(i_index.row());
  if (i_role == NameRole)
    return network.name(); // network name
  else if (i_role == ConnectionRole)
    return network.status(); // connection status

  return QVariant();
}

//-----------------------------------------------------------------------------
QHash<int, QByteArray> WifiNetworkModel::roleNames() const {
  QHash<int, QByteArray> roles;
  roles[NameRole] = "display";
  roles[ConnectionRole] = "connection";
  return roles;
}

//-----------------------------------------------------------------------------
QVariant WifiNetworkModel::get(int index) const {
  if (index >= 0 && index < m_networks.size())
    return QVariant::fromValue(m_networks.at(index));

  return QVariant();
}

//-----------------------------------------------------------------------------
void WifiNetworkModel::resetItems(const ItemList &i_networks) {
  beginResetModel();
  m_networks = i_networks;
  endResetModel();
}

//-----------------------------------------------------------------------------
void WifiNetworkModel::setConnectionStatus(const QString &i_networkName,
    WifiNetwork::ConnectionStatus i_connectionStatus) {
  for (int i = 0; i < m_networks.size(); ++i) {
    if (m_networks[i].name() == i_networkName) {
      m_networks[i].setStatus(i_connectionStatus);
      emit dataChanged(index(i), index(i), {ConnectionRole});
      return;
    }
  }
}
