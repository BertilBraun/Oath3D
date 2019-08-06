#include "Common/Common.h"
#include "BulletDebugDraw.h"

namespace Oath {
	namespace Render {

		BulletDebugDraw::BulletDebugDraw()
			:m_debugMode(0)
		{

		}

		void    BulletDebugDraw::drawLine(const btVector3& from, const btVector3& to, const btVector3& color)
		{
			//      if (m_debugMode > 0)
			{
				float tmp[6] = { from.getX(), from.getY(), from.getZ(),
							  to.getX(), to.getY(), to.getZ() };

				glPushMatrix();
				{
					glColor4f(color.getX(), color.getY(), color.getZ(), 1.0f);
					glVertexPointer(3,
						GL_FLOAT,
						0,
						&tmp);

					glPointSize(5.0f);
					glDrawArrays(GL_POINTS, 0, 2);
					glDrawArrays(GL_LINES, 0, 2);
				}
				glPopMatrix();
			}
		}

		void    BulletDebugDraw::setDebugMode(int debugMode)
		{
			m_debugMode = debugMode;
		}

		void    BulletDebugDraw::draw3dText(const btVector3& location, const char* textString)
		{
			//glRasterPos3f(location.x(),  location.y(),  location.z());
			//BMF_DrawString(BMF_GetFont(BMF_kHelvetica10),textString);
		}

		void    BulletDebugDraw::reportErrorWarning(const char* warningString)
		{
			LOGWARN(warningString);
		}

		void    BulletDebugDraw::drawContactPoint(const btVector3& pointOnB, const btVector3& normalOnB, btScalar distance, int lifeTime, const btVector3& color)
		{
			{
				//btVector3 to=pointOnB+normalOnB*distance;
				//const btVector3&from = pointOnB;
				//glColor4f(color.getX(), color.getY(), color.getZ(), 1.0f);   

				//GLDebugDrawer::drawLine(from, to, color);

				//glRasterPos3f(from.x(),  from.y(),  from.z());
				//char buf[12];
				//sprintf(buf," %d",lifeTime);
				//BMF_DrawString(BMF_GetFont(BMF_kHelvetica10),buf);
			}
		}
	}
}