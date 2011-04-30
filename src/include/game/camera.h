#ifndef CAMERA_H
#define CAMERA_H

typedef struct CameraType {
	int interpolated_cam;
	int interpolated_target;
	int coupled;
	int freedom[3];
	int type;
} CameraType;

typedef struct Camera {
	float cam[3];
	float target[3];
	float movement[4]; /* indices CAM_R, CAM_CHI, CAM_PHI, CAM_PHI_OFFSET */
	float pUser[16]; // static user data
	void *pUserData; // dynamic user data
	int bIsGlancing;
	CameraType type;
} Camera;

typedef enum eCamType { 
	CAM_CIRCLE = 0,
	CAM_FOLLOW,
	CAM_COCKPIT,
	CAM_FREE
} eCamType;

typedef enum ECamAxis { CAM_R = 0, CAM_CHI, CAM_PHI, CAM_PHI_OFFSET } ECamAxis;

void camera_ResetAll(void);
void nextCameraType(void);
void doCameraMovement(void);

float getAngle(int dir);
float getInterpolatedAngle(float t, float last, float now);

#endif
