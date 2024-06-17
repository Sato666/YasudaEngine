//-------------------------------------------------------------------------------------
// Class : FPS
// 
// FPSを管理するクラス
// 
// Yasuda.Aoto  2023.07.14
//-------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include	<thread>
#include	<iostream>
#include	<windows.h>
#include	<timeapi.h>

class FPS {
public:
	FPS() = delete;
	FPS(uint64_t f) {
		m_MicrosecondsPerFrame = static_cast<uint64_t>((1000.0f * 1000.0f) / f);
	}
	~FPS() {}

	uint64_t  CalcDelta() {
		m_current_time = timeGetTime();					// 現在時刻取得
		m_delta_time = m_current_time - m_last_time;	// 前回実行時からに経過時間を取得
		m_last_time = m_current_time;
		return m_delta_time;
	}

	void Wait() {
		int64_t sleep_time = m_MicrosecondsPerFrame - m_delta_time;
		if (sleep_time > 0) {
			float tt = sleep_time / 1000.0f;

			std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<int>(tt)));
			//指定した相対時間だけ現スレッドをブロックする (function template)
		}
	}
private:
	uint64_t m_MicrosecondsPerFrame = 0;				// micro second
	uint64_t m_current_time = 0;
	uint64_t m_last_time = 0;
	uint64_t m_delta_time = 0;
};