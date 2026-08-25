#pragma once

#include <QDBusAbstractAdaptor>
#include <QMainWindow>
#include "MainWindow.h"

class MainWindowAdaptor : public QDBusAbstractAdaptor {
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "org.miryugaming.RepoManager")
public:
    explicit MainWindowAdaptor(MainWindow *w)
        : QDBusAbstractAdaptor(w), m_window(w) {}

public Q_SLOTS:
    void showWindow() {
        m_window->show();
        m_window->raise();
        m_window->activateWindow();
        m_window->setWindowState(
            (m_window->windowState() & ~Qt::WindowMinimized) | Qt::WindowActive);
    }

private:
    MainWindow *m_window;
};
