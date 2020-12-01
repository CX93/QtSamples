#include "Aircraft.h"
#include "Bullet.h"

Aircraft::Aircraft(QObject *parent)
	: AbstractCxItem(parent)
{
	setData(0, "Aircraft");
	m_pTimer = new QTimer(this);
	connect(m_pTimer, &QTimer::timeout, this, &Aircraft::onCreateBullet);
}

Aircraft::~Aircraft()
{
}

void Aircraft::start()
{
	qDebug() << __FUNCTION__;
	m_isRunning = true;
	m_isCrash = false;
	if (!m_pTimer->isActive())
	{
		m_pTimer->start(BULLET_CREATE_FREQ);
	}
}

void Aircraft::pause()
{
	qDebug() << __FUNCTION__;
	m_isRunning = false;
	m_pTimer->stop();
}

void Aircraft::stop()
{
	qDebug() << __FUNCTION__;
	m_isRunning = false;
	m_pTimer->stop();
}

QRectF Aircraft::boundingRect() const
{
	return QRectF(-AIR_CRAFT_W / 2, -AIR_CRAFT_H / 2, AIR_CRAFT_W, AIR_CRAFT_H);

}
void Aircraft::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option)
    Q_UNUSED(widget)
	if (m_isRunning)
	{
		for (QGraphicsItem * pItem : scene()->collidingItems(this))
		{
			if (pItem->data(0).toString() == "Enemy")
			{
				m_isCrash = true;
				emit sigFinished();
				break;
			}
		}
	}
	painter->setRenderHint(QPainter::Antialiasing);
	if (m_isCrash)
	{
		//painter->setPen(Qt::red);
		painter->setBrush(Qt::red);
		painter->drawEllipse(boundingRect());
	}
	else
	{
		painter->setBrush(Qt::blue);
		QVector<QPoint> vets;
		vets.append(QPoint(0, -AIR_CRAFT_H / 2));
		vets.append(QPoint(-AIR_CRAFT_W / 2,0 ));
		vets.append(QPoint(-AIR_CRAFT_W / 2, AIR_CRAFT_H / 2));
		vets.append(QPoint(-AIR_CRAFT_W / 4, AIR_CRAFT_H / 4));
		vets.append(QPoint(AIR_CRAFT_W / 4, AIR_CRAFT_H / 4));
 		vets.append(QPoint(AIR_CRAFT_W / 2, AIR_CRAFT_H / 2));
 		vets.append(QPoint(AIR_CRAFT_W / 2, 0));
 		vets.append(QPoint(0, -AIR_CRAFT_H / 2));
		painter->drawPolygon(vets);
		//painter->drawRect(boundingRect());
	}
}

void Aircraft::onCreateBullet()
{
	Bullet * bullet = new Bullet(this);
	bullet->setPos(x(), y());
	scene()->addItem(bullet);
}

void Aircraft::keyPressEvent(QKeyEvent *event)
{
	if (!m_isRunning)
		return;
	if (event->key() == Qt::Key_Left)
	{
		if (pos().x() > -GAME_VIEW_W / 2 + AIR_CRAFT_W / 2)
		{
			setPos(x() - AIR_CRAFT_MOVE_STEP, y());
		}
	}
	else if (event->key() == Qt::Key_Right)
	{
		if (pos().x() < GAME_VIEW_W / 2 - AIR_CRAFT_W / 2)
		{
			setPos(x() + AIR_CRAFT_MOVE_STEP, y());
		}
	}
	else if (event->key() == Qt::Key_Up)
	{
		if (pos().y() > -GAME_VIEW_H / 2 + AIR_CRAFT_H / 2)
		{
			setPos(x(), y() - AIR_CRAFT_MOVE_STEP);
		}
	}
	else if (event->key() == Qt::Key_Down)
	{
		if (pos().y() < GAME_VIEW_H / 2 - AIR_CRAFT_H / 2)
		{
			setPos(x(), y() + AIR_CRAFT_MOVE_STEP);
		}
	}
	scene()->update();
	QGraphicsItem::keyPressEvent(event);
}
