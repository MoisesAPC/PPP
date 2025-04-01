#ifndef FILEMANAGER_H
#define FILEMANAGER_H

/**
 * @file FileManager.h
 * @brief FileManager header file
 *
 * @author Moisés Antonio Pestano Castro
 */

#include "include/file/FileLoader.h"
#include <QFile>
#include <QtEndian>
#include <QFileInfo>

/**
 * @class FileManager
 * @brief FileManager singleton class definition
 *
 * This singleton handles file-related operations.
 * This class is a wrapper for file tasks. The specific operations are handled
 * by the pre-assigned file parser (see the "loader" member variable).
 */
class FileManager {
    public:
        /**
         * @brief The file format of the opened file
         */
        enum eFormat {
            FORMAT_NOTE,                  // .note
            FORMAT_CONTROLLERPAK,         // .pak, .mpk
            FORMAT_CARTRIDGE,             // .eep
            FORMAT_DEXDRIVE               // .n64, .t64
        };

        /**
         * Helper struct that contains variables needed for identifying
         * individual saves inside Controller Pak-formatted files.
         */
        struct ControllerPakNotetableData {
            int index = -1;
            short region = SaveData::USA;
            unsigned int rawDataStartOffset = 0;

            ControllerPakNotetableData() { clearEntry(); }

            void clearEntry() {
                index = -1;
                region = SaveData::USA;
                rawDataStartOffset = 0;
            }
        };

        const unsigned int CONTROLLER_PAK_NOTE_TABLE_ENTRY_SIZE = 0x20;  /**< Size of each entry in the note table */
        const unsigned int CONTROLLER_PAK_NOTE_TABLE_NUM_ENTRIES = 16;   /**< Total number of elements in the note table */

        // Singleton-related functions
        static FileManager* getInstance() {
            if (instance == nullptr) {
                createInstance();
            }

            return instance;
        }

        static void createInstance() {
            instance = new FileManager();
        }

        static void destroyInstance() {
            delete instance;
            instance = nullptr;
        }

        // Inline getters and setters
        inline int getFileFormat() const {
            return format;
        }

        inline void setFileFormat(const int format_) {
            format = format_;
        }

        inline QString& getFilepath() {
            return filepath;
        }

        inline void setFilePath(const QString& filepath_) {
            filepath = filepath_;
        }

        inline QFile& getFile() {
            return *file;
        }

        inline QByteArray& getBuffer() {
            return *buffer;
        }

        inline FileLoader* getLoader() {
            return loader;
        }

        inline int getControllerPakCurrentlySelectedSaveIndex() const {
            return controllerPakCurrentlySelectedSaveIndex;
        }

        inline void setControllerPakCurrentlySelectedSaveIndex(int controllerPakCurrentlySelectedSaveIndex_) {
            controllerPakCurrentlySelectedSaveIndex = controllerPakCurrentlySelectedSaveIndex_;
        }

        inline std::vector<ControllerPakNotetableData>* getControllerPakNotetableDataArray() {
            return &noteTableArray;
        }

        inline bool wasFileOpened() const {
            return fileOpened;
        }

        inline void setFileOpened(const bool fileOpened_) {
            fileOpened = fileOpened_;
        }

        // Functions for the main file operations
        int openFile(const QString& filepath_);
        int writeFile(const QString& filepath_, bool isReplacingOldFile);

        // Functions or handling the note table data array
        unsigned int initNoteTableData(QFile& file);

        void clearNoteTableData() {
            for (int i = 0; i < noteTableArray.size(); i++) {
                noteTableArray[i].clearEntry();
            }
        }

    private:
        static FileManager* instance;

        // Constructors and destructor
        FileManager() {
            filepath = "";
            buffer = new QByteArray();
        }

        ~FileManager() {
            if (file != nullptr) {
                file->close();

                delete file;
                file = nullptr;
            }

            if (loader != nullptr) {
                delete loader;
                loader = nullptr;
            }

            if (buffer != nullptr) {
                delete buffer;
                buffer = nullptr;
            }
        }

        FileManager(const FileManager& obj) = delete; // Remove the copy constructor
        int determineFormat();

        int format = FORMAT_NOTE;                           /**< File format */
        int controllerPakCurrentlySelectedSaveIndex = 0;    /**< The index of the currently selected save in a loaded Controller Pak */

        QFile* file = nullptr;                              /**< Currently-opened file */
        QByteArray* buffer = nullptr;                       /**< File buffer containing the raw bytes for the currently-opened file */
        QString filepath;                                   /**< File path of the currently-opened file */
        FileLoader* loader = nullptr;                       /**< File format */
        /**< A file was opened at least once. Used for knowing if we have to enable or disable the Save buttons */
        bool fileOpened = false;

        /**
         * An array of "ControllerPakNotetableData". This is use on Controller Pak-specific formats to
         * store all the entries in the note table that contain Castlevania 64 saves.
         */
        std::vector<ControllerPakNotetableData> noteTableArray{CONTROLLER_PAK_NOTE_TABLE_NUM_ENTRIES};
};

#endif
