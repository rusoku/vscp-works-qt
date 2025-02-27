// cfrmnodeconfig.cpp
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
#ifndef CFRMNODECONFIG_H
#define CFRMNODECONFIG_H

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include "ctxevent.h"
#include <mdf.h>
#include <register.h>
#include <vscp.h>
#include <vscp-client-base.h>

#include <QDialog>
#include <QObject>
#include <QSpinBox>
#include <QTableView>
#include <QTableWidgetItem>
#include <QTreeWidget>
#include <QFile>

QT_BEGIN_NAMESPACE
class QAction;
class QIcon;
class QDialogButtonBox;
class QGroupBox;
class QLabel;
class QLineEdit;
class QSpinBox;
class QMenu;
class QMenuBar;
class QPushButton;
class QCheckBox;
class QTextEdit;
class QTextBrowser;
class QToolBar;
class QVBoxLayout;
class QAction;
class QTableWidgetItem;
class QTableWidget;
class QToolBox;
class QSpinBox;
class QComboBox;
class QTreeWidgetItem;
class QShortcut;
QT_END_NAMESPACE

#include <QMainWindow>
#include <QTableWidgetItem>

namespace Ui {
class CFrmNodeConfig;
}

#define NUMBER_OF_TABS 4

enum tabbarindex {
  TAB_BAR_INDEX_REGISTERS = 0,
  TAB_BAR_INDEX_REMOTEVARS,
  TAB_BAR_INDEX_DM,
  TAB_BAR_INDEX_FILES
};

enum registerColumns {
  REG_COL_POS = 0,
  REG_COL_ACCESS,
  REG_COL_VALUE,
  REG_COL_NAME
};

enum remotevarColumns {
  REMOTEVAR_COL_VALUE = 0,
  REMOTEVAR_COL_ACCESS,
  REMOTEVAR_COL_TYPE,
  REMOTEVAR_COL_NAME
};

enum dmColumns {
  DM_LEVEL1_COL_ORIGIN = 0,
  DM_LEVEL1_COL_FLAGS,
  DM_LEVEL1_COL_CLASS_MASK,
  DM_LEVEL1_COL_CLASS_FILTER,
  DM_LEVEL1_COL_TYPE_MASK,
  DM_LEVEL1_COL_TYPE_FILTER,
  DM_LEVEL1_COL_ACTION,
  DM_LEVEL1_COL_PARAMETER
};

enum filesColumns {
  FILE_COL_NAME = 0
};

#define TREE_LIST_REGISTER_TYPE  (QTreeWidgetItem::UserType + 1)
#define TREE_LIST_REMOTEVAR_TYPE (QTreeWidgetItem::UserType + 2)
#define TREE_LIST_DM_TYPE        (QTreeWidgetItem::UserType + 3)
#define TREE_LIST_MDF_FILE_TYPE  (QTreeWidgetItem::UserType + 4)

// ----------------------------------------------------------------------------

/*!
    Class that represent a row in the register list
*/
class CRegisterWidgetItem : public QTreeWidgetItem {

public:
  CRegisterWidgetItem(const QString& text);
  virtual ~CRegisterWidgetItem();

  /*!
    Register page
  */
  uint16_t m_regPage;

  /*!
    Register offset
  */
  uint32_t m_regOffset;
};

// ----------------------------------------------------------------------------

/*!
    Class that represent a row in the remote variable list
*/
class CRemoteVariableWidgetItem : public QTreeWidgetItem {

public:
  CRemoteVariableWidgetItem(const QString& text);
  virtual ~CRemoteVariableWidgetItem();

  /*!
    Holds a pointer to the remote variable
    MDF info
  */
  CMDF_RemoteVariable* m_pRemoteVariable;
};

// ----------------------------------------------------------------------------

/*!
    Class that represent a row in the DM list
*/
class CDmWidgetItem : public QTreeWidgetItem {

public:
  CDmWidgetItem(const QString& text);
  virtual ~CDmWidgetItem();

  /*!
    Holds index for the DM row
  */
  uint16_t m_row;

  /*!
    Pointer to DM MDF item
  */
  CMDF_DecisionMatrix* m_pDM;
};

// ----------------------------------------------------------------------------

