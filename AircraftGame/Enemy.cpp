#include "Enemy.h"

Enemy::Enemy(QObject *parent)
	: AbstractCxItem(parent)
{
	int num = rand() % (GAME_VIEW_W - ENEMY_W);

	setPos(rand() % 2 ? num / 2 : -num / 2, -GAME_VIEW_H / 2);

	m_pTimer = new QTimer(this);
	connect(m_pTimer, &QTimer::timeout, this, &Enemy::onMove);
	start();
	setData(0, "Enemy");
}

Enemy::~Enemy()
{
	qDebug() << __FUNCTION__;
}

void Enemy::start()
{
	qDebug() << __FUNCTION__;
	m_isRunning = true;
	if (!m_pTimer->isActive())
	{
		m_pTimer->start(ENEMY_MOVE_FREQ);
	}
}

void Enemy::pause()
{
	qDebug() << __FUNCTION__;
	m_isRunning = false;
	m_pTimer->stop();
}

void Enemy::stop()
{
	qDebug() << __FUNCTION__;
	m_isRunning = false;
	slefDel();
}

QRectF Enemy::boundingRect() const
{
	return QRectF(-ENEMY_W / 2, -ENEMY_H / 2, ENEMY_W, ENEMY_H);

}
void Enemy::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option)
    Q_UNUSED(widget)
	if (m_isRunning)
	{
		for (QGraphicsItem * pItem : scene()->collidingItems(this))
		{
			if (pItem->data(0).toString() == "Bullet")
			{
				slefDel();
				return;
			}
		}
	}	
	painter->setRenderHint(QPainter::Antialiasing);
	painter->rotate(m_rotation);
	painter->setBrush(Qt::darkGray);
	painter->drawRect(boundingRect());
}

void Enemy::slefDel()
{
	m_pTimer->stop();
	scene()->removeItem(this);
	deleteLater();
}

void Enemy::onMove()
{
	setPos(x(), y() + ENEMY_MOVE_STEP);
	if (pos().y() + boundingRect().height() > scene()->height())
	{
		slefDel();
	}
	else
	{
		scene()->update();
	}
	m_rotation += 2;
}
