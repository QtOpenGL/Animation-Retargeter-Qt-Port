/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing/
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** You may use this file under the terms of the BSD license as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of The Qt Company Ltd nor the names of its
**     contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "mainwindow.h"
#include "window.h"
#include "glwidget.h"
#include "engine/engine.h"
#include "loaddialog.h"
#include <QMenuBar>
#include <QMenu>
#include <QMessageBox>
#include <QKeyEvent>
#include <iostream>
#include <QApplication>

MainWindow::MainWindow(QApplication *):
    runSession(true)
{
    this->aName = qApp;
    aName->installEventFilter(this);
    engine = new Engine();
    window = new Window(engine);
    glwidget = window->getGLWidget();
    QMenuBar *menuBar = new QMenuBar;
    QMenu * fileMenu = menuBar->addMenu(tr("&File"));
    QMenu * animMenu = menuBar->addMenu(tr("&Animation"));

    //File menu
    QAction * openFromMesh = new QAction(tr("&Open From Mesh"), this);
    connect(openFromMesh, SIGNAL(triggered()), this, SLOT(onOpenFromMesh()));

    QAction * openToMesh = new QAction(tr("&Open To Mesh"), this);
    connect(openToMesh, SIGNAL(triggered()), this, SLOT(onOpenToMesh()));

    QAction * openAnim = new QAction(tr("&Open Animation"), this);
    connect(openAnim, SIGNAL(triggered()), this, SLOT(onOpenAnimation()));

    QAction * exitAct = new QAction(tr("&Exit"), this);
    connect(exitAct, SIGNAL(triggered()), this, SLOT(endSession()));

    //Animation menu
    QAction * playFrom = new QAction(tr("&Play 'From'"), this);
    connect(playFrom, SIGNAL(triggered()), engine, SLOT(onPlayFrom()));

    QAction * playTo = new QAction(tr("&Play 'To'"), this);
    connect(playTo, SIGNAL(triggered()), engine, SLOT(onPlayTo()));

    QAction * bindFrom = new QAction(tr("&Bind Pose 'From'"), this);
    connect(bindFrom, SIGNAL(triggered()), engine, SLOT(onBindFrom()));

    QAction * bindTo = new QAction(tr("&Bind Pose 'To'"), this);
    connect(bindTo, SIGNAL(triggered()), engine, SLOT(onBindTo()));

    QAction * hideFrom = new QAction(tr("&Hide 'From'"), this);
    connect(hideFrom, SIGNAL(triggered()), engine, SLOT(onHideFrom()));

    QAction * hideTo = new QAction(tr("&Hide 'To'"), this);
    connect(hideTo, SIGNAL(triggered()), engine, SLOT(onHideTo()));

    fileMenu->addAction(openFromMesh);
    fileMenu->addAction(openToMesh);
    fileMenu->addAction(openAnim);
    fileMenu->addAction(exitAct);

    animMenu->addAction(playFrom);
    animMenu->addAction(playTo);
    animMenu->addAction(bindFrom);
    animMenu->addAction(bindTo);
    animMenu->addAction(hideFrom);
    animMenu->addAction(hideTo);

    setMenuBar(menuBar);
    setCentralWidget(window);

}

void MainWindow::endSession(){
    runSession = false;
}

void MainWindow::paintGL(){
    glwidget->paint();
}

bool MainWindow::isSessionRunning(){
    return window->isVisible() && runSession;
}

void MainWindow::updateEngine(){
    engine->update();
}

void MainWindow::onOpenToMesh(){
    LoadDialog *dialog = new LoadDialog(this, engine, TO);
    dialog->exec();
    window->resetRetargetWidgets();
}

void MainWindow::onOpenAnimation(){
    LoadDialog * dialog = new LoadDialog(this, engine, ANIM);
    dialog->exec();
    window->resetRetargetWidgets();
}

void MainWindow::onOpenFromMesh(){
    LoadDialog *dialog = new LoadDialog(this, engine, FROM);
    dialog->exec();
    window->resetRetargetWidgets();
}

bool MainWindow::eventFilter(QObject *object, QEvent *e)
{
 if (e->type() == QEvent::KeyPress)
  {
      QKeyEvent *keyEvent = static_cast<QKeyEvent *>(e);

      switch(keyEvent->key()){

          case Qt::Key_W:
              engine->moveCameraForward();
              break;

          case Qt::Key_S:
              engine->moveCameraBackward();
              break;

          case Qt::Key_A:
              engine->moveCameraLeft();
              break;

          case Qt::Key_D:
              engine->moveCameraRight();
              break;

          case Qt::Key_R:
              engine->moveCameraUpward();
              break;

          case Qt::Key_F:
              engine->moveCameraDownward();
              break;

          case KEY_UP:
              engine->pointCameraUpward();
              break;

          case KEY_DOWN:
              engine->pointCameraDownward();
              break;

          case KEY_LEFT:
              engine->pointCameraLeft();
              break;

          case KEY_RIGHT:
              engine->pointCameraRight();
              break;
      }

  }
 return false;
}


MainWindow::~MainWindow(){
    glwidget->cleanup();
    delete engine;
}
