#pragma once

#include "AbstractCxItem.h"

class Enemy : public AbstractCxItem
{
	Q_OBJECT

public:
	Enemy(QObject *parent = nullptr);
	~Enemy();
public:
	void start() override;
	void pause() override;
	void stop() override;

	QRectF boundingRect() const override;
	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
private:
	void slefDel();
private slots:
	void onMove();
private:
	QTimer* m_pTimer = nullptr;
	bool m_isRunning = false;
	qreal m_rotation = 0;
};
