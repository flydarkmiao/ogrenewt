#include "ConveyorMatCallback.h"
#include "ConveyorBelt.h"

ConveyorMatCallback::ConveyorMatCallback( int conveyorID ) : OgreNewt::ContactCallback()
{
	mConveyorID = conveyorID;
}

ConveyorMatCallback::~ConveyorMatCallback(void)
{
}

int ConveyorMatCallback::onAABBOverlap( OgreNewt::Body* body0, OgreNewt::Body* body1, int threadIndex )
{
    // if none of the bodies is the belt, return 0 to remove contact
    if( body0->getType() == mConveyorID || body1->getType() == mConveyorID )
        return 1;

    return 0;
}

void ConveyorMatCallback::contactsProcess( OgreNewt::ContactJoint &contactJoint, Ogre::Real timeStep, int threadIndex )
{
    OgreNewt::Body* body0 = contactJoint.getBody0();
    OgreNewt::Body* body1 = contactJoint.getBody1();
	// first, find which body represents the conveyor belt!
	ConveyorBelt* belt = NULL;
	OgreNewt::Body* object = NULL;

	if (body0->getType() == mConveyorID)
	{
#ifndef OGRENEWT_NO_OGRE_ANY
		belt = Ogre::any_cast<ConveyorBelt*>(body0->getUserData());
#else
		belt = (ConveyorBelt*)body0->getUserData();
#endif
		object = body1;
	}

	if (body1->getType() == mConveyorID)
	{
#ifndef OGRENEWT_NO_OGRE_ANY
		belt = Ogre::any_cast<ConveyorBelt*>(body1->getUserData());
#else
		belt = (ConveyorBelt*)body1->getUserData();
#endif
		object = body0;
	}

	if (belt)
    {
	    // okay, found the belt... let's adjust the collision based on this.
    	Ogre::Vector3 thedir = belt->getGlobalDir();

        for( OgreNewt::Contact contact = contactJoint.getFirstContact(); contact; contact = contact.getNext() )
        {
	        contact.rotateTangentDirections( thedir );
            Ogre::Vector3 contactPos, contactNorm;
            contact.getPositionAndNormal(contactPos, contactNorm);
            Ogre::Vector3 objectPos;
            Ogre::Quaternion objectOri;
            object->getPositionOrientation(objectPos, objectOri);
            Ogre::Vector3 objectContactPointVel = object->getVelocity() + (contactPos - objectPos)*object->getOmega();
    	    Ogre::Real result_accel = belt->getSpeed() - thedir.dotProduct(objectContactPointVel);
            result_accel *= 10; // looks nicer
	        contact.setTangentAcceleration( result_accel, 0 );
        }
    }
}

