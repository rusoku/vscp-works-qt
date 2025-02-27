// vscpworks.h
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

#ifndef VSCPWORKS_H
#define VSCPWORKS_H

#include <vscp.h>

#include <version.h>
#include <vscp-client-base.h>
#include <vscpunit.h>
#include <mdf.h>
#include <register.h>

#include "cfrmsession.h"

#include <QApplication>
#include <QByteArray>
#include <QDateTime>
#include <QJSValue>
#include <QMainWindow>
#include <QMutex>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QObject>
#include <list>

#include <mustache.hpp>
#include <sqlite3.h>

#include <nlohmann/json.hpp>
// https://github.com/nlohmann/json
using json = nlohmann::json;

#include "spdlog/sinks/rotating_file_sink.h"
#include "spdlog/spdlog.h"

using namespace kainjow::mustache;

// Register to be used for signals
Q_DECLARE_METATYPE(vscpEvent)
Q_DECLARE_METATYPE(vscpEventEx)

/*!
  Set to true to remove development functionality
  for an official release build.
*/
#define VSCP_WORKS_RELEASE false

/// Application name used in titles and headers
#define APPNAME "VSCP Works"

// home folder is used for storage of program configuration
// system folder holds databases etc
#ifdef WIN32
#define DEFAULT_HOME_FOLDER        "c:/programdata/vscp/vscpworks/"
#define DEFAULT_VSCP_SYSTEM_FOLDER "c:/programdata/vscp/"
#else
#define DEFAULT_HOME_FOLDER        "~/.vscpworks/"
#define DEFAULT_VSCP_SYSTEM_FOLDER "/var/lib/vscp/"
#endif

enum class numerical_base { HEX,
                            DECIMAL,
                            OCTAL,
                            BINARY };

class FileDownloader;

/*!
    Encapsulates VSCP works main settings
*/
class vscpworks : public QApplication {

  Q_OBJECT

public:
  /*!
    Constructor
  */
  vscpworks(int& argc, char** argv);

  /*!
    Destructor
  */
  ~vscpworks();

  // Use non scure to prevent certificat errors
  const QString URL_EVENT_VERSION     = tr("https://www.vscp.org/events/version.json");
  const QString URL_EVENT_DATABASE    = tr("https://www.vscp.org/events/vscp_events.sqlite3");
  const QString URL_VSCPWORKS_VERSION = tr("https://www.vscp.org/version/vscpworks-version.js");

  /*!
    Add connection
    @param conn JSON connection object
    @param bSave Save connections if set to true
  */
  bool addConnection(json& conn, bool bSave = false);

  /*!
    Remove connection
    @param uuid Id for connection
    @param bSave Save connections if set to true
  */
  bool removeConnection(const QString& uuid, bool bSave = false);

  /*!
    Load configuration settings from disk
  */
  bool loadSettings(void);

  /*!
    Save configuration settings to disk
  */
  void writeSettings(void);

  /*!
    Save connections to disk
  */
  // void writeSettings(void);

  /*!
    Check the remote event information at
    https://vscp.org/events for the file
    version.json which hold the release
    version for the files. A new version
    of the db should be downloaded if this
    date is newer then stored.

    @return True if current rate is up to date, if not
        false is returned and a new version of the
        database should be downloaded.
  */
  // bool checkRemoteEventDbVersion(void);

  /*!
    Loading data from the VSCP Event database into memory
    @return true on success
  */
  bool loadEventDb(void);

  /*!
    Loading data from the vscpworks database GUID table into memory
    @return true on success
  */
  bool loadGuidTable(void);

  /*!
    Loading data from the vscpworks database sensorindex table into memory
    @return true on success
  */
  bool loadSensorTable(void);

  /*!
    Add GUID with symbolic name
    @param name GUID symbolic name of GUID to add
    @param guid GUID to add
    @return True on success, false on failure
  */
  bool addGuid(QString name, QString guid);

  /*!
    Get index for GUID record
    @param guid GUID on string form
    @return index or -1 if error.
  */
  int getIdxForGuidRecord(const QString& guid);

  /*!
    Convert integer number to selected base.
    The resulting string  representation of the number have
    - No prefix if decimal
    - "0x" prefix if hexadecimal
    - "0o" prefix if octal
    - "0b" prefix if binary
    @param value Integer that should be converted to a number
    in the current base.
    @param tobase If set to -1 (default) the current base is used for
        base, otherwise the set base will be used.
    @return String representing number with prepended base  prefix.
  */
  QString decimalToStringInBase(uint32_t value, int tobase = -1);

  /*!
    Convert integer number to selected base
    The resulting string  representation of the number have
    - No prefix if decimal
    - "0x" prefix if hexadecimal
    - "0o" prefix if octal
    - "0b" prefix if binary
    @param strvalue Integer on string form whish should be converted to
    a number in the current base.
    @param tobase If set to -1 (default) the current base is used for
        base, otherwise the set base will be used.
    @return String representing number with prepended base prefix.
  */
  QString decimalToStringInBase(const QString& strvalue, int tobase = -1);

