#include <chrono>

class MiTimer
{
	public:
		MiTimer();
		float Mark();
		float Peek() const;
	private:
		std::chrono::steady_clock::time_point last;
};

MiTimer::MiTimer()
{
	last = std::chrono::steady_clock::now();
}

float MiTimer::Mark()
{
	const auto old = last;
	last = std::chrono::steady_clock::now();
	const std::chrono::duration<float> frameTime = last - old;
	return frameTime.count();
}

float MiTimer::Peek() const
{
	return std::chrono::duration<float> (std::chrono::steady_clock::now() - last ).count();
}

