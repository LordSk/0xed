#pragma once
#include "base.h"
#include <QAbstractScrollArea>
#include <QComboBox>
#include <QStaticText>

enum: i32 {
    PANEL_COUNT = 3
};

class HexTableView: public QAbstractScrollArea
{
public:

    QColor colorBg = QColor(255, 255, 255);
    QColor colorHeaderBg = QColor(240, 240, 240);
    QColor colorHeaderLine = QColor(200, 200, 200);
    QColor colorHeaderText = QColor(150, 150, 150);
    QColor colorDataText = QColor(0, 0, 0);

    const i32 columnCount = 16;
    i32 columnWidth = 20;
    i32 columnHeaderHeight = 20;
    i32 rowCount = 100;
    i32 rowHeight = 20;
    i32 rowHeaderWidth = 22;

    u8* data = nullptr;
    i64 dataSize = 0;

    QChar dataHexStr[2048];
    QStaticText hexByteText[256];
    QSize hexByteTextSize;

    // PanelType
    enum: i32 {
        PT_HEX=0,
        PT_ASCII,
        PT_INT8,
        PT_INT16,
        PT_INT32,
        PT_INT64,
        PT_UINT8,
        PT_UINT16,
        PT_UINT32,
        PT_UINT64,
        PT_FLOAT32,
        PT_FLOAT64,

        PT_COUNT_MAX,
    };

    i32 panelType[PANEL_COUNT];
    QRect panelRect[PANEL_COUNT];

    HexTableView();

    void setData(u8* data_, i64 size_);
    void setPanelType(i32 panelId, i32 type);

    void resizeEvent(QResizeEvent* event) override;
    void paintEvent(QPaintEvent* event) override;

    void _makeDataHexString(i64 start, i32 size);
    void _paintPanelHex(QRect panelRect, QPainter& qp);
    void _paintPanelAscii(QRect rect, QPainter& qp);

    void _updatePanelRects();
};
