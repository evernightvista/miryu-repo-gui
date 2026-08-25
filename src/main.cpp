#include <QApplication>
#include <KLocalizedString>
#include <QIcon>
#include <QDBusConnection>
#include "MainWindow.h"
#include "DBusAdaptor.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    KLocalizedString::setApplicationDomain("miryu-repo-gui");
    app.setApplicationName("miryu-repo-gui");
    app.setOrganizationDomain("miryu");

    QIcon icon = QIcon::fromTheme("gpk-repo");
    if (icon.isNull()) {
        icon = QIcon::fromTheme("system-software-install");
    }
    app.setWindowIcon(icon);

    MainWindow w;

    MainWindowAdaptor adaptor(&w);
    QDBusConnection bus = QDBusConnection::sessionBus();
    bus.registerObject("/MainWindow", &w);
    bus.registerService("org.miryugaming.RepoManager");

    w.show();
    return app.exec();
}