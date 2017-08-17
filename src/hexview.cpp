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

DataPanelView::DataPanelView()
{
    // prepare every byte hex combinaison (better performance with static text)
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

    QString intStr;
    intStr.sprintf("%d", SCHAR_MIN);
    intCellMaxWidth[1] = fontMetrics().width(intStr);
    intStr.sprintf("%d", SHRT_MIN);
    intCellMaxWidth[2] = fontMetrics().width(intStr);
    intStr.sprintf("%d", INT_MIN);
    intCellMaxWidth[4] = fontMetrics().width(intStr);
    intStr.sprintf("%lld", INT64_MIN);
    intCellMaxWidth[8] = fontMetrics().width(intStr);
}

void DataPanelView::setData(u8* data_, i64 size_)
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

void DataPanelView::setPanelType(i32 panelId, i32 type)
{
    assert(panelId >= 0 && panelId < PANEL_COUNT);
    assert(type >= 0 && panelId < PT_COUNT_MAX);
    _panelType[panelId] = type;

    _updatePanelRects();
    viewport()->update();
}

void DataPanelView::resizeEvent(QResizeEvent* event)
{
    QSize newSize = event->size();
    i32 newStep = (newSize.height() - columnHeaderHeight) / rowHeight;
    verticalScrollBar()->setPageStep(newStep);
    verticalScrollBar()->setRange(0, rowCount - newStep); // minus one page

    _updatePanelRects();

    QAbstractScrollArea::resizeEvent(event);
}

void DataPanelView::paintEvent(QPaintEvent* event)
{
    /*static int counter = 0;
    qDebug("painting...%d", counter++);*/

    // TODO: custom rendering? this is VERY slow overall

    QWidget& viewp = *viewport();
    QPainter qp(&viewp);

    qp.fillRect(viewp.rect(), colorBg);

    for(i32 p = 0; p < PANEL_COUNT; ++p) {
        switch(_panelType[p]) {
            case PT_HEX:
                _paintPanelHex(_panelRect[p], qp);
                break;
            case PT_ASCII:
                _paintPanelAscii(_panelRect[p], qp);
                break;
            case PT_INT8:
                _paintPanelInt8(_panelRect[p], qp, false);
                    break;
            case PT_INT16:
                _paintPanelInteger(_panelRect[p], qp, 2, false);
                break;
            case PT_INT32:
                _paintPanelInteger(_panelRect[p], qp, 4, false);
                break;
            case PT_INT64:
                _paintPanelInteger(_panelRect[p], qp, 8, false);
                break;
            case PT_UINT8:
                _paintPanelInt8(_panelRect[p], qp, true);
                    break;
            case PT_UINT16:
                _paintPanelInteger(_panelRect[p], qp, 2, true);
                break;
            case PT_UINT32:
                _paintPanelInteger(_panelRect[p], qp, 4, true);
                break;
            case PT_UINT64:
                _paintPanelInteger(_panelRect[p], qp, 8, true);
                break;
        }
    }

    event->accept();
}

void DataPanelView::_drawHexByte(i32 val, QRect rect, QPainter& qp)
{
    QPixmap& pix = pixHexByteText[val];

    if(pix.isNull()) {
        pix = QPixmap(rect.size());
        //pix.fill(Qt::white);
        pix.fill(Qt::transparent);
        constexpr char base16[] = "0123456789ABCDEF";
        char hexStr[3];
        hexStr[0] = base16[val >> 4];
        hexStr[1] = base16[val & 15];
        hexStr[2] = 0;
        QPainter p(&pix);
        p.setPen(Qt::black);
        p.setFont(qp.font());
        p.drawText(QRect(0, 0, rect.width(), rect.height()), Qt::AlignCenter, hexStr);
    }

    qp.drawPixmap(rect.x(), rect.y(), pix);
}

