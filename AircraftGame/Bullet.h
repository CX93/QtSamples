#pragma once

#include "AbstractCxItem.h"


class Bullet : public AbstractCxItem
{
	Q_OBJECT

public:
	Bullet(QObject *parent = nullptr);
	~Bullet();
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
};
