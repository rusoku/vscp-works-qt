// cdlgconnsettingsmqtt.h
//
// This file is part of the VSCP (https://www.vscp.org)
//
// The MIT License (MIT)
//
// Copyright © 2000-2025 Ake Hedman, Grodans Paradis AB
// <info@grodansparadis.com>
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
//

#ifndef CDLGCONNSETTINGSMQTT_H
#define CDLGCONNSETTINGSMQTT_H

#include "vscp-client-mqtt.h"
#include "vscphelper.h"

#include <QDialog>
#include <QListWidgetItem>

// ----------------------------------------------------------------------------

class SubscribeItem : public QListWidgetItem {

public:
  SubscribeItem(const QString& topic, enumMqttMsgFormat fmt = autofmt, int qos = 0, uint32_t v5_options = 0);
  ~SubscribeItem();

  /// Getters/setters for topic
  QString getTopic(void) { return m_topic; };
  void setTopic(const QString& topic)
  {
    m_topic = topic;
    setText(topic);
  };

  /// Getters/setters for format
  enumMqttMsgFormat getFormat(void) { return m_format; };
  int getFormatInt(void) { return static_cast<int>(m_format); };
  void setFormat(enumMqttMsgFormat fmt) { m_format = fmt; };
  void setFormatFromInt(int fmt)
  {
    m_format = static_cast<enumMqttMsgFormat>(fmt);
  };

  /// Getters/setters for qos
  int getQos(void) { return m_qos; };
  void setQos(int qos) { m_qos = qos; };

  /// Getters/setters for v5 options
  uint32_t getV5Options(void) { return m_v5_options; };
  void setV5Options(uint32_t v5_options) { m_v5_options = v5_options; };

private:
  /// True if item is active
  bool m_bActive;

  /// Subscribe topic
  QString m_topic;

  /// Subscribe format
  enumMqttMsgFormat m_format;

  /// Quality of service
  int m_qos;

  /// Version 5 options
  uint32_t m_v5_options;
};

// ----------------------------------------------------------------------------

class PublishItem : public QListWidgetItem {

public:
  PublishItem(const QString& topic,
              enumMqttMsgFormat fmt = jsonfmt,
              int qos               = 0,
              bool bretain          = false);
  ~PublishItem();

  /// Getters/setters for topic
  QString getTopic(void) { return m_topic; };
  void setTopic(const QString& topic);

  /// Getters/setters for qos
  int getQos(void) { return m_qos; };
  void setQos(int qos) { m_qos = qos; };

  /// Getters/setters for retain
  bool getRetain(void) { return m_bRetain; };
  void setRetain(bool bretain) { m_bRetain = bretain; };

  /// Getters/setters for format
  enumMqttMsgFormat getFormat(void) { return m_format; };
  int getFormatInt(void) { return static_cast<int>(m_format); };
  void setFormat(enumMqttMsgFormat fmt) { m_format = fmt; };
  void setFormatFromInt(int fmt)
  {
    m_format = static_cast<enumMqttMsgFormat>(fmt);
  };

private:
  /// True if item is active
  bool m_bActive;

  /// Quality of service
  int m_qos;

  /// Retain message
  bool m_bRetain;

  // Publish topic
  QString m_topic;

  // Publishing format
  enumMqttMsgFormat m_format;
};

// ----------------------------------------------------------------------------

class UserEscapeItem : public QListWidgetItem {

public:
  UserEscapeItem(const QString& name, const QString& value);
  ~UserEscapeItem();

  /// Getters/setters for name
  QString getName(void) { return m_name; };
  void setName(const QString& name);

  /// Getters/setters for value
  QString getValue(void) { return m_value; };
  void setValue(const QString& value);

private:
  // Escape name
  QString m_name;

  // Escape value
  QString m_value;
};

// ----------------------------------------------------------------------------

namespace Ui {
class CDlgConnSettingsMqtt;
}

class CDlgConnSettingsMqtt : public QDialog {
  Q_OBJECT

public:
  explicit CDlgConnSettingsMqtt(QWidget* parent = nullptr);
  ~CDlgConnSettingsMqtt();

  /*!
      Set inital focus to description
  */
  void setInitialFocus(void);

  /*!
      Setters/getters for name/description
  */
  QString getName(void);
  void setName(const QString& str);

  /*!
      Setters/getters for host/broker
  */
  QString getBroker(void);
  void setBroker(const QString& str);