/*!
    Class that represent a row in the MDF file list
*/
class CMdfFileWidgetItem : public QTreeWidgetItem {

public:
  CMdfFileWidgetItem(const QString& text, int type = 0);
  virtual ~CMdfFileWidgetItem();

  static const int ITEM_OFFSET = 1000;

  /*!
    Holds index for the DM row
  */
  mdf_file_type m_mdfFileType;
  /*!
    Pointer to DM MDF item
  */
  CMDF_Picture* m_picture_obj;   // Picture file(s)
  CMDF_Video* m_video_obj;       // Video file(s)
  CMDF_Firmware* m_firmware_obj; // Firmware file(s)
  CMDF_Driver* m_driver_obj;     // Picture file(s)
  CMDF_Manual* m_manual_obj;     // Manual file(s)
  CMDF_Setup* m_setup_obj;       // Setup file(s)
};

// ----------------------------------------------------------------------------

/*!
    The session window
*/

class CFrmNodeConfig : public QMainWindow {
  Q_OBJECT

public:
  explicit CFrmNodeConfig(QWidget* parent = nullptr, json* pconnObj = nullptr);
  virtual ~CFrmNodeConfig();

  /*!
      set Initial focus
  */
  void setInitialFocus(void);

  /*!
      This method is a middle man between the communication
      callback and the session class. It add a VSCP event to the rc table
      and to the receive event table.
      @param pev Pointer to received event
  */
  //void threadReceive(vscpEvent* pev);

  /*!
  This is the callback used by client thread to deliver events
  @param ev Reference to VSCP event
  @param pobj Pointer to object CFrmSession)
*/

  void receiveCallback(vscpEvent& ev, void* pobj);

  /*!
      Connect to remote host
  */
  void doConnectToRemoteHost(void);

  /*!
      Disconnect from remote host and update UI to
      indicate this
  */
  void doDisconnectFromRemoteHost(void);

  /*!
    Do the actual update
    @param mdfpath Path to local MDF file. If empty then
                    the MDF file is read from the remote device.
    @return VSCP_ERROR_SUCCESS on success or error code on failure.
  */
  int doUpdate(std::string mdfpath);

  /*!
    Set the selected node id
    @param nodeid Value to set
  */
  void setNodeId(uint16_t nodeid)
  {
    if (nullptr != m_nodeidConfig)
      m_nodeidConfig->setValue(nodeid);
  };

public slots:

  /// Dialog return
  void done(int r);

  /*!
      Add a RX event to the receive list
      @param ev Event to add
      @param bReceive Set to true if this is a received event
  */
  void
  receiveRxRow(vscpEvent* pev);

  /*!
      Connect to remote host and update UI to
      indicate the result of the operation.
      @param checked false if not connected. True if connected.
  */
  void
  connectToRemoteHost(bool checked);

  /*!
      Show context menu for register table
      @param pos Position where right click took place
  */
  void
  showRegisterContextMenu(const QPoint& pos);

  /*!
      Show context menu for remote variable table
      @param pos Position where right click took place
  */
  void
  showRemoteVariableContextMenu(const QPoint& pos);

  /*!
      Show context menu for DM table
      @param pos Position where right click took place
  */
  void
  showDMContextMenu(const QPoint& pos);

  /*!
      Show context menu for files table
      @param pos Position where right click took place
  */
  void
  showMdfFilesContextMenu(const QPoint& pos);

  /// Open settings dialog
  void
  menu_open_main_settings(void);

  /*!
    Let user select a GUID to use
  */
  void selectGuid(void); // std::string strguid = "-"

  /*!
    ui checkbox for full Level II has been clicked. Assign
    value to
  */
  void checkFullLevel2(void);

  /*!
    Disable/enable MDF colors
  */
  void disableColors(bool bColors);

  /*!
    Base combo chaged value
    Update data
    @param index Index of the new value
  */
  void onBaseChange(int index);

  /*!
    A new interface as been selected
    @param index Index for new interface
  */
  void onInterfaceChange(int index);

  /*!
    A new nodeid as been selected
    @param nodeid New node id
  */
  void onNodeIdChange(int nodeid);

  /*!
    Single click on register line. Show help text about register
    @param item Widget item clicked.
    @param column Column clicked
  */
  void onRegisterTreeWidgetItemClicked(QTreeWidgetItem* item, int column);

