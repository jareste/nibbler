#pragma once

// Forward declaration to avoid circular dependency
class RaylibGraphic;

class RaylibTitleHandler {
	private:
		RaylibGraphic& graphic;

	public:
		RaylibTitleHandler() = delete;
		RaylibTitleHandler(RaylibGraphic& parent);
		~RaylibTitleHandler() = default;
		RaylibTitleHandler (const RaylibTitleHandler &other) = delete;
		RaylibTitleHandler &operator=(const RaylibTitleHandler &other) = delete;
		
		void drawTitle();
};