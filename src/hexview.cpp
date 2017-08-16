#include "hexview.h"
#include <assert.h>
#include <QAbstractTableModel>
#include <QStyledItemDelegate>
#include <QPainter>
#include <QHeaderView>
#include <QScrollBar>
#include <QResizeEvent>

inline i32 b16Log10(i32 num)
{
    if(num <= 0xFFFFFF) {
        return 6;
    }
    if(num <= 0xFFFFF) {
        return 5;
    }
    if(num <= 0xFFFF) {
        return 4;
    }
    if(num <= 0xFFF) {
        return 3;
    }
    if(num <= 0xFF) {
        return 2;
    }
    if(num <= 0xF) {
        return 1;
    }
    return 0;
}

HexTableView::HexTableView()
{
    char str[3];
    str[2] = 0;
    constexpr char base16[] = "0123456789ABCDEF";

    for(i32 i = 0; i < 256; ++i) {
        str[0] = base16[i >> 4];
        str[1] = base16[i & 15];
        hexByteText[i] = QStaticText(str);
        hexByteText[i].setPerformanceHint(QStaticText::PerformanceHint::AggressiveCaching);
    }

    hexByteTextSize = QSize(hexByteText[255].size().width(), hexByteText[255].size().height());

    for(i32 p = 0; p < PANEL_COUNT; ++p) {
        _panelType[p] = -1;
    }

    asciiFont = QFont("Courier New");
    asciiFont.setHintingPreference(QFont::HintingPreference::PreferNoHinting);
    asciiFont.setStyleStrategy(QFont::StyleStrategy::NoFontMerging);
}

void HexTableView::setData(u8* data_, i64 size_)
{
    data = data_;
    dataSize = size_;
    rowCount = dataSize / columnCount + 1;
    rowHeaderWidth = b16Log10(rowCount) * 4 + 12;
    QSize vs = viewport()->size();
    i32 newStep = (vs.height() - columnHeaderHeight) / rowHeight;
    verticalScrollBar()->setPageStep(newStep);
    verticalScrollBar()->setRange(0, rowCount - newStep); // minus one page

    _updatePanelRects();
    viewport()->update();
}

void HexTableView::setPanelType(i32 panelId, i32 type)
{
    assert(panelId >= 0 && panelId < PANEL_COUNT);
    assert(type >= 0 && panelId < PT_COUNT_MAX);
    _panelType[panelId] = type;

    _updatePanelRects();
    viewport()->update();
}

void HexTableView::resizeEvent(QResizeEvent* event)
{
    QSize newSize = event->size();
    i32 newStep = (newSize.height() - columnHeaderHeight) / rowHeight;
    verticalScrollBar()->setPageStep(newStep);
    verticalScrollBar()->setRange(0, rowCount - newStep); // minus one page

    _updatePanelRects();

    QAbstractScrollArea::resizeEvent(event);
}

void HexTableView::paintEvent(QPaintEvent* event)
{
    /*static int counter = 0;
    qDebug("painting...%d", counter++);*/

    QWidget& viewp = *viewport();
    QPainter qp(&viewp);

    qp.fillRect(viewp.rect(), QBrush(colorBg));

    for(i32 p = 0; p < PANEL_COUNT; ++p) {
        switch(_panelType[p]) {
            case PT_HEX:
                _paintPanelHex(_panelRect[p], qp);
                break;
            case PT_ASCII:
                _paintPanelAscii(_panelRect[p], qp);
                break;
        }
    }

    event->accept();
}

