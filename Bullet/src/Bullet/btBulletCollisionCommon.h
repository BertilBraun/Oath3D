/*
Bullet Continuous Collision Detection and Physics Library
Copyright (c) 2003-2006 Erwin Coumans  http://continuousphysics.com/Bullet/

This software is provided 'as-is', without any express or implied warranty.
In no event will the authors be held liable for any damages arising from the use of this software.
Permission is granted to anyone to use this software for any purpose, 
including commercial applications, and to alter it and redistribute it freely, 
subject to the following restrictions:

1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
3. This notice may not be removed or altered from any source distribution.
*/

#ifndef BULLET_COLLISION_COMMON_H
#define BULLET_COLLISION_COMMON_H

///Common headerfile includes for Bullet Collision Detection

///Bullet's btCollisionWorld and btCollisionObject definitions
#include "Bullet/BulletCollision/CollisionDispatch/btCollisionWorld.h"
#include "Bullet/BulletCollision/CollisionDispatch/btCollisionObject.h"

///Collision Shapes
#include "Bullet/BulletCollision/CollisionShapes/btBoxShape.h"
#include "Bullet/BulletCollision/CollisionShapes/btSphereShape.h"
#include "Bullet/BulletCollision/CollisionShapes/btCapsuleShape.h"
#include "Bullet/BulletCollision/CollisionShapes/btCylinderShape.h"
#include "Bullet/BulletCollision/CollisionShapes/btConeShape.h"
#include "Bullet/BulletCollision/CollisionShapes/btStaticPlaneShape.h"
#include "Bullet/BulletCollision/CollisionShapes/btConvexHullShape.h"
#include "Bullet/BulletCollision/CollisionShapes/btTriangleMesh.h"
#include "Bullet/BulletCollision/CollisionShapes/btConvexTriangleMeshShape.h"
#include "Bullet/BulletCollision/CollisionShapes/btBvhTriangleMeshShape.h"
#include "Bullet/BulletCollision/CollisionShapes/btScaledBvhTriangleMeshShape.h"
#include "Bullet/BulletCollision/CollisionShapes/btTriangleMeshShape.h"
#include "Bullet/BulletCollision/CollisionShapes/btTriangleIndexVertexArray.h"
#include "Bullet/BulletCollision/CollisionShapes/btCompoundShape.h"
#include "Bullet/BulletCollision/CollisionShapes/btTetrahedronShape.h"
#include "Bullet/BulletCollision/CollisionShapes/btEmptyShape.h"
#include "Bullet/BulletCollision/CollisionShapes/btMultiSphereShape.h"
#include "Bullet/BulletCollision/CollisionShapes/btUniformScalingShape.h"

///Narrowphase Collision Detector
#include "Bullet/BulletCollision/CollisionDispatch/btSphereSphereCollisionAlgorithm.h"

//#include "Bullet/BulletCollision/CollisionDispatch/btSphereBoxCollisionAlgorithm.h"
#include "Bullet/BulletCollision/CollisionDispatch/btDefaultCollisionConfiguration.h"

///Dispatching and generation of collision pairs (broadphase)
#include "Bullet/BulletCollision/CollisionDispatch/btCollisionDispatcher.h"
#include "Bullet/BulletCollision/BroadphaseCollision/btSimpleBroadphase.h"
#include "Bullet/BulletCollision/BroadphaseCollision/btAxisSweep3.h"
#include "Bullet/BulletCollision/BroadphaseCollision/btDbvtBroadphase.h"

///Math library & Utils
#include "Bullet/LinearMath/btQuaternion.h"
#include "Bullet/LinearMath/btTransform.h"
#include "Bullet/LinearMath/btDefaultMotionState.h"
#include "Bullet/LinearMath/btQuickprof.h"
#include "Bullet/LinearMath/btIDebugDraw.h"
#include "Bullet/LinearMath/btSerializer.h"

#endif  //BULLET_COLLISION_COMMON_H
