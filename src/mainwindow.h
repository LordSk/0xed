#pragma once
#include "base.h"
#include <assert.h>
#include <QMainWindow>
#include <QFileDialog>
#include <QMenu>
#include <QMenuBar>
#include <QAction>
#include <QStatusBar>
#include <QDropEvent>
#include <QMimeData>
#include <QGridLayout>
#include <QScrollArea>
#include "hexview.h"

struct FileData
{
    QString filename;
    u8* buffer = nullptr;
    i64 size = 0;
};

class MainWindow: public QMainWindow
{
    Q_OBJECT

public:
    FileData curFile;
    HexTableView* hexView;
    QWidget* centralWidget;
    QGridLayout* baseLayout;

    QComboBox* cbPanelType[PANEL_COUNT];
    const i32 panelHeaderHeight = 24;

public slots:
    void slot_openFile()
    {
        QString fileName = QFileDialog::getOpenFileName(this);

        if(!fileName.isEmpty()) {
            loadFile(fileName);
        }
    }

public:
    MainWindow()
    {
        setWindowTitle("0xed [v0.001]");

        QMenu* fileMenu = menuBar()->addMenu(tr("&File"));
        QAction* newAct = new QAction(tr("&Open"), this);
        newAct->setShortcuts(QKeySequence::Open);
        newAct->setStatusTip(tr("Create a new file"));
        connect(newAct, &QAction::triggered, this, &MainWindow::slot_openFile);
        fileMenu->addAction(newAct);

        centralWidget = new QWidget();
        baseLayout = new QGridLayout();
        baseLayout->setMargin(0);
        baseLayout->setSpacing(0);

        for(i32 i = 0; i < PANEL_COUNT; ++i) {
            cbPanelType[i] = new QComboBox();
            cbPanelType[i]->addItem("Hex");
            cbPanelType[i]->addItem("ASCII");

            cbPanelType[i]->insertSeparator(0xFFFFFF);

            cbPanelType[i]->addItem("Int 8");
            cbPanelType[i]->addItem("Int 16");
            cbPanelType[i]->addItem("Int 32");
            cbPanelType[i]->addItem("Int 64");

            cbPanelType[i]->insertSeparator(0xFFFFFF);

            cbPanelType[i]->addItem("Uint 8");
            cbPanelType[i]->addItem("Uint 16");
            cbPanelType[i]->addItem("Uint 32");
            cbPanelType[i]->addItem("Uint 64");

            cbPanelType[i]->insertSeparator(0xFFFFFF);

            cbPanelType[i]->addItem("Float 32");
            cbPanelType[i]->addItem("Float 64");

            cbPanelType[i]->setMaxVisibleItems(60);
            baseLayout->addWidget(cbPanelType[i], 0, i);
        }

        hexView = new HexTableView();
        baseLayout->addWidget(hexView, 1, 0, 1, 3);

        centralWidget->setLayout(baseLayout);
        setCentralWidget(centralWidget);

        resize(1280, 720);
        setAcceptDrops(true);
        statusBar()->showMessage(tr("Ready"));

        loadFile("C:\\Program Files (x86)\\NAMCO BANDAI Games\\DarkSouls\\"
                 "dvdbnd0.bhd5.extract\\map\\MapStudio\\m18_01_00_00.msb");

        hexView->setPanelType(0, HexTableView::PT_HEX);
        hexView->setPanelType(1, HexTableView::PT_ASCII);
        hexView->setPanelType(2, HexTableView::PT_ASCII);
    }

    void loadFile(const QString& fileName)
    {
        FILE* file = fopen(qstr_cstr(fileName), "rb");
        if(!file) {
            statusBar()->showMessage(tr("ERROR: Can not open file"));
            qDebug("ERROR: Can not open file %s", qstr_cstr(fileName));
            return;
        }

        if(curFile.buffer) {
            clearCurrentFile();
        }

        FileData fd;

        i64 start = ftell(file);
        fseek(file, 0, SEEK_END);
        i64 len = ftell(file) - start;
        fseek(file, start, SEEK_SET);

        fd.buffer = (u8*)malloc(len + 1);
        assert(fd.buffer);
        fd.size = len;
        fd.filename = fileName;

        // read
        fread(fd.buffer, 1, len, file);
        fd.buffer[len] = 0;

        statusBar()->showMessage(tr("File loaded"));

        fclose(file);
        curFile = fd;

        qDebug("file loaded path=%s size=%llu", qstr_cstr(fileName), curFile.size);

        onFileLoaded();
    }

    void onFileLoaded()
    {
        hexView->setData(curFile.buffer, curFile.size);
    }

    void clearCurrentFile()
    {
        free(curFile.buffer);
        curFile.buffer = nullptr;
        hexView->setData(0, 0);
    }

    void closeEvent(QCloseEvent *event) override
    {
        if(curFile.buffer) {
            free(curFile.buffer);
            curFile.buffer = nullptr;
        }
    }

    void resizeEvent(QResizeEvent* event) override
    {
        QMainWindow::resizeEvent(event);
    }

    void dragEnterEvent(QDragEnterEvent* event)
    {
        event->accept(); // needed to accept dropEvent
    }

    void dropEvent(QDropEvent* event) override
    {
        const QMimeData& mime = *event->mimeData();

        if(mime.hasUrls()) {
            QString path = mime.urls().at(0).toLocalFile();
            qDebug("drop_event filepath=%s", qstr_cstr(path));
            loadFile(path);
            event->accept();
        }
    }
};
