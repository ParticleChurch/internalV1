#pragma once

class CEventInfo {
public:
	enum {
		EVENT_INDEX_BITS = 8,
		EVENT_DATA_LEN_BITS = 11,
		MAX_EVENT_DATA = 192,  // ( 1<<8 bits == 256, but only using 192 below )
	};

	// 0 implies not in use
	short					m_class_id;
	float					m_fire_delay;
	const void* m_send_table;
	const ClientClass* m_client_class;//void*
	int						m_bits;
	uint8_t* m_data;
	int						m_flags;
	//PAD(0x1C);
	std::byte pad[0x16];
	CEventInfo* m_next;
};

class ClientState {

	class CClockDriftManager {

	public:
		float clock_offsets[0x10];
		uint32_t cur_clock_offset;
		uint32_t m_server_tick;
		uint32_t m_client_tick;
	};

public:
	char			pad000[0x9C];
	NetChannel* m_net_channel;
	int				m_nChallengeNr;
	char			pad001[0x4];
	double			m_connect_time;
	int				m_retry_number;
	char			pad002[0x54];
	int				m_nSignonState;
	char			pad003[0x4];
	double			m_flNextCmdTime;
	int				m_nServerCount;
	int				m_current_sequence;
	char			pad004[0x8];

	CClockDriftManager clock_drift_mgr;

	int				m_delta_tick;
	char			pad005[0x4];
	int				m_nViewEntity;
	int				m_nPlayerSlot;
	bool			m_bPaused;
	char			pad006[0x3];
	char			m_szLevelName[0x104];
	char			m_szLevelNameShort[0x28];
	char			pad007[0xD4];
	int				m_nMaxClients;
	char			pad008[0x4994];
	int				oldtickcount;
	float			m_tickRemainder;
	float			m_frameTime;
	int				m_last_outgoing_command;
	int				m_choked_commands;
	int				m_last_command_ack;
	int				m_last_server_tick;
	int				m_command_ack;
	int				m_nSoundSequence;
	int				m_last_progress_percent;
	bool			m_is_hltv;
	char			pad009[0x4B];
	Vec			m_viewangles;
	char			pad010[0xCC];
	CEventInfo* m_events;
};