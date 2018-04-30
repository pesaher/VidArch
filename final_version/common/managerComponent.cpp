#include "managerComponent.h"

 CManagerComponent::CManagerComponent(IBroadcaster* globalBroadcaster, IBroadcaster* localBroadcaster) : IManagerComponent(globalBroadcaster, localBroadcaster)
 {
	 m_globalBroadcaster = globalBroadcaster;
	 m_localBroadcaster = localBroadcaster;
 }

 void CManagerComponent::manageMessage(CMessage* message)
 {
	 // Update input, logic and rendering
	 CMessageUpdateInput* inputMessage = dynamic_cast<CMessageUpdateInput*>(message);
	 if (inputMessage)
		 updateInput(inputMessage->deltaTime);
	 CMessageUpdateLogic* logicMessage = dynamic_cast<CMessageUpdateLogic*>(message);
	 if (logicMessage)
		 updateLogic(logicMessage->deltaTime);
	 CMessageUpdateRender* renderMessage = dynamic_cast<CMessageUpdateRender*>(message);
	 if (renderMessage)
		 updateRender(renderMessage->deltaTime, renderMessage->renderPriority);
 }

 void CManagerComponent::broadcastMessage(CMessage* message)
 {
	 manageMessage(message);
 }
