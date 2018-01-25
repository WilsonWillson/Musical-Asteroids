#pragma once

class Interval
{
public:
	//void set_parent_clock(Clock *clock);

	float GetFrameStartTime()
	{
		
		return 0;
	}

	void set_seconds(float seconds)
	{
		interval_time = seconds;
		target_time = GetFrameStartTime() + seconds;
	}

	void set_frequency(float hz) { set_seconds(1.0f / hz); }

	bool check()
	{
		float current_time = GetFrameStartTime();
		return (current_time >= target_time);
	}

	bool check_and_decrement()
	{
		if (check()) {
			target_time += interval_time;
			return true;
		}
		else {
			return false;
		}
	}

	bool check_and_reset()
	{
		if (check()) {
			reset();
			return true;
		}
		else {
			return false;
		}
	}

	unsigned int decrement_all()
	{
		unsigned int count = 0;
		while (check_and_decrement()) {
			++count;
		}

		return count;
	}

	void reset()
	{
		target_time = GetFrameStartTime() + interval_time;
	}

public:
	float interval_time;
	float target_time = 40;

};