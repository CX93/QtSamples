#include "cxpainter.h"

static QStringList s_tips{
    "[Ctrl + A] to add image.",
    "[Ctrl + S] to save." ,
    "[Ctrl + Z] for undo operate." ,
    "[Ctrl + 0] to reset." ,
    "[Ctrl + 1] to change shape type." ,
    "Mouse press to draw shape." };

cxPainter::cxPainter(QWidget *parent)
    : QWidget(parent)
{
    qsrand(QDateTime::currentSecsSinceEpoch());
    setMouseTracking(true);
    setCursor(QCursor(Qt::CrossCursor));
    initShortCuts();
}

cxPainter::~cxPainter()
{
}

void cxPainter::initShortCuts()
{
    QShortcut  *pAdd = new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_A), this);
    connect(pAdd, &QShortcut::activated, this, &cxPainter::onAddImage);

    QShortcut  *pUndo = new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_Z), this);
    connect(pUndo, &QShortcut::activated, this, &cxPainter::onUndoOperate);

    QShortcut  *pSave = new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_S), this);
    connect(pSave, &QShortcut::activated, this, &cxPainter::onSaveImage);

    QShortcut  *pReset = new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_0), this);
    connect(pReset, &QShortcut::activated, this, &cxPainter::onReset);

    QShortcut  *pChangeShape = new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_1), this);
    connect(pChangeShape, &QShortcut::activated, [=]() {
        if (m_image.isNull())return;
        m_curShape.type = (cxShapeType)(m_curShape.type+1 >= Unknown ? Line : m_curShape.type + 1);
        update();
    });
}

void cxPainter::loadImageFunc(const QString& filename)
{
    if (!m_image.load(filename))
    {
        QMessageBox::warning(nullptr, "warning", "load image failed!");
        return;
    }
    onReset();
}

void cxPainter::saveImageFunc(const QString& filename, const QPixmap& pix)
{
    if (!pix.save(filename))
    {
        QMessageBox::warning(nullptr, "warning", "save image failed!");
        return;
    }
}

void cxPainter::moveToCenter()
{
    int x = width() / 2 - m_image.width()*m_scale / 2;
    int y = height() / 2 - m_image.height()*m_scale / 2;
    m_translate.setX(x);
    m_translate.setY(y);
}

void cxPainter::onAddImage()
{
    QString fileName = QFileDialog::getOpenFileName(nullptr, tr("Open File"), QStandardPaths::writableLocation(QStandardPaths::PicturesLocation), tr("Images (*.png *.jpg)"));
    if (!fileName.isEmpty())
    {
        QtConcurrent::run(this, &cxPainter::loadImageFunc, fileName);
    }
}

void cxPainter::onUndoOperate()
{
    if(m_curShape.display)
    {
        m_curShape.display=false;
        m_curShape.w = 0;
        m_curShape.h = 0;
        update();
        return;
    }

    if (m_shapes.size())
    {
        m_shapes.takeLast();
        update();
    }
}

void cxPainter::onSaveImage()
{
    if (m_image.isNull())return;
    QPixmap pix(QPixmap::fromImage(m_image));
    QPainter painter(&pix);
    painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing | QPainter::SmoothPixmapTransform);
    for (const cxShape& shape : m_shapes)
    {
        drawShape(painter, shape);
    }

    QString fileName = QFileDialog::getSaveFileName(this, tr("Save File"), QStandardPaths::writableLocation(QStandardPaths::PicturesLocation), tr("Images (*.png  *.jpg)"));
    if (!fileName.isEmpty())
    {
        QtConcurrent::run(this, &cxPainter::saveImageFunc, fileName, pix);
    }
}

void cxPainter::onReset()
{
    qreal wScale = width() / (qreal)m_image.width();
    qreal hScale = height() / (qreal)m_image.height();
    m_scale = wScale > hScale ? hScale : wScale;
    moveToCenter();
    m_shapes.clear();
    update();
}

