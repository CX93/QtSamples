#pragma once

#include <QObject>
#include <QDebug>
#include <QGraphicsItem>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QTimer>
#include <QDateTime>
#include <QKeyEvent>

#define GAME_VIEW_W 400
#define GAME_VIEW_H 400

#define  AIR_CRAFT_W 30
#define  AIR_CRAFT_H 30
#define  AIR_CRAFT_MOVE_STEP 8

#define  BULLET_W 10
#define  BULLET_H 10
#define  BULLET_MOVE_STEP 3
#define  BULLET_MOVE_FREQ  40
#define  BULLET_CREATE_FREQ 500

#define  ENEMY_W 30
#define  ENEMY_H 30
#define  ENEMY_MOVE_STEP 3
#define  ENEMY_MOVE_FREQ  20
#define  ENEMY_CREATE_FREQ 1000