#ifndef CXPAINTER_H
#define CXPAINTER_H
#include <QWidget>
#include <QPainter>
#include <QWheelEvent>
#include <QDebug>
#include <QFileDialog>
#include <QShortcut>
#include <QStandardPaths>
#include <QtConcurrent>
#include <QCursor>
#include <QMessageBox>
#include <QDateTime>
#include <QFontMetrics>

class cxPainter : public QWidget
{
    Q_OBJECT

public:
    cxPainter(QWidget *parent = Q_NULLPTR);
    ~cxPainter();
private:
    enum cxShapeType
    {
        Line,
        Circle,
        RectAngle,
        RoundedRect,
        Arc,
        Chord,
        Pie,
        Unknown
    };

    struct cxShape
    {
        cxShapeType type = Line;
        QColor penColor = Qt::red;
        QColor brushColor = Qt::blue;
        QPointF pt;
        int w = 0, h = 0;
        bool display = false;
    };

private:
    void initShortCuts();
    void loadImageFunc(const QString& filename);
    void saveImageFunc(const QString& filename, const QPixmap&);
    void moveToCenter();
    void drawShape(QPainter&,const cxShape&);
private slots:
void onAddImage();
void onUndoOperate();
void onSaveImage();
void onReset();
protected:
    void paintEvent(QPaintEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
private:
    QImage m_image;
    qreal m_scale = 1.0;
    QPoint m_translate{ 0,0 };
    QPoint m_curPos;

    QList<cxShape> m_shapes;
    cxShape m_curShape;
    bool m_mousePressed = false;
};

#endif // CXPAINTER_H
