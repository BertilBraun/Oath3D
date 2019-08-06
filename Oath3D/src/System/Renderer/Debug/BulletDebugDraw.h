#pragma once

#include "Bullet/LinearMath/btIDebugDraw.h"

namespace Oath {
	namespace Render {

		class BulletDebugDraw : public btIDebugDraw
		{
			int m_debugMode;

		public:
			BulletDebugDraw();

			virtual void   drawLine(const btVector3& from, const btVector3& to, const btVector3& color);

			virtual void   drawContactPoint(const btVector3& PointOnB, const btVector3& normalOnB, btScalar distance, int lifeTime, const btVector3& color);

			virtual void   reportErrorWarning(const char* warningString);

			virtual void   draw3dText(const btVector3& location, const char* textString);

			virtual void   setDebugMode(int debugMode);

			virtual int    getDebugMode() const { return m_debugMode; }

		};
	}
}