#include "cxGameView.h"

cxGameView::cxGameView(QWidget *parent)
	: QGraphicsView(parent)
{
	setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	setFixedSize(GAME_VIEW_W, GAME_VIEW_H);

	m_pScene = new QGraphicsScene(this);
	m_pScene->setSceneRect(-GAME_VIEW_W / 2., -GAME_VIEW_H / 2, GAME_VIEW_W, GAME_VIEW_H);
	setScene(m_pScene);
	createAircraft();

	m_pSpawnEnemyTimer = new QTimer(this);
	QObject::connect(m_pSpawnEnemyTimer, &QTimer::timeout, this, &cxGameView::onSpawnEnemy);
}

cxGameView::~cxGameView()
{
}

void cxGameView::createAircraft()
{
	m_pAircraft = new Aircraft(this);
	m_pAircraft->setPos(0, GAME_VIEW_H / 2 - AIR_CRAFT_H / 2);
	m_pAircraft->setFlag(QGraphicsItem::ItemIsFocusable);
	m_pAircraft->setFocus();
	m_pScene->addItem(m_pAircraft);
	connect(m_pAircraft, &Aircraft::sigFinished, [=]() {
		qDebug() << "finished";
		stop();
	});
}

void cxGameView::start()
{
	if (!m_pSpawnEnemyTimer->isActive())
	{
		m_state = Running;
		m_pSpawnEnemyTimer->start(ENEMY_CREATE_FREQ);
		QList<QGraphicsItem*> pItems = m_pScene->items();
		for (QGraphicsItem* pItem : pItems)
		{
			AbstractCxItem * pCxItem = qgraphicsitem_cast<AbstractCxItem *>(pItem);
			if (pCxItem)
			{
				pCxItem->start();
			}
		}
	}
}

void cxGameView::pause()
{
	m_state = Paused;
	m_pSpawnEnemyTimer->stop();
	QList<QGraphicsItem*> pItems = m_pScene->items();
	for (QGraphicsItem* pItem : pItems)
	{
		AbstractCxItem * pCxItem = qgraphicsitem_cast<AbstractCxItem *>(pItem);
		if (pCxItem)
		{
			pCxItem->pause();
		}
	}
}
void cxGameView::stop()
{
	m_state = Stopped;
	m_pSpawnEnemyTimer->stop();
	QList<QGraphicsItem*> pItems = m_pScene->items();
	for (QGraphicsItem* pItem : pItems)
	{
		AbstractCxItem * pCxItem = qgraphicsitem_cast<AbstractCxItem *>(pItem);
		if (pCxItem)
		{
			pCxItem->stop();
		}
	}
	scene()->update();
}

void cxGameView::onSpawnEnemy()
{
	Enemy * pEnemy = new Enemy(this);
	m_pScene->addItem(pEnemy);
}

void cxGameView::keyPressEvent(QKeyEvent *event)
{
	if (event->key() == Qt::Key_Space)
	{
		switch (m_state)
		{
		case cxGameView::Running:
			pause();
			break;
		case cxGameView::Paused:
			start();
			break;
		case cxGameView::Stopped:
			break;
		default:
			break;
		}
	}
	else if (event->key() == Qt::Key_Return)
	{
		start();
	}
	else if (event->key() == Qt::Key_Escape)
	{
		stop();
	}
	QGraphicsView::keyPressEvent(event);
}