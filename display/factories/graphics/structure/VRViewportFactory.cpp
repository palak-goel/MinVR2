/*
 * Copyright Regents of the University of Minnesota, 2016.  This software is released under the following license: http://opensource.org/licenses/
 * Source code originally developed at the University of Minnesota Interactive Visualization Lab (http://ivlab.cs.umn.edu).
 *
 * Code author(s):
 * 		Dan Orban (dtorban)
 */

#include <display/factories/graphics/structure/VRViewportFactory.h>
#include "display/graphics/structure/VRViewportNode.h"
#include "display/graphics/structure/VRTileNode.h"
#include "display/graphics/VRGraphicsRenderNode.h"
#include "display/scope/VRScopeNode.h"

namespace MinVR {

VRViewportFactory::VRViewportFactory(VRSystem* vrSystem, VRDisplayFactory* innerFactory) : m_vrSystem(vrSystem), m_innerFactory(innerFactory) {
}

VRViewportFactory::~VRViewportFactory() {
	delete m_innerFactory;
}

VRDisplay* VRViewportFactory::create(VRDataIndex& config,
		const std::string nameSpace) {

	VRDisplay* display = m_innerFactory->create(config, nameSpace);
	bool createdScope = false;
	if (!display)
	{
		std::cout << "Created scope" << std::endl;
		VRScopeNode<VRGraphicsWindowChild, VRGraphicsWindowChild>* scope = new VRScopeNode<VRGraphicsWindowChild, VRGraphicsWindowChild>();
		VRDisplayNode::createChildren<VRScopeNode<VRGraphicsWindowChild, VRGraphicsWindowChild>, VRGraphicsWindowChild>(scope, m_vrSystem->getDisplayFactory(), config, nameSpace);
		display = scope;
		createdScope = true;
	}

	VRHasDisplayChildren<VRGraphicsWindowChild>* displayWithChildren = dynamic_cast<VRHasDisplayChildren<VRGraphicsWindowChild>*>(display);

	if (displayWithChildren)
	{
		VRDisplayNode* displayNode = dynamic_cast<VRDisplayNode*>(display);
		int startChildren = displayNode->getChildren().size();

		VRTile tile;
		VRTileNode* tileNode = NULL;
		if (tile.read(config, nameSpace + "/tile", ""))
		{
			std::cout << "Created tile" << std::endl;
			tileNode = new VRTileNode(tile);
			displayWithChildren->insertChild(tileNode, 0);
		}

		VRRect viewport;
		VRViewportNode* viewportNode = NULL;
		if (viewport.read(config, nameSpace + "/viewport", ""))
		{
			std::cout << "Created viewport" << std::endl;
			viewportNode = new VRViewportNode(viewport);
			displayWithChildren->insertChild(viewportNode, 0);
		}

		if (startChildren == 0 && displayNode->getChildren().size() > startChildren)
		{
			displayWithChildren->addChild(new VRGraphicsRenderNode());
			std::cout << "Created renderer" << std::endl;
		}

		if (displayNode->getChildren().size() == 0 && createdScope) {
			std::cout << "destroyed scope" << std::endl;
			delete display;
			return NULL;
		}
	}

	return display;
}

} /* namespace MinVR */