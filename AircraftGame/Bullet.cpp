#include "Bullet.h"


Bullet::Bullet(QObject *parent)
	: AbstractCxItem(parent)
{
	m_pTimer = new QTimer(this);
	connect(m_pTimer, &QTimer::timeout, this, &Bullet::onMove);
	start();
	setData(0, "Bullet");
}

Bullet::~Bullet()
{
	qDebug() << __FUNCTION__;
}

void Bullet::start()
{
	qDebug() << __FUNCTION__;
	m_isRunning = true;
	if (!m_pTimer->isActive())
	{
		m_pTimer->start(BULLET_MOVE_FREQ);
	}
}

void Bullet::pause()
{
	qDebug() << __FUNCTION__;
	m_isRunning = false;
	m_pTimer->stop();
}

void Bullet::stop()
{
	qDebug() << __FUNCTION__;
	m_isRunning = false;
	slefDel();
}

QRectF Bullet::boundingRect() const
{
	return QRectF(-BULLET_W / 2, -BULLET_H / 2, BULLET_W, BULLET_H);
}

void Bullet::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option)
    Q_UNUSED(widget)
	if (m_isRunning)
	{
		for (QGraphicsItem * pItem : scene()->collidingItems(this))
		{
			if (pItem->data(0).toString() == "Enemy")
			{
				slefDel();
				return;
			}
		}
	}

	painter->setRenderHint(QPainter::Antialiasing);
	painter->setBrush(Qt::green);
	painter->drawEllipse(boundingRect());
}

void Bullet::slefDel()
{
	m_pTimer->stop();
	scene()->removeItem(this);
	deleteLater();
}

void Bullet::onMove()
{
	setPos(x(), y() - BULLET_MOVE_STEP);
	if (pos().y() + GAME_VIEW_H / 2 + boundingRect().height() < 0)
	{
		slefDel();
	}
	else
	{
		scene()->update();
	}
}
