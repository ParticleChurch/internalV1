// undefine stuff it this header has already been included
// there's probably a #pragma to disable the warning, but this was easy
#ifdef CONFIG_TAB
#undef CONFIG_TAB
#endif
#ifdef CONFIG_GROUP
#undef CONFIG_GROUP
#endif
#ifdef CONFIG_PROPERTY
#undef CONFIG_PROPERTY
#endif

#ifdef CONFIG_TAB_END
#undef CONFIG_TAB_END
#endif
#ifdef CONFIG_GROUP_END
#undef CONFIG_GROUP_END
#endif

#ifdef DECLARE_CONFIG
#undef DECLARE_CONFIG
#endif

// define if this is an implementation or a declaration
#ifdef CONFIG_IMPL
#define CONFIG_TAB(code_name, visible_name) namespace code_name { 
#define CONFIG_GROUP(code_name, visible_name) namespace code_name { 
#define CONFIG_PROPERTY(type, code_name, visible_name, ...) type code_name(std::string(_TAB_NAME) + "-" + _GROUP_NAME + "-" + visible_name, __VA_ARGS__);
#else
#define CONFIG_TAB(code_name, visible_name) namespace code_name { constexpr const char* _TAB_NAME = visible_name;
#define CONFIG_GROUP(code_name, visible_name) namespace code_name { constexpr const char* _GROUP_NAME = visible_name;
#define CONFIG_PROPERTY(type, code_name, visible_name, ...) extern type code_name;
#endif

#define CONFIG_TAB_END() }
#define CONFIG_GROUP_END() }

#define DECLARE_CONFIG \
	CONFIG_TAB(Offence, "Offence") \
		CONFIG_GROUP(Meta, "") \
			CONFIG_PROPERTY(BigHorizontalSelect, OffenceMode, "", { "Legit", "Rage" }) \
		CONFIG_GROUP_END() \
		CONFIG_GROUP(LegitAimbot, "Legit Aimbot") \
			CONFIG_PROPERTY(Boolean, Enable, "Enable", { &Meta::OffenceMode, 0 }) \
		CONFIG_GROUP_END() \
		CONFIG_GROUP(RageAimbot, "Rage Aimbot") \
			CONFIG_PROPERTY(Boolean, Enable, "Enable", { &Meta::OffenceMode, 1 }) \
		CONFIG_GROUP_END() \
	CONFIG_TAB_END()