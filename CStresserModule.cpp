#include "StdAfx.h"
#include "CSpeedModule.hpp"
#include "CStresserModule.hpp"
#include "CApplicationSpeedModule.hpp"

CStresserModule::CStresserModule() : CModule("Stresser", "Exploit") {
	m_iGameSpeedChargeForCounter = 0;
	m_pVecValues->push_back(m_pConfigureWnd = Q_new(CExtraValues)("Info", "sc_stresser_info", [](void* _UserData) {
		ImGui::Text("Info\n");
		ImGui::Text("This exploit uses TimeMachine and two types of SpeedHack. Therefore, for this exploit to work.");
		ImGui::Text("It is important that you have more FPS (between +200 or +400 FPS). You should also run the game from Steam using the launch parameter '-nomtex'..");
		ImGui::Text("                                                                                                                                                   ");
		ImGui::Text("Be careful when using the Time Machine + Anarchy Speed.\n");
		}, this));
	m_pVecValues->push_back(m_pMode = Q_new(CListValue)("Mode", "sc_stresser_mode", "Time Machine + Game Speed\0Time Machine + Anarchy Speed\0\0", 0));
	m_pVecValues->push_back(m_pStrength = Q_new(CIntegerValue)("Strength", "sc_stresser_strength", -2500, 2500, 0));
	m_pVecValues->push_back(m_pChargeFor = Q_new(CIntegerValue)("Charge for (in ticks)", "sc_stresser_charge_for", -2500, 2500, 0));
	m_pVecValues->push_back(m_pBlockMovements = Q_new(CBoolValue)("Block movements", "sc_stresser_block_movements", true));
	m_pVecValues->push_back(m_pDontSendNormalCmds = Q_new(CBoolValue)("Don't send normal cmds", "sc_stresser_choke_normal_cmds", false));
	m_pVecValues->push_back(m_pRandomLerpMsec = Q_new(CBoolValue)("Bypass detections", "sc_stresser_bypass_detections", true));

	m_pVecValues->push_back(m_pGameSpeed = Q_new(CFloatValue)("GameSpeed mode - Game speed", "sc_speed_game_spd_speed", 0.1f, 100000.f, 20000.f));
	m_pVecValues->push_back(m_pGameSpeedChargeFor = Q_new(CIntegerValue)("GameSpeed mode - Charge for", "sc_speed_game_spd_charge_for", 1, 256, 1));

	m_pVecValues->push_back(m_pAnarchyGameSpeed = Q_new(CFloatValue)("Anarchy mode - Game speed", "sc_speed_anarchy_speed", 0.1f, 100000.f, 12.f));
	m_pVecValues->push_back(m_pAnarchyBaseSpeed = Q_new(CFloatValue)("Anarchy mode - Base speed", "sc_speed_anarchy_base_speed", 0.1f, 1.0f, 0.2f));
	m_pVecValues->push_back(m_pAnarchyChargeFor = Q_new(CIntegerValue)("Anarchy mode - Charge for", "sc_speed_anarchy_charge_for", 1, 256, 1));

	m_iChargeCounter = 0;

	DefineConCmd("sc_lightning_stresser", CStresserModule);
}

void CStresserModule::OnEnable() {									  
	CModule::OnEnable();
	m_iGameSpeedChargeForCounter = 0;									         

//	ORIG_Cbuf_AddText("Hello World!");
	
//	ORIG_Cbuf_InsertText("Hello World!");

	CCheat::GetCheat()->m_pEventBus->RegisterListener(this);	 
}

void CStresserModule::OnDisable() {
	CCheat::GetCheat()->m_pEventBus->UnregisterListener(this);

	*g_pfNextCmdTime = -1.f;
	m_iGameSpeedChargeForCounter = 0;
	g_flBaseApplicationSpeed = 1.0f;
																		  
	if (g_p_fGameSpeed)
		*g_p_fGameSpeed = 1.0;
  
	*g_pfNextCmdTime = -1.f;
	CModule::OnDisable();
}

void CStresserModule::OnEvent(_In_ const ISimpleEvent* _Event) {
	if (_Event->GetType() == EEventType::kMoveEvent) {
		auto e = static_cast<const CMoveEvent*>(_Event);

		if (!m_pRandomLerpMsec->Get())
			e->m_pCmd->lerp_msec = 0;
		else
			e->m_pCmd->lerp_msec = rand() % (SHRT_MAX - SHRT_MIN + 1) + SHRT_MIN;
		*g_piLastOutgoingCmd = m_pStrength->Get();
	}

	if (_Event->GetType() == EEventType::kPreUpdateEvent) {
		auto e = static_cast<const CPreUpdateEvent*>(_Event);

		e->m_pCmd->lerp_msec = 0;
		*g_piLastOutgoingCmd = m_pStrength->Get();
		
	}

	if (_Event->GetType() == EEventType::kUpdateEvent) {
		auto e = static_cast<const CUpdateEvent*>(_Event);

		if (m_pMode->Get() == 0 /* Time Machine + Game Speed */) {							  
			if (!g_p_fGameSpeed) return;													  
																							  
			if (m_iGameSpeedChargeForCounter < m_pGameSpeedChargeFor->Get()) {				 
				*g_pfNextCmdTime = FLT_MAX;													  
				m_iGameSpeedChargeForCounter++;												  
			}																				 
			else {																			  
				*g_pfNextCmdTime = -1.f;													  
				m_iGameSpeedChargeForCounter = 0;											  
			}																				  
			*g_p_fGameSpeed = static_cast<double>(m_pGameSpeed->Get()) * 1000.0;			  
		} 																					  

		if (m_pMode->Get() == 1 /* Time Machine + Anarchy Speed */) {					      
			if (!g_p_fGameSpeed) return;													  
			g_flBaseApplicationSpeed = m_pAnarchyBaseSpeed->Get();							  
																							  																							 
			if (m_iGameSpeedChargeForCounter < m_pAnarchyChargeFor->Get()) {				  
				*g_pfNextCmdTime = FLT_MAX;													  
				m_iGameSpeedChargeForCounter++;												  
			}																				  
			else {																			  
				*g_pfNextCmdTime = -1.f;													  
				m_iGameSpeedChargeForCounter = 0;											  
			}																				  
																							  
			*g_p_fGameSpeed = static_cast<double>(m_pAnarchyGameSpeed->Get()) * 1000.0;		  
		}

		if (m_pBlockMovements->Get()) {
			e->m_pCmd->forwardmove = 0;
			e->m_pCmd->sidemove = 0;
		}

		if (m_iChargeCounter < m_pChargeFor->Get()) {
			*g_pfNextCmdTime = FLT_MAX;
			m_iChargeCounter++;
		}
		else {
			*g_pfNextCmdTime = -1.f;
			m_iChargeCounter = 0;
		}

		if (m_pDontSendNormalCmds->Get()) {
			*g_pfNextCmdTime = FLT_MAX;
		}

		if (!m_pRandomLerpMsec->Get())
			e->m_pCmd->lerp_msec = 0;
		else
			e->m_pCmd->lerp_msec = rand() % (SHRT_MAX - SHRT_MIN + 1) + SHRT_MIN;
		*g_piLastOutgoingCmd = m_pStrength->Get();
	}

}

const char* CStresserModule::GetTag() {												          
	return m_pMode->GetModeByIndex(m_pMode->Get());				                              
}
