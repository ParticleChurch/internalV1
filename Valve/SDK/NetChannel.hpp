#pragma once

#define FLOW_OUTGOING	0
#define FLOW_INCOMING	1
#define MAX_FLOWS		2		// in & out

class NetChannel {
public:
	DECLARE_VIRTUAL_METHOD(float, GetLatency, 9, (int flow), (flow));

private:
	std::byte pad1[0x14];

public:
	bool m_processing_messages;		// 0x0014
	bool m_should_delete;			// 0x0015

private:
	std::byte pad2[0x2];

public:
	int m_out_seq;					// 0x0018 last send outgoing sequence number
	int m_in_seq;					// 0x001C last received incoming sequnec number
	int m_out_seq_ack;				// 0x0020 last received acknowledge outgoing sequnce number
	int m_out_rel_state;			// 0x0024 state of outgoing reliable data (0/1) flip flop used for loss detection
	int m_in_rel_state;				// 0x0028 state of incoming reliable data
	int ChokedPackets;				// 0x002C number of choked packets

private:
	std::byte pad3[0x414];					// 0x0030
	
};