void DataPanelView::_drawInt8(i32 val, QRect rect, QPainter& qp)
{
    i32 id = val < 0 ? val + 255 : val;
    QPixmap& pix = pixInt8Text[id];

    if(pix.isNull()) {
        pix = QPixmap(rect.size());
        pix.fill(Qt::transparent);
        char decStr[5];
        sprintf(decStr, "%d", val);
        QPainter p(&pix);
        p.setPen(Qt::black);
        p.setFont(qp.font());
        p.drawText(QRect(0, 0, rect.width(), rect.height()), Qt::AlignRight | Qt::AlignVCenter, decStr);
    }

    qp.drawPixmap(rect.x(), rect.y(), pix);
}

void DataPanelView::_paintPanelHex(QRect panelRect, QPainter& qp)
{
    i32 topRowId = verticalScrollBar()->value();
    i32 rowMaxDrawnCount = verticalScrollBar()->pageStep() + 1;

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

                /*qp.drawStaticText(panelRect.x() + rowHeaderWidth + c * columnWidth + xoff,
                                  panelRect.y() + columnHeaderHeight + r * rowHeight + yoff,
                                  hexByteText[data[id]]);*/
                _drawHexByte(data[id],
                             QRect(panelRect.x() + rowHeaderWidth + c * columnWidth,
                                   panelRect.y() + columnHeaderHeight + r * rowHeight,
                                   columnWidth, rowHeight), qp);
            }
        }
    }

    // row header
    QRect rowHeaderRect = panelRect;
    rowHeaderRect.setWidth(rowHeaderWidth);
    qp.fillRect(rowHeaderRect, QBrush(colorHeaderBg));

    qp.setPen(colorHeaderLine);
    qp.drawLine(QPointF(rowHeaderRect.right(), rowHeaderRect.y() + columnHeaderHeight),
                QPointF(rowHeaderRect.right(), rowHeaderRect.bottom()));

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
    for(i32 c = 4; c < columnCount; c += 4) {
        qp.drawLine(QPointF(panelRect.x() + rowHeaderWidth + c * columnWidth,
                            panelRect.y() + columnHeaderHeight),
                    QPointF(panelRect.x() + rowHeaderWidth + c * columnWidth, panelRect.bottom()));
    }

    qp.drawLine(QPointF(panelRect.x() + rowHeaderWidth + columnCount * columnWidth,
                        panelRect.y()),
                QPointF(panelRect.x() + rowHeaderWidth + columnCount * columnWidth,
                        panelRect.bottom()));
}

void DataPanelView::_paintPanelAscii(QRect panelRect, QPainter& qp)
{
    _makeAsciiText();

    const QFont oldFont = qp.font();
    qp.setFont(asciiFont);

    i32 topRowId = verticalScrollBar()->value();
    i32 rowMaxDrawnCount = verticalScrollBar()->pageStep() + 1;

    qp.setPen(colorDataText);
    for(i32 r = 0; r < rowMaxDrawnCount && (r + topRowId) < rowCount; ++r) {
        qp.drawText(QRect(panelRect.x() + asciiHorizontalMargin,
                          panelRect.y() + columnHeaderHeight + r * rowHeight,
                          panelRect.width(),
                          rowHeight), Qt::AlignLeft | Qt::AlignBottom,
                    QString::fromLatin1(asciiText + (r * columnCount), columnCount));
    }

    qp.setFont(oldFont);
}

