#pragma once

#include "base.h"

#define CHARACTER_SPEED	200.f
#define RAFT_SPEED		100.f

class IBroadcaster;

enum ECollision { NONE, OVERLAP, HORIZONTAL_OPPOSITE, HORIZONTAL_FIRST, HORIZONTAL_SECOND, VERTICAL_OPPOSITE, VERTICAL_FIRST, VERTICAL_SECOND, DIAGONAL };
enum EPriority { NO_PRIORITY, LOW, MEDIUM, HIGH, TOP };
enum EMenu { MAIN, LEVEL, LANGUAGE };
enum ESelection { NONE_SELECTED, FIRST, SECOND, THIRD };

struct CMessage {
	virtual ~CMessage() {}
};

struct CMessageConsumable : virtual public CMessage
{
	bool consumed = false;
};

struct CMessageNotSelf : virtual public CMessage
{
	IBroadcaster* pointerToCreator = nullptr;
};

struct CMessageConsumableNotSelf : public CMessageConsumable, public CMessageNotSelf {};

struct CMessageUpdate : public CMessage
{
	float deltaTime = 0.f;
};

struct CMessageUpdateInput : public CMessageUpdate {};

struct CMessageUpdateLogic : public CMessageUpdate {};

struct CMessageUpdateRender : public CMessageUpdate
{
	EPriority renderPriority;
};

struct CMessageUpdateWithReturn : public CMessageUpdate
{
	ESelection changeLevel = NONE_SELECTED;
	uint32_t selectedLanguage = 2;
};

struct CMessageChangeVelocity : public CMessage
{
	vec2 velocity = { 0.f, 0.f };
};

struct CMessageChangePosition : public CMessage
{
	vec2 position = { 0.f, 0.f };
};

struct CMessageDamage : public CMessage
{
	bool bIsBeingDamaged = false;
};

struct CMessageGetDamage : public CMessageConsumable
{
	bool bIsBeingDamaged = false;
};

struct CMessageGetCollision : public CMessageConsumableNotSelf
{
	vec2 position = { 0.f, 0.f };
	ECollision collisionType = NONE;
	IBroadcaster* otherCollider = nullptr;
};

struct CMessageGetSize : public CMessageConsumable
{
	vec2 size = { 0.f, 0.f };
};

struct CMessageGetVelocity : public CMessageConsumable
{
	vec2 velocity = { 0.f, 0.f };
};

struct CMessageGetPosition : public CMessageConsumable
{
	vec2 position = { 0.f, 0.f };
};

struct CMessageCollisionNotifier : public CMessage
{
	ECollision collisionType = NONE;
};

struct CMessagePause : public CMessage {};

struct CMessageDestroy : public CMessage
{
	IBroadcaster* objective = nullptr;
};

struct CMessageSetHealth : public CMessage
{
	float health = 0.f;
};

struct CMessageSpawn : public CMessage {};

struct CMessageSpawnConsumable : public CMessageConsumable
{
	IBroadcaster* objective = nullptr;
};

struct CMessageGetCollisionable : CMessageConsumable
{
	bool bIsCollisionable = false;
};

struct CMessageActivateCollision : public CMessage {};

struct CMessageDestroyPlayer : public CMessage {};

struct CMessageEndGame : public CMessage {};

struct CMessageChangeText : public CMessage
{
	std::array<std::array<std::string, 3>, 3> texts;
	uint32_t counter = 0;
};

struct CMessageEnter : public CMessage {};

struct CMessageChangeState : public CMessageConsumable
{
	IBroadcaster* selector = nullptr;
};

struct CMessageGetState : public CMessageConsumable
{
	EMenu state = MAIN;
};

struct CMessageNotifyState : public CMessage
{
	EMenu state = MAIN;
};
