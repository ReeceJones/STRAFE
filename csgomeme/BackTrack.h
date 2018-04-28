//#pragma once
//#include "Entity.h"
//#include "Vector.h"
//#include "Offsets.h"
//
//struct CTickRecord;
//
//struct CValidTick {
//	explicit operator CTickRecord() const;
//
//	explicit operator bool() const noexcept {
//		return m_flSimulationTime > 0.f;
//	}
//
//	float m_flPitch = 0.f;
//	float m_flYaw = 0.f;
//	float m_flSimulationTime = 0.f;
//	CBaseEntity* m_pEntity = nullptr;
//};
//
//struct CTickRecord {
//	CTickRecord() {}
//	CTickRecord(CBaseEntity* Entity) {
//		m_flLowerBodyYawTarget = Entity->GetLowerBodyYawTarget();
//		m_angEyeAngles = *Entity->GetEyeAngles();
//		m_flCycle = Entity->GetCycle();
//		m_flSimulationTime = Entity->GetSimulationTime();
//		m_nSequence = Entity->GetSequence();
//		m_vecOrigin = Entity->GetOrigin();
//		m_vecVelocity = Entity->GetVelocity();
//		m_flPoseParameter = Entity->GetPoseParameters();
//		m_angAbsAngles = Entity->GetAbsAngles();
//		m_vecAbsOrigin = Entity->GetAbsOrigin();
//		tickcount = 0;
//	}
//
//	explicit operator bool() const noexcept {
//		return m_flSimulationTime > 0.f;
//	}
//
//	bool operator>(const CTickRecord& others) {
//		return (m_flSimulationTime > others.m_flSimulationTime);
//	}
//	bool operator>=(const CTickRecord& others) {
//		return (m_flSimulationTime >= others.m_flSimulationTime);
//	}
//	bool operator<(const CTickRecord& others) {
//		return (m_flSimulationTime < others.m_flSimulationTime);
//	}
//	bool operator<=(const CTickRecord& others) {
//		return (m_flSimulationTime <= others.m_flSimulationTime);
//	}
//	bool operator==(const CTickRecord& others) {
//		return (m_flSimulationTime == others.m_flSimulationTime);
//	}
//
//	float m_flLowerBodyYawTarget = 0.f;
//	QAngle m_angEyeAngles = QAngle(0, 0, 0);
//	float m_flCycle = 0.f;
//	float m_flSimulationTime = 0.f;
//	int m_nSequence = 0;
//	Vector m_vecOrigin = Vector(0, 0, 0);
//	Vector m_vecAbsOrigin = Vector(0, 0, 0);
//	Vector m_vecVelocity = Vector(0, 0, 0);
//	std::array<float, 24> m_flPoseParameter = {};
//	QAngle m_angAbsAngles = QAngle(0, 0, 0);
//	CValidTick validtick;
//	int tickcount = 0;
//};
//
//inline CValidTick::operator CTickRecord() const {
//	CTickRecord rec(m_pEntity);
//	rec.m_angEyeAngles.x = this->m_flPitch;
//	rec.m_angEyeAngles.y = this->m_flYaw;
//	rec.m_flSimulationTime = this->m_flSimulationTime;
//	return rec;
//}