void DataPanelView::_paintPanelInteger(const QRect panelRect, QPainter& qp, const i32 bytes, bool unsigned_)
{
    assert(columnCount % bytes == 0);

    i32 cellMaxWidth = intCellMaxWidth[bytes];
    i32 intColumnCount = columnCount / bytes;

    // column header
    QRect columnHeaderRect = panelRect;
    columnHeaderRect.setHeight(columnHeaderHeight);
    qp.fillRect(columnHeaderRect, QBrush(colorHeaderBg));

    const i32 chYoff = (columnHeaderHeight - hexByteTextSize.height()) / 2;
    qp.setPen(colorHeaderText);
    for(i32 c = 0; c < intColumnCount; ++c) {
        qp.drawStaticText(panelRect.x() + c * (cellMaxWidth + intCellMargin * 2) + intCellMargin,
                          panelRect.y() + chYoff,
                          hexByteText[c * bytes]);
    }

    i32 topRowId = verticalScrollBar()->value();
    i32 rowMaxDrawnCount = verticalScrollBar()->pageStep() + 1;

    QString intStr;
    qp.setPen(colorDataText);
    for(i32 r = 0; r < rowMaxDrawnCount && (r + topRowId) < rowCount; ++r) {
        i32 xoff = panelRect.x();

        for(i32 c = 0; c < columnCount; c += bytes) {
            i64 id = (r + topRowId) * columnCount + c;
            if(id >= dataSize) {
                break;
            }

            if(unsigned_) {
                switch(bytes) {
                    case 2: intStr.sprintf("%u", *(u16*)(data + id)); break;
                    case 4: intStr.sprintf("%u", *(u32*)(data + id)); break;
                    case 8: intStr.sprintf("%llu", *(u64*)(data + id)); break;
                }
            }
            else {
                switch(bytes) {
                    case 2: intStr.sprintf("%d", *(i16*)(data + id)); break;
                    case 4: intStr.sprintf("%d", *(i32*)(data + id)); break;
                    case 8: intStr.sprintf("%lld", *(i64*)(data + id)); break;
                }
            }

            qp.drawText(QRect(xoff,
                              panelRect.y() + columnHeaderHeight + r * rowHeight,
                              cellMaxWidth + intCellMargin,
                              rowHeight), Qt::AlignRight | Qt::AlignVCenter, intStr);
            xoff += cellMaxWidth + intCellMargin * 2;
        }
    }

    // vertical lines
    qp.setPen(colorHeaderLine);
    for(i32 c = 0; c < intColumnCount; ++c) {
        qp.drawLine(QPointF(panelRect.x() + (cellMaxWidth + intCellMargin * 2) * c,
                            panelRect.y()),
                    QPointF(panelRect.x() + (cellMaxWidth + intCellMargin * 2) * c,
                            panelRect.bottom()));
    }

    qp.drawLine(QPointF(panelRect.right(),
                        panelRect.y()),
                QPointF(panelRect.right(),
                        panelRect.bottom()));

    // horizontal lines
    qp.setPen(colorHeaderLine);
    for(i32 r = 0; r < rowMaxDrawnCount && (r + topRowId) <= rowCount; ++r) {
        qp.drawLine(QPointF(panelRect.x(),
                            panelRect.y() + columnHeaderHeight + r * rowHeight),
                    QPointF(panelRect.right(), panelRect.y() + columnHeaderHeight + r * rowHeight));
    }
}

