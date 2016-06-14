// libraryfeature.cpp
// Created 8/17/2009 by RJ Ryan (rryan@mit.edu)

#include "library/libraryfeature.h"

// KEEP THIS cpp file to tell scons that moc should be called on the class!!!
// The reason for this is that LibraryFeature uses slots/signals and for this
// to work the code has to be precompiles by moc
LibraryFeature::LibraryFeature(QObject *parent)
        : QObject(parent) {

}

LibraryFeature::LibraryFeature(UserSettingsPointer pConfig, QObject* parent)
        : QObject(parent),
          m_pConfig(pConfig) {
}

LibraryFeature::~LibraryFeature() {
    
}

void LibraryFeature::bindSidebarWidget(WLibrary* pSidebarWidget, KeyboardEventFilter *) {
    TreeItemModel* pTreeModel = getChildModel();
    //qDebug() << "LibraryFeature::bindSidebarWidget" << pTreeModel->rowCount();    
    WLibrarySidebar* pSidebar = new WLibrarySidebar(pSidebarWidget);
    pSidebarWidget->registerView(getViewName(), pSidebar);
    qDebug() << getViewName() << pTreeModel;
    pSidebar->setObjectName(getViewName());
    
    if (pTreeModel == nullptr) {
        return;
    }
   
    pSidebar->setModel(pTreeModel);
    
    connect(pSidebar, SIGNAL(clicked(const QModelIndex&)),
            pTreeModel, SLOT(clicked(const QModelIndex&)));
    connect(pSidebar, SIGNAL(doubleClicked(const QModelIndex&)),
            pTreeModel, SLOT(doubleClicked(const QModelIndex&)));
    connect(pSidebar, SIGNAL(rightClicked(const QPoint&, const QModelIndex&)),
            pTreeModel, SLOT(rightClicked(const QPoint&, const QModelIndex&)));
}

QStringList LibraryFeature::getPlaylistFiles(QFileDialog::FileMode mode) {
    QString lastPlaylistDirectory = m_pConfig->getValueString(
            ConfigKey("[Library]", "LastImportExportPlaylistDirectory"),
            QDesktopServices::storageLocation(QDesktopServices::MusicLocation));

    QFileDialog dialogg(NULL,
                     tr("Import Playlist"),
                     lastPlaylistDirectory,
                     tr("Playlist Files (*.m3u *.m3u8 *.pls *.csv)"));
    dialogg.setAcceptMode(QFileDialog::AcceptOpen);
    dialogg.setFileMode(mode);
    dialogg.setModal(true);

    // If the user refuses return
    if (! dialogg.exec()) return QStringList();
    return dialogg.selectedFiles();
}