  /*!
    Get connection name
    @param type Connection code
    @return String with connection descriptive name
  */
  QString getConnectionName(CVscpClient::connType type);

  /*!
    Create and open the VSCP Works database with tables and structure
  */
  bool openVscpWorksDatabase(void);

  /*!
    Get short token from class/type. That is a token on the form
    "TEMPERATURE" instead of "CLASS1_TYPE_MEASUREMENT_TEMPERATURE"
    @param vscpClass A valid VSCP class token
    @param vscpType A valid VSCP type token for the class.
    @return A string containing the token
  */
  QString getShortTypeToken(uint16_t vscpClass, uint16_t vscpType);

  /*!
    Get URL for specification page that have info about this class
    @param vscpClass VSCP class to get help URL for
    @return URL to help page
  */
  QString getHelpUrlForClass(uint16_t vscpClass);

  /*!
    Get URL for specification page section that have info about this class/type
    @param vscpClass VSCP class to get help URL for
    @param vscpType VSCP type to get help URL for
    @return URL to help page
  */
  QString getHelpUrlForType(uint16_t vscpClass, uint16_t vscpType);

  /*!
    Get unit information info from database for a specific event
    @param vscpClass VSCP Class to lookup unit for
    @param vscpClass VSCP TYpe to lookup unit for
    @param unit The VSCP specific unit code to look up
    @return Returns a CVscpUnit with unit data filled in. Defaults to zero.
  */
  CVscpUnit getUnitInfo(uint16_t vscpClass, uint16_t vscpType, uint8_t unit = 0);

  /*!
    Replace mustache special character variable data with
    proper values
    @param str String that should have tags replaces
    @return Handled string
  */
  std::string replaceVscpRenderVariables(const std::string& str);

  /*!
      Get VSCP data conversion function definitions and there names
      @param map A map that will get name/func pairs
      @param strVariable String that holds variable definitions
      @return True on success, false on failure
  */
  bool getVscpRenderFunctions(std::map<std::string, std::string>& map,
                              std::string& strVariables);

  /*!
    Fill the render template with data from a supplied map.
    The map holds pairs like

        lbl-start,    '<b>'
        lbl-end,      '</b>'
        val-start,    '<span style="color:rgb(0, 0, 153):">'
        val-end,      '</span>'
        newline,      '<br>'
        crc8,         '12345'
        ....

    @param map A map holding variables and value pairs
    @param strTemplate This is the template to use
    @return A string generated from the supplied template and variables
  */
  std::string renderVscpDataTemplate(std::map<std::string, std::string>& map,
                                     std::string& strTemplate);

  /*!
    Get render variables and template for a class/type pair
    @param vscpClass The VSCP class to get variables/template for
    @param vscpType The VSCP type to get variables/template for
    @param type Environment to get variables/template for, Default is "vscpworks"
    @return A string list with the variables string at pos 0 and
            the template in pos 1
  */
  QStringList getVscpRenderData(uint16_t vscpClass,
                                uint16_t vscpType,
                                QString type = "vscpworks");

  /*!
      Define global JavaScript object of a VSCP event for further
      processing by other evaluations
      @param engine Active Qt JavaScript engine
      @param pev VSCP Event to fill in
      @return True on success, false on failure
  */
  bool addVscpEventToJsRenderFunction(QJSEngine& engine, vscpEvent* pev);

  /*!
    A client adds itself to the list of child windows
    so the main window can delete the if needed.
  */
  void newChildWindow(QMainWindow* pwnd);

  /*!
    A client removes itself from the list of child windows when
    it destroys itself.
  */
  void clearChildWindow(QMainWindow* pwnd);

  /*!
    Download file from URL
    @param url URL to download from
    @param tempFileName Temporary file name to use
    @return CURLE_OK on success
  */
  CURLcode
  downLoadFromURL(const std::string& url, const std::string& tempFileName);

  /*!
    Download MDF
    @param url URL to download from
    @param tempFileName Temporary file name to use
    @return VSCP_ERROR_SUCCESS if all is OK and path to downloaded file is
      is in pathMDF
  */
  int downloadMDF(CStandardRegisters& stdregs,
                  CMDF& mdf,
                  QString& path,
                  std::function<void(int, const char*)> statusCallback = nullptr);

  // ========================================================================
  // ========================================================================

  // ------------------------------------------------------------------------
  // Global Configuration information below
  //   This info is read from a configuration file
  //   at startup and saved on close. The configuration
  //   file should be placed in the home folder.
  // ------------------------------------------------------------------------

  // ------------------------------------------------------------------------

  /// Folder used for configuration
  /// Linux: ~/.configure/VSCP/(vscpworks+.json)
  QString m_configFile;