  /*!
    Double click on register line. This juste select the row for all columns except
    the value column. Double clicking on this column edit's the value.
    @param item Widget item clicked.
    @param column Column clicked
  */
  void onRegisterTreeWidgetItemDoubleClicked(QTreeWidgetItem* item, int column);

  /*!
    Single click on remote variable line. Show help text about remote variable
    @param item Widget item clicked.
    @param column Column clicked
    @param item Widget item clicked.
    @param column Column clicked
  */
  void onRemoteVariableTreeWidgetItemClicked(QTreeWidgetItem* item, int column);

  /*!
    Double click on remote variable line. This juste select the row for all columns except
    the value column. Double clicking on this column edit's the value.
    @param item Widget item clicked.
    @param column Column clicked
  */
  void onRemoteVariableTreeWidgetItemDoubleClicked(QTreeWidgetItem* item, int column);

  /*!
    Single click on DM line. Show help text about remote variable
    @param item Widget item clicked.
    @param column Column clicked
    @param item Widget item clicked.
    @param column Column clicked
  */
  void onDMTreeWidgetItemClicked(QTreeWidgetItem* item, int column);

  /*!
    Double click on DM line. This juste select the row for all columns except
    the value column. Double clicking on this column edit's the value.
    @param item Widget item clicked.
    @param column Column clicked
  */
  void onDMTreeWidgetItemDoubleClicked(QTreeWidgetItem* item, int column);

  /*!
    Register value changed. If user edits the register value and it is changed
    then this method is called.
    @param item Widget item clicked.
    @param column Column clicked
  */
  void onRegisterTreeWidgetCellChanged(QTreeWidgetItem* item, int column);

  /*!
    Remote variable value changed. If user edits the remote variable value and it is changed
    then this method is called.
    @param item Widget item clicked.
    @param column Column clicked
  */
  void onRemoteVarTreeWidgetCellChanged(QTreeWidgetItem* item, int column);

  /*!
    DM value changed. If use edits the DM value and it is changed
    then this method is called.
    @param item Widget item clicked.
    @param column Column clicked
  */
  void onDMTreeWidgetCellChanged(QTreeWidgetItem* item, int column);

  /*!
    Fill standard register data for already
    loaded registers
  */
  bool renderStandardRegisters(void);

  /*!
    Fill register data from already loaded registers
  */
  int renderRegisters(void);

  /*!
    Fill remote variable data from already loaded MDF data
  */
  bool renderRemoteVariables(void);

  /*!
    Fill decsin matrix info from already loaded MDF data
  */
  bool renderDecisionMatrix(void);

  /*!
    Fill file data from already loaded MDF data
  */
  bool renderMdfFiles(void);

  /*!
    go to register for selected remote variable
  */
  void gotoRemoteVarRegisterPos(void);

  /*!
    Write all changed registers to the device
    @return VSCP_ERROR_SUCCESS on success, error code on failure.
  */
  int writeChanges(void);

  /*!
    Update data
  */
  void update(void);

  /*!
    Update all registers and read MDF again
  */
  void updateFull(void);

  /*!
    Update all registers and use local MDF
  */
  void updateLocal(void);

  /*!
    Read selected registers
  */
  void readSelectedRegisterValues(void);

  /*!
    Write selected registers
  */
  void writeSelectedRegisterValues(void);

  /*!
    Set selected registers to default value
  */
  void defaultSelectedRegisterValues(void);

  /*!
    Set all registers to default values
  */
  void defaultRegisterAll(void);

  /*!
    Undo selected register values
  */
  void undoSelectedRegisterValues(void);

  /*!
    Redo selected register values
  */
  void redoSelectedRegisterValues(void);

  /*!
    Show menu to select register page
    @param page Page to go to. Set to -1 to open choice dialog.
  */
  void gotoRegisterPage(int page = -1);

  /*!
    Got register on page
    @param page Page to go to. Set to -1 to open choice dialog.
    @param reg Register to go to. Set to -1 to open choice dialog.
  */
  void gotoRegisterOnPage(int page = -1, int reg = -1);

  /*!
    Go to DM or remote variable register for selected item
  */
  void gotoRegisterForSelected(void);

  /*!
    Goto register page 0
  */
  void gotoRegisterPage0(void);

