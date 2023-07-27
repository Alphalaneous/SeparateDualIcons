#ifndef __GJROBOTSPRITE_H__
#define __GJROBOTSPRITE_H__

#include <gd.h>

namespace gd {

	class CCAnimatedSprite;

	class GDH_DLL GJRobotSprite : public CCAnimatedSprite {
		PAD(8); // 0x244
		cocos2d::ccColor3B m_secondaryColor; // 0x24c

	public:
		void updateFrame(int frameID) {
			reinterpret_cast<void(__thiscall*)(GJRobotSprite*, int)>(base + 0x146700)(this, frameID);
		}

	};
}

#endif
