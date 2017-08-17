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

struct PanelComboItem
{
    const char* name;
    i32 panelType;
};

constexpr PanelComboItem panelComboItems[] = {
    { "Hex", DataPanelView::PT_HEX },
    { "ASCII", DataPanelView::PT_ASCII },
    { "", -1 },
    { "Int 8", DataPanelView::PT_INT8 },
    { "Int 16", DataPanelView::PT_INT16 },
    { "Int 32", DataPanelView::PT_INT32 },
    { "Int 64", DataPanelView::PT_INT64 },
    { "", -1 },
    { "UInt 8", DataPanelView::PT_UINT8 },
    { "UInt 16", DataPanelView::PT_UINT16 },
    { "UInt 32", DataPanelView::PT_UINT32 },
    { "UInt 64", DataPanelView::PT_UINT64 },
    { "", -1 },
    { "Float 32", DataPanelView::PT_FLOAT32 },
    { "Float 64", DataPanelView::PT_FLOAT64 }
};

constexpr i32 panelComboItemsCount = sizeof(panelComboItems) / sizeof(PanelComboItem);

class MainWindow: public QMainWindow
{
    Q_OBJECT

public:
    FileData curFile;
    DataPanelView* dataPanelView;
    QWidget* centralWidget;
    QVBoxLayout* baseLayout;
    QHBoxLayout* layoutPanelCombos;

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
        baseLayout = new QVBoxLayout();
        baseLayout->setMargin(0);
        baseLayout->setSpacing(0);

        layoutPanelCombos = new QHBoxLayout();
        layoutPanelCombos->setMargin(0);
        layoutPanelCombos->setSpacing(0);

        for(i32 i = 0; i < PANEL_COUNT; ++i) {
            cbPanelType[i] = new QComboBox();
            cbPanelType[i]->setMaxVisibleItems(60);
            cbPanelType[i]->setFixedWidth(100);

            for(i32 j = 0; j < panelComboItemsCount; ++j) {
                if(panelComboItems[j].panelType == -1) {
                    cbPanelType[i]->insertSeparator(0xFFFFFF);
                }
                else {
                    cbPanelType[i]->addItem(panelComboItems[j].name);
                }
            }

            connect(cbPanelType[i], (void(QComboBox::*)(int))&QComboBox::currentIndexChanged, [=]() {
                this->onPanelChanged(i);
            });

            layoutPanelCombos->addWidget(cbPanelType[i]);
        }

        layoutPanelCombos->addStretch();

        baseLayout->addLayout(layoutPanelCombos);

        dataPanelView = new DataPanelView();
        baseLayout->addWidget(dataPanelView);

        centralWidget->setLayout(baseLayout);
        setCentralWidget(centralWidget);

        resize(1280, 720);
        setAcceptDrops(true);
        statusBar()->showMessage(tr("Ready"));

        loadFile("C:\\Program Files (x86)\\NAMCO BANDAI Games\\DarkSouls\\"
                 "dvdbnd0.bhd5.extract\\map\\MapStudio\\m18_01_00_00.msb");

        dataPanelView->setPanelType(0, DataPanelView::PT_HEX);
        dataPanelView->setPanelType(1, DataPanelView::PT_ASCII);
        dataPanelView->setPanelType(2, DataPanelView::PT_INT32);
        cbPanelType[0]->setCurrentIndex(DataPanelView::PT_HEX);
        cbPanelType[1]->setCurrentIndex(DataPanelView::PT_ASCII);
        cbPanelType[2]->setCurrentIndex(DataPanelView::PT_INT32 + 1);
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
        dataPanelView->setData(curFile.buffer, curFile.size);
    }

    void clearCurrentFile()
    {
        free(curFile.buffer);
        curFile.buffer = nullptr;
        dataPanelView->setData(0, 0);
    }

    void onPanelChanged(i32 pid)
    {
        i32 val = panelComboItems[cbPanelType[pid]->currentIndex()].panelType;
        //qDebug("onPanelChanged(%d, %d)", pid, val);
        dataPanelView->setPanelType(pid, val);
        updatePanelComboWidths();
    }

    void updatePanelComboWidths()
    {
        for(i32 i = 0; i < PANEL_COUNT; ++i) {
            cbPanelType[i]->setFixedWidth(dataPanelView->_panelRect[i].width() + 1);
        }
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

        updatePanelComboWidths();
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
