// cdlgeditguid.cpp
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

#ifdef WIN32
#include <pch.h>
#endif

#include <vscp.h>
#include <vscphelper.h>

#include <vscpworks.h>

#include "cdlgeditguid.h"
#include "ui_cdlgeditguid.h"

#include <QMessageBox>
#include <QDesktopServices>
#include <QShortcut>
#include <QDebug>

#include <spdlog/async.h>
#include <spdlog/sinks/rotating_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/spdlog.h>

///////////////////////////////////////////////////////////////////////////////
// CTor
//

CDlgEditGuid::CDlgEditGuid(QWidget* parent)
  : QDialog(parent)
  , ui(new Ui::CDlgEditGuid)
{
  ui->setupUi(this);

  vscpworks* pworks = (vscpworks*)QCoreApplication::instance();

  connect(ui->btnSetDummyGuid, &QPushButton::clicked, this, &CDlgEditGuid::setDummyGuid);

  // Help
  QShortcut* shortcut = new QShortcut(QKeySequence(Qt::Key_F1), this, SLOT(showHelp()));
  shortcut->setAutoRepeat(false);

  QPushButton* helpButton = ui->buttonBox->button(QDialogButtonBox::Help);
  connect(helpButton, SIGNAL(clicked()), this, SLOT(showHelp()));

  setInitialFocus();
}

///////////////////////////////////////////////////////////////////////////////
// DTor
//

CDlgEditGuid::~CDlgEditGuid()
{
  delete ui;
}

///////////////////////////////////////////////////////////////////////////////
// setInitialFocus
//

void
CDlgEditGuid::setInitialFocus(void)
{
  ui->editGuid->setFocus();
}

///////////////////////////////////////////////////////////////////////////////
// setEditMode
//

void
CDlgEditGuid::setEditMode(void)
{
  ui->editGuid->setReadOnly(true);
  ui->btnSetDummyGuid->setVisible(false);
  ui->editName->setFocus();
}

///////////////////////////////////////////////////////////////////////////////
// setDummyGuid
//

void
CDlgEditGuid::setDummyGuid(void)
{
  ui->editGuid->setText("00:00:00:00:00:00:00:00:00:00:00:00:xx:xx:xx:xx");
}

// ----------------------------------------------------------------------------
//                             Getters & Setters
// ----------------------------------------------------------------------------

///////////////////////////////////////////////////////////////////////////////
// getGuid
//

QString
CDlgEditGuid::getGuid(void)
{
  return (ui->editGuid->text());
}

///////////////////////////////////////////////////////////////////////////////
// setGuid
//

void
CDlgEditGuid::setGuid(const QString& str)
{
  ui->editGuid->setText(str);
}

///////////////////////////////////////////////////////////////////////////////
// getName
//

QString
CDlgEditGuid::getName(void)
{
  return (ui->editName->text());
}

///////////////////////////////////////////////////////////////////////////////
// setName
//

void
CDlgEditGuid::setName(const QString& str)
{
  ui->editName->setText(str);
}

///////////////////////////////////////////////////////////////////////////////
// getDescription
//

QString
CDlgEditGuid::getDescription(void)
{
#if QT_VERSION >= 0x050E00
  // return (ui->editDescription->toMarkdown());
  return (ui->editDescription->toPlainText());
#else
  return (ui->editDescription->toPlainText());
#endif
}

///////////////////////////////////////////////////////////////////////////////
// setDescription
//

void
CDlgEditGuid::setDescription(const QString& str)
{
#if QT_VERSION >= 0x050E00
  ui->editDescription->setMarkdown(str);
#else
  ui->editDescription->setText(str);
#endif
}

///////////////////////////////////////////////////////////////////////////////
// showHelp
//

void
CDlgEditGuid::showHelp(void)
{
  QString link = "https://grodansparadis.github.io/vscp-works-qt/#/settings?id=guid";
  QDesktopServices::openUrl(QUrl(link));
}