void cxPainter::drawShape(QPainter& painter, const cxShape& shape)
{
    if (shape.display)
    {
        QPen pen;
        pen.setBrush(QBrush(shape.penColor));
        pen.setWidth(abs(shape.w > shape.h ? shape.h : shape.w) / 20 + 1);
        pen.setStyle(Qt::SolidLine);
        pen.setCapStyle(Qt::RoundCap);
        pen.setJoinStyle(Qt::RoundJoin);
        painter.setPen(pen);

        QLinearGradient linearGrad(shape.pt,shape.pt+QPointF(shape.w,shape.h));
        linearGrad.setColorAt(0,shape.brushColor);
        QColor endColor = shape.brushColor;
        endColor.setAlpha(endColor.alpha()+155);
        linearGrad.setColorAt(1,endColor);

        painter.setBrush(QBrush(linearGrad));

        switch (shape.type)
        {
        case Circle:
            painter.drawEllipse(shape.pt, abs(shape.w), abs(shape.h));
            break;
        case RectAngle:
            painter.drawRect(QRectF(shape.pt, QSize(abs(shape.w), abs(shape.h))));
            break;
        case Line:
            painter.drawLine(shape.pt, QPoint(shape.pt.x() + shape.w, shape.pt.y() + shape.h));
            break;
        case Arc:
            painter.drawArc(QRectF(shape.pt, QSize(abs(shape.w), abs(shape.h))), 0, 16 * ((shape.w + shape.h) % 360));
            break;
        case Chord:
            painter.drawChord(QRectF(shape.pt, QSize(abs(shape.w), abs(shape.h))), 0, 16 * ((shape.w + shape.h) % 360));
            break;
        case  Pie:
            painter.drawPie(QRectF(shape.pt, QSize(abs(shape.w), abs(shape.h))), 0, 16 * ((shape.w + shape.h) % 360));
            break;
        default:
            painter.drawRoundedRect(QRectF(shape.pt, QSize(abs(shape.w), abs(shape.h))), abs(shape.w) / 10, abs(shape.h) / 10);
            break;
        }
    }
}

void cxPainter::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)
    QPainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing | QPainter::SmoothPixmapTransform);
    if (m_image.isNull())
    {
        QFont ft;
        ft.setPixelSize(14);
        painter.setFont(ft);
        painter.setPen(Qt::blue);

        for (int i = 0; i < s_tips.size(); ++i)
        {
            const QString& tip = s_tips.at(i);
            painter.drawText(10, 20 * (i + 1), tip);
        }
        return;
    }
    painter.translate(m_translate);
    painter.scale(m_scale, m_scale);
    painter.drawImage(0, 0, m_image);
    painter.setClipRect(0, 0, m_image.width(), m_image.height());

    for (const cxShape& shape : m_shapes)
    {
        drawShape(painter, shape);
    }
    drawShape(painter, m_curShape);
}

void cxPainter::wheelEvent(QWheelEvent *event)
{
    if (m_image.isNull()) return;
    qreal s1 = m_scale;
    if (event->delta() > 0)
    {
        m_scale *= 1.1;
    }
    else
    {
        m_scale *= 0.9;
    }
    /*
    解方程
    (p-t)/s1 = (p-t+f)/s2
    */
    QPoint offset = m_scale / s1*(m_curPos - m_translate) + m_translate - m_curPos;
    m_translate -= offset;
    update();
}

void cxPainter::mouseMoveEvent(QMouseEvent *event)
{
    if (m_image.isNull()) return;
    if (event->buttons() == Qt::MidButton)
    {
        m_translate += (event->pos() - m_curPos);
        update();
    }
    else if (event->buttons() == Qt::LeftButton&&m_mousePressed)
    {
        m_curShape.w += (event->x() - m_curPos.x()) / (m_scale < 1 ? m_scale : 1);
        m_curShape.h += (event->y() - m_curPos.y()) / (m_scale < 1 ? m_scale : 1);
        update();
    }
    m_curPos = event->pos();
}

void cxPainter::mouseReleaseEvent(QMouseEvent *event)
{
    if (m_image.isNull()) return;
    if (!QRect(m_translate.x(), m_translate.y(), m_image.width()*m_scale, m_image.height()*m_scale).contains(event->pos()))
    {
        moveToCenter();
        update();
    }
    else if (event->button() == Qt::LeftButton)
    {
        m_shapes.push_back(m_curShape);
        m_curShape.display = false;
        m_curShape.w = 0;
        m_curShape.h = 0;
    }
    m_mousePressed = false;
    setCursor(QCursor(Qt::CrossCursor));
}

void cxPainter::mousePressEvent(QMouseEvent *event)
{
    Q_UNUSED(event)
    if (m_image.isNull()) return;

    if (event->button() == Qt::LeftButton)
    {
        if (m_curShape.display)
        {
            m_shapes.push_back(m_curShape);
            m_curShape.w = 0;
            m_curShape.h = 0;
        }
        m_curShape.pt = (event->pos() - m_translate) / m_scale;
        m_curShape.display = true;
        m_curShape.penColor = QColor(qrand() % 255, qrand() % 255, qrand() % 255);
        m_curShape.brushColor = QColor(qrand() % 255, qrand() % 255, qrand() % 255,50+ qrand() % 50);
    }
    else if (event->button() == Qt::MidButton)
    {
        setCursor(QCursor(Qt::SizeAllCursor));
    }
    m_mousePressed = true;
}
