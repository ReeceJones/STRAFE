#include "Math.h"
CMath g_Math;

void inline CMath::SinCos(float Radians, float *Sine, float *Cosine)
{
	*Sine = sin(Radians);
	*Cosine = cos(Radians);
}

void CMath::AngleVectors(const QAngle &Angles, Vector &Forward)
{
	Assert(s_bMathlibInitialized);
	Assert(forward);

	float sp, sy, cp, cy;

	CMath::SinCos(DEG2RAD(Angles[YAW]), &sy, &cy);
	CMath::SinCos(DEG2RAD(Angles[PITCH]), &sp, &cp);

	Forward.x = cp * cy;
	Forward.y = cp * sy;
	Forward.z = -sp;
}

float CMath::GetFov(const QAngle &LocalAngle, const QAngle &DestinationAngle)
{
	QAngle delta = DestinationAngle - LocalAngle;
	delta.Normalize();

	return sqrtf(powf(delta.x, 2.0f) + powf(delta.y, 2.0f));
}

void CMath::VectorAngles(const Vector &Forward, QAngle &Angles)
{
	if (Forward[1] == 0.0f && Forward[0] == 0.0f)
	{
		Angles[0] = (Forward[2] > 0.0f) ? 270.0f : 90.0f;
		Angles[1] = 0.0f;
	}
	else
	{
		Angles[0] = atan2(-Forward[2], Forward.Length2D()) * -180 / M_PI;
		Angles[1] = atan2(Forward[1], Forward[0]) * 180 / M_PI;

		if (Angles[1] > 90)
			Angles[1] -= 180;
		else if (Angles[1] < 90)
			Angles[1] += 180;
		else if (Angles[1] == 90)
			Angles[1] = 0;
	}

	Angles[2] = 0.0f;
}

QAngle CMath::CalcAngle(Vector Source, Vector Distance)
{
	QAngle angles;
	Vector delta = Source - Distance;

	CMath::VectorAngles(delta, angles);

	delta.Normalize();

	return angles;
}

void CMath::VectorTransform(Vector& in1, matrix3x4a_t& in2, Vector &Out)
{
	Out.x = in1.Dot(in2.m_flMatVal[0]) + in2.m_flMatVal[0][3];
	Out.y = in1.Dot(in2.m_flMatVal[1]) + in2.m_flMatVal[1][3];
	Out.z = in1.Dot(in2.m_flMatVal[2]) + in2.m_flMatVal[2][3];
}

void CMath::AngleVectors(const QAngle &angles, Vector *forward, Vector *right, Vector *up)
{
	float sr, sp, sy, cr, cp, cy;

	SinCos(DEG2RAD(angles[1]), &sy, &cy);
	SinCos(DEG2RAD(angles[0]), &sp, &cp);
	SinCos(DEG2RAD(angles[2]), &sr, &cr);

	if (forward)
	{
		forward->x = cp*cy;
		forward->y = cp*sy;
		forward->z = -sp;
	}

	if (right)
	{
		right->x = (-1 * sr*sp*cy + -1 * cr*-sy);
		right->y = (-1 * sr*sp*sy + -1 * cr*cy);
		right->z = -1 * sr*cp;
	}

	if (up)
	{
		up->x = (cr*sp*cy + -sr*-sy);
		up->y = (cr*sp*sy + -sr*cy);
		up->z = cr*cp;
	}
}