  /// Folder for writeable data
  /// Linux: ~/.local/share/vscp/vscpworks+
  QString m_shareFolder;

  // Folder used for VSCP files like db's
  // Linux:
  // vscp/drivers/level1 - contain level one drivers
  // vscp/drivers/level2 - contain level two drivers
  // Windows:
  // c:/program data/vscp/drivers/level1
  // c:/program data/vscp/drivers/level2
  QString m_vscpHomeFolder;

  // ---------------------------------------------------

  /// Enable darktheme
  bool m_bEnableDarkTheme;

  /// Numerical base for all numericals in system
  numerical_base m_base;

  /*!
    Prefered language to use for MDF information
  */
  std::string m_preferredLanguage;

  /*!
      If true (default) ask before deleting or
      clearing data
  */
  bool m_bAskBeforeDelete;

  /*!
    If set to true will not ask for save format
  */
  bool m_bSaveAlwaysJSON;

  //**************************************************************************
  //                            LOGGER (SPDLOG)
  //**************************************************************************

  bool m_bEnableFileLog;
  spdlog::level::level_enum m_fileLogLevel;
  std::string m_fileLogPattern;
  std::string m_fileLogPath;
  uint32_t m_maxFileLogSize;
  uint16_t m_maxFileLogFiles;

  bool m_bEnableConsoleLog;
  spdlog::level::level_enum m_consoleLogLevel;
  std::string m_consoleLogPattern;

  // ------------------------------------------------------------------------
  // Session
  // ------------------------------------------------------------------------

  /// Response timeout used for session window
  uint32_t m_session_timeout;

  /*!
      Maximum number of events in a session receive list
      -1 is no limit (default)
  */
  int m_session_maxEvents;

  /// Autoconnect if true when new session window is opened
  bool m_session_bAutoConnect;

  /// Show the full token for VSCP types
  bool m_session_bShowFullTypeToken;

  /// TX Rows autosaved/autoloaded on session start/end
  bool m_session_bAutoSaveTxRows;

  /*!
      VSCP Class display format in receive list
  */
  CFrmSession::classDisplayFormat m_session_ClassDisplayFormat;

  /*!
      VSCP Type display format in receive list
  */
  CFrmSession::typeDisplayFormat m_session_TypeDisplayFormat;

  /*!
      VSCP GUID display format in receive list
  */
  CFrmSession::guidDisplayFormat m_session_GuidDisplayFormat;

  // ------------------------------------------------------------------------
  // Configuration
  // ------------------------------------------------------------------------

  /// Base for configuration values
  numerical_base m_config_base;

  /// Disable colors in configuration
  bool m_config_bDisableColors;

  /// Response timeout used for configuration window
  uint32_t m_config_timeout;

  // ------------------------------------------------------------------------

  /*!
    if true device code on device must be the same as for firmware device code
    from MDF
  */

  bool m_firmware_devicecode_required;

  // ------------------------------------------------------------------------

  /// URL for event database
  QUrl m_eventDbUrl;

  /*!
      This is the date and time  for the last event
      database download
  */
  // QDateTime m_lastEventUrlDownLoad;

  /*!
      Latest VSCP event download
  */
  QDateTime m_lastEventDbLoadDateTime;

  /*!
      Latest VSCP events on server
  */
  QDateTime m_lastEventDbServerDateTime;

  // --------------------------------------------------

  /*!
    Version control for VSCP event database
  */
  FileDownloader* m_pVersionEventDbCtrl;

  /// List with defined connections uuid,conf-obj
  QMap<std::string, json> m_mapConn;

  /// Mutex protecting vscpClass/vscpType maps
  QMutex m_mutexVscpEventsMaps;

  /// VSCP classes (class-id) -> token
  std::map<uint16_t, QString> m_mapVscpClassToToken;

  /// VSCP (class-id + token-id) -> token
  std::map<uint32_t, QString> m_mapVscpTypeToToken;

  /// Mutex protecting GUID maps
  QMutex m_mutexGuidMap;

  /// VSCP GUID to symbolic GUID name
  std::map<QString, QString> m_mapGuidToSymbolicName;

  /// VSCP GUID discovery guid/date + client-info (discoverer)
  std::map<QString, QString> m_mapGuidToDiscovery;

  /// Mutex protecting Sensor Index maps
  QMutex m_mutexSensorIndexMap;

  /*!
      Sensor index to symbolic sensor name
      (link_to_guid << 8) + sensor
  */
  std::map<int, QString> m_mapSensorIndexToSymbolicName;

  /// VSCP works database
  // QSqlDatabase m_worksdb;
  sqlite3* m_db_vscp_works;

  /// VSCP event database
  sqlite3* m_db_vscp_classtype;

  // List with open childwindows
  std::list<QMainWindow*> m_childWindows;

  // ------------------------------------------------------------------------
  //                    Windows geometry
  // ------------------------------------------------------------------------
  QRect m_mainWindowRect;
};

#endif
