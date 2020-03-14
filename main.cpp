/*
   DGP - Deterministically Generated Passwords
   Filename: main.cpp
   Description: Main entry point for the QT version of DGP
   Author: Adam Oldham (ekcdd@gmx.com)

   Copyright (c) 2020  Adam Oldham
   All rights reserved.

   Distributed under the MIT/X11 software license, see the accompanying
   file license.txt or http://www.opensource.org/licenses/mit-license.php.

*/

#include "mainwindow.h"

#include <QApplication>
#include <QStyleFactory>

#include "dgp.h"

int main(int argc, char *argv[])
{
    QApplication::setStyle(QStyleFactory::create("fusion"));
    QGuiApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