void DataPanelView::_paintPanelInt8(const QRect panelRect, QPainter& qp, bool unsigned_)
{
    i32 cellMaxWidth = intCellMaxWidth[1];
    i32 intColumnCount = columnCount;

    // column header
    QRect columnHeaderRect = panelRect;
    columnHeaderRect.setHeight(columnHeaderHeight);
    qp.fillRect(columnHeaderRect, QBrush(colorHeaderBg));

    const i32 chYoff = (columnHeaderHeight - hexByteTextSize.height()) / 2;
    qp.setPen(colorHeaderText);
    for(i32 c = 0; c < intColumnCount; ++c) {
        qp.drawStaticText(panelRect.x() + c * (cellMaxWidth + intCellMargin * 2) + intCellMargin,
                          panelRect.y() + chYoff,
                          hexByteText[c]);
    }

    i32 topRowId = verticalScrollBar()->value();
    i32 rowMaxDrawnCount = verticalScrollBar()->pageStep() + 1;

    qp.setPen(colorDataText);
    if(unsigned_) {
        for(i32 r = 0; r < rowMaxDrawnCount && (r + topRowId) < rowCount; ++r) {
            i32 xoff = panelRect.x();

            for(i32 c = 0; c < columnCount; c++) {
                i64 id = (r + topRowId) * columnCount + c;
                if(id >= dataSize) {
                    break;
                }

                _drawInt8(*(u8*)(data + id),
                          QRect(xoff,
                              panelRect.y() + columnHeaderHeight + r * rowHeight,
                              cellMaxWidth + intCellMargin,
                              rowHeight), qp);
                xoff += cellMaxWidth + intCellMargin * 2;
            }
        }
    }
    else {
        for(i32 r = 0; r < rowMaxDrawnCount && (r + topRowId) < rowCount; ++r) {
            i32 xoff = panelRect.x();

            for(i32 c = 0; c < columnCount; c++) {
                i64 id = (r + topRowId) * columnCount + c;
                if(id >= dataSize) {
                    break;
                }

                _drawInt8(*(i8*)(data + id),
                          QRect(xoff,
                              panelRect.y() + columnHeaderHeight + r * rowHeight,
                              cellMaxWidth + intCellMargin,
                              rowHeight), qp);
                xoff += cellMaxWidth + intCellMargin * 2;
            }
        }
    }

    // vertical lines
    qp.setPen(colorHeaderLine);
    for(i32 c = 0; c < intColumnCount; ++c) {
        qp.drawLine(QPointF(panelRect.x() + (cellMaxWidth + intCellMargin * 2) * c,
                            panelRect.y()),
                    QPointF(panelRect.x() + (cellMaxWidth + intCellMargin * 2) * c,
                            panelRect.bottom()));
    }

    qp.drawLine(QPointF(panelRect.right(),
                        panelRect.y()),
                QPointF(panelRect.right(),
                        panelRect.bottom()));

    // horizontal lines
    qp.setPen(colorHeaderLine);
    for(i32 r = 0; r < rowMaxDrawnCount && (r + topRowId) <= rowCount; ++r) {
        qp.drawLine(QPointF(panelRect.x(),
                            panelRect.y() + columnHeaderHeight + r * rowHeight),
                    QPointF(panelRect.right(), panelRect.y() + columnHeaderHeight + r * rowHeight));
    }
}

void DataPanelView::_updatePanelRects()
{
    //qDebug("_updatePanelRects()");

    asciiFont.setPixelSize(12);
    QFontMetrics fm(asciiFont);
    asciiRowTextWidth = fm.width(".") * columnCount;

    i32 xoff = 0;
    for(i32 p = 0; p < PANEL_COUNT; ++p) {
        _panelRect[p] = viewport()->rect();
        _panelRect[p].setX(xoff);

        switch(_panelType[p]) {
            case PT_HEX:
                _panelRect[p].setWidth(rowHeaderWidth + columnCount * columnWidth);
                break;
            case PT_ASCII:
                _panelRect[p].setWidth(asciiRowTextWidth + asciiHorizontalMargin * 2);
                break;
            case PT_INT8:
            case PT_UINT8:
                _panelRect[p].setWidth(intCellMargin * (columnCount * 2) +
                                       intCellMaxWidth[1] * columnCount);
                break;
            case PT_INT16:
            case PT_UINT16:
                _panelRect[p].setWidth(intCellMargin * (columnCount) +
                                       intCellMaxWidth[2] * (columnCount / 2));
                break;
            case PT_INT32:
            case PT_UINT32:
                _panelRect[p].setWidth(intCellMargin * (columnCount / 2) +
                                       intCellMaxWidth[4] * (columnCount / 4));
                break;
            case PT_INT64:
            case PT_UINT64:
                _panelRect[p].setWidth(intCellMargin * (columnCount / 4) +
                                       intCellMaxWidth[8] * (columnCount / 8));
                break;
        }

        xoff += _panelRect[p].width();
        /*qDebug("panel_rect(%d, %d, %d, %d)",
               panelRect[p].x(), panelRect[p].y(), panelRect[p].width(), panelRect[p].height());*/
    }
}

void DataPanelView::_makeAsciiText()
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