  /*!
      Setters/getters for port
  */
  QString getClientId(void);
  void setClientId(const QString& clientid);

  /*!
      Setters/getters for Full level II
  */
  bool getFullL2(void);
  void setFullL2(bool l2);

  /*!
      Setters/getters for user
  */
  QString getUser(void);
  void setUser(const QString& str);

  /*!
      Setters/getters for password
  */
  QString getPassword(void);
  void setPassword(const QString& str);

  /*!
      Setters/getters for response timeout
  */
  uint32_t getResponseTimeout(void);
  void setResponseTimeout(uint32_t timeout);

  /*!
      Setters/getters for keepalive period
  */
  uint32_t getKeepAlive(void);
  void setKeepAlive(uint32_t timeout);

  /*!
      Setters/getters for connection timeout period
  */
  uint32_t getConnectTimeout(void);
  void setConnectTimeout(uint32_t timeout);

  /*!
      Setters/getters for clean session
  */
  bool isCleanSessionEnabled(void);
  void enableCleanSession(bool bClearSession);

  /*!
      Setters/getters for extended security
  */
  bool isExtendedSecurityEnabled(void);
  void enableExtendedSecurity(bool bExtendedSecurity);

  /*!
      Setters/getters for UseTopicForEventDefaults
  */
  bool isUseTopicForEventDefaults(void);
  void enableUseTopicForEventDefaults(bool b);

  /*!
      Setters/getters for TLS enable
  */
  bool isTLSEnabled(void);
  void enableTLS(bool bTLS);

  /*!
      Setters/getters for TLS enable
  */
  bool isVerifyPeerEnabled(void);
  void enableVerifyPeer(bool bverifypeer);

  /*!
      Setters/getters for TLS CA file
  */
  QString getCaFile(void);
  void setCaFile(const QString& str);

  /*!
      Setters/getters for TLS CA path
  */
  QString getCaPath(void);
  void setCaPath(const QString& str);

  /*!
      Setters/getters for TLS Cert file
  */
  QString getCertFile(void);
  void setCertFile(const QString& str);

  /*!
      Setters/getters for TLS key file
  */
  QString getKeyFile(void);
  void setKeyFile(const QString& str);

  /*!
      Setters/getters for TLS pw key file
  */
  QString getPwKeyFile(void);
  void setPwKeyFile(const QString& str);

  /*!
      Setters/getters for JSON config object
  */
  json getJson(void);
  void setJson(const json* pobj);

private slots:

  /// Add subscription
  void onAddSubscription(void);

  /// Edit subscription
  void onEditSubscription(void);

  /// Clone subscription
  void onCloneSubscription(void);

  /// Delete subscription
  void onDeleteSubscription(void);

  /// Add publish
  void onAddPublish(void);

  /// Edit publish
  void onEditPublish(void);

  /// Clone publish
  void onClonePublish(void);

  /// Delete publish
  void onDeletePublish(void);

  /// Add UserEscape
  void onAddUserEscape(void);

  /// Edit UserEscape
  void onEditUserEscape(void);

  /// Clone UserEscape
  void onCloneUserEscape(void);

  /// Delete UserEscape
  void onDeleteUserEscape(void);

  /// Test connection button clicked
  void onTestConnection(void);

  /// TLS settings button clicked
  void onTLSSettings(void);

  /// Get help with settings
  void onGetHelp(void);

  /// Subscription context menu
  void onSubscribeContextMenu(const QPoint& pos);

  /// Publish context menu
  void onPublishContextMenu(const QPoint& pos);

  /// User escape context menu
  void onUserEscapeContextMenu(const QPoint& pos);

private:
  Ui::CDlgConnSettingsMqtt* ui;

  /// TLS flag (secure transport if enabled)
  bool m_bTLS;

  /*!
      the server certificate will be verified and the connection
      aborted if the verification fails.
  */
  bool m_bVerifyPeer;

  /// CA file
  QString m_cafile;

  /// Path to CA file (can hold filename also)
  QString m_capath;

  /// Path to CERT file
  QString m_certfile;

  /// Key file
  QString m_keyfile;

  /// Password keyfile
  QString m_pwkeyfile;

  /*!
      This variable holds the connection type that
      the used select
  */
  CVscpClient::connType m_selected_type;

  // JSON configuration object
  json m_jsonConfig;

  /// VSCP MQTT client
  vscpClientMqtt m_client;
};

#endif // CDLGCONNSETTINGSMQTT_H