  /*!
    Go to register page 1
  */
  void gotoRegisterPage1(void);

  /*!
    Go to register page 2
  */
  void gotoRegisterPage2(void);

  /*!
    Go to register page 3
  */
  void gotoRegisterPage3(void);

  /*!
    Go to register page 4
  */
  void gotoRegisterPage4(void);

  /*!
    Go to register page 5
  */
  void gotoRegisterPage5(void);

  /*!
    Go to register page 6
  */
  void gotoRegisterPage6(void);

  /*!
    Go to register page 7
  */
  void gotoRegisterPage7(void);

  /*!
    Go to register page 8
  */
  void gotoRegisterPage8(void);

  /*!
    Go to register page 9
  */
  void gotoRegisterPage9(void);

  /*!
    Goto standard register page
  */
  void gotoRegisterPageStdReg(void);

  /*!
    Collapse all register items
  */
  void collapseAllRegisterTopItems(void);

  /*!
    Collapse all register items
  */
  void collapseAllFileTopItems(void);

  /*!
    Goto page and pos for DM
  */
  void gotoRegisterPageDM(int row = 0);

  /*!
    Go to register position for selected DM row
  */
  void gotoDMRegisterPos(void);

  /*!
    Save register record
    @param file Open file to save to
    @param itemReg Pointer to register treewidgetitem item
    @param bJSON true to write record as a JSON record (deafult) and
            false to write record as a XML record
  */
  void writeRegisterRecord(QFile& file, const CRegisterWidgetItem* itemReg, bool bJSON = true);

  /*!
    Save selected register values
  */
  void saveSelectedRegisterValues(void);

  /*!
    Save selected register values
  */
  void saveAllRegisterValues(void);

  /*!
    Save all or selected register values to disk.
    Format is JSON or XML as set in settings.
    @param bJSON True if save format should be JSON, else XML.
    @param bAll If true save all registers, else save selected registers
  */
  void saveRegisterValues(bool bJSON = true, bool bAll = false);

  /*!
    Load registers from XML file
    @param path Filename to load from
    @return VSCP_ERROR_SUCCESS on success, error code on failure.
  */
  int loadXMLRegs(const std::string& path);

  /*!
    Load registers from JSON file
    @param path Filename to load from
    @return VSCP_ERROR_SUCCESS on success, error code on failure.
  */
  int loadJSONRegs(const std::string& path);

  /*!
    Save selected register values
  */
  void loadRegisterValues(void);

  /*!
    Load MDF default
  */
  void loadDefaults(void);

  /*!
    Load MDF default to selected items
  */
  void loadDefaultsSelected(void);

  /*!
    Fill register HTML help info
  */
  void fillRegisterHtmlInfo(QTreeWidgetItem* item, int column);

  /*!
    Fill remote variable HTML help info
  */
  void fillRemoteVariableHtmlInfo(QTreeWidgetItem* item, int column);

  /*!
    Fill DM HTML help info
  */
  void fillDMHtmlInfo(QTreeWidgetItem* item, int column);

  /*!
    Fill HTML area with MDF information from
    the selected device
  */
  void fillDeviceHtmlInfo(void);

  /*!
    Either heading or an item has been clicked in the MDF file
    tree
    @param item Widget item clicked.
    @param column Column clicked
  */
  void onMdfFileTreeWidgetItemClicked(QTreeWidgetItem* item, int column);

  /*!
    Either heading or an item has been double clicked in the MDF file
    tree
    @param item Widget item clicked.
    @param column Column clicked
  */
  void onMdfFileTreeWidgetItemDoubleClicked(QTreeWidgetItem* item, int column);

  /*!
    Double click on MDF file line. Fill info in info area
    @param item Widget item clicked.
    @param column Column clicked
  */
  void fillMdfFileHtmlInfo(QTreeWidgetItem* item, int column);

  /*!
    Open edit DM row dialog
  */
  void editDMRow(void);

  /*!
    Toggle active state for DM row dialog
  */
  void toggleDMRow(void);

  /*!
    Read selected DM row from device
  */
  void readSelectedDMRow(void);

  /*!
    Write selected DM row to device
  */
  void writeSelectedDMRow(void);

