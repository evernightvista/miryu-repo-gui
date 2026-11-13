cmake_minimum_required(VERSION 3.16)
project(miryu-repo-gui VERSION 45.0.0 LANGUAGES CXX)

set(QT_MIN_VERSION "6.5.0")
set(KF_MIN_VERSION "6.0.0")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)

find_package(ECM ${KF_MIN_VERSION} REQUIRED NO_MODULE)
set(CMAKE_MODULE_PATH ${ECM_MODULE_PATH})

include(GNUInstallDirs)
include(KDEInstallDirs)

# kcoreaddons_add_plugin requires CMAKE_LIBRARY_OUTPUT_DIRECTORY to be set
# (normally done by KDECMakeSettings, but we avoid it globally to keep
# existing code working with implicit QString(const char*) conversions).
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/bin")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/bin")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/bin")

find_package(Qt6 ${QT_MIN_VERSION} REQUIRED COMPONENTS Core Widgets Network DBus)
find_package(KF6 ${KF_MIN_VERSION} REQUIRED COMPONENTS
    CoreAddons
    I18n
    KCMUtils
    WidgetsAddons
)

# ki18n_install() installs the compiled .mo file using the .po basename as the
# translation domain.  If the source tree stores translations as po/ja.po,
# po/fr.po, etc., that would install ja.mo/fr.mo and Fedora's %find_lang
# miryu-repo-gui will not find them.
#
# Normalize flat PO layouts to the KI18n layout expected by ki18n_install():
#   po/ja.po -> <build>/po-miryu-repo-gui/ja/miryu-repo-gui.po
# This keeps installation handled by ki18n_install() while producing:
#   /usr/share/locale/ja/LC_MESSAGES/miryu-repo-gui.mo
function(miryu_ki18n_install po_dir domain)
    set(src_dir "${CMAKE_CURRENT_SOURCE_DIR}/${po_dir}")

    if(NOT EXISTS "${src_dir}")
        return()
    endif()

    file(GLOB flat_po_files CONFIGURE_DEPENDS "${src_dir}/*.po")
    if(flat_po_files)
        set(normalized_dir "${CMAKE_CURRENT_BINARY_DIR}/po-${domain}")
        file(MAKE_DIRECTORY "${normalized_dir}")

        foreach(po_file IN LISTS flat_po_files)
            get_filename_component(lang "${po_file}" NAME_WE)
            file(MAKE_DIRECTORY "${normalized_dir}/${lang}")
            configure_file(
                "${po_file}"
                "${normalized_dir}/${lang}/${domain}.po"
                COPYONLY
            )
        endforeach()

        ki18n_install("${normalized_dir}")
        return()
    endif()

    # If the tree already uses KI18n's native layout, keep it unchanged.
    ki18n_install("${src_dir}")
endfunction()

add_executable(miryu-repo-gui
    src/main.cpp
    src/MainWindow.cpp
    src/MainWindow.h
    src/DBusAdaptor.h
    src/RepoManager.cpp
    src/MirrorTester.cpp
)

target_link_libraries(miryu-repo-gui
    Qt6::Core Qt6::Widgets Qt6::Network Qt6::DBus
    KF6::CoreAddons KF6::I18n KF6::WidgetsAddons
)

add_executable(miryu-repo-helper
    helper/evernight-vista-repo-helper.cpp
)

target_link_libraries(miryu-repo-helper
    Qt6::Core
    KF6::I18n
)

miryu_ki18n_install(po miryu-repo-gui)
miryu_ki18n_install(helper/po miryu-repo-helper)

# 安装 desktop 文件到 applications 目录
install(FILES data/miryu-repo-gui.desktop
        DESTINATION ${KDE_INSTALL_APPDIR})

# 安装 Polkit policy 文件
install(FILES data/org.miryugaming.repo.policy
        DESTINATION ${KDE_INSTALL_DATADIR}/polkit-1/actions)

install(TARGETS miryu-repo-gui
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})

install(TARGETS miryu-repo-helper
        RUNTIME DESTINATION ${CMAKE_INSTALL_LIBEXECDIR})

# KCM (KDE System Settings module) that launches the external Miryu Repository Manager GUI.
# Widgets-based KCMs install into the plasma/kcms/systemsettings_qwidgets plugin
# namespace so System Settings discovers them; the JSON metadata registers the
# module under the "System" section (X-KDE-System-Settings-Parent-Category).
kcoreaddons_add_plugin(kcm_miryu_repo_gui
    SOURCES kcm/kcm_miryu_repo_gui.cpp
    INSTALL_NAMESPACE "plasma/kcms/systemsettings_qwidgets"
)
target_link_libraries(kcm_miryu_repo_gui
    Qt6::Core
    Qt6::Gui
    Qt6::Widgets
    Qt6::DBus
    KF6::CoreAddons
    KF6::I18n
    KF6::KCMUtils
)
