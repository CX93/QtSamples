#pragma once


#include "common.h"

class AbstractCxItem : public QObject, public QGraphicsItem
{
	Q_OBJECT

public:
	AbstractCxItem(QObject *parent = nullptr);
	~AbstractCxItem();
public:
	virtual void start() = 0;
	virtual void pause() = 0;
	virtual void stop() = 0;
};
