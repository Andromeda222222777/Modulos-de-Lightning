/**
 * Copyright - xWhitey, 2023.
 * CTimeMachineModule.hpp - description
 *
 * Lightning a.k.a. lightning.tech (Sven Co-op) header file
 * Authors: xWhitey. Refer to common.hpp file for dependencies and their authors.
 * Do not delete this comment block. Respect others' work!
 */

#ifdef CSTRESSERMODULE_HPP_RECURSE_GUARD
#error Recursive header files inclusion detected in CStresserModule.hpp
#else CSTRESSERMODULE_HPP_RECURSE_GUARD

#define CSTRESSERMODULE_HPP_RECURSE_GUARD

#ifndef CSTRESSERMODULE_HPP_GUARD
#define CSTRESSERMODULE_HPP_GUARD
#pragma once

#ifdef __cplusplus

typedef struct CStresserModule : CModule {
	CStresserModule();

	virtual void OnEnable() override;
	virtual void OnDisable() override;
	virtual void OnEvent(const ISimpleEvent*) override;
	virtual const char* GetTag() override;

	CListValue* m_pMode = nullptr;
	CExtraValues* m_pConfigureWnd;
	CIntegerValue* m_pStrength = nullptr;
	CIntegerValue* m_pChargeFor = nullptr;
	CBoolValue* m_pBlockMovements = nullptr;
	CBoolValue* m_pDontSendNormalCmds = nullptr;
	CBoolValue* m_pRandomLerpMsec = nullptr;
	CFloatValue* m_pGameSpeed = nullptr;
	CIntegerValue* m_pGameSpeedChargeFor = nullptr;
	CFloatValue* m_pAnarchyGameSpeed = nullptr;
	CFloatValue* m_pAnarchyBaseSpeed = nullptr;
	CIntegerValue* m_pAnarchyChargeFor = nullptr;
	CFloatValue* m_pSpeed = nullptr;

	int m_iChargeCounter = 0;
	int m_iGameSpeedChargeForCounter = 0;
} CStresserModule;

using CStresserModule = struct CStresserModule;

#else !__cplusplus
#error C++ compiler required to compile CStresserModule.hpp
#endif __cplusplus

#endif CSTRESSERMODULE_HPP_GUARD

#undef CSTRESSERMODULE_HPP_RECURSE_GUARD
#endif CSTRESSERMODULE_HPP_RECURSE_GUARD