  /*!
    Go trough all registers and mark changed
  */
  void updateVisualRegisters(void);

  /*!
    Go trough all remote variables and mark changed
  */
  void updateVisualRemoteVariables(void);

  /*!
    Go trough DM rows and mark changed
  */
  void updateVisualDM(void);

  /*!
    Update remote variable listing
    @param offset Offset for register to update
    @param page Page for register to update
    @param bFromRegUpdate True if called from register update
  */
  void updateChangeRemoteVariable(uint32_t offset, uint16_t page, bool bFromRegUpdate = false);

  /*!
    Update DM listing
    @param offset Offset for register to update
    @param page Page for register to update
    @param bFromRegUpdate True if called from register update
  */
  void updateChangeDM(uint32_t offset, uint16_t page, bool bFromRegUpdate = false);

  /*!
    A main tab has been clicked
  */
  void onMainTabBarClicked(int index);

  /*!
    The main tab bar has changed
  */
  void onMainTabBarChanged(int index);

  /*!
    Search register with text content
  */
  void onSearchRegister(void);

  /*!
    Search next register
  */
  void onSearchNextRegister(void);

  /*!
    Select registers from search result
  */
  void onSelectSearchRegisters(void);

  /*!
    Search remote variable with text content
  */
  void onSearchRemoteVariable(void);

  /*!
    Search remote variable with text content
  */
  void onSearchNextRemoteVariable(void);

  /*!
    Select registers from search result
  */
  void onSelectSearchRemoteVars(void);

  /*!
    Show help
  */
  void showHelp(void);

signals:

  /// Data received from callback
  void dataReceived(vscpEvent* pev);

  

private:
  /// True if full Level II handling
  bool m_bFullLevel2;

  /// MDF definitions
  CMDF m_mdf;

  /// VSCP standard registers
  CStandardRegisters m_stdregs;

  /// VSCP device user registers
  CUserRegisters m_userregs;

  /// Number of updates. Cleared after a full update
  uint32_t m_nUpdates;

  // Points to top widget  item for standar registers
  QTreeWidgetItem* m_StandardRegTopPage;

  // Holds widget items for register page headers
  // page -> register item header
  std::map<uint16_t, QTreeWidgetItem*> m_mapPageToPageHeader;

  /// The VSCP client type
  CVscpClient::connType m_vscpConnType;

  /// Configuration data for the session
  json m_connObject;

  /*!
    Can be set to true when the system updates a register cell. Prevents
    value cells cell change events from doing stuff
  */
  bool m_bInternalChange;

  /*!
    True when the main HTML info has been written,
    false if specific info written to the info area.
  */
  bool m_bMainInfo;

  /*!
    Save positions for the info area for each tab
  */
  std::string m_saveInfoArea[4];

  /*!
    Shortcut for ctrl + i - Show MDF info in info area
  */
  // QShortcut *m_shortcut_info;

  /*!
    Maps registers to remote variables
  */
  std::map<uint32_t, CRemoteVariableWidgetItem*> m_mapReg2RemoteVariable;

  /*!
    Maps registers to DM
  */
  // std::map<uint32_t, CDMWidgetItem *> m_mapReg2DM;

  /*!
    List that get populated after a register search
  */
  QList<QTreeWidgetItem*> m_searchListRegs;

  /*!
    Current pos shown among register search results
  */
  uint32_t m_registerSearchPos;

  /*!
    List that get populated after a remote variable search
  */
  QList<QTreeWidgetItem*> m_searchListRemoteVars;

  /*!
    Current pos shown among remote variable search results
  */
  uint32_t m_remoteVarSearchPos;

  /// A pointer to a VSCP Client
  CVscpClient* m_vscpClient;

  // The UI definition
  Ui::CFrmNodeConfig* ui;

  /// Combo box for numerical base
  QComboBox* m_baseComboBox;

  /// Text box for configuration GUID
  QLineEdit* m_guidConfig;

  /// Button to open GUID selections dialog
  QPushButton* m_btnSetGUID;

  // Button to mark handling to be full Level II
  QCheckBox* m_btnFullLevel2;

  /// Spin box for configuration nodeid
  QSpinBox* m_nodeidConfig;

  // Contains interface information
  QComboBox* m_comboInterface;
};

#endif // CFrmNodeConfig_H
