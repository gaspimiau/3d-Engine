#pragma once

#include <queue>
#include <Windows.h>

class Mouse
{
	friend class Window;
	public:
		class Event
		{
			public:
				enum class Type
				{
					LPress,
					LRelease,
					RPress,
					RRelease,
					WheelUp,
					WheelDown,
					Move,
					Enter,
					Leave,
					Invalid
				};
			private:
				Type type;
				bool leftIsPressed;
				bool rigthIsPressed;
				int x;
				int y;
			public:
				Event() noexcept
					:
					type( Type::Invalid ),
					leftIsPressed( false ),
					rigthIsPressed( false ),
					x( 0 ),
					y( 0 )
				{}
				Event(Type type, const Mouse& parent) noexcept
					:
					type( type ),
					leftIsPressed( parent.leftIsPressed),
					rigthIsPressed( parent.rigthIsPressed),
					x( parent.x ),
					y( parent.y )
				{}
				//eventos
				bool IsValid() const noexcept
				{
					return type != Type::Invalid;
				}	
			/*	//desde aca lo implemente yo, nose si estara bien
				bool IsLPress() const noexcept
				{
					return type == Type::LPress;
				}
				bool IsLRelease() const noexcept
				{
					return type == Type::LRelease;
				}
				bool IsRPress() const noexcept
				{
					return type == Type::RPress;
				}
				bool IsRRelease() const noexcept
				{
					return type == Type::RRelease;
				}
				bool IsWheelUp() const noexcept
				{
					return type == Type::WheelUp;
				}
				bool IsWheelDown() const noexcept
				{
					return type == Type::WheelDown;
				}
				bool IsMove() const noexcept
				{
					return type == Type::Move;
				}*/
				//getters
				Type GetType() const noexcept
				{
					return type;	
				}
				std::pair<int,int> GetPos() const noexcept
				{
					return {x,y};
				}
				int GetPosX() const noexcept
				{
					return x;
				}
				int GetPosY() const noexcept
				{
					return y;
				}
				bool LeftIsPressed() const noexcept
				{
					return leftIsPressed;
				}
				bool RigthIsPressed() const noexcept
				{
					return rigthIsPressed;
				}
		};
	public:
		Mouse() = default;
		Mouse(const Mouse&) = delete;
		Mouse& operator=(const Mouse&) = delete;
		std::pair<int,int> GetPos() const noexcept;
		int GetPosX() const noexcept;
		int GetPosY() const noexcept;
		bool IsInWindow() const noexcept;
		bool LeftIsPressed() const noexcept;
		bool RigthIsPressed() const noexcept;
		Mouse::Event Read() noexcept;
		bool IsEmpty() const noexcept;
		void Flush() noexcept;
	private:
		void TrimBuffer() noexcept;
		void OnMouseMove(int x, int y) noexcept;
		void OnMouseLeave() noexcept;
		void OnMouseEnter() noexcept;
		void OnLeftPressed(int x, int y) noexcept;
		void OnLeftReleased(int x, int y) noexcept;
		void OnRigthPressed(int x, int y) noexcept;
		void OnRigthReleased(int x, int y) noexcept;
		void OnWheelUp(int x, int y) noexcept;
		void OnWheelDown(int x, int y) noexcept;
		void OnWheelDelta(int x, int y, int delta) noexcept;
	private:
		static constexpr unsigned int bufferSize = 16u;
		int x;
		int y;
		bool leftIsPressed = false;
		bool rigthIsPressed = false;
		bool isInWindow = false;
		int wheelDeltaCarry = 0;
		std::queue<Event> buffer;
};


