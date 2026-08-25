/*
    SPDX-FileCopyrightText: 2026 Evernight Vista Team <13278297951@sina.cn>
    SPDX-License-Identifier: GPL-3.0-or-later
*/

// KDE System Settings module that provides a launcher for the
// external Miryu Repository Manager application. It behaves like the built-in
// ExternalAppModule: the external app is automatically launched when
// the KCM is first shown, and a "Relaunch" button is provided.
// If the application is already running (detected via DBus),
// the existing window is activated instead of launching a second instance.
// It is registered under the "System" section via the
// X-KDE-System-Settings-Parent-Category field in kcm_miryu_repo_gui.json.

#include <KCModule>
#include <KPluginFactory>
#include <KPluginMetaData>
#include <KLocalizedString>

#include <QFont>
#include <QIcon>
#include <QLabel>
#include <QPushButton>
#include <QProcess>
#include <QVBoxLayout>
#include <QShowEvent>
#include <QTimer>
#include <QDBusConnection>
#include <QDBusConnectionInterface>
#include <QDBusInterface>
#include <QDBusReply>

static constexpr const char *DBUS_SERVICE = "org.miryugaming.RepoManager";
static constexpr const char *DBUS_PATH = "/MainWindow";

class MiryuRepoKcm : public KCModule
{
    Q_OBJECT
public:
    MiryuRepoKcm(QObject *parent, const KPluginMetaData &data)
        : KCModule(parent, data)
    {
        setButtons(KCModule::NoAdditionalButton);

        auto *layout = new QVBoxLayout(widget());
        layout->setContentsMargins(20, 20, 20, 20);
        layout->setSpacing(12);

        auto *title = new QLabel(i18nd("miryu-repo-gui", "Miryu Repository Manager"), widget());
        QFont titleFont = title->font();
        titleFont.setPointSize(titleFont.pointSize() + 6);
        titleFont.setBold(true);
        title->setFont(titleFont);

        auto *description = new QLabel(
            i18nd("miryu-repo-gui",
                  "Manage Evernight Vista repository mirrors with speed test"),
            widget());
        description->setWordWrap(true);

        m_status = new QLabel(widget());
        m_status->setWordWrap(true);
        m_status->setStyleSheet(QStringLiteral("color: palette(text)"));

        m_launchButton = new QPushButton(
            QIcon::fromTheme(QStringLiteral("gpk-repo")),
            i18nd("miryu-repo-gui", "Relaunch Miryu Repository Manager"),
            widget());
        m_launchButton->setObjectName(QStringLiteral("launchMiryuRepoManager"));

        connect(m_launchButton, &QPushButton::clicked, this, &MiryuRepoKcm::launchRepoManager);

        layout->addWidget(title);
        layout->addWidget(description);
        layout->addStretch(1);
        layout->addWidget(m_launchButton, 0, Qt::AlignLeft);
        layout->addSpacing(8);
        layout->addWidget(m_status);
        layout->addStretch(10);

        widget()->installEventFilter(this);
    }

    bool eventFilter(QObject *watched, QEvent *event) override
    {
        if (watched == widget() && event->type() == QEvent::Show && m_firstShow) {
            m_firstShow = false;
            QTimer::singleShot(0, this, &MiryuRepoKcm::launchRepoManager);
        }
        return KCModule::eventFilter(watched, event);
    }

private Q_SLOTS:
    void launchRepoManager()
    {
        QDBusConnection bus = QDBusConnection::sessionBus();
        QDBusConnectionInterface *iface = bus.interface();

        if (iface) {
            QDBusReply<bool> registered = iface->isServiceRegistered(QLatin1String(DBUS_SERVICE));
            if (registered.isValid() && registered.value()) {
                QDBusInterface remoteIface(
                    QLatin1String(DBUS_SERVICE),
                    QLatin1String(DBUS_PATH),
                    QLatin1String(DBUS_SERVICE),
                    bus);
                remoteIface.call("showWindow");
                m_status->setText(
                    i18nd("miryu-repo-gui",
                          "Miryu Repository Manager is already running. Window activated."));
                return;
            }
        }

        const QString program = QStringLiteral("miryu-repo-gui");
        qint64 pid = 0;
        if (QProcess::startDetached(program, {}, QString(), &pid)) {
            m_status->setText(
                i18nd("miryu-repo-gui",
                      "Miryu Repository Manager is an external application and has been "
                      "automatically launched."));
        } else {
            m_status->setText(
                i18nd("miryu-repo-gui",
                      "Could not launch Miryu Repository Manager. Make sure it is installed."));
        }
    }

private:
    QPushButton *m_launchButton = nullptr;
    QLabel *m_status = nullptr;
    bool m_firstShow = true;
};

K_PLUGIN_CLASS_WITH_JSON(MiryuRepoKcm, "kcm_miryu_repo_gui.json")

#include "kcm_miryu_repo_gui.moc"
