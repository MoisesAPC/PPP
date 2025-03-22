#include "include\windows\controllerpakselection\controllerpakselectionwindow.h"
#include "include\file\FileManager.h"
#include <QPushButton>

ControllerPakSelectionWindow::ControllerPakSelectionWindow(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::ControllerPakSelectionWindow)
{
    ui->setupUi(this);

    // Make sure to only set this window up if we're working with Controller Pak saves
    if ((FileManager::getInstance()->getFileFormat() == FileManager::FORMAT_CONTROLLERPAK) && (FileManager::getInstance()->getLoader() != nullptr)) {
        setupButtonBox();
    }
}

ControllerPakSelectionWindow::~ControllerPakSelectionWindow()
{
    delete ui;
}

QString ControllerPakSelectionWindow::getRegionName(const short region) const {
    switch (region) {
        default:
        case SaveData::USA:
            return "Americas";

        case SaveData::JPN:
            return "Japan";

        case SaveData::PAL:
            return "Europe";
    }
}

void ControllerPakSelectionWindow::setupButtonBox() {
    // Get the Controller Pak Castlevania save array
    FileLoaderControllerPak* loader = dynamic_cast<FileLoaderControllerPak*>(FileManager::getInstance()->getLoader());
    std::vector<FileLoaderControllerPak::IndexData>* saveArray = &loader->indexDataArray;

    for (unsigned int i = 0; i < saveArray->size(); i++) {
        int index = (*saveArray)[i].index;
        if (index == -1) {
            continue;
        }

        short region = (*saveArray)[i].region;

        QString buttonText = "Save " + QString::number(index + 1) + "\n" +
                             getRegionName(region);

        QPushButton* button = new QPushButton(buttonText);
        button->setFixedWidth(500);
        ui->buttonBox->addButton(button, QDialogButtonBox::ActionRole);

        connect(button, &QPushButton::clicked, this, [this, index]() {
            onButtonClicked(index);
        });
    }
}

void ControllerPakSelectionWindow::onButtonClicked(int saveIndex) {
    // Set the save index needed for knowing what save file to load
    // (see FileLoaderControllerPak::getRawDataOffsetStart())
    FileManager::getInstance()->setControllerPakCurrentlySelectedSaveIndex(saveIndex);

    // Make sure we return QDialog::Accepted upon exiting the window in order to tell FileManager that we can continue loading the file
    // (see closeEvent() for the opposite case, where X is pressed to exit the window instead)
    accept();
}
