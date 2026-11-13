Name:           miryu-repo-gui
Version:        45.0.0
Release:        1%{?dist}
Summary:        GUI tool for managing Miryu repository mirrors

License:        GPL-3.0-or-later
URL:            https://github.com/evernightvista/miryu-repo-gui
Source0:        https://github.com/evernightvista/miryu-repo-gui/archive/refs/heads/main.zip

BuildRequires:  cmake
BuildRequires:  gcc-c++
BuildRequires:  gettext
BuildRequires:  desktop-file-utils
BuildRequires:  extra-cmake-modules
BuildRequires:  cmake(Qt6Core)
BuildRequires:  cmake(Qt6Widgets)
BuildRequires:  cmake(Qt6Network)
BuildRequires:  cmake(KF6CoreAddons)
BuildRequires:  cmake(KF6I18n)
BuildRequires:  cmake(KF6WidgetsAddons)
BuildRequires:  cmake(KF6KCMUtils)

Requires:       polkit
Requires:       rpm
Requires:       dnf5
Requires:       evernight-vista-repos >= 45-1
Obsoletes:      evernight-vista-repo-gui
Provides:       evernight-vista-repo-gui

%description
Miryu Repo GUI is a graphical utility for managing repository
mirror configuration. It ships with a privileged helper integrated through
Polkit and localized user interface translations. A KDE System Settings
module (KCM) is included so that the GUI can be launched directly from
KDE System Settings.


%prep
%autosetup -n %{name}-main


%build
%cmake
%cmake_build


%install
%cmake_install

%find_lang miryu-repo-gui
%find_lang miryu-repo-helper
cat miryu-repo-helper.lang >> miryu-repo-gui.lang


%check
desktop-file-validate %{buildroot}%{_datadir}/applications/miryu-repo-gui.desktop


%files -f miryu-repo-gui.lang
%{_bindir}/miryu-repo-gui
%{_libexecdir}/miryu-repo-helper
%{_datadir}/applications/miryu-repo-gui.desktop
%{_datadir}/polkit-1/actions/org.miryugaming.repo.policy
/usr/lib64/qt6/plugins/plasma/kcms/systemsettings_qwidgets/libkcm_miryu_repo_gui.so


%changelog
* Fri Nov 13 2026 KairikiFedora <13278297951@sina.cn> - 45.0.0-1
- Fix BUG

* Mon Oct 26 2026 KairikiFedora <13278297951@sina.cn> - 1.0.2-4
- Change Terra NYIST as Default

* Wed Oct 14 2026 KairikiFedora <13278297951@sina.cn> - 1.0.2-3
- Revert Terra repo

* Sat Sep 05 2026 KairikiFedora <13278297951@sina.cn> - 1.0.2-2
- Add KRfoss and KRfoss Mirror4 Terra mirror sites
- Set KRfoss Mirror4 as the default Terra mirror

* Tue Aug 25 2026 KairikiFedora <13278297951@sina.cn> - 1.0.2-1
- Rename project from evernight-vista-repo-gui to miryu-repo-gui
- Add KDE System Settings (KCM) module for launching from System Settings
- Rename polkit action IDs from org.evernight.vista.repo to org.miryugaming.repo
- Adapt to dnf5 repo directory move from /etc/yum.repos.d to /usr/share/dnf5/repos.d

* Sun Aug 16 2026 KairikiFedora <13278297951@sina.cn> - 1.0.2-1
- Adapt to dnf5 repo directory move from /etc/yum.repos.d to /usr/share/dnf5/repos.d

* Sat Aug 15 2026 KairikiFedora <13278297951@sina.cn> - 1.0.1-1
- Fix Detect repo bug

* Tue Aug 11 2026 KairikiFedora <13278297951@sina.cn> - 1.0.0-1
- Package GUI executable, helper executable, desktop file, Polkit policy, and translations
