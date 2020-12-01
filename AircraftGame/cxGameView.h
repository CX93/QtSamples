#pragma once

#include "common.h"
#include "Aircraft.h"
#include "Enemy.h"
#include "AbstractCxItem.h"

class cxGameView : public QGraphicsView
{
	Q_OBJECT

public:
	cxGameView(QWidget *parent = nullptr);
	~cxGameView();
private:
	void createAircraft();

	void start();
	void pause();
	void stop();

	private slots:
	void onSpawnEnemy();

protected:
	void keyPressEvent(QKeyEvent *event) override;
private:
	QGraphicsScene* m_pScene = nullptr;
	Aircraft* m_pAircraft = nullptr;
private:
	QTimer* m_pSpawnEnemyTimer = nullptr;
private:
	enum State { Running, Paused, Stopped };
	State m_state = Stopped;

};
