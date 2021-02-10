#include "pch.h"
#include "Util.h"


namespace Util
{
	std::string boolToString(bool b)
	{
		return b ? "true" : "false";
	}

	char* stringToCharPtr(std::string str)
	{
		char* cStr = new char[str.length() + 1];
		strcpy(cStr, str.c_str());

		return cStr;
	}


	Vector3 Subtract(Vector3 src, Vector3 dst)
	{
		Vector3 diff;
		diff.x = src.x - dst.x;
		diff.y = src.y - dst.y;
		diff.z = src.z - dst.z;
		return diff;
	}

	float Magnitude(Vector3 vec)
	{
		return sqrtf(vec.x * vec.x + vec.y * vec.y + vec.z * vec.z);
	}

	float Distance(Vector3 src, Vector3 dst)
	{
		Vector3 diff = Subtract(src, dst);
		return Magnitude(diff);
	}
	
	Vector3 CalcAngle(Vector3 src, Vector3 dst)
	{

		float dx = dst.x - src.x;
		float dy = dst.y - src.y;
		double angleYaw = atan2(dy, dx) * 180 / PI;
		
		double distance = sqrt(dx * dx + dy * dy);
		float dz = dst.z - src.z;
		double anglePitch = atan2(dz, distance) * 180 / PI;
		
		Vector3 angle;
		angle.x = (float)angleYaw + 90;
		angle.y = (float)anglePitch;
		return angle;
	}


	bool WorldToScreen(Vector3 pos, Vector3& screen, float matrix[16], int windowWidth, int windowHeight)
	{
		//Matrix-vector Product, multiplying world(eye) coordinates by projection matrix = clipCoords
		Vector4 clipCoords;
		clipCoords.x = pos.x * matrix[0] + pos.y * matrix[4] + pos.z * matrix[8] + matrix[12];
		clipCoords.y = pos.x * matrix[1] + pos.y * matrix[5] + pos.z * matrix[9] + matrix[13];
		clipCoords.z = pos.x * matrix[2] + pos.y * matrix[6] + pos.z * matrix[10] + matrix[14];
		clipCoords.w = pos.x * matrix[3] + pos.y * matrix[7] + pos.z * matrix[11] + matrix[15];

		if (clipCoords.w < 0.1f)
			return false;

		//perspective division, dividing by clip.W = Normalized Device Coordinates
		Vector3 NDC;
		NDC.x = clipCoords.x / clipCoords.w;
		NDC.y = clipCoords.y / clipCoords.w;
		NDC.z = clipCoords.z / clipCoords.w;

		screen.x = (windowWidth / 2 * NDC.x) + (NDC.x + windowWidth / 2);
		screen.y = -(windowHeight / 2 * NDC.y) + (NDC.y + windowHeight / 2);
		return true;
	}
}