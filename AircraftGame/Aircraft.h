#pragma once


#include "AbstractCxItem.h"

class Aircraft : public AbstractCxItem
{
	Q_OBJECT

public:
	Aircraft(QObject *parent = nullptr);
	~Aircraft();
public:
	void start() override;
	void pause() override;
	void stop() override;

	QRectF boundingRect() const override;
	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
private slots:
void onCreateBullet();

protected:
	void keyPressEvent(QKeyEvent *event) override;

private:
	QTimer* m_pTimer = nullptr;
	bool m_isRunning = false;
	bool m_isCrash = false;
signals:
	void sigFinished();
};