void HexTableView::_paintPanelHex(QRect panelRect, QPainter& qp)
{
    // row header
    QRect rowHeaderRect = panelRect;
    rowHeaderRect.setWidth(rowHeaderWidth);
    qp.fillRect(rowHeaderRect, QBrush(colorHeaderBg));

    qp.setPen(colorHeaderLine);
    qp.drawLine(QPointF(rowHeaderRect.right(), rowHeaderRect.y() + columnHeaderHeight),
                QPointF(rowHeaderRect.right(), rowHeaderRect.bottom()));

    i32 topRowId = verticalScrollBar()->value();
    i32 rowMaxDrawnCount = verticalScrollBar()->pageStep() + 1;

    // row number in hex
    qp.setPen(colorHeaderText);
    QString hexText;
    for(i32 r = 0; r < rowMaxDrawnCount && (r + topRowId) < rowCount; ++r) {
        hexText.sprintf("%02X",  r + topRowId);
        qp.drawText(QRect(panelRect.x() + 5, panelRect.y() + columnHeaderHeight + r * rowHeight,
                          rowHeaderWidth, rowHeight), Qt::AlignLeft | Qt::AlignVCenter, hexText);
    }

    // horizontal lines
    qp.setPen(colorHeaderLine);
    for(i32 r = 1; r < rowMaxDrawnCount && (r + topRowId) <= rowCount; ++r) {
        qp.drawLine(QPointF(panelRect.x() + rowHeaderWidth,
                            panelRect.y() + columnHeaderHeight + r * rowHeight),
                    QPointF(panelRect.right(), panelRect.y() + columnHeaderHeight + r * rowHeight));
    }

    // column header
    QRect columnHeaderRect = panelRect;
    columnHeaderRect.setHeight(columnHeaderHeight);
    qp.fillRect(columnHeaderRect, QBrush(colorHeaderBg));

    qp.setPen(colorHeaderLine);
    qp.drawLine(QPointF(columnHeaderRect.x(), columnHeaderRect.bottom()),
                QPointF(columnHeaderRect.right(), columnHeaderRect.bottom()));

    // column number in hex
    const i32 chXoff = (columnWidth - hexByteTextSize.width()) / 2;
    const i32 chYoff = (columnHeaderHeight - hexByteTextSize.height()) / 2;
    qp.setPen(colorHeaderText);
    for(i32 c = 0; c < columnCount; ++c) {
        qp.drawStaticText(panelRect.x() + rowHeaderWidth + c * columnWidth + chXoff,
                          panelRect.y() + chYoff,
                          hexByteText[c]);
    }

    // vertical lines
    qp.setPen(colorHeaderLine);
    for(i32 c = 4; c <= columnCount; c += 4) {
        qp.drawLine(QPointF(panelRect.x() + rowHeaderWidth + c * columnWidth,
                            panelRect.y() + columnHeaderHeight),
                    QPointF(panelRect.x() + rowHeaderWidth + c * columnWidth, panelRect.bottom()));
    }

    // draw data
    if(data) {
        const i32 xoff = (columnWidth - hexByteTextSize.width()) / 2;
        const i32 yoff = (rowHeight - hexByteTextSize.height()) / 2;

        qp.setPen(colorDataText);
        for(i32 r = 0; r < rowMaxDrawnCount; ++r) {
            for(i32 c = 0; c < columnCount; ++c) {
                i64 id = (r+topRowId) * columnCount + c;
                if(id >= dataSize) {
                    break;
                }

                qp.drawStaticText(panelRect.x() + rowHeaderWidth + c * columnWidth + xoff,
                                  panelRect.y() + columnHeaderHeight + r * rowHeight + yoff,
                                  hexByteText[data[id]]);
            }
        }
    }
}

void HexTableView::_paintPanelAscii(QRect panelRect, QPainter& qp)
{
    _makeAsciiText();

    const QFont& oldFont = qp.font();
    qp.setFont(asciiFont);

    i32 topRowId = verticalScrollBar()->value();
    i32 rowMaxDrawnCount = verticalScrollBar()->pageStep() + 1;

    qp.setPen(colorDataText);
    for(i32 r = 0; r < rowMaxDrawnCount && (r + topRowId) < rowCount; ++r) {
        qp.drawText(QRect(panelRect.x(), panelRect.y() + columnHeaderHeight + r * rowHeight,
                          panelRect.width(), rowHeight), Qt::AlignLeft | Qt::AlignVCenter,
                    QString::fromLatin1(asciiText + (r * columnCount), columnCount));
    }

    qp.setFont(oldFont);
}

void HexTableView::_updatePanelRects()
{
    //qDebug("_updatePanelRects()");

    i32 xoff = 0;
    for(i32 p = 0; p < PANEL_COUNT; ++p) {
        _panelRect[p] = viewport()->rect();
        _panelRect[p].setX(xoff);

        switch(_panelType[p]) {
            case PT_HEX:
                _panelRect[p].setWidth(rowHeaderWidth + columnCount * columnWidth);
                break;
            case PT_ASCII:
                _panelRect[p].setWidth(columnCount * columnWidth);
                break;
        }

        xoff += _panelRect[p].width();
        /*qDebug("panel_rect(%d, %d, %d, %d)",
               panelRect[p].x(), panelRect[p].y(), panelRect[p].width(), panelRect[p].height());*/
    }
}

void HexTableView::_makeAsciiText()
{
    i32 topRowId = verticalScrollBar()->value();
    i32 rowMaxDrawnCount = verticalScrollBar()->pageStep() + 1;
    const i32 textSize = rowMaxDrawnCount * columnCount;

    memset(asciiText, '.', sizeof(asciiText));
    memmove(asciiText, data + (topRowId * columnCount), textSize);

    for(i32 i = 0; i < textSize; ++i) {
        if(asciiText[i] < 0x20) {
            asciiText[i] = '.';
        }
    }
}
