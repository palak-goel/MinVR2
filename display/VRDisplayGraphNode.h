#ifndef VRDISPLAYGRAPHNODE_H_
#define VRDISPLAYGRAPHNODE_H_

namespace MinVR {

class VRDisplayGraphNode
{
public:
	virtual ~VRDisplayGraphNode() {}

	virtual void render() = 0;
};

}

#endif
