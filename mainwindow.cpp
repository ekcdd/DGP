/*
   DGP - Deterministically Generated Passwords
   Filename: mainwindow.cpp
   Description: The main windows, this handles all functions that are
                related to it.

   Author: Adam Oldham (ekcdd@gmx.com)

   Copyright (c) 2020  Adam Oldham
   All rights reserved.

   Distributed under the MIT/X11 software license, see the accompanying
   file license.txt or http://www.opensource.org/licenses/mit-license.php.

*/

#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include <QRegExpValidator>
#include <QLineEdit>
#include <QMessageBox>



MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowFlags( Qt::WindowTitleHint |  Qt::WindowMinimizeButtonHint | Qt::WindowSystemMenuHint);
    setFixedSize(width(), height());
    setMaximumSize(width(),height());
    QRegExp rx("[a-z\\.\\- 0-9]{0,30}");
    QValidator *v = new QRegExpValidator(rx, this);
    ui->NameEdit->setValidator(v);
    connect(ui->pushButton, SIGNAL (clicked()), this, SLOT (handleButton()));
    connect(ui->GenerateButton, SIGNAL (clicked()), this, SLOT (handleGenerate()));
    connect(ui->CloseButton, SIGNAL (clicked()), this, SLOT (CloseMe()));
    connect(ui->MasterPassEdit, &QLineEdit::textChanged, this, &MainWindow::textCheck);
    connect(ui->NameEdit, &QLineEdit::textChanged, this, &MainWindow::textCheck);
    //options = new DGPOptions;
    for (auto const& md : VersionMap) {
        ui->AlgoCombo->addItem(QString::fromStdString(md.second));
        std::cout << md.second << " value: " << md.first << std::endl;
    }
    auto it = VersionMap.find(static_cast<unsigned int>(DefaultDGPVersion));
    std::string def;
    int index;
    if (it != VersionMap.end())
    {
        std::string def = it->second;
        index = ui->AlgoCombo->findText(QString::fromStdString(def));
        if (index >= 0)
        {
            ui->AlgoCombo->setCurrentIndex(index);
        }
    }
}

MainWindow::~MainWindow()
{
    delete ui;
   // delete options;
}
void MainWindow::CloseMe()
{
    this->close();
}
void MainWindow::handleGenerate()
{
    options.MasterKey = ui->MasterPassEdit->text().toStdString();
    options.MasterKeyLength = static_cast<uint32_t>(options.MasterKey.size());
    options.Name = ui->NameEdit->text().toStdString();
    options.NameLength = static_cast<uint32_t>(options.Name.size());
    options.Length = static_cast<uint32_t>(ui->PassLength->value());
    options.Rounds = static_cast<uint32_t>(ui->Rounds->value());
    options.AlphaNumericOnly = ui->AlphaCharsCheck->isChecked();
    std::string algoVer = ui->AlgoCombo->currentText().toStdString();
    bool algoFound = false;
    DGPVersion ver = DefaultDGPVersion;
    for (auto const& md : VersionMap) {
       if (md.second == algoVer)
       {
           algoFound = true;
           ver = static_cast<DGPVersion>(md.first);
       }
    }
    options.Version = ver;
    std::string pass = "";
    if (!DGP(options,pass)) {

        QMessageBox msgBox;
        msgBox.setBaseSize(400,170);

        msgBox.setText("Password Generation Failed");
        msgBox.setInformativeText("Could not generate your password at this time. Make sure all options are properly set and try again.");
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.setDefaultButton(QMessageBox::Ok);
        msgBox.setIcon(QMessageBox::Icon::Critical);
        msgBox.exec();
        return;
    }
    ui->lineEdit->setText(QString::fromStdString(pass));

}
void MainWindow::handleButton()
{
  if (ui->pushButton->isChecked())
  {
      ui->MasterPassEdit->setEchoMode(QLineEdit::EchoMode::Normal);
  }
  else {
      ui->MasterPassEdit->setEchoMode(QLineEdit::EchoMode::Password);
  }
}
void MainWindow::textCheck()
{
    ui->GenerateButton->setEnabled(
                ui->MasterPassEdit->text().length() > 0 &&
                ui->NameEdit->text().length() > 0
          );
}
