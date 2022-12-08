#pragma once

namespace live::tritone::vie {
	class UserInterfaceStyle {
	public:
		static enum class theme { THEME_LIGHT, THEME_DARK, THEME_DEFAULT};

		void set_style(struct nk_context* ctx, enum theme theme);
	};
}